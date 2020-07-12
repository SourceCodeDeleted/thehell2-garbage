char* FontSpriteMid;
int* TextBoxCel;
int Speech_CurLineY;
char* Speech_FirstDrawingLine;
char Speech_IsPanelVisible;
// 004A7914
int Speech_ScrollSteps[11] = { 0, 2, 4, 6, 8, 0, -1, -2, -3, -4, 0	};
int Speech_ScrollCurStep;
int Speech_ScrollStep;
uint Speech_PrevTick;

//----- (0042E4F5) --------------------------------------------------------
void FreeSpeechPanelPtrs()
{
	FreeMemAndZeroPtr(FontSpriteMid);
	FreeMemAndZeroPtr(TextBoxCel);
}

//----- (0042E519) --------------------------------------------------------
void LoadSpeechPanelData()
{
	FontSpriteMid = (char*)LoadFile("Data\\MedTextS.CEL", NULL);
	TextBoxCel = (int*)LoadFile("Data\\TextBox.CEL", NULL);
	Speech_IsPanelVisible = 0;
}

//----- (0042E543) --------------------------------------------------------
void __fastcall StartSpeech( int speechIndex )
{
	Speech& speech = Speeches[ speechIndex ];
	if( speech.HasText ){
		IsQUESTPanelVisible = false;
		Speech_FirstDrawingLine = speech.TextPtr;
		Speech_IsPanelVisible = true;
		Speech_CurLineY = Screen_TopEnd + Dialog_302_Height + (GUI_Height/*ScreenHeight*/ - Dialog_302_Height - GUI_MainPanelHeight) / 2 + Speech_TopMargin; // 500 в оригинале 
		Speech_ScrollStep = Speech_ScrollSteps[ speech.stepIndex ];
		Speech_ScrollCurStep = Speech_ScrollStep;
		Speech_PrevTick = GetTickCount(); // это таймер не влияет на гемплей, не перехватываем
	}
	PlayGlobalSound( speech.SoundIndex );
}

//----- (0042E600) --------------------------------------------------------
void __fastcall DrawLetterInSpeech( int xPos, int yPos, void* fontSet, int letterNumber )
{
	int letterIndex = letterNumber - 1;
	int* fontData = (int*)fontSet;
	int* lettersOffsets = &fontData[1];
	int letterOffset = lettersOffsets[letterIndex];
	int nextLetterOffset = lettersOffsets[letterIndex + 1];
	int length = nextLetterOffset - letterOffset;
	char* pictureData = (char*)fontSet + letterOffset;
	int linePixelsLeft = Speech_LetterMaxWidth;

	uchar* curPixelPtr = WorkingSurface + YOffsetHashTable[yPos] + xPos;
	int dialogBottomY = Screen_TopEnd + Dialog_302_Height + (GUI_Height/*ScreenHeight*/ - Dialog_302_Height - GUI_MainPanelHeight) / 2;
	uchar* speechTop = WorkingSurface + YOffsetHashTable[ dialogBottomY - Dialog_302_Height + Speech_TopMargin ];
	uchar* speechBottom = WorkingSurface + YOffsetHashTable[ dialogBottomY - Speech_TopMargin ];

	for( int row = 0; row < length; ){
		char curCode = pictureData[row++];
		if( curCode >= 0 && ( curPixelPtr < speechTop || curPixelPtr > speechBottom ) ){ // высота запрещенная 
			row += curCode;
			curCode = -curCode;// сразу пропускаем все байты 
		}
		if( curCode < 0 ){ // Пропустить (-curCode) байт 
			curPixelPtr -= curCode;
			linePixelsLeft += curCode;
			if( linePixelsLeft <= 0 ){
				curPixelPtr -= YOffsetHashTable[1] + Speech_LetterMaxWidth; // подъем на строку выше 
				linePixelsLeft = Speech_LetterMaxWidth;
			}
		}else{ // рисуем curCode пикселей
			for( int j = 0; j < curCode; j++ ){
				*curPixelPtr++ = pictureData[row++];
				if( --linePixelsLeft <= 0 ){
					curPixelPtr -= YOffsetHashTable[1] + Speech_LetterMaxWidth; // подъем на строку выше 
					linePixelsLeft = Speech_LetterMaxWidth;
				}
			}
		}
	}
}

// Отрисовка плавного поднимающегося текста речи NPC 
//----- (0042E6A2) --------------------------------------------------------
void DrawSpeech()
{
	int leftX = (WorkingWidth - (ScreenWidth - GUI_Width) - Dialog_591_Width) / 2 + Speech_LeftMargin; // левая координата отрисовки речи 
	int bottomY = Screen_TopEnd + Dialog_302_Height + (GUI_Height/*ScreenHeight*/ - Dialog_302_Height - GUI_MainPanelHeight) / 2 + Speech_TopMargin; // нижняя координата отрисовки речи 
	char* curChar = Speech_FirstDrawingLine; // текущая рисуемая буква = первая буква верхней рисуемой строки 
	char* secondDrawingLine = Speech_FirstDrawingLine; // первая буква второй рисуемой строки 
	char curLine[ Speech_LineMaxLen ]; // буфер рисуемой локализованной по кодовой странице строки 
	DrawBigDialogBox(); // окно вокруг текста с рамкой и фоном 
	int charY = Speech_CurLineY; // верхний пиксель строки 
	for( char* nextChar = curChar; *nextChar != '|' && charY < bottomY; charY += Speech_LineHeight ){ // рисуем все видимые в окне строки текста до низа 
		int curLineWidth = 0; // ширина выводимой строки в пикселях 
		int lineLen = 0; // длина рисуемой строки 
		for( nextChar = curChar; *nextChar != '|' && *nextChar != '\n' && curLineWidth < Dialog_591_Width - Speech_LeftMargin * 2 && lineLen < Speech_LineMaxLen; nextChar++ ){ // локализуем и заполняем до максимальной ширины заполняемую строку 
			if( char localChar = Codepage[ *nextChar ] ){ // нулевые символы пропускаем 
				curLine[ lineLen++ ] = localChar; // заполняем строку и считаем её длину 
				curLineWidth += FontWidthMid[ FontIndexMid[ localChar ] ] + Speech_Spacing; // считаем ширину строки в пикселях 
			}
		}
		if( *nextChar != '|' && *nextChar != '\n' ){ // если текст не закончился и следущий символ не перевод строки 
			for( ; lineLen > 0 && curLine[ lineLen - 1 ] != ' '; lineLen-- ); // обрезаем справа до ближайшего пробела чтобы не было разрыва слова 
		}
		curLine[ lineLen ] = 0; // завершаем выводимую строку нулём 
		int charX = leftX; // левый пиксель строки 
		for( char* localChar = curLine; *localChar != 0; ++localChar ){ // отрисовка строки 
			uchar charImageNum = FontIndexMid[ *localChar ]; // номер изображения буквы 
			if( charImageNum ){ // если не пробельный символ 
				DrawLetterInSpeech( charX, charY, FontSpriteMid, charImageNum ); // отрисовка буквы 
			}
			charX += FontWidthMid[ charImageNum ] + Speech_Spacing; // двигаем пиксельную каретку вправо (включая пробельные символы ) 
			if( *(++curChar) == '\n' ){ // двигаем каретку исходного текста 
				++curChar; // переводы строк были пропущены при локализации 
			}
		}
		if( secondDrawingLine == Speech_FirstDrawingLine ){ // если вывели первую строку 
			secondDrawingLine = curChar; // запоминаем начало второй 
		}
	}
	if( IsMainDraw ){
		uint curTick = GetTickCount(); // тик текущего кадра  // это таймер не влияет на гемплей, не перехватываем
		do{ // плавная прокрутка 
			if( Speech_ScrollStep > 0 ){ // положительные значения для попиксельного скроллинга внтури шага 
				if( Speech_ScrollCurStep-- > 1 ){ // пока не сдвинулись на все пиксели шага 
					Speech_CurLineY--; // попиксельный скроллинг 
				}else{
					Speech_ScrollCurStep = Speech_ScrollStep; // начинаем попиксельный шаг заново 
				}
			}else{ // отрицательные значения для скроллинга на весь шаг в несколько пикселей за кадр 
				Speech_CurLineY -= -Speech_ScrollStep + 1; // пошаговый скроллинг 
			}
		}while( Speech_CurLineY > bottomY - Dialog_302_Height && curTick - (Speech_PrevTick += Speech_LineScrollTime) < 0x7FFFFFFF ); // пока не верхняя граница текста 
		if( curTick - Speech_PrevTick < 0x7FFFFFFF ){ // Speech_LineScrollTime (50) миллисекунд на прокрутку одной строку вверх ( с проверкой на случай переполнения ) 
			Speech_CurLineY += Speech_LineHeight; // переходим на отслеживание верха следущей строки 
			Speech_FirstDrawingLine = secondDrawingLine; // отбрасываем верхнюю строку уползшую за верхний край 
			if( *Speech_FirstDrawingLine == '|' ){ // если текст закончился 
				Speech_IsPanelVisible = 0; // закрываем речевое окно 
			}
		}
	}
}

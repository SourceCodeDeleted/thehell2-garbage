#pragma once

#include <conio.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;

#define countof(a) (sizeof(a) / sizeof(*a))

int IntHexStruct [1000];		// Глобальный массив диапазонов значений.
int IntHexStructSize = 0;		// Хранилище текущего размера массива
int IntHexStructFormat [1000];	// Глобальный массив формата значений.
int IntHexStructFormatSize = 0;	// Хранилище текущего размера массива
char Buffer[1024];				// Основной буфер для передачи строковых массивов между сабами
char Buffer2[256];				// Дополнительный буфер для того же
int StructSize;					// Значение означающее максимальный байт структуры
int StructCount;				// Значение означающее количество структур
int FirstStructStart;			// Значение означающее начало первой структуры
int CurrentStructStart;			// Значение означающее начало текущей структуры
char StartString[256];			// Строка показывающая чем начинать вывод отдельной структуры
char FinishString[256];			// Строка показывающая чем кончать вывод отдельной структуры
int PtrOffset1 = 0;
int PtrOffset2 = 0;

int FileSize = 0;
char* TH1 = 0;	// загруженный в память .exe


// TH.exe segmentation (got from disasm) 
// в общем случае смещения сегментов .exe файла можно загрузить автоматически из самого .exe (гуглить PE Loader)     
// или можно загрузить екзешник в новый процесс силами операционной системы и читать данные из памяти этого процесса 
struct { int MemoryStart, FileStart; } ExeSegments[] = 
{
	{ 0x401000, 0x000400 },	// .text  00401000 00489000
	{ 0x489000, 0x088000 },	// .idata 00489000 004893C4
	{ 0x4893C4, 0x0883C4 },	// .rdata 004893C4 00494000
	{ 0x494000, 0x092A00 },	// .data  00494000 00702000
	{ 0x705000, 0x0D1C00 },	// .mod   00705000 00805000
	{ 0x805000, 0x1D1C00 },	// .rda2  00805000 00905000
	{ 0x905000, 0x2D1C00 },	// .dat2  00905000 00B05000
};

#define MAX_FILE_SIZE 1024*1024*1024

// преобразование адреса из файлового смещение в адрес в памяти 
int FileToMem( int fileOffset )
{
	for( int i = countof(ExeSegments) - 1; i >= 0; i-- ){
		if( fileOffset >= ExeSegments[i].FileStart ){
			return fileOffset + (ExeSegments[i].MemoryStart - ExeSegments[i].FileStart);
		}
	}
	return 0;
}

// преобразование адреса из адреса в памяти в файловое смещение 
int MemToFile( int memoryOffset )
{
	for( int i = countof(ExeSegments) - 1; i >= 0; i-- ){
		if( memoryOffset >= ExeSegments[i].MemoryStart ){
			return memoryOffset - (ExeSegments[i].MemoryStart - ExeSegments[i].FileStart);
		}
	}
	return 0;
}

// Альтернативный саб для теста чтения значения напрямую из бинарника.
int LoadExeFile(char* sourceName)
{
	FILE* sourceFile = fopen(sourceName,"rb");
	if( !sourceFile ){
		printf("\nError 1 - Can not read analyzed .exe file: %s\n", sourceName);
		return 1;
	}
	fseek(sourceFile, 0, SEEK_END);
	FileSize = ftell(sourceFile);
	fseek(sourceFile, 0, SEEK_SET);
	if( FileSize > MAX_FILE_SIZE ){
		printf("\nError 2 - Too big analyzed .exe file: %s\n", sourceName);
		return 2;
	}
	TH1 = new char[FileSize];
	fread(TH1, 1, FileSize, sourceFile);
 	fclose(sourceFile);
	return 1;
}

// Сабы отвечающие за выдачу информации в оутпут файл
char CharToHex2 (char inputChar)
{
	switch (inputChar){
		case 0:		return '0';
		case 1:		return '1';
		case 2:		return '2';
		case 3:		return '3';
		case 4:		return '4';
		case 5:		return '5';
		case 6:		return '6';
		case 7:		return '7';
		case 8:		return '8';
		case 9:		return '9';
		case 10:	return 'a';
		case 11:	return 'b';
		case 12:	return 'c';
		case 13:	return 'd';
		case 14:	return 'e';
		case 15:	return 'f';
		default:	return '0'; // При ошибке пока тоже 0
	}
}

// Составляем подобный саб
// Только вместо вывода в шестнадцатиричном формате берём инт значение полученное от DecodeNumberInDec 
// И пробуем получить стрововое значение от саба взятия строки. Если не получилось, пробуем другое смещение.
// 
void InsertChar (char charToInsert, int posToIncert){
	int len = strlen(Buffer2);
	for (int i = len + 1; i > posToIncert; i--){
		Buffer2 [i] = Buffer2 [i - 1];
	}
	Buffer2 [posToIncert] = charToInsert;
}

void FormatString ()
{
	// Добавление кавычек
	InsertChar ('"', 0);
	int len = strlen(Buffer2);
	InsertChar ('"', len);
	Buffer2 [len + 1] = 0;
	// Замена '\' на '\\'
	int slashPos[32], numOfSlash = 0;
	len = strlen(Buffer2);
	for (int i = 0; i < len; i++){
		if (Buffer2[i] == 92){
			slashPos[numOfSlash++] = i;
		}
	}
	for (int i = numOfSlash - 1; i >= 0; i--){
		InsertChar(92, slashPos[i]);
	}
	//Buffer2 [i] = startHexString [i];
	//Buffer2 [i];
}

bool GetStringByPtr(int ptr)
{
	bool isTrash = false, isEndOfString = false;
	for( int i = 0; !isTrash && !isEndOfString; i++ ){
		Buffer2 [i] = TH1[i + ptr];
		
		if( Buffer2[i] == 0 ){
			if( i == 0 ){
				isTrash = true;
			}
			isEndOfString = true;
		}

		if( i == 255 ){
			i = i;
		}
		// Тут ещё методы определения мусора к примуру повторяющиеся символы и недопустимые символы
	}
	if (!isTrash) {
		FormatString();
	}
	// Формат плохой, поэтому нужно сделать по образцу "Monsters\\Demskel\\Demskl%c.CL2" добавить кавычки, заменить одинарные "\" на "\\" и "Ф" на "%с"
	// Затем нужно будет создать файл в котором через пробел в строке будет строка и переменная которая имеет значением эту строку.
	// И перед выводом строки проверять на совпадение со всеми переменными. При совпадении заменять на идентификатор переменной.
	return isTrash;
}

void DecodeNumberInHex (int lByte, int hByte)
{
	// Тут у нас будет перевод байтов от l до h включительно в шестнадцатиричный формат 
	// добавляем знак "0x"
	// для перевода делаем в цикле:
	// Начиная со старшего байта (не включая его) берём его uchar и берём его частное от 16 и мод от 16
	char startHexString [] = "0x";
	for( int i = 0; i < 2; i++ ){
		Buffer2 [i] = startHexString [i];
	}
	int buffer2Pos = strlen(startHexString);
	for( int i = hByte - 1; i >= lByte; i-- ){
		char currentChar = TH1[i + CurrentStructStart];
		Buffer2 [buffer2Pos++] = CharToHex2(currentChar / 16);
		Buffer2 [buffer2Pos++] = CharToHex2(currentChar % 16);
	}
	Buffer2 [buffer2Pos] = 0;
}

int DecodeNumberInDec (int lByte, int hByte)
{
	// Тут у нас будет перевод байтов от l до h включительно в целочисленный формат 
	// для перевода делаем в цикле:
	// Начиная со старшего байта (не включая его) берём его uchar и прибавляем к значению
	// Умножаем значение на 16 и прибавляем следующий байт
	int resultValue = 0;
	for( int i = hByte - 1; i >= lByte; i-- ){
		resultValue <<= 8;
		resultValue += TH1[i + CurrentStructStart];
	}
	return resultValue;
}

void DecodeNumberInStringByPtr(int lByte, int hByte)
{
	bool isTrash = false;
	int currentPtr = DecodeNumberInDec (lByte, hByte);
	bool highPtr = currentPtr - PtrOffset1 >= FileSize - 20;
	bool lowPtr = currentPtr - PtrOffset2 < 0;
	if( currentPtr ){
		if( lowPtr ){
			isTrash = GetStringByPtr (currentPtr - PtrOffset1);
		}else{
			isTrash = GetStringByPtr (currentPtr - PtrOffset2);
		}
		if( isTrash ){
			if( !lowPtr ){
				isTrash = GetStringByPtr (currentPtr - PtrOffset1);
			}
		}
	}else{
		_itoa(currentPtr, Buffer2, 10);
	}
}

void GetCFormatedString ()
{
	char middleString[] = ", ";
	int bufferPosition = 0;
	// Нам нужно:
	// Первое - запись стартовой строки. Для монстров она будет иметь вид: "BaseMonster(" позже надо будет вывести это или в структ файл или в отдельный файл
	for( uint stringPos = 0; stringPos < strlen(StartString); stringPos++ ){
		Buffer[bufferPosition++] = StartString [stringPos];
	}
	// Второе - запись расшифрованных чисел в десячичной системе до тех пор пока они не закончатся. ", " между цифрами
	for( int i = 0; IntHexStruct[i] != StructSize; i++ ){
		if( i ){ // Добавление запятой
			for( uint stringPos = 0; stringPos < strlen(middleString); stringPos++ ){
				Buffer[bufferPosition++] = middleString[stringPos];
			}
		}
		// Добавление расшифрованного числа. Зависит от формата.
		if( IntHexStructFormat[i] == 10 ){
			int decodeNumber = DecodeNumberInDec (IntHexStruct[i], IntHexStruct[i + 1]);
			_itoa(decodeNumber, &Buffer[bufferPosition], 10);// ну как то так
			bufferPosition = strlen(Buffer);
		}else if( IntHexStructFormat[i] == 16 ){
			// Запись в шестнадцатиричном формате. Во первых требуется добавить "0x", а во вторых строку дешифрованную в шестнадцатиричный формат
			DecodeNumberInHex(IntHexStruct[i], IntHexStruct[i + 1]);
			for( int buffer2Pos = 0; buffer2Pos < 256 && Buffer2[buffer2Pos]; buffer2Pos++ ){
				Buffer[bufferPosition++] = Buffer2[buffer2Pos];
			}
		}else if( IntHexStructFormat[i] == 256 ){
			// Запись в шестнадцатиричном формате. Во первых требуется добавить "0x", а во вторых строку дешифрованную в шестнадцатиричный формат
			DecodeNumberInStringByPtr(IntHexStruct[i], IntHexStruct[i + 1]);
			for( int buffer2Pos = 0; buffer2Pos < 256 && Buffer2[buffer2Pos]; buffer2Pos++ ){
				Buffer[bufferPosition++] = Buffer2[buffer2Pos];
			}
		}
	}
	// Последнее - запись окончания вида "),/n"
	for( uint stringPos = 0; stringPos <= strlen(FinishString); stringPos++ ){
		if( stringPos == strlen(FinishString) ){
			Buffer[bufferPosition++] = 0;
		}else{
			Buffer[bufferPosition++] = FinishString[stringPos];
		}
	}
}
//

// Сабы отвечающие за чтение структуры из файла 
void HandleHexStructureLine (int lineIndex)
{
	istringstream inp(Buffer);
	int hexStructValue = 0;
	switch( lineIndex ){
		case 0:
			inp >> FirstStructStart;
			inp >> StructSize;
			inp >> StructCount;
			inp >> PtrOffset1;
			inp >> PtrOffset2;
			break;
		case 1:
			for( int i = 0; i < 256; i++ ){
				StartString[i] = Buffer[i];
			}
			break;
		case 2:
			for( int i = 0; i < 256; i++ ){
				FinishString[i] = Buffer[i];
			}
			break;
		case 3:
			for( IntHexStructSize = 0; hexStructValue != StructSize; IntHexStructSize++ ){
				inp >> hexStructValue;
				IntHexStruct[IntHexStructSize] = hexStructValue;
			}
			break;
		case 4:
			for( IntHexStructFormatSize = 0; IntHexStruct[IntHexStructFormatSize] != StructSize; IntHexStructFormatSize++ ){
				inp >> hexStructValue;
				IntHexStructFormat[IntHexStructFormatSize] = hexStructValue;
			}
			break;
	}
}

void ReadHexStructureFile ()
{
	// Прочитали структуру из файла
	ifstream* structureFileStream = new ifstream("hexStructure.txt");
	if( !structureFileStream->good() ){
		cout << "Error with open hexStructure.txt" << endl;
		return;
	}
	for( int lineIndex = 0; lineIndex < 5; lineIndex++ ){
		structureFileStream->getline(Buffer, 256);
		HandleHexStructureLine(lineIndex);
	}
	structureFileStream->close();
}

//
int main()
{
	if( int error = LoadExeFile("TH.exe") ){
		return error;
	}
	ReadHexStructureFile ();

	// Теперь делаем следующее:
	// 1. В цикле проходим количество раз по количеству монстров.
	// 2. В начале каждого цикла выставляем CurrentStructStart который изначально равняется числу считанному из файла а затем получающееся прибавлением размера структуры
	// 3. Выдаём в оутпут получившуюся в результате строку
	CurrentStructStart = FirstStructStart;

	/*FILE *outf = fopen("hexOutput.txt", "w");*/
	ofstream outf("hexOutput.txt");

	for( int currentStruct = 0; currentStruct < StructCount; currentStruct++ ){
		GetCFormatedString();
		outf << Buffer << "\n";

		// fprintf(outf, Buffer);
		// fprintf(outf, "\n");*/
		CurrentStructStart += StructSize;
	}
	
	outf.close();
	/*fclose(outf);*/
	return 0;
}

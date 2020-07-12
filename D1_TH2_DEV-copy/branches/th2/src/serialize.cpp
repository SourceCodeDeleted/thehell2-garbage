
#pragma optimize("gs",on)

#ifdef TH1

int CurOutMode = OM_CPP;
bool IgnoreEndMarker = false;
char* OutModes[4] = { "cpp", "txt", "xls", "xml" };
int MaxDifRecursion = 5;
bool OutElementAlwaysOnNewLine = false;


bool IsBadPtr( const void* p )
{
	MEMORY_BASIC_INFORMATION mbi = { 0 };
	if( VirtualQuery( p, &mbi, sizeof( mbi ) ) ){
		DWORD mask = (PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY);
		bool b = !(mbi.Protect & mask);
		// check the page is not a guard page
		if( mbi.Protect & (PAGE_GUARD | PAGE_NOACCESS) ){
			b = true;
		}
		return b;
	}
	return true;
}

// специализация вывода строки
SPEC_FUNC( char*, is_pointer, void ) out_< char* >( char* *strPtr, int, int)
{
	char* str = *strPtr;
	if( str == 0 ){
		cout << '0';
	}else if( IsBadPtr( str ) ){ // IsBadStringPtrA( str, 1024 ){
		cout << "0x" << hex << (uint)str << dec;
	}else{
		cout << '"';
		while( char c = *str++ ) switch( c ){
			case '\\': cout << "\\\\"; break;
			case '"' : cout << "\\\""; break;
			case '\t': cout << "\\t" ; break;
			case '\n': cout << "\\n" ; break;
			default  : cout << c;
		}
		cout << '"';
	}
}

// специализация вывода беззнакового байта 0..255
SPEC_FUNC( uchar, is_fundamental, void ) out_< uchar >(uchar *ucharPtr, int, int) 
{
	cout << (uint)*ucharPtr;
}

// специализация вывода знакового байта -128..127
SPEC_FUNC( char, is_fundamental, void ) out_< char >(char *charPtr, int, int) 
{
	cout 
		<< (int)*charPtr // раскомментировать если нужно значение
		//<< "'" << *charPtr << "'" // раскоментировать если нужен символ
		;
}

// генерация таблицы метаданных функций (адрес th1, адрес th2, имя, вывод, сравнение)
struct Sa64 { const SubInfo v[ FCnt ]; };
template< int i, typename = make_index_sequence<FCnt> > struct SA64; template< int i, size_t... l > struct SA64<i, index_sequence<l...> > 
{ const Sa64 v = { Sub< i * FCnt + l >::v ... }; };
template< typename = make_index_sequence<FCnt> > struct SA4096; template< size_t...l > struct SA4096<index_sequence<l...> > 
{ const Sa64 v[ FCnt ] = { SA64<l>{}.v ... }; };

// генерация таблицы метаданных глобальных переменных (адрес th1, адрес th2, имя)
struct Va64 { const VarInfo v[ VCnt ]; };
template< int i, typename = make_index_sequence<VCnt> > struct VA64; template< int i, size_t... l > struct VA64<i, index_sequence<l...> > 
{ const Va64 v = { Var< i * VCnt + l >::v() ... }; };
template< typename = make_index_sequence<VCnt> > struct VA4096; template< size_t...l > struct VA4096<index_sequence<l...> > 
{ const Va64 v[ VCnt ] = { VA64<l>{}.v ... }; };

// получение ссылки на таблицу функций
FuncTable& func_table()
{
	static const SA4096<> sa4096;
	return *(FuncTable*)&sa4096;
}

// получение ссылки на таблицу переменных
VarTable& var_table()
{
	static const VA4096<> va4096;
	return *(VarTable*) &va4096;
}

// получение указателя на данные функции в таблице по имени
VarInfo* var_info( char* name )
{
	static VarTable& var = var_table();
	for( int i = 0; var[ i ].adr != 0 && i < VCnt*VCnt; ++i ){
		if( ! strcmp(var[ i ].name, name) ){
			return &var[ i ];
		}
	}
	return 0;
}

// получение указателя на данные переменной в таблице по имени
SubInfo* func_info( char* name )
{
	static FuncTable& func = func_table();
	for( int i = 0; func[ i ].adr != 0 && i < FCnt*FCnt; ++i ){
		if( !strcmp( func[ i ].name, name ) ){
			return &func[ i ];
		}
	}
	return 0;
}

// получение индекса функции по ее оригинальному адресу
int func_index( void* adr, int th /*=1*/ )
{
	static FuncTable& func = func_table();
	if( th == 1 ){
		for( int i = 0; func[ i ].adr != 0 && i < FCnt*FCnt; ++i ){
			if( func[ i ].adr == (size_t) adr ){
				return i;
			}
		}
	}else{
		for( int i = 0; func[ i ].adr != 0 && i < FCnt*FCnt; ++i ){
			if( func[ i ].adrNew == adr ){
				return i;
			}
		}
	}
	return -1;
}

// получение индекса переменной по ее оригинальному адресу
int var_index( void* adr, int th /*=1*/ )
{
	static VarTable& var = var_table();
	if( th == 1 ){
		for( int i = 0; var[ i ].adr != 0 && i < VCnt*VCnt; ++i ){
			if( var[ i ].adr == (size_t) adr ){
				return i;
			}
		}
	}else{
		for( int i = 0; var[ i ].adr != 0 && i < VCnt*VCnt; ++i ){
			if( var[ i ].adrNew == adr ){
				return i;
			}
		}
	}
	return -1;
}

// получение имени функции th1 либо th2 по адресу
char* func_name( size_t adr, int th /*= 1*/ )
{
	static FuncTable& func = func_table();
	if( th == 1 ){
		for( int i = 0; func[ i ].adr != 0 && i < FCnt*FCnt; ++i ){
			if( func[ i ].adr == adr ){
				return func[ i ].name;
			}
		}
	}else{
		for( int i = 0; func[ i ].adr != 0 && i < FCnt*FCnt; ++i ){
			if( (size_t)func[ i ].adrNew == adr ){
				return func[ i ].name;
			}
		}
	}
	return nullptr;
}

// получение имени глобальной переменной th1 либо th2 по адресу
char* var_name( size_t adr, int th /*= 1*/ )
{
	static VarTable& var = var_table();
	if( th == 1 ){
		for( int i = 0; var[ i ].adr != 0 && i < VCnt*VCnt; ++i ){
			if( var[ i ].adr == adr ){
				return var[ i ].name;
			}
		}
	}else{
		for( int i = 0; var[ i ].adr != 0 && i < VCnt*VCnt; ++i ){
			if( (size_t) var[ i ].adrNew == adr ){
				return var[ i ].name;
			}
		}
	}
	return nullptr;
}

// специализация сравнения строк
SPEC_FUNC( char*, is_pointer, bool ) dif_< char* >( char* *strPtr1, char* *strPtr2 )
{
	char* str1 = *strPtr1;
	char* str2 = *strPtr2;
	if( str1 == 0 || str2 == 0 || IsBadPtr( str1 ) || IsBadPtr( str2 ) ){
		return str1 != str2; // return false;
	}else{
		return strcmp(str1, str2) != 0;
	}
}

// сравнение HINSTANCE, для того чтобы не показывало разницу
// когда оба WinMain HInstance валидные, TH 1 Base == 0x00400000 и TH 2 Base == 0x01400000
//SPEC_FUNC( HINSTANCE, is_pointer, bool ) dif_< HINSTANCE >( HINSTANCE * h1, HINSTANCE * h2)
//{
//	return (*h1 != 0 && *h2 != 0 && *h1 != *h2 )
//		?
//	(uint)*h1 != (uint)*h2 - ((uint)0x0140'0000/*TH2Base*/ - 0x0040'0000)
//		:
//	*h1 != *h2;
//}

// закомменченные функции не убирать, он помеченны как использованные при сверке и пока больше не используются
SPEC_OTHER_FUNC( void, is_function, bool ) dif_val<void>( void* var1, void* var2 ){ return var1 ? var2 == 0 : var2 != 0 /* var1 != var2 */ ; }
//SPEC_OTHER_FUNC( HWND__, is_function, bool ) dif_val<HWND__>( HWND__* var1, HWND__* var2 ){ return var1 != var2; }
SPEC_OTHER_FUNC( IDirectSoundBuffer, is_function, bool ) dif_val<IDirectSoundBuffer>( IDirectSoundBuffer* var1, IDirectSoundBuffer* var2 ){ return var1 != var2; }
//SPEC_OTHER_FUNC( IDirectDrawPalette, is_function, bool ) dif_val<IDirectDrawPalette>( IDirectDrawPalette* var1, IDirectDrawPalette* var2 ){ return var1 != var2; }
// тут как-то надо добавить сверку памяти самой поверхности (не факт что получится, ибо есть первичная в TH 1)
//SPEC_OTHER_FUNC( IDirectDrawSurface, is_function, bool ) dif_val<IDirectDrawSurface>( IDirectDrawSurface* var1, IDirectDrawSurface* var2 ){ return var1 != var2; }
// Возможно можно добавить получение данных по всем этим интерфейсам и сравнивать их ?
//SPEC_OTHER_FUNC( IDirectDraw, is_function, bool ) dif_val<IDirectDraw>( IDirectDraw* var1, IDirectDraw* var2 ){ return var1 != var2; }
SPEC_OTHER_FUNC( IDirectSound, is_function, bool ) dif_val<IDirectSound>( IDirectSound* var1, IDirectSound* var2 ){ return var1 != var2; }

// закомменченные функции не убирать, он помеченны как использованные при сверке и пока больше не используются
SPEC_OTHER_FUNC( void, is_function, void ) out_val<void>( void* var, int inner, int th ){ cout << "0x"; if( inner >= -100 ) cout << var; }
//SPEC_OTHER_FUNC( HWND__, is_function, void ) out_val<HWND__>( HWND__* var, int inner, int th ){ cout << "0x"; if( inner >= -100 ) cout << var; }
//SPEC_OTHER_FUNC( HINSTANCE__, is_function, void ) out_val<HINSTANCE__>( HINSTANCE__* var, int inner, int th ){ cout << "0x"; if( inner >= -100 ) cout << var; }
SPEC_OTHER_FUNC( IDirectSoundBuffer, is_function, void ) out_val<IDirectSoundBuffer>( IDirectSoundBuffer* var, int inner, int th ){ cout << "0x"; if( inner >= -100 ) cout << var; }
//SPEC_OTHER_FUNC( IDirectDrawPalette, is_function, void ) out_val<IDirectDrawPalette>( IDirectDrawPalette* var, int inner, int th ){ cout << "0x" << var; }
//SPEC_OTHER_FUNC( IDirectDrawSurface, is_function, void ) out_val<IDirectDrawSurface>( IDirectDrawSurface* var, int inner, int th ){ cout << "0x" << var; }
//SPEC_OTHER_FUNC( IDirectDraw, is_function, void ) out_val<IDirectDraw>( IDirectDraw* var, int inner, int th ){ cout << "0x" << var; }
SPEC_OTHER_FUNC( IDirectSound, is_function, void ) out_val<IDirectSound>( IDirectSound* var, int inner, int th ){ cout << "0x"; if( inner >= -100 ) cout << var; }
SPEC_OTHER_FUNC( IUnknown, is_function/*is_base_of*/, void ) out_val<IUnknown>( IUnknown* var, int inner, int th ){ cout << "0x"; if( inner >= -100 ) cout << var; }
SPEC_OTHER_FUNC( GUID, is_function, void ) out_val<GUID>( GUID* var, int inner, int th ){ cout << "0x"; if( inner >= -100 ) cout << var; }

//template<> func_out GetRet<void>(){}

#pragma optimize("",on)

// Выгрузка имен и сигнатур из исходников в ida (пока только имена и адреса)
// Тут же мини-фреймворк системы тестирования - функция вида dllexport которая
// выполняет тесты ( например, проверяет размеры переменных на непересечение )
// и пишет результат в лог-файл. В post-build event она вызывается через rundll32
// Этот подход похож на то, как работает MStest для C++
// ----- (th2) -------------------------------------------------------
extern "C" __declspec(dllexport) void CALLBACK OutIdc( HWND hwnd, HINSTANCE hinst, LPWSTR lpszCmdLine, int nCmdShow )
{
	if( ! lpszCmdLine && ! *lpszCmdLine ) return;
	wchar_t idcPath[ 260 ];
	wcscpy_s( idcPath, lpszCmdLine );
	wcscat_s( idcPath, L"lib\\TH.idc" );
	if( FILE* idc = _wfopen( idcPath, L"w" ) ){
		fprintf( idc, "#define UNLOADED_FILE 1\n#include <idc.idc>\n\nstatic main( void )\n{\n\n" );
		// check function sizes > 5 for non cross with next func on validate point set
		size_t prevAdr = 0;
		size_t prevSize = 0;
		char* prevName = "";
		FuncTable& func = func_table();
		for( int i = 0; func[ i ].adr && i < FCnt*FCnt; ++i ){
			if( func[ i ].adr != 0 ){
				fprintf( idc, "MakeName( 0x00%X, \"%s\" );", func[ i ].adr, func[ i ].name );
				if( func[ i ].adr - prevAdr < 5 ){
					fprintf( idc, " // %s size < 5 bytes", prevName );
				}
				prevName = func[ i ].name;
				prevAdr = func[ i ].adr;
				fprintf( idc, "\n" );
			}
		}
		fprintf(idc,"\n");
		VarTable& var = var_table();
		// validate all th1 sizeof and countof for no crossing
		prevAdr = 0;
		prevSize = 0;
		prevName = "";
		for( int i = 0; var[ i ].adr && i < VCnt*VCnt; ++i ){
			if( var[ i ].adr != 0 ){
				fprintf( idc, "MakeName( 0x00%X, \"%s\" );", var[ i ].adr, var[ i ].name );
				if( prevAdr + prevSize > var[ i ].adr ){
					fprintf( idc, " // Error: wrong size of %s", prevName );
				}
				prevName = var[ i ].name;
				prevAdr = var[ i ].adr;
				prevSize = var[ i ].size;
				fprintf( idc, "\n" );
			}
		}
		fprintf( idc, "\n}\n" );
		fclose( idc );
	}
}

SPEC_FUNC(FILE*, is_pointer, void) out_< FILE* >(FILE* *, int, int ){} // заглушка FILE*
SPEC_FUNC(HINSTANCE__*, is_pointer, void) out_< HINSTANCE__* >(HINSTANCE__* *, int, int ){} // заглушка HINSTANCE*
SPEC_FUNC(HWND__*, is_pointer, void) out_< HWND__* >(HWND__* *, int, int ){} // заглушка HWND__*
//SPEC_FUNC(MMRESULT*, is_pointer, void) out_< MMRESULT* >(MMRESULT* *, int, int ){} // заглушка MMRESULT*
//PALETTEENTRY
//guid

#endif // TH1

// разделение строки на массив строк (в том числе для парсина списка имен элементов enum)
char** enum_split( char* names, size_t count /*=2048*/ )
{
	char** name = new char*[ count + 1 ];
	bool name_begin = true;
	size_t i = 0;
	while( *names ){
		switch( *names ){
		case ',': name_begin = true; // идем ниже, break не нужен
		case ' ': *names = 0; break; // табы и переводы строк из списка убираются компилятором (проверено msvc)
		default: if( name_begin ){
			name[ i++ ] = names; name_begin = false;
		}
		}
		++names;
		if( i >= count ) break;
	}
	name[ i ] = 0;
	return name;
}

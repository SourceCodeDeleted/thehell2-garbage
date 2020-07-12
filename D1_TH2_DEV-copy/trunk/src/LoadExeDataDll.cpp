#include "stdafx.h"
// dll встраиваемая в дочерний процесс (.exe) для сериализации бинарных данных любых типов 
#include "LoadExe.h"

extern "C" __declspec(dllexport) void __stdcall OutArrayDll(char*);

using namespace std;

struct ExeText {
	char* varName;
	char* address;
	bool isTranslate;
	int amount;
};

enum DATA_FORMAT {
	DF_VAR,
	DF_ADDR,
	DF_TRANS,
	DF_AMOUNT,
	DF_SKIP
};

// split string in-place to substring by delimiter 
char* Delimit( char* &str, char delimiter )
{
	char* delimited = str;
	for( ; *str; str++ ){
		if( *str == delimiter ){
			*str++ = 0; 
			break;
		}
	}
	return delimited;
}

// вывод массива данных пользовательского типа (структуры, класса) 
// char* param - "формат_вывода имя_структуры адрес_начала_массива_в_виде_0x00NNNNNN максимальное_количество_элементов(по умолчанию 4096)"

// функция не дописана, умеет выводить только по одному массиву в cout
// можно на ее базе сделать выгрузку всех переменных в .cpp и всех сигнатур в .idc
__declspec(dllexport) void  __stdcall OutArrayDll(char* param)
{
	//MessageBoxA(NULL, "Hello from another process!\n", "Hello", MB_OK);
	//FILE* uniqs = fopen("C:\\-\\uniqs.cpp","w");
	//ofstream outf("hexOutput.txt");
	//cout << "Hello from another process!\n" << endl; return;

	char* mode = Delimit( param, ' ' );
	if( ! SetKeyByEnum( CurOutMode, mode, OutModes ) ){
		cout << "Out mode: " << mode << " not exists\n" << endl;
		return;
	}
	char* typeOrFileName = Delimit( param, ' ' );
	if( *param == 'f' ){  // out all arrays from data file 
		
		// read tab delimited table 
		// read whole text file in data buffer 
		FILE* dataFile = fopen( typeOrFileName, "rb" );
		fseek( dataFile, 0, SEEK_END ); //                                          
		long fSize = ftell( dataFile ); // get file size (work only in binary mode) 
		fseek( dataFile, 0, SEEK_SET ); //                                          
		char* data = new char[ fSize + 1 ];
		fread( (void*) data, fSize, 1, dataFile );
		data[ fSize ] = 0;
		fclose( dataFile );
		// translate all tabs to null-terminator for strings getting 
		char* dataEnd = &data[ fSize ];
		char* nextString = data;
		int attrib = DF_VAR;
		for( char* d = data; d < dataEnd; d++ ){
			if( *d == '\t' ){ // tab to null-terminated string 
				*d = 0;
				switch( attrib ){
				case DF_VAR: {

					attrib++; break; }
				case DF_ADDR: {
					//strtol( );
					attrib++; break; }
				case DF_TRANS: {

					attrib++; break; }
				case DF_AMOUNT: {

					attrib++; break; }
				}
			}else if( *d == '\r' || *d == '\n' ){ // next row of table 
				*d = 0;
				while( *( d + 1 ) == '\n' || *( d + 1 ) == '\r' ) d++;
				nextString = d + 1;
				attrib = DF_VAR;
			}
		}
		if( attrib == DF_AMOUNT ){ // last cell 

		}
	}else{ // out one array 
		istrstream in( param );
		uint address = 0;
		int  count   = 0;
		in >> hex >> address >> dec >> count >> IgnoreEndMarker;
		if( count <= 0 ){
			count = MAX_ARRAY_COUNT; 
		}
		// cout << param << endl;
		// cout << typeOrFileName << " 0x" << hex << address << ' ' << dec << count << endl; 
	
		OutArrayByName( typeOrFileName, (void*)address, count );

		// cout << flush;
		// _getch();
	}
}

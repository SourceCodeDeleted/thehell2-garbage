#include "stdafx.h"
#include "LoadExe.h"

// загрузка данных из .exe -----------------------------------------------------------
int LoadDataFromExe( char* exeName, char* param )
{
	char* dllName = "LoadExeDataDll.dll";
	char* dll = CheckDllName( dllName );
	if( ! dll ){
		cout << dllName << " not found" << endl;
		return ER_dll_not_found;
	}
	SetConsolePipe();
	HANDLE exe = LoadExe(exeName);
	if( ! InjectDllToExe( dll, exe ) ){
		cout << "cannot inject " << dllName << endl;
		return ER_cannot_inject_dll;
	}
	HANDLE runThread = RunDllSubInExe( dll, "_OutArrayDll@4", exe, param );
	if( runThread ){
		// максимальное ожидание на выполнение функции 10 секунд 
		WaitForSingleObject( runThread, 10000 );
	}
	TerminateProcess( exe, 0 ); // закрытие дочернего процесса 
	ReadFromPipe();
	//_getch();
	return 0;
}

/*/// пример сериализуемой структуры 
// вложенная структура 
struct_( Effect )
	_(int, id	);
	_(int, power);
	end_
};
// основная стуктура 
struct_( SomeItem )
	union_(
		int,	id	);
		int		EndMarker;
	};
	_(char*,	name );
	A(int,		damage, [1] );
	A(Effect,	effect, [2] );
	end_
};
// массив структур 
SomeItem Items[] = { 
	{ 1, "Sword"	, 35, 11, 70 , 13, 21 },
	{ 2, "Axe"		, 60, 12, 80 , 27, 235},
	{-1, 0, 0, 0, 0, 0, 0  }, // No item
};//*/

// ------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	/*// тест выгрузки тестовой структуры 
	OutArrayByName( "SomeItem", (void*)Items, 10 );
	CurOutMode = OM_TXT;
	out_array( Items );
	CurOutMode = OM_CPP;
	out_array( Items );
	CurOutMode = OM_XLS;
	out_array( Items );
 	_getch();
	return 0;
	//*/
	
	char name[512] = {0};	
	char address[64] = {0};
	char count[64] = {0};
	char param[1024] = {0};
	
	if( argc < 3 ){
		cout << "Invalid parameters\n\n"
				"LoadExeData out_mode struct_name|input_file_name [address [count [IgnoreEndMarker_flag]]]\n\n"
				"Out modes: ";
		out_( &OutModes );
		cout << endl;
		return ER_invalid_parametrs;
	}
	strcpy_s( param, argv[1] ); 
	strcat_s( param, " " ); // внимание, в конце уже есть пробел 
	char* path = "..\\lib\\";
	
	// карта памяти данных в TH.exe, формат записи в файле: имя_структуры адрес_массива_структур количество_элементов_массива 
	// адрес - виртуальный в адресном пространстве процесса (вида 0x00601234), а НЕ смещение относительно начала файла 
	char* dataMapName = "TH_exe_data_map.txt"; 

	// загрузка имен сериализуемых структур и их адресов в TH1 
	// кстати выгрузку имен и адресов (TH2, для отладки и авторефактора) можно сделать автоматически 
	FILE* structPtrFile = fopen( FindFilePath( dataMapName, path ), "r" );
	if( ! structPtrFile ){
		cout << "Not found exe data map file: " << dataMapName << endl;
		return ER_no_exe_data_map_file;
	}
	bool structNotFound = true;
	if( argc < 4 ){ // задано только имя типа, ищем его в таблице адресов (map file) сериализуемых типов 
		for( int i = 0; i < MAX_REFLECT && !feof(structPtrFile); i++ ){
			fscanf_s(structPtrFile, "%s", name, sizeof name);
			fscanf_s(structPtrFile, "%s", address, sizeof address);
			fscanf_s(structPtrFile, "%s", count, sizeof count);
			if( !strcmp( argv[2], name ) ){
				strcat_s( param, name ); 
				strcat_s( param, " " );
				strcat_s( param, address );
				strcat_s( param, " " );
				strcat_s( param, count );
				structNotFound = false;
				break;
			}
		}
	}else{
		for( int i = 2; i < argc; i++ ){
			strcat_s( param, argv[i] ); 
			strcat_s( param, " " );
		}
		structNotFound = false;
	}
	fclose(structPtrFile);

	// если имя структуры не найдено, проверяем на наличие файл с данными по выводу 
	if( structNotFound ){
		char* dataFileName = FindFilePath( argv[2], path );
		if( dataFileName ){
			strcat_s( param, dataFileName ); 
			strcat_s( param, " " );
			strcat_s( param, "f" ); // пометка для файла 
		}else{
			cout << "Struct or file \"" << argv[2] << "\" not found or non-serializable.\n Check " << dataMapName << endl;
			return ER_struct_not_found;
		}
	}
		
	char* exeName = "TH.exe";
	char* exePathName = FindFilePath( exeName, path );
	if( ! exePathName ){
		cout << "Not found input exe file: " << exeName;
		return ER_exe_not_found;
	}
	
	// static const type_meta* const serializedStructs = types<>::meta();
	// for( int i = 0; i < MAX_REFLECT && serializedStructs[i].outArray; i++ ){
	// 	cout << serializedStructs[i].typeName << endl;
	// }
	
	return LoadDataFromExe( exePathName, param );
	//_getch();
}

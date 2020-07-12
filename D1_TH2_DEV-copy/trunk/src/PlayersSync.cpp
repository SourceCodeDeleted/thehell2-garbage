
//----- (0047336E) --------------------------------------------------------
int __fastcall CheckServerTimeOut1(void* Dst)
{
	void* v1;   // ebx@1
	char* v2;   // esi@2
	int result; // eax@3
	int v4;     // edi@4
	v1 = Dst;
	if( ServerDataPtr && (v2 = ServerDataPtr, (int)(*((uint*)ServerDataPtr + 1) - GetTickCount_()) < 0) ){ // сетевая игра, перехватываем вызов
		ServerDataPtr = *(char**)ServerDataPtr;
		v4 = *(v2 + 8);
		memcpy(v1, v2 + 9, *(v2 + 8));
		FreeMem(v2);
		result = v4;
	}else{
		result = 0;
	}
	return result;
}

//----- (0047340C) --------------------------------------------------------
void ServerDataCycle()
{
	char* serverDataPtr; // eax@1
	char* v1;     // esi@2
	serverDataPtr = ServerDataPtr;
	if( ServerDataPtr ){
		do{
			v1 = *(char**)serverDataPtr;
			ServerDataPtr = 0;
			FreeMem(serverDataPtr);
			serverDataPtr = v1;
			ServerDataPtr = v1;
		}while( v1 );
	}
}

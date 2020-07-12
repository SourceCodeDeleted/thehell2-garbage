
//----- (00417A4E) -------------------------------------------------------- main
void SetupCriticalSection4()
{
	InitCriticalSection4();
	atexit( DelCriticalSection4 );
}

//----- (00417A58) -------------------------------------------------------- main
void InitCriticalSection4()
{
	InitializeCriticalSection(&CriticalSection4);
}

// тут нельзя менять __cdecl на __fastcall, тип задан в crt 
//----- (00417A70) -------------------------------------------------------- main
void DelCriticalSection4()
{
	DeleteCriticalSection(&CriticalSection4);
}

//----- (00417A7C) -------------------------------------------------------- main
void __fastcall SetNetCmdBuffersAsInvalidForPlayer(int playerIndex)
{
	EnterCriticalSection(&CriticalSection4);
	int* curPtr = NetCmdSendQueuePtr;
	while( curPtr ){
		int* header = (int*)curPtr;
		if( header[1] == playerIndex ){
			header[1] = 4;
		}
		curPtr = (int*)header[0];
	}
	LeaveCriticalSection(&CriticalSection4);
}

//----- (00417AAF) --------------------------------------------------------
void __fastcall SendBigNetMessage(int playerIndex, char netCmdIndex, void* src, size_t size)
{
	if( MaxCountOfPlayersInGame == 1 ){
		return;
	}
	char* buffer = (char*) AllocMem(size + 20);// 16 байт заголовок, 4 ,айта подстраховка?

	int* header = (int*)buffer;
	header[0] = 0;// тут будет указатель на следующий буфер в очереди или стеке. пока не ясно
	header[1] = playerIndex;
	header[2] = netCmdIndex;
	header[3] = size;

	memcpy(buffer + 16, src, size);
	EnterCriticalSection(&CriticalSection4);

	int* curPtr = NetCmdSendQueuePtr;
	int** aCurPtr = &NetCmdSendQueuePtr;
	while( curPtr ){
		aCurPtr = (int**) curPtr;
		curPtr = (int*) *curPtr;
	}
	*aCurPtr = header;// находим последний буфер и добавляем указатель в конец

	SetEvent(hEvent);
	LeaveCriticalSection(&CriticalSection4);
}

//----- (00417B2C) -------------------------------------------------------- main
void dthread()
{
	HANDLE v0;           // eax@2
	unsigned __int32 v2; // eax@4
	if( MaxCountOfPlayersInGame != 1 ){
		v0 = CreateEventA(0, 1, 0, 0);
		hEvent = v0;
		if( !v0 ){
			TerminateWithError("dthread:1\n%s", GetErrorTextForLastError());
		}
		IsEventCreated = 1;
		v2 = _beginthreadex(0, 0, dthread2, 0, 0, (unsigned int*)&ThreadId);
		hHandle = (HANDLE)v2;
		if( v2 == -1 ){
			TerminateWithError("dthread2:\n%s", GetErrorTextForLastError());
		}
	}
}

//----- (00417B99) -------------------------------------------------------- main
unsigned int __stdcall dthread2(void*)
{
	while( IsEventCreated ){
		if( !NetCmdSendQueuePtr ){
			if( WaitForSingleObject(hEvent, INFINITE) == -1 ){
				TerminateWithError("dthread4:\n%s", GetErrorTextForLastError());
			}
		}
		EnterCriticalSection(&CriticalSection4);
		int* curBuffer = NetCmdSendQueuePtr;
		if( curBuffer ){
			NetCmdSendQueuePtr = (int*)*curBuffer;
		}else{
			ResetEvent(hEvent);
		}
		LeaveCriticalSection(&CriticalSection4);
		if( curBuffer ){
			int* header = (int*)curBuffer;
			int playerIndex = header[1];
			int netCmdIndex = header[2];
			int size = header[3];

			if( playerIndex != 4 ){
				SendCmd24(playerIndex, netCmdIndex, curBuffer + 4, size);
			}
			uint delayCount = 1000 * size / Divider;
			if( delayCount >= 1 ){
				delayCount = 1;
			}
			FreeMem(curBuffer);
			if( delayCount ){
				Sleep(delayCount);
			}
		}
	}
	return 0;
}

//----- (00417C66) --------------------------------------------------------
void dthread3()
{
	if( !hEvent ){
		return;
	}
	IsEventCreated = 0;
	SetEvent(hEvent);
	if( hHandle != (HANDLE)-1 ){
		if( ThreadId != GetCurrentThreadId() ){
			if( WaitForSingleObject(hHandle, 0xFFFFFFFFu) == -1 ){
				TerminateWithError("dthread3:\n(%s)", GetErrorTextForLastError());
			}
			CloseHandle(hHandle);
			hHandle = (HANDLE)-1;
		}
	}
	CloseHandle(hEvent);
	hEvent = 0;

	int* curPtr = NetCmdSendQueuePtr;
	int* nextPtr;
	while( curPtr ){
		nextPtr = (int*)*curPtr;
		FreeMem((char*)curPtr);
		curPtr = nextPtr;
		NetCmdSendQueuePtr = curPtr;
	}
}

//----- (00417D08) -------------------------------------------------------- main
int SetupCriticalSection()
{
	InitCriticalSection();
	return atexit(DelCriticalSection);
}

//CRT calls;   functions calling this shit are from С-runtime lib
//----- (00417D12) --------------------------------------------------------
void InitCriticalSection()
{
	InitializeCriticalSection(&DDSurfaceCritSec);
}

//----- (00417D2A) --------------------------------------------------------
void DelCriticalSection()
{
	DeleteCriticalSection(&DDSurfaceCritSec);
}
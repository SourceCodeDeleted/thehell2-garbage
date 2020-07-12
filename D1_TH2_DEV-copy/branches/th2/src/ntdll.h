#ifndef _NTDLL_H_
#define _NTDLL_H_

#include <windows.h>

//
// Undef status codes defined in windows.h->winnt.h
//

#undef STATUS_WAIT_0
#undef STATUS_ABANDONED_WAIT_0
#undef STATUS_USER_APC
#undef STATUS_TIMEOUT
#undef STATUS_PENDING
#undef DBG_EXCEPTION_HANDLED
#undef DBG_CONTINUE
#undef STATUS_SEGMENT_NOTIFICATION
#undef DBG_TERMINATE_THREAD
#undef DBG_TERMINATE_PROCESS
#undef DBG_CONTROL_C
#undef DBG_CONTROL_BREAK
#undef DBG_COMMAND_EXCEPTION
#undef STATUS_GUARD_PAGE_VIOLATION
#undef STATUS_DATATYPE_MISALIGNMENT
#undef STATUS_BREAKPOINT
#undef STATUS_SINGLE_STEP
#undef DBG_EXCEPTION_NOT_HANDLED
#undef STATUS_ACCESS_VIOLATION
#undef STATUS_IN_PAGE_ERROR
#undef STATUS_INVALID_HANDLE
#undef STATUS_NO_MEMORY
#undef STATUS_ILLEGAL_INSTRUCTION
#undef STATUS_NONCONTINUABLE_EXCEPTION
#undef STATUS_INVALID_DISPOSITION
#undef STATUS_ARRAY_BOUNDS_EXCEEDED
#undef STATUS_FLOAT_DENORMAL_OPERAND
#undef STATUS_FLOAT_DIVIDE_BY_ZERO
#undef STATUS_FLOAT_INEXACT_RESULT
#undef STATUS_FLOAT_INVALID_OPERATION
#undef STATUS_FLOAT_OVERFLOW
#undef STATUS_FLOAT_STACK_CHECK
#undef STATUS_FLOAT_UNDERFLOW
#undef STATUS_INTEGER_DIVIDE_BY_ZERO
#undef STATUS_INTEGER_OVERFLOW
#undef STATUS_PRIVILEGED_INSTRUCTION
#undef STATUS_STACK_OVERFLOW
#undef STATUS_CONTROL_C_EXIT
#undef STATUS_FLOAT_MULTIPLE_FAULTS
#undef STATUS_FLOAT_MULTIPLE_TRAPS
#undef STATUS_REG_NAT_CONSUMPTION
#undef STATUS_SXS_EARLY_DEACTIVATION
#undef STATUS_SXS_INVALID_DEACTIVATION

#include <ntstatus.h>


//#pragma warning (disable:4005)

#undef NTSYSAPI

#ifdef __cplusplus
#define NTSYSAPI extern "C" //__declspec(dllimport)
#else
#define NTSYSAPI
#endif

#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
typedef LONG NTSTATUS;


//
// Section Access Rights.
//

// begin_winnt
#define SECTION_QUERY       0x0001
#define SECTION_MAP_WRITE   0x0002
#define SECTION_MAP_READ    0x0004
#define SECTION_MAP_EXECUTE 0x0008
#define SECTION_EXTEND_SIZE 0x0010

#define SECTION_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED|SECTION_QUERY|\
                            SECTION_MAP_WRITE |      \
                            SECTION_MAP_READ |       \
                            SECTION_MAP_EXECUTE |    \
                            SECTION_EXTEND_SIZE)
// end_winnt

#define SEGMENT_ALL_ACCESS SECTION_ALL_ACCESS



//
// Counted String
//

typedef struct _STRING {
    USHORT Length;
    USHORT MaximumLength;
#ifdef MIDL_PASS
    [size_is(MaximumLength), length_is(Length) ]
#endif // MIDL_PASS
    PCHAR Buffer;
} STRING;
typedef STRING *PSTRING;

typedef STRING ANSI_STRING;
typedef PSTRING PANSI_STRING;

typedef STRING OEM_STRING;
typedef PSTRING POEM_STRING;

//
// CONSTCounted String
//

typedef struct _CSTRING {
    USHORT Length;
    USHORT MaximumLength;
    CONST char *Buffer;
} CSTRING;
typedef CSTRING *PCSTRING;
#define ANSI_NULL ((CHAR)0)     // winnt

typedef STRING CANSI_STRING;
typedef PSTRING PCANSI_STRING;

//
// Unicode strings are counted 16-bit character strings. If they are
// NULL terminated, Length does not include trailing NULL.
//

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
#ifdef MIDL_PASS
    [size_is(MaximumLength / 2), length_is((Length) / 2) ] USHORT * Buffer;
#else // MIDL_PASS
    PWSTR  Buffer;
#endif // MIDL_PASS
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;
#define UNICODE_NULL ((WCHAR)0) // winnt


// begin_ntminiport begin_ntminitape

//
// Boolean
//

typedef UCHAR BOOLEAN;           // winnt
typedef BOOLEAN *PBOOLEAN;       // winnt

// end_ntminiport end_ntminitape

// begin_winnt

// end_winnt


//
// These macros are used to walk lists on a target system
//
#define CONTAINING_RECORD32(address, type, field) ( \
                                                  (ULONG)(address) - \
                                                  (ULONG)(&((type *)0)->field))

#define CONTAINING_RECORD64(address, type, field) ( \
                                                  (ULONGLONG)(address) - \
                                                  (ULONGLONG)(&((type *)0)->field))

typedef struct _STRING32 {
    USHORT   Length;
    USHORT   MaximumLength;
    ULONG  Buffer;
} STRING32;
typedef STRING32 *PSTRING32;

typedef STRING32 UNICODE_STRING32;
typedef UNICODE_STRING32 *PUNICODE_STRING32;

typedef STRING32 ANSI_STRING32;
typedef ANSI_STRING32 *PANSI_STRING32;


typedef struct _STRING64 {
    USHORT   Length;
    USHORT   MaximumLength;
    ULONGLONG  Buffer;
} STRING64;
typedef STRING64 *PSTRING64;

typedef STRING64 UNICODE_STRING64;
typedef UNICODE_STRING64 *PUNICODE_STRING64;

typedef STRING64 ANSI_STRING64;
typedef ANSI_STRING64 *PANSI_STRING64;



//
// Valid values for the Attributes field
//

#define OBJ_INHERIT             0x00000002L
#define OBJ_PERMANENT           0x00000010L
#define OBJ_EXCLUSIVE           0x00000020L
#define OBJ_CASE_INSENSITIVE    0x00000040L
#define OBJ_OPENIF              0x00000080L
#define OBJ_OPENLINK            0x00000100L
#define OBJ_KERNEL_HANDLE       0x00000200L
#define OBJ_VALID_ATTRIBUTES    0x000003F2L

//
// Object Attributes structure
//

typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR
    PVOID SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES;

//++
//
// VOID
// InitializeObjectAttributes(
//     OUT POBJECT_ATTRIBUTES p,
//     IN PUNICODE_STRING n,
//     IN ULONG a,
//     IN HANDLE r,
//     IN PSECURITY_DESCRIPTOR s
//     )
//
//--

#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
    }

#define DIRECTORY_QUERY                 (0x0001)
#define DIRECTORY_TRAVERSE              (0x0002)
#define DIRECTORY_CREATE_OBJECT         (0x0004)
#define DIRECTORY_CREATE_SUBDIRECTORY   (0x0008)
#define DIRECTORY_ALL_ACCESS (STANDARD_RIGHTS_REQUIRED | 0xF)

NTSYSAPI 
NTSTATUS
NTAPI
NtOpenDirectoryObject(
  OUT PHANDLE             DirectoryObjectHandle,
  IN ACCESS_MASK          DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes );

#define STATUS_NO_MORE_ENTRIES           ((NTSTATUS)0x8000001AL)

typedef struct _OBJDIR_INFORMATION {
  UNICODE_STRING          ObjectName;
  UNICODE_STRING          ObjectTypeName;
  BYTE                    Data[1];
} OBJDIR_INFORMATION, *POBJDIR_INFORMATION;

NTSYSAPI 
NTSTATUS
NTAPI
NtQueryDirectoryObject(
  IN HANDLE               DirectoryObjectHandle,
  OUT POBJDIR_INFORMATION DirObjInformation,
  IN ULONG                BufferLength,
  IN BOOLEAN              GetNextIndex,
  IN BOOLEAN              IgnoreInputIndex,
  IN OUT PULONG           ObjectIndex,
  OUT PULONG              DataWritten OPTIONAL );

NTSYSAPI
NTSTATUS
NTAPI
NtClose(
  IN HANDLE ObjectHandle );


NTSYSAPI 
NTSTATUS
NTAPI
NtOpenFile(

  OUT PHANDLE             FileHandle,
  IN ACCESS_MASK          DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes,
  OUT PVOID    IoStatusBlock,
  IN ULONG                ShareAccess,
  IN ULONG                OpenOptions );

typedef enum _FILE_INFORMATION_CLASS {


    FileDirectoryInformation=1,
    FileFullDirectoryInformation,
    FileBothDirectoryInformation,
    FileBasicInformation,
    FileStandardInformation,
    FileInternalInformation,
    FileEaInformation,
    FileAccessInformation,
    FileNameInformation,
    FileRenameInformation,
    FileLinkInformation,
    FileNamesInformation,
    FileDispositionInformation,
    FilePositionInformation,
    FileFullEaInformation,
    FileModeInformation,
    FileAlignmentInformation,
    FileAllInformation,
    FileAllocationInformation,
    FileEndOfFileInformation,
    FileAlternateNameInformation,
    FileStreamInformation,
    FilePipeInformation,
    FilePipeLocalInformation,
    FilePipeRemoteInformation,
    FileMailslotQueryInformation,
    FileMailslotSetInformation,
    FileCompressionInformation,
    FileCopyOnWriteInformation,
    FileCompletionInformation,
    FileMoveClusterInformation,
    FileQuotaInformation,
    FileReparsePointInformation,
    FileNetworkOpenInformation,
    FileObjectIdInformation,
    FileTrackingInformation,
    FileOleDirectoryInformation,
    FileContentIndexInformation,
    FileInheritContentIndexInformation,
    FileOleInformation,
    FileMaximumInformation
} FILE_INFORMATION_CLASS, *PFILE_INFORMATION_CLASS;

typedef struct FILE_BASIC_INFORMATION {
  LARGE_INTEGER  CreationTime;
  LARGE_INTEGER  LastAccessTime;
  LARGE_INTEGER  LastWriteTime;
  LARGE_INTEGER  ChangeTime;
  ULONG  FileAttributes;
} FILE_BASIC_INFORMATION, *PFILE_BASIC_INFORMATION;

typedef struct FILE_POSITION_INFORMATION {
  LARGE_INTEGER  CurrentByteOffset;
} FILE_POSITION_INFORMATION, *PFILE_POSITION_INFORMATION;

#define FILE_SUPERSEDE                  0x00000000
#define FILE_OPEN                       0x00000001
#define FILE_CREATE                     0x00000002
#define FILE_OPEN_IF                    0x00000003
#define FILE_OVERWRITE                  0x00000004
#define FILE_OVERWRITE_IF               0x00000005

NTSYSAPI 
NTSTATUS
NTAPI
NtQueryDirectoryFile(

  IN HANDLE               FileHandle,
  IN HANDLE               Event OPTIONAL,
  IN PVOID      ApcRoutine OPTIONAL,
  IN PVOID                ApcContext OPTIONAL,
  OUT PVOID    IoStatusBlock,
  OUT PVOID               FileInformation,
  IN ULONG                Length,
  IN FILE_INFORMATION_CLASS FileInformationClass,
  IN BOOLEAN              ReturnSingleEntry,
  IN PUNICODE_STRING      FileMask OPTIONAL,
  IN BOOLEAN              RestartScan );

//
// Process priority classes
//
#define PROCESS_PRIORITY_CLASS_UNKNOWN          0
#define PROCESS_PRIORITY_CLASS_IDLE             1
#define PROCESS_PRIORITY_CLASS_NORMAL           2
#define PROCESS_PRIORITY_CLASS_HIGH             3
#define PROCESS_PRIORITY_CLASS_REALTIME         4
#define PROCESS_PRIORITY_CLASS_BELOW_NORMAL     5
#define PROCESS_PRIORITY_CLASS_ABOVE_NORMAL     6

typedef struct _PROCESS_PRIORITY_CLASS
{
    BOOLEAN Foreground;
    UCHAR PriorityClass;
} PROCESS_PRIORITY_CLASS, *PPROCESS_PRIORITY_CLASS;

#define PRIORITY_CLASS_MASK  ( NORMAL_PRIORITY_CLASS | IDLE_PRIORITY_CLASS | HIGH_PRIORITY_CLASS | REALTIME_PRIORITY_CLASS )

typedef struct _CLIENT_ID {
    DWORD         UniqueProcess;
    DWORD         UniqueThread;
} CLIENT_ID;

typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID Pointer;
    };
    ULONG Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct _PEB_LDR_DATA {
  ULONG                   Length;
  BOOLEAN                 Initialized;
  PVOID                   SsHandle;
  LIST_ENTRY              InLoadOrderModuleList;
  LIST_ENTRY              InMemoryOrderModuleList;
  LIST_ENTRY              InInitializationOrderModuleList;
  
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _RTL_DRIVE_LETTER_CURDIR {
  USHORT                  Flags;
  USHORT                  Length;
  ULONG                   TimeStamp;
  UNICODE_STRING          DosPath;
} RTL_DRIVE_LETTER_CURDIR, *PRTL_DRIVE_LETTER_CURDIR;

typedef struct _CURDIR
{
	UNICODE_STRING DosPath;
	PVOID Handle;
} CURDIR, *PCURDIR;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
  ULONG                   MaximumLength;
  ULONG                   Length;
  ULONG                   Flags;
  ULONG                   DebugFlags;
  PVOID                   ConsoleHandle;
  ULONG                   ConsoleFlags;
  HANDLE                  StandardInput;
  HANDLE                  StandardOutput;
  HANDLE                  StandardError;
  CURDIR                  CurrentDirectory;
  UNICODE_STRING          DllPath;
  UNICODE_STRING          ImagePathName;
  UNICODE_STRING          CommandLine;
  PVOID                   Environment;
  ULONG                   StartingX;
  ULONG                   StartingY;
  ULONG                   CountX;
  ULONG                   CountY;
  ULONG                   CountCharsX;
  ULONG                   CountCharsY;
  ULONG                   FillAttribute;
  ULONG                   WindowFlags;
  ULONG                   ShowWindowFlags;
  UNICODE_STRING          WindowTitle;
  UNICODE_STRING          DesktopInfo;
  UNICODE_STRING          ShellInfo;
  UNICODE_STRING          RuntimeData;
  RTL_DRIVE_LETTER_CURDIR CurrentDirectories[0x20];
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;


NTSYSAPI
NTSTATUS
NTAPI
RtlCreateProcessParameters(
    OUT PRTL_USER_PROCESS_PARAMETERS *ProcessParameters,
    IN PUNICODE_STRING ImagePathName OPTIONAL,
    IN PUNICODE_STRING DllPath OPTIONAL,
    IN PUNICODE_STRING CurrentDirectory OPTIONAL,
    IN PUNICODE_STRING CommandLine OPTIONAL,
    IN PUNICODE_STRING Environment OPTIONAL,
    IN PUNICODE_STRING WindowTitle OPTIONAL,
    IN PUNICODE_STRING DesktopInfo OPTIONAL,
    IN PUNICODE_STRING ShellInfo OPTIONAL,
    IN PUNICODE_STRING RuntimeInfo OPTIONAL
   );

#define	CONSOLE_DETACHED_PROCESS	0xFFFFFFFF
#define	CONSOLE_NEW_CONSOLE			0xFFFFFFFE
#define	CONSOLE_CREATE_NO_WINDOW	0xFFFFFFFD

#define	RTL_USER_PROC_DISABLE_HEAP_DECOMMIT	0x100

#define	RTL_USER_PROC_PROFILE_USER		2
#define	RTL_USER_PROC_PROFILE_KERNEL	4
#define	RTL_USER_PROC_PROFILE_SERVER	8

NTSYSAPI
VOID
NTAPI
RtlDestroyProcessParameters(
    IN PRTL_USER_PROCESS_PARAMETERS ProcessParameters
   );


typedef VOID (NTAPI *PPEBLOCKROUTINE)(PVOID);

typedef PVOID *PPVOID, **PPPVOID;

typedef struct _PEB_FREE_BLOCK {
  struct _PEB_FREE_BLOCK  *Next;
  ULONG                   Size;
} PEB_FREE_BLOCK, *PPEB_FREE_BLOCK;

typedef struct _PEB {
  BOOLEAN                 InheritedAddressSpace;
  BOOLEAN                 ReadImageFileExecOptions;
  BOOLEAN                 BeingDebugged;
  BOOLEAN                 SpareBool;
  HANDLE                  Mutant;
  PVOID                   ImageBaseAddress;
  PPEB_LDR_DATA           Ldr;
  PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
  PVOID                   SubSystemData;
  PVOID                   ProcessHeap;
  PVOID                   FastPebLock;
  PPEBLOCKROUTINE         FastPebLockRoutine;
  PPEBLOCKROUTINE         FastPebUnlockRoutine;
  ULONG                   EnvironmentUpdateCount;
  PPVOID                  KernelCallbackTable;
	union {
		struct {
			PVOID                   EventLogSection;
			PVOID                   EventLog;
		};
		struct {
			PVOID                   SystemReserved[1];
			PVOID                   AltThunkSListPtr32;
		};
	};
  PPEB_FREE_BLOCK         FreeList;
  ULONG                   TlsExpansionCounter;
  PVOID                   TlsBitmap;
  ULONG                   TlsBitmapBits[2];
  PVOID                   ReadOnlySharedMemoryBase;
  PVOID                   ReadOnlySharedMemoryHeap;
  PPVOID                  ReadOnlyStaticServerData;
  PVOID                   AnsiCodePageData;
  PVOID                   OemCodePageData;
  PVOID                   UnicodeCaseTableData;
  ULONG                   NumberOfProcessors;
  ULONG                   NtGlobalFlag;
  ULONG                   Spare2;
  LARGE_INTEGER           CriticalSectionTimeout;
  ULONG                   HeapSegmentReserve;
  ULONG                   HeapSegmentCommit;
  ULONG                   HeapDeCommitTotalFreeThreshold;
  ULONG                   HeapDeCommitFreeBlockThreshold;
  ULONG                   NumberOfHeaps;
  ULONG                   MaximumNumberOfHeaps;
  PPVOID                  *ProcessHeaps;
  PVOID                   GdiSharedHandleTable;
  PVOID                   ProcessStarterHelper;
  PVOID                   GdiDCAttributeList;
  PVOID                   LoaderLock;
  ULONG                   OSMajorVersion;
  ULONG                   OSMinorVersion;
  USHORT                  OSBuildNumber;
	USHORT                  OSCSDVersion;
  ULONG                   OSPlatformId;
  ULONG                   ImageSubsystem;
  ULONG                   ImageSubSystemMajorVersion;
  ULONG                   ImageSubSystemMinorVersion;
	ULONG                   ImageProcessAffinityMask;
  ULONG                   GdiHandleBuffer[34];
  PVOID                   PostProcessInitRoutine; // void (*)();
  ULONG                   TlsExpansionBitmap;
  ULONG                   TlsExpansionBitmapBits[32];
  ULONG                   SessionId;
	ULARGE_INTEGER          AppCompatFlags;
	ULARGE_INTEGER          AppCompatFlagsUser;
	PVOID                   pShimData;
	PVOID                   AppCompatInfo;
	UNICODE_STRING          CSDVersion;
	PVOID                   ActivationContextData;
	PVOID                   ProcessAssemblyStorageMap;
	PVOID                   SystemDefaultActivationContextData;
	PVOID                   SystemAssemblyStorageMap;
	ULONG                   MinimumStackCommit;
} PEB, *PPEB;

typedef struct _ACTIVATION_CONTEXT_STACK {
	ULONG Flags;
	ULONG NextCookieSequenceNumber;
	PVOID ActiveFrame;
	LIST_ENTRY FrameListCache;
} ACTIVATION_CONTEXT_STACK, *PACTIVATION_CONTEXT_STACK;

typedef struct _GDI_TEB_BATCH {
	ULONG Offset;
	ULONG HDC;
	ULONG Buffer[310];
} GDI_TEB_BATCH, *PGDI_TEB_BATCH;

typedef struct _Wx86ThreadState {
	ULONG* CallBx86Eip;
	PVOID DeallocationCpu;
	BOOLEAN UseKnownWx86Dll;
	CHAR OleStubInvoked;
} Wx86ThreadState, *PWx86ThreadState;

typedef struct _TEB_ACTIVE_FRAME_CONTEXT {
	ULONG Flags;
	PSTR FrameName;
} TEB_ACTIVE_FRAME_CONTEXT, *PTEB_ACTIVE_FRAME_CONTEXT;

typedef struct _TEB_ACTIVE_FRAME {
	ULONG Flags;
	struct _TEB_ACTIVE_FRAME* Previous;
	PTEB_ACTIVE_FRAME_CONTEXT Context;
} TEB_ACTIVE_FRAME, *PTEB_ACTIVE_FRAME;

typedef struct _TEB {
  NT_TIB                  NtTib;
  PVOID                   EnvironmentPointer;
  CLIENT_ID               ClientId;
  PVOID                   ActiveRpcHandle;
  PVOID                   ThreadLocalStoragePointer;
  PPEB                    Peb;
  ULONG                   LastErrorValue;
  ULONG                   CountOfOwnedCriticalSections;
  PVOID                   CsrClientThread;
  PVOID                   Win32ThreadInfo;
  ULONG                   User32Reserved[26];
	ULONG                   UserReserved[5];
  PVOID                   WOW32Reserved;
  ULONG                   CurrentLocale;
  ULONG                   FpSoftwareStatusRegister;
  PVOID                   SystemReserved1[54];
  ULONG                   ExceptionCode;
	ACTIVATION_CONTEXT_STACK  ActivationContextStack;
  UCHAR                   SpareBytes1[24];
	GDI_TEB_BATCH           GdiTebBatch;
  CLIENT_ID               RealClientId;
  PVOID                   GdiCachedProcessHandle;
  ULONG                   GdiClientPID;
  ULONG                   GdiClientTID;
  PVOID                   GdiThreadLocalInfo;
  ULONG                   Win32ClientInfo[62];
  PVOID                   GlDispatchTable[233];
  ULONG                   GlReserved1[29];
  PVOID                   GlReserved2;
  PVOID                   GlSectionInfo;
  PVOID                   GlSection;
  PVOID                   GlTable;
  PVOID                   GlCurrentRC;
  PVOID                   GlContext;
  NTSTATUS                LastStatusValue;
  UNICODE_STRING          StaticUnicodeString;
  WCHAR                   StaticUnicodeBuffer[261];
  PVOID                   DeallocationStack;
  PVOID                   TlsSlots[64];
  LIST_ENTRY              TlsLinks;
  PVOID                   Vdm;
  PVOID                   ReservedForNtRpc;
  PVOID                   DbgSsReserved[2];
  ULONG                   HardErrorsAreDisabled;
  PVOID                   Instrumentation[16];
  PVOID                   WinSockData;
  ULONG                   GdiBatchCount;
	UCHAR                   InDbgPrint;
	UCHAR                   FreeStackOnTermination;
	UCHAR                   HasFiberData;
	UCHAR                   IdealProcessor;
  ULONG                   Spare3;
  PVOID                   ReservedForPerf;
  PVOID                   ReservedForOle;
  ULONG                   WaitingOnLoaderLock;
	Wx86ThreadState         Wx86Thread;
	PVOID*                  TlsExpansionSlots;
	ULONG                   ImpersonationLocale;
	ULONG                   IsImpersonating;
	PVOID                   NlsCache;
	PVOID                   pShimData;
	ULONG                   HeapVirtualAffinity;
	PVOID                   CurrentTransactionHandle;
	PTEB_ACTIVE_FRAME       ActiveFrame;
	BOOLEAN                 SafeThunkCall;
	BOOLEAN                 BooleanSpare[3];
} TEB, *PTEB;

//
// Private flags for loader data table entries
//

#define LDRP_STATIC_LINK                0x00000002
#define LDRP_IMAGE_DLL                  0x00000004
#define LDRP_LOAD_IN_PROGRESS           0x00001000
#define LDRP_UNLOAD_IN_PROGRESS         0x00002000
#define LDRP_ENTRY_PROCESSED            0x00004000
#define LDRP_ENTRY_INSERTED             0x00008000
#define LDRP_CURRENT_LOAD               0x00010000
#define LDRP_FAILED_BUILTIN_LOAD        0x00020000
#define LDRP_DONT_CALL_FOR_THREADS      0x00040000
#define LDRP_PROCESS_ATTACH_CALLED      0x00080000
#define LDRP_DEBUG_SYMBOLS_LOADED       0x00100000
#define LDRP_IMAGE_NOT_AT_BASE          0x00200000
#define LDRP_COR_IMAGE                  0x00400000
#define LDRP_COR_OWNS_UNMAP             0x00800000
#define LDRP_SYSTEM_MAPPED              0x01000000
#define LDRP_IMAGE_VERIFYING            0x02000000
#define LDRP_DRIVER_DEPENDENT_DLL       0x04000000
#define LDRP_ENTRY_NATIVE               0x08000000
#define LDRP_REDIRECTED                 0x10000000
#define LDRP_NON_PAGED_DEBUG_INFO       0x20000000
#define LDRP_MM_LOADED                  0x40000000
#define LDRP_COMPAT_DATABASE_PROCESSED  0x80000000

typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InLoadOrderLinks;           // \ 
	LIST_ENTRY InMemoryOrderLinks;         // +- see PEB_LDR_DATA
	LIST_ENTRY InInitializationOrderLinks; // /

	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;  // see LDRP_***
	WORD LoadCount;
	WORD TlsIndex;
	union
	{
		LIST_ENTRY HashLinks; // in LdrpHashTable[]
		struct
		{
			PVOID SectionPointer; // for kernel mode and session images only.
			ULONG CheckSum;       // for kernel mode images only.
		};
	};
	union
	{
		ULONG TimeDateStamp;
		PVOID LoadedImports; // for kernel mode images only.
	};
	PVOID EntryPointActivationContext;
	PVOID PatchInformation;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

__inline PPEB NtCurrentPeb () 
{ 
	return NtCurrentTeb()->Peb; 
} 

//#define NtCurrentPeb() (NtCurrentTeb()->Peb)

//
//  Doubly-linked list manipulation routines.
//

#define LIST_HEAD_INITIALIZER(X) {&(X), &(X)}


//
//  VOID
//  InitializeListHead32(
//      PLIST_ENTRY32 ListHead
//      );
//

#define InitializeListHead32(ListHead) (\
	(ListHead)->Flink = (ListHead)->Blink = PtrToUlong((ListHead)))


VOID
FORCEINLINE
InitializeListHead(
									 IN PLIST_ENTRY ListHead
									 )
{
	ListHead->Flink = ListHead->Blink = ListHead;
}

//
//  BOOLEAN
//  IsListEmpty(
//      PLIST_ENTRY ListHead
//      );
//

#define IsListEmpty(ListHead) \
	((ListHead)->Flink == (ListHead))



VOID
FORCEINLINE
RemoveEntryList(
								IN PLIST_ENTRY Entry
								)
{
	PLIST_ENTRY Blink;
	PLIST_ENTRY Flink;

	Flink = Entry->Flink;
	Blink = Entry->Blink;
	Blink->Flink = Flink;
	Flink->Blink = Blink;
}

PLIST_ENTRY
FORCEINLINE
RemoveHeadList(
							 IN PLIST_ENTRY ListHead
							 )
{
	PLIST_ENTRY Flink;
	PLIST_ENTRY Entry;

	Entry = ListHead->Flink;
	Flink = Entry->Flink;
	ListHead->Flink = Flink;
	Flink->Blink = ListHead;
	return Entry;
}



PLIST_ENTRY
FORCEINLINE
RemoveTailList(
							 IN PLIST_ENTRY ListHead
							 )
{
	PLIST_ENTRY Blink;
	PLIST_ENTRY Entry;

	Entry = ListHead->Blink;
	Blink = Entry->Blink;
	ListHead->Blink = Blink;
	Blink->Flink = ListHead;
	return Entry;
}


VOID
FORCEINLINE
InsertTailList(
							 IN PLIST_ENTRY ListHead,
							 IN PLIST_ENTRY Entry
							 )
{
	PLIST_ENTRY Blink;

	Blink = ListHead->Blink;
	Entry->Flink = ListHead;
	Entry->Blink = Blink;
	Blink->Flink = Entry;
	ListHead->Blink = Entry;
}


VOID
FORCEINLINE
InsertHeadList(
							 IN PLIST_ENTRY ListHead,
							 IN PLIST_ENTRY Entry
							 )
{
	PLIST_ENTRY Flink;

	Flink = ListHead->Flink;
	Entry->Flink = Flink;
	Entry->Blink = ListHead;
	Flink->Blink = Entry;
	ListHead->Flink = Entry;
}


//
//
//  PSINGLE_LIST_ENTRY
//  PopEntryList(
//      PSINGLE_LIST_ENTRY ListHead
//      );
//

#define PopEntryList(ListHead) \
	(ListHead)->Next;\
{\
	PSINGLE_LIST_ENTRY FirstEntry;\
	FirstEntry = (ListHead)->Next;\
	if (FirstEntry != NULL) {     \
	(ListHead)->Next = FirstEntry->Next;\
	}                             \
}


//
//  VOID
//  PushEntryList(
//      PSINGLE_LIST_ENTRY ListHead,
//      PSINGLE_LIST_ENTRY Entry
//      );
//

#define PushEntryList(ListHead,Entry) \
	(Entry)->Next = (ListHead)->Next; \
	(ListHead)->Next = (Entry)


typedef LONG KPRIORITY;

//
// Process Information Structures for NtQueryProcessInformation
//
typedef struct _PROCESS_BASIC_INFORMATION
{
    NTSTATUS ExitStatus;
    PPEB PebBaseAddress;
    ULONG AffinityMask;
    KPRIORITY BasePriority;
    ULONG UniqueProcessId;
    ULONG InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION,*PPROCESS_BASIC_INFORMATION;


typedef struct _SECTION_IMAGE_INFORMATION {
  PVOID                   EntryPoint;
  ULONG                   StackZeroBits;
  ULONG                   MaximumStackSize;
  ULONG                   CommittedStackSize;
  ULONG                   SubSystemType;
  WORD                    SubSystemVersionLow;
  WORD                    SubSystemVersionHigh;
  ULONG                   Unknown1;
  ULONG                   ImageCharacteristics;
  ULONG                   Machine;
  ULONG                   Unknown2[3];
} SECTION_IMAGE_INFORMATION, *PSECTION_IMAGE_INFORMATION;

typedef struct _INITIAL_TEB {
    struct {
        PVOID OldStackBase;
        PVOID OldStackLimit;
    } OldInitialTeb;
    PVOID StackBase;
    PVOID StackLimit;
    PVOID StackAllocationBase;
} INITIAL_TEB, *PINITIAL_TEB;

typedef struct _USER_STACK
{
	PVOID  FixedStackBase;
	PVOID  FixedStackLimit;
	PVOID  ExpandableStackBase;
	PVOID  ExpandableStackLimit;
	PVOID  ExpandableStackBottom;
} USER_STACK, *PUSER_STACK;
#define ARGUMENT_PRESENT(x) ( (x) != NULL)

#define STATUS_NOT_IMPLEMENTED ((NTSTATUS)0xC0000002L)

#define PCSZ PCHAR

typedef struct _LDR_MODULE {

  union
  {
	LIST_ENTRY              InLoadOrderModuleList;
	LIST_ENTRY              InMemoryOrderModuleList;
	LIST_ENTRY              InInitializationOrderModuleList;
	LIST_ENTRY				ModuleList;
  };
  PVOID                   BaseAddress;
  PVOID                   EntryPoint;
  ULONG                   SizeOfImage;
  UNICODE_STRING          FullDllName;
  UNICODE_STRING          BaseDllName;
  ULONG                   Flags;
  SHORT                   LoadCount;
  SHORT                   TlsIndex;
  LIST_ENTRY              HashTableEntry;
  ULONG                   TimeDateStamp;

} LDR_MODULE, *PLDR_MODULE;

#ifndef _CLR
__inline TEB * GetTEB()
{
	__asm mov eax, fs:[18h];
#pragma warning(disable:4035)
}
#pragma warning(default:4035)
#endif

NTSYSAPI
ULONG
NTAPI
RtlNtStatusToDosError (
   NTSTATUS Status
   );

NTSYSAPI
VOID
NTAPI
RtlInitString(
    PSTRING DestinationString,
    PCSZ SourceString
    );

NTSYSAPI
VOID
NTAPI
RtlInitAnsiString(
    PANSI_STRING DestinationString,
    PCSZ SourceString
    );

NTSYSAPI
VOID
NTAPI
RtlInitUnicodeString(
    PUNICODE_STRING DestinationString,
    PCWSTR SourceString
    );


NTSYSAPI
VOID
NTAPI
RtlCopyString(
    PSTRING DestinationString,
    PSTRING SourceString
    );

NTSYSAPI
CHAR
NTAPI
RtlUpperChar (
    CHAR Character
    );

NTSYSAPI
LONG
NTAPI
RtlCompareString(
    PSTRING String1,
    PSTRING String2,
    BOOLEAN CaseInSensitive
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlEqualString(
    PSTRING String1,
    PSTRING String2,
    BOOLEAN CaseInSensitive
    );


NTSYSAPI
VOID
NTAPI
RtlUpperString(
    PSTRING DestinationString,
    PSTRING SourceString
    );

//
// NLS String functions
//

NTSYSAPI
NTSTATUS
NTAPI
RtlAnsiStringToUnicodeString(
    PUNICODE_STRING DestinationString,
    PANSI_STRING SourceString,
    BOOLEAN AllocateDestinationString
    );


NTSYSAPI
NTSTATUS
NTAPI
RtlUnicodeStringToAnsiString(
    PANSI_STRING DestinationString,
    PUNICODE_STRING SourceString,
    BOOLEAN AllocateDestinationString
    );


NTSYSAPI
LONG
NTAPI
RtlCompareUnicodeString(
    PUNICODE_STRING String1,
    PUNICODE_STRING String2,
    BOOLEAN CaseInSensitive
    );

NTSYSAPI
BOOLEAN
NTAPI
RtlEqualUnicodeString(
    const UNICODE_STRING *String1,
    const UNICODE_STRING *String2,
    BOOLEAN CaseInSensitive
    );

// end_wdm

NTSYSAPI
BOOLEAN
NTAPI
RtlPrefixUnicodeString(
    IN PUNICODE_STRING String1,
    IN PUNICODE_STRING String2,
    IN BOOLEAN CaseInSensitive
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlUpcaseUnicodeString(
    PUNICODE_STRING DestinationString,
    PCUNICODE_STRING SourceString,
    BOOLEAN AllocateDestinationString
    );


NTSYSAPI
VOID
NTAPI
RtlCopyUnicodeString(
    PUNICODE_STRING DestinationString,
    PUNICODE_STRING SourceString
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlAppendUnicodeStringToString (
    PUNICODE_STRING Destination,
    PUNICODE_STRING Source
    );

NTSYSAPI
NTSTATUS
NTAPI
RtlAppendUnicodeToString (
    PUNICODE_STRING Destination,
    PCWSTR Source
    );

// end_ntndis end_wdm

NTSYSAPI
WCHAR
NTAPI
RtlUpcaseUnicodeChar(
    WCHAR SourceCharacter
    );

// begin_wdm

NTSYSAPI
VOID
NTAPI
RtlFreeUnicodeString(
    PUNICODE_STRING UnicodeString
    );

NTSYSAPI
VOID
NTAPI
RtlFreeAnsiString(
    PANSI_STRING AnsiString
    );


NTSYSAPI
ULONG
NTAPI
RtlxAnsiStringToUnicodeSize(
    PANSI_STRING AnsiString
    );

//
//  NTSYSAPI
//  ULONG
//  NTAPI
//  RtlAnsiStringToUnicodeSize(
//      PANSI_STRING AnsiString
//      );
//

#define RtlAnsiStringToUnicodeSize(STRING) (                 \
    NLS_MB_CODE_PAGE_TAG ?                                   \
    RtlxAnsiStringToUnicodeSize(STRING) :                    \
    ((STRING)->Length + sizeof(ANSI_NULL)) * sizeof(WCHAR) \
)
//
// Define the create disposition values
//

#define FILE_SUPERSEDE                  0x00000000
#define FILE_OPEN                       0x00000001
#define FILE_CREATE                     0x00000002
#define FILE_OPEN_IF                    0x00000003
#define FILE_OVERWRITE                  0x00000004
#define FILE_OVERWRITE_IF               0x00000005
#define FILE_MAXIMUM_DISPOSITION        0x00000005

//
// Define the create/open option flags
//

#define FILE_DIRECTORY_FILE                     0x00000001
#define FILE_WRITE_THROUGH                      0x00000002
#define FILE_SEQUENTIAL_ONLY                    0x00000004
#define FILE_NO_INTERMEDIATE_BUFFERING          0x00000008

#define FILE_SYNCHRONOUS_IO_ALERT               0x00000010
#define FILE_SYNCHRONOUS_IO_NONALERT            0x00000020
#define FILE_NON_DIRECTORY_FILE                 0x00000040
#define FILE_CREATE_TREE_CONNECTION             0x00000080

#define FILE_COMPLETE_IF_OPLOCKED               0x00000100
#define FILE_NO_EA_KNOWLEDGE                    0x00000200
#define FILE_OPEN_FOR_RECOVERY                  0x00000400
#define FILE_RANDOM_ACCESS                      0x00000800

#define FILE_DELETE_ON_CLOSE                    0x00001000
#define FILE_OPEN_BY_FILE_ID                    0x00002000
#define FILE_OPEN_FOR_BACKUP_INTENT             0x00004000
#define FILE_NO_COMPRESSION                     0x00008000

#define FILE_RESERVE_OPFILTER                   0x00100000
#define FILE_OPEN_REPARSE_POINT                 0x00200000
#define FILE_OPEN_NO_RECALL                     0x00400000
#define FILE_OPEN_FOR_FREE_SPACE_QUERY          0x00800000

#define FILE_COPY_STRUCTURED_STORAGE            0x00000041
#define FILE_STRUCTURED_STORAGE                 0x00000441

#define FILE_VALID_OPTION_FLAGS                 0x00ffffff
#define FILE_VALID_PIPE_OPTION_FLAGS            0x00000032
#define FILE_VALID_MAILSLOT_OPTION_FLAGS        0x00000032
#define FILE_VALID_SET_FLAGS                    0x00000036

//
// Define the I/O status information return values for NtCreateFile/NtOpenFile
//

#define FILE_SUPERSEDED                 0x00000000
#define FILE_OPENED                     0x00000001
#define FILE_CREATED                    0x00000002
#define FILE_OVERWRITTEN                0x00000003
#define FILE_EXISTS                     0x00000004
#define FILE_DOES_NOT_EXIST             0x00000005


typedef NTSTATUS
(NTAPI * PRTL_HEAP_COMMIT_ROUTINE)(
    IN PVOID Base,
    IN OUT PVOID *CommitAddress,
    IN OUT PSIZE_T CommitSize
    );

typedef struct _RTL_HEAP_PARAMETERS {
    ULONG Length;
    SIZE_T SegmentReserve;
    SIZE_T SegmentCommit;
    SIZE_T DeCommitFreeBlockThreshold;
    SIZE_T DeCommitTotalFreeThreshold;
    SIZE_T MaximumAllocationSize;
    SIZE_T VirtualMemoryThreshold;
    SIZE_T InitialCommit;
    SIZE_T InitialReserve;
    PRTL_HEAP_COMMIT_ROUTINE CommitRoutine;
    SIZE_T Reserved[ 2 ];
} RTL_HEAP_PARAMETERS, *PRTL_HEAP_PARAMETERS;

NTSYSAPI
PVOID
NTAPI
RtlCreateHeap (
    IN ULONG  Flags,
    IN PVOID  HeapBase  OPTIONAL,
    IN SIZE_T  ReserveSize  OPTIONAL,
    IN SIZE_T  CommitSize  OPTIONAL,
    IN PVOID  Lock  OPTIONAL,
    IN PRTL_HEAP_PARAMETERS  Parameters  OPTIONAL
    ); 

NTSYSAPI
PVOID
NTAPI
RtlDestroyHeap (
    IN PVOID  HeapHandle
    ); 

NTSYSAPI
PVOID
NTAPI
RtlAllocateHeap (
    IN PVOID  HeapHandle,
    IN ULONG  Flags,
    IN SIZE_T  Size
    ); 

NTSYSAPI
BOOLEAN
NTAPI
RtlFreeHeap (
    IN PVOID  HeapHandle,
    IN ULONG  Flags,
    IN PVOID  HeapBase
    ); 

// Undefine winnt.h defines.
#undef SEC_IMAGE
#undef SEC_RESERVE
#undef SEC_COMMIT

#define SEC_BASED        0x00200000
#define SEC_NO_CHANGE    0x00400000
#define SEC_IMAGE        0x01000000
#define SEC_VLM          0x02000000
#define SEC_RESERVE      0x04000000
#define SEC_COMMIT       0x08000000
#define SEC_NOCACHE      0x10000000

NTSYSAPI 
NTSTATUS
NTAPI
NtCreateSection(
  OUT PHANDLE             SectionHandle,
  IN ULONG                DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
  IN PLARGE_INTEGER       MaximumSize OPTIONAL,
  IN ULONG                PageAttributess,
  IN ULONG                SectionAttributes,
  IN HANDLE               FileHandle OPTIONAL );

NTSYSAPI 
NTSTATUS
NTAPI
ZwCreateSection(
  OUT PHANDLE             SectionHandle,
  IN ULONG                DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
  IN PLARGE_INTEGER       MaximumSize OPTIONAL,
  IN ULONG                PageAttributess,
  IN ULONG                SectionAttributes,
  IN HANDLE               FileHandle OPTIONAL );

typedef enum _SECTION_INHERIT
{
	ViewShare = 1,
	ViewUnmap = 2
} SECTION_INHERIT, *PSECTION_INHERIT;

#define MEM_DOS_LIM 0x40000000UL

NTSYSAPI
NTSTATUS
NTAPI 
  NtMapViewOfSection(
    IN HANDLE  SectionHandle,
    IN HANDLE  ProcessHandle,
    IN OUT PVOID  *BaseAddress,
    IN ULONG_PTR  ZeroBits,
    IN SIZE_T  CommitSize,
    IN OUT PLARGE_INTEGER  SectionOffset  OPTIONAL,
    IN OUT PSIZE_T  ViewSize,
    IN SECTION_INHERIT  InheritDisposition,
    IN ULONG  AllocationType,
    IN ULONG  Win32Protect
    );

NTSYSAPI
NTSTATUS
NTAPI 
  ZwMapViewOfSection(
    IN HANDLE  SectionHandle,
    IN HANDLE  ProcessHandle,
    IN OUT PVOID  *BaseAddress,
    IN ULONG_PTR  ZeroBits,
    IN SIZE_T  CommitSize,
    IN OUT PLARGE_INTEGER  SectionOffset  OPTIONAL,
    IN OUT PSIZE_T  ViewSize,
    IN SECTION_INHERIT  InheritDisposition,
    IN ULONG  AllocationType,
    IN ULONG  Win32Protect
    );

NTSYSAPI
NTSTATUS 
NTAPI
  NtUnmapViewOfSection(
    IN HANDLE  ProcessHandle,
    IN PVOID  BaseAddress
    );

NTSYSAPI
NTSTATUS 
NTAPI
  ZwUnmapViewOfSection(
    IN HANDLE  ProcessHandle,
    IN PVOID  BaseAddress
    );

typedef struct _SECTION_BASIC_INFORMATION {
  PVOID                   BaseAddress;
  ULONG                   SectionAttributes;
  LARGE_INTEGER           SectionSize;
} SECTION_BASIC_INFORMATION, *PSECTION_BASIC_INFORMATION;

typedef enum _SECTION_INFORMATION_CLASS {
	SectionBasicInformation,
	SectionImageInformation
} SECTION_INFORMATION_CLASS, *PSECTION_INFORMATION_CLASS;

NTSYSAPI 
NTSTATUS
NTAPI
NtQuerySection(
  IN HANDLE               SectionHandle,
  IN SECTION_INFORMATION_CLASS InformationClass,
  OUT PVOID               InformationBuffer,
  IN ULONG                InformationBufferSize,
  OUT PULONG              ResultLength OPTIONAL );

NTSYSAPI 
NTSTATUS
NTAPI
ZwQuerySection(
  IN HANDLE               SectionHandle,
  IN SECTION_INFORMATION_CLASS InformationClass,
  OUT PVOID               InformationBuffer,
  IN ULONG                InformationBufferSize,
  OUT PULONG              ResultLength OPTIONAL );


NTSYSAPI
NTSTATUS
NTAPI
LdrQueryImageFileExecutionOptions(
	IN PUNICODE_STRING   	 SubKey,
	IN PCWSTR  	ValueName,
	IN ULONG  	ValueSize,
	OUT PVOID  	Buffer,
	IN ULONG  	BufferSize,
	OUT PULONG RetunedLength  	OPTIONAL
	);

#define NtCurrentProcess() ( (HANDLE) -1 )  

NTSYSAPI 
NTSTATUS
NTAPI
NtCreateProcess(
  OUT PHANDLE           ProcessHandle,
  IN ACCESS_MASK        DesiredAccess,
  IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
  IN HANDLE             ParentProcess,
  IN BOOLEAN            InheritObjectTable,
  IN HANDLE             SectionHandle OPTIONAL,
  IN HANDLE             DebugPort OPTIONAL,
  IN HANDLE             ExceptionPort OPTIONAL );

NTSYSAPI 
NTSTATUS
NTAPI
ZwCreateProcess(
  OUT PHANDLE           ProcessHandle,
  IN ACCESS_MASK        DesiredAccess,
  IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
  IN HANDLE             ParentProcess,
  IN BOOLEAN            InheritObjectTable,
  IN HANDLE             SectionHandle OPTIONAL,
  IN HANDLE             DebugPort OPTIONAL,
  IN HANDLE             ExceptionPort OPTIONAL );

typedef enum _PROCESSINFOCLASS {              
    ProcessBasicInformation = 0,              
    ProcessQuotaLimits = 1,                   
    ProcessIoCounters = 2,                    
    ProcessVmCounters = 3,                    
    ProcessTimes = 4,                         
    ProcessBasePriority = 5,                  
    ProcessRaisePriority = 6,                 
    ProcessDebugPort = 7,                     
    ProcessExceptionPort = 8,                 
    ProcessAccessToken = 9,                   
    ProcessLdtInformation = 10,               
    ProcessLdtSize = 11,                      
    ProcessDefaultHardErrorMode = 12,         
    ProcessIoPortHandlers = 13,               
    ProcessPooledUsageAndLimits = 14,         
    ProcessWorkingSetWatch = 15,              
    ProcessUserModeIOPL = 16,                 
    ProcessEnableAlignmentFaultFixup = 17,    
    ProcessPriorityClass = 18,                
    ProcessWx86Information = 19,              
    ProcessHandleCount = 20,                  
    ProcessAffinityMask = 21,                 
    ProcessPriorityBoost = 22,                
    ProcessDeviceMap = 23,                    
    ProcessSessionInformation = 24,           
    ProcessForegroundInformation = 25,        
    ProcessWow64Information = 26,             
    ProcessImageFileName = 27,                
    ProcessLUIDDeviceMapsEnabled = 28,        
    ProcessBreakOnTermination = 29,           
    ProcessDebugObjectHandle = 30,            
    ProcessDebugFlags = 31,                   
    ProcessHandleTracing = 32,   
    ProcessUnknown33 = 33,
    ProcessExecuteFlags = 34,
    MaxProcessInfoClass                       
} PROCESSINFOCLASS, PROCESS_INFORMATION_CLASS;

NTSYSAPI 
NTSTATUS
NTAPI
NtSetInformationProcess(
  IN HANDLE               ProcessHandle,
  IN PROCESS_INFORMATION_CLASS ProcessInformationClass,
  IN PVOID                ProcessInformation,
  IN ULONG                ProcessInformationLength );

NTSYSAPI 
NTSTATUS
NTAPI
ZwSetInformationProcess(
  IN HANDLE               ProcessHandle,
  IN PROCESS_INFORMATION_CLASS ProcessInformationClass,
  IN PVOID                ProcessInformation,
  IN ULONG                ProcessInformationLength );


NTSYSAPI 
NTSTATUS
NTAPI
NtQueryInformationProcess(
  IN HANDLE               ProcessHandle,
  IN PROCESS_INFORMATION_CLASS ProcessInformationClass,
  OUT PVOID               ProcessInformation,
  IN ULONG                ProcessInformationLength,
  OUT PULONG              ReturnLength );

NTSYSAPI 
NTSTATUS
NTAPI
ZwQueryInformationProcess(
  IN HANDLE               ProcessHandle,
  IN PROCESS_INFORMATION_CLASS ProcessInformationClass,
  OUT PVOID               ProcessInformation,
  IN ULONG                ProcessInformationLength,
  OUT PULONG              ReturnLength );


#define DUPLICATE_SAME_ATTRIBUTES 0x00000004

NTSYSAPI 
NTSTATUS
NTAPI
NtDuplicateObject(
  IN HANDLE               SourceProcessHandle,
  IN HANDLE               SourceHandle,
  IN HANDLE               TargetProcessHandle,
  OUT PHANDLE             TargetHandle,
  IN ACCESS_MASK          DesiredAccess OPTIONAL,
  IN BOOLEAN              InheritHandle,
  IN ULONG                Options );

NTSYSAPI 
NTSTATUS
NTAPI
ZwDuplicateObject(
  IN HANDLE               SourceProcessHandle,
  IN HANDLE               SourceHandle,
  IN HANDLE               TargetProcessHandle,
  OUT PHANDLE             TargetHandle,
  IN ACCESS_MASK          DesiredAccess OPTIONAL,
  IN BOOLEAN              InheritHandle,
  IN ULONG                Options );

NTSYSAPI 
NTSTATUS
NTAPI
NtReadVirtualMemory(
  IN HANDLE               ProcessHandle,
  IN PVOID                BaseAddress,
  OUT PVOID               Buffer,
  IN ULONG                NumberOfBytesToRead,
  OUT PULONG              NumberOfBytesReaded OPTIONAL );

NTSYSAPI 
NTSTATUS
NTAPI
ZwReadVirtualMemory(
  IN HANDLE               ProcessHandle,
  IN PVOID                BaseAddress,
  OUT PVOID               Buffer,
  IN ULONG                NumberOfBytesToRead,
  OUT PULONG              NumberOfBytesReaded OPTIONAL );

NTSYSAPI 
NTSTATUS
NTAPI
NtWriteVirtualMemory(
  IN HANDLE               ProcessHandle,
  IN PVOID                BaseAddress,
  IN PVOID                Buffer,
  IN ULONG                NumberOfBytesToWrite,
  OUT PULONG              NumberOfBytesWritten OPTIONAL );

NTSYSAPI 
NTSTATUS
NTAPI
ZwWriteVirtualMemory(
  IN HANDLE               ProcessHandle,
  IN PVOID                BaseAddress,
  IN PVOID                Buffer,
  IN ULONG                NumberOfBytesToWrite,
  OUT PULONG              NumberOfBytesWritten OPTIONAL );

#define CONSOLE_HANDLE(h) ( ((h)==(HANDLE)3) || ((h)==(HANDLE)7) || ((h)==(HANDLE)0xb) )

NTSYSAPI
PIMAGE_NT_HEADERS
NTAPI
RtlImageNtHeader(
	PVOID	ImageBase
	);

NTSYSAPI
PVOID 
NTAPI 
RtlImageDirectoryEntryToData(
	PVOID pBase, 
	BOOLEAN bMappedAsImage, 
	USHORT directoryEntry, 
	PULONG size
	);

#define ROUND_UP(x, a) (((x) + (a) - 1) & ~((a) - 1))

NTSYSAPI 
NTSTATUS
NTAPI
NtAllocateVirtualMemory(
  IN HANDLE               ProcessHandle,
  IN OUT PVOID            *BaseAddress,
  IN ULONG                ZeroBits,
  IN OUT PULONG           RegionSize,
  IN ULONG                AllocationType,
  IN ULONG                Protect );

NTSYSAPI 
NTSTATUS
NTAPI
NtFreeVirtualMemory(
  IN HANDLE               ProcessHandle,
  IN PVOID                *BaseAddress,
  IN OUT PULONG           RegionSize,
  IN ULONG                FreeType );

NTSYSAPI 
NTSTATUS
NTAPI
NtProtectVirtualMemory(
  IN HANDLE               ProcessHandle,
  IN OUT PVOID            *BaseAddress,
  IN OUT PULONG           NumberOfBytesToProtect,
  IN ULONG                NewAccessProtection,
  OUT PULONG              OldAccessProtection );

NTSYSAPI 
NTSTATUS
NTAPI
ZwAllocateVirtualMemory(
  IN HANDLE               ProcessHandle,
  IN OUT PVOID            *BaseAddress,
  IN ULONG                ZeroBits,
  IN OUT PULONG           RegionSize,
  IN ULONG                AllocationType,
  IN ULONG                Protect );

NTSYSAPI 
NTSTATUS
NTAPI
ZwFreeVirtualMemory(
  IN HANDLE               ProcessHandle,
  IN PVOID                *BaseAddress,
  IN OUT PULONG           RegionSize,
  IN ULONG                FreeType );

NTSYSAPI 
NTSTATUS
NTAPI
ZwProtectVirtualMemory(
  IN HANDLE               ProcessHandle,
  IN OUT PVOID            *BaseAddress,
  IN OUT PULONG           NumberOfBytesToProtect,
  IN ULONG                NewAccessProtection,
  OUT PULONG              OldAccessProtection );

#define	 KGDT_NULL		0
#define	 KGDT_R0_CODE   8
#define	 KGDT_R0_DATA   16
#define  KGDT_R3_CODE   24
#define  KGDT_R3_DATA   32
#define  KGDT_TSS       40
#define  KGDT_R0_PCR    48
#define  KGDT_R3_TEB    56
#define  KGDT_VDM_TILE  64
#define  KGDT_LDT       72
#define  KGDT_DF_TSS    80
#define  KGDT_NMI_TSS   88

typedef struct _CSR_CAPTURE_HEADER {
    ULONG Length;
    struct _CSR_CAPTURE_HEADER* RelatedCaptureBuffer;
    ULONG CountMessagePointers;
    PCHAR FreeSpace;
    ULONG MessagePointerOffsets[1]; // Offsets within CSR_API_MSG of pointers
} CSR_CAPTURE_HEADER, *PCSR_CAPTURE_HEADER;

typedef struct _CSR_API_CONNECTINFO {
    OUT HANDLE ObjectDirectory;
    OUT PVOID SharedSectionBase;
    OUT PVOID SharedStaticServerData;
    OUT PVOID SharedSectionHeap;
    OUT ULONG DebugFlags;
    OUT ULONG SizeOfPebData;
    OUT ULONG SizeOfTebData;
    OUT ULONG NumberOfServerDllNames;
    OUT HANDLE ServerProcessId;
} CSR_API_CONNECTINFO, *PCSR_API_CONNECTINFO;

#define LPC_CLIENT_ID CLIENT_ID
#define LPC_SIZE_T SIZE_T
#define LPC_PVOID PVOID
#define LPC_HANDLE HANDLE

//                                
// LPC Port Message               
//                              

typedef struct _PORT_MESSAGE      
{                                 
    union                         
    {                             
        struct                    
        {                         
            USHORT DataLength;    
            USHORT TotalLength;   
        } s1;                     
        ULONG Length;             
    } u1;                         
    union                         
    {                             
        struct                    
        {                         
            USHORT Type;          
            USHORT DataInfoOffset;
        } s2;                     
        ULONG ZeroInit;           
    } u2;                         
    union                         
    {                             
        LPC_CLIENT_ID ClientId;   
        double DoNotUseThisField; 
    };                            
    ULONG MessageId;              
    union                         
    {                             
        LPC_SIZE_T ClientViewSize;
        ULONG CallbackId;         
    };                            
} PORT_MESSAGE, *PPORT_MESSAGE;   

typedef ULONG CSR_API_NUMBER;

//
// Message format for messages sent from the client to the server
//

typedef struct _CSR_CLIENTCONNECT_MSG {
    IN ULONG ServerDllIndex;
    IN OUT PVOID ConnectionInformation;
    IN OUT ULONG ConnectionInformationLength;
} CSR_CLIENTCONNECT_MSG, *PCSR_CLIENTCONNECT_MSG;

typedef struct _CSR_API_MSG {
    PORT_MESSAGE h;
    union {
        CSR_API_CONNECTINFO ConnectionRequest;
        struct {
            PCSR_CAPTURE_HEADER CaptureBuffer;
            CSR_API_NUMBER ApiNumber;
            ULONG ReturnValue;
            ULONG Reserved;
            union {
                CSR_CLIENTCONNECT_MSG ClientConnect;
                ULONG ApiMessageData[39];
            } u;
        };
    };
} CSR_API_MSG, *PCSR_API_MSG;

typedef struct _BASE_CREATEPROCESS_MSG {
	HANDLE ProcessHandle;
	HANDLE ThreadHandle;
	CLIENT_ID ClientId;
	ULONG Machine;
	CLIENT_ID DebuggerClientId;
	ULONG CreationFlags;
	ULONG VdmBinaryType;
} BASE_CREATEPROCESS_MSG, *PBASE_CREATEPROCESS_MSG;

typedef struct _BASE_CREATETHREAD_MSG {
	HANDLE ThreadHandle;
	CLIENT_ID ClientId;
} BASE_CREATETHREAD_MSG, *PBASE_CREATETHREAD_MSG;

typedef struct _BASE_API_MSG {
    PORT_MESSAGE h;
    union {
        CSR_API_CONNECTINFO ConnectionRequest;
        struct {
            PCSR_CAPTURE_HEADER CaptureBuffer;
            CSR_API_NUMBER ApiNumber;
            ULONG ReturnValue;
            ULONG Reserved;
            union {
                CSR_CLIENTCONNECT_MSG	ClientConnect;
				BASE_CREATEPROCESS_MSG	CreateProcess;
				BASE_CREATETHREAD_MSG   CreateThread;
                ULONG ApiMessageData[0x98];
            } u;
        };
    };
} BASE_API_MSG, *PBASE_API_MSG;

#define CSRSRV_SERVERDLL_INDEX          0
#define CSRSRV_FIRST_API_NUMBER         0

#define BASESRV_SERVERDLL_INDEX         1
#define BASESRV_FIRST_API_NUMBER        0

#define CONSRV_SERVERDLL_INDEX          2
#define CONSRV_FIRST_API_NUMBER         512

#define USERSRV_SERVERDLL_INDEX         3
#define USERSRV_FIRST_API_NUMBER        1024

#define BasepCreateProcess 0
#define BasepCreateThread  1

#define CSR_MAKE_API_NUMBER( DllIndex, ApiIndex ) \
    (CSR_API_NUMBER)(((DllIndex) << 16) | (ApiIndex))

#define CSR_APINUMBER_TO_SERVERDLLINDEX( ApiNumber ) \
    ((ULONG)((ULONG)(ApiNumber) >> 16))

#define CSR_APINUMBER_TO_APITABLEINDEX( ApiNumber ) \
    ((ULONG)((USHORT)(ApiNumber)))


typedef CLIENT_ID *PCLIENT_ID;

NTSYSAPI 
NTSTATUS
NTAPI
NtCreateThread(
  OUT PHANDLE             ThreadHandle,
  IN ACCESS_MASK          DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
  IN HANDLE               ProcessHandle,
  OUT PCLIENT_ID          ClientId,
  IN PCONTEXT             ThreadContext,
  IN PINITIAL_TEB         InitialTeb,
  IN BOOLEAN              CreateSuspended );

NTSYSAPI 
NTSTATUS
NTAPI
ZwCreateThread(
  OUT PHANDLE             ThreadHandle,
  IN ACCESS_MASK          DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
  IN HANDLE               ProcessHandle,
  OUT PCLIENT_ID          ClientId,
  IN PCONTEXT             ThreadContext,
  IN PINITIAL_TEB         InitialTeb,
  IN BOOLEAN              CreateSuspended );

NTSYSAPI
NTSTATUS
NTAPI
NtRequestWaitReplyPort(
	IN	HANDLE	PortHandle,
	IN	PVOID	InputBuffer,
	IN	PVOID	OutputBuffer
	);

//
// Local and Remove Port Views
//
typedef struct _PORT_VIEW
{
    ULONG Length;
    LPC_HANDLE SectionHandle;
    ULONG SectionOffset;
    LPC_SIZE_T ViewSize;
    LPC_PVOID ViewBase;
    LPC_PVOID ViewRemoteBase;
} PORT_VIEW, *PPORT_VIEW;

typedef struct _REMOTE_PORT_VIEW
{
    ULONG Length;
    LPC_SIZE_T ViewSize;
    LPC_PVOID ViewBase;
} REMOTE_PORT_VIEW, *PREMOTE_PORT_VIEW;

typedef struct _CSR_CONNECTION_INFO
{
	ULONG 	Unknown [2];
	HANDLE 	ObjectDirectory;
	PVOID 	SharedSectionBase;
	PVOID 	SharedSectionHeap;
	PVOID 	SharedSectionData;
	ULONG 	DebugFlags;
	ULONG 	Unknown2 [3];
	HANDLE 	ProcessId;
	ULONG 	Version;
} CSR_CONNECTION_INFO, *PCSR_CONNECTION_INFO;

#define RtlGetProcessHeap GetProcessHeap

NTSYSAPI
NTSTATUS
NTAPI
RtlAllocateAndInitializeSid (
	PSID_IDENTIFIER_AUTHORITY   	 IdentifierAuthority,
	UCHAR  	SubAuthorityCount,
	ULONG  	SubAuthority0,
	ULONG  	SubAuthority1,
	ULONG  	SubAuthority2,
	ULONG  	SubAuthority3,
	ULONG  	SubAuthority4,
	ULONG  	SubAuthority5,
	ULONG  	SubAuthority6,
	ULONG  	SubAuthority7,
	PSID *  	Sid
	);

NTSYSAPI
NTSTATUS
NTAPI
NtSecureConnectPort (
	OUT PHANDLE   	 ConnectedPort,
	IN PUNICODE_STRING  	PortName,
	IN PSECURITY_QUALITY_OF_SERVICE  	Qos,
	IN OUT PPORT_VIEW WriteMap  	OPTIONAL,
	IN PSID ServerSid  	OPTIONAL,
	IN OUT PREMOTE_PORT_VIEW ReadMap  	OPTIONAL,
	OUT PULONG MaxMessageSize  	OPTIONAL,
	IN OUT PVOID ConnectInfo  	OPTIONAL,
	IN OUT PULONG UserConnectInfoLength  	OPTIONAL
	);

NTSYSAPI
NTSTATUS
NTAPI
ZwSecureConnectPort (
	OUT PHANDLE   	 ConnectedPort,
	IN PUNICODE_STRING  	PortName,
	IN PSECURITY_QUALITY_OF_SERVICE  	Qos,
	IN OUT PPORT_VIEW WriteMap  	OPTIONAL,
	IN PSID ServerSid  	OPTIONAL,
	IN OUT PREMOTE_PORT_VIEW ReadMap  	OPTIONAL,
	OUT PULONG MaxMessageSize  	OPTIONAL,
	IN OUT PVOID ConnectInfo  	OPTIONAL,
	IN OUT PULONG UserConnectInfoLength  	OPTIONAL
	);

NTSYSAPI
NTSTATUS
NTAPI
  ZwOpenSymbolicLinkObject(
    OUT PHANDLE  LinkHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes
    );

NTSYSAPI
NTSTATUS
NTAPI
  ZwQuerySymbolicLinkObject(
    IN HANDLE LinkHandle,
    IN OUT PUNICODE_STRING LinkTarget,
    OUT PULONG ReturnedLength OPTIONAL
    );

NTSYSAPI 
NTSTATUS
NTAPI
NtTerminateProcess(
  IN HANDLE               ProcessHandle OPTIONAL,
  IN NTSTATUS             ExitStatus );

NTSYSAPI 
NTSTATUS
NTAPI
ZwTerminateThread(
  IN HANDLE               ProcessHandle OPTIONAL,
  IN NTSTATUS             ExitStatus );

NTSYSAPI 
NTSTATUS
NTAPI
ZwClose(
  IN HANDLE               Handle);



NTSYSAPI 
NTSTATUS
NTAPI
NtResumeThread(
  IN HANDLE               ThreadHandle,
  OUT PULONG              SuspendCount OPTIONAL );

NTSYSAPI 
NTSTATUS
NTAPI
ZwResumeThread(
  IN HANDLE               ThreadHandle,
  OUT PULONG              SuspendCount OPTIONAL );


typedef enum _LPC_TYPE {
    LPC_NEW_MESSAGE,
    LPC_REQUEST,
    LPC_REPLY,
    LPC_DATAGRAM,
    LPC_LOST_REPLY,
    LPC_PORT_CLOSED,
    LPC_CLIENT_DIED,
    LPC_EXCEPTION,
    LPC_DEBUG_EVENT,
    LPC_ERROR_EVENT,
    LPC_CONNECTION_REQUEST
} LPC_TYPE;

typedef struct _LPC_MESSAGE {
    USHORT      DataSize;
    USHORT      MessageSize;
    USHORT      MessageType;
    USHORT      VirtualRangesOffset;
    CLIENT_ID   ClientId;
    ULONG       MessageId;
    ULONG       SectionSize;
    UCHAR       Data[1];
} LPC_MESSAGE, *PLPC_MESSAGE;

typedef struct _LPC_SECTION_READ {
    ULONG Length;
    ULONG ViewSize;
    PVOID ViewBase;
} LPC_SECTION_READ, *PLPC_SECTION_READ;

typedef struct _LPC_SECTION_WRITE {
    ULONG   Length;
    HANDLE  SectionHandle;
    ULONG   SectionOffset;
    ULONG   ViewSize;
    PVOID   ViewBase;
    PVOID   TargetViewBase;
} LPC_SECTION_WRITE, *PLPC_SECTION_WRITE;


NTSYSAPI 
NTSTATUS
NTAPI
ZwCreatePort(
  OUT PHANDLE             PortHandle,
  IN POBJECT_ATTRIBUTES   ObjectAttributes,
  IN ULONG                MaxConnectInfoLength,
  IN ULONG                MaxDataLength,
  IN OUT PULONG           Reserved OPTIONAL );


NTSYSAPI
NTSTATUS
NTAPI
ZwConnectPort (
    OUT PHANDLE                     ClientPortHandle,
    IN PUNICODE_STRING              ServerPortName,
    IN PSECURITY_QUALITY_OF_SERVICE SecurityQos,
    IN OUT PLPC_SECTION_WRITE       ClientSharedMemory OPTIONAL,
    IN OUT PLPC_SECTION_READ        ServerSharedMemory OPTIONAL,
    OUT PULONG                      MaximumMessageLength OPTIONAL,
    IN OUT PVOID                    ConnectionInfo OPTIONAL,
    IN OUT PULONG                   ConnectionInfoLength OPTIONAL
);

typedef struct _LPC_SECTION_MEMORY {
	ULONG                   Length;
	ULONG                   ViewSize;
	PVOID                   ViewBase;
} LPC_SECTION_MEMORY, *PLPC_SECTION_MEMORY;


typedef struct _LPC_SECTION_OWNER_MEMORY {
	ULONG                   Length;
	HANDLE                  SectionHandle;
	ULONG                   OffsetInSection;
	ULONG                   ViewSize;
	PVOID                   ViewBase;
	PVOID                   OtherSideViewBase;
} LPC_SECTION_OWNER_MEMORY, *PLPC_SECTION_OWNER_MEMORY;


NTSYSAPI 
NTSTATUS
NTAPI
ZwAcceptConnectPort(
	OUT PHANDLE             ServerPortHandle,
	IN HANDLE               AlternativeReceivePortHandle OPTIONAL,
	IN PLPC_MESSAGE         ConnectionReply,
	IN BOOLEAN              AcceptConnection,
	IN OUT PLPC_SECTION_OWNER_MEMORY ServerSharedMemory OPTIONAL,
	OUT PLPC_SECTION_MEMORY ClientSharedMemory OPTIONAL );

NTSYSAPI 
NTSTATUS
NTAPI
ZwCompleteConnectPort(
  IN HANDLE               PortHandle );

NTSYSAPI 
NTSTATUS
NTAPI
ZwReplyPort(
  IN HANDLE               PortHandle,
  IN PLPC_MESSAGE         Reply );

NTSYSAPI 
NTSTATUS
NTAPI
ZwReplyWaitReplyPort(
  IN HANDLE               PortHandle,
  IN OUT PLPC_MESSAGE     Reply );

NTSYSAPI 
NTSTATUS
NTAPI
ZwReplyWaitReceivePort(
  IN HANDLE               PortHandle,
  OUT PHANDLE             ReceivePortHandle OPTIONAL,
  IN PLPC_MESSAGE         Reply OPTIONAL,
  OUT PLPC_MESSAGE        IncomingRequest );

NTSYSAPI 
NTSTATUS
NTAPI
ZwRequestPort(
  IN HANDLE               PortHandle,
  IN PLPC_MESSAGE         Request );

NTSYSAPI 
NTSTATUS
NTAPI
ZwRequestWaitReplyPort(
  IN HANDLE               PortHandle,
  IN PLPC_MESSAGE         Request,
  OUT PLPC_MESSAGE        IncomingReply );

NTSYSAPI 
NTSTATUS
NTAPI
ZwRegisterThreadTerminatePort(
  IN HANDLE               PortHandle );

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemBasicInformation,
    SystemProcessorInformation,
    SystemPerformanceInformation,
    SystemTimeOfDayInformation,
    SystemNotImplemented1,
    SystemProcessesAndThreadsInformation,
    SystemCallCounts,
    SystemConfigurationInformation,
    SystemProcessorTimes,
    SystemGlobalFlag,
    SystemNotImplemented2,
    SystemModuleInformation,
    SystemLockInformation,
    SystemNotImplemented3,
    SystemNotImplemented4,
    SystemNotImplemented5,
    SystemHandleInformation,
    SystemObjectInformation,
    SystemPagefileInformation,
    SystemInstructionEmulationCounts,
    SystemInvalidInfoClass1,
    SystemCacheInformation,
    SystemPoolTagInformation,
    SystemProcessorStatistics,
    SystemDpcInformation,
    SystemNotImplemented6,
    SystemLoadImage,
    SystemUnloadImage,
    SystemTimeAdjustment,
    SystemNotImplemented7,
    SystemNotImplemented8,
    SystemNotImplemented9,
    SystemCrashDumpInformation,
    SystemExceptionInformation,
    SystemCrashDumpStateInformation,
    SystemKernelDebuggerInformation,
    SystemContextSwitchInformation,
    SystemRegistryQuotaInformation,
    SystemLoadAndCallImage,
    SystemPrioritySeparation,
    SystemNotImplemented10,
    SystemNotImplemented11,
    SystemInvalidInfoClass2,
    SystemInvalidInfoClass3,
    SystemTimeZoneInformation,
    SystemLookasideInformation,
    SystemSetTimeSlipEvent,
    SystemCreateSession,
    SystemDeleteSession,
    SystemInvalidInfoClass4,
    SystemRangeStartInformation,
    SystemVerifierInformation,
    SystemAddVerifier,
    SystemSessionProcessesInformation
} SYSTEM_INFORMATION_CLASS;

/*
typedef struct _IO_COUNTERS {
    ULONGLONG  ReadOperationCount;
    ULONGLONG  WriteOperationCount;
    ULONGLONG  OtherOperationCount;
    ULONGLONG ReadTransferCount;
    ULONGLONG WriteTransferCount;
    ULONGLONG OtherTransferCount;
} IO_COUNTERS;
typedef IO_COUNTERS *PIO_COUNTERS;*/

typedef struct _VM_COUNTERS {
    SIZE_T PeakVirtualSize;
    SIZE_T VirtualSize;
    ULONG PageFaultCount;
    SIZE_T PeakWorkingSetSize;
    SIZE_T WorkingSetSize;
    SIZE_T QuotaPeakPagedPoolUsage;
    SIZE_T QuotaPagedPoolUsage;
    SIZE_T QuotaPeakNonPagedPoolUsage;
    SIZE_T QuotaNonPagedPoolUsage;
    SIZE_T PagefileUsage;
    SIZE_T PeakPagefileUsage;
} VM_COUNTERS;
typedef VM_COUNTERS *PVM_COUNTERS;

typedef struct _SYSTEM_THREADS_INFORMATION {
    LARGE_INTEGER   KernelTime;
    LARGE_INTEGER   UserTime;
    LARGE_INTEGER   CreateTime;
    ULONG           WaitTime;
    PVOID           StartAddress;
    CLIENT_ID       ClientId;
    KPRIORITY       Priority;
    KPRIORITY       BasePriority;
    ULONG           ContextSwitchCount;
    ULONG    State;
    ULONG    WaitReason;
} SYSTEM_THREADS_INFORMATION, *PSYSTEM_THREADS_INFORMATION;

// SystemProcessesAndThreadsInformation
typedef struct _SYSTEM_PROCESSES_INFORMATION {
    ULONG                       NextEntryDelta;
    ULONG                       ThreadCount;
    ULONG                       Reserved1[6];
    LARGE_INTEGER               CreateTime;
    LARGE_INTEGER               UserTime;
    LARGE_INTEGER               KernelTime;
    UNICODE_STRING              ProcessName;
    KPRIORITY                   BasePriority;
    ULONG                       ProcessId;
    ULONG                       InheritedFromProcessId;
    ULONG                       HandleCount;
    ULONG                       SessionId;
    ULONG                       Reserved2;
    VM_COUNTERS                 VmCounters;
    IO_COUNTERS                 IoCounters;
    SYSTEM_THREADS_INFORMATION  Threads[1];
} SYSTEM_PROCESSES_INFORMATION, *PSYSTEM_PROCESSES_INFORMATION;


NTSYSAPI
NTSTATUS
NTAPI
ZwQuerySystemInformation(
  SYSTEM_INFORMATION_CLASS SystemInformationClass,
  PVOID SystemInformation,
  ULONG SystemInformationLength,
  PULONG ReturnLength
);

// SystemHandleInformation
typedef struct _SYSTEM_HANDLE {
    ULONG       ProcessId;
    UCHAR       ObjectTypeNumber;
    UCHAR       Flags;
    USHORT      Handle;
    PVOID       Object;
    ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, *PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION {
	ULONG HandleCount;
	SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

//
// Thread Information Classes
//

typedef enum _THREADINFOCLASS {
    ThreadBasicInformation,
    ThreadTimes,
    ThreadPriority,
    ThreadBasePriority,
    ThreadAffinityMask,
    ThreadImpersonationToken,
    ThreadDescriptorTableEntry,
    ThreadEnableAlignmentFaultFixup,
    ThreadEventPair_Reusable,
    ThreadQuerySetWin32StartAddress,
    ThreadZeroTlsCell,
    ThreadPerformanceCount,
    ThreadAmILastThread,
    ThreadIdealProcessor,
    ThreadPriorityBoost,
    ThreadSetTlsArrayAddress,
    ThreadIsIoPending,
    ThreadHideFromDebugger,
    ThreadBreakOnTermination,
    MaxThreadInfoClass
    } THREADINFOCLASS;
//
// Process Information Structures
//

//
// PageFaultHistory Information
//  NtQueryInformationProcess using ProcessWorkingSetWatch
//
typedef struct _PROCESS_WS_WATCH_INFORMATION {
    PVOID FaultingPc;
    PVOID FaultingVa;
} PROCESS_WS_WATCH_INFORMATION, *PPROCESS_WS_WATCH_INFORMATION;

//
// Basic Process Information
//  NtQueryInformationProcess using ProcessBasicInfo
//

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationProcess(
  HANDLE ProcessHandle,
  PROCESSINFOCLASS ProcessInformationClass,
  PVOID ProcessInformation,
  ULONG ProcessInformationLength,
  PULONG ReturnLength
);

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryInformationThread (
    IN HANDLE           ThreadHandle,
    IN THREADINFOCLASS  ThreadInformationClass,
    OUT PVOID           ThreadInformation,
    IN ULONG            ThreadInformationLength,
    OUT PULONG          ReturnLength OPTIONAL
);

typedef enum _OBJECT_INFORMATION_CLASS
{
	ObjectBasicInformation,			// Result is OBJECT_BASIC_INFORMATION structure
	ObjectNameInformation,			// Result is OBJECT_NAME_INFORMATION structure
	ObjectTypeInformation,			// Result is OBJECT_TYPE_INFORMATION structure
	ObjectAllInformation,			// Result is OBJECT_ALL_INFORMATION structure
	ObjectDataInformation			// Result is OBJECT_DATA_INFORMATION structure	
} OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryObject (
    IN HANDLE               ObjectHandle,
    IN OBJECT_INFORMATION_CLASS    ObjectInformationClass,
    OUT PVOID               ObjectInformation,
    IN ULONG                Length,
    OUT PULONG              ResultLength
);

typedef struct _OBJECT_BASIC_INFO {
    ULONG           Attributes;
    ACCESS_MASK     GrantedAccess;
    ULONG           HandleCount;
    ULONG           ReferenceCount;
    ULONG           PagedPoolUsage;
    ULONG           NonPagedPoolUsage;
    ULONG           Reserved[3];
    ULONG           NameInformationLength;
    ULONG           TypeInformationLength;
    ULONG           SecurityDescriptorLength;
    LARGE_INTEGER   CreateTime;
} OBJECT_BASIC_INFO, *POBJECT_BASIC_INFO;

typedef struct _OBJECT_NAME_INFO {
    UNICODE_STRING  ObjectName;
    WCHAR           ObjectNameBuffer[1];
} OBJECT_NAME_INFO, *POBJECT_NAME_INFO;

typedef struct _OBJECT_TYPE_INFO {
    UNICODE_STRING  ObjectTypeName;
    UCHAR           Unknown[0x58];
    WCHAR           ObjectTypeNameBuffer[1];
} OBJECT_TYPE_INFO, *POBJECT_TYPE_INFO;

typedef struct _OBJECT_PROTECTION_INFO {
    BOOLEAN Inherit;
    BOOLEAN ProtectHandle;
} OBJECT_PROTECTION_INFO, *POBJECT_PROTECTION_INFO;


#define PAGE_SHIFT 12
#define PAGE_SIZE 0x1000

NTSYSAPI 
NTSTATUS
NTAPI
ZwContinue(
  IN PCONTEXT             ThreadContext,
  IN BOOLEAN              RaiseAlert );

NTSYSAPI 
NTSTATUS
NTAPI
NtCreateSymbolicLinkObject (
  OUT PHANDLE             pHandle,
  IN ACCESS_MASK          DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes,
  IN PUNICODE_STRING      DestinationName );

NTSYSAPI 
NTSTATUS
NTAPI
ZwCreateSymbolicLinkObject (
  OUT PHANDLE             pHandle,
  IN ACCESS_MASK          DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes,
  IN PUNICODE_STRING      DestinationName );

typedef struct SYSTEM_OBJECT_TYPE_INFORMATION
{
	ULONG	NextEntryOffset;
	ULONG	ObjectCount;
	ULONG	HandleCount;
	ULONG	TypeNumber;
	ULONG	InvalidAttributes;
	ULONG   GenericMapping;
	ULONG   ValidAccessMask;
	DWORD   PoolType;
	UCHAR	Unknown;
	UCHAR	XpPadding[15];
	UNICODE_STRING Name;

} *PSYSTEM_OBJECT_TYPE_INFORMATION;

typedef struct _SYSTEM_MODULE {
  ULONG                Reserved1;
  ULONG                Reserved2;
  PVOID                ImageBaseAddress;
  ULONG                ImageSize;
  ULONG                Flags;
  WORD                 Id;
  WORD                 Rank;
  WORD                 w018;
  WORD                 NameOffset;
  BYTE                 Name[256];
} SYSTEM_MODULE, *PSYSTEM_MODULE;


typedef struct _SYSTEM_MODULE_INFORMATION {
  ULONG                ModulesCount;
#pragma warning(disable: 4200)
  SYSTEM_MODULE        Modules[0];
#pragma warning(default: 4200)
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

NTSYSAPI 
NTSTATUS
NTAPI
NtCreateToken(
  OUT PHANDLE             TokenHandle,
  IN ACCESS_MASK          DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes,
  IN TOKEN_TYPE           TokenType,
  IN PLUID                AuthenticationId,
  IN PLARGE_INTEGER       ExpirationTime,
  IN PTOKEN_USER          TokenUser,
  IN PTOKEN_GROUPS        TokenGroups,
  IN PTOKEN_PRIVILEGES    TokenPrivileges,
  IN PTOKEN_OWNER         TokenOwner,
  IN PTOKEN_PRIMARY_GROUP TokenPrimaryGroup,
  IN PTOKEN_DEFAULT_DACL  TokenDefaultDacl,
  IN PTOKEN_SOURCE        TokenSource );

NTSYSAPI 
NTSTATUS
NTAPI
NtAllocateLocallyUniqueId(
  OUT PLUID               LocallyUniqueId );

typedef enum _HARDERROR_RESPONSE_OPTION {
    OptionAbortRetryIgnore,
    OptionOk,
    OptionOkCancel,
    OptionRetryCancel,
    OptionYesNo,
    OptionYesNoCancel,
    OptionShutdownSystem,
    OptionOkNoWait,
    OptionCancelTryContinue
} HARDERROR_RESPONSE_OPTION, *PHARDERROR_RESPONSE_OPTION;

typedef enum _HARDERROR_RESPONSE {
    ResponseReturnToCaller,
    ResponseNotHandled,
    ResponseAbort,
    ResponseCancel,
    ResponseIgnore,
    ResponseNo,
    ResponseOk,
    ResponseRetry,
    ResponseYes,
    ResponseTryAgain,
    ResponseContinue
} HARDERROR_RESPONSE, *PHARDERROR_RESPONSE;


NTSYSAPI 
NTSTATUS
NTAPI
ZwOpenProcessToken(
  IN HANDLE               ProcessHandle,
  IN ACCESS_MASK          DesiredAccess,
  OUT PHANDLE             TokenHandle );

/*
typedef enum _TOKEN_INFORMATION_CLASS {
    TokenUser = 1,
    TokenGroups,
    TokenPrivileges,
    TokenOwner,
    TokenPrimaryGroup,
    TokenDefaultDacl,
    TokenSource,
    TokenType,
    TokenImpersonationLevel,
    TokenStatistics,
    TokenRestrictedSids
} TOKEN_INFORMATION_CLASS;
*/

NTSYSAPI 
NTSTATUS
NTAPI
ZwQueryInformationToken(
  IN HANDLE               TokenHandle,
  IN TOKEN_INFORMATION_CLASS TokenInformationClass,
  OUT PVOID               TokenInformation,
  IN ULONG                TokenInformationLength,
  OUT PULONG              ReturnLength );

NTSYSAPI 
NTSTATUS
NTAPI
ZwSetInformationToken(
  IN HANDLE               TokenHandle,
  IN TOKEN_INFORMATION_CLASS TokenInformationClass,
  OUT PVOID               TokenInformation,
  IN ULONG                TokenInformationLength );

NTSYSAPI
NTSTATUS
NTAPI
ZwLoadDriver(
	IN PUNICODE_STRING RegistryPath
	);

NTSYSAPI
NTSTATUS
NTAPI
ZwUnloadDriver(
	IN PUNICODE_STRING RegistryPath
	);

typedef struct _FILE_NAME_INFORMATION {
  ULONG  FileNameLength;
  WCHAR  FileName[1];
} FILE_NAME_INFORMATION, *PFILE_NAME_INFORMATION;

NTSYSAPI
NTSTATUS
NTAPI
  ZwCreateFile(
    OUT PHANDLE  FileHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN PLARGE_INTEGER  AllocationSize  OPTIONAL,
    IN ULONG  FileAttributes,
    IN ULONG  ShareAccess,
    IN ULONG  CreateDisposition,
    IN ULONG  CreateOptions,
    IN PVOID  EaBuffer  OPTIONAL,
    IN ULONG  EaLength
    );

NTSYSAPI
NTSTATUS
NTAPI
  ZwDeviceIoControlFile(
    IN HANDLE  FileHandle,
    IN HANDLE  Event,
    IN PVOID  ApcRoutine,
    IN PVOID  ApcContext,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN ULONG  IoControlCode,
    IN PVOID  InputBuffer,
    IN ULONG  InputBufferLength,
    OUT PVOID  OutputBuffer,
    IN ULONG  OutputBufferLength
    ); 

NTSYSAPI 
NTSTATUS
NTAPI
NtQueryInformationFile(
  IN HANDLE               FileHandle,
  OUT PIO_STATUS_BLOCK    IoStatusBlock,
  OUT PVOID               FileInformation,
  IN ULONG                Length,
  IN FILE_INFORMATION_CLASS FileInformationClass );

NTSYSAPI 
NTSTATUS
NTAPI
ZwQueryInformationFile(
  IN HANDLE               FileHandle,
  OUT PIO_STATUS_BLOCK    IoStatusBlock,
  OUT PVOID               FileInformation,
  IN ULONG                Length,
  IN FILE_INFORMATION_CLASS FileInformationClass );

NTSYSAPI 
NTSTATUS
NTAPI
NtCreateDirectoryObject(
  OUT PHANDLE             DirectoryHandle,
  IN ACCESS_MASK          DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes );

NTSYSAPI 
NTSTATUS
NTAPI
ZwCreateDirectoryObject(
  OUT PHANDLE             DirectoryHandle,
  IN ACCESS_MASK          DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes );
  
NTSYSAPI 
NTSTATUS
NTAPI
ZwOpenFile(
  OUT PHANDLE             FileHandle,
  IN ACCESS_MASK          DesiredAccess,
  IN POBJECT_ATTRIBUTES   ObjectAttributes,
  OUT PIO_STATUS_BLOCK    IoStatusBlock,
  IN ULONG                ShareAccess,
  IN ULONG                OpenOptions );

NTSYSAPI 
NTSTATUS
NTAPI
ZwQuerySection(
  IN HANDLE               SectionHandle,
  IN SECTION_INFORMATION_CLASS InformationClass,
  OUT PVOID               InformationBuffer,
  IN ULONG                InformationBufferSize,
  OUT PULONG              ResultLength OPTIONAL );

NTSYSAPI 
NTSTATUS
NTAPI
ZwCreateProcess(
  OUT PHANDLE           ProcessHandle,
  IN ACCESS_MASK        DesiredAccess,
  IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
  IN HANDLE             ParentProcess,
  IN BOOLEAN            InheritObjectTable,
  IN HANDLE             SectionHandle OPTIONAL,
  IN HANDLE             DebugPort OPTIONAL,
  IN HANDLE             ExceptionPort OPTIONAL );



typedef struct _RTL_USER_PROCESS_INFORMATION {
  ULONG                   Size;
  HANDLE                  ProcessHandle;
  HANDLE                  ThreadHandle;
  CLIENT_ID               ClientId;
  SECTION_IMAGE_INFORMATION ImageInformation;
} RTL_USER_PROCESS_INFORMATION, *PRTL_USER_PROCESS_INFORMATION;


NTSYSAPI 
NTSTATUS
NTAPI
RtlCreateUserProcess(
  IN PUNICODE_STRING      ImagePath,
  IN ULONG                ObjectAttributes,
  IN OUT PRTL_USER_PROCESS_PARAMETERS ProcessParameters,
  IN PSECURITY_DESCRIPTOR ProcessSecurityDescriptor OPTIONAL,
  IN PSECURITY_DESCRIPTOR ThreadSecurityDescriptor OPTIONAL,
  IN HANDLE               ParentProcess,
  IN BOOLEAN              InheritHandles,
  IN HANDLE               DebugPort OPTIONAL,
  IN HANDLE               ExceptionPort OPTIONAL,
  OUT PRTL_USER_PROCESS_INFORMATION ProcessInformation );




//
// Debug Object Access Masks
//
#define DEBUG_OBJECT_WAIT_STATE_CHANGE      0x0001
#define DEBUG_OBJECT_ADD_REMOVE_PROCESS     0x0002
#define DEBUG_OBJECT_SET_INFORMATION        0x0004
#define DEBUG_OBJECT_ALL_ACCESS             (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x0F)

//
// Debug Object Information Classes for NtQueryDebugObject
//
typedef enum _DEBUGOBJECTINFOCLASS
{
    DebugObjectUnusedInformation,
    DebugObjectKillProcessOnExitInformation
} DEBUGOBJECTINFOCLASS, *PDEBUGOBJECTINFOCLASS;

//
// Debug Message API Number
//
typedef enum _DBGKM_APINUMBER
{
    DbgKmExceptionApi = 0,
    DbgKmCreateThreadApi = 1,
    DbgKmCreateProcessApi = 2,
    DbgKmExitThreadApi = 3,
    DbgKmExitProcessApi = 4,
    DbgKmLoadDllApi = 5,
    DbgKmUnloadDllApi = 6,
    DbgKmErrorReportApi = 7,
    DbgKmMaxApiNumber = 8,
} DBGKM_APINUMBER;

//
// Debug Object Information Structures
//
typedef struct _DEBUG_OBJECT_KILL_PROCESS_ON_EXIT_INFORMATION
{
    ULONG KillProcessOnExit;
} DEBUG_OBJECT_KILL_PROCESS_ON_EXIT_INFORMATION, *PDEBUG_OBJECT_KILL_PROCESS_ON_EXIT_INFORMATION;



//
// Debug States
//
typedef enum _DBG_STATE
{
    DbgIdle,
    DbgReplyPending,
    DbgCreateThreadStateChange,
    DbgCreateProcessStateChange,
    DbgExitThreadStateChange,
    DbgExitProcessStateChange,
    DbgExceptionStateChange,
    DbgBreakpointStateChange,
    DbgSingleStepStateChange,
    DbgLoadDllStateChange,
    DbgUnloadDllStateChange
} DBG_STATE, *PDBG_STATE;

//
// Debug Message Structures
//
typedef struct _DBGKM_EXCEPTION
{
    EXCEPTION_RECORD ExceptionRecord;
    ULONG FirstChance;
} DBGKM_EXCEPTION, *PDBGKM_EXCEPTION;

typedef struct _DBGKM_CREATE_THREAD
{
    ULONG SubSystemKey;
    PVOID StartAddress;
} DBGKM_CREATE_THREAD, *PDBGKM_CREATE_THREAD;

typedef struct _DBGKM_CREATE_PROCESS
{
    ULONG SubSystemKey;
    HANDLE FileHandle;
    PVOID BaseOfImage;
    ULONG DebugInfoFileOffset;
    ULONG DebugInfoSize;
    DBGKM_CREATE_THREAD InitialThread;
} DBGKM_CREATE_PROCESS, *PDBGKM_CREATE_PROCESS;

typedef struct _DBGKM_EXIT_THREAD
{
    NTSTATUS ExitStatus;
} DBGKM_EXIT_THREAD, *PDBGKM_EXIT_THREAD;

typedef struct _DBGKM_EXIT_PROCESS
{
    NTSTATUS ExitStatus;
} DBGKM_EXIT_PROCESS, *PDBGKM_EXIT_PROCESS;

typedef struct _DBGKM_LOAD_DLL
{
    HANDLE FileHandle;
    PVOID BaseOfDll;
    ULONG DebugInfoFileOffset;
    ULONG DebugInfoSize;
    PVOID NamePointer;
} DBGKM_LOAD_DLL, *PDBGKM_LOAD_DLL;

typedef struct _DBGKM_UNLOAD_DLL
{
    PVOID BaseAddress;
} DBGKM_UNLOAD_DLL, *PDBGKM_UNLOAD_DLL;

//
// User-Mode Debug State Change Structure
//
typedef struct _DBGUI_WAIT_STATE_CHANGE
{
    DBG_STATE NewState;
    CLIENT_ID AppClientId;
    union
    {
        struct
        {
            HANDLE HandleToThread;
            DBGKM_CREATE_THREAD NewThread;
        } CreateThread;
        struct
        {
            HANDLE HandleToProcess;
            HANDLE HandleToThread;
            DBGKM_CREATE_PROCESS NewProcess;
        } CreateProcessInfo;
        DBGKM_EXIT_THREAD ExitThread;
        DBGKM_EXIT_PROCESS ExitProcess;
        DBGKM_EXCEPTION Exception;
        DBGKM_LOAD_DLL LoadDll;
        DBGKM_UNLOAD_DLL UnloadDll;
    } StateInfo;
} DBGUI_WAIT_STATE_CHANGE, *PDBGUI_WAIT_STATE_CHANGE;

//
// LPC Debug Message
//
typedef struct _DBGKM_MSG
{
    PORT_MESSAGE h;
    DBGKM_APINUMBER ApiNumber;
    ULONG ReturnedStatus;
    union
    {
        DBGKM_EXCEPTION Exception;
        DBGKM_CREATE_THREAD CreateThread;
        DBGKM_CREATE_PROCESS CreateProcess;
        DBGKM_EXIT_THREAD ExitThread;
        DBGKM_EXIT_PROCESS ExitProcess;
        DBGKM_LOAD_DLL LoadDll;
        DBGKM_UNLOAD_DLL UnloadDll;
    };
} DBGKM_MSG, *PDBGKM_MSG;

//////////////////////////////////////////////////////////////////////////

//
// Product types
//

typedef enum _NT_PRODUCT_TYPE {
	NtProductWinNt = 1,
	NtProductLanManNt,
	NtProductServer
} NT_PRODUCT_TYPE, *PNT_PRODUCT_TYPE;


//
// the bit mask, SharedUserData->SuiteMask, is a ULONG
// so there can be a maximum of 32 entries
// in this enum.
//

typedef enum _SUITE_TYPE {
	SmallBusiness,
	Enterprise,
	BackOffice,
	CommunicationServer,
	TerminalServer,
	SmallBusinessRestricted,
	EmbeddedNT,
	DataCenter,
	SingleUserTS,
	Personal,
	Blade,
	MaxSuiteType
} SUITE_TYPE;

#define PROCESSOR_FEATURE_MAX 64

//
// Defined processor features
//

#define PF_FLOATING_POINT_PRECISION_ERRATA  0   // winnt
#define PF_FLOATING_POINT_EMULATED          1   // winnt
#define PF_COMPARE_EXCHANGE_DOUBLE          2   // winnt
#define PF_MMX_INSTRUCTIONS_AVAILABLE       3   // winnt
#define PF_PPC_MOVEMEM_64BIT_OK             4   // winnt
#define PF_ALPHA_BYTE_INSTRUCTIONS          5   // winnt
#define PF_XMMI_INSTRUCTIONS_AVAILABLE      6   // winnt
#define PF_3DNOW_INSTRUCTIONS_AVAILABLE     7   // winnt
#define PF_RDTSC_INSTRUCTION_AVAILABLE      8   // winnt
#define PF_PAE_ENABLED                      9   // winnt
#define PF_XMMI64_INSTRUCTIONS_AVAILABLE   10   // winnt

typedef enum _ALTERNATIVE_ARCHITECTURE_TYPE {
	StandardDesign,                 // None == 0 == standard design
	NEC98x86,                       // NEC PC98xx series on X86
	EndAlternatives                 // past end of known alternatives
} ALTERNATIVE_ARCHITECTURE_TYPE;

//
// Define system time structure.
//

#pragma pack (push, 4)
typedef struct _KSYSTEM_TIME {
	ULONG LowPart;
	LONG High1Time;
	LONG High2Time;
} KSYSTEM_TIME, *PKSYSTEM_TIME;
#pragma pack (pop)

//
// User/Kernel shared data.
//
// From ntddk.h + reversed fields.
//
// Note: When adding a new field that's processor-architecture-specific (for example, bound with #if i386),
// then place this field to be the last element in the KUSER_SHARED_DATA so that offsets into common
// fields are the same for Wow6432 and Win64.
//

typedef struct _KUSER_SHARED_DATA {

	//
	// Current low 32-bit of tick count and tick count multiplier.
	//
	// N.B. The tick count is updated each time the clock ticks.
	//

	volatile ULONG TickCountLow;
	ULONG TickCountMultiplier;

	//
	// Current 64-bit interrupt time in 100ns units.
	//

	volatile KSYSTEM_TIME InterruptTime;

	//
	// Current 64-bit system time in 100ns units.
	//

	volatile KSYSTEM_TIME SystemTime;

	//
	// Current 64-bit time zone bias.
	//

	volatile KSYSTEM_TIME TimeZoneBias;

	//
	// Support image magic number range for the host system.
	//
	// N.B. This is an inclusive range.
	//

	USHORT ImageNumberLow;
	USHORT ImageNumberHigh;

	//
	// Copy of system root in Unicode
	//

	WCHAR NtSystemRoot[ 260 ];

	//
	// Maximum stack trace depth if tracing enabled.
	//

	ULONG MaxStackTraceDepth;

	//
	// Crypto Exponent
	//

	ULONG CryptoExponent;

	//
	// TimeZoneId
	//

	ULONG TimeZoneId;

	ULONG LargePageMinimum; // vista
	ULONG Reserved2[ 7 ];

	//
	// product type
	//

	NT_PRODUCT_TYPE NtProductType;
	BOOLEAN ProductTypeIsValid;

	//
	// NT Version. Note that each process sees a version from its PEB, but
	// if the process is running with an altered view of the system version,
	// the following two fields are used to correctly identify the version
	//

	ULONG NtMajorVersion;
	ULONG NtMinorVersion;

	//
	// Processor Feature Bits
	//

	BOOLEAN ProcessorFeatures[PROCESSOR_FEATURE_MAX];

	//
	// Reserved fields - do not use
	//
	ULONG Reserved1;
	ULONG Reserved3;

	//
	// Time slippage while in debugger
	//

	volatile ULONG TimeSlip;

	//
	// Alternative system architecture.  Example: NEC PC98xx on x86
	//

	ALTERNATIVE_ARCHITECTURE_TYPE AlternativeArchitecture;

	//
	// If the system is an evaluation unit, the following field contains the
	// date and time that the evaluation unit expires. A value of 0 indicates
	// that there is no expiration. A non-zero value is the UTC absolute time
	// that the system expires.
	//

	LARGE_INTEGER SystemExpirationDate;

	//
	// Suite Support
	//

	ULONG SuiteMask; // see SUITE_TYPE

	//
	// TRUE if a kernel debugger is connected/enabled
	//

	BOOLEAN KdDebuggerEnabled;

	// only >=Vista, offset 0x2d5 - in previous var's padding
	BOOLEAN NXSupportPolicy;

	//
	// Current console session Id. Always zero on non-TS systems
	//
	volatile ULONG ActiveConsoleId;

	//
	// Force-dismounts cause handles to become invalid. Rather than
	// always probe handles, we maintain a serial number of
	// dismounts that clients can use to see if they need to probe
	// handles.
	//

	volatile ULONG DismountCount;

	//
	// This field indicates the status of the 64-bit COM+ package on the system.
	// It indicates whether the Itermediate Language (IL) COM+ images need to
	// use the 64-bit COM+ runtime or the 32-bit COM+ runtime.
	//

	ULONG ComPlusPackage;

	//
	// Time in tick count for system-wide last user input across all
	// terminal sessions. For MP performance, it is not updated all
	// the time (e.g. once a minute per session). It is used for idle
	// detection.
	//

	ULONG LastSystemRITEventTickCount;

	//
	// Number of physical pages in the system.  This can dynamically
	// change as physical memory can be added or removed from a running
	// system.
	//

	ULONG NumberOfPhysicalPages;

	//
	// True if the system was booted in safe boot mode.
	//

	BOOLEAN SafeBootMode;

	//
	// The following field is used for Heap  and  CritSec Tracing
	// The last bit is set for Critical Sec Collision tracing and
	// second Last bit is for Heap Tracing
	// Also the first 16 bits are used as counter.
	//

	union {
		ULONG TraceLogging;

		// >= vista
		ULONG SharedDataFlags;

		struct {
			ULONG DbgErrorPortPresent : 1;
			ULONG DbgElevationEnabled : 1;
			ULONG DbgVirtEnabled : 1;
			ULONG DbgInstallerDetectEnabled : 1;
			ULONG SystemDllRelocated : 1;
			ULONG SpareBits : 27;
		};
	};

	//
	// Depending on the processor, the code for fast system call
	// will differ, the following buffer is filled with the appropriate
	// code sequence and user mode code will branch through it.
	//
	// (32 bytes, using ULONGLONG for alignment).
	//

	union {
		ULONGLONG   Fill0;          // alignment
		ULONGLONG   TestRetInstruction;
	};

	PVOID       SystemCall;
	PVOID       SystemCallReturn;

	ULONGLONG   SystemCallPad[3];

#if _WIN32_WINNT >= _WIN32_WINNT_WIN2K

	// 0x320
	union 
	{
		KSYSTEM_TIME  TickCount;
		UINT64        TickCountQuad;
		ULONG         ReservedTickCountOverlay[3]; // win7 only
	};

	ULONG        Cookie;
	ULONG        CookiePad1; // 0x334

#if _WIN32_WINNT >= _WIN32_WINNT_LONGHORN // Vista

	// 0x338
	INT64        ConsoleSessionForegroundProcessId;
	ULONG        Wow64SharedInformation[16]; // 0x340

	union {
		struct {
			WORD         UserModeGlobalLogger[8];
			ULONG        HeapTracingPid[2];
			ULONG        CritSecTracingPid[2];
		} vista;
		struct {
			WORD         UserModeGlobalLogger[16];
		} w7;
	} uDbg;

	ULONG        ImageFileExecutionOptions;
	union
	{
		UINT64       AffinityPad;
		ULONG        ActiveProcessorAffinity;
	};
	UINT64       InterruptTimeBias;

#if _WIN32_WINNT >= 0x0601 // Windows 7

	// 0x3b8
	UINT64       TscQpcBias;
	ULONG        ActiveProcessorCount; // 0x3c0
	USHORT       ActiveGroupCount; // 0x3c4
	USHORT       Reserved4; // 0x3c6
	ULONG        AitSamplingValue; // 0x3c8
	ULONG        AppCompatFlag; // 0x3cc
	UINT64       SystemDllNativeRelocation; // 0x3d0
	ULONG        SystemDllWowRelocation; // 0x3d8
	ULONG        XStatePad; // 0x3dc
	XSTATE_CONFIGURATION XState; // 0x3e0 |0x210|

#endif // Win7

#endif // Vista

#endif

} KUSER_SHARED_DATA, *PKUSER_SHARED_DATA;


// Default kernel address of KUSER_SHARED_DATA mapping.
#define KI_USER_SHARED_DATA         0xffdf0000

// Default usermode address of KUSER_SHARED_DATA mapping (last user-mode page).
#define UM_USER_SHARED_DATA         0x7ffe0000


#define SharedUserData  ((const KUSER_SHARED_DATA * const) UM_USER_SHARED_DATA)


NTSYSAPI
NTSTATUS
NTAPI
DbgUiConnectToDbg(
	VOID
	);

NTSYSAPI
NTSTATUS
NTAPI
DbgUiContinue(
	IN PCLIENT_ID ClientId,
	IN NTSTATUS ContinueStatus
	);


NTSYSAPI
NTSTATUS 
NTAPI 
DbgUiConvertStateChangeStructure(
	IN PDBGUI_WAIT_STATE_CHANGE  WaitStateChange,
	OUT PVOID                    Win32DebugEvent	 
	);

NTSYSAPI
NTSTATUS 
NTAPI 
DbgUiDebugActiveProcess(
	IN HANDLE Process
	);

NTSYSAPI
HANDLE 
NTAPI 
DbgUiGetThreadDebugObject(
	VOID
	);

NTSYSAPI
NTSTATUS
NTAPI
DbgUiIssueRemoteBreakin(
	IN HANDLE Process
	);

NTSYSAPI
VOID
NTAPI
DbgUiRemoteBreakin(
	VOID
	);

NTSYSAPI
VOID
NTAPI
DbgUiSetThreadDebugObject(
	IN HANDLE DebugObject
	);

NTSYSAPI
NTSTATUS
NTAPI
DbgUiStopDebugging(
	IN HANDLE Process
	);

NTSYSAPI
NTSTATUS
NTAPI
DbgUiWaitStateChange(
	OUT PDBGUI_WAIT_STATE_CHANGE DbgUiWaitStateChange,
	IN PLARGE_INTEGER TimeOut
	);

NTSYSAPI 
NTSTATUS
NTAPI
NtReadFile(
  IN HANDLE               FileHandle,
  IN HANDLE               Event OPTIONAL,
  IN PVOID      ApcRoutine OPTIONAL,
  IN PVOID                ApcContext OPTIONAL,
  OUT PVOID    IoStatusBlock,
  OUT PVOID               Buffer,
  IN ULONG                Length,
  IN PLARGE_INTEGER       ByteOffset OPTIONAL,
  IN PULONG               Key OPTIONAL );

NTSYSAPI 
NTSTATUS
NTAPI
ZwReadFile(
  IN HANDLE               FileHandle,
  IN HANDLE               Event OPTIONAL,
  IN PVOID      ApcRoutine OPTIONAL,
  IN PVOID                ApcContext OPTIONAL,
  OUT PIO_STATUS_BLOCK    IoStatusBlock,
  OUT PVOID               Buffer,
  IN ULONG                Length,
  IN PLARGE_INTEGER       ByteOffset OPTIONAL,
  IN PULONG               Key OPTIONAL );

NTSYSAPI 
NTSTATUS
NTAPI
NtWriteFile(
  IN HANDLE               FileHandle,
  IN HANDLE               Event OPTIONAL,
  IN PVOID      ApcRoutine OPTIONAL,
  IN PVOID                ApcContext OPTIONAL,
  OUT PIO_STATUS_BLOCK    IoStatusBlock,
  IN PVOID                Buffer,
  IN ULONG                Length,
  IN PLARGE_INTEGER       ByteOffset OPTIONAL,
  IN PULONG               Key OPTIONAL );

NTSYSAPI 
NTSTATUS
NTAPI
ZwWriteFile(
  IN HANDLE               FileHandle,
  IN HANDLE               Event OPTIONAL,
  IN PVOID      ApcRoutine OPTIONAL,
  IN PVOID                ApcContext OPTIONAL,
  OUT PIO_STATUS_BLOCK    IoStatusBlock,
  IN PVOID                Buffer,
  IN ULONG                Length,
  IN PLARGE_INTEGER       ByteOffset OPTIONAL,
  IN PULONG               Key OPTIONAL );

NTSYSAPI 
NTSTATUS
NTAPI
ZwWaitForSingleObject(
  IN HANDLE               ObjectHandle,
  IN BOOLEAN              Alertable,
  IN PLARGE_INTEGER       TimeOut );

NTSYSAPI 
NTSTATUS
NTAPI
NtWaitForSingleObject(
  IN HANDLE               ObjectHandle,
  IN BOOLEAN              Alertable,
  IN PLARGE_INTEGER       TimeOut );

#define HARDERROR_OVERRIDE_ERRORMODE   0x10000000

NTSYSAPI 
NTSTATUS
NTAPI
NtRaiseHardError(
  IN NTSTATUS             ErrorStatus,
  IN ULONG                NumberOfParameters,
  IN ULONG                UnicodeStringParameterMask OPTIONAL,
  IN PVOID                *Parameters,
  IN HARDERROR_RESPONSE_OPTION ResponseOption,
  OUT PHARDERROR_RESPONSE Response );

NTSYSAPI 
NTSTATUS
NTAPI
ZwRaiseHardError(
  IN NTSTATUS             ErrorStatus,
  IN ULONG                NumberOfParameters,
  IN ULONG                UnicodeStringParameterMask OPTIONAL,
  IN PVOID                *Parameters,
  IN HARDERROR_RESPONSE_OPTION ResponseOption,
  OUT PHARDERROR_RESPONSE Response );

NTSYSAPI
NTSTATUS 
NTAPI
CsrClientCallServer(PVOID Request,
		    PVOID Reply OPTIONAL,
		    ULONG CsrApiNumber,
		    ULONG MaxRequestReplyLength);


#define SE_MIN_WELL_KNOWN_PRIVILEGE       (2L)
#define SE_CREATE_TOKEN_PRIVILEGE         (2L)
#define SE_ASSIGNPRIMARYTOKEN_PRIVILEGE   (3L)
#define SE_LOCK_MEMORY_PRIVILEGE          (4L)
#define SE_INCREASE_QUOTA_PRIVILEGE       (5L)

// end_wdm
//
// Unsolicited Input is obsolete and unused.
//

#define SE_UNSOLICITED_INPUT_PRIVILEGE    (6L)

// begin_wdm
#define SE_MACHINE_ACCOUNT_PRIVILEGE      (6L)
#define SE_TCB_PRIVILEGE                  (7L)
#define SE_SECURITY_PRIVILEGE             (8L)
#define SE_TAKE_OWNERSHIP_PRIVILEGE       (9L)
#define SE_LOAD_DRIVER_PRIVILEGE          (10L)
#define SE_SYSTEM_PROFILE_PRIVILEGE       (11L)
#define SE_SYSTEMTIME_PRIVILEGE           (12L)
#define SE_PROF_SINGLE_PROCESS_PRIVILEGE  (13L)
#define SE_INC_BASE_PRIORITY_PRIVILEGE    (14L)
#define SE_CREATE_PAGEFILE_PRIVILEGE      (15L)
#define SE_CREATE_PERMANENT_PRIVILEGE     (16L)
#define SE_BACKUP_PRIVILEGE               (17L)
#define SE_RESTORE_PRIVILEGE              (18L)
#define SE_SHUTDOWN_PRIVILEGE             (19L)
#define SE_DEBUG_PRIVILEGE                (20L)
#define SE_AUDIT_PRIVILEGE                (21L)
#define SE_SYSTEM_ENVIRONMENT_PRIVILEGE   (22L)
#define SE_CHANGE_NOTIFY_PRIVILEGE        (23L)
#define SE_REMOTE_SHUTDOWN_PRIVILEGE      (24L)
#define SE_UNDOCK_PRIVILEGE               (25L)
#define SE_SYNC_AGENT_PRIVILEGE           (26L)
#define SE_ENABLE_DELEGATION_PRIVILEGE    (27L)
#define SE_MANAGE_VOLUME_PRIVILEGE        (28L)
#define SE_MAX_WELL_KNOWN_PRIVILEGE       (SE_MANAGE_VOLUME_PRIVILEGE)


NTSYSAPI
NTSTATUS 
NTAPI
RtlAdjustPrivilege(
  ULONG    Privilege,
  BOOLEAN  Enable,
  BOOLEAN  CurrentThread,
  PBOOLEAN Enabled
 );

#define NtCurrentThread() ((HANDLE)-2)

NTSYSAPI
NTSTATUS
NTAPI
ZwGetContextThread(
    HANDLE ThreadHandle,
    PCONTEXT Context
    );

typedef enum _MEMORY_INFORMATION_CLASS {
    MemoryBasicInformation,
		MemoryWorkingSetList,
		MemorySectionName,
		MemoryBasicVlmInformation
} MEMORY_INFORMATION_CLASS, *PMEMORY_INFORMATION_CLASS;

typedef struct _MEMORY_SECTION_NAME {
	UNICODE_STRING SectionFileName;
} MEMORY_SECTION_NAME, *PMEMORY_SECTION_NAME;

NTSYSAPI
NTSTATUS
NTAPI
ZwQueryVirtualMemory(
    HANDLE ProcessHandle,
    PVOID VirtualAddress,
    MEMORY_INFORMATION_CLASS Class,
    PVOID Buffer,
    ULONG Length,
    PULONG ReturnedLength OPTIONAL
    );

NTSYSAPI 
NTSTATUS
NTAPI
ZwTerminateProcess(
  IN HANDLE               ProcessHandle OPTIONAL,
  IN NTSTATUS             ExitStatus );

NTSYSAPI
NTSTATUS 
NTAPI
ZwOpenKey(
  OUT PHANDLE  KeyHandle,
  IN ACCESS_MASK  DesiredAccess,
  IN POBJECT_ATTRIBUTES  ObjectAttributes
  );

NTSYSAPI 
NTSTATUS
NTAPI
NtQueryEaFile(
  IN HANDLE               FileHandle,
  OUT PIO_STATUS_BLOCK    IoStatusBlock,
  OUT PVOID               Buffer,
  IN ULONG                Length,
  IN BOOLEAN              ReturnSingleEntry,
  IN PVOID                EaList OPTIONAL,
  IN ULONG                EaListLength,
  IN PULONG               EaIndex OPTIONAL,
  IN BOOLEAN              RestartScan );

NTSYSAPI 
NTSTATUS
NTAPI
ZwQueryEaFile(
  IN HANDLE               FileHandle,
  OUT PIO_STATUS_BLOCK    IoStatusBlock,
  OUT PVOID               Buffer,
  IN ULONG                Length,
  IN BOOLEAN              ReturnSingleEntry,
  IN PVOID                EaList OPTIONAL,
  IN ULONG                EaListLength,
  IN PULONG               EaIndex OPTIONAL,
  IN BOOLEAN              RestartScan );

NTSYSAPI 
NTSTATUS
NTAPI
NtSetEaFile(
  IN HANDLE               FileHandle,
  OUT PIO_STATUS_BLOCK    IoStatusBlock,
  IN PVOID                EaBuffer,
  IN ULONG                EaBufferSize );

NTSYSAPI 
NTSTATUS
NTAPI
ZwSetEaFile(
  IN HANDLE               FileHandle,
  OUT PIO_STATUS_BLOCK    IoStatusBlock,
  IN PVOID                EaBuffer,
  IN ULONG                EaBufferSize );



typedef struct _FILE_FULL_EA_INFORMATION {
  ULONG                   NextEntryOffset;
  BYTE                    Flags;
  BYTE                    EaNameLength;
  USHORT                  EaValueLength;
  CHAR                    EaName[1];
} FILE_FULL_EA_INFORMATION, *PFILE_FULL_EA_INFORMATION;

typedef struct _FILE_GET_EA_INFORMATION {
  ULONG                   NextEntryOffset;
  BYTE                    EaNameLength;
  CHAR                    EaName[1];
} FILE_GET_EA_INFORMATION, *PFILE_GET_EA_INFORMATION;

NTSYSAPI
NTSTATUS
NTAPI
ZwResumeProcess (HANDLE);

NTSYSAPI
NTSTATUS 
NTAPI 
ZwSuspendProcess (HANDLE);

#define THREAD_STATE_INITIALIZED        0
#define THREAD_STATE_READY              1
#define THREAD_STATE_RUNNING            2
#define THREAD_STATE_STANDBY            3
#define THREAD_STATE_TERMINATED         4
#define THREAD_STATE_WAIT               5
#define THREAD_STATE_TRANSITION         6
#define THREAD_STATE_UNKNOWN            7

typedef enum _KWAIT_REASON {
	Executive,
	FreePage,
	PageIn,
	PoolAllocation,
	DelayExecution,
	Suspended,
	UserRequest,
	WrExecutive,
	WrFreePage,
	WrPageIn,
	WrPoolAllocation,
	WrDelayExecution,
	WrSuspended,
	WrUserRequest,
	WrEventPair,
	WrQueue,
	WrLpcReceive,
	WrLpcReply,
	WrVirtualMemory,
	WrPageOut,
	WrRendezvous,
	Spare2,
	Spare3,
	Spare4,
	Spare5,
	Spare6,
	WrKernel,
	MaximumWaitReason
} KWAIT_REASON;

NTSYSAPI
NTSTATUS
NTAPI
ZwSetInformationFile(
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    IN PVOID FileInformation,
    IN ULONG Length,
    IN FILE_INFORMATION_CLASS FileInformationClass
    );

NTSYSAPI
NTSTATUS
NTAPI
NtYieldExecution (
	);

NTSYSAPI
NTSTATUS
NTAPI
ZwYieldExecution (
	);

NTSYSAPI 
NTSTATUS
NTAPI
LdrLoadDll(
  IN PWCHAR               PathToFile OPTIONAL,
  IN PULONG               DllCharacteristics OPTIONAL,
  IN PUNICODE_STRING      ModuleFileName,
  OUT PVOID              *ModuleHandle );

NTSYSAPI 
NTSTATUS
NTAPI
LdrGetDllHandle(
  IN PWCHAR               PathToFile OPTIONAL,
  IN PULONG               DllCharacteristics OPTIONAL,
  IN PUNICODE_STRING      ModuleFileName,
  OUT PVOID              *ModuleHandle );

NTSYSAPI
NTSTATUS
NTAPI
LdrGetProcedureAddress (
    IN PVOID DllHandle,
    IN PANSI_STRING ProcedureName OPTIONAL,
    IN ULONG ProcedureNumber OPTIONAL,
    OUT PVOID *ProcedureAddress
    );

typedef enum _VdmServiceClass {
    VdmStartExecution,         // is also defined in ntos\ke\i386\biosa.asm
    VdmQueueInterrupt,
    VdmDelayInterrupt,
    VdmInitialize,
    VdmFeatures,
    VdmSetInt21Handler,
    VdmQueryDir,
    VdmPrinterDirectIoOpen,
    VdmPrinterDirectIoClose,
} VDMSERVICECLASS, *PVDMSERVICECLASS;

NTSYSAPI
NTSTATUS
NTAPI
NtVdmControl(
    IN VDMSERVICECLASS Service,
    IN OUT PVOID ServiceData
    );

NTSYSAPI
NTSTATUS
NTAPI
ZwVdmControl(
    IN VDMSERVICECLASS Service,
    IN OUT PVOID ServiceData
    );

NTSYSAPI
NTSTATUS
NTAPI
NtMakeTemporaryObject(
	IN HANDLE ObjectHandle
	);

NTSYSAPI
NTSTATUS
NTAPI
ZwMakeTemporaryObject(
	IN HANDLE ObjectHandle
	);

NTSYSAPI
NTSTATUS
NTAPI
NtMakePermanentObject(
	IN HANDLE ObjectHandle
	);

NTSYSAPI
NTSTATUS
NTAPI
ZwMakePermanentObject(
	IN HANDLE ObjectHandle
	);

NTSYSAPI
NTSTATUS 
NTAPI 	
ZwSetLdtEntries (
	ULONG Selector1,
	LDT_ENTRY LdtEntry1,
	ULONG Selector2,
	LDT_ENTRY LdtEntry2
	);

NTSYSAPI
NTSTATUS 
NTAPI 	
NtSetLdtEntries (
	ULONG Selector1,
	LDT_ENTRY LdtEntry1,
	ULONG Selector2,
	LDT_ENTRY LdtEntry2
	);

NTSYSAPI
NTSTATUS
NTAPI
NtDisplayString (
	PUNICODE_STRING
	);

NTSYSAPI
NTSTATUS
NTAPI
ZwDisplayString (
	PUNICODE_STRING
	);

// Object types
#define OB_TYPE_TYPE                    1
#define OB_TYPE_DIRECTORY               2
#define OB_TYPE_SYMBOLIC_LINK           3
#define OB_TYPE_TOKEN                   4
#define OB_TYPE_PROCESS                 5
#define OB_TYPE_THREAD                  6
#define OB_TYPE_EVENT                   7
#define OB_TYPE_EVENT_PAIR              8
#define OB_TYPE_MUTANT                  9
#define OB_TYPE_SEMAPHORE               10
#define OB_TYPE_TIMER                   11
#define OB_TYPE_PROFILE                 12
#define OB_TYPE_WINDOW_STATION          13
#define OB_TYPE_DESKTOP                 14
#define OB_TYPE_SECTION                 15
#define OB_TYPE_KEY                     16
#define OB_TYPE_PORT                    17
#define OB_TYPE_ADAPTER                 18
#define OB_TYPE_CONTROLLER              19
#define OB_TYPE_DEVICE                  20
#define OB_TYPE_DRIVER                  21
#define OB_TYPE_IO_COMPLETION           22
#define OB_TYPE_FILE                    23

//#pragma warning (default:4005)

NTSYSAPI 
NTSTATUS
NTAPI
NtDelayExecution(
	IN BOOLEAN Alertable, 
	IN PLARGE_INTEGER DelayInterval 
	); 

NTSYSAPI 
NTSTATUS
NTAPI
ZwDelayExecution(
	IN BOOLEAN Alertable, 
	IN PLARGE_INTEGER DelayInterval 
	); 

extern "C" NTSTATUS NTAPI RtlNormalizeProcessParams (PRTL_USER_PROCESS_PARAMETERS);

__forceinline
LARGE_INTEGER
LonglongToLargeInteger(
	LONGLONG Value
	)
{
	LARGE_INTEGER li;
	li.QuadPart = Value;
	return li;
}

__forceinline
LARGE_INTEGER
MillisecondsToLargeInterval(
	ULONG Milliseconds,
	BOOLEAN Relative
	)
{
	return LonglongToLargeInteger ( (Relative ? -1 : 1) * 10000 * Milliseconds );
}

typedef struct _KEY_VALUE_BASIC_INFORMATION {
	ULONG   TitleIndex;
	ULONG   Type;
	ULONG   NameLength;
	WCHAR   Name[1];            // Variable size
} KEY_VALUE_BASIC_INFORMATION, *PKEY_VALUE_BASIC_INFORMATION;

typedef struct _KEY_VALUE_FULL_INFORMATION {
	ULONG   TitleIndex;
	ULONG   Type;
	ULONG   DataOffset;
	ULONG   DataLength;
	ULONG   NameLength;
	WCHAR   Name[1];            // Variable size
	//          Data[1];            // Variable size data not declared
} KEY_VALUE_FULL_INFORMATION, *PKEY_VALUE_FULL_INFORMATION;

typedef struct _KEY_VALUE_PARTIAL_INFORMATION {
	ULONG   TitleIndex;
	ULONG   Type;
	ULONG   DataLength;
	UCHAR   Data[1];            // Variable size
} KEY_VALUE_PARTIAL_INFORMATION, *PKEY_VALUE_PARTIAL_INFORMATION;

typedef struct _KEY_VALUE_PARTIAL_INFORMATION_ALIGN64 {
	ULONG   Type;
	ULONG   DataLength;
	UCHAR   Data[1];            // Variable size
} KEY_VALUE_PARTIAL_INFORMATION_ALIGN64, *PKEY_VALUE_PARTIAL_INFORMATION_ALIGN64;

typedef struct _KEY_VALUE_ENTRY {
	PUNICODE_STRING ValueName;
	ULONG           DataLength;
	ULONG           DataOffset;
	ULONG           Type;
} KEY_VALUE_ENTRY, *PKEY_VALUE_ENTRY;

typedef enum _KEY_VALUE_INFORMATION_CLASS {
	KeyValueBasicInformation,
	KeyValueFullInformation,
	KeyValuePartialInformation,
	KeyValueFullInformationAlign64,
	KeyValuePartialInformationAlign64
} KEY_VALUE_INFORMATION_CLASS;

NTSYSAPI 
NTSTATUS
NTAPI
NtQueryValueKey(
	IN HANDLE               KeyHandle,
	IN PUNICODE_STRING      ValueName,
	IN KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
	OUT PVOID               KeyValueInformation,
	IN ULONG                Length,
	OUT PULONG              ResultLength );

NTSYSAPI 
NTSTATUS
NTAPI
ZwQueryValueKey(
	IN HANDLE               KeyHandle,
	IN PUNICODE_STRING      ValueName,
	IN KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
	OUT PVOID               KeyValueInformation,
	IN ULONG                Length,
	OUT PULONG              ResultLength );

#endif
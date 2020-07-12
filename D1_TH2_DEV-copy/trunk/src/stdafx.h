//#pragma once
#ifndef _stdafx_h_
#define _stdafx_h_
#else
#error reinclude stdafx.h
#endif

#pragma warning(disable: 4996 4091; error: 4551 4552 4553 4554 4555)

#define _USE_32BIT_TIME_T
#define _USING_32BIT_TIME_T_IS_STRONGLY_DEPRECATED

#define USE_32BITCOLOR
#define OLD_PCX_SCREENSHOT
#define HIRES
//#define DIRECTDRAW_VERSION 0x0300
//#define DIRECTSOUND_VERSION 0x0300
//#define DIRECTINPUT_VERSION 0x0300

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define NOMINMAX
#include <windows.h>
#include <io.h>
#include <ctype.h>
#include <ddraw.h>
//#include <ddrawi.h> // используется для storm.cpp ?
#include <d3d9.h>
#include <mmreg.h>
#include <austream.h>
#include <dsound.h>
#include <time.h>
#include <process.h>

#include <SHLOBJ.h>
#include <SHELLAPI.h>

#include <math.h>
#include <tchar.h>
#include <stdio.h>
#include <stdarg.h>
#include <CommDlg.h>
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <vector>
#include <cmath>
#include <string.h>
#include <stdlib.h>
#include <eh.h>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <strstream>
#include <assert.h>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <fcntl.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <intrin.h>
#include <atomic>
#include <map>
#include <stack>
#include <filesystem>

using namespace std;

#ifdef _DEBUG
#include <assert.h>
#endif

#define countof( a ) __crt_countof( a ) // считает только верхнее измерение ?
#define fullcountof( a ) ( sizeof( a ) / sizeof( remove_all_extents_t< decltype( a ) > ) ) // считает сразу все измерения
#define flat( a ) reinterpret_cast<remove_all_extents_t< decltype( a )> (&) [ fullcountof( a ) ]>(a)

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned long long u64;
typedef long long i64;
#define _UNKNOWN char // could not determine their types

#ifdef _MSC_VER
#if _MSC_VER < 1500
#define snprintf _snprintf
#define vsnprintf _vsnprintf
typedef int LSTATUS;
#endif
#endif

// определение относительно ссылки на элемент массива в виде типа самого массива, для удобной адресации к соседним клеткам 
// использовать auto cell = REL( Map )[ row ][ call ]; либо auto &cell = REL( Map )[ row ][ call ]; 
#define REL( arr ) *(decltype(&arr)) &arr

constexpr bool th2 = true;
#define check_size(...)
#define check_ofs(...)
#define reflect(...)
#define begin_(...)
#define _(...)
#define end_(...)
#define begin__(...)
#define _E(...)
#define _U(...)
#define end__(...)
#define reflect_enum(...)
#define SUB_TABLE
#define VAR_TABLE
#define VAR( adr, type1, type2, name ) extern type1 name type2;
#define VAR_( adr, type1, type2, name ) extern type1 name type2;
#define DYN( adr, type1, type2, name ) extern type1 name;
#define DYN_( adr, type1, type2, name ) extern type1 name;
#define SYS( adr, type1, type2, name ) extern type1 name type2;
#define OLD( adr, type1, type2, name ) extern type1 old_##name type2;
#define SUB( adr, ret, call, name, ... ) ret call name( __VA_ARGS__ ); 
#define SUB_( adr, ret, call, name, ... ) ret call name( __VA_ARGS__ );
#define CRT( adr, ret, call, name, ... ) ret call name( __VA_ARGS__ ); 
#define EXE( name )
#define _EXE( name )
#define isEXE false

#ifdef NDEBUG
#define Assert(expression) ((void)0)
#else
#define Assert(expression) (void)((!!(expression)) || ( SwitchFullscreen(1),_wassert(_CRT_WIDE(#expression), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) )
#endif

#include "bitsbytes.h"
#include "IniFile.h"
#include "enums.h"
#include "structs.h"
// out of array guards
#define GSZ 0x4000 // guard buffer size (16K * 4 = 64K)
//#define GUARD
#ifdef GUARD
extern int Z0[GSZ],Z1[GSZ],Z2[GSZ],Z3[GSZ],Z4[GSZ],Z5[GSZ],Z6[GSZ],Z7[GSZ],Z8[GSZ],Z9[GSZ],ZA[GSZ],ZB[GSZ],ZC[GSZ],ZD[GSZ],ZE[GSZ],ZF[GSZ];
#endif
#include "vars.h" 
#include "funcs.h"
#include "common.h"
//#include "th2_proposals.h"
//#define ALLOW_TH1_CONVERTATION
//#define MORE_COLOR
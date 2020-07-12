//#pragma once
#ifndef _bitsbytes_h_
#define _bitsbytes_h_
#else
#error reinclude bitsbytes.h
#endif

// work with bit masks
#define B(i) (1ui64<<(i))
//#define B64(i) (1ui64<<(i))
#define BIT(n) (1<<(n-1))
#define BIT_64(n) (1i64<<(n-1))
#define BIT_INTERVAL(x, y) (-1 << (x)) & (-1 >> (sizeof(int)*8-(y)))
#define ClearBit	( value, bitMask ) value	&= ~( bitMask )	// AND reverse
#define SetBit		( value, bitMask ) value	|=	( bitMask )	// OR 
#define CheckBit	( value, bitMask )(value	&	( bitMask ))// AND
#define ChangeBit	( value, bitMask ) value	^=	( bitMask )	// XOR
__forceinline DWORD MostBit( DWORD val ) { DWORD i = 0; _BitScanReverse( &i, val ); return i; } // версия без проверки на нулевое входное значение
//__forceinline DWORD MostBit( DWORD val ) { DWORD i; if( _BitScanReverse( &i, val ) ) return i; else return -1; } // если подали нуль, возвращаем -1

// first unsigned macros:
// Some convenience macros to make partial accesses nicer

#define LOBYTE_IDA(x)   (*((uchar*)&(x)))   // low byte
#define LOWORD_IDA(x)   (*((ushort*)&(x)))   // low word
#define LODWORD_IDA(x)  (*((uint*)&(x)))  // low dword
#define HIBYTE_IDA(x)   (*((uchar*)&(x)+1))
#define HIWORD_IDA(x)   (*((ushort*)&(x)+1))
#define HIDWORD_IDA(x)  (*((uint*)&(x)+1))
#define BYTEn(x, n)   (*((uchar*)&(x)+n))
#define WORDn(x, n)   (*((ushort*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1)         // byte 1 (counting from 0)
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define BYTE4(x)   BYTEn(x,  4)
#define BYTE5(x)   BYTEn(x,  5)
#define BYTE6(x)   BYTEn(x,  6)
#define BYTE7(x)   BYTEn(x,  7)
#define BYTE8(x)   BYTEn(x,  8)
#define BYTE9(x)   BYTEn(x,  9)
#define BYTE10(x)  BYTEn(x, 10)
#define BYTE11(x)  BYTEn(x, 11)
#define BYTE12(x)  BYTEn(x, 12)
#define BYTE13(x)  BYTEn(x, 13)
#define BYTE14(x)  BYTEn(x, 14)
#define BYTE15(x)  BYTEn(x, 15)
#define WORD1(x)   WORDn(x,  1)
#define WORD2(x)   WORDn(x,  2)         // third word of the object, unsigned
#define WORD3(x)   WORDn(x,  3)
#define WORD4(x)   WORDn(x,  4)
#define WORD5(x)   WORDn(x,  5)
#define WORD6(x)   WORDn(x,  6)
#define WORD7(x)   WORDn(x,  7)

// now signed macros (the same but with sign extension)
#define SLOBYTE_IDA(x)   (*((char*)&(x)))
#define SLOWORD_IDA(x)   (*((short*)&(x)))
#define SLODWORD_IDA(x)  (*((long*)&(x)))
#define SHIBYTE_IDA(x)   (*((char*)&(x)+1))
#define SHIWORD_IDA(x)   (*((short*)&(x)+1))
#define SHIDWORD_IDA(x)  (*((long*)&(x)+1))
#define SBYTEn(x, n)   (*((char*)&(x)+n))
#define SWORDn(x, n)   (*((short*)&(x)+n))
#define SBYTE1(x)   SBYTEn(x,  1)
#define SBYTE2(x)   SBYTEn(x,  2)
#define SBYTE3(x)   SBYTEn(x,  3)
#define SBYTE4(x)   SBYTEn(x,  4)
#define SBYTE5(x)   SBYTEn(x,  5)
#define SBYTE6(x)   SBYTEn(x,  6)
#define SBYTE7(x)   SBYTEn(x,  7)
#define SBYTE8(x)   SBYTEn(x,  8)
#define SBYTE9(x)   SBYTEn(x,  9)
#define SBYTE10(x)  SBYTEn(x, 10)
#define SBYTE11(x)  SBYTEn(x, 11)
#define SBYTE12(x)  SBYTEn(x, 12)
#define SBYTE13(x)  SBYTEn(x, 13)
#define SBYTE14(x)  SBYTEn(x, 14)
#define SBYTE15(x)  SBYTEn(x, 15)
#define SWORD1(x)   SWORDn(x,  1)
#define SWORD2(x)   SWORDn(x,  2)
#define SWORD3(x)   SWORDn(x,  3)
#define SWORD4(x)   SWORDn(x,  4)
#define SWORD5(x)   SWORDn(x,  5)
#define SWORD6(x)   SWORDn(x,  6)
#define SWORD7(x)   SWORDn(x,  7)

// represent some assembly instructions
// Feel free to modify them

// в исходниках нету, посмотреть оригинал
#define __ROL16__(x, y) _rotl16(x,y)				// (((x)<<(y))|((x)>>(16-y)))	// Rotate left
// оригинальный
#define __ROR16__(x, y) _rotr16(x,y)				// (((x)>>(y))|((x)<<(16-y)))	// Rotate right
// в исходниках нету, посмотреть оригинал
#define __ROL32__(x, y) _rotl(x,y)					// (((x)<<(y))|((x)>>(32-y)))	// Rotate left
// оригинальный в DrawSpellIcon, проверить остальные
#define __ROR32__(x, y) _rotr(x,y)					// ((x)>>(y))|((x)<<(32-y)))	// Rotate right
// можно попробовать интринсик, в оригинале shr + shl стоит
#define __ROL__(x, y) (((x)<<(y))|((x)>>(32-y)))		// Rotate left
#define __ROR__(x, y) (((x)>>(y))|((x)<<(32-y)))		// Rotate right
// остальные еще не проверил
#define __RCL__(x, y) invalid_operation				// Rotate left thru carry
#define __RCR__(x, y) invalid_operation				// Rotate right thru carry
#define __MKCADD__(x, y) invalid_operation			// Generate carry flag for an addition
#define __MKOADD__(x, y) invalid_operation			// Generate overflow flag for an addition
#define __MKCSHL__(x, y) (((x)>>(31-y))&1UL)			// Generate carry flag for a shift left
#define __MKCSHR__(x, y) (((x)>>(y-1))&1UL)			// Generate carry flag for a shift right
#define __MKCRCL__(x, y) invalid_operation			// Generate carry flag for a RCL
#define __MKCRCR__(x, y) invalid_operation			// Generate carry flag for a RCR
// Sets the byte in the operand to 1 if the Overflow Flag is set, otherwise sets the operand to 0.
#define __SETO__(x, y)   0							// Generate overflow flags for (x-y)


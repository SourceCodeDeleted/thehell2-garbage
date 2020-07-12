//#pragma once
#ifndef _common_h_
#define _common_h_
#else
#error reinclude common.h
#endif

// test intervals
#define InSegment( x, min_x, max_x )   ( (x) >= (min_x) && (x) <= (max_x) )
#define InInterval( x, min_x, max_x )   ( (x) > (min_x) && (x) < (max_x) )

// convert row40 & col40 to row112 & col112
#define Convert40to112( x40 )   ( 2 * (x40) + 16 )

#define FreeMemAndZeroPtr( ptr ) { void* oldAddr = ptr; ptr = NULL; FreeMem(oldAddr); }

// Если число больше максимума, присваеваем ему максимум
template<class T> __forceinline void LimitToMax( T& n, T max )
{
	if( n > max ) n = max;
}
// Если число меньше минимума, присваеваем ему минимум
template<class T> __forceinline void LimitToMin( T& n, T min )
{
	if( n < min ) n = min;
}
// Ограничиваем число сверху и снизу
template<class T, class V> __forceinline void LimitToRange( T& n, V min, V max)
{
	if( n < min ) n = min;
	else if( n > max ) n = max;
}

// если внутри есть вызов функции не th2-only и это не оптимизация оригинала
// то выносим в таблицу и ставим адрес th2
// Заворачиваем число вокруг диапазона [0; period - 1]
// этой функцией пытались заменить несколько разных операций, с возможным нарушением логики, проверить все вызовы
__forceinline void Wrap(int& n, int period)
{
	if( n >= period ){
		n -= period;
	}else if( n < 0 ){
		n += period;
	}
	// В оригинале нет подобных проверок, более того есть логика в которой явно требуется отрицательный результат
	// а если нужно обрезать любые числа то это можно делать так: (uint&)n %= period - 1 ;
	//#ifdef _DEBUG
	//if( !InSegment(n, 0, period - 1) ){
	//	TerminateWithError("Wrap illegal period (%d) or value (%d)", period, n);
	//}
	//#endif
}

// оптимизация, Функция получения итоговой строки из двух аргументов и строки в которую их помещать
__forceinline void sprints( char* dst, const char* stemplate, const char* s1, const char* s2 )
{
	static char res[ 4096 ]; //	с запасом
	sprintf( res, stemplate, s1, s2 );
	strcpy( dst, res );
}

enum MPQ_READING
{
  ONE_TRY   = 1,
  FIVE_TRY  = 0
};

#define XLAT()  LOBYTE_IDA(_EAX) = ((uchar*)_EBX)[_EAX&0xFF];

// добавлено для целей сверки, после можно заменить на (int) и включить Extended Instructions в компиляторе - станет быстрый инлайн
#define ftol(x) ((int)(__int64)((double)x))

// макрос для выбора значения из списка по индексу, например по сложности: int add = By( Difficulty, 10, 20, 30 );
//#define By(key, _1, _2, _3)  (key==0 ? (_1) : (key == 1 ? (_2) : (_3)))
template<typename T, typename... V> __forceinline T By( size_t i, T first, V... rest ){ T val[] = {first, (T)rest...}; return val[ i <= sizeof...(rest) ? i : sizeof...(rest) ]; }

// макрос для выражений вида baseMonsterIndex == 1 || baseMonsterIndex == 2 || ..., чтобы писать is(baseMonsterIndex, 1, 2, ...)
template<typename V, typename... T > __forceinline bool is ( V& var, T ... val ){ return ( ... || (var == val) ); }

// макрос для выражений вида baseMonsterIndex1 == 1 && baseMonsterIndex2 == 1 || ..., чтобы писать all(1, baseMonsterIndex1, baseMonsterIndex2, ...)
template<typename V, typename... T > __forceinline bool all( V val, T ... var ) { return (... && (var == val)); }

// макрос для выражений вида baseMonsterIndex != 1 && baseMonsterIndex != 2 && ..., чтобы писать not(baseMonsterIndex, 1, 2, ...)
template<typename V, typename... T > __forceinline bool not( V& var, T ... val ){ return ( ... && (var != val) ); }

// макрос для выражений вида item.AddsStrength += effectValue; item.AddsMagic += effectValue; ...
template<typename V, typename... T > __forceinline void addTo( V val, T& ... var ) { (..., (var += val)); }

// макрос для подряд идущих case
#define or : [[fallthrough]]; case

#define memzero(dst) memset( dst, 0, sizeof(dst) )

// try versioning struct size
enum Game_Param: uint { CurVersion = th2 ? 13 : 0 }; // при релизе архив версий можно будет очиситить и запретить загружать старые версии, либо сделать релизный идентификатор сейвов 'HEL2'
template<typename T> size_t StructSize(int version = -1) { return sizeof( T ); }
#define SS(T, ...) template<> inline size_t StructSize<T>(int version) { static const size_t s[] = { __VA_ARGS__ }; if( version = -1 ) version = SaveVersion; size_t size = sizeof(T); \
	for( int i = 0; i < countof(s); i+=2 ) if( size_t(version) > s[i] ) return size; else size = s[i+1]; return size; }
SS(Item, 12, 1081, 8, 889, 4, 877, 3, 581, 2, 555, 1, 372 ) // 9-13, 1081 (добавлять слева в массив индекс старшей
SS(Player, 12, 106224, 11, 73764, 10, 73760, 9, 73752, 8, 60824, 7, 56984, 6, 56728, 5, 56216, 4, 55488, 3, 35952, 2, 34144, 1, 23076, 0, 21960 ) // 12, 106224 // qndel - 13 = 106224 +4002 ?(added 1k ints +2 uchars)
#undef SS


Sha1Ctx SHA_CTXx3[3];

//*updated*
//----- (0046B48C) --------------------------------------------------------
void ClearSHAs()
{
	memset(SHA_CTXx3, 0, sizeof SHA_CTXx3);
}

//*updated*
//----- (0046B4A1) --------------------------------------------------------
void __fastcall SHA1Done( int part, uint* out )
{
	if( out ){
		Sha1Ctx& shaCtx = SHA_CTXx3[part];
		memcpy(out, shaCtx.hash, SHA1_DIGEST_SIZE);
	}
}

//*updated*
//----- (0046B4C3) --------------------------------------------------------
void __fastcall SHA1RunAndDone( int part, void* multiKey, uint* out )
{
	Sha1Ctx& shaCtx = SHA_CTXx3[part];
	ProcessSHA1_(shaCtx, (char*)multiKey, SHA1_BLOCK_SIZE);
	if( out ){
		SHA1Done(part, out);
	}
}

//*updated*
//----- (0046B4E9) --------------------------------------------------------
void __fastcall ProcessSHA1_( Sha1Ctx& sha1Ctx, char* aMultyKey, size_t size )
{
	int v6 = sha1Ctx.count[0] + 8 * size;
	if( v6 < (int)sha1Ctx.count[0] ){
		sha1Ctx.count[1]++;
	}
	sha1Ctx.count[0] = v6;
	sha1Ctx.count[1] += size >> 29;
	
	for( int i = size / SHA1_BLOCK_SIZE; i; i--, aMultyKey += SHA1_BLOCK_SIZE ){
		memcpy(sha1Ctx.wBuf, aMultyKey, SHA1_BLOCK_SIZE);
		ProcessSHA1(sha1Ctx);
	}
}

//*updated*
//----- (0046B53A) --------------------------------------------------------
void __fastcall ProcessSHA1( Sha1Ctx& ctx )
{
	int w[80];
	// 16 слов по 32-бита дополняются до 80 32-битовых слов:
	memcpy(w, ctx.wBuf, SHA1_BLOCK_SIZE);
	for( int i = 16; i < 80; i++ ){
		w[i] = w[i - 3] ^  w[i - 8] ^ w[i - 14] ^ w[i - 16];// циклический сдвиг влево 1
	}

	// Инициализация хеш-значений этой части:
	int a = ctx.hash[0];
	int b = ctx.hash[1];
	int c = ctx.hash[2];
	int d = ctx.hash[3];
	int e = ctx.hash[4];

	// Основной цикл:
	for( int i = 0; i < 80; ++i ){
		int f, k;
		if( i >= 0 && i <= 19 ){
			f = ~b & d | b & c;
			k = 0x5A827999;
		}else if( i >= 20 && i <= 39 ){
			f = b ^ c ^ d;
			k = 0x6ED9EBA1;
		}else if( i >= 40 && i <= 59 ){
			f = b & c | d & (b | c);
			k = 0x8F1BBCDC;
		}else if( i >= 60 && i <= 79 ){
			f = b ^ c ^ d;
			k = 0xCA62C1D6;
		}else{
			TerminateWithError( "Wrong SHA1 hash"); // сюда не должны попасть
		}
		int temp = __ROL__(a, 5) + f + e + k + w[i];
		e = d;
		d = c;
		c = __ROL__(b, 30);
		b = a;
		a = temp;
	}

	// Добавляем хеш-значение этой части к результату:
	ctx.hash[0] += a;
	ctx.hash[1] += b;
	ctx.hash[2] += c;
	ctx.hash[3] += d;
	ctx.hash[4] += e;
	memset(w, 0, sizeof w);
}

//*updated*
//----- (0046B737) --------------------------------------------------------
void __fastcall InitSHA1( int part )
{
	//Инициализация переменных:
	Sha1Ctx& ctx = SHA_CTXx3[part];
	ctx.hash[0] = 0x67452301;
	ctx.hash[1] = 0xEFCDAB89;
	ctx.hash[2] = 0x98BADCFE;
	ctx.hash[3] = 0x10325476;
	ctx.hash[4] = 0xC3D2E1F0;
	ctx.count[5] = 0;
	ctx.count[6] = 0;
}
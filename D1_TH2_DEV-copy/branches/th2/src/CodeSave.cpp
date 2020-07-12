
//*updated*
//----- (00403722) --------------------------------------------------------
int __fastcall DecodeSave( uchar* data, uint size, char* key )
{
	uchar block[128];
	uchar hash[SHA1_DIGEST_SIZE];  
	ApplyKey(false, key);
	if( size <= 8 ){
		return 0;
	}
	size -= 8;
	if( size & 63 ){ // не кратно 64
		return 0;
	}
	for( uint blockIndex = 0; blockIndex < size / SHA1_BLOCK_SIZE; blockIndex++, data += SHA1_BLOCK_SIZE ){
		memcpy(block, data, SHA1_BLOCK_SIZE);
		SHA1Done(0, (uint*)hash);
		for( int i = 0; i < SHA1_BLOCK_SIZE; i++ ){
			block[i] ^= hash[i % SHA1_DIGEST_SIZE];
		}
		SHA1RunAndDone(0, block, 0);
		memset(hash, 0, SHA1_DIGEST_SIZE);
		memcpy(data, block, SHA1_BLOCK_SIZE);
	}
	memset(block, 0, sizeof block);
	CodedFileHeader& codedFileHeader = *(CodedFileHeader*)data;
	if( codedFileHeader.field4 ){
		ClearSHAs();
		return 0;
	}
	SHA1Done(0, (uint*)hash);
	if( codedFileHeader.hash != *(uint*)hash ){
		memset(hash, 0, SHA1_DIGEST_SIZE);
		ClearSHAs();
		return 0;
	}
	ClearSHAs();
	return size + codedFileHeader.lastBlockSize - SHA1_BLOCK_SIZE;
}

//*updated*
//----- (0040382D) --------------------------------------------------------
void __fastcall ApplyKey( int isEncode, char* key )
{
	uchar block[136];
	srand(28760);
	for( int i = 0; i < 136; i++ ){
		block[i] = rand();
	}
	uchar dst[SHA1_BLOCK_SIZE];
	int keyIndex;
	int dstIndex;
	for( keyIndex = 0, dstIndex = 0; dstIndex < SHA1_BLOCK_SIZE; keyIndex++, dstIndex++ ){
		if( !key[keyIndex] ){
			keyIndex = 0;
		}
		dst[dstIndex] = key[keyIndex];
	}
	InitSHA1(0);
	uchar hash[SHA1_DIGEST_SIZE];
	SHA1RunAndDone(0, dst, (uint*)hash);
	ClearSHAs();
	for( int i = 0; i < 136; i++ ){
		block[i] ^= hash[i % SHA1_DIGEST_SIZE];
	}
	memset(dst, 0, SHA1_BLOCK_SIZE);
	memset(hash, 0, SHA1_DIGEST_SIZE);
	for( int i = 0; i < 3; i++ ){
		InitSHA1(i);
		SHA1RunAndDone(i, &block[72], 0);
	}
	memset(block, 0, 136);
}

//*updated*
//----- (00403901) --------------------------------------------------------
size_t __fastcall RoundBlockSizeTo64( size_t size )
{
	int loBits = size & 63;
	if( loBits ){
		size += SHA1_BLOCK_SIZE - loBits;
	}
	return size + sizeof(CodedFileHeader);// на самом деле после округления до 64 вверх, добавляется 8 байт заголовка
}

//*updated*
//----- (00403913) --------------------------------------------------------
void __fastcall EncodeSave( uchar* data, uint size, int maxsize, char* key )
{
	if( maxsize != RoundBlockSizeTo64(size) ){
		TerminateWithError("Invalid encode parameters");
	}
	ApplyKey(true, key);
	uchar lastBlockSize = 0;
	uint curBlockSize;
	uchar block[128];
	for( int dataLeft = size; dataLeft; dataLeft -= curBlockSize, data += SHA1_BLOCK_SIZE ){
		curBlockSize = dataLeft;
		if( dataLeft >= 64 ){
			curBlockSize = 64;
		}
		memcpy(block, data, curBlockSize);
		if( curBlockSize < SHA1_BLOCK_SIZE ){
			memset(&block[curBlockSize], 0, SHA1_BLOCK_SIZE - curBlockSize);
		}
		uchar hash1[SHA1_DIGEST_SIZE];
		SHA1Done(0, (unsigned int*)hash1);
		SHA1RunAndDone(0, block, 0);
		for( int i = 0; i < SHA1_BLOCK_SIZE; ++i ){
			block[i] ^= hash1[i % SHA1_DIGEST_SIZE];
		}
		memset(hash1, 0, SHA1_DIGEST_SIZE);
		memcpy(data, block, SHA1_BLOCK_SIZE);
		lastBlockSize = (char)curBlockSize;
	}
	memset(block, 0, sizeof block);
	uchar hash2[SHA1_DIGEST_SIZE];
	SHA1Done(0, (uint*)hash2);
	CodedFileHeader& codedFileHeader = *(CodedFileHeader*)data;
	codedFileHeader.hash = *(uint*)hash2;
	codedFileHeader.field4 = 0;
	codedFileHeader.lastBlockSize = lastBlockSize;
	codedFileHeader.field6 = 0;
	ClearSHAs();
}
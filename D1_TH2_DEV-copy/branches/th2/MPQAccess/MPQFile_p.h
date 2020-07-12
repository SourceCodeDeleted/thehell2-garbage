//////////////////////////////////////////////////////////////////////////
// THE HELL 2 MOD
// MPQAccess project
//
// File: H:\work\th2\MPQAccess\MPQFile_p.h
// Description:
//    MPQFile private class interface.
//	  DO NOT INCLUDE THIS FILE INTO YOUR PROJECT DIRECTLY
//    MPQFile uses pimpl approach to its interface
// Author: spaun2002@pisem.net
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "stdafx_mpqaccess.h"
#include "MPQInterfaces.h"
#include "MPQConsts.h"

namespace MPQAccess { namespace Private {
	class MPQFile_p;
}}

template<> 
void std::swap(MPQAccess::Private::MPQFile_p&, MPQAccess::Private::MPQFile_p&);

namespace MPQAccess {
	namespace Private {

/*

SFILE_INFO_HASH_INDEX	MPQ file handle	32-bit integer	Hash index of the open file.
SFILE_INFO_CODENAME1	MPQ file handle	32-bit integer	The first part of 64-bit code name from the hash table entry.
SFILE_INFO_CODENAME2	MPQ file handle	32-bit integer	The second part of 64-bit code name from the hash table entry.
SFILE_INFO_LOCALEID	MPQ file handle	32-bit integer	Locale ID of the file. For list of locale IDs, see SFileSetLocale.
SFILE_INFO_BLOCKINDEX	MPQ file handle	32-bit integer	Block index of the file in the block table.
SFILE_INFO_FILE_SIZE	MPQ file handle	32-bit integer	Size of uncompressed file, as saved in the block table.
SFILE_INFO_COMPRESSED_SIZE	MPQ file handle	32-bit integer	Size of compressed file, as saved in the block table.
SFILE_INFO_FLAGS	MPQ file handle	32-bit integer	Flags of the file from the block table.
SFILE_INFO_POSITION	MPQ file handle	32-bit integer	Position of the file data in the MPQ archive, relative to the begin of the MPQ file.
SFILE_INFO_KEY	MPQ file handle	32-bit integer	Encryption key of the file. If MPQ_FILE_FIX_KEY is set in the file flags, this is the key value after adjustment.
SFILE_INFO_KEY_UNFIXED	MPQ file handle	32-bit integer	Encryption key of the file. If MPQ_FILE_FIX_KEY is set in the file flags, this is the key value before adjustment.
SFILE_INFO_FILETIME	MPQ file handle	64-bit integer	File time of the file in the MPQ.

*/
typedef struct _tagMPQFileInfo {
	INT32 HashIndex;
	INT32 Codename1;
	INT32 Codename2;
	INT32 LocaleID;
	INT32 BlockIndex;
	INT32 UncompressedFileSize;
	INT32 CompressedFileSize;
	INT32 Flags;
	INT32 Position;
	INT32 Key;
	INT32 KeyUnfixed;
	INT64 FileTime;
} MPQ_FILE_INFO, *PMPQ_FILE_INFO;


//MPQFile_p is private implementation for MPQFile class.
//Do not include this file into your cpp files and don't try to access this class direcly
//Author: spaun2002
//Date: 19 June 2011
class MPQFile_p
{
public:
	MPQFile_p(const IMPQHandleContainer& arc, const std::string& fileName, const OpenFileMode mode);
	MPQFile_p(const IMPQHandleContainer& arc, const DWORD index);
	MPQFile_p(MPQFile_p&&);
	~MPQFile_p(void);
	bool Check() const;
	__declspec(property(get=GetFileSize)) INT64 FileSize;
	INT64 GetFileSize() const;
	void swap(MPQFile_p& _right) throw();
	MPQFile_p& operator=(MPQFile_p&&) throw();
	DWORD SetFileOffset(INT64 offset, DWORD moveMethod);
	DWORD Read(void *buffer, DWORD size);
	__declspec(property(get=GetFileName)) std::string FileName;
	std::string GetFileName() const;
	const MPQ_FILE_INFO& GetFileInfo() const;
	__declspec(property(get=GetIsLocalFile)) const bool IsLocalFile;
	const bool GetIsLocalFile() const;
private:
	//non-copyable
	MPQFile_p(const MPQFile_p&);
	MPQFile_p& operator=(const MPQFile_p&);
	bool GetFileInfo(PMPQ_FILE_INFO pfi);
	bool GetInfoChunk(DWORD infoType, void *buffer, size_t size);
private:
	HANDLE _handle;
	DWORD _lastError;
	bool _wasOpened;
	INT64 _fileSize;
	bool _isLocalFile;
	std::unique_ptr<MPQ_FILE_INFO> _fileInfo;
};

} //namespace Private
} //namespace MPQAccess

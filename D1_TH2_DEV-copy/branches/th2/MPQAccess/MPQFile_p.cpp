//////////////////////////////////////////////////////////////////////////
// THE HELL 2 MOD
// MPQAccess project
//
// File: H:\work\th2\MPQAccess\MPQFile_p.cpp
// Description:
//    MPQFile private class implementation
// Author: spaun2002@pisem.net
//////////////////////////////////////////////////////////////////////////
#include "MPQFile_p.h"

namespace MPQAccess { namespace Private {

MPQFile_p::MPQFile_p(const IMPQHandleContainer& arc, const std::string& fileName, const OpenFileMode mode)
{
	_lastError = ERROR_SUCCESS;
	_fileSize = -1;
	if (!(_wasOpened = SFileOpenFileEx(arc.getHandle(), 
		                               fileName.c_str(), 
									   static_cast<DWORD>(mode), 
									   &_handle)))
	{
		_lastError = GetLastError();
	}
	_isLocalFile = mode == OpenLocal;
	if (!_isLocalFile){
		_fileInfo.reset(new MPQ_FILE_INFO);
		GetFileInfo(_fileInfo.get());
	}
}

MPQFile_p::MPQFile_p( const IMPQHandleContainer& arc, const DWORD index )
{
	_lastError = ERROR_SUCCESS;
	_fileSize = -1;
	if (!(_wasOpened = SFileOpenFileEx(arc.getHandle(), 
		                               reinterpret_cast<const char*>(index), 
									   static_cast<DWORD>(OpenIndexed), 
									   &_handle)))
	{
		_lastError = GetLastError();
	}
	_isLocalFile = false;
	_fileInfo.reset(new MPQ_FILE_INFO);
	GetFileInfo(_fileInfo.get());
}

MPQFile_p::MPQFile_p( MPQFile_p&& _right)
{
	this->_handle = _right._handle;
	this->_lastError = _right._lastError;
	this->_wasOpened = _right._wasOpened;
	this->_fileSize = _right._fileSize;
	this->_fileInfo = std::move(_right._fileInfo);
	_right._handle = NULL;
}


MPQFile_p::~MPQFile_p(void)
{
	if (_wasOpened && NULL != _handle)
		SFileCloseFile(_handle);
}

bool MPQFile_p::Check() const 
{
	return _lastError == ERROR_SUCCESS && _wasOpened;
}

INT64 MPQFile_p::GetFileSize() const
{
	if (Check() && _fileSize == -1){
		DWORD hi = 0;
		DWORD lo = SFileGetFileSize(_handle, &hi);
		const_cast<MPQFile_p*>(this)->_fileSize = (static_cast<INT64>(hi) << 32) + lo;
	}
	return _fileSize;
}

void MPQFile_p::swap(MPQFile_p& _right)
{
	using std::swap;
	swap(this->_handle, _right._handle);
	swap(this->_lastError, _right._lastError);
	swap(this->_wasOpened, _right._wasOpened);
	swap(this->_fileSize, _right._fileSize);
	swap(this->_fileInfo, _right._fileInfo);
}

MPQFile_p& MPQFile_p::operator=( MPQFile_p&& _right)
{
	if (this != &_right){
		using std::swap;
		MPQFile_p tmpf(std::move(_right));
		swap(*this, tmpf);
	}
	return *this;
}

DWORD MPQFile_p::SetFileOffset(INT64 offset, DWORD moveMethod)
{
	if (Check()){
		LARGE_INTEGER li;
		li.QuadPart = offset;
		LONG hiPos = li.HighPart;
		
		li.LowPart = SFileSetFilePointer(_handle, li.LowPart, &hiPos, moveMethod);
		if (li.LowPart == SFILE_INVALID_SIZE){
			_lastError = GetLastError();
		}
		return li.LowPart;
	}
	return SFILE_INVALID_SIZE;
}

DWORD MPQFile_p::Read(void *buffer, DWORD size)
{
	if (Check()){
		DWORD bytesRead = 0;
		if (!SFileReadFile(_handle, buffer, size, &bytesRead, NULL)){
			if (GetLastError() != ERROR_HANDLE_EOF){
				_lastError = GetLastError();
				return -1;
			}
		}
		return bytesRead;
	}
	return -1;
}

std::string MPQFile_p::GetFileName() const
{
	if (Check()){
		std::unique_ptr<char[]> buffer(new char[MAX_PATH]);
		if (!SFileGetFileName(_handle, buffer.get())){
			const_cast<MPQFile_p*>(this)->_lastError = GetLastError();
			return std::string();
		}
		return std::string(buffer.get());
	}
	return std::string();
}

bool MPQFile_p::GetInfoChunk(DWORD infoType, void *buffer, size_t size)
{
	DWORD lenNeeded = 0;
	if (SFileGetFileInfo(_handle, infoType, buffer, size, &lenNeeded))
		return true;

	_lastError = GetLastError();
	return false;
}

bool MPQFile_p::GetFileInfo(PMPQ_FILE_INFO pfi)
{
	//Accroding to implementation of SFileGetFileInfo in stormLib we must not specify cbFileInfo. 
	//Otherwise we may get ERROR_INSUFFICIENT_BUFFER error
	return Check() && !IsLocalFile  
		&& GetInfoChunk(SFILE_INFO_HASH_INDEX, &pfi->HashIndex, 0)
		&&GetInfoChunk(SFILE_INFO_CODENAME1, &pfi->Codename1, 0)
		&&GetInfoChunk(SFILE_INFO_CODENAME2, &pfi->Codename2, 0)
		&&GetInfoChunk(SFILE_INFO_LOCALEID, &pfi->LocaleID, 0)
		&&GetInfoChunk(SFILE_INFO_BLOCKINDEX, &pfi->BlockIndex, 0)
		&&GetInfoChunk(SFILE_INFO_FILE_SIZE, &pfi->UncompressedFileSize, 0)
		&&GetInfoChunk(SFILE_INFO_COMPRESSED_SIZE, &pfi->CompressedFileSize, 0)
		&&GetInfoChunk(SFILE_INFO_FLAGS, &pfi->Flags, 0)
		&&GetInfoChunk(SFILE_INFO_POSITION, &pfi->Position, 0)
		&&GetInfoChunk(SFILE_INFO_KEY, &pfi->Key, 0)
		&&GetInfoChunk(SFILE_INFO_KEY_UNFIXED, &pfi->KeyUnfixed, 0)
		&&GetInfoChunk(SFILE_INFO_FILETIME, &pfi->FileTime, 0);
}

const MPQ_FILE_INFO& MPQFile_p::GetFileInfo() const
{
	if (IsLocalFile){
		throw std::exception("Unable to get information for local file");
	}
	if (Check()){
		return *_fileInfo;
	}
	throw std::exception("Unable to get file information");
}

const bool MPQFile_p::GetIsLocalFile() const
{
	return _isLocalFile;
}

}//namespace Private
}//namespace MPQAccess

template<> void std::swap(MPQAccess::Private::MPQFile_p& _left, 
	MPQAccess::Private::MPQFile_p& _right)
{
	_left.swap(_right);
}
#include "MPQArchive_p.h"

namespace MPQAccess {
	namespace Private {

MPQArchive_p::MPQArchive_p(const std::string& fileName)
{
	_lastError = ERROR_SUCCESS;
	if (!(_wasOpened = SFileOpenArchive(fileName.c_str(), 0, MPQ_OPEN_READ_ONLY, &_handle))){
		_lastError = GetLastError();
	}
}


MPQArchive_p::~MPQArchive_p(void)
{
	if (_wasOpened){
		SFileCloseArchive(_handle);
	}
}

const HANDLE MPQArchive_p::getHandle() const
{
	return _handle;
}

const bool MPQArchive_p::Check() const
{
	return _wasOpened && _lastError == ERROR_SUCCESS;
}

LCID MPQArchive_p::GetDefaultLocale()
{
	return SFileGetLocale();
}

void MPQArchive_p::SetDefaultLocale( LCID locale )
{
	SFileSetLocale(locale);
}

void MPQArchive_p::AddPatch( const std::string& fileName, const std::string& prefix )
{
	if (!_wasOpened) 
		return;
	if (!(SFileOpenPatchArchive(_handle, fileName.c_str(), prefix.c_str(), 0))){
		_lastError = GetLastError();
	}
}

bool MPQArchive_p::HasPatches() const
{
	return _wasOpened && SFileIsPatchedArchive(_handle);
}

bool MPQArchive_p::IsFileExists( const std::string& fileName ) const
{
	bool b = _wasOpened && SFileHasFile(_handle, fileName.c_str());
	if (!b && GetLastError() != ERROR_FILE_NOT_FOUND){
		const_cast<MPQArchive_p*>(this)->_lastError = GetLastError();
	} else {
		const_cast<MPQArchive_p*>(this)->_lastError = ERROR_SUCCESS;
	}
	return b;
}

std::unique_ptr<LCID[]> MPQArchive_p::EnumFileLocales( const std::string& fileName )
{
	DWORD dwSize = 0;
	
	if (_wasOpened && ERROR_INSUFFICIENT_BUFFER == (_lastError = SFileEnumLocales(_handle, fileName.c_str(), NULL, &dwSize, SFILE_OPEN_FROM_MPQ))){
		std::unique_ptr<LCID[]> locales(new LCID[dwSize]);
		_lastError = SFileEnumLocales(_handle, fileName.c_str(), locales.get(), &dwSize, SFILE_OPEN_FROM_MPQ);
		if (ERROR_SUCCESS == _lastError)
			return locales;
	}
	return NULL;
}

std::unique_ptr<LCID[]> MPQArchive_p::EnumFileLocales( int fileIndex )
{
    //we can't get locales form index-open file due to restrictions of stormlib
    throw std::exception("Not implemented");
}



}//namespace Private
}//namespace MPQAccess
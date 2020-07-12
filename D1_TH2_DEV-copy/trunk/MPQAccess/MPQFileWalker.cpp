#include "MPQFileWalker.h"

namespace MPQAccess{

MPQFileWalker::MPQFileWalker(const IMPQHandleContainer& arc, const std::string& fileName )
{
	if (!(_handle = SFileFindFirstFile(arc.getHandle(), fileName.c_str(), &_fd, NULL))){
		throw std::exception("Unable to walk files list");
	}
}

MPQFileWalker::MPQFileWalker(const IMPQHandleContainer& arc, const std::string& fileName, LCID locale )
{
	throw std::exception("NotImplemented");
}

MPQFileWalker::~MPQFileWalker()
{
	CloseHandle();
}

void MPQFileWalker::CloseHandle()
{
	if (_handle){
		SFileFindClose(_handle);
		_handle = NULL;
	}
}

std::string MPQFileWalker::operator*()
{
	return const_cast<const MPQFileWalker*>(this)->operator*();
}

const std::string MPQFileWalker::operator*() const
{
	if (_handle){
		return std::string(_fd.cFileName);
	}
	return std::string();
}

bool MPQFileWalker::FindNext()
{
	if (_handle)
		return SFileFindNextFile(_handle, &_fd);
	return false;
}

bool MPQFileWalker::FindNext( LCID locale )
{
	throw std::exception("NotImplemented");
	return false;
}

} //namespace MPQAccess
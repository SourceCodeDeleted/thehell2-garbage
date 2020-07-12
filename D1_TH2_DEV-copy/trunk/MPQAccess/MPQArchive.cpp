//////////////////////////////////////////////////////////////////////////
// THE HELL 2 MOD
// MPQAccess project
//
// File: MPQArchive.cpp
// Description:
//    MPQArchive class implementation
// Author: spaun2002@pisem.net
//////////////////////////////////////////////////////////////////////////


#include "MPQArchive.h"
#include "MPQArchive_p.h"

namespace MPQAccess {

MPQArchive::MPQArchive(const std::string &fileName)
{
	impl.reset(new Private::MPQArchive_p(fileName));
}


MPQArchive::~MPQArchive(void)
{
}

const HANDLE MPQArchive::getHandle() const
{
	return impl->getHandle();
}

MPQAccess::MPQFile MPQArchive::GetFile( const std::string& fileName )
{
	if (!impl->Check())
		throw std::runtime_error("Archive is in bad state");
	return MPQFile(*this, fileName);
}

MPQAccess::MPQFile MPQArchive::GetFileEx( const std::string& fileName, OpenFileMode mode )
{
	if (!impl->Check())
		throw std::runtime_error("Archive is in bad state");
	return MPQFile(*this, fileName, mode);
}

bool MPQArchive::IsFileExists( const std::string& fileName )
{
	return (impl->Check() && impl->IsFileExists(fileName));
}

}//namespace MPQAccess
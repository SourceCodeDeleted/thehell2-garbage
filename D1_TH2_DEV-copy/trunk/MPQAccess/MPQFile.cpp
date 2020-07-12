//////////////////////////////////////////////////////////////////////////
// THE HELL 2 MOD
// MPQAccess project
//
// File: MPQFile.cpp
// Description:
//    MPQFile class implementation
// Author: spaun2002@pisem.net
//////////////////////////////////////////////////////////////////////////
#include "MPQFile.h"
#include "MPQFile_p.h"

namespace MPQAccess{

MPQFile::MPQFile( MPQFile&& r)
{
	impl = std::move(r.impl);
}

MPQFile::MPQFile( const IMPQHandleContainer &archive, const std::string& fileName)
{
	impl.reset(new Private::MPQFile_p(archive, fileName, OpenFileMode::OpenLocal));
	if (!impl->Check())
		impl.reset(new Private::MPQFile_p(archive, fileName, OpenFileMode::OpenFromMPQ));
}

MPQFile::MPQFile( const IMPQHandleContainer &archive, const std::string& fileName, OpenFileMode mode )
{
	impl.reset(new Private::MPQFile_p(archive, fileName, mode));
}

MPQFile::MPQFile( const MPQFile& r)
{
    impl = r.impl;
}

MPQFile& MPQFile::operator=( const MPQFile& r)
{
	using std::swap;
	MPQFile tmp(r);
	swap(*this, tmp);
	return *this;
}

MPQFile& MPQFile::operator=( MPQFile&& r)
{
	if (this != &r){
		using std::swap;
		MPQFile tmp(std::move(r));
		swap(*this, tmp);
	}
	return *this;
}

void MPQFile::swap( MPQFile& r)
{
	using std::swap;
	swap(impl, r.impl);
}

bool MPQFile::Check() const
{
	return impl->Check();
}

DWORD MPQFile::Read( char* buffer, size_t size )
{
	if (!impl->Check())
		return -1;
	return impl->Read(buffer, size);
}

std::unique_ptr<char[]> MPQFile::ReadWholeFile()
{
	if (!impl->Check())
		throw std::runtime_error("File is in bad state");
	size_t fsize = impl->GetFileSize();
	std::unique_ptr<char[]> buffer(new char[fsize]);
	DWORD br = impl->Read(buffer.get(), fsize);
	if (br != fsize)
		throw std::runtime_error("Unable to read whole file into memory");
	return std::move(buffer);
}

}//namespace MPQAccess

template<> 
void std::swap(MPQAccess::MPQFile& left, MPQAccess::MPQFile& right)
{
	left.swap(right);
}

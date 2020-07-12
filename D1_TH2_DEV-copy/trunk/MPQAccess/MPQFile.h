//////////////////////////////////////////////////////////////////////////
// THE HELL 2 MOD
// MPQAccess project
//
// File: MPQFile.h
// Description:
//    MPQFile class interface
//    MPQFile uses pimpl approach to its interface so most of internals are
//      hidden into MPQFile_p class
// Author: spaun2002@pisem.net
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __MPQACCESS_MPQFILE_HEADER__
#define __MPQACCESS_MPQFILE_HEADER__

#include "stdafx_mpqaccess.h"
#include "MPQFwdDcls.h"
#include "MPQArchive.h"
#include "MPQConsts.h"

namespace MPQAccess {
	class MPQFile;

namespace Private {
	class MPQFile_p;	//Forward declaration for private implementation. Do not try to access this class directly
}}

template<> 
void std::swap(MPQAccess::MPQFile&, MPQAccess::MPQFile&);

namespace MPQAccess {

//MPQFile represent one file inside MPQ archive. Not MPQ archive itself
class MPQFile
{
public:
	explicit MPQFile(const IMPQHandleContainer &archive, const std::string& fileName);
	explicit MPQFile(const IMPQHandleContainer &archive, const std::string& fileName, OpenFileMode mode);
	MPQFile(const MPQFile&);
	MPQFile(MPQFile&&);
	MPQFile& operator=(const MPQFile&);
	MPQFile& operator=(MPQFile&&);
	void swap(MPQFile&);
	bool Check() const;
	DWORD Read(char* buffer, size_t size);
	std::unique_ptr<char[]> ReadWholeFile();
private:
	std::tr1::shared_ptr<Private::MPQFile_p> impl;
};

}//namespace MPQAccess

#endif //__MPQACCESS_MPQFILE_HEADER__
//////////////////////////////////////////////////////////////////////////
// THE HELL 2 MOD
// MPQAccess project
//
// File: MPQArchive.h
// Description:
//    MPQArchive class interface
// Author: spaun2002@pisem.net
//////////////////////////////////////////////////////////////////////////


#pragma once

#ifndef __MPQACCESS_MPQARCHIVE_HEADER__
#define __MPQACCESS_MPQARCHIVE_HEADER__

#include "stdafx_mpqaccess.h"
#include "MPQFwdDcls.h"
#include "MPQInterfaces.h"
#include "MPQFile.h"
#include "MPQConsts.h"


namespace MPQAccess {

	namespace Private {
		class MPQArchive_p;
	}

class MPQArchive : public IMPQHandleContainer
{
public:
	MPQArchive(const std::string &fileName);
	~MPQArchive(void);
	MPQAccess::MPQFile GetFile(const std::string& fileName);
	MPQAccess::MPQFile GetFileEx(const std::string& fileName, OpenFileMode mode);
	bool IsFileExists(const std::string& fileName);
	const HANDLE getHandle() const;
private:
	std::tr1::shared_ptr<Private::MPQArchive_p> impl;
};

}//namespace MPQAccess

#endif //__MPQACCESS_MPQARCHIVE_HEADER__
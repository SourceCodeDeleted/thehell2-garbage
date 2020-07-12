#pragma once
#include "stdafx_mpqaccess.h"
#include "MPQInterfaces.h"

namespace MPQAccess{

class MPQFileWalker
{
public:
	explicit MPQFileWalker(const IMPQHandleContainer& arc, const std::string& fileName);
	MPQFileWalker(const IMPQHandleContainer& arc, const std::string& fileName, LCID locale);
	~MPQFileWalker();
	
	std::string operator*();
	const std::string operator*() const;

	bool FindNext();
	bool FindNext(LCID locale);

private:
	//non-copyable
	MPQFileWalker(const MPQFileWalker&);
	MPQFileWalker& operator=(const MPQFileWalker&);
	void CloseHandle();
private:
	HANDLE _handle;
	SFILE_FIND_DATA _fd;
};

}//namespace MPQAccess
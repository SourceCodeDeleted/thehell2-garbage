#pragma once
#include "stdafx_mpqaccess.h"
#include <string>
namespace MPQAccess {
	namespace Private {

class MPQArchive_p
{
public:
	explicit MPQArchive_p(const std::string& fileName);
	~MPQArchive_p(void);
	const HANDLE getHandle() const;
	const bool Check() const;

	static LCID GetDefaultLocale();
	static void SetDefaultLocale(LCID locale);

	void AddPatch(const std::string& fileName, const std::string& prefix);
	bool HasPatches() const;

	bool IsFileExists(const std::string& fileName) const;

	std::unique_ptr<LCID[]> EnumFileLocales(const std::string& fileName);
	std::unique_ptr<LCID[]> EnumFileLocales(int fileIndex);

private:
	//non-copyable
	MPQArchive_p(const MPQArchive_p&);
	MPQArchive_p& operator=(const MPQArchive_p&);
private:
	HANDLE _handle;
	DWORD _lastError;
	bool _wasOpened;
};

	} //namespace Private
}//namespace MPQAccess
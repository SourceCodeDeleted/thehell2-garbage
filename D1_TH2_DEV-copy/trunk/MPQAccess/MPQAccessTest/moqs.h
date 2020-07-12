#include <string>
#include <CppUnitTest.h>
#include "stdafx_mpqaccess.h"
#include "MPQFile_p.h"

class MoqMPQArchive : public MPQAccess::IMPQHandleContainer
{
public:
	MoqMPQArchive(const std::string& fileName) : m_handle(NULL){
		if (!SFileOpenArchive(fileName.c_str(), 0, MPQ_OPEN_READ_ONLY, &m_handle)){
			std::string buf("Unable to open archive ");
			buf += fileName + "\n";
      Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(buf.c_str());
			throw std::exception(buf.c_str());
		}
	}
	~MoqMPQArchive() {
		if (NULL != m_handle){
			SFileCloseArchive(m_handle);
		}
	}
	virtual const HANDLE getHandle() const
	{
		return m_handle;
	}
private:
	HANDLE m_handle;
};
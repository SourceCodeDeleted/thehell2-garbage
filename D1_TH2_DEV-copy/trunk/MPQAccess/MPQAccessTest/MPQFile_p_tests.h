//Набор тестов для класса MPQFile_p
#include "../MPQFile_p.h"
#include <string>

class MoqMPQArchive : public MPQAccess::IMPQHandleContainer
{
public:
	MoqMPQArchive(const std::string& fileName) : m_handle(NULL){
		if (!SFileOpenArchive(fileName.c_str(), 0, MPQ_OPEN_READ_ONLY, &m_handle)){
			std::string buffer;
			buffer.reserve(255);
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), 0, buffer._Myptr(), buffer.size(), NULL);
			throw std::exception(buffer.c_str());
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


struct Test_OpenFileByName
{
public:
	void operator()()
	{
		MoqMPQArchive arch("testarchive.mpq");
		using MPQAccess::Private::MPQFile_p;
		MPQFile_p file(arch, "TestFile.txt", MPQAccess::Private::OpenFromMPQ);
		if (!file.Check())
			throw std::exception("Unable to open file TestFile.txt");
		MPQFile_p file2(arch, "testfile2.txt", MPQAccess::Private::OpenFromMPQ);
		if (!file.Check())
			throw std::exception("Unable to open file testfile2.txt");
	}
};
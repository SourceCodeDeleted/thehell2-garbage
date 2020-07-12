#include <CppUnitTest.h>
#include <vector>
#include "MPQFile_p.h"
#include "moqs.h"

extern std::string modulePath;
extern std::string previousWorkingDir;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ListFile_tests)
{
public:
  TEST_METHOD_INITIALIZE(Open_file_by_nameInitialize)
  {
    SetCurrentDirectory(modulePath.c_str());
  }

  TEST_METHOD_CLEANUP(Open_file_by_nameCleanup)
  {
    SetCurrentDirectory(previousWorkingDir.c_str());
  }

  TEST_METHOD(Open_listfile)
  {
    MoqMPQArchive arch("HELLFIRE.MPQ");
    using MPQAccess::Private::MPQFile_p;
    MPQFile_p fl(arch, "(listfile)", MPQAccess::OpenFromMPQ);
    Assert::IsTrue(fl.Check(), L"Unable to open (listfile)", LINE_INFO());
    size_t fileSize = fl.FileSize;
    std::vector<char> buf(fileSize);
    fl.Read(&buf[0], fileSize*sizeof(char));
    std::string content(&buf[0]);
    Logger::WriteMessage(content.c_str());
  }
};

#include <CppUnitTest.h>
#include "MPQArchive.h"

extern std::string modulePath;
extern std::string previousWorkingDir;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(MPQArchive_tests)
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

  TEST_METHOD(MPQArchive_GetFile)
  {
    using namespace MPQAccess;
    MPQArchive marc("HELLFIRE.MPQ");
    MPQFile mfile = marc.GetFile("Towners\\Girl\\Girls2.CEL");
    mfile = marc.GetFile("Towners\\Girl\\Girls1.CEL");
    Assert::IsTrue(mfile.Check(), L"Unable to open file Towners\\Girl\\Girls1.CEL", LINE_INFO());
  }
};
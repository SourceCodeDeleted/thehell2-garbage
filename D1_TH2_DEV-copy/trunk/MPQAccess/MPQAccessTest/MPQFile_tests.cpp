#include <CppUnitTest.h>
#include "MPQFile.h"
#include "moqs.h"

extern std::string modulePath;
extern std::string previousWorkingDir;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(MPQFile_tests)
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


  TEST_METHOD(MPQFile_ReadWholeFile)
  {
    using namespace MPQAccess;	
    MoqMPQArchive arc("HELLFIRE.MPQ");
    MPQAccess::MPQFile file(arc, "Towners\\Girl\\Girls1.CEL" );
    Assert::IsTrue(file.Check(), L"Unable to open Towners\\Girl\\Girls1.CEL", LINE_INFO());
    auto buffer = file.ReadWholeFile();
    Assert::IsNotNull(buffer.get(), L"Unable to read whole file into memory", LINE_INFO());
  }
};

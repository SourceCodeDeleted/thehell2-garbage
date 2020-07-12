#include <CppUnitTest.h>
#include <windows.h>
#include "MPQArchive_p.h"

extern std::string modulePath;
extern std::string previousWorkingDir;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(MPQArchive_p_tests)
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

  class EnumFileLocalesFunctor
  {
  public:
    explicit EnumFileLocalesFunctor(MPQAccess::Private::MPQArchive_p& arc, 
      std::unique_ptr<LCID[]>& locs, int index)
      : m_arc(arc), m_locs(locs), m_index(index)
    {
    }
    void operator()()
    {
      m_locs = m_arc.EnumFileLocales(m_index);
    }
  private:
    MPQAccess::Private::MPQArchive_p& m_arc;
    std::unique_ptr<LCID[]>& m_locs;
    int m_index;
  };

  TEST_METHOD(Open_archive)
  {
    using namespace MPQAccess::Private;
    MPQArchive_p arc("testarchive.mpq");
    Assert::IsTrue(arc.Check(), L"Unable to open archive testarchive.mpq", LINE_INFO());
    MPQArchive_p::SetDefaultLocale(0x419);
    LCID locale = MPQArchive_p::GetDefaultLocale();
    Assert::IsTrue(locale == 0x419, L"Unable to set or get locale", LINE_INFO());
    Assert::IsFalse(arc.HasPatches(), L"This archive shouldn't have any patches", LINE_INFO());
    Assert::IsTrue(arc.IsFileExists("TestFile.txt"), L"Unable to find TestFile1.txt", LINE_INFO());
    Assert::IsTrue(arc.IsFileExists("TestFile2.txt"), L"Unable to find TestFile2.txt", LINE_INFO());
    Assert::IsFalse(arc.IsFileExists("missedfile.txt"), L"There is no file like missedfile.txt in the archive", LINE_INFO());
    auto f1Locs = arc.EnumFileLocales("TestFile.txt");
    std::unique_ptr<LCID[]> f2Locs;
    Assert::ExpectException<std::exception>(EnumFileLocalesFunctor(arc, f2Locs, 0), 
      L"Getting locales by file index should throw an exception", LINE_INFO());
    Assert::IsFalse(f1Locs.get() == NULL, L"Unable to get locales by file name", LINE_INFO());
    Assert::IsTrue(f2Locs.get() == NULL, L"GetLocales by index started to return value", LINE_INFO());
  }
};

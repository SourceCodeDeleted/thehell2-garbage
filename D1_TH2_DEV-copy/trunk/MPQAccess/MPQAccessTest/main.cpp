//Набор тестов для класса MPQFile_p
#include <CppUnitTest.h>
#include <string>
#include <time.h>
#include <errno.h>
#include <sstream>
#include <iomanip>
#include <windows.h>
#include <vector>
#include "MPQFile_p.h"
#include "moqs.h"
#include "MPQConsts.h"

std::string modulePath;
std::string previousWorkingDir;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
  if (DLL_PROCESS_ATTACH == fdwReason)
  {
    previousWorkingDir.resize(MAX_PATH+1);
    GetCurrentDirectory(previousWorkingDir.size()-1, &previousWorkingDir[0]);

    modulePath.resize(MAX_PATH+1);
    GetModuleFileName(hinstDLL, &modulePath[0], modulePath.size()-1 );
    size_t i = modulePath.find_last_of("\\");
    if (modulePath.npos != i)
    {
      modulePath.erase(i);
    }
  } 

  return TRUE;
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(MPQ_File_tester)
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

  TEST_METHOD(Open_file_by_name)
  {
    MoqMPQArchive arch("testarchive.mpq");
    using MPQAccess::Private::MPQFile_p;
    MPQFile_p file(arch, "TestFile.txt", MPQAccess::OpenFromMPQ);
    Assert::IsTrue(file.Check(), L"Unable to open file TestFile.txt", LINE_INFO());
    MPQFile_p file2(arch, "testfile2.txt", MPQAccess::OpenFromMPQ);
    Assert::IsTrue(file2.Check(), L"Unable to open file testfile2.txt", LINE_INFO());
  }
  TEST_METHOD(Open_File_by_index)
  {
    MoqMPQArchive arch("testarchive.mpq");
    using MPQAccess::Private::MPQFile_p;
    MPQFile_p file(arch, 2);
    Assert::IsTrue(file.Check(), L"Unable to open file by index", LINE_INFO());
    const std::string fileName = file.FileName;
    Assert::IsFalse(fileName.empty(), L"Unable to get filename from indexed file", LINE_INFO());
    std::stringstream ss;
    std::string message;
    ss << "\tOpened file name: " << fileName << "\n";
    message = ss.str();
    Logger::WriteMessage(message.c_str());
    Assert::AreEqual("TestFile.txt", fileName.c_str());
    const MPQAccess::Private::MPQ_FILE_INFO fi = file.GetFileInfo();
    ss.clear();
    ss << "\tBlock index: " << fi.BlockIndex << "\n";
    ss << "\tCodename1: " << fi.Codename1 << "\n";
    ss << "\tCodename2: " << fi.Codename2 << "\n";
    ss << "\tCompressed file size: " << fi.CompressedFileSize << "\n";
    message = ss.str();
    Logger::WriteMessage(message.c_str());
    __time64_t fileTime = fi.FileTime;
    struct tm time;
    _gmtime64_s(&time, &fileTime);

    ss.clear();
    ss << "\tFile time: " << std::setw(2) << time.tm_mday << "//" << time.tm_mon << "//" 
      << std::setw(4) << time.tm_year << " " << std::setw(2) << time.tm_hour << ":" << time.tm_min << ":" 
      << time.tm_sec << "\n";
    ss << "\tFlags: " << std::hex << fi.Flags << "\n";
    ss << "\tHash index: " << std::dec << fi.HashIndex << "\n";
    ss << "\tKey: " << fi.Key << "\n";
    ss << "\tKey unfixed: " << fi.KeyUnfixed << "\n";
    ss << "\tLocalID: " << fi.LocaleID << "\n";
    ss << "\tPosition: " << fi.Position << "\n";
    ss << "\tUncompressed file size: " << fi.UncompressedFileSize << "\n";
    message = ss.str();
    Logger::WriteMessage(message.c_str());
  }

  TEST_METHOD(Read_whole_file)
  {
    MoqMPQArchive arch("HELLFIRE.MPQ");
    using MPQAccess::Private::MPQFile_p;
    MPQFile_p fl(arch, 0);
    Assert::IsTrue(fl.Check(), L"Unable to open indexed", LINE_INFO());
    const std::string fileName = fl.FileName;
    Assert::IsFalse(fileName.empty(), L"Unable to get file name from indexed file", LINE_INFO());
    std::stringstream ss;
    ss << "\tOpened file name: " << fileName << "\n";
    std::string message(ss.str());
    Logger::WriteMessage(message.c_str());
    int fileSize = static_cast<int>(fl.FileSize);
    std::vector<char> buffer(fileSize);
    fl.SetFileOffset(0, FILE_BEGIN);
    Assert::IsTrue(fl.Check(), L"Unable to set file offset", LINE_INFO());
    fl.Read(&buffer[0], fileSize*sizeof(char));
    Assert::IsTrue(fl.Check(), L"Unable to read data from buffer", LINE_INFO());
    Logger::WriteMessage("\tBuffer's readed\n");
  }

  TEST_METHOD(Read_file_in_pieces)
  {
    const int bufferSize = 1024;
    MoqMPQArchive arch("HELLFIRE.MPQ");
    using MPQAccess::Private::MPQFile_p;
    MPQFile_p testF(arch, 0);
    Assert::IsTrue(testF.Check(), L"Unable to open indexed file", LINE_INFO());
    int fileSize = testF.FileSize;
    std::vector<char> buffer(bufferSize);
    testF.SetFileOffset(0,0);
    std::stringstream ss;
    std::string message;
    int bytesRead = 0;
    while((bytesRead = testF.Read(&buffer[0], sizeof(char)*bufferSize)) > 0){
      ss.clear();
      ss << "\t" << bytesRead << " bytes has been read\n";
      message = ss.str();
      Logger::WriteMessage(message.c_str());
    }
    Assert::IsTrue(testF.Check(), L"Error happend during file reading", LINE_INFO());
  }

  TEST_METHOD(Open_notexisted_file)
  {
    MoqMPQArchive arch("HELLFIRE.MPQ");
    using MPQAccess::Private::MPQFile_p;
    MPQFile_p testF(arch, "SomeShitFile.txt", MPQAccess::OpenFromMPQ);
    Assert::IsFalse(testF.Check(), L"Somehow managed to open not existed file", LINE_INFO());
  }

  class GetFileInfoFunc
  {
  public:
    explicit GetFileInfoFunc(MPQAccess::Private::MPQFile_p& file)
      : m_file(file)
    {
    }
    const MPQAccess::Private::MPQ_FILE_INFO& operator()() const 
    {
      return m_file.GetFileInfo();
    }
  private:
    MPQAccess::Private::MPQFile_p& m_file;
  };

  TEST_METHOD(Open_file_from_outside_the_archive)
  {
    const int bufSize = 10;
    MoqMPQArchive arch("HELLFIRE.MPQ");
    using MPQAccess::Private::MPQFile_p;
    MPQFile_p testF(arch, "TestFile2.txt", MPQAccess::OpenLocal);
    Assert::IsTrue(testF.Check(), L"Unable to open file from outside the archive", LINE_INFO());
    Assert::IsTrue(testF.IsLocalFile, L"File must be local", LINE_INFO());
    int fileSize = testF.FileSize;
    Assert::IsTrue(fileSize > 0, L"Unable to get filesize for local file", LINE_INFO());
    std::stringstream ss;
    ss << "\tFile size: " << fileSize << " bytes\n";
    std::string message(ss.str());
    Logger::WriteMessage(message.c_str());
    std::vector<char> buf(bufSize);
    int bReaded = 0;
    while ((bReaded = testF.Read(&buf[0], bufSize*sizeof(char))) > 0){
      ss.clear();
      ss << "\t" << bReaded << " bytes read\n";
      message = ss.str();
      Logger::WriteMessage(message.c_str());
    }
    Assert::ExpectException<std::exception>(GetFileInfoFunc(testF), L"GetFileInfo shouldn't try to get info for local files", LINE_INFO());
    ss << "\tFile name: " << testF.FileName << "\n";
    message = ss.str();
    Logger::WriteMessage(message.c_str());
  }
};






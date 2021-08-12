// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <tchar.h>
#include <iostream>

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/filesystem.hpp>

static const std::wstring ksIMESuffix = _T(".ime");
static const std::wstring ksPDBSuffix = _T(".pdb");

int _tmain(int argc, TCHAR* argv[]) {
  namespace po = boost::program_options;
  po::options_description desc;
  desc.add_options()("imePath", po::wvalue<std::wstring>(), "ime path")(
      "imeName", po::wvalue<std::wstring>(), "ime name");
  po::variables_map vm;
  po::basic_parsed_options<wchar_t> parsed =
      po::wcommand_line_parser(argc, argv).options(desc).run();
  po::store(parsed, vm);

  std::wstring imePath = vm["imePath"].as<std::wstring>();
  std::wstring imeName = vm["imeName"].as<std::wstring>();

  if (imeName.empty() || imePath.empty()) {
    BOOST_LOG_TRIVIAL(error) << _T("There Must be a imeName and imePath!");
    return -1;
  }

  TCHAR szPath[MAX_PATH] = {0};
  if (!GetCurrentDirectory(MAX_PATH, szPath)) {
    BOOST_LOG_TRIVIAL(error) << _T("GetCurrentDirectory Failed!");
    return -1;
  }
  imePath = szPath;

  boost::filesystem::path fpIMEPath{imePath};
  boost::filesystem::path fpPDBPath{imePath};
  fpIMEPath /= imeName + ksIMESuffix;
  fpPDBPath /= imeName + ksPDBSuffix;

  memset(szPath, 0, sizeof(szPath));
  if (!GetSystemDirectory(szPath, MAX_PATH)) {
    BOOST_LOG_TRIVIAL(error) << _T("System dir not found!");
    std::wcout << _T("error")
               << _T("System dir not found!");
    return -1;
  }
  boost::filesystem::path fpSysIMEPath{szPath};
  boost::filesystem::path fpSysPDBPath{szPath};
  fpSysIMEPath /= imeName + ksIMESuffix;
  fpSysPDBPath /= imeName + ksPDBSuffix;

  if (!CopyFile(fpIMEPath.c_str(), fpSysIMEPath.c_str(), FALSE)) {
    DWORD last_error = GetLastError();
    BOOST_LOG_TRIVIAL(error)
        << _T("Copy IME To: ") << fpSysIMEPath.wstring() << _T("Failed!");
    return -1;
  }
  if (!CopyFile(fpPDBPath.c_str(), fpSysPDBPath.c_str(), FALSE)) {
    DWORD last_error = GetLastError();
    BOOST_LOG_TRIVIAL(error)
        << _T("Copy PDB To: ") << fpSysPDBPath.wstring() << _T("Failed!");
    return -1;
  }

  ImmInstallIME(fpSysIMEPath.c_str(), imeName.c_str());

  return 0;
}

int _tmain_bk(int argc, TCHAR* argv[]) {
  namespace po = boost::program_options;
  po::options_description desc;
  desc.add_options()("imePath", po::wvalue<std::wstring>(), "ime path")(
      "imeName", po::wvalue<std::wstring>(), "ime name");
  po::variables_map vm;
  po::basic_parsed_options<wchar_t> parsed =
      po::wcommand_line_parser(argc, argv).options(desc).run();
  po::store(parsed, vm);

  std::wstring imePath = vm["imePath"].as<std::wstring>();
  std::wstring imeName = vm["imeName"].as<std::wstring>();

  TCHAR szPath[MAX_PATH] = {0};
  if (!GetSystemDirectory(szPath, MAX_PATH)) {
    BOOST_LOG_TRIVIAL(error) << _T("System dir not found!");
    return -1;
  }
  std::wstring systemDir = szPath;
  BOOST_LOG_TRIVIAL(trace) << _T("System dir :") << systemDir;

  size_t pos = imePath.rfind('\\');
  std::wstring imeFileName;
  if (pos != std::wstring::npos) {
    imeFileName = imePath.substr(pos);
  } else {
    imeFileName = imePath;
  }

  std::wstring imeFilePathInSys = systemDir;
  imeFilePathInSys += _T("\\") + imeFileName;

  // DWORD dwRetVal = GetTempFileName(systemDir.c_str(), _T("tempime"), 0,
  // szPath); if (dwRetVal == 0) { DWORD dwError = GetLastError();
  //  BOOST_LOG_TRIVIAL(error)<<_T("Failed to generate temp file name.(Error
  //  Code=") <<dwError<<_T(")\n"); return -1;
  //}
  // std::wstring tempImeFilePath = szPath;
  // BOOST_LOG_TRIVIAL(trace) << _T("temp file name:") << tempImeFilePath;
  // MoveFile(imeFilePathInSys.c_str(), tempImeFilePath.c_str());
  // MoveFileEx(tempImeFilePath.c_str(), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);

  // BOOST_LOG_TRIVIAL(trace) << _T("[move file]") << imeFilePathInSys <<
  // _T("=>") << tempImeFilePath;
  if (!CopyFile(imePath.c_str(), imeFilePathInSys.c_str(), FALSE)) {
    DWORD last_error = GetLastError();
    BOOST_LOG_TRIVIAL(trace)
        << _T("[move file]") << imePath << _T("=>") << imeFilePathInSys
        << _T(". Failed! last_error: ") << last_error;
  }

  ImmInstallIME(imeFilePathInSys.c_str(), imeName.c_str());

  return 0;
}
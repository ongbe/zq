// stdafx.cpp : source file that includes just the standard includes
// AddIn.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "ToolsEx.h"
#include <windows.h>

// winrar压缩文件的方法

//  -ibck 后台运行winrar

// 普通调用winrar压缩的命令行
// winrar.exe a -as -r test.rar winrar

// 不压缩*\formats\*目录下的所有文件
// winrar.exe a -as -r -x*\formats\* test.rar winrar
// winrar.exe a -as -r -x*\formats\* -x*.sfx -x*.ion -x*.diz test.rar winrar

// 指定压缩为zip格式
// winrar.exe a -as -r -afzip -x*\formats\* -x*.sfx -x*.ion -x*.diz test.zip winrar
// winrar.exe a -as -r -afzip -ep1 -x*.pdb -x*.cache kdj.zip kdj\*


#pragma unmanaged


// 压缩指定工程
bool CToolsEx::Zip_Project(const std::string& strHomePath, const std::string& strWorkPath, 
				  const std::string& strProjectName, std::string& strRetZipFile)
{
	char csRunWinRar[1024] = {0};
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};

	sprintf_s(csRunWinRar, sizeof(csRunWinRar)-1, "%swinrar.exe a -as -r -afzip -ep1 -x*.pdb -x*.cache -x*.dll \"%s.zip\" \"%s\\*\"", 
			strHomePath.c_str(), strProjectName.c_str(), strProjectName.c_str());

    ZeroMemory( &si, sizeof(si) );
    ZeroMemory( &pi, sizeof(pi) );
    si.cb = sizeof(si);

    if( !CreateProcess( NULL,			// No module name (use command line)
			csRunWinRar,				// Command line
			NULL,						// Process handle not inheritable
			NULL,						// Thread handle not inheritable
			FALSE,						// Set handle inheritance to FALSE
			0,							// No creation flags
			NULL,						// Use parent's environment block
			strWorkPath.c_str(),		// Use parent's starting directory 
			&si,						// Pointer to STARTUPINFO structure
			&pi )						// Pointer to PROCESS_INFORMATION structure
			) {
        return false;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

	strRetZipFile = strWorkPath;
	strRetZipFile += strProjectName;
	strRetZipFile += ".zip";

	return true;
}

// 解压缩指定zip
bool CToolsEx::UnZip_Project(const std::string& strHomePath, const std::string& strWorkPath, 
				  const std::string& strProjectName, std::string& strRetZipFile, bool bReplace)
{
	char csRunWinRar[1024] = {0};
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};

	if(bReplace)
		sprintf_s(csRunWinRar, sizeof(csRunWinRar)-1, "%swinrar.exe x -y \"%s.zip\" \"%s\\\"", 
				strHomePath.c_str(), strProjectName.c_str(), strProjectName.c_str());
	else
		sprintf_s(csRunWinRar, sizeof(csRunWinRar)-1, "%swinrar.exe x \"%s.zip\" \"%s\\\"", 
				strHomePath.c_str(), strProjectName.c_str(), strProjectName.c_str());

    ZeroMemory( &si, sizeof(si) );
    ZeroMemory( &pi, sizeof(pi) );
    si.cb = sizeof(si);

    if( !CreateProcess( NULL,			// No module name (use command line)
			csRunWinRar,				// Command line
			NULL,						// Process handle not inheritable
			NULL,						// Thread handle not inheritable
			FALSE,						// Set handle inheritance to FALSE
			0,							// No creation flags
			NULL,						// Use parent's environment block
			strWorkPath.c_str(),		// Use parent's starting directory 
			&si,						// Pointer to STARTUPINFO structure
			&pi )						// Pointer to PROCESS_INFORMATION structure
			) {
        return false;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

	return true;
}

// 压缩指定工程
bool CToolsEx::Zip_Files(const std::string& strHomePath, const std::string& strWorkPath, 
						 const std::vector<std::string>& vecFileName, const std::string& strZipFileName)
{
	char csFilesParam[1024] = {0};
	char csRunWinRar[1024+256] = {0};
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};

	for(int i=0; i<vecFileName.size(); i++) {
		char csTemp[64] = {0};
		sprintf_s(csTemp, sizeof(csTemp)-1, "%s ", vecFileName[i].c_str());
		strcat_s(csFilesParam, sizeof(csFilesParam)-1, csTemp);
	}
	sprintf_s(csRunWinRar, sizeof(csRunWinRar)-1, "%swinrar.exe a -as -afzip \"%s\" %s", 
			strHomePath.c_str(), strZipFileName.c_str(), csFilesParam);

    ZeroMemory( &si, sizeof(si) );
    ZeroMemory( &pi, sizeof(pi) );
    si.cb = sizeof(si);

    if( !CreateProcess( NULL,			// No module name (use command line)
			csRunWinRar,				// Command line
			NULL,						// Process handle not inheritable
			NULL,						// Thread handle not inheritable
			FALSE,						// Set handle inheritance to FALSE
			0,							// No creation flags
			NULL,						// Use parent's environment block
			strWorkPath.c_str(),		// Use parent's starting directory 
			&si,						// Pointer to STARTUPINFO structure
			&pi )						// Pointer to PROCESS_INFORMATION structure
			) {
        return false;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

	return true;
}

// 解压缩指定zip
bool CToolsEx::UnZip_File(const std::string& strHomePath, const std::string& strWorkPath, 
				  const std::string& strFileName, bool bIncludePath)
{
	char csRunWinRar[1024] = {0};
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};

	if(bIncludePath)
		sprintf_s(csRunWinRar, sizeof(csRunWinRar)-1, "%s\\winrar.exe x -y \"%s\"", 
				strHomePath.c_str(), strFileName.c_str());
	else
		sprintf_s(csRunWinRar, sizeof(csRunWinRar)-1, "%s\\winrar.exe e -y \"%s\"", 
				strHomePath.c_str(), strFileName.c_str());

    ZeroMemory( &si, sizeof(si) );
    ZeroMemory( &pi, sizeof(pi) );
    si.cb = sizeof(si);

    if( !CreateProcess( NULL,			// No module name (use command line)
			csRunWinRar,				// Command line
			NULL,						// Process handle not inheritable
			NULL,						// Thread handle not inheritable
			FALSE,						// Set handle inheritance to FALSE
			0,							// No creation flags
			NULL,						// Use parent's environment block
			strWorkPath.c_str(),		// Use parent's starting directory 
			&si,						// Pointer to STARTUPINFO structure
			&pi )						// Pointer to PROCESS_INFORMATION structure
			) {
        return false;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

	return true;
}

void CToolsEx::SearchFiles(const std::string& strPath, const std::string strStdPath, 
						   std::set<std::string>& setFiles)
{
	std::string strFind(strPath);
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwTemp = 0;
	WIN32_FIND_DATA findFileData = {0};

	strFind += "*.*";
	hFind = FindFirstFile(strFind.c_str(), &findFileData);
	if(hFind == INVALID_HANDLE_VALUE) 
		return;

	// List all the other files in the directory.
	FindNextFile(hFind, &findFileData);
	while(FindNextFile(hFind, &findFileData)) {
		dwTemp = findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
		if(dwTemp) {
			std::string strNewPath(strPath);
			strNewPath += findFileData.cFileName;
			strNewPath += "\\";
			std::string strNewStdPath(strStdPath);
			strNewStdPath += findFileData.cFileName;
			strNewStdPath += "\\";
			SearchFiles(strNewPath, strNewStdPath, setFiles);
			continue;
		}
		dwTemp = findFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
		if(dwTemp) {
			std::string strFile(strStdPath);
			strFile += findFileData.cFileName;
			setFiles.insert(strFile);
			continue;
		}
	}
	FindClose(hFind);
}

void CToolsEx::DeleteAllFolder(const std::string& strFolder)
{
	std::string strFind(strFolder);
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwTemp = 0;
	WIN32_FIND_DATA findFileData = {0};

	strFind += "*.*";
	hFind = FindFirstFile(strFind.c_str(), &findFileData);
	if(hFind == INVALID_HANDLE_VALUE) 
		return;

	// List all the other files in the directory.
	FindNextFile(hFind, &findFileData);
	while(FindNextFile(hFind, &findFileData)) {
		dwTemp = findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
		if(dwTemp) {
			std::string strNewFolder(strFolder);
			strNewFolder += findFileData.cFileName;
			strNewFolder += "\\";
			DeleteAllFolder(strNewFolder);
			::RemoveDirectory(strNewFolder.c_str());
			continue;
		}
		dwTemp = findFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
		if(dwTemp) {
			std::string strFile(strFolder);
			strFile += findFileData.cFileName;
			::DeleteFile(strFile.c_str());
			continue;
		}
	}
	FindClose(hFind);

	::RemoveDirectory(strFolder.c_str());
	return;
}

void CToolsEx::CopyFolderFiles(const std::string& strSrcFolder, const std::string& strTgtFolder)
{
	std::string strFind(strSrcFolder);
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwTemp = 0;
	WIN32_FIND_DATA findFileData = {0};

	strFind += "*.*";
	hFind = FindFirstFile(strFind.c_str(), &findFileData);
	if(hFind == INVALID_HANDLE_VALUE) 
		return;

	// List all the other files in the directory.
	FindNextFile(hFind, &findFileData);
	while(FindNextFile(hFind, &findFileData)) {
		dwTemp = findFileData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE;
		if(dwTemp) {
			std::string strSrcFile(strSrcFolder), strTgtFile(strTgtFolder);
			strSrcFile += findFileData.cFileName;
			strTgtFile += findFileData.cFileName;
			::CopyFile(strSrcFile.c_str(), strTgtFile.c_str(), FALSE);
		}
	}
	FindClose(hFind);

	return;
}

void CToolsEx::GetIntersection(const std::set<std::string>& set1, 
							   const std::set<std::string>& set2, 
							   std::set<std::string>& retSet)
{
	std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), 
				inserter(retSet, retSet.begin()));
}

bool CToolsEx::IsExistFile(const std::string& strFile)
{
	HANDLE hFile = CreateFile(strFile.c_str(),				// file to open
                       GENERIC_READ,						// open for reading
                       FILE_SHARE_READ,						// share for reading
                       NULL,								// default security
                       OPEN_EXISTING,						// existing file only
                       FILE_ATTRIBUTE_NORMAL,				// normal file
                       NULL);								// no attr. template
	if(hFile==INVALID_HANDLE_VALUE) {
		// 指定的文件打开失败。
		return false;
	}

	DWORD dwFileLenHigh = 0;
	DWORD dwFileLen = ::GetFileSize(hFile, &dwFileLenHigh);
	::CloseHandle(hFile);

	return dwFileLen>0;
}

#pragma managed


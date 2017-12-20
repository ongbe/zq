// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include <string>

#pragma unmanaged

class CToolsEx
{
public:
	// 压缩指定工程
	static bool Zip_Project(const std::string& strHomePath, const std::string& strWorkPath, 
						const std::string& strProjectName, std::string& strRetZipFile);
	// 解压缩指定zip
	static bool UnZip_Project(const std::string& strHomePath, const std::string& strWorkPath, 
						const std::string& strProjectName, std::string& strRetZipFile, bool bReplace=false);

	// 压缩多个文件
	static bool Zip_Files(const std::string& strHomePath, const std::string& strWorkPath, 
						const std::vector<std::string>& vecFileName, const std::string& strZipFileName);
	// 解压缩指定zip
	static bool UnZip_File(const std::string& strHomePath, const std::string& strWorkPath, 
						const std::string& strFileName, bool bIncludePath=false);

	static void SearchFiles(const std::string& strPath, const std::string strStdPath, 
			std::set<std::string>& setFiles);

	static void DeleteAllFolder(const std::string& strFolder);
	static void CopyFolderFiles(const std::string& strSrcFolder, const std::string& strTgtFolder);

	static void GetIntersection(const std::set<std::string>& set1, 
							    const std::set<std::string>& set2, 
							    std::set<std::string>& retSet);

	static bool IsExistFile(const std::string& strFile);

};

#pragma managed

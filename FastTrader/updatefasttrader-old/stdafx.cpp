// stdafx.cpp : source file that includes just the standard includes
// updatefasttrader.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
//得到配置文件路径
void GetPath(std::string &filePath)
{
	char localPath[256];
	memset(localPath, 0, 256);

	GetModuleFileName( NULL, localPath, 256);
	for( int n = strlen(localPath); n > 0; n-- )
	{
		if( localPath[n-1] == '\\' ) 
		{
			localPath[n] = NULL; 
			break;
		}
	}

	std::stringstream ss;

	ss<<localPath;
	ss<<filePath;

	filePath = ss.str();	
}
void str_replace(std::string & str, const std::string & strsrc, const std::string &strdst) 
{ 
	std::string::size_type pos = 0;//位置 
	std::string::size_type srclen = strsrc.size();//要替换的字符串大小 
	std::string::size_type dstlen = strdst.size();//目标字符串大小 
	while((pos = str.find(strsrc,pos)) != std::string::npos) 
	{ 
		str.replace(pos,srclen,strdst); 
		pos += dstlen; 
	} 
}
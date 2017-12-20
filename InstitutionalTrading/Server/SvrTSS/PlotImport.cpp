
#include "PlotImport.h"
#include <windows.h>


CPlotImport::CPlotImport(void)
{
}

CPlotImport::~CPlotImport(void)
{
}

bool CPlotImport::Init(vector<PLOTCFGITEM>& vecPlotData)
{
	LockObject();
	m_mapPlotStatus.clear();
	for(int i=0; i<vecPlotData.size(); i++) {
		CPlotItem item;
		item.strPlotID = vecPlotData[i].plotID;
		item.strPath = vecPlotData[i].path;
		item.dwRealInval = vecPlotData[i].ReadInval;
		m_mapPlotStatus.insert(make_pair(vecPlotData[i].plotID, item));
	}
	UnlockObject();
	return true;
}

void CPlotImport::Release()
{
	LockObject();
	m_mapPlotStatus.clear();
	UnlockObject();
}

#if 1
bool CPlotImport::ImportPlot(queue<IMPORTPLOTDIRECT>& queuePlot)
{
	HANDLE hFile = NULL;
	MAPPLOTSTATUS::iterator it;
	DWORD dwCurrTick = ::GetTickCount();

	LockObject();
	for(it = m_mapPlotStatus.begin(); it != m_mapPlotStatus.end(); it++) {
		
		// 判读时间间隔是否满足要求
		if((dwCurrTick - it->second.dwPrevReadTime)/1000 < it->second.dwRealInval)
			continue;

		string strFile(it->second.strPath);

		hFile = ::CreateFile(strFile.c_str(),		// file to open
						   GENERIC_READ,			// open for reading
						   FILE_SHARE_READ|FILE_SHARE_WRITE,	// share for reading
						   NULL,					// default security
						   OPEN_EXISTING,			// existing file only
						   FILE_ATTRIBUTE_NORMAL,	// normal file
						   NULL);					// no attr. template
		if(hFile == INVALID_HANDLE_VALUE) {
#if 1
			// 考虑到读网络文件的问题，打开失败可能会是很长的时间，所以不再考虑尝试再打开一次。
			continue;
#else
			// 文件打开无效，尝试重新打开一次
			//printf("OpenFile [%s] is Fail!\n", strFile.c_str());
			Sleep(50);
			hFile = ::CreateFile(strFile.c_str(),		// file to open
							   GENERIC_READ,			// open for reading
							   FILE_SHARE_READ|FILE_SHARE_WRITE,	// share for reading
							   NULL,					// default security
							   OPEN_EXISTING,			// existing file only
							   FILE_ATTRIBUTE_NORMAL,	// normal file
							   NULL);					// no attr. template
			if(hFile == INVALID_HANDLE_VALUE) {
				//printf("After Sleep(50) OpenFile [%s] is Fail!\n", strFile.c_str());
				continue;
			}
#endif
		}

		DWORD dwFileSize = 0, dwFileHighSize = 0;
		DWORD dwBytesRead = 0;
		DWORD dwLastPos = 0;
		
		dwLastPos = it->second.dwPrevReadPos;

		dwFileSize = ::GetFileSize(hFile, &dwFileHighSize);
		int dwReadLen = dwFileSize - dwLastPos;
		if(dwReadLen==0) {
			//printf("File [%s] 文件长度[%u], 可读长度为0\n", strFile.c_str(), fileInfo.nFileSizeLow);
			CloseHandle(hFile);
			continue;
		}

		char* pBuff = new char[dwReadLen+4];

		ZeroMemory(pBuff, dwReadLen+4);

		//printf("file [%s], seek to [%u].\n", strFile.c_str(), dwLastPos);
		::SetFilePointer(hFile, dwLastPos, NULL, FILE_BEGIN);
		if(!::ReadFile(hFile, pBuff, dwReadLen, &dwBytesRead, NULL)) {
			::CloseHandle(hFile);
			delete pBuff;
			pBuff = NULL;
			continue;
		}
		::CloseHandle(hFile);

		dwLastPos += dwBytesRead;
		it->second.dwPrevReadPos = dwLastPos;
		it->second.dwPrevReadTime = dwCurrTick;
		
		// 把策略文本解析成为map数据格式
		ParsePlotData(pBuff, queuePlot);
		delete pBuff;
		pBuff = NULL;

	}
	UnlockObject();

	return true;
}
#else
bool CPlotImport::ImportPlot(queue<INSTPLOTDIRECT>& queueRet)
{
	string strFind(m_strReadPath);
	HANDLE hFile = NULL;
	BY_HANDLE_FILE_INFORMATION fileInfo;

	WIN32_FIND_DATA ffData = {0};
	strFind+="*.*";
	HANDLE hFind = FindFirstFile(strFind.c_str(), &ffData);
	if(hFind == INVALID_HANDLE_VALUE) 
		return FALSE;
	do {
		if(ffData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			continue;
		string strFile = m_strReadPath;
		strFile += ffData.cFileName;

		hFile = ::CreateFile(strFile.c_str(),		// file to open
						   GENERIC_READ,			// open for reading
						   FILE_SHARE_READ|FILE_SHARE_WRITE,	// share for reading
						   NULL,					// default security
						   OPEN_EXISTING,			// existing file only
						   FILE_ATTRIBUTE_NORMAL,	// normal file
						   NULL);					// no attr. template
		if(hFile == INVALID_HANDLE_VALUE) {
			//printf("OpenFile [%s] is Fail!\n", strFile.c_str());
			Sleep(50);
			hFile = ::CreateFile(strFile.c_str(),		// file to open
							   GENERIC_READ,			// open for reading
							   FILE_SHARE_READ|FILE_SHARE_WRITE,	// share for reading
							   NULL,					// default security
							   OPEN_EXISTING,			// existing file only
							   FILE_ATTRIBUTE_NORMAL,	// normal file
							   NULL);					// no attr. template
			if(hFile == INVALID_HANDLE_VALUE) {
				//printf("After Sleep(50) OpenFile [%s] is Fail!\n", strFile.c_str());
				continue;
			}
		}

		ZeroMemory(&fileInfo, sizeof(fileInfo));
		if(!GetFileInformationByHandle(hFile, &fileInfo))
			continue;

		DWORD dwBytesRead = 0;
		DWORD dwLastPos = 0;
		MAPFILEPOS::iterator it = m_mapFilePos.find(strFile);
		if(it != m_mapFilePos.end())
			dwLastPos = it->second;

		int dwReadLen = fileInfo.nFileSizeLow-dwLastPos;
		if(dwReadLen==0) {
			//printf("File [%s] 文件长度[%u], 可读长度为0\n", strFile.c_str(), fileInfo.nFileSizeLow);
			continue;
		}

		char* pBuff = new char[dwReadLen+4];

		ZeroMemory(pBuff, dwReadLen+4);

		//printf("file [%s], seek to [%u].\n", strFile.c_str(), dwLastPos);
		::SetFilePointer(hFile, dwLastPos, NULL, FILE_BEGIN);
		if(!::ReadFile(hFile, pBuff, dwReadLen, &dwBytesRead, NULL)) {
			::CloseHandle(hFile);
			continue;
		}
		::CloseHandle(hFile);

		dwLastPos += dwBytesRead;
		m_mapFilePos[strFile] = dwLastPos;
		
		// 把策略文本解析成为map数据格式
		ParsePlotData(pBuff, queueRet);

	} while(FindNextFile(hFind, &ffData));
	FindClose(hFind);

	return true;
}
#endif

BOOL CPlotImport::ParsePlotData(char* pBuff, queue<IMPORTPLOTDIRECT>& queuePlot)
{
	char* pHead = pBuff;
	char* pHeadNext = NULL;
	do {
		pHeadNext = GetPlotItem(pHead, queuePlot);
		if(pHeadNext!=NULL)
			pHead = pHeadNext;
	} while(pHeadNext!=NULL);
	return TRUE;
}

char* CPlotImport::GetPlotItem(char* csText, queue<IMPORTPLOTDIRECT>& queuePlot)
{
	if(csText==NULL)
		return NULL;

	int nItemLen = 0;
	char* pCurrItem = NULL;
	char* pNextItem = NULL;

	pCurrItem = csText;
	do 
	{
		while(pCurrItem[nItemLen]!='\r' && pCurrItem[nItemLen]!='\000')
		{
			nItemLen++;
		}
		if(pCurrItem[nItemLen]=='\000')
			break;
		if(nItemLen>=20)
			break;
		pCurrItem = pCurrItem+nItemLen+2;
	} while(pCurrItem[nItemLen]!='\000');

	if(pCurrItem[nItemLen]=='\r')
		pNextItem = pCurrItem+nItemLen+2;

	if(nItemLen<20)
		return pNextItem;

	char* csItem = new char[nItemLen+8];
	char csCodes[128];

	ZeroMemory(csItem, nItemLen+8);
	ZeroMemory(csCodes, sizeof(csCodes));
	strncpy(csItem, csText, nItemLen);

	IMPORTPLOTDIRECT plotDirect = {0};
	char csDateTime[32] = {0};

	int nRet = sscanf_s(csItem, "%[^,],%[^,],%[^,],%s", 
			csDateTime, sizeof(csDateTime)-1, 
			plotDirect.plotID, sizeof(plotDirect.plotID)-1, 
			plotDirect.instID, sizeof(plotDirect.instID)-1,
			plotDirect.direct, sizeof(plotDirect.direct)-1);
	_strlwr(plotDirect.direct);
	delete[] csItem;
	if(nRet != 4) {
		//printf("sscanf_s() return != 4\n");
		return pNextItem;
	}
	//printf("sscanf(), %s, %s, %s, %d\n", csDateTime, csPlotID, csInstrumentID, nDirectMode);
	
	queuePlot.push(plotDirect);

	return pNextItem;
}

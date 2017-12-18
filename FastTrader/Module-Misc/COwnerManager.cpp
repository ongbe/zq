
#include "StdAfx.h"
#include "COwnerManager.h"
#include "..\\inc\\Module-Misc2\\GlobalFunc.h"
#include "wx/file.h"
#include "wx/filename.h"


COwnerManager::COwnerManager()
{
}

COwnerManager::~COwnerManager() 
{
	SaveDataToFile();
}

bool COwnerManager::Init(std::string& strAccount, std::string& strTradingDay)
{
	m_strAccount = strAccount;
	m_strTradingDay = strTradingDay;
	m_setOwner.clear();
	return LoadDataFromFile();
}

bool COwnerManager::AppendOwner(int nFrontID, int nSessionID)
{
	__int64 nOwner = nFrontID;
	nOwner = nOwner<<32;
	nOwner += nSessionID;
	LockObject();
	m_setOwner.insert(nOwner);
	UnlockObject();
	return SaveDataToFile();
}

bool COwnerManager::HasOwner(int nFrontID, int nSessionID)
{
	__int64 nOwner = nFrontID;
	nOwner = nOwner<<32;
	nOwner += nSessionID;
	bool bHasFound = false;
	LockObject();
	bHasFound = (m_setOwner.find(nOwner)!=m_setOwner.end());
	UnlockObject();
	return bHasFound;
}

bool COwnerManager::LoadDataFromFile()
{
	std::string strAppPath;
    GlobalFunc::GetPath(strAppPath);
    strAppPath+="userdata\\";

	FILEHEAD tFileHead={0};

	wxString strFileName;
	
	strFileName = strAppPath;
	strFileName += wxT(m_strAccount.c_str());
	strFileName += wxT(".ses");
	wxFileName filename(strFileName); 
	if(!filename.FileExists())
		return FALSE;

	wxFile file;
	file.Open(strFileName);
	if(!file.IsOpened())
		return FALSE;

	file.Read(&tFileHead, sizeof(FILEHEAD));

	// 如果不是当前交易日，则不加载数据
	if(strcmp(tFileHead.csTradingDay, m_strTradingDay.c_str()) != 0) {
		file.Close();
		return TRUE;
	}

	__int64 nSessionKey = 0;

	LockObject();

	while(file.Read(&nSessionKey, sizeof(nSessionKey))==sizeof(nSessionKey)) {
		m_setOwner.insert(nSessionKey);
	}
	
	UnlockObject();

	file.Close();

	return TRUE;
}

bool COwnerManager::SaveDataToFile()
{
    // 创建目录
	std::string strAppPath;
    GlobalFunc::GetPath(strAppPath);
    strAppPath+="userdata\\";
    if(!wxDirExists(strAppPath.c_str())) 
		wxMkdir(strAppPath);

	FILEHEAD tFileHead={0};

	wxFile file;
	DWORD dwPos=0;

    wxString strFileName;

	
	strFileName = strAppPath;
	strFileName += wxT(m_strAccount.c_str());
	strFileName += wxT(".ses");
	file.Create(strFileName, true);
	if(!file.IsOpened()) {
		//wxMessageDialog dialog( NULL,LOADSTRING(SaveFail_DataFile_MESSAGE) ,LOADSTRING(SaveFail_Title),wxOK|wxICON_ERROR);
		//dialog.ShowModal();
		return FALSE;
	}

	strncpy(tFileHead.csTradingDay, m_strTradingDay.c_str(), sizeof(tFileHead.csTradingDay)-1);

	file.Write(&tFileHead, sizeof(FILEHEAD));

	LockObject();

	for(SETOWNER::iterator it=m_setOwner.begin(); it!=m_setOwner.end(); ++it) {
		file.Write(&(*it), sizeof(*it));
	}

	UnlockObject();

	//file.Seek(0);
	//file.Write(&tFileHead, sizeof(FILEHEAD));

	file.Close();

	return TRUE;
}

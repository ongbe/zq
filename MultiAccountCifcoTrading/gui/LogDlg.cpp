
#include "stdafx.h"
#include <list>
#include "LogDlg.h"
#include "../inc/Module-Misc/SimpleWriteLog.h"
//class IPlatformSvr;
extern PlatformSvrMgr* g_pPlatformMgr;

BEGIN_EVENT_TABLE(LogDlg,wxDialog)

EVT_BUTTON(wxID_EXIT,LogDlg::OnExit)
	EVT_SIZE(LogDlg::OnSize)
	EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_CHANGE, OnLanguageChanged)
END_EVENT_TABLE()

LogDlg* LogDlg::m_pLogDlg = NULL;

void LogDlg::OnExit( wxCommandEvent& event)
{
	this->Hide();
}

LogDlg* LogDlg::GetInstance()
{
	if( m_pLogDlg == NULL )
	{
		return NULL;
	}
	return m_pLogDlg;
}


LogDlg* LogDlg::CreateLogDlg(wxWindow* parent)
{
	m_pLogDlg = new LogDlg(parent);
	return m_pLogDlg;
}

LogDlg::LogDlg(wxWindow *parent):
wxDialog(parent, wxID_ANY,LOADSTRING(LOG_DLG_TITLE),wxDefaultPosition, wxSize(550,350),wxDEFAULT_DIALOG_STYLE|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxDIALOG_MODELESS )
,m_pList(NULL)
{
	DIALOG_CREATE_LOG();
	AUTO_SETSIZE();	
	m_pList = new wxExtListCtrl(this,wxID_ANY,wxDefaultPosition,wxSize(550,350),wxLCEXT_REPORT|wxLCEXT_VRULES);
	InitListCtrl(true);
}

wxString LogDlg::GetLastMsg()
{
	wxString strText;
	int row = m_pList->GetItemCount();
	if(row>0)
	{
		strText=m_pList->GetSubItemText(row-1,3);
	}
	return strText;
}

void LogDlg::InitListCtrl(bool bInit)
{
#define SETCOLTEXT(col,strid)\
	if(bInit) m_pList->InsertColumn(col,LOADSTRING(strid));\
	else      m_pList->SetColumn(col,LOADSTRING(strid) )

	SETCOLTEXT(0,LOG_LEVEL);
	SETCOLTEXT(1,LOG_TYPE);
	SETCOLTEXT(2,LOG_TIME);
	SETCOLTEXT(3,LOG_MSG);
	m_pList->SetColumnWidth(0,50);
	m_pList->SetColumnWidth(1,80);
	m_pList->SetColumnWidth(2,200);
	m_pList->SetColumnWidth(3,260);

	m_pList->SetEvenRowColour( wxColor(255,255,245) );
	m_pList->SetOddRowColour( wxColor(240,240,240) );
}

void LogDlg::WriteLog(std::string strType, LOG_DATA* pData, wxString strTime/* =wxEmptyString */)
{
	string strMsg = GetLogString( pData );
	strMsg = REPLACEFMTSTRING(strMsg);
	int item= m_pList->GetItemCount();
	item=m_pList->InsertItem(item, LOADSTRING(LOG_MSG));
	m_pList->SetItem(item, 1, LOADSTRING_TEXT(strType.c_str()) );
	m_pList->SetItem(item, 3, strMsg );

	wxStatusBar* pBar=(wxStatusBar*)GetParent();
	pBar->SetStatusText(strMsg,1);

	SYSTEMTIME lp;
	wxString strFirstPane;
	if(strTime.IsEmpty())
	{
		GetLocalTime(&lp); 
		char buf[64];
		sprintf(buf,LOG_TIME_FORMAT_STRING,lp.wYear,lp.wMonth,lp.wDay,lp.wHour,lp.wMinute,lp.wSecond,lp.wMilliseconds);
		m_pList->SetItem(item, 2, buf );
		strFirstPane.Printf(TIME_FORMAT,lp.wHour,lp.wMinute,lp.wSecond);
	}
	else
	{
		m_pList->SetItem(item, 2, strTime );
		if(strTime.Length()>18)
			strFirstPane=strTime.SubString(11,18);
		else
			strFirstPane=strTime;
	} 
	pBar->SetStatusText(strFirstPane);

	LogInfoMulti* pLogInfo = new LogInfoMulti();
	strcpy(pLogInfo->strType,strType.c_str());
	pLogInfo->pData = pData;
	m_pList->SetItemData(item,(long)pLogInfo);
}

bool GetDiffSeconds(SYSTEMTIME* t1, SYSTEMTIME* t2)  
{  
	FILETIME fTime1 = { 0, 0 };  
	FILETIME fTime2 = { 0, 0 };  
	SystemTimeToFileTime(t1, &fTime1);  
	SystemTimeToFileTime(t2, &fTime2);  
	return CompareFileTime(&fTime1,&fTime2)>0;
}
void LogDlg::OnSize(wxSizeEvent& event)
{
	if(m_pList)
	{
		int colwidth=event.GetSize().x-360;
		int index=m_pList->GetColumnCount()-1;
		if(colwidth<260) colwidth=260;
		m_pList->SetColumnWidth(index,colwidth);
	}
	event.Skip();

}

void LogDlg::OnLanguageChanged( wxCommandEvent& event )
{
	SetTitle(LOADSTRING(LOG_DLG_TITLE));
	InitListCtrl(false);
	wxString strMsg;
	for ( int i = 0; i <m_pList->GetItemCount(); i++)
	{
		LogInfoMulti* pInfo=(LogInfoMulti*)m_pList->GetItemData(i);
		if(pInfo)
		{
			strMsg = GetLogString( pInfo->pData );
			strMsg = REPLACEFMTSTRING(strMsg.c_str());
			m_pList->SetItem(i, 0, LOADSTRING(LOG_MSG));
			m_pList->SetItem( i, 1, LOADSTRING_TEXT(pInfo->strType));
			m_pList->SetItem( i, 3, strMsg);
		}
	}
	if(!strMsg.IsEmpty())
	{
		wxStatusBar* pBar = (wxStatusBar*)GetParent();
		pBar->SetStatusText(strMsg.c_str(), 1);
	}
}

std::string LogDlg::GetLogString( LOG_DATA* pData )
{
	if ( NULL == pData )
	{
		return "";
	}

	string strTitle;
	if ( !pData->strTitle.empty())
	{
		strTitle = LOADSTRING_TEXT(pData->strTitle);
		strTitle = strTitle + ":";
	}

	string strMsg;
	if ( !pData->strFormat.empty())
	{
		if ( pData->vParam.empty())
		{
			strMsg = LOADSTRING_TEXT(pData->strFormat);
		}
		else
		{
			char szBuff[1024];
			string strTempFormat = LOADSTRING_TEXT(pData->strFormat);
			memset( szBuff, 0 ,sizeof(szBuff));
			int nSize = pData->vParam.size();
			switch ( nSize )
			{
			case 1:
				sprintf( szBuff, strTempFormat.c_str(), GetLogParam(pData->vParam.at(0)).c_str());
				break;
			case 2:
				sprintf( szBuff, strTempFormat.c_str(), GetLogParam(pData->vParam.at(0)).c_str(), 
					GetLogParam(pData->vParam.at(1)).c_str());
				break;
			case 3:
				sprintf( szBuff, strTempFormat.c_str(), GetLogParam(pData->vParam.at(0)).c_str(), 
					GetLogParam(pData->vParam.at(1)).c_str(),
					GetLogParam(pData->vParam.at(2)).c_str());
				break;
			case 4:
				sprintf( szBuff, strTempFormat.c_str(), GetLogParam(pData->vParam.at(0)).c_str(), 
					GetLogParam(pData->vParam.at(1)).c_str(),
					GetLogParam(pData->vParam.at(2)).c_str(),
					GetLogParam(pData->vParam.at(3)).c_str());
				break;
			case 5:
				sprintf( szBuff, strTempFormat.c_str(), GetLogParam(pData->vParam.at(0)).c_str(), 
					GetLogParam(pData->vParam.at(1)).c_str(),
					GetLogParam(pData->vParam.at(2)).c_str(),
					GetLogParam(pData->vParam.at(3)).c_str(),
					GetLogParam(pData->vParam.at(4)).c_str());
				break;
			case 6:
				sprintf( szBuff, strTempFormat.c_str(), GetLogParam(pData->vParam.at(0)).c_str(), 
					GetLogParam(pData->vParam.at(1)).c_str(),
					GetLogParam(pData->vParam.at(2)).c_str(),
					GetLogParam(pData->vParam.at(3)).c_str(),
					GetLogParam(pData->vParam.at(4)).c_str(),
					GetLogParam(pData->vParam.at(5)).c_str());
				break;
			case 7:
				sprintf( szBuff, strTempFormat.c_str(), GetLogParam(pData->vParam.at(0)).c_str(), 
					GetLogParam(pData->vParam.at(1)).c_str(),
					GetLogParam(pData->vParam.at(2)).c_str(),
					GetLogParam(pData->vParam.at(3)).c_str(),
					GetLogParam(pData->vParam.at(4)).c_str(),
					GetLogParam(pData->vParam.at(5)).c_str(),
					GetLogParam(pData->vParam.at(6)).c_str());
				break;
			default:
				break;
			}

			strMsg = szBuff;
		}
	}

	return strTitle + strMsg;
}

std::string LogDlg::GetLogParam( LOG_PARAM logParam )
{
	if ( logParam.strParam.empty() )
	{
		return "";
	}

	string strParam;
	switch( logParam.eParamType )
	{
	case DEFAULT_TYPE:
		strParam = logParam.strParam;
		break;
	case LOAD_STRING_TYPE:
		strParam = LOADSTRING_TEXT(logParam.strParam);
		break;
	case ORDER_STATUS_MSG_TYPE:
		{
			int nRet = CFieldValueTool::String2OrderStatusMsg(logParam.strParam);
			if( nRet > 0)
			{
				strParam = CFieldValueTool::OrderStatusMsg2String( nRet, SVR_LANGUAGE).c_str();
			}
			else
			{
				strParam = logParam.strParam;
			}
		}
		break;
	case DIRECTION_TYPE:
        if(DEFAULT_SVR())
		    strParam = CFieldValueTool::Direction2String(logParam.strParam[0], DEFAULT_SVR()->GetServerParamCfg().LanguageId).c_str();
		else
			strParam="";
		break;
	case OFFSETFLAG_TYPE:
		strParam="";
		if(DEFAULT_SVR()) strParam = CFieldValueTool::OffsetFlag2String(logParam.strParam[0], DEFAULT_SVR()->GetServerParamCfg().LanguageId).c_str();
		break;
	case PRICETYPE_MSG_TYPE:
		strParam = CFieldValueTool::OrderPriceType2String(logParam.strParam[0],DEFAULT_SVR()->GetServerParamCfg().LanguageId).c_str();
		break;
	default:
		break;
	}

	return strParam;
}

LogDlg::~LogDlg()
{
	for ( int i = 0; i <m_pList->GetItemCount(); i++)
	{
		LogInfoMulti* pInfo=(LogInfoMulti*)m_pList->GetItemData(i);
		if ( NULL == pInfo )
		{
			continue;
		}

		if ( NULL == pInfo->pData )
		{
			delete pInfo;
			pInfo = NULL;
			continue;
		}

		delete pInfo->pData;
		delete pInfo;
		pInfo = NULL;
	}
}

void LogDlg::DestroyInstance()
{
	if ( NULL != m_pLogDlg )
	{
		delete m_pLogDlg;
		m_pLogDlg = NULL;
	}
}

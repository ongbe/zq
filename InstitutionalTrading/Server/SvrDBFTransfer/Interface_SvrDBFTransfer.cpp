#include "StdAfx.h"
#include "Interface_SvrDBFTransfer.h"
#include "FileMonitor.h"


//CInterface_SvrDBFTransfer* CInterface_SvrDBFTransfer::m_pObj=NULL;
//CInterface_SvrDBFTransfer& CInterface_SvrDBFTransfer::getObj(void)
//{
//    if(!m_pObj)
//        m_pObj=new CInterface_SvrDBFTransfer();
//
//    return *m_pObj;
//}

void CInterface_SvrDBFTransfer::regeditCallBackPtr(void *pThostFtdcMdSpi)
{
	if(pThostFtdcMdSpi)
		CFileMonitor::m_pThostFtdcMdSpi = (CThostFtdcMdSpi *)pThostFtdcMdSpi;
}
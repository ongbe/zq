#include "StdAfx.h"
#include "RiskProcess.h"
#include "UserApi.h"
#include "DBAccess.h"
#include "DataCenter.h"
#include "RiskMsgQueue.h"

CRiskProcess::CRiskProcess()

{
//	ReadComplexCtpData(m_MaxPositionLevel,m_mutex_MaxPositionLevel,"MaxPositionLevel.xml");
//	ReadSimpleCtpData(m_FundLossLevel,m_mutex_FundLossLevel,"FundLossLevel.xml");
//	ReadSimpleCtpData(m_GappedMarketLevel,m_mutex_GappedMarketLevel,"GappedMarketLevel.xml");
}
CRiskProcess::~CRiskProcess()
{
//	WriteComplexCtpData(m_MaxPositionLevel,m_mutex_MaxPositionLevel,"MaxPositionLevel.xml");
//	WriteSimpleCtpData(m_FundLossLevel,m_mutex_FundLossLevel,"FundLossLevel.xml");
//	WriteSimpleCtpData(m_GappedMarketLevel,m_mutex_GappedMarketLevel,"GappedMarketLevel.xml");	
}
void CRiskProcess::AddMaxPositionEvent(const std::string& InvestorID,const std::string& InstrumentID,
						 double dblValue,int nVolume, int levelID)
{
	RiskEventIDType EventID=GetMaxPositionEventID(InvestorID,InstrumentID,levelID);
	if(EventID<0)
		return;//重复事件，不发送
	SendRiskEvent(RI_MaxPosition,InvestorID,InstrumentID,EventID,nVolume,levelID);
}
void CRiskProcess::SendRiskEvent(RiskIndicatorType type,const std::string& InvestorID,
								 const std::string& InstrumentID,RiskEventIDType EventID,
								 double dblValue,int levelID)
{
	RiskEvent evt;
	memset(&evt,0,sizeof(RiskEvent));
	RiskEventHandling handle;
	memset(&handle,0,sizeof(RiskEventHandling));
	MessageInfo MsgInfo;
	memset(&MsgInfo,0,sizeof(MessageInfo));

	bool bSendMsg=false;

	//具体内容	
	evt.nRiskEventID=EventID;
	evt.nRiskEventSubID=CDataCenter::Get()->GetRiskEventSubID(EventID);
	evt.lEventTime=time(NULL);
	evt.nTradeAccountID=CDataCenter::Get()->GetTradeAccountID(InvestorID);
	evt.nRiskIndModuleID=CDataCenter::Get()->GetRiskIndModuleID(InvestorID);
	evt.nRiskIndicatorID=type;
	safestrcpy(evt.InstrumentID,sizeof(evt.InstrumentID),InstrumentID.c_str());
	evt.nRiskLevelID=levelID;
	evt.dblIndicatorValue=dblValue;
	evt.nIsValid=RiskEvent_Realtime;


	if(levelID>0)
	{
		bSendMsg=true;
		evt.nMsgSendStatus=evt.nRiskEventSubID==0?MsgStatus_New:MsgStatus_ChangeLevel;
		SAFE_GET_DATACENTER()->NewActiveEvent(evt);
	}
	else
	{
		evt.nMsgSendStatus=MsgStatus_Delete;
		SAFE_GET_DATACENTER()->DeleteActiveEvent(evt);
	}

	//notice
	EventMessageTemplate msgtemple;
	if(bSendMsg&&CDataCenter::Get()->GetEventMessageTemplate(type,levelID,msgtemple))
	{
		safestrcpy(MsgInfo.szTitle,sizeof(MsgTitle),msgtemple.szTitle);
		safestrcpy(MsgInfo.szContent,sizeof(MsgContent),msgtemple.szContent);		
	}
	else
	{
		int i =0;
	}
	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf(MsgInfo.szExpiredDate,"%04d-%02d-%2d",st.wYear,st.wMonth,st.wDay);
	safestrcpy(MsgInfo.szOwner, sizeof(MsgInfo.szOwner), "系统");
	//风险事件处理
	SAFE_GET_DATACENTER()->GetRiskEventHandlingID(handle.nRiskEventHandlingID);
	handle.lHandlingTime=time(NULL);
	handle.nTradeAccountID=evt.nTradeAccountID;
	handle.nRiskEventID=evt.nRiskEventID;
	handle.nRiskEventSubID=evt.nRiskEventSubID;
	handle.nAction=Action_ServerRecord;
	handle.nProcessStatus=evt.nMsgSendStatus;
	SAFE_GET_DATACENTER()->AddHandling(handle);

	CRisk2ClientMsgQueue* pMsgQueue=CRisk2ClientMsgQueue::Get();
	if(pMsgQueue==NULL)
		return;

	Risk2ClientData data={0};

	memset(&data,0,sizeof(data));
	data.CmdID=R2CMSGID_RISKEVENT;
	data.Main.Event=evt;
	data.AccID=evt.nTradeAccountID;
	pMsgQueue->AddMsg(data);


	memset(&data,0,sizeof(data));
	data.CmdID=R2CMSGID_EVENTHANDLING;
	data.AccID=handle.nTradeAccountID;
	data.Main.Handling=handle;
	pMsgQueue->AddMsg(data);	

	if(bSendMsg)
	{
		std::vector<TargetAccount>  Accounts;
		AssetOrgProRelation org;
		org.nAssetMgmtOrgID=CDataCenter::Get()->GetAssetMgmtOrgID(InvestorID);
		org.nFinancialProductID=CDataCenter::Get()->GetFinancialProductID(InvestorID);
		CDataCenter::Get()->GetRiskMgmtUserInfo(org,Accounts);
		if(Accounts.size()>0)
		{
			for (size_t i=0;i<Accounts.size();++i)
			{
				memset(&data,0,sizeof(data));
				data.CmdID=R2CMSGID_TARGETACCOUNT;
				data.AccID=evt.nTradeAccountID;
				data.Main.Target=Accounts[i];
				pMsgQueue->AddMsg(data);
			}
			memset(&data,0,sizeof(data));
			data.CmdID=R2CMSGID_MESSAGEINFO;
			data.AccID=evt.nTradeAccountID;
			data.Main.MsgInfo=MsgInfo;
			pMsgQueue->AddMsg(data);
		}
	}


}
void CRiskProcess::AddFundLossEvent(const std::string& InvestorID,
					  double dblValue,int levelID)
{
	RiskEventIDType EventID=GetFundLossEventID(InvestorID,levelID);
	if(EventID<0)
		return;//重复事件，不发送
	SendRiskEvent(RI_FundLoss,InvestorID,"",EventID,dblValue,levelID);
}
void CRiskProcess::AddGappedMarketEvent(const std::string& InvestorID,
						  double dblValue,int levelID)
{
	RiskEventIDType EventID=GetGappedMarketEventID(InvestorID,levelID);
	if(EventID<0)
		return;//重复事件，不发送
	SendRiskEvent(RI_GappedMarket,InvestorID,"",EventID,dblValue,levelID);
}
void CRiskProcess::AddMarginRatioEvent(const std::string& InvestorID,double dblValue,int levelID)
{
	RiskEventIDType EventID=GetMarginRatioEventID(InvestorID,levelID);
	if(EventID<0)
		return;//重复事件，不发送
	SendRiskEvent(RI_MarginRatio,InvestorID,"",EventID,dblValue,levelID);
}
void CRiskProcess::AddMaxProductPositionEvent(const std::string& InvestorID,const std::string& ProductID,
									   double dblValue,int levelID)
{
	RiskEventIDType EventID=GetMaxProductPositionEventID(InvestorID,ProductID,levelID);
	if(EventID<0)
		return;//重复事件，不发送
	SendRiskEvent(RI_ProductMaxPositon,InvestorID,ProductID,EventID,dblValue,levelID);
}
void CRiskProcess::AddFundNetValueEvent(const std::string& InvestorID,
					  double dblValue,int levelID)
{
	RiskEventIDType EventID=GetFundNetValueEventID(InvestorID,levelID);
	if(EventID<0)
	{
		if(NeedForceClose(InvestorID, dblValue))
		{//基金净值需要强平，则执行强平操作
			ForceClose(InvestorID);
		}
		return;//重复事件，不发送
	}
	SendRiskEvent(RI_FundNetValue,InvestorID,"",EventID,dblValue,levelID);	

	if(NeedForceClose(InvestorID, dblValue))
	{//基金净值需要强平，则执行强平操作
		ForceClose(InvestorID);
	}
}
RiskEventIDType CRiskProcess::FillEventLevelID(EventLevelID& evtlvl,int levelID )
{
	RiskEventIDType EventID=0;
	if(levelID==evtlvl.LevelID)
		EventID=-1;//相同风控事件,不重新发送
	else
	{
		if(evtlvl.LevelID==0)
		{
			SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
			evtlvl.EventID=EventID;
		}
		else
			EventID=evtlvl.EventID;//以相同ID更新状态
		evtlvl.LevelID=levelID;
	}
	return EventID;
}
RiskEventIDType CRiskProcess::GetMaxPositionEventID( const std::string& InvestorID, const std::string& InstrumentID, int levelID )
{
	RiskEventIDType EventID=0;
	std::map<std::string,std::map<std::string,EventLevelID>>::iterator it;
	m_mutex_MaxPositionLevel.write_lock();
	it=m_MaxPositionLevel.find(InvestorID);
	if(it!=m_MaxPositionLevel.end())
	{
		std::map<std::string,EventLevelID>& nestmap=it->second;
		std::map<std::string,EventLevelID>::iterator nestit=nestmap.find(InstrumentID);
		if(nestit!=nestmap.end())
		{
			EventLevelID& evtlvl=nestit->second;
			EventID=FillEventLevelID(evtlvl,levelID);
		}
		else
		{
			SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
			EventLevelID evtlvl;
			evtlvl.EventID=EventID;
			evtlvl.LevelID=levelID;
			nestmap[InstrumentID]=evtlvl;
		}
	}
	else
	{
		std::map<std::string,EventLevelID> nestmap;
		EventLevelID evtlvl;
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		nestmap[InstrumentID]=evtlvl;
		m_MaxPositionLevel[InvestorID]=nestmap;
	}
	m_mutex_MaxPositionLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetFundLossEventID( const std::string& InvestorID, int levelID )
{
	RiskEventIDType EventID=0;
	m_mutex_FundLossLevel.write_lock();
	std::map<std::string,EventLevelID>::iterator nestit=m_FundLossLevel.find(InvestorID);
	if(nestit!=m_FundLossLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
	}
	else
	{
		EventLevelID evtlvl;
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_FundLossLevel[InvestorID]=evtlvl;
	}
	m_mutex_FundLossLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetGappedMarketEventID( const std::string& InvestorID, int levelID )
{
	RiskEventIDType EventID=0;
	m_mutex_GappedMarketLevel.write_lock();
	std::map<std::string,EventLevelID>::iterator nestit=m_GappedMarketLevel.find(InvestorID);
	if(nestit!=m_GappedMarketLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_GappedMarketLevel[InvestorID]=evtlvl;
	}
	m_mutex_GappedMarketLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetMarginRatioEventID( const std::string& InvestorID, int levelID )
{
	RiskEventIDType EventID=0;
	m_mutex_MarginRatioLevel.write_lock();
	std::map<std::string,EventLevelID>::iterator nestit=m_MarginRatioLevel.find(InvestorID);
	if(nestit!=m_MarginRatioLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_MarginRatioLevel[InvestorID]=evtlvl;
	}
	m_mutex_MarginRatioLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetMaxProductPositionEventID( const std::string& InvestorID, const std::string& ProductID, int levelID )
{
	RiskEventIDType EventID=0;
	std::map<std::string,std::map<std::string,EventLevelID>>::iterator it;
	m_mutex_MaxProductPositionLevel.write_lock();
	it=m_MaxProductPositionLevel.find(InvestorID);
	if(it!=m_MaxProductPositionLevel.end())
	{
		std::map<std::string,EventLevelID>& nestmap=it->second;
		std::map<std::string,EventLevelID>::iterator nestit=nestmap.find(ProductID);
		if(nestit!=nestmap.end())
		{
			EventLevelID& evtlvl=nestit->second;
			EventID=FillEventLevelID(evtlvl,levelID);
		}
		else
		{
			SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
			EventLevelID evtlvl;
			evtlvl.EventID=EventID;
			evtlvl.LevelID=levelID;
			nestmap[ProductID]=evtlvl;
		}
	}
	else
	{
		std::map<std::string,EventLevelID> nestmap;
		EventLevelID evtlvl;
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		nestmap[ProductID]=evtlvl;
		m_MaxProductPositionLevel[InvestorID]=nestmap;
	}
	m_mutex_MaxProductPositionLevel.write_unlock();
	return EventID;
}
RiskEventIDType CRiskProcess::GetFundNetValueEventID( const std::string& InvestorID, int levelID )
{
	RiskEventIDType EventID=0;
	m_mutex_FundNetValueLevel.write_lock();
	std::map<std::string,EventLevelID>::iterator nestit=m_FundNetValueLevel.find(InvestorID);
	if(nestit!=m_FundNetValueLevel.end())
	{
		EventLevelID& evtlvl=nestit->second;
		EventID=FillEventLevelID(evtlvl,levelID);
	}
	else
	{
		SAFE_GET_DATACENTER()->GetRiskEventID(EventID);
		EventLevelID evtlvl;
		evtlvl.EventID=EventID;
		evtlvl.LevelID=levelID;
		m_FundNetValueLevel[InvestorID]=evtlvl;
	}
	m_mutex_FundNetValueLevel.write_unlock();
	return EventID;
}
bool CRiskProcess::HasMaxPositionRisk( const std::string& InvestorID, const std::string& InstrumentID) const
{
	std::map<std::string,std::map<std::string,EventLevelID>>::const_iterator it;
	bool bRet=false;
	m_mutex_MaxPositionLevel.read_lock();
	it=m_MaxPositionLevel.find(InvestorID);
	if(it!=m_MaxPositionLevel.end())
	{
		const std::map<std::string,EventLevelID>& nestmap=it->second;
		std::map<std::string,EventLevelID>::const_iterator nestit=nestmap.find(InstrumentID);
		if(nestit!=nestmap.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_MaxPositionLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasFundLossRisk( const std::string& InvestorID) const
{
	bool bRet=false;
	m_mutex_FundLossLevel.read_lock();
	std::map<std::string,EventLevelID>::const_iterator nestit=m_FundLossLevel.find(InvestorID);
	if(nestit!=m_FundLossLevel.end())
	{
		if(nestit!=m_FundLossLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_FundLossLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasGappedMarketRisk( const std::string& InvestorID) const
{
	bool bRet=false;
	m_mutex_GappedMarketLevel.read_lock();
	std::map<std::string,EventLevelID>::const_iterator nestit=m_GappedMarketLevel.find(InvestorID);
	if(nestit!=m_GappedMarketLevel.end())
	{
		if(nestit!=m_GappedMarketLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_GappedMarketLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasMarginRatioRisk( const std::string& InvestorID) const
{
	bool bRet=false;
	m_mutex_MarginRatioLevel.read_lock();
	std::map<std::string,EventLevelID>::const_iterator nestit=m_MarginRatioLevel.find(InvestorID);
	if(nestit!=m_MarginRatioLevel.end())
	{
		if(nestit!=m_MarginRatioLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_MarginRatioLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasMaxProductPositionRisk( const std::string& InvestorID, const std::string& ProductID) const
{
	std::map<std::string,std::map<std::string,EventLevelID>>::const_iterator it;
	bool bRet=false;
	m_mutex_MaxProductPositionLevel.read_lock();
	it=m_MaxProductPositionLevel.find(InvestorID);
	if(it!=m_MaxProductPositionLevel.end())
	{
		const std::map<std::string,EventLevelID>& nestmap=it->second;
		std::map<std::string,EventLevelID>::const_iterator nestit=nestmap.find(ProductID);
		if(nestit!=nestmap.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_MaxProductPositionLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::HasFundNetValueRisk( const std::string& InvestorID) const
{
	bool bRet=false;
	m_mutex_FundNetValueLevel.read_lock();
	std::map<std::string,EventLevelID>::const_iterator nestit=m_FundNetValueLevel.find(InvestorID);
	if(nestit!=m_FundNetValueLevel.end())
	{
		if(nestit!=m_FundNetValueLevel.end())
		{
			if(nestit->second.LevelID>0)
				bRet=true;
		}
	}
	m_mutex_FundNetValueLevel.read_unlock();
	return bRet;
}
bool CRiskProcess::NeedForceClose(const std::string& InvestorID, double dbValue)
{
	if(!CDataCenter::Get())
		return false;

	int nAccountID = CDataCenter::Get()->GetTradeAccountID(InvestorID);
	ForceCloseParam forceCloseParam;
	if(!CDataCenter::Get()->GetForceCloseParam(InvestorID, forceCloseParam))
		return false;

//	if(dbValue - forceCloseParam.fThreshold > -0.00001 && dbValue - forceCloseParam.fThreshold < 0.00001)
//		return false;
	if(dbValue > forceCloseParam.fThreshold )
		return false;

	if(!CDataCenter::Get()->HasForceClosePermission(InvestorID))
		return false;
	
	return true;
}
void CRiskProcess::ForceClose(const std::string& InvestorID)
{
	if(CForceCloseMsgQueue::Get())
		CForceCloseMsgQueue::Get()->AddForceCloseMsg(InvestorID);
}
//
//void CRiskProcess::ReadLevelFile()
//{
//	std::string strxmlpath="level.xml";
//	GetFolderFileName(strxmlpath,"cache");
//	TiXmlDocument myDocument;
//	if(myDocument.LoadFile(strxmlpath))
//	{
//		TiXmlElement* RootElement=myDocument.RootElement();
//		if(XML_ELEMENTTEXT(RootElement)=="root")
//		{
//			int storeday=0;
//			RootElement->Attribute("date",&storeday);
//			if(storeday==GetTodayString())
//			{
//				
//				TiXmlElement* p=NULL,*pInvestor=NULL,*pItem=NULL;
//				p=RootElement->FirstChildElement("MaxPosition");
//				if(p)
//				{
//					pInvestor=p->FirstChildElement("InvestorID");
//					while(pInvestor)
//					{
//						std::string strInvestorID=XML_ATTRIBUTE(pInvestor,"value");
//						pItem=pInvestor->FirstChildElement("item");
//						std::map<std::string,EventLevelID> nestmap;
//						while(pItem)
//						{
//							std::string strFirst=XML_ATTRIBUTE(pItem,"first");
//							EventLevelID evtlvl;
//							pItem->Attribute("event",&evtlvl.EventID);
//							pItem->Attribute("level",&evtlvl.LevelID);
//							nestmap[strFirst]=evtlvl;
//							pItem=pItem->NextSiblingElement("item");
//						}
//						m_mutex_MaxPositionLevel.write_lock();
//						m_MaxPositionLevel[strInvestorID]=nestmap;
//						m_mutex_MaxPositionLevel.write_unlock();
//						pInvestor=pInvestor->NextSiblingElement("InvestorID");
//					}
//				}
//				p=RootElement->FirstChildElement("FundLoss");
//				if(p)
//				{
//					pItem=p->FirstChildElement("item");
//					while(pItem)
//					{
//						std::string strFirst=XML_ATTRIBUTE(pItem,"first");
//						EventLevelID evtlvl;
//						pItem->Attribute("event",&evtlvl.EventID);
//						pItem->Attribute("level",&evtlvl.LevelID);
//						m_mutex_FundLossLevel.write_lock();
//						m_FundLossLevel[strFirst]=evtlvl;
//						m_mutex_FundLossLevel.write_unlock();
//						pItem=pItem->NextSiblingElement("item");
//					}
//				}
//				p=RootElement->FirstChildElement("GappedMarket");
//				if(p)
//				{
//					pItem=p->FirstChildElement("item");
//					while(pItem)
//					{
//						std::string strFirst=XML_ATTRIBUTE(pItem,"first");
//						EventLevelID evtlvl;
//						pItem->Attribute("event",&evtlvl.EventID);
//						pItem->Attribute("level",&evtlvl.LevelID);
//						m_mutex_GappedMarketLevel.write_lock();
//						m_GappedMarketLevel[strFirst]=evtlvl;
//						m_mutex_GappedMarketLevel.write_unlock();
//						pItem=pItem->NextSiblingElement("item");
//					}
//				}
//			}	
//		}
//	}
//
//}
//
//void CRiskProcess::WriteLevelFile() const
//{
//	std::map<std::string,std::map<std::string,EventLevelID>>::const_iterator mit;
//	std::map<std::string,EventLevelID>::const_iterator	it;
//	TiXmlDocument myDocument;
//
//	TiXmlElement *RootElement = new TiXmlElement("root");
//	RootElement->SetAttribute("date",GetTodayString());
//	myDocument.LinkEndChild(RootElement);
//
//	TiXmlElement *pTopMap= new TiXmlElement("MaxPosition");
//	m_mutex_MaxPositionLevel.read_lock();
//	for (mit=m_MaxPositionLevel.begin();mit!=m_MaxPositionLevel.end();++mit)
//	{
//		const std::string& InvestorID=mit->first;
//		const std::map<std::string,EventLevelID>& mmap=mit->second;
//		TiXmlElement *pNestMap = new TiXmlElement("InvestorID");
//		pNestMap->SetAttribute("value",InvestorID);
//		pTopMap->LinkEndChild(pNestMap);
//		for (it=mmap.begin();it!=mmap.end();++it)
//		{
//			TiXmlElement *pItem = new TiXmlElement("item");
//			pItem->SetAttribute("first",it->first);
//			pItem->SetAttribute("event",it->second.EventID);
//			pItem->SetAttribute("level",it->second.LevelID);
//			pNestMap->LinkEndChild(pItem);		
//		}
//	}
//	m_mutex_MaxPositionLevel.read_unlock();
//	RootElement->LinkEndChild(pTopMap); 
//
//	pTopMap= new TiXmlElement("FundLoss");
//	m_mutex_FundLossLevel.read_lock();
//	for (it=m_FundLossLevel.begin();it!=m_FundLossLevel.end();++it)
//	{
//		TiXmlElement *pItem = new TiXmlElement("item");
//		pItem->SetAttribute("first",it->first);
//		pItem->SetAttribute("event",it->second.EventID);
//		pItem->SetAttribute("level",it->second.LevelID);
//		pTopMap->LinkEndChild(pItem);		
//	}
//	m_mutex_FundLossLevel.read_unlock();
//	RootElement->LinkEndChild(pTopMap); 
//
//	pTopMap= new TiXmlElement("GappedMarket");
//	m_mutex_GappedMarketLevel.read_lock();
//	for (it=m_GappedMarketLevel.begin();it!=m_GappedMarketLevel.end();++it)
//	{
//		TiXmlElement *pItem = new TiXmlElement("item");
//		pItem->SetAttribute("first",it->first);
//		pItem->SetAttribute("event",it->second.EventID);
//		pItem->SetAttribute("level",it->second.LevelID);
//		pTopMap->LinkEndChild(pItem);		
//	}
//	m_mutex_GappedMarketLevel.read_unlock();
//	RootElement->LinkEndChild(pTopMap); 
//
//	std::string strxmlpath="level.xml";
//	GetFolderFileName(strxmlpath,"cache");
//	myDocument.SaveFile(strxmlpath);
//}
//
//
//

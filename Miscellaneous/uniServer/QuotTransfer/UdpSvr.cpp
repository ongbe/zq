#include "QuotTransfer.h"
#include "easytime.h"



#define PREFIX_PACKAGECACHE		0xa973b71c					//Package开始标志
#define TAIL_PACKAGECACHE		0x5a7b9c2d					//Package结束标志

#define CMDID_QuotInfo_ctp		4001


#pragma pack(push)
#pragma pack(4)

//下面定义Package包头结构
typedef struct
{
	DWORD	Prefix;						                    //前导标志，用于标志一个package的开始
	int		CmdID;						                    //Package的命令ID，可以是CMDID_xxxx
	int		PlatformID;					                    //交易平台ID，可以是PLATFORM_xxxx
	int		ExchangeID;					                    //交易所ID，可以是EXCHANGE_xxxx

	int		DataLen;					                    //数据长度，指内容的长度。注意真正内容的大小是按DataLen长度用零补整，使之为4字节边界。
	DWORD	dwCRC;						                    //内容的算术和，内容包括PackageHead和Data和Tail
	DWORD	dwSeq;						                    //同一类Package的序列号
}PackageHeadStru;

/***********************************************************
*   name    :   Stru_QuotAdditionalInfo
*   brief   :   行情信息的附加参数
***********************************************************/
typedef struct
{
    time_t  RecvTime_utc;                                   //接收到该条行情信息的时间的UTC秒数
    int     RecvTime_ms;                                    //接收到该条行情信息的时间的毫秒数
    time_t  UpdateTime_utc;                                 //该条行情信息更新的时间的UTC秒数，开盘前，此值可能为零
    int     UpdateTime_ms;                                  //该条行情信息更新的时间的毫秒数
    int     Seq;
}Stru_QuotAdditionalInfo;

#pragma pack(pop)


void SendNewQuotData(Stru_Quot2& QuotData);



//从udp simple得到数据后，调用此回调函数进行处理
QUOTTRANSFER_API void UdpRecvFunc(char *pData,int DataLen,Stru_IPAddress& SvrAddr,Stru_IPAddress& RemoteAddr,SOCKET hSocket)
{
    PackageHeadStru* pPackageHead;                          //包头指针
    CThostFtdcDepthMarketDataField* pThostQuotData;         //行情数据指针
    Stru_QuotAdditionalInfo* pAdditionalInfo;               //附加信息指针，如更新时间、接收时间等

	if(DataLen<sizeof(PackageHeadStru)+sizeof(CThostFtdcDepthMarketDataField)+sizeof(Stru_QuotAdditionalInfo))
		return;

    pPackageHead=(PackageHeadStru*)pData;
    pThostQuotData=(CThostFtdcDepthMarketDataField*)((char*)pData+sizeof(PackageHeadStru));
    pAdditionalInfo=(Stru_QuotAdditionalInfo*)((char*)pData+sizeof(PackageHeadStru)+sizeof(CThostFtdcDepthMarketDataField));

	if(pPackageHead->Prefix!=PREFIX_PACKAGECACHE) 
		return;
	if(pPackageHead->CmdID!=CMDID_QuotInfo_ctp)
		return;

	Stru_Quot2 TmpQuot;
	std::string strInstrumentID;

	memset(&TmpQuot,0,sizeof(TmpQuot));
	TmpQuot.Head=PACKAGEHEAD;
	TmpQuot.PackageLen=sizeof(TmpQuot);
	TmpQuot.CRCValue=0;
	TmpQuot.Cmd=0;
	strncpy(TmpQuot.InstrumentID,pThostQuotData->InstrumentID,sizeof(TmpQuot.InstrumentID)-1);
	TmpQuot.OpenPrice=pThostQuotData->OpenPrice;
	TmpQuot.LastPrice=pThostQuotData->LastPrice;
	TmpQuot.Volume=pThostQuotData->Volume;
	TmpQuot.OpenInterest=(int)pThostQuotData->OpenInterest;			//持仓量
	TmpQuot.PreSettlementPrice=pThostQuotData->PreSettlementPrice;	//昨结算价
	TmpQuot.PreClosePrice=pThostQuotData->PreClosePrice;			//昨收盘价
    TmpQuot.UpdateTime_utc=(DWORD)CEasyTime(NULL,pThostQuotData->UpdateTime).getutc();			//更新时间,utc格式，单位为秒
    TmpQuot.UpdateTimeMS=pThostQuotData->UpdateMillisec;			//更新时间毫秒数
//	TmpQuot.UpdateTime_utc=(DWORD)pAdditionalInfo->UpdateTime_utc;			//更新时间,utc格式，单位为秒
//	TmpQuot.UpdateTimeMS=pAdditionalInfo->UpdateTime_ms;			//更新时间毫秒数
	TmpQuot.Tail=PACKAGETAIL;										//包尾
	
	strInstrumentID=TmpQuot.InstrumentID;

	g_Quots[strInstrumentID]=TmpQuot;

	SendNewQuotData(TmpQuot);

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	if(!CWinService::IsRunInServiceMode())
	{
		printf("%02d:%02d:%02d.%03d 新行情到来 %s\n",CurTime.wHour,CurTime.wMinute,CurTime.wSecond,CurTime.wMilliseconds%1000,
			TmpQuot.InstrumentID);
	}
}

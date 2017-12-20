#pragma once
//想把某个ID的所有数据都存在这个类下面
#include <string>
#include "../inc/Quotation/CritSec.h"
#include "../inc/Quotation/AutoLock.h"
#include "..\\inc\\ISvr\\BusinessData.hpp"
#include "../inc/Quotation/TechIndex.h"
#define  KONGKAI    "空开"
#define  DUOKAI     "多开"
#define  DUOPING	"多平"
#define  KONGPING   "空平"
#define  DUOHUAN    "多换"
#define  KONGHUAN   "空换"
#define  SHUANGPING "双平"
#define  SHUANGKAI  "双开"
class CFData
{
public:
	CFData();
	~CFData();
public:
	string  m_ExchangeName; //交易所名称
	string  m_ContractName; //合约名称
	string  m_ContractDate; //合约到期
	
	string	m_strTradingDay;//交易日
	int		m_VolumeMultiple;//合约乘数
	double	m_PreSettlementPrice;//昨结算价
	double	m_UpperLimitPrice;///涨停板价
	double	m_LowerLimitPrice;	///跌停板价
	double  m_PriceTick;//最小变动价位
	int     m_Simulate;//是否模拟，1-模拟，0-真实
	double			 m_fMaxValue;//今日最高价
	double			 m_fMinValue;//今日最低价
//	double			  m_dbLastdbPrice;      //前结算价
	vector<STLine *>  m_vecTLine;
	vector<struct tm> m_vecTimeScales;				//小到大排序
	
protected:	
	vector<DWORD>     m_vecTMWidthSpan;			//各个交易时间区间的总长
	DWORD			  m_TMWidthCount;			//交易时间刻度总共大小
	int				  m_nOneDayScalesCount;		//一天交易的时间总长（单位：秒）	
	CCritSec		  m_CritSecVector_TLine;//用于同步的	
	struct tm		  m_tmLastTrans_TLine;  //最后一次的行情的时间    
	int				  m_nNumDigits;         //保留有效数字位数，最小变动位对应
	STLine			  m_sTLineLast;			//最后一秒种的实时行情
	bool			  m_bInitKLine;
	bool			  m_bInitTLine;
	
	
public:
	void SetPriceTick(double  PriceTick);//设置最小变动位
	int  GetDotDigits();//得到保留有效位数
	bool SetFuture();
	void SetDataInTime(PlatformStru_DepthMarketData* pData, int nTrace);//处理实时行情，实时行情都在这个函数里面进行处理
	bool SaveFile_TLine();
	bool ReadFile_TLine();
	void SetLastTransTime_TLine(struct tm tmTime);
	void SetScales(vector<struct tm>& vecScales);
	void SetTodayScales(vector<struct tm>& vecTimeScales);
	void ClearMemory_TLine();
	void SetLastClosePrice(double dbPrice);
	BOOL SetInTimeData_TLine(STLine* pTLine);
	BOOL SetInTimeData_DiffSpan_TLine(STLine* pTLine);
	void CalcMaxMin();//计算今天历史数据的最大 最小值
protected:
	void			  Init();
	struct tm		  GetTimeInSpan_TLine(struct tm tmPara, int nSpan);	
	bool			  DirExist(const char *pszDirName) ;
	bool			  CreateDir(const char *pszDirName);//创建目录，包含子目录，可以创建多级子目录； 以后存储文件创建路径可以优化为这个函数完成
	bool			  GetFileSavePath_TLine(wxString &filepath);
	bool			  SaveOneFile_TLine(wxString filepath, vector<STLine *>& vecTLine);

//以上为tline相关，以下为kline相关
public:
	bool SetPhrase(EnumPhrase enumPhrase);	
	bool ReadFile();
	bool SaveFile();
	void SetLastTransTime(struct tm tmTime);
	void ClearMemory();
	void ClearMemory_Mark();
	void *m_pWnd;
	void SetWnd(void*  pWnd){ m_pWnd = pWnd; };
	vector<SKLine *>* GetVectorCommon(EnumPhrase enumPhrase);
	bool				AddMarks(KTYPE enumType, struct tm tmTime, double dbPrice);
	MAPsmark			m_mapMarks;
	EnumMark			m_enumMark;

	SPoint				*m_pDelMark;//k线，量能模块需要删除的某个标注

protected:	
	bool GetFileSavePath(wxString &filepath);
	bool GetSaveFileName(string  ExchangeName, string  ContractName, string  ContractDate, EnumPhrase enumPhrase, wxString &filepath);
	bool SaveOneFile(wxString filepath, vector<SKLine *>& vecKLine);

	BOOL SetInTimeData_DiffSpan(SKLine* pKLine);
	BOOL SetInTimeData_Day(vector<SKLine *>& vecKLinePara, SKLine* pKLine);
	BOOL SetInTimeData_Week(vector<SKLine *>& vecKLinePara, SKLine* pKLine);
	BOOL SetInTimeData_Month(vector<SKLine *>& vecKLinePara, SKLine* pKLine);
	BOOL SetInTimeData_MINS(vector<SKLine *>& vecKLinePara, SKLine* pKLine, int nTimeSpan);

	struct tm GetTimeInSpan(struct tm tmPara, int nSpan);
	EnumPhrase		 m_enumPhrase;   //时间周期;以后这个变量要去掉，现在暂时存储marks时候要用到，以后marks也要分周期存储
	//////////////////////////////////////////////////////////////////////////
	/*
	1分钟提供最近15个交易日数据
	5分-60分钟都是提供30个交易日的数据
	*/
	//////////////////////////////////////////////////////////////////////////
	vector<SKLine *>* m_vecKLineCommon;
	

	vector<SKLine *> m_vecKLineDay; 
	bool  m_bSelDay;

	vector<SKLine *> m_vecKLineWeek; 
	bool  m_bSelWeek;

	vector<SKLine *> m_vecKLineMonth; 
	bool  m_bSelMonth;

	vector<SKLine *> m_vecKLineMin1;
	bool  m_bSelMin1;

	vector<SKLine *> m_vecKLineMin5; 
	bool  m_bSelMin5;

	vector<SKLine *> m_vecKLineMin15; 
	bool  m_bSelMin15;

	vector<SKLine *> m_vecKLineMin30; 
	bool  m_bSelMin30;

	vector<SKLine *> m_vecKLineMin60;  
	bool  m_bSelMin60;
	
	SKLine				m_sKLineLast;
	CCritSec			m_CritSecVector;
	void				SetSelMinsValue(EnumPhrase enumPhrase);
	bool				GetSelMinsValue(EnumPhrase enumPhrase);

	bool				IsOneWeek(struct tm tm1, struct tm tm2);	
	bool				IsOneMonth(struct tm tm1, struct tm tm2);

	bool				ReadFileMarks();
	bool				SaveFileMarks();
	bool				SaveOneFile(wxString filepath, MAPsmark&	mapMarks);


	int					GetPhraseSpan(EnumPhrase enumPhrase);
	struct tm			m_tmLastTrans;
	void				SetLastTransTime_DiffSpan(struct tm tmTime, vector<SKLine *>& m_vecKLineCommon, EnumPhrase enmuPhrase);
	BOOL				SetInTimeData(SKLine* pKLine);

public:	//实时数据，右侧面板
	vector<SQIntime *>				m_vecQIntime;//实时

	bool				SetFuture_QIntime();
	bool				SetInTimeData_QIntime(PlatformStru_DepthMarketData& MarketDataInfo);
	bool				SaveFile_QIntime();	
	void				ClearMemory_QIntime();
	void                GetNeiWaiPan(int& nNeiPan, int& nWaiPan);
	
protected:
	CCritSec						m_CritSecVector_QIntime;
	bool							m_bInitQIntime;
	bool							m_bInitQIntimeFirst;//第一笔数据
	PlatformStru_DepthMarketData	m_MarketDataInfoLast;
	int								m_nNeiPan;//内盘
	int								m_nWaiPan;//外盘	
	bool				IsSameAsPrice(double dbPrice, double dbOrgPrice, bool bBid);//bBid为true时，是判断买一价相同；返回值true为同于卖一价；或者低于买入价
	bool				ReadFile_QIntime();
public://技术指标相关	
	bool CalcKTechIndex(vector<SKLine *>& m_vecKLine, string strTechIndexNameOrg, string strTechIndexName, EnumPhrase enumPhrase, CTechIndex*& pTechIndex, vector<int>* vecConfigDefault = NULL);
	void GetTechIndexConfig(string strTechIndex, EnumPhrase enumPhrase, vector<int>& vecConfig);
	void SetTechIndexConfig(string strTechIndex, EnumPhrase enumPhrase, vector<int>& vecConfig);

private:
	void	ClearMemory_TechIndex();
	CCritSec					m_CritSecVector_TechIndex;
	map<string, CTechIndex*>	m_mapName2TechIndex; //	
	map<string, vector<int>>	m_mapName2Config;   
	vector<string>				m_vecChangeName;
	bool GetConfigFrmFile(string strTechIndexName, EnumPhrase enumPhrase, vector<int>& vecCofig);
	void split(const string& s, char c, vector<string>& v);

	void GetDefaultConfigPara(string strTechIndexName, vector<int>& vecCofig);
	void Calc(string strTechIndexName, vector<SKLine *>& m_vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, CTechIndex*& pTechIndex);
};

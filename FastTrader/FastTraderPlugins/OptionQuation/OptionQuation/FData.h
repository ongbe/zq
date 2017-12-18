#pragma once
#include "stdafx.h"
#include "QuoteCommon.h"

class  CFData
{
public:
	CFData(void);
	~CFData(void);

public:
	//记录原始实时数据；主要用于跟历史服务器数据有差别的时候处理
	bool		    m_bRecordIntime;
	//深度行情的原始数据
	std::vector<PlatformStru_DepthMarketData>  m_vecDepthMarketData;	
	CRITICAL_SECTION	m_csDepthMarketData;


public:
	string		     m_ExchangeName;			//交易所名称
	string		     m_strInstrument;			//合约名称

	string		     m_strTradingDay;			//交易日
	int			     m_VolumeMultiple;			//合约乘数
	double		     m_PreSettlementPrice;		//昨结算价
	double			 m_UpperLimitPrice;			//涨停板价
	double			 m_LowerLimitPrice;			//跌停板价
	double			 m_PriceTick;				//最小变动价位
//	int				 m_Simulate;				//是否模拟，1-模拟，0-真实
	
	double			 m_fMaxValue;				//今日最高价
	double			 m_fMinValue;				//今日最低价

	vector<struct tm> m_vecTimeScales;			//小到大排序
	CRITICAL_SECTION  m_csTimeScales;

private:
	vector<STLine>   m_vecTLine;
	CRITICAL_SECTION	m_csTLine;
	bool m_bTLineShow;

	EnumPhrase		 m_enumPhrase;   //时间周期;以后这个变量要去掉，现在暂时存储marks时候要用到，以后marks也要分周期存储
	//////////////////////////////////////////////////////////////////////////
	/*
	1分钟提供最近15个交易日数据
	5分-60分钟都是提供30个交易日的数据
	*/
	//////////////////////////////////////////////////////////////////////////

	vector<SKLine>		m_vecKLineDay; 
	CRITICAL_SECTION	m_csKLineDay;
	bool m_bDayShow;

	vector<SKLine>		m_vecKLineWeek; 
	CRITICAL_SECTION	m_csKLineWeek;
	bool m_bWeekShow;

	vector<SKLine>		m_vecKLineMonth; 
	CRITICAL_SECTION	m_csKLineMonth;
	bool m_bMonthShow;

	vector<SKLine>		m_vecKLineMin1;
	CRITICAL_SECTION	m_csKLineMin1;
	bool m_bMin1Show;
//小于500条的返回则视为返回结束

	vector<SKLine>		m_vecKLineMin5; 
	CRITICAL_SECTION	m_csKLineMin5;
	bool m_bMin5Show;

	vector<SKLine>		m_vecKLineMin15; 
	CRITICAL_SECTION	m_csKLineMin15;
	bool m_bMin15Show;

	vector<SKLine>		m_vecKLineMin30; 
	CRITICAL_SECTION	m_csKLineMin30;
	bool m_bMin30Show;

	vector<SKLine>		m_vecKLineMin60;  
	CRITICAL_SECTION	m_csKLineMin60;
	bool m_bMin60Show;


protected:	
	vector<DWORD>     m_vecTMWidthSpan;			//各个交易时间区间的总长
	DWORD			  m_TMWidthCount;			//交易时间刻度总共大小
	int				  m_nOneDayScalesCount;		//一天交易的时间总长（单位：秒）		
	struct tm		  m_tmLastTrans_TLine;		//最后一次的行情的时间    
	int				  m_nNumDigits;				//保留有效数字位数，最小变动位对应
	STLine			  m_sTLineLast;				//最后一秒种的实时行情
	
	SKLine			  m_sKLineLast;



public:
	void SetDataInTime(PlatformStru_DepthMarketData* pData);//处理实时行情，实时行情都在这个函数里面进行处理
	BOOL SetInTimeData_TLine(PlatformStru_DepthMarketData* pData);
	BOOL SetInTimeData_DiffSpan_TLine(STLine& pTLine);
	struct tm GetTimeInSpan_TLine(struct tm tmPara, int nSpan);
	struct tm GetTimeInSpan(struct tm tmPara, int nSpan);
	struct tm GetTimeInSpan_History(struct tm tmPara, int nSpan);

	BOOL SetInTimeData_DiffSpan(SKLine& pKLine, EnumPhrase enumPhrase, bool bShow);
	BOOL SetInTimeData_Day(vector<SKLine>& vecKLinePara, SKLine& pKLine);
	BOOL SetInTimeData_Week(vector<SKLine>& vecKLinePara, SKLine& pKLine);
	BOOL SetInTimeData_Month(vector<SKLine>& vecKLinePara, SKLine& pKLine);
	BOOL SetInTimeData_MINS(vector<SKLine>& vecKLinePara, SKLine& pKLine, int nTimeSpan);
	void SetLastClosePrice(double dbPrice);
	BOOL SetInTimeData_KLine(PlatformStru_DepthMarketData* pData, 
														bool bDayShow,
														bool bWeekShow,
														bool bMonthShow,
														bool bMin1Show,
														bool bMin5Show,
														bool bMin15Show,
														bool bMin30Show,
														bool bMin60Show);
	bool IsOneWeek(struct tm tm1, struct tm tm2);
	bool IsOneMonth(struct tm tm1, struct tm tm2);
	
	void TestVector(vector<SKLine> vecKLinePara);
	bool GetVecKLine(EnumPhrase enumPhrase, vector<SKLine>& vec);
	bool GetVecTLine(vector<STLine>& vec);

	bool IsKLineQueryFinish(EnumPhrase enumPhrase);
private:
	bool GetVecKLineDay(vector<SKLine>& vec);
	bool GetVecKLineWeek(vector<SKLine>& vec);
	bool GetVecKLineMonth(vector<SKLine>& vec);
	bool GetVecKLineMin1(vector<SKLine>& vec);
	bool GetVecKLineMin5(vector<SKLine>& vec);
	bool GetVecKLineMin15(vector<SKLine>& vec);
	bool GetVecKLineMin30(vector<SKLine>& vec);
	bool GetVecKLineMin60(vector<SKLine>& vec);

public:
	void SetScales(vector<struct tm>& vecScales);

	bool SaveFile_TLine();
	bool ReadFile_TLine();
	bool ReadFile(EnumPhrase enumPhrase);
	bool SaveFile();
	bool GetFileSavePath(CString &filepath);
	bool SaveOneFile(CString filepath, vector<SKLine>& vecKLine);
	bool GetSelMinsValue(EnumPhrase enumPhrase);
	bool GetFileSavePath_TLine(CString &filepath);
	bool GetSaveFileName(string  ContractDate, EnumPhrase enumPhrase, CString &filepath);
	bool SaveOneFile_TLine(CString filepath, vector<STLine>& vecTLine);
	
	bool AppendHistoryTLineData(std::vector<TimeLineData>& vecTimeLineData);
	bool AppendRealTimeTLineData();
	bool AppendHistoryKLineData_Min1(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Min1();
	bool AppendHistoryKLineData_Min5(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Min5();
	bool AppendHistoryKLineData_Min15(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Min15();
	bool AppendHistoryKLineData_Min30(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Min30();
	bool AppendHistoryKLineData_Min60(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Min60();
	bool AppendHistoryKLineData_Day(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Day();
	bool AppendHistoryKLineData_Week(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Week();
	bool AppendHistoryKLineData_Month(std::vector<KLineData>& vecData);
	bool AppendRealTimeKLineData_Month();






	public://技术指标相关	
		bool CalcKTechIndex(vector<SKLine>& vecKLine, string strTechIndexNameOrg, string strTechIndexName, STechCalcPara& sTCPara, CTechIndex& pTechIndex, vector<int>* vecConfigDefault = NULL);
		void GetTechIndexConfig(string strTechIndex, EnumPhrase enumPhrase, vector<int>& vecConfig);
		void SetTechIndexConfig(string strTechIndex, EnumPhrase enumPhrase, vector<int>& vecConfig);

private:
	void	ClearMemory_TechIndex();
//	CCritSec					m_CritSecVector_TechIndex;
	map<string, CTechIndex>	m_mapName2TechIndex; 	
	map<string, vector<int>>	m_mapName2Config;   
	vector<string>				m_vecChangeName;
	bool GetConfigFrmFile(string strTechIndexName, EnumPhrase enumPhrase, vector<int>& vecCofig);
	void split(const string& s, char c, vector<string>& v);

	void GetDefaultConfigPara(string strTechIndexName, vector<int>& vecCofig);
	void Calc(string strTechIndexName, vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, CTechIndex& pTechIndex);

};

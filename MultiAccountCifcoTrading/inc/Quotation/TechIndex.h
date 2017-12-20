#pragma once
//想把某个ID的所有数据都存在这个类下面
#include <string>
#include "../inc/Quotation/CritSec.h"
#include "../inc/Quotation/AutoLock.h"
struct STechIndexValue 
{ 
	struct tm tmTime;
	string strName;
	double fValue; 
};	
typedef vector<STechIndexValue>  VecTechIndexValue; 
class CTechIndex
{
public:
	CTechIndex(void);
	~CTechIndex(void);	
	
	wxString				m_strTechName;		//BOLL
	EnumPhrase				m_enumPhrase;
	VecTechIndexValue*		m_vecData;         //MID UPPER LOWER 三个数组
	int						m_nVecDataCount;	
	
	int						m_nDrawBody;//主体绘制 macd 只要绘制 diff，dea；不需要绘制macd，所以这个值是2


	VecTechIndexValue*		m_vecMiddle;        //各种技术指标的中间值，对作图没有实际意义
	int						m_nVecMiddleCount;

	vector<int>				m_vecConfig;		// BOLL(26,2);中的26,2的数组
	vector<string>			m_vecConfigText;	// MID UPPER LOWER	


public:
	void ClearMemory();
	void Calc_MA(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_VOL(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_KDJ(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_MACD(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_BOLL(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_UOS(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_BIAS(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_DMI(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_PSY(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText);
	void Calc_ROC(vector<SKLine*>& vecKLine, EnumPhrase enumPhrase, vector<int>& vecConfig, vector<string>&	vecConfigText);

};


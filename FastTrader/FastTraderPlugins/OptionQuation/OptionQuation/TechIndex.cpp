#include "StdAfx.h"
#include "TechIndex.h"
#include "TechDefine.h"

CTechIndex::CTechIndex(void)
{	
	m_strTechName = "";
	m_vecData		 = NULL;	
	m_nVecDataCount  = 0;	
	m_nDrawBody      = 0;
	m_vecConfig.clear();		
	m_vecConfigText.clear();

	m_vecMiddle = NULL;
	m_nVecMiddleCount = 0;



}
CTechIndex::~CTechIndex(void)
{	
	ClearMemory();
}
CTechIndex::CTechIndex(const CTechIndex& other)
{
	m_strTechName		= other.m_strTechName;
	m_vecData			= NULL;	
	if(other.m_vecData && other.m_nVecDataCount>0)
	{
		m_vecData = new VecTechIndexValue[other.m_nVecDataCount];
		for(int i=0; i< other.m_nVecDataCount; i++)
			m_vecData[i] = other.m_vecData[i];
	}	
	m_nVecDataCount  = other.m_nVecDataCount;	
	m_nDrawBody      = other.m_nDrawBody;
	m_vecConfig      = other.m_vecConfig;
	m_vecConfigText = other.m_vecConfigText;

	if(other.m_vecMiddle&& other.m_nVecMiddleCount>0)
	{
		m_vecMiddle = new VecTechIndexValue[other.m_nVecMiddleCount];
		for(int i=0; i< other.m_nVecMiddleCount; i++)
			m_vecMiddle[i] = other.m_vecMiddle[i];
	}	

}
CTechIndex& CTechIndex::operator =(const CTechIndex& other)
{
	ClearMemory();
	m_strTechName		= other.m_strTechName;	
	if(other.m_vecData && other.m_nVecDataCount>0)
	{
		m_vecData = new VecTechIndexValue[other.m_nVecDataCount];
		for(int i=0; i< other.m_nVecDataCount; i++)
			m_vecData[i] = other.m_vecData[i];
	}	
	m_nVecDataCount  = other.m_nVecDataCount;	
	m_nDrawBody      = other.m_nDrawBody;
	m_vecConfig      = other.m_vecConfig;
	m_vecConfigText = other.m_vecConfigText;

	if(other.m_vecMiddle&& other.m_nVecMiddleCount>0)
	{
		m_vecMiddle = new VecTechIndexValue[other.m_nVecMiddleCount];
		for(int i=0; i< other.m_nVecMiddleCount; i++)
			m_vecMiddle[i] = other.m_vecMiddle[i];
	}	
	return *this;
}
void CTechIndex::ClearMemory()
{
	m_vecConfig.clear();		
	m_vecConfigText.clear();
	for(int n = 0; n< m_nVecDataCount; n++)
	{
		if(m_vecData)
		{
			m_vecData[n].clear();			
		}
	}
	if(m_vecData && m_nVecDataCount>0)
	{
		delete []m_vecData;
		m_vecData = NULL;
	}

	for(int n = 0; n< m_nVecMiddleCount; n++)
	{
		if(m_vecMiddle)
			m_vecMiddle[n].clear();		
	}
	if(m_vecMiddle && m_nVecMiddleCount >0)
	{
		delete []m_vecMiddle;
		m_vecMiddle = NULL;
	}
}
void CTechIndex::Calc_MA(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>& vecConfigText)
{//vecKLine 日期由近到远，今天的在0序号
	if(vecConfig.size() == 0)
		return;
	ASSERT(vecConfig.size() == vecConfigText.size());

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;
	m_strTechName = MA;

	m_vecData = new VecTechIndexValue[vecConfig.size()];
	m_nVecDataCount = vecConfig.size();
	m_nDrawBody = vecConfig.size();
	
	for(int i = 0; i< (int)vecConfig.size(); i++)
	{
		int nDay = vecConfig[i];
		if(nDay == 0)
			continue;

		VecTechIndexValue& pTech = m_vecData[i];
		for(int nItem = 0; nItem < (int)vecKLine.size(); nItem++)
		{
			SKLine& pKline = vecKLine[nItem];
			STechIndexValue sValue;
			sValue.tmTime  = pKline.dwTime;
			sValue.strName = vecConfigText[i];//ma1
			sValue.fValue  = MIN_VALUE_DEFAULT;
			if(nItem + nDay > (int)vecKLine.size())
			{//超出范围，没有形成足够天数，比如只有四天数据，自然没有5日线
				
			}
			else
			{
				double dbValue = 0.0;
				for(int nBegin = nItem; nBegin < nItem + nDay; nBegin++)
				{
					SKLine& pKBegin = vecKLine[nBegin];
					dbValue = dbValue + pKBegin.fClosePrice;	
				}
				sValue.fValue = dbValue/nDay;
			}
			pTech.push_back(sValue);
		}	
			
	}
}
void CTechIndex::Calc_VOL(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>& vecConfigText)
{//vecKLine 日期由近到远，今天的在0序号
	if(vecConfig.size() == 0)
		return;
	ASSERT(vecConfig.size() == vecConfigText.size());

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;
	m_strTechName = VOL;

	m_vecData = new VecTechIndexValue[vecConfig.size()];
	m_nVecDataCount = vecConfig.size();
	m_nDrawBody = vecConfig.size();
	
	for(int i = 0; i < (int)vecConfig.size(); i++)
	{
		int nDay = vecConfig[i];
		if(nDay == 0)
			continue;

		VecTechIndexValue& pTech = m_vecData[i];
		for(int nItem = 0; nItem < (int)vecKLine.size(); nItem++)
		{
			SKLine& pKline = vecKLine[nItem];
			STechIndexValue sValue;
			sValue.tmTime  = pKline.dwTime;
			sValue.strName = vecConfigText[i];//ma1
			sValue.fValue  = MIN_VALUE_DEFAULT;
			if(nItem + nDay > (int)vecKLine.size())
			{//超出范围，没有形成足够天数，比如只有四天数据，自然没有5日线

			}
			else
			{
				double dbValue = 0.0;
				for(int nBegin = nItem; nBegin < nItem + nDay; nBegin++)
				{
					SKLine& pKBegin = vecKLine[nBegin];
					dbValue = dbValue + pKBegin.dwVolume;
				}
				sValue.fValue = dbValue/nDay;
			}
			pTech.push_back(sValue);
		}	

	}
}
void CTechIndex::Calc_KDJ(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 3)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = KDJ;
	if(vecKLine.size() == 0)
		return;

	m_nVecDataCount = 3;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];//最后一个数组用于保存RSV数据
	m_nDrawBody = 3;
	

	m_vecMiddle = new VecTechIndexValue[1];
	m_nVecMiddleCount = 1;
	int nSize = vecKLine.size();
	for(int i = nSize-2; i>=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];//第2天

		float fMax =MIN_VALUE_DEFAULT , fMin = MAX_VALUE_DEFAULT;		
		for(int j=0; j< vecConfig[0] && (i+j)<nSize; j++)
		{
			SKLine& pStkInfo = vecKLine[i+j];
			if(fMax < pStkInfo.fHighPrice)
				fMax = pStkInfo.fHighPrice;
			if(fMin > pStkInfo.fLowPrice)
				fMin = pStkInfo.fLowPrice;				
		}

		SKLine& pStkInfo0 = vecKLine[i+1];//SHANG一天

		if(i== nSize -2)
		{
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo0.dwTime;
			sValue.strName = vecConfigText[0];//ma1
			sValue.fValue  = MIN_VALUE_DEFAULT;
			for(int p=0;p <nSize; p++)
			{
				m_vecData[0].push_back(sValue);
				m_vecData[1].push_back(sValue);
				m_vecData[2].push_back(sValue);
				m_vecMiddle[0].push_back(sValue);
			}
			if(IsDoubleSame(fMax, fMin))
				sValue.fValue = 100;
			else
				sValue.fValue = (pStkInfo.fClosePrice - fMin)/(fMax - fMin) *100;
			
			m_vecData[0][i]= sValue;
			sValue.strName = vecConfigText[1];
			m_vecData[1][i]= sValue;
			sValue.strName = vecConfigText[2];
			m_vecData[2][i]= sValue;
			m_vecMiddle[0][i]= sValue;		
			continue;
		}
		STechIndexValue sValue;
		sValue.tmTime  = pStkInfo0.dwTime;
		sValue.strName = vecConfigText[2];
		sValue.fValue  = MIN_VALUE_DEFAULT;

		if(IsDoubleSame(fMax, fMin))
			sValue.fValue = 100;
		else
			sValue.fValue= (pStkInfo.fClosePrice - fMin)/(fMax - fMin) *100;
	
		m_vecMiddle[0][i] = sValue;

		sValue.strName = vecConfigText[0];
		sValue.fValue  = (float)( (vecConfig[1]-1.0)/vecConfig[1] * m_vecData[0][i+1].fValue+ 1.0/vecConfig[1] * m_vecMiddle[0][i].fValue );
		m_vecData[0][i] = sValue;
		
		sValue.strName = vecConfigText[1];
		sValue.fValue  = (float)( (vecConfig[1]-1.0)/vecConfig[1] * m_vecData[1][i+1].fValue + 1.0/vecConfig[1] * m_vecData[0][i].fValue    );
		m_vecData[1][i] = sValue;
	
		sValue.strName = vecConfigText[2];
		sValue.fValue  =  vecConfig[2] * m_vecData[0][i].fValue -  (vecConfig[2]-1)*m_vecData[1][i].fValue;
		m_vecData[2][i] = sValue;
	}
	
}
void CTechIndex::Calc_MACD(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{//vecConfigText文字的次序：DIFF,DEA,MACD; vecConfig 12;26;9
	if(vecConfig.size() != 3)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = MACD;
	if(vecKLine.size() == 0)
		return;

	m_nVecDataCount   = 3;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 2;
	

	m_nVecMiddleCount = 2;
	m_vecMiddle = new VecTechIndexValue[2];
	int nSize = vecKLine.size();
	for(int i= nSize -1; i>=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];//第2天
		if(i== nSize-1)
		{
			STechIndexValue sValue;
			sValue.fValue = MIN_VALUE_DEFAULT;
			for(int p=0;p <nSize; p++)
			{
				m_vecMiddle[0].push_back(sValue);
				m_vecMiddle[1].push_back(sValue);
				m_vecData[0].push_back(sValue);
				m_vecData[1].push_back(sValue);
				m_vecData[2].push_back(sValue);				
			}
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "EMA12";
			sValue.fValue  = pStkInfo.fClosePrice;
			m_vecMiddle[0][i] = sValue;
				
			sValue.strName = "EMA26";
			m_vecMiddle[1][i] = sValue;
			
			sValue.strName = "DIFF";
			sValue.fValue  = 0.0;
			m_vecData[0][i] = sValue;

			sValue.strName = "DEA";
			sValue.fValue  = 0.0;
			m_vecData[1][i] = sValue;

			sValue.strName = MACD;
			m_vecData[2][i] = sValue;
			continue;
		}
		SKLine& pStkInfo0 = vecKLine[i+1];
/*/////////////////////////////////////////////////////////////////////////
nday12 = vecConfig[0];
/////////////////////////////////////////////////////////////////////////*/
		STechIndexValue sValue;
		sValue.tmTime  = pStkInfo.dwTime;
		sValue.strName = "EMA12";
		sValue.fValue  = (float)( (vecConfig[1] -1.0)/(vecConfig[1] +1)*m_vecMiddle[0][i+1].fValue + 2.0/(vecConfig[1] + 1)*pStkInfo.fClosePrice);		
		m_vecMiddle[0][i] = sValue;

		sValue.strName = "EMA26";
		sValue.fValue  = (float)((vecConfig[0] -1.0)/(vecConfig[0] +1)*m_vecMiddle[1][i+1].fValue + 2.0/(vecConfig[0] + 1)*pStkInfo.fClosePrice);
		m_vecMiddle[1][i] = sValue;

		sValue.strName = "DIFF";
		sValue.fValue  = m_vecMiddle[0][i].fValue - m_vecMiddle[1][i].fValue;
		m_vecData[0][i] = sValue;
		
		sValue.strName = "DEA";
		sValue.fValue  = (float)((vecConfig[2] -1.0)/(vecConfig[2] + 1.0)*m_vecData[1][i+1].fValue + 2.0/(vecConfig[2] + 1.0)*m_vecData[0][i].fValue);;
		m_vecData[1][i] = sValue;

		sValue.strName = MACD;
		sValue.fValue  = 2*(m_vecData[0][i].fValue - m_vecData[1][i].fValue);
		m_vecData[2][i] = sValue;
	}
}
void CTechIndex::Calc_BOLL(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 2)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = BOLL;
	if(vecKLine.size() == 0)
		return;

	m_nVecDataCount   = 3;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 3;

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);
		m_vecData[1].push_back(sValue);
		m_vecData[2].push_back(sValue);				
	}
	float fSum = 0.0;
	for(int i = nSize -1; i>=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];
		fSum += pStkInfo.fClosePrice;
		if(i + vecConfig[0] <= nSize)
		{
			if(i + vecConfig[0] < nSize)
			{
				int nItem = i+vecConfig[0];
				SKLine& pStkInfoT = vecKLine[nItem];
				fSum -= pStkInfoT.fClosePrice;
			}
			float fMID = fSum/vecConfig[0];

			float fSumDev = 0.0;
			for(int j =0; j<vecConfig[0]; j++)
			{
				SKLine& pStkInfoT1 = vecKLine[i+j];
				fSumDev += (pStkInfoT1.fClosePrice - fMID)*(pStkInfoT1.fClosePrice - fMID);
			}
			
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "MID";
			sValue.fValue  = fMID;
			m_vecData[0][i] = sValue;

			sValue.strName = "UPPER";
			sValue.fValue  = float(fMID + vecConfig[1] * sqrt(fSumDev/vecConfig[0]));;
			m_vecData[1][i] = sValue;

			sValue.strName = "LOWER";
			sValue.fValue  = float(fMID - vecConfig[1] * sqrt(fSumDev/vecConfig[0]));;
			m_vecData[2][i] = sValue;
		}
	}

}
void CTechIndex::Calc_UOS(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 4)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = UOS;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 2;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 2;

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);
		m_vecData[1].push_back(sValue);		
	}
	float fSum71  = 0.0,  fSum72  = 0.0;
	float fSum141 = 0.0,  fSum142 = 0.0;
	float fSum281 = 0.0,  fSum282 = 0.0;

	for(int i = nSize-2; i>=0; i-- )
	{
		SKLine& pStkInfo = vecKLine[i];
		SKLine& pStkInfoBefore = vecKLine[i+1];
		float fTH = max(pStkInfo.fHighPrice , pStkInfoBefore.fClosePrice);
		float fTL = min(pStkInfo.fLowPrice , pStkInfoBefore.fClosePrice);
		fSum71 += pStkInfo.fClosePrice - fTL;
		fSum72 += fTH - fTL;
		if(i + vecConfig[0] < nSize-1)
		{
			SKLine& pStkInfoT = vecKLine[i + vecConfig[0]];
			SKLine& pStkInfoTBefore = vecKLine[i + vecConfig[0]+1];
			float fTH = max(pStkInfoT.fHighPrice, pStkInfoTBefore.fClosePrice);
			float fTL = min(pStkInfoT.fLowPrice, pStkInfoTBefore.fClosePrice);
			fSum71 -= pStkInfoT.fClosePrice - fTL;
			fSum72 -= fTH - fTL;
		}

		fSum141 += pStkInfo.fClosePrice - fTL;
		fSum142 += fTH - fTL;
		if(i + vecConfig[1] < nSize-1)
		{
			SKLine& pStkInfoT14 = vecKLine[i + vecConfig[1]];
			SKLine& pStkInfoT14Before = vecKLine[i + vecConfig[1] + 1];
			float fTH = max(pStkInfoT14.fHighPrice, pStkInfoT14Before.fClosePrice);
			float fTL = min(pStkInfoT14.fLowPrice, pStkInfoT14Before.fClosePrice);
			fSum141 -= pStkInfoT14.fClosePrice - fTL;
			fSum142 -= fTH - fTL;
		}

		fSum281 += pStkInfo.fClosePrice - fTL;
		fSum282 += fTH - fTL;
		if(i + vecConfig[2] <= nSize)
		{
			if(i + vecConfig[2] < nSize-1)
			{
				SKLine& pStkInfoT28 = vecKLine[i + vecConfig[2]];
				SKLine& pStkInfoT28Before = vecKLine[i + vecConfig[2] + 1];
				float fTH = max(pStkInfoT28.fHighPrice, pStkInfoT28Before.fClosePrice);
				float fTL = min(pStkInfoT28.fLowPrice, pStkInfoT28Before.fClosePrice);
				fSum281 -= pStkInfoT28.fClosePrice - fTL;
				fSum282 -= fTH - fTL;
			} 
			//输出终极指标:(ACC1*N2*N3+ACC2*N1*N3+ACC3*N1*N2)*100/(N1*N2+N1*N3+N2*N3)
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "UOS";

			double dbCount = 0.0;
			if(!IsDoubleSame(fSum72, 0))
				dbCount += fSum71/fSum72*vecConfig[1]*vecConfig[2];
			if(!IsDoubleSame(fSum142, 0))
				dbCount += fSum141/fSum142*vecConfig[0]*vecConfig[2];
			if(!IsDoubleSame(fSum282, 0))
				dbCount += fSum281/fSum282*vecConfig[0]*vecConfig[1];
			sValue.fValue  = dbCount*100/(vecConfig[1]*vecConfig[2] + vecConfig[0]*vecConfig[2] + vecConfig[0]*vecConfig[1]);
			m_vecData[0][i] = sValue;

			sValue.strName = "MUOS";
			if(i == 1818)
			{
				int i =0;
			}
			if(i + vecConfig[2] == nSize)
				m_vecData[1][i] = sValue;
			else
			{
				sValue.fValue  = (vecConfig[3] -1.0)/(vecConfig[3] +1)* m_vecData[1][i+1].fValue + 2.0/(vecConfig[3] + 1)* m_vecData[0][i].fValue;
				m_vecData[1][i] = sValue;
			}		
		}
	}


}
void CTechIndex::Calc_BIAS(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 3)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = BIAS;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 3;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 3;

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);
		m_vecData[1].push_back(sValue);		
		m_vecData[2].push_back(sValue);		
	}
	float f6days =0.0, f12days =0.0, f24days =0.0;
	for(int k = nSize-1; k>=0; k--)
	{
		SKLine& pStkInfo = vecKLine[k];
		f6days  = f6days + pStkInfo.fClosePrice;
		f12days = f12days + pStkInfo.fClosePrice;
		f24days = f24days + pStkInfo.fClosePrice;

		if(k <= nSize-vecConfig[0])
		{
			float fQuChuShouPanJia =0.0;
			if(k +vecConfig[0] <nSize)
			{
				fQuChuShouPanJia = vecKLine[k + vecConfig[0]].fClosePrice;
				f6days = f6days - fQuChuShouPanJia;
			}
			float fMA = f6days /vecConfig[0];	

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "BIAS1";
			sValue.fValue  = (pStkInfo.fClosePrice - fMA)/fMA *100;
			m_vecData[0][k] = sValue;
		} 

		if(k <= nSize-vecConfig[1])
		{
			float fQuChuShouPanJia =0.0;
			if(k +vecConfig[1] <nSize)
			{
				fQuChuShouPanJia = vecKLine[k + vecConfig[1]].fClosePrice;
				f12days = f12days - fQuChuShouPanJia;
			}
			float fMA = f12days /vecConfig[1];	

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "BIAS2";
			sValue.fValue  = (pStkInfo.fClosePrice - fMA)/fMA *100;
			m_vecData[1][k] = sValue;
		} 

		if(k <= nSize-vecConfig[2])
		{
			float fQuChuShouPanJia =0.0;
			if(k +vecConfig[2] <nSize)
			{
				fQuChuShouPanJia = vecKLine[k + vecConfig[2]].fClosePrice;
				f24days = f24days - fQuChuShouPanJia;
			}
			float fMA = f24days /vecConfig[2];	

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "BIAS3";
			sValue.fValue  = (pStkInfo.fClosePrice - fMA)/fMA *100;
			m_vecData[2][k] = sValue;
		} 
	}


}
void CTechIndex::Calc_DMI(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 2)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = DMI;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 4;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 4;

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);
		m_vecData[1].push_back(sValue);		
		m_vecData[2].push_back(sValue);		
		m_vecData[3].push_back(sValue);	
	}
	float fDMPSum =0.0, fDMMSum =0.0, fTRSum =0.0;
	float fADXSum = 0.0;
	for(int i = nSize -2; i >= 0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];
		SKLine pStkInfoBefore = vecKLine[i+1];
		float f1 = pStkInfo.fHighPrice - pStkInfo.fLowPrice;
		float f2 = pStkInfo.fHighPrice - pStkInfoBefore.fClosePrice;
		if(f2 <0)
			f2 = -f2;
		float f3 = pStkInfoBefore.fClosePrice - pStkInfo.fLowPrice;
		if(f3 < 0)
			f3 = -f3;
		float fTR = f1;
		if(f2 > fTR)
			fTR = f2;
		if(f3 > fTR)
			fTR = f3;
		fTRSum  = fTRSum + fTR;
		SKLine& pStkInfo0 = vecKLine[i+1];
		float fHD = pStkInfo.fHighPrice - pStkInfo0.fHighPrice;
		float fLD = pStkInfo0.fLowPrice - pStkInfo.fLowPrice;

		float fDMP = 0;
		if(fHD > 0 &&  fHD > fLD)
			fDMP = fHD;
		float fDMM = 0;
		if(fLD >0 && fLD > fHD)
			fDMM = fLD;
		fDMPSum = fDMPSum + fDMP;
		fDMMSum = fDMMSum + fDMM;

		if(i + vecConfig[0] <= nSize)
		{
			if(i+vecConfig[0] < nSize-1)//第一天不是有效数据，不需要处理
			{
				SKLine& pStkInfoT = vecKLine[i+vecConfig[0]];
				SKLine& pStkInfoTBefore = vecKLine[i+vecConfig[0] +1];
				float f1 = pStkInfoT.fHighPrice - pStkInfoT.fLowPrice;
				float f2 = (float)fabs(pStkInfoT.fHighPrice - pStkInfoTBefore.fClosePrice);
				float f3 = (float)fabs(pStkInfoTBefore.fClosePrice - pStkInfoT.fLowPrice);
				float fTR = f1;
				if(f2 > fTR)
					fTR = f2;
				if(f3 > fTR)
					fTR = f3;
				fTRSum  = fTRSum - fTR;

				SKLine pStkInfoT0 = vecKLine[i+vecConfig[0]+1];

				float fHD = pStkInfoT.fHighPrice - pStkInfoT0.fHighPrice;
				float fLD = pStkInfoT0.fLowPrice - pStkInfoT.fLowPrice;

				float fDMP = 0;
				if(fHD > 0 &&  fHD > fLD)
					fDMP = fHD;
				float fDMM = 0;
				if(fLD >0 && fLD > fHD)
					fDMM = fLD;
				fDMPSum = fDMPSum - fDMP;
				fDMMSum = fDMMSum - fDMM;
			}

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "PDI";			
			if(fTRSum <0.00001 && fTRSum >-0.00001)
				sValue.fValue = 0;//出现0的时候这个值是否赋值为0不是很清楚，目前也没有专业资料可参考
			else
				sValue.fValue  = fDMPSum *100/vecConfig[0]/(fTRSum/vecConfig[0]);
			m_vecData[0][i] = sValue;

			sValue.strName = "MDI";
			if(fTRSum <0.00001 && fTRSum >-0.00001)
				sValue.fValue = 0;//出现0的时候这个值是否赋值为0不是很清楚，目前也没有专业资料可参考
			else
				sValue.fValue  = fDMMSum *100/vecConfig[0]/(fTRSum/vecConfig[0]);		
			m_vecData[1][i] = sValue;
			if(m_vecData[1][i].fValue + m_vecData[0][i].fValue < 0.00001 && m_vecData[1][i].fValue + m_vecData[0][i].fValue >-0.00001)
				fADXSum = 0;//出现0的时候这个值是否赋值为0不是很清楚，目前也没有专业资料可参考
			else
				fADXSum =( fADXSum + (float)fabs(m_vecData[1][i].fValue - m_vecData[0][i].fValue)/(m_vecData[1][i].fValue + m_vecData[0][i].fValue)*100);
		}

		if(i + vecConfig[0] + vecConfig[1] -1 <= nSize)
		{	
			if(i + vecConfig[0] + vecConfig[1] -1 < nSize)
			{
				if((int)m_vecData[0].size() > vecConfig[1] && (int)m_vecData[1].size() > vecConfig[1])
				{	
					if(m_vecData[1][i + vecConfig[1]].fValue + m_vecData[0][i + vecConfig[1]].fValue < 0.00001 && 
						m_vecData[1][i + vecConfig[1]].fValue + m_vecData[0][i + vecConfig[1]].fValue >-0.00001)
						fADXSum = 0;//出现0的时候这个值是否赋值为0不是很清楚，目前也没有专业资料可参考
					else
						fADXSum = fADXSum - (float)fabs(m_vecData[1][i + vecConfig[1]].fValue - m_vecData[0][i + vecConfig[1]].fValue)/(m_vecData[1][i + vecConfig[1]].fValue + m_vecData[0][i + vecConfig[1]].fValue)*100;
				}
			}
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "ADX";
			sValue.fValue  = fADXSum/vecConfig[1]; 
			m_vecData[2][i] = sValue;
		}

		if(i + vecConfig[0] + vecConfig[1] + vecConfig[1] -1 <= nSize)
		{	
			if((int)m_vecData[2].size() > vecConfig[1])
			{
				STechIndexValue sValue;
				sValue.tmTime  = pStkInfo.dwTime;
				sValue.strName = "ADXR";
				sValue.fValue  = (m_vecData[2][i + vecConfig[1]].fValue + m_vecData[2][i].fValue)/2;
				m_vecData[3][i] = sValue;
			}
		}
	}
}
void CTechIndex::Calc_PSY(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 1)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = PSY;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 1;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 1;

	int nSize = vecKLine.size();
	if(nSize < vecConfig[0])
		return;
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);		
	}
	int nSum12 =0; 
	for(int i = nSize -2; i>=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];
		SKLine& pStkInfoBefore = vecKLine[i+1];
		if(pStkInfo.fClosePrice > pStkInfoBefore.fClosePrice)
		{
			nSum12 = nSum12 +1;			
		}
		if(i + vecConfig[0] <= nSize)
		{
			if(i + vecConfig[0]< nSize-1)//第一天不算，所以nSize -1
			{
				SKLine& pStkInfoT0 = vecKLine[i+vecConfig[0]];
				SKLine& pStkInfoT0Before = vecKLine[i+vecConfig[0]+1];
				if(pStkInfoT0.fClosePrice > pStkInfoT0Before.fClosePrice)
					nSum12 = nSum12 - 1;					
			}

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = " ";
			sValue.fValue  = (float)100.0*nSum12/vecConfig[0];
			//m_vecData[0].insert(m_vecData[0].begin(), sValue);	
			m_vecData[0][i] = sValue;
		}		
	}	
}
void CTechIndex::Calc_ROC(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 2)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = ROC;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 2;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 2;

	int nSize = vecKLine.size();

	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);
		m_vecData[1].push_back(sValue);			
	}
	float fSum = 0.0;
	for(int i = nSize - vecConfig[0] -1; i>=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];
		SKLine& pStkInfo0 = vecKLine[i+ vecConfig[0]];
	
		STechIndexValue sValue;
		sValue.tmTime  = pStkInfo.dwTime;
		sValue.strName = "ROC";
		sValue.fValue  = 100 * (pStkInfo.fClosePrice - pStkInfo0.fClosePrice)/pStkInfo0.fClosePrice;
		//m_vecData[0].insert(m_vecData[0].begin(), sValue);	
		m_vecData[0][i] = sValue;

		fSum = fSum + m_vecData[0][i].fValue;
		if(i + vecConfig[0] + vecConfig[1]  <= nSize)
		{	
			if(i + vecConfig[0] + vecConfig[1]  < nSize)
			{
				fSum = fSum - m_vecData[0][i+vecConfig[1]].fValue;
			}
			sValue.strName = "ROCMA";
			sValue.fValue  = fSum/vecConfig[1]; 
			//m_vecData[1].insert(m_vecData[1].begin(), sValue);	
			m_vecData[1][i] = sValue;
		}
	}	
}
void CTechIndex::Calc_BBI(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 4)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = BBI;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 1;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 1;

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);		
	}
	float n3DaysChengJiaoLiang =0, n3DaysChengJiaoJine =0;
	float n6DaysChengJiaoLiang =0, n6DaysChengJiaoJine =0;
	float n12DaysChengJiaoLiang =0, n12DaysChengJiaoJine =0;
	float n24DaysChengJiaoLiang =0, n24DaysChengJiaoJine =0;

	for(int k = nSize-1; k>=0; k--)
	{
		SKLine& pStkInfo = vecKLine[k];
		n3DaysChengJiaoLiang += pStkInfo.dwVolume; n3DaysChengJiaoJine += pStkInfo.fSumTradeVolume;
		n6DaysChengJiaoLiang += pStkInfo.dwVolume; n6DaysChengJiaoJine += pStkInfo.fSumTradeVolume;
		n12DaysChengJiaoLiang += pStkInfo.dwVolume; n12DaysChengJiaoJine += pStkInfo.fSumTradeVolume;
		n24DaysChengJiaoLiang += pStkInfo.dwVolume; n24DaysChengJiaoJine += pStkInfo.fSumTradeVolume;

		float COSTMA3 = 0.0, COSTMA6 = 0.0, COSTMA12 = 0.0,COSTMA24 = 0.0;
		int nDays = vecConfig[0];

		if(k <= nSize-nDays)
		{
			if(k +nDays <nSize)
			{				
				float nChengJiaoLiang = vecKLine[k + nDays].dwVolume;
				float nChengJiaoJine  = vecKLine[k + nDays].fSumTradeVolume;
				n3DaysChengJiaoLiang -= nChengJiaoLiang;
				n3DaysChengJiaoJine  -=  nChengJiaoJine;
			}
			COSTMA3 = (float)n3DaysChengJiaoJine /n3DaysChengJiaoLiang/sTCPara.m_VolumeMultiple;		
		} 


		nDays = vecConfig[1];

		if(k <= nSize-nDays)
		{
			if(k +nDays <nSize)
			{				
				float	nChengJiaoLiang = vecKLine[k + nDays].dwVolume;
				float	nChengJiaoJine  = vecKLine[k + nDays].fSumTradeVolume;
				n6DaysChengJiaoLiang -= nChengJiaoLiang;
				n6DaysChengJiaoJine  -=  nChengJiaoJine;
			}
			COSTMA6 = (float)n6DaysChengJiaoJine /n6DaysChengJiaoLiang/sTCPara.m_VolumeMultiple;		
		} 

		nDays = vecConfig[2];

		if(k <= nSize-nDays)
		{
			if(k +nDays <nSize)
			{				
				float	nChengJiaoLiang = vecKLine[k + nDays].dwVolume;
				float	nChengJiaoJine  = vecKLine[k + nDays].fSumTradeVolume;
				n12DaysChengJiaoLiang -= nChengJiaoLiang;
				n12DaysChengJiaoJine  -=  nChengJiaoJine;
			}
			COSTMA12 = (float)n12DaysChengJiaoJine /n12DaysChengJiaoLiang/sTCPara.m_VolumeMultiple;		

		} 
		nDays = vecConfig[3];

		if(k <= nSize-nDays)
		{
			if(k +nDays <nSize)
			{				
				float	nChengJiaoLiang = vecKLine[k + nDays].dwVolume;
				float	nChengJiaoJine  = vecKLine[k + nDays].fSumTradeVolume;
				n24DaysChengJiaoLiang -= nChengJiaoLiang;
				n24DaysChengJiaoJine  -=  nChengJiaoJine;
			}
			COSTMA24 = (float)n24DaysChengJiaoJine /n24DaysChengJiaoLiang/sTCPara.m_VolumeMultiple;
		//	pStkInfo.sJiShu.fZhiBiao[CBBI_1] = (COSTMA3 + COSTMA6 + COSTMA12 + COSTMA24)/4;

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "";
			sValue.fValue  = (COSTMA3 + COSTMA6 + COSTMA12 + COSTMA24)/4;
			m_vecData[0][k] = sValue;
		} 


	}

}
void CTechIndex::Calc_EXPMA(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 4)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = EXPMA;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 4;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 4;

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);		
		m_vecData[1].push_back(sValue);		
		m_vecData[2].push_back(sValue);		
		m_vecData[3].push_back(sValue);		
	}

	for(int k = nSize-1; k>=0; k--)
	{
		SKLine& pStkInfo = vecKLine[k];
		if(k== nSize-1)
		{
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "MA1";
			sValue.fValue  = pStkInfo.fClosePrice;
			m_vecData[0][k] = sValue;
			sValue.strName = "MA2";
			m_vecData[1][k] = sValue;
			sValue.strName = "MA3";
			m_vecData[2][k] = sValue;
			sValue.strName = "MA4";
			m_vecData[3][k] = sValue;
			continue;
		}
		STechIndexValue sValue;
		sValue.tmTime  = pStkInfo.dwTime;
		sValue.strName = "MA1";
		sValue.fValue  = m_vecData[0][k+1].fValue * (vecConfig[0] -1)/(vecConfig[0] + 1) + pStkInfo.fClosePrice*2/(vecConfig[0] +1);
		m_vecData[0][k] = sValue;

		sValue.strName = "MA2";
		sValue.fValue  = m_vecData[1][k+1].fValue * (vecConfig[1] -1)/(vecConfig[1] + 1) + pStkInfo.fClosePrice*2/(vecConfig[1] +1);
		m_vecData[1][k] = sValue;

		sValue.strName = "MA3";
		sValue.fValue  = m_vecData[2][k+1].fValue * (vecConfig[2] -1)/(vecConfig[2] + 1) + pStkInfo.fClosePrice*2/(vecConfig[2] +1);
		m_vecData[2][k] = sValue;

		sValue.strName = "MA4";
		sValue.fValue  = m_vecData[3][k+1].fValue * (vecConfig[3] -1)/(vecConfig[3] + 1) + pStkInfo.fClosePrice*2/(vecConfig[3] +1);
		m_vecData[3][k] = sValue;
	}
}
void CTechIndex::Calc_DMA(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 3)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = DMA;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 2;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 2;

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);		
		m_vecData[1].push_back(sValue);		
	}

	float fSum10=0, fSum50 =0;
	float fSumdda = 0;
	for(int i= nSize -1; i>=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];
		fSum10 += pStkInfo.fClosePrice;
		fSum50 += pStkInfo.fClosePrice;

		if(i+ vecConfig[0] <= nSize)
		{
			if(i+ vecConfig[0]<nSize)
			{
				fSum10 -= vecKLine[i + vecConfig[0]].fClosePrice;
			}
		}
		if(i+ vecConfig[1] <= nSize)
		{
			if(i+ vecConfig[1]<nSize)
			{
				fSum50 -= vecKLine[i + vecConfig[1]].fClosePrice;
			}
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "DDD";
			sValue.fValue  =  fSum10/vecConfig[0] - fSum50/vecConfig[1];
			m_vecData[0][i]  = sValue;
			
		//	pStkInfo.sJiShu.fZhiBiao[DMA_DDA] = fSum10/vecConfig[0] - fSum50/vecConfig[1];
			fSumdda += sValue.fValue;//         pStkInfo.sJiShu.fZhiBiao[DMA_DDA];

			if(i + vecConfig[1] + vecConfig[2] -1 <= nSize)
			{
				if(i + vecConfig[1] + vecConfig[2] -1 < nSize)
				{
					fSumdda -= m_vecData[0][i+ vecConfig[2]].fValue;
				}
				STechIndexValue sValue;
				sValue.tmTime  = pStkInfo.dwTime;
				sValue.strName = "AMA";
				sValue.fValue  =  fSumdda/ vecConfig[2];
				m_vecData[1][i] = sValue;
				//pStkInfo.sJiShu.fZhiBiao[DMA_AMA] = fSumdda/ vecConfig[2];
			}


		}
	}
}
/*
HLC:=REF(MA((HIGH+LOW+CLOSE)/3,N),1);
HV:=EMA(HHV(HIGH,N),3);
LV:=EMA(LLV(LOW,N),3);
STOR:EMA(2*HV-LV,3);
MIDR:EMA(HLC+HV-LV,3);
WEKR:EMA(HLC*2-LV,3);
WEKS:EMA(HLC*2-HV,3);
MIDS:EMA(HLC-HV+LV,3);
STOS:EMA(2*LV-HV,3);

HLC赋值:昨日(最高价+最低价+收盘价)/3的N日简单移动平均
HV赋值:N日内最高价的最高值的3日指数移动平均
LV赋值:N日内最低价的最低值的3日指数移动平均
输出STOR:2*HV-LV的3日指数移动平均
输出MIDR:HLC+HV-LV的3日指数移动平均
输出WEKR:HLC*2-LV的3日指数移动平均
输出WEKS:HLC*2-HV的3日指数移动平均
输出MIDS:HLC-HV+LV的3日指数移动平均
输出STOS:2*LV-HV的3日指数移动平均
*/

void CTechIndex::Calc_SAR(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 3)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = SAR;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 1;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 0;//SAR需要特殊画法

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);				
	}
	
	if(nSize < vecConfig[0])
		return;

	float fSum10 = 0, fSum5 = 0, fSum50 = 0;
	for(int i = nSize-1; i>= nSize -10 && i>=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];
		fSum10 += pStkInfo.fClosePrice;
		if( i < nSize - 5)
			fSum5 += pStkInfo.fClosePrice;
		if( i <= nSize - 5 && i > nSize -10)
			fSum50 += pStkInfo.fClosePrice;
	}
	BOOL bShangShen = FALSE;
	BOOL bChange    = FALSE;
	float fYINZI = 0;
	if(fSum10/10 < fSum5/5 && fSum5 > fSum50)
		bShangShen = TRUE;
	int i =0;
	for( i = nSize -vecConfig[0]-1; i>=0; i--)
	{
		float fMin = MAX_VALUE_DEFAULT, fMax =MIN_VALUE_DEFAULT;
		for(int j =0; j<vecConfig[0]; j++)
		{//过去nDay10的最大最小值，不包括当日
			SKLine& pStkInfo = vecKLine[i+j+1];
			if(fMin > pStkInfo.fLowPrice)
				fMin = pStkInfo.fLowPrice;
			if(fMax < pStkInfo.fHighPrice)
				fMax = pStkInfo.fHighPrice;			
		}

		SKLine& pStkInfo = vecKLine[i];
		if(i == nSize -vecConfig[0]-1)
		{
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "";
			
			if(bShangShen)
			{
				sValue.fValue  =  fMin;
				m_vecData[0][i] = sValue;
			}				
			else 
			{
				sValue.fValue  =  fMax;
				m_vecData[0][i] = sValue;
			}				
			continue;
		}

		fYINZI += (float)vecConfig[1]/100.0;


		SKLine& pStkInfo0 = vecKLine[i+1];
		if(bShangShen)
		{//如果是上涨状态
			if(bChange)
			{//刚从下跌状态转变而来
				STechIndexValue sValue;
				sValue.tmTime  = pStkInfo.dwTime;
				sValue.strName = "";
				sValue.fValue  =  fMin;
				m_vecData[0][i] = sValue;
				
				fYINZI = (float)vecConfig[1]/100.0;
				bChange = FALSE;
				continue;
			}

			if(pStkInfo.fHighPrice < fMax || pStkInfo.fHighPrice < pStkInfo0.fHighPrice || IsDoubleSame(pStkInfo.fHighPrice, pStkInfo0.fHighPrice))
			{//今日没有新高
				fYINZI -= (float)vecConfig[1]/100.0;
				if(fYINZI < vecConfig[1]/100.0)
					fYINZI = (float)vecConfig[1]/100.0;
			}
			if(fYINZI > vecConfig[2]/100.0)
				fYINZI = (float)vecConfig[2]/100.0;

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "";
			sValue.fValue  =  m_vecData[0][i+1].fValue + fYINZI*(fMax - m_vecData[0][i+1].fValue);;
			m_vecData[0][i] = sValue;
		//	pStkInfo.sJiShu.fZhiBiao[SAR_SAR] = pStkInfo0.sJiShu.fZhiBiao[SAR_SAR] + fYINZI*(fMax - pStkInfo0.sJiShu.fZhiBiao[SAR_SAR]);
			if(m_vecData[0][i].fValue > pStkInfo.fLowPrice)
			{//上涨转为下跌
				bShangShen = !bShangShen;
				bChange = TRUE;
			}
		}
		else
		{
			if(bChange)
			{
				STechIndexValue sValue;
				sValue.tmTime  = pStkInfo.dwTime;
				sValue.strName = "";
				sValue.fValue  =  fMax;
				m_vecData[0][i] = sValue;
			//	pStkInfo.sJiShu.fZhiBiao[SAR_SAR] = fMax;
				fYINZI = (float)vecConfig[1]/100.0;
				bChange = FALSE;
				continue;
			}
			if(pStkInfo.fLowPrice > fMin || pStkInfo.fLowPrice > pStkInfo0.fLowPrice || IsDoubleSame(pStkInfo.fLowPrice, pStkInfo0.fLowPrice))
			{//今天没有新低或者今日最低价高于昨日的
				fYINZI -= (float)vecConfig[1]/100.0;
				if(fYINZI < vecConfig[1]/100.0)
					fYINZI = (float)vecConfig[1]/100.0;
			}
			if(fYINZI > vecConfig[2]/100.0)
				fYINZI = (float)vecConfig[2]/100.0;

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "";
			sValue.fValue  =  m_vecData[0][i+1].fValue - fYINZI*(m_vecData[0][i+1].fValue - fMin );
			m_vecData[0][i] = sValue;
		//	pStkInfo.sJiShu.fZhiBiao[SAR_SAR] = pStkInfo0.sJiShu.fZhiBiao[SAR_SAR]- fYINZI*(pStkInfo0.sJiShu.fZhiBiao[SAR_SAR] - fMin );
			if(m_vecData[0][i].fValue < pStkInfo.fHighPrice)
			{//下跌转为上涨
				bShangShen = !bShangShen;
				bChange = TRUE;
			}
		}
	}
}
void CTechIndex::Calc_TRIX(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 2)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = TRIX;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 2;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 2;//SAR需要特殊画法

	m_nVecMiddleCount = 3;
	m_vecMiddle = new VecTechIndexValue[3];

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);			
		m_vecData[1].push_back(sValue);	
		m_vecMiddle[0].push_back(sValue);	
		m_vecMiddle[1].push_back(sValue);
		m_vecMiddle[2].push_back(sValue);
	}

	if(nSize < vecConfig[0])
		return;
	float fSumTRIX =0.0;
	for(int i = nSize-1; i>=0; i-- )
	{
		SKLine& pStkInfo = vecKLine[i];
		if(i == nSize -1)
		{
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "TRIX_EMA12";
			sValue.fValue  = pStkInfo.fClosePrice;				
			m_vecMiddle[0][i] = sValue;

			sValue.strName = "TRIX_DEMA12";
			sValue.fValue  = pStkInfo.fClosePrice;				
			m_vecMiddle[1][i] = sValue;

			sValue.strName = "TRIX_TEMA12";
			sValue.fValue  = pStkInfo.fClosePrice;				
			m_vecMiddle[2][i] = sValue;

			continue;
		}
		STechIndexValue sValue;
		sValue.tmTime  = pStkInfo.dwTime;
		sValue.strName = "TRIX_EMA12";
		sValue.fValue  = m_vecMiddle[0][i+1].fValue*(vecConfig[0] -1)/(vecConfig[0]+1) + pStkInfo.fClosePrice*2/(vecConfig[0]+1);				
		m_vecMiddle[0][i] = sValue;

		sValue.strName = "TRIX_DEMA12";
		sValue.fValue  = m_vecMiddle[1][i+1].fValue*(vecConfig[0] -1)/(vecConfig[0]+1) + m_vecMiddle[0][i].fValue *2/(vecConfig[0]+1);			
		m_vecMiddle[1][i] = sValue;

		sValue.strName = "TRIX_TEMA12";
		sValue.fValue  = m_vecMiddle[2][i+1].fValue*(vecConfig[0] -1)/(vecConfig[0]+1) + m_vecMiddle[1][i].fValue *2/(vecConfig[0]+1);			
		m_vecMiddle[2][i] = sValue;

		sValue.strName = "TRIX";
		sValue.fValue  = 100*(m_vecMiddle[2][i].fValue - m_vecMiddle[2][i+1].fValue)/m_vecMiddle[2][i+1].fValue;
		m_vecData[0][i] = sValue;
/*
		SKLine& pStkInfo0 = vecKLine[i +1];
		pStkInfo.sJiShu.fZhiBiao[TRIX_EMA12] = pStkInfo0.sJiShu.fZhiBiao[TRIX_EMA12]*(vecConfig[0] -1)/(vecConfig[0]+1) + pStkInfo.fClosePrice*2/(vecConfig[0]+1);
		pStkInfo.sJiShu.fZhiBiao[TRIX_DEMA12] = pStkInfo0.sJiShu.fZhiBiao[TRIX_DEMA12]*(vecConfig[0] -1)/(vecConfig[0]+1) + pStkInfo.sJiShu.fZhiBiao[TRIX_EMA12] *2/(vecConfig[0]+1);
		pStkInfo.sJiShu.fZhiBiao[TRIX_TEMA12] = pStkInfo0.sJiShu.fZhiBiao[TRIX_TEMA12]*(vecConfig[0] -1)/(vecConfig[0]+1) + pStkInfo.sJiShu.fZhiBiao[TRIX_DEMA12] *2/(vecConfig[0]+1);
		pStkInfo.sJiShu.fZhiBiao[TRIX_TRIX]   = 100*(pStkInfo.sJiShu.fZhiBiao[TRIX_TEMA12] - pStkInfo0.sJiShu.fZhiBiao[TRIX_TEMA12])/pStkInfo0.sJiShu.fZhiBiao[TRIX_TEMA12];
*/		if(i <= nSize-1-vecConfig[0] )
			fSumTRIX +=  m_vecData[0][i].fValue;
			//fSumTRIX +=  pStkInfo.sJiShu.fZhiBiao[TRIX_TRIX];
		if(i <= nSize-vecConfig[0] - vecConfig[1])
		{
			if(i < nSize-vecConfig[0] - vecConfig[1])
				fSumTRIX = fSumTRIX - m_vecData[0][i+vecConfig[1]].fValue;
			//fSumTRIX = fSumTRIX - vecKLine[i+vecConfig[1]].sJiShu.fZhiBiao[TRIX_TRIX];

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "TRMA";
			sValue.fValue  = fSumTRIX/vecConfig[1];
			m_vecData[1][i] = sValue;
			//pStkInfo.sJiShu.fZhiBiao[TRIX_MATR] = fSumTRIX/vecConfig[1];
		}
	}
}

void CTechIndex::Calc_MTM(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 2)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = MTM;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 2;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 2;

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);			
		m_vecData[1].push_back(sValue);		
	}


	if(nSize < vecConfig[0])
		return;
	float fSumMTM =0.0;
	for(int i = nSize-1-vecConfig[0]; i>=0; i-- )
	{
		SKLine& pStkInfo = vecKLine[i];
		SKLine& pStkInfo0 = vecKLine[i + vecConfig[0]];
		
		STechIndexValue sValue;
		sValue.tmTime  = pStkInfo.dwTime;
		sValue.strName = "MTM";
		sValue.fValue  = pStkInfo.fClosePrice - pStkInfo0.fClosePrice;
		m_vecData[0][i] = sValue;
		fSumMTM += pStkInfo.fClosePrice - pStkInfo0.fClosePrice;

		if(i <= nSize-vecConfig[0] - vecConfig[1])
		{
			if(i < nSize-vecConfig[0] - vecConfig[1])
				fSumMTM = fSumMTM - m_vecData[0][i+vecConfig[1]].fValue;
		
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "MTMMA";
			sValue.fValue  = fSumMTM/vecConfig[1];
			m_vecData[1][i] =sValue;
		}
	}
}
void CTechIndex::Calc_CRTECH(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 4)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = CRTECH;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 4;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 4;

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);			
		m_vecData[1].push_back(sValue);		
		m_vecData[2].push_back(sValue);			
		m_vecData[3].push_back(sValue);		
	}

	float fSum1 =0.0, fSum2 = 0.0;
	float fSum5 =0.0, fSum10 =0.0, fSum20 =0.0;
	for(int i = nSize -2; i>=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];

		SKLine& pStkInfo0 = vecKLine[i+1];
		float fMID = (pStkInfo0.fHighPrice + pStkInfo0.fLowPrice + pStkInfo0.fClosePrice)/3;
		fSum1 = fSum1 + max(0.0, pStkInfo.fHighPrice - fMID);
		fSum2 = fSum2 + max(0.0, fMID - pStkInfo.fLowPrice);


		if(i + vecConfig[0] <= nSize)
		{
			if(i + vecConfig[0] < nSize-1)
			{
				SKLine& pStkInfoT = vecKLine[i + vecConfig[0]];				
				SKLine& pStkInfoT0 = vecKLine[i+ vecConfig[0] +1];
				float fMIDT = (pStkInfoT0.fHighPrice + pStkInfoT0.fLowPrice + pStkInfoT0.fClosePrice)/3;
				fSum1 = fSum1 - max(0.0, pStkInfoT.fHighPrice - fMIDT);
				fSum2 = fSum2 - max(0.0, fMIDT - pStkInfoT.fLowPrice);			
			}
			if(IsDoubleSame(fSum2, 0))
			{
				STechIndexValue sValue;
				sValue.tmTime  = pStkInfo.dwTime;
				sValue.strName = "CR";
				sValue.fValue  = m_vecData[0][i+1].fValue;
				m_vecData[0][i] =sValue;
			//	pStkInfo.sJiShu.fZhiBiao[CR_26] = pStkInfo0.sJiShu.fZhiBiao[CR_26];
			}
			else
			{
				STechIndexValue sValue;
				sValue.tmTime  = pStkInfo.dwTime;
				sValue.strName = "CR";
				sValue.fValue  = fSum1/fSum2 *100;
				m_vecData[0][i] =sValue;
				//pStkInfo.sJiShu.fZhiBiao[CR_26] = fSum1/fSum2 *100;
			}
			fSum5 = fSum5 + m_vecData[0][i].fValue;
			fSum10 = fSum10 + m_vecData[0][i].fValue;
			fSum20 = fSum20 + m_vecData[0][i].fValue;

		//	fSum5 = fSum5 + pStkInfo.sJiShu.fZhiBiao[CR_26];
		//	fSum10 = fSum10 + pStkInfo.sJiShu.fZhiBiao[CR_26];
		//	fSum20 = fSum20 + pStkInfo.sJiShu.fZhiBiao[CR_26];
		}

		if(i + vecConfig[0] + vecConfig[1]-1 <= nSize)
		{
			if(i + vecConfig[0] + vecConfig[1]-1 < nSize)
			{
				fSum5 = fSum5 -  m_vecData[0][i  + vecConfig[1]].fValue;
				//SKLine& pStkInfoT1 = vecKLine[i  + vecConfig[1]];
				//fSum5 = fSum5 - pStkInfoT1.sJiShu.fZhiBiao[CR_26];
			}
			if(i - int(vecConfig[1]/2.5+1) >=0)
			{
				STechIndexValue sValue;
				sValue.tmTime  = pStkInfo.dwTime;
				sValue.strName = "MA1";
				sValue.fValue  = fSum5/vecConfig[1];

				m_vecData[1][i - int(vecConfig[1]/2.5+1)] = sValue;
				//SKLine& pStkInfoT2 = vecKLine[i - int(vecConfig[1]/2.5+1) ];
				//pStkInfoT2.sJiShu.fZhiBiao[CR_MA5] = fSum5/vecConfig[1];				
			}
		}

		if(i + vecConfig[0] + vecConfig[2]-1 <= nSize)
		{
			if(i + vecConfig[0] + vecConfig[2]-1 < nSize)
			{
				//SKLine& pStkInfoT1 = vecKLine[i  + vecConfig[2]];
				//fSum10 = fSum10 - pStkInfoT1.sJiShu.fZhiBiao[CR_26];
				fSum10 = fSum10 - m_vecData[0][i  + vecConfig[2]].fValue;
			}
			if(i - int(vecConfig[2]/2.5+1) >=0)
			{
				STechIndexValue sValue;
				sValue.tmTime  = pStkInfo.dwTime;
				sValue.strName = "MA2";
				sValue.fValue  = fSum10/vecConfig[2];

				m_vecData[2][i - int(vecConfig[2]/2.5+1)] = sValue;
			//	SKLine& pStkInfoT2 = vecKLine[i - int(vecConfig[2]/2.5+1) ];
			//	pStkInfoT2.sJiShu.fZhiBiao[CR_MA10]= fSum10/vecConfig[2];				
			}
		}

		if(i + vecConfig[0] + vecConfig[3]-1 <= nSize)
		{
			if(i + vecConfig[0] + vecConfig[3]-1 < nSize)
			{
				fSum20 = fSum20 - m_vecData[0][i  + vecConfig[3]].fValue;
			//	SKLine& pStkInfoT1 = vecKLine[i  + vecConfig[3]];
			//	fSum20 = fSum20 - pStkInfoT1.sJiShu.fZhiBiao[CR_26];
			}
			if(i - int(vecConfig[3]/2.5+1) >=0)
			{
				STechIndexValue sValue;
				sValue.tmTime  = pStkInfo.dwTime;
				sValue.strName = "MA3";
				sValue.fValue  = fSum20/vecConfig[3];			

				m_vecData[3][i - int(vecConfig[3]/2.5+1)] = sValue;
			//	SKLine& pStkInfoT2 = vecKLine[i - int(vecConfig[3]/2.5+1) ];
			//	pStkInfoT2.sJiShu.fZhiBiao[CR_MA20] = fSum20/vecConfig[3];				
			}
		}
	}
	
}
void CTechIndex::Calc_VR(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 2)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = VR;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 2;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 2;

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);		
		m_vecData[1].push_back(sValue);
	}

	float fSumZhang =0.0, fSumDie =0.0, fSumHengPan = 0.0;
	float fSumVR = 0.0;

	for(int i = nSize -2; i>=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];
		SKLine& pStkInfo0 = vecKLine[i+1];
		if(pStkInfo.fClosePrice > pStkInfo0.fClosePrice)
			fSumZhang = fSumZhang + pStkInfo.dwVolume;
		else if(pStkInfo.fClosePrice < pStkInfo0.fClosePrice)
			fSumDie = fSumDie + pStkInfo.dwVolume;
		else
			fSumHengPan = fSumHengPan + pStkInfo.dwVolume;


		if(i + vecConfig[0]<= nSize)
		{
			if(i + vecConfig[0]< nSize-1)
			{
				SKLine pStkInfoT = vecKLine[i + vecConfig[0]];
				SKLine pStkInfoT0 = vecKLine[i + vecConfig[0]+1];
				if(pStkInfoT.fClosePrice > pStkInfoT0.fClosePrice)
					fSumZhang = fSumZhang - pStkInfoT.dwVolume;
				else if(pStkInfoT.fClosePrice < pStkInfoT0.fClosePrice)
					fSumDie = fSumDie - pStkInfoT.dwVolume;
				else
					fSumHengPan = fSumHengPan - pStkInfoT.dwVolume;			
			}
			if(IsDoubleSame(0.0, fSumDie*2+fSumHengPan))
			{
				STechIndexValue sValue;
				sValue.tmTime  = pStkInfo.dwTime;
				sValue.strName = "VR";
				sValue.fValue  = 100;	

				m_vecData[0][i]  = sValue;
				//pStkInfo.sJiShu.fZhiBiao[VR_VR26] = 100;
			}
			else
			{
				STechIndexValue sValue;
				sValue.tmTime  = pStkInfo.dwTime;
				sValue.strName = "VR";
				sValue.fValue  = 100*(fSumZhang)/(fSumDie+fSumHengPan);	

				m_vecData[0][i]  = sValue;
			//	pStkInfo.sJiShu.fZhiBiao[VR_VR26] = 100*(fSumZhang)/(fSumDie+fSumHengPan);
			}
			
			fSumVR = fSumVR + m_vecData[0][i].fValue;
			//fSumVR = fSumVR + pStkInfo.sJiShu.fZhiBiao[VR_VR26];


			if(i + vecConfig[0] + vecConfig[1] -1 <= nSize)
			{
				if(i + vecConfig[0] + vecConfig[1] -1< nSize)
				{
				//	SKLine pStkInfoT0 = vecKLine[i+nMADay6];
				//	fSumVR = fSumVR - pStkInfoT0.sJiShu.fZhiBiao[VR_VR26];		
					fSumVR = fSumVR - m_vecData[0][i+vecConfig[1]].fValue;
				}

				STechIndexValue sValue;
				sValue.tmTime  = pStkInfo.dwTime;
				sValue.strName = "MA1";
				sValue.fValue  = fSumVR/vecConfig[1];
				
				m_vecData[1][i]  = sValue;
				//pStkInfo.sJiShu.fZhiBiao[VR_MA6] = fSumVR/vecConfig[1];
			}

		}
	}
}
void CTechIndex::Calc_OBV(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 0)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = OBV;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 1;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 1;

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);			
	}

	
	float fSum = 0.0;
	float fSum12 = 0.0, fSum26 = 0.0;
	for(int i = nSize -2; i >=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];
		SKLine& pStkInfo0 = vecKLine[i+1];
		if(pStkInfo.fClosePrice == pStkInfo0.fClosePrice)
		{//横盘不变

		}
		else if(pStkInfo.fClosePrice > pStkInfo0.fClosePrice)
			fSum = fSum + pStkInfo.dwVolume;
		else
			fSum = fSum - pStkInfo.dwVolume;

		STechIndexValue sValue;
		sValue.tmTime  = pStkInfo.dwTime;
		sValue.strName = "";
		sValue.fValue  = fSum;
		m_vecData[0][i]  = sValue;
	//	pStkInfo.sJiShu.fZhiBiao[OBV_OBV] = fSum;		
	}
}
void CTechIndex::Calc_ASI(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 0)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = ASI;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 1;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 1;

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);			
	//	m_vecData[1].push_back(sValue);			
	}

	
	float fASI = 0;
	float fSumASI =0;
	//CFloatArray aryASI;
	//aryASI.Add(0);//主要是从nSize -2 开始的
	for(int i = nSize -2; i>=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];
		SKLine& pStkInfo0 = vecKLine[i+1];
		float fLC = pStkInfo0.fClosePrice;
		float fAA = (float)fabs(pStkInfo.fHighPrice - fLC);
		float fBB = (float)fabs(pStkInfo.fLowPrice - fLC);
		float fCC = (float)fabs(pStkInfo.fHighPrice - pStkInfo0.fLowPrice);
		float fDD = (float)fabs(fLC - pStkInfo0.fOpenPrice);
		float fR =0;
		if(fAA - fBB > 0.00001 && fAA - fCC > 0.00001)
			fR = fAA+ fBB/2 + fDD/4;
		else
		{
			if(fBB - fCC > 0.00001 && fBB - fAA > 0.00001)
				fR = fBB + fAA/2 + fDD/4;
			else
				fR = fCC+ fDD/4;
		}
		float fX = pStkInfo.fClosePrice - fLC + (pStkInfo.fClosePrice- pStkInfo.fOpenPrice)/2 + fLC - pStkInfo0.fOpenPrice;
		float fSI = 16*fX/fR*max(fAA, fBB);
		fASI += fSI;
		STechIndexValue sValue;
		sValue.tmTime  = pStkInfo.dwTime;
		sValue.strName = "";
		sValue.fValue  = fASI;
		m_vecData[0][i]  = sValue;
	
	//	fSumASI += fASI;
	//	aryASI.Add(pStkInfo.sJiShu.fZhiBiao[ASI_ASI]);
	//	if(i + nMADay6 <= nSize)
	//	{
	//		if(i+ nMADay6 < nSize)
	//		{
		//		fSumASI -= aryASI[nSize -1 -(i+nMADay6)];
	//		}
		//	pStkInfo.sJiShu.fZhiBiao[ASI_MAASI] = fSumASI/nMADay6;
	//	}
	}
//	aryASI.RemoveAll();
	
}

/*EMV
*
//招商证券600005 ZGGM BYJC 通过测试
VOLUME:=MA(VOL,N)/VOL;
MID:=100*(HIGH+LOW-REF(HIGH+LOW,1))/(HIGH+LOW);
EMV:MA(MID*VOLUME*(HIGH-LOW)/MA(HIGH-LOW,N),N);
MAEMV:MA(EMV,M);	

*/
void CTechIndex::Calc_EMV(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 2)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = EMV;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 2;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 2;

	m_nVecMiddleCount = 2;
	m_vecMiddle = new VecTechIndexValue[2];

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);			
		m_vecData[1].push_back(sValue);		
		m_vecMiddle[0].push_back(sValue);			
		m_vecMiddle[1].push_back(sValue);		
	}

	float fSumVolume = 0, fSumHigh_Low = 0, fSumEMV =0, fSumMAEMV = 0;
	for(int i= nSize -1; i>=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];
		fSumVolume += pStkInfo.dwVolume;
		fSumHigh_Low += pStkInfo.fHighPrice - pStkInfo.fLowPrice;
		float fVOLUME = 0, fMID = 0, fMAHigh_Low =0;
		if(i+1<nSize)
		{
			SKLine& pStkInfo0 = vecKLine[i+1];
			fMID = 100*(pStkInfo.fHighPrice + pStkInfo.fLowPrice - pStkInfo0.fHighPrice - pStkInfo0.fLowPrice)/(pStkInfo.fHighPrice + pStkInfo.fLowPrice);
		}
		if(i + vecConfig[0] <= nSize)
		{
			if(i + vecConfig[0] < nSize)
			{
				SKLine& pStkInfoT = vecKLine[i+vecConfig[0]];
				fSumVolume		-= pStkInfoT.dwVolume;
				fSumHigh_Low	-= pStkInfoT.fHighPrice - pStkInfoT.fLowPrice;
			}
			fVOLUME = fSumVolume/vecConfig[0]/pStkInfo.dwVolume;
			fMAHigh_Low = fSumHigh_Low/vecConfig[0];
			float fEMV = 0;
			if(IsDoubleSame(0.0, fMAHigh_Low))
			{
				fEMV= 0;
			}
			else
				fEMV = fMID * fVOLUME *(pStkInfo.fHighPrice - pStkInfo.fLowPrice)/fMAHigh_Low;
			fSumEMV += fEMV;

			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "";
			sValue.fValue  = fEMV;
			m_vecMiddle[0][i] = sValue;			
		}
		else
		{
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "";
			sValue.fValue  = 0;
			m_vecMiddle[0][i] = sValue;
		}			

		if(i + vecConfig[0] + vecConfig[0]-1<= nSize)
		{
			if(i + vecConfig[0]+ vecConfig[0]-1 < nSize)
			{
				fSumEMV -= m_vecMiddle[0][i+vecConfig[0]].fValue;
			}
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "EMV";
			sValue.fValue  = fSumEMV/vecConfig[0];;
			m_vecData[0][i] = sValue;

			fSumMAEMV += m_vecData[0][i].fValue;

			sValue.fValue  = m_vecData[0][i].fValue;
			m_vecMiddle[1][i] = sValue;	
		}
		else
		{
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "";
			sValue.fValue  = 0;
			m_vecMiddle[1][i] = sValue;
		}			

		if(i + vecConfig[0] + vecConfig[0] + vecConfig[1]-1-1<= nSize)
		{
			if(i + vecConfig[0]+ vecConfig[0] + vecConfig[1]-1-1 < nSize)
			{
				fSumMAEMV -= m_vecMiddle[1][i+vecConfig[1]].fValue;				
			}
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "EMVA";
			sValue.fValue  = fSumMAEMV/vecConfig[1];
			m_vecData[1][i] = sValue;
		}
	}	
}
void CTechIndex::Calc_CCI(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 1)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = CCI;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 1;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 1;

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);			
	}


	float fSum =0.0, fSumCCI = 0;
	for(int i = nSize -1; i>=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];
		float fTYP = (pStkInfo.fHighPrice + pStkInfo.fLowPrice + pStkInfo.fClosePrice)/3;
		fSum = fSum + fTYP;
		if(i+vecConfig[0] <= nSize)
		{
			if(i+vecConfig[0] < nSize)
			{
				SKLine& pStkInfoT = vecKLine[i+vecConfig[0]];
				float fTYPt = (pStkInfoT.fHighPrice + pStkInfoT.fLowPrice + pStkInfoT.fClosePrice)/3;
				fSum = fSum - fTYPt;			
			}

			float fAve = fSum/vecConfig[0];
			float fSumA = 0.0;
			for(int j =0; j< vecConfig[0]; j++)
			{//TYP的N日平均绝对偏差
				SKLine& pStkInfoT2 = vecKLine[i+j];
				float fTYPt2 = (pStkInfoT2.fHighPrice + pStkInfoT2.fLowPrice + pStkInfoT2.fClosePrice)/3;

				fSumA = fSumA + (float)fabs(fTYPt2-fAve);
			}
			//	输出CCI:(TYP-TYP的N日简单移动平均)/(0.015*TYP的N日平均绝对偏差)
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "";
			sValue.fValue  = (fTYP-fAve)/((float)0.015 * (fSumA/vecConfig[0]));
			m_vecData[0][i] = sValue;
			
		}
	}

}
void CTechIndex::Calc_RSI(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 3)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = RSI;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 3;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 3;

	m_nVecMiddleCount = 6;
	m_vecMiddle = new VecTechIndexValue[6];

	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);		
		m_vecData[1].push_back(sValue);	
		m_vecData[2].push_back(sValue);	

		m_vecMiddle[0].push_back(sValue);	
		m_vecMiddle[1].push_back(sValue);	
		m_vecMiddle[2].push_back(sValue);	
		m_vecMiddle[3].push_back(sValue);	
		m_vecMiddle[4].push_back(sValue);	
		m_vecMiddle[5].push_back(sValue);	
	}

	for(int i = nSize -1; i>=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];		
		if(i == nSize -1)
		{
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "RSI1";
			sValue.fValue  = 0;
			m_vecData[0][i] = sValue;

			m_vecMiddle[0][i] = sValue;
			m_vecMiddle[1][i] = sValue;
			m_vecMiddle[2][i] = sValue;
			m_vecMiddle[3][i] = sValue;
			m_vecMiddle[4][i] = sValue;
			m_vecMiddle[5][i] = sValue;	
			continue;
		} // 若Y=SMA(X,N,M) 则 Y=(M*X+(N-M)*Y')/N, 其中Y'表示上一周期Y值,N必须大于M。"
		SKLine& pStkInfo0 = vecKLine[i+1];
		float fSum1_6 = (max(pStkInfo.fClosePrice - pStkInfo0.fClosePrice, 0.0)+(vecConfig[0]-1)*m_vecMiddle[0][i+1].fValue)/vecConfig[0];
		float fSum2_6 = (float)(fabs(pStkInfo.fClosePrice - pStkInfo0.fClosePrice)+(vecConfig[0]-1)*m_vecMiddle[1][i+1].fValue)/vecConfig[0];
		
		STechIndexValue sValue;
		sValue.tmTime  = pStkInfo.dwTime;
		sValue.strName = "";
		sValue.fValue  = fSum1_6;
		m_vecMiddle[0][i] = sValue;

		sValue.fValue  = fSum2_6;
		m_vecMiddle[1][i] = sValue;

		sValue.strName = "RSI1";
		sValue.fValue  = fSum1_6/fSum2_6 *100;
		m_vecData[0][i] = sValue;

		float fSum1_12 = (max(pStkInfo.fClosePrice - pStkInfo0.fClosePrice, 0.0)+(vecConfig[1]-1)*m_vecMiddle[2][i+1].fValue)/vecConfig[1];
		float fSum2_12 = (float)(fabs(pStkInfo.fClosePrice - pStkInfo0.fClosePrice)+(vecConfig[1]-1)*m_vecMiddle[3][i+1].fValue)/vecConfig[1];
			
		sValue.fValue  = fSum1_12;
		m_vecMiddle[2][i] = sValue;
		sValue.fValue  = fSum2_12;
		m_vecMiddle[3][i] = sValue;

		sValue.strName = "RSI1";
		sValue.fValue  = fSum1_12/fSum2_12 *100;
		m_vecData[1][i] = sValue;

		float fSum1_24 = (max(pStkInfo.fClosePrice - pStkInfo0.fClosePrice, 0.0)+(vecConfig[2]-1)*m_vecMiddle[4][i+1].fValue)/vecConfig[2];
		float fSum2_24 = (float)(fabs(pStkInfo.fClosePrice - pStkInfo0.fClosePrice)+(vecConfig[2]-1)*m_vecMiddle[5][i+1].fValue)/vecConfig[2];
	
		sValue.fValue  = fSum1_24;
		m_vecMiddle[4][i] = sValue;
		sValue.fValue  = fSum2_24;
		m_vecMiddle[5][i] = sValue;

		sValue.strName = "RSI2";
		sValue.fValue  = fSum1_24/fSum2_24 *100;
		m_vecData[2][i] = sValue;
	}
}
void CTechIndex::Calc_CDP(vector<SKLine>& vecKLine, STechCalcPara& sTCPara, vector<int>& vecConfig, vector<string>&	vecConfigText)
{
	if(vecConfig.size() != 0)
		return;

	ClearMemory();	
	m_enumPhrase  = sTCPara.enumPhrase;
	m_vecConfig = vecConfig;
	m_vecConfigText = vecConfigText;

	m_strTechName = CDP;
	if(vecKLine.size() == 0)
		return;
	m_nVecDataCount   = 5;
	m_vecData = new VecTechIndexValue[m_nVecDataCount];
	m_nDrawBody = 5;


	int nSize = vecKLine.size();
	for(int p=0;p <nSize; p++)
	{
		STechIndexValue sValue;
		sValue.fValue = MIN_VALUE_DEFAULT;
		m_vecData[0].push_back(sValue);		
		m_vecData[1].push_back(sValue);	
		m_vecData[2].push_back(sValue);	
		m_vecData[3].push_back(sValue);	
		m_vecData[4].push_back(sValue);			
	}
	
	for(int i= nSize-1; i>=0; i--)
	{
		SKLine& pStkInfo = vecKLine[i];
		float fCDP = (pStkInfo.fHighPrice + pStkInfo.fLowPrice + pStkInfo.fClosePrice)/3;
	
		STechIndexValue sValue;
		sValue.tmTime  = pStkInfo.dwTime;
		sValue.strName = "CDP";
		sValue.fValue  = fCDP;
		m_vecData[0][i] = sValue;

		sValue.strName = "NH";
		sValue.fValue  = 2*fCDP - pStkInfo.fLowPrice;
		m_vecData[3][i] = sValue;

		sValue.strName = "NL";
		sValue.fValue  = 2*fCDP - pStkInfo.fHighPrice;
		m_vecData[4][i] = sValue;

	//	pStkInfo.sJiShu.fZhiBiao[CDP_CDP]= fCDP;
	//	pStkInfo.sJiShu.fZhiBiao[CDP_NH] = 2*fCDP - pStkInfo.fLowPrice;
	//	pStkInfo.sJiShu.fZhiBiao[CDP_NL] = 2*fCDP - pStkInfo.fHighPrice;

		if(i<nSize-1)
		{
			SKLine& pStkInfo0 = vecKLine[i+1];
			float fPT = pStkInfo0.fHighPrice - pStkInfo0.fLowPrice;
			
			STechIndexValue sValue;
			sValue.tmTime  = pStkInfo.dwTime;
			sValue.strName = "AH";
			sValue.fValue  = fCDP + fPT;
			m_vecData[1][i] = sValue;

			sValue.strName = "AL";
			sValue.fValue  = fCDP - fPT;
			m_vecData[2][i] = sValue;
		//	pStkInfo.sJiShu.fZhiBiao[CDP_AH] = fCDP + fPT;
		//	pStkInfo.sJiShu.fZhiBiao[CDP_AL] = fCDP - fPT;
		}
	}
	


}
// AlgoNo1.hpp : 定义算法交易一号系统
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "IniFile.h"
#include "Tools_Win32.h"


#include <math.h>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

//保存策略计算参数的ini文件名
#define IniFileName_AlgoNo1 "AlgoNo1.ini"


class CAlgo_No1
{
public:
	CAlgo_No1()
	{
		clear();
	};
	CAlgo_No1(const CAlgo_No1& r)
	{
		*this=r;
	}
	CAlgo_No1& operator= (const CAlgo_No1& r)
	{
		strInstrumentName=r.strInstrumentName;
		direction=r.direction;
		OffsetID=r.OffsetID;
		bTradeTrend=r.bTradeTrend;

		S=r.S;
		X=r.X;
		T=r.T;
		N=r.N;

		tau=r.tau;
		maxnj=r.maxnj;
		maxnj2=r.maxnj2;
		vecxj=r.vecxj;
		vecnj=r.vecnj;
		vecxj2=r.vecxj2;
		vecnj2=r.vecnj2;
		vecTradedVolume=r.vecTradedVolume;
		return *this;
	}
	void clear(void)
	{
		strInstrumentName.clear();
		direction=0;
		OffsetID=0;
		bTradeTrend=true;

		S=0.0f;
		X=0;
		T=1.0f;
		N=1;
		lambda=0;				//风险偏好λ
		sigma=0;				//日波动率σ
		alpha=0;				//日收益α
		eta=0;					//短期冲击成本参数η
		epsilon=0;				//短期冲击成本参数ε
		gamma=0;				//长期冲击成本参数γ


		tau=1.0f;
		maxnj=0.0f;
		maxnj2=0.0f;
		vecxj.clear();
		vecnj.clear();
		vecxj2.clear();
		vecnj2.clear();
		vecTradedVolume.clear();
	}
	//读取ini文件的配置信息
	bool LoadFromIni(void)
	{
		char Buf[512];
		char PathFileName[512];

		//获取程序路径
		memset(PathFileName,0,sizeof(PathFileName));
		CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(IniFileName_AlgoNo1));
		strcat(PathFileName,IniFileName_AlgoNo1);
		CIniFile *pIniFile=new CIniFile(PathFileName);
		if(!pIniFile) return false;

		pIniFile->GetIniKeyStr("Algo_No1","strInstrumentName","",Buf,sizeof(Buf));
		strInstrumentName=Buf;
		direction=pIniFile->GetIniKeyVal("Algo_No1","direction",1);
		OffsetID=pIniFile->GetIniKeyVal("Algo_No1","OffsetID",0);
		int itmp=pIniFile->GetIniKeyVal("Algo_No1","m_TradeTrend",0);
		bTradeTrend=itmp==0?false:true;

		S=pIniFile->GetIniKeyVal2("Algo_No1","S",0.0f);
		X=pIniFile->GetIniKeyVal("Algo_No1","X",0);
		T=pIniFile->GetIniKeyVal2("Algo_No1","T",1.0f);
		N=pIniFile->GetIniKeyVal("Algo_No1","N",1);

		lambda=pIniFile->GetIniKeyVal2("Algo_No1","lambda",0.0f);
		sigma=pIniFile->GetIniKeyVal2("Algo_No1","sigma",0.0f);
		alpha=pIniFile->GetIniKeyVal2("Algo_No1","alpha",0.0f);
		eta=pIniFile->GetIniKeyVal2("Algo_No1","eta",0.0f);
		epsilon=pIniFile->GetIniKeyVal2("Algo_No1","epsilon",0.0f);
		gamma=pIniFile->GetIniKeyVal2("Algo_No1","gamma",0.0f);

		tau=pIniFile->GetIniKeyVal2("Algo_No1","tau",0.0f);
		xbar=pIniFile->GetIniKeyVal2("Algo_No1","xbar",0.0f);
		etabar=pIniFile->GetIniKeyVal2("Algo_No1","etabar",0.0f);
		ksquare=pIniFile->GetIniKeyVal2("Algo_No1","ksquare",0.0f);
		k=pIniFile->GetIniKeyVal2("Algo_No1","k",0.0f);

		maxnj=pIniFile->GetIniKeyVal2("Algo_No1","maxnj",0.0f);
		maxnj2=pIniFile->GetIniKeyVal2("Algo_No1","maxnj2",0.0f);

		vecxj=pIniFile->GetIniKeyVal3("Algo_No1","vecxj");
		vecnj=pIniFile->GetIniKeyVal3("Algo_No1","vecnj");
		vecxj2=pIniFile->GetIniKeyVal3("Algo_No1","vecxj2");
		vecnj2=pIniFile->GetIniKeyVal3("Algo_No1","vecnj2");
		vecTradedVolume=pIniFile->GetIniKeyVal4("Algo_No1","vecTradedVolume");

		if(pIniFile)
		{
			delete pIniFile;
			pIniFile=NULL;
		}
		return true;
	}

	//保存配置信息到ini文件
	void SaveIni(void)
	{
		char PathFileName[512];

		//获取程序路径
		memset(PathFileName,0,sizeof(PathFileName));
		CTools_Win32::GetMainPath(PathFileName,sizeof(PathFileName)-1-strlen(IniFileName_AlgoNo1));
		strcat(PathFileName,IniFileName_AlgoNo1);
		CIniFile *pIniFile=new CIniFile(PathFileName);
		if(!pIniFile) return;

		pIniFile->UpdateKeyStr("Algo_No1","strInstrumentName",strInstrumentName.c_str());
		pIniFile->UpdateKeyVal("Algo_No1","direction",(int)direction);
		pIniFile->UpdateKeyVal("Algo_No1","OffsetID",(int)OffsetID);
		pIniFile->UpdateKeyVal("Algo_No1","bTradeTrend",(int)bTradeTrend);

		pIniFile->UpdateKeyVal2("Algo_No1","S",S);
		pIniFile->UpdateKeyVal("Algo_No1","X",(int)X);
		pIniFile->UpdateKeyVal2("Algo_No1","T",T);
		pIniFile->UpdateKeyVal("Algo_No1","N",(int)N);

		pIniFile->UpdateKeyVal2("Algo_No1","lambda",lambda);
		pIniFile->UpdateKeyVal2("Algo_No1","sigma",sigma);
		pIniFile->UpdateKeyVal2("Algo_No1","alpha",alpha);
		pIniFile->UpdateKeyVal2("Algo_No1","eta",eta);
		pIniFile->UpdateKeyVal2("Algo_No1","epsilon",epsilon);
		pIniFile->UpdateKeyVal2("Algo_No1","gamma",gamma);

		pIniFile->UpdateKeyVal2("Algo_No1","tau",tau);
		pIniFile->UpdateKeyVal2("Algo_No1","xbar",xbar);
		pIniFile->UpdateKeyVal2("Algo_No1","etabar",etabar);
		pIniFile->UpdateKeyVal2("Algo_No1","ksquare",ksquare);
		pIniFile->UpdateKeyVal2("Algo_No1","k",k);

		pIniFile->UpdateKeyVal2("Algo_No1","maxnj",maxnj);
		pIniFile->UpdateKeyVal2("Algo_No1","maxnj2",maxnj2);

		pIniFile->UpdateKeyVal3("Algo_No1","vecxj",vecxj);
		pIniFile->UpdateKeyVal3("Algo_No1","vecnj",vecnj);
		pIniFile->UpdateKeyVal3("Algo_No1","vecxj2",vecxj2);
		pIniFile->UpdateKeyVal3("Algo_No1","vecnj2",vecnj2);
		pIniFile->UpdateKeyVal4("Algo_No1","vecTradedVolume",vecTradedVolume);

		if(pIniFile)
		{
			delete pIniFile;
			pIniFile=NULL;
		}

	}
	bool calculate(string& errstr)
	{
		if(N==0) { errstr="时间分段数不能为零";  return false; }
		if(lambda==0) { errstr="风险偏好λ不能为零";  return false; }
		if(sigma==0) { errstr="日波动率σ不能为零";  return false; }

		//计算tau
		tau=T/(double)N;
		//计算xbar
		xbar=alpha/2.0f/lambda/sigma/sigma;
		//计算etabar
		etabar=eta-gamma*tau/2.0f;
		if(etabar==0) { errstr="计算过程中ηbar为零";  return false; }
		//计算k^2
		ksquare=lambda*sigma*sigma/etabar;
		if(ksquare<0) ksquare*=-1;
		//计算k
		k=sqrt(ksquare);

		vecxj.clear();
		vecnj.clear();
		vecxj2.clear();
		vecnj2.clear();
		vecTradedVolume.clear();

		//计算最终结果

		double tmpxj;
		double tmpsinh1,tmpsinh2,tmpsinh3;

		//计算xj无趋势/xj2有趋势
		for(int j=0;j<=N;j++)
		{
			tmpsinh1=sinh(k*(T-(double)j*tau));
			if(tmpsinh1==HUGE_VAL||tmpsinh1==-HUGE_VAL)
			{
				errstr="计算过程错误，sinh(k*(T-tj))结果错误！";
				return false;
			}
			tmpsinh2=sinh(k*T);
			if(tmpsinh2==HUGE_VAL||tmpsinh2==-HUGE_VAL||tmpsinh2==0)
			{
				errstr="计算过程错误，sinh(kT)结果错误！";
				return false;
			}
			tmpsinh3=sinh(k*(double)j*tau);
			if(tmpsinh3==HUGE_VAL||tmpsinh3==-HUGE_VAL)
			{
				errstr="计算过程错误，sinh(ktj)结果错误！";
				return false;
			}

			tmpxj=tmpsinh1/tmpsinh2*X;
			vecxj.push_back(tmpxj);

			tmpxj+=(1-(tmpsinh1+tmpsinh3)/tmpsinh2)*xbar;
			vecxj2.push_back(tmpxj);
		}
		
		maxnj=0.0f;
		maxnj2=0.0f;
		double tmpnj,tmpnj2;
		//计算nj无趋势和nj2有趋势
		for(int j=1;j<=N;j++)
		{
			tmpnj=vecxj[j-1]-vecxj[j];
			tmpnj2=vecxj2[j-1]-vecxj2[j];
			vecnj.push_back(tmpnj);
			vecnj2.push_back(tmpnj2);
			vecTradedVolume.push_back(0);
			if(maxnj<fabs(tmpnj)) maxnj=fabs(tmpnj);
			if(maxnj2<fabs(tmpnj2)) maxnj2=fabs(tmpnj2);
		}

		return true;
	}


	//输入项
	string			strInstrumentName;	//合约名称
	int				direction;			//方向，0:买；1:卖
	int				OffsetID;			//开平，0:开仓；1:平仓；2:平今
	bool			bTradeTrend;		//是否按有趋势的方案进行交易

	double			S;					//价格
	int				X;					//X手
	double			T;					//T日
	int				N;					//N份
	double			lambda;				//风险偏好λ
	double			sigma;				//日波动率σ
	double			alpha;				//日收益α
	double			eta;				//短期冲击成本参数η
	double			epsilon;			//短期冲击成本参数ε
	double			gamma;				//长期冲击成本参数γ

	//结果项
	//计算结果,xk(无趋势),nk(无趋势),xk2(有趋势),nk2(有趋势)
	double			tau;				//τ=T/N
	double			xbar;				//xbar=alpha/2/lambda/sigma/sigma
	double			etabar;				//etabar=eta-gamma*tau/2
	double			ksquare;			//ksquare=lambda*sigma*sigma/etabar;
	double			k;

	double			maxnj;
	double			maxnj2;
	vector<double>	vecxj;
	vector<double>	vecnj;
	vector<double>	vecxj2;
	vector<double>	vecnj2;
	vector<int>		vecTradedVolume;	//已成交的量

};


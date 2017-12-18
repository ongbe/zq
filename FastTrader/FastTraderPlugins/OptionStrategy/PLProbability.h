#pragma once
#include "NormalDistribution.h"
#include <math.h>

class CPLProbability
{
public:
    CPLProbability(void);
    ~CPLProbability(void);

    //构造
    //mu: 		标的物均价，采用当前价
    //sigma:	标的物价格年化波动率
    //days:	    距离期权到期日天数
    CPLProbability(double mu,double sigma,int days)
        : m_mu(mu)
    {
        m_sigma=sigma/sqrt((double)250)*sqrt((double)days);
    };

    //取当前显示的价格区间。取到后需要根据最小变动价位进行调整
    void GetRange(double& MinPrice,double& MaxPrice) 
    { 
        MinPrice=m_mu-m_sigma*m_mu*2.58;  MaxPrice=m_mu+m_sigma*m_mu*2.58; 
    };

    //计算价格对应的概率密度。当x=mu时，概率密度最大，对应到Y轴的Hei*90%处，以此确定纵坐标方向显示比例
    double CalcuProbabilityDensity(double xPrice) 
    { 
        return CNormalDistribution:: Calcu_ProbabilityDensity(xPrice,m_mu,m_sigma*m_mu); 
    };

    //计算价格对应的面积，积分区间为-无穷大至xPrice
    double CalcuDistribution(double xPrice) 
    { 
        return CNormalDistribution:: Calcu_Distribution(xPrice,m_mu,m_sigma*m_mu);
    }

private:
    double m_mu;
    double m_sigma;

};

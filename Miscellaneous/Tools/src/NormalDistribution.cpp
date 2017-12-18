#include "NormalDistribution.h"
#include <math.h>

#ifdef WIN32
#pragma unmanaged
#endif 

#define PI       3.14159265358979323846

//用于计算标准正态分布：正态分布函数近似值，使用6项近似余函数
double CNormalDistribution::CStandardNormalDistribution::Fi_erf_6(double x)
{
    double a=abs(x);
    return 0.5*(1+erf_6(a/sqrt(2.0)));
}

//用于计算标准正态分布：正态分布函数六项级数近似余函数
double CNormalDistribution::CStandardNormalDistribution::erf_6(double x)
{
    const double a[]={0.070523084,0.0422820123,0.0092705272,0.0001520143,0.0002765672,0.0000430638};
    double t=0;
    for(int i=0;i<6;i++)
	    t=t+a[i]*pow(x, i+1);
    return 1-pow(1+t, -16);
}

//********************************************************************************************
//计算标准正态分布积分 
//      积分区间: 负无穷大--x; 1/sqrt(2*pi)*exp(-pow(t,2)/2)dt
//参考：http://zh.wikipedia.org/wiki/%E5%B8%B8%E6%85%8B%E5%88%86%E4%BD%88
//参考：http://wenku.baidu.com/link?url=ygq3GC5X5x38PUMvOYz6sEvh12KM9ZDJHabYO-cpLHR1D6XvTpDzigKBNw7V0PBbFcnPA6FwNt2r4IbETUxlmyVxllJ7MZOCUFBoniDL0YO
//参考：正态函数求值算法的示例类,计算标准正态分布表,误差小于百万分之一
//      @author 欧阳康,北京师范大学教育技术学院,QQ:78692844
//********************************************************************************************
double  CNormalDistribution::CStandardNormalDistribution::Calcu_Distribution(double x)
{
    return x==0?0.5:(x>0?Fi_erf_6(x):1-Fi_erf_6(x));
}

//********************************************************************************************
//计算正态分布概率密度  
//      f(x)=1/sqrt(2*pi)*exp(-pow(x-mu,2)/(2*pow(mu,2)))
//参考：http://wenku.baidu.com/view/231fa2176edb6f1aff001f37.html
//参考：http://baike.baidu.com/view/45379.htm?fr=aladdin#7_1
//********************************************************************************************
double CNormalDistribution::Calcu_ProbabilityDensity(double x,double mu,double sigma)
{
    if(sigma==0) return 0.0;
    return exp(-1*pow(x-mu,2)/2/pow(sigma,2))/sqrt(2*PI)/sigma;
}

//********************************************************************************************
//计算正态分布积分  
//      u=(x-mu)/sigma, 再用u求标准正态分布
//参考：http://baike.baidu.com/view/45379.htm?fr=aladdin#7_1
//********************************************************************************************
double CNormalDistribution::Calcu_Distribution(double x,double mu,double sigma)
{
    if(sigma==0) return 0.0;
    double u=(x-mu)/sigma;
    return CStandardNormalDistribution::Calcu_Distribution(u);
}

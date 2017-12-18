/*
 * NormalDistribution.h
 *
 * 正态分布曲线计算
 *
 */

#ifndef NORMALDISTRIBUTION_H_
#define NORMALDISTRIBUTION_H_


#ifdef WIN32
#pragma managed(push,off)
#endif

class CNormalDistribution
{
public:

    class CStandardNormalDistribution
    {
    public:

        //********************************************************************************************
        //计算标准正态分布积分 
        //      积分区间: 负无穷大--x; 1/sqrt(2*pi)*exp(-t^2/2)dt; t=x
        //参考：http://zh.wikipedia.org/wiki/%E5%B8%B8%E6%85%8B%E5%88%86%E4%BD%88
        //参考：http://wenku.baidu.com/link?url=ygq3GC5X5x38PUMvOYz6sEvh12KM9ZDJHabYO-cpLHR1D6XvTpDzigKBNw7V0PBbFcnPA6FwNt2r4IbETUxlmyVxllJ7MZOCUFBoniDL0YO
        //参考：正态函数求值算法的示例类,计算标准正态分布表,误差小于百万分之一
        //      @author 欧阳康,北京师范大学教育技术学院,QQ:78692844
        //********************************************************************************************
        static double Calcu_Distribution(double x);

    private:
	    //用于计算标准正态分布：正态分布函数近似值，使用6项近似余函数
	    static double Fi_erf_6(double x);
	    //用于计算标准正态分布：正态分布函数六项级数近似余函数
	    static double erf_6(double x);
    };

    //********************************************************************************************
    //计算标准正态分布积分 
    //      积分区间: 负无穷大--x; 1/sqrt(2*pi)*exp(-t^2/2)dt; t=x
    //参考：http://zh.wikipedia.org/wiki/%E5%B8%B8%E6%85%8B%E5%88%86%E4%BD%88
    //参考：http://wenku.baidu.com/link?url=ygq3GC5X5x38PUMvOYz6sEvh12KM9ZDJHabYO-cpLHR1D6XvTpDzigKBNw7V0PBbFcnPA6FwNt2r4IbETUxlmyVxllJ7MZOCUFBoniDL0YO
    //参考：正态函数求值算法的示例类,计算标准正态分布表,误差小于百万分之一
    //      @author 欧阳康,北京师范大学教育技术学院,QQ:78692844
    //********************************************************************************************
    static double Calcu_Distribution(double x);

    //********************************************************************************************
    //计算正态分布概率密度  
    //      f(x)=1/sqrt(2*pi)*exp(-(x-mu)^2/(2*mu^2))
    //参考：http://wenku.baidu.com/view/231fa2176edb6f1aff001f37.html
    //参考：http://baike.baidu.com/view/45379.htm?fr=aladdin#7_1
    //********************************************************************************************
    static double Calcu_ProbabilityDensity(double x,double mu,double sigma);

    //********************************************************************************************
    //计算正态分布积分  u=(x-mu)/sigma, 再用u求标准正态分布
    //参考：http://baike.baidu.com/view/45379.htm?fr=aladdin#7_1
    //********************************************************************************************
    static double Calcu_Distribution(double x,double mu,double sigma);

};


#ifdef WIN32
#pragma managed(pop)
#endif

#endif /* NORMALDISTRIBUTION_H_ */

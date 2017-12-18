/*******************************************************************************
*	@file	:   util.h
*	@brief  :
*               1. 小工具函数的申明
*
*	@author :   jacky <panxun.li@cithinc.com>
*   @version:   0.1
*   @history:
*	            1. created on 2010/02/25
*
*******************************************************************************/

#ifndef _UTIL_H_
#define _UTIL_H_

#include <vector>
#include <cctype>  //for calling isspace
#include <algorithm> //for calling remove
#include <functional> //for calling not1,ptr_fun
#include <limits> //for numeric_limits<double>::max()

using namespace std;
namespace util
{
	/*---------------------------------------------------------
	|                      1,数字类小工具                     |
	---------------------------------------------------------*/

	inline int getExpoBase2(const double& d)
	{
		int i = 0;
		((short *)(&i))[0] = (((short *)(&d))[3] & (short)0x7ff0);
		return (i >> 4) - 1023;
	}

	inline bool	equals(const double& d1, const double& d2)
	{
		if (d1 == d2)
			return true;
		int e1 = getExpoBase2(d1);
		int e2 = getExpoBase2(d2);
		int e3 = getExpoBase2(d1 - d2);
		if ((e3 - e2 < -48) && (e3 - e1 < -48))
			return true;
		return false;
	}

	inline int compare(const double& d1, const double& d2)
	{
		if (equals(d1, d2) == true)
			return 0;
		if (d1 > d2)
			return 1;
		return -1;
	}

	inline bool	greater(const double& d1, const double& d2)
	{
		if (equals(d1, d2) == true)
			return false;
		if (d1 > d2)
			return true;
		return false;
	}

	inline bool	greaterOrEqual(const double& d1, const double& d2)
	{
		if (equals(d1, d2) == true)
			return true;
		if (d1 > d2)
			return true;
		return false;
	}

	inline bool	less(const double& d1, const double& d2)
	{
		if (equals(d1, d2) == true)
			return false;
		if (d1 < d2)
			return true;
		return false;
	}

	inline bool	lessOrEqual(const double& d1, const double& d2)
	{
		if (equals(d1, d2) == true)
			return true;
		if (d1 < d2)
			return true;
		return false;
	}

	inline bool isInvalidValue(const double& d)
	{
		return equals(fabs(d), 1.7976931348623158e+308);
	}
	inline bool isInvalidValue(const float& d)
	{
		return equals(fabs(d), numeric_limits<float>::quiet_NaN());
	}
	inline bool isInvalidValue(const int& d)
	{
		return d==0xfeeefeee;
	}
	inline double GetDoubleInvalidValue()
	{
        return 1.7976931348623158e+308;
	}
	inline float GetFloatInvalidValue()
	{
        return numeric_limits<float>::quiet_NaN();
	}
	inline int GetIntInvalidValue()
	{
        return 0xfeeefeee;
	}
    inline bool IsHaveDot(string& str)
    {
		return (string::npos == str.find('.'))?false:true;
    }
}


#define CommonBtnHeight 20



#endif
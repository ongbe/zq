/*
 * digittools.h
 *
 *  tools for digit
 *
 *  Created on: Dec 1, 2012
 *      Author: root
 */

#ifndef DIGITTOOLS_H_
#define DIGITTOOLS_H_

#ifdef WIN32
#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#endif

#include <limits>

#ifdef WIN32
#pragma managed(push,off)
#endif 


namespace DigitTools
{
	inline double GetInvalidValue_CTPDouble()
	{
        return 1.7976931348623158e+308;
	}
	inline float GetInvalidValue_Float()
	{
        return std::numeric_limits<float>::quiet_NaN();
	}
	inline int GetInvalidValue_CTPInt()
	{
        return 0xfeeefeee;
	}

	inline int getExpoBase2_internal(const double& d)
	{
		int i = 0;
		((short *)(&i))[0] = (((short *)(&d))[3] & (short)0x7ff0);
		return (i >> 4) - 1023;
	}
	inline bool	equals_double(const double& d1, const double& d2)
	{
		if (d1 == d2)
			return true;
		int e1 = getExpoBase2_internal(d1);
		int e2 = getExpoBase2_internal(d2);
		int e3 = getExpoBase2_internal(d1 - d2);
		if ((e3 - e2 < -48) && (e3 - e1 < -48))
			return true;
		return false;
	}

	inline bool isInvalidValue_CTPDouble(const double& d)
	{
		return equals_double(fabs(d), GetInvalidValue_CTPDouble());
	}
	inline bool isInvalidValue_Float(const float& d)
	{
        return equals_double(fabs(d), GetInvalidValue_Float());
	}
	inline bool isInvalidValue_CTPInt(const int& d)
	{
		return d==GetInvalidValue_CTPInt();
	}

	inline int compare_double(const double& d1, const double& d2)
	{
		if (equals_double(d1, d2) == true)
			return 0;
		if (d1 > d2)
			return 1;
		return -1;
	}

	inline bool	greater_double(const double& d1, const double& d2)
	{
		if (equals_double(d1, d2) == true)
			return false;
		if (d1 > d2)
			return true;
		return false;
	}

	inline bool	greater_equal_double(const double& d1, const double& d2)
	{
		if (equals_double(d1, d2) == true)
			return true;
		if (d1 > d2)
			return true;
		return false;
	}

	inline bool	less_double(const double& d1, const double& d2)
	{
		if (equals_double(d1, d2) == true)
			return false;
		if (d1 < d2)
			return true;
		return false;
	}

	inline bool	less_equal_double(const double& d1, const double& d2)
	{
		if (equals_double(d1, d2) == true)
			return true;
		if (d1 < d2)
			return true;
		return false;
	}

}


#ifdef WIN32
#pragma managed(pop)
#endif 


#undef int64_EasyTime
#undef snprintf_EasyTime


#endif /* EASYTIME_H_ */

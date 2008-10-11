#ifndef _UTIL_H_
#define _UTIL_H_

#define Debugf printf
#include "templdefs.h"
#include <algorithm>

inline float clamp(float x, float minx, float maxx)
{
	return std::min(std::max(x, minx), maxx);
}

#endif //_UTIL_H_

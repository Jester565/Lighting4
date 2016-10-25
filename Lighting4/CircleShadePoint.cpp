#include "CircleShadePoint.h"
#include "CircleLightSource.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace lighting
{
	CircleShadePoint::CircleShadePoint(float x, float y)
		:ShadePoint(x, y)
	{
		rads = atan2(y, x);
		if (rads < 0)
		{
			rads += M_PI * 2;
		}
		radixVal = (unsigned int)(rads * ((float)CircleLightSource::RADIX_MAX_NUM / (2 * M_PI)));
		rads = (radixVal * (2 * M_PI)) / (float)CircleLightSource::RADIX_MAX_NUM;
	}

	CircleShadePoint::~CircleShadePoint()
	{
		
	}
}

#include "AboveShadePoint.h"
#include "AboveLightSource.h"

namespace lighting
{
	AboveShadePoint::AboveShadePoint(float x, float y, int minX, int maxX)
		:ShadePoint(x, y)
	{
		float range = maxX - minX;
		float normalizedX = x - minX;
		radixVal = (normalizedX / range) * AboveLightSource::RADIX_MAX_NUM;
	}

	AboveShadePoint::~AboveShadePoint()
	{
	}
}
#include "ShadePoint.h"
#include "LightBlocker.h"
#define _USE_MATH_DEFINES
#include <math.h>

bool GetIntersectPoint(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y, float * i_x, float * i_y)
{
	float s1_x, s1_y, s2_x, s2_y;
	s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

	float s, t;
	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = (s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		if (i_x != NULL)
			*i_x = p0_x + (t * s1_x);
		if (i_y != NULL)
			*i_y = p0_y + (t * s1_y);
		return true;
	}

	return false;
}

ShadePoint::ShadePoint(float x, float y)
	:x(x), y(y)
{
	rads = atan2(y, x);
	if (rads < 0)
	{
		rads += M_PI * 2;
	}
}

bool ShadePoint::checkIntersect(float x1, float y1, float x2, float y2, float & cX, float & cY)
{
	return GetIntersectPoint(x1, y1, x2, y2, this->x, this->y, this->connectedPoint->x, this->connectedPoint->y, &cX, &cY);
}

ShadePoint::~ShadePoint()
{
}

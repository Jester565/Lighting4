#include "LightBlocker.h"
#include <math.h>

namespace lighting
{

	LightBlocker::LightBlocker(float x, float y, float epX1, float epY1, float epX2, float epY2)
		:epX1(epX1), epY1(epY1), epX2(epX2), epY2(epY2), rotateXOff1(0), rotateYOff1(0), rotateXOff2(0), rotateYOff2(0)
	{
		setGlobalXY(x, y);
	}

	void LightBlocker::setGlobalXY(float x, float y)
	{
		x1 = x + epX1 + rotateXOff1;
		y1 = y + epY1 + rotateYOff1;
		x2 = x + epX2 + rotateXOff2;
		y2 = y + epY2 + rotateYOff2;
	}

	void LightBlocker::setRads(float cX, float cY, float rads)
	{
		float x = x1 - epX1;
		float y = y1 - epY1;
		float angle1 = atan2(epY1 - cY, epX1 - cX);
		float dis1 = sqrt((epY1 - cY)*(epY1 - cY) + (epX1 - cX)*(epX1 - cX));
		rotateXOff1 = (cX + cos(angle1 + rads) * dis1) - epX1;
		rotateYOff1 = (cY + sin(angle1 + rads) * dis1) - epY1;
		float angle2 = atan2(epY2 - cY, epX2 - cX);
		float dis2 = sqrt((epY2 - cY)*(epY2 - cY) + (epX2 - cX)*(epX2 - cX));
		rotateXOff2 = cX + cos(angle2 + rads) * dis2 - epX2;
		rotateYOff2 = cY + sin(angle2 + rads) * dis2 - epY2;
		setGlobalXY(x, y);
	}

	LightBlocker::~LightBlocker()
	{
	}

}

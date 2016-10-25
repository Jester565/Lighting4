#include "LightBlockerContainer.h"
#include "LightLayer.h"

namespace lighting
{

	LightBlockerContainer::LightBlockerContainer(LightLayer * lightLayer)
		:owner(lightLayer), cX(0), cY(0), rads(0), x(0), y(0)
	{

	}

	void LightBlockerContainer::setXY(float x, float y)
	{
		for (auto it = lightBlockers.begin(); it != lightBlockers.end(); it++)
		{
			(*it)->setGlobalXY(x, y);
		}
	}

	void LightBlockerContainer::addLine(float x1, float y1, float x2, float y2)
	{
		LightBlocker* lightBlocker = new LightBlocker(x, y, x1, y1, x2, y2);
		lightBlockers.push_back(lightBlocker);
		owner->addLightBlocker(lightBlocker);
	}

	void LightBlockerContainer::initSquare(float w, float h)
	{
		addLine(w, 0, 0, 0);
		addLine(0, 0, 0, h);
		addLine(0, h, w, h);
		addLine(w, 0, w, h);
	}

	void LightBlockerContainer::setCXYToCenter()
	{
		float avgX = 0;
		float avgY = 0;
		for (auto it = lightBlockers.begin(); it != lightBlockers.end(); it++)
		{
			avgX += (*it)->epX1 + (*it)->epX2;
			avgY += (*it)->epY1 + (*it)->epY2;
		}
		avgX /= (lightBlockers.size() * 2);
		avgY /= (lightBlockers.size() * 2);
		cX = avgX;
		cY = avgY;
	}

	LightBlockerContainer::~LightBlockerContainer()
	{
		for (auto it = lightBlockers.begin(); it != lightBlockers.end(); it++)
		{
			owner->removeLightBlocker(*it);
		}
		lightBlockers.clear();
	}
}

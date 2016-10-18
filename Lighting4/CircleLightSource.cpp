#include "CircleLightSource.h"

namespace lighting
{
	CircleLightSource::CircleLightSource(LightLayer * ownerLightLayer, float radius, uint8_t r, uint8_t g, uint8_t b)
	{
	}

	void lighting::CircleLightSource::setLightColor(uint8_t r, uint8_t b, uint8_t g)
	{
	}

	CircleLightSource::~CircleLightSource()
	{
	}

	bool CircleLightSource::CrossesZero(ShadePoint * testPoint)
	{
		return false;
	}
	float CircleLightSource::GetRadDif(ShadePoint * ep1, ShadePoint * ep2, float oriRads)
	{
		return 0.0f;
	}
	void CircleLightSource::transferHeldVars()
	{
	}
	void CircleLightSource::drawLocal()
	{
	}

	void CircleLightSource::createShadePoints(std::list<LightBlocker*>& lightBlockers)
	{
	}
	void CircleLightSource::mapShadePoints()
	{
	}
	void CircleLightSource::resetPoints()
	{
	}
	void CircleLightSource::handleBoundCollisions(float & x1, float & y1, float & x2, float & y2)
	{
	}
	void CircleLightSource::handleFirstShadePoint(ShadePoint *& alphaPoint, float & firstX, float & firstY, int & i, bool & radAtZero)
	{
	}
	void CircleLightSource::handleLastShadePoint(ShadePoint * alphaPoint, bool radAtZero, float prevX, float prevY, float firstX, float firstY)
	{
	}
	void CircleLightSource::updateCastPoints(ShadePoint * updatePoint)
	{
	}
	ShadePoint * CircleLightSource::shadowCast(float rads, float & cX, float & cY, ShadePoint * exceptionPoint)
	{
		return nullptr;
	}
	bool CircleLightSource::getAlphaLineAtRad(ShadePoint *& alphaPoint, int & i, float & lastX, float & lastY)
	{
		return false;
	}
	bool CircleLightSource::CheckPointFront(ShadePoint * alphaPoint, ShadePoint * checkPoint)
	{
		return false;
	}
}

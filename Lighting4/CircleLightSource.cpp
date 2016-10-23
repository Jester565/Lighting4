#include "CircleLightSource.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "LightLayer.h"

namespace lighting
{
	uint64_t CircleLightSource::ShadePointsProcessed = 0;
	uint64_t CircleLightSource::ShadowCalled = 0;
	uint64_t CircleLightSource::CastPointsProcessed = 0;
	uint64_t CircleLightSource::TotalCycles = 0;

	/// <summary>
	/// The radix maximum number{CC2D43FA-BBC4-448A-9D0B-7B57ADF2655C}
	/// </summary>
	unsigned int CircleLightSource::RADIX_MAX_NUM = pow(2, 32) - 1;

	const float CircleLightSource::MAX_NEG_FLOAT = -std::numeric_limits<float>::max();

	CircleLightSource::CircleLightSource(LightLayer * ownerLightLayer, float radius, uint8_t r, uint8_t g, uint8_t b)
		:LightSource(ownerLightLayer), radius(radius)
	{
		setLightColor(r, g, b);
		al_set_new_bitmap_flags(SHADE_MAP_FLAGS);
		shadeMap = al_create_bitmap((radius * 2) * owner->getLightBmpScale(), (radius * 2) * owner->getLightBmpScale());
	}

	void lighting::CircleLightSource::setLightColor(uint8_t r, uint8_t g, uint8_t b)
	{
		lightColor = al_map_rgba(r, g, b, 0);
	}
	
	CircleLightSource::~CircleLightSource()
	{
		delete shadeMap;
		shadeMap = nullptr;
	}

	void CircleLightSource::radixSortShadePoints()
	{
		for (int bI = 0; bI < RADIX_MAX_BITS; bI += RADIX_BASE_BITS)
		{
			countingSortShadePoints(bI);
		}
	}

	void CircleLightSource::countingSortShadePoints(int bI)
	{
		std::vector <unsigned int> counts(RADIX_BASE_NUM);
		for (int i = 0; i < shadePoints.size(); i++)
		{
			int countIdx = (shadePoints[i]->radixVal >> bI) & 0xF;
			counts.at(countIdx)++;
		}

		for (int i = 1; i < RADIX_BASE_NUM; i++)
		{
			counts.at(i) += counts.at(i - 1);
		}
		
		std::vector<CircleShadePoint*> output(shadePoints.size());
		for (int i = shadePoints.size() - 1; i >= 0; i--)
		{
			int countIdx = (shadePoints[i]->radixVal >> bI) & 0xF;
			output[counts[countIdx] - 1] = shadePoints[i];
			counts[countIdx]--;
		}
		for (int i = 0; i < shadePoints.size(); i++)
		{
			shadePoints[i] = output[i];
		}
	}

	bool CircleLightSource::CrossesZero(CircleShadePoint * testPoint)
	{
		float ep1Rad = testPoint->rads;
		float ep2Rad = testPoint->getConnectPoint()->rads;
		if (ep1Rad < ep2Rad)
		{
			return (ep2Rad - M_PI > ep1Rad);
		}
		return (ep1Rad - M_PI > ep2Rad);
	}

	float CircleLightSource::GetRadDif(CircleShadePoint * ep1, CircleShadePoint * ep2, float oriRads)
	{
		float lineRad = atan2(ep2->y - ep1->y, ep2->x - ep1->x);	//angle between first and second endpoint
		if (lineRad < 0)
		{
			lineRad += 2 * M_PI;	//normalize the radians
		}
		if (oriRads > M_PI)
		{
			if (lineRad < M_PI)
			{
				lineRad += 2 * M_PI;
			}
		}
		return lineRad - oriRads;
	}

	bool CircleLightSource::CheckPointFront(CircleShadePoint * alphaPoint, CircleShadePoint * checkPoint)
	{
		float cX;	//Useless variables
		float cY;
		return !alphaPoint->checkIntersect(0, 0, checkPoint->x, checkPoint->y, cX, cY);
	}

	void CircleLightSource::transferHeldVars()
	{
		this->x = heldX;
		this->y = heldY;
	}

	void CircleLightSource::createShadePoints()
	{
		std::list <LightBlocker*> lightBlockers = owner->lightBlockers;
		resetPoints(lightBlockers.size());
		for (auto it = lightBlockers.begin(); it != lightBlockers.end(); it++)
		{
			float x1 = (*it)->x1 - x;
			float y1 = (*it)->y1 - y;
			float x2 = (*it)->x2 - x;
			float y2 = (*it)->y2 - y;
			if (handleBoundCollisions(x1, y1, x2, y2))
			{
				CircleShadePoint* updatePoint1 = new CircleShadePoint(x1, y1);
				CircleShadePoint* updatePoint2 = new CircleShadePoint(x2, y2);
				updatePoint1->setConnectPoint(updatePoint2);
				updatePoint2->setConnectPoint(updatePoint1);
				if (CrossesZero(updatePoint1))
				{
					if (updatePoint1->rads != 0 && updatePoint2->rads != 0)
					{
						if (updatePoint1->rads > M_PI)
						{
							castPoints.insert(updatePoint1);
						}
						else
						{
							castPoints.insert(updatePoint2);
						}
					}
				}
				shadePoints.push_back(updatePoint1);
				shadePoints.push_back(updatePoint2);
			}
		}
		radixSortShadePoints();
		//std::sort(shadePoints.begin(), shadePoints.end(), RadialShadePointCompare);
	}

	void CircleLightSource::mapShadePoints()
	{
		TotalCycles++;  //TEMP
		ShadePointsProcessed += shadePoints.size();
		float firstAlphaContactX = 0;	//The position of the contact of the first line at angle=0
		float firstAlphaContactY = 0;
		CircleShadePoint* alphaPoint = nullptr;	//Represents an endpoint from the "alpha line".  This is the line closest to the origin of the light as you rotate.  This allows us to skip over ray casting in many cases.
		int i = 0;
		bool radAtZero = false;
		handleFirstShadePoint(alphaPoint, firstAlphaContactX, firstAlphaContactY, i, radAtZero);
		float alphaContactX = firstAlphaContactX;	//This was the last position a new alphapoint was assigned, used for keeping track of drawing
		float alphaContactY = firstAlphaContactY;
		for (int i = 0; i < shadePoints.size(); i++)
		{
			//The current point is at the end of the alphaLine (now you have to decide who is the successor to the alphaPoint)
			if (shadePoints.at(i)->rads == alphaPoint->getConnectPoint()->rads)
			{
				addDrawPoints(alphaContactX, alphaContactY, alphaPoint->getConnectPoint()->x, alphaPoint->getConnectPoint()->y);
				//save the distance of the endpoint of the current alphaLine so we can check if the new alphapoint returned is past that distance
				CircleShadePoint* disPoint = alphaPoint->getConnectPoint();
				float alphaDis = sqrt(pow(disPoint->x, 2) + pow(disPoint->y, 2));
				CircleShadePoint* radAlphaPoint = nullptr;
				bool found = getAlphaLineAtRad(radAlphaPoint, i, alphaContactX, alphaContactY);
				if (!found)
				{
					alphaPoint = shadowCast(shadePoints.at(i)->rads, alphaContactX, alphaContactY, alphaPoint);
				}
				else
				{
					//distance to the newly found alphaPoint
					float pointDis = sqrt(pow(alphaContactX, 2) + pow(alphaContactY, 2));
					//If the new found point is further from the origin, we need to check that there were no lines between those points
					if (pointDis > alphaDis)
					{
						float cX;
						float cY;
						CircleShadePoint* contactPoint = shadowCast(shadePoints.at(i)->rads, cX, cY, alphaPoint);
						float contactDis = sqrt(pow(cX, 2) + pow(cY, 2));
						//Is the line closer or the point found earlier?
						if (contactDis < pointDis)
						{
							alphaPoint = contactPoint;
							alphaContactX = cX;
							alphaContactY = cY;
						}
						else
						{
							alphaPoint = radAlphaPoint;
						}
					}
					else
					{
						alphaPoint = radAlphaPoint;
					}
				}
			}
			else if (CheckPointFront(alphaPoint, shadePoints.at(i)))	//Is the point in front of the alpha line?
			{
				float alphaCX;
				float alphaCY;
				alphaPoint->checkIntersect(0, 0, cos(shadePoints.at(i)->rads) * radius * 2, sin(shadePoints.at(i)->rads) * radius * 2, alphaCX, alphaCY);
				addDrawPoints(alphaContactX, alphaContactY, alphaCX, alphaCY);
				//We know this will return a valid alphaPoint because, the only way this would be in front of the alphaLine is if it wasn't already in front
				//so it must be going up in radians (the direction we want)
				getAlphaLineAtRad(alphaPoint, i, alphaContactX, alphaContactY);
			}
			else
			{
				//Even if this point isn't significant right now, it can still be used in shadowCast.  Maintaining this map will shorten the time it takes to shadowCast.
				updateCastPoints(shadePoints.at(i));
			}
		}
		handleLastShadePoint(alphaPoint, radAtZero, alphaContactX, alphaContactY, firstAlphaContactX, firstAlphaContactY);
	}

	void CircleLightSource::drawLocal()
	{
		al_set_target_bitmap(shadeMap);
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
		al_clear_to_color(al_map_rgba(0, 0, 0, 255));	//clear the bitmap to all black (0, 0, 0, 255)
		//Draw triangles where the light is, so the black shadow now becomes transparent because we are drawing color code (r, g, b, 0) direclty to the map
		for (int i = 0; i < drawPoints.size(); i += 4)
		{
			al_draw_filled_triangle(drawPoints.at(i), drawPoints.at(i + 1), drawPoints.at(i + 2), drawPoints.at(i + 3), radius * owner->getLightBmpScale(), radius * owner->getLightBmpScale(), lightColor);	
		}
		al_draw_filled_triangle(drawPoints.at(0), drawPoints.at(1), drawPoints.at(drawPoints.size() - 2), drawPoints.at(drawPoints.size() - 1), radius * owner->getLightBmpScale(), radius * owner->getLightBmpScale(), lightColor);
		//Complicated, just leave as is
		al_set_separate_blender(ALLEGRO_DEST_MINUS_SRC, ALLEGRO_ALPHA, ALLEGRO_ALPHA, ALLEGRO_SRC_MINUS_DEST, ALLEGRO_ONE, ALLEGRO_ONE);
		al_draw_scaled_bitmap(LSource_Map, 0, 0, LSource_Map_W, LSource_Map_H, 0, 0, (radius * 2) * owner->getLightBmpScale(), (radius * 2) * owner->getLightBmpScale(), NULL);
	}

	void CircleLightSource::drawToLightMap()
	{
		al_draw_scaled_bitmap(shadeMap, 0, 0, (radius * 2) * owner->getLightBmpScale(), (radius * 2) * owner->getLightBmpScale(), (x - radius) * owner->getLightBmpScale(), (y - radius) * owner->getLightBmpScale(), (radius * 2) * owner->getLightBmpScale(), (radius * 2) * owner->getLightBmpScale(), NULL);
	}

	void CircleLightSource::resetPoints(size_t lightBlockersSize)
	{
		for (int i = BOUND_POINTS_SIZE; i < shadePoints.size(); i++)
		{
			delete shadePoints.at(i);
		}
		drawPoints.clear();
		castPoints.clear();
		shadePoints.clear();
		shadePoints.reserve(lightBlockersSize * 2 + BOUND_POINTS_SIZE);
		createBoundShadePoints();
	}

	void CircleLightSource::createBoundShadePoints()
	{
		CircleShadePoint* nRnR1 = new CircleShadePoint(-radius, -radius);
		CircleShadePoint* nRpR1 = new CircleShadePoint(-radius, radius);
		nRnR1->setConnectPoint(nRpR1);
		nRpR1->setConnectPoint(nRnR1);
		shadePoints.push_back(nRpR1);
		shadePoints.push_back(nRnR1);
		CircleShadePoint* nRnR2 = new CircleShadePoint(-radius, -radius);
		CircleShadePoint* pRnR1 = new CircleShadePoint(radius, -radius);
		nRnR2->setConnectPoint(pRnR1);
		pRnR1->setConnectPoint(nRnR2);
		shadePoints.push_back(nRnR2);
		shadePoints.push_back(pRnR1);
		CircleShadePoint* pRnR2 = new CircleShadePoint(radius, -radius);
		CircleShadePoint* pRpR1 = new CircleShadePoint(radius, radius);
		pRnR2->setConnectPoint(pRpR1);
		pRpR1->setConnectPoint(pRnR2);
		castPoints.emplace(pRnR2);	//Because this line passes angle=0, add it to cast points
		shadePoints.push_back(pRnR2);
		shadePoints.push_back(pRpR1);
		CircleShadePoint* pRpR2 = new CircleShadePoint(radius, radius);
		CircleShadePoint* nRpR2 = new CircleShadePoint(-radius, radius);
		pRpR2->setConnectPoint(nRpR2);
		nRpR2->setConnectPoint(pRpR2);
		shadePoints.push_back(pRpR2);
		shadePoints.push_back(nRpR2);
	}


	bool CircleLightSource::handleBoundCollisions(float & x1, float & y1, float & x2, float & y2)
	{
		bool p1In = x1 > -radius && x1 < radius && y1 > -radius && y1 < radius;  //if x1 and y1 are in the bounds
		bool p2In = x2 > -radius && x2 < radius && y2 > -radius && y2 < radius;  //if x2 and y2 are in the bounds
		if (!p1In && !p2In)
		{
			bool p1Set = false;
			for (int i = 0; i < BOUND_POINTS_SIZE; i += 2)
			{
				float cX;
				float cY;
				//checks if line made by (x1, y1) and (x2, y2) intersects with bounds
				if (shadePoints.at(i)->checkIntersect(x1, y1, x2, y2, cX, cY))
				{
					if (!p1Set)
					{
						x1 = cX;
						y1 = cY;
						p1Set = true;
					}
					else
					{
						x2 = cX;
						y2 = cY;
					}
				}
			}
			//Means that the line is not anywhere near the light and does not need to be considered
			if (!p1Set)
			{
				return false;
			}
		}
		else if (!p1In)
		{
			for (int i = 0; i < BOUND_POINTS_SIZE; i += 2)
			{
				float cX;
				float cY;
				if (shadePoints.at(i)->checkIntersect(x1, y1, x2, y2, cX, cY))
				{
					x1 = cX;
					y1 = cY;
					break;
				}
			}
		}
		else if (!p2In)
		{
			for (int i = 0; i < BOUND_POINTS_SIZE; i += 2)
			{
				float cX;
				float cY;
				if (shadePoints.at(i)->checkIntersect(x1, y1, x2, y2, cX, cY))
				{
					x2 = cX;
					y2 = cY;
					break;
				}
			}
		}
		bringEqualBoundToInBound(x1);
		bringEqualBoundToInBound(y1);
		bringEqualBoundToInBound(x2);
		bringEqualBoundToInBound(y2);
		return true;
	}

	void CircleLightSource::addDrawPoints(float x1, float y1, float x2, float y2)
	{
		drawPoints.push_back((x1 + radius) * owner->getLightBmpScale());
		drawPoints.push_back((y1 + radius) * owner->getLightBmpScale());
		drawPoints.push_back((x2 + radius) * owner->getLightBmpScale());
		drawPoints.push_back((y2 + radius) * owner->getLightBmpScale());
	}

	void CircleLightSource::handleFirstShadePoint(CircleShadePoint *& alphaPoint, float & firstX, float & firstY, int & i, bool & radAtZero)
	{
		//Since points are in order, shadePoints.at(0) would have a radian of 0 if it existed
		if (shadePoints.at(0)->rads == 0)
		{
			if (getAlphaLineAtRad(alphaPoint, i, firstX, firstY))
			{
				//Checks if shadowCast point is in front of the point at angle = 0
				float pointDis = sqrt(pow(firstX, 2) + pow(firstY, 2));
				float cX;
				float cY;
				CircleShadePoint* contactPoint = shadowCast(shadePoints.at(i)->rads, cX, cY, alphaPoint);
				float contactDis = sqrt(pow(cX, 2) + pow(cY, 2));
				if (contactDis < pointDis)
				{
					alphaPoint = contactPoint;
					firstX = cX;
					firstY = cY;
				}
				else
				{
					radAtZero = true;
				}
			}
		}
		if (alphaPoint == nullptr)
		{
			alphaPoint = shadowCast(0, firstX, firstY);
		}
	}

	void CircleLightSource::handleLastShadePoint(CircleShadePoint * alphaPoint, bool radAtZero, float prevX, float prevY, float firstX, float firstY)
	{
		if (alphaPoint->getConnectPoint()->rads != 0 && radAtZero)
		{
			float cX = 0;
			float cY = 0;
			alphaPoint->checkIntersect(0, 0, radius * 2, 0, cX, cY);
			addDrawPoints(prevX, prevY, cX, cY);
		}
		else if (alphaPoint->getConnectPoint()->rads != 0)
		{
			addDrawPoints(prevX, prevY, firstX, firstY);
		}
		else
		{
			addDrawPoints(prevX, prevY, alphaPoint->getConnectPoint()->x, alphaPoint->getConnectPoint()->y);
		}
	}

	void CircleLightSource::updateCastPoints(CircleShadePoint * updatePoint)
	{
		if (CrossesZero(updatePoint))
		{
			if (updatePoint->rads <= updatePoint->getConnectPoint()->rads)
			{
				castPoints.erase(updatePoint->getConnectPoint());
			}
			else
			{
				castPoints.insert(updatePoint);
			}
		}
		else
		{
			if (updatePoint->rads >= updatePoint->getConnectPoint()->rads)
			{
				castPoints.erase(updatePoint->getConnectPoint());
			}
			else
			{
				castPoints.insert(updatePoint);
			}
		}
	}

	CircleShadePoint * CircleLightSource::shadowCast(float rads, float & cX, float & cY, CircleShadePoint * exceptionPoint)
	{
		ShadowCalled++;
		CastPointsProcessed += castPoints.size();
		cX = cos(rads) * radius * 2;	//coordinates for endpoint to check for intersects on.  Other endpoint is at orign.
		cY = sin(rads) * radius * 2;
		ShadePoint* shadowPoint = nullptr;
		for (auto it = castPoints.begin(); it != castPoints.end(); it++)
		{
			CircleShadePoint* updatePoint = *it;
			//check if either of the lines of the endpoint have the same coordinates as the exceptionPoint
			if (exceptionPoint == nullptr || !(updatePoint->x == exceptionPoint->x && updatePoint->y == exceptionPoint->y || 
				updatePoint->getConnectPoint()->x == exceptionPoint->x && updatePoint->getConnectPoint()->y == exceptionPoint->y || 
				updatePoint->x == exceptionPoint->getConnectPoint()->x && updatePoint->y == exceptionPoint->getConnectPoint()->y || 
				updatePoint->getConnectPoint()->x == exceptionPoint->getConnectPoint()->x && updatePoint->getConnectPoint()->y == exceptionPoint->getConnectPoint()->y))
			{
				//if an intersect occured cX and cY would be set to the intersect position, meaning that other points will now have be closer to the origin than cX and cY because the length of the line has decreased
				if (updatePoint->checkIntersect(0, 0, cX, cY, cX, cY))
				{
					shadowPoint = updatePoint;
				}
			}
		}
		return (CircleShadePoint*)shadowPoint;	//SHOULD NEVER BE NULLPTR
	}

	bool CircleLightSource::getAlphaLineAtRad(CircleShadePoint *& alphaPoint, int & i, float & alphaContactX, float & alphaContactY)
	{
		float maxRads = -FLT_MAX;	//Keeps track of the highest radian, the higher the radian the further in front the line is.  
		int maxI = -1;	//Set to the index of the closest and highly angled point
		float minDis = FLT_MAX;	//Keeps track of the lowest distance, the lower the distance the further in front the line is.
		float rads = shadePoints.at(i)->rads;	//The radian value the point from the origin
		while (i < shadePoints.size() && shadePoints.at(i)->rads == rads)
		{
			updateCastPoints(shadePoints.at(i));
			float radDif = GetRadDif(shadePoints.at(i), shadePoints.at(i)->getConnectPoint(), rads);
			bool zeroRadIntersected = CrossesZero(shadePoints.at(i));
			if (rads == 0 && zeroRadIntersected)
			{
				zeroRadIntersected = false;
			}
			if (radDif > 0 && radDif < M_PI)
			{
				if (!zeroRadIntersected && shadePoints.at(i)->getConnectPoint()->rads > rads || zeroRadIntersected && zeroRadIntersected && shadePoints.at(i)->getConnectPoint()->rads < rads)
				{
					float dis = sqrt(pow(shadePoints.at(i)->x, 2) + pow(shadePoints.at(i)->y, 2));
					if (dis < minDis)
					{
						minDis = dis;
						maxRads = FLT_MIN;
					}
					if (dis == minDis && radDif > maxRads)
					{
						maxI = i;
						maxRads = radDif;
					}
				}
			}
			i++;
		}
		if (maxI != -1)
		{
			alphaPoint = shadePoints.at(maxI);
			alphaContactX = shadePoints.at(maxI)->x;
			alphaContactY = shadePoints.at(maxI)->y;
		}
		i--;
		return (maxI != -1);
	}
}

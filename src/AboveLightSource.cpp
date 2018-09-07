#include "AboveLightSource.h"
#include "LightLayer.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

namespace lighting
{
	AboveLightSource::AboveLightSource(LightLayer * ownerLightLayer, int yOff, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		:LightSource(ownerLightLayer), yOff(yOff)
	{
		setLightColor(r, g, b, a);
	}

	void AboveLightSource::setLightColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		lightColor = al_map_rgba(r, g, b, a);
	}

	AboveLightSource::~AboveLightSource()
	{
	}

	void AboveLightSource::countingSortShadePoints(int bI)
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

		std::vector<AboveShadePoint*> output(shadePoints.size());
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

	void AboveLightSource::radixSortShadePoints()
	{
		for (int bI = 0; bI < RADIX_MAX_BITS; bI += RADIX_BASE_BITS)
		{
			countingSortShadePoints(bI);
		}
	}

	bool AboveLightSource::CheckPointFront(AboveShadePoint * alphaPoint, AboveShadePoint * checkPoint)
	{
		float cX;
		float cY;
		return !alphaPoint->checkIntersect(checkPoint->x, -(BOUND_OFF + LINE_CHECK_OFF), checkPoint->x, checkPoint->y, cX, cY);
	}

	void AboveLightSource::createShadePoints()
	{
		resetPoints();
		int minX = getMinX();
		int maxX = getMaxX();
		for (auto it = owner->lightBlockers.begin(); it != owner->lightBlockers.end(); it++)
		{
			float x1 = (*it)->x1;
			float y1 = (*it)->y1;
			float x2 = (*it)->x2;
			float y2 = (*it)->y2;
			if (x1 < minX && x2 < minX)
			{
				continue;
			}
			if (x1 > maxX && x2 > maxX)
			{
				continue;
			}
			AboveShadePoint* sp1 = new AboveShadePoint(x1, y1, minX, maxX);
			AboveShadePoint* sp2 = new AboveShadePoint(x2, y2, minX, maxX);
			sp1->setConnectPoint(sp2);
			sp2->setConnectPoint(sp1);
			if (x1 < minX)
			{
				sp1->x = minX;
				castPoints.emplace(sp1);
			}
			else if (x2 < minX)
			{
				sp2->x = minX;
				castPoints.emplace(sp2);
			}
			if (x1 > maxX)
			{
				sp1->x = maxX;
			}
			else if (x2 > maxX)
			{
				sp2->x = maxX;
			}
			shadePoints.push_back(sp1);
			shadePoints.push_back(sp2);
		}
		for (auto it = owner->aboveLightBlockers.begin(); it != owner->aboveLightBlockers.end(); it++)
		{
			float x1 = (*it)->x1;
			float x2 = (*it)->x2;
			if (x1 < minX && x2 < minX)
			{
				continue;
			}
			if (x1 > maxX && x2 > maxX)
			{
				continue;
			}
			AboveShadePoint* sp1 = new AboveShadePoint(x1, ABOVE_LIGHT_BLOCKER_Y, minX, maxX);
			AboveShadePoint* sp2 = new AboveShadePoint(x2, ABOVE_LIGHT_BLOCKER_Y, minX, maxX);	
			sp1->setConnectPoint(sp2);
			sp2->setConnectPoint(sp1);
			if (x1 < minX)
			{
				sp1->x = minX;
				castPoints.emplace(sp1);
			}
			else if (x2 < minX)
			{
				sp2->x = minX;
				castPoints.emplace(sp2);
			}
			if (x1 > maxX)
			{
				sp1->x = maxX;
			}
			else if (x2 > maxX)
			{
				sp2->x = maxX;
			}
			shadePoints.push_back(sp1);
			shadePoints.push_back(sp2);
		}
		radixSortShadePoints();
	}

	void AboveLightSource::mapShadePoints()
	{
		AboveShadePoint* alphaPoint = nullptr;
		int i = 0;
		float alphaContactX = 0;
		float alphaContactY = 0;
		handleFirstShadePoints(alphaPoint, alphaContactX, alphaContactY, i);
		for (; i < shadePoints.size(); i++)
		{
			if (shadePoints.at(i)->x == alphaPoint->getConnectPoint()->x)
			{
				addDrawPoints(alphaContactX, alphaContactY, alphaPoint->getConnectPoint()->x, alphaPoint->getConnectPoint()->y);
				float alphaConnectY = alphaPoint->getConnectPoint()->y;
				bool found = getAlphaLineAtX(alphaPoint, i, alphaContactX, alphaContactY);
				if (!found)
				{
					alphaContactX = alphaPoint->getConnectPoint()->x;
					alphaPoint = shadowCast(alphaContactX, alphaContactY, alphaPoint);
				}
				else
				{
					if (alphaContactY > alphaConnectY)
					{
						float cY;
						AboveShadePoint* contactPoint = shadowCast(alphaPoint->x, cY, alphaPoint);
						if (cY < alphaPoint->y)
						{
							alphaPoint = contactPoint;
							alphaContactY = cY;
						}
						
					}
				}
			}
			else if (CheckPointFront(alphaPoint, shadePoints.at(i)))
			{
				float alphaCX;
				float alphaCY;
				alphaPoint->checkIntersect(shadePoints.at(i)->x, -(LINE_CHECK_OFF + BOUND_OFF), shadePoints.at(i)->x, owner->drawToHeight + LINE_CHECK_OFF + BOUND_OFF, alphaCX, alphaCY);
				addDrawPoints(alphaContactX, alphaContactY, alphaCX, alphaCY);
				getAlphaLineAtX(alphaPoint, i, alphaContactX, alphaContactY);
			}
			else
			{
				updateCastPoints(shadePoints.at(i));
			}
		}
		handleLastShadePoints(alphaPoint, alphaContactX, alphaContactY);
	}

	void AboveLightSource::drawToLightMap()
	{
		al_set_separate_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE, ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
		for (int i = 0; i < drawPoints.size(); i += 4)
		{
			if (drawPoints.at(i + 1) == drawPoints.at(i + 3))
			{
				al_draw_filled_rectangle(drawPoints.at(i), 0, drawPoints.at(i + 2), drawPoints.at(i + 3), lightColor);
			}
			else
			{
				std::vector<float> verts = { drawPoints.at(i), 0, drawPoints.at(i), drawPoints.at(i + 1), drawPoints.at(i + 2), drawPoints.at(i + 3), drawPoints.at(i + 2), 0 };
				al_draw_filled_polygon(verts.data(), 4, lightColor);
			}
		}
	}

	void AboveLightSource::createBoundShadePoints()
	{
		AboveShadePoint* p1 = new AboveShadePoint(getMinX(), owner->drawToHeight + BOUND_OFF, getMinX(), getMaxX());
		AboveShadePoint* p2 = new AboveShadePoint(getMaxX(), owner->drawToHeight + BOUND_OFF, getMinX(), getMaxX());
		p1->setConnectPoint(p2);
		p2->setConnectPoint(p1);
		shadePoints.push_back(p1);
		shadePoints.push_back(p2);
		castPoints.emplace(p1);
	}

	void AboveLightSource::resetPoints()
	{
		drawPoints.clear();
		for (int i = 0; i < shadePoints.size(); i++)
		{
			delete shadePoints.at(i);
		}
		shadePoints.clear();
		shadePoints.reserve((owner->lightBlockers.size() + owner->aboveLightBlockers.size()) * 2 + BOUND_POINTS_SIZE);
		createBoundShadePoints();
	}

	void AboveLightSource::updateCastPoints(AboveShadePoint * updatePoint)
	{
		if (updatePoint->x >= updatePoint->getConnectPoint()->x)
		{
			castPoints.erase(updatePoint->getConnectPoint());
		}
		else
		{
			castPoints.emplace(updatePoint);
		}
	}

	bool AboveLightSource::getAlphaLineAtX(AboveShadePoint *& alphaPoint, int & i, float & alphaContactX, float & alphaContactY)
	{
		int maxI = -1;
		float minY = FLT_MAX;
		float minYFar = FLT_MAX;
		float x = shadePoints.at(i)->x;
		while (i < shadePoints.size() && shadePoints.at(i)->x == x)
		{
			updateCastPoints(shadePoints.at(i));
			if (shadePoints.at(i)->getConnectPoint()->x > shadePoints.at(i)->x)
			{
				float pointY = shadePoints.at(i)->y;
				if (pointY < minY)
				{
					minY = pointY;
					minYFar = FLT_MAX;
				}
				if (pointY == minY)
				{
					if (shadePoints.at(i)->getConnectPoint()->y < minYFar)
					{
						minYFar = shadePoints.at(i)->getConnectPoint()->y;
						maxI = i;
					}
				}
			}
			i++;
		}
		if (maxI != -1)
		{
			alphaPoint = shadePoints.at(maxI);
			alphaContactX = alphaPoint->x;
			alphaContactY = alphaPoint->y;
		}
		i--;
		return (maxI != -1);
	}
	
	void AboveLightSource::handleFirstShadePoints(AboveShadePoint *& alphaPoint, float & firstX, float & firstY, int & i)
	{
		//The first point has to be minX()
		if (getAlphaLineAtX(alphaPoint, i, firstX, firstY))
		{
			float pointDis = sqrt(pow(firstX, 2) + pow(firstY, 2));
			float cX = getMinX();
			float cY;
			AboveShadePoint* contactPoint = shadowCast(cX, cY, alphaPoint);
			float contactDis = sqrt(pow(cX, 2) + pow(cY, 2));
			if (contactDis < pointDis)
			{
				alphaPoint = contactPoint;
				firstX = cX;
				firstY = cY;
			}
		}
	}

	void AboveLightSource::handleLastShadePoints(AboveShadePoint * alphaPoint, float alphaContactX, float alphaContactY)
	{

	}
	
	int AboveLightSource::getMaxX()
	{
		return BOUND_OFF + owner->drawToWidth;
	}

	void AboveLightSource::addDrawPoints(float x1, float y1, float x2, float y2)
	{
		drawPoints.push_back(x1 * owner->getLightBmpScale());
		drawPoints.push_back((y1 + yOff) * owner->getLightBmpScale());
		drawPoints.push_back(x2 * owner->getLightBmpScale());
		drawPoints.push_back((y2 + yOff) * owner->getLightBmpScale());
	}

	AboveShadePoint * AboveLightSource::shadowCast(float x, float & cY, AboveShadePoint * exceptionPoint)
	{
		cY = owner->drawToHeight + BOUND_OFF + LINE_CHECK_OFF;
		AboveShadePoint* shadowPoint = nullptr;
		for (auto it = castPoints.begin(); it != castPoints.end(); it++)
		{
			AboveShadePoint* shadePoint = (*it);
			if (exceptionPoint == nullptr || shadePoint->x != exceptionPoint->x && shadePoint->getConnectPoint()->x != exceptionPoint->x)
			{
				if (shadePoint->checkIntersect(x, -(BOUND_OFF + LINE_CHECK_OFF), x, cY, x, cY))
				{
					shadowPoint = shadePoint;
				}
			}
		}
		return shadowPoint;
	}
}

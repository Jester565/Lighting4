#pragma once
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>
#include "LightBlocker.h"

namespace lighting
{
	class LightLayer;
	
	/// <summary>
	/// Contains related <see cref="LightBlocker"/>s, such as the lines that make up a shape.  Handles adding and removing from <see cref="LightMap"/>.
	/// </summary>
	class LightBlockerContainer
	{
	public:		
		/// <summary>
		/// Initializes a new instance of the <see cref="LightBlockerContainer"/> class.
		/// </summary>
		/// <param name="ownerLightLayer">Value to set <see cref="owner"/> to.  Represents the <see cref="LightLayer"/> the <see cref="LightBlocker"/>s will belong to.</param>
		LightBlockerContainer(LightLayer* ownerLightLayer);
				
		/// <summary>
		/// Sets the xy of all elements of <see cref="lightBlockers"/>.
		/// </summary>
		/// <param name="x">The x.</param>
		/// <param name="y">The y.</param>
		void setXY(float x, float y);

		/// <summary>
		/// Adds a <see cref="LightBlocker"/> with the specified endpoints.
		/// </summary>
		/// <param name="x1">The x1.</param>
		/// <param name="y1">The y1.</param>
		/// <param name="x2">The x2.</param>
		/// <param name="y2">The y2.</param>
		void addLine(float x1, float y1, float x2, float y2);

		/// <summary>
		/// Creates four <see cref="LightBlocker"/>s in the shape of a rectangle, the top left corner of the rectangle is at <see cref="x"/> and <see cref="y"/>.
		/// </summary>
		/// <param name="w">The w.</param>
		/// <param name="h">The h.</param>
		void initSquare(float w, float h);
		
		/// <summary>
		/// Increments <see cref="rads"/> by <paramref name="deltaDegs"/> and rotates all elements in <see cref="lightBlockers"/>.
		/// </summary>
		/// <param name="deltaDegs">The amount of degrees to rotate by.</param>
		void changeDegs(float deltaDegs)
		{
			rads += deltaDegs * (M_PI / 180);
			setLightBlockerRads();
		}

		/// <summary>
		/// Set <see cref="rads"/> to the converted valie of <paramref name="degs"/> and rotates all elements in <see cref="lightBlockers"/>.
		/// </summary>
		/// <param name="degs">The angle to set <see cref="rads"/> to.</param>
		void setDegs(float degs)
		{
			rads = degs * (M_PI / 180);
			setLightBlockerRads();
		}

		/// <summary>
		/// Increments <see cref="rads"/> by the value of the parameter <paramref name="rads"/> and rotates all elements in <see cref="lightBlockers"/>.
		/// </summary>
		/// <param name="deltaRads">The amount of radians to rotate by.</param>
		void changeRads(float deltaRads)
		{
			rads += deltaRads;
			setLightBlockerRads();
		}

		/// <summary>
		/// Set <see cref="rads"/> data member to the converted value of <paramref name="rads"/> and rotates all elements in <see cref="lightBlockers"/>.
		/// </summary>
		/// <param name="rads">The angle to set <see cref="rads"/> to.</param>
		void setRads(float rads)
		{
			this->rads = rads;
			setLightBlockerRads();
		}
		
		/// <summary>
		/// Setter for attributes <see cref="cX"/> and <see cref="cY"/>.  Which represent the coordinates to rotate around.
		/// </summary>
		/// <param name="cX">The c x.</param>
		/// <param name="cY">The c y.</param>
		void setCXY(float cX, float cY)
		{
			this->cX;
			this->cY;
		}
		
		/// <summary>
		/// Sets attributes <see cref="cX"/> and <see cref="cY"/> to the center of the <see cref="lightBlockers"/>.
		/// </summary>
		void setCXYToCenter();

		/// <summary>
		/// Finalizes an instance of the <see cref="LightBlockerContainer"/> class.  Removes all elements of <see cref="lightBlockers"/> from the <see cref="owner"/>.
		/// </summary>
		~LightBlockerContainer();

	private:
				
		/// <summary>
		/// Sets elements of <see cref="lightBlockers"/> to the angle <see cref="rads"/>.
		/// </summary>
		void setLightBlockerRads()
		{
			for (auto it = lightBlockers.begin(); it != lightBlockers.end(); it++)
			{
				(*it)->setRads(rads, cX, cY);
			}
		}
		/// <summary>
		/// The <see cref="LightBlocker"/>s created by <c>this</c>.  Should form a shape.
		/// </summary>
		std::list <LightBlocker*> lightBlockers;
				
		/// <summary>
		/// The <see cref="LightLayer"/> that the <see cref="lightBlockers"/> are added and removed from.
		/// </summary>
		LightLayer* owner;

		/// <summary>
		/// Coordinates of the <see cref="lightBlockers"/> on the screen.
		/// </summary>
		float x, y;
		
		/// <summary>
		/// Coordinates to rotate <see cref="lightBlockers"/> around.
		/// </summary>
		float cX, cY;
		
		/// <summary>
		/// The angle of the <see cref="lightBlockers"/>.
		/// </summary>
		float rads;
	};
}
#pragma once
#include <list>
#include "LightBlocker.h"

namespace lighting
{
	class LightLayer;
	
	/// <summary>
	/// Represents a set of <see cref="LightBlocker">s that form a shape.
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

		}

		/// <summary>
		/// Set <see cref="rads"/> to the converted valie of <paramref name="degs"/> and rotates all elements in <see cref="lightBlockers"/>.
		/// </summary>
		/// <param name="degs">The angle to set <see cref="rads"/> to.</param>
		void setDegs(float degs)
		{

		}

		/// <summary>
		/// Increments <see cref="rads"/> by the value of the parameter <paramref name="rads"/> and rotates all elements in <see cref="lightBlockers"/>.
		/// </summary>
		/// <param name="deltaRads">The amount of radians to rotate by.</param>
		void changeRads(float deltaRads)
		{

		}

		/// <summary>
		/// Set <see cref="rads"/> data member to the converted value of <paramref name="rads"/> and rotates all elements in <see cref="lightBlockers"/>.
		/// </summary>
		/// <param name="rads">The angle to set <see cref="rads"/> to.</param>
		void setRads(float rads)
		{

		}
		
		/// <summary>
		/// Setter for attributes <see cref="cX"/> and <see cref="cY"/>.  Which represent the coordinates to rotate around.
		/// </summary>
		/// <param name="cX">The c x.</param>
		/// <param name="cY">The c y.</param>
		void setCXY(float cX, float cY)
		{

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
		/// The <see cref="LightBlocker"/>s created by <code>this</code>.  Should form a shape.
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
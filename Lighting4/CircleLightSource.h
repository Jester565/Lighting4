#pragma once
#include <unordered_set>
#include <set>
#include <vector>
#include "LightSource.h"
#include "CircleShadePoint.h"

namespace lighting
{	
	/// <summary>
	/// A child of <see cref="LightSource" /> represents a full circle of light that would be produced by an oil lamp or a light bulb.
	/// </summary>
	/// <seealso cref="LightSource" />

	class CircleLightSource : public LightSource
	{
	public:	

		static uint64_t ShadePointsProcessed;
		static uint64_t ShadowCalled;
		static uint64_t CastPointsProcessed;
		static uint64_t TotalCycles;

		/// <summary>
		/// Number of bits for the radix base
		/// </summary>
		static const uint8_t RADIX_BASE_BITS = 8;
		
		/// <summary>
		/// The maximum decimal value for the radix base.
		/// </summary>
		static const unsigned int RADIX_BASE_NUM = 256;
		
		/// <summary>
		/// Number of bits for the maximum value of elements sorted by radix sort
		/// </summary>
		static const uint8_t RADIX_MAX_BITS = 24;
		
		/// <summary>
		/// The maximum decimal value of the elements sorted by <see cref="::radixSortShadePoints"/>.
		/// </summary>
		static unsigned int RADIX_MAX_NUM;

		/// <summary>
		/// Set to the most negative possible value of float.
		/// </summary>
		static const float MAX_NEG_FLOAT;
				
		/// <summary>
		/// Initializes a new instance of the <see cref="CircleLightSource"/> class.  Automatically adds itself to the <paramref name="ownerLightLayer"/>.
		/// </summary>
		/// <param name="ownerLightLayer">The <see cref="LightLayer"/> <c>this</c> should be added to.</param>
		/// <param name="radius">The radius of the light circle.  Determines the width of <see cref="shadeMap"/>.</param>
		/// <param name="r">The red value of <see cref="lightColor"/>(0 to 255).</param>
		/// <param name="g">The green value of <see cref="lightColor"/> (0 to 255).</param>
		/// <param name="b">The blue value of <see cref="lightColor"/> (0 to 255).</param>
		CircleLightSource(LightLayer* ownerLightLayer, float radius, uint8_t r = UINT8_MAX, uint8_t g = UINT8_MAX, uint8_t b = UINT8_MAX);
				
		/// <summary>
		/// Sets the horizontal and vertical position of the center of the <see cref="CircleLightSource"/> on the screen.  Values are stored in <see cref="heldX"/> and <see cref="heldY"/> until <see cref="transferHeldVars()"/> is called.
		/// </summary>
		/// <param name="x">The horizontal position.</param>
		/// <param name="y">The vertical position.</param>
		void setXY(float x, float y)
		{
			heldX = x;
			heldY = y;
		}

		/// <summary>
		/// Sets the color of the light.  Modifier for <see cref="lightColor"/>.
		/// </summary>
		/// <param name="r">The red value of the color (0 to 255).</param>
		/// <param name="g">The green value of the color (0 to 255).</param>
		/// <param name="b">The blue value of the color (0 to 255).</param>
		virtual void setLightColor(uint8_t r, uint8_t g, uint8_t b);
		
		virtual ~CircleLightSource();

	protected:
		/// <summary>
		/// Uses radix sort to sort the elements of <see cref="shadePoints"/>.
		/// </summary>
		void radixSortShadePoints();
		
		/// <summary>
		/// Called by <see cref="radixSortShadePoints"/> as a helper method for the sorting process
		/// </summary>
		/// <param name="bI">The amount of bits to shift by.</param>
		void countingSortShadePoints(int bI);

		/// <summary>
		/// The allegro bitmap flags for creating the <see cref="shadeMap"/>.
		/// </summary>
		static const int SHADE_MAP_FLAGS = ALLEGRO_NO_PRESERVE_TEXTURE | ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR;;
				
		/// <summary>
		/// The amount of preset <see cref="CircleShadePoint"/>s that will be added to <see cref="shadePoints"/>.
		/// </summary>
		static const int BOUND_POINTS_SIZE = 8;

		/// <summary>
		/// If the line created by <paramref name="testPoint"/> and its connected point cross angle = 0, return <c>true</c>, <c>false</c> otherwise.
		/// </summary>
		/// <param name="testPoint"> The <see cref="CircleShadePoint"/> which represents one of the endpoints of the line to test.</param>
		/// <returns>If the line passes through angle = 0.</returns>
		static bool CrossesZero(CircleShadePoint* testPoint);
		
		/// <summary>
		/// Finds the angle of <paramref name="ep2"/> from origin <paramref name="ep1"/> relative to <paramref name="oriRads"/>.
		/// </summary>
		/// <par>
		/// If <c>0</c>was returned, that means the angle between <paramref name="ep2"/> and <paramref name="ep1"/> is in line with the <paramref name="oriRads"/>.
		/// </par>
		/// <param name="ep1">The <see cref="CircleShadePoint"/> representing the first endpoint.</param>
		/// <param name="ep2">The <see cref="CircleShadePoint"/> representing the second endpoint (this is usually just the <see cref="ShadePoint::connectedPoint"/> of <paramref name="ep1"/>.</param>
		/// <param name="oriRads">The radian the line relative to the <see cref="CircleLightSource"/> center (this is usualy just the <see cref="ShadePoint::rads"/> of <paramref name="ep1"/></param>
		/// <returns>The angle of <paramref name="ep2"/> from origin <paramref name="ep1"/> relative to <paramref name="oriRads"/>.</returns>
		static float GetRadDif(CircleShadePoint* ep1, CircleShadePoint* ep2, float oriRads);
		
		/// <summary>
		/// Calculates if the <paramref name="checkPoint"/> is in front of <paramref name="alphaPoint"/>.
		/// </summary>
		/// <param name="alphaPoint">The <see cref="CircleShadePoint"/> representing one of the endpoints of the line that will be used to check <paramref name="checkPoint"/>.</param>
		/// <param name="checkPoint">The <see cref="CircleShadePoint"/> to check if in front of line made by <paramref name="alphaPoint"/>.</param>
		/// <returns><c>true</c> if  <paramref name="checkPoint"/> is closer to the center of the <see cref="LightSource"/> than the line made by <paramref name="alphaPoint"/></returns>
		static bool CheckPointFront(CircleShadePoint* alphaPoint, CircleShadePoint* checkPoint);

		/// <summary>
		/// Sets the <see paramref="x"/>, <see paramref="y"/> attributes to the corresponding "held" attributes.  Called from <see cref="LightLayer::dispatch()"/>.
		/// </summary>
		virtual void transferHeldVars() override;

		/// <summary>
		/// Converts all elements of <paramref name="lightBlockers" /> into two <see cref="CircleShadePoint"/>s which are stored in the <see cref="shadePoints" /> vector and then sorted using <see cref="::radixSortShadePoints"/>.  Function also handles edge cases.
		/// </summary>
		virtual void createShadePoints() override;
		
		/// <summary>
		/// Processes all of the elements in <see cref="shadePoints"/> and converts them to elements of <see cref="LightSource::drawPoints"/>.  The <see cref="LightSource::drawPoints"/> are placed at the end of shadows.
		/// </summary>
		virtual void mapShadePoints();

		/// <summary>
		/// Uses the <see cref="LightSource::drawPoint"/> vector, which was set by <see cref="mapShadePoints"/>, to draw onto the <see cref="shadeMap"/>.
		/// </summary>
		virtual void drawLocal() override;
		
		/// <summary>
		/// Draws <see cref="shadeMap"/> to the <see cref="LightLayer::lightMap">.  Assumes lightMap is already set as the target bitmap.
		/// </summary>
		virtual void drawToLightMap() override;

		/// <summary>
		/// Restores <see cref="shadePoints" /> and
		/// </summary>
		/// <param name="lightBlockersSize">Size of the list of <see cref="lightBlocker"/>s passed to be converted to <see cref="CircleShadePoint"/>s.</param>
		virtual void resetPoints(size_t lightBlockersSize);
		
		/// <summary>
		/// Creates and pushes <see cref="CircleShadePoint"/>s representing the edges of the light to <see cref="shadePoints"/>.
		/// </summary>
		virtual void createBoundShadePoints();
		
		/// <summary>
		/// Checks if the lines created by these coordinates intersect with the boundaries.  If they do, find the collision point and set the coordinates to that.  Calls <see cref="::moveIntoRadius" /> on all coordinates.
		/// </summary>
		/// <param name="x1">The horizontal position of the first endpoint.  Used as out parameter.</param>
		/// <param name="y1">The vertical position of the first endpoint.  Used as out parameter.</param>
		/// <param name="x2">The horizontal position of the second endpoint.  Used as out parameter.</param>
		/// <param name="y2">The vertical position of the second endpoint.  Used as out parameter</param>
		/// <returns>Whether the line is within the bounds of the light.</returns>
		virtual bool handleBoundCollisions(float& x1, float& y1, float& x2, float& y2);
				
		/// <summary>
		/// If num is on the radius, it is brought into the radius to avoid collisions with <see cref="ShadePoint"/>s.
		/// </summary>
		/// <param name="num">The value to check if on bound.  Output parameter</param>
		virtual void bringEqualBoundToInBound(float& num)
		{
			if ((int)num == -radius)
			{
				num = -radius + 1;
			}
			else if ((int)num == radius)
			{
				num = radius - 1;
			}
		}
		
		/// <summary>
		/// Converts parameters into screen coordinates and pushes them to <see cref="drawPoints"/>.
		/// </summary>
		/// <param name="x1">The horizontal position of the first endpoint.</param>
		/// <param name="y1">The vertical position of the first endpoint.</param>
		/// <param name="x2">The horizontal position of the second endpoint.</param>
		/// <param name="y2">The vertical position of the second endpoint.</param>
		virtual void addDrawPoints(float x1, float y1, float x2, float y2);

		/// <summary>
		/// Handles the first <see cref="CircleShadePoint"/> from <see cref="shadePoints"/> appropiatly.  Called at beginning of <see cref="::mapShadePoints"/> function.
		/// </summary>
		/// <param name="alphaPoint">One of the endpoints of the line closest to origin.  Completely an output parameter.</param>
		/// <param name="firstX">The first x of the collision with the line created by <paramref name="alphaPoint"/> at angle=<c>0</c></param>
		/// <param name="firstY">The first y of the collision with the line created by <paramref name="alphaPoint"/> at angle=<c>0</c>.</param>
		/// <param name="i">Used as an output parameter for the number of elements of <see cref="shadePoints"/> that were processed finding the first valid line.</param>
		/// <param name="radAtZero"><c>true</c> when a <see cref="CircleShadePoint"/> was found exactly at angle=<c>0</c>.</param>
		virtual void handleFirstShadePoint(CircleShadePoint*& alphaPoint, float& firstX, float& firstY, int& i, bool& radAtZero);
				
		/// <summary>
		/// Called after the <see cref="shadePoints"/> have been entirely iterated over by <see cref="::mapShadePoints"/>.
		/// </summary>
		/// <param name="alphaPoint">The last <see cref="ShadePoint"/> that had a line closest to the origin.</param>
		/// <param name="radAtZero">Whether the first <see cref="ShadePoint"/> was a point directly at angle=<c>0</c>.</param>
		/// <param name="prevX">The previousX value when a collision with <see cref="alphaPoint"/> occured.</param>
		/// <param name="prevY">The previousY value when a collision with <see cref="alphaPoint"/> occured.</param>
		/// <param name="firstX">The x value of the first <see cref="CirleShadePoint"/>.</param>
		/// <param name="firstY">The y value of the first <see cref="CircleShadePoint"/>.</param>
		virtual void handleLastShadePoint(CircleShadePoint* alphaPoint, bool radAtZero, float prevX, float prevY, float firstX, float firstY);
		
		/// <summary>
		/// When the end of the line of an alphaPoint is reached,  this function will check if any <see cref="ShadePoint"/>s at the same radian
		/// as the endpoint are valid.  If any are valid, it will return the line that will be closest to the origin using the output parameters.
		/// </summary>
		/// <par>
		/// Will add the <paramref name="updatePoint"/> to <see cref="castPoints"/> if the <see cref="ShadePoint::connectedPoint"/> has a greater angle.  Will remove <paramref name="updatePoint"/>'s 
		/// <see cref="ShadePoint::connectedPoint"/> from <see cref="castPOints"/> if <paramref name="updatePoint"/> has a radian greater than the <see cref="ShadePoint::connectedPoint"/>.  Will account for lines crossing radian zero.
		/// </par>
		/// <param name="updatePoint">The <see cref="CircleShadePoint"/> to check if adding or removing is needed.</param>
		virtual void updateCastPoints(CircleShadePoint* updatePoint);
		
		/// <summary>
		/// When a line ends and there is no clear point to go to, this method is called to iterate through all of the lines below the radian
		/// and find which one is closest to the origin and return it.
		/// </summary>
		/// <param name="rads">The radian to check.</param>
		/// <param name="cX">Output parameter of the horizontal position of a collision with the <see cref="ShadePoint"/> being returned.</param>
		/// <param name="cY">>Output parameter of the vertical position of a collision with the <see cref="ShadePoint"/> being returned.</param>
		/// <param name="exceptionPoint">Set a point that cannot be a valid return, will be ignored.</param>
		/// <returns>A <see cref="CircleShadePoint"/> representing an endpoint for a line closest to the origin at angle <paramref name="rads"/></returns>
		virtual CircleShadePoint* shadowCast(float rads, float& cX, float& cY, CircleShadePoint* exceptionPoint = nullptr);
				
		/// <summary>
		/// Will iterate through all of the <see cref="ShadePoint"/>s with the same radian as the element in <see cref="shadePoints"/> at index <paramref name="i"/> and set the <paramref name="alphaPoint"/>, <paramref name="prevX"/>, and lastY to the point that has the minimum distance 
		/// from the origin, and the highest angle between it and its connecting point.  If a valid point is not found, return false and leave alphaPoint, prevX, prevY as they were.
		/// </summary>
		/// <param name="alphaPoint">A <see cref="ShadePoint"/> representing an endpoint of the line that was last closest to the origin.  Output parameter.</param>
		/// <param name="i">The index of the <see cref="ShadePoint"/> we want to check the radians of in the <see cref="shadePoints"> attribute.  Output parameter.</param>
		/// <param name="prevX">The previous collision position with the line created by <paramref name="alphaPoint"/>. Output parameter.</param>
		/// <param name="prevY">The previous collision position with the line created by <paramref name="alphaPoint"/>. Output parameter.</param>
		/// <returns><c>true</c> if a valid point was found, <c>false</c> otherwise.</returns>
		bool getAlphaLineAtRad(CircleShadePoint*& alphaPoint, int& i, float& prevX, float& prevY);

		/// <summary>
		/// Keeps track of all of the <see cref="CircleShadePoint"/>s that have a line that could be shadow casted to.
		/// </summary>		
		/// <par>
		/// Constantly added and removed from by <see cref="::updateCastPoints"/> as <see cref="shadePoints"/> are processed by <see cref="::mapShadePoints"/>.
		/// Elements in this list should only include lines that can possibly be shadow casted onto by <see cref="::shadowCast">.
		/// </par>
		std::unordered_set <CircleShadePoint*> castPoints;
		
		/// <summary>
		/// Endpoints of <see cref="LightBlocker"/>s.  Populated by <see cref="::createShadePoints"/>.
		/// </summary>
		std::vector <CircleShadePoint*> shadePoints;
		
		/// <summary>
		/// Stores the x and y of the edges of shadows in the local bitmap.  Populated by <see cref="::mapShadePoints"/>.
		/// </summary>
		std::vector <float> drawPoints;

		/// <summary>
		/// The color of the light.
		/// </summary>
		ALLEGRO_COLOR lightColor;

		/// <summary>
		/// Bitmap where shadows are drawn to.
		/// </summary>
		ALLEGRO_BITMAP* shadeMap;
		
		/// <summary>
		/// Horizontal position of the center of the light on the screen.
		/// </summary>
		float x;
		
		/// <summary>
		/// Vertical position of the center of the light on the screen.
		/// </summary>
		float y;
		
		/// <summary>
		/// The radius of the <c>this</c> <see cref="LightBlocker"/>.
		/// </summary>
		float radius;

		/// <summary>
		/// Temporarily stores position of the <see cref="CircleLightSource"/> until <see cref="transferHeldVars()"/> is called, where the values are assigned to <see cref="x"/> and <see cref="y"/>.
		/// </summary>
		float heldX;
		
		/// <summary>
		/// Temporarily stores position of the <see cref="CircleLightSource"/> until <see cref="transferHeldVars()"/> is called, where the values are assigned to <see cref="x"/> and <see cref="y"/>.
		/// </summary>
		float heldY;
	};
}

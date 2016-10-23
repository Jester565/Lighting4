#pragma once
#include "LightSource.h"
#include "AboveShadePoint.h"
#include <unordered_set>

namespace lighting
{	
	/// <summary>
	/// Represents the sun or any planetary source of light.  Can be blocked by <see cref="AboveLightBlocker">s or <see cref="LightBlocker"/>s.
	/// </summary>
	/// <seealso cref="LightSource" />
	class AboveLightSource : public LightSource
	{
		friend class LightLayer;
	public:		
		/// <summary>
		/// The default color code value for r, g, b, a
		/// </summary>
		static const uint8_t DEFAULT_COLOR_VAL = 180;
		
		/// <summary>
		/// The y value <see cref="AboveLightBlocker"/> will be added at.
		/// </summary>
		static const int ABOVE_LIGHT_BLOCKER_Y = -30;
		
		/// <summary>
		/// How far the bound points are from the screen.
		/// </summary>
		static const int BOUND_OFF = 400;
		
		/// <summary>
		/// The amount of bits in the radix base
		/// </summary>
		static const uint8_t RADIX_BASE_BITS = 4;
		
		/// <summary>
		/// The max value of the radix base
		/// </summary>
		static const unsigned int RADIX_BASE_NUM = 16;
		
		/// <summary>
		/// The maximum radix value in bits.
		/// </summary>
		static const uint8_t RADIX_MAX_BITS = 16;
		
		/// <summary>
		/// The maximum value of the radix.
		/// </summary>
		static const unsigned int RADIX_MAX_NUM = 65535;
		
		/// <summary>
		/// Initializes a new instance of the <see cref="AboveLightSource"/> class.  Automatically adds itelf to <paramref name="ownerLightLayer"/>.
		/// </summary>
		/// <param name="ownerLightLayer">The <see cref="LightLayer"/> <code>this</code> will be added to.</param>
		/// <param name="yOff">This value is added to the light y so the light can go further than the object it hit.</param>
		/// <param name="r">The red color value.</param>
		/// <param name="g">The green color value.</param>
		/// <param name="b">The blue color value.</param>
		/// <param name="a">a.</param>
		AboveLightSource(LightLayer* ownerLightLayer, int yOff = 0, uint8_t r = DEFAULT_COLOR_VAL, uint8_t g = DEFAULT_COLOR_VAL, uint8_t b = DEFAULT_COLOR_VAL, uint8_t a = DEFAULT_COLOR_VAL);
		
		/// <summary>
		/// Sets <see cref="lightColor"/>.
		/// </summary>
		/// <param name="r">The red color value.</param>
		/// <param name="g">The green color value.</param>
		/// <param name="b">The blue color value.</param>
		/// <param name="a">The alpha color value.</param>
		void setLightColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		
		/// <summary>
		/// Finalizes an instance of the <see cref="AboveLightSource"/> class.  Automatically removes itself from the <see cref="owner"/>.
		/// </summary>
		virtual ~AboveLightSource();

	protected:		
		/// <summary>
		/// Amount of bound <see cref="AboveShadePoint"/>s.
		/// </summary>
		static const int BOUND_POINTS_SIZE = 2;
		
		/// <summary>
		/// When shadowCasting or checking points, this is how far off the y value is from the sceen to check for collisoins.
		/// </summary>
		static const int LINE_CHECK_OFF = 2000;
				
		/// <summary>
		/// Countings the sort shade points.
		/// </summary>
		/// <param name="bI">The bits to shift by.</param>
		void countingSortShadePoints(int bI);
		
		/// <summary>
		/// Radixes the sort shade points based on their x-values.
		/// </summary>
		void radixSortShadePoints();

		/// <summary>
		/// Checks if <paramref name="checkPoint"/> is in front of the line created by <see cref="alphaPoint"/>.
		/// </summary>
		/// <param name="alphaPoint">One of the endpoints of the alpha line.</param>
		/// <param name="checkPoint">The point to check if in front of alpha line.</param>
		/// <returns>If <paramref name="checkPoint"/> is in front of the line made by <see cref="alphaPoint"/>.</returns>
		static bool CheckPointFront(AboveShadePoint* alphaPoint, AboveShadePoint* checkPoint);
		
		/// <summary>
		/// Transfers the held vars.  No implementation.
		/// </summary>
		virtual void transferHeldVars() override
		{

		}
		
		/// <summary>
		/// Populates <see cref="shadePoints"/> by using <see cref="owner"/>'s aboveLightBlockers and lightBlockers.
		/// </summary>
		virtual void createShadePoints() override;
		
		/// <summary>
		/// Processes the <see cref="shadePoints"/> and converts them to <see cref="drawPoints"/>.
		/// </summary>
		virtual void mapShadePoints() override;
		
		/// <summary>
		/// No body, there is no local drawing needed.
		/// </summary>
		virtual void drawLocal() override
		{

		}
		
		/// <summary>
		/// Iterates through <see cref="drawPoints"/> to draw to lightMap.
		/// </summary>
		virtual void drawToLightMap() override;
		
		/// <summary>
		/// Populates <see cref="shadePoints"/> with the boundary <see cref="AboveShadePoint"/>s.
		/// </summary>
		virtual void createBoundShadePoints();
		
		/// <summary>
		/// Clears and deletes elements in all point containers.
		/// </summary>
		virtual void resetPoints();
		
		/// <summary>
		/// Updates the cast points.
		/// </summary>
		/// <param name="updatePoint">The <see cref="AboveShadePoint"/> to check if needing adding or removing from <see cref="castPoints"/>.</param>
		virtual void updateCastPoints(AboveShadePoint* updatePoint);
		
		/// <summary>
		/// Will iterate through all of the <see cref="ShadePoint"/>s with the same x as the element in <see cref="shadePoints"/> at index <paramref name="i"/> and set the <paramref name="alphaPoint"/>, <paramref name="alphaContactX"/>, and <paramref name="alphaContactY"/> to the point that has the minimum distance 
		/// from the origin, and the highest angle between it and its connecting point.  If a valid point is not found, return false and leave alphaPoint, prevX, prevY as they were.
		/// </summary>
		/// <param name="alphaPoint"><see cref="ShadePoint"/> represneting of the end points of the alphaLine.</param>
		/// <param name="i">The index of <see cref="shadePoints"/> with the x-value to check.</param>
		/// <param name="alphaContactX">The last place an alphaContact occured.</param>
		/// <param name="alphaContactY">The last place an alphaContact occureed.</param>
		/// <returns></returns>
		virtual bool getAlphaLineAtX(AboveShadePoint*& alphaPoint, int& i, float& alphaContactX, float& alphaContactY);
				
		/// <summary>
		/// Handles the first shade points.
		/// </summary>
		/// <param name="alphaPoint">The alpha point.</param>
		/// <param name="firstX">The first x.</param>
		/// <param name="firstY">The first y.</param>
		/// <param name="i">The i.</param>
		virtual void handleFirstShadePoints(AboveShadePoint*& alphaPoint, float& firstX, float& firstY, int& i);
		
		/// <summary>
		/// Handles the last shade points. Currently no body.
		/// </summary>
		/// <param name="alphaPoint">The alpha point.</param>
		/// <param name="alphaContactX">The alpha contact x.</param>
		/// <param name="alphaContactY">The alpha contact y.</param>
		virtual void handleLastShadePoints(AboveShadePoint* alphaPoint, float alphaContactX, float alphaContactY);
		
		/// <summary>
		/// Value added to the y-component of <see cref="drawPoints"/>.
		/// </summary>
		int yOff;
				
		/// <summary>
		/// The minimum value of x for elements of <see cref="shadePoints"/>.
		/// </summary>
		/// <returns></returns>
		int getMinX()
		{
			return -BOUND_OFF;
		}
		
		/// <summary>
		/// The maximum value of x for elements of <see cref="shadePoints"/>.
		/// </summary>
		/// <returns></returns>
		int getMaxX();

		void addDrawPoints(float x1, float y1, float x2, float y2);

		/// <summary>
		/// Returns the line closest to the top of the screen at the given <paramref name="x"/>
		/// </summary>
		/// <param name="x">The x.</param>
		/// <param name="cY">The contactY, output parameter.</param>
		/// <param name="exceptionPoint">The exception point, will not be returned.</param>
		/// <returns></returns>
		AboveShadePoint* shadowCast(float x, float& cY, AboveShadePoint* exceptionPoint = nullptr);

		/// <summary>
		/// The coordinates of the points to draw.  Even is x, odd is y.
		/// </summary>
		std::vector <float> drawPoints;
		
		/// <summary>
		/// Keeps track of the <see cref="AboveShadePoint"/>s that can be shadow casted to.
		/// </summary>
		std::unordered_set <AboveShadePoint*> castPoints;
		
		/// <summary>
		/// The <see cref="ShadePoint"/>s created by <see cref="::createShadePoints"/>.
		/// </summary>
		std::vector <AboveShadePoint*> shadePoints;
		
		/// <summary>
		/// The color of the light.
		/// </summary>
		ALLEGRO_COLOR lightColor;
	};
}

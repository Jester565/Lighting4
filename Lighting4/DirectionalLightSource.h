#pragma once
#include "CircleLightSource.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace lighting
{	

	class LightLayer;

	/// <summary>
	/// Represents a light in the form of a flash light or any light beam limited to an angle
	/// </summary>
	/// <seealso cref="CircleLightSource" />
	class DirectionalLightSource : public CircleLightSource
	{
	public:
		
		/// <summary>
		/// Initializes a new instance of the <see cref="DirectionalLightSource"/> class.  Initializes all of the bitmaps and sets their sizes.  Adds itself to <paramref name="ownerLightLayer"/>.
		/// </summary>
		/// <param name="ownerLightLayer">The lightLayer that owns it.</param>
		/// <param name="radius">The radius of the light.</param>
		/// <param name="degs">The spread of the light beam.</param>
		/// <param name="r">The red color value for the light.</param>
		/// <param name="g">The green color value for the light.</param>
		/// <param name="b">The blue color value for the light.</param>
		DirectionalLightSource(LightLayer* ownerLightLayer, int radius, float degs, uint8_t r = UINT8_MAX, uint8_t g = UINT8_MAX, uint8_t b = UINT8_MAX);
				
		/// <summary>
		/// Draws the beam and shadows to the bitmap <see cref="directionalLSourceMap"/>.
		/// </summary>
		virtual void drawLocal() override;
		
		/// <summary>
		/// Draws light beam to the lightMap.  Assumes lightMap is already set to target bitmap.
		/// </summary>
		virtual void drawToLightMap() override;

		/// <summary>
		/// Sets the angle of the beam in degrees.
		/// </summary>
		/// <param name="degs">The angle of the beam.</param>
		void setDegs(float degs)
		{
			rads = (degs * (M_PI / 180));
		}
		
		/// <summary>
		/// Changes the angle of the beam.
		/// </summary>
		/// <param name="deltaDegs">The change in degrees.</param>
		void changeDegs(float deltaDegs)
		{
			rads += deltaDegs * (M_PI / 180);
		}
		
		/// <summary>
		/// Sets the angle of the beam.
		/// </summary>
		/// <param name="rads">The angle to set the beam to.</param>
		void setRads(float rads)
		{
			this->rads = rads;
		}
		
		/// <summary>
		/// Changes the angle of the beam.
		/// </summary>
		/// <param name="deltaRads">The change in rads.</param>
		void changeRads(float deltaRads)
		{
			this->rads += deltaRads;
		}

		virtual ~DirectionalLightSource();

	protected:		
		/// <summary>
		/// Bitmap of the directional light, created in constructor and is not changed
		/// </summary>
		ALLEGRO_BITMAP* directionalLSourceMapSave;
				
		/// <summary>
		/// Bitmap where <see cref="CircleLightSource::shadeMap"/> and <see cref="directionalLSourceMapSave"/> are combined.
		/// </summary>
		ALLEGRO_BITMAP* directionalLSourceMap;
		
		/// <summary>
		/// The width of the directionalLSource bitmaps.
		/// </summary>
		int directionalLSourceW;
		
		/// <summary>
		/// The height of the directionalLSource bitmaps.
		/// </summary>
		int directionalLSourceH;
				
		/// <summary>
		/// The angle of the beam (not the spread of it, in radians)
		/// </summary>
		float rads;
	};
}

#pragma once
#include <unordered_map>
#include <list>
#include <string>
#include <allegro5/bitmap.h>
#include "ShadePoint.h"
#include "LightBlocker.h"

namespace lighting
{
	class LightLayer;
	
	/// <summary>
	/// Abstract class represnting an light that can be blocked
	/// </summary>
	class LightSource
	{
	public:		
		/// <summary>
		/// Initializes the <see cref="LSourceMap"/>
		/// </summary>
		static void InitLSourceMap();

		/// <summary>
		/// Initializes a new instance of the <see cref="LightSource"/> class.  Will automatically add to the <paramref name="ownerLightLayer"/>.
		/// </summary>
		/// <param name="lightLayer">The light layer that will own <c>this</c>.  Value is assigned to <see cref="owner"/></param>
		LightSource(LightLayer* ownerLightLayer);

		/// <summary>
		/// Finalizes an instance of the <see cref="LightSource"/> class.  Removes the <c>this</c> from the <see cref="owner"/>.
		/// </summary>
		virtual ~LightSource();

	protected:		
		/// <summary>
		/// The allegro bitmap flags for loading <see cref="LSource_Map"/>
		/// </summary>
		static const int LSOURCE_MAP_FLAGS;
		
		/// <summary>
		/// The directory to load the <see cref="LSource_Map"/> from.
		/// </summary>
		static const std::string LSOURCE_IMG_DIR;
		
		/// <summary>
		/// The bitmap of a circular light, shadows are drawn onto this bitmap.
		/// </summary>
		static ALLEGRO_BITMAP* LSource_Map;
		
		/// <summary>
		/// The width of <see cref="LSource_Map"/>
		/// </summary>
		static int LSource_Map_W;
		
		/// <summary>
		/// The height of <see cref="LSource_Map"/>
		/// </summary>
		static int LSource_Map_H;
		
		/// <summary>
		/// Converts elements of <see cref="lightBlockers"/> to <see cref="ShadePoint"/>s, populating the vector <see cref="shadePoints"/>.
		/// </summary>
		/// <param name="lightBlockers">The <see cref="LightBlocker"/>s to be converted.</param>
		virtual void createShadePoints(std::list <LightBlocker*>& lightBlockers) = 0;
		
		/// <summary>
		/// Uses <see cref="shadePoints"/> to calculate the <see cref="drawPoints"/>.  This will handle shadows.
		/// </summary>
		virtual void mapShadePoints() = 0;
		
		/// <summary>
		/// Now that drawing operations are possible, <see cref="drawPoints"/> can be drawn to a bitmap data member, if it exists.
		/// </summary>
		virtual void drawLocal() = 0;
		
		/// <summary>
		/// Draws to the <see cref="LightLayer::lightMap">.  Assumes lightMap is already set as the target bitmap.
		/// </summary>
		virtual void drawToLightMap() = 0;
		
		/// <summary>
		/// When the <see cref="LightSource"/> is being processed, setting some variables may not be thread safe, so they are stored in heldVariables, this function tranfers their values.
		/// </summary>
		virtual void transferHeldVars() = 0;
		
		/// <summary>
		/// Stores the <see cref="ShadePoint"/>s, populated by <see cref="createShadePoints()"/>.
		/// </summary>
		std::vector <ShadePoint*> shadePoints;
		
		/// <summary>
		/// Stores the points to be drawn to.  Populated by <see cref="mapShadePoints()"/>.
		/// </summary>
		std::vector <float> drawPoints;

		/// <summary>
		/// The owner of <code>this</code>.  Set by constructor and is not reassigned afterwards.  Pointer is used to access lightBmpW and lightBmpH for drawing operations.  Also allows <c>this</c> to remove itself when <see cref="~LightSource()"/> is called.
		/// </summary>
		LightLayer* owner;
	};
}

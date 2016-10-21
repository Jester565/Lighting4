#pragma once
#include "ShadePoint.h"

namespace lighting
{	
	/// <summary>
	/// Variant of <see cref="ShadePoint"/> made for <see cref="AboveLightSource"/>s.
	/// </summary>
	/// <seealso cref="ShadePoint" />
	class AboveShadePoint : public ShadePoint
	{
	public:		
		/// <summary>
		/// Initializes a new instance of the <see cref="AboveShadePoint"/> class.  Sets the position and the <see cref="radixVal"/>.
		/// </summary>
		/// <param name="x">The horizontal position.</param>
		/// <param name="y">The vertical position.</param>
		/// <param name="maxX">The maximum value for <paramref name="x"/>, used to set <see cref="radixVal"/>.</param>
		AboveShadePoint(float x, float y, int minX, int maxX);
		
		/// <summary>
		/// Gets the <see cref="connectedPoint"/> which represents the other endpoint of the line.
		/// </summary>
		/// <returns></returns>
		AboveShadePoint* getConnectPoint()
		{
			return(AboveShadePoint*)connectedPoint;
		}

		unsigned int radixVal;
				
		/// <summary>
		/// Finalizes an instance of the <see cref="AboveShadePoint"/> class.
		/// </summary>
		~AboveShadePoint();
	};
}
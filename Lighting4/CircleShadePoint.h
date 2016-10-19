#pragma once
#include "ShadePoint.h"

namespace lighting
{
	class CircleShadePoint : public ShadePoint
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="CircleShadePoint"/> class.
		/// </summary>
		/// <param name="x">The x.</param>
		/// <param name="y">The y.</param>
		CircleShadePoint(float x, float y);

		/// <summary>
		/// Gets the other endpoint of the line.  May be <code>nullptr</code>.
		/// </summary>
		/// <returns>The other endpoint, stored in the <see cref="ShadePoint::connectedPoint"/> attribute.</returns>
		CircleShadePoint* getConnectPoint()
		{
			return (CircleShadePoint*)connectedPoint;
		}

		/// <summary>
		/// The angle of the coordinates when rotated around origin (0, 0).
		/// </summary>
		float rads;

		/// <summary>
		/// The integer value represented <see cref="rads"/> at a certain precision.
		/// </summary>
		unsigned int radixVal;

		~CircleShadePoint();
	};
}

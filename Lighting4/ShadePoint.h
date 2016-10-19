#pragma once


namespace lighting
{
	class ShadePoint
	{
	public:
		/// <summary>
		/// Assigns the local coordinates to the parameters and calculates the
		/// angle of those values from the origin (0, 0).
		/// </summary>
		/// <param name="x">The horizontal coordinate of the point.</param>
		/// <param name="y">The vertical coordinate of the point.</param>
		ShadePoint(float x, float y);

		/// <summary>
		/// Modifier for the attribute <see cref="ShadePoint::connectedPoint"/>.  Represents the other
		/// endpoint of a line.
		/// </summary>
		/// <param name="connectPoint"></param>
		void setConnectPoint(ShadePoint* connectPoint)
		{
			this->connectedPoint = connectPoint;
		}

		/// <summary>
		/// Checks if the endpoints passed in as parameters form a line that would
		/// intersect with a line formed by connecting the coordinates from <c> this </c> 
		/// <see cref="ShadePoint"/>s coordinates to the coordinates from <see cref="ShadePoint::connectedPoint"/>.
		/// </summary>
		/// <param name="x1">The horizontal position of the first endpoint.</param>
		/// <param name="y1">The vertical position of the first endpoint.</param>
		/// <param name="x2">The horizontal position of the second endpoint.</param>
		/// <param name="y2">The vertical position of the second endpoint.</param>
		/// <param name="cX">Serves as an output parameter.  If a collision occurs, the value
		/// will represent the horizontal position of the collision.  Otherwise, it is unmodified.</param>
		/// <param name="cY">Serves as an output parameter.  If a collision occurs, the value
		/// will represent the vertical position of the collision.  Otherwise, it is unmodified.</param>
		/// <returns><c>true</c> if the lines intersected. <c>false</c> otherwise.</returns>
		bool checkIntersect(float x1, float y1, float x2, float y2, float& cX, float& cY);

		/// <summary>
		/// The horizontal position of the point.
		/// </summary>
		float x;

		/// <summary>
		/// The horizontal position of the point.
		/// </summary>

		float y;

		/// <summary>
		/// Finalizes an instance of the <see cref="ShadePoint"/> class.  Note that
		/// <see cref="ShadePoint::connectedPoint"/> will not be deleted.
		/// </summary>
		virtual ~ShadePoint();

	protected:

		/// <summary>
		/// The other endpoint of the line.  Initially set to <c>nullptr</c> but can be
		/// set by the method <see cref="ShadePoint::setConnectPoint(ShadePoint*)"/>.
		/// </summary>
		ShadePoint* connectedPoint;
	};
}

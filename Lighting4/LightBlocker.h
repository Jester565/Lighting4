#pragma once

namespace lighting
{	
	/// <summary>
	/// Represents a line that will be used by <see cref="LightSource">s to determine if their rays are being blocked.
	/// </summary>
	class LightBlocker
	{
	public:		
		/// <summary>
		/// Initializes a new instance of the <see cref="LightBlocker"/> class.  Specifying the <paramref name="x"/> and <paramref name="y"/> of the line and the offset of the endpoints from <paramref name="x"/> and <paramref name="y"/>.
		/// </summary>
		/// <param name="x">The horizontal postion of the line.</param>
		/// <param name="y">The vertical position of the line.</param>
		/// <param name="epX1">The horizontal displacement of the first endpoint from <paramref name="x"/>. Sets <see cref="epX1"/>.</param>
		/// <param name="epY1">The vertical displacement of the first endpoint from <paramref name="y"/>. Sets <see cref="epY1"/>.</param>
		/// <param name="epX2">The horizontal displacement of the second endpoint from <paramref name="x"/>. Sets <see cref="epX2"/>.</param>
		/// <param name="epY2">The vertical displacement of the second endpoint from <paramref name="x"/>. Sets <see cref="epY2"/>.</param>
		LightBlocker(float x, float y, float epX1, float epY1, float epX2, float epY2);

		
		/// <summary>
		/// Sets the location of the line.  Modifies attributes <see cref="x1"/>, <see cref="y1"/> and <see cref="x2"/>, <see cref="y2">.
		/// </summary>
		/// <param name="x">The horizontal position.</param>
		/// <param name="y">The vertical position.</param>
		void setGlobalXY(float x, float y);

		/// <summary>
		/// Rotates the endpoints around <paramref name="cX"/> and <paramref name="cY"/>.  
		/// The change in coordinates from the rotation are stored in <see cref="rotateXOff1"/>, <see cref="rotateYOff1"/> and <see cref="rotateXOff2"/>, <see cref="rotateYOff2"/>.
		/// </summary>
		/// <param name="cX">The horizontal displacement from <see cref="x"/> to rotate around.</param>
		/// <param name="cY">The vertical displacement from <see cref="y"/> to rotate around.</param>
		/// <param name="rads">The rads.</param>
		void setRads(float cX, float cY, float rads);
		
		/// <summary>
		/// Horizontal position of first endpoint on the screen.
		/// </summary>
		float x1;

		/// <summary>
		/// Vertical position of first endpoint on the screen.
		/// </summary>
		float y1;
		
		/// <summary>
		/// Horizontal position of the second endpoint on the screen.
		/// </summary>
		float x2;
		
		/// <summary>
		/// Vertical position of the second endpoint on the screen.
		/// </summary>
		float y2;
		
		/// <summary>
		/// The displacement between x and the endpointX
		/// </summary>
		float epX1;

		/// <summary>
		/// The displacement between y and the endpointY
		/// </summary>
		float epY1;

		/// <summary>
		/// The displacement between x and the endpointX
		/// </summary>
		float epX2;

		/// <summary>
		/// The displacement between y and the endpointY
		/// </summary>
		float epY2;

		/// <summary>
		/// The horizontal displacement of the first endpoint due to rotation, set by <see cref="setRads()"/>.
		/// </summary>
		float rotateXOff1;
		
		/// <summary>
		/// The vertical displacement of the first endpoint due to rotation, set by <see cref="setRads()"/>.
		/// </summary>
		float rotateYOff1;

		/// <summary>
		/// The horizontal displacement of the second endpoint due to rotation, set by <see cref="setRads()"/>.
		/// </summary>
		float rotateXOff2;

		/// <summary>
		/// The vertical displacement of the second endpoint due to rotation, set by <see cref="setRads()"/>.
		/// </summary>
		float rotateYOff2;
		
		/// <summary>
		/// Finalizes an instance of the <see cref="LightBlocker"/> class.  Default destructor.
		/// </summary>
		~LightBlocker();
	};
}

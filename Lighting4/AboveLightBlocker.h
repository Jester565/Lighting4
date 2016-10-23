#pragma once
namespace lighting
{
	class LightLayer;
	
	/// <summary>
	/// Only used by <see cref="AboveLightSource"/>s to block out light from above.
	/// </summary>
	class AboveLightBlocker
	{
	public:		
		/// <summary>
		/// Initializes a new instance of the <see cref="AboveLightBlocker"/> class at a certain position and width.  Adds itself to <paramref name="owner"/>.
		/// </summary>
		/// <param name="owner">The <see cref="LightLayer"/> that <c>this</c> will be added to.</param>
		/// <param name="x">The horizontal position of the blocker.</param>
		/// <param name="epX1">The offset of the first endpoint from <paramref name="x"/>.</param>
		/// <param name="epX2">The offset of the second endpoint from <paramref name="x"/>.</param>
		AboveLightBlocker(LightLayer* owner, float x, float epX1, float epX2);
		
		/// <summary>
		/// Sets the horizontal position.
		/// </summary>
		/// <param name="x">The x coordinate on the screen.</param>
		void setX(float x)
		{
			x1 = x + epX1;
			x2 = x + epX2;
		}
		
		/// <summary>
		/// The horizontal position of the first endpoint on the screen.
		/// </summary>
		float x1;		
		/// <summary>
		/// The horizontal position of the second endpoint on the screen.
		/// </summary>
		float x2;		
		/// <summary>
		/// The offset of the first endpoint from x.
		/// </summary>
		float epX1;
		
		/// <summary>
		/// The offset of the second endpoint from y.
		/// </summary>
		float epX2;
		
		/// <summary>
		/// Finalizes an instance of the <see cref="AboveLightBlocker"/> class. Removes itself from <see cref="owner"/>.
		/// </summary>
		~AboveLightBlocker();

	private:		
		/// <summary>
		/// The <see cref="LightLayer"/> that owns <c>this</c>.
		/// </summary>
		LightLayer* owner;
	};
}

#pragma once

namespace lighting
{
	class LightLayer;

	class LightSource
	{
	public:

		/// <summary>
		/// Initializes a new instance of the <see cref="LightSource"/> class.  Will automatically add to the <paramref name="lightLayer"/>.
		/// </summary>
		/// <param name="lightLayer">The light layer that will own <c>this</c>.  Value is assigned to <see cref="owner"/></param>
		LightSource(LightLayer* lightLayer);

		/// <summary>
		/// Finalizes an instance of the <see cref="LightSource"/> class.  Removes the <c>this</c> from the <see cref="owner"/>.
		/// </summary>
		virtual ~LightSource();

	protected:
		/// <summary>
		/// The owner of <c>this</c>.  Set by constructor and is not reassigned afterwards.  Pointer is used to access lightBmpW and lightBmpH for drawing operations.  Also allows <c>this</c> to remove itself when <see cref="~LightSource()"/> is called.
		/// </summary>
		LightLayer* owner;
	};
}

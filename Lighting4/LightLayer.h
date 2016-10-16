#pragma once

namespace lighting
{
	class LightLayer
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="LightLayer"/> class.  
		/// </summary>
		/// <param name="drawToBmpW">The draw to BMP w.</param>
		/// <param name="drawToBmpH">The draw to BMP h.</param>
		/// <param name="lightBmpScale">The light BMP scale.</param>
		/// <param name="maxThreads">The maximum threads.</param>
		LightLayer(int drawToBmpW, int drawToBmpH, double lightBmpScale, size_t maxThreads);
		
		~LightLayer();
	};
}

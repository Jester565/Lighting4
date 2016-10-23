#pragma once
#include <vector>
#include <allegro5/shader.h>

namespace lighting
{	
	/// <summary>
	/// Generates and stores data necessary to create a <see cref="GaussianBlurrer"/>.
	/// </summary>
	class GaussianKernelData
	{
	public:		
		/// <summary>
		/// Initializes a new instance of the <see cref="GaussianKernelData"/> with no attributes set.
		/// </summary>
		GaussianKernelData();
		
		/// <summary>
		/// Initializes a new instance of the <see cref="GaussianKernelData"/> class.  Sets all attributes generating elements of arrays.
		/// </summary>
		/// <param name="pixelW">The width of the kernel.</param>
		/// <param name="sigma">The sigma.</param>
		GaussianKernelData(int pixelW, float sigma);
		
		/// <summary>
		/// The distance RGB values are to be taken from to blend between two pixels for better efficiency.
		/// </summary>
		std::vector <float> pixelOffsets;
		
		/// <summary>
		/// The RGB value of the pixels are mutliplied by these weights depending on their distance.
		/// </summary>
		std::vector <float> pixelWeights;
		
		/// <summary>
		/// The kernelWidth the data was calculated from.
		/// </summary>
		int kernelWidth;
		
		/// <summary>
		/// The sigma the data was calculated from.
		/// </summary>
		float sigma;

		~GaussianKernelData();
	};
}

#include "GaussianKernelData.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace lighting
{
	GaussianKernelData::GaussianKernelData()
		:kernelWidth(0), sigma(0)
	{
	}

	GaussianKernelData::GaussianKernelData(int kernelW, float sigma)
		:kernelWidth(kernelW), sigma(sigma)
	{
		if (kernelW < 0)
		{
			throw std::invalid_argument("kernelW must be greater than 0");
		}
		kernelW = round(kernelW / 4);
		kernelW *= 4;
		kernelW++;
		std::vector<std::vector<double>> kernel = std::vector<std::vector<double>>(kernelW);
		for (int i = 0; i < kernel.size(); i++)
		{
			kernel.at(i) = std::vector<double>(kernelW);
		}
		double mean = kernelW / 2;
		double sum = 0.0; // For accumulating the kernel values
		for (int x = 0; x < kernelW; ++x)
			for (int y = 0; y < kernelW; ++y) {
				kernel[x][y] = exp(-0.5 * (pow((x - mean) / sigma, 2.0) + pow((y - mean) / sigma, 2.0)))
					/ (2 * M_PI * sigma * sigma);

				// Accumulate the kernel values
				sum += kernel[x][y];
			}

		// Normalize the kernel
		for (int x = 0; x < kernelW; ++x)
		{
			for (int y = 0; y < kernelW; ++y)
			{
				kernel[x][y] /= sum;
			}
		}
		std::vector <double> oneKernel;
		for (int i = 0; i < kernelW; i++)
		{
			double totalCol = 0;
			for (int j = 0; j < kernelW; j++)
			{
				totalCol += kernel[i][j];
			}
			oneKernel.push_back(totalCol);
		}
		int kI = kernelW / 2 + 1;
		pixelWeights.push_back(oneKernel[kernelW / 2]);
		pixelOffsets.push_back(0);
		for (int i = 1; i <= kernelW / 2; i += 2)
		{
			double offset = (i * oneKernel[kI] + (i + 1) * oneKernel[kI + 1]) / (oneKernel[kI] + oneKernel[kI + 1]);
			pixelOffsets.push_back(offset);
			pixelWeights.push_back(oneKernel[kI] + oneKernel[kI + 1]);
			kI += 2;
		}
	}

	GaussianKernelData::~GaussianKernelData()
	{
	}
}

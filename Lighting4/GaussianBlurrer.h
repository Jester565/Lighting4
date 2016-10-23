#pragma once
#include <allegro5/bitmap.h>
#include <string>
#include "GaussianKernelData.h"

namespace lighting
{
	class LightLayer;

	/// <summary>
	/// Takes bitmaps and uses a two pass gaussian blur on it.  Specify which shaders to use and the kernelData. Adds itself to <paramref name="owner"/>.
	/// </summary>
	class GaussianBlurrer
	{
	public:		
		/// <summary>
		/// Initializes a new instance of the <see cref="GaussianBlurrer"/> class.  Creating new shaders with the proper <paramref name="kernelData"/>.
		/// </summary>
		/// <param name="owner">The <see cref="LightLayer"/> that owns <code>this</code>.</param>
		/// <param name="kernelData">The <see cref="GaussianKernelData"/> used to set the variables of the shaders.  Uses the PixelOffsets and PixelWeights to set the shaders.  The size of PixelOffsets and PixelWeights must be less than 12 (When generating the KernelData, use values less than 44)</param>
		/// <param name="vertShaderPath">The vertex shader path, must be compatible with Allegro (See their default shader for an example).</param>
		/// <param name="pixelShaderPathX">The pixel shader path for horizontal gaussian blur.  Should have BmpWidth, PixelOffsets, and PixelWeights variables to set.</param>
		/// <param name="pixelShaderPathY">The pixel shader path for vertical gaussian blur.  Should have BmpHeight, PixelOffsets, and PixelWeights variables to set.</param>
		/// <param name="platform">The platform to create the shaders on, HLSL or GLSL.</param>
		GaussianBlurrer(LightLayer* owner, GaussianKernelData& kernelData, const std::string& vertShaderPath, const std::string& pixelShaderPathX, const std::string& pixelShaderPathY, ALLEGRO_SHADER_PLATFORM platform = ALLEGRO_SHADER_AUTO);
		
		/// <summary>
		/// Called by <see cref="LightMap"/>.  The <paramref name="mapToBlur"/> will be fully gaussian blurred and the <see cref="placeholder"/> bitmap will be horizontally blurred.  (WARNING SHADER NOT SET TO NULLPTR WHEN FINISHED).
		/// </summary>
		/// <param name="mapToBlur">The map to blur.</param>
		/// <param name="placeholder">The placeholder.</param>
		void blur(ALLEGRO_BITMAP* mapToBlur, ALLEGRO_BITMAP* placeholder);
		
		/// <summary>
		/// Finalizes an instance of the <see cref="GaussianBlurrer"/> class.  Removes itself from <paramref name="owner"/>.
		/// </summary>
		~GaussianBlurrer();

	protected:		
		/// <summary>
		/// Creates a new shader using the paths specified.
		/// </summary>
		/// <param name="vertShaderPath">The vertex shader path, must be compatible with Allegro (See their default shader for an example).</param>
		/// <param name="pixelShaderPath">The pixel shader path.</param>
		/// <param name="platform">The shader platform (HLSL or GLSL).</param>
		/// <returns></returns>
		ALLEGRO_SHADER* getShader(const std::string& vertShaderPath, const std::string& pixelShaderPath, ALLEGRO_SHADER_PLATFORM platform);		
		/// <summary>
		/// The horizontal gaussian shader (called first).
		/// </summary>
		ALLEGRO_SHADER* shaderX;		
		/// <summary>
		/// The vertial gaussian shader (called last).
		/// </summary>
		ALLEGRO_SHADER* shaderY;		
		/// <summary>
		/// The <see cref="LightLayer"/> that owns <code>this</code>.
		/// </summary>
		LightLayer* owner;
	};
}

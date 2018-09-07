#include "GaussianBlurrer.h"
#include "LightLayer.h"
#include <allegro5/allegro.h>
#include <iostream>
#include <string>

namespace lighting
{
	ALLEGRO_SHADER* GaussianBlurrer::getShader(const std::string & vertShaderPath, const std::string & pixelShaderPath, ALLEGRO_SHADER_PLATFORM platform)
	{
		ALLEGRO_SHADER* shader = al_create_shader(platform);
		if (!shader)
		{
			std::cerr << "Could not create shader" << std::endl;
			return nullptr;
		}
		if (!al_attach_shader_source_file(shader, ALLEGRO_VERTEX_SHADER, vertShaderPath.c_str()))
		{
			std::cerr << "Unable to compile the vertex shader " << vertShaderPath << std::endl;
			std::cerr << al_get_shader_log(shader) << std::endl;
			return nullptr;
		}
		if (!al_attach_shader_source_file(shader, ALLEGRO_PIXEL_SHADER, pixelShaderPath.c_str()))
		{
			std::cerr << "Unable to compile the pixel shader " << pixelShaderPath << std::endl;
			std::cerr << al_get_shader_log(shader) << std::endl;
			return nullptr;
		}
		if (!al_build_shader(shader))
		{
			std::cerr << "Unable to build the shaders " << vertShaderPath << " " << pixelShaderPath << std::endl;
			std::cerr << al_get_shader_log(shader) << std::endl;
			return nullptr;
		}
		return shader;
	}
	GaussianBlurrer::GaussianBlurrer(LightLayer* owner, GaussianKernelData& kernelData, const std::string& vertShaderPath, const std::string& pixelShaderPath1, const std::string& pixelShaderPath2, ALLEGRO_SHADER_PLATFORM platform)
		:owner(owner)
	{
		shaderX = getShader(vertShaderPath, pixelShaderPath1, platform);
		shaderY = getShader(vertShaderPath, pixelShaderPath2, platform);
		if (!al_use_shader(shaderX))
		{
			std::cerr << "Unable to use the shader" << std::endl;
			std::cerr << al_get_shader_log(shaderX) << std::endl;
		}
		if (!al_set_shader_float_vector("PixelOffsets", 1, kernelData.pixelOffsets.data(), kernelData.pixelOffsets.size()))
		{
			std::cerr << "Unable to set PixelOffsets" << std::endl;
		}
		if (!al_set_shader_float_vector("BlurWeights", 1, kernelData.pixelWeights.data(), kernelData.pixelWeights.size()))
		{
			std::cerr << "Unable to set BlurWeights" << std::endl;
		}
		if (!al_set_shader_float("BmpWidth", owner->getLightBmpScale() * owner->drawToWidth))
		{
			std::cerr << "Unable to set BmpWidth" << std::endl;
		}
		if (!al_use_shader(shaderY))
		{
			std::cerr << "Unable to use the shader" << std::endl;
			std::cerr << al_get_shader_log(shaderY) << std::endl;
		}
		if (!al_set_shader_float_vector("PixelOffsets", 1, kernelData.pixelOffsets.data(), kernelData.pixelOffsets.size()))
		{
			std::cerr << "Unable to set PixelOffsets" << std::endl;
		}
		if (!al_set_shader_float_vector("BlurWeights", 1, kernelData.pixelWeights.data(), kernelData.pixelWeights.size()))
		{
			std::cerr << "Unable to set BlurWeights" << std::endl;
		}
		if (!al_set_shader_float("BmpHeight", owner->getLightBmpScale() * owner->drawToWidth))
		{
			std::cerr << "Unable to set BmpHeight" << std::endl;
		}
		al_use_shader(nullptr);
		owner->addGaussianBlurrer(this);
	}

	void GaussianBlurrer::blur(ALLEGRO_BITMAP * originalMap, ALLEGRO_BITMAP * gausMap)
	{
		al_set_target_bitmap(gausMap);
		if (!al_use_shader(shaderX))
		{
			std::cerr << "Unable to use the shader" << std::endl;
			std::cerr << al_get_shader_log(shaderX) << std::endl;
		}
		al_draw_bitmap(originalMap, 0, 0, NULL);
		al_set_target_bitmap(originalMap);
		if (!al_use_shader(shaderY))
		{
			std::cerr << "Unable to use the shader" << std::endl;
			std::cerr << al_get_shader_log(shaderY) << std::endl;
		}
		al_draw_bitmap(gausMap, 0, 0, NULL);
	}

	GaussianBlurrer::~GaussianBlurrer()
	{
		owner->removeGaussianBlurrer(this);
		al_destroy_shader(shaderX);
		shaderX = nullptr;
		al_destroy_shader(shaderY);
		shaderY = nullptr;
	}
}
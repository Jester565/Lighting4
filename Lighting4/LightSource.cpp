#include "LightSource.h"
#include "allegro5/bitmap_io.h"
#include "LightLayer.h"

namespace lighting
{
	const int LSOURCE_MAP_FLAGS = ALLEGRO_MIN_LINEAR;  //Used to have ALLEGRO_MAG_LINEAR flag

	const std::string LightSource::LSOURCE_IMG_DEFAULT_DIR = "light.png";

	ALLEGRO_BITMAP* LightSource::LSource_Map = nullptr;

	int LightSource::LSource_Map_W = 0;

	int LightSource::LSource_Map_H = 0;

	void LightSource::InitLSourceMap(const std::string& lightDir)
	{
		al_set_new_bitmap_flags(LSOURCE_MAP_FLAGS);
		LSource_Map = al_load_bitmap(lightDir.c_str());
		if (LSource_Map == nullptr)
		{
			std::string err = "Could not load the LSource_Map from the file ";
			err += lightDir;
			err += "... Use LightGenerator to create this file";
			throw std::exception(err.c_str());
		}
		LSource_Map_W = al_get_bitmap_width(LSource_Map);
		LSource_Map_H = al_get_bitmap_height(LSource_Map);
	}

	LightSource::LightSource(LightLayer* lightLayerOwner)
		:owner(lightLayerOwner)
	{
		owner->addLightSource(this);
	}

	LightSource::~LightSource()
	{
		owner->removeLightSource(this);
	}
}
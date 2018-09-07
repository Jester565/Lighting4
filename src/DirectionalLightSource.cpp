#include "DirectionalLightSource.h"
#include "LightLayer.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

namespace lighting
{
	DirectionalLightSource::DirectionalLightSource(LightLayer * ownerLightLayer, int radius, float degs, uint8_t r, uint8_t g, uint8_t b)
		:CircleLightSource(ownerLightLayer, radius, r, g, b), rads(0)
	{
		float rads = degs * (M_PI / 180.0f);
		float dlmH = (sin(rads / 2) / cos(rads / 2)) * radius * 2;
		directionalLSourceW = radius * owner->getLightBmpScale();
		directionalLSourceH = dlmH * owner->getLightBmpScale();
		al_set_new_bitmap_flags(SHADE_MAP_FLAGS);
		directionalLSourceMapSave = al_create_bitmap(directionalLSourceW, directionalLSourceH);
		al_set_new_bitmap_flags(SHADE_MAP_FLAGS);
		directionalLSourceMap = al_create_bitmap(directionalLSourceW, directionalLSourceH);
		ALLEGRO_BITMAP* prevBitmap = al_get_target_bitmap();
		al_set_target_bitmap(directionalLSourceMapSave);
		al_set_separate_blender(ALLEGRO_DEST_MINUS_SRC, ALLEGRO_ZERO, ALLEGRO_INVERSE_ALPHA, ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);
		al_clear_to_color(al_map_rgba(255, 255, 255, 0));
		//The y position of the LSource_Map to draw from
		float sY = ((LSource_Map_H / 2.0f) - (((float)dlmH / (float)radius)*(LSource_Map_H / 2)) / 2.0f);
		//The height of the LSource_Map in the draw
		float sH = (((float)dlmH / (float)radius)*(LSource_Map_H / 2));
		al_draw_scaled_bitmap(LSource_Map, (LSource_Map_W / 2), sY, (LSource_Map_W / 2.0f), sH, 0, 0, radius * owner->getLightBmpScale(), dlmH * owner->getLightBmpScale(), NULL);
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
		//Get rid of slices of the light image to form cone
		al_draw_filled_triangle(0, (dlmH / 2) * owner->getLightBmpScale(), radius * owner->getLightBmpScale(), 0, 0, 0, al_map_rgba(255, 255, 255, 0));
		al_draw_filled_triangle(0, (dlmH / 2) * owner->getLightBmpScale(), radius * owner->getLightBmpScale(), dlmH * owner->getLightBmpScale(), 0, dlmH * owner->getLightBmpScale(), al_map_rgba(255, 255, 255, 0));
		al_set_target_bitmap(prevBitmap);
		//set blender mode to default
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
	}

	void DirectionalLightSource::drawLocal()
	{
		al_set_target_bitmap(shadeMap);
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
		al_clear_to_color(al_map_rgba(0, 0, 0, 255));
		for (int i = 0; i < drawPoints.size() - 2; i += 4)
		{
			al_draw_filled_triangle(drawPoints.at(i), drawPoints.at(i + 1), drawPoints.at(i + 2), drawPoints.at(i + 3), radius * owner->getLightBmpScale(), radius * owner->getLightBmpScale(), lightColor);
		}
		al_draw_filled_triangle(drawPoints.at(0), drawPoints.at(1), drawPoints.at(drawPoints.size() - 2), drawPoints.at(drawPoints.size() - 1), radius * owner->getLightBmpScale(), radius * owner->getLightBmpScale(), lightColor);
		al_set_target_bitmap(directionalLSourceMap);
		al_clear_to_color(lightColor);
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ZERO);
		//rotate and draw shadeMap to the directionalLSourceMap so that not all of the light is drawn.
		al_draw_scaled_rotated_bitmap(shadeMap, radius * owner->getLightBmpScale(), radius * owner->getLightBmpScale(), 0, directionalLSourceH / 2, 1, 1, -rads, NULL);
		al_set_separate_blender(ALLEGRO_DEST_MINUS_SRC, ALLEGRO_ALPHA, ALLEGRO_ALPHA, ALLEGRO_SRC_MINUS_DEST, ALLEGRO_ONE, ALLEGRO_ONE);
		al_draw_bitmap(directionalLSourceMapSave, 0, 0, NULL);
	}

	void DirectionalLightSource::drawToLightMap()
	{
		al_draw_rotated_bitmap(directionalLSourceMap, 0, (directionalLSourceH / 2), x * owner->getLightBmpScale(), y * owner->getLightBmpScale(), rads, NULL);
	}

	DirectionalLightSource::~DirectionalLightSource()
	{
		al_destroy_bitmap(directionalLSourceMapSave);
		directionalLSourceMapSave = nullptr;
		al_destroy_bitmap(directionalLSourceMap);
		directionalLSourceMap = nullptr;
	}
}

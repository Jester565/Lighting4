#pragma once
#include <Core.h>
#include <LightLayer.h>
#include <CircleLightSource.h>
#include <AboveLightSource.h>
#include <LightBlockerContainer.h>

using namespace AllegroExt;
using namespace lighting;

class TestCore : public Core
{
public:
	TestCore();

	bool init() override;

	void draw() override;

	~TestCore();

private:
	LightLayer* lightLayer;
	CircleLightSource* lightSource;
	AboveLightSource* sun;
	LightBlockerContainer* lbc;
};

#pragma once
#include <Core.h>
#include <LightLayer.h>
#include <CircleLightSource.h>
#include <AboveLightSource.h>
#include <LightBlockerContainer.h>
#include <GaussianKernelData.h>
#include <GaussianBlurrer.h>

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
	GaussianKernelData kernelData;
	GaussianBlurrer* blurrer;
	LightLayer* lightLayer;
	CircleLightSource* lightSource;
	AboveLightSource* sun;
	LightBlockerContainer* lbc;
};

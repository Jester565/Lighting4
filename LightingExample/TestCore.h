#pragma once
#include <Core.h>
#include <LightLayer.h>
#include <CircleLightSource.h>
#include <AboveLightSource.h>
#include <LightBlockerContainer.h>
#include <GaussianKernelData.h>
#include <GaussianBlurrer.h>
#include <FPSLogger.h>

using namespace AllegroExt;
using namespace lighting;

class TestCore : public Core
{
public: 
	static const int LS_SIZE = 4;
	TestCore();

	bool init() override;

	void draw() override;

	~TestCore();

private:
	FPSLogger* fpsLogger;
	GaussianKernelData kernelData;
	GaussianBlurrer* blurrer;
	LightLayer* lightLayer;
	CircleLightSource* circleLightSource;
	LightBlockerContainer* lbc;
};

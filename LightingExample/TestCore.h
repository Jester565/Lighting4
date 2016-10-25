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

	void populateLBC(int x, int y, int rowNum, int colNum, int lbcWidth, int offset);

	~TestCore();

private:
	FPSLogger* fpsLogger;
	GaussianKernelData kernelData;
	GaussianBlurrer* blurrer;
	LightLayer* lightLayer;
	std::vector <CircleLightSource*> lightSources;
	AboveLightSource* sun;
	int drawCount;
	float lightVX;
	float lightVY;
	float lightX, lightY;
	int passes;
	void handlePass();
	void generateReport();
	uint64_t startTimeMillis;
	std::vector <LightBlockerContainer*> lbcs;
};

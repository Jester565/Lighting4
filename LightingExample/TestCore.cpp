#include "TestCore.h"
#include <iostream>

TestCore::TestCore()
	:Core(""), kernelData(43, 9), lightLayer(nullptr)
{
}

bool TestCore::init()
{
	if (Core::init())
	{
		std::cout << kernelData.pixelOffsets.size() << std::endl;
		//lightLayer = new LightLayer(.25);
		lightLayer = new LightLayer(STANDARD_WIDTH, STANDARD_HEIGHT, .25);
		lightSource = new CircleLightSource(lightLayer, 500);
		lightSource->setXY(STANDARD_WIDTH / 2, STANDARD_HEIGHT / 2);
		//sun = new AboveLightSource(lightLayer, 0, 200, 200, 200, 200);
		lbc = new LightBlockerContainer(lightLayer);
		blurrer = new GaussianBlurrer(lightLayer, kernelData, "VertShader.hlsl", "XFragShader.hlsl", "YFragShader.hlsl");
		lbc->initSquare(40, 40);
		lbc->setXY(STANDARD_WIDTH / 2 - 200, STANDARD_HEIGHT / 2 - 40);
		return true;
	}
	return false;
}

void TestCore::draw()
{
	//lightLayer->dispatch();
	lightLayer->detach();
	lightLayer->draw();
}


TestCore::~TestCore()
{
}

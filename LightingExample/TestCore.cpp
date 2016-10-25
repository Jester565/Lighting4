#include "TestCore.h"
#include <LightBlocker.h>
#include <Windows.h>
#include <iostream>

TestCore::TestCore()
	:Core(""), kernelData(43, 9), lightLayer(nullptr)
{
	
}

bool TestCore::init()
{
	if (Core::init())
	{
		fpsLogger = new FPSLogger(1);
		lightLayer = new LightLayer(STANDARD_WIDTH, STANDARD_HEIGHT, 1, 4);
		circleLightSource = new CircleLightSource(lightLayer, 100, 240, 240, 240);
		lbc = new LightBlockerContainer(lightLayer);
		lbc->initSquare(500, 40);
		lbc->setXY(750, 500);
		LightBlocker* lightBlocker = new LightBlocker(500, 500, 0, 0, 500, 0);
		//lightLayer->addLightBlocker(lightBlocker);
		return true;
	}
	return false;
}

void TestCore::draw()
{
	circleLightSource->setXY(AllegroExt::Input::InputManager::mouseX, AllegroExt::Input::InputManager::mouseY);
	lightLayer->detach();
	lightLayer->draw();
}

TestCore::~TestCore()
{
}

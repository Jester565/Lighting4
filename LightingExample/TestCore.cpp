#include "TestCore.h"
#include <Windows.h>
#include <iostream>

TestCore::TestCore()
	:Core(""), kernelData(43, 9), drawCount(0), lightLayer(nullptr), lightX(STANDARD_WIDTH / 2), lightY(STANDARD_HEIGHT / 2), lightVX(1), lightVY(0), passes(0)
{
	
}

bool TestCore::init()
{
	if (Core::init())
	{
		fpsLogger = new FPSLogger(1);
		std::cout << kernelData.pixelOffsets.size() << std::endl;
		lightLayer = new LightLayer(STANDARD_WIDTH, STANDARD_HEIGHT, .25, 4);
		for (int i = 0; i < LS_SIZE; i++)
		{
			CircleLightSource* circleLightSource = new CircleLightSource(lightLayer, 900);
			lightSources.push_back(circleLightSource);
		}
		//blurrer = new GaussianBlurrer(lightLayer, kernelData, "VertShader.hlsl", "XFragShader.hlsl", "YFragShader.hlsl");
		populateLBC(0, 100, 1, 1, 2100, 70);
		populateLBC(0, 200, 50, 1, 50, 70);
		//populateLBC(25, 290, 50, 1, 50, 70);
		populateLBC(50, 380, 80, 1, 20, 70);
		//populateLBC(0, 200, 700, 4, 7, 6);
		//populateLBC(10, 600, 200, 2, 5, 50);
		populateLBC(10, 690, 100, 1, 10, 50);
		//populateLBC(0, 480, 700, 4, 2, 6);
		//populateLBC(10, 1000, 700, 1, 4, 0);
		return true;
	}
	return false;
}

void TestCore::draw()
{
	if (drawCount == 0)
	{
		startTimeMillis = GetTickCount64();
	}
	lightX += lightVX * rate;
	lightY += lightVY * rate;
	for (int i = 0; i < LS_SIZE; i++)
	{
		lightSources.at(i)->setXY(lightX, lightY);
	}
	
	lightLayer->detach();
	lightLayer->draw();
	fpsLogger->draw(10, 30, 25);
	drawCount++;
	if (lightX > STANDARD_WIDTH)
	{
		handlePass();
	}
	if (lightY > STANDARD_HEIGHT)
	{
		handlePass();
	}
}

void TestCore::populateLBC(int x, int y, int rowNum, int colNum, int lbcWidth, int offSet)
{
	for (int i = 0; i < rowNum; i++)
	{
		for (int j = 0; j < colNum; j++)
		{
			LightBlockerContainer* lbc = new LightBlockerContainer(lightLayer);
			lbc->initSquare(lbcWidth, lbcWidth);
			lbc->setXY(x + i * (lbcWidth + offSet), y + j * (lbcWidth + offSet));
			lbcs.push_back(lbc);
		}
	}
}


TestCore::~TestCore()
{
}

void TestCore::handlePass()
{
	passes++;
	if (passes == 1)
	{
		lightX = 0;
		lightY = 270;
		lightVX = 10;
	}
	else if (passes == 2)
	{
		lightX = 0;
		lightY = 710;
		lightVX = 1;
	}
	else if (passes == 3)
	{
		lightX = 1000;
		lightY = 0;
		lightVX = 0;
		lightVY = .5;
	}
	else if (passes == 4)
	{
		lightX = 0;
		lightY = STANDARD_HEIGHT / 2;
		lightVX = 20;
		lightVY = 10;
	}
	else
	{
		generateReport();
		for (int i = 0; i < lbcs.size(); i++)
		{
			delete lbcs.at(i);
		}
		lbcs.clear();
		for (int i = 0; i < LS_SIZE; i++)
		{
			delete lightSources.at(i);
		}
		lightSources.clear();
		delete lightLayer;
		lightLayer = nullptr;
		shutDown();
	}
}

void TestCore::generateReport()
{
	std::cout << std::endl << "STANDARD VARIABLE REPORT: " << std::endl;
	std::cout << "NUM LIGHT SOURCES: " << lightSources.size() << std::endl;
	std::cout << "NUM LIGHT BLOCKERS: " << lightLayer->getNumLightBlockers() << std::endl;
	std::cout << "NUM LIGHT BLOCKER CONTAINER: " << lbcs.size() << std::endl << std::endl;
	std::cout << "CIRLCE LIGHT SOURCE PERFORMANCE REPORT: " << std::endl;
	std::cout << "Total ShadePointsProcessed: " << CircleLightSource::ShadePointsProcessed << std::endl;
	std::cout << "Total CastPointsProcessed: " << CircleLightSource::CastPointsProcessed << std::endl;
	std::cout << "CastPointsProcessed / ShadePointsProcessed " << (float)(CircleLightSource::CastPointsProcessed) / (float)CircleLightSource::ShadePointsProcessed << std::endl << std::endl;
	std::cout << "NET PERFORMANCE REPORT: " << std::endl;
	std::cout << "AVG MS: " << (GetTickCount64() - startTimeMillis) / (float)drawCount << std::endl;
	std::cout << "AVG FPS: " << 1000.0f / ((GetTickCount64() - startTimeMillis) / (float)drawCount) << std::endl;
	std::cout << "Total # of DRAW CALLS: " << drawCount << std::endl;
	std::cout << "Total MS: " << GetTickCount64() - startTimeMillis << std::endl;
}

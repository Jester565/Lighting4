#include "LightRunnable.h"

namespace lighting
{
	LightRunnable::LightRunnable()
		:runThread(&LightRunnable::run, this), lightBlockersCopied(false), shadowsProcessed(false), runTime(0)
	{
		shadowsProcessedMutex.lock();
		runThread.detach();
	}

	void LightRunnable::run()
	{
		while (true)
		{
			lightBlockersCopiedMutex.lock();
			shadowsProcessedMutex.lock();
			auto startTime = std::chrono::high_resolution_clock::now();
			transferLightBlockers();
			lightBlockersCopiedMutex.unlock();
			processShadows();
			shadowsProcessedMutex.unlock();
			auto endTime = std::chrono::high_resolution_clock::now();
			runTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
			while (true)
			{
				shadowsProcessedMutex.lock();
				if (!shadowsProcessed)
				{
					shadowsProcessedMutex.unlock();
					break;
				}
				shadowsProcessedMutex.unlock();
			}
		}
	}

	std::list<LightSource*>::iterator LightRunnable::addLightSource(LightSource * lightSource)
	{
		lightSources.push_front(lightSource);
		return lightSources.begin();	//Iterators are safe to store
	}

	void LightRunnable::removeLightSource(std::list<LightSource*>::iterator removeIter)
	{
		lightSources.erase(removeIter);
	}

	LightRunnable::~LightRunnable()
	{
		
	}

	void LightRunnable::transferLightBlockers()
	{
		for (auto it = lightSources.begin(); it != lightSources.end(); it++)
		{
			(*it)->createShadePoints();
		}
		lightBlockersCopied = true;
	}

	void LightRunnable::processShadows()
	{
		for (auto it = lightSources.begin(); it != lightSources.end(); it++)
		{
			(*it)->mapShadePoints();
		}
		shadowsProcessed = true;
	}
} 
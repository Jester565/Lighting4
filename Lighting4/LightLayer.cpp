#include "LightLayer.h"
#include <iostream>
#include <allegro5/allegro.h>
#include "LightRunnable.h"
#include "LightSource.h"


namespace lighting
{
	LightLayer::LightLayer(int drawToBmpW, int drawToBmpH, double lightBmpScale, size_t maxThreads)
		:drawToWidth(drawToBmpW), drawToHeight(drawToBmpH), lightBmpScale(lightBmpScale), threadsProcessing(false)
	{
		if (maxThreads != MAX_THREAD_TO_CORES)
		{
			this->maxThreads = maxThreads;
		}
		else
		{
			setMaxThreadsToNumCores();
		}
		al_set_new_bitmap_flags(LIGHT_MAP_FLAGS);
		lightMap = al_create_bitmap((int)(drawToBmpW * lightBmpScale), (int)(drawToBmpH * lightBmpScale));
		LightSource::InitLSourceMap();
	}

	void LightLayer::detach()
	{
		transferHeldVars();
		for (auto it = lightRunnables.begin(); it != lightRunnables.end(); it++)
		{
			(*it)->shadowsProcessed = false;
			(*it)->lightBlockersCopied = false;
			(*it)->shadowsProcessedMutex.unlock();
		}
		std::list <LightRunnable*> copyLightRunnables;
		while (copyLightRunnables.size() > 0)
		{
			for (auto it = copyLightRunnables.begin(); it != copyLightRunnables.end();)
			{
				bool erased = false;
				LightRunnable* lightRunnable = (*it);
				if (lightRunnable->lightBlockersCopiedMutex.try_lock())
				{
					if (lightRunnable->lightBlockersCopied)
					{
						erased = true;
						it = copyLightRunnables.erase(it);
					}
					lightRunnable->lightBlockersCopiedMutex.unlock();
				}
				if (!erased)
				{
					it++;
				}
			}
		}
	}

	void LightLayer::draw()
	{
		ALLEGRO_BITMAP* prevBitmap = al_get_target_bitmap();
		std::list <LightRunnable*> unfinishedLRs = lightRunnables;
		while (unfinishedLRs.size() > 0)
		{
			for (auto it = unfinishedLRs.begin(); it != unfinishedLRs.end();)
			{
				bool erased = false;;
				LightRunnable* lr = (*it);
				if (lr->shadowsProcessedMutex.try_lock())
				{
					if (lr->shadowsProcessed)
					{
						erased = true;
						lr->drawLocal();
						it = unfinishedLRs.erase(it);
					}
					else
					{
						lr->shadowsProcessedMutex.unlock();
					}
				}
				if (!erased)
				{
					it++;
				}
			}
		}
		al_set_target_bitmap(lightMap);
		al_set_separate_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE, ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
		for (auto it = lightRunnables.begin(); it != lightRunnables.end(); it++)
		{
			(*it)->drawToLightMap();
		}
		al_set_target_bitmap(prevBitmap);
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_ALPHA);
		al_draw_scaled_bitmap(lightMap, 0, 0, drawToWidth * lightBmpScale, drawToHeight * lightBmpScale, 0, 0, drawToWidth, drawToHeight, NULL);
		al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
		al_set_target_bitmap(lightMap);
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));
		al_set_target_bitmap(prevBitmap);
	}

	void LightLayer::addLightSource(LightSource * lightSource)
	{
		if (threadsProcessing)
		{
			heldAddLightSources.push(lightSource);
		}
		else
		{
			addLightSourceUnsafe(lightSource);
		}
	}

	void LightLayer::removeLightSource(LightSource * lightSource)
	{
		if (threadsProcessing)
		{
			heldRemoveLightSources.push(lightSource);
		}
		else
		{
			removeLightSourceUnsafe(lightSource);
		}
	}

	void LightLayer::addLightBlocker(LightBlocker * lightBlocker)
	{
		lightBlockers.push_back(lightBlocker);
		//lightBlockerTrackerMap.emplace(std::make_pair(lightBlocker, lightBlockers.end()));
	}

	void LightLayer::removeLightBlocker(LightBlocker * lightBlocker)
	{
		//auto trackMapIter = lightBlockerTrackerMap.find(lightBlocker);
		lightBlockers.remove(lightBlocker);
		//lightBlockers.erase(trackMapIter->second);
		//lightBlockerTrackerMap.erase(lightBlocker);
	}

	LightLayer::~LightLayer()
	{
		al_destroy_bitmap(lightMap);
	}

	void LightLayer::addAboveLightBlocker(AboveLightBlocker * aboveLightBlocker)
	{
		aboveLightBlockers.emplace(aboveLightBlocker);
	}

	void LightLayer::removeAboveLightBlocker(AboveLightBlocker * aboveLightBlocker)
	{
		aboveLightBlockers.erase(aboveLightBlocker);
	}

	void LightLayer::transferHeldVars()
	{
		while (!heldAddLightSources.empty())
		{
			addLightSourceUnsafe(heldAddLightSources.front());
			heldAddLightSources.pop();
		}
		while (!heldRemoveLightSources.empty())
		{
			removeLightSourceUnsafe(heldRemoveLightSources.front());
			heldRemoveLightSources.pop();
		}
		for (auto it = lightRunnables.begin(); it != lightRunnables.end(); it++)
		{
			(*it)->transferHeldVars();
		}
	}

	void LightLayer::addLightSourceUnsafe(LightSource * lightSource)
	{
		LightRunnable* lrAdd = nullptr;	//light runnable that lightSource was added to
		std::list <LightSource*>::iterator lsAddIter; //iterator to the list the lightSource was added to
		if (lightRunnables.size() < maxThreads)
		{
			LightRunnable* lightRunnable = new LightRunnable();
			lrAdd = lightRunnable;
			lsAddIter = lightRunnable->addLightSource(lightSource);
			lightRunnables.push_back(lightRunnable);
		}
		else
		{
			LightRunnable* minTimeRunnable = lightRunnables.front();
			int estimateSourceTime = 0;
			for (auto it = lightRunnables.begin(); it != lightRunnables.end(); it++)
			{
				if ((*it)->getRunTime() < minTimeRunnable->getRunTime())
				{
					minTimeRunnable = *it;
				}
				estimateSourceTime += (*it)->getRunTime() / (*it)->getLightSourceSize();
			}
			lrAdd = minTimeRunnable;
			lsAddIter = minTimeRunnable->addLightSource(lightSource);
			minTimeRunnable->incrRunTime(estimateSourceTime);
		}
		lightSourceTrackerMap.emplace(std::make_pair(lightSource, std::make_pair(lrAdd, lsAddIter)));
	}

	void LightLayer::removeLightSourceUnsafe(LightSource * lightSource)
	{
		auto trackIter = lightSourceTrackerMap.find(lightSource);
		trackIter->second.first->removeLightSource(trackIter->second.second);
		lightSourceTrackerMap.erase(lightSource);
	}

	void LightLayer::setMaxThreadsToNumCores()
	{
		maxThreads = std::thread::hardware_concurrency();
		if (maxThreads == 0)
		{
			maxThreads = 4;
			std::cerr << "Could not get the number of cores, assumed 4" << std::endl;
		}
	}
}

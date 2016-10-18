#include "LightRunnable.h"

namespace lighting
{
	LightRunnable::LightRunnable(LightSource * initialLightSource)
	{
	}

	void LightRunnable::run()
	{
	}

	std::list<LightSource*>::iterator LightRunnable::addLightSource(LightSource * lightSource)
	{
		return std::list<LightSource*>::iterator();
	}

	void LightRunnable::removeLightSource(std::list<LightSource*>::iterator removeIter)
	{

	}

	LightRunnable::~LightRunnable()
	{
	}
	void LightRunnable::transferLightBlockers()
	{
	}
	void LightRunnable::processShadows()
	{
	}
} 
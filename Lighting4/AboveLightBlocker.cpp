#include "AboveLightBlocker.h"
#include "LightLayer.h"

namespace lighting
{
	AboveLightBlocker::AboveLightBlocker(LightLayer * owner, float x, float epX1, float epX2)
		:owner(owner), epX1(epX1), epX2(epX2)
	{
		setX(x);
		owner->addAboveLightBlocker(this);
	}

	AboveLightBlocker::~AboveLightBlocker()
	{
		owner->removeAboveLightBlocker(this);
	}
}

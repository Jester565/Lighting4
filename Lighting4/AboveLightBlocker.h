#pragma once
namespace lighting
{
	class LightLayer;

	class AboveLightBlocker
	{
	public:
		AboveLightBlocker(LightLayer* owner, float x, float epX1, float epX2);

		void setX(float x)
		{
			x1 = x + epX1;
			x2 = x + epX2;
		}

		float x1;
		float x2;
		float epX1;
		float epX2;

		~AboveLightBlocker();

	private:
		LightLayer* owner;
	};
}

#include "TestCore.h"
#include <iostream>

int main()
{
	TestCore tCore;
	tCore.setFPSCap(1000);
	tCore.run();
}
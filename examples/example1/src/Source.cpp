#include "TestCore.h"
#include <iostream>

int main()
{
	std::cout << "Running Example..." << std::endl;
	TestCore tCore;
	tCore.setFPSCap(1000);
	tCore.run();
}
#include "App.h"
#include <iostream>

int main()
{
	// allocation
	auto app = new App();

	// initialise and loop
	app->run("AIE", 1280, 720, false);

	// deallocation
	delete app;

	return 0;
}
#define STB_IMAGE_IMPLEMENTATION

#include "pbr/first_app.h"

//std
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main()
{
	pbr::FirstApp app{};

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;

		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
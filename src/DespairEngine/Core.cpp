#include "DespairEngine/Core.h"

namespace engine
{
	Core::Core()
	{
		DEBUG_INFO("Welcome to spdlog!");
		DEBUG_ERROR("Some error message with arg: {}", 1);

		DEBUG_WARNING("Easy padding in numbers like {:08d}", 12);
		DEBUG_CRITICAL("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
		DEBUG_INFO("Support for floats {:03.2f}", 1.23456);
		DEBUG_INFO("Positional args are {1} {0}..", "too", "supported");
		DEBUG_INFO("{:<30}", "left aligned");
	}
	
	Core::~Core()
	{

	}

	int Core::start(unsigned int width, unsigned int height, const char* title)
	{
		if (!glfwInit()) return -1;

		window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return -1;
		
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			DEBUG_CRITICAL("Failed to initialize GLAD");
			return -1;
		}

		glClearColor(1, 0, 0, 0);

		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glfwSwapBuffers(window);
			glfwPollEvents();
			update();
		}

		glfwTerminate();
		return 0;
	}
}
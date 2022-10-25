#pragma once

#include "DespairEngine/Window.h"

namespace engine
{
	Window::Window(const char* title, unsigned int width, unsigned int height) 
		: wnd_data({title, width, height})
	{
		int result = initWindow();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
	}

	Window::~Window()
	{
		closeWindow();
	}

	int Window::initWindow()
	{
		if (!glfwInit()) return -1;

		window = glfwCreateWindow(wnd_data.width, wnd_data.height, wnd_data.title, NULL, NULL);
		if (!window)
		{
			closeWindow();
			return -1;

		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			DEBUG_CRITICAL("Failed to initialize GLAD");
			return -1;
		}

		glfwSetWindowUserPointer(window, &wnd_data);

		glfwSetWindowSizeCallback(window,
			[](GLFWwindow* window, int width, int height)
			{
				WindowData& wnd_data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				wnd_data.width = width;
				wnd_data.height = height;

				EventWindowResize event(width, height);
				wnd_data.eventCallBackFn(event);
			}
		);

		glfwSetCursorPosCallback(window,
			[](GLFWwindow* window, double x, double y)
			{
				WindowData& wnd_data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				EventMouseMoved event(x, y);
				wnd_data.eventCallBackFn(event);
			}
		);

		glfwSetWindowCloseCallback(window,
			[](GLFWwindow* window)
			{
				WindowData& wnd_data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				EventWindowClose event;
				wnd_data.eventCallBackFn(event);
			}
		);
		
		return 0;
	}

	void Window::closeWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void Window::update()
	{
		glClearColor(1, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(getWidth());
		io.DisplaySize.y = static_cast<float>(getHeight());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
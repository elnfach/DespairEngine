#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "DespairEngine/Debugger.hpp"

#include "DespairEngine/modules/UIModule.hpp"
#include "DespairEngine/rendering/opengl/RendererOpenGL.hpp"

#include "Window.hpp"

namespace engine
{
	Window::Window(const char* title, unsigned int width, unsigned int height)
		: m_window_data({ title, width, height })
	{
		int result = initWindow();
	}

	Window::~Window()
	{
		closeWindow();
	}

	int Window::initWindow()
	{
		glfwSetErrorCallback([](int error_code, const char* description)
			{
				DEBUG_CRITICAL("GLFW error: {0}", description);
			}
		);

		if (!glfwInit())
		{
			DEBUG_CRITICAL("Can't initialize GLFW!");
			return -1;
		}

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
		m_ptr_window = glfwCreateWindow(m_window_data.width, m_window_data.height, m_window_data.title, NULL, NULL);
		if (!m_ptr_window)
		{
			return -2;
		}

		if (!RendererOpenGL::init(m_ptr_window))
		{
			DEBUG_CRITICAL("Failed to initialize OpenGL renderer");
			return -3;
		}

		glfwSetWindowUserPointer(m_ptr_window, &m_window_data);

		glfwSetKeyCallback(m_ptr_window,
			[](GLFWwindow* pWindow, int key, int scancode, int action, int mods)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
				switch (action)
				{
					case GLFW_PRESS:
					{
						EventKeyPressed event(static_cast<KeyCode>(key), false);
						data.eventCallBackFn(event);
						break;
					}
					case GLFW_RELEASE:
					{
						EventKeyReleased event(static_cast<KeyCode>(key));
						data.eventCallBackFn(event);
						break;
					}
					case GLFW_REPEAT:
					{
						EventKeyPressed event(static_cast<KeyCode>(key), true);
						data.eventCallBackFn(event);
						break;
					}
				}
			}
		);

		glfwSetMouseButtonCallback(m_ptr_window,
			[](GLFWwindow* pWindow, int button, int action, int mods)
			{
				WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
		double x_pos;
		double y_pos;
		glfwGetCursorPos(pWindow, &x_pos, &y_pos);
		switch (action)
		{
		case GLFW_PRESS:
		{
			EventMouseButtonPressed event(static_cast<MouseButton>(button), x_pos, y_pos);
			data.eventCallBackFn(event);
			break;
		}
		case GLFW_RELEASE:
		{
			EventMouseButtonReleased event(static_cast<MouseButton>(button), x_pos, y_pos);
			data.eventCallBackFn(event);
			break;
		}
		}
			}
		);

		glfwSetWindowSizeCallback(m_ptr_window,
			[](GLFWwindow* window, int width, int height)
			{
				WindowData& wnd_data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				wnd_data.width = width;
				wnd_data.height = height;

				EventWindowResize event(width, height);
				wnd_data.eventCallBackFn(event);
			}
		);

		glfwSetCursorPosCallback(m_ptr_window,
			[](GLFWwindow* window, double x, double y)
			{
				WindowData& wnd_data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
				

				EventMouseMoved event(x, y);
				wnd_data.eventCallBackFn(event);
			}
		);

		glfwSetWindowCloseCallback(m_ptr_window,
			[](GLFWwindow* window)
			{
				WindowData& wnd_data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

				EventWindowClose event;
				wnd_data.eventCallBackFn(event);
			}
		);

		glfwSetFramebufferSizeCallback(m_ptr_window,
			[](GLFWwindow* window, int width, int height)
			{
				RendererOpenGL::setViewport(width, height);
			}
		);

		UIModule::onWindowCreate(m_ptr_window);

		return 0;
	}

	void Window::closeWindow()
	{
		UIModule::onWindowClose();
		glfwDestroyWindow(m_ptr_window);
		glfwTerminate();
	}

	void Window::update()
	{
		glfwSwapBuffers(m_ptr_window);
		glfwPollEvents();
	}

	glm::vec2 Window::getCurrentCursorPosition() const
	{
		double x_pos;
		double y_pos;
		glfwGetCursorPos(m_ptr_window, &x_pos, &y_pos);
		return { x_pos, y_pos };
	}
}
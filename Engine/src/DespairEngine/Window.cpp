#pragma once

#include <DespairEngine/Window.hpp>

namespace engine
{
	static bool s_GLFW_initialized = false;

	GLfloat points[] = 
	{
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	GLfloat colors[] =
	{
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	const char* vertex_shader =
		"#version 460\n"
		"layout(location = 0) in vec3 vertex_position;"
		"layout(location = 1) in vec3 vertex_color;"
		"out vec3 color;"
		"void main() {"
		"   color = vertex_color;"
		"   gl_Position = vec4(vertex_position, 1.0);"
		"}";
	const char* fragment_shader =
		"#version 460\n"
		"in vec3 color;"
		"out vec4 frag_color;"
		"void main() {"
		"   frag_color = vec4(color, 1.0);"
		"}";

	std::unique_ptr<ShaderProgram> p_shader_program;
	std::unique_ptr<VertexBuffer> p_points_vbo;
	std::unique_ptr<VertexBuffer> p_colors_vbo;
	GLuint vao;

	Window::Window(const char* title, unsigned int width, unsigned int height)
		: wnd_data({ title, width, height })
	{
		int result = initWindow();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
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
			//DEBUG_CRITICAL("Failed to initialize GLAD");
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

		glfwSetFramebufferSizeCallback(window,
			[](GLFWwindow* window, int width, int height)
			{
				glViewport(0, 0, width, height);
			}
		);

		p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
		if (!p_shader_program->isCompiled())
		{
			return false;
		}

		p_points_vbo = std::make_unique<VertexBuffer>(points, sizeof(points));
		p_colors_vbo = std::make_unique<VertexBuffer>(colors, sizeof(colors));

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);
		p_points_vbo->bind();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glEnableVertexAttribArray(1);
		p_colors_vbo->bind();
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

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

		p_shader_program->bind();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize.x = static_cast<float>(getWidth());
		io.DisplaySize.y = static_cast<float>(getHeight());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
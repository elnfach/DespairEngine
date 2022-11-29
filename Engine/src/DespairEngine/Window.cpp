#include <memory>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "DespairEngine/Event.hpp"
#include "DespairEngine/rendering/opengl/ShaderProgram.hpp"
#include "DespairEngine/rendering/opengl/VertexBuffer.hpp"
#include "DespairEngine/rendering/opengl/VertexArray.hpp"
#include "DespairEngine/rendering/opengl/IndexBuffer.hpp"

#include "DespairEngine/Window.hpp"

namespace engine
{
	static bool s_GLFW_initialized = false;

	GLfloat positions_colors2[] = {
	   -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
	   -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f,
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f
	};

	GLuint indices[] = {
		0, 1, 2, 3, 2, 1
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

	std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
	std::unique_ptr<IndexBuffer> p_index_buffer;
	std::unique_ptr<VertexArray> p_vao;

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

		BufferLayout buffer_layout_1vec3
		{
			ShaderDataType::Float3
		};

		BufferLayout buffer_layout_2vec3
		{
			ShaderDataType::Float3,
			ShaderDataType::Float3
		};

		p_vao = std::make_unique<VertexArray>();
		p_positions_colors_vbo = std::make_unique<VertexBuffer>(positions_colors2, sizeof(positions_colors2), buffer_layout_2vec3);
		p_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));

		p_vao->add_vertex_buffer(*p_positions_colors_vbo);
		p_vao->set_index_buffer(*p_index_buffer);

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
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// ImGui::ShowDemoWindow();

		p_shader_program->bind();
		p_vao->bind();
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(p_vao->get_indices_count()), GL_UNSIGNED_INT, nullptr);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
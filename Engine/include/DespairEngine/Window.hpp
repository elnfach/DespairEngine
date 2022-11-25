#ifndef ENGINE_WINDOW_HPP
#define ENGINE_WINDOW_HPP

#include <DespairEngine/Event.hpp>
#include <DespairEngine/rendering/opengl/ShaderProgram.hpp>
#include <DespairEngine/rendering/opengl/VertexBuffer.hpp>
#include <DespairEngine/rendering/opengl/VertexArray.hpp>
#include <DespairEngine/rendering/opengl/IndexBuffer.hpp>

#include <glfw/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace engine
{
	class Window
	{
	public:
		using EventCallBackFn = std::function<void(BaseEvent&)>;

		Window(const char* title, unsigned int width, unsigned int height);
		~Window();

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;
		
		void update();

		const char* getTitle() const { return wnd_data.title; }
		unsigned int getWidth() const { return wnd_data.width; }
		unsigned int getHeight() const { return wnd_data.height; }
		
		void set_event_callback(const EventCallBackFn& callback) { wnd_data.eventCallBackFn = callback; }
	private:
		struct WindowData
		{
			const char* title;
			unsigned int width;
			unsigned int height;
			EventCallBackFn eventCallBackFn;
		};

		GLFWwindow* window = nullptr;
		WindowData wnd_data;

		int initWindow();
		void closeWindow();
	};
}

#endif // !ENGINE_WINDOW_HPP
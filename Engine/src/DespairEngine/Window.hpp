#ifndef ENGINE_WINDOW_HPP
#define ENGINE_WINDOW_HPP

#include <string>
#include <memory>
#include <glm/ext/vector_float2.hpp>

#include "DespairEngine/Event.hpp"

struct GLFWwindow;

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

		const char* getTitle() const { return m_window_data.title; }
		unsigned int getWidth() const { return m_window_data.width; }
		unsigned int getHeight() const { return m_window_data.height; }
		
		void set_event_callback(const EventCallBackFn& callback) { m_window_data.eventCallBackFn = callback; }
	private:
		struct WindowData
		{
			const char* title;
			unsigned int width;
			unsigned int height;
			EventCallBackFn eventCallBackFn;
		};

		GLFWwindow* m_window = nullptr;
		WindowData m_window_data;
		float m_background_color[4] = { 0.33f, 0.33f, 0.33f, 0.f };

		int initWindow();
		void closeWindow();
	};
}

#endif // !ENGINE_WINDOW_HPP
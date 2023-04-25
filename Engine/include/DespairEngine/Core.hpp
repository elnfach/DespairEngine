#ifndef ENGINE_CORE_HPP
#define ENGINE_CORE_HPP

#include <memory>

#include "DespairEngine/Event.hpp"
#include "DespairEngine/Camera.hpp"

namespace engine
{
	class Core
	{
	public:
		Core();
		virtual ~Core();

		Core(const Core&) = delete;
		Core(Core&&) = delete;
		Core& operator=(const Core&) = delete;
		Core& operator=(Core&&) = delete;

		virtual int start(const char* title, unsigned int width, unsigned int height);
		void close();

		virtual void update() = 0;

		virtual void onUIDraw() {}

		virtual void onMouseButtonEvent(const MouseButton button_code,
			const double x_pos,
			const double y_pos,
			const bool pressed) {}

		glm::vec2 getCurrentCursorPosition() const;

		float camera_position[3] = { 0.f, 0.f, 1.f };
		float camera_rotation[3] = { 0.f, 0.f, 0.f };
		float camera_fov = 60.f;
		float camera_near_plane = 0.1f;
		float camera_far_plane = 100.f;
		bool perspective_camera = true;
		Camera camera{ glm::vec3(-5.f, 0.f, 0.f) };

	private:
		void draw();
		std::unique_ptr<class Window> m_ptr_window;

		EventDispatcher m_event_dispatcher;
		bool m_window_is_closed = false;
	};
}

#endif // !ENGINE_CORE_HPP
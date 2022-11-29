#include "Event.hpp"


#ifndef ENGINE_CORE_HPP
#define ENGINE_CORE_HPP

namespace engine
{
	class Core
	{
		std::unique_ptr<class Window> window;

		EventDispatcher event_dispatcher;
		bool isClosed = false;
	public:
		Core();
		virtual ~Core();

		Core(const Core&) = delete;
		Core(Core&&) = delete;
		Core& operator=(const Core&) = delete;
		Core& operator=(Core&&) = delete;

		int start(const char* title, unsigned int width, unsigned int height);
		virtual void update() = 0;
	};
}

#endif // !ENGINE_CORE_HPP
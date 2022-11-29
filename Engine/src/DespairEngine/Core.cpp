#include "DespairEngine/Window.hpp"
#include "DespairEngine/Debugger.hpp"

#include "DespairEngine/Core.hpp"

namespace engine
{
	Core::Core()
	{
		DEBUG_INFO("Starting Application");
	}
	
	Core::~Core()
	{
		DEBUG_INFO("Closing Application");
	}

	int Core::start(const char* title, unsigned int width, unsigned int height)
	{
		window = std::make_unique<Window>(title, width, height);
		
		event_dispatcher.add_event_listener<EventMouseMoved>(
			[](EventMouseMoved& event)
			{
				// DEBUG_INFO("[Mouse Moved] Mouse moved to {0}x{1}", event.x, event.y);
			}
		);
		event_dispatcher.add_event_listener<EventWindowResize>(
			[](EventWindowResize& event)
			{
				DEBUG_INFO("[RESIZED] Changed size to {0}x{1}", event.width, event.height);
			}
		);
		event_dispatcher.add_event_listener<EventWindowClose>(
			[&](EventWindowClose& event)
			{
				DEBUG_INFO("[Window Closed]");
				isClosed = true;
			}
		);
		window->set_event_callback(
			[&](BaseEvent& event)
			{
				event_dispatcher.dispatch(event);
			}
		);
		while (!isClosed)
		{
			window->update();
			update();
		}
		window = nullptr;

		return 0;
	}
}
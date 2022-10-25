#ifndef ENGINE_EVENT_H
#define ENGINE_EVENT_H

#include <array>
#include <functional>

namespace engine
{
	enum class EventType
	{
		WINDOW_RESIZE = 0,
		WINDOW_CLOSE,

		KEY_PRESSED,
		KEY_RELEASED,

		MOUSE_BUTTON_PRESSED,
		MOUSE_BUTTON_RELEASED,
		MOUSE_MOVED,

		EVENT_COUNT

	};

	struct BaseEvent
	{
		virtual ~BaseEvent() = default;
		virtual EventType getType() const = 0;
	};

	class EventDispatcher
	{
	public:
		template<typename T>
		void add_event_listener(std::function<void(T&)> callback)
		{
			auto baseCallback = [func = std::move(callback)](BaseEvent& event)
			{
				if (event.getType() == T::type)
				{
					func(static_cast<T&>(event));
				}
			};

			eventCallbacks[static_cast<size_t>(T::type)] = std::move(baseCallback);			
		}

		void dispatch(BaseEvent& event)
		{
			auto& callback = eventCallbacks[static_cast<size_t>(event.getType())];
			if (callback)
			{
				callback(event);
			}
		}

	private:
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EVENT_COUNT)> eventCallbacks;
	};

	struct EventMouseMoved : public BaseEvent
	{
		EventMouseMoved(const double x, const double y)
			: x(x),
			y(y)
		{

		}

		virtual EventType getType() const override
		{
			return type;
		}

		double x;
		double y;

		static const EventType type = EventType::MOUSE_MOVED;
	};

	struct EventWindowResize : BaseEvent
	{
		EventWindowResize(const unsigned int width, const unsigned int height)
			: width(width),
			height(height)
		{

		}

		virtual EventType getType() const override
		{
			return type;
		}

		const unsigned int width;
		const unsigned int height;

		static const EventType type = EventType::WINDOW_RESIZE;
	};

	struct EventWindowClose : BaseEvent
	{
		virtual EventType getType() const override
		{
			return type;
		}

		static const EventType type = EventType::WINDOW_CLOSE;
	};
}

#endif // !ENGINE_EVENT_H
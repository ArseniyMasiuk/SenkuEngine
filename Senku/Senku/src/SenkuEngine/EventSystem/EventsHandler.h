#pragma once
#include "PrecompiledHeader.h"

#include "KeyCodes.h"
#include "MouseCodes.h"

#define BIT(x) (1<<x)


enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	//maybe later some app events
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory
{
	None = 0,
	EventCategoryApplication = BIT(0),
	EventCategoryInput = BIT(1),
	EventCategoryKeyboard = BIT(2),
	EventCategoryMouse = BIT(3),
	EventCategoryMouseButton = BIT(4)
};

class Event
{
public:
	virtual EventType GetEventType() const = 0;
	virtual int GetCategoryFlags() const = 0;
	virtual std::string ToString() const = 0;

	bool isInCategory(EventCategory category)
	{
		return (GetCategoryFlags() & category) > 1;
	}

	virtual ~Event() = default;

public:

	//bool Handled = false;

};

// Hazel engine by Cherno, i stole it from there )) a bit later will investigate all this big brain move, for now i dont understand all from this line

// todo: make sure that subscribed object still exist
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

using EventCallbackFn = std::function<void(Event&)>;


//class EventDispatcher
//{
//public:
//
//	// F will be deduced by the compiler
//	template<typename T, typename F>
//	static bool Dispatch(const F& func, Event& m_Event)
//	{
//		if (m_Event.GetEventType() == T::GetStaticType())
//		{
//			m_Event.Handled |= func(static_cast<T&>(m_Event));
//			return true;
//		}
//		return false;
//	}
//private:
//	
//};


struct Subscription
{
	EventCategory eventCategory;
	EventType eventType;
};

class EventsHandler
{
	static EventsHandler eventsHandler;
	std::map < int, std::vector<EventCallbackFn>> m_Subscribers;
public:
	
	static EventsHandler& GetInstance() { static EventsHandler instance; return instance; };

	void SubscribeForEvent(EventCategory eventCategory, const EventCallbackFn& callBack)
	{
		auto found = m_Subscribers.find(eventCategory);
		if (found != m_Subscribers.end())
		{
			found->second.push_back(callBack);
			return;
		}
		auto inserted = m_Subscribers.insert(std::make_pair(eventCategory, std::vector<EventCallbackFn> {callBack}));
	}

	void SubscribeForEvents(const EventCallbackFn& callBack)
	{
		// since i want to add lauers i want event handler to deliver to all layers based on their place in stack from top to bottom
		// and it will be habdled in application function
		// so for now i will add function that subscribes for all events

		// but i will keep function that allows to subscribe for specific events, maybe it will be helpfull in the future
		auto inserted = m_Subscribers.insert(std::make_pair(EventCategory::None, std::vector<EventCallbackFn> {callBack}));
	}


	void PublishEvent(Event& e)
	{

		for (auto it = m_Subscribers.begin(); it != m_Subscribers.end(); ++it)
		{
			if (it->first == EventCategory::None)
			{
				for (auto it : it->second)
					(it)(e);
			}
			
			if (it->first == e.GetCategoryFlags())
			{
				for (auto it : it->second)
					(it)(e);
			}
			//else
			//	std::cout << "There is no subscribers for event: " << e.ToString() << std::endl;
		}
	}
	
};

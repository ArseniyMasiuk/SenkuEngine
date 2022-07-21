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

};

// Hazel engine by Cherno, i stole it from there )) a bit later will investigate all this big brain move, for now i dont understand all from this line

// todo: make sure that subscribed object still exist
#define BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

using EventCallbackFn = std::function<void(Event&)>;


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

	void PublishEvent(Event& e)
	{
		auto listOfSubscribers = m_Subscribers.find(e.GetCategoryFlags());
		if (listOfSubscribers != m_Subscribers.end())
		{
			for (auto it = listOfSubscribers->second.begin(); it != listOfSubscribers->second.end(); ++it)
				(*it)(e);
		}
		//else
		//	std::cout << "There is no subscribers for event: " << e.ToString() << std::endl;
	}
	
};

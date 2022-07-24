#pragma once
#include <sstream>
#include "EventsHandler.h"

namespace Senku
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(const unsigned int width, const unsigned int height)
			:m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		virtual int GetCategoryFlags() const override { return EventCategory::EventCategoryApplication; }
		virtual EventType GetEventType() const override { return EventType::WindowResize; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		virtual int GetCategoryFlags() const override { return EventCategory::EventCategoryApplication; }
		virtual EventType GetEventType() const override { return EventType::WindowClose; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowCloseEvent";
			return ss.str();
		}
	};
}
#pragma once
#include <sstream>
#include "EventsHandler.h"
#include "MouseCodes.h"

class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(const float x, float y)
		: m_MouseX(x), m_MouseY(y) {}

	float GetX() { return m_MouseX; }
	float GetY() { return m_MouseY; }

	virtual int GetCategoryFlags() const override { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; }
	virtual EventType GetEventType() const override { return EventType::MouseMoved; }
	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
		return ss.str();
	}

private:
	float m_MouseX, m_MouseY;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(const float xOffset, const float yOffset)
	:m_xOffset(xOffset), m_yOffset(yOffset){}

	float GetOffsetX() { return m_xOffset; }
	float GetOffsetY() { return m_yOffset; }

	virtual int GetCategoryFlags() const override { return EventCategory::EventCategoryMouse | EventCategory::EventCategoryInput; }
	virtual EventType GetEventType() const override { return EventType::MouseScrolled; }
	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << m_xOffset << ", " << m_yOffset;
		return ss.str();
	}

private:
	float m_xOffset, m_yOffset;
};

class MouseButtonEvent :public Event
{
public:
	MouseCode GetMouseButton() const { return m_Button; }
	virtual int GetCategoryFlags() const override {return EventCategory::EventCategoryMouse | EventCategory::EventCategoryMouseButton | EventCategory::EventCategoryInput;}

protected:
	MouseButtonEvent(const MouseCode button) :m_Button(button) {}
	MouseCode m_Button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(const MouseCode button)
		:MouseButtonEvent(button) {}

	virtual EventType GetEventType() const override { return EventType::MouseButtonPressed; }
	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_Button;
		return ss.str();
	}
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(const MouseCode button)
		:MouseButtonEvent(button) {}

	virtual EventType GetEventType() const override { return EventType::MouseButtonReleased; }
	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_Button;
		return ss.str();
	}
};
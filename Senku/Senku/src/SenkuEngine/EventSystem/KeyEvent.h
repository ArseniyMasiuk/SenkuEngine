#pragma once
#include <sstream>
#include "EventsHandler.h"
#include "KeyCodes.h"

class KeyEvent : public Event
{
public:
	KeyboardKeyCode GetKeyCode() const { return m_KeyCode; }

	virtual int GetCategoryFlags() const override { return EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput; }
protected:
	KeyEvent(const KeyboardKeyCode keycode)
		:m_KeyCode(keycode) {}

	KeyboardKeyCode m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(KeyboardKeyCode keycode, const unsigned short repeatCount)
		:KeyEvent(keycode), m_RepeatCount(repeatCount) {}

	virtual EventType GetEventType() const override { return EventType::KeyPressed; }
	virtual std::string ToString() const override 
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
		return ss.str();
	}

private:
	unsigned short m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(const KeyboardKeyCode keycode)
		:KeyEvent(keycode) {}

	virtual EventType GetEventType() const override { return EventType::KeyReleased; }

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_KeyCode;
		return ss.str();
	}
};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(const KeyboardKeyCode keycode)
		:KeyEvent(keycode) {}

	virtual EventType GetEventType() const override { return EventType::KeyTyped; }

	virtual std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_KeyCode;
		return ss.str();
	}
};
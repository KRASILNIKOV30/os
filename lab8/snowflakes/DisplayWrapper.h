#pragma once
#include <X11/Xlib.h>
#include <iostream>

class DisplayWrapper
{
public:
	DisplayWrapper()
		: m_display(XOpenDisplay(nullptr))
	{
		if (!m_display)
		{
			throw std::runtime_error("Cannot open display");
		}
	}

	DisplayWrapper(const DisplayWrapper&) = delete;
	DisplayWrapper operator=(DisplayWrapper&) = delete;

	[[nodiscard]] Display* Get() const
	{
		return m_display;
	}

	~DisplayWrapper()
	{
		if (m_display)
		{
			XCloseDisplay(m_display);
		}
	}

private:
	Display* m_display;
};
#pragma once
#include "DisplayWrapper.h"
#include <X11/Xlib.h>

class GCWrapper
{
public:
	GCWrapper(DisplayWrapper const& display, Window const& window)
		: m_gc(XCreateGC(display.Get(), window, 0, nullptr))
		, m_display(display)
	{
	}

	GCWrapper(const GCWrapper&) = delete;
	GCWrapper& operator=(const GCWrapper&) = delete;

	[[nodiscard]] GC Get() const
	{
		return m_gc;
	}

	~GCWrapper()
	{
		XFreeGC(m_display.Get(), m_gc);
	}

private:
	GC m_gc;
	DisplayWrapper const& m_display;
};
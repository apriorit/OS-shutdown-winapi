#pragma once
#include <windows.h>
#include <stdexcept>

class EventGuard
{
public:
	EventGuard() : m_event(NULL)
	{
		m_event = CreateEvent( NULL, FALSE, FALSE, NULL );
		if (!m_event)
			throw std::runtime_error("Cannot create event");
	}

	~EventGuard()
	{
		if (m_event)
			CloseHandle(m_event);
	}

	void set()
	{
		if (m_event)
			SetEvent(m_event);
	}

	HANDLE get()
	{
		return m_event;
	}

private:
	HANDLE m_event;

};
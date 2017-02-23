#pragma once
#include <windows.h>
#include <string>
#include "FileLog.h"
#include "EventGuard.h"

class Service
{
public: 
	Service(const std::wstring& name);
	~Service();
	void Start();
	void Stop();
	static VOID WINAPI ServiceMainImpl(Service& service);
	static DWORD WINAPI ServiceWorkerThread (Service* service);
private:
	BOOL UpdateStatus(DWORD currentState, DWORD win32ExitCode, DWORD waitHint);
	static DWORD WINAPI CtrlHandler(DWORD control, DWORD eventType, LPVOID eventData, LPVOID context);
	

	std::wstring            m_serviceName;
	EventGuard                    m_stopEvent;
	SERVICE_STATUS_HANDLE    m_statusHandle;
	SERVICE_STATUS            m_status;
	DWORD                     m_checkPoint;
	FileLog					m_Log;
};
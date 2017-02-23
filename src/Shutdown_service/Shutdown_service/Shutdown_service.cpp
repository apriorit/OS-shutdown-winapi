#include "stdafx.h"
#include <windows.h>
#include "FileLog.h"
#include "Service.h"

VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv);

std::wstring g_serviceName = L"shutdownservice";

int _tmain(int argc, _TCHAR* argv[])
{
	SERVICE_TABLE_ENTRY ServiceTable[] = 
	{
		{&g_serviceName[0], (LPSERVICE_MAIN_FUNCTION) ServiceMain},
		{NULL, NULL}
	};
	if (StartServiceCtrlDispatcher (ServiceTable) == FALSE)
	{
		return GetLastError ();
	}
	return 0;
}

VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv)
{
	try
	{
		Service service(g_serviceName);
		Service::ServiceMainImpl(service);
	}
	catch (...)
	{
	}
} 


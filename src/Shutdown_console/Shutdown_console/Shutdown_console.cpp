#include "stdafx.h"
#include <windows.h>
#include "EventGuard.h"
#include "FileLog.h"
#include <iostream>
#include <conio.h>


FileLog g_log;

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_SHUTDOWN_EVENT:
		{
			g_log.WriteToLog("Computer is shutting down");
			return TRUE;
		}
	case CTRL_LOGOFF_EVENT:
		{
			g_log.WriteToLog("Current user logs off");
			return TRUE;
		}
	default:
		{
			g_log.WriteToLog("Default handler is used");
		}
	}
	return FALSE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		if (!g_log.Open("test.txt"))
			throw std::runtime_error("Cannot open log file");
		g_log.WriteToLog("---------------------------------------");

		std::cout<<"Press any key to exit"<<std::endl;

		if (!SetConsoleCtrlHandler(HandlerRoutine, TRUE))
			throw std::runtime_error("Cannot register Console control handler");

		EventGuard stopEvent;
		//simulating work:
		while (true)
		{
			if (WaitForSingleObject(stopEvent.get(), 0 ) == WAIT_OBJECT_0)
				break;
			Sleep(2000); 
			if (_kbhit())
				break;
		}
	}
	catch (const std::exception& ex)
	{
		std::cout<<ex.what();
		getchar();
	}
	return 0;
}
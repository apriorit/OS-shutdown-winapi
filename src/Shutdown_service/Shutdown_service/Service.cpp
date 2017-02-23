#include "Service.h"
#include <thread>

const int g_pendingTimeout = 10000;

Service::Service( const std::wstring& serviceName ):
	m_serviceName( serviceName ), 
	m_checkPoint(0),
	m_statusHandle(NULL)
{
	memset(&m_status, 0, sizeof(SERVICE_STATUS));    
	m_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS; 
	m_status.dwServiceSpecificExitCode = 0;   

	if (!m_Log.Open("test.txt"))
		throw std::runtime_error("Cannot open log file");
	m_Log.WriteToLog("------------------------------------------");
}

Service::~Service() 
{
	UpdateStatus (SERVICE_STOPPED, NO_ERROR, 0);
	m_Log.Close();
}

void Service::Start()
{
	m_Log.WriteToLog("Service is started");
	m_statusHandle = RegisterServiceCtrlHandlerEx(m_serviceName.c_str(), CtrlHandler, this);
	if (!m_statusHandle)
	{
		m_Log.WriteToLog("Cannot register service control handler");
		return;
	}
	UpdateStatus(SERVICE_RUNNING, NO_ERROR, g_pendingTimeout);
}

void Service::Stop()
{
	UpdateStatus(SERVICE_STOPPED, NO_ERROR, 0);
	m_stopEvent.set();
}

DWORD WINAPI Service::CtrlHandler( DWORD CtrlCode, DWORD eventType,
						 LPVOID eventData, LPVOID context)
{
	Service* service = static_cast<Service*>(context);
	if (!service)
	{
		if (CtrlCode == SERVICE_CONTROL_INTERROGATE)
			return NO_ERROR;
		else
			return ERROR_CALL_NOT_IMPLEMENTED;
	}
	
	switch (CtrlCode) 
	{
     case SERVICE_CONTROL_STOP :
		 {
			service->m_Log.WriteToLog("Service is stopping");
			service->Stop();
			return NO_ERROR;
		 }
	case SERVICE_CONTROL_POWEREVENT:
		{
			switch (eventType)
			{
			case PBT_APMSUSPEND:
				service->m_Log.WriteToLog("Computer is suspending");
				return NO_ERROR;
			case PBT_APMQUERYSUSPEND:	
				service->m_Log.WriteToLog("Computer starts suspending");
				return NO_ERROR;
			}
		}
	case SERVICE_CONTROL_SESSIONCHANGE:
		{
			switch (eventType)
			{
				case WTS_SESSION_LOGOFF:
					service->m_Log.WriteToLog("User is logging off");
					break;
				case WTS_SESSION_LOCK:
					service->m_Log.WriteToLog("User locks the session");
					break;
			}
			return NO_ERROR;
		}
	//the commented case should be used if you want to block system restart while your work is completed
	//try to uncomment this case and comment the SERVICE_CONTROL_SHUTDOWN case
	//don't forget to change the list of accepted controls in UpdateStatus method
	/*case SERVICE_CONTROL_PRESHUTDOWN:
		{
			service->m_Log.WriteToLog("Preshutdown is detected");
			Sleep(30000); //simulating long work
			return NO_ERROR;
		}*/
	case SERVICE_CONTROL_SHUTDOWN:
		{
			service->m_Log.WriteToLog("Computer is shutting down");
			return NO_ERROR;
		}
	default:
		{
			service->m_Log.WriteToLog("Got another control code");
		}
    }

	return ERROR_CALL_NOT_IMPLEMENTED;
}  

BOOL Service::UpdateStatus(DWORD currentState, DWORD win32ExitCode, DWORD waitHint)
{    

	m_status.dwCurrentState = currentState;
	m_status.dwWin32ExitCode = win32ExitCode;
	m_status.dwWaitHint = waitHint;

	if (currentState == SERVICE_START_PENDING)
	{
		m_status.dwControlsAccepted = 0;
	}
	else
	{
		m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_POWEREVENT | SERVICE_ACCEPT_SESSIONCHANGE | SERVICE_ACCEPT_SHUTDOWN; 
		
		//Don't forget to change the list of accepted controls if you want to handle preshutdown control instead of shutdown
		//m_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_POWEREVENT | SERVICE_ACCEPT_SESSIONCHANGE | SERVICE_ACCEPT_PRESHUTDOWN; 
	}

	if ( (currentState == SERVICE_RUNNING) || (currentState == SERVICE_STOPPED) )
	{
		m_status.dwCheckPoint = 0;
	}
	else 
	{
		m_status.dwCheckPoint = m_checkPoint++;
	}

	return ::SetServiceStatus( m_statusHandle, &m_status );
}

DWORD WINAPI Service::ServiceWorkerThread (Service* service)
{
	service->m_Log.WriteToLog("ServiceWorkerThread is started");
	//  Periodically check if the service has been requested to stop
	while (WaitForSingleObject(service->m_stopEvent.get(), 0) != WAIT_OBJECT_0)
	{        
		//  Simulate some work by sleeping
		Sleep(2000);
	}
	return ERROR_SUCCESS;
} 

VOID WINAPI Service::ServiceMainImpl(Service& service)
{
 	service.Start();
	std::thread workingThread (&Service::ServiceWorkerThread, &service);
	workingThread.join();
 }
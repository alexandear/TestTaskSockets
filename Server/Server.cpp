#include "stdafx.h"
#include "ServerXml.h"


const LPWSTR SERVICE_NAME  = L"ServerXml";

ServerXml server;
SERVICE_STATUS g_ServiceStatus = {0};
SERVICE_STATUS_HANDLE g_StatusHandle = nullptr;
HANDLE g_ServiceStopEvent = INVALID_HANDLE_VALUE;

VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv);
VOID WINAPI ServiceCtrlHandler (DWORD);
DWORD WINAPI ServiceWorkerThread (LPVOID lpParam);


int main() {

    SERVICE_TABLE_ENTRY ServiceTable[] =  {
        {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION) ServiceMain},
        {nullptr, nullptr}
    };

    if (StartServiceCtrlDispatcher (ServiceTable) == FALSE) {
       OutputDebugString(L"ServerXml: Main: StartServiceCtrlDispatcher returned error");
       return GetLastError ();
    }

	return 0;
}


VOID WINAPI ServiceMain (DWORD argc, LPTSTR *argv) {
    DWORD Status = E_FAIL;

    g_StatusHandle = RegisterServiceCtrlHandler (SERVICE_NAME, ServiceCtrlHandler);

    if (g_StatusHandle == nullptr) {
        OutputDebugString(L"ServerXml: ServiceMain: RegisterServiceCtrlHandler returned error");
        return;
    }

    // Tell the service controller we are starting
    ZeroMemory (&g_ServiceStatus, sizeof (g_ServiceStatus));
    g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    g_ServiceStatus.dwControlsAccepted = 0;
    g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwServiceSpecificExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;

    if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE) {
        OutputDebugString(L"My Sample Service: ServiceMain: SetServiceStatus returned error");
    }

    // Create stop event to wait on later.
    g_ServiceStopEvent = CreateEvent (nullptr, TRUE, FALSE, nullptr);
    if (g_ServiceStopEvent == nullptr) {
        OutputDebugString(L"My Sample Service: ServiceMain: CreateEvent(g_ServiceStopEvent) returned error");

        g_ServiceStatus.dwControlsAccepted = 0;
        g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        g_ServiceStatus.dwWin32ExitCode = GetLastError();
        g_ServiceStatus.dwCheckPoint = 1;

        if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE) {
		    OutputDebugString(L"ServerXml: ServiceMain: SetServiceStatus returned error");
	    }
        return;
    }    

    // Tell the service controller we are started
    g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 0;

    if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE) {
	    OutputDebugString(L"ServerXml: ServiceMain: SetServiceStatus returned error");
    }

    // Start the thread that will perform the main task of the service
    HANDLE hThread = CreateThread (nullptr, 0, ServiceWorkerThread, nullptr, 0, nullptr);

    // Wait until our worker thread exits effectively signaling that the service needs to stop
    WaitForSingleObject (hThread, INFINITE);
        
    /* 
     * Perform any cleanup tasks
     */
    CloseHandle (g_ServiceStopEvent);

    g_ServiceStatus.dwControlsAccepted = 0;
    g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    g_ServiceStatus.dwWin32ExitCode = 0;
    g_ServiceStatus.dwCheckPoint = 3;

    if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE) {
	    OutputDebugString(L"ServerXml: ServiceMain: SetServiceStatus returned error");
    }

    return;
}


VOID WINAPI ServiceCtrlHandler (DWORD CtrlCode) {

    switch (CtrlCode) {
     case SERVICE_CONTROL_STOP :

        if (g_ServiceStatus.dwCurrentState != SERVICE_RUNNING)
           break;
        
        g_ServiceStatus.dwControlsAccepted = 0;
        g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
        g_ServiceStatus.dwWin32ExitCode = 0;
        g_ServiceStatus.dwCheckPoint = 4;

        if (SetServiceStatus (g_StatusHandle, &g_ServiceStatus) == FALSE) {
			OutputDebugString(L"ServerXml: ServiceCtrlHandler: SetServiceStatus returned error");
		}

        // This will signal the worker thread to start shutting down
        SetEvent (g_ServiceStopEvent);
        break;
    }
}


DWORD WINAPI ServiceWorkerThread (LPVOID lpParam) {
    //  Periodically check if the service has been requested to stop
    while (WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0) {
        server.update();            
    }

    return ERROR_SUCCESS;
}

// TestProject_WS.cpp: определяет точку входа для консольного приложения.
//
#include "stdafx.h"
#include "TestProject_WS.h"
#include "Receiver.h"
#include "CWriter.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
LPTSTR servicePath;
LPWSTR serviceName(_T("TestProject_WS"));
SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE hStatus;

// Единственный объект приложения

CWinApp theApp;
const char PREFERED_QUEUE_TO_SERVER[26] = "message_queue_con_to_serv";
const char PREFERED_QUEUE_TO_CLIENT[26] = "message_queue_con_to_clie";


using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// инициализировать MFC, а также печать и сообщения об ошибках про сбое
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: измените код ошибки соответственно своим потребностям
			_tprintf(_T("Критическая ошибка: сбой при инициализации MFC\n"));
			nRetCode = 1;
		}
		else
		{
				servicePath = LPTSTR(argv[0]);
				CWriter Writer;
				if(argc - 1 == 0) {
					SERVICE_TABLE_ENTRY ServiceTable[1];
					ServiceTable[0].lpServiceName = serviceName;
					ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
					if(!StartServiceCtrlDispatcher(ServiceTable)) {
						Writer.AddLogMessage(_T("Error: StartServiceCtrlDispatcher"));
					}
				} else if( wcscmp(argv[argc-1], _T("install")) == 0) {
					InstallService();
					_StartService();
				} else if( wcscmp(argv[argc-1], _T("remove")) == 0) {
					RemoveService();
				} else if( wcscmp(argv[argc-1], _T("start")) == 0 ){
					_StartService();
				}
		}
	}
	else
	{
		// TODO: Измените код ошибки соответственно своим потребностям
		_tprintf(_T("Критическая ошибка: неудачное завершение GetModuleHandle\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

void ServiceMain(int argc, char** argv) {
	//Ожидание для отладки
	std::this_thread::sleep_for(std::chrono::seconds(10));
	CMessageReceiver Receiver;
	CMessageSender Sender;
	CWriter Writer;
	serviceStatus.dwServiceType    = SERVICE_WIN32_OWN_PROCESS;
	serviceStatus.dwCurrentState    = SERVICE_START_PENDING;
	serviceStatus.dwControlsAccepted  = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	serviceStatus.dwWin32ExitCode   = 0;
	serviceStatus.dwServiceSpecificExitCode = 0;
	serviceStatus.dwCheckPoint     = 0;
	serviceStatus.dwWaitHint      = 0;

	hStatus = RegisterServiceCtrlHandler(serviceName, (LPHANDLER_FUNCTION)ControlHandler);
	if (hStatus == (SERVICE_STATUS_HANDLE)0) {
		return;
	} 

	serviceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus (hStatus, &serviceStatus);
	//Запускаем принятие сообщений
	Receiver.StartListern(std::string(PREFERED_QUEUE_TO_SERVER));
	Writer.StartWriting(&Receiver);
	PSID userSid = NULL;
	LPWSTR userStringId;
	//DWORD WINAPI lastErr;
	//HRESULT result;
	HANDLE hToken;
	DWORD sessionsSize = 0;
	DWORD sessionid;
	PWTS_SESSION_INFO sessions = 0;
	USES_CONVERSION;
	while (serviceStatus.dwCurrentState == SERVICE_RUNNING)
	{
		WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &sessions, &sessionsSize);
		for (DWORD i = 0; i < sessionsSize; ++i)
		{
			WTS_SESSION_INFO sessionI = sessions[i];
			if (WTSActive == sessionI.State)
			{ 
				sessionid = sessionI.SessionId;
				break;
			}
		} 	
	
		if(sessions)
		{
			WTSFreeMemory(sessions);
		}
		WTSQueryUserToken(sessionid, &hToken);
		CWriter::GetUserSid(hToken, &userSid);
		if(hToken)
		{
			CloseHandle(hToken);
		}
		ConvertSidToStringSid(userSid, &userStringId);
		if(userSid)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)userSid);
		}
		Sender.SendMessageW(std::string(W2A(userStringId)), std::string(PREFERED_QUEUE_TO_CLIENT));
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
	//Остановим принятие сообщений
	Receiver.StopListern();
	if(Writer.m_stopWriterThread)
	{
		Writer.StopWriting();
	}
	return;
}

void ControlHandler(DWORD request) {
	CWriter Writer;
	switch(request){
	case SERVICE_CONTROL_STOP:
		Writer.AddLogMessage(_T("Stopped."));

		serviceStatus.dwWin32ExitCode = 0;
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus (hStatus, &serviceStatus);
		return;

	case SERVICE_CONTROL_SHUTDOWN:
		Writer.AddLogMessage(_T("Shutdown."));

		serviceStatus.dwWin32ExitCode = 0;
		serviceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus (hStatus, &serviceStatus);
		return;

	default:
		break;
	}

	SetServiceStatus (hStatus, &serviceStatus);

	return;
} 

int InstallService() {
  SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
  CWriter Writer;
  if(!hSCManager) 
  {
    Writer.AddLogMessage(_T("Error: Can't open Service Control Manager"));
    return -1;
  }
  
  SC_HANDLE hService = CreateService(
     hSCManager,
     serviceName,
     serviceName,
     SERVICE_ALL_ACCESS,
     SERVICE_WIN32_OWN_PROCESS,
     SERVICE_DEMAND_START,
     SERVICE_ERROR_NORMAL,
     servicePath,
     NULL, NULL, NULL, NULL, NULL
  );

  if(!hService) {
    int err = GetLastError();
    switch(err) {
      case ERROR_ACCESS_DENIED:
        Writer.AddLogMessage(_T("Error: ERROR_ACCESS_DENIED"));
        break;
      case ERROR_CIRCULAR_DEPENDENCY:
        Writer.AddLogMessage(_T("Error: ERROR_CIRCULAR_DEPENDENCY"));
        break;
      case ERROR_DUPLICATE_SERVICE_NAME:
        Writer.AddLogMessage(_T("Error: ERROR_DUPLICATE_SERVICE_NAME"));
        break;
      case ERROR_INVALID_HANDLE:
        Writer.AddLogMessage(_T("Error: ERROR_INVALID_HANDLE"));
        break;
      case ERROR_INVALID_NAME:
        Writer.AddLogMessage(_T("Error: ERROR_INVALID_NAME"));
        break;
      case ERROR_INVALID_PARAMETER:
        Writer.AddLogMessage(_T("Error: ERROR_INVALID_PARAMETER"));
        break;
      case ERROR_INVALID_SERVICE_ACCOUNT:
        Writer.AddLogMessage(_T("Error: ERROR_INVALID_SERVICE_ACCOUNT"));
        break;
      case ERROR_SERVICE_EXISTS:
        Writer.AddLogMessage(_T("Error: ERROR_SERVICE_EXISTS"));
        break;
      default:
        Writer.AddLogMessage(_T("Error: Undefined"));
    }
    CloseServiceHandle(hSCManager);
    return -1;
  }
  CloseServiceHandle(hService);
  
  CloseServiceHandle(hSCManager);
  Writer.AddLogMessage(_T("Success install service!"));
  return 0;
}

int RemoveService() {
	CWriter Writer;
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if(!hSCManager) 
	{
		Writer.AddLogMessage(_T("Error: Can't open Service Control Manager"));
		return -1;
	}
	SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_STOP | DELETE);
	if(!hService) 
	{
		Writer.AddLogMessage(_T("Error: Can't remove service"));
		CloseServiceHandle(hSCManager);
		return -1;
	}

	DeleteService(hService);
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	Writer.AddLogMessage(_T("Success remove service!"));
	return 0;
}

int _StartService() {
  SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
  CWriter Writer;
  //DWORD WINAPI LastErr;
  //HRESULT Result;
  //LastErr = GetLastError();
  SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_START);
  //LastErr = GetLastError();
  if(!StartService(hService, 0, NULL)) 
  {
	  //LastErr = GetLastError();
	  //Result = HRESULT_FROM_WIN32(GetLastError());
	  CloseServiceHandle(hSCManager);
	  Writer.AddLogMessage(_T("Error: Can't start service"));
	  return -1;
  }
  
  CloseServiceHandle(hService);
  CloseServiceHandle(hSCManager);
  return 0;
}

// TestProject_E1.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TestProject_E1.h"
#include "TestProject_E1Dlg.h"
#include "Receiver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestProject_E1App

BEGIN_MESSAGE_MAP(CTestProject_E1App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestProject_E1App construction

CTestProject_E1App::CTestProject_E1App()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTestProject_E1App object

CTestProject_E1App theApp;


// CTestProject_E1App initialization

BOOL CTestProject_E1App::InitInstance()
{
	bool AlreadyRunning;

    HANDLE hMutexOneInstance = ::CreateMutex( NULL, TRUE,
        _T("CTestProject_E1App"));

    AlreadyRunning = (GetLastError() == ERROR_ALREADY_EXISTS);

    if (hMutexOneInstance != NULL) 
    {
        ::ReleaseMutex(hMutexOneInstance);
    }
    if ( AlreadyRunning )
    { 
		MessageBox(0,_T("Already Running"),NULL,MB_OK);
		return FALSE; // заканчиваем выполнение
    } 

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CTestProject_E1Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}


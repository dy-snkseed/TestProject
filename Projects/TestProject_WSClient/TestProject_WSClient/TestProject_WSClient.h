
// TestProject_WSClient.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTestProject_WSClientApp:
// О реализации данного класса см. TestProject_WSClient.cpp
//

class CTestProject_WSClientApp : public CWinApp
{
public:
	CTestProject_WSClientApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTestProject_WSClientApp theApp;

// TestProject_Sender_E1.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTestProject_Sender_E1App:
// О реализации данного класса см. TestProject_Sender_E1.cpp
//

class CTestProject_Sender_E1App : public CWinApp
{
public:
	CTestProject_Sender_E1App();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTestProject_Sender_E1App theApp;

// TestProject_WSClient.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CTestProject_WSClientApp:
// � ���������� ������� ������ ��. TestProject_WSClient.cpp
//

class CTestProject_WSClientApp : public CWinApp
{
public:
	CTestProject_WSClientApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CTestProject_WSClientApp theApp;
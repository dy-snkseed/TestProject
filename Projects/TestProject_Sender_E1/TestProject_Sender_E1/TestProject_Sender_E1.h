
// TestProject_Sender_E1.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CTestProject_Sender_E1App:
// � ���������� ������� ������ ��. TestProject_Sender_E1.cpp
//

class CTestProject_Sender_E1App : public CWinApp
{
public:
	CTestProject_Sender_E1App();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CTestProject_Sender_E1App theApp;
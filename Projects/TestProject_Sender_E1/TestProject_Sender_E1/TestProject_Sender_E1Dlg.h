
// TestProject_Sender_E1Dlg.h : файл заголовка
//

#pragma once
#include "afxwin.h"


// диалоговое окно CTestProject_Sender_E1Dlg
class CTestProject_Sender_E1Dlg : public CDialogEx
{
// Создание
public:
	CTestProject_Sender_E1Dlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_TESTPROJECT_SENDER_E1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked_SendButton();
	CListBox SendedMessagesList;
};

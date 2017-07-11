
// TestProject_WSClientDlg.h : файл заголовка
//

#pragma once
#include "afxwin.h"


// диалоговое окно CTestProject_WSClientDlg
class CTestProject_WSClientDlg : public CDialogEx
{
// Создание
public:
	CTestProject_WSClientDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_TESTPROJECT_WSCLIENT_DIALOG };

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
	afx_msg LRESULT OnCwMessage(WPARAM wParam, LPARAM lParam);
	CEdit MessageEditBox;
	CButton StartListenButton;
	CListBox SendedMessagesList;
	CListBox ReceivedMessagesList;
	afx_msg void OnBnClicked_SendMessageButton();
	afx_msg void OnBnClicked_StartListenButton();
	afx_msg void OnBnClicked_StopListenButton();
};

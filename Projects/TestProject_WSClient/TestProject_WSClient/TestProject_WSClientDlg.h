
// TestProject_WSClientDlg.h : ���� ���������
//

#pragma once
#include "afxwin.h"


// ���������� ���� CTestProject_WSClientDlg
class CTestProject_WSClientDlg : public CDialogEx
{
// ��������
public:
	CTestProject_WSClientDlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
	enum { IDD = IDD_TESTPROJECT_WSCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// ��������� DDX/DDV


// ����������
protected:
	HICON m_hIcon;

	// ��������� ������� ����� ���������
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

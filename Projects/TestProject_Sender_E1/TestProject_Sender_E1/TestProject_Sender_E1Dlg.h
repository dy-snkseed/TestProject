
// TestProject_Sender_E1Dlg.h : ���� ���������
//

#pragma once
#include "afxwin.h"


// ���������� ���� CTestProject_Sender_E1Dlg
class CTestProject_Sender_E1Dlg : public CDialogEx
{
// ��������
public:
	CTestProject_Sender_E1Dlg(CWnd* pParent = NULL);	// ����������� �����������

// ������ ����������� ����
	enum { IDD = IDD_TESTPROJECT_SENDER_E1_DIALOG };

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
	afx_msg void OnBnClicked_SendButton();
	CListBox SendedMessagesList;
};

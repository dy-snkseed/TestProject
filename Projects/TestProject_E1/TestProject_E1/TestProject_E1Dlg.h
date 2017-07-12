
// TestProject_E1Dlg.h : header file
//

#pragma once

#include "afxwin.h"
#include "Receiver.h"
#include "CWriter.h"


// CTestProject_E1Dlg dialog
class CTestProject_E1Dlg : public CDialogEx
{
	CMessageReceiver *m_receiver;
	CWriter *m_writer;

	bool StartListen();
	bool StopListen();

// Construction
public:
	CTestProject_E1Dlg(CWnd* pParent = NULL);	// standard constructor
	~CTestProject_E1Dlg();	

// Dialog Data
	enum { IDD = IDD_TESTPROJECT_E1_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnCwMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClicked_StartListenButton();
	afx_msg void OnBnClicked_StopListenButton();
	CButton StartListenButton;
	CEdit QueueNameEdit;
	CListBox ReceivedMessagesList;
};


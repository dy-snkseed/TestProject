
// TestProject_E1Dlg.h : header file
//

#pragma once
#include "afxwin.h"


// CTestProject_E1Dlg dialog
class CTestProject_E1Dlg : public CDialogEx
{
// Construction
public:
	CTestProject_E1Dlg(CWnd* pParent = NULL);	// standard constructor

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
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit2();
	CEdit Edit2;
	CEdit Edit1;
	CListBox List1;
	afx_msg void OnBnClickedButton2();
	CButton Button1;
};



// TestProject_E1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestProject_E1.h"
#include "TestProject_E1Dlg.h"
#include "afxdialogex.h"
#include "Receiver.h"
#include "CWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestProject_E1Dlg dialog

CMessageReceiver Receiver;
CWriter Writer;

CTestProject_E1Dlg::CTestProject_E1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestProject_E1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestProject_E1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Edit1);
	DDX_Control(pDX, IDC_LIST1, List1);
	DDX_Control(pDX, IDC_BUTTON1, Button1);
}

BEGIN_MESSAGE_MAP(CTestProject_E1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT1, &CTestProject_E1Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CTestProject_E1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestProject_E1Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CTestProject_E1Dlg message handlers

BOOL CTestProject_E1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestProject_E1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestProject_E1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestProject_E1Dlg::OnEnChangeEdit1()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

     //#1015

	// TODO:  Добавьте код элемента управления
}


void CTestProject_E1Dlg::OnBnClickedButton1()
{
	//Получим название канала
	//Запустим прослушивание и запись сообщений
	Receiver.StartListern(CWriter::GetStringFromEdit(IDC_EDIT1));
	Writer.StartWriting(&Receiver, IDC_LIST1);
	//Отключим кнопку и поле ввода названия канала
	Button1.EnableWindow(false);
	Edit1.EnableWindow(false);
}


void CTestProject_E1Dlg::OnBnClickedButton2()
{
	//Остановим прослушивание и запись сообщений
	Receiver.StopListern();
	Writer.StopWriting();
	//Включим кнопку и поле ввода названия канала
	Button1.EnableWindow(true);
	Edit1.EnableWindow(true);
}

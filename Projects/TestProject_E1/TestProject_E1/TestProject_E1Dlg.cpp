
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
#ifndef CW_MESSAGE
#define CW_MESSAGE (WM_USER + 100)
#endif


// CTestProject_E1Dlg dialog



CTestProject_E1Dlg::CTestProject_E1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestProject_E1Dlg::IDD, pParent)
	, m_receiver(nullptr)
	, m_writer(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CTestProject_E1Dlg::~CTestProject_E1Dlg()
{
	StopListen();
}

void CTestProject_E1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_START_LISTEN_BUTTON, StartListenButton);
	DDX_Control(pDX, IDC_QUEUE_NAME_EDIT, QueueNameEdit);
	DDX_Control(pDX, IDC_RECEIVED_MESSAGES_LIST, ReceivedMessagesList);
}

BEGIN_MESSAGE_MAP(CTestProject_E1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(CW_MESSAGE, &CTestProject_E1Dlg::OnCwMessage)
	ON_BN_CLICKED(IDC_START_LISTEN_BUTTON, &CTestProject_E1Dlg::OnBnClicked_StartListenButton)
	ON_BN_CLICKED(IDC_STOP_LISTEN_BUTTON, &CTestProject_E1Dlg::OnBnClicked_StopListenButton)
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

bool CTestProject_E1Dlg::StartListen()
{
	if (m_receiver == nullptr && m_writer == nullptr)
	{
		m_receiver = new CMessageReceiver;
		m_writer = new CWriter;
		//Получим название канала
		//Запустим прослушивание и запись сообщений
		m_receiver->StartListern(CWriter::GetStringFromEdit(IDC_QUEUE_NAME_EDIT));
		m_writer->StartWriting(m_receiver, IDC_RECEIVED_MESSAGES_LIST);
	
		return true;
	}
	else
	{
		return false;
	}
}

bool CTestProject_E1Dlg::StopListen()
{
	//Остановим прослушивание и запись сообщений
	if (m_receiver != nullptr && m_writer != nullptr)
	{
		m_receiver->StopListern();
		m_writer->StopWriting();

		delete m_receiver;
		m_receiver = nullptr;

		delete m_writer;
		m_writer = nullptr;

		return true;
	}
	else
	{
		return false;
	}
}

void CTestProject_E1Dlg::OnBnClicked_StartListenButton()
{
	bool result = StartListen();
	if (result)
	{
		//Отключим кнопку и поле ввода названия канала
		StartListenButton.EnableWindow(false);
		QueueNameEdit.EnableWindow(false);
	}
}

void CTestProject_E1Dlg::OnBnClicked_StopListenButton()
{
	bool result = StopListen();
	if (result)
	{
		//Включим кнопку и поле ввода названия канала
		StartListenButton.EnableWindow(true);
		QueueNameEdit.EnableWindow(true);
	}
}

LRESULT CTestProject_E1Dlg::OnCwMessage(WPARAM wParam, LPARAM receivedMessage)
{
	UNREFERENCED_PARAMETER(receivedMessage);
	LPTSTR reinterpredMessage = reinterpret_cast<LPTSTR>(receivedMessage);
	// Handle message here.
	ReceivedMessagesList.AddString((LPCTSTR)reinterpredMessage);
	delete[] reinterpredMessage;
	return 0;
}
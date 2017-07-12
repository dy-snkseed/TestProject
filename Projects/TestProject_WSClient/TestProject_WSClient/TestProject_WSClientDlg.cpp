
// TestProject_WSClientDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "TestProject_WSClient.h"
#include "TestProject_WSClientDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#ifndef CW_MESSAGE
#define CW_MESSAGE (WM_USER + 100)
#endif


// диалоговое окно CTestProject_WSClientDlg
const char PREFERED_QUEUE_TO_SERVER[26] = "message_queue_con_to_serv";
const char PREFERED_QUEUE_TO_CLIENT[26] = "message_queue_con_to_clie";


CTestProject_WSClientDlg::CTestProject_WSClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestProject_WSClientDlg::IDD, pParent)
	, m_receiver(nullptr)
	, m_writer(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CTestProject_WSClientDlg::~CTestProject_WSClientDlg()
{
	StopListen();
}

void CTestProject_WSClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE_TEXT_EDIT, MessageEditBox);
	DDX_Control(pDX, IDC_START_LISTEN_BUTTON, StartListenButton);
	DDX_Control(pDX, IDC_SENDED_MESSAGES_LIST, SendedMessagesList);
	DDX_Control(pDX, IDC_RECEIVED_MESSAGES_LIST, ReceivedMessagesList);
}

BEGIN_MESSAGE_MAP(CTestProject_WSClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(CW_MESSAGE, &CTestProject_WSClientDlg::OnCwMessage)
	ON_BN_CLICKED(IDC_SEND_MESSAGE_BUTTON, &CTestProject_WSClientDlg::OnBnClicked_SendMessageButton)
	ON_BN_CLICKED(IDC_START_LISTEN_BUTTON, &CTestProject_WSClientDlg::OnBnClicked_StartListenButton)
	ON_BN_CLICKED(IDC_STOP_LISTEN_BUTTON, &CTestProject_WSClientDlg::OnBnClicked_StopListenButton)
END_MESSAGE_MAP()


// обработчики сообщений CTestProject_WSClientDlg


BOOL CTestProject_WSClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTestProject_WSClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTestProject_WSClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CTestProject_WSClientDlg::StartListen()
{
	if (m_receiver == nullptr && m_writer == nullptr)
	{
		m_receiver = new CMessageReceiver;
		m_writer = new CWriter;
		//Получим название канала
		//Запустим прослушивание и запись сообщений
		m_receiver->StartListern(std::string(PREFERED_QUEUE_TO_CLIENT));
		m_writer->StartWriting(m_receiver, IDC_RECEIVED_MESSAGES_LIST);
	
		return true;
	}
	else
	{
		return false;
	}
}

bool CTestProject_WSClientDlg::StopListen()
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

void CTestProject_WSClientDlg::OnBnClicked_SendMessageButton()
{
	USES_CONVERSION;
	CMessageSender Sender;
	HANDLE hUserToken = NULL;
	PSID userSid = NULL;
	LPWSTR userStringId;
	OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hUserToken );
	CWriter::GetUserSid(hUserToken, &userSid);
	ConvertSidToStringSid(userSid, &userStringId);
	HeapFree(GetProcessHeap(), 0, (LPVOID)userSid);
	Sender.SendMessageW((CWriter::GetStringFromEdit(IDC_MESSAGE_TEXT_EDIT)+std::string(" from ")+std::string(W2A(userStringId))), std::string(PREFERED_QUEUE_TO_SERVER));
	//Запишем лог
	SendedMessagesList.AddString(CWriter::GetCStringTime() + ' ' + CWriter::GetStringFromEdit(IDC_MESSAGE_TEXT_EDIT).c_str());
}


void CTestProject_WSClientDlg::OnBnClicked_StartListenButton()
{
	bool result = StartListen();
	if (result)
	{
		//Отключим кнопку
		StartListenButton.EnableWindow(false);
	}
}


void CTestProject_WSClientDlg::OnBnClicked_StopListenButton()
{
	bool result = StopListen();
	if (result)
	{
		//Включим кнопку
		StartListenButton.EnableWindow(true);
	}
}
LRESULT CTestProject_WSClientDlg::OnCwMessage(WPARAM wParam, LPARAM receivedMessage)
{
	UNREFERENCED_PARAMETER(receivedMessage);
	LPTSTR reinterpredMessage = reinterpret_cast<LPTSTR>(receivedMessage);
	// Handle message here.
	ReceivedMessagesList.AddString((LPCTSTR)reinterpredMessage);
	delete[] reinterpredMessage;
	return 0;
}
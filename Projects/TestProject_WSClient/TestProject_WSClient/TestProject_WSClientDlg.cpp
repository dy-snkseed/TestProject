
// TestProject_WSClientDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "TestProject_WSClient.h"
#include "TestProject_WSClientDlg.h"
#include "afxdialogex.h"
#include "Receiver.h"
#include "CWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// диалоговое окно CTestProject_WSClientDlg
const char PREFERED_QUEUE_TO_SERVER[26] = "message_queue_con_to_serv";
const char PREFERED_QUEUE_TO_CLIENT[26] = "message_queue_con_to_clie";


CTestProject_WSClientDlg::CTestProject_WSClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestProject_WSClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestProject_WSClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, MessageEditBox);
	DDX_Control(pDX, IDC_BUTTON2, StartListenButton);
	DDX_Control(pDX, IDC_LIST1, SendedMessagesList);
	DDX_Control(pDX, IDC_LIST2, ReceivedMessagesList);
}

BEGIN_MESSAGE_MAP(CTestProject_WSClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestProject_WSClientDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CTestProject_WSClientDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestProject_WSClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestProject_WSClientDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// обработчики сообщений CTestProject_WSClientDlg
CMessageReceiver *Receiver;
CWriter *Writer;

BOOL CTestProject_WSClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	Receiver = nullptr;
	Writer = nullptr;
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



void CTestProject_WSClientDlg::OnBnClickedButton1()
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
	Sender.SendMessageW((CWriter::GetStringFromEdit(IDC_EDIT1)+std::string(" from ")+std::string(W2A(userStringId))), std::string(PREFERED_QUEUE_TO_SERVER));
	//Запишем лог
	SendedMessagesList.AddString(CWriter::GetCStringTime() + ' ' + CWriter::GetStringFromEdit(IDC_EDIT1).c_str());
}


void CTestProject_WSClientDlg::OnEnChangeEdit1()
{

}


void CTestProject_WSClientDlg::OnBnClickedButton2()
{
	Receiver = new CMessageReceiver;
	Writer = new CWriter;
	//Получим название канала
	//Запустим прослушивание и запись сообщений
	Receiver->StartListern(std::string(PREFERED_QUEUE_TO_CLIENT));
	Writer->StartWriting(Receiver, IDC_LIST2);
	//Отключим кнопку и поле ввода названия канала
	StartListenButton.EnableWindow(false);
}


void CTestProject_WSClientDlg::OnBnClickedButton3()
{
	if(Receiver&&Writer)
	{
		//Остановим прослушивание и запись сообщений
		Receiver->StopListern();
		Writer->StopWriting();
		delete Receiver;
		Receiver = nullptr;
		delete Writer;
		Writer = nullptr;
		//Включим кнопку и поле ввода названия канала
		StartListenButton.EnableWindow(true);
	}
}

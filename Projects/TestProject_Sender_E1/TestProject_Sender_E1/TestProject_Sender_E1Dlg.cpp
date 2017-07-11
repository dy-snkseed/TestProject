
// TestProject_Sender_E1Dlg.cpp : файл реализации
//

#include "stdafx.h"
#include "TestProject_Sender_E1.h"
#include "TestProject_Sender_E1Dlg.h"
#include "afxdialogex.h"
#include "Receiver.h"
#include "CWriter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// диалоговое окно CTestProject_Sender_E1Dlg



CTestProject_Sender_E1Dlg::CTestProject_Sender_E1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestProject_Sender_E1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestProject_Sender_E1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SENDED_MESSAGES_LIST, SendedMessagesList);
}

BEGIN_MESSAGE_MAP(CTestProject_Sender_E1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND_BUTTON, &CTestProject_Sender_E1Dlg::OnBnClicked_SendButton)
END_MESSAGE_MAP()


// обработчики сообщений CTestProject_Sender_E1Dlg

BOOL CTestProject_Sender_E1Dlg::OnInitDialog()
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

void CTestProject_Sender_E1Dlg::OnPaint()
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
HCURSOR CTestProject_Sender_E1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestProject_Sender_E1Dlg::OnBnClicked_SendButton()
{
	CMessageSender Sender;
	Sender.SendMessageW(CWriter::GetStringFromEdit(IDC_MESSAGE_TEXT_EDIT), CWriter::GetStringFromEdit(IDC_QUEUE_NAME_EDIT));
	//Запишем лог
	SendedMessagesList.AddString(CWriter::GetCStringTime() + ' ' + CWriter::GetStringFromEdit(IDC_MESSAGE_TEXT_EDIT).c_str());
}

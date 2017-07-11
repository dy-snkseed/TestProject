
// TestProject_WSClientDlg.cpp : ���� ����������
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


// ���������� ���� CTestProject_WSClientDlg
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


// ����������� ��������� CTestProject_WSClientDlg
CMessageReceiver *Receiver;
CWriter *Writer;

BOOL CTestProject_WSClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ������ ������ ��� ����� ����������� ����. ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������

	// TODO: �������� �������������� �������������
	Receiver = nullptr;
	Writer = nullptr;
	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������. ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.

void CTestProject_WSClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �������� ���������� ��� ���������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ������������ ������ �� ������ ����������� ��������������
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ��������� ������
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ������� �������� ��� ������� ��� ��������� ����������� ������� ��� �����������
//  ���������� ����.
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
	//������� ���
	SendedMessagesList.AddString(CWriter::GetCStringTime() + ' ' + CWriter::GetStringFromEdit(IDC_EDIT1).c_str());
}


void CTestProject_WSClientDlg::OnEnChangeEdit1()
{

}


void CTestProject_WSClientDlg::OnBnClickedButton2()
{
	Receiver = new CMessageReceiver;
	Writer = new CWriter;
	//������� �������� ������
	//�������� ������������� � ������ ���������
	Receiver->StartListern(std::string(PREFERED_QUEUE_TO_CLIENT));
	Writer->StartWriting(Receiver, IDC_LIST2);
	//�������� ������ � ���� ����� �������� ������
	StartListenButton.EnableWindow(false);
}


void CTestProject_WSClientDlg::OnBnClickedButton3()
{
	if(Receiver&&Writer)
	{
		//��������� ������������� � ������ ���������
		Receiver->StopListern();
		Writer->StopWriting();
		delete Receiver;
		Receiver = nullptr;
		delete Writer;
		Writer = nullptr;
		//������� ������ � ���� ����� �������� ������
		StartListenButton.EnableWindow(true);
	}
}

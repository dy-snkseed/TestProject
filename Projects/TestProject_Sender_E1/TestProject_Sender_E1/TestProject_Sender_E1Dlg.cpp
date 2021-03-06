
// TestProject_Sender_E1Dlg.cpp : ���� ����������
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


// ���������� ���� CTestProject_Sender_E1Dlg



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


// ����������� ��������� CTestProject_Sender_E1Dlg

BOOL CTestProject_Sender_E1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ������ ������ ��� ����� ����������� ����. ����� ������ ��� �������������,
	//  ���� ������� ���� ���������� �� �������� ����������
	SetIcon(m_hIcon, TRUE);			// ������� ������
	SetIcon(m_hIcon, FALSE);		// ������ ������

	// TODO: �������� �������������� �������������

	return TRUE;  // ������� �������� TRUE, ���� ����� �� ������� �������� ����������
}

// ��� ���������� ������ ����������� � ���������� ���� ����� ��������������� ����������� ���� �����,
//  ����� ���������� ������. ��� ���������� MFC, ������������ ������ ���������� ��� �������������,
//  ��� ������������� ����������� ������� ��������.

void CTestProject_Sender_E1Dlg::OnPaint()
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
HCURSOR CTestProject_Sender_E1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestProject_Sender_E1Dlg::OnBnClicked_SendButton()
{
	CMessageSender Sender;
	Sender.SendMessageW(CWriter::GetStringFromEdit(IDC_MESSAGE_TEXT_EDIT), CWriter::GetStringFromEdit(IDC_QUEUE_NAME_EDIT));
	//������� ���
	SendedMessagesList.AddString(CWriter::GetCStringTime() + ' ' + CWriter::GetStringFromEdit(IDC_MESSAGE_TEXT_EDIT).c_str());
}

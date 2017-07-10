#pragma once
#include "Receiver.h"
#include "stdafx.h"

class CWriter
{
public:
	//�����������
	CWriter();
	//����������
	~CWriter();
	//������ ���� ��� ������
	int AddLogMessage(TCHAR* message);
	//��������� �������� ������� � ������� CString
	static CString GetCStringTime();
	//��������� ������ �� Edit`a
	static std::string GetStringFromEdit(int idc_editbox);
	//����� ������ � ListBox
	static void CWriteListThread(CMessageReceiver *recevier, int idc_list, CWriter *parent);
	//����� ������ � ���� ��� ������
	static void CWriteFileThread(CMessageReceiver *recevier, CWriter *parent);
	//���������� ������� ������ ������ � ListBox
	bool StartWriting(CMessageReceiver *recevier, int idc_list);
	//���������� ������� ������ ������ � ����
	bool StartWriting(CMessageReceiver *recevier);
	//��������� ������� ������
	bool StopWriting();
	//���� �������� ������������� ����� ����
	bool m_isLogExists;
	//���� ��������� ������
	bool m_stopWriterThread;
	//������� ��������� User SID
	static bool GetUserSid(HANDLE userToken, PSID *userSid);
	//��� ����� ����
	CString m_logFileName;
	//��� ����� ���������
	CString m_messageFileName;
private:
	//��������� �� ���� ����
	FILE *m_logFile;
	//��������� �� ���� ���������
	FILE *m_messagesFile;
	//��������� �� �����
	boost::thread * m_writerThread;
};
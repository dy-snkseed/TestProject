#pragma once

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <boost/thread/thread.hpp>
#include "boost/lexical_cast.hpp"
#include <iostream>
#include <vector>

using namespace boost::interprocess;




typedef std::list<std::string> N_CStringList;

class CMessageSender
{
public:
	CMessageSender(void);
	~CMessageSender(void);
	//	�������� ���������
	bool SendMessage(std::string Message, std::string message_queue_name);

};

//////////////////////////////////////////////////////////////////////////
//
//	����� ������� � ������ ��������� ���������
//

class CMessageReceiver
{
public:
	CMessageReceiver(void);
	~CMessageReceiver(void);

	
	// ������� ������ ������ �� �������
	static void TreadReadFromQueue(CMessageReceiver* receiver, std::string message_queue_name);


	// ������� ������ ������� � ��������� �� �������

	// ������ ������
	bool StartListern(std::string message_queue_name);

	// ��������� ������
	bool StopListern();

	// �������� ������� ������

	// �������� ������ �������
	unsigned GetQueueSize();
	//	������� ���������
	std::string	PopMessage();
	// �������� ���������
	bool PushMessage(char* message);

private:
	// ����� ������ ��������� �� �������.
	boost::thread * m_IPCWorktThread;
	// ������ ������ ��������� �� ������
	boost::thread * m_SocketWorkThread;
	//	 ���������� ������ ����������� ���������.
	N_CStringList		m_MessageList;
	// ����������� ������ ��� ������� � ������
	boost::mutex    m_MessageMutex;

	//	���� ��������� ������
	bool	m_bStopReadFromQueue;

	//////////////////////////////////////////////////////////////////////////
	// � � � � � � � � � � � �
public:
	//	�������� ������������ 
	bool MakeSelfTest();
private:
	// �������� ����� �������� ������������ �� ������/������
	bool TestMultyThreadIOTest();
	// �������� ������������ ������/������ �/�� �������
	bool TestMessageQueueIOTest();


};



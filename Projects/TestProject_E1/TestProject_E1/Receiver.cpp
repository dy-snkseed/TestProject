#include "stdafx.h"
#include "Receiver.h"
#define _SCL_SECURE_NO_WARNINGS
// ������������ ������ ��������� 4kb
size_t const max_msg_size = 0x1000;
// ������������ ������ ������� 255 ���������
size_t const max_msg_count = 0x100;
// ��� ����� 10
//size_t const max_msg_count = 10;
//	�������� �������
char const	 PREFERED_QUEUE_TO_SERVER[26] = "message_queue_con_to_serv";
char const	 PREFERED_QUEUE_TO_CLIENT[26] = "message_queue_con_to_clie";



/************************************************************************/
//	�����������
/************************************************************************/
CMessageSender::CMessageSender(void)
{

}

/************************************************************************/
//	����������
/************************************************************************/
CMessageSender::~CMessageSender(void)
{

}

/************************************************************************/
//	�������� ���������
/************************************************************************/
bool CMessageSender::SendMessage(std::string Message, std::string message_queue_name)
{
	try
	{
		//Create a message_queue.
		boost::interprocess::message_queue mq_o(open_only,message_queue_name.c_str());
		//	�������� ������ �������
		if (mq_o.get_num_msg()<(max_msg_count-1))
		{
			// ������
			//std::string str = Message;
			mq_o.send(Message.data(), Message.size(), 0);
			// 
			return true;
		}
		else
		{
			// ������� �����������
			boost::this_thread::sleep( boost::posix_time::seconds(5) );
			//	�������� ������ �������
			if (mq_o.get_num_msg()<(max_msg_count-1))
			{
				// ������
				//std::string str = Message;
				mq_o.send(Message.data(), Message.size(), 0);
				// 
				return true;
			}
		}
	}
	catch(...)
	{
		return false;
	}
	return false;
}

/************************************************************************/
//	�����������
/************************************************************************/
CMessageReceiver::CMessageReceiver(void)
{
	//ThreadMutex = new boost::mutex;
	m_IPCWorktThread = NULL;
}

/************************************************************************/
//	����������
/************************************************************************/
CMessageReceiver::~CMessageReceiver(void)
{
}

/************************************************************************/
// ������� ������ ������ �� �������
/************************************************************************/
void CMessageReceiver::TreadReadFromQueue(CMessageReceiver* receiver, std::string message_queue_name)
{
	// ������ ����������� ���������
	size_t msg_size;
	//	���� ���������� ���������
	unsigned msg_prio;
	//	������� ������� �������
	try
	{
		//Erase previous message queue
		boost::interprocess::message_queue::remove(message_queue_name.c_str());
		//	�������� ����� �������.
		boost::interprocess::permissions perm; 
		//	������ ���� � ����
		perm.set_unrestricted(); 
		//Open a message queue.
		message_queue mq(create_only,message_queue_name.c_str(),max_msg_count,max_msg_size,perm);
		//	���� � ��� ��� ���������� ������������
		while (receiver->m_bStopReadFromQueue==false)
		{
			//	���� ���� ���������
			if (mq.get_num_msg()>0)
			{
				//	������
				std::string s;
				//	 �������� ������������ ������
				s.resize(max_msg_size);
				//	������ ���������
				mq.receive(&s[0], s.size(), msg_size, msg_prio);
				//	��������� ������
				s.resize(msg_size);
				//	������� ��� � ������
				receiver->PushMessage((char*)s.c_str());
			}
			else
			{
				// ���� 1/10 �������
				boost::this_thread::sleep( boost::posix_time::milliseconds(100) );
			}
		}
	}
	catch (...)
	{
	}
}

/************************************************************************/
// ������ ������
/************************************************************************/
bool CMessageReceiver::StartListern(std::string message_queue_name)
{
	try
	{
		if (m_IPCWorktThread)
		{
			// �������� ����
			m_bStopReadFromQueue = true;
			//	�������� �������� ������
			m_IPCWorktThread->join();
			//	������.
			delete m_IPCWorktThread;
			//	�������.
			m_IPCWorktThread = NULL;
		}
		//	��������� ���� ������
		m_bStopReadFromQueue = false;
		// �������� ������ ������ ������ �� �������
		m_IPCWorktThread = new boost::thread( CMessageReceiver::TreadReadFromQueue,this,message_queue_name);
	}
	catch(boost::interprocess::interprocess_exception &ex)
	{
		std::string reason = ex.what();
	}

	//	���������.
	return true;
}


/************************************************************************/
// ��������� ������
/************************************************************************/
bool CMessageReceiver::StopListern()
{
	// �������� ����
	m_bStopReadFromQueue = true;
	//	�������� �������� ������
	m_IPCWorktThread->join();
	//	������.
	delete m_IPCWorktThread;
	// �������.
	m_IPCWorktThread = NULL;


	// ���������
	return true;
}

/************************************************************************/
// �������� ������ �������
/************************************************************************/
unsigned CMessageReceiver::GetQueueSize()
{	
	// ����������� �� ������ �� �������
	boost::unique_lock<boost::mutex> scoped_lock(m_MessageMutex);
	//	������� ������
	unsigned size = m_MessageList.size();
	//	������ 
	return size;
}

/************************************************************************/
//	������� ���������
/************************************************************************/
std::string	CMessageReceiver::PopMessage()
{
	// ����������� �� ������ �� �������
	boost::unique_lock<boost::mutex> scoped_lock(m_MessageMutex);
	//	������
	std::string  result;
	//	������� ������
	unsigned size = m_MessageList.size();
	//	���� ���� ������.
	if (size > 0)
	{
		// �������� ������
		result = *m_MessageList.begin();
		// ������ ������ ������.
		m_MessageList.erase(m_MessageList.begin());
	}
	//	������ ���������
	return result;
}

/************************************************************************/
//	�������� ���������
/************************************************************************/
bool CMessageReceiver::PushMessage(char* message)
{
	// ����������� �� ������ �� �������
	boost::unique_lock<boost::mutex> scoped_lock(m_MessageMutex);
	//	������
	std::string  result = message;
	//	������� ������ � ����� ������
	m_MessageList.push_back(result);
	// ������ ���������
	return true;
}


//////////////////////////////////////////////////////////////////////////
// � � � � � � � � � � � �
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
//	�������� ������������ 
/************************************************************************/
bool CMessageReceiver::MakeSelfTest()
{
	if (TestMultyThreadIOTest()==false)
	{
		return false;
	}
	if (TestMessageQueueIOTest()==false)
	{
		return false;
	}


	return true;
}

/************************************************************************/
// �������� ������������ ������/������ �/�� �������
/************************************************************************/
bool CMessageReceiver::TestMessageQueueIOTest()
{
	//	�������� ����� ������.
	StartListern(std::string(PREFERED_QUEUE_TO_CLIENT));
	//	���� 1 ���.
	boost::this_thread::sleep( boost::posix_time::seconds(1) );
	//Open the message_queue.
	boost::interprocess::message_queue mq_o(open_only,PREFERED_QUEUE_TO_CLIENT);
	
	for (int i= 0x00;i< 10; i++)
	{
		std::string s;
		s+= "string";
		s+= boost::lexical_cast<std::string>(i);
		s+= "\r\n";
		mq_o.send(s.data(), s.size(), 0);
	}
	//	���� 1 ���.
	boost::this_thread::sleep( boost::posix_time::seconds(1) );
	// �������� - ������� �����
	if (mq_o.get_num_msg()==0x00)
	{
		if (m_MessageList.size()==10)
		{
			// ������� �������.
			m_MessageList.clear();
			// �� �����
			return true;
		}
	}
	// ������� �������.
	m_MessageList.clear();
	// �� �����
	return false;
}

/************************************************************************/
// ����� ������ ������
/************************************************************************/
void WriteThreadMessages(CMessageReceiver* rec)
{
	for (unsigned i = 0x00;i<100;i++)
	{
		rec->PushMessage("string");
		//boost::this_thread::sleep( boost::posix_time::milliseconds(1) );
	}
}

/************************************************************************/
// ����� ������ ������
/************************************************************************/
void ReadThreadMessages(CMessageReceiver* rec)
{
	int count = 0;
	for (;;)
	{
		if (rec->GetQueueSize()>0)
		{
			std::string str = rec->PopMessage();
			if (str == "string")
			{
				count++;
			}
		}
		// ������ 50 ���������
		if (count==50)
		{
			break;
		}
		//boost::this_thread::sleep( boost::posix_time::milliseconds(2) );
	}
}

/************************************************************************/
// ������� ����� �������� ������������ �� ������/������
/************************************************************************/
bool CMessageReceiver::TestMultyThreadIOTest()
{
	bool result = false;

	boost::thread wthread1(WriteThreadMessages,this);
	boost::thread wthread2(WriteThreadMessages,this);
	boost::thread rthread1(ReadThreadMessages,this);
	boost::thread rthread2(ReadThreadMessages,this);
	boost::thread rthread3(ReadThreadMessages,this);

	// ��� ������ �� ������
	wthread1.join();
	wthread2.join();
	// ��� �� ������
	rthread1.join();
	rthread2.join();
	rthread3.join();
	
	//
	//	��������
	if (GetQueueSize()==50)
	{
		result = true;
	}

	//	������� �������
	m_MessageList.clear();
	//	������ ���������.
	return result;
}

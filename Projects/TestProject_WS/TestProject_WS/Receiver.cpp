#include "stdafx.h"
#include "Receiver.h"
#define _SCL_SECURE_NO_WARNINGS
// максимальный размер сообщения 4kb
size_t const max_msg_size = 0x1000;
// максимальный размер очереди 255 элементов
size_t const max_msg_count = 0x100;
// для теста 10
//size_t const max_msg_count = 10;
//	название очереди
char const	 PREFERED_QUEUE_TO_SERVER[26] = "message_queue_con_to_serv";
char const	 PREFERED_QUEUE_TO_CLIENT[26] = "message_queue_con_to_clie";



/************************************************************************/
//	конструктор
/************************************************************************/
CMessageSender::CMessageSender(void)
{

}

/************************************************************************/
//	деструктор
/************************************************************************/
CMessageSender::~CMessageSender(void)
{

}

/************************************************************************/
//	отослать сообщение
/************************************************************************/
bool CMessageSender::SendMessage(std::string Message, std::string message_queue_name)
{
	try
	{
		//Create a message_queue.
		boost::interprocess::message_queue mq_o(open_only,message_queue_name.c_str());
		//	проверим размер очереди
		if (mq_o.get_num_msg()<(max_msg_count-1))
		{
			// строка
			//std::string str = Message;
			mq_o.send(Message.data(), Message.size(), 0);
			// 
			return true;
		}
		else
		{
			// очередь переполнена
			boost::this_thread::sleep( boost::posix_time::seconds(5) );
			//	проверим размер очереди
			if (mq_o.get_num_msg()<(max_msg_count-1))
			{
				// строка
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
//	конструктор
/************************************************************************/
CMessageReceiver::CMessageReceiver(void)
{
	//ThreadMutex = new boost::mutex;
	m_IPCWorktThread = NULL;
}

/************************************************************************/
//	деструктор
/************************************************************************/
CMessageReceiver::~CMessageReceiver(void)
{
}

/************************************************************************/
// функция потока чтения из очереди
/************************************************************************/
void CMessageReceiver::TreadReadFromQueue(CMessageReceiver* receiver, std::string message_queue_name)
{
	// размер полученного сообщения
	size_t msg_size;
	//	типа предыдущее сообщение
	unsigned msg_prio;
	//	пробуем создать очередь
	try
	{
		//Erase previous message queue
		boost::interprocess::message_queue::remove(message_queue_name.c_str());
		//	создадим права доступа.
		boost::interprocess::permissions perm; 
		//	доступ есть у всех
		perm.set_unrestricted(); 
		//Open a message queue.
		message_queue mq(create_only,message_queue_name.c_str(),max_msg_count,max_msg_size,perm);
		//	пока у нас нет надобности остановиться
		while (receiver->m_bStopReadFromQueue==false)
		{
			//	если есть сообщения
			if (mq.get_num_msg()>0)
			{
				//	строка
				std::string s;
				//	 выставим максимальный размер
				s.resize(max_msg_size);
				//	читаем сообщение
				mq.receive(&s[0], s.size(), msg_size, msg_prio);
				//	выровняем размер
				s.resize(msg_size);
				//	добавим его в список
				receiver->PushMessage((char*)s.c_str());
			}
			else
			{
				// ждем 1/10 секунды
				boost::this_thread::sleep( boost::posix_time::milliseconds(100) );
			}
		}
	}
	catch (...)
	{
	}
}

/************************************************************************/
// запуск потока
/************************************************************************/
bool CMessageReceiver::StartListern(std::string message_queue_name)
{
	try
	{
		if (m_IPCWorktThread)
		{
			// выставим флаг
			m_bStopReadFromQueue = true;
			//	дождемся закрытия потока
			m_IPCWorktThread->join();
			//	удалим.
			delete m_IPCWorktThread;
			//	обнулим.
			m_IPCWorktThread = NULL;
		}
		//	установим флаг работы
		m_bStopReadFromQueue = false;
		// создадим объект потока чтения из очереди
		m_IPCWorktThread = new boost::thread( CMessageReceiver::TreadReadFromQueue,this,message_queue_name);
	}
	catch(boost::interprocess::interprocess_exception &ex)
	{
		std::string reason = ex.what();
	}

	//	результат.
	return true;
}


/************************************************************************/
// остановка потока
/************************************************************************/
bool CMessageReceiver::StopListern()
{
	// выставим флаг
	m_bStopReadFromQueue = true;
	//	дождемся закрытия потока
	m_IPCWorktThread->join();
	//	удалим.
	delete m_IPCWorktThread;
	// обнулим.
	m_IPCWorktThread = NULL;


	// результат
	return true;
}

/************************************************************************/
// получить размер очереди
/************************************************************************/
unsigned CMessageReceiver::GetQueueSize()
{	
	// блокируемся до выхода из функции
	boost::unique_lock<boost::mutex> scoped_lock(m_MessageMutex);
	//	получим размер
	unsigned size = m_MessageList.size();
	//	вернем 
	return size;
}

/************************************************************************/
//	получим сообщение
/************************************************************************/
std::string	CMessageReceiver::PopMessage()
{
	// блокируемся до выхода из функции
	boost::unique_lock<boost::mutex> scoped_lock(m_MessageMutex);
	//	строка
	std::string  result;
	//	получим размер
	unsigned size = m_MessageList.size();
	//	если есть данные.
	if (size > 0)
	{
		// запомним строку
		result = *m_MessageList.begin();
		// удалим первую строку.
		m_MessageList.erase(m_MessageList.begin());
	}
	//	веренм результат
	return result;
}

/************************************************************************/
//	поместим сообщение
/************************************************************************/
bool CMessageReceiver::PushMessage(char* message)
{
	// блокируемся до выхода из функции
	boost::unique_lock<boost::mutex> scoped_lock(m_MessageMutex);
	//	строка
	std::string  result = message;
	//	добавим строку в хвост списка
	m_MessageList.push_back(result);
	// веренм результат
	return true;
}


//////////////////////////////////////////////////////////////////////////
// Т е с т и р о в а н и е
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
//	выполним тестирование 
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
// выполним тестирование чтения/записи в/из очереди
/************************************************************************/
bool CMessageReceiver::TestMessageQueueIOTest()
{
	//	запустим поток чтения.
	StartListern(std::string(PREFERED_QUEUE_TO_CLIENT));
	//	ждем 1 сек.
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
	//	ждем 1 сек.
	boost::this_thread::sleep( boost::posix_time::seconds(1) );
	// проверим - очередь пуста
	if (mq_o.get_num_msg()==0x00)
	{
		if (m_MessageList.size()==10)
		{
			// очистим очередь.
			m_MessageList.clear();
			// на выход
			return true;
		}
	}
	// очистим очередь.
	m_MessageList.clear();
	// на выход
	return false;
}

/************************************************************************/
// поток записи данных
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
// поток чтения данных
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
		// читаем 50 сообщений
		if (count==50)
		{
			break;
		}
		//boost::this_thread::sleep( boost::posix_time::milliseconds(2) );
	}
}

/************************************************************************/
// выпоним много поточное тестирование на чтение/запись
/************************************************************************/
bool CMessageReceiver::TestMultyThreadIOTest()
{
	bool result = false;

	boost::thread wthread1(WriteThreadMessages,this);
	boost::thread wthread2(WriteThreadMessages,this);
	boost::thread rthread1(ReadThreadMessages,this);
	boost::thread rthread2(ReadThreadMessages,this);
	boost::thread rthread3(ReadThreadMessages,this);

	// два потока на запись
	wthread1.join();
	wthread2.join();
	// три на чтение
	rthread1.join();
	rthread2.join();
	rthread3.join();
	
	//
	//	проверка
	if (GetQueueSize()==50)
	{
		result = true;
	}

	//	очистим очередь
	m_MessageList.clear();
	//	вернем результат.
	return result;
}

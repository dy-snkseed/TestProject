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
	//	отослать сообщение
	bool SendMessage(std::string Message, std::string message_queue_name);

};

//////////////////////////////////////////////////////////////////////////
//
//	класс который в потоке принимает сообщения
//

class CMessageReceiver
{
public:
	CMessageReceiver(void);
	~CMessageReceiver(void);

	
	// функция потока чтения из очереди
	static void TreadReadFromQueue(CMessageReceiver* receiver, std::string message_queue_name);


	// получим первый элемент с удалением из очереди

	// запуск потока
	bool StartListern(std::string message_queue_name);

	// остановка потока
	bool StopListern();

	// проверка статуса потока

	// получить размер очереди
	unsigned GetQueueSize();
	//	получим сообщение
	std::string	PopMessage();
	// отправим сообщение
	bool PushMessage(char* message);

private:
	// поток чтения сообщений из очереди.
	boost::thread * m_IPCWorktThread;
	// потока чтения сообщений из сокета
	boost::thread * m_SocketWorkThread;
	//	 собственно список прочитанных сообщений.
	N_CStringList		m_MessageList;
	// рекурсивный мутекс для доступа к списку
	boost::mutex    m_MessageMutex;

	//	флаг остановки чтения
	bool	m_bStopReadFromQueue;

	//////////////////////////////////////////////////////////////////////////
	// Т е с т и р о в а н и е
public:
	//	выполним тестирование 
	bool MakeSelfTest();
private:
	// выполним много поточное тестирование на чтение/запись
	bool TestMultyThreadIOTest();
	// выполним тестирование чтения/записи в/из очереди
	bool TestMessageQueueIOTest();


};



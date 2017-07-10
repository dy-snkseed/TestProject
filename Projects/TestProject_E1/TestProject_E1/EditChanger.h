#pragma once
#include "afxwin.h"
#include "Receiver.h"
#include <ctime>


//////////////////////////////////////////////////////////////////////////
//
//	класс который в потоке записывает сообщения в list
//
class CEditChanger
{
public:
	CEditChanger(void);
	~CEditChanger(void);

	//Запуск потока, записывающего поступающие сообщения в list
	bool StartChanger(CMessageReceiver*, CListBox*);

	//Функция, зщаписывающая сообщения из receiver в listbox
    static	void Change(CMessageReceiver*, CListBox*, CEditChanger*);

	//Остановка потока
	bool StopChanger();
private:
	//Флаг остановки потока
	bool m_StopChanger;

	//Указатель на поток
	boost::thread * m_ChangerThread;
};
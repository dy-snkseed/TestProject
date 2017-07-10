#pragma once
#include "Receiver.h"
#include "stdafx.h"

class CWriter
{
public:
	//Конструктор
	CWriter();
	//Деструктор
	~CWriter();
	//Запись лога для службы
	int AddLogMessage(TCHAR* message);
	//Получение текущего времени в формате CString
	static CString GetCStringTime();
	//Получение строки из Edit`a
	static std::string GetStringFromEdit(int idc_editbox);
	//Поток записи в ListBox
	static void CWriteListThread(CMessageReceiver *recevier, int idc_list, CWriter *parent);
	//Поток записи в файл для службы
	static void CWriteFileThread(CMessageReceiver *recevier, CWriter *parent);
	//Перегрузка запуска потока записи в ListBox
	bool StartWriting(CMessageReceiver *recevier, int idc_list);
	//Перегрузка запуска потока записи в файл
	bool StartWriting(CMessageReceiver *recevier);
	//Остановка потоков записи
	bool StopWriting();
	//Флаг проверки существования файла лога
	bool m_isLogExists;
	//Флаг остановки потока
	bool m_stopWriterThread;
	//Функция получения User SID
	static bool GetUserSid(HANDLE userToken, PSID *userSid);
	//Имя файла лога
	CString m_logFileName;
	//Имя файла сообщений
	CString m_messageFileName;
private:
	//Указатель на файл лога
	FILE *m_logFile;
	//Указатель на файл сообщений
	FILE *m_messagesFile;
	//Указатель на поток
	boost::thread * m_writerThread;
};
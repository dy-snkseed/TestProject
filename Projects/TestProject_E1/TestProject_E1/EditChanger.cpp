#include "stdafx.h"
#include "EditChanger.h"

CEditChanger::CEditChanger(void)
{
	m_StopChanger = false;
};
CEditChanger::~CEditChanger(void)
{
};

void CEditChanger::Change(CMessageReceiver *recevier, CListBox *list, CEditChanger *parent)
{
	
	while(parent->m_StopChanger==false)
	{
		CString temp1(recevier->PopMessage().c_str());
		time_t seconds = time(NULL);
		tm* timeinfo = localtime(&seconds);
		CString time(asctime(timeinfo));
		if(!temp1.IsEmpty())list->AddString(time + ' ' + temp1);
		boost::this_thread::sleep( boost::posix_time::milliseconds(100));
	}
};



bool CEditChanger::StartChanger(CMessageReceiver *recevier, CListBox *list)
{
	m_ChangerThread = new boost::thread( CEditChanger::Change,recevier,list,this);
	return true;
};

bool CEditChanger::StopChanger()
{
	// выставим флаг
	m_StopChanger = true;
	//	дождемся закрытия потока
	m_ChangerThread->join();
	//	удалим.
	delete m_ChangerThread;
	// обнулим.
	m_ChangerThread = NULL;

	return true;
}
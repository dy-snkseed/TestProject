#pragma once
#include "afxwin.h"
#include "Receiver.h"
#include <ctime>


//////////////////////////////////////////////////////////////////////////
//
//	����� ������� � ������ ���������� ��������� � list
//
class CEditChanger
{
public:
	CEditChanger(void);
	~CEditChanger(void);

	//������ ������, ������������� ����������� ��������� � list
	bool StartChanger(CMessageReceiver*, CListBox*);

	//�������, ������������� ��������� �� receiver � listbox
    static	void Change(CMessageReceiver*, CListBox*, CEditChanger*);

	//��������� ������
	bool StopChanger();
private:
	//���� ��������� ������
	bool m_StopChanger;

	//��������� �� �����
	boost::thread * m_ChangerThread;
};
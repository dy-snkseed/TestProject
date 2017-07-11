#include "stdafx.h"
#include "CWriter.h"

#define LOG_FILE_NAME "C://Test_service_LOG.txt"
#define MESSAGES_FILE_NAME "C://MessagesTestService.txt"

CWriter::CWriter()
{
	m_logFile = NULL;
	m_messagesFile = NULL;
	m_writerThread = NULL;
	m_stopWriterThread = true;
	m_logFileName = LOG_FILE_NAME;
	m_messageFileName = MESSAGES_FILE_NAME;
}
CWriter::~CWriter()
{
	StopWriting();
}

int CWriter::AddLogMessage(TCHAR* message)
{
	m_logFile = _tfsopen((CT2W)m_logFileName, _T("a"), _SH_DENYNO);
	m_isLogExists = m_logFile!=NULL ?  true : false;
	if(m_isLogExists) 
	{
		_ftprintf_s(m_logFile, _T("%.24s : %s\n"), GetCStringTime(), message);
	}
	if(m_logFile) 
	{ 
		fclose(m_logFile);
	}
	m_logFile = NULL;
	return 0;
}

CString CWriter::GetCStringTime()
{
	time_t seconds = time(NULL);
	struct tm timeinfo;
	localtime_s(&timeinfo, &seconds);
	char buff[26];
	asctime_s(buff, sizeof buff, &timeinfo);
	CString time(buff);
	return time;
}
std::string CWriter::GetStringFromEdit(int idc_editbox)
{
	CString cStringMessage;
	LPTSTR  lpBuffer = cStringMessage.GetBuffer(255);
	GetDlgItemText(AfxGetMainWnd()->m_hWnd, idc_editbox, lpBuffer, 255);
	CT2CA convertedText (lpBuffer);
	std::string stdStringText (convertedText);
	cStringMessage.ReleaseBuffer();
	return stdStringText;
}
void CWriter::CWriteListThread(CMessageReceiver *recevier, CWriter *parent)
{
	while(parent->m_stopWriterThread==false)
	{
		CString receivedMessage(recevier->PopMessage().c_str());
		if(!receivedMessage.IsEmpty())
		{
			//list->AddString(GetCStringTime() + ' ' + temp1);
			CString message(GetCStringTime() + ' ' + receivedMessage);
			LPTSTR  lpBuffer = message.GetBuffer( );
			LPTSTR postLongLivingMessage = new TCHAR[message.GetLength()+1];
			_tcscpy_s(postLongLivingMessage,message.GetLength( )+1, lpBuffer);
			PostMessage(AfxGetApp()->GetMainWnd()->m_hWnd, CW_MESSAGE, 0, (LPARAM)postLongLivingMessage);
			message.ReleaseBuffer();
		}
		boost::this_thread::sleep( boost::posix_time::milliseconds(100));
	}
}
void CWriter::CWriteFileThread(CMessageReceiver *recevier, CWriter *parent)
{
	while(parent->m_stopWriterThread==false)
	{
		CString receivedMessage(recevier->PopMessage().c_str());
		parent->m_messagesFile = _tfsopen((CT2W)parent->m_messageFileName, _T("a"), _SH_DENYNO);
		if(!receivedMessage.IsEmpty() && parent->m_messagesFile)
		{
			_ftprintf_s(parent->m_messagesFile, _T("%.24s : %s\n"), GetCStringTime(), receivedMessage);
		}
		if(parent->m_messagesFile) fclose(parent->m_messagesFile);
		parent->m_messagesFile = NULL;
		boost::this_thread::sleep( boost::posix_time::milliseconds(100));
	}
}
bool CWriter::StartWriting(CMessageReceiver *recevier,int idc_list)
{
	if(m_stopWriterThread)
	{
		m_stopWriterThread = false;
		m_writerThread = new boost::thread( CWriter::CWriteListThread,recevier,this);
		return true;
	}
}
bool CWriter::StartWriting(CMessageReceiver *recevier)
{
	if(m_stopWriterThread)
	{
		m_stopWriterThread = false;
		m_writerThread = new boost::thread( CWriter::CWriteFileThread,recevier,this);
		return true;
	}
}
bool CWriter::StopWriting()
{
	m_stopWriterThread = true;
	if(m_writerThread)
	{
		m_writerThread->join();
		delete m_writerThread;
	}
	m_writerThread = NULL;
	if(m_messagesFile) 
	{
		fclose(m_messagesFile);
	}
	m_messagesFile = NULL;
	return true;
}
bool CWriter::GetUserSid(HANDLE hToken, PSID *userSid)
{
	BOOL bSuccess = FALSE;
	//DWORD dwIndex;
	DWORD dwLength = 0;
	PTOKEN_USER userToken = NULL;

	// Verify the parameter passed in is not NULL.
	if (NULL == userSid)
	{
		if (userToken != NULL)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)userToken);
		}

		return bSuccess;
	}

	// Get required buffer size and allocate the TOKEN_GROUPS buffer.

	if (!GetTokenInformation(
		hToken,         // handle to the access token
		TokenUser,    // get information about the token's groups 
		(LPVOID) userToken,   // pointer to TOKEN_GROUPS buffer
		0,              // size of buffer
		&dwLength       // receives required buffer size
		)) 
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) 
		{
			if (userToken != NULL)
			{
				HeapFree(GetProcessHeap(), 0, (LPVOID)userToken);
			}

			return bSuccess;
		}

		userToken = (PTOKEN_USER)HeapAlloc(GetProcessHeap(),
			HEAP_ZERO_MEMORY, dwLength);

		if (userToken == NULL)
		{
			if (userToken != NULL)
			{
				HeapFree(GetProcessHeap(), 0, (LPVOID)userToken);
			}

			return bSuccess;
		}
	}

	// Get the token group information from the access token.

	if (!GetTokenInformation(
		hToken,         // handle to the access token
		TokenUser,    // get information about the token's groups 
		(LPVOID) userToken,   // pointer to TOKEN_GROUPS buffer
		dwLength,       // size of buffer
		&dwLength       // receives required buffer size
		)) 
	{

		if (userToken != NULL)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)userToken);
		}

		return bSuccess;

	}

	// Loop through the groups to find the logon SID.


	// Found the logon SID; make a copy of it.

	dwLength = GetLengthSid(userToken->User.Sid);
	*userSid = (PSID) HeapAlloc(GetProcessHeap(),
		HEAP_ZERO_MEMORY, dwLength);
	if (*userSid == NULL) 
	{
		if (userToken != NULL)
		{
			HeapFree(GetProcessHeap(), 0, (LPVOID)userToken);
		}

		return bSuccess;
	}
	if (!CopySid(dwLength, *userSid, userToken->User.Sid)) 
	{
		HeapFree(GetProcessHeap(), 0, (LPVOID)*userSid);
		{
			if (userToken != NULL)
			{
				HeapFree(GetProcessHeap(), 0, (LPVOID)userToken);
			}

			return bSuccess;
		}
	};

	bSuccess = TRUE;


	// Free the buffer for the token groups.

	if (userToken != NULL)
		HeapFree(GetProcessHeap(), 0, (LPVOID)userToken);

	return bSuccess;
}
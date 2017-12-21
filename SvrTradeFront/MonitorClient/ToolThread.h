#pragma once
class CToolThread
{
public:
	CToolThread(LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter)
		:m_bNeedExit(false)
	{
		m_Handle=::CreateThread(NULL,0,lpStartAddress,lpParameter,0,NULL);
	}
	~CToolThread()
	{
		DWORD	ExitCode;
		m_bNeedExit=true;
		if(m_Handle)
		{
			WaitForSingleObject((HANDLE)m_Handle,2000);
			if(GetExitCodeThread((HANDLE)m_Handle,&ExitCode)!=0&&ExitCode==STILL_ACTIVE)
				TerminateThread((HANDLE)m_Handle,0);
			CloseHandle(m_Handle);
			m_Handle=NULL;
		}
	}
	bool IsNeedExit() const {return m_bNeedExit;}
private:
	HANDLE m_Handle;
	bool m_bNeedExit;
};
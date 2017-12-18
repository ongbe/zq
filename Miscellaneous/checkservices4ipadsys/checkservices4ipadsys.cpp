// checkservices4ipadsys.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "WinService.h"
#include "Tools_Ansi.h"
#include "Tools_Win32.h"
#include "TcpLayer.h"


int _tmain(int argc, _TCHAR* argv[])
{
    int			NeedStopID;
	char		InputBuf[256],ich;
	HANDLE		hSleepEvent;

	
    CTcpLayer::Init();

	NeedStopID=0;

	printf("press 'exit' to exit......\n");

	memset(InputBuf,0,sizeof(InputBuf));

	hSleepEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
	while(!NeedStopID)
	{
		if(_kbhit())
		{
			ich=(char)_getche();
			if(ich==10||ich==13)
			{
				if(stricmp(InputBuf,"exit")==0)
					break;
				memset(InputBuf,0,sizeof(InputBuf));
				printf("\r\n");
			}
			else if(ich==8)
			{
				int Len=strlen(InputBuf);
				if(Len>0)
				{
					InputBuf[Len-1]=0;
					printf("\r%s",InputBuf);
				}
			}
			else
				InputBuf[strlen(InputBuf)]=ich;
		}
		else
			WaitForSingleObject(hSleepEvent,100);
	}
	CloseHandle(hSleepEvent); 

    CTcpLayer::Stop();
    CTcpLayer::Release();
	return 0;
}


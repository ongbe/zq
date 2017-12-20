#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include "easymutex.h"
#include "PlotCtrlCommon.h"

using namespace std;

class CPlotImport
{
public:
	class CPlotItem {
	public:
		CPlotItem() {
			dwRealInval = 0;
			dwPrevReadTime = 0;
			dwPrevReadPos = 0;
		};
		virtual ~CPlotItem() {
		};

	public:
		string	strPlotID;
		string	strPath;
		DWORD	dwRealInval;
		DWORD	dwPrevReadTime;
		DWORD	dwPrevReadPos;
	};

	typedef struct {
		char	plotID[32];
		char	path[512];
		int		ReadInval;
	} PLOTCFGITEM, *LPPLOTCFGITEM;

public:
	typedef map<string, CPlotItem> MAPPLOTSTATUS;

public:
	CPlotImport(void);
	virtual ~CPlotImport(void);

	void LockObject() {
		m_mutex.lock();
	};
	void UnlockObject() {
		m_mutex.unlock();
	};

	bool Init(vector<PLOTCFGITEM>& vecPlotData);
	void Release();
	bool ImportPlot(queue<IMPORTPLOTDIRECT>& queuePlot);

protected:
	BOOL ParsePlotData(char* pBuff, queue<IMPORTPLOTDIRECT>& queuePlot);
	char* GetPlotItem(char* csText, queue<IMPORTPLOTDIRECT>& queuePlot);

protected:
	Ceasymutex		m_mutex;
	MAPPLOTSTATUS	m_mapPlotStatus;

};

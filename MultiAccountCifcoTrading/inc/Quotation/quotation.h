// Quotation.cpp : Defines the exported functions for the DLL application.
//

#pragma once

#include "KLineView.h"
#include "KLine.h"
#include "KVolume.h"
#include "KScales.h"

#include "TLineView.h"
#include "TLine.h"
#include "TVolume.h"
#include "TScales.h"
#include "AutoLock.h"
#include "CritSec.h"
#include "TCPDataService.h"
#include "KSplitterWindow.h"
#include "QPanel.h"

#include "FData.h"










#ifdef _DEBUG
	#ifdef _UNICODE
		#pragma comment (lib,"..\\Lib\\QuotationUD.lib")
	#else
		#pragma comment (lib,"..\\Lib\\QuotationD.lib")
	#endif
#else
	#ifdef _UNICODE
		#pragma comment (lib,"..\\Lib\\QuotationU.lib")
	#else
		#pragma comment (lib,"..\\Lib\\Quotation.lib")
	#endif
#endif  

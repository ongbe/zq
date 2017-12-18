// Tools_Win32.h: interface for the CTools_Win32 class.
//
//	Author		:lzq
//				 Copyright (C), 2008-2028
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLS_WIN32_H__9AD36DCF_6C92_44A3_94C5_B08759A9FCD7__INCLUDED_)
#define AFX_TOOLS_WIN32_H__9AD36DCF_6C92_44A3_94C5_B08759A9FCD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//是否使用psapi来枚举进程ID、获取进程模块名称。如果使用，则需要link psapi.lib。默认不使用
//#define _USE_PSAPI_

#pragma warning(disable : 4996)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)

#include <windows.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "public.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

#ifdef WIN32
#pragma managed(push,off)
#endif 


class CTools_Win32  
{
public:
	CTools_Win32();
	virtual ~CTools_Win32();


public:

	static void PaintClientBackGroundWithBmp(HANDLE hDC,HANDLE hWnd,HANDLE hMainWnd,HANDLE hBmp,void *pDCRect=NULL);


	//	用hbmp画窗口的背景，不考虑排除子窗口区域
	//	hDC、hWnd：画图的窗口和句柄
	//	hRefWnd：参考窗口句柄，hbmp的左上角对齐到hRefWnd的左上角
	static void PaintBackGroundWithBmp(HANDLE hDC,HANDLE hWnd,HANDLE hBmp,HANDLE hRefWnd);


	//	将MemDC的内容拷贝到hDC上，排除子窗口区域
	//	hDC、hWnd：画图的窗口和句柄
	//	pDCRect:hDC在hWnd上的位置，hDC的原点就是pDCRect的原点
	//	MemDC以hWnd为原点，hDC以pDCRect或hWnd为原点
	static void MemDCTohDC(HANDLE hDC,HANDLE MemDC,HANDLE hWnd,void *pDCRect=NULL);

	//	在HDC上用指定刷子填充，排除子窗口区域
	//	hDC、hWnd：画图的窗口和句柄
	static void FillRectExcludeChildren(HANDLE hDC,HANDLE hWnd,HANDLE hBr);


	//	取hbmp的宽度和高度，像素为单位
	static BOOL GetSizeOfBmp(HANDLE hBmp,int &BmpWid,int &BmpHei);

	static BOOL DrawhBmp(HDC hDC,int DstPox,int DstPoy,int DstWid,int DstHei,HBITMAP hBmp,int SrcPox,int SrcPoy);

	static BOOL DrawBmp(HDC hDC,char *BmpName,int ParentWid,int ParentHei);

	//	从Bitmap文件生成HBiTMAP
	static HBITMAP GetHBITMAPFromFile(HWND RefWnd,char *BmpName);

	//	将hbmp强制转换为24位的hbmp
	static HBITMAP Get24BitsHBitmap(HBITMAP hSourBmp,HWND RefWnd);

	static void GetBmpSize(HBITMAP hBmp,int *pWid,int *pHei,int *pBitsCount);

	//	在hDC上画hBmp，如果存在hMaskBmp，则执行Mask操作。目标尺寸为Wid和Hei，四个方向的Space 保持原样
	//	要求分9个区拷贝bmp，4个角原样拷贝，上下两边横向放大，左右两边纵向放大，中心纵横放大
	//	1 2 3
	//	4 5 6
	//	7 8 9
	//	Pox、Poy、Wid、Hei是在放大后的bmp中要取的位置大小
	//	hDC和hDCPox、hDCPoy是将bmp指定部分画到hDC中的指定位置上
	//	此函数是画整个Bmp
	static void DrawEntireBmpWithStretchNoMask(HDC hDC,int hDCPox,int hDCPoy,HBITMAP hBmp,int DstWid,int DstHei,int LeftSpace,int RightSpace,int TopSpace,int BomSpace);

	//	在hDC上画hBmp，如果存在hMaskBmp，则执行Mask操作。目标尺寸为Wid和Hei，四个方向的Space 保持原样
	//	将hBmp放大到DstWid和DstHei大小，然后将Pox\Poy\Wid\Hei部分拷贝到hDC的hDCPox/hDCPoy 位置上
	//	要求分9个区拷贝bmp，4个角原样拷贝，上下两边横向放大，左右两边纵向放大，中心纵横放大
	//	1 2 3
	//	4 5 6
	//	7 8 9
	//	Pox、Poy、Wid、Hei是在放大后的bmp中要取的位置大小
	//	hDC和hDCPox、hDCPoy是将bmp指定部分画到hDC中的指定位置上
	static void DrawBmpWithStretch(HDC hDC,int hDCPox,int hDCPoy,int Pox,int Poy,int Wid,int Hei,HBITMAP hBmp,HBITMAP hMaskBmp,int DstWid,int DstHei,int LeftSpace,int RightSpace,int TopSpace,int BomSpace);

	//	拉伸hBmp，目标尺寸为Wid和Hei，四个方向的Space 保持原样
	//	要求分9个区拷贝bmp，4个角原样拷贝，上下两边横向放大，左右两边纵向放大，中心纵横放大
	//	1 2 3
	//	4 5 6
	//	7 8 9
	//	hDC是参考的DC，决定hBmp的位数等
	static HBITMAP CreateStretchBmp(HDC hDC,HBITMAP hBmp,int DstWid,int DstHei,int LeftSpace,int RightSpace,int TopSpace,int BomSpace);


	static void		MyLineTo(HDC hDC,int StartX,int StartY,int EndX,int EndY,COLORREF LineCol);
	static inline void	MyLineTo2(HDC hDC,int StartX,int StartY,int EndX,int EndY) { MoveToEx(hDC,StartX,StartY,NULL);  LineTo(hDC,EndX,EndY); }
	static void		Set4Pixel(HDC hDC,int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,COLORREF Col);
	static BYTE		GetDeferentBetweenTwoColor(COLORREF Col1,COLORREF Col2);
	static BYTE		GetGrayValFromRGB(COLORREF iCol);
	static COLORREF	GetPixelColorAve(HDC hDC,int Pox,int Poy);
	static void		DrawWindowFram(HDC hDC,int Pox,int Poy,int Wid,int Hei,int LineWid,COLORREF *pCol1,COLORREF *pCol2);
	static void		Draw3DHoriLine(HDC hDC,int Pox,int Poy,int Wid,HPEN hPen1,HPEN hPen2,HPEN hPen3);
	static void		DrawRect(HDC hDC,RECT*Rect);
	static void		DrawLeavenRect(HDC hDC,RECT Rect,COLORREF TopCol,COLORREF BtnCol);
	static COLORREF	GetLeavenColor(COLORREF TopCol,COLORREF BtnCol,int Topy,int Btny,int Cury);
	static COLORREF	GetAveColor(COLORREF Col1,COLORREF Col2);


	static void SleepByEvt(int TimeoutMS);

	static void GetWorkAreaSize(int &ScnWid,int &ScnHei);
	static void GetMainPath(char *pMainPath,int iSize);
	static string GetMainPath();
	static void GetFileNameWithMainPath(char *pMainPath,int iSize,const char* pPureFileName);
	static void CheckMainPath(char *pRltMainPath,int iSize,const char *refMainPath);

	static void CreateMultipleDirectory(char *iPath);

	//	在串中用环境变量值替换%%包含的部分
	static void ReplaceWithEnvironmentVariable(char *pBuf,int BufSize);


	static void ScreenToClientRect(HWND hWnd,RECT *pRect);
	static void ClientToScreenRect(HWND hWnd,RECT *pRect);
	static void ResizeChildWindow(HWND hWnd,HWND hParentWnd,int NewWid,int NewHei,BOOL RePaintID);
	//	移动窗口位置和/或改变窗口大小，使用相对位置和相对尺寸，可以是负值
	static void MyMoveWindow(HWND hWnd,int DeltaPox,int DeltaPoy,int DeltaWid,int DeltaHei);		
	static int MyGetEditWndVal(HWND iWnd);
	static void MyGetEditWndStr(HWND iWnd,char *Buf,int BufSize);
	static void MySetEditWndVal(HWND iWnd,int Val);
	static BOOL IsFileUpdate(const char *FileName,struct _stat *pLastFileStat);

																
	//	取当前Cpu的Tick的数量
	static LONGLONG MyGetCpuTickCounter(void);						
	//	取当前Cpu的Tick的频率，单位为每秒多少次
	static LONGLONG MyGetCpuTickFreq(void);							
	//	取当前的复杂时间，64位，高32位为time_t，低32位的高16位为毫秒数，余下16位是为了区分秒-毫秒数相同的时间而设置的计数器
	static LONGLONG MyGetComplexTime_Exclusize(void);					

	//	获取当前系统内存使用情况，包括内存使用百分比、全部内存大小，已使用内存大小
	static void GetSystemMemeryUsage(DWORD *pCurUsagePercent,DWORD *pTotalMemSize,DWORD *pRltUsedMemSize);
																
	//	判断操作系统是否是WindowsNT以上的版本
	static BOOL IsWindowsNt(void);	




	//	申请读写信号量，成功返回指针，失败返回0
	static char* AllocWinSem(void);									
	//	释放读写信号量
	static void ReleaseWinSem(char* pSem);							
	//	要求独占信号量
	static void TakeWinSem(char* pSem);								
	//	停止独占信号量
	static void GiveWinSem(char* pSem);								

	static void GetClientSize(HWND hWnd,int* prltwid,int* prlthei);
	static int GetClientWid(HWND hWnd);
	static int GetClientHei(HWND hWnd);
	static void GetTextoutSize(HDC hDC,const char* pbuf,int buflen,int*prltwid,int*prlthei);
	static void GetTextoutSize2(HDC hDC,int textoutval,int*prltwid,int*prlthei);
	static SIZE GetTextoutSize3(HDC hDC,const char* pbuf,int buflen);
	static SIZE GetTextoutSize4(HDC hDC,int textoutval);

	//	停止线程，停止前最多等待timeoutseconds秒
	static void StopThread(HANDLE hThread,int timeoutseconds=8);

	//GBK和UTF8转换
	static string GBKToUTF8(const string& strGBK);
	static string UTF8ToGBK(const string& strUTF8);  

	//通过EnumWindow的方法，查找顶层窗口。和FindWindow的区别：需要匹配hParent
	static HWND FindDesktopWindowByParent(HWND hParent,const char* ClassName,const char* WindowName);

	//获取指定模块名称的进程ID。这里的名称不包含路径
	static vector<DWORD> GetProcessIDByExeFileName(const char* ExeFileName);
	//杀死进程。这里的名称不包含路径
	static bool KillProcess(const char* ExeFileName);
};


#ifdef WIN32
#pragma managed(pop)
#endif 

#endif // !defined(AFX_TOOLS_WIN32_H__9AD36DCF_6C92_44A3_94C5_B08759A9FCD7__INCLUDED_)

// Tools_Win32.cpp: implementation of the CTools_Win32 class.
//
//	Description	:1. Defines tool functions 
//	Version		:1.0
//	Date		:2008-4-30
//	Author		:lzq
//				 Copyright (C), 2008-2028
//	
//	History		:
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>

#include <iostream>
using namespace std;



#include "Tools_Win32.h"


#pragma comment(lib, "ws2_32.lib")											

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTools_Win32::CTools_Win32()
{

}

CTools_Win32::~CTools_Win32()
{

}


//	用hbmp画窗口的背景，画时排除子窗口区域
//	hDC、hWnd：画图的窗口和句柄
//	pDCRect:hDC在hWnd上的位置
//	hMainWnd：程序的主窗口，用来定位
void CTools_Win32::PaintClientBackGroundWithBmp(HANDLE hDC,HANDLE hWnd,HANDLE hMainWnd,HANDLE hBmp,void *pDCRect)
{
	RECT	Rect,iRect,iRect1;
	POINT	Pt;
	HDC		MemDC;
	HWND	hChildWnd;
	int		iWid,iHei,iPox,iPoy,i,j,k,DestPox,DestPoy;
	int		ChildLeft[100],ChildRight[100],ChildTop[100],ChildBottom[100],ChildNum;
	int		x1[sizeof(ChildLeft)/sizeof(ChildLeft[0])+2],y1[sizeof(ChildLeft)/sizeof(ChildLeft[0])+2];


	::GetClientRect((HWND)hWnd,&Rect);
	iWid=Rect.right-Rect.left;
	iHei=Rect.bottom-Rect.top;
	Pt.x=Rect.left;
	Pt.y=Rect.top;
	::ClientToScreen((HWND)hWnd,&Pt);
	::ScreenToClient((HWND)hMainWnd,&Pt);
	iPox=Pt.x;
	iPoy=Pt.y;

	MemDC=::CreateCompatibleDC((HDC)hDC);
	::SelectObject(MemDC,(HBITMAP)hBmp);

	hChildWnd=::GetWindow((HWND)hWnd,GW_CHILD);
	ChildNum=0;
	memset((char*)ChildLeft,0,sizeof(ChildLeft));
	memset((char*)ChildRight,0,sizeof(ChildRight));
	memset((char*)ChildTop,0,sizeof(ChildTop));
	memset((char*)ChildBottom,0,sizeof(ChildBottom));

																				//取各个子窗口的矩形
	for(i=0;i<sizeof(ChildLeft)/sizeof(ChildLeft[0])&&hChildWnd;i++)
	{
		if(::IsWindowVisible(hChildWnd))
		{
			::GetWindowRect(hChildWnd,&Rect);
			Pt.x=Rect.left;
			Pt.y=Rect.top;
			::ScreenToClient((HWND)hMainWnd,&Pt);
			ChildLeft[ChildNum]=Pt.x;
			ChildTop[ChildNum]=Pt.y;
			ChildRight[ChildNum]=Rect.right-Rect.left+Pt.x;
			ChildBottom[ChildNum]=Rect.bottom-Rect.top+Pt.y;
			x1[ChildNum*2]=ChildLeft[ChildNum];
			x1[ChildNum*2+1]=ChildRight[ChildNum];
			y1[ChildNum*2]=ChildTop[ChildNum];
			y1[ChildNum*2+1]=ChildBottom[ChildNum];
			ChildNum++;
		}
		hChildWnd=::GetWindow(hChildWnd,GW_HWNDNEXT);
	}
	x1[ChildNum*2]=iPox;
	x1[ChildNum*2+1]=iWid+iPox;
	y1[ChildNum*2]=iPoy;
	y1[ChildNum*2+1]=iHei+iPoy;
																				//将所有x坐标和y坐标排序
	for(i=0;i<ChildNum*2+2-1;i++)
	{
		for(j=0;j<ChildNum*2+2-1-i;j++)
		{
			if(x1[j]>x1[j+1])
			{
				k=x1[j];
				x1[j]=x1[j+1];
				x1[j+1]=k;
			}
			if(y1[j]>y1[j+1])
			{
				k=y1[j];
				y1[j]=y1[j+1];
				y1[j+1]=k;
			}
		}
	}
																				//每个分割出来的小矩形，如果不在子窗口区域内，画之
	for(i=0;i<ChildNum*2+2-1;i++)
	{
		for(j=0;j<ChildNum*2+2-1;j++)
		{
			if(x1[i+1]>x1[i]&&y1[j+1]>y1[j])
			{
				for(k=0;k<ChildNum;k++)
					if(x1[i]>=ChildLeft[k]&&x1[i]<ChildRight[k]&&y1[j]>=ChildTop[k]&&y1[j]<ChildBottom[k])
						break;
				if(k>=ChildNum)
				{									//不在任何子窗口区域内
					if(!pDCRect)
						::BitBlt((HDC)hDC,x1[i]-iPox,y1[j]-iPoy,x1[i+1]-x1[i],y1[j+1]-y1[j],MemDC,x1[i],y1[j],SRCCOPY);
					else
					{
						SetRect(&iRect,x1[i]-iPox,y1[j]-iPoy,x1[i+1]-iPox,y1[j+1]-iPoy);
						::IntersectRect(&iRect1,&iRect,(RECT*)pDCRect);
						if(!::IsRectEmpty(&iRect1))
						{
							DestPox=iRect1.left-((RECT*)pDCRect)->left;
							DestPoy=iRect1.top-((RECT*)pDCRect)->top;
							::BitBlt((HDC)hDC,DestPox,DestPoy,iRect1.right-iRect1.left,iRect1.bottom-iRect1.top,MemDC,iPox+iRect1.left,iPoy+iRect1.top,SRCCOPY);
						}
					}
				}
			}
		}
	}
	::DeleteDC(MemDC);
}


//	用hbmp画窗口的背景，不考虑排除子窗口区域
//	hDC、hWnd：画图的窗口和句柄
//	hRefWnd：参考窗口句柄，hbmp的左上角对齐到hRefWnd的左上角
void CTools_Win32::PaintBackGroundWithBmp(HANDLE hDC,HANDLE hWnd,HANDLE hBmp,HANDLE hRefWnd)
{
	RECT	Rect;
	HANDLE	MemDC;
	POINT	Pt;
	int		iWid,iHei;
	int		BmpWid,BmpHei;
	int		DstPox,DstPoy,DstWid,DstHei,SrcPox,SrcPoy,SrcWid,SrcHei;
	int		RefPox,RefPoy;
	
	
	GetSizeOfBmp((HBITMAP)hBmp,BmpWid,BmpHei);

	::GetClientRect((HWND)hWnd,&Rect);
	iWid=Rect.right-Rect.left;
	iHei=Rect.bottom-Rect.top;
	Pt.x=Rect.left;
	Pt.y=Rect.top;
	::ClientToScreen((HWND)hWnd,&Pt);
	::ScreenToClient((HWND)hRefWnd,&Pt);
	RefPox=Pt.x;
	RefPoy=Pt.y;

	DstPox=0;
	DstPoy=0;
	DstWid=iWid;
	DstHei=iHei;
	SrcPox=0;
	SrcPoy=0;
	SrcWid=BmpWid;
	SrcHei=BmpHei;

	if(RefPox>0)
	{
		SrcPox=RefPox;
		SrcWid-=SrcPox;
	}
	else if(RefPox<0)
	{
		DstPox=-RefPox;
		DstWid-=DstPox;
	}
	if(RefPoy>0)
	{
		SrcPoy=RefPoy;
		SrcHei-=SrcPoy;
	}
	else if(RefPoy<0)
	{
		DstPoy=-RefPoy;
		DstHei-=DstPoy;
	}
	if(DstWid>SrcWid) DstWid=SrcWid;
	if(DstHei>SrcHei) DstHei=SrcHei;


	if(DstWid>0&&DstHei>0&&SrcWid>0&&SrcHei>0)
	{
		MemDC=(HANDLE)::CreateCompatibleDC((HDC)hDC);
		::SelectObject((HDC)MemDC,(HBITMAP)hBmp);
		::BitBlt((HDC)hDC,DstPox,DstPoy,DstWid,DstHei,(HDC)MemDC,SrcPox,SrcPoy,SRCCOPY);
		::DeleteDC((HDC)MemDC);
	}
}


//	将MemDC的内容拷贝到hDC上，排除子窗口区域
//	hDC、hWnd：画图的窗口和句柄
//	pDCRect:hDC在hWnd上的位置，hDC的原点就是pDCRect的原点
//	MemDC以hWnd为原点，hDC以pDCRect或hWnd为原点
void CTools_Win32::MemDCTohDC(HANDLE hDC,HANDLE MemDC,HANDLE hWnd,void *pDCRect)
{
	RECT	Rect,iRect,iRect1;
	POINT	Pt;
	HWND	hChildWnd;
	int		iWid,iHei,i,j,k,DestPox,DestPoy;
	int		ChildLeft[100],ChildRight[100],ChildTop[100],ChildBottom[100],ChildNum;
	int		x1[sizeof(ChildLeft)/sizeof(ChildLeft[0])+2],y1[sizeof(ChildLeft)/sizeof(ChildLeft[0])+2];


	::GetClientRect((HWND)hWnd,&Rect);
	iWid=Rect.right-Rect.left;
	iHei=Rect.bottom-Rect.top;


	hChildWnd=::GetWindow((HWND)hWnd,GW_CHILD);
	ChildNum=0;
	memset((char*)ChildLeft,0,sizeof(ChildLeft));
	memset((char*)ChildRight,0,sizeof(ChildRight));
	memset((char*)ChildTop,0,sizeof(ChildTop));
	memset((char*)ChildBottom,0,sizeof(ChildBottom));

																				//取各个子窗口的矩形
	for(i=0;i<sizeof(ChildLeft)/sizeof(ChildLeft[0])&&hChildWnd;i++)
	{
		if(::IsWindowVisible(hChildWnd))
		{
			::GetWindowRect(hChildWnd,&Rect);
			Pt.x=Rect.left;
			Pt.y=Rect.top;
			::ScreenToClient((HWND)hWnd,&Pt);
			ChildLeft[ChildNum]=Pt.x;
			ChildTop[ChildNum]=Pt.y;
			ChildRight[ChildNum]=Rect.right-Rect.left+Pt.x;
			ChildBottom[ChildNum]=Rect.bottom-Rect.top+Pt.y;
			x1[ChildNum*2]=ChildLeft[ChildNum];
			x1[ChildNum*2+1]=ChildRight[ChildNum];
			y1[ChildNum*2]=ChildTop[ChildNum];
			y1[ChildNum*2+1]=ChildBottom[ChildNum];
			ChildNum++;
		}
		hChildWnd=::GetWindow(hChildWnd,GW_HWNDNEXT);
	}
	x1[ChildNum*2]=0;
	x1[ChildNum*2+1]=iWid;
	y1[ChildNum*2]=0;
	y1[ChildNum*2+1]=iHei;
																				//将所有x坐标和y坐标排序
	for(i=0;i<ChildNum*2+2-1;i++)
	{
		for(j=0;j<ChildNum*2+2-1-i;j++)
		{
			if(x1[j]>x1[j+1])
			{
				k=x1[j];
				x1[j]=x1[j+1];
				x1[j+1]=k;
			}
			if(y1[j]>y1[j+1])
			{
				k=y1[j];
				y1[j]=y1[j+1];
				y1[j+1]=k;
			}
		}
	}
																				//每个分割出来的小矩形，如果不在子窗口区域内，画之
	for(i=0;i<ChildNum*2+2-1;i++)
	{
		for(j=0;j<ChildNum*2+2-1;j++)
		{
			if(x1[i+1]>x1[i]&&y1[j+1]>y1[j])
			{
				for(k=0;k<ChildNum;k++)
					if(x1[i]>=ChildLeft[k]&&x1[i]<ChildRight[k]&&y1[j]>=ChildTop[k]&&y1[j]<ChildBottom[k])
						break;
				if(k>=ChildNum)
				{									//不在任何子窗口区域内
					if(!pDCRect)
						::BitBlt((HDC)hDC,x1[i],y1[j],x1[i+1]-x1[i],y1[j+1]-y1[j],(HDC)MemDC,x1[i],y1[j],SRCCOPY);
					else
					{
						SetRect(&iRect,x1[i],y1[j],x1[i+1],y1[j+1]);
						::IntersectRect(&iRect1,&iRect,(RECT*)pDCRect);
						if(!::IsRectEmpty(&iRect1))
						{
							DestPox=iRect1.left-((RECT*)pDCRect)->left;
							DestPoy=iRect1.top-((RECT*)pDCRect)->top;
							::BitBlt((HDC)hDC,DestPox,DestPoy,iRect1.right-iRect1.left,iRect1.bottom-iRect1.top,(HDC)MemDC,iRect1.left,iRect1.top,SRCCOPY);
						}
					}
				}
			}
		}
	}
	::DeleteDC((HDC)MemDC);
}

//	在HDC上用指定刷子填充，排除子窗口区域
//	hDC、hWnd：画图的窗口和句柄
void CTools_Win32::FillRectExcludeChildren(HANDLE hDC,HANDLE hWnd,HANDLE hBr)
{
	RECT	Rect,iFillRect;
	POINT	Pt;
	HWND	hChildWnd;
	int		iWid,iHei,i,j,k;
	int		ChildLeft[200],ChildRight[200],ChildTop[200],ChildBottom[200],ChildNum;
	int		x1[ArrayElements(ChildLeft)+2],y1[ArrayElements(ChildLeft)+2];


	::GetClientRect((HWND)hWnd,&Rect);
	iWid=Rect.right-Rect.left;
	iHei=Rect.bottom-Rect.top;


	hChildWnd=::GetWindow((HWND)hWnd,GW_CHILD);
	ChildNum=0;
	memset((char*)ChildLeft,0,sizeof(ChildLeft));
	memset((char*)ChildRight,0,sizeof(ChildRight));
	memset((char*)ChildTop,0,sizeof(ChildTop));
	memset((char*)ChildBottom,0,sizeof(ChildBottom));

																				//取各个子窗口的矩形
	for(i=0;i<ArrayElements(ChildLeft)&&ChildNum*2+1<ArrayElements(ChildLeft)&&hChildWnd;i++)
	{
		if(::IsWindowVisible(hChildWnd))
		{
			::GetWindowRect(hChildWnd,&Rect);
			Pt.x=Rect.left;
			Pt.y=Rect.top;
			::ScreenToClient((HWND)hWnd,&Pt);
			ChildLeft[ChildNum]=Pt.x;
			ChildTop[ChildNum]=Pt.y;
			ChildRight[ChildNum]=Rect.right-Rect.left+Pt.x;
			ChildBottom[ChildNum]=Rect.bottom-Rect.top+Pt.y;
			x1[ChildNum*2]=ChildLeft[ChildNum];
			x1[ChildNum*2+1]=ChildRight[ChildNum];
			y1[ChildNum*2]=ChildTop[ChildNum];
			y1[ChildNum*2+1]=ChildBottom[ChildNum];
			ChildNum++;
		}
		hChildWnd=::GetWindow(hChildWnd,GW_HWNDNEXT);
	}
	x1[ChildNum*2]=0;
	x1[ChildNum*2+1]=iWid;
	y1[ChildNum*2]=0;
	y1[ChildNum*2+1]=iHei;
																				//将所有x坐标和y坐标排序
	for(i=0;i<ChildNum*2+2-1;i++)
	{
		for(j=0;j<ChildNum*2+2-1-i;j++)
		{
			if(x1[j]>x1[j+1])
			{
				k=x1[j];
				x1[j]=x1[j+1];
				x1[j+1]=k;
			}
			if(y1[j]>y1[j+1])
			{
				k=y1[j];
				y1[j]=y1[j+1];
				y1[j+1]=k;
			}
		}
	}
															//每个分割出来的小矩形，如果不在子窗口区域内，画之
	for(i=0;i<ChildNum*2+2-1;i++)
	{
		for(j=0;j<ChildNum*2+2-1;j++)
		{
			if(x1[i+1]>x1[i]&&y1[j+1]>y1[j])
			{
				for(k=0;k<ChildNum;k++)
					if(x1[i]>=ChildLeft[k]&&x1[i]<ChildRight[k]&&y1[j]>=ChildTop[k]&&y1[j]<ChildBottom[k])
						break;
				if(k>=ChildNum)
				{											//不在任何子窗口区域内
					SetRect(&iFillRect,x1[i],y1[j],x1[i+1],y1[j+1]);
					FillRect((HDC)hDC,&iFillRect,(HBRUSH)hBr);
				}
			}
		}
	}
}




//	取hbmp的宽度和高度，像素为单位
BOOL CTools_Win32::GetSizeOfBmp(HANDLE hBmp,int &BmpWid,int &BmpHei)
{
	BITMAP			Bmp;

	if(!hBmp) return FALSE;
	
	GetObject((HBITMAP)hBmp,sizeof(Bmp),&Bmp);
	BmpWid=Bmp.bmWidth;
	BmpHei=Bmp.bmHeight;
	return TRUE;
}


BOOL CTools_Win32::DrawhBmp(HDC hDC,int DstPox,int DstPoy,int DstWid,int DstHei,HBITMAP hBmp,int SrcPox,int SrcPoy)
{
	HDC MemDC;
	if(!hDC||!hBmp) return FALSE;
	MemDC=CreateCompatibleDC(hDC);
	SelectObject(MemDC,hBmp);
	BitBlt(hDC,DstPox,DstPoy,DstWid,DstHei,MemDC,SrcPox,SrcPoy,SRCCOPY);
	DeleteDC(MemDC);
	return TRUE;
}

//	画bmp
BOOL CTools_Win32::DrawBmp(HDC hDC,char *BmpName,int ParentWid,int ParentHei)
{
	HFILE hF;
	int i,FileSize;

	BITMAPFILEHEADER bmfh;
	BITMAPINFO *bmif;
	BITMAPINFOHEADER *bmih;
	RGBQUAD *aColors;
	BYTE *lpDibBits;
	int MaxColNum,UseColNum;

	HPALETTE hPal,OldPal;
	LOGPALETTE *lgpl;

	hF=_lopen(BmpName,OF_READ);
	if(hF==HFILE_ERROR) return FALSE;
	FileSize=_llseek(hF,0,2);
	_llseek(hF,0,0);       
	_lread(hF,&bmfh,sizeof(BITMAPFILEHEADER));
	bmif=(BITMAPINFO *)malloc(sizeof(BITMAPINFO));
	if(!bmif) {  _lclose(hF);  return FALSE; }
	bmih=(BITMAPINFOHEADER *)bmif;
	_lread(hF,bmih,sizeof(BITMAPINFOHEADER));
	switch(bmih->biBitCount)
	{
		case 1: MaxColNum=2;   break;
		case 4: MaxColNum=16;  break;
		case 8: MaxColNum=256; break;
		default:MaxColNum=0;   break;
	}
	UseColNum=bmih->biClrUsed;
	if(UseColNum==0||UseColNum>MaxColNum) UseColNum=MaxColNum;
	bmif=(BITMAPINFO *)realloc(bmif,sizeof(BITMAPINFO)+MaxColNum*sizeof(RGBQUAD));
	if(!bmif) {  _lclose(hF);  return FALSE; }
	bmih=(BITMAPINFOHEADER *)bmif;
	aColors=(RGBQUAD *)((char *)bmif+sizeof(BITMAPINFOHEADER));
	if(MaxColNum>0)
		_lread(hF,aColors,MaxColNum*sizeof(RGBQUAD));

	FileSize-=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+MaxColNum*sizeof(RGBQUAD);
	if(FileSize<0||FileSize>20000000) return FALSE;

	lpDibBits=(BYTE *)malloc(FileSize);
	if(!lpDibBits) {  free(bmif);  _lclose(hF);  return FALSE; }
	_lread(hF,lpDibBits,FileSize);
	_lclose(hF);

	lgpl=(LOGPALETTE *)malloc(sizeof(LOGPALETTE)+UseColNum*sizeof(PALETTEENTRY));
	if(!lgpl) {  free(bmif);  free(lpDibBits);  return FALSE; }
	lgpl->palVersion=0x300;
	lgpl->palNumEntries=UseColNum;
	for(i=0;i<UseColNum;i++)
	{
		lgpl->palPalEntry[i].peRed=aColors[i].rgbRed;
		lgpl->palPalEntry[i].peGreen=aColors[i].rgbGreen;
		lgpl->palPalEntry[i].peBlue=aColors[i].rgbBlue;
		lgpl->palPalEntry[i].peFlags=NULL;
	}
	hPal=CreatePalette(lgpl);
	OldPal=SelectPalette(hDC,hPal,FALSE);
	UnrealizeObject(hPal);
	RealizePalette(hDC);
	i=SetDIBitsToDevice(hDC,(ParentWid-bmih->biWidth)/2,(ParentHei-bmih->biHeight)/2,
										bmih->biWidth,bmih->biHeight,0,0,0,bmih->biHeight,
										lpDibBits,bmif,DIB_RGB_COLORS);
	SelectPalette(hDC,OldPal,FALSE);
	DeleteObject(hPal);
	free(lpDibBits);
	free(lgpl);
	free(bmif);
	return TRUE;
}


//	从Bitmap文件生成HBiTMAP
HBITMAP CTools_Win32::GetHBITMAPFromFile(HWND RefWnd,char *BmpName)
{
	HBITMAP						hBmp;
	HFILE							hF;
	int								FileSize;
	HDC								hDC;

	BITMAPFILEHEADER	bmfh;
	BITMAPINFO				*pbmif;
	BITMAPINFOHEADER  *pbmih;
	char							bmifBuf[sizeof(BITMAPINFO)+256*sizeof(RGBQUAD)];
	BYTE							*lpDibBits;
	int								MaxColNum,UseColNum;

	pbmif=(BITMAPINFO*)bmifBuf;
	pbmih=(BITMAPINFOHEADER*)bmifBuf;
	memset(bmifBuf,0,sizeof(bmifBuf));

	hF=_lopen(BmpName,OF_READ);
	if(hF==HFILE_ERROR) return NULL;

	FileSize=_llseek(hF,0,2);
	_llseek(hF,0,0);
	_lread(hF,&bmfh,sizeof(BITMAPFILEHEADER));
	_lread(hF,pbmih,sizeof(BITMAPINFOHEADER));

	switch(pbmih->biBitCount)
	{
		case 1: MaxColNum=2;   break;
		case 4: MaxColNum=16;  break;
		case 8: MaxColNum=256; break;
		default:MaxColNum=0;   break;
	}

	UseColNum=pbmih->biClrUsed;
	if(UseColNum==0) UseColNum=MaxColNum;
	if(UseColNum>MaxColNum) UseColNum=MaxColNum;
	if(UseColNum>0)
		_lread(hF,(char*)&(pbmif->bmiColors),UseColNum*sizeof(RGBQUAD));

	FileSize-=bmfh.bfOffBits;
	if(FileSize<0||FileSize>20000000) { _lclose(hF);  return NULL; }
	lpDibBits=(BYTE *)malloc(FileSize);
	if(!lpDibBits) { _lclose(hF);  return NULL; }
	_llseek(hF,bmfh.bfOffBits,0);
	_lread(hF,lpDibBits,FileSize);
	_lclose(hF);

	hDC=GetDC(RefWnd);
	
	hBmp=CreateDIBitmap(hDC,pbmih,CBM_INIT,lpDibBits,pbmif,DIB_RGB_COLORS);

	ReleaseDC(RefWnd,hDC);
	free(lpDibBits);
	return hBmp;
}


//	将hbmp强制转换为24位的hbmp
HBITMAP CTools_Win32::Get24BitsHBitmap(HBITMAP hSourBmp,HWND RefWnd)
{
	BITMAP	Bmp;
	HDC			hDC,hSourDC,hDestDC;
	HBITMAP	hDestBmp;

	GetObject(hSourBmp,sizeof(Bmp),&Bmp);
	if(Bmp.bmBitsPixel==24) return hSourBmp;

	hDC=GetDC(RefWnd);
	hSourDC=CreateCompatibleDC(hDC);
	hDestDC=CreateCompatibleDC(hDC);
	SelectObject(hSourDC,hSourBmp);

	hDestBmp=CreateBitmap(Bmp.bmWidth,Bmp.bmHeight,1,24,NULL);
	SelectObject(hDestDC,hDestBmp);
	
	BitBlt(hDestDC,0,0,Bmp.bmWidth,Bmp.bmHeight,hSourDC,0,0,SRCCOPY);

	DeleteDC(hSourDC);
	DeleteDC(hDestDC);
	ReleaseDC(RefWnd,hDC);

	return hDestBmp;
}


void CTools_Win32::GetBmpSize(HBITMAP hBmp,int *pWid,int *pHei,int *pBitsCount)
{
	BITMAP Bmp;
	GetObject(hBmp,sizeof(Bmp),&Bmp);
	if(pWid) *pWid=Bmp.bmWidth;
	if(pHei) *pHei=Bmp.bmHeight;
	if(pBitsCount) *pBitsCount=Bmp.bmBitsPixel;
}


//	在hDC上画hBmp，如果存在hMaskBmp，则执行Mask操作。目标尺寸为Wid和Hei，四个方向的Space 保持原样
//	要求分9个区拷贝bmp，4个角原样拷贝，上下两边横向放大，左右两边纵向放大，中心纵横放大
//	1 2 3
//	4 5 6
//	7 8 9
//	Pox、Poy、Wid、Hei是在放大后的bmp中要取的位置大小
//	hDC和hDCPox、hDCPoy是将bmp指定部分画到hDC中的指定位置上
//	此函数是画整个Bmp
void CTools_Win32::DrawEntireBmpWithStretchNoMask(HDC hDC,int hDCPox,int hDCPoy,HBITMAP hBmp,int DstWid,int DstHei,int LeftSpace,int RightSpace,int TopSpace,int BomSpace)
{
	HDC			MemDC1;
	int			BmpWid,BmpHei;

	if(!hBmp) return;

	SetStretchBltMode(hDC,COLORONCOLOR);
	MemDC1=CreateCompatibleDC(hDC);
	SelectObject(MemDC1,hBmp);
	GetBmpSize(hBmp,&BmpWid,&BmpHei,NULL);

	if(TopSpace>0)
	{
		if(LeftSpace>0)
			BitBlt(hDC,hDCPox,hDCPoy,LeftSpace,TopSpace,MemDC1,0,0,SRCCOPY);
		if(DstWid-LeftSpace-RightSpace>0&&BmpWid-LeftSpace-RightSpace>0)
		{
			if(DstWid-LeftSpace-RightSpace!=BmpWid-LeftSpace-RightSpace)
				StretchBlt(hDC,hDCPox+LeftSpace,hDCPoy,DstWid-LeftSpace-RightSpace,TopSpace,MemDC1,LeftSpace,0,BmpWid-LeftSpace-RightSpace,TopSpace,SRCCOPY);
			else 
				BitBlt(hDC,hDCPox+LeftSpace,hDCPoy,DstWid-LeftSpace-RightSpace,TopSpace,MemDC1,LeftSpace,0,SRCCOPY);
		}
		if(RightSpace>0)
			BitBlt(hDC,hDCPox+DstWid-RightSpace,hDCPoy,RightSpace,TopSpace,MemDC1,BmpWid-RightSpace,0,SRCCOPY);
	}

	if(DstHei-TopSpace-BomSpace>0&&BmpHei-TopSpace-BomSpace>0)
	{
		if(LeftSpace>0)
		{
			if(DstHei-TopSpace-BomSpace!=BmpHei-TopSpace-BomSpace)
				StretchBlt(hDC,hDCPox,hDCPoy+TopSpace,LeftSpace,DstHei-TopSpace-BomSpace,MemDC1,0,TopSpace,LeftSpace,BmpHei-TopSpace-BomSpace,SRCCOPY);
			else 
				BitBlt(hDC,hDCPox,hDCPoy+TopSpace,LeftSpace,DstHei-TopSpace-BomSpace,MemDC1,0,TopSpace,SRCCOPY);
		}
		if(DstWid-LeftSpace-RightSpace>0&&BmpWid-LeftSpace-RightSpace>0)
		{
			if(DstWid-LeftSpace-RightSpace!=BmpWid-LeftSpace-RightSpace||DstHei-TopSpace-BomSpace!=BmpHei-TopSpace-BomSpace)
				StretchBlt(hDC,hDCPox+LeftSpace,hDCPoy+TopSpace,DstWid-LeftSpace-RightSpace,DstHei-TopSpace-BomSpace,MemDC1,LeftSpace,TopSpace,BmpWid-LeftSpace-RightSpace,BmpHei-TopSpace-BomSpace,SRCCOPY);
			else 
				BitBlt(hDC,hDCPox+LeftSpace,hDCPoy+TopSpace,DstWid-LeftSpace-RightSpace,DstHei-TopSpace-BomSpace,MemDC1,LeftSpace,TopSpace,SRCCOPY);
		}
		if(RightSpace>0)
		{
			if(DstHei-TopSpace-BomSpace!=BmpHei-TopSpace-BomSpace)
				StretchBlt(hDC,hDCPox+DstWid-RightSpace,hDCPoy+TopSpace,RightSpace,DstHei-TopSpace-BomSpace,MemDC1,BmpWid-RightSpace,TopSpace,RightSpace,BmpHei-TopSpace-BomSpace,SRCCOPY);
			else 
				BitBlt(hDC,hDCPox+DstWid-RightSpace,hDCPoy+TopSpace,RightSpace,DstHei-TopSpace-BomSpace,MemDC1,BmpWid-RightSpace,TopSpace,SRCCOPY);
		}
	}

	if(BomSpace>0)
	{
		if(LeftSpace>0)
			BitBlt(hDC,hDCPox,hDCPoy+DstHei-BomSpace,LeftSpace,BomSpace,MemDC1,0,BmpHei-BomSpace,SRCCOPY);
		if(DstWid-LeftSpace-RightSpace>0&&BmpWid-LeftSpace-RightSpace>0)
		{
			if(DstWid-LeftSpace-RightSpace!=BmpWid-LeftSpace-RightSpace)
				StretchBlt(hDC,hDCPox+LeftSpace,hDCPoy+DstHei-BomSpace,DstWid-LeftSpace-RightSpace,BomSpace,MemDC1,LeftSpace,BmpHei-BomSpace,BmpWid-LeftSpace-RightSpace,BomSpace,SRCCOPY);
			else 
				BitBlt(hDC,hDCPox+LeftSpace,hDCPoy+DstHei-BomSpace,DstWid-LeftSpace-RightSpace,BomSpace,MemDC1,LeftSpace,BmpHei-BomSpace,SRCCOPY);
		}
		if(RightSpace>0)
			BitBlt(hDC,hDCPox+DstWid-RightSpace,hDCPoy+DstHei-BomSpace,RightSpace,BomSpace,MemDC1,BmpWid-RightSpace,BmpHei-BomSpace,SRCCOPY);
	}

	DeleteObject(MemDC1);

}


//	在hDC上画hBmp，如果存在hMaskBmp，则执行Mask操作。目标尺寸为Wid和Hei，四个方向的Space 保持原样
//	将hBmp放大到DstWid和DstHei大小，然后将Pox\Poy\Wid\Hei部分拷贝到hDC的hDCPox/hDCPoy 位置上
//	要求分9个区拷贝bmp，4个角原样拷贝，上下两边横向放大，左右两边纵向放大，中心纵横放大
//	1 2 3
//	4 5 6
//	7 8 9
//	Pox、Poy、Wid、Hei是在放大后的bmp中要取的位置大小
//	hDC和hDCPox、hDCPoy是将bmp指定部分画到hDC中的指定位置上
void CTools_Win32::DrawBmpWithStretch(HDC hDC,int hDCPox,int hDCPoy,int Pox,int Poy,int Wid,int Hei,HBITMAP hBmp,HBITMAP hMaskBmp,int DstWid,int DstHei,int LeftSpace,int RightSpace,int TopSpace,int BomSpace)
{
	HDC			MemDC,MemDC1;
	int			BmpWid,BmpHei;
	HBITMAP	MemBmp,MaskBmp;

	if(!hBmp) return;

	MemDC=CreateCompatibleDC(hDC);
	MemBmp=CreateCompatibleBitmap(hDC,DstWid,DstHei);
	SelectObject(MemDC,MemBmp);

	MemDC1=CreateCompatibleDC(MemDC);
	SelectObject(MemDC1,hBmp);
	GetBmpSize(hBmp,&BmpWid,&BmpHei,NULL);
	SetStretchBltMode(MemDC,COLORONCOLOR);

	if(TopSpace>0)
	{
		if(LeftSpace>0)
			BitBlt(MemDC,0,0,LeftSpace,TopSpace,MemDC1,0,0,SRCCOPY);
		if(DstWid-LeftSpace-RightSpace>0&&BmpWid-LeftSpace-RightSpace>0)
		{
			if(DstWid-LeftSpace-RightSpace!=BmpWid-LeftSpace-RightSpace)
				StretchBlt(MemDC,LeftSpace,0,DstWid-LeftSpace-RightSpace,TopSpace,MemDC1,LeftSpace,0,BmpWid-LeftSpace-RightSpace,TopSpace,SRCCOPY);
			else 
				BitBlt(MemDC,LeftSpace,0,DstWid-LeftSpace-RightSpace,TopSpace,MemDC1,LeftSpace,0,SRCCOPY);
		}
		if(RightSpace>0)
			BitBlt(MemDC,DstWid-RightSpace,0,RightSpace,TopSpace,MemDC1,BmpWid-RightSpace,0,SRCCOPY);
	}

	if(DstHei-TopSpace-BomSpace>0&&BmpHei-TopSpace-BomSpace>0)
	{
		if(LeftSpace>0)
		{
			if(DstHei-TopSpace-BomSpace!=BmpHei-TopSpace-BomSpace)
				StretchBlt(MemDC,0,TopSpace,LeftSpace,DstHei-TopSpace-BomSpace,MemDC1,0,TopSpace,LeftSpace,BmpHei-TopSpace-BomSpace,SRCCOPY);
			else 
				BitBlt(MemDC,0,TopSpace,LeftSpace,DstHei-TopSpace-BomSpace,MemDC1,0,TopSpace,SRCCOPY);
		}
		if(DstWid-LeftSpace-RightSpace>0&&BmpWid-LeftSpace-RightSpace>0)
		{
			if(DstWid-LeftSpace-RightSpace!=BmpWid-LeftSpace-RightSpace||DstHei-TopSpace-BomSpace!=BmpHei-TopSpace-BomSpace)
				StretchBlt(MemDC,LeftSpace,TopSpace,DstWid-LeftSpace-RightSpace,DstHei-TopSpace-BomSpace,MemDC1,LeftSpace,TopSpace,BmpWid-LeftSpace-RightSpace,BmpHei-TopSpace-BomSpace,SRCCOPY);
			else 
				BitBlt(MemDC,LeftSpace,TopSpace,DstWid-LeftSpace-RightSpace,DstHei-TopSpace-BomSpace,MemDC1,LeftSpace,TopSpace,SRCCOPY);
		}
		if(RightSpace>0)
		{
			if(DstHei-TopSpace-BomSpace!=BmpHei-TopSpace-BomSpace)
				StretchBlt(MemDC,DstWid-RightSpace,TopSpace,RightSpace,DstHei-TopSpace-BomSpace,MemDC1,BmpWid-RightSpace,TopSpace,RightSpace,BmpHei-TopSpace-BomSpace,SRCCOPY);
			else 
				BitBlt(MemDC,DstWid-RightSpace,TopSpace,RightSpace,DstHei-TopSpace-BomSpace,MemDC1,BmpWid-RightSpace,TopSpace,SRCCOPY);
		}
	}

	if(BomSpace>0)
	{
		if(LeftSpace>0)
			BitBlt(MemDC,0,DstHei-BomSpace,LeftSpace,BomSpace,MemDC1,0,BmpHei-BomSpace,SRCCOPY);
		if(DstWid-LeftSpace-RightSpace>0&&BmpWid-LeftSpace-RightSpace>0)
		{
			if(DstWid-LeftSpace-RightSpace!=BmpWid-LeftSpace-RightSpace)
				StretchBlt(MemDC,LeftSpace,DstHei-BomSpace,DstWid-LeftSpace-RightSpace,BomSpace,MemDC1,LeftSpace,BmpHei-BomSpace,BmpWid-LeftSpace-RightSpace,BomSpace,SRCCOPY);
			else 
				BitBlt(MemDC,LeftSpace,DstHei-BomSpace,DstWid-LeftSpace-RightSpace,BomSpace,MemDC1,LeftSpace,BmpHei-BomSpace,SRCCOPY);
		}
		if(RightSpace>0)
			BitBlt(MemDC,DstWid-RightSpace,DstHei-BomSpace,RightSpace,BomSpace,MemDC1,BmpWid-RightSpace,BmpHei-BomSpace,SRCCOPY);
	}

	DeleteObject(MemDC1);

	if(hMaskBmp) MaskBmp=CreateStretchBmp(hDC,hMaskBmp,DstWid,DstHei,LeftSpace,RightSpace,TopSpace,BomSpace);
	else MaskBmp=NULL;

	if(!MaskBmp)
		BitBlt(hDC,hDCPox,hDCPoy,Wid,Hei,MemDC,Pox,Poy,SRCCOPY);
	else 
		MaskBlt(hDC,hDCPox,hDCPoy,Wid,Hei,MemDC,Pox,Poy,MaskBmp,Pox,Poy,MAKEROP4(SRCCOPY,SRCPAINT));

	DeleteDC(MemDC);
	DeleteObject(MemBmp);
	if(MaskBmp) DeleteObject(MaskBmp);
}

//	拉伸hBmp，目标尺寸为Wid和Hei，四个方向的Space 保持原样
//	要求分9个区拷贝bmp，4个角原样拷贝，上下两边横向放大，左右两边纵向放大，中心纵横放大
//	1 2 3
//	4 5 6
//	7 8 9
//	hDC是参考的DC，决定hBmp的位数等
HBITMAP CTools_Win32::CreateStretchBmp(HDC hDC,HBITMAP hBmp,int DstWid,int DstHei,int LeftSpace,int RightSpace,int TopSpace,int BomSpace)
{
	HDC			MemDC,MemDC1;
	int			BmpWid,BmpHei;
	HBITMAP	MemBmp;

	if(!hBmp) return NULL;

	MemDC=CreateCompatibleDC(hDC);
	SelectObject(MemDC,hBmp);
	MemBmp=CreateCompatibleBitmap(hDC,DstWid,DstHei);
	SelectObject(MemDC,MemBmp);
	SetStretchBltMode(MemDC,COLORONCOLOR);

	MemDC1=CreateCompatibleDC(hDC);
	SelectObject(MemDC1,hBmp);
	GetBmpSize(hBmp,&BmpWid,&BmpHei,NULL);

	if(TopSpace>0)
	{
		if(LeftSpace>0)
			BitBlt(MemDC,0,0,LeftSpace,TopSpace,MemDC1,0,0,SRCCOPY);
		if(DstWid-LeftSpace-RightSpace>0&&BmpWid-LeftSpace-RightSpace>0)
		{
			if(DstWid-LeftSpace-RightSpace!=BmpWid-LeftSpace-RightSpace)
				StretchBlt(MemDC,LeftSpace,0,DstWid-LeftSpace-RightSpace,TopSpace,MemDC1,LeftSpace,0,BmpWid-LeftSpace-RightSpace,TopSpace,SRCCOPY);
			else 
				BitBlt(MemDC,LeftSpace,0,DstWid-LeftSpace-RightSpace,TopSpace,MemDC1,LeftSpace,0,SRCCOPY);
		}
		if(RightSpace>0)
			BitBlt(MemDC,DstWid-RightSpace,0,RightSpace,TopSpace,MemDC1,BmpWid-RightSpace,0,SRCCOPY);
	}

	if(DstHei-TopSpace-BomSpace>0&&BmpHei-TopSpace-BomSpace>0)
	{
		if(LeftSpace>0)
		{
			if(DstHei-TopSpace-BomSpace!=BmpHei-TopSpace-BomSpace)
				StretchBlt(MemDC,0,TopSpace,LeftSpace,DstHei-TopSpace-BomSpace,MemDC1,0,TopSpace,LeftSpace,BmpHei-TopSpace-BomSpace,SRCCOPY);
			else 
				BitBlt(MemDC,0,TopSpace,LeftSpace,DstHei-TopSpace-BomSpace,MemDC1,0,TopSpace,SRCCOPY);
		}
		if(DstWid-LeftSpace-RightSpace>0&&BmpWid-LeftSpace-RightSpace>0)
		{
			if(DstWid-LeftSpace-RightSpace!=BmpWid-LeftSpace-RightSpace||DstHei-TopSpace-BomSpace!=BmpHei-TopSpace-BomSpace)
				StretchBlt(MemDC,LeftSpace,TopSpace,DstWid-LeftSpace-RightSpace,DstHei-TopSpace-BomSpace,MemDC1,LeftSpace,TopSpace,BmpWid-LeftSpace-RightSpace,BmpHei-TopSpace-BomSpace,SRCCOPY);
			else 
				BitBlt(MemDC,LeftSpace,TopSpace,DstWid-LeftSpace-RightSpace,DstHei-TopSpace-BomSpace,MemDC1,LeftSpace,TopSpace,SRCCOPY);
		}
		if(RightSpace>0)
		{
			if(DstHei-TopSpace-BomSpace!=BmpHei-TopSpace-BomSpace)
				StretchBlt(MemDC,DstWid-RightSpace,TopSpace,RightSpace,DstHei-TopSpace-BomSpace,MemDC1,BmpWid-RightSpace,TopSpace,RightSpace,BmpHei-TopSpace-BomSpace,SRCCOPY);
			else 
				BitBlt(MemDC,DstWid-RightSpace,TopSpace,RightSpace,DstHei-TopSpace-BomSpace,MemDC1,BmpWid-RightSpace,TopSpace,SRCCOPY);
		}
	}

	if(BomSpace>0)
	{
		if(LeftSpace>0)
			BitBlt(MemDC,0,DstHei-BomSpace,LeftSpace,BomSpace,MemDC1,0,BmpHei-BomSpace,SRCCOPY);
		if(DstWid-LeftSpace-RightSpace>0&&BmpWid-LeftSpace-RightSpace>0)
		{
			if(DstWid-LeftSpace-RightSpace!=BmpWid-LeftSpace-RightSpace)
				StretchBlt(MemDC,LeftSpace,DstHei-BomSpace,DstWid-LeftSpace-RightSpace,BomSpace,MemDC1,LeftSpace,BmpHei-BomSpace,BmpWid-LeftSpace-RightSpace,BomSpace,SRCCOPY);
			else 
				BitBlt(MemDC,LeftSpace,DstHei-BomSpace,DstWid-LeftSpace-RightSpace,BomSpace,MemDC1,LeftSpace,BmpHei-BomSpace,SRCCOPY);
		}
		if(RightSpace>0)
			BitBlt(MemDC,DstWid-RightSpace,DstHei-BomSpace,RightSpace,BomSpace,MemDC1,BmpWid-RightSpace,BmpHei-BomSpace,SRCCOPY);
	}

	DeleteObject(MemDC1);
	DeleteObject(MemDC);
	return MemBmp;
}





void CTools_Win32::MyLineTo(HDC hDC,int StartX,int StartY,int EndX,int EndY,COLORREF LineCol)
{
  HPEN		hPen,OldPen;
	hPen=CreatePen(PS_SOLID,1,LineCol);
	OldPen=(HPEN)SelectObject(hDC,hPen);
	MoveToEx(hDC,StartX,StartY,NULL);
	LineTo(hDC,EndX,EndY);
	SelectObject(hDC,OldPen);
	DeleteObject(hPen);
}

//	画矩形边框
void CTools_Win32::DrawRect(HDC hDC,RECT*Rect)
{
	MoveToEx(hDC,Rect->left,Rect->top,NULL);
	LineTo(hDC,Rect->right,Rect->top);
	LineTo(hDC,Rect->right,Rect->bottom);
	LineTo(hDC,Rect->left,Rect->bottom);
	LineTo(hDC,Rect->left,Rect->top);
}

//	用指定的颜色画4个点。如果点坐标<0则不画该点
void CTools_Win32::Set4Pixel(HDC hDC,int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,COLORREF Col)
{
	if(x1>=0&&y1>=0) SetPixel(hDC,x1,y1,Col);
	if(x2>=0&&y2>=0) SetPixel(hDC,x2,y2,Col);
	if(x3>=0&&y3>=0) SetPixel(hDC,x3,y3,Col);
	if(x4>=0&&y4>=0) SetPixel(hDC,x4,y4,Col);
}

BYTE CTools_Win32::GetDeferentBetweenTwoColor(COLORREF Col1,COLORREF Col2)
{
	BYTE DeltaR,DeltaG,DeltaB;
	DeltaR=GetRValue(Col1)>=GetRValue(Col2)?GetRValue(Col1):GetRValue(Col2);
	DeltaG=GetGValue(Col1)>=GetGValue(Col2)?GetGValue(Col1):GetGValue(Col2);
	DeltaB=GetBValue(Col1)>=GetBValue(Col2)?GetBValue(Col1):GetBValue(Col2);
	return (BYTE)(((int)DeltaR+(int)DeltaG+(int)DeltaB)/3);
}

//	将RGB转换成灰度
BYTE CTools_Win32::GetGrayValFromRGB(COLORREF iCol)
{
	return (BYTE)(((int)GetRValue(iCol)*30+(int)GetGValue(iCol)*59+(int)GetBValue(iCol)*11)/100);
}

//	取一个点附近的平均颜色和灰度值
COLORREF CTools_Win32::GetPixelColorAve(HDC hDC,int Pox,int Poy)
{
	COLORREF					Col1,Col2,Col3,Col4,Col5,Col6;
	int								iR,iG,iB;
	Col1=GetPixel(hDC,Pox,Poy);
	Col2=GetPixel(hDC,Pox+4,Poy);
	Col3=GetPixel(hDC,Pox+4,Poy+4);
	Col4=GetPixel(hDC,Pox,Poy+4);
	Col5=GetPixel(hDC,Pox+8,Poy);
	Col6=GetPixel(hDC,Pox+8,Poy+4);
	iR=((int)GetRValue(Col1)+(int)GetRValue(Col2)+(int)GetRValue(Col3)+(int)GetRValue(Col4)+(int)GetRValue(Col5)+(int)GetRValue(Col6))/6;
	iG=((int)GetGValue(Col1)+(int)GetGValue(Col2)+(int)GetGValue(Col3)+(int)GetGValue(Col4)+(int)GetGValue(Col5)+(int)GetGValue(Col6))/6;
	iB=((int)GetBValue(Col1)+(int)GetBValue(Col2)+(int)GetBValue(Col3)+(int)GetBValue(Col4)+(int)GetBValue(Col5)+(int)GetBValue(Col6))/6;
	return RGB(iR,iG,iB);
}


//**********************************************************
//	Name:	DrawWindowFram
//	Description:	画立体边框
//	LineWid:边框的宽度
//	pCol1:画左边和上边的颜色组，颜色数量等于边框宽度，从外到内排列
//	pCol2:画右边和下边的颜色组，颜色数量等于边框宽度，从外到内排列
//**********************************************************
void CTools_Win32::DrawWindowFram(HDC hDC,int Pox,int Poy,int Wid,int Hei,int LineWid,COLORREF *pCol1,COLORREF *pCol2)
{
	HPEN		hPen,OldPen;
	int			i;

  OldPen=(HPEN)SelectObject(hDC,(HPEN)GetStockObject(WHITE_PEN));
	for(i=0;i<LineWid;i++)
	{
	  hPen=CreatePen(PS_SOLID,1,pCol1[i]);
		SelectObject(hDC,hPen);
		MoveToEx(hDC,Pox+Wid-1-i,Poy+i,NULL);
		LineTo(hDC,Pox+i,Poy+i);
		LineTo(hDC,Pox+i,Poy+Hei-1-i);
		SelectObject(hDC,OldPen);
		DeleteObject(hPen);

		hPen=CreatePen(PS_SOLID,1,pCol2[i]);
		SelectObject(hDC,hPen);
		LineTo(hDC,Pox+Wid-1-i,Poy+Hei-1-i);
		LineTo(hDC,Pox+Wid-1-i,Poy+i);
		SelectObject(hDC,OldPen);
		DeleteObject(hPen);
	}
  return;

}

//	画3D水平线
void CTools_Win32::Draw3DHoriLine(HDC hDC,int Pox,int Poy,int Wid,HPEN hPen1,HPEN hPen2,HPEN hPen3)
{
	HPEN		OldPen;

	OldPen=(HPEN)SelectObject(hDC,hPen1);
	MoveToEx(hDC,Pox,Poy,NULL);
	LineTo(hDC,Pox+Wid,Poy);

	SelectObject(hDC,hPen2);
	MoveToEx(hDC,Pox,Poy+1,NULL);
	LineTo(hDC,Pox+Wid,Poy+1);

	SelectObject(hDC,hPen3);
	MoveToEx(hDC,Pox,Poy+2,NULL);
	LineTo(hDC,Pox+Wid,Poy+2);

	SelectObject(hDC,OldPen);
}


//	画从上到下渐变的矩形
void CTools_Win32::DrawLeavenRect(HDC hDC,RECT Rect,COLORREF TopCol,COLORREF BtnCol)
{
	HPEN							OldPen,hPen;
	int								i,DeltaR,DeltaG,DeltaB,DeltaY;
	int								TopR,TopG,TopB,BtnR,BtnG,BtnB;

	OldPen=(HPEN)SelectObject(hDC,(HPEN)GetStockObject(BLACK_PEN));
	TopR=(int)GetRValue(TopCol);
	TopG=(int)GetGValue(TopCol);
	TopB=(int)GetBValue(TopCol);
	BtnR=(int)GetRValue(BtnCol);
	BtnG=(int)GetGValue(BtnCol);
	BtnB=(int)GetBValue(BtnCol);
	DeltaR=BtnR-TopR;
	DeltaG=BtnG-TopG;
	DeltaB=BtnB-TopB;
	DeltaY=Rect.bottom-Rect.top;
	if(DeltaY<=0) return;

	for(i=0;i<Rect.bottom-Rect.top;i++)
	{
		hPen=CreatePen(PS_SOLID,1,RGB(TopR+i*DeltaR/DeltaY,TopG+i*DeltaG/DeltaY,TopB+i*DeltaB/DeltaY));
		SelectObject(hDC,hPen);
		
		MoveToEx(hDC,Rect.left,Rect.top+i,NULL);
		LineTo(hDC,Rect.right,Rect.top+i);

		SelectObject(hDC,OldPen);
		DeleteObject(hPen);
	}
}

//	取渐变的颜色值
COLORREF CTools_Win32::GetLeavenColor(COLORREF TopCol,COLORREF BtnCol,int Topy,int Btny,int Cury)
{
	int			DeltaR,DeltaG,DeltaB,DeltaY;
	int			TopR,TopG,TopB,BtnR,BtnG,BtnB;

	TopR=(int)GetRValue(TopCol);
	TopG=(int)GetGValue(TopCol);
	TopB=(int)GetBValue(TopCol);
	BtnR=(int)GetRValue(BtnCol);
	BtnG=(int)GetGValue(BtnCol);
	BtnB=(int)GetBValue(BtnCol);
	DeltaR=BtnR-TopR;
	DeltaG=BtnG-TopG;
	DeltaB=BtnB-TopB;
	DeltaY=Btny-Topy;
	if(Cury<Topy) Cury=Topy;
	if(Cury>Btny) Cury=Btny;
	if(DeltaY<=0) return TopCol;
	else return RGB(TopR+(Cury-Topy)*DeltaR/DeltaY,TopG+(Cury-Topy)*DeltaG/DeltaY,TopB+(Cury-Topy)*DeltaB/DeltaY);
}

//	取两个颜色的中间颜色
COLORREF CTools_Win32::GetAveColor(COLORREF Col1,COLORREF Col2)
{
	return RGB((GetRValue(Col1)+GetRValue(Col2))/2,(GetGValue(Col1)+GetGValue(Col2))/2,(GetBValue(Col1)+GetBValue(Col2))/2);
}



//function		:SleepByEvt
//description	:1. sleep ms
void CTools_Win32::SleepByEvt(int TimeoutMS)
{
	HANDLE	hEvent=NULL;
	hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
	if(hEvent&&TimeoutMS>0)
		WaitForSingleObject(hEvent,TimeoutMS);
	if(hEvent)
		CloseHandle(hEvent); 
}


void CTools_Win32::GetWorkAreaSize(int &ScnWid,int &ScnHei)
{
	RECT			Rect;

	SystemParametersInfo(SPI_GETWORKAREA,0,&Rect,0);
	ScnWid=Rect.right-Rect.left;
	ScnHei=Rect.bottom-Rect.top;
}


void CTools_Win32::GetMainPath(char *pMainPath,int iSize)
{
	int i;

	if(!pMainPath) return;
	if(iSize<2) return;

	memset(pMainPath,0,iSize);
	GetModuleFileName(NULL,pMainPath,iSize-1);
	i=strlen(pMainPath);
	while(i>0&&pMainPath[i-1]!='\\'&&pMainPath[i-1]!=':') 
	pMainPath[--i]=0;
	if(pMainPath[i-1]!='\\')
	strcat((char*)pMainPath,"\\");
}

void CTools_Win32::CheckMainPath(char *pRltMainPath,int iSize,const char *refMainPath)
{
	if(!pRltMainPath) return;
	if(iSize<2) return;

	if(refMainPath&&refMainPath[0])
		strncpy(pRltMainPath,refMainPath,iSize-2);
	else
		GetMainPath(pRltMainPath,iSize-2);

	if(pRltMainPath[strlen(pRltMainPath)-1]!='\\')
		strcat(pRltMainPath,"\\");
}


//	创建多级目录
void CTools_Win32::CreateMultipleDirectory(char *iPath)
{
	int			i,j,Len;
	char		iBuf[256];

	Len=strlen(iPath);
	if(Len>254) Len=254;
	i=0;
	j=0;
	while(i<Len&&j<20)
	{
		while(i<Len&&(iPath[i]!='\\'||i==0||iPath[i-1]==':')) i++;
		memcpy(iBuf,iPath,i);
		iBuf[i]=0;
		CreateDirectory(iBuf,NULL);
		i++;
		j++;
	}
}

//	在串中用环境变量值替换%%包含的部分
void CTools_Win32::ReplaceWithEnvironmentVariable(char *pBuf,int BufSize)
{
	int				i,j,Len,VarLen;
	char			Var[256];
	char			Env[256];
	BOOL			bReplace;

	while(1)
	{
		bReplace=FALSE;
		Len=strlen(pBuf);
		i=j=-1;

		for(i=0;i<Len;i++) if(pBuf[i]=='%') break;
		if(i<Len)
			for(i++,j=i;j<Len;j++) if(pBuf[j]=='%') break;

		if(i>=0&&i<Len&&j>=0&&j<Len&&j>i&&j-i<sizeof(Env))
		{
			memcpy(Env,pBuf+i,j-i);
			Env[j-i]=0;
			Var[0]=Var[sizeof(Var)-1]=0;
			GetEnvironmentVariable(Env,Var,sizeof(Var)-1);
			VarLen=strlen(Var);

			if(Len-2-(j-i)+VarLen<BufSize)
			{
				memmove(pBuf+i-1+VarLen,pBuf+j+1,Len-(j+1));
				memcpy(pBuf+i-1,Var,VarLen);
				pBuf[Len-2-(j-i)+VarLen]=0;
				bReplace=TRUE;
			}
		}
		if(!bReplace) break;
	}
}

//	将Rect从屏幕坐标转换成窗口的Client坐标
void CTools_Win32::ScreenToClientRect(HWND hWnd,RECT *pRect)
{
	POINT		Pt;
	Pt.x=pRect->left;
	Pt.y=pRect->top;
	ScreenToClient(hWnd,&Pt);
	pRect->right=(pRect->right-pRect->left+Pt.x);
	pRect->bottom=(pRect->bottom-pRect->top+Pt.y);
	pRect->left=Pt.x;
	pRect->top=Pt.y;
}

//	将Rect从窗口的Client坐标转换成屏幕坐标
void CTools_Win32::ClientToScreenRect(HWND hWnd,RECT *pRect)
{
	POINT		Pt;
	Pt.x=pRect->left;
	Pt.y=pRect->top;
	ClientToScreen(hWnd,&Pt);
	pRect->right=(pRect->right-pRect->left+Pt.x);
	pRect->bottom=(pRect->bottom-pRect->top+Pt.y);
	pRect->left=Pt.x;
	pRect->top=Pt.y;
}


//	改变一个子窗口的大小，位置不变
void CTools_Win32::ResizeChildWindow(HWND hWnd,HWND hParentWnd,int NewWid,int NewHei,BOOL RePaintID)
{
	RECT iRect;
	GetWindowRect(hWnd,&iRect);
	ScreenToClientRect(hParentWnd,&iRect);
	MoveWindow(hWnd,iRect.left,iRect.top,NewWid,NewHei,RePaintID);
}

//	移动窗口位置和/或改变窗口大小，使用相对位置和相对尺寸，可以是负值
void CTools_Win32::MyMoveWindow(HWND hWnd,int DeltaPox,int DeltaPoy,int DeltaWid,int DeltaHei)
{
	HWND			hParent;
	RECT			Rect;
	int				Pox,Poy,Wid,Hei;
	
	GetWindowRect(hWnd,&Rect);
	hParent=GetParent(hWnd);
	if(hParent!=NULL&&hParent!=hWnd)
		ScreenToClientRect(hParent,&Rect);
	Pox=Rect.left;
	Poy=Rect.top;
	Wid=Rect.right-Rect.left;
	Hei=Rect.bottom-Rect.top;

	Pox+=DeltaPox;
	Poy+=DeltaPoy;
	Wid+=DeltaWid;
	Hei+=DeltaHei;

	MoveWindow(hWnd,Pox,Poy,Wid,Hei,TRUE);
}


//	从编辑框窗口取出值
int CTools_Win32::MyGetEditWndVal(HWND iWnd)
{
	char			Buf[128];
	int				i,Len;
	Buf[0]=0;
	GetWindowText(iWnd,Buf,100);
	for(i=0,Len=strlen(Buf);i<Len&&(Buf[i]==' '||Buf[i]=='\t');i++);
	return atoi(Buf);
}

void CTools_Win32::MyGetEditWndStr(HWND iWnd,char *Buf,int BufSize)
{
	if(BufSize<=0) return;

	GetWindowText(iWnd,Buf,BufSize);
	if(Buf[BufSize-1]) Buf[BufSize-1]=0;
}

void CTools_Win32::MySetEditWndVal(HWND iWnd,int Val)
{
	char	Buf[128];
	sprintf(Buf,"%d",Val);
	SetWindowText(iWnd,Buf);
}

//function		:IsFileUpdate
//description	:tell you whether the file was update 
BOOL CTools_Win32::IsFileUpdate(char *FileName,struct _stat *pLastFileStat)
{
	struct _stat	CurIniFileStat;
	BOOL			bRlt;

	memset((char*)&CurIniFileStat,0,sizeof(CurIniFileStat));
   _stat(FileName,&CurIniFileStat);
   if(	CurIniFileStat.st_ctime!=pLastFileStat->st_ctime||
		CurIniFileStat.st_mtime!=pLastFileStat->st_mtime||
		CurIniFileStat.st_size!=pLastFileStat->st_size)
		bRlt=TRUE;
   else bRlt=FALSE;
   memcpy((char*)pLastFileStat,(char*)&CurIniFileStat,sizeof(CurIniFileStat));
   return bRlt;
}



//	取当前Cpu的Tick的数量
LONGLONG CTools_Win32::MyGetCpuTickCounter(void)
{
	LARGE_INTEGER	CpuCount;
	QueryPerformanceCounter(&CpuCount);
	return CpuCount.QuadPart;
}


//	取当前Cpu的Tick的频率，单位为每秒多少次
LONGLONG CTools_Win32::MyGetCpuTickFreq(void)
{
	LARGE_INTEGER	CpuFreq;
	QueryPerformanceFrequency(&CpuFreq);
	return CpuFreq.QuadPart;
}


//	取当前的复杂时间，64位，高32位为time_t，低32位的高16位为毫秒数，余下16位是为了区分秒-毫秒数相同的时间而设置的计数器
LONGLONG CTools_Win32::MyGetComplexTime_Exclusize(void)
{
	static time_t	LastSecs=0;
	static int		LastMS=0;
	static int		LastMSCount=0;
	struct _timeb	itime;

	_ftime(&itime);
	if(LastSecs==itime.time&&LastMS==itime.millitm) LastMSCount++;
	else LastMSCount=0;
	LastSecs=itime.time;
	LastMS=itime.millitm;

	return (((LONGLONG)LastSecs)<<32) + (((LONGLONG)LastMS)<<16) + ((LONGLONG)LastMSCount);
}

//	获取当前系统内存使用情况，包括内存使用百分比、全部内存大小，已使用内存大小
void CTools_Win32::GetSystemMemeryUsage(DWORD *pCurUsagePercent,DWORD *pTotalMemSize,DWORD *pRltUsedMemSize)
{
	MEMORYSTATUS MemStatus;

	GlobalMemoryStatus(&MemStatus);
	if(pCurUsagePercent) 
		*pCurUsagePercent=MemStatus.dwMemoryLoad;
	if(pTotalMemSize)
		*pTotalMemSize=MemStatus.dwTotalPhys;
	if(pRltUsedMemSize)
		*pRltUsedMemSize=MemStatus.dwTotalPhys>=MemStatus.dwAvailPhys?MemStatus.dwTotalPhys-MemStatus.dwAvailPhys:0;
}


//	判断操作系统是否是WindowsNT以上的版本
BOOL CTools_Win32::IsWindowsNt(void)
{
	if(GetVersion()<0x80000000) return TRUE;
	else return FALSE;
}


//申请读写信号量，成功返回指针，失败返回0
char* CTools_Win32::AllocWinSem(void)
{
	char			*pSem;

	pSem=(char*)new CRITICAL_SECTION;
	if(pSem) 
		InitializeCriticalSection((CRITICAL_SECTION*)pSem);

	return pSem;
}

//释放读写信号量
void CTools_Win32::ReleaseWinSem(char* pSem)
{
	if(pSem)
	{														//如果信号量正在被锁定，等待其释放
		TakeWinSem(pSem);
		GiveWinSem(pSem);
	}
		
	try
	{
		DeleteCriticalSection((CRITICAL_SECTION*)pSem);
		delete (CRITICAL_SECTION*)pSem;
	}
	catch(...)
	{
	}
}

//要求独占信号量
void CTools_Win32::TakeWinSem(char* pSem)
{
	if(pSem)
	{
		try
		{
			EnterCriticalSection((CRITICAL_SECTION*)pSem);
		}
		catch(...)
		{
		}
	}
}

//停止独占信号量
void CTools_Win32::GiveWinSem(char* pSem)
{
	if(pSem)
	{
		try
		{
			LeaveCriticalSection((CRITICAL_SECTION*)pSem);
		}
		catch(...)
		{
		}
	}
}

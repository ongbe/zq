#ifndef _MEMDC_H_
#define _MEMDC_H_

#pragma once


#include "CommPublic.h"
#include "LineChartCtrl.h"
#include <vector>
#include <set>
using namespace std;

//////////////////////////////////////////////////
// CustomMemDC - memory DC
//////////////////////////////////////////////////////////////////////////

//#include "LogFile.h"
//#define NormalDistributionData "data.log"

class CustomMemDC : public CDC {
private:	
    CBitmap		m_bitmap;		// Offscreen bitmap
    CBitmap*	m_oldBitmap;	// bitmap originally found in CMemDC
    CDC*		m_pDC;			// Saves CDC passed in constructor
    CRect		m_rect;			// Rectangle of drawing area.
	CRect		m_rectTop;	    // Rectangle of drawing top area.
    CRect		m_rectBottom;   // Rectangle of drawing bottom area.
    BK_COLOR_MODE m_ColorMode;
    BOOL		m_bMemDC;		// TRUE if CDC really is a Memory DC.
   

    /// 画图参数
    CHART_TYPE  m_ChartType;
    //所画区域
    int m_nRectLeft,m_nRectRight,m_nRectTop,m_nRectBottom,m_nRectWidth,m_nRectHeight;
    //x轴物理和逻辑坐标换算 参考点
    double  m_dXAxisPosMin, m_dXAxisPosMax;//x物理坐标最大值和最小值
    double  m_dXAxisPosValueMin, m_dXAxisPosValueMax;//x逻辑坐标最大值和最小值(行权价)    

    double  m_dYAxisPosMin, m_dYAxisPosMax;//y物理坐标最大值和最小值
    double  m_dYAxisPosValueMin, m_dYAxisPosValueMax;//y逻辑坐标最大值和最小值(最大损益权利金)

    double  m_dBalanceXPos,m_dBalanceYPos;//损益平衡点物理坐标

    double  m_dXQuotPricePos, m_dXQuotPricePosValue;//最新价坐标

    

public:
    CustomMemDC(CDC* pDC, 
                const CRect* pRectTop,
                const CRect* pRectBottom,
                BK_COLOR_MODE colorMode,
                CHART_TYPE type) : CDC()                              
    {
        ASSERT(pDC != NULL); 

        // Some initialization
        m_pDC = pDC;
        m_oldBitmap = NULL;
        m_bMemDC = !pDC->IsPrinting();

        m_rectTop = *pRectTop;
        m_rect = m_rectBottom = *pRectBottom;
        m_ColorMode = colorMode;
        m_ChartType = type;

        m_nRectLeft = m_rectTop.left;
        m_nRectRight = m_rectTop.right;
        m_nRectTop = m_rectTop.top;
        m_nRectBottom = m_rectTop.bottom;
        m_nRectWidth = m_rectTop.Width();
        m_nRectHeight = m_rectTop.Height();

        if (m_bMemDC) 
        {
            // Create a Memory DC
            CreateCompatibleDC(pDC);
            pDC->LPtoDP(&m_rect);

            m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
            m_oldBitmap = SelectObject(&m_bitmap);

            SetMapMode(pDC->GetMapMode());
            pDC->DPtoLP(&m_rect);
            SetWindowOrg(m_rect.left, m_rect.top);
        } 
        else 
        {
            // Make a copy of the relevent parts of the current DC for printing
            m_bPrinting = pDC->m_bPrinting;
            m_hDC       = pDC->m_hDC;
            m_hAttribDC = pDC->m_hAttribDC;
        }

        // draw background 
        DrawBackground();

        if(CDataInfo::NewInstance()->GetCaculateFlag())
            DrawElements();

        DrawBorder();
    }
    void DrawBackground()
    {
        FillSolidRect(m_rectBottom, (m_ColorMode == Color_Light)?RGB(249,249,249):RGB(9,58,117));
        FillSolidRect(m_rectTop   , (m_ColorMode == Color_Light)?RGB(234,234,234):RGB(0,0,0));
    }

    void DrawBorder()
    {
        CPen pen(PS_SOLID,1,RGB(149,149,149));
        CPen *oldPen = SelectObject(&pen);

        MoveTo(m_rectTop.left,m_rectTop.top);
        LineTo(m_rectTop.right,m_rectTop.top);
        LineTo(m_rectTop.right,m_rectTop.bottom);
        LineTo(m_rectTop.left,m_rectTop.bottom);
        LineTo(m_rectTop.left,m_rectTop.top);

        SelectObject(oldPen);
    }

    void DrawElements()
    {
        if(!CDataInfo::NewInstance()->GetInit())
            return;

        CPen pen(PS_SOLID,1,RGB(149,149,149));
        CPen *oldPen = SelectObject(&pen); 

        CFont myFont; 
        LOGFONT lFont; 
        memset(&lFont, 0, sizeof(LOGFONT)); 
        lFont.lfHeight = 14;
        lFont.lfWeight = 14;
        BOOL bRes = myFont.CreateFontIndirect(&lFont); 
        ASSERT(bRes); 
        CFont * pOldFont = SelectObject(&myFont); 

        SetBkMode(TRANSPARENT);
        SetTextColor((m_ColorMode == Color_Light)?RGB(30,30,30):RGB(249,249,249));


        CString str;
        bool bBalance;
        vector<sPoint>     vPos;        //所有转折点和平衡点逻辑坐标 

        //x轴刻度
        vector<double> vXAxisPos;
        double dPriceMin, dSpaceValue, dXPosValueMin, dXPosValueMax;
        CDataInfo::NewInstance()->GetXAxisInfo(vXAxisPos,dPriceMin,dSpaceValue,dXPosValueMin,dXPosValueMax);
        DrawXAxis(vXAxisPos, dPriceMin, dSpaceValue,dXPosValueMin,dXPosValueMax);

        if(eProfitChart == m_ChartType)
        {       
            //(假定)确定Y轴物理坐标最值
            m_dYAxisPosMin = m_nRectBottom - (m_nRectHeight*0.15);
            m_dYAxisPosMax = m_nRectTop + (m_nRectHeight*0.15);
        }
        else if(eProbaChart == m_ChartType)
        {
            //(假定)确定Y轴物理坐标最值
            m_dYAxisPosMin = m_nRectBottom;
            m_dYAxisPosMax = m_nRectTop + (m_nRectHeight*0.10);
        }

        CDataInfo::NewInstance()->GetYPosValue(m_ChartType,m_dYAxisPosValueMin,m_dYAxisPosValueMax);

        if (eProfitChart == m_ChartType)
        {    
            vector<_POINT> _vPoint;
            bBalance = CDataInfo::NewInstance()->GetPos(_vPoint);

            //填充嬴损区域
            DrawRect(bBalance, _vPoint);

            //画盈亏平衡点
            DrawBalancePos();  
        }
        else if(eProbaChart == m_ChartType)
        {
            bool bLostRect = false;
            double dPrice = m_dXAxisPosValueMin;
            double dXposValueStart = dPrice;
            double dValue = 0.0;

            _POINT _PT, _PTAdd;
            vector<_POINT> vPoints;

            for (; dPrice < m_dXAxisPosValueMax; dPrice += 0.2)
            {                
                if( IsBalancePos(dPrice) )
                {
                    _PTAdd.x = GetXPos(dPrice);
                    _PTAdd.y = GetYPos(m_dYAxisPosValueMin);
                    vPoints.push_back(_PTAdd);
                    
                    bLostRect = (CDataInfo::NewInstance()->GetYPosValue(dPrice - 0.5) < 0);
                    DrawProbaChart(false, bLostRect, _PTAdd, vPoints);
                    
                    dValue = 0.0;
                    dXposValueStart = dPrice;
                    vPoints.clear();


                    vPoints.push_back(_PTAdd);
                }
                else
                {
                    _PT.x = GetXPos(dPrice);
                    _PT.y = GetYPos(CDataInfo::NewInstance()->GetProbaYPosValue(dPrice));
                    vPoints.push_back(_PT);
                }

                dValue += _PT.y;
            }

            _PTAdd.x = GetXPos(dXposValueStart);
            _PTAdd.y = GetYPos(m_dYAxisPosValueMin);
            vPoints.insert(vPoints.begin(),_PTAdd);

            bLostRect = (CDataInfo::NewInstance()->GetYPosValue(dPrice - 0.5) < 0);
            DrawProbaChart(true, bLostRect, _PTAdd, vPoints);

            DrawArea();
        }

        //画最新价  
        DrawQuotPrice();

        //画十字交叉线
        DrawCorss(m_ChartType);


        //释放GDI资源 
        SelectObject(oldPen);
        SelectObject(pOldFont); 
        bRes = myFont.DeleteObject(); 
        ASSERT(bRes); 
        pOldFont = NULL; 
    }

    ~CustomMemDC()	
    {		
        if (m_bMemDC) {
            // Copy the offscreen bitmap onto the screen.
            m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
                this, m_rect.left, m_rect.top, SRCCOPY);			

            //Swap back the original bitmap.
            SelectObject(m_oldBitmap);
        } else {
            // All we need to do is replace the DC with an illegal value,
            // this keeps us from accidently deleting the handles associated with
            // the CDC that was passed to the constructor.			
            m_hDC = m_hAttribDC = NULL;
        }	

        //if(m_pLog)
        //{
        //    delete m_pLog;
        //    m_pLog = NULL;
        //}
    }

    // Allow usage as a pointer	
    CustomMemDC* operator->() 
    {
        return this;
    }	

    // Allow usage as a pointer	
    operator CustomMemDC*() 
    {
        return this;
    }

    //////////////////////////////////////////////////////////////////////////
    void DrawXAxis(const vector<double>& vXAxisPos, const double& dPriceMin, const double& dSpaceValue, const double& dXPosValueMin,const double& dXPosValueMax)
    {
        double dXAxisPos, dXAxisPosValue;
        CString str;
        int i = 0;
        for(vector<double>::const_iterator it = vXAxisPos.begin();it != vXAxisPos.end();it++,i++)
        {
            dXAxisPos = *it;
            MoveTo((int)dXAxisPos,   m_nRectBottom);
            LineTo((int)dXAxisPos,   m_nRectBottom-4); 

            dXAxisPosValue = (dPriceMin+i*dSpaceValue);
            str = Price2String(dXAxisPosValue,CDataInfo::NewInstance()->GetInstPriceTick());
            DrawText(str, CRect((int)dXAxisPos-12, m_nRectBottom+2,(int)dXAxisPos+25,m_nRectBottom+15), DT_SINGLELINE);
        }

        //确定X轴物理坐标最值
        m_dXAxisPosMin = m_nRectLeft;
        m_dXAxisPosMax = m_nRectRight;

        //确定X轴逻辑坐标最值
        m_dXAxisPosValueMin =  dXPosValueMin;
        m_dXAxisPosValueMax =  dXPosValueMax;

        //确定最新价物理坐标
        m_dXQuotPricePosValue = CDataInfo::NewInstance()->GetLastPrice();
        m_dXQuotPricePos = GetXPos(m_dXQuotPricePosValue);
    }
    /*
    画最新价
    */
    void DrawQuotPrice()
    {
        SetTextColor((m_ColorMode == Color_Light)?RGB(30,30,30):RGB(249,249,249));

        CString str;
        POINT   ptLastPrice[3];
        ptLastPrice[0] = CPoint((int)m_dXQuotPricePos,       m_nRectBottom-2);
        ptLastPrice[1] = CPoint((int)m_dXQuotPricePos - 4,   m_nRectBottom - 7);
        ptLastPrice[2] = CPoint((int)m_dXQuotPricePos + 4,   m_nRectBottom - 7);

        CBrush brushQuotPrice(RGB(52,181,63));//((m_ColorMode==Color_Light)?RGB(229,181,169):RGB(255,4,0));
        SelectObject(&brushQuotPrice);
        Polygon(ptLastPrice,3);

        str = Price2String(m_dXQuotPricePosValue,CDataInfo::NewInstance()->GetInstPriceTick());
        DrawText(str, CRect((int)m_dXQuotPricePos-12, m_nRectBottom-20, (int)m_dXQuotPricePos+25, m_nRectBottom+4), DT_SINGLELINE);
    }
    void DrawBalancePos()
    {
        _POINT _PT;
        double dBalanceXPos, dBalanceYPos;
        vector<sBalancePt> vBalancePos;
        CDataInfo::NewInstance()->GetBalancePoint(vBalancePos);

        if(vBalancePos.empty())
        {        
            //画Y轴0刻度   
            CString str;
            str.Format(_T("%s"), "0");
            DrawText(str, CRect(m_nRectLeft-7, (int)m_dBalanceYPos-7,m_nRectLeft+4,(int)m_dBalanceYPos+4), DT_SINGLELINE);
            return;
        }
        else 
        {
            for (vector<_POINT>::iterator it = vBalancePos.begin(); it != vBalancePos.end(); it++)
            {
                _PT = *it;

                dBalanceXPos = GetXPos(_PT.x);
                dBalanceYPos = GetYPos(_PT.y);

                CString str;
                CBrush brushBalance(RGB(193,146,2));
                SelectObject(&brushBalance);
                Ellipse((int)dBalanceXPos-5, (int)dBalanceYPos-5, (int)dBalanceXPos+5, (int)dBalanceYPos+5);

                str = Price2String(_PT.x,CDataInfo::NewInstance()->GetInstPriceTick());
                SetTextColor(RGB(193,146,2));
                DrawText(str, CRect((int)dBalanceXPos-20, (int)dBalanceYPos+4, (int)dBalanceXPos+15,(int)dBalanceYPos+18), DT_SINGLELINE); 
                
                m_dBalanceYPos = GetYPos(_PT.y);
            }
        }

        SetTextColor((m_ColorMode == Color_Light)?RGB(30,30,30):RGB(249,249,249));

        //画Y轴0刻度   
        CString str;
        str.Format(_T("%s"), "0");
        DrawText(str, CRect(m_nRectLeft-7, (int)m_dBalanceYPos-7,m_nRectLeft+4,(int)m_dBalanceYPos+4), DT_SINGLELINE);
    }
    void DrawRect(bool bBalance, vector<_POINT> _vPoint)
    {
        _POINT _PT, _PTAdd, _PTStart;
        vector<_POINT> _vPos;       

        bool bResult,bLostRect;
        double dValue = 0.0;
        vector<_POINT>::iterator it = _vPoint.begin();
        vector<_POINT>::iterator itStart = it, itDraw;

        if(!bBalance)
        {  
            _PT = *it;
            bLostRect = _PT.y < 0; 

            if(m_dYAxisPosValueMin == m_dYAxisPosValueMax) 
            {
                if(m_dYAxisPosValueMax == 0)
                {
                    m_dBalanceYPos = m_nRectTop;
                    return;
                }

                m_dBalanceYPos = m_nRectTop + m_nRectHeight/2;
            }
            else
            {
                if(m_dYAxisPosValueMin > 0)
                    m_dBalanceYPos = m_nRectBottom;
                else if(m_dYAxisPosValueMax < 0)
                    m_dBalanceYPos = m_nRectTop;
            }

            DrawRect(bBalance,bLostRect,_vPoint);
            return;
        }

        for (;it != _vPoint.end();it++)
        {
            _PT = *it;
            if(itStart!=_vPoint.end())
                _PTStart = *itStart;

            bResult = IsBalancePos(_PT);
            if(bResult)
            {
                _vPos.push_back(_PT);
                
                _PTAdd.x = _PTStart.x;
                _PTAdd.y = _PT.y;
                _vPos.push_back(_PTAdd);

                bLostRect = dValue < 0;

                DrawRect(bBalance,bLostRect,_vPos);

                dValue = 0;
                _vPos.clear();
                _vPos.push_back(_PT); 
                itStart = it;
            }
            else
                _vPos.push_back(_PT);

            dValue += _PT.y;
        }

        if(it == _vPoint.end())
        {
            if(_PT.y == _PTStart.y)
            {
                if(_PT.y > 0)
                    bLostRect = false;
                else if(_PT.y < 0)
                    bLostRect = true;

                _PTAdd.x = _PT.x;
                _PTAdd.y = 0.0;            
                _vPos.push_back(_PTAdd);

                _PTAdd.x = _PTStart.x;
                _PTAdd.y = 0.0;            
                _vPos.push_back(_PTAdd);
            }
            else
            {
                bLostRect = dValue < 0;

                _PTAdd.x = _PT.x;
                _PTAdd.y = _PTStart.y;            
                _vPos.push_back(_PTAdd);
            }

            DrawRect(bBalance,bLostRect,_vPos);
            _vPos.clear();
        }
    }
    bool IsBalancePos(_POINT pt)
    {
        _POINT _PT;
        vector<sBalancePt> vBalancePos;
        CDataInfo::NewInstance()->GetBalancePoint(vBalancePos);
        for(vector<_POINT>::iterator it = vBalancePos.begin(); it!=vBalancePos.end();it++)
        {
            _PT = *it;

            if(_PT == pt)
                return true;
        }

        return false;
    }
    bool IsBalancePos(double dXposValue)
    {
        _POINT _PT;
        vector<sBalancePt> vBalancePos;
        CDataInfo::NewInstance()->GetBalancePoint(vBalancePos);
        for(vector<_POINT>::iterator it = vBalancePos.begin(); it!=vBalancePos.end();it++)
        {
            _PT = *it;

            if(fabs(dXposValue - _PT.x)<0.1)
                return true;
        }

        return false;
    }
    void DrawRect(bool bBalance, bool bLostRect, vector<_POINT> _vPos)
    {   
        int i,nSize;
        _POINT ptAdd;
        POINT *point = NULL;
        CBrush brush(((m_ColorMode==Color_Light)?(bLostRect?RGB(150,199,97):RGB(229,181,169)):(bLostRect?RGB(0,132,0):RGB(140,0,0))));        

        LPtranslatetoDP(_vPos);

        if(!bBalance)
        {
            ptAdd.x = m_dXAxisPosMin;
            ptAdd.y = m_nRectTop;//m_dBalanceYPos;            
            _vPos.insert(_vPos.begin(),ptAdd);

            ptAdd.x = m_dXAxisPosMax;
            ptAdd.y = m_nRectTop;//m_dBalanceYPos;            
            _vPos.insert(_vPos.end(),ptAdd);
        }

        nSize = _vPos.size();
        point = new POINT[nSize]; 
        for (i=0;i<nSize;i++)
        {
            point[i].x = (LONG)_vPos[i].x;
            point[i].y = (LONG)_vPos[i].y;
        }

        SelectObject(&brush);
        Polygon(point,nSize);  

        if(point)
        {
            delete[] point;
            point = NULL;
        }
    }
    void DrawArea(bool bLast, bool bLostRect,_POINT ptBalance)
    {
        CString str;
        double dArea=0.0,dBalanceXPos;
        static double dAreaRest=0.0;
        static double dArea2=0.0;
        int left=0,top=0,width=0,height=0;
        static bool bFlag = true;
        COLORREF textREF = bLostRect?RGB(0,255,0):RGB(255,0,0);

        dBalanceXPos = GetXPosValue(ptBalance.x);        
        dArea = bLast?(1-dAreaRest):CDataInfo::NewInstance()->GetArea(dBalanceXPos);
        dAreaRest = dArea;
        if(!bLast) dArea -= dArea2;

        str.Format(_T("%6.2f%%"), dArea*100.0);
        SetTextColor(textREF);

        left = ptBalance.x + ((bFlag)?-45:0);
        top = m_nRectTop+(m_nRectHeight/2);
        width = left + 45;
        height = top + 30;

        DrawText(str, CRect(left, top, width, height), DT_SINGLELINE);

        dArea2 = dArea;
        bFlag = !bFlag;
    }
    void DrawArea()
    {
        bool bLostRect;
        COLORREF textREF;
        CString str;
        double dArea=0.0,dArea2=0.0,dAreaRest=0.0,dBalanceXPos;
        _POINT _PT;
        vector<sBalancePt> vBalancePos;
        CDataInfo::NewInstance()->GetBalancePoint(vBalancePos);
        if(vBalancePos.empty())
            return;

        int i=1;
        for (vector<_POINT>::iterator it = vBalancePos.begin(); it != vBalancePos.end(); it++,i++)
        {
            _PT = *it;
            bLostRect = (CDataInfo::NewInstance()->GetYPosValue(_PT.x - 0.5) < 0);
            textREF = bLostRect?RGB(0,255,0):RGB(255,0,0);
            dBalanceXPos = GetXPos(_PT.x);

            dArea = CDataInfo::NewInstance()->GetArea(_PT.x);  
            dArea2 = dArea;
            dArea -= dAreaRest;

            str.Format(_T("%6.2f%%"), dArea*100.0);
            SetTextColor(textREF);
            DrawText(str, CRect((int)dBalanceXPos-45, m_nRectTop+((i%2)?(m_nRectHeight/2):(m_nRectHeight/2.4)), (int)dBalanceXPos,m_nRectTop+((i%2)?(m_nRectHeight/2):(m_nRectHeight/2.4))+30), DT_SINGLELINE);

            dAreaRest = dArea;
        }


        str.Format(_T("%6.2f%%"), (1-dArea2)*100.0);

        bLostRect = (CDataInfo::NewInstance()->GetYPosValue(_PT.x + 0.5) < 0);
        textREF = bLostRect?RGB(0,255,0):RGB(255,0,0);

        SetTextColor(textREF);
        DrawText(str, CRect((int)dBalanceXPos+2, m_nRectTop+((i%2)?(m_nRectHeight/2):(m_nRectHeight/2.4)), (int)dBalanceXPos+45,m_nRectTop+((i%2)?(m_nRectHeight/2):(m_nRectHeight/2.4))+30), DT_SINGLELINE);
    }
    void DrawCorss(CHART_TYPE eType)
    {
        CString str;
        bool   bMouseIn = CDataInfo::NewInstance()->GetMouseIn();
        CPoint ptMouse = CDataInfo::NewInstance()->GetMousePt();
        bool bDraw = ( bMouseIn && (m_rectTop.left <= ptMouse.x && ptMouse.x <= m_rectTop.right) );
        if( bDraw)
        {
            CBrush brush(RGB(52,181,63));
            SelectObject(&brush);

            //垂直线
            MoveTo(ptMouse.x,m_nRectTop);
            LineTo(ptMouse.x,m_nRectBottom);

            POINT pt[3];
            pt[0] = CPoint(ptMouse.x,       m_nRectBottom-2);
            pt[1] = CPoint(ptMouse.x - 4,   m_nRectBottom - 7);
            pt[2] = CPoint(ptMouse.x + 4,   m_nRectBottom - 7);
            Polygon(pt,3);

            //鼠标x物理坐标对应的逻辑坐标
            double dXAxisPosValue,dYAxisPosValue;
            dXAxisPosValue = GetXPosValue(ptMouse.x);
            
            str = Price2String(dXAxisPosValue,CDataInfo::NewInstance()->GetInstPriceTick());
            SetTextColor(RGB(147,38,255));
            DrawText(str, CRect(ptMouse.x+2, m_nRectBottom-20, ptMouse.x+35,m_nRectBottom+8), DT_SINGLELINE);

            //draw y Asix
            int nPos;
            dYAxisPosValue = CDataInfo::NewInstance()->GetYPosValue(dXAxisPosValue);
            nPos = (int)GetYPos(dYAxisPosValue);

            //平行线
            if(eProbaChart == m_ChartType)
            {
                double dArea,dArea2,dAreaRest=0.0;               
                dArea = CDataInfo::NewInstance()->GetArea(dXAxisPosValue);
                dArea2 = 1-dArea;
                str.Format(_T("%6.2f"), dArea*100.0);
                str += "%";
                SetTextColor(RGB(0,132,0));
                DrawText(str, CRect(ptMouse.x-45, m_nRectTop+10, ptMouse.x,m_nRectTop+30), DT_SINGLELINE);

                str.Format(_T("%6.2f"), dArea2*100.0);
                str += "%";
                SetTextColor(RGB(254,4,0));
                DrawText(str, CRect(ptMouse.x+2, m_nRectTop+10, ptMouse.x+45,m_nRectTop+30), DT_SINGLELINE);

                return;
            }

            MoveTo(m_nRectLeft,nPos);
            LineTo(m_nRectRight,nPos);

            pt[0] = CPoint(m_nRectLeft - 2,   nPos);
            pt[1] = CPoint(m_nRectLeft - 7,   nPos - 4);
            pt[2] = CPoint(m_nRectLeft - 7,   nPos + 4);
            Polygon(pt,3);

            str = Price2String(dYAxisPosValue,CDataInfo::NewInstance()->GetInstPriceTick());
            SetTextColor(RGB(147,38,255));
            DrawText(str, CRect(m_nRectLeft+2, nPos-14, m_nRectLeft+55,nPos-1), DT_SINGLELINE);
        }
    }

    void LPtranslatetoDP(vector<_POINT>& _vPots)
    {
        vector<_POINT> vLP(_vPots);

        _vPots.clear();

        _POINT _PTTemp, _PT;

        for (vector<_POINT>::iterator it = vLP.begin();it != vLP.end();it++)
        {
            _PTTemp = *it;
            _PT.x = GetXPos(_PTTemp.x);
            _PT.y = GetYPos(_PTTemp.y);

            _vPots.push_back(_PT);
        }
    }
    /*
    通过Y轴逻辑坐标值得到物理坐标值
    输入：
        dYPosValue Y的逻辑坐标       
    输出：
        Y的物理坐标
    */
    double GetYPos(const double& dYPosValue)
    {
        if(eProfitChart == m_ChartType)
        {       
            //(假定)确定Y轴物理坐标最值
            m_dYAxisPosMin = m_nRectBottom - (m_nRectHeight*0.15);
            m_dYAxisPosMax = m_nRectTop + (m_nRectHeight*0.15);  
        }
        else if(eProbaChart == m_ChartType)
        {
            //(假定)确定Y轴物理坐标最值
            m_dYAxisPosMin = m_nRectBottom;
            m_dYAxisPosMax = m_nRectTop + (m_nRectHeight*0.10); 
        }

        if(fabs(m_dYAxisPosValueMax-m_dYAxisPosValueMin) == 0)
        {
            if(m_dYAxisPosValueMax == 0)
                return -m_nRectTop;
            else if(m_dYAxisPosValueMax > 0)
                return m_dYAxisPosMax;
            else
                return m_dYAxisPosMin;          
        }

        double dValue = m_dYAxisPosMin + (dYPosValue-m_dYAxisPosValueMin)*(m_dYAxisPosMax-m_dYAxisPosMin)/(m_dYAxisPosValueMax-m_dYAxisPosValueMin);

        if(dValue<m_nRectTop)
            dValue = m_nRectTop;
        if(dValue>m_nRectBottom)
            dValue = m_nRectBottom;

        return dValue;
    }
    /*
    通过X轴逻辑坐标值得到物理坐标值
    输入：
        dXPosValue X的逻辑坐标       
    输出：
        X的物理坐标
    */
    double GetXPos(const double& dXPosValue)
    {
        if(fabs(m_dXAxisPosValueMax-m_dXAxisPosValueMin)<=0.000001)
            return m_nRectLeft;

        double dValue = (dXPosValue*(m_dXAxisPosMax-m_dXAxisPosMin) + m_dXAxisPosValueMax*m_dXAxisPosMin - m_dXAxisPosValueMin*m_dXAxisPosMax)/(m_dXAxisPosValueMax-m_dXAxisPosValueMin);
        if(dValue<m_nRectLeft)
            dValue = m_nRectLeft;
        if(dValue>m_nRectRight)
            dValue = m_nRectRight;
        return dValue;
    }
    /*
    通过X轴物理坐标值得到逻辑坐标值
    输入：
        dXPos X的物理坐标       
    输出：
        X的逻辑坐标
    */
    double GetXPosValue(const double& dXPos)
    {
        return ( m_dXAxisPosValueMin*m_dXAxisPosMax - m_dXAxisPosValueMax*m_dXAxisPosMin + (m_dXAxisPosValueMax-m_dXAxisPosValueMin)*dXPos )/(m_dXAxisPosMax-m_dXAxisPosMin);
    }
   /*
    画盈亏区域
    输入：
        bLast       是否是最后一个区域
        bLostRect   是否是亏损区域
        _ptBalance  平衡点
        _vPos       区域的边值
    */
    void DrawProbaChart(bool bLast, bool bLostRect, _POINT _ptBalance, vector<_POINT> _vPos)
    {
        int i,nSize;
        _POINT ptAdd;
        POINT *point = NULL;
        CBrush brush(((m_ColorMode==Color_Light)?(bLostRect?RGB(150,199,97):RGB(229,181,169)):(bLostRect?RGB(0,132,0):RGB(140,0,0))));  

        nSize = _vPos.size();
        point = new POINT[nSize]; 
        for (i=0;i<nSize;i++)
        {
            point[i].x = (LONG)_vPos[i].x;
            point[i].y = (LONG)_vPos[i].y;
        }

        SelectObject(&brush);
        Polygon(point,nSize);  

        if(point)
        {
            delete[] point;
            point = NULL;
        }
    }
};


#endif
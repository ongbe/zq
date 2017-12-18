// NumericEdit.cpp : implementation file
//

#include "stdafx.h"
#include "NumericEdit.h"


// CNumericEdit

IMPLEMENT_DYNAMIC(CNumericEdit, CEdit)

///
/// <summary></summary>
///
CNumericEdit::CNumericEdit()
{
	// determine the decimal delimiter buffer size
	const int nBuffLen = ::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, NULL, 0 );
	_ASSERT( nBuffLen > 0 );

	// get the decimal number delimiter
	const int nResult = ::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, 
		m_strDelim.GetBuffer(nBuffLen), nBuffLen );
	_ASSERT(nResult != 0);
	m_strDelim.ReleaseBuffer();

    m_strInfinity = '-';

}

///
/// <summary></summary>
///
CNumericEdit::~CNumericEdit()
{
}


BEGIN_MESSAGE_MAP(CNumericEdit, CEdit)	
	ON_WM_CHAR()	
END_MESSAGE_MAP()



// CNumericEdit message handlers

///
/// <summary></summary>
///
/* 将一个指定小数点位数的双精度数, 格式化成带千分号的字符串 dec为小数点位数*/
static char *mdou(double val,int dec,int& CommaCount)
{
    int i,sign;
    int len=24; /* 限定字符串的最大长度 */
    char *s;
    s=(char *)calloc(len+1,sizeof(char));
    /* 先置空格串 */
    /* 然后,从低位到高位将双精度数格式化成字符串 */
    for(i=0;i<len;i++) *(s+i)=' ';
    s+=len;
    *s='\0';
    /* 置符号位 */
    if(val<0){sign=1;val*=-1;} else sign=0;

    /* 误差纠正 */
    val+=0.1e-7; 

    /* 先格式化小数部分 */
    for(i=0;i<dec;i++) val*=10;
    for(i=0;i<dec;i++){*--s=(int)fmod(val,10)+'0';val/=10;}
    if(dec>0) *--s='.'; /* 加入小数点 */
    /* 再格式化整数部分 */
    for(i=1;val>=10;val/=10,i++){
        *--s=(int)fmod(val,10)+'0';
        if(i%3==0) {*--s=',';CommaCount++;} /* 加入千分符 */
    }
    if((int)val>0) *--s=(int)val+'0'; /* 加入最高位 */
    if(sign) *--s='-'; /* 加入符号位 */
    return(s);
}

void CNumericEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
    CString strText;
    GetWindowText( strText );

	if( nChar == m_strDelim )
	{
		if( strText.IsEmpty() )
		{
			SetWindowText( (_T("0") + m_strDelim) );
			SetSel( 2, 2, FALSE );
			return;
		}
		else
		{
			// if the decimal point already entered, not allow enter more points
			if( strText.Find( m_strDelim ) > 0 )
				return;
            else
            {
                CString strDot = strText+m_strDelim;
                SetWindowText(strDot);
                SetSel( strDot.GetLength()+1, strDot.GetLength()+1, FALSE );
                return;
            }

		}
	}else if ( nChar == m_strInfinity)
	{
        if( strText.IsEmpty() )
        {
            SetWindowText( (/*_T("0") + */m_strInfinity) );
            SetSel( 2, 2, FALSE );
            return;
        }
        else
        {
            // if the decimal point already entered, not allow enter more points
            int nStart,nEnd;
            GetSel(nStart,nEnd);

            if(nEnd-nStart == strText.GetLength())//全部选择ȫ
                SetWindowText(" ");
            else
            {
                if( strText.Find( m_strInfinity ) >= 0)
                    return;
            }

            SetSel(0,0,FALSE);//将光标设置在首位
        }
	}

	// 8 - back space
	// 46 - .
	// 48 - 1
	// 57 - 0
	if( (nChar == m_strDelim) || (nChar) == m_strInfinity || (nChar >= '0' && nChar <= '9') || (nChar == 8) )
	{
        /*加千分号，未完成
        if(strText.GetLength() >= 3)
        {
            CEdit::OnChar(nChar, nRepCnt, nFlags);
            GetWindowText( strText );
            strText.Remove(',');
            int lenDot = strText.Find('.');
            strText.Delete(lenDot,100);
            int CommaCount = 0;
            double dVal = atof((LPSTR)(LPCTSTR)strText);
            char* sVal = mdou(dVal,0,CommaCount);
            SetWindowText((LPCTSTR)sVal);
            SetSel(strText.GetLength()+CommaCount,strText.GetLength()+CommaCount,FALSE);
            
            return;
        }            
        else*/
		    CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}
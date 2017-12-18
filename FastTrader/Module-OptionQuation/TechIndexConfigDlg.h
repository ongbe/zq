#pragma once

#include "NumSpinCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"

#define MAX_CONFIG_ITEMS  6
class CInitConfig
{
public:
	CString		strTechIndex;
	int			nIndex;//在k线分割图里面第几个序号;相对于TECHINDEX_MAX里面的序号
	bool		bLeftShow[MAX_CONFIG_ITEMS];
	CString		strLeft[MAX_CONFIG_ITEMS];
	bool		bConfigShow[MAX_CONFIG_ITEMS];
	int			nConfig[MAX_CONFIG_ITEMS];
	bool		bRightShow[MAX_CONFIG_ITEMS];
	CString	strRight[MAX_CONFIG_ITEMS];
	vector<int> vecConfigMin;
	vector<int> vecConfigMax;
	CInitConfig()
	{
		strTechIndex = "";
		for(int i =0; i< MAX_CONFIG_ITEMS; i++)
		{
			bLeftShow[i]	= 0;
			strLeft[i]		= "";
			bConfigShow[i]	= 0;
			nConfig[i]		= 0;
			bRightShow[i]	= 0;
			strRight[i]		= "";
		}
	}
	CInitConfig& operator =(const CInitConfig& other)
	{
		strTechIndex = other.strTechIndex;
		for(int i =0; i< MAX_CONFIG_ITEMS; i++)
		{
			bLeftShow[i]	= other.bLeftShow[i];
			strLeft[i]		= other.strLeft[i];
			bConfigShow[i]	= other.bConfigShow[i];
			nConfig[i]		= other.nConfig[i];
			bRightShow[i]	= other.bRightShow[i];
			strRight[i]		= other.strRight[i];
			vecConfigMin	= other.vecConfigMin;
			vecConfigMax	= other.vecConfigMax;
		}
		return *this;
	}
};
class CTechIndexConfigDlg : public CDialog
{
	DECLARE_DYNAMIC(CTechIndexConfigDlg)

public:
	CTechIndexConfigDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTechIndexConfigDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TechIndexConfig };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	void InitDialog( CInitConfig&  cInitConfig);
	void OnSpinValue(int nID, int nValue);
public:

	CStatic WxStaticText1[MAX_CONFIG_ITEMS];	
	CNumSpinCtrl WxSpinCtrl1[MAX_CONFIG_ITEMS];
	CStatic WxStaticText11[MAX_CONFIG_ITEMS];
	CEdit m_Edit[MAX_CONFIG_ITEMS];

	HFONT				m_hSysFontStatic;//头部字体
	CFont				m_FontStatic;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	


public:
	 CInitConfig m_cInitConfig;
	void SetTechIndex(CString strTechIndex, int nIndex, EnumPhrase enumPhrase, vector<int>& vecValue);
	void SetMsgWnd(CWnd *pMsgWnd){ m_pMsgWnd = pMsgWnd; }

	CWnd *m_pMsgWnd;
	bool m_bInit;

	bool WriteConfigFrmFile(string strTechIndexName, EnumPhrase enumPhrase, vector<int>& vecCofig, bool bDel = false);
	bool m_bDelConfig;   //恢复默认配置，需要从配置中删除
	bool m_bWriteConfig; //改变了默认的参数，需要写配置文件
	EnumPhrase m_enumPhrase;
public:
	vector<int>  m_vecConfig;


	
	afx_msg void OnNMThemeChangedSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMOutofmemorySpin1(NMHDR *pNMHDR, LRESULT *pResult);
};

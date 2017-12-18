// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类
#include <Afxcview.h>



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <vector>
#include <map>
#include <set>
#include "MyEdit.h"
#include "sizecbar.h"

#define MAX_PWD_LENGTH	16
#define MIN_PWD_LENGTH	6
#define MAX_ACCOUNT_LENGTH	16
#define MAX_TRUST_ACCOUNT_NAME_LENGTH	200
#define MIN_ACCOUNT_LENGTH	1
#define MAX_USER_NAME_LENGTH	20
#define MAX_CONTACTINFO_LENGTH	100
#define MAX_IDCARD_NO_LENGTH	20
#define MAX_PERMISSION_NAME_LENGTH	20
#define MAX_PHONE_NO_LENGTH	20
#define MAX_MSG_TITLE_LENGTH	20
#define MAX_MSG_CONTENT_LENGTH	1000

#define INVALID_ACOUNTID	-1
#define OUT_TIME_SEC	60
#define TIMER_LOGON_CHECK	1000
#define TIMER_MODIFY_PWD_CHECK	1001
#define TIMER_BROKER_INFO_CHECK	1002
#define TIMER_NET_AND_LOGON_CHECK	1003

#define MSG_SEARCH					WM_USER+501
#define MSG_LIST_SEL_CHANGE			WM_USER+502
#define MSG_MAIN_TAB_SEL_CHANGE		WM_USER+503
#define MSG_MODIFY_ACCOUNT			WM_USER+504
#define MSG_DEL_ACCOUNT				WM_USER+505
#define MSG_ADD_ACCOUNT				WM_USER+506
#define MSG_TARGETACCOUNT_CHANGE	WM_USER+507
#define MSG_SAVE_WARNING_SET		WM_USER+508

enum OperatorType
{
	OPERATOR_ADD,
	OPERATOR_MODIFY
};

enum AccountType
{
	ACCOUNT_TRUST_TRADE,
	ACCOUNT_MANAGE,
	ACCOUNT_TRADER
};

const char VALID_FOREVER_DATE[] = "9999-12-31";

const LPCTSTR lpszAccoutActive = _T("启用");
const LPCTSTR lpszAccountStop = _T("未启用");
const LPCTSTR lpszStatusLogon = _T("已登录");
const LPCTSTR lpszStatusLogout = _T("未登录");

const LPCTSTR lpszAccountError = _T("账号必须为1~16个字符");
const LPCTSTR lpszPasswordError = _T("密码必须为6~16个字符");
const LPCTSTR lpszOriPasswordError = _T("原密码输入错误！");
const LPCTSTR lpszPasswordNotSame = _T("两次输入的密码不一致！");
const LPCTSTR lpszSamePassword = _T("新旧密码不能一样！");
const LPCTSTR lpszPasswordModified = _T("密码已修改!");
const LPCTSTR lpszEmptyPermission = _T("权限不能为空！");
const LPCTSTR lpszEmptyOrg = _T("所属资管不能为空！");
const LPCTSTR lpszEmptyAccountStatus = _T("账号状态不能为空！");
const LPCTSTR lpszEmptyBrokerCode = _T("经纪公司代码不能为空！");

const LPCTSTR lpszQryBrokerError = _T("网络连接异常，请求经纪公司列表失败！");
const LPCTSTR lpszQryPermissionModuleError = _T("网络连接异常，获取权限数据失败！");
const LPCTSTR lpszQryServerGroupError = _T("网络连接异常，请求服务器组列表失败！");
const LPCTSTR lpszQryError = _T("网络连接异常，操作失败！");
const LPCTSTR lpszLogonError = _T("网络连接异常，登录失败！");
const LPCTSTR lpszLogonOutTime = _T("登录超时！");
const LPCTSTR lpszOperatorOutTime = _T("操作超时！");
const LPCTSTR lpszSelectOneItem = _T("请选择一条数据！");
const LPCTSTR lpszQuit = _T("是否退出系统？");
const LPCTSTR lpszCanModifySelDataOnly = _T("只能修改自己创建的数据！");
const LPCTSTR lpszCanDelSelfDataOnly = _T("只能删除自己创建的数据！");
const LPCTSTR lpszChangePermissionTypeWarning = _T("管理/风控权限只能选择一项，点击[保存]后\n会完全覆盖之前数据，请确认！");
const LPCTSTR lpszAddAccountSuccess = _T("新增用户成功，请刷新列表！");
const LPCTSTR lpszModifyAccountSuccess = _T("修改用户成功，请刷新列表！");
const LPCTSTR lpszDelAccountSuccess = _T("删除用户成功，请刷新列表！");
const LPCTSTR lpszModifyAccountStatusWarning = _T("在线用户如果被停用，将会被立即关闭窗口，是否确认该操作？");
const LPCTSTR lpszDelDataComfirm = _T("确认要删除该数据？");
const LPCTSTR lpszDelDataCaseCadeComfirm = _T("该操作将删除账号及其关联的所有子账号，并且不可恢复，是否继续？");
const LPCTSTR lpszDelBrokerComfirm = _T("删除经纪公司，将同步删除其下的服务器组和地址。请确认！");

const LPCTSTR lpszPrompt = _T("提示");
const LPCTSTR lpszAllAccount = _T("全部");

const LPCTSTR lpszPromtAction[] = 
{
	_T("鸣笛"),
	_T("窗口震动"),
	_T("鸣笛、窗口震动"),
};

CString GetAccountFormatString(double dAmount, int nPrecision);

#pragma warning(disable:4996) 

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

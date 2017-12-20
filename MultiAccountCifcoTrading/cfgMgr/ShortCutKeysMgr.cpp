#include "StdAfx.h"
#include <wx/defs.h>

#include "Module-Misc/GLobalFunc.h"
#include "cfgmgr/ShortCutKeysMgr.h"

ShortCutKeysMgr::ShortCutKeysMgr(void)
{
	m_KeyNameMap.clear();
	m_usableShortCutKeySet.clear();
	m_usedShortCutKeySet.clear();
	m_dulplicateKeys.clear();
}

ShortCutKeysMgr::~ShortCutKeysMgr(void)
{
    m_KeyNameMap.clear();
	m_usableShortCutKeySet.clear();
	m_usedShortCutKeySet.clear();
	m_dulplicateKeys.clear();
}

bool ShortCutKeysMgr::InitCfg(TiXmlElement *root, bool bFromUserCfg )
{
	InitKeyNameMap();
	return true;
}

bool ShortCutKeysMgr::SaveCfg(TiXmlElement *root)
{
	return true;
}

void ShortCutKeysMgr::InitKeyNameMap()
{
    m_KeyNameMap.insert(std::make_pair(WXK_BACK,"Backspace"));
    m_KeyNameMap.insert(std::make_pair(WXK_ESCAPE,"Esc"));
    m_KeyNameMap.insert(std::make_pair(WXK_SPACE,"Space"));
    m_KeyNameMap.insert(std::make_pair(WXK_DELETE,"Delete"));
    m_KeyNameMap.insert(std::make_pair(WXK_SHIFT,"Shift"));
    m_KeyNameMap.insert(std::make_pair(WXK_ALT,"Alt"));
    m_KeyNameMap.insert(std::make_pair(WXK_CONTROL,"Ctrl"));
    m_KeyNameMap.insert(std::make_pair(WXK_PAUSE,"Pause"));
    m_KeyNameMap.insert(std::make_pair(WXK_END,"End"));
    m_KeyNameMap.insert(std::make_pair(WXK_HOME,"Home"));
    m_KeyNameMap.insert(std::make_pair(WXK_LEFT,"LEFT"));
    m_KeyNameMap.insert(std::make_pair(WXK_UP,"UP"));
    m_KeyNameMap.insert(std::make_pair(WXK_RIGHT,"RIGHT"));
    m_KeyNameMap.insert(std::make_pair(WXK_DOWN,"DOWN"));

    m_KeyNameMap.insert(std::make_pair(WXK_INSERT,"Insert"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD0,"NumPad 0"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD1,"NumPad 1"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD2,"NumPad 2"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD3,"NumPad 3"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD4,"NumPad 4"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD5,"NumPad 5"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD6,"NumPad 6"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD7,"NumPad 7"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD8,"NumPad 8"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD9,"NumPad 9"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD_MULTIPLY,"*"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD_ADD,"+"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD_DIVIDE,"/"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD_SUBTRACT,"-"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMPAD_DECIMAL,"NumPad ."));
    m_KeyNameMap.insert(std::make_pair(WXK_F1,"F1"));
    m_KeyNameMap.insert(std::make_pair(WXK_F2,"F2"));
    m_KeyNameMap.insert(std::make_pair(WXK_F3,"F3"));
    m_KeyNameMap.insert(std::make_pair(WXK_F4,"F4"));
    m_KeyNameMap.insert(std::make_pair(WXK_F5,"F5"));
    m_KeyNameMap.insert(std::make_pair(WXK_F6,"F6"));
    m_KeyNameMap.insert(std::make_pair(WXK_F7,"F7"));
    m_KeyNameMap.insert(std::make_pair(WXK_F8,"F8"));
    m_KeyNameMap.insert(std::make_pair(WXK_F9,"F9"));
    m_KeyNameMap.insert(std::make_pair(WXK_F10,"F10"));
    m_KeyNameMap.insert(std::make_pair(WXK_F11,"F11"));
    m_KeyNameMap.insert(std::make_pair(WXK_F12,"F12"));
    m_KeyNameMap.insert(std::make_pair(WXK_NUMLOCK,"NumLock"));
    m_KeyNameMap.insert(std::make_pair(WXK_SCROLL,"Scroll"));
    m_KeyNameMap.insert(std::make_pair(WXK_PAGEUP,"PageUp"));
    m_KeyNameMap.insert(std::make_pair(WXK_PAGEDOWN,"PageDown"));
	m_KeyNameMap.insert(std::make_pair(65,"A"));
	m_KeyNameMap.insert(std::make_pair(66,"B"));
	m_KeyNameMap.insert(std::make_pair(67,"C"));
	m_KeyNameMap.insert(std::make_pair(68,"D"));
	m_KeyNameMap.insert(std::make_pair(69,"E"));
	m_KeyNameMap.insert(std::make_pair(70,"F"));
	m_KeyNameMap.insert(std::make_pair(71,"G"));
	m_KeyNameMap.insert(std::make_pair(72,"H"));
	m_KeyNameMap.insert(std::make_pair(73,"I"));
	m_KeyNameMap.insert(std::make_pair(74,"J"));
	m_KeyNameMap.insert(std::make_pair(75,"K"));
	m_KeyNameMap.insert(std::make_pair(76,"L"));
	m_KeyNameMap.insert(std::make_pair(77,"M"));
	m_KeyNameMap.insert(std::make_pair(78,"N"));
	m_KeyNameMap.insert(std::make_pair(79,"O"));
	m_KeyNameMap.insert(std::make_pair(80,"P"));
	m_KeyNameMap.insert(std::make_pair(81,"Q"));
	m_KeyNameMap.insert(std::make_pair(82,"R"));
	m_KeyNameMap.insert(std::make_pair(83,"S"));
	m_KeyNameMap.insert(std::make_pair(84,"T"));
	m_KeyNameMap.insert(std::make_pair(85,"U"));
	m_KeyNameMap.insert(std::make_pair(86,"V"));
	m_KeyNameMap.insert(std::make_pair(87,"W"));
	m_KeyNameMap.insert(std::make_pair(88,"X"));
	m_KeyNameMap.insert(std::make_pair(89,"Y"));
	m_KeyNameMap.insert(std::make_pair(90,"Z"));
   	m_KeyNameMap.insert(std::make_pair(91,"["));
	m_KeyNameMap.insert(std::make_pair(92,"]"));
   	m_KeyNameMap.insert(std::make_pair(126,"`"));
	m_KeyNameMap.insert(std::make_pair(44,","));
	m_KeyNameMap.insert(std::make_pair(59,";"));
	m_KeyNameMap.insert(std::make_pair(39,"'"));

}

void ShortCutKeysMgr::ClearCfg()
{
	m_KeyNameMap.clear();
	m_usableShortCutKeySet.clear();
	m_usedShortCutKeySet.clear();
	m_dulplicateKeys.clear();
}

void ShortCutKeysMgr::ClearUsableShortCutKeys()
{
	m_usableShortCutKeySet.clear();
}

void ShortCutKeysMgr::AddUsableShortCutKey( const std::string& strKey )
{
	m_usableShortCutKeySet.insert(strKey);
}

void ShortCutKeysMgr::DelUsableShortCutKey( const std::string& strKey )
{
	if ( strKey.empty() )
	{
		return;
	}

	std::set<std::string>::iterator it = m_usableShortCutKeySet.find(strKey);
	if ( it != m_usableShortCutKeySet.end())
	{
		m_usableShortCutKeySet.erase(it);
	}
}

void ShortCutKeysMgr::InitUsableShortCutKey()
{
	if ( m_KeyNameMap.empty())
	{
		m_usableShortCutKeySet.clear();
		return;
	}

	std::map<int, std::string>::iterator it = m_KeyNameMap.begin();
	for ( ;it != m_KeyNameMap.end(); it++)
	{
		m_usableShortCutKeySet.insert(it->second);
	}
}

bool ShortCutKeysMgr::IsShortKeyUsable( const std::string& strKey )
{
	if ( strKey.empty() )
	{
		return true;
	}

	std::set<std::string>::iterator it = m_usableShortCutKeySet.find(strKey);
	if ( it != m_usableShortCutKeySet.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ShortCutKeysMgr::ClearUsedShortCutKeys()
{
	m_usedShortCutKeySet.clear();
	m_dulplicateKeys.clear();
}

void ShortCutKeysMgr::CalcDulplicateShortCutKey( const std::string& strKey )
{
	if ( strKey.empty())
	{
		return;
	}

	std::set<std::string>::iterator it = m_usedShortCutKeySet.find( strKey );
	if ( it == m_usedShortCutKeySet.end() )
	{
		m_usedShortCutKeySet.insert( strKey );
	}
	else
	{
		m_dulplicateKeys.insert(strKey);
	}
}

bool ShortCutKeysMgr::IsShortKeyDulplicate( const std::string& strKey )
{
	if ( strKey.empty() )
	{
		return false;
	}

	std::set<std::string>::iterator it = m_dulplicateKeys.find( strKey );
	if ( it != m_dulplicateKeys.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::string ShortCutKeysMgr::GetDulplicateKeyString()
{
	std::string strTemp;
	std::set<std::string>::iterator it = m_dulplicateKeys.begin();
	for ( ; it != m_dulplicateKeys.end(); ++it )
	{
		if ( it != m_dulplicateKeys.begin() )
		{
			strTemp += ",";
		}

		strTemp += *it;
	}

	return strTemp;
}

#pragma once


#include "stdafx.h"
#include "popmsgwin.h"
#include "doupdate.h"

#include <string>
using std::string;



class Cdoupdate2 : public Cdoupdate
{
public:
    Cdoupdate2();
    ~Cdoupdate2();


    bool m_bUpdateSucc;

    static void CALLBACK TimerCBFunc( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);

private:

    static char FastTraderFileName[512];

    //下面两个方法禁用
	Cdoupdate2(const Cdoupdate2&);
	Cdoupdate2& operator= (const Cdoupdate2&);
};

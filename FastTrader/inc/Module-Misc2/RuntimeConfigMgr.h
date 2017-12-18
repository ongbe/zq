#ifndef _RUNTIMECONFIGMGR_H_
#define _RUNTIMECONFIGMGR_H_

#ifdef WIN32
#pragma managed(push,off)
#endif 

class CRuntimeConfigMgr
{
public:
    static int UI_ColorScheme;      //UI界面配色方案，期权终端有效，0:浅底色方案；1:深底色方案
};


#ifdef WIN32
#pragma managed(pop)
#endif 

#endif



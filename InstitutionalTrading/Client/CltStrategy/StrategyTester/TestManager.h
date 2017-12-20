#pragma once
#include "..\CommonClasses\DataEntity.h"
#include "TestEntity.h"

using namespace std;


// 类CTestManager，用管理CTestEntity。
// 主要功能：
// 1.  创建初始化CTestEntity。
// 2.  执行策略测试过程。
// 3.  暂停、启动、终止策略测试过程。


class CTestManager
{
protected:
	CTestManager(void);
	virtual ~CTestManager(void);

public:
	static CTestEntity* CreateTestEntity(const std::string& strSavePath, const std::string& strRunPath);

public:
	static CTestEntity* sm_pTestEntity;

};

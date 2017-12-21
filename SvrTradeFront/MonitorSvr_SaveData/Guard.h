#pragma once

class Ceasymutex;

class CGuard
{
public:
	CGuard(Ceasymutex* pMutex);
	~CGuard(void);

private:
	Ceasymutex* m_pMutex;
};

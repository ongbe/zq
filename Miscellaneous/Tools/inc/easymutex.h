/*
 * easymutex.h
 *
 *  It can be used in windows and linux
 *
 *  Define customize mutex, that auto initialize as create.
 *  You can use it as a static member of a class, then you shouldn't care it's initialize
 *
 *  Created on: Dec 1, 2012
 *      Author: root
 */

#ifndef EASYMUTEX_H_
#define EASYMUTEX_H_

#ifndef WIN32
#include <pthread.h>
#endif

#include <string>
#include <map>
using std::string;
using std::map;

#ifdef WIN32
#pragma managed(push,off)
#endif

class Ceasymutex
{
public:
	Ceasymutex();
	~Ceasymutex();
	void lock(void) const;
	void unlock(void) const;
	bool trylock(void) const;

	Ceasymutex(const Ceasymutex&);
	Ceasymutex& operator= (const Ceasymutex&);

private:

#ifndef WIN32
	mutable pthread_mutex_t m_mutex;
#else
	mutable void* m_pcs;
#endif
};

class CeasymutexGuard
{
public:
	CeasymutexGuard(const Ceasymutex& emutex);
	~CeasymutexGuard(void);
private:
	const Ceasymutex& m_mutex;

    CeasymutexGuard(const CeasymutexGuard&);
	CeasymutexGuard& operator= (const CeasymutexGuard&);
};


#ifdef WIN32
#pragma managed(pop)
#endif

#endif /* EASYMUTEX_H_ */

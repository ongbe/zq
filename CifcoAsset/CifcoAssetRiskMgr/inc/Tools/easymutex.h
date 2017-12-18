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
class Ceasymutex
{
public:
	Ceasymutex() { pthread_mutex_init(&m_mutex,NULL); };
	~Ceasymutex() { pthread_mutex_destroy(&m_mutex); }
	void lock(void) { pthread_mutex_lock(&m_mutex); }
	void unlock(void) { pthread_mutex_unlock(&m_mutex); }

private:
	pthread_mutex_t m_mutex;
	Ceasymutex(const Ceasymutex&);
	Ceasymutex& operator= (const Ceasymutex&);
};

#else


#pragma managed(push,off)

class Ceasymutex
{
public:
	Ceasymutex();
	~Ceasymutex();
	void lock(void);
	void unlock(void);

private:
	void* m_pcs;
	Ceasymutex(const Ceasymutex&);
	Ceasymutex& operator= (const Ceasymutex&);
};

#pragma managed(pop)

#endif

#endif /* EASYMUTEX_H_ */

/*
 * tmpstackbuf.h
 *
 *  Define customize tem stack buf.
 *  If size less than MaxSizeInStack, using alloca to alloc memory in stack, else alloc memory in heap
 *
 *  Created on: Jan 23, 2013
 *      Author: root
 */

#ifndef TMPSTACKBUF_H_
#define TMPSTACKBUF_H_

#ifndef WIN32
#include <unistd.h>
#endif

#ifdef WIN32
#pragma managed(push,off)
#endif


//alloc stack memory, this memory will be auto free when calling function returned
#ifndef AllocTmpStackBuf

//alloc memory to name. maxsizeinstack=10240
#define AllocTmpStackBuf(name,bufsize)		\
	CTmpStackBuf name( (bufsize)<=10240?alloca(bufsize):malloc(bufsize) , bufsize, (bufsize)<=10240?true:false );

//alloc memory to name
#define AllocTmpStackBuf2(name,bufsize,maxsizeinstack)		\
	CTmpStackBuf name( (bufsize)<=(maxsizeinstack)?alloca(bufsize):malloc(bufsize) , bufsize, (bufsize)<=(maxsizeinstack)?true:false );

//alloc memory to name, then copy data to it from dataptr
#define AllocTmpStackBuf3(name,dataptr,bufsize,maxsizeinstack)		\
	CTmpStackBuf name( (bufsize)<=(maxsizeinstack)?alloca(bufsize):malloc(bufsize) , bufsize, (bufsize)<=(maxsizeinstack)?true:false );	\
	if((dataptr)!=NULL&&name.m_pbuf!=NULL) memcpy(name.m_pbuf,(dataptr),(bufsize));


//set TmpStackBuf to name, name is an existing object of CTmpStackBuf
#define SetTmpStackBuf(name,bufsize,maxsizeinstack)		\
	name.set( (bufsize)<=(maxsizeinstack)?alloca(bufsize):malloc(bufsize) , bufsize, (bufsize)<=(maxsizeinstack)?true:false );

//set TmpStackBuf to name, name is an existing object of CTmpStackBuf. then copy data to it from dataptr
#define SetTmpStackBuf2(name,bufsize,maxsizeinstack)		\
	name.set( (bufsize)<=(maxsizeinstack)?alloca(bufsize):malloc(bufsize) , bufsize, (bufsize)<=(maxsizeinstack)?true:false );	\
	if((dataptr)!=NULL&&name.m_pbuf!=NULL) memcpy(name.m_pbuf,(dataptr),(bufsize));

#endif



class CTmpStackBuf
{
public:
	CTmpStackBuf()
	:m_pbuf(NULL),
	 m_size(0),
	 m_bstack(false)
	{
	}
	CTmpStackBuf(void* pPtr,int isize,bool bStack)
	:m_pbuf(pPtr),
	 m_size(isize),
	 m_bstack(bStack)
	{
	};
	virtual ~CTmpStackBuf()
	{
		if(m_pbuf!=NULL&&!m_bstack)
			free(m_pbuf);
	}
	void set(void* pPtr,int isize,bool bStack)
	{
		if(m_pbuf!=NULL&&!m_bstack)
			free(m_pbuf);
		m_pbuf=pPtr;
		m_size=isize;
		m_bstack=bStack;
	}


	void 	*m_pbuf;
	int		m_size;

private:
	bool 	m_bstack;

	CTmpStackBuf& operator=(const CTmpStackBuf&);
	CTmpStackBuf(const CTmpStackBuf&);
};

#ifdef WIN32
#pragma managed(pop)
#endif

#endif /* TMPSTACKBUF_H_ */

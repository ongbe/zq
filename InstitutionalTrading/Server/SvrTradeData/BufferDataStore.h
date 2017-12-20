#pragma once

#include "easymutex.h"
#include <list>
#include <vector>
#include "Interface_DBOpr.h"
#include "commonmacro.h"
#define  LIST_MAX_SIZE 2000
typedef void (*FullCallback)();

template <typename T>
class CBufferDataStore
{

/*指向类的非静态成员函数的指针*/
typedef int (IInterfaceDBOpr::*pWriteInfo)(const std::string & nsTableName,
										   const std::string & nsUserName,
										   const std::string & strTime,
										   const std::vector<T> & nInfo,
										   bool nbDelete);
public:
	CBufferDataStore(){mbFirstSave = true;mbCallbackSaved = true;}




	inline void Push(const T& nEle,FullCallback  npFun );

	inline void SaveToDB(IInterfaceDBOpr * npDBIpr,
		pWriteInfo npWriteFun,
		const std::string & nsTableName,
		const std::string & nsUserName,
		const std::string & strTime,
		bool nbDelete = true);

	inline void Clear()
	{
		mMutex.lock();
		mListBufferData.clear();
		mMutex.unlock();
	}

	int Size()
	{
		mMutex.lock();
		int lnSize = mListBufferData.size();
		mMutex.unlock();
		return lnSize;
	}
private:
	bool mbFirstSave;
	bool mbCallbackSaved;
	std::list<T> mListBufferData;
	Ceasymutex mMutex;


	
};


template <typename T>
void inline CBufferDataStore<T>::Push(const T& nEle,FullCallback  npFun)
{
	mMutex.lock();
	mListBufferData.push_back(nEle);
	if(mListBufferData.size() >= LIST_MAX_SIZE && npFun && mbCallbackSaved)
	{
		OUTPUT_LOG("buffer full call back %d %x",mListBufferData.size(),this);
		npFun();
		mbCallbackSaved = false;
	}
	mMutex.unlock();
}


template <typename T>
void inline CBufferDataStore<T>::SaveToDB(IInterfaceDBOpr * npDBIpr,
										  pWriteInfo npWriteFun,
										  const std::string & nsTableName,
										  const std::string & nsUserName,
										  const std::string & strTime,
										  bool nbDelete )
{
	mMutex.lock();
	do 
	{
		if(npDBIpr == NULL || nsTableName.empty() || mListBufferData.size() == 0)
			break;
	
		OUTPUT_LOG("buffer save to db %d %x",mListBufferData.size(),this);
		std::vector<T> lTempVec(mListBufferData.begin(),mListBufferData.end());
		mListBufferData.clear();
		mbCallbackSaved = true;
	
		//std::copy(mListBufferData.begin(),mListBufferData.end(),lTempVec.begin());
		if(nbDelete)
		{
			(npDBIpr->*npWriteFun)(nsTableName,
				nsUserName,strTime,lTempVec,
				mbFirstSave );	
			mbFirstSave = false;
			

		}
		else
		{
			(npDBIpr->*npWriteFun)(nsTableName,
				nsUserName,strTime,lTempVec,
				false );			
		}
	
		break;
	} while (true);
	mMutex.unlock();	
	return;
}
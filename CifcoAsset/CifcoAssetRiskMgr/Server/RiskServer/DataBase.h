#pragma  once
#pragma warning( disable : 4996 )
char* const g_strRiskMgrPath="hash.xml";
char* const g_strCachePath="cache";
char* const g_root="root";
char* const g_date="date";
char* const g_index="index";
char* const g_value="value";
char* const g_hashmax="hashmax";
char* const g_map="map";
char* const g_first="first";
char* const g_second="second";
char* const g_instrument="instrument";
char* const g_Account="account";
#pragma warning( disable : 4996 )
class CDataBase
{
public:
	struct  RiskFileItem
	{
		int				id;
		int				size;
		int				ptr_offset;
	};
	CDataBase()
	{

	}
	virtual ~CDataBase()
	{

	}
public:
	std::string HexString(const char* p,int len)
	{
		std::string str;
		char pBuffer[16];
		for (int i=0;i<len;++i)
		{
			sprintf(pBuffer,"%02X",p[i]);
			str+=pBuffer[0];
			str+=pBuffer[1];
		}
		return str;
	}
	void HexValue(const char* pHex,char* p)
	{
		size_t len=strlen(pHex)/2;
		for (size_t i=0;i<len;++i)
		{
			p[i]=GetCharValue(pHex[2*i])*16+GetCharValue(pHex[2*i+1]);
		}
	}
	char GetCharValue(char c)
	{
		if(c>='0'&&c<='9')
			return c-'0';
		if(c>='A'&&c<='F')
			return c='A';
		if(c>='a'&&c<='f')
			return c='a';
		return 0;
	}
	template<typename T>  
	T ConvertString(const char* pStr)
	{
		T val;
		HexValue(pStr,(char*)&val);
		return val;
	}
	template<>  
	int ConvertString(const char* pStr)
	{
		return atoi(pStr);
	}
	template<>  
	std::string ConvertString(const char* pStr)
	{
		return pStr;
	}
	template<typename T>  
	std::string ConvertType(T& val)
	{
		return HexString((char*)&val,sizeof(T));
	}
	template<>  
	std::string ConvertType<int>(int& val)
	{
		char buffer[64];
		sprintf(buffer,"%d",val);
		return buffer;
	}
	template<>  
	std::string ConvertType<std::string>(std::string& val)
	{
		return val;
	}
public:
	template<typename K,typename T>
	void ReadMap(const TiXmlElement* pParent,const char* pTag,std::map<K,T>& datamap)
	{
		const TiXmlElement* pPair=pParent->FirstChildElement(pTag);
		while(pPair)
		{
			const char* pFirst=pPair->Attribute(g_first);
			const char* pSecond=pPair->Attribute(g_second);
			if(pFirst&&pSecond)
			{
				K key=ConvertString<K>(pFirst);
				T val=ConvertString<T>(pSecond);
				datamap[key]=val;
			}
			pPair=pPair->NextSiblingElement(pTag);
		}
	}
	template<typename K,typename T>
	void WriteMap( TiXmlElement* pParent,const char* pTag,const std::map<K,T>& datamap)
	{
		std::map<K,T>::const_iterator it;
		for (it=datamap.begin();it!=datamap.end();++it)
		{
			TiXmlElement *pNode = new TiXmlElement(pTag);
			pNode->SetAttribute(g_first,it->first);

			pNode->SetAttribute(g_second,ConvertType(it->second));
			pParent->LinkEndChild(pNode); 
		}
	}
public:
	TiXmlElement* ReadRoot(TiXmlDocument& myDocument,const std::string& strxmlpath)
	{
		if(myDocument.LoadFile(strxmlpath))
		{
			TiXmlElement* RootElement=myDocument.RootElement();
			if(XML_ELEMENTTEXT(RootElement)==g_root)
			{
				int date;
				if(RootElement->Attribute(g_date,&date))
				{
					if(date==GetTodayString())
					{
						return RootElement;
					}
				}
			}
		}
		return NULL;
	}
	TiXmlElement* WriteRoot(TiXmlDocument& myDocument)
	{
		TiXmlElement *RootElement = new TiXmlElement(g_root);
		RootElement->SetAttribute(g_date,GetTodayString());
		myDocument.LinkEndChild(RootElement);
		return RootElement;
	}
	template<typename T>
	void ReadTodayRiskEvent(std::map<int,std::vector<T>>& datamap,int& nID,CReadWriteLock& rwlock,std::string strxmlpath)
	{
		rwlock.write_lock();
		datamap.clear();
		rwlock.write_unlock();
		std::map<int,std::vector<T>>::const_iterator it;
		//////////////////////////////////////////////////////////////////////////
		GetFolderFileName(strxmlpath,g_strCachePath);
		HANDLE hHandle=CreateFile(strxmlpath.c_str(),GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
		if(hHandle!=INVALID_HANDLE_VALUE)
		{
			DWORD dwReadSize = 0,dwLenth=GetFileSize(hHandle,NULL);
			UCHAR* startpos=(UCHAR*)alloca(dwLenth);
			RiskFileItem* pBuffer=(RiskFileItem*)startpos;
			if(ReadFile(hHandle,pBuffer,dwLenth,&dwReadSize,NULL)&&dwReadSize==dwLenth)
			{
				int idays=pBuffer[0].id;
				int count=pBuffer[0].size;
				if(idays==GetTodayString())
				{
					nID=pBuffer[0].ptr_offset;
					for (int i=1;i<=count;++i)
					{
						T* pVec=(T*)(startpos+pBuffer[i].ptr_offset);
						std::vector<T> vec;
						for (int v=0;v<pBuffer[i].size;++v)
						{
							vec.push_back(pVec[v]);
						}
						rwlock.write_lock();
						datamap[pBuffer[i].id]=vec;
						rwlock.write_unlock();
					}
				}
			}
			CloseHandle(hHandle);
		}
	}
	template<typename T>
	void WriteTodayRiskEvent(const std::map<int,std::vector<T>>& datamap,int nID,CReadWriteLock& rwlock,std::string strxmlpath)
	{
		std::map<int,std::vector<T>>::const_iterator it;
		//////////////////////////////////////////////////////////////////////////
		GetFolderFileName(strxmlpath,g_strCachePath);
		HANDLE hHandle=CreateFile(strxmlpath.c_str(),GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
		if(hHandle!=INVALID_HANDLE_VALUE)
		{
			DWORD dwWriteSize = 0;
			int mapsize=datamap.size();
			int i,vecdatas=0;
			rwlock.read_lock();
			for (it=datamap.begin();it!=datamap.end();++it)
			{
				vecdatas+=it->second.size();
			}
			int dwHeadLen=(mapsize+1)*sizeof(RiskFileItem);
			int dwLenth=dwHeadLen+vecdatas*sizeof(T);
			UCHAR* startpos=(UCHAR*)alloca(dwLenth);
			RiskFileItem* pBuffer=(RiskFileItem*)startpos;
			memset(pBuffer,0,dwLenth);
			pBuffer[0].id=GetTodayString();
			pBuffer[0].size=mapsize;
			pBuffer[0].ptr_offset=nID;
			vecdatas=0;
			for (i=1,it=datamap.begin();it!=datamap.end();++it,++i)
			{
				const std::vector<T>& nestvec=it->second;
				pBuffer[i].id=it->first;
				pBuffer[i].size=nestvec.size();
				pBuffer[i].ptr_offset=dwHeadLen+vecdatas*sizeof(T);
				if(nestvec.size()>0)
					memcpy(startpos+pBuffer[i].ptr_offset,&nestvec[0],nestvec.size()*sizeof(T));
				vecdatas+=nestvec.size();
			}
			rwlock.read_unlock();
			WriteFile(hHandle,pBuffer,dwLenth,&dwWriteSize,NULL);
			CloseHandle(hHandle);
		}
	}
	template<typename K,typename T>
	void ReadComplexCtpData(std::map<std::string,std::map<K,T>>& datamap,CReadWriteLock& rwlock,std::string strxmlpath)
	{
		GetFolderFileName(strxmlpath,g_strCachePath);
		TiXmlDocument myDocument;
		TiXmlElement* RootElement=ReadRoot(myDocument,strxmlpath);
		if(RootElement)
		{
			TiXmlElement* pInstr=RootElement->FirstChildElement(g_Account);
			while(pInstr)
			{
				const char* pStrInstrument=pInstr->Attribute(g_value);
				if(pStrInstrument)
				{
					std::map<K,T> nestmap;
					ReadMap(pInstr,g_map,nestmap);
					rwlock.write_lock();
					datamap[pStrInstrument]=nestmap;
					rwlock.write_unlock();
				}
				pInstr=pInstr->NextSiblingElement(g_Account);
			}	
		}
	}
	template<typename K,typename T>
	void WriteComplexCtpData(const std::map<std::string,std::map<K,T>>& datamap,CReadWriteLock& rwlock,std::string strxmlpath)
	{
		TiXmlDocument myDocument;
		TiXmlElement *RootElement = WriteRoot(myDocument);

		std::map<std::string,std::map<K,T>>::const_iterator it;
		std::map<K,T>::const_iterator nestit;
		rwlock.read_lock();
		for (it=datamap.begin();it!=datamap.end();++it)
		{
			TiXmlElement *pStrTag = new TiXmlElement(g_Account);
			pStrTag->SetAttribute(g_value,it->first);
			WriteMap(pStrTag,g_map,it->second);
			RootElement->LinkEndChild(pStrTag); 
		}
		rwlock.read_unlock();
		GetFolderFileName(strxmlpath,g_strCachePath);
		myDocument.SaveFile(strxmlpath);
	}
	template<typename T>
	void ReadSimpleCtpData(std::map<std::string,T>& datamap,CReadWriteLock& rwlock,std::string strxmlpath)
	{
		GetFolderFileName(strxmlpath,g_strCachePath);
		TiXmlDocument myDocument;
		TiXmlElement* RootElement=ReadRoot(myDocument,strxmlpath);
		if(RootElement)
		{
			rwlock.write_lock();
			ReadMap(RootElement,g_map,datamap);		
			rwlock.write_unlock();
		}
	}
	template<typename T>
	void WriteSimpleCtpData(const std::map<std::string,T>& datamap,CReadWriteLock& rwlock,std::string strxmlpath)
	{
		TiXmlDocument myDocument;
		TiXmlElement *RootElement = WriteRoot(myDocument);

		rwlock.read_lock();
		WriteMap(RootElement,g_map,datamap);
		rwlock.read_unlock();
		GetFolderFileName(strxmlpath,g_strCachePath);
		myDocument.SaveFile(strxmlpath);
	}
};
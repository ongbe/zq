/*
 * MyXML.cpp
 *
 *  Created on: Dec 6, 2011
 *      Author: root
 */

#ifdef WIN32
#pragma warning(disable : 4996)
#pragma warning(disable : 4786)
#pragma warning(disable : 4793)
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>

#include "MyXML.h"
#include "FileOpr.h"

#ifdef WIN32
#pragma unmanaged
#endif


CMyXML::CMyXML(void)
{
	m_bvalid=true;
}

CMyXML::CMyXML(const string& nodename,const string& nodecontent)
{
	m_bvalid=true;
	m_name=nodename;
	m_content=nodecontent;
}

CMyXML::CMyXML(const string& xmlfilename)
{
	m_bvalid=false;

	if(!xmlfilename.empty())
	{
		char* pXML;
		int filelen;
		UNITYPE_FILE fd;
		bool ret;
		char xmlpathfilename[1024];
		int namelen;
#ifdef WIN32
	char seperator='\\';
#else
	char seperator='/';
#endif


		if(!CFileOpr::getcurrentexepath(xmlpathfilename,sizeof(xmlpathfilename)-1))
			return;

		namelen=strlen(xmlpathfilename);
		if(namelen+32+xmlfilename.length()>=sizeof(xmlpathfilename))
			return;
		xmlpathfilename[strlen(xmlpathfilename)+1]=0;
		xmlpathfilename[strlen(xmlpathfilename)]=seperator;
		strcat(xmlpathfilename,xmlfilename.c_str());

		FILE_OPEN_RDONLY(fd,xmlpathfilename);
		if(FILE_ISVALID(fd))
		{
			FILE_SEEK(filelen,fd,0,2);
			pXML=(char*)malloc(filelen+1);
			if(pXML)
			{
				FILE_SEEK2(fd,0,0);
				FILE_READ(fd,pXML,filelen);
				pXML[filelen]=0;
				ret=Parse_internal(pXML,0,filelen,0);
				if(!ret)
				{
					m_bvalid=false;
				}
				else
				{
					m_bvalid=true;
				}
				free(pXML);
			}
			FILE_CLOSE(fd);
		}
	}
}

//in entry, EndOff is end of this pXML, after return ,EndOff is end of this Node
CMyXML::CMyXML(const char*pXML,const int StartOff,int& EndOff,const int CallLevel)
{
	m_bvalid=false;

	bool ret;
	ret=Parse_internal(pXML,StartOff,EndOff,CallLevel);
	if(!ret)
	{
		m_bvalid=false;
	}
	else
	{
		m_bvalid=true;
	}
}

CMyXML::CMyXML(const CMyXML& r)
{
	*this=r;
}


CMyXML::~CMyXML()
{
}

CMyXML& CMyXML::operator= (const CMyXML& r)
{
	m_bvalid=r.m_bvalid;
	m_name=r.m_name;
	m_content=r.m_content;

	m_mapattr=r.m_mapattr;
	m_mapsubnode=r.m_mapsubnode;

	return *this;
}


//get node by given name, if RootName==NULL, it will be ignore, else it must match rootname of this object
CMyXML* CMyXML::GetNode(bool bCreateIfNull,CMyXML** pprltParentNode,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	int i;
	CMyXML *pRltNode,*pParentNode;
	const char* pName;

	pParentNode=NULL;
	if(RootName!=NULL&&m_name.empty()&&bCreateIfNull)
		m_name=string(RootName);

	if(RootName==NULL) pRltNode=this;
	else if(m_name==string(RootName)) pRltNode=this;
	else return NULL;
	assert(pRltNode!=NULL);

	for(i=0;i<8;i++)
	{
		switch(i)
		{
			case 0:	pName=Name1;  break;
			case 1:	pName=Name2;  break;
			case 2:	pName=Name3;  break;
			case 3:	pName=Name4;  break;
			case 4:	pName=Name5;  break;
			case 5:	pName=Name6;  break;
			case 6:	pName=Name7;  break;
			case 7:	pName=Name8; break;
		}
		if(pName==NULL||pName[0]==0)
			return pRltNode;

		string NodeName=string(pName);
		multimap<string,CMyXML>::iterator it=pRltNode->m_mapsubnode.find(NodeName);
		if(it==pRltNode->m_mapsubnode.end()&&bCreateIfNull)
		{
			pRltNode->m_mapsubnode.insert(pair<string,CMyXML>(NodeName,CMyXML(NodeName,"")));
			it=pRltNode->m_mapsubnode.find(NodeName);
		}

		if(it==pRltNode->m_mapsubnode.end())
			return NULL;

		pParentNode=pRltNode;
		pRltNode=&it->second;
	}
	if(pprltParentNode) (*pprltParentNode)=pParentNode;
	return pRltNode;
}

bool CMyXML::GetNodeAttrValue(string& RltValue,const string AttrName,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	RltValue.clear();

	const CMyXML* pNode=GetNode(false,NULL,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(!pNode)
		return false;

	multimap<string,string>::const_iterator it=pNode->m_mapattr.find(AttrName);
	if(it==pNode->m_mapattr.end())
	{
		RltValue.clear();
		return false;
	}
	RltValue=it->second;
	return true;
}

bool CMyXML::GetNodeAttrValueInt(int& RltValue,const string AttrName,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	string RltStr;
	bool ret;
	ret=GetNodeAttrValue(RltStr,AttrName,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(!ret)
	{
		RltValue=0;
		return false;
	}
	RltValue=atoi(RltStr.c_str());
	return true;
}

bool CMyXML::GetNodeContent(string& RltContent,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	const CMyXML* pNode=GetNode(false,NULL,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(!pNode)
	{
		RltContent.clear();
		return false;
	}
	RltContent=pNode->m_content;
	return true;

}

bool CMyXML::GetNodeContentInt(int& RltContent,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	string RltStr;
	bool ret;
	ret=GetNodeContent(RltStr,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(!ret)
	{
		RltContent=0;
		return false;
	}
	RltContent=atoi(RltStr.c_str());
	return true;
}

//append node by given name
//if RootName==NULL, it will be ignore, else it must match rootname of this object
bool CMyXML::AppendNode(const CMyXML& NewNode,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	if(!NewNode.m_bvalid||NewNode.m_name.empty())
		return false;

	CMyXML *pParentNode=GetNode(true,NULL,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(!pParentNode) return false;

	pParentNode->m_mapsubnode.insert(pair<string,CMyXML>(NewNode.m_name,NewNode));
	return true;
}


//remove node from xml, if RootName==NULL, it will be ignore, else it must match rootname of this object
bool CMyXML::RemoveNode(const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	CMyXML *pParentNode=NULL;
	CMyXML *pCurNode=GetNode(false,&pParentNode,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);

	if(pParentNode!=NULL&&pCurNode!=NULL)
	{
		multimap<string,CMyXML>::iterator it=pParentNode->m_mapsubnode.find(pCurNode->m_name);
		if(it!=pParentNode->m_mapsubnode.end())
		{
			pParentNode->m_mapsubnode.erase(it);
			return true;
		}
	}
	return false;
}

//modify or append node by given name, if RootName==NULL, it will be ignore, else it must match rootname of this object
bool CMyXML::ModifyOrAppendNode(const CMyXML& NewNode,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	CMyXML *pParentNode=GetNode(true,NULL,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);

	if(pParentNode!=NULL)
	{
		multimap<string,CMyXML>::iterator it=pParentNode->m_mapsubnode.find(NewNode.m_name);
		if(it!=pParentNode->m_mapsubnode.end())
			pParentNode->m_mapsubnode.erase(it);
		pParentNode->m_mapsubnode.insert(pair<string,CMyXML>(NewNode.m_name,NewNode));
		return true;
	}
	return false;
}


//save xml to file
bool CMyXML::SaveXML(const char* xmlname,const char* pathname)
{
	UNITYPE_FILE fd;
	char filename[1024];
	char xmlpathname[1024];
	int namelen;
	char buf[2048];
	bool ret;

	if(!m_bvalid)
		return false;

	if(pathname==NULL)
	{
		if(!CFileOpr::getcurrentexepath(xmlpathname,sizeof(xmlpathname)-1))
			return false;

		namelen=strlen(xmlpathname);
		if(namelen+32+strlen(xmlname)>=sizeof(filename))
			return false;
	}
	else
	{
		if(strlen(pathname)>=sizeof(xmlpathname)||strlen(pathname)+strlen(xmlname)+10>=sizeof(filename))
			return false;
		strcpy(xmlpathname,pathname);
	}
#ifdef WIN32
	char seperator='\\';
#else
	char seperator='/';
#endif
	sprintf(filename,"%s%c%s",xmlpathname,seperator,xmlname);

#ifdef WIN32
	{
		FILE_CREATE(fd,filename);
		if(!FILE_ISVALID(fd))
		{
			CFileOpr::makemultidir(xmlpathname);
			FILE_CREATE(fd,filename);
		}
	}
#else
	if(access(xmlpathname,F_OK)==0||CFileOpr::makemultidir(xmlpathname)==true)
		fd=creat(filename,S_IRWXU);
#endif
	if(!FILE_ISVALID(fd))
		return false;

	sprintf(buf,"<?xml version=\"1.0\" encoding=\"GB2312\"?>\r\n");
	FILE_WRITE(fd,buf,strlen(buf));

	ret=SaveNode_internal((long int)fd,0);

	FILE_CLOSE(fd);

	return ret;
}

//save node into xml file. it will be recursive call
bool CMyXML::SaveNode_internal(long int ifd,int level)
{
	int i;
	bool bInOneLine=false;
	bool bInOneAngleBracket=false;

	if(!m_bvalid) return false;
	if(level>MaxNodeLevel) return false;

	UNITYPE_FILE fd=(UNITYPE_FILE)ifd;

	if(m_mapsubnode.empty()&&m_content.find('\r')==string::npos&&m_content.find('\n')==string::npos)
	{
		bInOneLine=true;
		if(m_content.empty())
			bInOneAngleBracket=true;
	}

	//write name of node
	for(i=0;i<level;i++)
		FILE_WRITE(fd,"\t",1);
	FILE_WRITE(fd,"<",1);
	FILE_WRITE(fd,m_name.c_str(),m_name.length());

	//write attr of node
	multimap<string,string>::iterator it_attr=m_mapattr.begin();
	for(;it_attr!=m_mapattr.end();it_attr++)
	{
		FILE_WRITE(fd," ",1);
		FILE_WRITE(fd,it_attr->first.c_str(),it_attr->first.length());
		FILE_WRITE(fd,"=\"",2);
		FILE_WRITE(fd,it_attr->second.c_str(),it_attr->second.length());
		FILE_WRITE(fd,"\" ",2);
	}

	//write tail of name
	if(bInOneAngleBracket)
	{
		FILE_WRITE(fd," />\r\n",5);
		return true;
	}

	FILE_WRITE(fd,">",1);
	if(!bInOneLine)
		FILE_WRITE(fd,"\r\n",2);

	//write content
	if(!m_content.empty())
	{
		if(!bInOneLine)
		{
			for(i=0;i<level+1;i++)
				FILE_WRITE(fd,"\t",1);
		}

		FILE_WRITE(fd,m_content.c_str(),m_content.length());

		if(!bInOneLine)
		{
			FILE_WRITE(fd,"\r\n",2);
		}
	}


	//write subnode
	multimap<string,CMyXML>::iterator it_subnode=m_mapsubnode.begin();
	for(;it_subnode!=m_mapsubnode.end();it_subnode++)
	{
		if(!it_subnode->second.SaveNode_internal((long int)fd,level+1))
			return false;
	}


	//write tail of node
	if(!bInOneLine)
	{
		for(i=0;i<level;i++)
			FILE_WRITE(fd,"\t",1);
	}
	FILE_WRITE(fd,"</",2);
	FILE_WRITE(fd,m_name.c_str(),m_name.length());
	FILE_WRITE(fd,">\r\n",3);

	return true;
}
//skip space, \r, \n, \t in xml string, from begin, forward
void CMyXML::SkipSpaceInString_internal(const char*pXML,const int Tail,int& Off)
{
	while(Off<Tail&&(pXML[Off]==' '||pXML[Off]=='\t'||pXML[Off]=='\r'||pXML[Off]=='\n'))
		Off++;
}

//skip space, \r, \n, \t in xml string, from tail, backward
void CMyXML::SkipSpaceInStringReverse_internal(const char*pXML,const int Head,int& Off)
{
	while(Off>Head&&(pXML[Off-1]==' '||pXML[Off-1]=='\t'||pXML[Off-1]=='\r'||pXML[Off-1]=='\n'))
		Off--;
}

//skip nonspace, not(space, \r, \n, \t) in xml string
void CMyXML::SkipTextInString_internal(const char*pXML,const int Tail,int& Off)
{
	while(Off<Tail&&(pXML[Off]!=' '&&pXML[Off]!='\t'&&pXML[Off]!='\r'&&pXML[Off]!='\n'))
		Off++;
}

//get range of head of a xml.
//Usually, range defined by StartOff-EndOff will like <?xml version="1.0" encoding="GB2312"?>
bool CMyXML::GetXmlHeadRange_internal(const char*pXML,const int Len,int& StartOff,int& EndOff)
{
	int Off,Off2;

	Off=0;
	SkipSpaceInString_internal(pXML,Len,Off);

	if(pXML[Off]!='<'||pXML[Off+1]!='?')
		return false;
	Off2=Off+2;
	while(Off2<Len&&pXML[Off2]!='>')
		Off2++;
	if(pXML[Off2]=='>'&&pXML[Off2-1]=='?')
	{
		StartOff=Off;
		EndOff=Off2+1;
		return true;
	}
	return false;
}

//get range of angle brackets. this range include angle bracket, that means it start of '<' and end of '>'
//Usually, result range will like: <name>
bool CMyXML::GetRangeOfAngleBracket_internal(const char* pXML,const int Off,const int Tail,int& StartOff,int& EndOff)
{
	StartOff=Off;
	SkipSpaceInString_internal(pXML,Tail,StartOff);
	if(pXML[StartOff]!='<') return false;

	EndOff=StartOff+1;
	while(EndOff<Tail&&pXML[EndOff]!='>') EndOff++;
	if(EndOff==Tail) return false;
	else
	{
		EndOff++;
		return true;
	}
}

//get range outside angle brackets. the range is start of '>' and end of '<'
//Usually, <name> xxx </name>, then result range defined by StartOff-EndOff will be: xxx
void CMyXML::GetRangeOutsideAngleBracket_internal(const char* pXML,const int Off,const int Tail,int& StartOff,int& EndOff)
{
	StartOff=Off;
	while(StartOff>0&&pXML[StartOff-1]!='>')
		StartOff--;

	EndOff=StartOff;
	while(EndOff<Tail&&pXML[EndOff]!='<')
	{
		EndOff++;
	}
}

//get name of node for part inside a pair of angle bracket.
//Usually, if node is like <name attr=value>, then name will be returned
bool CMyXML::GetNodeNameInAngleBracket_internal(const char* pXML,const int NodeStart,const int NodeEnd,string& strname)
{
	int Off1,Off2;

	if(pXML[NodeStart]!='<') return false;
	if(pXML[NodeEnd-1]!='>') return false;
	Off1=NodeStart+1;
	SkipSpaceInString_internal(pXML,NodeEnd,Off1);
	Off2=Off1;
	SkipTextInString_internal(pXML,NodeEnd,Off2);
	if(Off2==NodeEnd)
	{
		Off2--;
		if(Off2>0&&pXML[Off2-1]=='/') Off2--;
	}

	if(Off2<=Off1)
		return false;
	strname.assign(pXML+Off1,Off2-Off1);
	return true;
}

//get value of attr inside angle brackets
//Usually node like <NodeName AttrName1="value1" AttrName2=value2>, then name-value pair will be returned
bool CMyXML::GetNodeAttributeInBracket_internal(const char* pXML,const int NodeStart,const int NodeEnd,multimap<string,string>& attr)
{
	int Off,Off1,Off2;
	string attrname,attrvalue;

	attr.clear();

	if(pXML[NodeStart]!='<') return false;
	if(pXML[NodeEnd-1]!='>') return false;

	Off=NodeStart;

	//skip space
	SkipSpaceInString_internal(pXML,NodeEnd,Off);

	//skip node name
	SkipTextInString_internal(pXML,NodeEnd,Off);
	if(Off==NodeEnd)
	{
		Off--;
		if(Off>0&&pXML[Off-1]=='/') Off--;
	}

	//loop for find attr
	while(Off<NodeEnd)
	{
		attrname.clear();
		attrvalue.clear();

		//skip space
		SkipSpaceInString_internal(pXML,NodeEnd,Off);

		Off1=Off;

		//find equal sign
		while(Off<NodeEnd&&pXML[Off]!='=') Off++;
		if(Off>=NodeEnd)
			break;

		//find attrname
		Off2=Off;
		SkipSpaceInStringReverse_internal(pXML,Off1,Off2);
		if(Off2>=Off1)
			attrname.assign(pXML+Off1,Off2-Off1);

		//find attrvalue
		Off++;
		SkipSpaceInString_internal(pXML,NodeEnd,Off);
		if(pXML[Off]=='\"')
		{
			Off++;
			Off1=Off;
			while(Off<NodeEnd&&pXML[Off]!='\"') Off++;
			Off2=Off;
			if(Off<NodeEnd)
				Off++;
		}
		else
		{
			Off1=Off;
			SkipTextInString_internal(pXML,NodeEnd,Off);
			Off2=Off;
		}
		if(Off2==NodeEnd)
		{
			Off2--;
			if(Off2>0&&pXML[Off2-1]=='/') Off2--;
		}
		SkipSpaceInString_internal(pXML,NodeEnd,Off1);
		SkipSpaceInStringReverse_internal(pXML,Off1,Off2);
		if(Off2>=Off1)
			attrvalue.assign(pXML+Off1,Off2-Off1);

		if(!attrname.empty()&&!attrvalue.empty())
			attr.insert(pair<string,string>(attrname,attrvalue));
	}
	return true;
}

//parse xml
//a node must begin with <node, and end with </node> or />
//CallLevel:Level of Call. the max level is 8. Because this routine was recursive called, so it is necessary that control calling level
//in entry, EndOff is end of this pXML, after return ,EndOff is end of this Node
bool CMyXML::Parse_internal(const char*pXML,const int StartOff,int& EndOff,const int CallLevel)
{
	int Off1,Off2,Off;

	if(CallLevel>MaxNodeLevel) return false;

	Off=StartOff;

	//if CallLevel==0, it means there is xml head should be skip. It is allowed that a xml without head
	if(CallLevel==0)
	{
		if(GetXmlHeadRange_internal(pXML,EndOff,Off1,Off2))
		{
			Off=Off2;
		}
	}

	//skip space in head of xml
	SkipSpaceInString_internal(pXML,EndOff,Off);

	//node must begin with '<'
	if(Off>=EndOff||pXML[Off]!='<')
		return false;

	//get range of anglebracket
	if(!GetRangeOfAngleBracket_internal(pXML,Off,EndOff,Off1,Off2))
		return false;

	//get name of this node
	if(!GetNodeNameInAngleBracket_internal(pXML,Off1,Off2,m_name))
		return false;

	//get attr of this node
	GetNodeAttributeInBracket_internal(pXML,Off1,Off2,m_mapattr);

	//if end with "/>", then the node is completed
	if(Off2-Off1>2&&pXML[Off2-2]=='/'&&pXML[Off2-1]=='>')
	{
		EndOff=Off2;
		if(m_name.empty()) return false;
		else return true;
	}

	//loop to process node content and subnode
	Off=Off2;
	while(Off<EndOff)
	{
		//skip space
		SkipSpaceInString_internal(pXML,EndOff,Off);

		if(pXML[Off]!='<')
		{
			//add to content of node
			GetRangeOutsideAngleBracket_internal(pXML,Off,EndOff,Off1,Off2);
			Off=Off2;
			SkipSpaceInString_internal(pXML,Off2,Off1);
			SkipSpaceInStringReverse_internal(pXML,Off1,Off2);
			if(Off2>Off1)
			{
				if(m_content.empty())
				{
					m_content.assign(pXML+Off1,Off2-Off1);
				}
				else
				{
					m_content.append("\r\n",2);
					m_content.append(pXML+Off1,Off2-Off1);
				}
			}
			Off=Off2;
			continue;
		}
		else if(Off<EndOff-2&&pXML[Off]=='<'&&pXML[Off+1]=='/')
		{
			//it's </node>, means this node is completed
			if(!GetRangeOfAngleBracket_internal(pXML,Off,EndOff,Off1,Off2))
				return false;
			Off=Off2;
			Off1+=2;
			Off2--;
			SkipSpaceInString_internal(pXML,Off2,Off1);
			SkipSpaceInStringReverse_internal(pXML,Off1,Off2);
			if(Off2>Off1&&m_name==string(pXML+Off1,Off2-Off1))
			{
				EndOff=Off;
				if(m_name.empty()) return false;
				else return true;
			}
			else return false;
		}
		else
		{
			int newEndOff=EndOff;
			CMyXML tmpxml(pXML,Off,newEndOff,CallLevel+1);
			//follows should be a subnode, like <subnode>...</subnode>
			m_mapsubnode.insert(pair<string,CMyXML>(tmpxml.m_name,tmpxml));
			Off=newEndOff;
		}
	}

	return false;
}


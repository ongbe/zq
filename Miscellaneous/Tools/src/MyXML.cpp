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
#include "Tools_Ansi.h"

using std::pair;

#ifdef WIN32
#pragma unmanaged
#endif


CMyXML::CMyXML(void)
:   m_bvalid(true)
{
}

CMyXML::CMyXML(const string& nodename,const string& nodecontent)
:   m_bvalid(true)
{
	m_name=nodename;
	m_content=nodecontent;
}

CMyXML::CMyXML(const char* pxmlfilename,bool bfilenameincludepath,bool bRtnBlankIfUnExist)
:   m_bvalid(false)
{
	string xmlfilename(pxmlfilename);

	if(!xmlfilename.empty())
	{
		char* pXML;
		int filelen;
		UNITYPE_FILE fd;
		bool ret;
		char xmlpathfilename[1024];

#ifdef WIN32
		char seperator='\\';
#else
		char seperator='/';
#endif

		memset(xmlpathfilename,0,sizeof(xmlpathfilename));
		if(bfilenameincludepath) 
		{
			if((int)xmlfilename.size()>=(int)sizeof(xmlpathfilename)) 
				return;
			strncpy(xmlpathfilename,xmlfilename.c_str(),sizeof(xmlpathfilename)-1);
		}
		else
		{
			if(!CFileOpr::getcurrentexepath(xmlpathfilename,sizeof(xmlpathfilename)-1))
				return;

			int namelen=strlen(xmlpathfilename);
			if(namelen+32+xmlfilename.length()>=sizeof(xmlpathfilename))
				return;
			xmlpathfilename[strlen(xmlpathfilename)+1]=0;
			xmlpathfilename[strlen(xmlpathfilename)]=seperator;
			strcat(xmlpathfilename,xmlfilename.c_str());
		}

		m_xmlpathfilename=string(xmlpathfilename);

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
		else if(bRtnBlankIfUnExist)
		{
			m_bvalid=true;
		}
	}
}

//in entry, EndOff is end of this pXML, after return ,EndOff is end of this Node
CMyXML::CMyXML(const char*pXML,const int StartOff,int& EndOff,const int CallLevel)
:   m_bvalid(false)
{
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
:	m_bvalid(false)
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
    m_xmlpathfilename=r.m_xmlpathfilename;

	return *this;
}


//get node by given name, if RootName==NULL, it will be ignore, else it must match rootname of this object
CMyXML* CMyXML::GetNode(bool bCreateIfNull,CMyXML** pprltParentNode,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	int i;
	CMyXML *pRltNode;
	const char* pName;

	if(!m_bvalid) return NULL;

	if(pprltParentNode) (*pprltParentNode)=NULL;

	if(RootName!=NULL&&m_name.empty()&&bCreateIfNull)
		m_name=string(RootName);

	if(RootName==NULL||m_name==string(RootName)) pRltNode=this;
	else return NULL;

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
		{
			return pRltNode;
		}

		string NodeName=string(pName);
		multimap<string,CMyXML>::iterator it=pRltNode->m_mapsubnode.find(NodeName);
		if(it==pRltNode->m_mapsubnode.end())
		{
			if(bCreateIfNull)
			{
				pRltNode->m_mapsubnode.insert(pair<string,CMyXML>(NodeName,CMyXML(NodeName,"")));
				it=pRltNode->m_mapsubnode.find(NodeName);
			}
			else return NULL;
		}

		if(pprltParentNode) (*pprltParentNode)=pRltNode;

		pRltNode=&it->second;
	}
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
	if(!GetNodeAttrValue(RltStr,AttrName,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8))
	{
		RltValue=0;
		return false;
	}
	else
	{
		RltValue=atoi(RltStr.c_str());
		return true;
	}
}
string CMyXML::GetNodeAttrValue2(const string DefContent,const string AttrName,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	const CMyXML* pNode=GetNode(false,NULL,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(!pNode) return DefContent;

	multimap<string,string>::const_iterator it=pNode->m_mapattr.find(AttrName);
	if(it==pNode->m_mapattr.end()) return DefContent;
	return it->second;
}
int CMyXML::GetNodeAttrValueInt2(int DefVal,const string AttrName,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	string RltStr;
	if(!GetNodeAttrValue(RltStr,AttrName,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8)) return DefVal;
	else return  atoi(RltStr.c_str());
}

bool CMyXML::GetNodeAttrValuebool2(bool DefVal,const string AttrName,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	string RltStr;
	if(!GetNodeAttrValue(RltStr,AttrName,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8)) return DefVal;
	else 
	{
		if(RltStr=="0") return false;
		else if(RltStr=="1") return true;
		else if(stricmp(RltStr.c_str(),"true")==0) return true;
		else if(stricmp(RltStr.c_str(),"false")==0) return false;
		else return DefVal;
	}
}

bool CMyXML::GetNodeContent(string& RltContent,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	const CMyXML* pNode=GetNode(false,NULL,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(!pNode)
	{
		RltContent.clear();
		return false;
	}
	else
	{
		RltContent=pNode->m_content;
		return true;
	}
}

bool CMyXML::GetNodeContentInt(int& RltContent,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	string RltStr;
	if(!GetNodeContent(RltStr,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8))
	{
		RltContent=0;
		return false;
	}
	else
	{
		RltContent=atoi(RltStr.c_str());
		return true;
	}
}
string CMyXML::GetNodeContent2(const string DefContent,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	const CMyXML* pNode=GetNode(false,NULL,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(!pNode) return DefContent;
	return pNode->m_content;
}

int CMyXML::GetNodeContentInt2(int DefVal,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	string RltStr;
	bool ret;
	ret=GetNodeContent(RltStr,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(!ret) return DefVal;
	else return atoi(RltStr.c_str());
}

//1/true表示true；0/false表示false
bool CMyXML::GetNodeContentbool2(bool DefVal,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	string RltStr;
	if(!GetNodeContent(RltStr,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8)) return DefVal;
    else 
	{
		if(RltStr=="0") return false;
		else if(RltStr=="1") return true;
		else if(stricmp(RltStr.c_str(),"true")==0) return true;
		else if(stricmp(RltStr.c_str(),"false")==0) return false;
		else return DefVal;
	}
}

vector<string> CMyXML::GetNodeContent3(const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	vector<string> Contents;
	CMyXML* pParentNode;
	const CMyXML* pNode=GetNode(false,&pParentNode,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(pNode&&pParentNode)
	{
		multimap<string,CMyXML>::const_iterator itlower=pParentNode->m_mapsubnode.lower_bound(pNode->m_name);
		multimap<string,CMyXML>::const_iterator itupper=pParentNode->m_mapsubnode.upper_bound(pNode->m_name);
		for(;itlower!=itupper;itlower++)
			Contents.push_back(itlower->second.m_content);
	}
	return Contents;
}
set<string> CMyXML::GetNodeContent4(const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	set<string> Contents;
	CMyXML* pParentNode;
	const CMyXML* pNode=GetNode(false,&pParentNode,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(pNode&&pParentNode)
	{
		multimap<string,CMyXML>::const_iterator itlower=pParentNode->m_mapsubnode.lower_bound(pNode->m_name);
		multimap<string,CMyXML>::const_iterator itupper=pParentNode->m_mapsubnode.upper_bound(pNode->m_name);
		for(;itlower!=itupper;itlower++)
			Contents.insert(itlower->second.m_content);
	}
	return Contents;
}

bool CMyXML::GetSubContents(vector<string>& SubContents,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	SubContents.clear();
	const CMyXML* pNode=GetNode(false,NULL,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(!pNode) return false;
	for(multimap<string,CMyXML>::const_iterator it=pNode->m_mapsubnode.begin();it!=pNode->m_mapsubnode.end();it++)
	{
		SubContents.push_back(it->second.m_content);
	}
	return true;
}
//取所有子串数组
void CMyXML::GetSubNodes(vector<pair<string,string>>& SubNodes,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	SubNodes.clear();
	const CMyXML* pNode=GetNode(false,NULL,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(!pNode) return;
	for(multimap<string,CMyXML>::const_iterator it=pNode->m_mapsubnode.begin();it!=pNode->m_mapsubnode.end();it++)
	{
        SubNodes.push_back(make_pair(it->second.m_name,it->second.m_content));
	}
}
//去名字相同的所有子节点
vector<CMyXML> CMyXML::GetSubNodes(const string& nameofsubnode,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	vector<CMyXML> rltnodes;
	const CMyXML* pNode=GetNode(false,NULL,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(pNode)
		rltnodes=CTools_Ansi::GetValuesInMultimap<string,CMyXML>(pNode->m_mapsubnode,nameofsubnode);
	return rltnodes;
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

void CMyXML::clear(void)
{
	m_bvalid=true;;
	m_name.clear();
	m_content.clear();
	m_mapattr.clear();
	m_mapsubnode.clear();
}

//modify or append node by given name, if RootName==NULL, it will be ignore, else it must match rootname of this object
bool CMyXML::ModifyOrAppendNode(const CMyXML& NewNode,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	CMyXML *pParentNode=GetNode(true,NULL,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);

	if(pParentNode==NULL) return false;

	multimap<string,CMyXML>::iterator it=pParentNode->m_mapsubnode.find(NewNode.m_name);
	if(it!=pParentNode->m_mapsubnode.end())
		pParentNode->m_mapsubnode.erase(it);
	pParentNode->m_mapsubnode.insert(pair<string,CMyXML>(NewNode.m_name,NewNode));
	return true;
}
bool CMyXML::ModifyOrAppendNode_save(const CMyXML& NewNode,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	if(m_xmlpathfilename.empty()) return false;
	bool brlt=ModifyOrAppendNode(NewNode,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(!brlt) return brlt;
	SaveXML(m_xmlpathfilename.c_str(),"");
	return true;
}

bool CMyXML::ModifyOrAppendSubNodes(const char* SubNodeName,const vector<string>& SubNodeContents,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	if(!SubNodeName) return false;

	CMyXML *pParentNode=GetNode(true,NULL,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);

	if(pParentNode==NULL) return false;

	pParentNode->m_mapsubnode.clear();
	string strSubNodeName(SubNodeName);
	for(int i=0;i<(int)SubNodeContents.size();i++)
	{
		pParentNode->m_mapsubnode.insert(pair<string,CMyXML>(strSubNodeName,CMyXML(strSubNodeName,SubNodeContents[i])));
	}
	return true;
}

bool CMyXML::ModifyOrAppendSubNodes_save(const char* SubNodeName,const vector<string>& SubNodeContents,const char* RootName,const char* Name1,const char* Name2,const char* Name3,const char* Name4,const char* Name5,const char* Name6,const char* Name7,const char* Name8)
{
	if(m_xmlpathfilename.empty()) return false;
	bool brlt=ModifyOrAppendSubNodes(SubNodeName,SubNodeContents,RootName,Name1,Name2,Name3,Name4,Name5,Name6,Name7,Name8);
	if(!brlt) return brlt;
	SaveXML(m_xmlpathfilename.c_str(),"");
	return true;
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

	memset(filename,0,sizeof(filename));
	if(pathname==NULL)
	{
		if(!CFileOpr::getcurrentexepath(filename,sizeof(filename)-1))
			return false;

		namelen=strlen(filename);
		if(namelen+32+strlen(xmlname)>=sizeof(filename))
			return false;
	}
	else
	{
		if(strlen(pathname)>=sizeof(filename)||strlen(pathname)+strlen(xmlname)+10>=sizeof(filename))
			return false;
		strcpy(filename,pathname);
	}
	strcpy(xmlpathname,filename);

#ifdef WIN32
	char seperator='\\';
#else
	char seperator='/';
#endif

	int len=strlen(filename);
	if(len>0)
	{
		filename[len++]=seperator;
		filename[len]=0;
	}
	strcat(filename,xmlname);

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

bool CMyXML::SaveXML()
{
    return SaveXML(m_xmlpathfilename.c_str(),"");
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

//skip space, \r, \n, \t, and comment line in xml string, from tail, backward
void CMyXML::SkipSpaceInStringReverse_internal(const char*pXML,const int Head,int& RltEnd)
{
    //while(true)
    //{
        //int OldEnd=RltEnd;
	    while(RltEnd>Head&&(pXML[RltEnd-1]==' '||pXML[RltEnd-1]=='\t'||pXML[RltEnd-1]=='\r'||pXML[RltEnd-1]=='\n'))
		    RltEnd--;

    //    int LineOff=RltEnd-1;
    //    while(LineOff>Head&&pXML[LineOff]!='\r'&&pXML[LineOff]!='\n') 
    //        LineOff--;
    //    int CommentOff=LineOff;
    //    while(CommentOff<RltEnd-1&&(pXML[CommentOff]!='/'||pXML[CommentOff+1]!='/')) 
    //        CommentOff++;
    //    if(CommentOff<RltEnd-1)
    //        RltEnd=CommentOff;

    //    if(RltEnd==OldEnd||RltEnd<=Head) break;
    //}
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

		if(!attrname.empty())
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
                string tmpcontent=CTools_Ansi::FilterLineComment(string(pXML+Off1,Off2-Off1));
                if(!tmpcontent.empty())
                {
				    if(m_content.empty()) m_content=tmpcontent;
				    else m_content+=string("\r\n")+tmpcontent;
                }
			}
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
		{   //it's start with '<', means beginning of a node
            string subname;

			int newEndOff=EndOff;
            int newOff1,newOff2;

	        //get range of anglebracket
	        if(!GetRangeOfAngleBracket_internal(pXML,Off,newEndOff,newOff1,newOff2))
		        return false;
	        //get name of this node
	        if(!GetNodeNameInAngleBracket_internal(pXML,newOff1,newOff2,subname))
		        return false;

			//follows should be a subnode, like <subnode>...</subnode>
            //insert a blank node first, than parse it.
            multimap<string,CMyXML>::iterator it=m_mapsubnode.insert(make_pair(subname,CMyXML(subname,string(""))));
            if(!it->second.Parse_internal(pXML,Off,newEndOff,CallLevel+1))
                it->second.m_bvalid=false;

            Off=newEndOff;
		}
	}

	return false;
}


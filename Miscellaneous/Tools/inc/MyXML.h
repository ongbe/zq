/*
 * MyXML.h
 *
 *1. all operation to this object haven't protection of mutex. so you should do it in a single thread
 *2. the valid xml must coded in GB2312/GBK
 *3. content in a node must appear ahead of subnode
 *
 *  Created on: Dec 6, 2011
 *      Author: root
 */

#ifndef MYXML_H_
#define MYXML_H_

#include <map>
#include <string>
#include <vector>
#include <set>
using std::multimap;
using std::map;
using std::string;
using std::vector;
using std::set;
using std::pair;

#ifdef WIN32
#pragma managed(push,off)
#endif 

#define MaxNodeLevel 9
//note: xml text must coded in GB2312/GBK
class CMyXML
{
public:
	CMyXML(void);
	CMyXML(const string& nodename,const string& nodecontent);
	CMyXML(const char* pxmlfilename,bool bfilenameincludepath=false,bool bRtnBlankIfUnExist=false);
	CMyXML(const char* pXML,const int StartOff,int& EndOff,const int CallLevel=0);
	CMyXML(const CMyXML& r);
	virtual ~CMyXML();

	CMyXML& operator= (const CMyXML&);

	//get node by given name, if RootName==NULL, it will be ignore, else it must match rootname of this object
	CMyXML* GetNode(bool bCreateIfNull,CMyXML** pprltParentNode,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);

	bool GetNodeAttrValue(string& RltValue,const string AttrName,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);
	bool GetNodeAttrValueInt(int& RltValue,const string AttrName,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);
	string GetNodeAttrValue2(const string DefContent,const string AttrName,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);
	int GetNodeAttrValueInt2(int DefVal,const string AttrName,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);
	bool GetNodeAttrValuebool2(bool DefVal,const string AttrName,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);

	bool GetNodeContent(string& RltContent,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);
	bool GetNodeContentInt(int& RltContent,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);
	string GetNodeContent2(const string DefContent,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);
	int GetNodeContentInt2(int DefVal,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);
	bool GetNodeContentbool2(bool DefVal,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);
	vector<string> GetNodeContent3(const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);
	set<string> GetNodeContent4(const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);

	bool GetSubContents(vector<string>& SubContents,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);
	void GetSubNodes(vector<pair<string,string>>& SubNodes,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);
	vector<CMyXML> GetSubNodes(const string& nameofsubnode,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);

	//append node by given name, if RootName==NULL, it will be ignore, else it must match rootname of this object
	bool AppendNode(const CMyXML& NewNode,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);

	//remove node from xml, if RootName==NULL, it will be ignore, else it must match rootname of this object
	bool RemoveNode(const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);

	void clear(void);

	//modify or append node by given name, if RootName==NULL, it will be ignore, else it must match rootname of this object
	bool ModifyOrAppendNode(const CMyXML& NewNode,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);
	//modify or append node, then save into file
	bool ModifyOrAppendNode_save(const CMyXML& NewNode,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);

	//modify or append node by given name, if RootName==NULL, it will be ignore, else it must match rootname of this object
	bool ModifyOrAppendSubNodes(const char* SubNodeName,const vector<string>& SubNodeContents,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);
	//modify or append node, then save into file
	bool ModifyOrAppendSubNodes_save(const char* SubNodeName,const vector<string>& SubNodeContents,const char* RootName=NULL,const char* Name1=NULL,const char* Name2=NULL,const char* Name3=NULL,const char* Name4=NULL,const char* Name5=NULL,const char* Name6=NULL,const char* Name7=NULL,const char* Name8=NULL);

	//save xml to file. xmlname is in current path if pathname==NULL, or is including path if pathname==""
	bool SaveXML(const char* xmlname,const char* pathname=NULL);
	bool SaveXML(void);

private:
	//skip space, \r, \n, \t in xml string
	void SkipSpaceInString_internal(const char*pXML,const int Tail,int& Off);

	//skip space reverse, \r, \n, \t in xml string
	void SkipSpaceInStringReverse_internal(const char*pXML,const int Head,int& Off);

	//skip nonspace, not(\r, \n, \t) in xml string
	void SkipTextInString_internal(const char*pXML,const int Tail,int& Off);

	//get range of head of a xml. usually like <?xml version="1.0" encoding="GB2312"?>
	bool GetXmlHeadRange_internal(const char*pXML,const int Len,int& StartOff,int& EndOff);

	//get range of angle bracket. this range include angle bracket, that means it start of '<' and end of '>'
	bool GetRangeOfAngleBracket_internal(const char* pXML,const int Off,const int Tail,int& StartOff,int& EndOff);

	//get range outside angle brackets. the range is start of '>' and end of '<'
	void GetRangeOutsideAngleBracket_internal(const char* pXML,const int Off,const int Tail,int& StartOff,int& EndOff);

	//get name of node for part inside a pair of angle bracket.
	//Usually, if node is like <name attr=value>, then name will be returned
	bool GetNodeNameInAngleBracket_internal(const char* pXML,const int NodeStart,const int NodeEnd,string& strname);

	//get value of attr inside angle brackets
	//Usually node like <NodeName AttrName="value"> or <NodeName AttrName=value>, then value will be returned
	bool GetNodeAttributeInBracket_internal(const char* pXML,const int NodeStart,const int NodeEnd,multimap<string,string>& attr);

	//parse xml
	//CallLevel:Level of Call. the max level is 8. Because this routine was recursive called, so it is necessary that control calling level
	//in entry, EndOff is end of this pXML, after return ,EndOff is end of this Node
	bool Parse_internal(const char*pXML,const int StartOff,int& EndOff,const int CallLevel);

	//save node into xml file. it will be recursive call
	bool SaveNode_internal(long int ifd,int level);



public:
	bool m_bvalid;
	string m_name;
	string m_content;
	multimap<string,string> m_mapattr;
	multimap<string,CMyXML> m_mapsubnode;

	string m_xmlpathfilename;
};

#ifdef WIN32
#pragma managed(pop)
#endif 

#endif /* MYXML_H_ */

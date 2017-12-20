// SOAPTitle.cpp: implementation of the SOAPTitle class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _USE_TOP_SPLITTER
#include "SOAPTitle.h"
extern wxArrayString g_HttpUrls;
SoapGuideThread::SoapGuideThread(wxWindow* parent,wxString url)
{
	m_parent=parent;
	m_url=url;
}
SoapGuideThread::~SoapGuideThread()
{
}
void SoapGuideThread::SetParam(wxString s_maxCount)
{
	m_maxCount=s_maxCount;
}
void* SoapGuideThread::Entry()
{
	CoInitialize(NULL); //初始化
	ISoapSerializerPtr Serializer; //Serialize指针用于发送SOAP消息
	ISoapReaderPtr Reader; //定义读取指针
	ISoapConnectorPtr Connector; //定义连接指针
	MSXML2::IXMLDOMDocumentPtr pDoc; 

	HRESULT hr1=Connector.CreateInstance(__uuidof(HttpConnector30)); //创建连接实例
	HRESULT hr2=Serializer.CreateInstance(__uuidof(SoapSerializer30));//创建一个Serializer实例 
	HRESULT hr3=Reader.CreateInstance(__uuidof(SoapReader30)); 	
	HRESULT hr4 = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument40)); 
	if(FAILED(hr1)||FAILED(hr2)||FAILED(hr3)||FAILED(hr4)) return NULL;

	try
	{	
		Connector->Property["EndPointURL"]=m_url.c_str();//连接属性得到将要进行连接的URL
		Connector->Connect();//和服务器连接 
		Connector->Property["SoapAction"] =g_HttpUrls[UrlIndex_GuideAction].c_str();
		Connector->BeginMessage(); //开始准备发送消息
		// 将serializer连接到connector的输入字符串 
		Serializer->Init(_variant_t((IUnknown*)Connector->InputStream));
		// 创建SOAP消息 
		Serializer->StartEnvelope("soap","",""); 
		Serializer->StartBody(""); 
 		Serializer->StartElement("findRealTimeList",g_HttpUrls[UrlIndex_GuideNameSpace].c_str(),"","ns2");// 方法+命名空间
		Serializer->StartElement("maxCount","","",""); 
		Serializer->WriteString( m_maxCount.c_str() ); //
		Serializer->EndElement();
		Serializer->EndElement(); 
		Serializer->EndBody(); 
		Serializer->EndEnvelope(); 	

		Connector->EndMessage(); //向网站发送消息	

		Reader->Load(_variant_t((IUnknown*)Connector->OutputStream), ""); 
		MSXML2::IXMLDOMNodePtr DOMptr=Reader->RpcResult->firstChild;//返回结果
		string strXML=(const char*)DOMptr->text;	
		_bstr_t bs = strXML.c_str();
		pDoc->loadXML(bs);
		MSXML2::IXMLDOMNodePtr pRootNode = pDoc->selectSingleNode("response/realtimeList");	
		if (pRootNode==NULL)	return false;
		MSXML2::IXMLDOMNodePtr pNode = pRootNode->firstChild;
		MSXML2::IXMLDOMNamedNodeMapPtr pAttr;
		long bFirst=1;
		while(pNode)
		{		
			pAttr = pNode->attributes;

			MSXML2::IXMLDOMNodePtr pIDAttr = pAttr->getQualifiedItem("id","");
			_bstr_t bs_id = pIDAttr->Gettext();

			MSXML2::IXMLDOMNodePtr pNameAttr = pAttr->getQualifiedItem("content","");
			_bstr_t bs_content = pNameAttr->Gettext();

			MSXML2::IXMLDOMNodePtr pCateAttr = pAttr->getQualifiedItem("userName","");
			_bstr_t bs_userName = pCateAttr->Gettext();

			MSXML2::IXMLDOMNodePtr pDateAttr = pAttr->getQualifiedItem("createDate","");
			_bstr_t bs_createDate = pDateAttr->Gettext();

			if(!TestDestroy()&&m_parent)
			{
				wxCommandEvent evt(wxEVT_SOAP_MSG);
				evt.SetInt(atoi((char *)bs_id ));
				evt.SetExtraLong(bFirst);
				wxString strText=(char *)bs_userName;
                strText+="\n";
				strText+=(char *)bs_content;
                strText+=" \t ";
				strText+=(char *)bs_createDate;
                strText+="\n";
				evt.SetString(strText.c_str());
				m_parent->AddPendingEvent(evt);
				bFirst=0;
			}
			pNode = pNode->nextSibling;
		}	
	}
	catch(...) 
	{


	}
	CoUninitialize();
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//CResearchReportThread
CResearchReportThread::CResearchReportThread(wxWindow* parent,wxString url)
{
	m_parent=parent;
	m_url=url;
}
CResearchReportThread::~CResearchReportThread()
{
}
void CResearchReportThread::SetParam(wxString s_type,wxString s_categoryId,wxString s_fieldId,wxString s_maxCount)
{
	m_type=s_type;
	m_categoryId=s_categoryId;
	m_fieldId=s_fieldId;
	m_maxCount=s_maxCount;
}
void* CResearchReportThread::Entry()
{
	CoInitialize(NULL); //初始化
	ISoapSerializerPtr Serializer; //Serialize指针用于发送SOAP消息
	ISoapReaderPtr Reader; //定义读取指针
	ISoapConnectorPtr Connector; //定义连接指针
	MSXML2::IXMLDOMDocumentPtr pDoc; 

	HRESULT hr1=Connector.CreateInstance(__uuidof(HttpConnector30)); //创建连接实例
	HRESULT hr2=Serializer.CreateInstance(__uuidof(SoapSerializer30));//创建一个Serializer实例 
	HRESULT hr3=Reader.CreateInstance(__uuidof(SoapReader30)); 	
	HRESULT hr4 = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument40)); 
	if(FAILED(hr1)||FAILED(hr2)||FAILED(hr3)||FAILED(hr4)) return NULL;

	try
	{	
		Connector->Property["EndPointURL"]=m_url.c_str();//连接属性得到将要进行连接的URL
		Connector->Connect();//和服务器连接 
		Connector->Property["SoapAction"] =g_HttpUrls[UrlIndex_GuideAction].c_str();
		Connector->BeginMessage(); //开始准备发送消息
		// 将serializer连接到connector的输入字符串 
		Serializer->Init(_variant_t((IUnknown*)Connector->InputStream));
		// 创建SOAP消息 
		Serializer->StartEnvelope("soap","",""); 
		Serializer->StartBody(""); 
		Serializer->StartElement("findArticleList",g_HttpUrls[UrlIndex_GuideNameSpace].c_str(),"","ns2");// 方法+命名空间
		Serializer->StartElement("type","","",""); 
		Serializer->WriteString( m_type.c_str() ); //传入类型	
		Serializer->EndElement();
		Serializer->StartElement("categoryId","","",""); 
		Serializer->WriteString( m_categoryId.c_str() ); //	
		Serializer->EndElement();
		Serializer->StartElement("fieldId","","",""); 
		Serializer->WriteString( m_fieldId.c_str() ); //	
		Serializer->EndElement();
		Serializer->StartElement("maxCount","","",""); 
		Serializer->WriteString( m_maxCount.c_str() ); //
		Serializer->EndElement();
		Serializer->EndElement(); 
		Serializer->EndBody(); 
		Serializer->EndEnvelope(); 	

		Connector->EndMessage(); //向网站发送消息	

		Reader->Load(_variant_t((IUnknown*)Connector->OutputStream), ""); 
		MSXML2::IXMLDOMNodePtr DOMptr=Reader->RpcResult->firstChild;//返回结果
		string strXML=(const char*)DOMptr->text;	
		_bstr_t bs = strXML.c_str();
		pDoc->loadXML(bs);
		MSXML2::IXMLDOMNodePtr pRootNode = pDoc->selectSingleNode("response/articleList");	
		if (pRootNode==NULL)	return false;
		MSXML2::IXMLDOMNodePtr pNode = pRootNode->firstChild;
		MSXML2::IXMLDOMNamedNodeMapPtr pAttr;
		long bFirst=1;
		while(pNode)
		{		
			pAttr = pNode->attributes;
			MSXML2::IXMLDOMNodePtr pIDAttr = pAttr->getQualifiedItem("id","");
			_bstr_t bs_id = pIDAttr->Gettext();

			MSXML2::IXMLDOMNodePtr pNameAttr = pAttr->getQualifiedItem("title","");
			_bstr_t bs_name = pNameAttr->Gettext();
			if(!TestDestroy()&&m_parent)
			{
				wxCommandEvent evt(wxEVT_SOAP_MSG);
				evt.SetInt(atoi((char *)bs_id ));
				evt.SetExtraLong(bFirst);
				evt.SetString((char *)bs_name);
				m_parent->AddPendingEvent(evt);
				bFirst=0;
			}
			pNode = pNode->nextSibling;
		}	
	}
	catch(...) 
	{


	}
	CoUninitialize();
	return NULL;
}

SoapGuideArticleThread::SoapGuideArticleThread(wxWindow* parent,wxString url,wxString articleId)
{
	m_parent=parent;
	m_url=url;
	m_articleId=articleId;
}
SoapGuideArticleThread::~SoapGuideArticleThread()
{

}
void* SoapGuideArticleThread::Entry()
{
	CoInitialize(NULL); //初始化
	ISoapSerializerPtr Serializer; //Serialize指针用于发送SOAP消息
	ISoapReaderPtr Reader; //定义读取指针
	ISoapConnectorPtr Connector; //定义连接指针
	MSXML2::IXMLDOMDocumentPtr pDoc; 

	HRESULT hr1=Connector.CreateInstance(__uuidof(HttpConnector30)); //创建连接实例
	HRESULT hr2=Serializer.CreateInstance(__uuidof(SoapSerializer30));//创建一个Serializer实例 
	HRESULT hr3=Reader.CreateInstance(__uuidof(SoapReader30)); 	
	HRESULT hr4 = pDoc.CreateInstance(__uuidof(MSXML2::DOMDocument40)); 
	if(FAILED(hr1)||FAILED(hr2)||FAILED(hr3)||FAILED(hr4)) return NULL;

	try
	{	
		Connector->Property["EndPointURL"]=m_url.c_str();//连接属性得到将要进行连接的URL
		Connector->Connect();//和服务器连接 
		Connector->Property["SoapAction"] =g_HttpUrls[UrlIndex_GuideAction].c_str();
		Connector->BeginMessage(); //开始准备发送消息
		// 将serializer连接到connector的输入字符串 
		Serializer->Init(_variant_t((IUnknown*)Connector->InputStream));
		// 创建SOAP消息 
		Serializer->StartEnvelope("soap","",""); 
		Serializer->StartBody(""); 
		Serializer->StartElement("showArticle",g_HttpUrls[UrlIndex_GuideNameSpace].c_str(),"","ns2");// 方法+命名空间
		Serializer->StartElement("articleId","","",""); 
		Serializer->WriteString( m_articleId.c_str() ); //传入类型	
		Serializer->EndElement();
		Serializer->EndElement(); 
		Serializer->EndBody(); 
		Serializer->EndEnvelope(); 	

		Connector->EndMessage(); //向网站发送消息	

		Reader->Load(_variant_t((IUnknown*)Connector->OutputStream), ""); 
		MSXML2::IXMLDOMNodePtr DOMptr=Reader->RpcResult->firstChild;//返回结果
		string strXML=(const char*)DOMptr->text;	
		_bstr_t bs = strXML.c_str();
		pDoc->loadXML(bs);
		MSXML2::IXMLDOMNodePtr pNode = pDoc->selectSingleNode("response/article");	
		if (pNode==NULL)	return false;
		MSXML2::IXMLDOMNamedNodeMapPtr pAttr;
		pAttr = pNode->attributes;
		MSXML2::IXMLDOMNodePtr pIDAttr = pAttr->getQualifiedItem("id","");
		_bstr_t bs_id = pIDAttr->Gettext();
		MSXML2::IXMLDOMNodePtr pTitleAttr = pAttr->getQualifiedItem("title","");
		_bstr_t bs_title = pTitleAttr->Gettext();
		MSXML2::IXMLDOMNodePtr pPubTimeAttr = pAttr->getQualifiedItem("pubTime","");
		_bstr_t bs_pubTime = pPubTimeAttr->Gettext();
		MSXML2::IXMLDOMNodePtr pFromAttr = pAttr->getQualifiedItem("sourceUrl","");
		_bstr_t bs_sourceUrl = pFromAttr->Gettext();
		MSXML2::IXMLDOMNodePtr pContentAttr = pAttr->getQualifiedItem("content","");
		_bstr_t bs_content = pContentAttr->Gettext();
		MSXML2::IXMLDOMNodePtr pAuthorAttr = pAttr->getQualifiedItem("author","");
		_bstr_t bs_author = pAuthorAttr->Gettext();
		wxString strTmpHtml=(char*)GlobalFunc::GetXmlFileResource("IDR_XML_CONTENT_HTML",_T("XML"));
#define BSTR_GETSTR(p)   ((char*)p)?((char*)p):""
		wxString htmlStr=wxString::Format(strTmpHtml,BSTR_GETSTR(bs_title), BSTR_GETSTR(bs_pubTime), BSTR_GETSTR(bs_sourceUrl),BSTR_GETSTR(bs_content));
		if(!TestDestroy()&&m_parent)
		{
			wxCommandEvent evt(wxEVT_SOAP_MSG);
			evt.SetExtraLong(-1);
			evt.SetString(htmlStr.c_str());
			evt.SetClientObject(new wxStringClientData((char*)bs_title));
			m_parent->AddPendingEvent(evt);
		}

	}
	catch(...) 
	{


	}
	CoUninitialize();
	return NULL;
}

#endif

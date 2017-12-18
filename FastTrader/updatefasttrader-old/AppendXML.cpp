#include "stdafx.h"
#include "AppendXML.h"

#pragma warning( disable : 4996 )


void AppendXML::ParaseUpdateXml(TiXmlNode* pParent,TiXmlNode* pParentDest, vecAttri& VecStr,  MapUnique& mapUnique)
{
	if(pParent == NULL)
		return;

	if(VecStr.size() == 0)
	{
		string str = pParent->Value();
		cout << pParent->Value() << endl;

		SAttribute* p = new SAttribute(str, "", "");
		VecStr.push_back(p);
	}

	TiXmlNode* pchild = pParent->FirstChild();
	while(pchild)
	{

		int t = pchild->Type();
		if( t == TiXmlNode::TINYXML_ELEMENT)
		{
			cout << pchild->Value() << " ";			

			vector<SAttribute*> VecStrTemp(VecStr);
			string strLastItem = pchild->Value();
			string strAttri    = "";
			string strValue    = "";
			map<string, string>::iterator iterT = mapUnique.find(strLastItem);
			if(iterT != mapUnique.end())
			{
				strAttri = iterT->second;
				const char*	pAttribute = pchild->ToElement()->Attribute(strAttri.c_str());
				if(pAttribute != NULL)
					strValue = pAttribute;			
			}
			SAttribute* p = new SAttribute(strLastItem,strAttri, strValue);
			VecStrTemp.push_back(p);

			vecAttri vecTemp;
			TiXmlAttribute* attr = pchild->ToElement()->FirstAttribute();
			if(attr)
			{
				TiXmlNode* node = pchild;
				while(node)
				{					
					bool bAddNewAttribute = true;
					while(attr)
					{
						cout << ""  << attr->Name() << "=" << attr->Value();
						string strA = attr->Name();
						string strV = attr->Value();
						SAttribute *p = new SAttribute(pchild->Value(),attr->Name(), attr->Value());
						vecTemp.push_back(p);
						attr = attr->Next();
						bAddNewAttribute = false;
					}
					cout << endl;
					node =  node->NextSiblingElement();
					break;
				}
			}
			if(!IsItemExist(pchild, pParentDest, VecStrTemp, vecTemp,mapUnique))
				InsertItem(pParentDest,  VecStrTemp, vecTemp);
			ParaseUpdateXml(pchild, pParentDest, VecStrTemp, mapUnique);
		}
		else if( t == TiXmlNode::TINYXML_TEXT)
		{
			string str = pchild->Value();		
			InsertItem(pParentDest,  VecStr, VecStr, str);
		}
		pchild =  pchild->NextSibling();
	}
}
bool AppendXML::GetUniqueMap(string strFileName, MapUnique& mapUnique)
{
	string strPath = strFileName;
	TiXmlDocument doc(strPath.c_str());
	if(!doc.LoadFile())
	{
		wxString str = wxString::Format(_T("read xml file [%s] rror!\n"), strPath.c_str());
		OutputDebugString(str.c_str());	
		return false;
	}

	TiXmlElement* root = doc.RootElement();	
	if(!root) return false;
	TiXmlNode *pNode = root->FirstChild();
	while(pNode)
	{
		string strName = pNode->Value(); 
		string strValue ="";
		TiXmlNode *pValue = pNode->FirstChild();
		if(pValue)
			strValue = pValue->Value();

		mapUnique.insert(pair <string,string> (strName, strValue));
		pNode = pNode->NextSibling();
	}
	return true;
}
bool AppendXML::IsItemExist(TiXmlNode* pSrcChild,TiXmlNode* pParentDest, vecAttri& VecStr, vecAttri& vecTemp, MapUnique& mapUnique)
{
	if(VecStr.size() == 0|| VecStr.size() == 1)
		return false;

	TiXmlNode *pNode = pParentDest;
	for(vector<SAttribute*>::iterator iter = VecStr.begin()+1; iter != VecStr.end(); iter++)
	{
		SAttribute* p = *iter;
		pNode = pNode->FirstChild(p->strItem.c_str());
		if(p->strAttri != "")
		{			
			while(pNode)
			{
				const char*	pAttribute = pNode->ToElement()->Attribute(p->strAttri.c_str());
				if(pAttribute != NULL)
				{
					string str = pAttribute;
					if( str == p->strValue)
					{	//					
						break;
					}
					pNode = pNode->NextSibling(p->strItem.c_str());
				}
				else
					break;				
			}			
		}
		if(!pNode)
		{
			return false;
		}			
	}

	SAttribute* pLastItem = VecStr[VecStr.size() -1];//比如 Instrument
	map<string, string>::iterator iterT = mapUnique.find(pLastItem->strItem);
	if(iterT == mapUnique.end())
	{//item 的唯一性为ID，但是ID却不是该有的属性
		if(pNode != NULL)
			return true;
		return false;
	}
	else
	{//如果在新的升级文件中没有该唯一性的标识，同时旧文件中已经有该节点，则不升级
		if(pSrcChild)
		{
			const char*	pAttribute = pSrcChild->ToElement()->Attribute(pLastItem->strItem.c_str());
			if(pAttribute == NULL)
				return true;
		}
	}

	string strAttri = iterT->second;

	string strAttributeValue = "";
	for(vector<SAttribute*>::iterator iter = vecTemp.begin(); iter != vecTemp.end(); iter++)
	{
		SAttribute* pAttri = *iter;
		if(pAttri->strAttri == strAttri)
		{
			strAttributeValue = pAttri->strValue;
			break;
		}
	}

	if(strAttributeValue == "")
		return false;

	while (pNode)
	{		
		string str = pNode->Value();
		//	if(str != strLastItem)
		//		return false;
		TiXmlElement *pElemDest = pNode->ToElement();


		const char*	pAttribute = pElemDest->Attribute(strAttri.c_str());
		if(pAttribute == NULL)
			return false;

		string strValueDest(pAttribute);

		if(strAttributeValue != "" && strAttributeValue == strValueDest)
			return true;
		pNode = pNode->NextSibling(pLastItem->strItem.c_str());
	}
	return false;
}
bool AppendXML::InsertItem(TiXmlNode* pParentDest, vecAttri& VecStr,  vecAttri& vecTemp, string strValue)
{
	if(VecStr.size() == 0 || VecStr.size() == 1)
		return true;

	vector<SAttribute*> VecStrTemp(VecStr);
	VecStrTemp.erase(VecStrTemp.end()-1);

	TiXmlNode *pNode = pParentDest;

	for(vector<SAttribute*>::iterator iter = VecStrTemp.begin()+1; iter != VecStrTemp.end(); iter++)
	{
		SAttribute* p = *iter;
		pNode = pNode->FirstChild(p->strItem.c_str());

		if(p->strAttri != "")
		{			
			while(pNode)
			{
				const char*	pAttribute = pNode->ToElement()->Attribute(p->strAttri.c_str());
				if(pAttribute != NULL)
				{
					string str = pAttribute;
					if( str == p->strValue)
					{						
						break;
					}
					pNode = pNode->NextSibling(p->strItem.c_str());
				}
				else
					break;				
			}
		}
	}

	

	if(strValue != "")
	{
		SAttribute* pLast = VecStr[VecStr.size() - 1];
		pNode= pNode->FirstChild(pLast->strItem.c_str());
		if(pNode == NULL)
			return false;
		TiXmlElement *pElementItem = pNode->ToElement();
		if(pElementItem)
		{
			TiXmlNode *pNodeValue = pElementItem->FirstChild();
			if(pNodeValue && pNodeValue->Value() != "")
			{//已经有这个节点，则忽略不做处理
			//eg: <Action>5002</Action> 已经有action则忽略不做处理
				return true;
			}

			pNode = pElementItem->FirstChild(strValue.c_str());
			TiXmlText newText(strValue.c_str());
			if(pNode)
			{
				//pElementItem->ReplaceChild(pNode, newText );		
			}
			else
			{
				//TiXmlText * ptext = new TiXmlText( newText );
				pElementItem->InsertEndChild(newText);
			}
		}
	}
	else
	{
		SAttribute* pItem = VecStr[VecStr.size() -1];
		TiXmlElement *InstrumentItem = new TiXmlElement(pItem->strItem.c_str());

		for(vector<SAttribute*>::iterator iterSAttri = vecTemp.begin(); iterSAttri != vecTemp.end(); iterSAttri ++)
		{
			SAttribute* p = *iterSAttri;
			string strAttribute = p->strAttri;
			string strAttriValue = p->strValue;
			InstrumentItem->SetAttribute(strAttribute.c_str(), strAttriValue.c_str());
		}	
		pNode->LinkEndChild(InstrumentItem);
	}



	return true;

}
void AppendXML::SetFiles(string strUnique, string strFile, string strUpdateFile)
{
	m_strUnique		= strUnique;
	m_strFile		= strFile;
	m_strUpdateFile = strUpdateFile;
}
bool AppendXML::ProcessFile()
{
	string strPathU = m_strUnique;
	MapUnique mapUnique;
	if(!GetUniqueMap(m_strUnique, mapUnique))
	{
		OutputDebugString(_T("GetUniqueMap ERROR!"));
		return false;
	}


	string strPath = m_strUpdateFile;
	TiXmlDocument doc(strPath.c_str());
	if(!doc.LoadFile())
	{
		wxString str = wxString::Format(_T("read xml file [%s] rror!\n"), strPath.c_str());
		OutputDebugString(str.c_str());	
		return false;
	}

	TiXmlElement* root = doc.RootElement();
	if(!root)
	{
		wxString str = wxString::Format(_T("read xml file [%s] rror!\n"), strPath.c_str());
		OutputDebugString(str.c_str());	
		return false;
	}


	string strPath2 = m_strFile;
	TiXmlDocument doc2(strPath2.c_str());
	if(!doc2.LoadFile())
	{
	//	wxString str = wxString::Format(_T("loadfile [%s] failed!\n"), strPath2.c_str());
	//	OutputDebugString(str.c_str());	
		doc.SaveFile(strPath2.c_str());
		return true;
	}

	TiXmlElement* root2 = doc2.RootElement();
	if(!root2)
	{
		doc.SaveFile(strPath2.c_str());
		return true;
	}

	string strRoot1 = root->Value();
	string strRoot2 = root2->Value();
	if(strRoot1 != strRoot2)
	{
		doc.SaveFile(strPath2.c_str());
		return true;
	}
	TiXmlNode *pNode1 = root->FirstChild("version");
	TiXmlNode *pNode2 = root2->FirstChild("version");
	if(pNode1 && pNode2)
	{
		const char *p1 = pNode1->ToElement()->GetText();
		const char *p2 = pNode2->ToElement()->GetText();
		if(p1 && p2)
		{
		//	if(strcmp(p1, p2) > 0)
			if(compareVersion(p2, p1))
			{
				string strValue =  p1;
				TiXmlText newText(strValue.c_str());
				if(pNode1)
				{
					TiXmlNode *nodeT = pNode2->FirstChild();
					pNode2->ToElement()->ReplaceChild(nodeT, newText );		
				}
			}
			else
				return true;//本地的版本更高，则不升级
		}
	}


	vecAttri vecStr;
	ParaseUpdateXml(root, root2, vecStr,mapUnique);
	doc2.SaveFile(strPath2.c_str());

	return true;
}
bool AppendXML::compareVersion(const char* localVersion,const char* remoteVersion)
{
	int i1,i2,i3,i4;
	int j1,j2,j3,j4;
	sscanf(localVersion,"%d,%d,%d,%d",&i1,&i2,&i3,&i4);
	sscanf(remoteVersion,"%d,%d,%d,%d",&j1,&j2,&j3,&j4);

	if(i1>j1||
		i1==j1&&i2>j2||
		i1==j1&&i2==j2&&i3>j3||
		i1==j1&&i2==j2&&i3==j3&&i4>j4)
	{
		//localversion>remoteversion
		return false;
	}
	else if(i1==j1&&i2==j2&&i3==j3&&i4==j4)
	{
		//local==remote
		return false;
	}
	
	//local<remote
	return true;
}
#pragma once
#pragma warning( disable : 4996 )
class CStringPair
{
public:
	CStringPair()
	{
		std::string strxmlpath="pair.hash";
		GetFolderFileName(strxmlpath,"cache");
		char strIndex[64]={0},strValue[256]={0};
		int index=0;
		while (true)
		{
			sprintf(strIndex,"%d",++index);
			if(0==GetPrivateProfileString("main",strIndex,NULL,strValue,256,strxmlpath.c_str()))
				break;
			m_vector_content.push_back(strValue);
			m_map_content[strValue]=index;
		}
	}
	virtual ~CStringPair()
	{	
		std::string strxmlpath="pair.hash";
		GetFolderFileName(strxmlpath,"cache");
		char strIndex[256]={0};
		for (size_t index=0;index<m_vector_content.size();++index)
		{
			sprintf(strIndex,"%d",index);
			WritePrivateProfileString("main",strIndex,m_vector_content[index].c_str(),strxmlpath.c_str());
		}
	}
	int Get(const std::string& str)
	{
		if(m_map_content.count(str)==0)
		{
			m_vector_content.push_back(str);
			int index=static_cast<int>(m_vector_content.size());
			m_map_content.insert(std::make_pair(str,index));
			return index;
		}
		else
			return m_map_content[str];
	}

private:
	std::map<std::string,int>	m_map_content;
	std::vector<std::string>	m_vector_content;
};

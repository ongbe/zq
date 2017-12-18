#ifndef ENCRYPT_CHARACTERMAP_H_
#define ENCRYPT_CHARACTERMAP_H_

//定义一种字符映射策略加密算法
//1. 这是个单实例类


#ifdef WIN32
#pragma managed(push,off)
#endif 



class CEncrypt_Charactermap
{
public:
	//取对象，必要时创建对象
	static CEncrypt_Charactermap& getObj(void);

    void EncryptString(char* pString);
    void DecryptString(char* pString);

private:
	unsigned char DecryptMap[256];
	static CEncrypt_Charactermap* m_pObj;

	CEncrypt_Charactermap();
};

#ifdef WIN32
#pragma managed(pop)
#endif 


#endif

#include "StdAfx.h"
#include "stdio.h"
#include <wincrypt.h>
#include "CryptoAPI.h"

bool CCryptoAPI::Encode3DES(std::string srcstr,UCHAR** pwd,int& pwdlen)
{
	HCRYPTPROV hCryptProv;
	HCRYPTKEY hKey; 
	if(GetHCRYPTPROV(hCryptProv)&&GetHCRYPTKEY(hCryptProv,hKey))
	{
		DWORD dwSrclenth=srcstr.size(),dwPwdLength=srcstr.size();
		if(CryptEncrypt(hKey,NULL,TRUE,0,NULL,&dwPwdLength,srcstr.size()))
		{
			*pwd =new UCHAR[dwPwdLength];
			memcpy(*pwd,srcstr.c_str(),srcstr.size());
			if(CryptEncrypt(hKey,NULL,TRUE,0,*pwd,&dwSrclenth,dwPwdLength))
			{
				pwdlen=dwPwdLength;
				return true;
			}
		}
	}
	return false;
}
BOOL CCryptoAPI::GetHCRYPTPROV(HCRYPTPROV& hCryptProv)
{
	if(CryptAcquireContext(
		&hCryptProv,               // 返回CSP句柄
		NULL,                  // 密码容器名
		NULL,                      // NULL时使用默认CSP名（微软RSA Base Provider）
		PROV_RSA_SCHANNEL,             // CSP类型
		0))                        // Flag values
	{
		return TRUE;
	}
	return CryptAcquireContext(
		&hCryptProv, 
		NULL, 
		NULL, 
		PROV_RSA_SCHANNEL, 
		CRYPT_NEWKEYSET); //创建以UserName为名的密钥容器
	return true;
}
BOOL CCryptoAPI::GetHCRYPTKEY(const HCRYPTPROV& hCryptProv,HCRYPTKEY& hKey)
{
	static BYTE DesKeyBlob[] = {
		0x08,0x02,0x00,0x00,0x09,0x66,0x00,0x00, // BLOB header 
		0x10,0x00,0x00,0x00,                     // key length, in bytes
		0xF8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01  // DES key with parity
	};
	CryptImportKey(hCryptProv, DesKeyBlob, sizeof(DesKeyBlob), 0, CRYPT_EXPORTABLE, &hKey);
	return TRUE;
}
bool CCryptoAPI::Decode3DES(const UCHAR* pwd,const int pwdlen,std::string& srcstr)
{
	HCRYPTPROV hCryptProv;
	HCRYPTKEY hKey; 
	DWORD dwLength=pwdlen;
	UCHAR* EncryptedText=new UCHAR[dwLength+1];
	memcpy(EncryptedText,pwd,dwLength);
	if(GetHCRYPTPROV(hCryptProv)&&GetHCRYPTKEY(hCryptProv,hKey))
	{
		if(CryptDecrypt(hKey,NULL,TRUE,0,EncryptedText,&dwLength))
		{
			EncryptedText[dwLength]=0;
			srcstr=(char*)EncryptedText;
			delete EncryptedText;
			return true;
		}
	}
	delete EncryptedText;
	return false;
}
char CCryptoAPI::GetHex(int n)
{
	if(n>=0&&n<=9)
		return '0'+n;
	else
		return 'A'+n-10;
}
int CCryptoAPI::GetChar(char c)
{
	if(c>='0'&&c<='9')
		return c-'0';
	else
		return c-'A'+10;
}
void CCryptoAPI::Encode3DES_ToString( char* strText)
{
	if(strText)
	{
		std::string strPassword;
		UCHAR* pwd=NULL;
		int pwdlen=0;
		Encode3DES(strText,&pwd,pwdlen);
		for (int i=0;i<pwdlen;++i)
		{
			strPassword+=GetHex(pwd[i]>>4);
			strPassword+=GetHex(pwd[i]&0xF);
		}
		strcpy(strText,strPassword.c_str());
		delete pwd ;
	}

}

void CCryptoAPI::Decode3DES_ToString( char* strPwd)
{
	int len=strlen(strPwd);
	UCHAR* tmp=new UCHAR[len/2];
	ZeroMemory(tmp,len/2);
	for (int i=0;i<len;++i)
	{
		if(i%2==0)
			tmp[i/2]|=(GetChar(strPwd[i])<<4);
		else
			tmp[i/2]|=GetChar(strPwd[i]);
	}
	std::string srcstr;
	CCryptoAPI::Decode3DES(tmp,len/2,srcstr);
	strcpy(strPwd,srcstr.c_str());
	delete  tmp;
}

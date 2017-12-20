#include <string>
class CCryptoAPI
{
public:
	static void Encode3DES_ToString( char* strText);
	static void Decode3DES_ToString( char* strPwd);	
	static bool Encode3DES(std::string srcstr,UCHAR** pwd,int& pwdlen);
	static bool Decode3DES(const UCHAR* pwd,const int pwdlen,std::string& srcstr);
protected:
	static BOOL GetHCRYPTPROV(HCRYPTPROV& hCryptProv);
	static BOOL GetHCRYPTKEY(const HCRYPTPROV& hCryptProv,HCRYPTKEY& hKey);
	static char GetHex(int n);
	static int GetChar(char c);
};

//定义一种字符映射策略加密算法


#include "encrypt_charactermap.h"
#include <string.h>

#ifdef WIN32
#pragma unmanaged
#endif 

// 字符映射表
const unsigned char EncryptMap[256-32] = {
	228, 	178, 	68, 	155, 	118, 	177, 	35, 	221, 	126, 	231, 	
	180, 	46, 	161, 	208, 	63, 	249, 	47, 	116, 	182, 	210, 	
	36, 	235, 	176, 	166, 	100, 	58, 	150, 	223, 	237, 	198, 	
	137, 	169, 	81, 	69, 	119, 	234, 	52, 	111, 	127, 	164, 	
	195, 	64, 	136, 	60, 	142, 	101, 	70, 	191, 	192, 	190, 	
	255, 	165, 	230, 	132, 	160, 	187, 	139, 	43, 	92, 	114, 	
	250, 	107, 	88, 	33, 	84, 	233, 	51, 	91, 	144, 	44, 	
	207, 	181, 	93, 	90, 	40, 	227, 	185, 	152, 	240, 	121, 	
	163, 	123, 	143, 	246, 	239, 	124, 	244, 	154, 	77, 	94, 	
	175, 	254, 	85, 	215, 	162, 	170, 	134, 	86, 	98, 	211, 	
	61, 	48, 	120, 	217, 	138, 	95, 	146, 	99, 	213, 	75, 	
	65, 	87, 	197, 	125, 	106, 	140, 	149, 	115, 	96, 	199, 	
	194, 	39, 	97, 	252, 	103, 	109, 	37, 	209, 	245, 	128, 	
	117, 	49, 	67, 	214, 	129, 	151, 	148, 	45, 	41, 	205, 	
	80, 	225, 	168, 	108, 	79, 	174, 	171, 	236, 	38, 	202, 	
	32, 	220, 	229, 	72, 	184, 	110, 	159, 	59, 	226, 	156, 	
	183, 	242, 	189, 	206, 	57, 	248, 	56, 	172, 	73, 	83, 	
	200, 	74, 	53, 	157, 	167, 	89, 	147, 	201, 	42, 	82, 	
	112, 	179, 	131, 	186, 	50, 	158, 	130, 	62, 	141, 	54, 	
	102, 	204, 	105, 	153, 	122, 	145, 	238, 	251, 	203, 	241, 	
	218, 	188, 	212, 	193, 	113, 	135, 	133, 	76, 	104, 	196, 	
	55, 	232, 	66, 	243, 	173, 	216, 	247, 	253, 	78, 	34, 	
	219, 	222, 	224, 	71 };

CEncrypt_Charactermap* CEncrypt_Charactermap::m_pObj=0;


CEncrypt_Charactermap& CEncrypt_Charactermap::getObj(void)
{
    if(!m_pObj)
        m_pObj=new CEncrypt_Charactermap();
    return *m_pObj;
}

CEncrypt_Charactermap::CEncrypt_Charactermap()
{
    memset(DecryptMap,0,sizeof(DecryptMap));
    //根据EncryptMap, 创建DecryptMap
    for(int i=32;i<(int)sizeof(DecryptMap);i++)
    {
        for(int j=0;j<(int)sizeof(EncryptMap);j++)
        {
            if(EncryptMap[j]==(unsigned char)i)
            {
                DecryptMap[i]=(unsigned char)j+32;
                break;
            }
        }
    }
}

void CEncrypt_Charactermap::EncryptString(char* pString)
{
	if(!pString) return;

    int len=strlen(pString);
    for(int i=0;i<len;i++)
    {
        unsigned char ch=(unsigned char)pString[i];
        if(ch>=32)
        {
            pString[i]=(char)EncryptMap[ch-32];
        }
    }
}
void CEncrypt_Charactermap::DecryptString(char* pString)
{
	if(!pString) return;

    int len=strlen(pString);
    for(int i=0;i<len;i++)
    {
        unsigned char ch=(unsigned char)pString[i];
        if(ch>=32&&(int)ch<(int)sizeof(DecryptMap))
        {
            pString[i]=(char)DecryptMap[ch];
        }
    }
}



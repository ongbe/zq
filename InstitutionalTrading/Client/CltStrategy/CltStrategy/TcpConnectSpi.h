#pragma once

class CTcpConnectSpi
{
public:
	virtual void OnFrontConnected()=0;
	virtual void OnFrontDisconnected()=0;
	// 解析数据，仅在收到tcp数据时调用
	virtual BOOL OnRecvData(char* pData, int nDataLen, DWORD nCmdId, DWORD nModuleId, 
			DWORD nSeq, DWORD nSubSeq, DWORD nUserData1, DWORD nUserData2, DWORD nUserData3, 
			DWORD nUserData4, char* pSrcPackage)=0;

protected:
	CTcpConnectSpi() {};
	virtual ~CTcpConnectSpi() {};
    
};

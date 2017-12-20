#ifndef __INSTRUMENTSOCKET_H__
#define __INSTRUMENTSOCKET_H__
class CInstrumentSocket
{
public:
	CInstrumentSocket(string strAddr);
	~CInstrumentSocket(void);
protected:
	SOCKET CreateTcpSocket(void);
	bool string_getline(char* pData,char* out,int len);
	void GetString(char* pData,int datalen);
private:
	char* m_pTmpRecvBuf;
	int m_SORCVBUFSize;
	int m_SOSNDBUFSize;
	SOCKET	m_hSocket;
};
#endif
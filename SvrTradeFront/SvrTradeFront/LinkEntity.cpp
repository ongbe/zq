#include "LinkEntity.h"
#include "SvrTradeFront.h"
#include "FTTraderApi.h"
#include "openssl/err.h"
#include "openssl/pem.h"

#include "FileOpr.h"
#define LOG_INFO(fmt, ...)  CFileOpr::getObj().writelocallogDaily("log","SvrTradefrontModule", "[%s][%d]:"fmt, __FILE__, __LINE__, __VA_ARGS__);

SSL_METHOD* CLinkEntity::sm_SLLMeth = NULL;
SSL_CTX* CLinkEntity::sm_SSLCtx = NULL;
SOCKET CLinkEntity::sm_SocketListen = 0;

DWORD WINAPI CLinkEntity::Proc_SSLRead(LPVOID pParameter)
{
	if(pParameter==NULL)
		return -1;

	CLinkEntity* pEntity = (CLinkEntity*)pParameter;

	if(!pEntity->SSLAccept())
		return -2;

	int nBuffLen = 2*1024*1024;
	char* pBuff = new char[nBuffLen];
	int nDataLen = 0;

	memset(pBuff, 0, nBuffLen);
	while((nDataLen = SSL_read(pEntity->m_pSSL, pBuff, nBuffLen)) > 0) {
		// 这里实现解析数据包

		
		//pEntity->ParseOnePackage(pBuff+nPackIndex, nPackLen);


		memset(pBuff, 0, nBuffLen);
	}

	delete pBuff;

	return 0;
}

DWORD WINAPI CLinkEntity::Proc_SSLWrite(LPVOID pParameter)
{
	if(pParameter==NULL)
		return -1;

	CLinkEntity* pEntity = (CLinkEntity*)pParameter;
	BUFFDATA* pData = NULL;
	int nDataLen = 0;

	while(TRUE) {
		if((pData = (BUFFDATA*)pEntity->PopOneSendPackage()) != NULL) {
			nDataLen = SSL_write(pEntity->m_pSSL, pData->pBuff, pData->nLen);
			delete pData->pBuff;
			if(nDataLen <= 0)
				break;
		}
		Sleep(50);
	}

	return 0;
}


BOOL CLinkEntity::InitSSL()
{
	int err = 0;
	short int nPort = {0};
	struct sockaddr_in sa_serv;
	char csAddress[128] = {0};
	char cscertfile[MAX_PATH] = {0};
	char csprikeyfile[MAX_PATH] = {0};

	char lsPath[MAX_PATH] = {0};
	GetModuleFileName(NULL,lsPath,MAX_PATH);
	*(strrchr(lsPath,'\\') + 1)= NULL;
	strcat(lsPath,SSLLINKSETTING_FILE);

	//memset(csAddress,0,sizeof(csAddress));
	//GetPrivateProfileString("connect","RemoteAddr","127.0.0.1",csAddress,sizeof(csAddress)-1,lsPath);
	//m_strRemoteAddr = csAddress;
	//m_nRemotePort = (short int)GetPrivateProfileInt("connect","RemotePort",9009,lsPath);

	//memset(csAddress,0,sizeof(csAddress));
	//GetPrivateProfileString("connect","LocalAddr","127.0.0.1",csAddress,sizeof(csAddress)-1,lsPath);
	nPort = (short int)GetPrivateProfileInt("connect","LocalPort",9004,lsPath);
	
	memset(cscertfile,0,sizeof(cscertfile));
	GetPrivateProfileString("SSL","CertFile","C:\\cacert\\cacert.pem",cscertfile,sizeof(cscertfile)-1,lsPath);
	memset(csprikeyfile,0,sizeof(csprikeyfile));
	GetPrivateProfileString("SSL","privateKeyFile","C:\\cacert\\privatekey.pem",csprikeyfile,sizeof(csprikeyfile)-1,lsPath);

	SSL_load_error_strings();
	SSLeay_add_ssl_algorithms();
	sm_SLLMeth = (SSL_METHOD*)SSLv23_server_method();
	sm_SSLCtx = SSL_CTX_new (sm_SLLMeth);
	if (!sm_SSLCtx) {
		ERR_print_errors_fp(stderr);
		return FALSE;
	}

	if (SSL_CTX_use_certificate_file(sm_SSLCtx, cscertfile, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		return FALSE;
	}
	if (SSL_CTX_use_PrivateKey_file(sm_SSLCtx, csprikeyfile, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		return FALSE;
	}

	if (!SSL_CTX_check_private_key(sm_SSLCtx)) {
		return FALSE;
	}

	sm_SocketListen = socket (AF_INET, SOCK_STREAM, 0);
	//CHK_ERR(listen_sd, "socket");
	if(sm_SocketListen == INVALID_SOCKET) {
		return FALSE;
	}

	memset (&sa_serv, '\0', sizeof(sa_serv));
	sa_serv.sin_family      = AF_INET;
	sa_serv.sin_addr.s_addr = htonl(INADDR_ANY);
	sa_serv.sin_port        = htons (nPort);

	err = bind(sm_SocketListen, (LPSOCKADDR) &sa_serv, sizeof(sa_serv));
	//CHK_ERR(err, "bind");
	if(err == INVALID_SOCKET) {
		return FALSE;
	}

	err = listen (sm_SocketListen, 5);
	//CHK_ERR(err, "listen");
	if(err == INVALID_SOCKET) {
		return FALSE;
	}

	return TRUE;
}

BOOL CLinkEntity::ReleaseSSL()
{
	closesocket (sm_SocketListen);
	SSL_CTX_free (sm_SSLCtx);
	return FALSE;
}

CLinkEntity::CLinkEntity(void)
{
}

CLinkEntity::~CLinkEntity(void)
{	
}

BOOL CLinkEntity::Init(int nSocket, BOOL bForSSL)
{
	m_hSocket = nSocket;
	m_bForSSL = bForSSL;

	DWORD dwThreadID = 0;

	m_hRecvThread = ::CreateThread(NULL, 0, Proc_SSLRead, this, 0, &dwThreadID);
	m_hSendThread = ::CreateThread(NULL, 0, Proc_SSLWrite, this, CREATE_SUSPENDED, &dwThreadID);

	return TRUE;
}

void CLinkEntity::Release()
{
	
}

BOOL CLinkEntity::SSLAccept()
{
	int err = 0;
	SSL* ssl = SSL_new (sm_SSLCtx);
	//CHK_NULL(ssl);
	if(ssl == NULL)
		return FALSE;

	SSL_set_fd (ssl, m_hSocket);
	err = SSL_accept (ssl);
	//CHK_SSL(err);
	if(err == -1) {
		SSL_free (ssl);
		return FALSE;
	}

	X509* client_cert = NULL;
	char* str = NULL;
	client_cert = SSL_get_peer_certificate (ssl);
	if (client_cert != NULL) {
		str = X509_NAME_oneline (X509_get_subject_name (client_cert), 0, 0);
		//CHK_NULL(str);
		OPENSSL_free (str);

		str = X509_NAME_oneline (X509_get_issuer_name  (client_cert), 0, 0);
		//CHK_NULL(str);
		OPENSSL_free (str);

		X509_free (client_cert);
	}

	m_pSSL = ssl;
	::ResumeThread(m_hSendThread);

	CFTTraderApi* pFFTApi = new CFTTraderApi();
	//pFFTApi->Init(this);
	m_pFFTApi = pFFTApi;

	return TRUE;
}

// 用于SSL发送线程获得一个缓存的数据包
LPVOID CLinkEntity::PopOneSendPackage()
{
	return NULL;
}

int CLinkEntity::SendData(void* pBuff, int nLen)
{
	return 0;
}

//发送数据, 对sendData的一个封装
int CLinkEntity::SendPkgData(unsigned int nCMDID, const void* pData, int nLen, 
							 unsigned int seq, unsigned int Subseq, unsigned int Userdata1, 
							 unsigned int Userdata2, unsigned int Userdata3, unsigned int Userdata4)
{
	return 0;
}

// 用于SSL或非SSL接收线程解析一个数据包
int CLinkEntity::ParseOnePackage(void* pBuff, int nPackageLen)
{
	return 0;
}

BOOL CLinkEntity::ReqCloseSSLLinkEntity()
{
	return TRUE;
}

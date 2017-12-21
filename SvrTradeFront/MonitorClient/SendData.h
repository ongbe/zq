#pragma once

class CSendData
{
public:
	CSendData(void);
	~CSendData(void);

	static void Init(void);
	static void Release(void);
	static CSendData* Get();
	static CSendData* m_SendData;


	void AddMsg( const RecvData& data);
	bool GetMsg( RecvData& data);

	void AddClientMsg( const RecvDataMsg& data );
	bool GetClientMsg( RecvDataMsg& data );




private:
	CCmdMsgQueue m_cmdMsgQueue;
	CRITICAL_SECTION m_CS;

		
};

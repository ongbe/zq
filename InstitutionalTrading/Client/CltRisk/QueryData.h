#pragma once
#include <vector>
#define  ERROR_PROMPT            "提示"
#define  ERROR_CANNOTSCREENING   "所设查询条件未筛选出任何帐号"
#define  ERROR_QUERY_TIMEOUT     "查询超时"
#define  ERROR_QUERY_NONDATA     "查询无返回数据"
#define  ERROR_QUERY_DATEPROMPT  "起始日期必须小于等于结束日期"
class CQueryData
{
public:
	CQueryData(void);
public:
	static CQueryData* NewInstance();
	static void DestroyInstance();
	static CQueryData* m_pInstance;

	//查询SEQ记录
	void SetSendSeq(int nSeq);
	void GetSendSeq(std::vector<int>&   vecSeq);
	void ReceiveSeq(int nSeq);
	bool IsEmpty();
	void ClearSendSeq();

private:
	 std::vector<int>   m_vecSeq;

};

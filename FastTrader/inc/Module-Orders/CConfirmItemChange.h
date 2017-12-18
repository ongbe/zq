#pragma once
#include "stdafx.h"

class CConfirmItemChange
{
public:
	enum {	conNothing, 
			conStdOrderConfirm,
			conAutoQuotClickConfirm,
			conAutoQuotDBClickConfirm,
			conAutoPositionClickConfirm,
			conAutoPositionDBClickConfirm,
			conAutoKeyPressConfirm
	};


public:
	CConfirmItemChange();
	~CConfirmItemChange();

	CConfirmItemChange& operator= (const CConfirmItemChange& other)
	{
		if(this == &other)
			return *this;

		m_nItemNum = other.m_nItemNum;
		m_strItemParam = other.m_strItemParam;
		return *this;
	};

	void SetItemNum(int nItemNum) {
		m_nItemNum = nItemNum;
	};
	int GetItemNum() {
		return m_nItemNum;
	};
	void SetItemParam(std::string& strParam) {
		m_strItemParam = strParam;
	};
	std::string& GetItemParam() {
		return m_strItemParam;
	};

	//void ConfirmAgent(bool bHasConfirm);
	void ChgToNotConfirm();

private:
	int				m_nItemNum;
	std::string		m_strItemParam;
};

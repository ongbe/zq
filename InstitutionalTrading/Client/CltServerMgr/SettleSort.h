#pragma once
//成交
class Comparer_0
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		string strtemp1(m1.ExchangeID);
		string strtemp2(m2.ExchangeID);

		return strtemp1 < strtemp2;	
	}
};
class Comparer_1
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
        string strtemp1(m1.TradeID);
		string strtemp2(m2.TradeID);
		CTools_Ansi::mytrimleft(strtemp1);
		CTools_Ansi::mytrimleft(strtemp2);
		return atoi(strtemp1.c_str()) < atoi(strtemp2.c_str());	
	}
};
class Comparer_2
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.Direction < m2.Direction;	
	}
};
class Comparer_3
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		string strtemp1(m1.InvestorID);
		string strtemp2(m2.InvestorID);
		return strtemp1 < strtemp2;	
	}
};
class Comparer_4
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		string strtemp1(m1.InstrumentID);
		string strtemp2(m2.InstrumentID);
		return strtemp1 < strtemp2;	
	}
};
class Comparer_5
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.OffsetFlag < m2.OffsetFlag;	
	}
};
class Comparer_6
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.HedgeFlag < m2.HedgeFlag;
	}
};
class Comparer_7
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.Volume < m2.Volume;	
	}
};

class Comparer_8
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.Price< m2.Price;	
	}
};
class Comparer_9
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		string strtemp1(m1.TradeTime);
		string strtemp2(m2.TradeTime);
		return strtemp1 < strtemp2;	

	}
};

class Comparer_10
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.TradeType < m2.TradeType;
	}
};
class Comparer_11
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		string strtemp1(m1.OrderSysID);
		string strtemp2(m2.OrderSysID);
		CTools_Ansi::mytrimleft(strtemp1);
		CTools_Ansi::mytrimleft(strtemp2);
		return atoi(strtemp1.c_str()) < atoi(strtemp2.c_str());	
	}
};
class Comparer_12
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.CloseProfitByDate < m2.CloseProfitByDate;	
	}
};
class Comparer_13
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.TradeCommission < m2.TradeCommission;	
	}
};
class Comparer_14
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		string strtemp1(m1.TradeDate);
		string strtemp2(m2.TradeDate);
		return strtemp1 < strtemp2;	
	}
};

class AComparer_0
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		string strtemp1(m1.ExchangeID);
		string strtemp2(m2.ExchangeID);
		return strtemp1 > strtemp2;	
	}
};
class AComparer_1
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		string strtemp1(m1.TradeID);
		string strtemp2(m2.TradeID);
		CTools_Ansi::mytrimleft(strtemp1);
		CTools_Ansi::mytrimleft(strtemp2);
		return atoi(strtemp1.c_str()) > atoi(strtemp2.c_str());	
	}
};
class AComparer_2
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.Direction > m2.Direction;
	}
};
class AComparer_3
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		string strtemp1(m1.InvestorID);
		string strtemp2(m2.InvestorID);
		return strtemp1 > strtemp2;	
	}
};
class AComparer_4
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		string strtemp1(m1.InstrumentID);
		string strtemp2(m2.InstrumentID);
		return strtemp1 > strtemp2;	
	}
};
class AComparer_5
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.OffsetFlag > m2.OffsetFlag;	
	}
};
class AComparer_6
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.HedgeFlag > m2.HedgeFlag;
	}
};
class AComparer_7
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.Volume > m2.Volume;	
	}
};
class AComparer_8
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.Price> m2.Price;	
	}
};
class AComparer_9
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		string strtemp1(m1.TradeTime);
		string strtemp2(m2.TradeTime);
		return strtemp1 > strtemp2;	

	}
};

class AComparer_10
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{	
		return m1.TradeType > m2.TradeType;
	}
};
class AComparer_11
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		string strtemp1(m1.OrderSysID);
		string strtemp2(m2.OrderSysID);
		CTools_Ansi::mytrimleft(strtemp1);
		CTools_Ansi::mytrimleft(strtemp2);
		return atoi(strtemp1.c_str()) > atoi(strtemp2.c_str());	
	}
};
class AComparer_12
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.CloseProfitByDate > m2.CloseProfitByDate;	
	}
};
class AComparer_13
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		return m1.TradeCommission > m2.TradeCommission;	
	}
};
class AComparer_14
{
public:

	bool operator () (const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2)
	{
		string strtemp1(m1.TradeDate);
		string strtemp2(m2.TradeDate);
		return strtemp1 > strtemp2;	
	}
};


//持仓明细

class PDComparer_0
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		string strtemp1(m1.InvestorID);
		string strtemp2(m2.InvestorID);

		return strtemp1 < strtemp2;	
	}
};
class PDComparer_1
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		string strtemp1(m1.InvestorID);
		string strtemp2(m2.InvestorID);

		return strtemp1 < strtemp2;			
	}
};
class PDComparer_2
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		string strtemp1(m1.InstrumentID);
		string strtemp2(m2.InstrumentID);
		return strtemp1 < strtemp2;			
	}
};
class PDComparer_3
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.Direction < m2.Direction;	
	}
};
class PDComparer_4
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.HedgeFlag < m2.HedgeFlag;
	}
};
class PDComparer_5
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.Volume < m2.Volume;
	}
};
class PDComparer_6
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.OpenPrice < m2.OpenPrice;
	}
};
class PDComparer_7
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.SettlementPrice < m2.SettlementPrice;	
	}
};

class PDComparer_8
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		string strtemp1(m1.OpenDate);
		string strtemp2(m2.OpenDate);
		return strtemp1 < strtemp2;	
	}
};
class PDComparer_9
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		string strtemp1(m1.TradeID);
		string strtemp2(m2.TradeID);
		CTools_Ansi::mytrimleft(strtemp1);
		CTools_Ansi::mytrimleft(strtemp2);
		return atoi(strtemp1.c_str()) < atoi(strtemp2.c_str());	

	}
};

class PDComparer_10
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.LastSettlementPrice < m2.LastSettlementPrice;
	}
};
class PDComparer_11
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.Margin < m2.Margin;
	}
};
class PDComparer_12
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.CloseVolume < m2.CloseVolume;	
	}
};
class PDComparer_13
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.PositionProfitByDate < m2.PositionProfitByDate;	
	}
};
class PDComparer_14
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.CloseProfitByDate < m2.CloseProfitByDate;	
	}
};

class PDAComparer_0
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		string strtemp1(m1.InvestorID);
		string strtemp2(m2.InvestorID);

		return strtemp1 >strtemp2;			
	}
};

class PDAComparer_1
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		string strtemp1(m1.InvestorID);
		string strtemp2(m2.InvestorID);

		return strtemp1 >strtemp2;		
	}
};
class PDAComparer_2
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		string strtemp1(m1.InstrumentID);
		string strtemp2(m2.InstrumentID);
		return strtemp1 > strtemp2;			
	}
};
class PDAComparer_3
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.Direction > m2.Direction;	
	}
};
class PDAComparer_4
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.HedgeFlag > m2.HedgeFlag;
	}
};
class PDAComparer_5
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.Volume > m2.Volume;
	}
};
class PDAComparer_6
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.OpenPrice > m2.OpenPrice;
	}
};
class PDAComparer_7
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.SettlementPrice > m2.SettlementPrice;	
	}
};

class PDAComparer_8
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		string strtemp1(m1.OpenDate);
		string strtemp2(m2.OpenDate);
		return strtemp1 > strtemp2;	
	}
};
class PDAComparer_9
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		string strtemp1(m1.TradeID);
		string strtemp2(m2.TradeID);
		CTools_Ansi::mytrimleft(strtemp1);
		CTools_Ansi::mytrimleft(strtemp2);
		return atoi(strtemp1.c_str()) > atoi(strtemp2.c_str());	

	}
};

class PDAComparer_10
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.LastSettlementPrice > m2.LastSettlementPrice;
	}
};
class PDAComparer_11
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.Margin > m2.Margin;
	}
};
class PDAComparer_12
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.CloseVolume > m2.CloseVolume;	
	}
};
class PDAComparer_13
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.PositionProfitByDate > m2.PositionProfitByDate;	
	}
};
class PDAComparer_14
{
public:

	bool operator () (const PlatformStru_PositionDetail & m1, const PlatformStru_PositionDetail & m2)
	{
		return m1.CloseProfitByDate > m2.CloseProfitByDate;	
	}
};
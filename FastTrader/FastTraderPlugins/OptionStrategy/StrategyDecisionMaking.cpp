#include "StdAfx.h"
#include "StrategyDecisionMaking.h"
#include "CommPublic.h"

StrategyDecisionMaking::StrategyDecisionMaking(void)
{
}

StrategyDecisionMaking::~StrategyDecisionMaking(void)
{
}

bool StrategyDecisionMaking::GetOptionStrategyOrders( eStrategyID     strategyID,
							 const string      strUnderlyingInstrumentID,
							 vector<PlatformStru_InstrumentInfo>  vecOptionIns,
							 map<string,PlatformStru_DepthMarketData>   maplastQuot,

							 vector<PlatformStru_OrderInfo>&      outOrders)
{

	if(strUnderlyingInstrumentID.empty() || vecOptionIns.size() ==0 || maplastQuot.size() ==0 )
		return false;
	

	PlatformStru_OrderInfo order;
	memset(&order,0,sizeof(order));
	memcpy(order.Account,CDataInfo::NewInstance()->GetAccount().c_str(),sizeof(order.Account));
	order.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	order.CombOffsetFlag[0] = THOST_FTDC_OF_Open;

	switch(strategyID)
	{
	case StrategyID_Rose://. 以卖价买入一手行权价最小的虚值看涨期权合约。
		{			
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			vector<PlatformStru_InstrumentInfo> vecTempIns;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())					 
				{
					if (itIns->StrikePrice > itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);				
					}
				}
				
			}
			GetMiniPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_Crash://. 以卖价买入一手行权价最大的虚值看跌期权合约。
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			vector<PlatformStru_InstrumentInfo> vecTempIns;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			
			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions  && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);		
					}
				}
				
			}
			GetMaxPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_Peakback://.触顶回落  以买价卖出一手行权价最小的虚值看涨期权合约。
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			
			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			
			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice > itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);	
					}
				}
				
			}
			GetMiniPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_Bottomout://. 触底回升。 以买价卖出一手行权价最大的虚值看跌期权合约
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);	

					}
				}
				
			}
			GetMaxPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_Breakout_Straddle://. 突破-买入跨式策略。 1. 以卖价买入一手行权价最大的实值看涨期权合约 同时 以卖价买入一手相同行权价的看跌期权合约。
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			vector<PlatformStru_InstrumentInfo> vecTempIns;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);	

					}
				}
				
			}
			GetMaxPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice == Ins.StrikePrice)
					{				
						Ins=*itIns;
					}
				}
				
			}
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_Breakout_Strangle://. 突破-买入勒式策略。 1 以卖价买进一手行权价较小的虚值看涨期权合约 同时 以卖价买入一手行权价较大的虚值看跌期权合约
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			double dbPrice=0.0;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			dbPrice = itFuture->second.LastPrice;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
				    if (itIns->StrikePrice > itFuture->second.LastPrice)
				    {
					    vecTempIns.push_back(*itIns);					

				    }
				}
			}

			GetMiniPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			vecTempIns.clear();
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{				
						vecTempIns.push_back(*itIns);	
					}
				}
				
			}

			GetMaxPriceIns(vecTempIns,Ins);
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_Consolidation_Straddle://.盘整-卖出跨式策略。 以买价卖出一手行权价较大的实值看涨期权合约 同时 以买价卖出一手相同到期日 相同行权价的看跌期权。
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			vector<PlatformStru_InstrumentInfo> vecTempIns;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);	

					}
				}
				
			}
			GetMaxPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice == Ins.StrikePrice)
					{				
						Ins=*itIns;
					}
				}
				
			}
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_Consolidation_Strangle://. 盘整-卖出勒式策略。 以买价卖出一手行权价较小的虚值看涨期权合约 同时 以买价卖出一手行权价较大的虚值看跌期权。
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			double dbPrice=0.0;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			dbPrice = itFuture->second.LastPrice;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice > itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);					

					}
				}
				
			}

			GetMiniPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			vecTempIns.clear();
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{				
						vecTempIns.push_back(*itIns);	
					}
				}
				
			}

			GetMaxPriceIns(vecTempIns,Ins);
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_ConsolidationUp_Call://.盘涨Call。 以卖价买入一手行权价较大的实值看涨期权合约 同时 以买价卖出一手行权价较小的虚值看涨期权。
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			double dbPrice=0.0;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			dbPrice = itFuture->second.LastPrice;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);					

					}
				}
				
			}

			GetMaxPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			vecTempIns.clear();
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice > itFuture->second.LastPrice)
					{				
						vecTempIns.push_back(*itIns);	
					}
				}
				
			}

			GetMiniPriceIns(vecTempIns,Ins);
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_ConsolidationUp_Put://.盘涨Put。 以卖价买入一手行权价较大的虚值看跌期权合约 同时 以买价卖出一手行权价较小的实值看跌期权
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			double dbPrice=0.0;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			dbPrice = itFuture->second.LastPrice;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice< itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);					

					}
				}
				
			}

			GetMaxPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			vecTempIns.clear();
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice > itFuture->second.LastPrice)
					{				
						vecTempIns.push_back(*itIns);	
					}
				}
				
			}

			GetMiniPriceIns(vecTempIns,Ins);
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_ConsolidationDown_Call://.盘跌Call。 以卖价买入一手行权价较小的虚值看涨期权合约 同时 以买价卖出一手行权价较大的实值看涨期权
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			double dbPrice=0.0;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			dbPrice = itFuture->second.LastPrice;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice > itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);					

					}
				}
				
			}

			GetMiniPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			vecTempIns.clear();
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_CallOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{				
						vecTempIns.push_back(*itIns);	
					}
				}
				
			}

			GetMaxPriceIns(vecTempIns,Ins);
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	case StrategyID_ConsolidationDown_Put://.盘跌Put。 ：以卖价买入一手行权价较小的实值看跌期权合约 同时 以买价卖出一手行权价较大的虚值看跌期权。
		{
			PlatformStru_InstrumentInfo Ins;
			memset(&Ins,0,sizeof(Ins));
			double dbPrice=0.0;

			map<string,PlatformStru_DepthMarketData>::iterator itFuture =   maplastQuot.find(strUnderlyingInstrumentID);
			if(itFuture == maplastQuot.end())
				return false;
			dbPrice = itFuture->second.LastPrice;

			vector<PlatformStru_InstrumentInfo>::iterator itIns = vecOptionIns.begin();
			vector<PlatformStru_InstrumentInfo> vecTempIns;
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())
				{
					if (itIns->StrikePrice > itFuture->second.LastPrice)
					{
						vecTempIns.push_back(*itIns);					

					}
				}
				
			}

			GetMiniPriceIns(vecTempIns,Ins);
			map<string,PlatformStru_DepthMarketData>::iterator itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Buy;
			order.LimitPrice=itOption->second.AskPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

			memset(&order,0,sizeof(order));
			vecTempIns.clear();
			itIns = vecOptionIns.begin();
			for (itIns;itIns != vecOptionIns.end(); itIns++)
			{
				if (itIns->OptionsType == THOST_FTDC_CP_PutOptions && itIns->StrikePrice != util::GetDoubleInvalidValue())					
				{
					if (itIns->StrikePrice < itFuture->second.LastPrice)
					{				
						vecTempIns.push_back(*itIns);	
					}
				}
				
			}

			GetMaxPriceIns(vecTempIns,Ins);
			itOption =   maplastQuot.find(Ins.InstrumentID);
			if(itOption == maplastQuot.end())
				return false;
			strncpy(order.InstrumentID, Ins.InstrumentID,sizeof(order.InstrumentID)-1);
			order.Direction = THOST_FTDC_D_Sell;
			order.LimitPrice=itOption->second.BidPrice1;
			order.VolumeTotalOriginal = 1;
			order.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
			outOrders.push_back(order);

		}
		break;
	default:
		break;
	}
	return true;

}
void StrategyDecisionMaking::GetMiniPriceIns(vector<PlatformStru_InstrumentInfo>& vecTempIns,PlatformStru_InstrumentInfo& outData)
{
    if(vecTempIns.empty())
        return;

	vector<PlatformStru_InstrumentInfo>::iterator it = vecTempIns.begin();
	outData = *it;
	for (++it; it !=vecTempIns.end(); it++)
	{

		if ( it->StrikePrice < outData.StrikePrice )
		{
			outData = *it;
		}
	}

	
}

void StrategyDecisionMaking::GetMaxPriceIns(vector<PlatformStru_InstrumentInfo>& vecTempIns,PlatformStru_InstrumentInfo& outData)
{

	vector<PlatformStru_InstrumentInfo>::iterator it = vecTempIns.begin();
	outData = *it;
	for (++it; it !=vecTempIns.end(); it++)
	{

		if ( it->StrikePrice > outData.StrikePrice )
		{
			outData = *it;
		}
	}


}
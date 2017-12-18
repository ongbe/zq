/* patsIntf.h  v2.8.3 */
#ifndef _WINDOWS_
#include <windows.h>
#endif

#pragma once
#pragma pack(push,PATSAPI,1)

const int PATS_CORE_MAIN	= 283;	// SP_FLEX
const int PATS_CORE_SP		= 6;	// SP_FLEX
const int PATS_CORE_PATCH	= 1;	// SP_FLEX
const int PATS_CORE_MOD     = 0;	// SP_FLEX
//CONSTANTS
#define ptAPIversion "v2.8.3" 

#define ptGateway 'G' // application environment types
#define ptClient 'C' 
#define ptTestClient 'T' 
#define ptTestGateway 'g' 
#define ptDemoClient 'D' 
#define ptBroker 'B' 
#define ptTestBroker 'b' 

// Error codes returned by API
#define ptSuccess 0 
#define ptErrNotInitialised 1 // ptInitialise not run yet
#define ptErrCallbackNotSet 2 // some callback addresses nil
#define ptErrUnknownCallback 3 // unknown callback ID
#define ptErrNotLoggedOn 4 // user has not successfully logged on
#define ptErrInvalidPassword 5 // old pwd incorrect on ptSetPassword
#define ptErrBlankPassword 6 // password may not be blank
#define ptErrNotEnabled 7 // user/accnt not enabled for this action
#define ptErrInvalidIndex 8 // index provided to ptGet<xxx> is invalid
#define ptErrUnknownAccount 9 // trader account not found
#define ptErrNoData 10 // could not find any data to return
#define ptErrFalse 11 // generic value not set/known error
#define ptErrUnknownError 12 // *** NOT USED - Spare return code ***
#define ptErrWrongVersion 13 // mismatch between application and API
#define ptErrBadMsgType 14 // Msg Type not ptAlert or ptNormal
#define ptErrUnknownMsgID 15 // msg ID sequence no. not found
#define ptErrBufferOverflow 16 // not enough room to write report
#define ptErrBadPassword 17 // new password was not recognisable text
#define ptErrNotConnected 18 // not connected to host or price feed
#define ptErrUnknownCurrency 19 // currency not recognised
#define ptErrNoReport 20 // no matching report for report type
#define ptErrUnknownOrderType 21 // order type not known by API
#define ptErrUnknownContract 22 // contractname/date unknown
#define ptErrUnknownCommodity 23 // commodity name not known
#define ptErrPriceRequired 24 // required price to entered for new order
#define ptErrUnknownOrder 25 // specified order ID not valid
#define ptErrInvalidState 26 // order is not in valid state for action
#define ptErrInvalidPrice 27 // supplied price string is invalid
#define ptErrPriceNotRequired 28 // price specified and should not be
#define ptErrInvalidVolume 29 // volume (lots) is not valid
#define ptErrAmendDisabled 30 // amend not enabled for exch. (use cancel/add)
#define ptErrQueryDisabled 31 // ORPI query not enabled for exch.
#define ptErrUnknownExchange 32 // that exchange not known
#define ptErrUnknownFill 33 // fill ID not for valid fill
#define ptErrNotTradable 34 // Trader is View Only
#define ptErrTASUnavailable 35 // Transaction server is not connected
#define ptErrMDSUnavailable 36 // MDS not connected
#define ptErrNotAlphaNumeric 37 // new password was not alpha-numeric
#define ptErrInvalidUnderlying 38 // invalid underlying contract for strategy
#define ptErrUntradableOType 39 // user is not allowed to trade with selected order type
#define ptErrNoPreallocOrders 40 // returned when the user has no preallocated orders remaining
//    and the request for more ids has been rejected.
#define ptErrDifferentMarkets 41 // Crossing Error - the contracts are in different markets
#define ptErrDifferentOrderTypes 42 // Crossing Error - the orders are different types
#define ptOrderAlreadyReceived 43 // user is not allowed to trade with selected order type
#define ptVTSItemInvalid 44 // the user has tried to retrieve an invalid Variable Tick Size
#define ptErrInvalidOrderParent 45 // The user has tried to add an order to an invalid parent order - OMI
#define ptErrNotAggOrder 46 //The user has tried to set an order to DoneForDay that isn't an Aggrgate Order - OMI
#define ptErrOrderAlreadyAmending 47 // The order has already been passed to the Core Components.  It will be held in a queue until a valid state is returned
#define ptErrNotTradableContract 48 //The user does not have permission to access this contract information
#define ptErrFailedDecompress 49 // Unable to decompress contract data
#define ptErrAmendMarketSuspended 50 //TGE Specific error - the user will get an error if the order cannot be amended
#define ptErrGTOrderCancelled 51 //GT Message used to identify orders that have been cancelled due to the exchange closing.
#define ptErrInvalidAmendOrderType 52 //This Order cannot be amended to this Order Type

#define ptErrInvalidAlgoXML 53 // invalid algo string


#define ptErrInvalidIPAddress 54 

#define ptErrLast 54 // Keep this in sync with last error number
// Should be same as highest error number (except 99)

#define ptErrUnexpected 99 // unexpected error trapped - routine aborted

// Constants for callback types.
#define ptHostLinkStateChange 1 
#define ptPriceLinkStateChange 2 
#define ptLogonStatus 3 
#define ptMessage 4 
#define ptOrder 5 
#define ptForcedLogout 6 
#define ptDataDLComplete 7 
#define ptPriceUpdate 8 
#define ptFill 9 
#define ptStatusChange 10 
#define ptContractAdded 11 
#define ptContractDeleted 12 
#define ptExchangeRate 13 
#define ptConnectivityStatus 14 
#define ptOrderCancelFailure 15 
#define ptAtBestUpdate 16 
#define ptTickerUpdate 17 
#define ptMemoryWarning 18 
#define ptSubscriberDepthUpdate 19 
#define ptVTSCallback 20 
#define ptDOMUpdate 21 
#define ptSettlementCallback 22 
#define ptStrategyCreateSuccess 23 
#define ptStrategyCreateFailure 24 
#define ptAmendFailureCallback 25 
// Eurodollar changes
#define ptGenericPriceUpdate 26 
#define ptBlankPrice 27 
// Jan release callbacks
#define ptOrderSentFailure 28 
#define ptOrderQueuedFailure 29 
#define ptOrderBookReset 30 
// Global Trading Changes
#define ptExchangeUpdate 31 
#define ptCommodityUpdate 32 
#define ptContractDateUpdate 33 

#define ptPurgeCompleted 36 
#define ptTraderAdded 37 
#define ptOrderTypeUpdate 38 


// Constants to describe socket link states
#define ptLinkOpened 1 
#define ptLinkConnecting 2 
#define ptLinkConnected 3 
#define ptLinkClosed 4 
#define ptLinkInvalid 5 

// User message types
#define ptAlert 1 
#define ptNormal 2 

//Group types inside messages
#define ptFillGroup 0 
#define ptLegsGroup 1 
#define ptOrderGroup 2 

//Logon States
#define ptLogonFailed 0 
#define ptLogonSucceeded 1 
#define ptForcedOut 2 
#define ptObsoleteVers 3 
#define ptWrongEnv 4 
#define ptDatabaseErr 5 
#define ptInvalidUser 6 
#define ptLogonRejected 7 
#define ptInvalidAppl 8 
#define ptLoggedOn 9 
#define ptInvalidLogonState 99 

//Fill Types
#define ptNormalFill 1 
#define ptExternalFill 2 
#define ptNettedFill 3 
#define ptRetainedFill 5 
#define ptBlockLegFill 52 

// Pats Order Type Ids
#define ptOrderTypeMarket 1 
#define ptOrderTypeLimit 2 
#define ptOrderTypeLimitFAK 3 
#define ptOrderTypeLimitFOK 4 
#define ptOrderTypeStop 5 
#define ptOrderTypeSynthStop 6 
#define ptOrderTypeSynthStopLimit 7 
#define ptOrderTypeMIT 8 
#define ptOrderTypeSynthMIT 9 
#define ptOrderTypeMarketFOK 10 
#define ptOrderTypeMOO 11 
#define ptOrderTypeIOC 12 
#define ptOrderTypeStopRise 13 
#define ptOrderTypeStopFall 14 
#define ptOrderTypeRFQ 15 
#define ptOrderTypeStopLoss 16 
#define ptLimitAtOpen 17 
#define ptMLM 18 
#define ptAggregateOrder 25 
#define ptCustomerRequest 26 
#define ptRFQi 27 
#define ptRFQt 28 
#define ptCrossingBatchType 42 
#define ptBasisBatchType 43 
#define ptBlockBatchType 44 
#define ptAABatchType 45 
#define ptCrossFaKBatchType 46 
#define ptGTCMarket 50 
#define ptGTCLimit 51 
#define ptGTCStop 52 
#define ptGTDMarket 53 
#define ptGTDLimit 54 
#define ptGTDStop 55 
#define ptSETSRepenter 90 
#define ptSETSRepcancel 91 
#define ptSETSRepprerel 92 
#define ptSETSSectDel 93 
#define ptSETSInstDel 94 
#define ptSETSCurDel 95 
#define ptIceberg 130 
#define ptGhost 131 
#define ptProtected 132 
#define ptStop 133 

//Internal BatchIDs
#define ptBatchID "10000" 

//Order States
#define ptQueued 1 
#define ptSent 2 
#define ptWorking 3 
#define ptRejected 4 
#define ptCancelled 5 
#define ptBalCancelled 6 
#define ptPartFilled 7 
#define ptFilled 8 
#define ptCancelPending 9 
#define ptAmendPending 10 
#define ptUnconfirmedFilled 11 
#define ptUnconfirmedPartFilled 12 
#define ptHeldOrder 13 
#define ptCancelHeldOrder 14 
#define ptTransferred 20 
#define ptExternalCancelled 24 // added for GT

//Order Sub States
#define ptSubStatePending 1 
#define ptSubStateTriggered 2 

// Price Movement
#define ptPriceSame 0 
#define ptPriceRise 1 
#define ptPriceFall 2 


//GENERIC PRICES
#define ptBuySide 1 //	The RFQ is a buy order
#define ptSellSide 2 //	The RFQ is a sell order
#define ptBothSide 3 //	The RFQ is a for both sides
#define PtCrossSide 4 //	This is for crossing RFQs

//Fill Sub Types
#define ptFillSubTypeSettlement 1 
#define ptFillSubTypeMinute 2 
#define ptFillSubTypeUnderlying 3 
#define ptFillSubTypeReverse 4 

//Settlement Price Types
#define ptStlLegacyPrice 0 
#define ptStlCurPrice 7 
#define ptStlLimitUp 21 
#define ptStlLimitDown 22 
#define ptStlExecDiff 23
#define ptStlYDSPPrice 24
#define ptStlNewPrice 25
#define ptStlRFQiPrice 26
#define ptStlRFQtPrice 27 
#define ptStlIndicative 28 
#define ptEFPVolume 33 
#define ptEFSVolume 34 
#define ptBlockVolume 35 
#define ptEFPCummVolume 36 
#define ptEFSCummVolume 37 
#define ptBlockCummVolume 38 

// Price Changes
#define ptChangeBid 0x000001 
#define ptChangeOffer 0x000002 
#define ptChangeImpliedBid 0x000004 
#define ptChangeImpliedOffer 0x000008 
#define ptChangeRFQ 0x000010 
#define ptChangeLast 0x000020 
#define ptChangeTotal 0x000040 
#define ptChangeHigh 0x000080 
#define ptChangeLow 0x000100 
#define ptChangeOpening 0x000200 
#define ptChangeClosing 0x000400 
#define ptChangeBidDOM 0x000800 
#define ptChangeOfferDOM 0x001000 
#define ptChangeTGE 0x002000 
#define ptChangeSettlement 0x004000 
#define ptChangeIndic 0x008000 

// Mask for Cleared Prices
#define ptChangeClear 0x00181F // Bid, Offer, Implied Bid, Implied Offer,
// RFQ, Bid DOM, Offer DOM
// Contract Date Market Status
#define ptStateUndeclared -$0001 
#define ptStateNormal 0x00 
#define ptStateExDiv 0x01 
#define ptStateAuction 0x02 
#define ptStateSuspended 0x04 
#define ptStateClosed 0x08 
#define ptStatePreOpen 0x10 
#define ptStatePreClose 0x20 
#define ptStateFastMarket 0x40 

// Preallocated States
#define ptIDsNull -1 
#define ptIDsReceived 0 
#define ptIDsRejected 1 
#define ptIDsRequested 2 

// Global Trading Alpha 16 June release AMC
#define ptGTUndefined 0 
#define ptGTActive 1 
#define ptGTInactive 2 
#define ptGTExpired 3 

#define ptGTStartOfDay 0 
#define ptGTEndOfDay 1 

// Strategy creation codes
#define ptFUT_CALENDAR 'E' 
#define ptFUT_BUTTERFLY 'B' 
#define ptFUT_CONDOR 'W' 
#define ptFUT_STRIP 'M' 
#define ptFUT_PACK 'O' 
#define ptFUT_BUNDLE 'Y' 
#define ptFUT_RTS 'Z' 
#define ptOPT_BUTTERFLY 'B' 
#define ptOPT_SPREAD 'D' 
#define ptOPT_CALENDAR_SPREAD 'E' 
#define ptOPT_DIAG_CALENDAR_SPREAD 'F' 
#define ptOPT_GUTS 'G' 
#define ptOPT_RATIO_SPREAD 'H' 
#define ptOPT_IRON_BUTTERFLY 'I' 
#define ptOPT_COMBO 'J' 
#define ptOPT_STRANGLE 'K' 
#define ptOPT_LADDER 'L' 
#define ptOPT_STRADDLE_CALENDAR_SPREAD 'N' 
#define ptOPT_DIAG_STRADDLE_CALENDAR_SPREAD 'P' 
#define ptOPT_STRADDLE 'S' 
#define ptOPT_CONDOR 'W' 
#define ptOPT_BOX 'X' 
#define ptOPT_SYNTHETIC_CONVERSION_REVERSAL 'r' 
#define ptOPT_CALL_SPREAD_VS_PUT 'x' 
#define ptOPT_PUT_SPREAD_VS_CALL 'y' 
#define ptOPT_STRADDLE_VS_OPTION 'z' 
#define ptVOL_REVERSAL_CONVERSION 'R' 
#define ptVOL_OPTION 'V' 
#define ptVOL_LADDER 'a' 
#define ptVOL_CALL_SPREAD_VS_PUT 'c' 
#define ptVOL_SPREAD 'd' 
#define ptVOL_COMBO 'j' 
#define ptVOL_PUT_SPREAD_VS_CALL 'p' 
#define ptVOL_STRADDLE 's' 
#define ptDIV_C_CALENDAR 'I' 
#define ptDIV_C_SPREAD 'H' 
#define ptDIV_CONVERSION 'G' 
#define ptDIV_F_SPREAD 'E' 
#define ptDIV_P_CALENDAR 'A' 
#define ptDIV_P_SPREAD 'B' 
#define ptDIV_STRADDLE 'D' 
#define ptDIV_STRANGLE 'J' 

//TYPEDEFS

// General type definitions
typedef CHAR Array2[3];
typedef CHAR Array3[4];
typedef CHAR Array5[6];
typedef CHAR Array6[7];
typedef CHAR Array8[9];
typedef CHAR Array9[10];
typedef CHAR Array10[11];
typedef CHAR Array14[15];
typedef CHAR Array15[16];
typedef CHAR Array16[17];
typedef CHAR Array20[21];
typedef CHAR Array25[26];
typedef CHAR Array30[31];
typedef CHAR Array32[33];
typedef CHAR Array36[37];

typedef CHAR Array50[51];
typedef CHAR Array60[61];
typedef CHAR Array70[71];
typedef CHAR Array120[121];
typedef CHAR Array250[251];
typedef CHAR Array255[256];
typedef CHAR Array500[501];
typedef CHAR ExchNameStr[11];
typedef CHAR ExchRateStr[21];
typedef CHAR ConNameStr[11];
typedef CHAR ConDateStr[51];
typedef CHAR TraderStr[21];
typedef CHAR FloatStr[21];
typedef CHAR PriceStr[21];
typedef CHAR TextType[61];
typedef CHAR LongTextType[501];
typedef CHAR DateStr[9];
typedef CHAR TimeStr[7];
typedef Array10 LegType[5];
typedef CHAR FillIDStr[71];
typedef CHAR CurrencyStr[11];
typedef CHAR MDSTokenStr[11];
typedef CHAR RepTypeStr[21];
typedef CHAR CurrNameStr[11];
typedef CHAR CertNameStr[51];
typedef CHAR ReportTypeStr[21];
typedef CHAR MsgIDStr[11];
typedef CHAR DebugStr[251];
typedef CHAR OrderIDStr[11];
typedef CHAR ExchIDStr[31];
typedef CHAR OrderTypeStr[11];
typedef CHAR UserNameStr[11];
typedef CHAR DeviceLabelStr[37];
typedef CHAR DeviceTypeStr[4];
typedef CHAR StatusStr[4];
typedef CHAR SeverityStr[4];
typedef CHAR DeviceNameStr[21];
typedef CHAR CommentaryStr[256];
typedef CHAR ExchangeIdStr[21];
typedef CHAR OwnerStr[21];
typedef CHAR TimeStampStr[15];
typedef CHAR SystemIDStr[11];
typedef CHAR GTDStr[9];
typedef Array10 LegStruct[5];
typedef WCHAR WideArray500[500];
typedef CHAR AmendTypesArray[501];
typedef CHAR* AlgoBuff;

//DATATYPES
// Common PATS basic data types
typedef AmendTypesArray FAR *AmendTypesArrayptr;
typedef CertNameStr FAR *CertNamePtr;
typedef PINT IntegerPtr;
typedef ConNameStr FAR *ConNamePtr;
typedef ConDateStr FAR *ConDatePtr;
typedef CurrNameStr FAR *CurrNamePtr;
typedef ExchNameStr FAR *ExchNamePtr;
typedef ExchRateStr FAR *ExchRatePtr;
typedef MsgIDStr FAR *MsgIDPtr;
typedef OrderTypeStr FAR *OrderTypePtr;
typedef OrderIDStr FAR *OrderIDPtr;
typedef FloatStr FAR *FloatPtr;
typedef PriceStr FAR *PricePtr;
typedef ReportTypeStr FAR *ReportTypePtr;
typedef TraderStr FAR *TraderPtr;
typedef FillIDStr FAR *FillIDPtr;
typedef UserNameStr FAR *UserNameStrPtr;
typedef DebugStr FAR *DebugStrPtr;
typedef MDSTokenStr FAR *MDSTokenPtr;
typedef AlgoBuff FAR *AlgoBuffPtr;
//OBJECTS
// Structures for reference data calls
typedef struct {
              Array25 Version;
} APIBuildVer, FAR *APIBuildVerPtr;

typedef struct {
              Array10 Name;
              CHAR QueryEnabled;
              CHAR AmendEnabled;
              INT Strategy;
              CHAR CustomDecs;
              INT Decimals;
              CHAR TicketType;
              CHAR RFQA;
              CHAR RFQT;
              CHAR EnableBlock;
              CHAR EnableBasis;
              CHAR EnableAA;
              CHAR EnableCross;
              INT GTStatus;
} ExchangeStruct, FAR *ExchangeStructPtr;

typedef struct {
              Array20 TraderAccount;
              Array20 BackOfficeID;
              CHAR Tradable;
              INT LossLimit;
} TraderAcctStruct, FAR *TraderAcctStructPtr;

typedef struct {
              Array10 OrderType;
              Array10 ExchangeName;
              INT OrderTypeID;
              BYTE NumPricesReqd;
              BYTE NumVolumesReqd;
              BYTE NumDatesReqd;
              CHAR AutoCreated;
              CHAR TimeTriggered;
              CHAR RealSynthetic;
              CHAR GTCFlag;
              Array2 TicketType;
              CHAR PatsOrderType;
              INT AmendOTCount;
              Array50 AlgoXML;
} OrderTypeStruct, FAR *OrderTypeStructPtr;

typedef struct {
              Array10 ExchangeName;
              Array10 ContractName;
              Array10 Currency;
              Array10 Group;
              Array10 OnePoint;
              INT TicksPerPoint;
              Array10 TickSize;
              INT GTStatus;
} CommodityStruct, FAR *CommodityStructPtr;

//     contract type (F,C,P)
//     commodity
//     maturity date
//     strike price
//     whatever, eg volume ratio
typedef struct {
              Array10 ContractName;
              Array50 ContractDate;
              Array10 ExchangeName;
              Array8 ExpiryDate;
              Array8 LastTradeDate;
              INT NumberOfLegs;
              INT TicksPerPoint;
              Array10 TickSize;
              CHAR Tradable;
              INT GTStatus;
              Array20 Margin;
              CHAR ESATemplate;
              Array16 MarketRef;
              Array10 lnExchangeName;
              Array10 lnContractName;
              Array50 lnContractDate;
              LegStruct ExternalID[32];
} ContractStruct, FAR *ContractStructPtr;

typedef struct {
              Array10 ContractName;
              Array50 ContractDate;
              Array10 ExchangeName;
              Array8 ExpiryDate;
              Array8 LastTradeDate;
              INT NumberOfLegs;
              INT TicksPerPoint;
              Array10 TickSize;
              BYTE Tradable;
              INT GTStatus;
              Array20 Margin;
              CHAR ESATemplate;
              Array16 MarketRef;
              Array10 lnExchangeName;
              Array10 lnContractName;
              Array50 lnContractDate;
              LegStruct ExternalID[16];
} ExtendedContractStruct, FAR *ExtendedContractStructPtr;

// Structures for user-level requests
typedef struct {
              BYTE Status;
              Array60 Reason;
              Array20 DefaultTraderAccount;
              CHAR ShowReason;
              CHAR DOMEnabled;
              CHAR PostTradeAmend;
              Array255 UserName;
              CHAR GTEnabled;
} LogonStatusStruct, FAR *LogonStatusStructPtr;

typedef struct {
              Array255 UserID;
              Array255 Password;
              Array255 NewPassword;
              CHAR Reset;
              CHAR Reports;
} LogonStruct, FAR *LogonStructPtr;

typedef struct {
              Array10 MsgID;
              Array500 MsgText;
              CHAR IsAlert;
              CHAR Status;
} MessageStruct, FAR *MessageStructPtr;

// Structures for issuing or registering callbacks
typedef struct {
              BYTE OldState;
              BYTE NewState;
} LinkStateStruct, FAR *LinkStateStructPtr;

typedef struct {
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
} ContractUpdStruct, FAR *ContractUpdStructPtr;

typedef struct {
              Array10 ExchangeName;
              Array10 ContractName;
} CommodityUpdStruct, FAR *CommodityUpdStructPtr;

typedef struct {
              Array10 ExchangeName;
} ExchangeUpdStruct, FAR *ExchangeUpdStructPtr;

typedef struct {
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
} DOMUpdStruct, FAR *DOMUpdStructPtr;

typedef struct {
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
} PriceUpdStruct, FAR *PriceUpdStructPtr;

typedef struct {
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
} AtBestUpdStruct, FAR *AtBestUpdStructPtr;

typedef struct {
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
} SubscriberDepthUpdStruct, FAR *SubscriberDepthUpdStructPtr;

typedef struct {
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
              INT Status;
} StatusUpdStruct, FAR *StatusUpdStructPtr;

typedef struct {
              Array10 OrderID;
              Array70 FillID;
} FillUpdStruct, FAR *FillUpdStructPtr;

typedef struct {
              Array10 OrderID;
              Array10 OldOrderID;
              BYTE OrderStatus;
              INT OFSeqNumber;
              INT OrderTypeId;
} OrderUpdStruct, FAR *OrderUpdStructPtr;

typedef struct {
              Array20 Exchange;
              Array20 Commodity;
              INT Count;
} VTSUpdStruct, FAR *VTSUpdStructPtr;

typedef struct {
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
              INT SettlementType;
              Array20 Price;
              Array6 Time;
              Array8 Date;
} SettlementPriceStruct, FAR *SettlementPriceStructPtr;

typedef struct {
              Array10 UserName;
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ReqContractDate;
              Array50 GenContractDate;
} StrategyCreateSuccessStruct, FAR *StrategyCreateSuccessStructPtr;

typedef struct {
              Array10 UserName;
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
              Array60 Text;
} StrategyCreateFailureStruct, FAR *StrategyCreateFailureStructPtr;

typedef struct {
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
} BlankPriceStruct, FAR *BlankPriceStructPtr;

typedef struct {
              Array10 Currency;
} ExchangeRateUpdStruct, FAR *ExchangeRateUpdStructPtr;

typedef struct {
              Array36 DeviceLabel;
              Array3 DeviceType;
              Array3 Status;
              Array3 Severity;
              Array20 DeviceName;
              Array255 Commentary;
              Array20 ExchangeID;
              Array20 Owner;
              Array14 TimeStamp;
              Array10 SystemID;
} ConnectivityStatusUpdStruct, FAR *ConnectivityStatusUpdStructPtr;

typedef struct {
              Array3 Firm;
              INT Volume;
              CHAR BestType;
} AtBestStruct, FAR *AtBestStructPtr;

typedef struct {
              Array20 BidPrice;
              Array20 OfferPrice;
              Array3 LastBuyer;
              Array3 LastSeller;
} AtBestPriceStruct, FAR *AtBestPriceStructPtr;

typedef struct {
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
              Array20 BidPrice;
              INT BidVolume;
              Array20 OfferPrice;
              INT OfferVolume;
              Array20 LastPrice;
              INT LastVolume;
              CHAR Bid;
              CHAR Offer;
              CHAR Last;
} TickerUpdStruct, FAR *TickerUpdStructPtr;

typedef struct {
              Array20 Price;
              INT Volume;
              Array3 Firm;
              CHAR DepthType;
} SubscriberDepthStruct, FAR *SubscriberDepthStructPtr;

// Structures for trading routines
typedef struct {
              INT Index;
              FillIDStr FillId;
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
              CHAR BuyOrSell;
              INT Lots;
              Array20 Price;
              Array10 OrderID;
              Array8 DateFilled;
              Array6 TimeFilled;
              Array8 DateHostRecd;
              Array6 TimeHostRecd;
              Array30 ExchOrderId;
              BYTE FillType;
              Array20 TraderAccount;
              Array10 UserName;
              Array70 ExchangeFillID;
              Array20 ExchangeRawPrice;
              Array70 ExecutionID;
              INT GTStatus;
              INT SubType;
			  Array20 CounterParty;
			  Array2 Leg;
} FillStruct, FAR *FillStructPtr;

typedef struct {
              Array20 Profit;
              INT Buys;
              INT Sells;
} PositionStruct, FAR *PositionStructPtr;

typedef struct {
              Array20 Price;
              INT Volume;
              BYTE AgeCounter;
              BYTE Direction;
              BYTE Hour;
              BYTE Minute;
              BYTE Second;
} PriceDetailStruct, FAR *PriceDetailStructPtr;

// Returned price details. Cast this as aa array of the following type
//   "Packed Array [1..<n>] of PriceDetailStruct"
// if you wish to use a loop to decode price strings to real numbers.
typedef struct {
              PriceDetailStruct Bid;
              PriceDetailStruct Offer;
              PriceDetailStruct ImpliedBid;
              PriceDetailStruct ImpliedOffer;
              PriceDetailStruct RFQ;
              PriceDetailStruct Last0;
              PriceDetailStruct Last1;
              PriceDetailStruct Last2;
              PriceDetailStruct Last3;
              PriceDetailStruct Last4;
              PriceDetailStruct Last5;
              PriceDetailStruct Last6;
              PriceDetailStruct Last7;
              PriceDetailStruct Last8;
              PriceDetailStruct Last9;
              PriceDetailStruct Last10;
              PriceDetailStruct Last11;
              PriceDetailStruct Last12;
              PriceDetailStruct Last13;
              PriceDetailStruct Last14;
              PriceDetailStruct Last15;
              PriceDetailStruct Last16;
              PriceDetailStruct Last17;
              PriceDetailStruct Last18;
              PriceDetailStruct Last19;
              PriceDetailStruct Total;
              PriceDetailStruct High;
              PriceDetailStruct Low;
              PriceDetailStruct Opening;
              PriceDetailStruct Closing;
              PriceDetailStruct BidDOM0;
              PriceDetailStruct BidDOM1;
              PriceDetailStruct BidDOM2;
              PriceDetailStruct BidDOM3;
              PriceDetailStruct BidDOM4;
              PriceDetailStruct BidDOM5;
              PriceDetailStruct BidDOM6;
              PriceDetailStruct BidDOM7;
              PriceDetailStruct BidDOM8;
              PriceDetailStruct BidDOM9;
              PriceDetailStruct BidDOM10;
              PriceDetailStruct BidDOM11;
              PriceDetailStruct BidDOM12;
              PriceDetailStruct BidDOM13;
              PriceDetailStruct BidDOM14;
              PriceDetailStruct BidDOM15;
              PriceDetailStruct BidDOM16;
              PriceDetailStruct BidDOM17;
              PriceDetailStruct BidDOM18;
              PriceDetailStruct BidDOM19;
              PriceDetailStruct OfferDOM0;
              PriceDetailStruct OfferDOM1;
              PriceDetailStruct OfferDOM2;
              PriceDetailStruct OfferDOM3;
              PriceDetailStruct OfferDOM4;
              PriceDetailStruct OfferDOM5;
              PriceDetailStruct OfferDOM6;
              PriceDetailStruct OfferDOM7;
              PriceDetailStruct OfferDOM8;
              PriceDetailStruct OfferDOM9;
              PriceDetailStruct OfferDOM10;
              PriceDetailStruct OfferDOM11;
              PriceDetailStruct OfferDOM12;
              PriceDetailStruct OfferDOM13;
              PriceDetailStruct OfferDOM14;
              PriceDetailStruct OfferDOM15;
              PriceDetailStruct OfferDOM16;
              PriceDetailStruct OfferDOM17;
              PriceDetailStruct OfferDOM18;
              PriceDetailStruct OfferDOM19;
              PriceDetailStruct LimitUp;
              PriceDetailStruct LimitDown;
              PriceDetailStruct ExecutionUp;
              PriceDetailStruct ExecutionDown;
              PriceDetailStruct ReferencePrice;
              PriceDetailStruct pvCurrStl;
              PriceDetailStruct pvSODStl;
              PriceDetailStruct pvNewStl;
              PriceDetailStruct pvIndBid;
              PriceDetailStruct pvIndOffer;
              INT Status;
              INT ChangeMask;
              INT PriceStatus;
} PriceStruct, FAR *PriceStructPtr;

typedef struct {
              Array20 TraderAccount;
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
              CHAR BuyOrSell;
              Array20 AveragePrice;
              Array25 Reference;
              CHAR DoneForDay;
              INT Xref;
} NewAggOrderStruct, FAR *NewAggOrderStructPtr;

typedef struct {
              Array20 TraderAccount;
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
              CHAR BuyOrSell;
              INT ActualAmount;
              Array10 OrderType;
              Array20 Price;
              Array20 Price2;
              Array10 ParentID;
              INT TotalVolume;
              INT CumulativeVol;
              Array20 AveragePrice;
              Array25 Reference;
              INT Xref;
} NewCustReqStruct, FAR *NewCustReqStructPtr;

typedef struct {
              Array20 TraderAccount;
              Array10 OrderType;
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
              CHAR BuyOrSell;
              Array20 Price;
              Array20 Price2;
              INT Lots;
              Array10 LinkedOrder;
              CHAR OpenOrClose;
              INT Xref;
              INT XrefP;
              Array8 GoodTillDate;
              CHAR TriggerNow;
              Array25 Reference;
              Array50 ESARef;
              INT Priority;
              Array8 TriggerDate;
              Array6 TriggerTime;
              Array10 BatchID;
              Array10 BatchType;
              INT BatchCount;
              Array10 BatchStatus;
              Array10 ParentID;
              CHAR DoneForDay;
              Array255 BigRefField;
              Array32 SenderLocationID;
              Array20 Rawprice;
              Array20 Rawprice2;
              Array70 ExecutionID;
              Array20 ClientID;
              CHAR APIM;
              Array20 APIMUser;
              Array10 YDSPAudit;
              Array10 ICSNearLegPrice;
              Array10 ICSFarLegPrice;
              INT MinClipSize;
              INT MaxClipSize;
              CHAR Randomise;
              Array2 TicketType;
              Array3 TicketVersion;
              Array10 ExchangeField;
              Array20 BOFID;
              Array5 Badge;
              Array10 LocalUserName;
              Array20 LocalTrader;
              Array20 LocalBOF;
              Array10 LocalOrderID;
              Array10 LocalExAcct;
              Array10 RoutingID1;
              Array10 RoutingID2;
              CHAR Inactive;
} NewOrderStruct, FAR *NewOrderStructPtr;

typedef struct {
              Array20 Pr1_Price;
              INT Pr1_Volume;
              Array20 Pr2_Price;
              INT Pr2_Volume;
              Array20 Pr3_Price;
              INT Pr3_Volume;
              Array10 St_Type;
              Array20 St_Price;
              Array20 St_Step_1;
              Array20 St_Step_2;
} ProtectionStruc, FAR *ProtectionStrucPtr;

typedef struct {
              NewOrderStruct NewOrders[10];
} BEPNewOrderStruct, FAR *BEPNewOrderStructPtr;

typedef struct {
              OrderIDStr OrderIDs[10];
} BEPOrderIDStruct, FAR *BEPOrderIDStructPtr;

typedef struct {
              Array20 Price;
              Array20 Price2;
              INT Lots;
              Array10 LinkedOrder;
              CHAR OpenOrClose;
              Array20 Trader;
              Array25 Reference;
              INT Priority;
              Array8 TriggerDate;
              Array6 TriggerTime;
              Array10 BatchID;
              Array10 BatchType;
              INT BatchCount;
              Array10 BatchStatus;
              Array10 ParentID;
              CHAR DoneForDay;
              Array255 BigRefField;
              Array32 SenderLocationID;
              Array20 Rawprice;
              Array20 Rawprice2;
              Array70 ExecutionID;
              Array20 ClientID;
              Array50 ESARef;
              Array10 YDSPAudit;
              Array10 ICSNearLegPrice;
              Array10 ICSFarLegPrice;
              INT MaxClipSize;
              Array10 LocalUserName;
              Array20 LocalTrader;
              Array20 LocalBOF;
              Array10 LocalOrderID;
              Array10 LocalExAcct;
              Array10 RoutingID1;
              Array10 RoutingID2;
              Array10 AmendOrderType;
              Array10 TargetUserName;
} AmendOrderStruct, FAR *AmendOrderStructPtr;

typedef enum {
utInvalid = 0,
utAmend = 1,
utCancel = 2,
utDeactivate = 3
} TUpdateType ;

typedef struct {
              Array10 OrderID;
              TUpdateType UpdateType;
              AmendOrderStruct AmendDetails;
              INT CancelTimeOut;
} AmendingOrderStruct, FAR *AmendingOrderStructPtr;

typedef struct {
              Array10 PrimaryOrder;
              Array10 SecondaryOrder;
} CrossingOrderIDs, FAR *CrossingOrderIDsPtr;

typedef struct {
              INT Index;
              CHAR Historic;
              CHAR Checked;
              Array10 OrderID;
              Array10 DisplayID;
              Array30 ExchOrderID;
              Array10 UserName;
              Array20 TraderAccount;
              Array10 OrderType;
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
              CHAR BuyOrSell;
              Array20 Price;
              Array20 Price2;
              INT Lots;
              Array10 LinkedOrder;
              INT AmountFilled;
              INT NoOfFills;
              Array20 AveragePrice;
              BYTE Status;
              CHAR OpenOrClose;
              Array8 DateSent;
              Array6 TimeSent;
              Array8 DateHostRecd;
              Array6 TimeHostRecd;
              Array8 DateExchRecd;
              Array6 TimeExchRecd;
              Array8 DateExchAckn;
              Array6 TimeExchAckn;
              Array60 NonExecReason;
              INT Xref;
              INT XrefP;
              INT UpdateSeq;
              Array8 GoodTillDate;
              Array25 Reference;
              INT Priority;
              Array8 TriggerDate;
              Array6 TriggerTime;
              INT SubState;
              Array10 BatchID;
              Array10 BatchType;
              INT BatchCount;
              Array10 BatchStatus;
              Array10 ParentID;
              CHAR DoneForDay;
              Array255 BigRefField;
              INT Timeout;
              Array120 QuoteID;
              INT LotsPosted;
              INT ChildCount;
              INT ActLots;
              Array32 SenderLocationID;
              Array20 Rawprice;
              Array20 Rawprice2;
              Array70 ExecutionID;
              Array20 ClientID;
              Array50 ESARef;
              Array20 ISINCode;
              Array20 CashPrice;
              CHAR Methodology;
              Array20 BasisRef;
              Array8 EntryDate;
              Array6 EntryTime;
              CHAR APIM;
              Array20 APIMUser;
              Array10 ICSNearLegPrice;
              Array10 ICSFarLegPrice;
              Array8 CreationDate;
              INT OrderHistorySeq;
              INT MinClipSize;
              INT MaxClipSize;
              CHAR Randomise;
              CHAR ProfitLevel;
              INT OFSeqNumber;
              Array10 ExchangeField;
              Array20 BOFID;
              Array5 Badge;
              INT GTStatus;
              Array10 LocalUserName;
              Array20 LocalTrader;
              Array20 LocalBOF;
              Array10 LocalOrderID;
              Array10 LocalExAcct;
              Array10 RoutingID1;
              Array10 RoutingID2;
              Array20 FreeTextField1;
              Array20 FreeTextField2;
              CHAR Inactive;
} OrderDetailStruct, FAR *OrderDetailStructPtr;

typedef struct {
              Array10 Exchange;
              Array10 ContractName;
              Array20 LowerLim;
              Array20 UpperLim;
              INT Multiplier;
} VTSDetailStruct, FAR *VTSDetailStructPtr;

typedef struct {
              CHAR ContractType;
              Array50 ContractDate;
              Array10 Price;
              INT Ratio;
              Array10 ContractName;
} StratLegStruct, FAR *StratLegStructPtr;

typedef struct {
              StratLegStruct Leg0;
              StratLegStruct Leg1;
              StratLegStruct Leg2;
              StratLegStruct Leg3;
              StratLegStruct Leg4;
              StratLegStruct Leg5;
              StratLegStruct Leg6;
              StratLegStruct Leg7;
              StratLegStruct Leg8;
              StratLegStruct Leg9;
              StratLegStruct Leg10;
              StratLegStruct Leg11;
              StratLegStruct Leg12;
              StratLegStruct Leg13;
              StratLegStruct Leg14;
              StratLegStruct Leg15;
} StrategyLegsStruct, FAR *StrategyLegsStructPtr;

typedef struct {
              Array20 Leg0Price;
              Array20 Leg1Price;
              Array20 Leg2Price;
              Array20 Leg3Price;
              Array20 Leg4Price;
              Array20 Leg5Price;
              Array20 Leg6Price;
              Array20 Leg7Price;
              Array20 Leg8Price;
              Array20 Leg9Price;
              Array20 Leg10Price;
              Array20 Leg11Price;
              Array20 Leg12Price;
              Array20 Leg13Price;
              Array20 Leg14Price;
              Array20 Leg15Price;
} LegPriceStruct, FAR *LegPriceStructPtr;

typedef struct {
              Array20 ISINCode;
              Array20 CashPrice;
              CHAR Methodology;
              Array20 Reference;
} BasisOrderStruct, FAR *BasisOrderStructPtr;

//Generic price structure added as part of CME Eurodollar changes
typedef struct {
              Array10 ExchangeName;
              Array10 ContractName;
              Array50 ContractDate;
              INT PriceType;
              CHAR BuyOrSell;
} GenericPriceStruct, FAR *GenericPriceStructPtr;


typedef VOID (WINAPI FAR * ProcAddr)(VOID);
typedef VOID (WINAPI FAR * LinkProcAddr)( LinkStateStructPtr);
typedef VOID (WINAPI FAR * MsgProcAddr)( MsgIDPtr);
typedef VOID (WINAPI FAR * PriceProcAddr)( PriceUpdStructPtr);
typedef VOID (WINAPI FAR * DOMProcAddr)( DOMUpdStructPtr);
typedef VOID (WINAPI FAR * AtBestProcAddr)( AtBestUpdStructPtr);
typedef VOID (WINAPI FAR * SubscriberDepthProcAddr)( SubscriberDepthUpdStructPtr);
typedef VOID (WINAPI FAR * OrderProcAddr)( OrderUpdStructPtr);
typedef VOID (WINAPI FAR * FillProcAddr)( FillUpdStructPtr);
typedef VOID (WINAPI FAR * ContractProcAddr)( ContractUpdStructPtr);
typedef VOID (WINAPI FAR * CommodityProcAddr)( CommodityUpdStructPtr);
typedef VOID (WINAPI FAR * ExchangeProcAddr)( ExchangeUpdStructPtr);
typedef VOID (WINAPI FAR * StatusProcAddr)( StatusUpdStructPtr);
typedef VOID (WINAPI FAR * ExchangeRateProcAddr)( ExchangeRateUpdStructPtr);
typedef VOID (WINAPI FAR * ConStatusProcAddr)( ConnectivityStatusUpdStructPtr);
typedef VOID (WINAPI FAR * TickerUpdateProcAddr)( TickerUpdStructPtr);
typedef VOID (WINAPI FAR * AmendFailureProcAddr)( OrderUpdStructPtr);
typedef VOID (WINAPI FAR * GenericPriceProcAddr)( GenericPriceStructPtr);
typedef VOID (WINAPI FAR * SettlementProcAddr)( SettlementPriceStructPtr);
typedef VOID (WINAPI FAR * StrategyCreateSuccessProcAddr)( StrategyCreateSuccessStructPtr);
typedef VOID (WINAPI FAR * StrategyCreateFailureProcAddr)( StrategyCreateFailureStructPtr);
typedef VOID (WINAPI FAR * BlankPriceProcAddr)( BlankPriceStructPtr);
typedef VOID (WINAPI FAR * OrderBookResetProcAddr)(VOID);
typedef VOID (WINAPI FAR * TraderAddedProcAddr)( TraderAcctStructPtr);
typedef VOID (WINAPI FAR * OrderTypeUpdateAddr)( OrderTypeStructPtr);

typedef INT  (WINAPI * lp_ptRegisterCallback)(INT callbackID, ProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterLinkStateCallback)(INT callbackID, LinkProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterMsgCallback)(INT callbackID, MsgProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterOrderCallback)(INT callbackID, OrderProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterOrderTypeUpdateCallback)(INT callbackID, OrderTypeUpdateAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterFillCallback)(INT callbackID, FillProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterPriceCallback)(INT callbackID, PriceProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterBlankPriceCallback)(INT callbackID, BlankPriceProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterGenericPriceCallback)(INT callbackID, GenericPriceProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterDOMCallback)(INT callbackID, DOMProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterContractCallback)(INT callbackID, ContractProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterCommodityCallback)(INT callbackID, CommodityProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterExchangeCallback)(INT callbackID, ExchangeProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterStatusCallback)(INT CallbackID, StatusProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterExchangeRateCallback)(INT CallbackID, ExchangeRateProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterConStatusCallback)(INT CallbackID, ConStatusProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterOrderCancelFailureCallback)(INT callbackID, OrderProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterOrderSentFailureCallback)(INT callbackID, OrderProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterOrderQueuedFailureCallback)(INT callbackID, OrderProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterAtBestCallback)(INT callbackID, AtBestProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterTickerCallback)(INT callbackID, TickerUpdateProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterAmendFailureCallback)(INT callbackID, AmendFailureProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterSubscriberDepthCallback)(INT callbackID, SubscriberDepthProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterSettlementCallback)(INT callbackID, SettlementProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterStrategyCreateSuccess)(INT callbackID, StrategyCreateSuccessProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterStrategyCreateFailure)(INT callbackID, StrategyCreateFailureProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterOrderBookReset)(INT CallbackID, OrderBookResetProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptRegisterTraderAdded)(INT callbackID, TraderAddedProcAddr CBackProc);
typedef INT  (WINAPI * lp_ptCountExchanges)(PINT Count);
typedef INT  (WINAPI * lp_ptGetExchange)(INT Index, ExchangeStructPtr ExchangeDetails);
typedef INT  (WINAPI * lp_ptGetExchangeByName)(ExchNamePtr ExchangeName, ExchangeStructPtr ExchangeDetails);
typedef INT  (WINAPI * lp_ptExchangeExists)(ExchNamePtr ExchangeName);
typedef INT  (WINAPI * lp_ptCountTraders)(PINT Count);
typedef INT  (WINAPI * lp_ptGetTrader)(INT Index, TraderAcctStructPtr TraderDetails);
typedef INT  (WINAPI * lp_ptGetTraderByName)(TraderPtr TraderAccount, TraderAcctStructPtr TraderDetails);
typedef INT  (WINAPI * lp_ptTraderExists)(TraderPtr TraderAccount);
typedef INT  (WINAPI * lp_ptCountOrderTypes)(PINT Count);
typedef INT  (WINAPI * lp_ptGetOrderType)(INT Index, OrderTypeStructPtr OrderTypeRec, AmendTypesArray AmendOrderTypes);
typedef INT  (WINAPI * lp_ptGetExchangeRate)(CurrNamePtr Currency, ExchRatePtr ExchRate);
typedef INT  (WINAPI * lp_ptCountReportTypes)(PINT Count);
typedef INT  (WINAPI * lp_ptGetReportType)(INT Index, ReportTypePtr ReportType);
typedef INT  (WINAPI * lp_ptReportTypeExists)(ReportTypePtr ReportType);
typedef INT  (WINAPI * lp_ptGetReportSize)(ReportTypePtr ReportType, PINT ReportSize);
typedef INT  (WINAPI * lp_ptGetReport)(ReportTypePtr ReportType, INT BufferSize, PVOID BufferAddr);
typedef INT  (WINAPI * lp_ptCountCommodities)(PINT Count);
typedef INT  (WINAPI * lp_ptGetCommodity)(INT Index, CommodityStructPtr Commodity);
typedef INT  (WINAPI * lp_ptCommodityExists)(ExchNamePtr ExchangeName, ConNamePtr ContractName);
typedef INT  (WINAPI * lp_ptGetCommodityByName)(ExchNamePtr ExchangeName, ConNamePtr ContractName, CommodityStructPtr Commodity);
typedef INT  (WINAPI * lp_ptCountContracts)(PINT Count);
typedef INT  (WINAPI * lp_ptGetContract)(INT Index, ContractStructPtr Contract);
typedef INT  (WINAPI * lp_ptGetContractByName)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, ContractStructPtr Contract);
typedef INT  (WINAPI * lp_ptGetContractByExternalID)(ContractStructPtr  ContractIn, ContractStructPtr ContractOut);
typedef INT  (WINAPI * lp_ptGetExtendedContract)(INT Index, ExtendedContractStructPtr ExtContract);
typedef INT  (WINAPI * lp_ptGetExtendedContractByName)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, ExtendedContractStructPtr ExtContract);
typedef INT  (WINAPI * lp_ptContractExists)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
typedef INT  (WINAPI * lp_ptInitialise)(CHAR Env, LPCSTR  APIversion, LPCSTR  ApplicID, LPCSTR  ApplicVersion, LPCSTR License, bool InitReset);
typedef INT  (WINAPI * lp_ptGetAPIBuildVersion)(APIBuildVerPtr APIVersion);
typedef INT  (WINAPI * lp_ptReady)( );
typedef INT  (WINAPI * lp_ptPurge)(LPCSTR  PDate, LPCSTR PTime);
typedef INT  (WINAPI * lp_ptDisconnect)( );
typedef INT  (WINAPI * lp_ptSetHostAddress)(LPCSTR  IPaddress, LPCSTR IPSocket);
typedef INT  (WINAPI * lp_ptSetPriceAddress)(LPCSTR  IPaddress, LPCSTR IPSocket);
typedef INT  (WINAPI * lp_ptEnable)(INT Code);
typedef INT  (WINAPI * lp_ptDisable)(INT Code);
typedef INT  (WINAPI * lp_ptLogString)(DebugStrPtr DebugStr);
typedef INT  (WINAPI * lp_ptOMIEnabled)(CHAR Enabled);
typedef INT  (WINAPI * lp_ptNotifyAllMessages)(CHAR Enabled);
typedef INT  (WINAPI * lp_ptSetHostReconnect)(INT Interval);
typedef INT  (WINAPI * lp_ptSetPriceReconnect)(INT Interval);
typedef INT  (WINAPI * lp_ptSetPriceAgeCounter)(INT MaxAge);
typedef INT  (WINAPI * lp_ptSubscribePrice)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
typedef INT  (WINAPI * lp_ptSubscribeToMarket)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
typedef INT  (WINAPI * lp_ptUnsubscribePrice)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
typedef INT  (WINAPI * lp_ptUnsubscribeToMarket)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
typedef INT  (WINAPI * lp_ptPriceSnapshot)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, INT Wait);
typedef INT  (WINAPI * lp_ptSetEncryptionCode)(CHAR Code);
typedef INT  (WINAPI * lp_ptSetHandShakePeriod)(INT Seconds);
typedef INT  (WINAPI * lp_ptSetHostHandShake)(INT  Interval, INT TimeOut);
typedef INT  (WINAPI * lp_ptSetPriceHandShake)(INT  Interval, INT TimeOut);
typedef INT  (WINAPI * lp_ptSetInternetUser)(CHAR Enabled);
typedef INT  (WINAPI * lp_ptSetClientPath)(LPCSTR Path);
typedef LPCSTR  (WINAPI * lp_ptGetErrorMessage)(INT Error);
typedef INT  (WINAPI * lp_ptDumpLastError)( );
typedef INT  (WINAPI * lp_ptSnapdragonEnabled)( );
typedef INT  (WINAPI * lp_ptSubscribeBroadcast)(ExchNamePtr ExchangeName);
typedef INT  (WINAPI * lp_ptUnsubscribeBroadcast)(ExchNamePtr ExchangeName);
typedef INT  (WINAPI * lp_ptCountFills)(PINT Count);
typedef INT  (WINAPI * lp_ptGetFill)(INT Index, FillStructPtr Fill);
typedef INT  (WINAPI * lp_ptGetFillByID)(FillIDPtr ID, FillStructPtr Fill);
typedef INT  (WINAPI * lp_ptGetContractPosition)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, TraderPtr TraderAccount, PositionStructPtr Position);
typedef INT  (WINAPI * lp_ptGetOpenPosition)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, TraderPtr TraderAccount, PositionStructPtr Position);
typedef INT  (WINAPI * lp_ptGetTotalPosition)(TraderPtr TraderAccount, PositionStructPtr Position);
typedef INT  (WINAPI * lp_ptGetAveragePrice)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, TraderPtr TraderAccount, PricePtr Price);
typedef INT  (WINAPI * lp_ptCountOrders)(PINT Count);
typedef INT  (WINAPI * lp_ptGetOrder)(INT Index, OrderDetailStructPtr OrderDetail);
typedef INT  (WINAPI * lp_ptGetOrderByID)(OrderIDPtr OrderID, OrderDetailStructPtr OrderDetail, INT OFSequenceNumber);
typedef INT  (WINAPI * lp_ptGetOrderIndex)(OrderIDPtr OrderID, PINT Index);
typedef INT  (WINAPI * lp_ptBlankPrices)( );
typedef INT  (WINAPI * lp_ptGetPrice)(INT Index, PriceStructPtr CurrentPrice);
typedef INT  (WINAPI * lp_ptGetPriceForContract)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, PriceStructPtr CurrentPrice);
typedef INT  (WINAPI * lp_ptGetGenericPrice)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, INT PriceType, INT Side, PriceStructPtr Price);
typedef INT  (WINAPI * lp_ptAddOrder)(NewOrderStructPtr NewOrder, OrderIDPtr OrderID);
typedef INT  (WINAPI * lp_ptAddAlgoOrder)(NewOrderStructPtr NewOrder, INT BuffSize, AlgoBuff AlgoBuffer, OrderIDPtr OrderID);
typedef INT  (WINAPI * lp_ptGetOrderEx)(INT Index, OrderDetailStructPtr OrderDetail, AlgoBuffPtr AlgoDetail, PINT AlgoSize);
typedef INT  (WINAPI * lp_ptGetOrderByIDEx)(OrderIDPtr OrderID, OrderDetailStructPtr OrderDetail, AlgoBuffPtr AlgoDetail, PINT AlgoSize, INT OFSequenceNumber);
typedef INT  (WINAPI * lp_ptGetOrderHistoryEx)(INT  Index, INT Position, OrderDetailStructPtr OrderDetail, AlgoBuffPtr AlgoDetail, PINT AlgoSize);
typedef INT  (WINAPI * lp_ptAddProtectionOrder)(NewOrderStructPtr NewOrder, ProtectionStrucPtr Protection, OrderIDPtr OrderID);
typedef INT  (WINAPI * lp_ptAddBatchOrder)(INT OrderCount, BEPNewOrderStructPtr BEPNewOrder, BEPOrderIDStructPtr BEPOrderIDs);
typedef INT  (WINAPI * lp_ptAddAggregateOrder)(NewAggOrderStructPtr NewAggOrder, OrderIDPtr OrderID);
typedef INT  (WINAPI * lp_ptAddCustRequest)(NewCustReqStructPtr NewCustReq, OrderIDPtr OrderID);
typedef INT  (WINAPI * lp_ptReParent)(OrderIDPtr  OrderID, OrderIDPtr DestParentID);
typedef INT  (WINAPI * lp_ptDoneForDay)(OrderIDPtr OrderID);
typedef INT  (WINAPI * lp_ptAddCrossingOrder)(NewOrderStructPtr  PrimaryOrder, NewOrderStructPtr SecondaryOrder, LegPriceStructPtr LegPrices, CrossingOrderIDsPtr OrderIDs, CHAR FAK);
typedef INT  (WINAPI * lp_ptAddBlockOrder)(NewOrderStructPtr  PrimaryOrder, NewOrderStructPtr SecondaryOrder, LegPriceStructPtr LegPrices, CrossingOrderIDsPtr OrderIDs);
typedef INT  (WINAPI * lp_ptAddBasisOrder)(NewOrderStructPtr  PrimaryOrder, NewOrderStructPtr SecondaryOrder, BasisOrderStructPtr BasisOrder, CrossingOrderIDsPtr OrderID);
typedef INT  (WINAPI * lp_ptAddAAOrder)(NewOrderStructPtr  PrimaryOrder, NewOrderStructPtr SecondaryOrder, UserNameStrPtr  BidUser, UserNameStrPtr OfferUser, CrossingOrderIDsPtr OrderID);
typedef INT  (WINAPI * lp_ptGetConsolidatedPosition)(ExchNamePtr Exchange, ConNamePtr ContractName, ConDatePtr ContractDate, TraderPtr TraderAccount, INT PositionType, FillStructPtr fill);
typedef INT  (WINAPI * lp_ptAmendOrder)(OrderIDPtr OrderID, AmendOrderStructPtr NewDetails);
typedef INT  (WINAPI * lp_ptAmendAlgoOrder)(OrderIDPtr OrderID, INT BuffSize, AlgoBuff AlgoBuffer, AmendOrderStructPtr NewDetails);
typedef INT  (WINAPI * lp_ptCancelOrder)(OrderIDPtr OrderID);
typedef INT  (WINAPI * lp_ptActivateOrder)(OrderIDPtr OrderID);
typedef INT  (WINAPI * lp_ptDeactivateOrder)(OrderIDPtr OrderID);
typedef INT  (WINAPI * lp_ptCancelBuys)(TraderPtr TraderAccount, ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
typedef INT  (WINAPI * lp_ptCancelSells)(TraderPtr TraderAccount, ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
typedef INT  (WINAPI * lp_ptCancelOrders)(TraderPtr TraderAccount, ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate);
typedef INT  (WINAPI * lp_ptCancelAll)(TraderPtr TraderAccount);
typedef INT  (WINAPI * lp_ptOrderChecked)(OrderIDPtr OrderID, CHAR Checked);
typedef INT  (WINAPI * lp_ptQueryOrderStatus)(OrderIDPtr OrderID);
typedef INT  (WINAPI * lp_ptCountOrderHistory)(INT Index, PINT Count);
typedef INT  (WINAPI * lp_ptGetOrderHistory)(INT  Index, INT Position, OrderDetailStructPtr OrderDetail);
typedef INT  (WINAPI * lp_ptSetUserIDFilter)(CHAR Enabled);
typedef INT  (WINAPI * lp_ptNextOrderSequence)(PINT Sequence);
typedef INT  (WINAPI * lp_ptLogOn)(LogonStructPtr LogonDetails);
typedef INT  (WINAPI * lp_ptLogOff)( );
typedef INT  (WINAPI * lp_ptGetLogonStatus)(LogonStatusStructPtr LogonStatus);
typedef INT  (WINAPI * lp_ptDOMEnabled)( );
typedef INT  (WINAPI * lp_ptPostTradeAmendEnabled)( );
typedef INT  (WINAPI * lp_ptEnabledFunctionality)(PINT  FunctionalityEnabled, PINT SoftwareEnabled);
typedef INT  (WINAPI * lp_ptCountUsrMsgs)(PINT Count);
typedef INT  (WINAPI * lp_ptGetUsrMsg)(INT Index, MessageStructPtr UserMsg);
typedef INT  (WINAPI * lp_ptGetUsrMsgByID)(MsgIDPtr MsgID, MessageStructPtr UserMsg);
typedef INT  (WINAPI * lp_ptAcknowledgeUsrMsg)(MsgIDPtr MsgID);
typedef INT  (WINAPI * lp_ptPriceStep)(double Price, double TickSize, INT NumSteps, INT TicksPerPoint);
typedef INT  (WINAPI * lp_ptPLBurnRate)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, TraderPtr TraderAccount, FloatPtr BurnRate);
typedef INT  (WINAPI * lp_ptOpenPositionExposure)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, TraderPtr TraderAccount, FloatPtr Exposure);
typedef INT  (WINAPI * lp_ptBuyingPowerRemaining)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, TraderPtr TraderAccount, FloatPtr BPRemaining);
typedef INT  (WINAPI * lp_ptBuyingPowerUsed)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, TraderPtr TraderAccount, FloatPtr BPUsed);
typedef INT  (WINAPI * lp_ptMarginForTrade)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, TraderPtr TraderAccount, PINT Lots, OrderIDPtr OrderID, PricePtr Price, FloatPtr MarginReqd);
typedef INT  (WINAPI * lp_ptTotalMarginPaid)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, TraderPtr TraderAccount, FloatPtr MarginReqd);
typedef INT  (WINAPI * lp_ptGetMarginPerLot)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, TraderPtr TraderAccount, FloatPtr Margin);
typedef INT  (WINAPI * lp_ptSetOrderCancelFailureDelay)(INT Code);
typedef INT  (WINAPI * lp_ptSetOrderSentFailureDelay)(INT Code);
typedef INT  (WINAPI * lp_ptSetOrderQueuedFailureDelay)(INT Code);
typedef INT  (WINAPI * lp_ptCountContractAtBest)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, PINT Count);
typedef INT  (WINAPI * lp_ptGetContractAtBest)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, INT Index, AtBestStructPtr AtBest);
typedef INT  (WINAPI * lp_ptGetContractAtBestPrices)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, AtBestPriceStructPtr AtBestPrices);
typedef INT  (WINAPI * lp_ptCountContractSubscriberDepth)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, PINT Count);
typedef INT  (WINAPI * lp_ptGetContractSubscriberDepth)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, INT Index, SubscriberDepthStructPtr SubscriberDepth);
typedef INT  (WINAPI * lp_ptSuperTASEnabled)( );
typedef INT  (WINAPI * lp_ptSetSSL)(CHAR Enabled);
typedef INT  (WINAPI * lp_ptSetPDDSSL)(CHAR Enabled);
typedef INT  (WINAPI * lp_ptSetMDSToken)(MDSTokenPtr MDSToken);
typedef INT  (WINAPI * lp_ptSetSSLCertificateName)(CertNamePtr CertName);
typedef INT  (WINAPI * lp_ptSetPDDSSLCertificateName)(CertNamePtr CertName);
typedef INT  (WINAPI * lp_ptSetSSLClientAuthName)(CertNamePtr CertName);
typedef INT  (WINAPI * lp_ptSetPDDSSLClientAuthName)(CertNamePtr CertName);
typedef INT  (WINAPI * lp_ptSetSuperTAS)(CHAR Enabled);
typedef INT  (WINAPI * lp_ptSetMemoryWarning)(INT MemAmount);
typedef INT  (WINAPI * lp_ptCreateStrategy)(CHAR StrategyCode, INT NoOfLegs, ExchNamePtr ExchangeName, ConNamePtr ContractName, StrategyLegsStructPtr Legs);
typedef INT  (WINAPI * lp_ptGetOptionPremium)(ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, CHAR BuySell, PricePtr Price, INT Lots, FloatPtr OPr);
typedef INT  (WINAPI * lp_ptLockUpdates)( );
typedef INT  (WINAPI * lp_ptUnlockUpdates)( );
typedef INT  (WINAPI * lp_ptAddOrderEx)(NewOrderStructPtr NewOrder, OrderIDPtr OrderID, UserNameStrPtr UserName);
typedef INT  (WINAPI * lp_ptAmendOrderEx)(OrderIDPtr OrderID, AmendOrderStructPtr NewDetails, UserNameStrPtr UserName);
typedef INT  (WINAPI * lp_ptCancelOrderEx)(OrderIDPtr OrderID, UserNameStrPtr UserName);
typedef INT  (WINAPI * lp_ptCancelBuysEx)(TraderPtr TraderAccount, ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, UserNameStrPtr UserName);
typedef INT  (WINAPI * lp_ptCancelSellsEx)(TraderPtr TraderAccount, ExchNamePtr ExchangeName, ConNamePtr ContractName, ConDatePtr ContractDate, UserNameStrPtr UserName);


	// Callback declarations
VOID WINAPI HostLinkStateChange(LinkStateStructPtr data);
VOID WINAPI PriceLinkStateChange(LinkStateStructPtr data);
VOID WINAPI LogonStatus();
VOID WINAPI Message(MsgIDPtr MsgID);
VOID WINAPI Order(OrderUpdStructPtr data);
VOID WINAPI ForcedLogout();
VOID WINAPI DataDLComplete();
VOID WINAPI PriceUpdate(PriceUpdStructPtr PriceUpdate);
VOID WINAPI Fill(FillUpdStructPtr data);
VOID WINAPI StatusChange(StatusUpdStructPtr StatusUpdate);
VOID WINAPI ContractAdded(ContractUpdStructPtr data);
VOID WINAPI ContractDeleted(ContractUpdStructPtr data);
VOID WINAPI OrderCancelFailure(OrderUpdStructPtr data);
VOID WINAPI ExchangeRate(ExchangeRateUpdStructPtr data);
VOID WINAPI ConStatus(ConnectivityStatusUpdStructPtr data);
VOID WINAPI AtBest(AtBestUpdStructPtr data);
VOID WINAPI Ticker(TickerUpdStructPtr data);
VOID WINAPI SubscriberDepthUpdate(SubscriberDepthUpdStructPtr SupbscriberDepthUpdate);

#pragma pack (pop,PATSAPI)


#pragma  once

/////////////////////////////////////////////////////////////////////
#define TABLENAME_TRADEDATA_ORDERS "ExecuteIn_Orders"
#define TABLENAME_EXECUTEIN_ORDERS "Settlement_Trade"
#define TABLENAME_CTP_ORDERS "CTP_Orders"

const char* gSqlScriptOrderFormat = "\r\n\
BEGIN\r\n\
DROP INDEX \"%s_IV\";\r\n\
\r\n\
DROP INDEX \"%s_II\";\r\n\
\r\n\
DROP TABLE \"%s\" CASCADE CONSTRAINTS;\r\n\
\r\n\
CREATE TABLE \"%s\" (\r\n\
								  \"BROKERID\"           VARCHAR2(11),\r\n\
								  \"INVESTORID\"         VARCHAR2(13),\r\n\
								  \"INSTRUMENTID\"       VARCHAR2(31),\r\n\
								  \"ORDERREF\"           VARCHAR2(13),\r\n\
								  \"USERID\"             VARCHAR2(16),\r\n\
								  \"ORDERPRICETYPE\"     CHAR,\r\n\
								  \"DIRECTION\"          CHAR,\r\n\
								  \"COMBOFFSETFLAG\"     VARCHAR2(5),\r\n\
								  \"COMBHEDGEFLAG\"      VARCHAR2(5),\r\n\
								  \"LIMITPRICE\"         BINARY_DOUBLE,\r\n\
								  \"VOLUMETOTALORIGINAL\" INTEGER,\r\n\
								  \"TIMECONDITION\"      CHAR,\r\n\
								  \"GTDDATE\"            VARCHAR2(9),\r\n\
								  \"VOLUMECONDITION\"    CHAR,\r\n\
								  \"MINVOLUME\"          INTEGER,\r\n\
								  \"CONTINGENTCONDITION\" CHAR,\r\n\
								  \"STOPPRICE\"          BINARY_DOUBLE,\r\n\
								  \"FORCECLOSEREASON\"   CHAR,\r\n\
								  \"ISAUTOSUSPEND\"      INTEGER,\r\n\
								  \"BUSINESSUNIT\"       VARCHAR2(21),\r\n\
								  \"REQUESTID\"          INTEGER,\r\n\
								  \"ORDERLOCALID\"       VARCHAR2(13),\r\n\
								  \"EXCHANGEID\"         VARCHAR2(9),\r\n\
								  \"PARTICIPANTID\"      VARCHAR2(11),\r\n\
								  \"CLIENTID\"           VARCHAR2(11),\r\n\
								  \"EXCHANGEINSTID\"     VARCHAR2(31),\r\n\
								  \"TRADERID\"           VARCHAR2(21),\r\n\
								  \"INSTALLID\"          INTEGER,\r\n\
								  \"ORDERSUBMITSTATUS\"  CHAR,\r\n\
								  \"NOTIFYSEQUENCE\"     INTEGER,\r\n\
								  \"TRADINGDAY\"         VARCHAR2(9),\r\n\
								  \"SETTLEMENTID\"       INTEGER,\r\n\
								  \"ORDERSYSID\"         VARCHAR2(21),\r\n\
								  \"ORDERSOURCE\"        CHAR,\r\n\
								  \"ORDERSTATUS\"        CHAR,\r\n\
								  \"ORDERTYPE\"          CHAR,\r\n\
								  \"VOLUMETRADED\"       INTEGER,\r\n\
								  \"VOLUMETOTAL\"        INTEGER,\r\n\
								  \"INSERTDATE\"         VARCHAR2(9),\r\n\
								  \"INSERTTIME\"         VARCHAR2(9),\r\n\
								  \"ACTIVETIME\"         VARCHAR2(9),\r\n\
								  \"SUSPENDTIME\"        VARCHAR2(9),\r\n\
								  \"UPDATETIME\"         VARCHAR2(9),\r\n\
								  \"CANCELTIME\"         VARCHAR2(9),\r\n\
								  \"ACTIVETRADERID\"     VARCHAR2(21),\r\n\
								  \"CLEARINGPARTID\"     VARCHAR2(11),\r\n\
								  \"SEQUENCENO\"         INTEGER,\r\n\
								  \"FRONTID\"            INTEGER,\r\n\
								  \"SESSIONID\"          INTEGER,\r\n\
								  \"USERPRODUCTINFO\"    VARCHAR2(11),\r\n\
								  \"STATUSMSG\"          VARCHAR2(81),\r\n\
								  \"USERFORCECLOSE\"     INTEGER,\r\n\
								  \"ACTIVEUSERID\"       VARCHAR2(16),\r\n\
								  \"BROKERORDERSEQ\"     INTEGER,\r\n\
								  \"RELATIVEORDERSYSID\" VARCHAR2(21),\r\n\
								  \"AVGPRICE\"           BINARY_DOUBLE,\r\n\
								  \"EXSTATUS\"           INTEGER,\r\n\
								  \"VALIDATEDATE\"       VARCHAR2(11),\r\n\
								  \"INSERTDBTIME\"       DATE\r\n\
								  );\r\n\
\r\n\
CREATE INDEX \"%s_II\" ON \"%s\" (\r\n\
	\"INVESTORID\" ASC\r\n\
	);\r\n\
\r\n\
\r\n\
CREATE INDEX \"%s_IV\" ON \"%s\" (\r\n\
	\"VALIDATEDATE\" ASC\r\n\
	);\r\n\
END;";


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TABLENAME_TRADEDATA_TRADES "TradeData_Trades"
#define TABLENAME_SETTLEMENT_TRADES "Settlement_Trade"
#define TABLENAME_CTP_TRADES "CTP_Trades"

const char* gSqlScriptTraderFormat = "\
\
DROP INDEX \"%s_Index_Validate\";\
\
DROP INDEX \"%s_Index_InvestorID\";\
\
DROP TABLE \"%s\" CASCADE CONSTRAINTS;\
\
CREATE TABLE \"%s\"  (\
								  \"BROKERID\"           VARCHAR2(11),\
								  \"INVESTORID\"         VARCHAR2(13),\
								  \"INSTRUMENTID\"       VARCHAR2(31),\
								  \"ORDERREF\"           VARCHAR2(13),\
								  \"USERID\"             VARCHAR2(16),\
								  \"EXCHANGEID\"         VARCHAR2(9),\
								  \"TRADEID\"            VARCHAR2(21),\
								  \"DIRECTION\"          CHAR,\
								  \"ORDERSYSID\"         VARCHAR2(21),\
								  \"PARTICIPANTID\"      VARCHAR2(11),\
								  \"CLIENTID\"           VARCHAR2(11),\
								  \"TRADINGROLE\"        CHAR,\
								  \"EXCHANGEINSTID\"     VARCHAR2(31),\
								  \"OFFSETFLAG\"         CHAR,\
								  \"HEDGEFLAG\"          CHAR,\
								  \"PRICE\"              BINARY_DOUBLE,\
								  \"VOLUME\"             INTEGER,\
								  \"TRADEDATE\"          VARCHAR2(9),\
								  \"TRADETIME\"          VARCHAR2(9),\
								  \"TRADETYPE\"          CHAR,\
								  \"PRICESOURCE\"        CHAR,\
								  \"TRADERID\"           VARCHAR2(21),\
								  \"ORDERLOCALID\"       VARCHAR2(13),\
								  \"CLEARINGPARTID\"     VARCHAR2(11),\
								  \"BUSINESSUNIT\"       VARCHAR2(21),\
								  \"SEQUENCENO\"         INTEGER,\
								  \"TRADINGDAY\"         VARCHAR2(9),\
								  \"SETTLEMENTID\"       INTEGER,\
								  \"BROKERORDERSEQ\"     INTEGER,\
								  \"TRADESOURCE\"        CHAR,\
								  \"CLOSEPROFITBYDATE\"  BINARY_DOUBLE,\
								  \"CLOSEPROFITBYTRADE\" BINARY_DOUBLE,\
								  \"TRADECOMMISSION\"    BINARY_DOUBLE,\
								  \"VALIDATEDATE\"       VARCHAR2(11),\
								  \"INSERTDBTIME\"       DATE\
								  );\
\
\
CREATE INDEX \"Index_InvestorID\" ON \"%s\" (\
	\"INVESTORID\" ASC\
	);\
\
\
CREATE INDEX \"Index_Validate\" ON \"%s\" (\
	\"VALIDATEDATE\" ASC\
	);\
";


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TABLENAME_TRADEDATA_INSTRUMENT "TradeData_Instruements"

const char* gSqlScriptInstrumentFormat = "\
DROP INDEX \"%s_Index_Validate\";\
\
DROP TABLE \"%s\" CASCADE CONSTRAINTS;\
\
CREATE TABLE \"%s\"  (\
										\"INSTRUMENTID\"       VARCHAR2(64),\
										\"EXCHANGEID\"         VARCHAR2(9),\
										\"INSTRUMENTNAME\"     VARCHAR2(64),\
										\"EXCHANGEINSTID\"     VARCHAR2(31),\
										\"PRODUCTID\"          VARCHAR2(64),\
										\"PRODUCTCLASS\"       CHAR,\
										\"DELIVERYYEAR\"       INTEGER,\
										\"DELIVERYMONTH\"      INTEGER,\
										\"MAXMARKETORDERVOLUME\" INTEGER,\
										\"MINMARKETORDERVOLUME\" INTEGER,\
										\"MAXLIMITORDERVOLUME\" INTEGER,\
										\"MINLIMITORDERVOLUME\" INTEGER,\
										\"VOLUMEMULTIPLE\"     INTEGER,\
										\"PRICETICK\"          BINARY_DOUBLE,\
										\"CREATEDATE\"         VARCHAR2(9),\
										\"OPENDATE\"           VARCHAR2(9),\
										\"EXPIREDATE\"         VARCHAR2(9),\
										\"STARTDELIVDATE\"     VARCHAR2(9),\
										\"ENDDELIVDATE\"       VARCHAR2(9),\
										\"INSTLIFEPHASE\"      CHAR,\
										\"ISTRADING\"          INTEGER,\
										\"POSITIONTYPE\"       CHAR,\
										\"POSITIONDATETYPE\"   CHAR,\
										\"LONGMARGINRATIO\"    BINARY_DOUBLE,\
										\"SHORTMARGINRATIO\"   BINARY_DOUBLE,\
										\"VALIDATEDATE\"       VARCHAR2(11),\
										\"INSERTDBTIME\"       DATE\
										);\
\
\
CREATE INDEX \"%s_Index_Validate\" ON \"%s\" (\
	\"VALIDATEDATE\" ASC\
	);\
\
";


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TABLENAME_TRADEDATA_COMMISSION "TradeData_Commission"
#define TABLENAME_SETTLEMENT_COMMISSION "Settlement_Commission"

const char* gSqlScriptCommissionFormat = "\
\
DROP INDEX \"%s_Index_Validate\";\
DROP INDEX \"%s_Index_BrokerId\";\
DROP INDEX \"%s_Index_INVESTORID\";\
\
DROP TABLE \"%s\" CASCADE CONSTRAINTS;\
\
\
CREATE TABLE \"%s\"  (\
									  \"INSTRUMENTID\"       VARCHAR2(31),\
									  \"INVESTORRANGE\"      CHAR,\
									  \"BROKERID\"           VARCHAR2(11),\
									  \"INVESTORID\"         VARCHAR2(13),\
									  \"OPENRATIOBYMONEY\"   BINARY_DOUBLE,\
									  \"OPENRATIOBYVOLUME\"  BINARY_DOUBLE,\
									  \"CLOSERATIOBYMONEY\"  BINARY_DOUBLE,\
									  \"CLOSERATIOBYVOLUME\" BINARY_DOUBLE,\
									  \"CLOSETODAYRATIOBYMONEY\" BINARY_DOUBLE,\
									  \"CLOSETODAYRATIOBYVOLUME\" BINARY_DOUBLE,\
									  \"VALIDATEDATE\"       VARCHAR2(11),\
									  \"INSERTDBTIME\"       DATE\
									  );\
CREATE INDEX \"%s_Index_BrokerId\" ON \"%s\" (\
	\"BROKERID\" ASC\
	);\
CREATE INDEX \"%s_Index_INVESTORID\" ON \"%s\" (\
	\"INVESTORID\" ASC\
	);\
CREATE INDEX \"%s_Index_Validate\" ON \"%s\" (\
	\"VALIDATEDATE\" ASC\
	);\
";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TABLENAME_TRADEDATA_MARGIN "TradeData_MarginRate"
#define TABLENAME_SETTLEMENT_MARGIN "Settlement_MarginRate"

const char* gSqlScriptMarginFormat = "\
\
DROP INDEX \"%s_Index_Validate\";\
DROP INDEX \"%s_Index_BrokerId\";\
DROP INDEX \"%s_Index_INVESTORID\";\
\
DROP TABLE \"%s\" CASCADE CONSTRAINTS;\
\
CREATE TABLE \"%s\"  (\
									  \"INSTRUMENTID\"       VARCHAR2(31),\
									  \"INVESTORRANGE\"      CHAR,\
									  \"BROKERID\"           VARCHAR2(11),\
									  \"INVESTORID\"         VARCHAR2(13),\
									  \"HEDGEFLAG\"          CHAR,\
									  \"LONGMARGINRATIOBYMONEY\" BINARY_DOUBLE,\
									  \"LONGMARGINRATIOBYVOLUME\" BINARY_DOUBLE,\
									  \"SHORTMARGINRATIOBYMONEY\" BINARY_DOUBLE,\
									  \"SHORTMARGINRATIOBYVOLUME\" BINARY_DOUBLE,\
									  \"ISRELATIVE\"         INTEGER,\
									  \"VALIDATEDATE\"       VARCHAR2(11),\
									  \"INSERTDBTIME\"       DATE\
									  );\
\
CREATE INDEX \"%s_Index_BrokerId\" ON \"%s\" (\
	\"BROKERID\" ASC\
	);\
CREATE INDEX \"%s_Index_INVESTORID\" ON \"%s\" (\
	\"INVESTORID\" ASC\
	);\
CREATE INDEX \"%s_Index_Validate\" ON \"%s\" (\
	\"VALIDATEDATE\" ASC\
	);\
";


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TABLENAME_TRADEDATA_POSITION "TradeData_Position"
#define TABLENAME_SETTLEMENT_POSITION "Settlement_Position"

const char* gSqlScriptPositionFormat = "\
\
DROP INDEX \"%s_Index_Validate\";\
DROP INDEX \"%s_Index_INVESTORID\";\
\
DROP TABLE \"%s\" CASCADE CONSTRAINTS;\
\
CREATE TABLE \"%s\"  (\
									\"INSTRUMENTID\"       VARCHAR2(31),\
									\"BROKERID\"           VARCHAR2(11),\
									\"INVESTORID\"         VARCHAR2(13),\
									\"POSIDIRECTION\"      CHAR,\
									\"HEDGEFLAG\"          CHAR,\
									\"TRADINGDAY\"         VARCHAR2(9),\
									\"POSITIONDATE\"       CHAR,\
									\"SETTLEMENTID\"       INTEGER,\
									\"POSITION\"           INTEGER,\
									\"TODAYPOSITION\"      INTEGER,\
									\"YDPOSITION\"         INTEGER,\
									\"OPENVOLUME\"         INTEGER,\
									\"CLOSEVOLUME\"        INTEGER,\
									\"OPENAMOUNT\"         BINARY_DOUBLE,\
									\"CLOSEAMOUNT\"        BINARY_DOUBLE,\
									\"POSITIONCOST\"       BINARY_DOUBLE,\
									\"OPENCOST\"           BINARY_DOUBLE,\
									\"LONGFROZEN\"         INTEGER,\
									\"SHORTFROZEN\"        INTEGER,\
									\"LONGFROZENAMOUNT\"   BINARY_DOUBLE,\
									\"SHORTFROZENAMOUNT\"  BINARY_DOUBLE,\
									\"FROZENMARGIN\"       BINARY_DOUBLE,\
									\"FROZENCOMMISSION\"   BINARY_DOUBLE,\
									\"FROZENCASH\"         BINARY_DOUBLE,\
									\"COMMISSION\"         BINARY_DOUBLE,\
									\"PREMARGIN\"          BINARY_DOUBLE,\
									\"USEMARGIN\"          BINARY_DOUBLE,\
									\"EXCHANGEMARGIN\"     BINARY_DOUBLE,\
									\"MARGINRATEBYMONEY\"  BINARY_DOUBLE,\
									\"MARGINRATEBYVOLUME\" BINARY_DOUBLE,\
									\"CASHIN\"             BINARY_DOUBLE,\
									\"POSITIONPROFIT\"     BINARY_DOUBLE,\
									\"CLOSEPROFIT\"        BINARY_DOUBLE,\
									\"CLOSEPROFITBYDATE\"  BINARY_DOUBLE,\
									\"CLOSEPROFITBYTRADE\" BINARY_DOUBLE,\
									\"PRESETTLEMENTPRICE\" BINARY_DOUBLE,\
									\"SETTLEMENTPRICE\"    BINARY_DOUBLE,\
									\"COMBPOSITION\"       INTEGER,\
									\"COMBLONGFROZEN\"     INTEGER,\
									\"COMBSHORTFROZEN\"    INTEGER,\
									\"POSITIONPROFITBYTRADE\" BINARY_DOUBLE,\
									\"TOTALPOSITIONPROFITBYDATE\" BINARY_DOUBLE,\
									\"VALIDATEDATE\"       VARCHAR2(11),\
									\"INSERTDBTIME\"       DATE\
									);\
\
\
CREATE INDEX \"%s_Index_INVESTORID\" ON \"%s\" (\
	\"INVESTORID\" ASC\
	);\
CREATE INDEX \"%s_Index_Validate\" ON \"%s\" (\
	\"VALIDATEDATE\" ASC\
	);\
";


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TABLENAME_TRADEDATA_POSITION_DETAIL "TradeData_PositionDetail"
#define TABLENAME_SETTLEMENT_POSITION_DETAIL "Settlement_PositionDetail"

const char* gSqlScriptPositionDetailFormat = "\
\
DROP INDEX \"%s_Index_Validate\";\
DROP INDEX \"%s_Index_INVESTORID\";\
\
DROP TABLE \"%s\" CASCADE CONSTRAINTS;\
\
CREATE TABLE \"%s\"  (\
	\"INSTRUMENTID\"       VARCHAR2(31),\
	\"BROKERID\"           VARCHAR2(11),\
	\"INVESTORID\"         VARCHAR2(13),\
	\"HEDGEFLAG\"          CHAR,\
	\"DIRECTION\"          CHAR,\
	\"OPENDATE\"           VARCHAR2(9),\
	\"TRADEID\"            VARCHAR2(21),\
	\"VOLUME\"             INTEGER,\
	\"OPENPRICE\"          BINARY_DOUBLE,\
	\"TRADINGDAY\"         VARCHAR2(9),\
	\"SETTLEMENTID\"       INTEGER,\
	\"TRADETYPE\"          CHAR,\
	\"COMINSTRUMENTID\"    VARCHAR2(31),\
	\"EXCHANGEID\"         VARCHAR2(9),\
	\"CLOSEPROFITBYDATE\"  BINARY_DOUBLE,\
	\"CLOSEPROFITBYTRADE\" BINARY_DOUBLE,\
	\"POSITIONPROFITBYDATE\" BINARY_DOUBLE,\
	\"POSITIONPROFITBYTRADE\" BINARY_DOUBLE,\
	\"MARGIN\"             BINARY_DOUBLE,\
	\"EXCHMARGIN\"         BINARY_DOUBLE,\
	\"MARGINRATEBYMONEY\"  BINARY_DOUBLE,\
	\"MARGINRATEBYVOLUME\" BINARY_DOUBLE,\
	\"LASTSETTLEMENTPRICE\" BINARY_DOUBLE,\
	\"SETTLEMENTPRICE\"    BINARY_DOUBLE,\
	\"CLOSEVOLUME\"        INTEGER,\
	\"CLOSEAMOUNT\"        BINARY_DOUBLE,\
	\"VALIDATEDATE\"       VARCHAR2(11),\
	\"INSERTDBTIME\"       DATE\
	);\
\
\
CREATE INDEX \"%s_Index_INVESTORID\" ON \"%s\" (\
	\"INVESTORID\" ASC\
	);\
CREATE INDEX \"%s_Index_Validate\" ON \"%s\" (\
	\"VALIDATEDATE\" ASC\
	);\
";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TABLENAME_TRADEDATA_FUNDINFO "TradeData_FUNDINFO"
#define TABLENAME_SETTLEMENT_FUNDINFO "Settlement_FundInfo"

const char* gSqlScriptFundInfoFormat = "\
\
DROP INDEX \"%s_Index_Validate\";\
DROP INDEX \"%s_Index_INVESTORID\";\
\
DROP TABLE \"%s\" CASCADE CONSTRAINTS;\
\
CREATE TABLE \"%s\"  (\
									\"BROKERID\"           VARCHAR2(11),\
									\"ACCOUNTID\"          VARCHAR2(13),\
									\"PREMORTAGE\"         BINARY_DOUBLE,\
									\"PRECREDIT\"          BINARY_DOUBLE,\
									\"PREDEPOSIT\"         BINARY_DOUBLE,\
									\"PREBALANCE\"         BINARY_DOUBLE,\
									\"PREMARGIN\"          BINARY_DOUBLE,\
									\"INTERESTBASE\"       BINARY_DOUBLE,\
									\"INTEREST\"           BINARY_DOUBLE,\
									\"DEPOSIT\"            BINARY_DOUBLE,\
									\"WITHDRAW\"           BINARY_DOUBLE,\
									\"FROZENMARGIN\"       BINARY_DOUBLE,\
									\"FROZENCASH\"         BINARY_DOUBLE,\
									\"FROZENCOMMISSION\"   BINARY_DOUBLE,\
									\"CURRMARGIN\"         BINARY_DOUBLE,\
									\"CASHIN\"             BINARY_DOUBLE,\
									\"COMMISSION\"         BINARY_DOUBLE,\
									\"CLOSEPROFIT\"        BINARY_DOUBLE,\
									\"POSITIONPROFIT\"     BINARY_DOUBLE,\
									\"BALANCE\"            BINARY_DOUBLE,\
									\"AVAILABLE\"          BINARY_DOUBLE,\
									\"WITHDRAWQUOTA\"      BINARY_DOUBLE,\
									\"RESERVE\"            BINARY_DOUBLE,\
									\"TRADINGDAY\"         VARCHAR2(9),\
									\"SETTLEMENTID\"       INTEGER,\
									\"CREDIT\"             BINARY_DOUBLE,\
									\"MORTGAGE\"           BINARY_DOUBLE,\
									\"EXCHANGEMARGIN\"     BINARY_DOUBLE,\
									\"DELIVERYMARGIN\"     BINARY_DOUBLE,\
									\"EXCHANGEDELIVERYMARGIN\" BINARY_DOUBLE,\
									\"STATICPROFIT\"       BINARY_DOUBLE,\
									\"DYNAMICPROFIT\"      BINARY_DOUBLE,\
									\"RISKDEGREE\"         BINARY_DOUBLE,\
									\"VALIDATEDATE\"       VARCHAR2(11),\
									\"INSERTDBTIME\"       DATE\
									);\
\
CREATE INDEX \"%s_Index_INVESTORID\" ON \"%s\" (\
	\"VALIDATEDATE\" ASC\
	);\
CREATE INDEX \"%s_Index_Validate\" ON \"%s\" (\
	\"VALIDATEDATE\" ASC\
	);\
";


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TABLENAME_SETTLEMTPRICE_QUTO "TradeData_Quto"

const char* gSqlScriptQutoFormat = "\
\
DROP INDEX \"%s_Index_Validate\";\
\
DROP TABLE \"%s\" CASCADE CONSTRAINTS;\
\
CREATE TABLE \"TRADEDATA_QUTO\"  (\
								\"TRADINGDAY\"         VARCHAR2(9),\
								\"INSTRUMENTID\"       VARCHAR2(64),\
								\"EXCHANGEID\"         VARCHAR2(9),\
								\"EXCHANGEINSTID\"     VARCHAR2(31),\
								\"LASTPRICE\"          BINARY_DOUBLE,\
								\"PRESETTLEMENTPRICE\" BINARY_DOUBLE,\
								\"PRECLOSEPRICE\"      BINARY_DOUBLE,\
								\"PREOPENINTEREST\"    BINARY_DOUBLE,\
								\"OPENPRICE\"          BINARY_DOUBLE,\
								\"HIGHESTPRICE\"       BINARY_DOUBLE,\
								\"LOWESTPRICE\"        BINARY_DOUBLE,\
								\"VOLUME\"             INTEGER,\
								\"TURNOVER\"           BINARY_DOUBLE,\
								\"OPENINTEREST\"       BINARY_DOUBLE,\
								\"CLOSEPRICE\"         BINARY_DOUBLE,\
								\"SETTLEMENTPRICE\"    BINARY_DOUBLE,\
								\"UPPERLIMITPRICE\"    BINARY_DOUBLE,\
								\"LOWERLIMITPRICE\"    BINARY_DOUBLE,\
								\"PREDELTA\"           BINARY_DOUBLE,\
								\"CURRDELTA\"          BINARY_DOUBLE,\
								\"UPDATETIME\"         VARCHAR2(9),\
								\"UPDATEMILLISEC\"     INTEGER,\
								\"BIDPRICE1\"          BINARY_DOUBLE,\
								\"BIDVOLUME1\"         INTEGER,\
								\"ASKPRICE1\"          BINARY_DOUBLE,\
								\"ASKVOLUME1\"         INTEGER,\
								\"BIDPRICE2\"          BINARY_DOUBLE,\
								\"BIDVOLUME2\"         INTEGER,\
								\"ASKPRICE2\"          BINARY_DOUBLE,\
								\"ASKVOLUME2\"         INTEGER,\
								\"BIDPRICE3\"          BINARY_DOUBLE,\
								\"BIDVOLUME3\"         INTEGER,\
								\"ASKPRICE3\"          BINARY_DOUBLE,\
								\"ASKVOLUME3\"         INTEGER,\
								\"BIDPRICE4\"          BINARY_DOUBLE,\
								\"BIDVOLUME4\"         INTEGER,\
								\"ASKPRICE4\"          BINARY_DOUBLE,\
								\"ASKVOLUME4\"         INTEGER,\
								\"BIDPRICE5\"          BINARY_DOUBLE,\
								\"BIDVOLUME5\"         INTEGER,\
								\"ASKPRICE5\"          BINARY_DOUBLE,\
								\"ASKVOLUME5\"         INTEGER,\
								\"AVERAGEPRICE\"       BINARY_DOUBLE,\
								\"BIDPRICE6\"          BINARY_DOUBLE,\
								\"BIDVOLUME6\"         INTEGER,\
								\"ASKPRICE6\"          BINARY_DOUBLE,\
								\"ASKVOLUME6\"         INTEGER,\
								\"BIDPRICE7\"          BINARY_DOUBLE,\
								\"BIDVOLUME7\"         INTEGER,\
								\"ASKPRICE7\"          BINARY_DOUBLE,\
								\"ASKVOLUME7\"         INTEGER,\
								\"BIDPRICE8\"          BINARY_DOUBLE,\
								\"BIDVOLUME8\"         INTEGER,\
								\"ASKPRICE8\"          BINARY_DOUBLE,\
								\"ASKVOLUME8\"         INTEGER,\
								\"BIDPRICE9\"          BINARY_DOUBLE,\
								\"BIDVOLUME9\"         INTEGER,\
								\"ASKPRICE9\"          BINARY_DOUBLE,\
								\"ASKVOLUME9\"         INTEGER,\
								\"BIDPRICE10\"         BINARY_DOUBLE,\
								\"BIDVOLUME10\"        INTEGER,\
								\"ASKPRICE10\"         BINARY_DOUBLE,\
								\"ASKVOLUME10\"        INTEGER,\
								\"NEWVOLUME\"          INTEGER,\
								\"VALIDATEDATE\"       VARCHAR2(11),\
								\"INSERTDBTIME\"       DATE\
								);\
\
CREATE INDEX \"%s_Index_Validate\" ON \"%s\" (\
	\"VALIDATEDATE\" ASC\
	);\
";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TABLENAME_FUND_INOUT "TradeData_InitFund"
const char* gSqlScriptFundInOutFormat = "\
\
DROP INDEX \"%s_Index_Validate\";\
\
DROP TABLE \"%s\" CASCADE CONSTRAINTS;\
CREATE TABLE \"TRADEDATA_FUNDINOUT\"  (\
									 \"USERID\"             VARCHAR2(21),\
									 \"EINOUT\"             INTEGER,\
									 \"VOLUMN\"             BINARY_DOUBLE,\
									 \"OPUSERID\"           VARCHAR2(21),\
									 \"DESC\"               VARCHAR2(200),\
									 \"OPDATE\"             VARCHAR2(11),\
									 \"OPTIME\"             VARCHAR2(9),\
									 \"VAILDATEDATE\"       DATE\
									 );\
\
CREATE INDEX \"%s_Index_Validate\" ON \"%s\" (\
	\"VALIDATEDATE\" ASC\
	);\
";
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TABLENAME_INIT_FUND "TradeData_InitFund"

const char* gSqlScriptInitFundFormat = "\
\
DROP INDEX \"%s_Index_Validate\";\
DROP INDEX \"%s_Index_UserID\";\
DROP TABLE \"%s\" CASCADE CONSTRAINTS;\
CREATE TABLE \"%s\"  (\
									\"USERID\"             VARCHAR2(21),\
									\"INITFUND\"           BINARY_DOUBLE,\
									\"VAILDATEDATE\"       DATE\
									);\
\
CREATE INDEX \"%s_Index_Validate\" ON \"%s\" (\
	\"VALIDATEDATE\" ASC\
	);\
CREATE INDEX \"%s_Index_UserID\" ON \"%s\" (\
	\"USERID\" ASC\
	);\
";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TABLENAME_TRADING_DAY "TradingDay"

const char* gSqlScriptTradingDayFormat = "\
\
\
DROP TABLE \"%s\" CASCADE CONSTRAINTS;\
CREATE TABLE \"%s\"  (\
							\"TRADINGDAY\"         VARCHAR2(11),\
							\"VALIDATEDATE\"       DATE\
							);\
";



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TABLENAME_SETTLEMENT_DAY "SETTLEMENTDAY"

const char* gSqlScriptSettlementDayFormat = "\
\
\
DROP TABLE \"%s\" CASCADE CONSTRAINTS;\
CREATE TABLE \"%s\"  (\
					\"SETTLEMENTDAY\"         VARCHAR2(11),\
					\"VALIDATEDATE\"       DATE\
					);\
";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TABLENAME_SETTLEMENT_CONFIRM "TradeData_SettlementConfirm"

const char* gSqlScriptSettlementConfirmFormat = "\
DROP INDEX \"%s_Index_Validate\";\
DROP INDEX \"%s_Index_UserID\";\
\
DROP TABLE \"%s\" CASCADE CONSTRAINTS;\
CREATE TABLE \"TRADEDATA_SETTLEMENTCONFIRM\"  (\
	\"USERID\"             VARCHAR2(21),\
	\"CONFIRMDATE\"        VARCHAR2(11),\
	\"CONFIRMTIME\"        VARCHAR2(9),\
	\"VAILDATEDATE\"       VARCHAR2(11),\
	\"INSERTDBTIME\"       DATE\
	);\
CREATE INDEX \"%s_Index_UserID\" ON \"%s\" (\
	\"USERID\" ASC\
	);\
CREATE INDEX \"%s_Index_Validate\" ON \"%s\" (\
	\"VALIDATEDATE\" ASC\
	);\
";


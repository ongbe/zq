
@rem modify file version to 1.0.0.mmddhhmm
:copy
cd..
copy Miscellaneous\uniServer\binRelease\SvrTSS.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrTSS.dll
copy Miscellaneous\uniServer\binRelease\SvrRisk.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrRisk.dll
copy Miscellaneous\uniServer\binRelease\SvrTradeExcute.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrTradeExcute.dll
copy Miscellaneous\uniServer\binRelease\SvrStatistics.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrStatistics.dll
copy Miscellaneous\uniServer\binRelease\SvrSettlement.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrSettlement.dll
copy Miscellaneous\uniServer\binReleaseSimulate\SvrOffer.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrOffer.dll
copy Miscellaneous\uniServer\binReleaseSimulate\SvrSimulateTrade.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrSimulateTrade.dll
copy Miscellaneous\uniServer\binRelease\SvrTradeData.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrTradeData.dll
copy Miscellaneous\uniServer\binRelease\SvrTradingStrategy.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrTradingStrategy.dll
copy Miscellaneous\uniServer\binRelease\SvrMsg.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrMsg.dll
copy Miscellaneous\uniServer\binRelease\SvrFinanceProduct.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrFinanceProduct.dll
copy Miscellaneous\uniServer\binRelease\SvrLogin.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\svrLogin.dll
copy Miscellaneous\uniServer\binRelease\SvrUserOrg.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrUserOrg.dll
copy Miscellaneous\uniServer\binRelease\SvrNotifyAndAsk.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrNotifyAndAsk.dll
copy Miscellaneous\uniServer\binRelease\SvrBrokerInfo.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrBrokerInfo.dll 
copy Miscellaneous\uniServer\binRelease\SvrTcp.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrTcp.dll
copy Miscellaneous\uniServer\binRelease\SvrTemplate.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrTemplate.dll
copy Miscellaneous\uniServer\binRelease\SvrPresentation.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrPresentation.dll
copy Miscellaneous\uniServer\binRelease\SvrDBOpr.dll,InstitutionalTrading\publish\部署\模拟版交易服务器\SvrDBOpr.dll
copy Miscellaneous\uniServer\binRelease\uniServer.exe,InstitutionalTrading\publish\部署\模拟版交易服务器\UniServer.exe



goto qutoserver
:qutoserver
copy Miscellaneous\uniServer\binRelease\SvrTcp.dll,InstitutionalTrading\publish\部署\模拟行情服务器\SvrTcp.dll
copy Miscellaneous\uniServer\binRelease\SvrDBOpr.dll,InstitutionalTrading\publish\部署\模拟行情服务器\SvrDBOpr.dll
copy Miscellaneous\uniServer\binReleaseSimulate\SvrQuotServer.dll,InstitutionalTrading\publish\部署\模拟行情服务器\SvrQuotServer.dll
copy Miscellaneous\uniServer\binRelease\uniServer.exe,InstitutionalTrading\publish\部署\模拟行情服务器\UniServer.exe

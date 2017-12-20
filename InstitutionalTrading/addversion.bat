
@rem modify file version to 1.0.0.mmddhhmm
:copy
cd..
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrTSS.dll,InstitutionalTrading\publish\server\SvrTSS.dll, InstitutionalTrading\version.res,,, 
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrRisk.dll,InstitutionalTrading\publish\server\SvrRisk.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrTradeExcute.dll,InstitutionalTrading\publish\server\SvrTradeExcute.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrStatistics.dll,InstitutionalTrading\publish\server\SvrStatistics.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrSettlement.dll,InstitutionalTrading\publish\server\SvrSettlement.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrOffer.dll,InstitutionalTrading\publish\server\SvrOffer.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrTradeData.dll,InstitutionalTrading\publish\server\SvrTradeData.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrTradingStrategy.dll,InstitutionalTrading\publish\server\SvrTradingStrategy.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrMsg.dll,InstitutionalTrading\publish\server\SvrMsg.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrFinanceProduct.dll,InstitutionalTrading\publish\server\SvrFinanceProduct.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrLogin.dll,InstitutionalTrading\publish\server\svrLogin.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrUserOrg.dll,InstitutionalTrading\publish\server\SvrUserOrg.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrNotifyAndAsk.dll,InstitutionalTrading\publish\server\SvrNotifyAndAsk.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrBrokerInfo.dll,InstitutionalTrading\publish\server\SvrBrokerInfo.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrTcp.dll,InstitutionalTrading\publish\server\SvrTcp.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrTemplate.dll,InstitutionalTrading\publish\server\SvrTemplate.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrPresentation.dll,InstitutionalTrading\publish\server\SvrPresentation.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrDBOpr.dll,InstitutionalTrading\publish\server\SvrDBOpr.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\uniServer.exe,InstitutionalTrading\publish\server\qauniServer.exe, InstitutionalTrading\version.res,,,

goto qutoserver
:qutoserver
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrTcp.dll,InstitutionalTrading\publish\quotsever\SvrTcp.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrDBOpr.dll,InstitutionalTrading\publish\quotsever\SvrDBOpr.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\SvrQuotServer.dll,InstitutionalTrading\publish\quotsever\SvrQuotServer.dll, InstitutionalTrading\version.res,,,
InstitutionalTrading\ResHacker.exe -addskip Miscellaneous\uniServer\binRelease\uniServer.exe,InstitutionalTrading\publish\quotsever\qaquotserver.exe, InstitutionalTrading\version.res,,,

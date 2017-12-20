
rem modify file version to 1.0.0.mmddhhmm

rem copy client

rem copy Risk clent
copy Release\cfg.xml,publish\部署\风控终端\cfg.xml
copy Release\CltRisk.exe,publish\部署\风控终端\CltRisk.exe
copy Release\IPControl.dll,publish\部署\风控终端\IPControl.dll
copy Release\CurrencyBox.dll,publish\部署\风控终端\CurrencyBox.dll
copy Release\AnywndComboBox.dll,publish\部署\风控终端\AnywndComboBox.dll
copy Release\SimpleReportControl.dll,publish\部署\风控终端\SimpleReportControl.dll
copy Release\TriTreeControl.dll,publish\部署\风控终端\TriTreeControl.dll
copy Release\WeifenLuo.WinFormsUI.Docking.dll,publish\部署\风控终端\WeifenLuo.WinFormsUI.Docking.dll

rem copy SeverMgr clent
copy Release\cfg.xml,publish\部署\后台管理终端\cfg.xml
copy Release\CltServerMgr.exe,publish\部署\后台管理终端\CltServerMgr.exe
copy Release\IPControl.dll,publish\部署\后台管理终端\IPControl.dll
copy Release\CurrencyBox.dll,publish\部署\后台管理终端\CurrencyBox.dll
copy Release\AnywndComboBox.dll,publish\部署\后台管理终端\AnywndComboBox.dll
copy Release\SimpleReportControl.dll,publish\部署\后台管理终端\SimpleReportControl.dll
copy Release\TriTreeControl.dll,publish\部署\后台管理终端\TriTreeControl.dll
copy Release\WeifenLuo.WinFormsUI.Docking.dll,publish\部署\后台管理终端\WeifenLuo.WinFormsUI.Docking.dll

rem copy Presentation clent
copy Release\cfg.xml,publish\部署\展现终端\cfg.xml
copy Release\CltPresentation.exe,publish\部署\展现终端\CltPresentation.exe
copy Release\AnywndComboBox.dll,publish\部署\展现终端\AnywndComboBox.dll
copy Release\SimpleReportControl.dll,publish\部署\展现终端\SimpleReportControl.dll
copy Release\CFMap.Core.dll,publish\部署\展现终端\CFMap.Core.dll
copy Release\CFMap.WinForm.Control.dll,publish\部署\展现终端\CFMap.WinForm.Control.dll
copy Release\ListPanel.dll,publish\部署\展现终端\ListPanel.dll
copy Release\Microsoft.Office.Interop.Excel.dll,publish\部署\展现终端\Microsoft.Office.Interop.Excel.dll
copy Release\Microsoft.Vbe.Interop.dll,publish\部署\展现终端\Microsoft.Vbe.Interop.dll
copy Release\Office.dll,publish\部署\展现终端\Office.dll
copy Release\SimpleGradientProgressBar.dll,publish\部署\展现终端\SimpleGradientProgressBar.dll
copy Release\SimpleGuage.dll,publish\部署\展现终端\SimpleGuage.dll
copy Release\SimpleFlagControl.dll,publish\部署\展现终端\SimpleFlagControl.dll
copy Release\System.Data.SQLite.DLL,publish\部署\展现终端\System.Data.SQLite.DLL

rem copy strateay manage client
copy Server\RunSingleStrategy\bin\Release\RunSingleStrategy.exe,publish\部署\策略管理终端\SupportFiles\RunSingleStrategy.exe

rem copy server
cd..
copy Miscellaneous\uniServer\binRelease\SvrTSS.dll,InstitutionalTrading\publish\部署\交易服务器\SvrTSS.dll
copy Miscellaneous\uniServer\binRelease\SvrRisk.dll,InstitutionalTrading\publish\部署\交易服务器\SvrRisk.dll
copy Miscellaneous\uniServer\binRelease\SvrTradeExcute.dll,InstitutionalTrading\publish\部署\交易服务器\SvrTradeExcute.dll
copy Miscellaneous\uniServer\binRelease\SvrStatistics.dll,InstitutionalTrading\publish\部署\交易服务器\SvrStatistics.dll
copy Miscellaneous\uniServer\binRelease\SvrSettlement.dll,InstitutionalTrading\publish\部署\交易服务器\SvrSettlement.dll
copy Miscellaneous\uniServer\binRelease\SvrOffer.dll,InstitutionalTrading\publish\部署\交易服务器\SvrOffer.dll
copy Miscellaneous\uniServer\binRelease\SvrTradeData.dll,InstitutionalTrading\publish\部署\交易服务器\SvrTradeData.dll
copy Miscellaneous\uniServer\binRelease\SvrTradingStrategy.dll,InstitutionalTrading\publish\部署\交易服务器\SvrTradingStrategy.dll
copy Miscellaneous\uniServer\binRelease\SvrMsg.dll,InstitutionalTrading\publish\部署\交易服务器\SvrMsg.dll
copy Miscellaneous\uniServer\binRelease\SvrFinanceProduct.dll,InstitutionalTrading\publish\部署\交易服务器\SvrFinanceProduct.dll
copy Miscellaneous\uniServer\binRelease\SvrLogin.dll,InstitutionalTrading\publish\部署\交易服务器\svrLogin.dll
copy Miscellaneous\uniServer\binRelease\SvrUserOrg.dll,InstitutionalTrading\publish\部署\交易服务器\SvrUserOrg.dll
copy Miscellaneous\uniServer\binRelease\SvrNotifyAndAsk.dll,InstitutionalTrading\publish\部署\交易服务器\SvrNotifyAndAsk.dll
copy Miscellaneous\uniServer\binRelease\SvrBrokerInfo.dll,InstitutionalTrading\publish\部署\交易服务器\SvrBrokerInfo.dll 
copy Miscellaneous\uniServer\binRelease\SvrTcp.dll,InstitutionalTrading\publish\部署\交易服务器\SvrTcp.dll
copy Miscellaneous\uniServer\binRelease\SvrTemplate.dll,InstitutionalTrading\publish\部署\交易服务器\SvrTemplate.dll
copy Miscellaneous\uniServer\binRelease\SvrPresentation.dll,InstitutionalTrading\publish\部署\交易服务器\SvrPresentation.dll
copy Miscellaneous\uniServer\binRelease\SvrDBOpr.dll,InstitutionalTrading\publish\部署\交易服务器\SvrDBOpr.dll
copy Miscellaneous\uniServer\binRelease\uniServer.exe,InstitutionalTrading\publish\部署\交易服务器\UniServer.exe

copy Miscellaneous\uniServer\binRelease\SvrTSS.dll,InstitutionalTrading\publish\部署\策略服务器\SvrTSS.dll
copy Miscellaneous\uniServer\binRelease\SvrTradeExcute.dll,InstitutionalTrading\publish\部署\策略服务器\SvrTradeExcute.dll
copy Miscellaneous\uniServer\binRelease\SvrOffer.dll,InstitutionalTrading\publish\部署\策略服务器\SvrOffer.dll
copy Miscellaneous\uniServer\binRelease\SvrTradeData.dll,InstitutionalTrading\publish\部署\策略服务器\SvrTradeData.dll
copy Miscellaneous\uniServer\binRelease\SvrTradingStrategy.dll,InstitutionalTrading\publish\部署\策略服务器\SvrTradingStrategy.dll
copy Miscellaneous\uniServer\binRelease\SvrMsg.dll,InstitutionalTrading\publish\部署\策略服务器\SvrMsg.dll
copy Miscellaneous\uniServer\binRelease\SvrFinanceProduct.dll,InstitutionalTrading\publish\部署\策略服务器\SvrFinanceProduct.dll
copy Miscellaneous\uniServer\binRelease\SvrLogin.dll,InstitutionalTrading\publish\部署\策略服务器\svrLogin.dll
copy Miscellaneous\uniServer\binRelease\SvrStrategy.dll,InstitutionalTrading\publish\部署\策略服务器\SvrStrategy.dll
copy Miscellaneous\uniServer\binRelease\SvrUserOrg.dll,InstitutionalTrading\publish\部署\策略服务器\SvrUserOrg.dll
copy Miscellaneous\uniServer\binRelease\SvrNotifyAndAsk.dll,InstitutionalTrading\publish\部署\策略服务器\SvrNotifyAndAsk.dll
copy Miscellaneous\uniServer\binRelease\SvrBrokerInfo.dll,InstitutionalTrading\publish\部署\策略服务器\SvrBrokerInfo.dll 
copy Miscellaneous\uniServer\binRelease\SvrTcp.dll,InstitutionalTrading\publish\部署\策略服务器\SvrTcp.dll
copy Miscellaneous\uniServer\binRelease\SvrTemplate.dll,InstitutionalTrading\publish\部署\策略服务器\SvrTemplate.dll
copy Miscellaneous\uniServer\binRelease\SvrDBOpr.dll,InstitutionalTrading\publish\部署\策略服务器\SvrDBOpr.dll
copy Miscellaneous\uniServer\binRelease\uniServer.exe,InstitutionalTrading\publish\部署\策略服务器\UniServer.exe
copy Miscellaneous\uniServer\binRelease\SvrRedis.dll,InstitutionalTrading\publish\部署\策略服务器\SvrRedis.dll


copy Miscellaneous\uniServer\binRelease\SvrTcp.dll,InstitutionalTrading\publish\部署\风控服务器\SvrTcp.dll
copy Miscellaneous\uniServer\binRelease\SvrDBOpr.dll,InstitutionalTrading\publish\部署\风控服务器\SvrDBOpr.dll
copy Miscellaneous\uniServer\binRelease\SvrNotifyAndAsk.dll,InstitutionalTrading\publish\部署\风控服务器\SvrNotifyAndAsk.dll
copy Miscellaneous\uniServer\binRelease\SvrBrokerInfo.dll,InstitutionalTrading\publish\部署\风控服务器\SvrBrokerInfo.dll 
copy Miscellaneous\uniServer\binRelease\SvrUserOrg.dll,InstitutionalTrading\publish\部署\风控服务器\SvrUserOrg.dll
copy Miscellaneous\uniServer\binRelease\SvrFinanceProduct.dll,InstitutionalTrading\publish\部署\风控服务器\SvrFinanceProduct.dll
copy Miscellaneous\uniServer\binRelease\SvrMsg.dll,InstitutionalTrading\publish\部署\风控服务器\SvrMsg.dll
copy Miscellaneous\uniServer\binRelease\SvrTradeData.dll,InstitutionalTrading\publish\部署\风控服务器\SvrTradeData.dll
copy Miscellaneous\uniServer\binRelease\SvrRisk.dll,InstitutionalTrading\publish\部署\风控服务器\SvrRisk.dll
copy Miscellaneous\uniServer\binRelease\SvrLogin.dll,InstitutionalTrading\publish\部署\风控服务器\svrLogin.dll
copy Miscellaneous\uniServer\binRelease\uniServer.exe,InstitutionalTrading\publish\部署\风控服务器\UniServer.exe



rem goto qutoserver
rem :qutoserver
copy Miscellaneous\uniServer\binRelease\SvrTcp.dll,InstitutionalTrading\publish\部署\行情服务器\SvrTcp.dll
copy Miscellaneous\uniServer\binRelease\SvrDBOpr.dll,InstitutionalTrading\publish\部署\行情服务器\SvrDBOpr.dll
copy Miscellaneous\uniServer\binRelease\SvrQuotServer.dll,InstitutionalTrading\publish\部署\行情服务器\SvrQuotServer.dll
copy Miscellaneous\uniServer\binRelease\uniServer.exe,InstitutionalTrading\publish\部署\行情服务器\UniServer.exe

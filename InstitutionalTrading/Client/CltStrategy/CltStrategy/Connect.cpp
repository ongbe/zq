// This is the main DLL file.

#include "stdafx.h"
#include "Connect.h"
#include "ToolsEx.h"
#include "CreateIndicatiorForm.h"
#include "CreatStrategy.h"
#include "FormUserLogin.h"
#include "StrategyPublish.h"
#include "DownLoadForm.h"
#include "StrategyManagement.h"
#include "StrategyPlan.h"
#include "StrategyExport.h"
#include "FileCover.h"
#include "..\StrategyTester\TestRecordForm.h"

using namespace StrategyTester;
using namespace System::Runtime::InteropServices;


void CltStrategy::Connect::OnAddInsUpdate(System::Array ^%custom)
{
}

void CltStrategy::Connect::OnBeginShutdown(System::Array ^%custom)
{
	//Command^ commandObj = _applicationObject->Commands->Item(
	//	"CltStrategy.Connect.CreateStrategy",-1);
	//if(commandObj != nullptr)
	//	commandObj->Delete();

	//CommandBar^ menuObj = ((Microsoft::VisualStudio::CommandBars::CommandBars^)_applicationObject->CommandBars)["策略开发"];
	//if (menuObj != nullptr)
	//{
	//	_applicationObject->Commands->RemoveCommandBar(menuObj);
	//}
	//


	//CommandBar^ menuObjst = ((Microsoft::VisualStudio::CommandBars::CommandBars^)_applicationObject->CommandBars)["CreateStrategy"];

	//if (menuObjst != nullptr)
	//{
	//	_applicationObject->Commands->RemoveCommandBar(menuObjst);
	//}


	//Command^ commandObjIn = _applicationObject->Commands->Item(
	//	"CltStrategy.Connect.CreateIndicator",-1);
	//if(commandObjIn != nullptr)
	//	commandObjIn->Delete();
	//CommandBar^ menuObjin = ((Microsoft::VisualStudio::CommandBars::CommandBars^)_applicationObject->CommandBars)["CreateIndicator"];

	//if (menuObjin != nullptr)
	//{
	//	_applicationObject->Commands->RemoveCommandBar(menuObjin);
	//}

	
}

void CltStrategy::Connect::OnConnection(System::Object ^Application, ext_ConnectMode ConnectMode, System::Object ^AddInInst, System::Array ^%custom)
{
    _applicationObject = dynamic_cast<DTE2^>(Application);
    _addInInstance = dynamic_cast<AddIn^>(AddInInst);
	std::string strDllFile = Tools::String2string(_addInInstance->SatelliteDllPath);
	_SetAddinDllFileName(strDllFile);

	DWORD dwRet = 0;
	char csIniFile[1024] = {0};
	char csTemp[1024] = {0};

	strncpy(csIniFile, _GetAddinDllFileName().c_str(), sizeof(csIniFile));
	csIniFile[strlen(csIniFile)-4] = 0;
	strcat(csIniFile, ".ini");

	ZeroMemory(csTemp, sizeof(csTemp));
	dwRet = ::GetPrivateProfileString("RunPath", "StrategyPath", "", csTemp, sizeof(csTemp)-1, csIniFile);
	if(dwRet>0 && csTemp[dwRet-1]!='\\')
		strcat(csTemp, "\\");
	_SetWorkPath(std::string(csTemp));
	dwRet = ::GetPrivateProfileString("RunPath", "SupportFilesPath", "", csTemp, sizeof(csTemp)-1, csIniFile);
	if(dwRet>0 && csTemp[dwRet-1]!='\\')
		strcat(csTemp, "\\");
	_SetSupportFilesPath(std::string(csTemp));
	_GetPopMsg();

	m_pDataService->ResetDTEObject(Marshal::GetIUnknownForObject(_applicationObject).ToPointer());

    if(ConnectMode == ext_ConnectMode::ext_cm_UISetup)
    {
		m_pDataService->Init();
		m_pDataService->Reload(_GetProjectsPath());

        array< Object^ >^ contextGUIDs = gcnew array< Object^ >(0);
        Commands2 ^commands = dynamic_cast<Commands2^>(_applicationObject->Commands);

		String^ strSolutionFile = gcnew String(_GetProjectsPath().c_str());
		strSolutionFile += "策略平台.sln";

		//设置solution窗口
		EnvDTE90::Solution3^ soln = (EnvDTE90::Solution3^)_applicationObject->Solution;
		bool bFoundSoln = File::Exists(strSolutionFile);
		if(bFoundSoln) {
			soln->Open(strSolutionFile);
		}
		else {
			soln->Create(gcnew String(_GetProjectsPath().c_str()), "策略平台");
			EnvDTE::Project^ clProj = soln->AddSolutionFolder( "策略");
			EnvDTE::Project^ indiProj=soln->AddSolutionFolder( "指标");
			SolutionFolder^ SF = (SolutionFolder^)indiProj->Object;
			SF->AddSolutionFolder("价格指标");
			SF->AddSolutionFolder("交易量指标");
			SF->AddSolutionFolder("指标样例");
			soln->AddSolutionFolder( "日志");
			soln->AddSolutionFolder( "结果");
		}

		String^ menuNameDev = "策略开发";
		String^ menuNameManage = "策略管理";
		
		try
		{
			Microsoft::VisualStudio::CommandBars::CommandBar^ cmbBar = 
					((Microsoft::VisualStudio::CommandBars::CommandBars^)
					  _applicationObject->CommandBars)["MenuBar"];

			//Microsoft::VisualStudio::CommandBars::CommandBarControls^ cmbBarCtrls = cmbBar->Controls;
			//for each(Microsoft::VisualStudio::CommandBars::CommandBarControl^ cmbBarItem in cmbBarCtrls) {
			//	if(cmbBarItem->Caption == "策略开发") 
			//		cmbBarItem->Visible = false;
			//}

			//Add a command to the Commands collection:
			// Command command = commands.AddNamedCommand2(_addInInstance, "create Strategy", "create Strategy", "create a Strategy", false, 0, ref contextGUIDS, (int)vsCommandStatus.vsCommandStatusSupported + (int)vsCommandStatus.vsCommandStatusEnabled, (int)vsCommandStyle.vsCommandStylePictAndText, vsCommandControlType.vsCommandControlTypeMRUButton);
			Command^ commandStrategy = commands->AddNamedCommand2(_addInInstance, "CreateStrategy", "创建策略",
				"创建策略", true, 127, contextGUIDs,
				(int)vsCommandStatus::vsCommandStatusSupported + (int)vsCommandStatus::vsCommandStatusEnabled,
				(int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);
			Command^ commandIndicator = commands->AddNamedCommand2(_addInInstance, "CreateIndicator", "创建指标",
				"创建指标", true, 127, contextGUIDs,
				(int)vsCommandStatus::vsCommandStatusSupported + (int)vsCommandStatus::vsCommandStatusEnabled,
				(int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);
			Command^ commandUserLogin = commands->AddNamedCommand2(_addInInstance, "UserLogin", "策略服务器登录",
				"策略服务器登录", true, 127, contextGUIDs,
				(int)vsCommandStatus::vsCommandStatusSupported + (int)vsCommandStatus::vsCommandStatusEnabled,
				(int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);
			Command^ commandUpload = commands->AddNamedCommand2(_addInInstance, "Upload", "上传策略和指标",
				"上传策略和指标", true, 127, contextGUIDs,
				(int)vsCommandStatus::vsCommandStatusSupported + (int)vsCommandStatus::vsCommandStatusEnabled,
				(int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);
			Command^ commandDownload = commands->AddNamedCommand2(_addInInstance, "Download", "下载策略和指标",
				"下载策略和指标", true, 127, contextGUIDs,
				(int)vsCommandStatus::vsCommandStatusSupported + (int)vsCommandStatus::vsCommandStatusEnabled,
				(int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);
			Command^ commandStrategyManage = commands->AddNamedCommand2(_addInInstance, "StrategyManage", "策略管理",
				"策略管理", true, 127, contextGUIDs,
				(int)vsCommandStatus::vsCommandStatusSupported + (int)vsCommandStatus::vsCommandStatusEnabled,
				(int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);
			Command^ commandStrategyPlan = commands->AddNamedCommand2(_addInInstance, "StrategyPlan", "策略实例管理",
				"策略实例管理", true, 127, contextGUIDs,
				(int)vsCommandStatus::vsCommandStatusSupported + (int)vsCommandStatus::vsCommandStatusEnabled,
				(int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);
			Command^ commandStrategyImport = commands->AddNamedCommand2(_addInInstance, "StrategyImport", "策略导入",
				"策略导入", true, 127, contextGUIDs,
				(int)vsCommandStatus::vsCommandStatusSupported + (int)vsCommandStatus::vsCommandStatusEnabled,
				(int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);
			Command^ commandStrategyExport = commands->AddNamedCommand2(_addInInstance, "StrategyExport", "策略导出",
				"策略导出", true, 127, contextGUIDs,
				(int)vsCommandStatus::vsCommandStatusSupported + (int)vsCommandStatus::vsCommandStatusEnabled,
				(int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);
			Command^ commandStrategyTest = commands->AddNamedCommand2(_addInInstance, "StrategyTest", "策略测试",
				"策略测试", true, 127, contextGUIDs,
				(int)vsCommandStatus::vsCommandStatusSupported + (int)vsCommandStatus::vsCommandStatusEnabled,
				(int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);
            Command^ commandTestRecord = commands->AddNamedCommand2(_addInInstance, "TestRecord", "策略测试结果",
                "策略测试结果", true, 127, contextGUIDs,
                (int)vsCommandStatus::vsCommandStatusSupported + (int)vsCommandStatus::vsCommandStatusEnabled,
                (int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);

			CommandBar^ menuObj = (CommandBar^)_applicationObject->Commands->AddCommandBar(menuNameDev, vsCommandBarType::vsCommandBarTypeMenu, ((Microsoft::VisualStudio::CommandBars::CommandBars^)_applicationObject->CommandBars)["MenuBar"],
				17);
			CommandBar^ menuObjManage = (CommandBar^)_applicationObject->Commands->AddCommandBar(menuNameManage, vsCommandBarType::vsCommandBarTypeMenu, ((Microsoft::VisualStudio::CommandBars::CommandBars^)_applicationObject->CommandBars)["MenuBar"],
				17);
			
			CommandBarButton^ buttonObjStrategy,^buttonObjIndicator, ^buttonObjUserLogin,^buttonObjUpload,^buttonObjDownload,^buttonObjStrategyManage,^buttonObjStrategyPlan,^buttonObjStrategyImport,^buttonObjStrategyExport,^buttonObjStrategyTsest,^buttonObjTestRecord;
			buttonObjStrategy = (CommandBarButton^)commandStrategy->AddControl(menuObj, menuObj->Controls->Count + 1);
			buttonObjIndicator = (CommandBarButton^)commandIndicator->AddControl(menuObj, menuObj->Controls->Count + 1);
			buttonObjStrategyTsest = (CommandBarButton^)commandStrategyTest->AddControl(menuObj, menuObj->Controls->Count + 1);

			buttonObjUserLogin = (CommandBarButton^)commandUserLogin->AddControl(menuObjManage, menuObjManage->Controls->Count + 1);
			buttonObjUpload = (CommandBarButton^)commandUpload->AddControl(menuObjManage, menuObjManage->Controls->Count + 1);
			buttonObjDownload = (CommandBarButton^)commandDownload->AddControl(menuObjManage, menuObjManage->Controls->Count + 1);
			buttonObjStrategyManage = (CommandBarButton^)commandStrategyManage->AddControl(menuObjManage, menuObjManage->Controls->Count + 1);
			buttonObjStrategyPlan = (CommandBarButton^)commandStrategyPlan->AddControl(menuObjManage, menuObjManage->Controls->Count + 1);
			buttonObjStrategyImport = (CommandBarButton^)commandStrategyImport->AddControl(menuObjManage, menuObjManage->Controls->Count + 1);
			buttonObjStrategyExport = (CommandBarButton^)commandStrategyExport->AddControl(menuObjManage, menuObjManage->Controls->Count + 1);
            buttonObjTestRecord = (CommandBarButton^)commandTestRecord->AddControl(menuObjManage, menuObjManage->Controls->Count + 1);

			buttonObjStrategy->Style = MsoButtonStyle::msoButtonIcon;
			buttonObjIndicator->Style = MsoButtonStyle::msoButtonIcon;
			buttonObjUserLogin->Style = MsoButtonStyle::msoButtonIcon;
			buttonObjUpload->Style = MsoButtonStyle::msoButtonIcon;
			buttonObjDownload->Style = MsoButtonStyle::msoButtonIcon;
			buttonObjStrategyManage->Style = MsoButtonStyle::msoButtonIcon;
			buttonObjStrategyPlan->Style = MsoButtonStyle::msoButtonIcon;
			buttonObjStrategyImport->Style = MsoButtonStyle::msoButtonIcon;
			buttonObjStrategyExport->Style = MsoButtonStyle::msoButtonIcon;
			buttonObjStrategyTsest->Style = MsoButtonStyle::msoButtonIcon;
            buttonObjTestRecord->Style = MsoButtonStyle::msoButtonIcon;
			//将子菜单加入Project的右键菜单
			CommandBar^ projBar = ((Microsoft::VisualStudio::CommandBars::CommandBars^)_applicationObject->CommandBars)["Project"];
			/*commandStrategy->AddControl(projBar, 1);
			commandIndicator->AddControl(projBar, 2);*/
			commandStrategyTest->AddControl(projBar, 1);

			/*commandUserLogin->AddControl(projBar, 3);
			commandUpload->AddControl(projBar, 4);
			commandDownload->AddControl(projBar, 5);*/

			//将子菜单加入Folder的右键菜单
			CommandBar^ folderBar = ((Microsoft::VisualStudio::CommandBars::CommandBars^)_applicationObject->CommandBars)["Folder"];
			/*commandStrategy->AddControl(folderBar, 1);
			commandIndicator->AddControl(folderBar, 2);*/
			commandStrategyTest->AddControl(folderBar, 1);
			/*commandUserLogin->AddControl(folderBar, 3);
			commandUpload->AddControl(folderBar, 4);
			commandDownload->AddControl(folderBar,5);*/
		
		}
		catch (System::ArgumentException ^argumentException)
		{
			//If we are here, then the exception is probably because a command with that name
			//  already exists. If so there is no need to recreate the command and we can 
			//  safely ignore the exception.
		}


        String ^toolsMenuName;

        try
        {
            //If you would like to move the command to a different menu, change the word "Tools" to the 
            //  English version of the menu. This code will take the culture, append on the name of the menu
            //  then add the command to that menu. You can find a list of all the top-level menus in the file
            //  CommandBar.resx.
            ResourceManager ^resourceManager = gcnew ResourceManager("CltStrategy.CommandBar", Assembly::GetExecutingAssembly());
            CultureInfo ^cultureInfo = gcnew System::Globalization::CultureInfo(_applicationObject->LocaleID);
			if(cultureInfo->TwoLetterISOLanguageName == "zh")
			{
				CultureInfo ^parentCultureInfo = cultureInfo->Parent;
				toolsMenuName = resourceManager->GetString(String::Concat(parentCultureInfo->Name, "Tools"));
			}
			else
			{
				toolsMenuName = resourceManager->GetString(String::Concat(cultureInfo->TwoLetterISOLanguageName, "Tools"));
			}
        }
        catch(...)
        {
            //We tried to find a localized version of the word Tools, but one was not found.
            //  Default to the en-US word, which may work for the current culture.
            toolsMenuName = "Tools";
        }

        //Place the command on the tools menu.
        //Find the MenuBar command bar, which is the top-level command bar holding all the main menu items:
        _CommandBars ^commandBars = dynamic_cast<CommandBars^>(_applicationObject->CommandBars);
        CommandBar ^menuBarCommandBar = dynamic_cast<CommandBar^>(commandBars["MenuBar"]);

        //Find the Tools command bar on the MenuBar command bar:
        CommandBarControl ^toolsControl = menuBarCommandBar->Controls[toolsMenuName];
        CommandBarPopup ^toolsPopup = dynamic_cast<CommandBarPopup^>(toolsControl);

        //This try/catch block can be duplicated if you wish to add multiple commands to be handled by your Add-in,
        //  just make sure you also update the QueryStatus/Exec method to include the new command names.
        try
        {	
            //Add a command to the Commands collection:
            Command ^command = commands->AddNamedCommand2(_addInInstance, "CltStrategy", "CltStrategy", "Executes the command for CltStrategy", true, 59, contextGUIDs, (int)vsCommandStatus::vsCommandStatusSupported+(int)vsCommandStatus::vsCommandStatusEnabled, (int)vsCommandStyle::vsCommandStylePictAndText, vsCommandControlType::vsCommandControlTypeButton);

            //Add a control for the command to the tools menu:
            if((command) && (toolsPopup))
            {
                command->AddControl(toolsPopup->CommandBar, 1);
            }
        }
		catch(System::ArgumentException ^argumentException)
        {
			//If we are here, then the exception is probably because a command with that name
			//  already exists. If so there is no need to recreate the command and we can 
            //  safely ignore the exception.
        }
    }
}

void CltStrategy::Connect::OnStartupComplete(System::Array ^%custom)
{
}

void CltStrategy::Connect::OnDisconnection(ext_DisconnectMode removeMode, System::Array ^%custom)
{
	try
	{
		m_pDataService->SaveList(_GetProjectsPath());
		if(removeMode == Extensibility::ext_DisconnectMode::ext_dm_HostShutdown
			|| removeMode == Extensibility::ext_DisconnectMode::ext_dm_UserClosed 
			|| removeMode == Extensibility::ext_DisconnectMode::ext_dm_SolutionClosed )
		{
			Command^ commandObj = _applicationObject->Commands->Item(
				"CltStrategy.Connect.CreateStrategy",-1);
			if(commandObj != nullptr)
				commandObj->Delete();

			CommandBar^ menuObj = ((Microsoft::VisualStudio::CommandBars::CommandBars^)_applicationObject->CommandBars)["策略开发"];
			if (menuObj != nullptr)
			{
				_applicationObject->Commands->RemoveCommandBar(menuObj);
			}
			CommandBar^ menuObjManage = ((Microsoft::VisualStudio::CommandBars::CommandBars^)_applicationObject->CommandBars)["策略管理"];
			if (menuObjManage != nullptr)
			{
				_applicationObject->Commands->RemoveCommandBar(menuObjManage);
			}

			//CommandBar^ menuObjst = ((Microsoft::VisualStudio::CommandBars::CommandBars^)_applicationObject->CommandBars)["CreateStrategy"];

			//if (menuObjst != nullptr)
			//{
			//	_applicationObject->Commands->RemoveCommandBar(menuObjst);
			//}

			//Command^ commandObjS = _applicationObject->Commands->Item(
			//	"CltStrategy.Connect.CreateStrategy",-1);
			//if(commandObjS != nullptr)
			//	commandObjS->Delete();

			//CommandBar^ menuObjin = ((Microsoft::VisualStudio::CommandBars::CommandBars^)_applicationObject->CommandBars)["CreateIndicator"];
		
			//if (menuObjin != nullptr)
			//{
			//	_applicationObject->Commands->RemoveCommandBar(menuObjin);
			//}

			//Command^ commandObjIn = _applicationObject->Commands->Item(
			//	"CltStrategy.Connect.CreateIndicator",-1);
			//if(commandObjIn != nullptr)
			//	commandObjIn->Delete();

			try {
			RemoveComand("CreateStrategy");
			RemoveComand("CreateIndicator");
			RemoveComand("UserLogin");
			RemoveComand("Upload");
			RemoveComand("Download");
			RemoveComand("StrategyManage");
			RemoveComand("StrategyPlan");
			RemoveComand("StrategyImport");
			RemoveComand("StrategyExport");
			RemoveComand("StrategyTest");
            RemoveComand("TestRecord");
			} catch(Exception^ e) {}
			
			
		}
	}
	catch (System::ArgumentException ^argumentException)
	{


	}
}

void CltStrategy::Connect::RemoveComand(String ^CmdName)
{
	CommandBar^ menuObj = ((Microsoft::VisualStudio::CommandBars::CommandBars^)_applicationObject->CommandBars)[CmdName];

	if (menuObj != nullptr)
	{
		_applicationObject->Commands->RemoveCommandBar(menuObj);
	}
	String ^ Cmd = "CltStrategy.Connect." + CmdName;
	Command^ command = _applicationObject->Commands->Item(Cmd,-1);
	if(command != nullptr)
		command->Delete();
}
void CltStrategy::Connect::Exec(String ^CmdName, vsCommandExecOption ExecuteOption, Object ^%VariantIn, Object ^%VariantOut, bool %handled)
{
    handled = false;
    if(ExecuteOption == vsCommandExecOption::vsCommandExecOptionDoDefault)
    {
        if(!CmdName->CompareTo("CltStrategy.Connect.CltStrategy"))
        {
            handled = true;
            return;
        }
		if(!CmdName->CompareTo("CltStrategy.Connect.CreateStrategy"))
		{
			CreatStrategy^ dlg = gcnew CreatStrategy();
			dlg->setApplicationObject(_applicationObject);
			dlg->ShowDialog();

			handled = true;
			return;
		}
		if(!CmdName->CompareTo("CltStrategy.Connect.CreateIndicator"))
		{
			CreateIndicatiorForm^ dlg = gcnew CreateIndicatiorForm();
			dlg->setApplicationObject(_applicationObject);
			dlg->ShowDialog();
			/*if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
			{

			}*/


			handled = true;
			return;
		}
		if(CmdName->CompareTo("CltStrategy.Connect.UserLogin")==0)
		{
			 if(!m_pDataService->IsConnectedTrader()) 
			 {
				 MessageBox::Show("与服务器连接未建立，不能登录！", "错误", MessageBoxButtons::OK);
			 }
			 else 
			 {
				 FormUserLogin^ dlg = gcnew FormUserLogin();
				 dlg->ShowDialog();
			 }

			handled = true;
			return;
		}
		if(CmdName->CompareTo("CltStrategy.Connect.Upload")==0)
		{
			if(m_pDataService->GetLoginTraderStatus() != CDataManageEx::conLogon) 
			{
				MessageBox::Show("未登录到策略服务器。", "错误", MessageBoxButtons::OK);
			}
			else 
			{
				StrategyPublish^ dlg = gcnew StrategyPublish();
				dlg->setApplicationObject(_applicationObject);
				dlg->ShowDialog();
			}

			handled = true;
			return;
		}
		if(CmdName->CompareTo("CltStrategy.Connect.Download")==0)
		{
			if(m_pDataService->GetLoginTraderStatus() != CDataManageEx::conLogon) 
			{
				MessageBox::Show("未登录到策略服务器。", "错误", MessageBoxButtons::OK);
			}
			else 
			{
				DownLoadForm^ dlg = gcnew DownLoadForm();
				dlg->ShowDialog();
			}

			handled = true;
			return;
		}
		if(CmdName->CompareTo("CltStrategy.Connect.StrategyManage")==0)
		{
			if(m_pDataService->GetLoginTraderStatus() != CDataManageEx::conLogon) 
			{
				MessageBox::Show("未登录到策略服务器。", "错误", MessageBoxButtons::OK);
			}
			else 
			{
				StrategyManagement^ dlg = gcnew StrategyManagement();
				dlg->ShowDialog();
			}

			handled = true;
			return;
		}
		if(CmdName->CompareTo("CltStrategy.Connect.StrategyPlan")==0)
		{
			if(m_pDataService->GetLoginTraderStatus() != CDataManageEx::conLogon) 
			{
				MessageBox::Show("未登录到策略服务器。", "错误", MessageBoxButtons::OK);
			}
			else 
			{
				StrategyPlan^ dlg = gcnew StrategyPlan();
				dlg->ShowDialog();
			}

			handled = true;
			return;
		}
		if(CmdName->CompareTo("CltStrategy.Connect.StrategyImport")==0)
		{
			System::Windows::Forms::OpenFileDialog^ dlg = gcnew OpenFileDialog();
			dlg->Title ="请选择要导入的策略Zip文件";
			dlg->Filter = "zip压缩文件(*.zip)|*.zip";
			if(dlg->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				ImportStrategiesAndIndicators(Tools::String2string(dlg->FileName));
			}

			handled = true;
			return;
		}
		if(CmdName->CompareTo("CltStrategy.Connect.StrategyExport")==0)
		{
			StrategyExport^ dlg = gcnew StrategyExport();
			dlg->ShowDialog();

			handled = true;
			return;
		}
		if(CmdName->CompareTo("CltStrategy.Connect.StrategyTest")==0)
		{
			//启动StrategyTester
			String^ strStrategyName;
			String^ strFilePath;
			bool bSelect = false;
			for each(SelectedItem^ item in _applicationObject->SelectedItems) {
				Debug::WriteLine(item->Project->default);
				Debug::WriteLine(item->Project->FileName);
				Debug::WriteLine(item->Project->FullName);
				Debug::WriteLine(item->Project->UniqueName);
				strStrategyName = item->Project->default;
				strFilePath = item->Project->FileName;
				bSelect = true;
				break;
			}

			if(bSelect && m_pDataService!=NULL) {
				std::string strStrategyID = Tools::String2string(strStrategyName);
				std::string strPath = Tools::String2string(strFilePath);
				CStrategyDataEx strategy;
				int i=0;
				char csTemp[1024];
				memset(csTemp, 0, sizeof(csTemp));
				strncpy(csTemp, strPath.c_str(), sizeof(csTemp)-1);
				i = strlen(csTemp)-1;
				while(i>0) {
					if(csTemp[i]=='\\') {
						csTemp[i+1]=0;
						break;
					}
					i--;
				}
				strPath = csTemp;
				if(!m_pDataService->GetStrategy(strStrategyID, strategy)) {
					MessageBox::Show("选择的项目不是策略，不能进行测试。", "错误", MessageBoxButtons::OK);
					handled = true;
					return;
				}
				else {
					RunStategyTest(1, strPath, strStrategyID);
				}
			}

			handled = true;
			return;
		}
        if(CmdName->CompareTo("CltStrategy.Connect.TestRecord")==0)
        {

            RunStategyTest(2, std::string(""), std::string(""));


            handled = true;
            return;
        }
    }
}

void CltStrategy::Connect::QueryStatus(String ^CmdName, vsCommandStatusTextWanted NeededText, vsCommandStatus %StatusOption, Object ^%CommandText)
{
    if(NeededText == vsCommandStatusTextWanted::vsCommandStatusTextWantedNone)
    {
        if(!CmdName->CompareTo("CltStrategy.Connect.CltStrategy"))
        {
            StatusOption = (vsCommandStatus)(vsCommandStatus::vsCommandStatusSupported+vsCommandStatus::vsCommandStatusEnabled);
            return;
        }
		if(!CmdName->CompareTo("CltStrategy.Connect.CreateStrategy"))
		{
			 StatusOption = (vsCommandStatus)(vsCommandStatus::vsCommandStatusSupported+vsCommandStatus::vsCommandStatusEnabled);
			return;
		}
		if(!CmdName->CompareTo("CltStrategy.Connect.CreateIndicator"))
		{
			 StatusOption = (vsCommandStatus)(vsCommandStatus::vsCommandStatusSupported+vsCommandStatus::vsCommandStatusEnabled);
			return;
		}
		if(!CmdName->CompareTo("CltStrategy.Connect.UserLogin"))
		{
			StatusOption = (vsCommandStatus)(vsCommandStatus::vsCommandStatusSupported+vsCommandStatus::vsCommandStatusEnabled);
			return;
		}
		if(!CmdName->CompareTo("CltStrategy.Connect.Upload"))
		{
			StatusOption = (vsCommandStatus)(vsCommandStatus::vsCommandStatusSupported+vsCommandStatus::vsCommandStatusEnabled);
			return;
		}
		if(!CmdName->CompareTo("CltStrategy.Connect.Download"))
		{
			StatusOption = (vsCommandStatus)(vsCommandStatus::vsCommandStatusSupported+vsCommandStatus::vsCommandStatusEnabled);
			return;
		}
		if(!CmdName->CompareTo("CltStrategy.Connect.StrategyManage"))
		{
			StatusOption = (vsCommandStatus)(vsCommandStatus::vsCommandStatusSupported+vsCommandStatus::vsCommandStatusEnabled);
			return;
		}
		if(!CmdName->CompareTo("CltStrategy.Connect.StrategyPlan"))
		{
			StatusOption = (vsCommandStatus)(vsCommandStatus::vsCommandStatusSupported+vsCommandStatus::vsCommandStatusEnabled);
			return;
		}
		if(!CmdName->CompareTo("CltStrategy.Connect.StrategyImport"))
		{
			StatusOption = (vsCommandStatus)(vsCommandStatus::vsCommandStatusSupported+vsCommandStatus::vsCommandStatusEnabled);
			return;
		}
		if(!CmdName->CompareTo("CltStrategy.Connect.StrategyExport"))
		{
			StatusOption = (vsCommandStatus)(vsCommandStatus::vsCommandStatusSupported+vsCommandStatus::vsCommandStatusEnabled);
			return;
		}
		if(!CmdName->CompareTo("CltStrategy.Connect.StrategyTest"))
		{
			StatusOption = (vsCommandStatus)(vsCommandStatus::vsCommandStatusSupported+vsCommandStatus::vsCommandStatusEnabled);
			return;
		}
        if(!CmdName->CompareTo("CltStrategy.Connect.TestRecord"))
        {
            StatusOption = (vsCommandStatus)(vsCommandStatus::vsCommandStatusSupported+vsCommandStatus::vsCommandStatusEnabled);
            return;
        }
    }
}

Project^ CltStrategy::Connect::GetProjectByName(EnvDTE90::Solution3^ soln, String^ projName) 
{
	Project^ proj = nullptr;
	for(int i = 0; i<soln->Projects->Count; i++) {
		proj = soln->Projects->Item(i+1);
		if(proj->default == projName) {
			return proj;
		}
		else {
			for(int i = 0; i<proj->ProjectItems->Count; i++) {
				ProjectItem^ proj2 = proj->ProjectItems->Item(i+1);
				if(proj2->default == projName) {
					return (Project^)proj2->Object;
				}
			}
		}
	}
	return nullptr;
}

bool CltStrategy::Connect::ImportStrategiesAndIndicators(const std::string& strImportFile)
{
	std::set<std::string> setIndicator;		//策略名
	std::set<std::string> setStrategy;		//指标名
	std::set<std::string> setProjects;		//项目名
	std::set<std::string> setProjectsTemp;	//项目名
	std::string strImportPath(_GetTempPath() + "testExtra\\");

	CToolsEx::DeleteAllFolder(strImportPath);
	::CreateDirectory(strImportPath.c_str(), NULL);
	CToolsEx::UnZip_File(_GetSupportFilesPath(), strImportPath, 
			strImportFile, true);

	try { 
		StreamReader^ sr = gcnew StreamReader(
				gcnew String((strImportPath + "indicatorsExport.txt").c_str()));
		String^ line = nullptr;
		while(line = sr->ReadLine()) {
			if(!String::IsNullOrEmpty(line)) {
				setIndicator.insert(Tools::String2string(line));
				setProjects.insert(Tools::String2string(line));
				setProjectsTemp.insert(Tools::String2string(line));
			}
		}
		sr->Close();
	}
	catch (Exception^ e) {
	}

	try { 
		StreamReader^ sr = gcnew StreamReader(
				gcnew String((strImportPath + "StrategiesExport.txt").c_str()));
		String^ line = nullptr;
		while(line = sr->ReadLine()) {
			if(!String::IsNullOrEmpty(line)) {
				setStrategy.insert(Tools::String2string(line));
				setProjects.insert(Tools::String2string(line));
				setProjectsTemp.insert(Tools::String2string(line));
			}
		}
		sr->Close();
	}
	catch (Exception^ e) {
	}

	std::set<std::string>::iterator itIndi;
	std::set<std::string>::iterator itStra;
	std::string strRet;
	for(itIndi = setIndicator.begin(); itIndi != setIndicator.end(); itIndi++) {
		CToolsEx::UnZip_Project(_GetSupportFilesPath(), strImportPath, 
				*itIndi, strRet, true);
	}
	for(itStra = setStrategy.begin(); itStra != setStrategy.end(); itStra++) {
		CToolsEx::UnZip_Project(_GetSupportFilesPath(), strImportPath, 
				*itStra, strRet, true);
	}

	std::set<std::string> setFilesTemp;
	std::set<std::string> setFiles;

	m_pDataService->GetProjectFiles(strImportPath, setProjectsTemp, setFilesTemp);
	m_pDataService->GetProjectFiles(_GetProjectsPath(), setProjects, setFiles);

	std::set<std::string> setSetionFiles;
	CToolsEx::GetIntersection(setFiles, setFiles, setSetionFiles);

	// 这里需要显示被覆盖的文件，应弹出一个对话框，
	// setSetionFiles里包含所有的别覆盖的文件

	if (setSetionFiles.size() > 0 )
	{ 
		string strFileName="";
		std::set<std::string>::iterator itFile = setSetionFiles.begin();
		while (itFile!= setSetionFiles.end())
		{
			strFileName += *itFile + "\r\n";
			itFile++;
		}

		FileCover^ dlg = gcnew FileCover(strFileName);

		if (System::Windows::Forms::DialogResult::Yes != dlg->ShowDialog())
		{
			return false;
		}

	}



	// 用户选择继续导入后

	CToolsEx::UnZip_File(_GetSupportFilesPath(), _GetProjectsPath(), 
		strImportFile, true);

	for(itIndi = setIndicator.begin(); itIndi != setIndicator.end(); itIndi++) {
		CToolsEx::UnZip_Project(_GetSupportFilesPath(), _GetProjectsPath(), 
			*itIndi, strRet, true);

		try {
			std::string strPath(_GetProjectsPath());
			strPath += *itIndi;
			strPath += "\\params.xml";

			CIndicatorDataEx indicator;
			
			indicator.ImportFromXmlFile(strPath);
			indicator.SetIndicatorPrivate(1);
			//indicator.SetOwner(rsp.strTraderName);
			m_pDataService->AddIndcator(indicator);

			String^ projectFile = (gcnew String(_GetProjectsPath().c_str())) 
					+ (gcnew String((*itIndi).c_str())) + "\\"
					+ (gcnew String((*itIndi).c_str())) + ".csproj";
			EnvDTE90::Solution3^ soln = (EnvDTE90::Solution3^)
				_applicationObject->Solution;
			Project^ proj = GetProjectByName(soln, 
				gcnew String(indicator.GetIndicatorFile().c_str()));
			if(proj != nullptr) {
				SolutionFolder^ SF = (SolutionFolder^)proj->Object;
				SF->AddFromFile(projectFile);
			}
		}
		catch(Exception^ e) {
		}
	}
	for(itStra = setStrategy.begin(); itStra != setStrategy.end(); itStra++) {
		CToolsEx::UnZip_Project(_GetSupportFilesPath(), _GetProjectsPath(), 
			*itStra, strRet, true);

		try {
			std::string strPath(_GetProjectsPath());
			strPath += *itStra;
			strPath += "\\params.xml";

			CStrategyDataEx strategy;
			
			strategy.ImportFromXmlFile(strPath);
			strategy.SetStrategyPrivate(1);
			//strategy.SetOwner(rsp.strTraderName);
			m_pDataService->AddStrategy(strategy);

			String^ projectFile = (gcnew String(_GetProjectsPath().c_str())) 
					+ (gcnew String((*itStra).c_str())) + "\\"
					+ (gcnew String((*itStra).c_str())) + ".csproj";
			EnvDTE90::Solution3^ soln = (EnvDTE90::Solution3^)
				_applicationObject->Solution;
			Project^ proj = GetProjectByName(soln, "策略");
			if(proj != nullptr) {
				SolutionFolder^ SF = (SolutionFolder^)proj->Object;
				SF->AddFromFile(projectFile);
			}
		}
		catch(Exception^ e) {
		}
	}

	MessageBox::Show("导入完成");

	return true;
}

bool CltStrategy::Connect::RunStategyTest(int nRunMode, const std::string& strPath, const std::string& strStrategyID)
{
	std::string strRunParams;
	std::string strStategyRunPath;

	char csIniFile[1024] = {0};

	strncpy(csIniFile, _GetAddinDllFileName().c_str(), sizeof(csIniFile));
	csIniFile[strlen(csIniFile)-4] = 0;
	strcat(csIniFile, ".ini");

	if(nRunMode==1) {
		strStategyRunPath = _GetTempPath()+strStrategyID+"\\";

		CToolsEx::DeleteAllFolder(strStategyRunPath);
		::CreateDirectory(strStategyRunPath.c_str(), NULL);
		CToolsEx::CopyFolderFiles(_GetSupportFilesPath(), strStategyRunPath);
		CToolsEx::CopyFolderFiles(strPath+"bin\\Debug\\", strStategyRunPath);

		// <tester strategyid='MATrend' strategypath='E:\\strategy\\Projects\\MATrend\\' traderid='888003' password='888888' inifile='CltStrategy.ini' />
		strRunParams = "1 \"<tester strategyid='" + strStrategyID 
				+ "' strategypath='" + strPath + "' runpath='" + strStategyRunPath 
				+ "' historypath='" + _GetHistoryDataPath() 
				+ "' traderid='" + _GetCurrUserID() + "' password='" + _GetPassword() 
				+ "' inifile='" + std::string(csIniFile) + "' />\"";
	}
	else if(nRunMode==2) {
		strStategyRunPath = _GetHistoryDataPath();
		strRunParams = "2 \"<tester inifile='" + std::string(csIniFile) + "' />\"";
	}
	else 
		return false;

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&pi,0,sizeof(pi));
	memset(&si,0,sizeof(si));
	si.cb=sizeof(si);
	si.wShowWindow=SW_SHOW;
	si.dwFlags=STARTF_USESHOWWINDOW;

	bool bRet = CreateProcess(NULL, (LPSTR)(std::string("StrategyTester.exe ") + strRunParams).c_str(), 
			NULL, NULL, FALSE, 0, NULL, (LPSTR)strStategyRunPath.c_str(), &si, &pi);

	return true;
}

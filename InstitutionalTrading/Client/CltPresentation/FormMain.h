#pragma once
#include "RiskMapForm.h"
#include "ReportForm.h"
#include "ReportDataProviderFactory.h"
#include "RiskCombinationScreenSelect.h"
#include "RiskMapData.h"
namespace CltPresentation {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FormMain
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormMain : public System::Windows::Forms::Form
	{
	public:
		FormMain(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			mnDefaultItem = 0;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormMain()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	protected: 
	private: System::Windows::Forms::ToolStripMenuItem^  系统ToolStripMenuItem;



	private: System::Windows::Forms::ToolStripMenuItem^  ExitToolStripMenuItem;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
	private: System::Windows::Forms::ToolStripMenuItem^  设置ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  统计ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  权益绩效ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  交易绩效ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  持仓统计ToolStripMenuItem;	
	private: System::Windows::Forms::ToolStripMenuItem^  展现ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  地图展现ToolStripMenuItem;



	private:Int32 mnDefaultItem;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->系统ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->设置ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ExitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->统计ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->权益绩效ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->交易绩效ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->持仓统计ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->展现ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->地图展现ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->menuStrip1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->系统ToolStripMenuItem, 
				this->统计ToolStripMenuItem, this->展现ToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(682, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// 系统ToolStripMenuItem
			// 
			this->系统ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->设置ToolStripMenuItem, 
				this->ExitToolStripMenuItem});
			this->系统ToolStripMenuItem->Name = L"系统ToolStripMenuItem";
			this->系统ToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->系统ToolStripMenuItem->Text = L"系统";
			// 
			// 设置ToolStripMenuItem
			// 
			this->设置ToolStripMenuItem->Name = L"设置ToolStripMenuItem";
			this->设置ToolStripMenuItem->Size = System::Drawing::Size(94, 22);
			this->设置ToolStripMenuItem->Text = L"设置";
			this->设置ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::设置ToolStripMenuItem_Click);
			// 
			// ExitToolStripMenuItem
			// 
			this->ExitToolStripMenuItem->Name = L"ExitToolStripMenuItem";
			this->ExitToolStripMenuItem->Size = System::Drawing::Size(94, 22);
			this->ExitToolStripMenuItem->Text = L"退出";
			this->ExitToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::ExitToolStripMenuItem_Click);
			// 
			// 统计ToolStripMenuItem
			// 
			this->统计ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->权益绩效ToolStripMenuItem, 
				this->交易绩效ToolStripMenuItem, this->持仓统计ToolStripMenuItem});
			this->统计ToolStripMenuItem->Name = L"统计ToolStripMenuItem";
			this->统计ToolStripMenuItem->Size = System::Drawing::Size(65, 20);
			this->统计ToolStripMenuItem->Text = L"历史统计";
			// 
			// 权益绩效ToolStripMenuItem
			// 
			this->权益绩效ToolStripMenuItem->Name = L"权益绩效ToolStripMenuItem";
			this->权益绩效ToolStripMenuItem->Size = System::Drawing::Size(118, 22);
			this->权益绩效ToolStripMenuItem->Text = L"权益绩效";
			this->权益绩效ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::权益绩效ToolStripMenuItem_Click);
			// 
			// 交易绩效ToolStripMenuItem
			// 
			this->交易绩效ToolStripMenuItem->Name = L"交易绩效ToolStripMenuItem";
			this->交易绩效ToolStripMenuItem->Size = System::Drawing::Size(118, 22);
			this->交易绩效ToolStripMenuItem->Text = L"交易绩效";
			this->交易绩效ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::交易绩效ToolStripMenuItem_Click);
			// 
			// 持仓统计ToolStripMenuItem
			// 
			this->持仓统计ToolStripMenuItem->Name = L"持仓统计ToolStripMenuItem";
			this->持仓统计ToolStripMenuItem->Size = System::Drawing::Size(118, 22);
			this->持仓统计ToolStripMenuItem->Text = L"持仓统计";
			this->持仓统计ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::持仓统计ToolStripMenuItem_Click);
			// 
			// 展现ToolStripMenuItem
			// 
			this->展现ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->地图展现ToolStripMenuItem});
			this->展现ToolStripMenuItem->Name = L"展现ToolStripMenuItem";
			this->展现ToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->展现ToolStripMenuItem->Text = L"展现";
			// 
			// 地图展现ToolStripMenuItem
			// 
			this->地图展现ToolStripMenuItem->Name = L"地图展现ToolStripMenuItem";
			this->地图展现ToolStripMenuItem->Size = System::Drawing::Size(118, 22);
			this->地图展现ToolStripMenuItem->Text = L"地图展现";
			this->地图展现ToolStripMenuItem->Click += gcnew System::EventHandler(this, &FormMain::地图展现ToolStripMenuItem_Click);
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripStatusLabel1});
			this->statusStrip1->Location = System::Drawing::Point(0, 478);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(682, 22);
			this->statusStrip1->TabIndex = 1;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(41, 17);
			this->toolStripStatusLabel1->Text = L"已登录";
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &FormMain::timer1_Tick);
			// 
			// FormMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(682, 500);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"FormMain";
			this->Text = L"机构交易版本统计展现终端";
			this->Load += gcnew System::EventHandler(this, &FormMain::FormMain_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		//退出
	private: System::Void ExitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 Close();
			 }
			 //登录

	private: System::Void 地图展现ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 RiskMapForm^ lForm = gcnew RiskMapForm();
				 lForm->ShowDialog();
			 }
	private: System::Void 权益绩效ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {

				 ReportForm^ lReportDialog = gcnew ReportForm();
				 lReportDialog->Text = "帐户权益绩效统计";
				 lReportDialog->DataProvider = CAccountInterestsDataProviderFactory::Instance()->CreateDataProvider();	
				 lReportDialog->ReportConrtolConfigGuid = "{9CBB8291-655B-499a-BF46-A5ACB7DA6F92}";
				 if(lReportDialog->ShowDialog() == ::DialogResult::OK)
				 {
				 }
			 }
	private: System::Void 交易绩效ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 ReportForm^ lReportDialog = gcnew ReportForm();
				 lReportDialog->Text = "帐户交易绩效统计";
				 lReportDialog->DataProvider =  CAccountTradesReportDataProviderFactory::Instance()->CreateDataProvider();
				 lReportDialog->ReportConrtolConfigGuid = "{D9E386DA-B1A6-4a07-A65B-8E6D64EDAD8D}";				
				 if(lReportDialog->ShowDialog() == ::DialogResult::OK)
				 {
				 }
			 }
	private: System::Void 持仓统计ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 ReportForm^ lReportDialog = gcnew ReportForm();
				 lReportDialog->Text = "持仓统计";
				 lReportDialog->DataProvider =  CAccountHodeReportDataProviderFactory::Instance()->CreateDataProvider();
                 lReportDialog->ReportConrtolConfigGuid = "{{5D2F1664-DCB3-48e1-9A21-8089AEBA3F36}}";			
				 if(lReportDialog->ShowDialog() == ::DialogResult::OK)
				 {
				 }
			 }
	private: System::Void 设置ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 RiskCombinationScreenSelect ^ lDlg = gcnew RiskCombinationScreenSelect();
				 if(lDlg->ShowDialog() == ::DialogResult::OK)
			  {
			  }

			 }
	private: System::Void FormMain_Load(System::Object^  sender, System::EventArgs^  e) {
				 int nWnd = this->Handle.ToInt32();
				 CTcpLayer::SetMainWndHandle(nWnd);
				 CTcpLayer::SubscribePkg(CMDID_GetALLError_Push, nWnd);

				 this->timer1->Interval = 100;
				 this->timer1->Start();
				



			 }
			 //重载WndProc，以处理WM_USER_Win消息
	protected:virtual void WndProc( Message% m ) override
			  {

				  if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_DataArrival )
				  {
					  CTcpLayer::ParseRecvPackageInWindowThread();						
				  }			
				  else if( m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_ConnStatus )
				  {
					  int nConnectStatus = m.LParam.ToInt32();
					  switch(nConnectStatus)
					  {
					  case 0:
						  toolStripStatusLabel1->Text = "与服务器断开连接,请重新登录";
						  break;
					  case 1:
						  //toolStripStatusLabel1->Text = "与交易服务器连接中...";
						  break;
					  case 2:
						  toolStripStatusLabel1->Text = "与服务器连接成功";
						  break;
					  case 3:
						  //toolStripStatusLabel1->Text = "与交易服务器连接失败,,请重新登录";
						  break;
					  }
				  } 
				  else if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival )
				  {
					  char *p = (char*)m.LParam.ToInt32();
					  Stru_UniPkgHead head;					
					  memset(&head, 0, sizeof(head));
					  memcpy(&head, p, sizeof(head));

				
					  switch ( head.cmdid )
					  {	 
					  case CMDID_GetALLError_Push:
						  {     
							  char szError[256];
							  if ( head.userdata3 == CF_ERROR_SUCCESS )
							  {
								  std::vector<sErrorInfo> lvecError;
								  int nCount = head.len / sizeof(sErrorInfo);
								  for ( int i = 0; i < nCount; i++ )
								  {
									  memset(szError,0,sizeof(szError));
									  sErrorInfo* pErrorInfo = (sErrorInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(sErrorInfo));
									  switch (pErrorInfo->meErrorType)
									  {
									  case FirstLevelError:
										  sprintf(szError,"%s: %s","1级错误，影响系统运行的错误",pErrorInfo->msError); 
										  break;
									  case SecondLevelError:
										  sprintf(szError,"%s: %s","2级错误，不影响系统运行的错误",pErrorInfo->msError); 
										  break;
									  case ThirdLevelError:
										  sprintf(szError,"%s: %s","3级错误，不影响系统运行",pErrorInfo->msError); 
										  break;
									  case OperationLogNeedSend:
										  sprintf(szError,"%s: %s","系统操作日志，推送到其他的客户端正常日志",pErrorInfo->msError); 
										  break;
									  }  			
									  _GetPopMsg()->SetTitle("展现系统消息");
									  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
										  szError);
								  }

							  }							
							  break;
						  }
					  default:break;
					  }

					
						
				  }
				  Form::WndProc ( m );
			  }
	private:System::Void LoadAutoConfig(cliext::vector<ScreenConfig^>^&  nScreensConfig)
			{
				/*
				<Screens>
				<Screen Type="1" OrgId="2" OrgName="北京" PrdID="" ProName="">      
				</Screen>
				<Screen Type="2" OrgId="2" OrgName="北京" ProID="33" ProName="理财1">      
				</Screen>
				<Screen Type="2" OrgId="2" OrgName="北京" ProID="33" ProName="理财1">      
				</Screen>
				<Screen Type="3" OrgId="" OrgName="" ProID="" ProName="">    //地图   
				</Screen>
				</Screens>
				*/
				//添加4个默认的
				System::Xml::XmlDocument ^ mXmlDoc = gcnew System::Xml::XmlDocument();
				nScreensConfig->clear();			
				try
				{
					String ^ path = System::Windows::Forms::Application::StartupPath + "\\Screens.xml";
					mXmlDoc->Load(path);
				}
				catch(Exception ^ e)
				{
					return;
				}

				XmlNode ^ lDefaultNode = mXmlDoc->SelectSingleNode("/Screens");
				if(lDefaultNode != nullptr)
				{
					mnDefaultItem =  Convert::ToInt32(lDefaultNode->Attributes["Default"]->Value);
				}


				int i = 0;
				XmlNodeList^ lScreenNodeList = mXmlDoc->SelectNodes("/Screens/Screen");
				if(lScreenNodeList != nullptr)
				{
					for each (XmlNode^ lScreenNode in lScreenNodeList)
				 {

					 ScreenConfig ^ lConfig = gcnew ScreenConfig();

					 lConfig->mnType =  Convert::ToInt32(lScreenNode->Attributes["Type"]->Value);
					 lConfig->mnOrgID =  Convert::ToInt32(lScreenNode->Attributes["OrgId"]->Value);
					 lConfig->msOrgName =  lScreenNode->Attributes["OrgName"]->Value;
					 lConfig->mnProID =  Convert::ToInt32(lScreenNode->Attributes["ProId"]->Value);
					 lConfig->msProName =  lScreenNode->Attributes["ProName"]->Value;	

					 nScreensConfig->push_back(lConfig);
				 }
				}

			}

	public:void ShowDailogWithScreenConfig(ScreenConfig ^  nConfig,bool bAdjustSize,int nIndex)
		   {
			   System::Drawing::Size lSize =
				   System::Drawing::Size(System::Windows::Forms::Screen::PrimaryScreen->WorkingArea.Width/2,
				   System::Windows::Forms::Screen::PrimaryScreen->WorkingArea.Height/2);

			   ScreenConfig ^ lConfig = nConfig;
			   int i = nIndex;
			   if(lConfig)
			   {
				   if(lConfig->mnType == 1)
				   {
					   //组织机构
					   DepartmentShow ^ ltempShowForm = gcnew DepartmentShow();
					   ltempShowForm->CurrentOrgID = lConfig->mnOrgID;
					   ltempShowForm->CurrentOrgName = lConfig->msOrgName;
					   if(bAdjustSize)
						   ltempShowForm->CombinationScreenSetting( Point( (i%2)*lSize.Width ,(i/2)*lSize.Height ),lSize);
					   ltempShowForm->Show();


				   }
				   if(lConfig->mnType == 2)
				   {
					   //理财产品
					   ProductShow ^ lp = gcnew ProductShow();
					   lp->CurrentOrgID = lConfig->mnOrgID;
					   lp->CurrentProductID =lConfig->mnProID;
					   lp->CurrentProductName = lConfig->msProName;
					   if(bAdjustSize)
						   lp->CombinationScreenSetting( Point( (i%2)*lSize.Width ,(i/2)*lSize.Height ),lSize);
					   lp->Show();

				   }
				   if(lConfig->mnType == 3)
				   {
					   //理财产品
					   RiskMapForm ^ lp = gcnew RiskMapForm();
					   /*	 lp->CurrentOrgID = lConfig->mnOrgID;
					   lp->CurrentProductID =lConfig->mnProID;
					   lp->CurrentProductName = lConfig->msProName;*/
					   if(bAdjustSize)
						   lp->CombinationScreenSetting( Point( (i%2)*lSize.Width ,(i/2)*lSize.Height ),lSize);
					   lp->Show();

				   }
			   }
		   }

	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
				 //
				 this->timer1->Stop();
				 cliext::vector<ScreenConfig^>^ nScreensConfig = gcnew cliext::vector<ScreenConfig^>();
				 LoadAutoConfig(nScreensConfig);

				 if(mnDefaultItem == 1)
				 {
					 RiskMapForm^ lForm = gcnew RiskMapForm();
					 lForm->ShowDialog();
				 }
				 else if(mnDefaultItem == 2)
				 {
					 ShowDailogWithScreenConfig(nScreensConfig[0],false,0);
				 }
				 else if(mnDefaultItem == 3)
				 {
					 if(nScreensConfig->size() != 4 && nScreensConfig->size() > 0)
					 {
						 ShowDailogWithScreenConfig(nScreensConfig[0],false,0);

					 }
					 else if(nScreensConfig->size() == 4)
					 {
						 for(int i = 0 ; i < 4 ; i ++)
						 {				  
							 ShowDailogWithScreenConfig(nScreensConfig[i],true,i);
						 }
					 }

				 }
			 }
};
}


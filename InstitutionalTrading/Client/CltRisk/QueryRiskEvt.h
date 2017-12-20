#pragma once
#include "CommonDef.h"
#include "CCommonFunc.h"
#include "COrganAndAccountIDTreeListDlg.h"
#include "RiskData.h"
#include "CommonErrorCode.h"
#include "QueryData.h"
#include "Tools.h"
#include "popmsgwin.h"
#include "TcpLayer.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltRisk {

	/// <summary>
	/// Summary for QueryRiskEvt
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class QueryRiskEvt : public System::Windows::Forms::Form
	{
	public:
		QueryRiskEvt(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			mDataTable = gcnew DataTable();
			m_pLogin = new sLoginRsp;
			HasPrivlegeReturn = false;
			m_pQueryData = CQueryData::NewInstance();
			m_nSeq=0;
			bQueryRet=false;
			
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~QueryRiskEvt()
		{
			if (components)
			{
				delete components;
			}
			if (m_pLogin)
			{
				delete m_pLogin;
				m_pLogin = NULL;
			}
			CQueryData::DestroyInstance();
			
		}
	private: int m_nSeq;
	private: CQueryData*     m_pQueryData;
			 bool HasPrivlegeReturn;
			 sLoginRsp* m_pLogin;
	private: DataTable ^ mDataTable;
			 bool        bQueryRet;
	private: COrganAndAccountIDTreeListDlg^ m_COrganAndAccountIDTreeListDlg;
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: AnywndComboBox::AnywndComboBoxControl^  comboBox_user;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  comboBox_RiskIndicator;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::ComboBox^  comboBox_RiskLevel;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker2;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Button^  button_query;
	private: System::Windows::Forms::Button^  button_clear;
	private: SimpleReportControl::SimpleReportControlControl^  dataGridView1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_riskevtID;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_user;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_username;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_RiskIndicator;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_detail;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_time;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_status;
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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->comboBox_user = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboBox_RiskIndicator = (gcnew System::Windows::Forms::ComboBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->comboBox_RiskLevel = (gcnew System::Windows::Forms::ComboBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->dateTimePicker2 = (gcnew System::Windows::Forms::DateTimePicker());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->button_query = (gcnew System::Windows::Forms::Button());
			this->button_clear = (gcnew System::Windows::Forms::Button());
			this->dataGridView1 = (gcnew SimpleReportControl::SimpleReportControlControl());
			this->Column_riskevtID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_user = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_username = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_RiskIndicator = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_detail = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_time = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_status = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(37, 33);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"交易员";
			// 
			// comboBox_user
			// 
			this->comboBox_user->BackColor = System::Drawing::Color::White;
			this->comboBox_user->ChildControl = nullptr;
			this->comboBox_user->DropDownHeight = 1;
			this->comboBox_user->DropDownWidth = 1;
			this->comboBox_user->FormattingEnabled = true;
			this->comboBox_user->IntegralHeight = false;
			this->comboBox_user->Location = System::Drawing::Point(84, 25);
			this->comboBox_user->MaxDropDownItems = 1;
			this->comboBox_user->Name = L"comboBox_user";
			this->comboBox_user->Size = System::Drawing::Size(100, 20);
			this->comboBox_user->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(41, 83);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(77, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"风险指标类型";
			this->label2->Visible = false;
			// 
			// comboBox_RiskIndicator
			// 
			this->comboBox_RiskIndicator->FormattingEnabled = true;
			this->comboBox_RiskIndicator->Location = System::Drawing::Point(124, 75);
			this->comboBox_RiskIndicator->Name = L"comboBox_RiskIndicator";
			this->comboBox_RiskIndicator->Size = System::Drawing::Size(113, 20);
			this->comboBox_RiskIndicator->TabIndex = 3;
			this->comboBox_RiskIndicator->Visible = false;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(267, 83);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(53, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"风险等级";
			this->label3->Visible = false;
			// 
			// comboBox_RiskLevel
			// 
			this->comboBox_RiskLevel->FormattingEnabled = true;
			this->comboBox_RiskLevel->Location = System::Drawing::Point(326, 75);
			this->comboBox_RiskLevel->Name = L"comboBox_RiskLevel";
			this->comboBox_RiskLevel->Size = System::Drawing::Size(106, 20);
			this->comboBox_RiskLevel->TabIndex = 5;
			this->comboBox_RiskLevel->Visible = false;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(229, 31);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(53, 12);
			this->label4->TabIndex = 6;
			this->label4->Text = L"起始日期";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->CustomFormat = L"yyyy-MM-dd";
			this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker1->Location = System::Drawing::Point(293, 23);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(115, 21);
			this->dateTimePicker1->TabIndex = 7;
			// 
			// dateTimePicker2
			// 
			this->dateTimePicker2->CustomFormat = L"yyyy-MM-dd";
			this->dateTimePicker2->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker2->Location = System::Drawing::Point(506, 24);
			this->dateTimePicker2->Name = L"dateTimePicker2";
			this->dateTimePicker2->Size = System::Drawing::Size(113, 21);
			this->dateTimePicker2->TabIndex = 8;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(447, 28);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(53, 12);
			this->label5->TabIndex = 9;
			this->label5->Text = L"结束日期";
			// 
			// button_query
			// 
			this->button_query->Location = System::Drawing::Point(667, 23);
			this->button_query->Name = L"button_query";
			this->button_query->Size = System::Drawing::Size(75, 23);
			this->button_query->TabIndex = 10;
			this->button_query->Text = L"查询";
			this->button_query->UseVisualStyleBackColor = true;
			this->button_query->Click += gcnew System::EventHandler(this, &QueryRiskEvt::button_query_Click);
			// 
			// button_clear
			// 
			this->button_clear->Location = System::Drawing::Point(774, 22);
			this->button_clear->Name = L"button_clear";
			this->button_clear->Size = System::Drawing::Size(75, 23);
			this->button_clear->TabIndex = 11;
			this->button_clear->Text = L"清空";
			this->button_clear->UseVisualStyleBackColor = true;
			this->button_clear->Click += gcnew System::EventHandler(this, &QueryRiskEvt::button_clear_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->dataGridView1->ConfigGuid = L"{B584946D-1D89-4795-BB84-96CB51435A05}";
			this->dataGridView1->Location = System::Drawing::Point(39, 111);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->RealDataTable = nullptr;
			this->dataGridView1->SimpleMode = true;
			this->dataGridView1->Size = System::Drawing::Size(810, 361);
			this->dataGridView1->StatisticDataTable = nullptr;
			this->dataGridView1->TabIndex = 12;
			// 
			// Column_riskevtID
			// 
			this->Column_riskevtID->Name = L"Column_riskevtID";
			// 
			// Column_user
			// 
			this->Column_user->Name = L"Column_user";
			// 
			// Column_username
			// 
			this->Column_username->Name = L"Column_username";
			// 
			// Column_RiskIndicator
			// 
			this->Column_RiskIndicator->Name = L"Column_RiskIndicator";
			// 
			// Column_detail
			// 
			this->Column_detail->Name = L"Column_detail";
			// 
			// Column_time
			// 
			this->Column_time->Name = L"Column_time";
			// 
			// Column_status
			// 
			this->Column_status->Name = L"Column_status";
			// 
			// timer1
			// 
			this->timer1->Interval = 180000;
			this->timer1->Tick += gcnew System::EventHandler(this, &QueryRiskEvt::timer1_Tick);
			// 
			// QueryRiskEvt
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(894, 494);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->button_clear);
			this->Controls->Add(this->button_query);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->dateTimePicker2);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->comboBox_RiskLevel);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboBox_RiskIndicator);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboBox_user);
			this->Controls->Add(this->label1);
			this->Name = L"QueryRiskEvt";
			this->Text = L"历史风险事件查询";
			this->Load += gcnew System::EventHandler(this, &QueryRiskEvt::QueryRiskEvt_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &QueryRiskEvt::QueryRiskEvt_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void QueryRiskEvt_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent();

				 SetDataTableColumns();
				 int nHwnd = this->Handle.ToInt32();
				 CTcpLayer::SubscribePkg(Cmd_RM_QryRiskEvent_Rsp, nHwnd);

				 CRiskData::GetInstance()->GetLogonInfo(*m_pLogin);

				 m_COrganAndAccountIDTreeListDlg = gcnew COrganAndAccountIDTreeListDlg();             
				 m_COrganAndAccountIDTreeListDlg->ResetRelateCombo(comboBox_user);
				 comboBox_user->ChildControl = m_COrganAndAccountIDTreeListDlg; 
				 ResetComboBoxTextReadOnly(comboBox_user);

				 this->dateTimePicker1->MaxDate = System::DateTime::Now;
				 this->dateTimePicker2->MaxDate = System::DateTime::Now;

				 dataGridView1->RealDataTable = mDataTable;


			 }

	private: System::Void SetDataTableColumns()
			 {
				 mDataTable->Columns->Clear();
				 mDataTable->Columns->Add(gcnew DataColumn("风险事件ID",     Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("交易员ID",   Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("交易员名称",   Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("风险指标类型",     Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("风险详情",   Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("当前风险阀值",   Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("风险等级更新时间",     Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("风险事件状态",     Type::GetType("System.String")));
			 }

	private: System::Void button_query_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 dataGridView1->Clear();
				 mDataTable->Rows->Clear();
				 std::string strDateStart = Tools::String2string(this->dateTimePicker1->Text->ToString());
				 std::string strDateEnd = Tools::String2string(this->dateTimePicker2->Text->ToString());
				 if(dateTimePicker2->Value.Date < dateTimePicker1->Value.Date) 
				 {
					 MessageBox::Show(ERROR_QUERY_DATEPROMPT, "提示",MessageBoxButtons::OK);
					 return;

				 }
				 RiskEventQueryKey Condition;
				 memset(&Condition,0,sizeof(Condition));	
				 strncpy(Condition.szStartDate, strDateStart.c_str(),sizeof(Condition.szStartDate)-1);
				 strncpy(Condition.szEndDate, strDateEnd.c_str(),sizeof(Condition.szEndDate)-1);
				 String^ istrt=comboBox_user->Text;
				 if(String::IsNullOrEmpty(istrt))
				 {
					 
					MessageBox::Show(NULL_QUERY_CONDITION, "提示",MessageBoxButtons::OK);
				 }
				 else
				 {
					 std::set<int> setAccntID;
					 m_COrganAndAccountIDTreeListDlg->GetSelectID(setAccntID);
					 if(setAccntID.size()==0)					 
					 {
						 MessageBox::Show(ERROR_CANNOTSCREENING, "提示",MessageBoxButtons::OK);
					 }
					 else
					 {
						 bQueryRet=false;
						 HasPrivlegeReturn = false;
						 this->button_query->Enabled = false;
						 m_nSeq = 0;
						 m_pQueryData->ClearSendSeq();
						 this->timer1->Start();

						 std::set<int>::iterator it = setAccntID.begin();
						 while( it!= setAccntID.end())
						 {
							 Condition.nTradeAccountID = *it;
							 CTcpLayer::SendData(Cmd_RM_QryRiskEvent_Req,(void *)&Condition, sizeof(Condition), m_nSeq, m_pLogin->mnUserID);
							 m_pQueryData->SetSendSeq(m_nSeq);
							 m_nSeq++;
							 it++;
						 }
					 }
					 

				 }
			 }
	private: System::Void button_clear_Click(System::Object^  sender, System::EventArgs^  e) 
			 {

				 dataGridView1->Clear();
				 mDataTable->Rows->Clear();

			 }

	protected:virtual void WndProc( Message% m ) override
			  {
				  if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival )
				  {

					  char *p = (char*)m.LParam.ToInt32();
					  Stru_UniPkgHead head;					
					  memset(&head, 0, sizeof(head));
					  memcpy(&head, p, sizeof(head));

					  switch ( head.cmdid )
					  {	 
					  case Cmd_RM_QryRiskEvent_Rsp://查询风险事件返回

						  {
							  m_pQueryData->ReceiveSeq(head.seq);
							  this->timer1->Stop();
							 
							  if ( head.userdata1 == CF_ERROR_SUCCESS )
							  {
								  int pkgLen=head.len;
								  int nCount = head.len / sizeof(RiskEvent);

								  if(nCount == 0)
									  break;

								  std::vector< RiskEvent> vecRiskEvent;

								  for ( int i = 0; i < nCount; i++ )
								  {
									  RiskEvent* pField = (RiskEvent*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(RiskEvent));

									  vecRiskEvent.push_back(*pField);

								  }
								  //loadcomboBoxRiskLevel(mapRiskEvent);
								  loadDataGridView1(vecRiskEvent);
								  dataGridView1->Refresh2();
								  if (vecRiskEvent.size() != 0)
								  {
									  bQueryRet=true;
								  }

							  }
							  else 
							  {
								  if ( head.userdata1 == CF_ERROR_COMMON_NO_PRIVILEGE )
								  {
									  if ( HasPrivlegeReturn )
									  {
										  break;
									  }
									  else
									  {
										  HasPrivlegeReturn = true;
										  string str = (char*)(p+sizeof(Stru_UniPkgHead));
										  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
											  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
											  str.c_str());
									  }
								  }						  
								  else
								  {
									  string str = (char*)(p+sizeof(Stru_UniPkgHead));
									  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
										  str.c_str());
								  }

							  }
							  this->timer1->Start();
							  break;
						  }

					  }
					  if (m_pQueryData->IsEmpty())
					  {
						  this->button_query->Enabled = true;
						  this->timer1->Stop();
						  
						  if ( !bQueryRet)
						  {
							  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
								  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
								  ERROR_QUERY_NONDATA);
						  }
					  }
				  }
				  Form::WndProc ( m );
			  }

			  private: System::Void loadcomboBoxRiskLevel(std::map<int, RiskEvent>& mapRiskEvent)
					   {
						   std::set<int> setRiskLevel;
						   std::map<int, RiskEvent>::iterator it = mapRiskEvent.begin();
						   for(it ; it != mapRiskEvent.end(); it++)
						   {
							   setRiskLevel.insert(it->second.nRiskLevelID);
						   }

						   std::set<int>::iterator itset= setRiskLevel.begin();
						   while (itset!=setRiskLevel.end())
						   { 
							   comboBox_RiskLevel->Items->Add((*itset).ToString());
							   itset++;
						   }

					   }
			  private: string RiskIndicatorID2String(RiskIndicatorType  type)
					   {
						   string szIndicatorName;

						   switch(type)
						   {
						   case RI_MaxPosition:
							   szIndicatorName = "合约最大持仓";
							   break;
						   case RI_MarketShock:
							   szIndicatorName = "市场冲击风险";
							   break;
						   case RI_SingleHoldTime:
							   szIndicatorName = "账户限亏";
							   break;
						   case RI_AccountLoss:
							   szIndicatorName = "今日限亏";
							   break;
						   case RI_TodayLoss:
							   szIndicatorName = "隔夜跳空";
							   break;
						   case RI_GappedMarket:
							   szIndicatorName = "保证金比例风险";
							   break;
						   case RI_MarginRatio:
							   szIndicatorName = "单笔持仓时间";
							   break;
						   case RI_FundNetValue:
							   szIndicatorName = "基金净值风险";
							   break;                                
						   }  
						   return szIndicatorName;
					   }
			  private: System::Void loadDataGridView1(std::vector< RiskEvent>& vecRiskEvent)
					   {


						   /*string strRiskLevel = Tools::String2string(comboBox_RiskLevel->Text);
						   string strRiskIndicator = Tools::String2string(comboBox_RiskIndicator->Text);*/
						   std::vector<RiskEvent>::iterator it = vecRiskEvent.begin();
						   for(it; it != vecRiskEvent.end(); it++)
						   {
							   string szTraderName,szIndicatorName,szEventStatus;
							   // szIndicatorName = RiskIndicatorID2String(it->nRiskIndicatorID);
							   map<RiskIndicatorType,RiskIndicator> mapRiskIndicator;
							   CRiskData::GetInstance()->GetRiskIndicator(mapRiskIndicator);

							   map<RiskIndicatorType,RiskIndicator>::iterator it2 = mapRiskIndicator.find(it->nRiskIndicatorID);
							   if(it2!=mapRiskIndicator.end())
							   {
								   szIndicatorName = (string)(it2->second.szName);
							   }

							   /* if ((!String::IsNullOrEmpty(comboBox_RiskLevel->Text)) && (it->nRiskLevelID != atoi(strRiskLevel.c_str())))
							   {
							   break;
							   }

							   if ((!String::IsNullOrEmpty(comboBox_RiskIndicator->Text)) && ( szIndicatorName != strRiskIndicator))
							   {
							   break;
							   }*/

							   if(!CRiskData::GetInstance()->GetUserNameByID(it->nTradeInvestorID,szTraderName))
								   szTraderName="";

							   char szIndicatorValue[64];
							   memset(szIndicatorValue,0,sizeof(szIndicatorValue));
							   switch (it->nRiskIndicatorID)
							   {
							   case RI_MaxPosition://合约最大持仓;   合约持仓总数（买卖方向汇总）
								   sprintf(szIndicatorValue, "%.0f手", it->dblIndicatorValue);
								   break;
							   case RI_MarketShock://市场冲击风险;   合约持仓总数（买卖方向汇总）占市场总持仓数百分比     
							   case RI_AccountLoss://账户限亏		  （今日动态权益 -- 账户初始权益）/账户初始权益
							   case RI_TodayLoss://今日限亏		  账户今日亏损占日初权益的百分比
							   case RI_GappedMarket://隔夜跳空        账户各合约持仓综合后（买卖方向中和），预测第二天最大跳空亏损占账户初始权益百分比
							   case RI_MarginRatio://保证金比例风险  保证金和动态权益的比值（风险度）
								   sprintf(szIndicatorValue, "%.2f%%", it->dblIndicatorValue*100);
								   break;
							   case RI_SingleHoldTime://单笔持仓时间  
								   sprintf(szIndicatorValue, "%.0f秒", it->dblIndicatorValue);
								   break;
							   case RI_FundNetValue://基金净值风险
								   sprintf(szIndicatorValue, "%.5f", it->dblIndicatorValue);
								   break;
							   }



							   String^ szRiskDesc = String::Empty;
							   if(it->nRiskIndicatorID == RI_MaxPosition || 
								   it->nRiskIndicatorID == RI_MarketShock ||
								   it->nRiskIndicatorID == RI_SingleHoldTime)
								   szRiskDesc = Tools::string2String(it->InstrumentID)+":"+it->nRiskLevelID.ToString()+"级风险";
							   else
								   szRiskDesc = it->nRiskLevelID.ToString()+"级风险";


							   switch(it->nMsgSendStatus)
							   {
							   case MsgStatus_New:
								   szEventStatus = "发生";
								   break;
							   case MsgStatus_ChangeLevel:
								   szEventStatus = "等级变更";
								   break;
							   case MsgStatus_Delete:
								   szEventStatus = "消失";
								   break;
							   case MsgStatus_Appoint:
								   szEventStatus = "委派给交易员";
								   break;
							   case MsgStatus_Finished:
								   szEventStatus = "风险事件解决记录";
								   break;
							   case MsgStatus_ForceClose:
								   szEventStatus = "执行手动强平记录";
								   break;
							   }

							   struct tm tmptm;
							   char prlt[32] = {0};
							   memcpy((char*)&tmptm,(char*)localtime(&(it->lEventTime)),sizeof(tmptm));
							   sprintf_s(prlt,"%04d%02d%02d %02d:%02d:%02d",tmptm.tm_year+1900,tmptm.tm_mon+1,tmptm.tm_mday,tmptm.tm_hour,tmptm.tm_min,tmptm.tm_sec);
							   DataRow ^ dr = mDataTable->NewRow();
							   dr[0] = it->nRiskEventID.ToString(); 
							   dr[1] = it->nTradeInvestorID.ToString(); 
							   dr[2] = Tools::string2String(szTraderName.c_str());
							   dr[3] = Tools::string2String(szIndicatorName.c_str());
							   dr[4] = szRiskDesc;
                               dr[5] = Tools::string2String(szIndicatorValue);
							   dr[6] = Tools::string2String(prlt);
							   dr[7] = Tools::string2String(szEventStatus.c_str());
							   mDataTable->Rows->Add(dr);
						   }
					   }


			  private: System::Void QueryRiskEvt_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
					   {

						   this->timer1->Stop();
						   IntPtr hWnd=this->Handle;
						   CTcpLayer::UnsubscribeAllPkg((int)hWnd);
					   }
			  private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
					   {
						   if (!m_pQueryData->IsEmpty())
						   {
							   _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
								   CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
								   ERROR_QUERY_TIMEOUT);
						   }
						   this->button_query->Enabled = true;
						   this->timer1->Stop();
					   }
			  };
	}

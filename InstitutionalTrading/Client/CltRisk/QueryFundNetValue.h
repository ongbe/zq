#pragma once
#include "CommonDef.h"
#include "CCommonFunc.h"
#include "COrganAndAccountIDTreeListDlg.h"
#include "RiskData.h"
#include "CommonErrorCode.h"
#include "QueryData.h"
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
	/// Summary for QueryFundNetValue
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class QueryFundNetValue : public System::Windows::Forms::Form
	{
	public:
		QueryFundNetValue(void)
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
		~QueryFundNetValue()
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
		 bool        bQueryRet;
		sLoginRsp* m_pLogin;

	private: DataTable ^ mDataTable;
	private: COrganAndAccountIDTreeListDlg^ m_COrganAndAccountIDTreeListDlg;
	private: System::Windows::Forms::Label^  label1;
	private: AnywndComboBox::AnywndComboBoxControl^  comboBox1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker2;
	private: System::Windows::Forms::Button^  button_query;
	private: System::Windows::Forms::Button^  button_clear;
	private: SimpleReportControl::SimpleReportControlControl^  dataGridView1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_userid;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_innervolumn;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_outervolumn;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_InnerNetAsset;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_OuterNetAsset;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_InnerPerNet;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_OuterPerNet;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_TotalNetAsset;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_date;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;
	protected: 

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
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker2 = (gcnew System::Windows::Forms::DateTimePicker());
			this->button_query = (gcnew System::Windows::Forms::Button());
			this->button_clear = (gcnew System::Windows::Forms::Button());
			this->Column_userid = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_innervolumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_outervolumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_InnerNetAsset = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_OuterNetAsset = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_InnerPerNet = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_OuterPerNet = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_TotalNetAsset = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_date = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->comboBox1 = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->dataGridView1 = (gcnew SimpleReportControl::SimpleReportControlControl());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(26, 26);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"交易员";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(27, 60);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"起始日期";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->CustomFormat = L"yyyy-MM-dd";
			this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker1->Location = System::Drawing::Point(94, 56);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(121, 21);
			this->dateTimePicker1->TabIndex = 3;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(269, 60);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(53, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"结束日期";
			// 
			// dateTimePicker2
			// 
			this->dateTimePicker2->CustomFormat = L"yyyy-MM-dd";
			this->dateTimePicker2->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker2->Location = System::Drawing::Point(338, 56);
			this->dateTimePicker2->Name = L"dateTimePicker2";
			this->dateTimePicker2->Size = System::Drawing::Size(117, 21);
			this->dateTimePicker2->TabIndex = 5;
			// 
			// button_query
			// 
			this->button_query->Location = System::Drawing::Point(28, 96);
			this->button_query->Name = L"button_query";
			this->button_query->Size = System::Drawing::Size(75, 23);
			this->button_query->TabIndex = 6;
			this->button_query->Text = L"查询";
			this->button_query->UseVisualStyleBackColor = true;
			this->button_query->Click += gcnew System::EventHandler(this, &QueryFundNetValue::button_query_Click);
			// 
			// button_clear
			// 
			this->button_clear->Location = System::Drawing::Point(139, 96);
			this->button_clear->Name = L"button_clear";
			this->button_clear->Size = System::Drawing::Size(75, 23);
			this->button_clear->TabIndex = 7;
			this->button_clear->Text = L"清空";
			this->button_clear->UseVisualStyleBackColor = true;
			this->button_clear->Click += gcnew System::EventHandler(this, &QueryFundNetValue::button_clear_Click);
			// 
			// Column_userid
			// 
			this->Column_userid->HeaderText = L"交易员账号";
			this->Column_userid->Name = L"Column_userid";
			this->Column_userid->ReadOnly = true;
			// 
			// Column_innervolumn
			// 
			this->Column_innervolumn->HeaderText = L"场内份额";
			this->Column_innervolumn->Name = L"Column_innervolumn";
			this->Column_innervolumn->ReadOnly = true;
			// 
			// Column_outervolumn
			// 
			this->Column_outervolumn->HeaderText = L"场外份额";
			this->Column_outervolumn->Name = L"Column_outervolumn";
			this->Column_outervolumn->ReadOnly = true;
			// 
			// Column_InnerNetAsset
			// 
			this->Column_InnerNetAsset->HeaderText = L"场内资产净值";
			this->Column_InnerNetAsset->Name = L"Column_InnerNetAsset";
			this->Column_InnerNetAsset->ReadOnly = true;
			// 
			// Column_OuterNetAsset
			// 
			this->Column_OuterNetAsset->HeaderText = L"场外资产净值";
			this->Column_OuterNetAsset->Name = L"Column_OuterNetAsset";
			this->Column_OuterNetAsset->ReadOnly = true;
			// 
			// Column_InnerPerNet
			// 
			this->Column_InnerPerNet->HeaderText = L"场内单位净值";
			this->Column_InnerPerNet->Name = L"Column_InnerPerNet";
			this->Column_InnerPerNet->ReadOnly = true;
			// 
			// Column_OuterPerNet
			// 
			this->Column_OuterPerNet->HeaderText = L"场外单位净值";
			this->Column_OuterPerNet->Name = L"Column_OuterPerNet";
			this->Column_OuterPerNet->ReadOnly = true;
			// 
			// Column_TotalNetAsset
			// 
			this->Column_TotalNetAsset->HeaderText = L"总资产净值";
			this->Column_TotalNetAsset->Name = L"Column_TotalNetAsset";
			this->Column_TotalNetAsset->ReadOnly = true;
			// 
			// Column_date
			// 
			this->Column_date->HeaderText = L"日期";
			this->Column_date->Name = L"Column_date";
			this->Column_date->ReadOnly = true;
			// 
			// timer1
			// 
			this->timer1->Interval = 180000;
			this->timer1->Tick += gcnew System::EventHandler(this, &QueryFundNetValue::timer1_Tick);
			// 
			// comboBox1
			// 
			this->comboBox1->BackColor = System::Drawing::Color::White;
			this->comboBox1->ChildControl = nullptr;
			this->comboBox1->DropDownHeight = 1;
			this->comboBox1->DropDownWidth = 1;
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->IntegralHeight = false;
			this->comboBox1->Location = System::Drawing::Point(93, 18);
			this->comboBox1->MaxDropDownItems = 1;
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 20);
			this->comboBox1->TabIndex = 1;
			// 
			// dataGridView1
			// 
			this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->dataGridView1->ConfigGuid = L"{3B304FED-26A1-4c08-A7CA-9FBE605DD16D}";
			this->dataGridView1->Location = System::Drawing::Point(29, 144);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->RealDataTable = nullptr;
			this->dataGridView1->SimpleMode = true;
			this->dataGridView1->Size = System::Drawing::Size(818, 373);
			this->dataGridView1->StatisticDataTable = nullptr;
			this->dataGridView1->TabIndex = 8;
			// 
			// QueryFundNetValue
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(875, 547);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->button_clear);
			this->Controls->Add(this->button_query);
			this->Controls->Add(this->dateTimePicker2);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->label1);
			this->Name = L"QueryFundNetValue";
			this->Text = L"历史基金净值查询";
			this->Load += gcnew System::EventHandler(this, &QueryFundNetValue::QueryFundNetValue_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &QueryFundNetValue::QueryFundNetValue_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void QueryFundNetValue_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent();
                 SetDataTableColumns();
				 int nHwnd = this->Handle.ToInt32();
				 CTcpLayer::SubscribePkg(Cmd_RM_QueryFundNetCalcResult_Rsp, nHwnd);

                 CRiskData::GetInstance()->GetLogonInfo(*m_pLogin);
				 m_COrganAndAccountIDTreeListDlg = gcnew COrganAndAccountIDTreeListDlg();             
				 m_COrganAndAccountIDTreeListDlg->ResetRelateCombo(comboBox1);
				 comboBox1->ChildControl = m_COrganAndAccountIDTreeListDlg; 
				 ResetComboBoxTextReadOnly(comboBox1);

				 this->dateTimePicker1->MaxDate = System::DateTime::Now;
				 this->dateTimePicker2->MaxDate = System::DateTime::Now;

				 dataGridView1->RealDataTable = mDataTable;
			 }
 private: System::Void SetDataTableColumns()
		  {
			  mDataTable->Columns->Clear();
			  mDataTable->Columns->Add(gcnew DataColumn("交易员账号",     Type::GetType("System.String")));
			  mDataTable->Columns->Add(gcnew DataColumn("场内份额",   Type::GetType("System.Double")));
			  mDataTable->Columns->Add(gcnew DataColumn("场外份额",   Type::GetType("System.Double")));
			  mDataTable->Columns->Add(gcnew DataColumn("场内资产净值",     Type::GetType("System.Double")));
			  mDataTable->Columns->Add(gcnew DataColumn("场外资产净值",   Type::GetType("System.Double")));
			  mDataTable->Columns->Add(gcnew DataColumn("场内单位净值",     Type::GetType("System.Double")));
			  mDataTable->Columns->Add(gcnew DataColumn("场外单位净值",     Type::GetType("System.Double")));
			  mDataTable->Columns->Add(gcnew DataColumn("总资产净值",     Type::GetType("System.Double")));
			  mDataTable->Columns->Add(gcnew DataColumn("日期",     Type::GetType("System.String")));
		  }
private: System::Void button_query_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 std::string strDateStart = Tools::String2string(this->dateTimePicker1->Text->ToString());
			 std::string strDateEnd = Tools::String2string(this->dateTimePicker2->Text->ToString());
			 RiskEventQueryKey Condition;
			 memset(&Condition,0,sizeof(Condition));	
			 strncpy(Condition.szStartDate, strDateStart.c_str(),sizeof(Condition.szStartDate)-1);
			 strncpy(Condition.szEndDate, strDateEnd.c_str(),sizeof(Condition.szEndDate)-1);
			  if(dateTimePicker2->Value.Date < dateTimePicker1->Value.Date) 
			 {
				 MessageBox::Show(ERROR_QUERY_DATEPROMPT, "提示",MessageBoxButtons::OK);
				  return;

			 }
			 String^ istrt=comboBox1->Text;
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
						 CTcpLayer::SendData(Cmd_RM_QueryFundNetCalcResult_Req,(void *)&Condition, sizeof(Condition), m_nSeq, m_pLogin->mnUserID);
						 m_pQueryData->SetSendSeq(m_nSeq);
						 m_nSeq++;
						 it++;
					 }
				 }

			 }
			 dataGridView1->Clear();
			 mDataTable->Rows->Clear();
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
					  
				  case  Cmd_RM_QueryFundNetCalcResult_Rsp://查询风险事件返回
				  { 
					  m_pQueryData->ReceiveSeq(head.seq);
					  this->timer1->Stop();
					  if ( head.userdata1 == CF_ERROR_SUCCESS )
					  {
						  int pkgLen=head.len;
						  int nCount = head.len / sizeof(NetFundCalcResult);

						  if(nCount == 0)
							  break;
						  std::vector<NetFundCalcResult> vecNetFundCalcResult;
						  ProcessQueryRiskEvtPkg((const char*)m.LParam.ToInt32(), vecNetFundCalcResult,nCount);
						  loadDataGridView1(vecNetFundCalcResult);
						  dataGridView1->Refresh2();
						  if (vecNetFundCalcResult.size() != 0)
						  {
							  bQueryRet=true;
						  }
					  }
					  else if ( head.userdata1 == CF_ERROR_COMMON_NO_PRIVILEGE )
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

		  private: System::Void loadDataGridView1(std::vector<NetFundCalcResult>& vecNetFundCalcResult)
				   {
					   
					   std::vector<NetFundCalcResult>::iterator it = vecNetFundCalcResult.begin();
					   for(it; it != vecNetFundCalcResult.end(); it++)
					   {						   
						   DataRow ^ dr = mDataTable->NewRow();
	
						   string szTraderName;
						   if(!CRiskData::GetInstance()->GetUserNameByID(it->nTradeAccountID,szTraderName))
							   szTraderName="";
						   dr[0] = Tools::string2String(szTraderName.c_str());//帐号

						   //char str[50];
						   //memset(&str, 0, sizeof(str));
						   //sprintf(str, "%.2f", it->dInnerVolumn);
						   //dr[1] = Tools::string2String(str); //场内份额

						   //memset(&str, 0, sizeof(str));
						   //sprintf(str, "%.2f", it->dOuterVolumn);
						   //dr[2] = Tools::string2String(str);//场外份额

						   //memset(&str, 0, sizeof(str));
						   //sprintf(str, "%.2f", it->dInnerNetAsset);
						   //dr[3] = Tools::string2String(str);//场内资产净值

						   //memset(&str, 0, sizeof(str));
						   //sprintf(str, "%.2f", it->dOuterNetAsset);
						   //dr[4] = Tools::string2String(str);//场外资产净值

						   //memset(&str, 0, sizeof(str));
						   //sprintf(str, "%.2f", it->dInnerPerNet);
						   //dr[5] = Tools::string2String(str);//场内单位净值

						   //memset(&str, 0, sizeof(str));
						   //sprintf(str, "%.2f", it->dOuterPerNet);
						   //dr[6] = Tools::string2String(str);//场外单位净值

						   //memset(&str, 0, sizeof(str));
						   //sprintf(str, "%.2f", it->dTotalNetAsset);
						   //dr[7] = Tools::string2String(str);//总资产净值

						   dr[1] = it->dInnerVolumn;
						   dr[2] = it->dOuterVolumn;
						   dr[3] = it->dInnerNetAsset;
						   dr[4] = it->dOuterNetAsset;
						   dr[5] = it->dInnerPerNet;
						   dr[6] = it->dOuterPerNet;
						   dr[7] = it->dTotalNetAsset;

						   dr[8] =  Tools::string2String(it->szUpdateDate);  //日期

						    mDataTable->Rows->Add(dr);
					   }
				   }
				   bool ProcessQueryRiskEvtPkg(const char* pPkg,std::vector<NetFundCalcResult>& vecNetFundCalcResult,int fieldcnt)
				   {
					   for ( int i = 0; i < fieldcnt; i++ )
					   {
						   NetFundCalcResult* pField = (NetFundCalcResult*)((char*)(pPkg+sizeof(Stru_UniPkgHead)) + i*sizeof(NetFundCalcResult));
						   vecNetFundCalcResult.push_back(*pField);
					   }
					   return true;
				   }

private: System::Void QueryFundNetValue_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
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

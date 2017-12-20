#pragma once
#include "CommonDef.h"
#include "CCommonFunc.h"
#include "COrganAndAccountIDTreeListDlg.h"
#include "FormInstrumentIDTreeList.h"
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
	/// Summary for QueryVerifyOrder
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class QueryVerifyOrder : public System::Windows::Forms::Form
	{
	public:
		QueryVerifyOrder(void)
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
		~QueryVerifyOrder()
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
		 bool        bQueryRet;

	private: DataTable ^ mDataTable;
	private: COrganAndAccountIDTreeListDlg^ m_COrganAndAccountIDTreeListDlg;
	private: FormInstrumentIDTreeList^ m_FormInstrumentIDTreeList;
	private: System::Windows::Forms::Label^  label1;
	private: AnywndComboBox::AnywndComboBoxControl^  comboBox_user;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
	private: System::Windows::Forms::Label^  label3;
	private: AnywndComboBox::AnywndComboBoxControl^  comboBox_ins;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Button^  button_query;
	private: System::Windows::Forms::Button^  button_clear;
	private: SimpleReportControl::SimpleReportControlControl^  dataGridView1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1_LocalRequestID;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1_OrderSysID;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_user;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_ins;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_direction;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_OffsetFlag;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_price;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_volume;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_InsertDate;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_time;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_VerifyUser;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_VerifyDate;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_result;
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
			this->comboBox_user = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->comboBox_ins = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->button_query = (gcnew System::Windows::Forms::Button());
			this->button_clear = (gcnew System::Windows::Forms::Button());
			this->dataGridView1 = (gcnew SimpleReportControl::SimpleReportControlControl());
			this->Column1_LocalRequestID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column1_OrderSysID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_user = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_ins = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_direction = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_OffsetFlag = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_price = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_volume = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_InsertDate = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_time = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_VerifyUser = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_VerifyDate = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_result = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(29, 32);
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
			this->comboBox_user->Location = System::Drawing::Point(76, 23);
			this->comboBox_user->MaxDropDownItems = 1;
			this->comboBox_user->Name = L"comboBox_user";
			this->comboBox_user->Size = System::Drawing::Size(81, 20);
			this->comboBox_user->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(206, 32);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(29, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"日期";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->CustomFormat = L"yyyy-MM-dd";
			this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			this->dateTimePicker1->Location = System::Drawing::Point(250, 23);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(113, 21);
			this->dateTimePicker1->TabIndex = 3;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(422, 27);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(29, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"品种";
			// 
			// comboBox_ins
			// 
			this->comboBox_ins->BackColor = System::Drawing::Color::White;
			this->comboBox_ins->ChildControl = nullptr;
			this->comboBox_ins->DropDownHeight = 1;
			this->comboBox_ins->DropDownWidth = 1;
			this->comboBox_ins->FormattingEnabled = true;
			this->comboBox_ins->IntegralHeight = false;
			this->comboBox_ins->Location = System::Drawing::Point(457, 23);
			this->comboBox_ins->MaxDropDownItems = 1;
			this->comboBox_ins->Name = L"comboBox_ins";
			this->comboBox_ins->Size = System::Drawing::Size(109, 20);
			this->comboBox_ins->TabIndex = 5;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(642, 28);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(53, 12);
			this->label4->TabIndex = 6;
			this->label4->Text = L"审核状态";
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"全部", L"通过", L"待审核", L"禁止"});
			this->comboBox1->Location = System::Drawing::Point(718, 19);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(99, 20);
			this->comboBox1->TabIndex = 7;
			this->comboBox1->Text = L"全部";
			// 
			// button_query
			// 
			this->button_query->Location = System::Drawing::Point(31, 68);
			this->button_query->Name = L"button_query";
			this->button_query->Size = System::Drawing::Size(75, 23);
			this->button_query->TabIndex = 8;
			this->button_query->Text = L"查询";
			this->button_query->UseVisualStyleBackColor = true;
			this->button_query->Click += gcnew System::EventHandler(this, &QueryVerifyOrder::button_query_Click);
			// 
			// button_clear
			// 
			this->button_clear->Location = System::Drawing::Point(191, 68);
			this->button_clear->Name = L"button_clear";
			this->button_clear->Size = System::Drawing::Size(75, 23);
			this->button_clear->TabIndex = 9;
			this->button_clear->Text = L"清空";
			this->button_clear->UseVisualStyleBackColor = true;
			this->button_clear->Click += gcnew System::EventHandler(this, &QueryVerifyOrder::button_clear_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->dataGridView1->ConfigGuid = L"{5F95FE77-EEEA-4ab1-B0A1-138F374A8A4C}";
			this->dataGridView1->Location = System::Drawing::Point(31, 119);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->RealDataTable = nullptr;
			this->dataGridView1->SimpleMode = true;
			this->dataGridView1->Size = System::Drawing::Size(867, 338);
			this->dataGridView1->StatisticDataTable = nullptr;
			this->dataGridView1->TabIndex = 10;
			// 
			// Column1_LocalRequestID
			// 
			this->Column1_LocalRequestID->HeaderText = L"本地请求编号";
			this->Column1_LocalRequestID->Name = L"Column1_LocalRequestID";
			// 
			// Column1_OrderSysID
			// 
			this->Column1_OrderSysID->HeaderText = L"报单编号";
			this->Column1_OrderSysID->Name = L"Column1_OrderSysID";
			// 
			// Column_user
			// 
			this->Column_user->HeaderText = L"交易员账号";
			this->Column_user->Name = L"Column_user";
			// 
			// Column_ins
			// 
			this->Column_ins->HeaderText = L"合约";
			this->Column_ins->Name = L"Column_ins";
			// 
			// Column_direction
			// 
			this->Column_direction->HeaderText = L"方向";
			this->Column_direction->Name = L"Column_direction";
			// 
			// Column_OffsetFlag
			// 
			this->Column_OffsetFlag->HeaderText = L"开平";
			this->Column_OffsetFlag->Name = L"Column_OffsetFlag";
			// 
			// Column_price
			// 
			this->Column_price->HeaderText = L"价格";
			this->Column_price->Name = L"Column_price";
			// 
			// Column_volume
			// 
			this->Column_volume->HeaderText = L"数量";
			this->Column_volume->Name = L"Column_volume";
			// 
			// Column_InsertDate
			// 
			this->Column_InsertDate->HeaderText = L"报单日期";
			this->Column_InsertDate->Name = L"Column_InsertDate";
			// 
			// Column_time
			// 
			this->Column_time->HeaderText = L"委托时间";
			this->Column_time->Name = L"Column_time";
			// 
			// Column_VerifyUser
			// 
			this->Column_VerifyUser->HeaderText = L"审核员";
			this->Column_VerifyUser->Name = L"Column_VerifyUser";
			// 
			// Column_VerifyDate
			// 
			this->Column_VerifyDate->HeaderText = L"审核日期";
			this->Column_VerifyDate->Name = L"Column_VerifyDate";
			// 
			// Column_result
			// 
			this->Column_result->HeaderText = L"审核结果";
			this->Column_result->Name = L"Column_result";
			// 
			// timer1
			// 
			this->timer1->Interval = 180000;
			this->timer1->Tick += gcnew System::EventHandler(this, &QueryVerifyOrder::timer1_Tick);
			// 
			// QueryVerifyOrder
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(925, 484);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->button_clear);
			this->Controls->Add(this->button_query);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->comboBox_ins);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboBox_user);
			this->Controls->Add(this->label1);
			this->Name = L"QueryVerifyOrder";
			this->Text = L"报单审核记录查询";
			this->Load += gcnew System::EventHandler(this, &QueryVerifyOrder::QueryVerifyOrder_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &QueryVerifyOrder::QueryVerifyOrder_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void QueryVerifyOrder_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent();

				 int nHwnd = this->Handle.ToInt32();
				 CTcpLayer::SubscribePkg(Cmd_RM_QryVerifyOrder_Rsp, nHwnd);

                 CRiskData::GetInstance()->GetLogonInfo(*m_pLogin);
				 SetDataTableColumns();
				 m_COrganAndAccountIDTreeListDlg = gcnew COrganAndAccountIDTreeListDlg();             
				 m_COrganAndAccountIDTreeListDlg->ResetRelateCombo(comboBox_user);
				 comboBox_user->ChildControl = m_COrganAndAccountIDTreeListDlg; 
				 ResetComboBoxTextReadOnly(comboBox_user);				

				 m_FormInstrumentIDTreeList = gcnew FormInstrumentIDTreeList();
				 m_FormInstrumentIDTreeList->ResetRelateCombo(comboBox_ins);
				 comboBox_ins->ChildControl = m_FormInstrumentIDTreeList;
				 ResetComboBoxTextReadOnly(comboBox_ins);

				 this->dateTimePicker1->MaxDate = System::DateTime::Now;
				 dataGridView1->RealDataTable = mDataTable;

			 }
	private: System::Void SetDataTableColumns()
			 {
				 mDataTable->Columns->Clear();
				 //mDataTable->Columns->Add(gcnew DataColumn("本地请求编号",     Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("报单编号",   Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("交易员账号",   Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("合约",   Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("方向",     Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("开平",   Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("价格",     Type::GetType("System.Double")));
				 mDataTable->Columns->Add(gcnew DataColumn("数量",     Type::GetType("System.Int32")));
				 mDataTable->Columns->Add(gcnew DataColumn("报单日期",     Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("委托时间",   Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("审核员",     Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("审核日期",     Type::GetType("System.String")));
				 mDataTable->Columns->Add(gcnew DataColumn("审核结果",     Type::GetType("System.String")));
			 }
	private: System::Void dataGridView1_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
			 }
	private: System::Void button_query_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 SQueryOrder Condition;
				 memset(&Condition,0,sizeof(Condition));
				 std::string strDate = Tools::String2string(this->dateTimePicker1->Text->ToString());
				 strncpy(Condition.szVerifyDate,strDate.c_str(),sizeof(Condition.szVerifyDate)-1);
				 Condition.bIsProduct = true;
				 std::string strResult = Tools::String2string(this->comboBox1->Text->ToString());

				 int result =-2;
				 if ( strResult == "通过")
				 {
					 result = 1;
				 }
				 else if( strResult == "待审核")
				 {
					 result = -1;
				 }
				 else if( strResult == "禁止")
				 {
					 result = 0;
				 }
				 else if( strResult == "全部")
				 {
					 result = -2;
				 }
				 Condition.nResult = result;


				 std::set<int> setAccntID;
				 m_COrganAndAccountIDTreeListDlg->GetSelectID(setAccntID);
				 if (setAccntID.size() == 0)
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

					 do{
						 string userName;
						 if((setAccntID.size() != 0) && CRiskData::GetInstance()->GetUserNameByID( *it,userName))
						 {
							 strncpy(Condition.Account,userName.c_str(),sizeof(Condition.Account)-1);
						 }

						 std::set<string> setIns;
						 if ( m_FormInstrumentIDTreeList->GetSelectID(setIns))
						 {
							 std::set<string>::iterator it2= setIns.begin();
							 while (it2 != setIns.end())
							 { 
								 strncpy(Condition.InstrumentID,(*it2).c_str(),sizeof(Condition.InstrumentID)-1);
								 CTcpLayer::SendData(Cmd_RM_QryVerifyOrder_Req,(void *)&Condition, sizeof(Condition), m_nSeq,m_pLogin->mnUserID);
								 m_pQueryData->SetSendSeq(m_nSeq);
								 m_nSeq++;
								 it2++;
							 }
						 }
						 else
						 {
							 CTcpLayer::SendData(Cmd_RM_QryVerifyOrder_Req,(void *)&Condition, sizeof(Condition), m_nSeq, m_pLogin->mnUserID);
							 m_pQueryData->SetSendSeq(m_nSeq);
							 m_nSeq++;
						 }

						 it++;
					 }while( it!= setAccntID.end());
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
					  case Cmd_RM_QryVerifyOrder_Rsp:

						  {
							  m_pQueryData->ReceiveSeq(head.seq);
							  this->timer1->Stop();
							  if ( head.userdata1 == CF_ERROR_SUCCESS )
							  {
								  int pkgLen=head.len;
								  int nCount = head.len / sizeof(SQueryOrderResult);

								  if(nCount == 0)
									  break;

								  std::vector< SQueryOrderResult> vecField;

								  for ( int i = 0; i < nCount; i++ )
								  {
									  SQueryOrderResult* pField = (SQueryOrderResult*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(SQueryOrderResult));

									  vecField.push_back(*pField);

								  }
								  loadDataGridView1(vecField);
								  dataGridView1->Refresh2();
								  if (vecField.size() != 0)
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

	private: const char* TradeDirection2String(char param)
			 {

				 const static char* Buy="买";
				 const static char* Sell="卖";
				 switch(param)
			  {
				 case THOST_FTDC_D_Buy:return Buy;
				 case THOST_FTDC_D_Sell:return Sell;
				 default: return "";
			  }
			 }

	private: const char* OffsetFlag2String(char param)
			 {
				 const static char* Open="开仓";
				 const static char* Close="平仓";
				 const static char* ForceClose="强平";
				 const static char* CloseToday="平今";
				 const static char* CloseYesterday="平昨";
				 switch(param)
				 {
				 case THOST_FTDC_OF_Open:		return Open;
				 case THOST_FTDC_OF_Close:		return Close;
				 case THOST_FTDC_OF_ForceClose:	return ForceClose;
				 case THOST_FTDC_OF_CloseToday:	return CloseToday;
				 case THOST_FTDC_OF_CloseYesterday:return CloseYesterday;
				 default: return "";
				 }
			 }

	private: System::Void loadDataGridView1(std::vector< SQueryOrderResult>& vecField)
			 {	
				 std::vector< SQueryOrderResult>::iterator it = vecField.begin();
				 for(it; it != vecField.end(); it++)
				 {
					 string szResult;

					 /*if(!CRiskData::GetInstance()->GetUserNameByID(it->nVerifyUser,szVerifyUserName))
						 szVerifyUserName="";*/

					 switch(it->nResult)
					 {
					 case 1:
						 szResult = "通过";
						 break;
					 case 0:
						 szResult = "禁止";
						 break;
					 case -1:
						 szResult = "待审核";
						 break;
					 default:
						 szResult = "";
						 break;

					 }

					 DataRow ^ dr = mDataTable->NewRow();
					 //dr[0]= Tools::string2String(it->order.strLocalRequestID); 
					 dr[0] = Tools::string2String(it->order.OrderSysID);
					 dr[1] = Tools::string2String(it->order.InvestorID);
					 dr[2] = Tools::string2String(it->order.InstrumentID);
					 dr[3] = Tools::string2String(TradeDirection2String(it->order.Direction)); 
					 dr[4] = Tools::string2String(OffsetFlag2String(it->order.CombOffsetFlag[0])); 
					 dr[5] = it->order.LimitPrice;
					 dr[6] = it->order.VolumeTotalOriginal;
					 dr[7] = Tools::string2String(it->order.InsertDate);
					 dr[8] = Tools::string2String(it->order.InsertTime);
					 dr[9] = Tools::string2String(it->VerifyUserName);
					 dr[10] = Tools::string2String(it->szVerifyDate);
					 dr[11] = Tools::string2String(szResult.c_str());

					 mDataTable->Rows->Add(dr);
				 }
			 }

	private: System::Void QueryVerifyOrder_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
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

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "COrganAndAccountIDTreeListDlg.h"
#include "CCommonFunc.h"
#include "CDataInfo.h"
namespace Client {

	/// <summary>
	/// Summary for QueryFundNetValueModule
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class QueryFundNetValueModule : public System::Windows::Forms::Form
	{
	public:
		QueryFundNetValueModule(void)
		{
			InitializeComponent();
			m_pdataInfo = CDataInfo::NewInstance();
			IntPtr hWnd=this->Handle;
			CTcpLayer::SubscribePkg( Cmd_RM_QueryFundNetCalcResult_Rsp ,(int)(hWnd.ToInt32()));
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~QueryFundNetValueModule()
		{
			if (components)
			{
				delete components;
			}
		}
	private: CDataInfo*                        m_pdataInfo;
	private: AnywndComboBox::AnywndComboBoxControl^  comboAccount;
	private: COrganAndAccountIDTreeListDlg^    dlgOrganAndAccountIDTreeList;

	protected: 
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::DateTimePicker^  dateTimePickerStart;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Button^  button_clear;
	private: System::Windows::Forms::Button^  button_query;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_nTradeAccountID;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_dInnerVolumn;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_dOuterVolumn;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_dInnerNetAsset;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_dOuterNetAsset;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_dInnerPerNet;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_dOuterPerNet;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_dTotalNetAsset;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_szUpdateDate;













	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->comboAccount = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->dateTimePickerStart = (gcnew System::Windows::Forms::DateTimePicker());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->button_clear = (gcnew System::Windows::Forms::Button());
			this->button_query = (gcnew System::Windows::Forms::Button());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->ID_nTradeAccountID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ID_dInnerVolumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ID_dOuterVolumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ID_dInnerNetAsset = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ID_dOuterNetAsset = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ID_dInnerPerNet = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ID_dOuterPerNet = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ID_dTotalNetAsset = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ID_szUpdateDate = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// comboAccount
			// 
			this->comboAccount->BackColor = System::Drawing::Color::White;
			this->comboAccount->ChildControl = nullptr;
			this->comboAccount->DropDownHeight = 1;
			this->comboAccount->DropDownWidth = 1;
			this->comboAccount->FormattingEnabled = true;
			this->comboAccount->IntegralHeight = false;
			this->comboAccount->Location = System::Drawing::Point(98, 22);
			this->comboAccount->MaxDropDownItems = 1;
			this->comboAccount->Name = L"comboAccount";
			this->comboAccount->Size = System::Drawing::Size(121, 20);
			this->comboAccount->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(27, 30);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"账户号";
			// 
			// dateTimePickerStart
			// 
			this->dateTimePickerStart->Location = System::Drawing::Point(98, 62);
			this->dateTimePickerStart->Name = L"dateTimePickerStart";
			this->dateTimePickerStart->Size = System::Drawing::Size(121, 21);
			this->dateTimePickerStart->TabIndex = 18;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(27, 65);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(53, 12);
			this->label6->TabIndex = 17;
			this->label6->Text = L"起始日期";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->Location = System::Drawing::Point(316, 65);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(115, 21);
			this->dateTimePicker1->TabIndex = 20;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(241, 69);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(53, 12);
			this->label4->TabIndex = 19;
			this->label4->Text = L"结束日期";
			// 
			// button_clear
			// 
			this->button_clear->Location = System::Drawing::Point(131, 104);
			this->button_clear->Name = L"button_clear";
			this->button_clear->Size = System::Drawing::Size(75, 23);
			this->button_clear->TabIndex = 22;
			this->button_clear->Text = L"清空";
			this->button_clear->UseVisualStyleBackColor = true;
			this->button_clear->Click += gcnew System::EventHandler(this, &QueryFundNetValueModule::button_clear_Click);
			// 
			// button_query
			// 
			this->button_query->Location = System::Drawing::Point(24, 104);
			this->button_query->Name = L"button_query";
			this->button_query->Size = System::Drawing::Size(75, 23);
			this->button_query->TabIndex = 21;
			this->button_query->Text = L"查询";
			this->button_query->UseVisualStyleBackColor = true;
			this->button_query->Click += gcnew System::EventHandler(this, &QueryFundNetValueModule::button_query_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(9) {this->ID_nTradeAccountID, 
				this->ID_dInnerVolumn, this->ID_dOuterVolumn, this->ID_dInnerNetAsset, this->ID_dOuterNetAsset, this->ID_dInnerPerNet, this->ID_dOuterPerNet, 
				this->ID_dTotalNetAsset, this->ID_szUpdateDate});
			this->dataGridView1->Location = System::Drawing::Point(22, 135);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(699, 309);
			this->dataGridView1->TabIndex = 23;
			// 
			// ID_nTradeAccountID
			// 
			this->ID_nTradeAccountID->HeaderText = L"账号ID";
			this->ID_nTradeAccountID->Name = L"ID_nTradeAccountID";
			this->ID_nTradeAccountID->ReadOnly = true;
			this->ID_nTradeAccountID->Width = 66;
			// 
			// ID_dInnerVolumn
			// 
			this->ID_dInnerVolumn->HeaderText = L"场内份额";
			this->ID_dInnerVolumn->Name = L"ID_dInnerVolumn";
			this->ID_dInnerVolumn->ReadOnly = true;
			this->ID_dInnerVolumn->Width = 78;
			// 
			// ID_dOuterVolumn
			// 
			this->ID_dOuterVolumn->HeaderText = L"场外份额";
			this->ID_dOuterVolumn->Name = L"ID_dOuterVolumn";
			this->ID_dOuterVolumn->ReadOnly = true;
			this->ID_dOuterVolumn->Width = 78;
			// 
			// ID_dInnerNetAsset
			// 
			this->ID_dInnerNetAsset->HeaderText = L"场内资产净值";
			this->ID_dInnerNetAsset->Name = L"ID_dInnerNetAsset";
			this->ID_dInnerNetAsset->ReadOnly = true;
			this->ID_dInnerNetAsset->Width = 72;
			// 
			// ID_dOuterNetAsset
			// 
			this->ID_dOuterNetAsset->HeaderText = L"场外资产净值";
			this->ID_dOuterNetAsset->Name = L"ID_dOuterNetAsset";
			this->ID_dOuterNetAsset->ReadOnly = true;
			this->ID_dOuterNetAsset->Width = 72;
			// 
			// ID_dInnerPerNet
			// 
			this->ID_dInnerPerNet->HeaderText = L"场内单位净值";
			this->ID_dInnerPerNet->Name = L"ID_dInnerPerNet";
			this->ID_dInnerPerNet->ReadOnly = true;
			this->ID_dInnerPerNet->Width = 72;
			// 
			// ID_dOuterPerNet
			// 
			this->ID_dOuterPerNet->HeaderText = L"场外单位净值";
			this->ID_dOuterPerNet->Name = L"ID_dOuterPerNet";
			this->ID_dOuterPerNet->ReadOnly = true;
			this->ID_dOuterPerNet->Width = 72;
			// 
			// ID_dTotalNetAsset
			// 
			this->ID_dTotalNetAsset->HeaderText = L"总资产净值";
			this->ID_dTotalNetAsset->Name = L"ID_dTotalNetAsset";
			this->ID_dTotalNetAsset->ReadOnly = true;
			this->ID_dTotalNetAsset->Width = 72;
			// 
			// ID_szUpdateDate
			// 
			this->ID_szUpdateDate->HeaderText = L"日期";
			this->ID_szUpdateDate->Name = L"ID_szUpdateDate";
			this->ID_szUpdateDate->ReadOnly = true;
			this->ID_szUpdateDate->Width = 51;
			// 
			// QueryFundNetValueModule
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(733, 456);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->button_clear);
			this->Controls->Add(this->button_query);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->dateTimePickerStart);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->comboAccount);
			this->Controls->Add(this->label1);
			this->Name = L"QueryFundNetValueModule";
			this->Text = L"查询基金历史净值";
			this->Load += gcnew System::EventHandler(this, &QueryFundNetValueModule::QueryFundNetValueModule_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
		private: System::Void QueryFundNetValueModule_Load(System::Object^  sender, System::EventArgs^  e) 
				 {
					dlgOrganAndAccountIDTreeList = gcnew COrganAndAccountIDTreeListDlg();
					dlgOrganAndAccountIDTreeList->ResetRelateCombo(comboAccount);
					comboAccount->ChildControl = dlgOrganAndAccountIDTreeList;

				
					 ResetComboBoxTextReadOnly(comboAccount);

					 // comboAccount
					 this->comboAccount->ChildControl = nullptr;
					 this->comboAccount->DropDownHeight = 1;
					 this->comboAccount->DropDownWidth = 1;
					 this->comboAccount->FormattingEnabled = true;
					 this->comboAccount->IntegralHeight = false;
					 this->comboAccount->MaxDropDownItems = 1;
					 //this->comboAccount->DropDown += gcnew System::EventHandler(this, &QueryRiskEvt::comboAccount_DropDown);

					


					 this->dateTimePicker1->MaxDate = System::DateTime::Now.AddDays(-1);
					 this->dateTimePicker1->Format = DateTimePickerFormat::Custom;
					 this->dateTimePicker1->CustomFormat = "yyyy-MM-dd";

					 this->dateTimePickerStart->MaxDate = System::DateTime::Now.AddDays(-1);
					 this->dateTimePickerStart->Format = DateTimePickerFormat::Custom;
					 this->dateTimePickerStart->CustomFormat = "yyyy-MM-dd";

				 }
				 //重载WndProc，以处理WM_COMMAND消息
protected:virtual void WndProc( Message% m ) override
		  {
			  if (  m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival )
			  {

				  char *p = (char*)m.LParam.ToInt32();
				  Stru_UniPkgHead head;					
				  memset(&head, 0, sizeof(head));
				  memcpy(&head, p, sizeof(head));

				  if ( head.cmdid == Cmd_RM_QueryFundNetCalcResult_Rsp)//查询风险事件返回
				  {
					  if ( head.userdata1 == OPERATOR_SUCCESS )
					  {
						  int pkgLen=head.len;
						  int MessageCnt=pkgLen/sizeof(NetFundCalcResult);
							
						  std::vector<NetFundCalcResult> vecNetFundCalcResult;
						  ProcessQueryRiskEvtPkg((const char*)m.LParam.ToInt32(), vecNetFundCalcResult,MessageCnt);
						  loadDataGridView1(vecNetFundCalcResult);
					  }
					  else
						{
							string str = (char*)(p+sizeof(Stru_UniPkgHead));
							String^ strtemp = Tools::string2String(str.c_str());
							MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);
						}
				  }
			  }

			  Form::WndProc ( m );
		  }
		  private: System::Void loadDataGridView1(std::vector<NetFundCalcResult>& vecNetFundCalcResult)
				   {
					   std::vector<NetFundCalcResult>::iterator it = vecNetFundCalcResult.begin();
					   for(it = vecNetFundCalcResult.begin(); it != vecNetFundCalcResult.end(); it++)
					   {						   
						   int newRowIndex=dataGridView1->Rows->Add();
						   DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
						   TrustTradeAccount account;
						   memset(&account,0,sizeof(account));
						   m_pdataInfo->GetTrustTradeAccount(it->nTradeAccountID,account);
						   dr->Cells[0]->Value = Tools::string2String(account.szAccount);//帐号

						   char str[50];
						   memset(&str, 0, sizeof(str));
						   sprintf(str, "%.2f", it->dInnerVolumn);
						   dr->Cells[1]->Value = Tools::string2String(str); //场内份额

						   memset(&str, 0, sizeof(str));
						   sprintf(str, "%.2f", it->dOuterVolumn);
						   dr->Cells[2]->Value = Tools::string2String(str);//场外份额

						   memset(&str, 0, sizeof(str));
						   sprintf(str, "%.2f", it->dInnerNetAsset);
						   dr->Cells[3]->Value = Tools::string2String(str);//场内资产净值

						   memset(&str, 0, sizeof(str));
						   sprintf(str, "%.2f", it->dOuterNetAsset);
						   dr->Cells[4]->Value = Tools::string2String(str);//场外资产净值

						   memset(&str, 0, sizeof(str));
						   sprintf(str, "%.2f", it->dInnerPerNet);
						   dr->Cells[5]->Value = Tools::string2String(str);//场内单位净值

						   memset(&str, 0, sizeof(str));
						   sprintf(str, "%.2f", it->dOuterPerNet);
						   dr->Cells[6]->Value = Tools::string2String(str);//场外单位净值

						   memset(&str, 0, sizeof(str));
						   sprintf(str, "%.2f", it->dTotalNetAsset);
						   dr->Cells[7]->Value = Tools::string2String(str);//总资产净值

						   dr->Cells[8]->Value =  Tools::string2String(it->szUpdateDate);  //日期
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
#pragma endregion
	private: System::Void button_query_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 std::set<int> setOrganID;
				 std::set<int> setFinanProdID;
				 //std::map<int, RiskEvent> mapRiskEvent;
				 std::string strDateStart = Tools::String2string(this->dateTimePickerStart->Text->ToString());
				 std::string strDateEnd = Tools::String2string(this->dateTimePicker1->Text->ToString());
				 String^ istrt=comboAccount->Text;
				 if(String::IsNullOrEmpty(istrt))
				 {
					 String^ strtemp = NULL_QUERY_CONDITION;
					 MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);
				 }
				 else
				 {

					 std::set<int> setAccntID;
					 dlgOrganAndAccountIDTreeList->GetSelectID(setAccntID);
					 ////请求查询风险事件
					 RiskEventQueryKey Condition;
					 memset(&Condition,0,sizeof(Condition));	
					 strncpy(Condition.szStartDate, strDateStart.c_str(),sizeof(Condition.szStartDate)-1);
					 strncpy(Condition.szEndDate, strDateEnd.c_str(),sizeof(Condition.szEndDate)-1);
			
					  dataGridView1->Rows->Clear();

					 std::set<int>::iterator itstr=setAccntID.begin();
					 for(itstr;itstr!=setAccntID.end();itstr++)
					 {
						 Condition.nTradeAccountID = *itstr;
						 CTcpLayer::SendData(Cmd_RM_QueryFundNetCalcResult_Req,(void *)&Condition, sizeof(Condition), 0);
					 }
					
				 }

			 }
private: System::Void button_clear_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 dataGridView1->Rows->Clear();
		 }
};
}

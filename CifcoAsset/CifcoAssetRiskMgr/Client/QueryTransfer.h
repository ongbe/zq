#pragma once

#include <string>
#include <map>
using namespace std;
#include "TcpLayer.h"
#include "Tools.h"
#include "QueryTransfer2.h"
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "AccountChoose.h"
#include "COrganAndAccountIDTreeListDlg.h"
#include "CCommonFunc.h"
#include "RiskManageStruct.h"


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;


namespace Client {

	/// <summary>
	/// Summary for QueryTransfer
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class QueryTransfer : public System::Windows::Forms::Form
	{
	public:
		QueryTransfer(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			GridView1_Load();
			m_QueryTransfer2 = new QueryTransfer2();
			m_pdataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~QueryTransfer()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected: 






    private: QueryTransfer2*                  m_QueryTransfer2;
	private: CDataInfo*                        m_pdataInfo;
	private: COrganAndAccountIDTreeListDlg^    dlgOrganAndAccountIDTreeList;

    private: AccountChoose^ AccountChooseForm;
	private: System::Windows::Forms::ComboBox^  comboBoxDirection;
	private: System::Windows::Forms::Button^  buttonQuery;
	private: System::Windows::Forms::Button^  buttonClear;
	private: System::Windows::Forms::DataGridView^  dataGridView1;












	private: System::Windows::Forms::Label^  label4;




	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnAccount;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnAssetManagement;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnFinancialProducts;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnDate;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnDirection;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnFundType;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnBank;

	private: AnywndComboBox::AnywndComboBoxControl^  comboAccount;






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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->comboBoxDirection = (gcnew System::Windows::Forms::ComboBox());
			this->buttonQuery = (gcnew System::Windows::Forms::Button());
			this->buttonClear = (gcnew System::Windows::Forms::Button());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnAccount = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnAssetManagement = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnFinancialProducts = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnDate = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnDirection = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnFundType = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnBank = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->comboAccount = (gcnew AnywndComboBox::AnywndComboBoxControl());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(30, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"帐号：";
			// 
			// comboBoxDirection
			// 
			this->comboBoxDirection->FormattingEnabled = true;
			this->comboBoxDirection->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"", L"出金", L"入金"});
			this->comboBoxDirection->Location = System::Drawing::Point(385, 15);
			this->comboBoxDirection->Name = L"comboBoxDirection";
			this->comboBoxDirection->Size = System::Drawing::Size(121, 20);
			this->comboBoxDirection->TabIndex = 7;
			// 
			// buttonQuery
			// 
			this->buttonQuery->Location = System::Drawing::Point(32, 66);
			this->buttonQuery->Name = L"buttonQuery";
			this->buttonQuery->Size = System::Drawing::Size(75, 23);
			this->buttonQuery->TabIndex = 8;
			this->buttonQuery->Text = L"查询";
			this->buttonQuery->UseVisualStyleBackColor = true;
			this->buttonQuery->Click += gcnew System::EventHandler(this, &QueryTransfer::buttonQuery_Click);
			// 
			// buttonClear
			// 
			this->buttonClear->Location = System::Drawing::Point(162, 66);
			this->buttonClear->Name = L"buttonClear";
			this->buttonClear->Size = System::Drawing::Size(75, 23);
			this->buttonClear->TabIndex = 9;
			this->buttonClear->Text = L"清空";
			this->buttonClear->UseVisualStyleBackColor = true;
			this->buttonClear->Click += gcnew System::EventHandler(this, &QueryTransfer::buttonClear_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(10) {this->Column1, 
				this->ColumnAccount, this->ColumnAssetManagement, this->ColumnFinancialProducts, this->ColumnDate, this->Column2, this->ColumnDirection, 
				this->Column3, this->ColumnFundType, this->ColumnBank});
			this->dataGridView1->Location = System::Drawing::Point(32, 109);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(1041, 408);
			this->dataGridView1->TabIndex = 10;
			this->dataGridView1->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &QueryTransfer::dataGridView1_CellContentClick);
			// 
			// Column1
			// 
			this->Column1->HeaderText = L"序列号";
			this->Column1->Name = L"Column1";
			this->Column1->ReadOnly = true;
			this->Column1->Width = 61;
			// 
			// ColumnAccount
			// 
			this->ColumnAccount->HeaderText = L"账号";
			this->ColumnAccount->Name = L"ColumnAccount";
			this->ColumnAccount->ReadOnly = true;
			this->ColumnAccount->Width = 51;
			// 
			// ColumnAssetManagement
			// 
			this->ColumnAssetManagement->HeaderText = L"账号所属资管";
			this->ColumnAssetManagement->Name = L"ColumnAssetManagement";
			this->ColumnAssetManagement->ReadOnly = true;
			this->ColumnAssetManagement->Width = 72;
			// 
			// ColumnFinancialProducts
			// 
			this->ColumnFinancialProducts->HeaderText = L"账号所属理财产品";
			this->ColumnFinancialProducts->Name = L"ColumnFinancialProducts";
			this->ColumnFinancialProducts->ReadOnly = true;
			this->ColumnFinancialProducts->Width = 83;
			// 
			// ColumnDate
			// 
			this->ColumnDate->HeaderText = L"日期";
			this->ColumnDate->Name = L"ColumnDate";
			this->ColumnDate->ReadOnly = true;
			this->ColumnDate->Width = 51;
			// 
			// Column2
			// 
			this->Column2->HeaderText = L"出入金金额";
			this->Column2->Name = L"Column2";
			this->Column2->ReadOnly = true;
			this->Column2->Width = 72;
			// 
			// ColumnDirection
			// 
			this->ColumnDirection->HeaderText = L"出入金方向";
			this->ColumnDirection->Name = L"ColumnDirection";
			this->ColumnDirection->ReadOnly = true;
			this->ColumnDirection->Width = 72;
			// 
			// Column3
			// 
			this->Column3->HeaderText = L"有效标志";
			this->Column3->Name = L"Column3";
			this->Column3->ReadOnly = true;
			this->Column3->Width = 61;
			// 
			// ColumnFundType
			// 
			this->ColumnFundType->HeaderText = L"出入金类型";
			this->ColumnFundType->Name = L"ColumnFundType";
			this->ColumnFundType->ReadOnly = true;
			this->ColumnFundType->Width = 72;
			// 
			// ColumnBank
			// 
			this->ColumnBank->HeaderText = L"经济公司代码";
			this->ColumnBank->Name = L"ColumnBank";
			this->ColumnBank->ReadOnly = true;
			this->ColumnBank->Width = 72;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(338, 23);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(41, 12);
			this->label4->TabIndex = 5;
			this->label4->Text = L"方向：";
			// 
			// comboAccount
			// 
			this->comboAccount->BackColor = System::Drawing::Color::White;
			this->comboAccount->ChildControl = nullptr;
			this->comboAccount->DropDownHeight = 1;
			this->comboAccount->DropDownWidth = 1;
			this->comboAccount->FormattingEnabled = true;
			this->comboAccount->IntegralHeight = false;
			this->comboAccount->Location = System::Drawing::Point(77, 15);
			this->comboAccount->MaxDropDownItems = 1;
			this->comboAccount->Name = L"comboAccount";
			this->comboAccount->Size = System::Drawing::Size(121, 20);
			this->comboAccount->TabIndex = 11;
			// 
			// QueryTransfer
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1085, 555);
			this->Controls->Add(this->comboAccount);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->buttonClear);
			this->Controls->Add(this->buttonQuery);
			this->Controls->Add(this->comboBoxDirection);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label1);
			this->Name = L"QueryTransfer";
			this->Text = L"当日出入金查询";
			this->Load += gcnew System::EventHandler(this, &QueryTransfer::QueryTransfer_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void dataGridView1_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
			 {
			 }
    private: System::Void QueryTransfer_Load(System::Object^  sender, System::EventArgs^  e) 
		     {
				 //先隐藏treeView
				// this->treeView1->Hide();
				 ////添加文本框点击事件
				 //this->textBoxAccount->Click += gcnew  System::EventHandler(this, &QueryTransfer::textBox1_Click);
				 //添加点击复选框事件
				 // this->treeView1->AfterCheck += gcnew  System::Windows::Forms::TreeViewEventHandler(this,&QueryTransfer::treeView1_AfterCheck);

				 //comboAccount

				 this->comboAccount->ChildControl = nullptr;
				 this->comboAccount->DropDownHeight = 1;
				 this->comboAccount->DropDownWidth = 1;
				 this->comboAccount->FormattingEnabled = true;
				 this->comboAccount->IntegralHeight = false;
				 this->comboAccount->MaxDropDownItems = 1;

				 dlgOrganAndAccountIDTreeList = gcnew COrganAndAccountIDTreeListDlg();
				 dlgOrganAndAccountIDTreeList->ResetRelateCombo(comboAccount);
				 comboAccount->ChildControl = dlgOrganAndAccountIDTreeList;

				  ResetComboBoxTextReadOnly(comboAccount);

				 //订阅查询账户出入金返回
				IntPtr hWnd=this->Handle;
				m_QueryTransfer2->SubscribeEvt((unsigned int)(hWnd.ToInt32()));
			
		     }
	private: System::Void GridView1_Load()
			 {

				 ////列表添加空白行
				 //for(int i=0; i<15; i++)
				 //{
					// this->dataGridView1->Rows->Add();
				 //}
			 }
    private: System::Void buttonQuery_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //String^ istrt=comboAccount->Text;
			 //const char*pchars = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(istrt)).ToPointer();
			 //std::string strAccount(pchars);
			 //Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)pchars));
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

				 ////向服务器端请求查询账户出入金
				 m_QueryTransfer2->ReqQueryTransfer(setAccntID);
				  dataGridView1->Rows->Clear();
			 }
			 }
			 //得到出入金方向
	String^ GetSeqDepositDirectionString(TFundDirectionType SeqDepositDirection)
		{
			string str ="";
			switch(SeqDepositDirection)
			{
			case T_FD_In:
				str = STR_T_FD_In;
				break;
			case T_FD_Out:
				str = STR_T_FD_Out;
				break;
			
			default:break;
			}
			return Tools::string2String(str.c_str());
		}
	
	//得到出入金类型
	String^ GetSeqDepositTypeString(TFundIOTypeType SeqDepositType)
		{
			string str ="";
			switch(SeqDepositType)
			{
			case T_FIOT_FundIO:
				str = STR_T_FIOT_FundIO;
				break;
			case T_FIOT_Transfer:
				str = STR_T_FIOT_Transfer;
				break;
			
			default:break;
			}
			return Tools::string2String(str.c_str());
		}
//得到出入金有效标志
	String^ GetSeqDepositFlagString(TAvailabilityFlagType SeqDepositFlag)
		{
			string str ="";
			switch(SeqDepositFlag)
			{
			case T_AVAF_Invalid:
				str = STR_T_AVAF_Invalid;
				break;
			case T_AVAF_Valid:
				str = STR_T_AVAF_Valid;
				break;
			case T_AVAF_Repeal:
				str = STR_T_AVAF_Repeal;
				break;
			
			default:break;
			}
			return Tools::string2String(str.c_str());
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

					  if ( head.cmdid == Cmd_RM_QryTraderDeposit_Rsp)
					  {
						  if ( head.userdata1 == OPERATOR_SUCCESS )
						  {
							  int pkgLen=head.len;
							  int AccCnt=pkgLen/sizeof(SeqDepositField);
							  std::map<int, SeqDepositField> mapSeqDeposit;

							  String^ istrt=comboBoxDirection->Text;
							  const char*pchars = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(istrt)).ToPointer();
							  std::string Direction(pchars);
							  Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)pchars));

							  bool brlt=m_QueryTransfer2->ProcessTransferEvtPkg((const char*)m.LParam.ToInt32(),mapSeqDeposit,AccCnt,Direction);
							  if(brlt)
							  {
								  std::map<int, SeqDepositField>::iterator it = mapSeqDeposit.begin();
								  for(it;it!=mapSeqDeposit.end();it++)
								  {

									  int newRowIndex=dataGridView1->Rows->Add();
									  DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
									  dr->Cells[0]->Value = Tools::string2String(it->second.SequenceNo);//序列号
									  dr->Cells[1]->Value = Tools::string2String(it->second.InvestorID);//账号
									  TrustTradeAccount account;
									  memset(&account,0,sizeof(account));
									  m_pdataInfo->GetTrustTradeAccount(it->second.InvestorID,account);
									  AssetMgmtOrganization organ;
									  memset(&organ, 0, sizeof(organ));
									  if(m_pdataInfo->GetAssetMgmtOrg(account.nAssetMgmtOrgID, organ))
										  dr->Cells[2]->Value = Tools::string2String(organ.szName);//账号所属资管
									  FinancialProduct finanProduct;
									  memset(&finanProduct, 0, sizeof(finanProduct));
									  if(m_pdataInfo->GetFinancialProduct(account.nFinancialProductID, finanProduct))
										  dr->Cells[3]->Value = Tools::string2String(finanProduct.szName);//账号所属理财产品
									  dr->Cells[4]->Value = Tools::string2String(it->second.Time);//日期

									  string strAmount = m_QueryTransfer2->Double2String(it->second.Amount);
									  dr->Cells[5]->Value = Tools::string2String(strAmount.c_str());//金额
									  dr->Cells[6]->Value = GetSeqDepositDirectionString(it->second.Direction);//方向
									  dr->Cells[7]->Value = GetSeqDepositFlagString(it->second.Flag);//出入金有效标志
									  dr->Cells[6]->Value = GetSeqDepositTypeString(it->second.Type);//出入金类型
									  dr->Cells[7]->Value = Tools::string2String(it->second.BrokerID);
								  }
							  }
							  //else
							  //{

								 // String^ strtemp = NULL_QUERY_RESULE;
								 // MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);
							  //}

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

private: System::Void comboBoxAccount_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {

		 }
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //显示treeView
			// this->treeView1->Show();
		 }
 private: System::Void textBox1_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //显示treeView
			// this->treeView1->Show();
		 }

private: System::Void buttonClear_Click(System::Object^  sender, System::EventArgs^  e) 
		 {

			dataGridView1->Rows->Clear();

			/* for(int   i=0;i<dataGridView1->Rows->Count;i++)   
			 {   
				 dataGridView1->Rows->RemoveAt(i);  

			 }  */ 
		 }
};
}

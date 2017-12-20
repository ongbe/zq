#pragma once
#include "TcpLayer.h"
#include "CommonDef.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "CDataInfo.h"
#include "Tools.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

	/// <summary>
	/// Summary for FundConfirmForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FundConfirmForm : public System::Windows::Forms::Form
	{
	public:
		FundConfirmForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pLogin = new sLoginRsp();
			m_pDataInfo = CDataInfo::NewInstance();
			InvalideValue=util::GetDoubleInvalidValue();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FundConfirmForm()
		{
			if (components)
			{
				delete components;
			}
			if(m_pLogin)
			{
				delete m_pLogin;
				m_pLogin = nullptr;
			}
		}


	private: sLoginRsp*     m_pLogin;
	private: CDataInfo*     m_pDataInfo;
	private: double         InvalideValue;
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Button^  button_query;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::Button^  button_confirm;
	private: System::Windows::Forms::Button^  button_apply;

	private: System::Windows::Forms::Button^  button_cancel;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column4;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column5;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column14;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column6;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column7;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column8;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column9;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column10;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column12;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column13;




























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
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->button_query = (gcnew System::Windows::Forms::Button());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column4 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column5 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column14 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column6 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column7 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column8 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column9 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column10 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column12 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column13 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->button_confirm = (gcnew System::Windows::Forms::Button());
			this->button_apply = (gcnew System::Windows::Forms::Button());
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(36, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"交易员";
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(100, 13);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(149, 20);
			this->comboBox1->TabIndex = 1;
			// 
			// button_query
			// 
			this->button_query->Location = System::Drawing::Point(377, 10);
			this->button_query->Name = L"button_query";
			this->button_query->Size = System::Drawing::Size(75, 23);
			this->button_query->TabIndex = 2;
			this->button_query->Text = L"查询";
			this->button_query->UseVisualStyleBackColor = true;
			this->button_query->Click += gcnew System::EventHandler(this, &FundConfirmForm::button_query_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(13) {this->Column1, 
				this->Column2, this->Column3, this->Column4, this->Column5, this->Column14, this->Column6, this->Column7, this->Column8, this->Column9, 
				this->Column10, this->Column12, this->Column13});
			this->dataGridView1->ImeMode = System::Windows::Forms::ImeMode::Disable;
			this->dataGridView1->Location = System::Drawing::Point(38, 48);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->Size = System::Drawing::Size(803, 409);
			this->dataGridView1->TabIndex = 3;
			this->dataGridView1->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &FundConfirmForm::dataGridView1_CellEndEdit);
			// 
			// Column1
			// 
			this->Column1->HeaderText = L"账户号";
			this->Column1->Name = L"Column1";
			this->Column1->Width = 61;
			// 
			// Column2
			// 
			this->Column2->HeaderText = L"账户名称";
			this->Column2->Name = L"Column2";
			this->Column2->Width = 61;
			// 
			// Column3
			// 
			this->Column3->HeaderText = L"日期";
			this->Column3->Name = L"Column3";
			this->Column3->Width = 51;
			// 
			// Column4
			// 
			this->Column4->HeaderText = L"期初结存";
			this->Column4->Name = L"Column4";
			this->Column4->Width = 61;
			// 
			// Column5
			// 
			this->Column5->HeaderText = L"出金";
			this->Column5->Name = L"Column5";
			this->Column5->Width = 51;
			// 
			// Column14
			// 
			this->Column14->HeaderText = L"入金";
			this->Column14->Name = L"Column14";
			this->Column14->Width = 51;
			// 
			// Column6
			// 
			this->Column6->HeaderText = L"期末结存";
			this->Column6->Name = L"Column6";
			this->Column6->Width = 61;
			// 
			// Column7
			// 
			this->Column7->HeaderText = L"可用资金";
			this->Column7->Name = L"Column7";
			this->Column7->Width = 61;
			// 
			// Column8
			// 
			this->Column8->HeaderText = L"平仓盈亏";
			this->Column8->Name = L"Column8";
			this->Column8->Width = 61;
			// 
			// Column9
			// 
			this->Column9->HeaderText = L"风险度";
			this->Column9->Name = L"Column9";
			this->Column9->Width = 61;
			// 
			// Column10
			// 
			this->Column10->HeaderText = L"持仓盯市盈亏";
			this->Column10->Name = L"Column10";
			this->Column10->Width = 72;
			// 
			// Column12
			// 
			this->Column12->HeaderText = L"手续费";
			this->Column12->Name = L"Column12";
			this->Column12->Width = 61;
			// 
			// Column13
			// 
			this->Column13->HeaderText = L"占用保证金";
			this->Column13->Name = L"Column13";
			this->Column13->Width = 72;
			// 
			// button_confirm
			// 
			this->button_confirm->Location = System::Drawing::Point(377, 506);
			this->button_confirm->Name = L"button_confirm";
			this->button_confirm->Size = System::Drawing::Size(111, 23);
			this->button_confirm->TabIndex = 4;
			this->button_confirm->Text = L"确认提交";
			this->button_confirm->UseVisualStyleBackColor = true;
			this->button_confirm->Click += gcnew System::EventHandler(this, &FundConfirmForm::button_confirm_Click);
			// 
			// button_apply
			// 
			this->button_apply->Location = System::Drawing::Point(650, 506);
			this->button_apply->Name = L"button_apply";
			this->button_apply->Size = System::Drawing::Size(75, 23);
			this->button_apply->TabIndex = 5;
			this->button_apply->Text = L"应用";
			this->button_apply->UseVisualStyleBackColor = true;
			this->button_apply->Click += gcnew System::EventHandler(this, &FundConfirmForm::button_apply_Click);
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(766, 506);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(75, 23);
			this->button_cancel->TabIndex = 6;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &FundConfirmForm::button_cancel_Click);
			// 
			// FundConfirmForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(875, 546);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->button_apply);
			this->Controls->Add(this->button_confirm);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->button_query);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->label1);
			this->Name = L"FundConfirmForm";
			this->Text = L"资金确认";
			this->Load += gcnew System::EventHandler(this, &FundConfirmForm::FundConfirmForm_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FundConfirmForm::FundConfirmForm_FormClosed);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void FundConfirmForm_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent(); 
				 int nHwnd = this->Handle.ToInt32();
				 //CTcpLayer::SetMainWndHandle(nHwnd);

				 this->Column1->ReadOnly = true;
				 this->Column2->ReadOnly = true;
				 this->Column3->ReadOnly = true;
				 this->Column9->ReadOnly = true;

				 std::map<int, UserInfo>  lMapFiled;
				 m_pDataInfo->GetUserInfo(lMapFiled);
				 std::map<int, UserInfo>::iterator it=  lMapFiled.begin();
				 while (it!=lMapFiled.end())
				 {
					 if ( it->second.nUserType == USER_TYPE_TRADER )
					 {											 
						 string strTemp =it->second.szAccount;
						 string strTempN =it->second.szAlias;
						 strTemp += "+" + strTempN;
						 comboBox1->Items->Add(Tools::string2String(strTemp.c_str()));
					 }
					 
					 it++;
				 }
				 m_pDataInfo->GetLogonInfo(*m_pLogin);
			
				 CTcpLayer::SubscribePkg(Cmd_Settlement_QueryFund_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_ModifyFund_Rsp, nHwnd);

				 if ( !CTcpLayer::SendData(Cmd_Settlement_QueryFund_Req, NULL, 0, 0,m_pLogin->mnUserID))					 
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }

				  m_pDataInfo->ClearFund();

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
					  case Cmd_Settlement_QueryFund_Rsp:
						  {
							  if ( head.userdata1 == CF_ERROR_SUCCESS )
							  {
								  std::map<std::string,PlatformStru_TradingAccountInfo>  lMapField;
								  int nCount = head.len / sizeof(PlatformStru_TradingAccountInfo);
								  if(nCount == 0)
									  break;
								  for ( int i = 0; i < nCount; i++ )
								  {
									  PlatformStru_TradingAccountInfo* pField = (PlatformStru_TradingAccountInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(PlatformStru_TradingAccountInfo));

									  lMapField.insert(make_pair(pField->AccountID,*pField));
								  }
								  m_pDataInfo->SetFund(lMapField);
								   ShowFund();

							  }
							  else
							  {
								  string str = (char*)(p+sizeof(Stru_UniPkgHead));
								  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
									  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
									  str.c_str());
							  }

							  break;
						  }
					  case Cmd_Settlement_ModifyFund_Rsp:
						  {
							  if ( head.userdata1 == CF_ERROR_SUCCESS )
							  {								

								  m_pDataInfo->ClearModifiedFund();


							  }
							  else
							  {
								  string str = (char*)(p+sizeof(Stru_UniPkgHead));
								  String^ strtemp = Tools::string2String(str.c_str());
								  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
									  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
									  str.c_str());
							  }

							  break;
						  }

					  default:
						  break;
					  }

				  }

				  Form::WndProc ( m );
			  }

	 private: System::Void ShowFund()
			  {

				  string strUserID = Tools::String2string(comboBox1->Text);
				  if ( strUserID =="" )
				  {
					  strUserID = SELECT_ALL;
				  }
				  unsigned pos = strUserID.find("+");        
				  std::string str = strUserID.substr ( 0,pos );

				  std::vector<PlatformStru_TradingAccountInfo> lvecField;		 			 
				  lvecField.clear();		
				  m_pDataInfo->GetFund(lvecField,str);

				  dataGridView1->Rows->Clear();
				  std::vector<PlatformStru_TradingAccountInfo>::iterator it;
				  for(it = lvecField.begin(); it != lvecField.end(); it++)
				  {

					  int newRowIndex=dataGridView1->Rows->Add();
					  DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
					  dr->Cells[0]->Value = Tools::string2String((*it).AccountID);
					  UserInfo sUserInfo;
					  m_pDataInfo->GetUserInfoByAccount((*it).AccountID,sUserInfo);
					  dr->Cells[1]->Value = Tools::string2String(sUserInfo.szAlias);
					  dr->Cells[2]->Value = Tools::string2String((*it).TradingDay);

					  char strPrice[128];
					  if ((*it).StaticProfit != InvalideValue)
					  {
						  memset(strPrice,0,sizeof(strPrice));
						  sprintf(strPrice, "%.2f",( *it).StaticProfit);//静态权益==期初结存
						  dr->Cells[3]->Value = Tools::string2String(strPrice);
					  }

					  if ((*it).Withdraw != InvalideValue)
					  {
						  memset(strPrice,0,sizeof(strPrice));
						  sprintf(strPrice, "%.2f",(*it).Withdraw);//出金
						  dr->Cells[4]->Value = Tools::string2String(strPrice);
					  }
					  if ((*it).Deposit!= InvalideValue)
					  {
						  memset(strPrice,0,sizeof(strPrice));
						  sprintf(strPrice, "%.2f",( *it).Deposit);//入金
						  dr->Cells[5]->Value = Tools::string2String(strPrice);
					  }
					  if ((*it).DynamicProfit!= InvalideValue)
					  {
						  memset(strPrice,0,sizeof(strPrice));
						  sprintf(strPrice, "%.2f",( *it).DynamicProfit);
						  dr->Cells[6]->Value = Tools::string2String(strPrice);//期末结存
					  }
					  if ((*it).Available!= InvalideValue)
					  {
						  memset(strPrice,0,sizeof(strPrice));
						  sprintf(strPrice, "%.2f",( *it).Available);
						  dr->Cells[7]->Value = Tools::string2String(strPrice);
					  }
					  if ((*it).CloseProfit!= InvalideValue)
					  {
						  memset(strPrice,0,sizeof(strPrice));
						  sprintf(strPrice, "%.2f",( *it).CloseProfit);
						  dr->Cells[8]->Value = Tools::string2String(strPrice);
					  }
					  if ((*it).RiskDegree!= InvalideValue)
					  {
						  memset(strPrice,0,sizeof(strPrice));
						  if(util::isInvalidValue(( *it).RiskDegree))//风险度
							  sprintf(strPrice, "%s","-");
						  else
							  sprintf(strPrice, "%.2f%%",( *it).RiskDegree);

						  dr->Cells[9]->Value = Tools::string2String(strPrice);
					  }
					  if ((*it).PositionProfit!= InvalideValue)
					  {
						  memset(strPrice,0,sizeof(strPrice));
						  sprintf(strPrice, "%.2f",( *it).PositionProfit);
						  dr->Cells[10]->Value = Tools::string2String(strPrice);
					  }
					  if ((*it).Commission!= InvalideValue)
					  {
						  memset(strPrice,0,sizeof(strPrice));
						  sprintf(strPrice, "%.2f",( *it).Commission);
						  dr->Cells[11]->Value = Tools::string2String(strPrice);
					  }
					  if ((*it).CurrMargin!= InvalideValue)
					  {
						  memset(strPrice,0,sizeof(strPrice));
						  sprintf(strPrice, "%.2f",( *it).CurrMargin);
						  dr->Cells[12]->Value = Tools::string2String(strPrice);
					  }
				  }
			  }
	private: System::Void button_query_Click(System::Object^  sender, System::EventArgs^  e) 
			 {

				 ShowFund();

			 }
	private: System::Void button_confirm_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"是否确定资金？", "提示", MessageBoxButtons::YesNo);
				 if ( dr == System::Windows::Forms::DialogResult::Yes)
				 { 
					 std::map<std::string,PlatformStru_TradingAccountInfo> lMapField;
					 m_pDataInfo->GteModifiedFund(lMapField);
					 if ( !lMapField.empty())
					 {	 
						 std::vector<PlatformStru_TradingAccountInfo> lVecField;

						 std::map<std::string,PlatformStru_TradingAccountInfo>::iterator lIter = lMapField.begin();
						 while(lIter != lMapField.end())
						 {
							 lVecField.push_back(lIter->second);
							 lIter++;
						 }
						 m_pDataInfo->GetLogonInfo(*m_pLogin);	
						 if ( !CTcpLayer::SendData(Cmd_Settlement_ModifyFund_Req,&lVecField[0],sizeof(PlatformStru_TradingAccountInfo)*lVecField.size(), 0, m_pLogin->mnUserID))
						 {
							 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
						 }
						 std::map<std::string,PlatformStru_TradingAccountInfo>::iterator it =lMapField.begin();
						 while (it!=lMapField.end())
						 {
							 m_pDataInfo->SetFund(it->second);
							 it++;
						 }

						 m_pDataInfo->ClearModifiedFund();

					 }



					 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
					 this->Close();
				 }
			 }
	private: System::Void dataGridView1_CellEndEdit(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
			 {
				 if (dataGridView1->Rows[e->RowIndex]->Cells[e->ColumnIndex]->FormattedValue == "")
				 {
					 MessageBox::Show("请输入修改的资金值！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 return;
				 }

				 PlatformStru_TradingAccountInfo Field;
				 memset(&Field,0,sizeof(Field));
				 string strAccount = Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[0]->Value->ToString());
				 m_pDataInfo->GetFund(Field,strAccount);
				 double fundData = atof(Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[e->ColumnIndex]->Value->ToString()).c_str());

				 switch(e->ColumnIndex)
				 {
				 case  3:
					 {
						 if ( Field.StaticProfit == fundData )
							 return;
						 else
							 Field.StaticProfit = fundData;

						 break;
					 }

				 case 4:
					 {
						 if ( Field.Withdraw == fundData )
							 return;
						 else
							 Field.Withdraw = fundData;

						 break;
					 }
				 case 5:
					 {
						 if ( Field.Deposit == fundData )
							 return;
						 else
							 Field.Deposit = fundData;

						 break;
					 }
				 case 6:
					 {
						 if ( Field.DynamicProfit == fundData )
							 return;
						 else
						 {
							 Field.DynamicProfit = fundData;
							 Field.RiskDegree = ( Field.CurrMargin + 
								 Field.DeliveryMargin )
								 *100
								 /Field.DynamicProfit;
							 char strPrice[128];
							 memset(strPrice,0,sizeof(strPrice));
							 if(util::isInvalidValue(Field.RiskDegree) || Field.DynamicProfit == 0.0 )//风险度
								 sprintf(strPrice, "%s","-");
							 else
								 sprintf(strPrice, "%.2f%%",Field.RiskDegree);
							 dataGridView1->Rows[e->RowIndex]->Cells[9]->Value = Tools::string2String(strPrice);
						 }

						 break;
					 }
				 case 7:
					 {
						 if ( Field.Available == fundData )
							 return;
						 else
							 Field.Available = fundData;

						 break;
					 }
				 case 8:
					 {
						 if ( Field.CloseProfit == fundData )
							 return;
						 else
							 Field.CloseProfit = fundData;

						 break;
					 }

				 case 10:
					 {
						 if ( Field.PositionProfit == fundData )
							 return;
						 else
							 Field.PositionProfit = fundData;

						 break;
					 }
				 case 11:
					 {
						 if ( Field.Commission == fundData )
							 return;
						 else
							 Field.Commission = fundData;

						 break;
					 }

				 case 12:
					 {
						 if ( Field.CurrMargin == fundData )
							 return;
						 else
						 {
							 Field.CurrMargin = fundData;
							 Field.RiskDegree = ( Field.CurrMargin + 
								 Field.DeliveryMargin )
								 *100
								 /Field.DynamicProfit;
							 char strPrice[128];
							 memset(strPrice,0,sizeof(strPrice));
							 if(util::isInvalidValue(Field.RiskDegree))//风险度
								 sprintf(strPrice, "%s","-");
							 else
								 sprintf(strPrice, "%.2f%%",Field.RiskDegree);
							 dataGridView1->Rows[e->RowIndex]->Cells[9]->Value = Tools::string2String(strPrice);
						 }

						 break;
					 }
				 }
				 if (dataGridView1->Rows[e->RowIndex]->Cells[3]->Value != nullptr)
				 {
					 Field.StaticProfit = atof(Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[3]->Value->ToString()).c_str());
				 }
				 if (dataGridView1->Rows[e->RowIndex]->Cells[4]->Value != nullptr)
				 {
					 Field.Withdraw = atof(Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[4]->Value->ToString()).c_str());
				 }
				 if (dataGridView1->Rows[e->RowIndex]->Cells[5]->Value != nullptr)
				 {
					 Field.Deposit = atof(Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[5]->Value->ToString()).c_str());
				 }
				 if (dataGridView1->Rows[e->RowIndex]->Cells[6]->Value != nullptr)
				 {
					 Field.DynamicProfit = atof(Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[6]->Value->ToString()).c_str());
				 }
				 if (dataGridView1->Rows[e->RowIndex]->Cells[7]->Value != nullptr)
				 {
					 Field.Available = atof(Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[7]->Value->ToString()).c_str());
				 }
				 if (dataGridView1->Rows[e->RowIndex]->Cells[8]->Value != nullptr)
				 {
					 Field.CloseProfit = atof(Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[8]->Value->ToString()).c_str());
				 }
				 if (dataGridView1->Rows[e->RowIndex]->Cells[10]->Value != nullptr)
				 {
					 Field.PositionProfit = atof(Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[10]->Value->ToString()).c_str());
				 }
				 if (dataGridView1->Rows[e->RowIndex]->Cells[11]->Value != nullptr)
				 {
					 Field.Commission = atof(Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[11]->Value->ToString()).c_str());
				 }
				 if (dataGridView1->Rows[e->RowIndex]->Cells[12]->Value != nullptr)
				 {
					 Field.CurrMargin = atof(Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[12]->Value->ToString()).c_str());
				 }
				 m_pDataInfo->SetModifiedFund(Field);
				 m_pDataInfo->SetFund(Field);

			 }

	private: System::Void button_apply_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 std::map<std::string,PlatformStru_TradingAccountInfo> lMapField;
				 m_pDataInfo->GteModifiedFund(lMapField);
				 if (lMapField.empty())
					 return;
				 std::vector<PlatformStru_TradingAccountInfo> lVecField;

				 std::map<std::string,PlatformStru_TradingAccountInfo>::iterator lIter = lMapField.begin();
				 while(lIter != lMapField.end())
				 {
					 lVecField.push_back(lIter->second);
					 lIter++;
				 }
				 m_pDataInfo->GetLogonInfo(*m_pLogin);	
				 if ( !CTcpLayer::SendData(Cmd_Settlement_ModifyFund_Req,&lVecField[0],sizeof(PlatformStru_TradingAccountInfo)*lVecField.size(), 0, m_pLogin->mnUserID))
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }
			 }
	private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 std::map<std::string,PlatformStru_TradingAccountInfo> lMapField;
				 m_pDataInfo->GteModifiedFund(lMapField);
				 if ( lMapField.size() > 0)
				 {
					 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"您已做数据修改，但是未保存，是否取消操作并关闭窗口？", "提示", MessageBoxButtons::YesNo);
					 if ( dr == System::Windows::Forms::DialogResult::Yes)
					 { 
						 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
						 m_pDataInfo->ClearModifiedFund();
						 this->Close();
					 }
				 }
				 else
					 this->Close();
			 }
	private: System::Void FundConfirmForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
			 {
				 std::map<std::string,PlatformStru_TradingAccountInfo> lMapField;
				 m_pDataInfo->GteModifiedFund(lMapField);
				 if ( lMapField.size() > 0)
				 {
					 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"您已做数据修改，但是未保存，是否取消操作并关闭窗口？", "提示", MessageBoxButtons::YesNo);
					 if ( dr == System::Windows::Forms::DialogResult::Yes)
					 { 
						 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
						 m_pDataInfo->ClearModifiedFund();
						 this->Close();
					 }
				 }
				 else
					 this->Close();

				 IntPtr hWnd=this->Handle;
				 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
				 
			 }
	};
}

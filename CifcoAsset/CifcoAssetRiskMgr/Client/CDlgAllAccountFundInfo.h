#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
#include <set>
#include <map>
#include <queue>
#include "defineGridCols.h"
#include "CCommonFunc.h"
#include "TcpLayer.h"
#include "CDataInfo.h"
#include "CFieldDetail.h"
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "CommonPkg.h"
#include "Tools.h"
#include "CDlgAllAccountFundInfoData.h"
namespace Client {

	/// <summary>
	/// Summary for CDlgAllAccountFundInfo
	/// </summary>
	public ref class CDlgAllAccountFundInfo : public System::Windows::Forms::Form
	{
	public:
		CDlgAllAccountFundInfo(void)
		{
			InitializeComponent();
			InitGridHead();
			m_pdataInfo = CDataInfo::NewInstance();		
			save = CDlgAllAccountFundInfoData::NewInstance();
		//	SetAllQueryAccout();
		//	SubscribeData();

			IntPtr hWnd=this->Handle;
			CTcpLayer::SubscribePkg(Cmd_RM_SubscribeFund_Rsp, (int)hWnd);
			CTcpLayer::SubscribePkg(Cmd_RM_Fund_Push, (int)hWnd);
			CTcpLayer::SubscribePkg(Cmd_RM_FundNetValue_push, (int)hWnd);
			CTcpLayer::SubscribePkg(Cmd_RM_UnSubscribeFund_Rsp, (int)hWnd);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CDlgAllAccountFundInfo()
		{
			if (components)
			{
				delete components;
			}
			CDlgAllAccountFundInfoData::ReleaseInstance(save);
		}
		
	private: CDataInfo* m_pdataInfo;
	private: CDlgAllAccountFundInfoData* save;
	private: System::Windows::Forms::DataGridView^  dataGridFund;
	private: System::Windows::Forms::Label^  label1;
	protected: 

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
			this->dataGridFund = (gcnew System::Windows::Forms::DataGridView());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridFund))->BeginInit();
			this->SuspendLayout();
			// 
			// dataGridFund
			// 
			this->dataGridFund->AllowUserToAddRows = false;
			this->dataGridFund->AllowUserToDeleteRows = false;
			this->dataGridFund->AllowUserToResizeRows = false;
			this->dataGridFund->BackgroundColor = System::Drawing::SystemColors::Control;
			this->dataGridFund->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->dataGridFund->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridFund->EditMode = System::Windows::Forms::DataGridViewEditMode::EditProgrammatically;
			this->dataGridFund->Location = System::Drawing::Point(5, 15);
			this->dataGridFund->Margin = System::Windows::Forms::Padding(1);
			this->dataGridFund->MultiSelect = false;
			this->dataGridFund->Name = L"dataGridFund";
			this->dataGridFund->ReadOnly = true;
			this->dataGridFund->RowHeadersVisible = false;
			this->dataGridFund->RowTemplate->Height = 23;
			this->dataGridFund->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridFund->Size = System::Drawing::Size(491, 367);
			this->dataGridFund->TabIndex = 18;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(3, 2);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 12);
			this->label1->TabIndex = 19;
			this->label1->Text = L"资金账户信息";
			// 
			// CDlgAllAccountFundInfo
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::GrayText;
			this->ClientSize = System::Drawing::Size(489, 370);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->dataGridFund);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Name = L"CDlgAllAccountFundInfo";
		//	this->Load += gcnew System::EventHandler(this, &CDlgAllAccountFundInfo::CDlgRealTimeFundInfo_Load);
			this->SizeChanged += gcnew System::EventHandler(this, &CDlgAllAccountFundInfo::CDlgAllAccountFundInfo_SizeChanged);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridFund))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void CDlgAllAccountFundInfo_SizeChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 int wid=this->Size.Width;
				 int hei=this->Size.Height;
				 int pox=this->Location.X;
				 int poy=this->Location.Y;

				 dataGridFund->SetBounds(pox, 20, wid, hei-20);
			 }
	protected:virtual void WndProc( Message% m ) override 
			  {
				  if(m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival)
				  {
					  char *p = (char*)m.LParam.ToInt32();
					  Stru_UniPkgHead head;

					  memset(&head, 0, sizeof(head));
					  memcpy(&head, p, sizeof(head));
					  switch(head.cmdid) 
					  {
						case Cmd_RM_SubscribeFund_Rsp:
							{
			//					dwTcpEndTime = GetTickCount();
								if(head.userdata1 == OPERATOR_SUCCESS) 
								{
									RiskAllAccountField* pFundInfo = NULL;
									int nItemCount = head.len/sizeof(RiskAllAccountField);

									save->LockObject();
									for(int i=0; i<nItemCount; i++) 
									{
										pFundInfo = (RiskAllAccountField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(RiskAllAccountField));
										
										TrustTradeAccount traderAccount;
										if(!m_pdataInfo->GetTrustTradeAccount(std::string(pFundInfo->pre.InvestorID), traderAccount)) 
											continue;
										std::set<int>::iterator it = save->setQueryAccountID.find(traderAccount.nTradeAccountID);
										if( it== save->setQueryAccountID.end())//判断是不是这个页面查询的，不是则返回
											continue;

										save->queueAllFundInfo.push(*pFundInfo);
									}
									save->UnlockObject();
									if(head.userdata2 == head.userdata3 - 1)
									{
								//		dwShowStartTime = GetTickCount();
										std::queue<RiskAllAccountField> queueRet;
										save->CheckNewData(m_pdataInfo, queueRet);
										ShowQueue(queueRet);
								//		dwShowEndTime = GetTickCount();
										{
								//			char csText[1024] = {0};
									//		sprintf(csText, "实时资金查询，本次查询时间消耗 传输==%u, 显示==%u",
									//			dwTcpEndTime-dwTcpStartTime, dwShowEndTime-dwShowStartTime);
									//		this->Text = Tools::string2String(csText);
										}
									}
								}
								else 
								{
									p[sizeof(head)+head.len-1]=0;
									//TRACE(p+sizeof(head));
								}
							}
							break;
						case Cmd_RM_UnSubscribeFund_Rsp:
							if(head.userdata1 == OPERATOR_SUCCESS)
							{
							}
							else 
							{
								p[sizeof(head)+head.len-1]=0;
								//TRACE(p+sizeof(head));
							}
							break;
						case Cmd_RM_Fund_Push:
							{
								if(head.userdata1 == OPERATOR_SUCCESS)
								{
									RiskSyncAccountField* pFundInfo = NULL;
									int nItemCount = head.len/sizeof(RiskSyncAccountField);

									save->LockObject();
									for(int i=0; i<nItemCount; i++) 
									{
										pFundInfo = (RiskSyncAccountField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(RiskSyncAccountField));

										TrustTradeAccount traderAccount;
										if(!m_pdataInfo->GetTrustTradeAccount(std::string(pFundInfo->InvestorID), traderAccount)) 
											continue;
										std::set<int>::iterator it = save->setQueryAccountID.find(traderAccount.nTradeAccountID);
										if( it== save->setQueryAccountID.end())//判断是不是这个页面查询的，不是则返回
											continue;

										save->queueLastFundInfo.push(*pFundInfo);
									}
									save->UnlockObject();

									std::queue<RiskSyncAccountField> queueRet;
									save->CheckNewData(m_pdataInfo, queueRet);
									ShowQueue(queueRet);

								}
								break;
							case Cmd_RM_FundNetValue_push:
							{
								if(head.userdata1 == OPERATOR_SUCCESS)
								{
									NetValueInfo* pNetValueInfo = NULL;
									int nItemCount = head.len/sizeof(NetValueInfo);

									save->LockObject();
									for(int i=0; i<nItemCount; i++) 
									{
										pNetValueInfo = (NetValueInfo*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(NetValueInfo));
										std::string strKey = pNetValueInfo->szAccount;
										std::map<std::string, int>::iterator it = save->mapKey2Index.find(strKey);
										if(it == save->mapKey2Index.end()) 
										   continue;
										int nIndex = GetRowIndexByRowID(it->second);
										int nCol = -1;
										if((nCol = GetColIndexByColID(conCol_RTQA_NETVALUE))>=0) 
										{
											DataGridViewRow^ dr = dataGridFund->Rows[nIndex];
											char szValue[128];
											memset(szValue, 0, sizeof(szValue));
											sprintf(szValue, "%0.6f", pNetValueInfo->dbValue);
											dr->Cells[nCol]->Value = Tools::string2String(szValue);
										}
										
									}
									save->UnlockObject();
								}
							}
							}
							break;
						 };
				  }

				  Form::WndProc(m);
			  }
			  int GetRowIndexByRowID(int nRowID) {
				  for(int i=0; i<dataGridFund->Rows->Count; i++) 
				  {
					  if(int::Parse(dataGridFund->Rows[i]->Tag->ToString()) == nRowID)
						  return dataGridFund->Rows[i]->Index;
				  }
				  return -1;
			  }
			  int GetColIndexByColID(int nColID) {
				  for(int i=0; i<dataGridFund->Columns->Count; i++)
				  {
					  if(int::Parse(dataGridFund->Columns[i]->Tag->ToString()) == nColID)
						  return dataGridFund->Columns[i]->Index;
				  }
				  return -1;
			  }
			  void InitGridHead() 
			  {

				  dataGridFund->Columns->Clear();

				  int nColID = 0;

				  System::Windows::Forms::DataGridViewTextBoxColumn^ gridColumn = 
					  (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_InvestorID;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"投资者代码";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_InvestorID));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_NETVALUE;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"基金净值";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_NETVALUE));
				  dataGridFund->Columns->Add(gridColumn);


				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_Balance;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"总权益";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_Balance));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_Withdraw;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"出入金金额";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_Withdraw));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_CurrMargin;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"当前保证金总额";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_CurrMargin));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_PositionProfit;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"持仓盈亏";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_PositionProfit));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_CloseProfit;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"平仓盈亏";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_CloseProfit));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_AssertMgmtOrgan;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"账号所属资管";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_AssertMgmtOrgan));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_FinanProduct;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"账号所属理财产品";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_FinanProduct));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_BrokerID;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"经纪公司代码";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_BrokerID));
				  dataGridFund->Columns->Add(gridColumn);

				
				

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_Commission;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"手续费";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_Commission));
				  dataGridFund->Columns->Add(gridColumn);

				  

				 
				 

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_Available;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"可用资金";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_Available));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_Mortgage;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"质押金额";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_Mortgage));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_ExchangeMargin;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"交易所保证金";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_ExchangeMargin));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_WithdrawQuota;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"可取资金";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_WithdrawQuota));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_Credit;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"信用额度";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_Credit));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_FrozenMargin;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"保证金冻结";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_FrozenMargin));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_FrozenCommission;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"手续费冻结";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_FrozenCommission));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_AccountID;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"投资者帐号";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_AccountID));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_PreMortgage;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"上次质押金额";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_PreMortgage));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_PreCredit;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"上次信用额度";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_PreCredit));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_PreDeposit;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"上次存款额";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_PreDeposit));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_PreBalance;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"上次总权益";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_PreBalance));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_PreMargin;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"上次占用的保证金";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_PreMargin));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_Reserve;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"基本准备金";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_Reserve));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_PreExchMargin;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"上次交易所保证金";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_PreExchMargin));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_ForceCloseStat;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"历史强平次数";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_ForceCloseStat));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_DeliveryMargin;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"投资者交割保证金";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_DeliveryMargin));
				  dataGridFund->Columns->Add(gridColumn);

				  gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
				  nColID = conCol_RTQA_ExchangeDeliveryMargin;
				  gridColumn->Tag = nColID;
				  gridColumn->HeaderText = L"交易所交割保证金";
				  gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_RTQA_ExchangeDeliveryMargin));
				  dataGridFund->Columns->Add(gridColumn);

			  }
			  void UpdateRow(int nIndex, RiskSyncAccountField& lastFund, SeqPreRiskAccountField& preFund, bool bOnlyLastFund) 
			  {
#if 1
				  if(nIndex<0)
					  return;
				  int nCol = -1;
				  std::string strText;
				  DataGridViewRow^ dr = dataGridFund->Rows[nIndex];
				  char strTemp[1024];

				  if((nCol = GetColIndexByColID(conCol_RTQA_InvestorID))>=0) 
				  {
					  dr->Cells[nCol]->Value = Tools::string2String(lastFund.InvestorID);
				  }
				  TrustTradeAccount traderAccount;
				  memset(&traderAccount, 0, sizeof(traderAccount));
				  if(m_pdataInfo->GetTrustTradeAccount(std::string(lastFund.InvestorID), traderAccount))
				  {
					  AssetMgmtOrganization organ;
					  memset(&organ, 0, sizeof(organ));
					  if(m_pdataInfo->GetAssetMgmtOrg(traderAccount.nAssetMgmtOrgID, organ))
					  {
						  if((nCol = GetColIndexByColID(conCol_RTQA_AssertMgmtOrgan))>=0)
						  {
							  dr->Cells[nCol]->Value = Tools::string2String(organ.szName);
						  }
					  }
					  FinancialProduct finanProduct;
					  memset(&finanProduct, 0, sizeof(finanProduct));
					  if(m_pdataInfo->GetFinancialProduct(traderAccount.nFinancialProductID, finanProduct)) 
					  {
						  if((nCol = GetColIndexByColID(conCol_RTQA_FinanProduct))>=0) 
						  {
							  dr->Cells[nCol]->Value = Tools::string2String(finanProduct.szName);
						  }
					  }
				  }
				  if((nCol = GetColIndexByColID(conCol_RTQA_BrokerID))>=0)
				  {
					  dr->Cells[nCol]->Value = Tools::string2String(lastFund.BrokerID);
				  }
				  if((nCol = GetColIndexByColID(conCol_RTQA_Withdraw))>=0) 
				  {
					  sprintf(strTemp, "%0.2f", lastFund.Withdraw);
					  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
				  }
				  if((nCol = GetColIndexByColID(conCol_RTQA_CurrMargin))>=0)
				  {
					  sprintf(strTemp, "%0.2f", lastFund.CurrMargin);
					  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
				  }
				  if((nCol = GetColIndexByColID(conCol_RTQA_Commission))>=0)
				  {
					  sprintf(strTemp, "%0.2f", lastFund.Commission);
					  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
				  }
				  if((nCol = GetColIndexByColID(conCol_RTQA_CloseProfit))>=0)
				  {
					  sprintf(strTemp, "%0.2f", lastFund.CloseProfit);
					  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
				  }
				  if((nCol = GetColIndexByColID(conCol_RTQA_PositionProfit))>=0)
				  {
					  sprintf(strTemp, "%0.2f", lastFund.PositionProfit);
					  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
				  }
				  if((nCol = GetColIndexByColID(conCol_RTQA_Balance))>=0) 
				  {
					  sprintf(strTemp, "%0.2f", lastFund.Balance);
					  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
				  }
				  if((nCol = GetColIndexByColID(conCol_RTQA_Available))>=0) 
				  {
					  sprintf(strTemp, "%0.2f", lastFund.Available);
					  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
				  }
				  if((nCol = GetColIndexByColID(conCol_RTQA_Mortgage))>=0)
				  {
					  sprintf(strTemp, "%0.2f", lastFund.Mortgage);
					  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
				  }
				  if((nCol = GetColIndexByColID(conCol_RTQA_ExchangeMargin))>=0)
				  {
					  sprintf(strTemp, "%0.2f", lastFund.ExchangeMargin);
					  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
				  }
				  if((nCol = GetColIndexByColID(conCol_RTQA_WithdrawQuota))>=0)
				  {
					  sprintf(strTemp, "%0.2f", lastFund.WithdrawQuota);
					  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
				  }
				  if((nCol = GetColIndexByColID(conCol_RTQA_Credit))>=0)
				  {
					  sprintf(strTemp, "%0.2f", lastFund.Credit);
					  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
				  }
				  if((nCol = GetColIndexByColID(conCol_RTQA_FrozenMargin))>=0)
				  {
					  sprintf(strTemp, "%0.2f", lastFund.FrozenMargin);
					  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
				  }
				  if((nCol = GetColIndexByColID(conCol_RTQA_FrozenCommission))>=0)
				  {
					  sprintf(strTemp, "%0.2f", lastFund.FrozenCommission);
					  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
				  }

				  if(!bOnlyLastFund) {
					  if((nCol = GetColIndexByColID(conCol_RTQA_AccountID))>=0)
					  {
						  dr->Cells[nCol]->Value = Tools::string2String(preFund.AccountID);
					  }
					  if((nCol = GetColIndexByColID(conCol_RTQA_PreMortgage))>=0)
					  {
						  sprintf(strTemp, "%0.2f", preFund.PreMortgage);
						  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
					  }
					  if((nCol = GetColIndexByColID(conCol_RTQA_PreCredit))>=0)
					  {
						  sprintf(strTemp, "%0.2f", preFund.PreCredit);
						  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
					  }
					  if((nCol = GetColIndexByColID(conCol_RTQA_PreBalance))>=0)
					  {
						  sprintf(strTemp, "%0.2f", preFund.PreBalance);
						  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
					  }
					  if((nCol = GetColIndexByColID(conCol_RTQA_PreDeposit))>=0)
					  {
						  sprintf(strTemp, "%0.2f", preFund.PreDeposit);
						  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
					  }
					  if((nCol = GetColIndexByColID(conCol_RTQA_PreMargin))>=0) 
					  {
						  sprintf(strTemp, "%0.2f", preFund.PreMargin);
						  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
					  }
					  if((nCol = GetColIndexByColID(conCol_RTQA_Reserve))>=0)
					  {
						  sprintf(strTemp, "%0.2f", preFund.Reserve);
						  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
					  }
					  if((nCol = GetColIndexByColID(conCol_RTQA_PreExchMargin))>=0) 
					  {
						  sprintf(strTemp, "%0.2f", preFund.PreExchMargin);
						  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
					  }
					  if((nCol = GetColIndexByColID(conCol_RTQA_ForceCloseStat))>=0) 
					  {
						  sprintf(strTemp, "%0d", preFund.ForceCloseStat);
						  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
					  }
					  if((nCol = GetColIndexByColID(conCol_RTQA_DeliveryMargin))>=0)
					  {
						  sprintf(strTemp, "%0.2f", preFund.DeliveryMargin);
						  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
					  }
					  if((nCol = GetColIndexByColID(conCol_RTQA_ExchangeDeliveryMargin))>=0) 
					  {
						  sprintf(strTemp, "%0.2f", preFund.ExchangeDeliveryMargin);
						  dr->Cells[nCol]->Value = Tools::string2String(strTemp);
					  }
				  }
#endif
			  }
			  void ShowQueue(std::queue<RiskAllAccountField>& queueRet) 
			  {
				  std::map<string, int>::iterator it;
				  int nIndex = -1;
				  char strTemp[1024];
				  std::string strKey;
				  strKey.reserve(1024);
				  save->LockObject();
				  while(!queueRet.empty()) 
				  {
					  RiskAllAccountField field = queueRet.front();
					  queueRet.pop();

					  memset(strTemp, 0, sizeof(strTemp));
					  sprintf(strTemp, "%s", field.cur.InvestorID);
					  strKey = strTemp;
					  it = save->mapKey2Index.find(strKey);
					  if(it != save->mapKey2Index.end()) 
					  {
						  nIndex = GetRowIndexByRowID(it->second);
					  }
					  else 
					  {
						  DataGridViewRow^ newRow = gcnew DataGridViewRow;
						  int nRowID = save->GetNextRowID();
						  newRow->Tag = nRowID;
						  nIndex = dataGridFund->Rows->Add(newRow);
						  if(nIndex>=0) 
						  {
							  save->mapKey2Index.insert(std::make_pair(strKey, nRowID));
							  save->mapIndex2Key.insert(std::make_pair(nRowID, strKey));
						  }
					  }
					  if(nIndex>=0) 
					  {
						  SeqPreRiskAccountField preFund;
						  memset(&preFund, 0, sizeof(preFund));
						  UpdateRow(nIndex, field.cur, field.pre, false);
					  }
				  }
				  save->UnlockObject();
			  }
			  void ShowQueue(std::queue<RiskSyncAccountField>& queueRet) 
			  {
				  std::map<string, int>::iterator it;
				  int nIndex = -1;
				  char strTemp[1024];
				  std::string strKey;
				  strKey.reserve(1024);
				  save->LockObject();
				  while(!queueRet.empty())
				  {
					  RiskSyncAccountField field = queueRet.front();
					  queueRet.pop();

					  memset(strTemp, 0, sizeof(strTemp));
					  sprintf(strTemp, "%s", field.InvestorID);
					  strKey = strTemp;
					  it = save->mapKey2Index.find(strKey);
					  if(it != save->mapKey2Index.end()) 
					  {
						  nIndex = GetRowIndexByRowID(it->second);
					  }
					  else 
					  {
						  DataGridViewRow^ newRow = gcnew DataGridViewRow;
						  int nRowID = save->GetNextRowID();
						  newRow->Tag = nRowID;
						  nIndex = dataGridFund->Rows->Add(newRow);
						  if(nIndex>=0)
						  {
							  save->mapKey2Index.insert(std::make_pair(strKey, nRowID));
							  save->mapIndex2Key.insert(std::make_pair(nRowID, strKey));
						  }
					  }
					  if(nIndex>=0) 
					  {
						  SeqPreRiskAccountField preFund;
						  memset(&preFund, 0, sizeof(preFund));
						  UpdateRow(nIndex, field, preFund, true);
					  }
				  }
				  save->UnlockObject();
			  }
public:
void SetAllQueryAccout()
{
	MAPTrustTradeAccount mapTrustTradeAccount;
	m_pdataInfo->GetTrustTradeAccount(mapTrustTradeAccount);	

	LoginRsp loginRsp;
	m_pdataInfo->GetLoginRsp(loginRsp); 
	MAPTrustTradeAccount::iterator it = mapTrustTradeAccount.begin();
	for(; it != mapTrustTradeAccount.end(); )
	{//不监控资管组织iD和理财产品与登录风控员不一样的情况
		TrustTradeAccount tAccount = it->second;
		if(tAccount.nAssetMgmtOrgID != loginRsp.nAssetMgmtOrgID 
			|| tAccount.nFinancialProductID != loginRsp.nFinancialProductID)
			it = mapTrustTradeAccount.erase(it);
		else
			++it;										
	}

	int nSize = mapTrustTradeAccount.size();
	if(nSize >0)
	{
		//int *p = new int[nSize];
		int i = 0;
		for(MAPTrustTradeAccount::iterator it = mapTrustTradeAccount.begin(); it != mapTrustTradeAccount.end(); it++)
		{
			int nAccount = it->first;
		//	p[i++] = nAccount;
			save->setQueryAccountID.insert(nAccount);
		
		}
	
		//delete []p;
	}
}
	private: System::Void CDlgRealTimeFundInfo_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 InitGridHead();

/*				 dlgFinancialProductIDList = gcnew CFinancialProductIDListDlg();
				 dlgFinancialProductIDList->ResetRelateCombo(comboFinaProduct);
				 comboFinaProduct->ChildControl = dlgFinancialProductIDList;

				 dlgMgmtOrganIDTreeList = gcnew CAssetMgmtOrganIDTreeListDlg();
				 dlgMgmtOrganIDTreeList->ResetFinaProductDlg(dlgFinancialProductIDList);
				 dlgMgmtOrganIDTreeList->ResetRelateCombo(comboOrganization);
				 comboOrganization->ChildControl = dlgMgmtOrganIDTreeList;

				 dlgOrganAndAccountIDTreeList = gcnew COrganAndAccountIDTreeListDlg();
				 dlgOrganAndAccountIDTreeList->ResetRelateCombo(comboAccount);
				 comboAccount->ChildControl = dlgOrganAndAccountIDTreeList;
*/
				 IntPtr hWnd=this->Handle;
				 CTcpLayer::SubscribePkg(Cmd_RM_SubscribeFund_Rsp, (int)hWnd);
				 CTcpLayer::SubscribePkg(Cmd_RM_Fund_Push, (int)hWnd);
				 CTcpLayer::SubscribePkg(Cmd_RM_FundNetValue_push, (int)hWnd);
				 CTcpLayer::SubscribePkg(Cmd_RM_UnSubscribeFund_Rsp, (int)hWnd);
/*
				 ResetComboBoxTextReadOnly(comboFinaProduct);
				 ResetComboBoxTextReadOnly(comboOrganization);
				 ResetComboBoxTextReadOnly(comboAccount);

				 comboOrganization->Enabled = false;
				 comboFinaProduct->Enabled = false;
				 comboAccount->Enabled = true;*/

#ifndef _DEBUG
	//			 if(m_pdataInfo!=NULL)
	//				 btnQuery->Enabled = m_pdataInfo->HasPermission(TODAY_ACCOUNT_QUERY);
#endif
			 }
	};

}

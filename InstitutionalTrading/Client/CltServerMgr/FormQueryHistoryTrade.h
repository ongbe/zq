#pragma once

#include "TcpLayer.h"
#include "defineGridCols.h"
#include "CCommonFunc.h"
#include "CDataInfo.h"
#include "FormQueryHistoryTradeData.h"
#include "PanelOrganDealerTree.h"
#include "FieldDetail.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
//using namespace System::Drawing;


namespace CltServerMgr {

	/// <summary>
	/// Summary for FormQueryHistoryTrade
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormQueryHistoryTrade : public System::Windows::Forms::Form
	{
	public:
		FormQueryHistoryTrade(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			save = NULL;
			m_pdataInfo = CDataInfo::NewInstance();
			m_bHasExit = false;
			m_bHasChanged_QueryParam = true;
			save = FormQueryHistoryTradeData::NewInstance();
			dTableTrade = gcnew DataTable();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormQueryHistoryTrade()
		{
			if (components)
			{
				delete components;
			}
		}
	private: CDataInfo* m_pdataInfo;
	private: FormQueryHistoryTradeData* save;
	private: bool m_bQuerying;
	private: bool m_bHasExit;
	private: bool m_bHasChanged_QueryParam;
	private: unsigned long dwTcpStartTime;
	private: unsigned long dwTcpEndTime;
	private: unsigned long dwShowStartTime;
	private: unsigned long dwShowEndTime;
	private: DataTable ^ dTableTrade;

	private: PanelOrganDealerTree^  panelOrganDealerTreeList;

	protected: 
	private: System::Windows::Forms::Button^  btnResetZero;
	private: System::Windows::Forms::Button^  btnQuery;
	private: System::Windows::Forms::DateTimePicker^  dtpEnd;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::DateTimePicker^  dtpStart;
	private: System::Windows::Forms::Label^  label1;
	private: AnywndComboBox::AnywndComboBoxControl^  comboAccount;
	private: System::Windows::Forms::Label^  label4;
	private: SimpleReportControl::SimpleReportControlControl^  viewGridTrade;
	private: System::Windows::Forms::Timer^  timerTimeout;
	private: System::Windows::Forms::ProgressBar^  progQueryBar;

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
			this->btnResetZero = (gcnew System::Windows::Forms::Button());
			this->btnQuery = (gcnew System::Windows::Forms::Button());
			this->dtpEnd = (gcnew System::Windows::Forms::DateTimePicker());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->dtpStart = (gcnew System::Windows::Forms::DateTimePicker());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->comboAccount = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->viewGridTrade = (gcnew SimpleReportControl::SimpleReportControlControl());
			this->timerTimeout = (gcnew System::Windows::Forms::Timer(this->components));
			this->progQueryBar = (gcnew System::Windows::Forms::ProgressBar());
			this->SuspendLayout();
			// 
			// btnResetZero
			// 
			this->btnResetZero->Location = System::Drawing::Point(273, 58);
			this->btnResetZero->Name = L"btnResetZero";
			this->btnResetZero->Size = System::Drawing::Size(83, 24);
			this->btnResetZero->TabIndex = 36;
			this->btnResetZero->Text = L"清空";
			this->btnResetZero->UseVisualStyleBackColor = true;
			this->btnResetZero->Click += gcnew System::EventHandler(this, &FormQueryHistoryTrade::btnResetZero_Click);
			// 
			// btnQuery
			// 
			this->btnQuery->Location = System::Drawing::Point(68, 58);
			this->btnQuery->Name = L"btnQuery";
			this->btnQuery->Size = System::Drawing::Size(83, 24);
			this->btnQuery->TabIndex = 37;
			this->btnQuery->Text = L"查询";
			this->btnQuery->UseVisualStyleBackColor = true;
			this->btnQuery->Click += gcnew System::EventHandler(this, &FormQueryHistoryTrade::btnQuery_Click);
			// 
			// dtpEnd
			// 
			this->dtpEnd->CustomFormat = L"yyyy-MM-dd";
			this->dtpEnd->Location = System::Drawing::Point(246, 31);
			this->dtpEnd->Name = L"dtpEnd";
			this->dtpEnd->Size = System::Drawing::Size(110, 21);
			this->dtpEnd->TabIndex = 35;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(186, 36);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 32;
			this->label2->Text = L"结束日期";
			// 
			// dtpStart
			// 
			this->dtpStart->CustomFormat = L"yyyy-MM-dd";
			this->dtpStart->Location = System::Drawing::Point(68, 31);
			this->dtpStart->Name = L"dtpStart";
			this->dtpStart->Size = System::Drawing::Size(110, 21);
			this->dtpStart->TabIndex = 34;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(9, 36);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 33;
			this->label1->Text = L"起始日期";
			// 
			// comboAccount
			// 
			this->comboAccount->AllowDrop = true;
			this->comboAccount->BackColor = System::Drawing::Color::White;
			this->comboAccount->ChildControl = nullptr;
			this->comboAccount->DropDownHeight = 1;
			this->comboAccount->DropDownWidth = 1;
			this->comboAccount->FormattingEnabled = true;
			this->comboAccount->IntegralHeight = false;
			this->comboAccount->Location = System::Drawing::Point(68, 7);
			this->comboAccount->MaxDropDownItems = 1;
			this->comboAccount->Name = L"comboAccount";
			this->comboAccount->Size = System::Drawing::Size(288, 20);
			this->comboAccount->TabIndex = 31;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(9, 12);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(41, 12);
			this->label4->TabIndex = 30;
			this->label4->Text = L"账户号";
			// 
			// viewGridTrade
			// 
			this->viewGridTrade->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->viewGridTrade->ConfigGuid = L"{1F68708F-06CC-4ed4-986D-B4666CD237BD}";
			this->viewGridTrade->Location = System::Drawing::Point(8, 86);
			this->viewGridTrade->Name = L"viewGridTrade";
			this->viewGridTrade->RealDataTable = nullptr;
			this->viewGridTrade->SimpleMode = true;
			this->viewGridTrade->Size = System::Drawing::Size(795, 435);
			this->viewGridTrade->StatisticDataTable = nullptr;
			this->viewGridTrade->TabIndex = 39;
			// 
			// timerTimeout
			// 
			this->timerTimeout->Interval = 3000;
			this->timerTimeout->Tick += gcnew System::EventHandler(this, &FormQueryHistoryTrade::timerTimeout_Tick);
			// 
			// progQueryBar
			// 
			this->progQueryBar->Location = System::Drawing::Point(157, 65);
			this->progQueryBar->Name = L"progQueryBar";
			this->progQueryBar->Size = System::Drawing::Size(109, 10);
			this->progQueryBar->TabIndex = 22;
			// 
			// FormQueryHistoryTrade
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(813, 531);
			this->Controls->Add(this->progQueryBar);
			this->Controls->Add(this->viewGridTrade);
			this->Controls->Add(this->btnResetZero);
			this->Controls->Add(this->btnQuery);
			this->Controls->Add(this->dtpEnd);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->dtpStart);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboAccount);
			this->Controls->Add(this->label4);
			this->Name = L"FormQueryHistoryTrade";
			this->Text = L"查询成交历史数据";
			this->Load += gcnew System::EventHandler(this, &FormQueryHistoryTrade::FormQueryHistoryTrade_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FormQueryHistoryTrade::FormQueryHistoryTrade_FormClosed);
			this->Resize += gcnew System::EventHandler(this, &FormQueryHistoryTrade::FormQueryHistoryTrade_Resize);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

protected:virtual void WndProc( Message% m ) override {

		if(m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival) {
			char *p = (char*)m.LParam.ToInt32();
			Stru_UniPkgHead head;
			
			memset(&head, 0, sizeof(head));
			memcpy(&head, p, sizeof(head));
			switch(head.cmdid) {
			case Cmd_RM_QryHistoryTrade_Rsp:
			{
				if(head.userdata1 == CF_ERROR_SUCCESS) {
					PlatformStru_TradeInfo* pTrade = NULL;
					int nItemCount = head.len/sizeof(PlatformStru_TradeInfo);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pTrade = (PlatformStru_TradeInfo*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(PlatformStru_TradeInfo));

						// 这里应该不需要，一般理解为返回即为需要的数据
						std::set<std::string>::iterator it = 
								save->setQueryAccountID.find(pTrade->InvestorID);
						if( it== save->setQueryAccountID.end())//判断是不是这个页面查询的，不是则返回
							continue;

						save->queueTradeInfo.push(*pTrade);
					}
					save->UnlockObject();
					
					progQueryBar->Value = progQueryBar->Value+1;

					if(progQueryBar->Value==progQueryBar->Maximum) {
						m_bQuerying = false;
						this->btnQuery->Text = gcnew String("查询");
						save->LockObject();
						if(!save->queueTradeInfo.empty()) {
							ShowQueue(save->queueTradeInfo);
						}
						else {
							_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
									CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, "查询历史成交，无数据返回。");
						}
						save->UnlockObject();
					}
					else
						save->NextQuery();


					//if(save->queueTradeInfo.size() == 0) {
					//	_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
					//			CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
					//			"历史成交查询：本次查询无数据返回。");
					//	return;
					//}
					//
					////std::queue<PlatformStru_TradeInfo> queueRet;
					////save->CheckNewData(m_pdataInfo, queueRet);
					//ShowQueue(save->queueTradeInfo);

				}
				else {
					_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
							CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, p+sizeof(Stru_UniPkgHead));
				}
			}
				break;
			};
		}

		Form::WndProc(m);
	}

	int GetColIndex(string& strColName) {
#if 1
		for(int i=0; i<dTableTrade->Columns->Count; i++) {
			if(dTableTrade->Columns[i]->Caption == Tools::string2String(strColName.c_str()))
				return dTableTrade->Columns->IndexOf(dTableTrade->Columns[i]->ColumnName);
		}
		return -1;
#else
		return dTableTrade->Columns->IndexOf(Tools::string2String(strColName.c_str()));
#endif
	}

	void InitGridHead() {

		dTableTrade->Columns->Clear();

		int nColID = 0;
		System::Data::DataColumn^ gridColumn;

		gridColumn = dTableTrade->Columns->Add(L"交易日", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_TradingDay));

		gridColumn = dTableTrade->Columns->Add(L"账户号", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_InvestorID));

		//gridColumn = dTableTrade->Columns->Add(L"账户名称", Type::GetType("System.String"));
		//gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_InvestorName));

		gridColumn = dTableTrade->Columns->Add(L"合约代码", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_InstrumentID));

		gridColumn = dTableTrade->Columns->Add(L"买卖方向", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_Direction));

		gridColumn = dTableTrade->Columns->Add(L"投机套保标志", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_HedgeFlag));

		gridColumn = dTableTrade->Columns->Add(L"报单引用", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_OrderRef));

		gridColumn = dTableTrade->Columns->Add(L"交易所代码", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_ExchangeID));

		gridColumn = dTableTrade->Columns->Add(L"成交编号", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_TradeID));

		gridColumn = dTableTrade->Columns->Add(L"报单编号", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_OrderSysID));

		gridColumn = dTableTrade->Columns->Add(L"开平标志", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_OffsetFlag));

		gridColumn = dTableTrade->Columns->Add(L"价格", Type::GetType("System.Double"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_Price));

		gridColumn = dTableTrade->Columns->Add(L"数量", Type::GetType("System.Int32"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_Volume));

		gridColumn = dTableTrade->Columns->Add(L"成交日期", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_TradeDate));

		gridColumn = dTableTrade->Columns->Add(L"成交时间", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_TradeTime));

		gridColumn = dTableTrade->Columns->Add(L"成交类型", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_TradeType));

		gridColumn = dTableTrade->Columns->Add(L"成交价来源", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_PriceSource));

		gridColumn = dTableTrade->Columns->Add(L"本地报单编号", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_OrderLocalID));

		gridColumn = dTableTrade->Columns->Add(L"序号", Type::GetType("System.Int32"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_SequenceNo));

		gridColumn = dTableTrade->Columns->Add(L"成交来源", Type::GetType("System.String"));
		gridColumn->Caption = Tools::string2String(MACRO2STRING(conCol_HQT_TradeSource));

	}

	void UpdateRow(DataRow^ row, PlatformStru_TradeInfo& trade) {

		if(row == nullptr)
			return;
		int nCol = -1;
		char strTemp[1024];

		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_TradingDay))))>=0) {
			row[nCol] = Tools::string2String(trade.TradingDay);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_InvestorID))))>=0) {
			row[nCol] = Tools::string2String(trade.InvestorID);
		}
		//if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_InvestorName))))>=0) {
		//	row[nCol] = Tools::string2String(trade.Account);
		//}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_InstrumentID))))>=0) {
			row[nCol] = Tools::string2String(trade.InstrumentID);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_OrderRef))))>=0) {
			row[nCol] = Tools::string2String(trade.OrderRef);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_ExchangeID))))>=0) {
			row[nCol] = Tools::string2String(trade.ExchangeID);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_TradeID))))>=0) {
			row[nCol] = Tools::string2String(trade.TradeID);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_Direction))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::Direction2String(trade.Direction));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_HedgeFlag))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::HedgeFlag2String(trade.HedgeFlag));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_OrderSysID))))>=0) {
			row[nCol] = Tools::string2String(trade.OrderSysID);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_OffsetFlag))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::OffsetFlag2String(trade.OffsetFlag));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_Price))))>=0) {
			sprintf(strTemp, "%0.3f", trade.Price);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_Volume))))>=0) {
			sprintf(strTemp, "%d", trade.Volume);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_TradeDate))))>=0) {
			row[nCol] = Tools::string2String(trade.TradeDate);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_TradeTime))))>=0) {
			row[nCol] = Tools::string2String(trade.TradeTime);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_TradeType))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::TradeType2String(trade.TradeType));
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_PriceSource))))>=0) {
			sprintf(strTemp, "%c", trade.PriceSource);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_OrderLocalID))))>=0) {
			row[nCol] = Tools::string2String(trade.OrderLocalID);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_SequenceNo))))>=0) {
			sprintf(strTemp, "%d", trade.SequenceNo);
			row[nCol] = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndex(string(MACRO2STRING(conCol_HQT_TradeSource))))>=0) {
			sprintf(strTemp, "%s", CFieldDetail::TradeSourceString(trade.TradeSource));
			row[nCol] = Tools::string2String(strTemp);
		}
	}

	void ShowQueue(std::queue<PlatformStru_TradeInfo>& queueRet) {

		save->LockObject();
		
		while(!queueRet.empty()) {
			PlatformStru_TradeInfo field = queueRet.front();
			queueRet.pop();

			DataRow^ newRow = dTableTrade->NewRow();
			UpdateRow(newRow, field);
			dTableTrade->Rows->Add(newRow);
		}
		save->UnlockObject();

		viewGridTrade->Refresh2();
	}

	private: System::Void FormQueryHistoryTrade_Load(System::Object^  sender, System::EventArgs^  e) {
				 InitGridHead();
				 panelOrganDealerTreeList = gcnew PanelOrganDealerTree();
				 panelOrganDealerTreeList->ResetRelateCombo(comboAccount);
				 comboAccount->ChildControl = panelOrganDealerTreeList;

				 IntPtr hWnd=this->Handle;
				 CTcpLayer::SubscribePkg(Cmd_RM_QryHistoryTrade_Rsp, (int)hWnd);

				 ResetComboBoxTextReadOnly(comboAccount);
				 comboAccount->Enabled = true;

				 viewGridTrade->RealDataTable = dTableTrade;
			 }
	private: System::Void FormQueryHistoryTrade_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
				 IntPtr hWnd=this->Handle;
				 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
			 }
	private: System::Void FormQueryHistoryTrade_Resize(System::Object^  sender, System::EventArgs^  e) {
                 System::Drawing::Rectangle rect = this->ClientRectangle;
				 System::Drawing::Rectangle rectGrid = viewGridTrade->DisplayRectangle;
				 System::Drawing::Size size(rect.Width-viewGridTrade->Left-10, rect.Height-viewGridTrade->Top-10);
				 viewGridTrade->Size = size;
			 }
	private: System::Void btnQuery_Click(System::Object^  sender, System::EventArgs^  e) {

				 if(m_bQuerying) {
					if(MessageBox::Show("是否终止本次查询？", 
							"警告", MessageBoxButtons::YesNo, MessageBoxIcon::Warning)
							==::DialogResult::Yes) {
						 save->StopQuery();
						 m_bQuerying = false;
						 progQueryBar->Value = 0;
						 this->btnQuery->Text = gcnew String("查询");
						 if(MessageBox::Show("是否显示未完成的查询结果？", 
								"警告", MessageBoxButtons::YesNo, MessageBoxIcon::Warning)
								==::DialogResult::Yes) {
							save->LockObject();
							if(!save->queueTradeInfo.empty()) {
								ShowQueue(save->queueTradeInfo);
							}
							else {
								_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, "查询历史成交，无数据返回。");
							}
							save->UnlockObject();
						}
					}
					 return;
				 }
				 
				 if(!m_bHasChanged_QueryParam)
					 return;

				 bool bHasData = false;
				 viewGridTrade->Clear();
				 dTableTrade->Rows->Clear();

				 save->ResetQuery();
				 progQueryBar->Value = 0;

				 std::string strStartDate;
				 std::string strEndDate;
				 std::set<std::string> setAccountID;
				 // 然后根据查询条件获得需要订阅的AccountID集合

				 strStartDate = Tools::String2string(dtpStart->Value.Date.ToString("yyyyMMdd"));
				 strEndDate = Tools::String2string(dtpEnd->Value.Date.ToString("yyyyMMdd"));
				 if(atoi(strEndDate.c_str()) < atoi(strStartDate.c_str())) {
					 MessageBox::Show("起始日期必须小于等于结束日期。");
					 return;
				 }

				 strStartDate = Tools::String2string(dtpStart->Value.Date.ToString("yyyy-MM-dd"));
				 strEndDate = Tools::String2string(dtpEnd->Value.Date.ToString("yyyy-MM-dd"));

				 // 将查询集合保存起来
				 save->LockObject();
				 bHasData = panelOrganDealerTreeList->GetSelectID(save->setQueryAccountID);
				 save->UnlockObject();

				 dwTcpStartTime = GetTickCount();
				 dwTcpEndTime = 0;
				 dwShowStartTime = 0;
				 dwShowEndTime = 0;

				 if(bHasData) {
					 int nCount = 0;
					 sLoginRsp loginRsp;
					 memset(&loginRsp, 0, sizeof(loginRsp));
					 m_pdataInfo->GetLogonInfo(loginRsp);
					 save->NewQuery(strStartDate, strEndDate, loginRsp.mnUserID, nCount);
					 progQueryBar->Minimum = 0;
					 progQueryBar->Maximum = nCount;
					 //this->btnQuery->Enabled = false;
					 //this->timerTimeout->Start();
					 this->btnQuery->Text = gcnew String("停止");
					 m_bQuerying = true;
				 }
				 else
					 MessageBox::Show("所设查询条件未筛选出任何帐号。", 
							"提示", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			 }
	private: System::Void btnResetZero_Click(System::Object^  sender, System::EventArgs^  e) {
				 m_bHasChanged_QueryParam = true;
				 panelOrganDealerTreeList->Clear();
				 if(!m_bHasChanged_QueryParam)
					 return;

				 viewGridTrade->Clear();
				 dTableTrade->Rows->Clear();
				 save->ResetQuery();
			 }
private: System::Void timerTimeout_Tick(System::Object^  sender, System::EventArgs^  e) {
			 this->btnQuery->Enabled = true;
			 this->timerTimeout->Stop();
		 }
};
}

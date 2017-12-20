#pragma once
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
	/// Summary for AddPDform
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class AddPDform : public System::Windows::Forms::Form
	{
	public:
		AddPDform(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pType = new OperType;
			m_pPositionDetail = new PlatformStru_PositionDetail;
			m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AddPDform()
		{
			if (components)
			{
				delete components;
			}
			if ( NULL != m_pType )
			{
				delete m_pType;
				m_pType = NULL;
			}

			if ( NULL != m_pPositionDetail )
			{
				delete m_pPositionDetail;
				m_pPositionDetail = NULL;
			}
		}
	private: CDataInfo* m_pDataInfo;
	private: OperType* m_pType;
	private: PlatformStru_PositionDetail* m_pPositionDetail;   
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::ComboBox^  comboBox_user;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox_Ins;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::ComboBox^  comboBox_direction;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::ComboBox^  comboBox_HedgeFlag;


	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  textBox_volum;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::TextBox^  textBox_openPrice;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::ComboBox^  comboBox_exchange;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::TextBox^  textBox_TradeID;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
	private: System::Windows::Forms::Button^  button_caculate;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::TextBox^  textBox_settlePrice;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::TextBox^  textBox_LastSettlementPrice;


	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::TextBox^  textBox_Margin;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::TextBox^  textBox_PositionProfitByDate;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::TextBox^  textBox_CloseVolume;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::TextBox^  textBox_CloseProfitByDate;
	private: System::Windows::Forms::Button^  button_confirm;

	private: System::Windows::Forms::Button^  button_cancel;


	public:
		void SetOperType(OperType eType)
		{
			*m_pType = eType;
		}
		void SetPositionDetail(const PlatformStru_PositionDetail& PositionDetail)
		{
			*m_pPositionDetail = PositionDetail;
		}

		PlatformStru_PositionDetail* GetPositionDetail()
		{
			return m_pPositionDetail;
		}







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
			this->comboBox_user = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox_Ins = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->comboBox_direction = (gcnew System::Windows::Forms::ComboBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->comboBox_HedgeFlag = (gcnew System::Windows::Forms::ComboBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->textBox_volum = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->textBox_openPrice = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->comboBox_exchange = (gcnew System::Windows::Forms::ComboBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->textBox_TradeID = (gcnew System::Windows::Forms::TextBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->button_caculate = (gcnew System::Windows::Forms::Button());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->textBox_settlePrice = (gcnew System::Windows::Forms::TextBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->textBox_LastSettlementPrice = (gcnew System::Windows::Forms::TextBox());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->textBox_Margin = (gcnew System::Windows::Forms::TextBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->textBox_PositionProfitByDate = (gcnew System::Windows::Forms::TextBox());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->textBox_CloseVolume = (gcnew System::Windows::Forms::TextBox());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->textBox_CloseProfitByDate = (gcnew System::Windows::Forms::TextBox());
			this->button_confirm = (gcnew System::Windows::Forms::Button());
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(26, 24);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"交易员";
			// 
			// comboBox_user
			// 
			this->comboBox_user->FormattingEnabled = true;
			this->comboBox_user->Location = System::Drawing::Point(86, 15);
			this->comboBox_user->Name = L"comboBox_user";
			this->comboBox_user->Size = System::Drawing::Size(108, 20);
			this->comboBox_user->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(223, 24);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"合约代码";
			// 
			// textBox_Ins
			// 
			this->textBox_Ins->Location = System::Drawing::Point(282, 15);
			this->textBox_Ins->Name = L"textBox_Ins";
			this->textBox_Ins->Size = System::Drawing::Size(87, 21);
			this->textBox_Ins->TabIndex = 3;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(423, 23);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(35, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"买\\卖";
			// 
			// comboBox_direction
			// 
			this->comboBox_direction->FormattingEnabled = true;
			this->comboBox_direction->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"净", L"多头", L"空头"});
			this->comboBox_direction->Location = System::Drawing::Point(485, 15);
			this->comboBox_direction->Name = L"comboBox_direction";
			this->comboBox_direction->Size = System::Drawing::Size(57, 20);
			this->comboBox_direction->TabIndex = 5;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(606, 23);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(59, 12);
			this->label4->TabIndex = 6;
			this->label4->Text = L"投机\\套保";
			// 
			// comboBox_HedgeFlag
			// 
			this->comboBox_HedgeFlag->FormattingEnabled = true;
			this->comboBox_HedgeFlag->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"投机", L"套保", L"套利"});
			this->comboBox_HedgeFlag->Location = System::Drawing::Point(684, 15);
			this->comboBox_HedgeFlag->Name = L"comboBox_HedgeFlag";
			this->comboBox_HedgeFlag->Size = System::Drawing::Size(94, 20);
			this->comboBox_HedgeFlag->TabIndex = 7;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(28, 72);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(29, 12);
			this->label5->TabIndex = 8;
			this->label5->Text = L"手数";
			// 
			// textBox_volum
			// 
			this->textBox_volum->Location = System::Drawing::Point(86, 63);
			this->textBox_volum->Name = L"textBox_volum";
			this->textBox_volum->Size = System::Drawing::Size(69, 21);
			this->textBox_volum->TabIndex = 9;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(225, 71);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(41, 12);
			this->label6->TabIndex = 10;
			this->label6->Text = L"开仓价";
			// 
			// textBox_openPrice
			// 
			this->textBox_openPrice->Location = System::Drawing::Point(282, 61);
			this->textBox_openPrice->Name = L"textBox_openPrice";
			this->textBox_openPrice->Size = System::Drawing::Size(87, 21);
			this->textBox_openPrice->TabIndex = 11;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(425, 71);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(41, 12);
			this->label7->TabIndex = 12;
			this->label7->Text = L"交易所";
			// 
			// comboBox_exchange
			// 
			this->comboBox_exchange->FormattingEnabled = true;
			this->comboBox_exchange->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"上期所", L"郑商所", L"大商所", L"中金所"});
			this->comboBox_exchange->Location = System::Drawing::Point(485, 61);
			this->comboBox_exchange->Name = L"comboBox_exchange";
			this->comboBox_exchange->Size = System::Drawing::Size(104, 20);
			this->comboBox_exchange->TabIndex = 13;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(26, 124);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(53, 12);
			this->label8->TabIndex = 14;
			this->label8->Text = L"成交编号";
			// 
			// textBox_TradeID
			// 
			this->textBox_TradeID->Location = System::Drawing::Point(86, 114);
			this->textBox_TradeID->Name = L"textBox_TradeID";
			this->textBox_TradeID->Size = System::Drawing::Size(108, 21);
			this->textBox_TradeID->TabIndex = 15;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(227, 122);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(53, 12);
			this->label9->TabIndex = 16;
			this->label9->Text = L"成交日期";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->CustomFormat = L"yyyyMMdd";
			this->dateTimePicker1->Location = System::Drawing::Point(286, 114);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(142, 21);
			this->dateTimePicker1->TabIndex = 17;
			// 
			// button_caculate
			// 
			this->button_caculate->Location = System::Drawing::Point(667, 111);
			this->button_caculate->Name = L"button_caculate";
			this->button_caculate->Size = System::Drawing::Size(75, 23);
			this->button_caculate->TabIndex = 18;
			this->button_caculate->Text = L"计算刷新";
			this->button_caculate->UseVisualStyleBackColor = true;
			this->button_caculate->Click += gcnew System::EventHandler(this, &AddPDform::button_caculate_Click);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(30, 171);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(41, 12);
			this->label10->TabIndex = 19;
			this->label10->Text = L"结算价";
			// 
			// textBox_settlePrice
			// 
			this->textBox_settlePrice->Location = System::Drawing::Point(86, 168);
			this->textBox_settlePrice->Name = L"textBox_settlePrice";
			this->textBox_settlePrice->ReadOnly = true;
			this->textBox_settlePrice->Size = System::Drawing::Size(100, 21);
			this->textBox_settlePrice->TabIndex = 20;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(229, 176);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(41, 12);
			this->label11->TabIndex = 21;
			this->label11->Text = L"昨结算";
			// 
			// textBox_LastSettlementPrice
			// 
			this->textBox_LastSettlementPrice->Location = System::Drawing::Point(286, 168);
			this->textBox_LastSettlementPrice->Name = L"textBox_LastSettlementPrice";
			this->textBox_LastSettlementPrice->ReadOnly = true;
			this->textBox_LastSettlementPrice->Size = System::Drawing::Size(100, 21);
			this->textBox_LastSettlementPrice->TabIndex = 22;
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(419, 170);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(65, 12);
			this->label13->TabIndex = 25;
			this->label13->Text = L"占用保证金";
			// 
			// textBox_Margin
			// 
			this->textBox_Margin->Location = System::Drawing::Point(495, 162);
			this->textBox_Margin->Name = L"textBox_Margin";
			this->textBox_Margin->ReadOnly = true;
			this->textBox_Margin->Size = System::Drawing::Size(94, 21);
			this->textBox_Margin->TabIndex = 26;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(26, 221);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(53, 12);
			this->label14->TabIndex = 27;
			this->label14->Text = L"持仓盈亏";
			// 
			// textBox_PositionProfitByDate
			// 
			this->textBox_PositionProfitByDate->Location = System::Drawing::Point(86, 218);
			this->textBox_PositionProfitByDate->Name = L"textBox_PositionProfitByDate";
			this->textBox_PositionProfitByDate->ReadOnly = true;
			this->textBox_PositionProfitByDate->Size = System::Drawing::Size(100, 21);
			this->textBox_PositionProfitByDate->TabIndex = 28;
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(225, 221);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(41, 12);
			this->label15->TabIndex = 29;
			this->label15->Text = L"平仓量";
			// 
			// textBox_CloseVolume
			// 
			this->textBox_CloseVolume->Location = System::Drawing::Point(286, 218);
			this->textBox_CloseVolume->Name = L"textBox_CloseVolume";
			this->textBox_CloseVolume->Size = System::Drawing::Size(100, 21);
			this->textBox_CloseVolume->TabIndex = 30;
			this->textBox_CloseVolume->Text = L"0";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(427, 226);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(53, 12);
			this->label16->TabIndex = 31;
			this->label16->Text = L"平仓盈亏";
			// 
			// textBox_CloseProfitByDate
			// 
			this->textBox_CloseProfitByDate->Location = System::Drawing::Point(486, 221);
			this->textBox_CloseProfitByDate->Name = L"textBox_CloseProfitByDate";
			this->textBox_CloseProfitByDate->Size = System::Drawing::Size(100, 21);
			this->textBox_CloseProfitByDate->TabIndex = 32;
			this->textBox_CloseProfitByDate->Text = L"0";
			// 
			// button_confirm
			// 
			this->button_confirm->Location = System::Drawing::Point(273, 302);
			this->button_confirm->Name = L"button_confirm";
			this->button_confirm->Size = System::Drawing::Size(75, 23);
			this->button_confirm->TabIndex = 33;
			this->button_confirm->Text = L"确定";
			this->button_confirm->UseVisualStyleBackColor = true;
			this->button_confirm->Click += gcnew System::EventHandler(this, &AddPDform::button_confirm_Click);
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(421, 302);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(75, 23);
			this->button_cancel->TabIndex = 35;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &AddPDform::button_cancel_Click);
			// 
			// AddPDform
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(802, 379);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->button_confirm);
			this->Controls->Add(this->textBox_CloseProfitByDate);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->textBox_CloseVolume);
			this->Controls->Add(this->label15);
			this->Controls->Add(this->textBox_PositionProfitByDate);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->textBox_Margin);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->textBox_LastSettlementPrice);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->textBox_settlePrice);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->button_caculate);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->textBox_TradeID);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->comboBox_exchange);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->textBox_openPrice);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->textBox_volum);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->comboBox_HedgeFlag);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->comboBox_direction);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->textBox_Ins);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboBox_user);
			this->Controls->Add(this->label1);
			this->Name = L"AddPDform";
			this->Text = L"新增持仓明细";
			this->Load += gcnew System::EventHandler(this, &AddPDform::AddPDform_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void AddPDform_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent();
				 std::map<int, UserInfo> mapUserInfo;
				 m_pDataInfo->GetUserInfo(mapUserInfo);
				 std::map<int, UserInfo>::iterator it =mapUserInfo.begin();
				 while ( it  != mapUserInfo.end() )
				 {
					 if ( it->second.nUserType == USER_TYPE_TRADER )
					 {	
					     comboBox_user->Items->Add(Tools::string2String(it->second.szAccount));
					 }
					 it++;
				 }

				 if ( *m_pType == Oper_Add )
				 {
					 this->Text = "新增持仓明细";
					 

				 }
				 else
				 {
					 this->Text = "修改持仓明细";

					 comboBox_user->Text  = Tools::string2String(m_pPositionDetail->InvestorID);                 
					 textBox_Ins->Text  = Tools::string2String(m_pPositionDetail->InstrumentID);					 
					 comboBox_direction->Text  = Tools::string2String(m_pDataInfo->Direction2String(m_pPositionDetail->Direction)); 
					 comboBox_HedgeFlag->Text  = Tools::string2String(m_pDataInfo->HedgeFlag2String(m_pPositionDetail->HedgeFlag));  
					 char strPrice[128];
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%d",m_pPositionDetail->Volume);
					 textBox_volum->Text  = Tools::string2String(strPrice);    
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%f",m_pPositionDetail->OpenPrice);
					 textBox_openPrice->Text  = Tools::string2String(strPrice); 
					 string exchange =m_pDataInfo->ExchangeID2String(m_pPositionDetail->ExchangeID);
					 comboBox_exchange->Text  = Tools::string2String(exchange.c_str()); 
					 textBox_TradeID->Text  = Tools::string2String(m_pPositionDetail->TradeID);
					 char OpenYear[5]={0}, OpenMonth[3]={0},OpenDay[3]={0};
					 strncpy(OpenYear,m_pPositionDetail->OpenDate,4);
					 strncpy(OpenMonth,m_pPositionDetail->OpenDate+4,2);
					 strncpy(OpenDay,m_pPositionDetail->OpenDate+6,2);
					 dateTimePicker1->Value = DateTime(2001, 10, 20);//DateTime(atoi(OpenYear),atoi(OpenMonth),atoi(OpenDay));
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%f",m_pPositionDetail->SettlementPrice);
					 textBox_settlePrice->Text  = Tools::string2String(strPrice); 
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%f",m_pPositionDetail->LastSettlementPrice);
					 textBox_LastSettlementPrice->Text  = Tools::string2String(strPrice); 
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%f",m_pPositionDetail->Margin);
					 textBox_Margin->Text  = Tools::string2String(strPrice); 
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%f",m_pPositionDetail->PositionProfitByDate);
					 textBox_PositionProfitByDate->Text  = Tools::string2String(strPrice); 
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%d",m_pPositionDetail->CloseVolume);
					 textBox_CloseVolume->Text  = Tools::string2String(strPrice); 
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%f",m_pPositionDetail->CloseProfitByDate);
					 textBox_CloseProfitByDate->Text  = Tools::string2String(strPrice); 
				 }
			 }
private: bool InputCheck()
		 {
			 if( String::IsNullOrEmpty(textBox_Ins->Text) || String::IsNullOrEmpty(textBox_volum->Text)
				 || String::IsNullOrEmpty(textBox_openPrice->Text) || String::IsNullOrEmpty(textBox_TradeID->Text))  
			 {
				 MessageBox::Show("合约代码、手数、开仓价、成交编号为必填项!");    
				 return false;
			 }
			 if (!m_pDataInfo->IsExitIns(Tools::String2string(textBox_Ins->Text)))
			 {
				 MessageBox::Show("请输入正确的合约代码!");    
				 return false;
			 }
			 if( atoi(Tools::String2string(textBox_volum->Text).c_str()) <= 0 )		
			 {
				 MessageBox::Show("请输入正确的手数!");    
				 return false;
			 }
			 if( atof(Tools::String2string(textBox_openPrice->Text).c_str()) <= 0 )		
			 {
				 MessageBox::Show("请输入正确的开仓价!");    
				 return false;
			 }
			
			 return true;
		 }
private: System::Void button_caculate_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(InputCheck()== false)
				 return;
			 strncpy(m_pPositionDetail->InvestorID , Tools::String2string(comboBox_user->Text).c_str(),sizeof(m_pPositionDetail->InvestorID));
			 strncpy(m_pPositionDetail->InstrumentID , Tools::String2string(textBox_Ins->Text).c_str(),sizeof(m_pPositionDetail->InstrumentID));
			 m_pPositionDetail->Direction = m_pDataInfo->Direction2ID(Tools::String2string(comboBox_direction->Text));
			 m_pPositionDetail->HedgeFlag = m_pDataInfo->HedgeFlag2ID(Tools::String2string(comboBox_HedgeFlag->Text));
			 m_pPositionDetail->Volume = atoi(Tools::String2string(textBox_volum->Text).c_str());
			 m_pPositionDetail->OpenPrice = atof(Tools::String2string(textBox_openPrice->Text).c_str());
			 strncpy(m_pPositionDetail->ExchangeID , Tools::String2string(comboBox_exchange->Text).c_str(),sizeof(m_pPositionDetail->ExchangeID));
			 strncpy(m_pPositionDetail->TradeID , Tools::String2string(textBox_TradeID->Text).c_str(),sizeof(m_pPositionDetail->TradeID));
			 strncpy(m_pPositionDetail->OpenDate , Tools::String2string(dateTimePicker1->Text).c_str(),sizeof(m_pPositionDetail->OpenDate));

			 char strPrice[128];
			 memset(strPrice,0,sizeof(strPrice));
			 m_pPositionDetail->SettlementPrice = m_pDataInfo->GetSettlementPrice(Tools::String2string(textBox_settlePrice->Text));
			 sprintf(strPrice, "%f",m_pPositionDetail->SettlementPrice);
			 textBox_settlePrice->Text  = Tools::string2String(strPrice);
			 memset(strPrice,0,sizeof(strPrice));
			 m_pPositionDetail->LastSettlementPrice = m_pDataInfo->GetLastSettlementPrice(Tools::String2string(textBox_LastSettlementPrice->Text));
			 sprintf(strPrice, "%f", m_pPositionDetail->LastSettlementPrice);
			 textBox_LastSettlementPrice->Text  = Tools::string2String(strPrice);

			 //计算保证金

			 PlatformStru_InstrumentInfo InstrumentInfo;
			 m_pDataInfo->GetInstrunmentInfobyID(m_pPositionDetail->InstrumentID,InstrumentInfo);
			 std::string strBrokerID;
			 std::string strAccountID;
			 std::string InvestorID=m_pPositionDetail->InvestorID;
			 m_pDataInfo->GetTradeAccountByUserID(InvestorID, strBrokerID, strAccountID);
			 RateKey  key(strAccountID.c_str(),m_pPositionDetail->InstrumentID,strBrokerID.c_str());
			 CommissionMarginRate rate;

			 m_pDataInfo->GetRateByKey(key,rate);

			 if(m_pPositionDetail->Direction==THOST_FTDC_D_Buy)
			 {
				 m_pPositionDetail->Margin= m_pPositionDetail->Volume*
					 InstrumentInfo.VolumeMultiple*
					 m_pPositionDetail->SettlementPrice*
					 rate.LongMarginRatioByMoney+
					 m_pPositionDetail->Volume*
					 rate.LongMarginRatioByVolume;
			 }
			 else
			 {
				 m_pPositionDetail->Margin=  m_pPositionDetail->Volume*
					 InstrumentInfo.VolumeMultiple*
					 m_pPositionDetail->SettlementPrice*
					 rate.ShortMarginRatioByMoney+
					 m_pPositionDetail->Volume*
					 rate.ShortMarginRatioByVolume;

			 }

			 memset(strPrice,0,sizeof(strPrice));
			 sprintf(strPrice, "%f",m_pPositionDetail->Margin);
			 textBox_Margin->Text  = Tools::string2String(strPrice);

			 //计算逐日盯市持仓盈亏
			 bool bTodayPositionDetail = IsToday(m_pPositionDetail->OpenDate);

			 if(bTodayPositionDetail)
			 {
				 if(m_pPositionDetail->Direction==THOST_FTDC_D_Buy)
					 m_pPositionDetail->PositionProfitByDate=(m_pPositionDetail->SettlementPrice - m_pPositionDetail->OpenPrice)*m_pPositionDetail->Volume*InstrumentInfo.VolumeMultiple;
				 else
					 m_pPositionDetail->PositionProfitByDate=-1*(m_pPositionDetail->SettlementPrice-m_pPositionDetail->OpenPrice)*m_pPositionDetail->Volume*InstrumentInfo.VolumeMultiple;
			 }
			 else 
			 {
				 if(m_pPositionDetail->Direction==THOST_FTDC_D_Buy)
					 m_pPositionDetail->PositionProfitByDate=(m_pPositionDetail->SettlementPrice-m_pPositionDetail->LastSettlementPrice)*m_pPositionDetail->Volume*InstrumentInfo.VolumeMultiple;   //逐日盯市持仓盈亏
				 else
					 m_pPositionDetail->PositionProfitByDate=-1*(m_pPositionDetail->SettlementPrice-m_pPositionDetail->LastSettlementPrice)*m_pPositionDetail->Volume*InstrumentInfo.VolumeMultiple;   //逐日盯市持仓盈亏
			 }
			 if(m_pPositionDetail->PositionProfitByDate==-0) m_pPositionDetail->PositionProfitByDate=0;
			 
			 memset(strPrice,0,sizeof(strPrice));
			 sprintf(strPrice, "%f",m_pPositionDetail->PositionProfitByDate);
			 textBox_PositionProfitByDate->Text  = Tools::string2String(strPrice);
			 
		 }
		 //判断一个字符串日期是否是当天
		private: bool IsToday(const char *OpenDate)
		 {
		 	int tradingday=0;
		 
		 	if(OpenDate==NULL) return false;

			time_t lTime;
			struct tm * lTimeinfo; 
			time(&lTime);
			lTimeinfo = localtime(&lTime);
			_SettlementDate lcBuf = {0};
			sprintf_s(lcBuf,"%04d%02d%02d",1900+lTimeinfo->tm_year,1+lTimeinfo->tm_mon,lTimeinfo->tm_mday);
		 
		 	string Tradingday=lcBuf;
		 
		 	tradingday=atoi(Tradingday.c_str());
		 	if(tradingday==0) 
		 	{
		 		char OpenYear[5]={0}, OpenMonth[3]={0},OpenDay[3]={0};
		 		time_t tCurdate = time(NULL);
		 		struct tm local;
		 		localtime_s(&local, &tCurdate);
		 		tradingday= ((local.tm_year+1900)*10000+(local.tm_mon+1))*100+local.tm_mday;
		 	}
		 	return (tradingday==atoi(OpenDate));
		 }
private: System::Void button_confirm_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if( atoi(Tools::String2string(textBox_CloseVolume->Text).c_str()) < 0 )		
			 {
				 MessageBox::Show("请输入正确的平仓量!");    
				 return;
			 }
			 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
			 m_pPositionDetail->CloseVolume = atoi(Tools::String2string(textBox_CloseVolume->Text).c_str());
			 m_pPositionDetail->CloseProfitByDate = atof(Tools::String2string(textBox_CloseProfitByDate->Text).c_str());
			 this->Close();
		 }

private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Close();
		 }
};
}

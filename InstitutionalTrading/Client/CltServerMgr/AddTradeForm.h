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
	/// Summary for AddTradeForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class AddTradeForm : public System::Windows::Forms::Form
	{
	public:
		AddTradeForm(String^ tradeDate)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pTradeInfo = new PlatformStru_TradeInfo;
			
			m_pDataInfo = CDataInfo::NewInstance();
			m_pType = new OperType;
			m_tradeDate =tradeDate;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AddTradeForm()
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

			if ( NULL != m_pTradeInfo )
			{
				delete m_pTradeInfo;
				m_pTradeInfo = NULL;
			}
			
		}
	private: CDataInfo* m_pDataInfo;
	private: OperType* m_pType;
	private: PlatformStru_TradeInfo* m_pTradeInfo;  
			 String^ 	m_tradeDate;
	

	protected: 

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox_Ins;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  textBox_TradeID;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::ComboBox^  comboBox_user;

	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::ComboBox^  comboBox_direction;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::ComboBox^  comboBox_OffsetFlag;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::ComboBox^  comboBox_HedgeFlag;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::TextBox^  textBox_price;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::TextBox^  textBox_volum;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::ComboBox^  comboBox_TradeType;

	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Button^  button_confirm;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::TextBox^  textBox_OrderSysID;
	private: System::Windows::Forms::TextBox^  textBox_TradeTime;

	private: System::Windows::Forms::Label^  label1;

	private: System::Windows::Forms::Button^  button_cancel;



	public:
		void SetOperType(OperType eType)
		{
			*m_pType = eType;
		}
		void SetTradeInfo(const PlatformStru_TradeInfo& TradeInfo)
		{
			*m_pTradeInfo = TradeInfo;
		}

		PlatformStru_TradeInfo* GetTradeInfo()
		{
			return m_pTradeInfo;
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
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox_Ins = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox_TradeID = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->comboBox_user = (gcnew System::Windows::Forms::ComboBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->comboBox_direction = (gcnew System::Windows::Forms::ComboBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->comboBox_OffsetFlag = (gcnew System::Windows::Forms::ComboBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->comboBox_HedgeFlag = (gcnew System::Windows::Forms::ComboBox());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->textBox_price = (gcnew System::Windows::Forms::TextBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->textBox_volum = (gcnew System::Windows::Forms::TextBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->comboBox_TradeType = (gcnew System::Windows::Forms::ComboBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->button_confirm = (gcnew System::Windows::Forms::Button());
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->textBox_OrderSysID = (gcnew System::Windows::Forms::TextBox());
			this->textBox_TradeTime = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(34, 25);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"合约代码";
			// 
			// textBox_Ins
			// 
			this->textBox_Ins->Location = System::Drawing::Point(119, 22);
			this->textBox_Ins->Name = L"textBox_Ins";
			this->textBox_Ins->Size = System::Drawing::Size(95, 21);
			this->textBox_Ins->TabIndex = 3;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(34, 78);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(53, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"成交编号";
			// 
			// textBox_TradeID
			// 
			this->textBox_TradeID->Location = System::Drawing::Point(119, 69);
			this->textBox_TradeID->Name = L"textBox_TradeID";
			this->textBox_TradeID->Size = System::Drawing::Size(95, 21);
			this->textBox_TradeID->TabIndex = 5;
			this->textBox_TradeID->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &AddTradeForm::textBox_TradeID_KeyPress);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(307, 77);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(41, 12);
			this->label4->TabIndex = 6;
			this->label4->Text = L"交易员";
			// 
			// comboBox_user
			// 
			this->comboBox_user->FormattingEnabled = true;
			this->comboBox_user->Location = System::Drawing::Point(376, 70);
			this->comboBox_user->Name = L"comboBox_user";
			this->comboBox_user->Size = System::Drawing::Size(100, 20);
			this->comboBox_user->TabIndex = 7;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(34, 123);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(35, 12);
			this->label5->TabIndex = 8;
			this->label5->Text = L"买\\卖";
			// 
			// comboBox_direction
			// 
			this->comboBox_direction->FormattingEnabled = true;
			this->comboBox_direction->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"买", L"卖"});
			this->comboBox_direction->Location = System::Drawing::Point(119, 115);
			this->comboBox_direction->Name = L"comboBox_direction";
			this->comboBox_direction->Size = System::Drawing::Size(95, 20);
			this->comboBox_direction->TabIndex = 9;
			this->comboBox_direction->Text = L"买";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(309, 123);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(35, 12);
			this->label6->TabIndex = 10;
			this->label6->Text = L"开\\平";
			// 
			// comboBox_OffsetFlag
			// 
			this->comboBox_OffsetFlag->FormattingEnabled = true;
			this->comboBox_OffsetFlag->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"开仓", L"平仓", L"强平", L"平今", L"平昨"});
			this->comboBox_OffsetFlag->Location = System::Drawing::Point(376, 115);
			this->comboBox_OffsetFlag->Name = L"comboBox_OffsetFlag";
			this->comboBox_OffsetFlag->Size = System::Drawing::Size(100, 20);
			this->comboBox_OffsetFlag->TabIndex = 11;
			this->comboBox_OffsetFlag->Text = L"开仓";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(30, 168);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(59, 12);
			this->label7->TabIndex = 12;
			this->label7->Text = L"投机\\套保";
			// 
			// comboBox_HedgeFlag
			// 
			this->comboBox_HedgeFlag->FormattingEnabled = true;
			this->comboBox_HedgeFlag->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"投机", L"套保", L"套利"});
			this->comboBox_HedgeFlag->Location = System::Drawing::Point(119, 160);
			this->comboBox_HedgeFlag->Name = L"comboBox_HedgeFlag";
			this->comboBox_HedgeFlag->Size = System::Drawing::Size(96, 20);
			this->comboBox_HedgeFlag->TabIndex = 13;
			this->comboBox_HedgeFlag->Text = L"投机";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(32, 229);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(53, 12);
			this->label8->TabIndex = 14;
			this->label8->Text = L"成交价格";
			// 
			// textBox_price
			// 
			this->textBox_price->Location = System::Drawing::Point(117, 219);
			this->textBox_price->Name = L"textBox_price";
			this->textBox_price->Size = System::Drawing::Size(95, 21);
			this->textBox_price->TabIndex = 15;
			this->textBox_price->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &AddTradeForm::textBox_price_KeyPress);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(309, 228);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(53, 12);
			this->label9->TabIndex = 16;
			this->label9->Text = L"成交数量";
			// 
			// textBox_volum
			// 
			this->textBox_volum->Location = System::Drawing::Point(374, 218);
			this->textBox_volum->Name = L"textBox_volum";
			this->textBox_volum->Size = System::Drawing::Size(100, 21);
			this->textBox_volum->TabIndex = 17;
			this->textBox_volum->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &AddTradeForm::textBox_volum_KeyPress);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(307, 167);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(53, 12);
			this->label10->TabIndex = 18;
			this->label10->Text = L"成交类型";
			// 
			// comboBox_TradeType
			// 
			this->comboBox_TradeType->FormattingEnabled = true;
			this->comboBox_TradeType->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"普通成交", L"期权执行", L"OTC成交", L"期转现衍生成交", 
				L"组合衍生成交"});
			this->comboBox_TradeType->Location = System::Drawing::Point(376, 159);
			this->comboBox_TradeType->Name = L"comboBox_TradeType";
			this->comboBox_TradeType->Size = System::Drawing::Size(100, 20);
			this->comboBox_TradeType->TabIndex = 19;
			this->comboBox_TradeType->Text = L"普通成交";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(34, 296);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(53, 12);
			this->label11->TabIndex = 21;
			this->label11->Text = L"成交时间";
			// 
			// button_confirm
			// 
			this->button_confirm->Location = System::Drawing::Point(89, 356);
			this->button_confirm->Name = L"button_confirm";
			this->button_confirm->Size = System::Drawing::Size(75, 23);
			this->button_confirm->TabIndex = 22;
			this->button_confirm->Text = L"确定";
			this->button_confirm->UseVisualStyleBackColor = true;
			this->button_confirm->Click += gcnew System::EventHandler(this, &AddTradeForm::button_confirm_Click);
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(307, 356);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(75, 23);
			this->button_cancel->TabIndex = 23;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &AddTradeForm::button_cancel_Click);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(309, 296);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(53, 12);
			this->label12->TabIndex = 24;
			this->label12->Text = L"报单编号";
			// 
			// textBox_OrderSysID
			// 
			this->textBox_OrderSysID->Location = System::Drawing::Point(376, 287);
			this->textBox_OrderSysID->Name = L"textBox_OrderSysID";
			this->textBox_OrderSysID->Size = System::Drawing::Size(100, 21);
			this->textBox_OrderSysID->TabIndex = 25;
			this->textBox_OrderSysID->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &AddTradeForm::textBox_OrderSysID_KeyPress);
			// 
			// textBox_TradeTime
			// 
			this->textBox_TradeTime->Location = System::Drawing::Point(119, 287);
			this->textBox_TradeTime->Name = L"textBox_TradeTime";
			this->textBox_TradeTime->Size = System::Drawing::Size(93, 21);
			this->textBox_TradeTime->TabIndex = 26;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(102, 321);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(125, 12);
			this->label1->TabIndex = 27;
			this->label1->Text = L"(时间格式: HH:MM:SS)";
			// 
			// AddTradeForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(544, 412);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox_TradeTime);
			this->Controls->Add(this->textBox_OrderSysID);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->button_confirm);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->comboBox_TradeType);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->textBox_volum);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->textBox_price);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->comboBox_HedgeFlag);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->comboBox_OffsetFlag);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->comboBox_direction);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->comboBox_user);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->textBox_TradeID);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->textBox_Ins);
			this->Controls->Add(this->label2);
			this->Name = L"AddTradeForm";
			this->Text = L"AddTradeForm";
			this->Load += gcnew System::EventHandler(this, &AddTradeForm::AddTradeForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void AddTradeForm_Load(System::Object^  sender, System::EventArgs^  e) 
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
					 this->Text = "新增成交";


				 }
				 else
				 {
					 this->Text = "修改成交";

					 //textBox_TradeID->ReadOnly=true;
					 //textBox_OrderSysID->ReadOnly=true;

					 comboBox_user->Text  = Tools::string2String(m_pTradeInfo->InvestorID);                 
					 textBox_Ins->Text  = Tools::string2String(m_pTradeInfo->InstrumentID);					 
					 comboBox_direction->Text  = Tools::string2String(m_pDataInfo->TradeDirection2String(m_pTradeInfo->Direction)); 
					 comboBox_HedgeFlag->Text  = Tools::string2String(m_pDataInfo->HedgeFlag2String(m_pTradeInfo->HedgeFlag));  
					 comboBox_OffsetFlag->Text  = Tools::string2String(m_pDataInfo->OffsetFlag2String(m_pTradeInfo->OffsetFlag)); 
					 char strPrice[128];
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%d",m_pTradeInfo->Volume);
					 textBox_volum->Text  = Tools::string2String(strPrice);    
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%f",m_pTradeInfo->Price);
					 textBox_price->Text  = Tools::string2String(strPrice); 
					 textBox_TradeID->Text  = Tools::string2String(m_pTradeInfo->TradeID)->Trim();
					 /*char OpenYear[5]={0}, OpenMonth[3]={0},OpenDay[3]={0},hour[3]={0},mimute[3]={0},second[3]={0};
					 strncpy(OpenYear,m_pTradeInfo->TradeDate,4);
					 strncpy(OpenMonth,m_pTradeInfo->TradeDate+4,2);
					 strncpy(OpenDay,m_pTradeInfo->TradeDate+6,2);
					 strncpy(hour,m_pTradeInfo->TradeTime,2);
					 strncpy(mimute,m_pTradeInfo->TradeTime+3,2);
					 strncpy(second,m_pTradeInfo->TradeTime+6,2);*/
					 textBox_TradeTime->Text = Tools::string2String(m_pTradeInfo->TradeTime);;
					 comboBox_TradeType->Text  =Tools::string2String(m_pDataInfo->TradeType2String(m_pTradeInfo->TradeType));
					 textBox_OrderSysID->Text  = Tools::string2String(m_pTradeInfo->OrderSysID)->Trim();	
				 }
			 }
			 private: bool InputCheck()
					  {
						  if( String::IsNullOrEmpty(comboBox_user->Text))  
						  {
							  MessageBox::Show("请输入交易员账号!");    
							  return false;
						  } 
						  if( String::IsNullOrEmpty(textBox_OrderSysID->Text))  
						  {
							  MessageBox::Show("请输入报单编号!");    
							  return false;
						  } 
						  if( String::IsNullOrEmpty(textBox_TradeID->Text))  
						  {
							  MessageBox::Show("请输入成交编号!");    
							  return false;
						  } 
						  /*char date[9]="";
						  char datetime[256] ="";
						  string  dp =Tools::String2string(dateTimePicker1->Text);
						  strncpy(datetime,dp.c_str(),dp.length());
						  sprintf(date,"%.4s%.2s%.2s",datetime,datetime+5,datetime+8);*/
						  char Direction = m_pDataInfo->TradeDirection2ID(Tools::String2string(comboBox_direction->Text));
						  char HedgeFlag = m_pDataInfo->HedgeFlag2ID(Tools::String2string(comboBox_HedgeFlag->Text));
						  TradeKey key(Tools::String2string(comboBox_user->Text).c_str(),Tools::String2string(textBox_Ins->Text).c_str(),Tools::String2string(textBox_TradeID->Text).c_str(),
							  Tools::String2string(m_tradeDate).c_str(),Direction,HedgeFlag);
						  if ( *m_pType == Oper_Add && m_pDataInfo->IsExitTradeID(key))						 
						  {
							  MessageBox::Show("此条成交已存在!");    
							  return false;
						  }
						  if ( String::IsNullOrEmpty(textBox_Ins->Text) ||(!m_pDataInfo->IsExitIns(Tools::String2string(textBox_Ins->Text))))
						  {
							  MessageBox::Show("请输入正确的合约代码!");    
							  return false;
						  }
						  if( String::IsNullOrEmpty(textBox_volum->Text) ||((atoi(Tools::String2string(textBox_volum->Text).c_str())) <= 0 ))		
						  {
							  MessageBox::Show("请输入正确的成交数量!");    
							  return false;
						  }
						  if( String::IsNullOrEmpty(textBox_price->Text)|| (atof(Tools::String2string(textBox_price->Text).c_str()) <= 0) )		
						  {
							  MessageBox::Show("请输入正确的成交价格!");    
							  return false;
						  }
						  return true;
					  }
private: System::Void button_confirm_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(InputCheck()== false)
				 return;
			/* char date[9]="";
			 char time[9]="";
			 char datetime[256] ="";
			 string  dp =Tools::String2string(dateTimePicker1->Text);
			 strncpy(datetime,dp.c_str(),dp.length());
			 sprintf(date,"%.4s%.2s%.2s",datetime,datetime+5,datetime+8);
			 sprintf(time,"%.8s",datetime+11); */
			 strncpy(m_pTradeInfo->TradingDay , Tools::String2string(m_tradeDate).c_str(),sizeof(m_pTradeInfo->TradingDay));
			  strncpy(m_pTradeInfo->TradeDate , Tools::String2string(m_tradeDate).c_str(),sizeof(m_pTradeInfo->TradeDate));
			 strncpy(m_pTradeInfo->TradeTime ,  Tools::String2string((textBox_TradeTime->Text)->Trim()).c_str(),sizeof(m_pTradeInfo->TradeTime));
			 strncpy(m_pTradeInfo->InvestorID , Tools::String2string(comboBox_user->Text).c_str(),sizeof(m_pTradeInfo->InvestorID));
			 strncpy(m_pTradeInfo->InstrumentID , Tools::String2string(textBox_Ins->Text).c_str(),sizeof(m_pTradeInfo->InstrumentID));			 
			 m_pTradeInfo->OffsetFlag = m_pDataInfo->OffsetFlag2ID(Tools::String2string(comboBox_OffsetFlag->Text));
			 m_pTradeInfo->HedgeFlag = m_pDataInfo->HedgeFlag2ID(Tools::String2string(comboBox_HedgeFlag->Text));
			 m_pTradeInfo->Volume = atoi(Tools::String2string(textBox_volum->Text).c_str());
			 m_pTradeInfo->Price = atof(Tools::String2string(textBox_price->Text).c_str());
			 string strExchangeID=m_pDataInfo->GetExchangeID(m_pTradeInfo->InstrumentID);
			 strncpy(m_pTradeInfo->ExchangeID , strExchangeID.c_str(),sizeof(m_pTradeInfo->ExchangeID));
			 strncpy(m_pTradeInfo->Account , m_pTradeInfo->InvestorID,sizeof(m_pTradeInfo->InvestorID));
			 
             m_pTradeInfo->Direction = m_pDataInfo->TradeDirection2ID(Tools::String2string(comboBox_direction->Text));
			 sprintf(m_pTradeInfo->TradeID ,"%12s", Tools::String2string(textBox_TradeID->Text).c_str());					
			 sprintf(m_pTradeInfo->OrderSysID ,"%12s", Tools::String2string(textBox_OrderSysID->Text).c_str());
			 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
			 this->Close();
		 }
private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			  this->Close();
		 }
private: System::Void textBox_TradeID_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) 
		 {
			 //允许输入的字符
			 String^ AstrictChar = "0123456789";

			 // //「BackSpace」「Delete」后退键正常删除操作
			 if ((Keys)(e->KeyChar) == Keys::Back)
			 {
				 return;
			 }
			 //「Ctrl+C」(3)「Ctrl+X」(24)特殊组合键正常
			 //「Ctrl+Z」(26) 撤消组合键正常
			 if ((e->KeyChar == 3) || (e->KeyChar == 24) || (e->KeyChar == 26))
			 {
				 return;
			 }

			 //允许输入的字符外，
			 if (AstrictChar->IndexOf(e->KeyChar.ToString()) == -1)
			 {
				 e->Handled = true;
				 return;
			 }
		 }
private: System::Void textBox_OrderSysID_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) 
		 {
			 //允许输入的字符
			 String^ AstrictChar = "0123456789";

			 // //「BackSpace」「Delete」后退键正常删除操作
			 if ((Keys)(e->KeyChar) == Keys::Back )
			 {
				 return;
			 }
			 //「Ctrl+C」(3)「Ctrl+X」(24)特殊组合键正常
			 //「Ctrl+Z」(26) 撤消组合键正常
			 if ((e->KeyChar == 3) || (e->KeyChar == 24) || (e->KeyChar == 26))
			 {
				 return;
			 }

			 //允许输入的字符外，
			 if (AstrictChar->IndexOf(e->KeyChar.ToString()) == -1)
			 {
				 e->Handled = true;
				 return;
			 }
		 }
private: System::Void textBox_volum_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) 
		 {
			 //允许输入的字符
			 String^ AstrictChar = "0123456789";

			 // //「BackSpace」「Delete」后退键正常删除操作
			 if ((Keys)(e->KeyChar) == Keys::Back )
			 {
				 return;
			 }
			 //「Ctrl+C」(3)「Ctrl+X」(24)特殊组合键正常
			 //「Ctrl+Z」(26) 撤消组合键正常
			 if ((e->KeyChar == 3) || (e->KeyChar == 24) || (e->KeyChar == 26))
			 {
				 return;
			 }

			 //允许输入的字符外，
			 if (AstrictChar->IndexOf(e->KeyChar.ToString()) == -1)
			 {
				 e->Handled = true;
				 return;
			 }
		 }
private: System::Void textBox_price_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) 
		 {
			 //允许输入的字符
			 String^ AstrictChar = "0123456789.";

			 // //「BackSpace」「Delete」后退键正常删除操作
			 if ((Keys)(e->KeyChar) == Keys::Back )
			 {
				 return;
			 }
			 //「Ctrl+C」(3)「Ctrl+X」(24)特殊组合键正常
			 //「Ctrl+Z」(26) 撤消组合键正常
			 if ((e->KeyChar == 3) || (e->KeyChar == 24) || (e->KeyChar == 26))
			 {
				 return;
			 }

			 //允许输入的字符外，
			 if (AstrictChar->IndexOf(e->KeyChar.ToString()) == -1)
			 {
				 e->Handled = true;
				 return;
			 }
		 }

//private: System::Void dateTimePicker1_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
//		 {
//			 this->dateTimePicker1->CustomFormat = L"HH:mm:ss";
//		 }
//private: System::Void dateTimePicker1_DropDown(System::Object^  sender, System::EventArgs^  e) 
//		 {
//			 this->dateTimePicker1->CustomFormat = L"HH:mm:ss";
//		 }
//private: System::Void dateTimePicker1_CloseUp(System::Object^  sender, System::EventArgs^  e) {
//			  this->dateTimePicker1->CustomFormat = L"HH:mm:ss";
//		 }
};
}

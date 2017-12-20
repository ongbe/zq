#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Tools.h"
#include "CDataInfo.h"

namespace CltServerMgr {

	/// <summary>
	/// Summary for TradeAccountForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class TradeAccountForm : public System::Windows::Forms::Form
	{
	public:
		TradeAccountForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pType = new OperType;
            m_pCurTradeAccount = new TradeAccount;
            m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TradeAccountForm()
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

            if ( NULL != m_pCurTradeAccount )
            {
                delete m_pCurTradeAccount;
                m_pCurTradeAccount = NULL;
            }
		}
    /************************************************************************/
        ref class BrokerData
        {
        public:
            BrokerData(int nBrokerID, String^ strBrokerCode, String^ strCompany)
            {
                m_nBrokerID = nBrokerID;
                m_strCode = strBrokerCode;
                m_strCompany = strCompany;
            }

            virtual String^ ToString() override
            {
                String^ str = m_strCompany; /*+ "(";
                str = str + m_strCode;
                str = str + ")";*/
                return str;
            }

            int GetBrokerID()
            {
                return m_nBrokerID;
            }

		public:
			int m_nBrokerID;
			String^ m_strCode;
			String^ m_strCompany;
		};

        ref class ServerGroupData
        {
        public:
            ServerGroupData(int nServerGroupID,String^ strServerGroupName,int nBrokerID,int nIsSettle)
            {
                m_nServerGroupID = nServerGroupID;
                m_strServerGroupName = strServerGroupName;
                m_nBrokerID = nBrokerID;
                m_nIsSettle = nIsSettle;
            }

            virtual String^ ToString() override
            {
                String^ str = m_strServerGroupName;
                return str;
            }

            int GetServerGroupID()
            {
                return m_nServerGroupID;
            }

		public:
            int     m_nServerGroupID;
            String^ m_strServerGroupName;
            int     m_nBrokerID;
            int     m_nIsSettle;
		};
/************************************************************************/

    private: CDataInfo* m_pDataInfo;
    private: OperType* m_pType;
    private: TradeAccount* m_pCurTradeAccount;  


    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Label^  label4;
    private: System::Windows::Forms::Label^  label5;
    private: System::Windows::Forms::Label^  label6;
    private: System::Windows::Forms::Label^  label7;
    private: System::Windows::Forms::Label^  label8;
    private: System::Windows::Forms::Label^  label9;
    private: System::Windows::Forms::Label^  label10;
    private: System::Windows::Forms::TextBox^  TradeAccount_textBox;
    private: System::Windows::Forms::TextBox^  Pwd_textBox;
    private: System::Windows::Forms::TextBox^  Profit_textBox;
    private: System::Windows::Forms::TextBox^  Alias_textBox;
    private: System::Windows::Forms::TextBox^  RealName_textBox;
    private: System::Windows::Forms::TextBox^  ContactWay_textBox;
    private: System::Windows::Forms::TextBox^  IDCardNo_textBox;
    private: System::Windows::Forms::ComboBox^  Broker_comboBox;
    private: System::Windows::Forms::ComboBox^  ServerGroup_comboBox;
    private: System::Windows::Forms::ComboBox^  IDCardType_comboBox;

    private: System::Windows::Forms::Button^  Cancel_button;
    private: System::Windows::Forms::GroupBox^  groupBox1;
private: System::Windows::Forms::Button^  OK_button;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

    public:
        void SetOperType(OperType eType)
        {
            *m_pType = eType;
        }
        void SetCurTradeAccount(const TradeAccount& oTradeAccount)
        {
            *m_pCurTradeAccount = oTradeAccount;
        }

        TradeAccount* GetCurTradeAccount()
        {
            return m_pCurTradeAccount;
        }

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->label7 = (gcnew System::Windows::Forms::Label());
            this->label8 = (gcnew System::Windows::Forms::Label());
            this->label9 = (gcnew System::Windows::Forms::Label());
            this->label10 = (gcnew System::Windows::Forms::Label());
            this->TradeAccount_textBox = (gcnew System::Windows::Forms::TextBox());
            this->Pwd_textBox = (gcnew System::Windows::Forms::TextBox());
            this->Profit_textBox = (gcnew System::Windows::Forms::TextBox());
            this->Alias_textBox = (gcnew System::Windows::Forms::TextBox());
            this->RealName_textBox = (gcnew System::Windows::Forms::TextBox());
            this->ContactWay_textBox = (gcnew System::Windows::Forms::TextBox());
            this->IDCardNo_textBox = (gcnew System::Windows::Forms::TextBox());
            this->Broker_comboBox = (gcnew System::Windows::Forms::ComboBox());
            this->ServerGroup_comboBox = (gcnew System::Windows::Forms::ComboBox());
            this->IDCardType_comboBox = (gcnew System::Windows::Forms::ComboBox());
            this->Cancel_button = (gcnew System::Windows::Forms::Button());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->OK_button = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(29, 29);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(53, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"账户号：";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(29, 66);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(41, 12);
            this->label2->TabIndex = 0;
            this->label2->Text = L"密码：";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(29, 103);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(65, 12);
            this->label3->TabIndex = 0;
            this->label3->Text = L"经纪公司：";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Location = System::Drawing::Point(29, 140);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(65, 12);
            this->label4->TabIndex = 0;
            this->label4->Text = L"服务器组：";
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Location = System::Drawing::Point(29, 177);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(65, 12);
            this->label5->TabIndex = 0;
            this->label5->Text = L"初始权益：";
            this->label5->Visible = false;
            // 
            // label6
            // 
            this->label6->AutoSize = true;
            this->label6->Location = System::Drawing::Point(252, 29);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(41, 12);
            this->label6->TabIndex = 0;
            this->label6->Text = L"别名：";
            // 
            // label7
            // 
            this->label7->AutoSize = true;
            this->label7->Location = System::Drawing::Point(252, 66);
            this->label7->Name = L"label7";
            this->label7->Size = System::Drawing::Size(65, 12);
            this->label7->TabIndex = 0;
            this->label7->Text = L"真实姓名：";
            // 
            // label8
            // 
            this->label8->AutoSize = true;
            this->label8->Location = System::Drawing::Point(252, 103);
            this->label8->Name = L"label8";
            this->label8->Size = System::Drawing::Size(65, 12);
            this->label8->TabIndex = 0;
            this->label8->Text = L"联系方式：";
            // 
            // label9
            // 
            this->label9->AutoSize = true;
            this->label9->Location = System::Drawing::Point(252, 140);
            this->label9->Name = L"label9";
            this->label9->Size = System::Drawing::Size(65, 12);
            this->label9->TabIndex = 0;
            this->label9->Text = L"证件类型：";
            // 
            // label10
            // 
            this->label10->AutoSize = true;
            this->label10->Location = System::Drawing::Point(252, 177);
            this->label10->Name = L"label10";
            this->label10->Size = System::Drawing::Size(65, 12);
            this->label10->TabIndex = 0;
            this->label10->Text = L"证件号码：";
            // 
            // TradeAccount_textBox
            // 
            this->TradeAccount_textBox->Location = System::Drawing::Point(93, 25);
            this->TradeAccount_textBox->MaxLength = 13;
            this->TradeAccount_textBox->Name = L"TradeAccount_textBox";
            this->TradeAccount_textBox->Size = System::Drawing::Size(100, 21);
            this->TradeAccount_textBox->TabIndex = 1;
            // 
            // Pwd_textBox
            // 
            this->Pwd_textBox->Location = System::Drawing::Point(93, 62);
            this->Pwd_textBox->Name = L"Pwd_textBox";
            this->Pwd_textBox->PasswordChar = '*';
            this->Pwd_textBox->Size = System::Drawing::Size(100, 21);
            this->Pwd_textBox->TabIndex = 2;
            // 
            // Profit_textBox
            // 
            this->Profit_textBox->Location = System::Drawing::Point(93, 173);
            this->Profit_textBox->Name = L"Profit_textBox";
            this->Profit_textBox->Size = System::Drawing::Size(100, 21);
            this->Profit_textBox->TabIndex = 5;
            this->Profit_textBox->Visible = false;
            this->Profit_textBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &TradeAccountForm::textBox_KeyPress);
            // 
            // Alias_textBox
            // 
            this->Alias_textBox->Location = System::Drawing::Point(315, 25);
            this->Alias_textBox->Name = L"Alias_textBox";
            this->Alias_textBox->Size = System::Drawing::Size(100, 21);
            this->Alias_textBox->TabIndex = 6;
            // 
            // RealName_textBox
            // 
            this->RealName_textBox->Location = System::Drawing::Point(315, 62);
            this->RealName_textBox->Name = L"RealName_textBox";
            this->RealName_textBox->Size = System::Drawing::Size(100, 21);
            this->RealName_textBox->TabIndex = 7;
            // 
            // ContactWay_textBox
            // 
            this->ContactWay_textBox->Location = System::Drawing::Point(315, 99);
            this->ContactWay_textBox->Name = L"ContactWay_textBox";
            this->ContactWay_textBox->Size = System::Drawing::Size(100, 21);
            this->ContactWay_textBox->TabIndex = 8;
            // 
            // IDCardNo_textBox
            // 
            this->IDCardNo_textBox->Location = System::Drawing::Point(315, 173);
            this->IDCardNo_textBox->MaxLength = 19;
            this->IDCardNo_textBox->Name = L"IDCardNo_textBox";
            this->IDCardNo_textBox->Size = System::Drawing::Size(100, 21);
            this->IDCardNo_textBox->TabIndex = 10;
            this->IDCardNo_textBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &TradeAccountForm::textBox_KeyPress);
            // 
            // Broker_comboBox
            // 
            this->Broker_comboBox->FormattingEnabled = true;
            this->Broker_comboBox->Location = System::Drawing::Point(93, 99);
            this->Broker_comboBox->Name = L"Broker_comboBox";
            this->Broker_comboBox->Size = System::Drawing::Size(100, 20);
            this->Broker_comboBox->TabIndex = 3;
            this->Broker_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &TradeAccountForm::Broker_comboBox_SelectedIndexChanged);
            // 
            // ServerGroup_comboBox
            // 
            this->ServerGroup_comboBox->FormattingEnabled = true;
            this->ServerGroup_comboBox->Location = System::Drawing::Point(93, 136);
            this->ServerGroup_comboBox->Name = L"ServerGroup_comboBox";
            this->ServerGroup_comboBox->Size = System::Drawing::Size(100, 20);
            this->ServerGroup_comboBox->TabIndex = 4;
            // 
            // IDCardType_comboBox
            // 
            this->IDCardType_comboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->IDCardType_comboBox->FormattingEnabled = true;
            this->IDCardType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"身份证", L"军官证", L"教师证", L"其他"});
            this->IDCardType_comboBox->Location = System::Drawing::Point(315, 136);
            this->IDCardType_comboBox->Name = L"IDCardType_comboBox";
            this->IDCardType_comboBox->Size = System::Drawing::Size(100, 20);
            this->IDCardType_comboBox->TabIndex = 9;
            // 
            // Cancel_button
            // 
            this->Cancel_button->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Cancel_button->Location = System::Drawing::Point(251, 217);
            this->Cancel_button->Name = L"Cancel_button";
            this->Cancel_button->Size = System::Drawing::Size(75, 23);
            this->Cancel_button->TabIndex = 12;
            this->Cancel_button->Text = L"取消";
            this->Cancel_button->UseVisualStyleBackColor = true;
            this->Cancel_button->Click += gcnew System::EventHandler(this, &TradeAccountForm::Cancel_button_Click);
            // 
            // groupBox1
            // 
            this->groupBox1->Location = System::Drawing::Point(12, 7);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(419, 198);
            this->groupBox1->TabIndex = 4;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"用户资料";
            // 
            // OK_button
            // 
            this->OK_button->Location = System::Drawing::Point(118, 217);
            this->OK_button->Name = L"OK_button";
            this->OK_button->Size = System::Drawing::Size(75, 23);
            this->OK_button->TabIndex = 13;
            this->OK_button->Text = L"确定";
            this->OK_button->UseVisualStyleBackColor = true;
            this->OK_button->Click += gcnew System::EventHandler(this, &TradeAccountForm::OK_button_Click);
            // 
            // TradeAccountForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(453, 252);
            this->Controls->Add(this->OK_button);
            this->Controls->Add(this->Cancel_button);
            this->Controls->Add(this->IDCardType_comboBox);
            this->Controls->Add(this->ServerGroup_comboBox);
            this->Controls->Add(this->Broker_comboBox);
            this->Controls->Add(this->IDCardNo_textBox);
            this->Controls->Add(this->ContactWay_textBox);
            this->Controls->Add(this->RealName_textBox);
            this->Controls->Add(this->Alias_textBox);
            this->Controls->Add(this->Profit_textBox);
            this->Controls->Add(this->Pwd_textBox);
            this->Controls->Add(this->TradeAccount_textBox);
            this->Controls->Add(this->label10);
            this->Controls->Add(this->label5);
            this->Controls->Add(this->label9);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->label8);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label7);
            this->Controls->Add(this->label6);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->groupBox1);
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"TradeAccountForm";
            this->Text = L"委托交易账户";
            this->Load += gcnew System::EventHandler(this, &TradeAccountForm::TradeAccountForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
private: System::Void TradeAccountForm_Load(System::Object^  sender, System::EventArgs^  e) {
             this->CenterToScreen();

             //加载经纪公司
             LoadBroker();

             if ( *m_pType == Oper_Add )
             {
                 this->Text = "新增委托交易账户";

                 IDCardType_comboBox->SelectedIndex = 0;
             }
             else
             {
                 this->Text = "修改委托交易账户";

                 TradeAccount_textBox->Enabled = false;
                 TradeAccount_textBox->Text  = Tools::string2String(m_pCurTradeAccount->szTradeAccount);                 
                 Pwd_textBox->Text  = Tools::string2String(m_pCurTradeAccount->szPwd);
//                  Profit_textBox->Text = ::Convert::ToString(m_pCurTradeAccount->dProfit);
                 Alias_textBox->Text  = Tools::string2String(m_pCurTradeAccount->szAlias);                 
                 RealName_textBox->Text  = Tools::string2String(m_pCurTradeAccount->szRealName);
                 ContactWay_textBox->Text = Tools::string2String(m_pCurTradeAccount->szContact);
                 IDCardNo_textBox->Text  = Tools::string2String(m_pCurTradeAccount->szIDCardNo); 

                 std::string strBroker = m_pDataInfo->GetBrokerNameByID(m_pCurTradeAccount->nBrokerID); 
                 std::string strServerGroup = m_pDataInfo->GetServerGroupNameByID(m_pCurTradeAccount->nBrokerID,m_pCurTradeAccount->nServerGroupID);

                 Broker_comboBox->Text = Tools::string2String(strBroker.c_str());
                 ServerGroup_comboBox->Text = Tools::string2String(strServerGroup.c_str());

                 array<String^>^ strCardType = {"身份证","军官证","教师证","其他"};
                 IDCardType_comboBox->Text = strCardType[m_pCurTradeAccount->nIDCardType];    
             }
         }
private: System::Void OK_button_Click(System::Object^  sender, System::EventArgs^  e) {
           if(String::IsNullOrEmpty(TradeAccount_textBox->Text)) 
           {
               MessageBox::Show("账户号不允许为空!");  
               return;
           }

           if(String::IsNullOrEmpty(Pwd_textBox->Text)) 
           {
               MessageBox::Show("密码不允许为空!");  
               return;
           }

           if(Broker_comboBox->Items->Count>0 && Broker_comboBox->SelectedIndex < 0) 
           {
               MessageBox::Show("经纪公司选择不正确!");  
               return;
           }

           if(ServerGroup_comboBox->Items->Count>0 && ServerGroup_comboBox->SelectedIndex<0) 
           {
               MessageBox::Show("服务器组选择不正确!");  
               return;
           }

//            if(String::IsNullOrEmpty(Profit_textBox->Text)) 
//            {
//                MessageBox::Show("初始权益不允许为空!");  
//                return;
//            }

           TradeAccount oTA;
           if(*m_pType == Oper_Modify) oTA.nTradeAccountID = m_pCurTradeAccount->nTradeAccountID;
           strcpy(oTA.szTradeAccount,Tools::String2string(TradeAccount_textBox->Text).c_str());
           strcpy(oTA.szPwd,Tools::String2string(Pwd_textBox->Text).c_str()); 
           strcpy(oTA.szAlias,Tools::String2string(Alias_textBox->Text).c_str());
           strcpy(oTA.szRealName,Tools::String2string(RealName_textBox->Text).c_str()); 
           strcpy(oTA.szContact,Tools::String2string(ContactWay_textBox->Text).c_str());
           strcpy(oTA.szIDCardNo,Tools::String2string(IDCardNo_textBox->Text).c_str()); 

//            oTA.dProfit = ::Convert::ToDouble(Profit_textBox->Text);
           oTA.nBrokerID =m_pDataInfo->GetBrokerIDByName(Tools::String2string(Broker_comboBox->Text));
           oTA.nServerGroupID = m_pDataInfo->GetServerGroupID(oTA.nBrokerID,Tools::String2string(ServerGroup_comboBox->Text));
           oTA.nIDCardType = IDCardType_comboBox->SelectedIndex;


           if(*m_pType==Oper_Add)
           {
               if(!m_pDataInfo->IsValidOfTradeAccount(Tools::String2string(TradeAccount_textBox->Text)))    
               {
                   MessageBox::Show("委托交易账号名称重名,请重新输入!");                 
                   return;
               }

               memcpy(m_pCurTradeAccount,&oTA,sizeof(TradeAccount));

               this->DialogResult = ::DialogResult::OK;
               this->Close();
           }

           if(*m_pType==Oper_Modify)
           {
               if(*m_pCurTradeAccount == oTA)
                   this->DialogResult = ::DialogResult::Cancel;
               else
               {
                   if(0==strcmp(m_pCurTradeAccount->szTradeAccount,oTA.szTradeAccount))
                   {
                       this->DialogResult = ::DialogResult::OK;
                       memcpy(m_pCurTradeAccount,&oTA,sizeof(TradeAccount));
                       this->Close();
                   }
                   else
                   {
                       if(!m_pDataInfo->IsValidOfTradeAccount(oTA.szTradeAccount))
                       {                             
                           MessageBox::Show("委托交易账号名称重名,请重新输入!"); 
                       }
                       else
                       {
                           this->DialogResult = ::DialogResult::OK;
                           memcpy(m_pCurTradeAccount,&oTA,sizeof(TradeAccount));
                           this->Close();
                       }
                   }
               }                 
           }
       }
private: System::Void Cancel_button_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
private: System::Void Broker_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             ServerGroup_comboBox->Items->Clear();

			 BrokerData^ brokerData = (BrokerData^)Broker_comboBox->SelectedItem;

			 std::map<int, ServerGroup> mapServerGroup;
			 m_pDataInfo->GetServerGroups(brokerData->GetBrokerID(), mapServerGroup);
             if(mapServerGroup.empty())   
                 return;          

			 std::map<int, ServerGroup>::iterator it = mapServerGroup.begin();
			 for ( int i = 0; it != mapServerGroup.end(); it++,i++ )
			 {
                 ServerGroupData^  data = gcnew ServerGroupData(it->first, 
                                                                Tools::string2String(it->second.szServerName),
                                                                it->second.nBrokerID,
                                                                it->second.nIsSettle);
                 ServerGroup_comboBox->Items->Add(data);
			 } 

             ServerGroup_comboBox->SelectedIndex = 0;
         }
private: System::Void textBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             if (e->KeyChar < '0' || e->KeyChar > '9')   
                 if (e->KeyChar != (char)8)    
                     e->Handled = true;
         }
private: void LoadBroker()
         {
             std::map<int,BrokerInfo> mapBrokerInfo;
             m_pDataInfo->GetBrokerInfos(mapBrokerInfo);
             if(!mapBrokerInfo.empty())
             {
                 BrokerData^ data = nullptr;
                 std::map<int,BrokerInfo>::iterator it = mapBrokerInfo.begin();
                 for(;it != mapBrokerInfo.end();it++)
                 {
                     data = gcnew BrokerData(it->second.nBrokerID, 
                         Tools::string2String(it->second.szBrokerCode),
                         Tools::string2String(it->second.szCompany));

                     Broker_comboBox->Items->Add(data); 
                 }     
             }
         }
};
}

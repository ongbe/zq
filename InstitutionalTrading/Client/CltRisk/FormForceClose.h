#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "RiskData.h"
#include "Tools.h"

namespace CltRisk {

	/// <summary>
	/// Summary for FormForceClose
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormForceClose : public System::Windows::Forms::Form
	{
	public:
		FormForceClose(sForceClose& oFC,double fPriceTick)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

            textBox_AccountID->Text = Tools::string2String(oFC.AccountID);
            textBox_InstrumentID->Text = Tools::string2String(oFC.InstrumentID);
            textBox_Direction->Text = (oFC.Direction == THOST_FTDC_D_Buy)?"买":"卖";
            comboBox_OffsetFlag->SelectedIndex = 0;
            textBox_Price->Text = (oFC.Direction == '0')?(Tools::string2String(Price2string(oFC.UpperLimitPrice,fPriceTick).c_str())):(Tools::string2String(Price2string(oFC.LowerLimitPrice,fPriceTick).c_str()));//oFC.UpperLimitPrice.ToString("F2"):oFC.LowerLimitPrice.ToString("F2");
            textBox_LimitPrice->Text = Tools::string2String(Price2string(oFC.UpperLimitPrice,fPriceTick).c_str())+"/"+Tools::string2String(Price2string(oFC.LowerLimitPrice,fPriceTick).c_str());
            textBox_OpenInterest->Text = oFC.OpenInterest.ToString("G");
            textBox_TodayOpenInterest->Text = oFC.TodayOpenInterest.ToString("G");

            this->CenterToParent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormForceClose()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::Button^  OkButton;
    private: System::Windows::Forms::ComboBox^  comboBox_OffsetFlag;
    private: System::Windows::Forms::TextBox^  textBox_TodayOpenInterest;

    protected: 




    private: System::Windows::Forms::TextBox^  textBox_OpenInterest;

    private: System::Windows::Forms::TextBox^  textBox_LimitPrice;

    private: System::Windows::Forms::TextBox^  textBox_Price;

    private: System::Windows::Forms::TextBox^  textBox_Volume;

    private: System::Windows::Forms::TextBox^  textBox_Direction;

    private: System::Windows::Forms::TextBox^  textBox_InstrumentID;


    private: System::Windows::Forms::TextBox^  textBox_AccountID;




    private: System::Windows::Forms::Label^  label12;
    private: System::Windows::Forms::Label^  label10;
    private: System::Windows::Forms::Label^  label9;
    private: System::Windows::Forms::Label^  label11;

    private: System::Windows::Forms::Label^  label8;
    private: System::Windows::Forms::Label^  label4;
    private: System::Windows::Forms::Label^  label7;

    private: System::Windows::Forms::Label^  label6;

    private: System::Windows::Forms::Label^  label1;

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
            this->OkButton = (gcnew System::Windows::Forms::Button());
            this->comboBox_OffsetFlag = (gcnew System::Windows::Forms::ComboBox());
            this->textBox_TodayOpenInterest = (gcnew System::Windows::Forms::TextBox());
            this->textBox_OpenInterest = (gcnew System::Windows::Forms::TextBox());
            this->textBox_LimitPrice = (gcnew System::Windows::Forms::TextBox());
            this->textBox_Price = (gcnew System::Windows::Forms::TextBox());
            this->textBox_Volume = (gcnew System::Windows::Forms::TextBox());
            this->textBox_Direction = (gcnew System::Windows::Forms::TextBox());
            this->textBox_InstrumentID = (gcnew System::Windows::Forms::TextBox());
            this->textBox_AccountID = (gcnew System::Windows::Forms::TextBox());
            this->label12 = (gcnew System::Windows::Forms::Label());
            this->label10 = (gcnew System::Windows::Forms::Label());
            this->label9 = (gcnew System::Windows::Forms::Label());
            this->label11 = (gcnew System::Windows::Forms::Label());
            this->label8 = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->label7 = (gcnew System::Windows::Forms::Label());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->SuspendLayout();
            // 
            // OkButton
            // 
            this->OkButton->Location = System::Drawing::Point(98, 352);
            this->OkButton->Name = L"OkButton";
            this->OkButton->Size = System::Drawing::Size(75, 23);
            this->OkButton->TabIndex = 4;
            this->OkButton->Text = L"强平下单";
            this->OkButton->UseVisualStyleBackColor = true;
            this->OkButton->Click += gcnew System::EventHandler(this, &FormForceClose::OkButton_Click);
            // 
            // comboBox_OffsetFlag
            // 
            this->comboBox_OffsetFlag->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->comboBox_OffsetFlag->FormattingEnabled = true;
            this->comboBox_OffsetFlag->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"平仓", L"平今", L"平昨"});
            this->comboBox_OffsetFlag->Location = System::Drawing::Point(104, 125);
            this->comboBox_OffsetFlag->Name = L"comboBox_OffsetFlag";
            this->comboBox_OffsetFlag->Size = System::Drawing::Size(141, 20);
            this->comboBox_OffsetFlag->TabIndex = 1;
            // 
            // textBox_TodayOpenInterest
            // 
            this->textBox_TodayOpenInterest->Enabled = false;
            this->textBox_TodayOpenInterest->Location = System::Drawing::Point(104, 289);
            this->textBox_TodayOpenInterest->Name = L"textBox_TodayOpenInterest";
            this->textBox_TodayOpenInterest->Size = System::Drawing::Size(141, 21);
            this->textBox_TodayOpenInterest->TabIndex = 23;
            // 
            // textBox_OpenInterest
            // 
            this->textBox_OpenInterest->Enabled = false;
            this->textBox_OpenInterest->Location = System::Drawing::Point(104, 256);
            this->textBox_OpenInterest->Name = L"textBox_OpenInterest";
            this->textBox_OpenInterest->Size = System::Drawing::Size(141, 21);
            this->textBox_OpenInterest->TabIndex = 21;
            // 
            // textBox_LimitPrice
            // 
            this->textBox_LimitPrice->Enabled = false;
            this->textBox_LimitPrice->Location = System::Drawing::Point(104, 223);
            this->textBox_LimitPrice->Name = L"textBox_LimitPrice";
            this->textBox_LimitPrice->Size = System::Drawing::Size(141, 21);
            this->textBox_LimitPrice->TabIndex = 19;
            // 
            // textBox_Price
            // 
            this->textBox_Price->Location = System::Drawing::Point(104, 190);
            this->textBox_Price->Name = L"textBox_Price";
            this->textBox_Price->Size = System::Drawing::Size(141, 21);
            this->textBox_Price->TabIndex = 3;
            // 
            // textBox_Volume
            // 
            this->textBox_Volume->Location = System::Drawing::Point(104, 157);
            this->textBox_Volume->Name = L"textBox_Volume";
            this->textBox_Volume->Size = System::Drawing::Size(141, 21);
            this->textBox_Volume->TabIndex = 2;
            //this->textBox_Volume->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &FormForceClose::textBox_KeyPress);
            // 
            // textBox_Direction
            // 
            this->textBox_Direction->Enabled = false;
            this->textBox_Direction->Location = System::Drawing::Point(104, 92);
            this->textBox_Direction->Name = L"textBox_Direction";
            this->textBox_Direction->Size = System::Drawing::Size(141, 21);
            this->textBox_Direction->TabIndex = 28;
            // 
            // textBox_InstrumentID
            // 
            this->textBox_InstrumentID->Enabled = false;
            this->textBox_InstrumentID->Location = System::Drawing::Point(104, 59);
            this->textBox_InstrumentID->Name = L"textBox_InstrumentID";
            this->textBox_InstrumentID->Size = System::Drawing::Size(141, 21);
            this->textBox_InstrumentID->TabIndex = 2;
            // 
            // textBox_AccountID
            // 
            this->textBox_AccountID->Enabled = false;
            this->textBox_AccountID->Location = System::Drawing::Point(104, 26);
            this->textBox_AccountID->Name = L"textBox_AccountID";
            this->textBox_AccountID->Size = System::Drawing::Size(141, 21);
            this->textBox_AccountID->TabIndex = 1;
            // 
            // label12
            // 
            this->label12->AutoSize = true;
            this->label12->Location = System::Drawing::Point(28, 294);
            this->label12->Name = L"label12";
            this->label12->Size = System::Drawing::Size(65, 12);
            this->label12->TabIndex = 9;
            this->label12->Text = L"可平今量：";
            // 
            // label10
            // 
            this->label10->AutoSize = true;
            this->label10->Location = System::Drawing::Point(28, 228);
            this->label10->Name = L"label10";
            this->label10->Size = System::Drawing::Size(65, 12);
            this->label10->TabIndex = 10;
            this->label10->Text = L"涨跌停价：";
            // 
            // label9
            // 
            this->label9->AutoSize = true;
            this->label9->Location = System::Drawing::Point(28, 195);
            this->label9->Name = L"label9";
            this->label9->Size = System::Drawing::Size(65, 12);
            this->label9->TabIndex = 5;
            this->label9->Text = L"平仓价格：";
            // 
            // label11
            // 
            this->label11->AutoSize = true;
            this->label11->Location = System::Drawing::Point(40, 261);
            this->label11->Name = L"label11";
            this->label11->Size = System::Drawing::Size(53, 12);
            this->label11->TabIndex = 6;
            this->label11->Text = L"可平量：";
            // 
            // label8
            // 
            this->label8->AutoSize = true;
            this->label8->Location = System::Drawing::Point(28, 162);
            this->label8->Name = L"label8";
            this->label8->Size = System::Drawing::Size(65, 12);
            this->label8->TabIndex = 11;
            this->label8->Text = L"平仓手数：";
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Location = System::Drawing::Point(52, 64);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(41, 12);
            this->label4->TabIndex = 15;
            this->label4->Text = L"合约：";
            // 
            // label7
            // 
            this->label7->AutoSize = true;
            this->label7->Location = System::Drawing::Point(28, 129);
            this->label7->Name = L"label7";
            this->label7->Size = System::Drawing::Size(65, 12);
            this->label7->TabIndex = 16;
            this->label7->Text = L"平仓类型：";
            // 
            // label6
            // 
            this->label6->AutoSize = true;
            this->label6->Location = System::Drawing::Point(52, 97);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(41, 12);
            this->label6->TabIndex = 12;
            this->label6->Text = L"买卖：";
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(40, 31);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(53, 12);
            this->label1->TabIndex = 14;
            this->label1->Text = L"交易员：";
            // 
            // FormForceClose
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(271, 403);
            this->Controls->Add(this->OkButton);
            this->Controls->Add(this->comboBox_OffsetFlag);
            this->Controls->Add(this->textBox_TodayOpenInterest);
            this->Controls->Add(this->textBox_OpenInterest);
            this->Controls->Add(this->textBox_LimitPrice);
            this->Controls->Add(this->textBox_Price);
            this->Controls->Add(this->textBox_Volume);
            this->Controls->Add(this->textBox_Direction);
            this->Controls->Add(this->textBox_InstrumentID);
            this->Controls->Add(this->textBox_AccountID);
            this->Controls->Add(this->label12);
            this->Controls->Add(this->label10);
            this->Controls->Add(this->label9);
            this->Controls->Add(this->label11);
            this->Controls->Add(this->label8);
            this->Controls->Add(this->label4);
            this->Controls->Add(this->label7);
            this->Controls->Add(this->label6);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"FormForceClose";
            this->Text = L"手动强平";
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

        private:
/*
            int GetNumDigits(double dbTick)
            {
                int pos,nNumDigits=0;
                char strPriceText[512]={0};
                sprintf_s(strPriceText,512, "%f", dbTick);
                for(pos=strlen(strPriceText)-1;pos>=0;pos--)
                {
                    if(strPriceText[pos]=='0')
                        strPriceText[pos]=0;
                    else
                        break;
                }
                for(pos=strlen(strPriceText)-1;pos>=0;pos--)
                {
                    if(strPriceText[pos]!='.')
                        nNumDigits++;
                    else
                        break;
                }
                return nNumDigits;

            }
            string Price2string(double dbData, double dbTick)
            {
                char strPriceText[512]={0},strFormatText[512]={0};
                strcpy_s(strPriceText, "-");
                if(util::isInvalidValue(dbData)||util::isInvalidValue(dbTick)) 
                    return strPriceText;
                sprintf_s(strFormatText,"%%0.%df", GetNumDigits(dbTick));
                sprintf_s(strPriceText, strFormatText, dbData);
                return string(strPriceText);
            }
*/
            bool Verify()
            {
                bool bRet = true;

                String^ textVolume = textBox_Volume->Text;
                if(String::IsNullOrEmpty(textBox_Volume->Text) || textVolume == "0")
                {
                    MessageBox::Show("平仓手数不能为空或零");
                    return !bRet;
                }

                int rlt = textVolume->IndexOf('.');
                if( rlt!=-1)
                {
                    MessageBox::Show("平仓手数不支持小数，请重新输入");
                    return !bRet;
                }

                if(String::IsNullOrEmpty(textBox_Price->Text))
                {
                    MessageBox::Show("平仓价格不能为空");
                    return !bRet;
                }


                int nVolume = Convert::ToInt32(textBox_Volume->Text);
                int nOpenInterest = Convert::ToInt32(textBox_OpenInterest->Text);
                int nTodayOpenInterest = Convert::ToInt32(textBox_TodayOpenInterest->Text);
                switch(comboBox_OffsetFlag->SelectedIndex)
                {
                case 0://平仓
                    if(nVolume>nOpenInterest)
                    {
                        MessageBox::Show("平仓不足");
                        return !bRet;
                    }
                    break;
                case 1://平今
                    if (nVolume>nTodayOpenInterest)
                    {
                        MessageBox::Show("平今不足"); 
                        return !bRet;
                    }
                    break;
                case 2://平昨
                    if(nVolume>(nOpenInterest-nTodayOpenInterest))
                    {
                        MessageBox::Show("平昨不足");
                        return !bRet;
                    }
                    break;
                }

                return bRet;
            }

// private: System::Void textBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
//              if ((e->KeyChar <= 48 || e->KeyChar >=57) && (e->KeyChar != 8))  
//                  e->Handled = true;  
// 
//          }

        public:
                bool GetParam(PlatformStru_InputOrder& OrderInsert)
                {
                    string szAccount = Tools::String2string(textBox_AccountID->Text);
                    string szInst    = Tools::String2string(textBox_InstrumentID->Text);
                    double dLimitPrice = Convert::ToDouble(textBox_Price->Text);
                    char   cDirect = (textBox_Direction->Text == "买")?THOST_FTDC_D_Buy:THOST_FTDC_D_Sell;  
                    char   cOffsetFlag = (comboBox_OffsetFlag->SelectedIndex==0)?THOST_FTDC_OF_Close:((comboBox_OffsetFlag->SelectedIndex==1)?THOST_FTDC_OF_CloseToday:THOST_FTDC_OF_CloseYesterday);
                    int nVolume = Convert::ToInt32(textBox_Volume->Text);

                    memcpy((void*)OrderInsert.InvestorID,szAccount.c_str(),sizeof(OrderInsert.InvestorID)-1);
                    memcpy((void*)OrderInsert.InstrumentID,szInst.c_str(),sizeof(OrderInsert.InstrumentID)-1);
                    OrderInsert.OrderPriceType='2';					//市价 THOST_FTDC_OPT_AnyPrice='1' 限价 THOST_FTDC_OPT_LimitPrice='2';
                    OrderInsert.LimitPrice = dLimitPrice;
                    OrderInsert.Direction=cDirect;			        //买卖方向，0-'0'=买；1-'1'=卖
                    OrderInsert.CombOffsetFlag[0]=cOffsetFlag;	    //开平标志，0-'1'=平仓 1-'3'=平今 2-'4'=平今
                    OrderInsert.CombHedgeFlag[0]='1';				//投机套保，'1'=投机；'3'=套保
                    OrderInsert.VolumeTotalOriginal=nVolume;		//数量
                    OrderInsert.TimeCondition='3';					//有效期类型，市价单对应：立即完成，否则撤销='1'; 限价单对应：当日有效='3'
                    OrderInsert.VolumeCondition='1';				//成交量类型，'1'表示任何数量
                    OrderInsert.MinVolume=1;
                    OrderInsert.ContingentCondition='1';			//立即触发
                    OrderInsert.ForceCloseReason='0';				//强平原因，'0'费强平
                    OrderInsert.OrderClass='N';						//新报单         
                    memcpy((void*)OrderInsert.strAccount,szAccount.c_str(),sizeof(OrderInsert.strAccount)-1);


                    return true;
                }


    private: System::Void OkButton_Click(System::Object^  sender, System::EventArgs^  e) {
                 if(Verify())
                     this->DialogResult = ::DialogResult::OK;
             }
};
}

#pragma once

#include "RiskData.h"
#include "CommonStruct.h"
#include "RiskManageStruct.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

extern map<int,double>  g_mapRiskLevelInfo2;

namespace CltRisk {

	/// <summary>
	/// Summary for FormRiskPlanSetup_Add_RiskLevel
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormRiskPlanSetup_Add_RiskLevel : public System::Windows::Forms::Form
	{
	public:
		FormRiskPlanSetup_Add_RiskLevel(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_nOrgID = -1;
            m_nRiskIndicatorID = -1;
            m_nRiskLevel = 0;
            m_bEmpty = true;
            m_pType = new OperType;
            m_pCurRiskWaring = new RiskWaring;
            m_szActionText = String::Empty;
		}
        FormRiskPlanSetup_Add_RiskLevel(int nOrgID,int nRiskIndicatorID)
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //
            m_nOrgID = nOrgID;
            m_nRiskIndicatorID = nRiskIndicatorID;
            m_nRiskLevel = 0;
            m_bEmpty = true;
            m_pType = new OperType;
            m_pCurRiskWaring = new RiskWaring;
            m_szActionText = String::Empty;
        }
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormRiskPlanSetup_Add_RiskLevel()
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
            if ( NULL != m_pCurRiskWaring )
            {
                delete m_pCurRiskWaring;
                m_pCurRiskWaring = NULL;
            }
		}
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::TextBox^  textBox_RiskLevel;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::TextBox^  textBox_LevelValue;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::CheckedListBox^  checkedListBox_Action;


    private: System::Windows::Forms::Button^  button1;
    private: System::Windows::Forms::Button^  button2;
    protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

    private:
        OperType* m_pType;
        RiskWaring* m_pCurRiskWaring; 
        int m_ActionValue;
        int m_nRiskLevel;
        int m_nOrgID;
        int m_nRiskIndicatorID;
        String^ m_szActionText;
        float m_fRiskValue;
    private: System::Windows::Forms::Label^  label_Unit;
             bool m_bEmpty;
        
    public:
        void SetOperType(OperType eType)
        {
            *m_pType = eType;
        }
        void SetCurRiskWaring(const RiskWaring& riskWaring)
        {
            *m_pCurRiskWaring = riskWaring;
        }
        RiskWaring* GetCurRiskWaring()
        {
            return m_pCurRiskWaring;
        }
        void SetDialogTitle(String^ szTitle)
        {
            this->Text = szTitle;
        }
        void SetRiskLevelNumber(int nLevel)
        {
            m_nRiskLevel = nLevel;
        }
        void SetRiskValue(float riskValue)
        {
            m_fRiskValue = riskValue;
        }
        void SetEmpty(bool bValue)
        {
            m_bEmpty = bValue;
        }
        void SetRiskLevelText(String^ szRiskLevel)
        {
            textBox_RiskLevel->Text = szRiskLevel;
        }
        String^ GetRiskLevelText()
        {
            return textBox_RiskLevel->Text;
        }
        void SetLevelValueText(String^ szLevelValue)
        {
            textBox_LevelValue->Text = szLevelValue;
        }
        String^ GetLevelValueText()
        {
            return textBox_LevelValue->Text;
        }
        void SetAction(String^ szActionText)
        {
            array<System::String^>^ sArray = szActionText->Split(';');
            if(sArray->Length == 0)
                return;

            String^ szAction1 = String::Empty;
            String^ szAction2 = String::Empty;
            for (int i = 0; i < checkedListBox_Action->Items->Count; i++)
            {
                  szAction1 = checkedListBox_Action->GetItemText(checkedListBox_Action->Items[i]);
                  for each (String^ item in sArray)  
                  {
                      szAction2 = item->ToString()->Trim();
                      if ( 0==szAction1->CompareTo(szAction2))
                          checkedListBox_Action->SetItemCheckState(i,CheckState::Checked);
                  }

            }
        }
        void SetActionText(String^ szActionText)
        {
            m_szActionText = szActionText;
        }   
        String^ GetActionText()
        {
            String^ strCollected = String::Empty;
            for (int i = 0; i < checkedListBox_Action->Items->Count; i++)
            {
                if (checkedListBox_Action->GetItemChecked(i))
                {
                    if (strCollected == String::Empty)
                        strCollected = checkedListBox_Action->GetItemText(checkedListBox_Action->Items[i]);
                    else
                        strCollected = strCollected + ";" + checkedListBox_Action->GetItemText(checkedListBox_Action->Items[i]);
                }
            }

            return strCollected;
        }
        int GetActionValue()
        {
            return m_ActionValue;
        }

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->textBox_RiskLevel = (gcnew System::Windows::Forms::TextBox());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->textBox_LevelValue = (gcnew System::Windows::Forms::TextBox());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->checkedListBox_Action = (gcnew System::Windows::Forms::CheckedListBox());
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->button2 = (gcnew System::Windows::Forms::Button());
            this->label_Unit = (gcnew System::Windows::Forms::Label());
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(12, 22);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(53, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"风险等级";
            // 
            // textBox_RiskLevel
            // 
            this->textBox_RiskLevel->Location = System::Drawing::Point(77, 19);
            this->textBox_RiskLevel->Name = L"textBox_RiskLevel";
            this->textBox_RiskLevel->ReadOnly = true;
            this->textBox_RiskLevel->Size = System::Drawing::Size(148, 21);
            this->textBox_RiskLevel->TabIndex = 1;
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(36, 61);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(29, 12);
            this->label2->TabIndex = 2;
            this->label2->Text = L"阀值";
            // 
            // textBox_LevelValue
            // 
            this->textBox_LevelValue->Location = System::Drawing::Point(77, 58);
            this->textBox_LevelValue->MaxLength = 20;
            this->textBox_LevelValue->Name = L"textBox_LevelValue";
            this->textBox_LevelValue->Size = System::Drawing::Size(148, 21);
            this->textBox_LevelValue->TabIndex = 3;
            this->textBox_LevelValue->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &FormRiskPlanSetup_Add_RiskLevel::textBox_LevelValue_KeyPress);
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(36, 98);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(29, 12);
            this->label3->TabIndex = 4;
            this->label3->Text = L"动作";
            // 
            // checkedListBox_Action
            // 
            this->checkedListBox_Action->CheckOnClick = true;
            this->checkedListBox_Action->FormattingEnabled = true;
            this->checkedListBox_Action->Location = System::Drawing::Point(77, 98);
            this->checkedListBox_Action->Name = L"checkedListBox_Action";
            this->checkedListBox_Action->Size = System::Drawing::Size(148, 164);
            this->checkedListBox_Action->TabIndex = 5;
            // 
            // button1
            // 
            this->button1->Location = System::Drawing::Point(56, 292);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(75, 23);
            this->button1->TabIndex = 6;
            this->button1->Text = L"确定";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Click += gcnew System::EventHandler(this, &FormRiskPlanSetup_Add_RiskLevel::button_OK_Click);
            // 
            // button2
            // 
            this->button2->Location = System::Drawing::Point(162, 292);
            this->button2->Name = L"button2";
            this->button2->Size = System::Drawing::Size(75, 23);
            this->button2->TabIndex = 7;
            this->button2->Text = L"取消";
            this->button2->UseVisualStyleBackColor = true;
            this->button2->Click += gcnew System::EventHandler(this, &FormRiskPlanSetup_Add_RiskLevel::button_Cancel_Click);
            // 
            // label_Unit
            // 
            this->label_Unit->AutoSize = true;
            this->label_Unit->Location = System::Drawing::Point(231, 63);
            this->label_Unit->Name = L"label_Unit";
            this->label_Unit->Size = System::Drawing::Size(0, 12);
            this->label_Unit->TabIndex = 8;
            // 
            // FormRiskPlanSetup_Add_RiskLevel
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(284, 330);
            this->Controls->Add(this->label_Unit);
            this->Controls->Add(this->button2);
            this->Controls->Add(this->button1);
            this->Controls->Add(this->checkedListBox_Action);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->textBox_LevelValue);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->textBox_RiskLevel);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"FormRiskPlanSetup_Add_RiskLevel";
            this->Text = L"新增风险等级";
            this->Load += gcnew System::EventHandler(this, &FormRiskPlanSetup_Add_RiskLevel::FormRiskPlanSetup_Add_RiskLevel_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void FormRiskPlanSetup_Add_RiskLevel_Load(System::Object^  sender, System::EventArgs^  e) {
                 CenterToScreen();

                 checkedListBox_Action->Items->Clear();  
                 
                 checkedListBox_Action->Items->Add("鸣笛", false); 
                 checkedListBox_Action->Items->Add("震动", false);
                 label_Unit->Text = "%";

                 switch(m_nRiskIndicatorID)
                 {
                 case RI_MaxPosition://合约最大持仓
                     checkedListBox_Action->Items->Add("限制开仓(单合约)", false); 
                     checkedListBox_Action->Items->Add("限制下单(单合约)", false); 
                     label_Unit->Text = "手数";
                     break;
                 case RI_MarketShock://市场冲击风险
                     checkedListBox_Action->Items->Add("限制开仓(单合约)", false); 
                     checkedListBox_Action->Items->Add("限制下单(单合约)", false);
                     checkedListBox_Action->Items->Add("市价强平(单合约)", false);                     
                     break;
                 case RI_AccountLoss://账户限亏
                     checkedListBox_Action->Items->Add("限制开仓", false); 
                     checkedListBox_Action->Items->Add("限制下单", false);
                     checkedListBox_Action->Items->Add("市价强平", false);
                     break;
                 case RI_TodayLoss://今日限亏
                     checkedListBox_Action->Items->Add("限制开仓", false); 
                     checkedListBox_Action->Items->Add("限制下单", false);
                     checkedListBox_Action->Items->Add("市价强平", false);
                     break;
                 case RI_GappedMarket://隔夜跳空
                     checkedListBox_Action->Items->Add("限制开仓", false); 
                     checkedListBox_Action->Items->Add("限制下单", false);
                     checkedListBox_Action->Items->Add("市价强平", false);
                     break;
                 case RI_MarginRatio://保证金比例风险
                     checkedListBox_Action->Items->Add("限制开仓", false); 
                     checkedListBox_Action->Items->Add("限制下单", false);
                     checkedListBox_Action->Items->Add("市价强平", false);
                     break;
                 case RI_SingleHoldTime://单笔持仓时间
                     checkedListBox_Action->Items->Add("市价强平(单合约单笔)", false); 
                     label_Unit->Text = "秒数";
                     break;
                 case RI_FundNetValue://基金净值风险
                     checkedListBox_Action->Items->Add("限制开仓", false); 
                     checkedListBox_Action->Items->Add("限制下单", false);
                     checkedListBox_Action->Items->Add("市价强平", false);  
                     label_Unit->Text = "";
                     break;
                 }

                 checkedListBox_Action->Items->Add("消息提示", false); 

                 SetAction(m_szActionText);
             }
private: System::Void button_OK_Click(System::Object^  sender, System::EventArgs^  e) {
             if(String::IsNullOrEmpty(textBox_LevelValue->Text))
             {
                 MessageBox::Show("请填风险阀值");
                 return;
             }
             if(checkedListBox_Action->CheckedItems->Count==0)
             {
                 MessageBox::Show("请选择风险动作");
                 return;
             }

             //判断风险阀值大小顺序
             float fRiskValue = (float)Convert::ToDouble(textBox_LevelValue->Text);
             int nLeveLCount = (int)g_mapRiskLevelInfo2.size();


             int m_nRiskLevel2 = m_nRiskLevel;
             switch(m_nRiskIndicatorID)
             {
             case RI_MaxPosition:
             case RI_MarketShock:
             case RI_AccountLoss:
             case RI_TodayLoss:
             case RI_GappedMarket:
             case RI_MarginRatio:
             case RI_SingleHoldTime:
                 if(*m_pType == Oper_Add) 
                 {
                     if (!g_mapRiskLevelInfo2.empty())
                     {
                         map<int,double>::iterator it = g_mapRiskLevelInfo2.find(m_nRiskLevel-1);
                         if(it!=g_mapRiskLevelInfo2.end())
                         {
                             if(fRiskValue>=it->second)
                             {
                                 MessageBox::Show("阀值的大小要按风险方案设置(倒序)，请参考相关说明文档");
                                 return;
                             }
                         }
                     }
                 }
                 else if(*m_pType == Oper_Modify) 
                 {
                     if (m_nRiskLevel == 1 && nLeveLCount > 1)
                     {       
                         map<int,double>::iterator it = g_mapRiskLevelInfo2.find(m_nRiskLevel+1);
                         if(it!=g_mapRiskLevelInfo2.end())
                         {
                             if(fRiskValue<=(it->second))
                             {
                                 MessageBox::Show("阀值的大小要按风险方案设置(倒序)，请参考相关说明文档");
                                 return;
                             }
                         }
                     }else if(m_nRiskLevel == nLeveLCount && nLeveLCount > 1)
                     {
                         map<int,double>::iterator it = g_mapRiskLevelInfo2.find(m_nRiskLevel-1);
                         if(it!=g_mapRiskLevelInfo2.end())
                         {
                             if(fRiskValue>=(it->second))
                             {
                                 MessageBox::Show("阀值的大小要按风险方案设置(倒序)，请参考相关说明文档");
                                 return;
                             }
                         }
                     }else if(nLeveLCount > 1)
                     {
                         map<int,double>::iterator it = g_mapRiskLevelInfo2.find(m_nRiskLevel-1);
                         if(it!=g_mapRiskLevelInfo2.end())
                         {
                             if(fRiskValue>=(it->second))
                             {
                                 MessageBox::Show("阀值的大小要按风险方案设置(倒序)，请参考相关说明文档");
                                 return;
                             }  
                         }

                         map<int,double>::iterator it2 = g_mapRiskLevelInfo2.find(m_nRiskLevel+1);
                         if(it2!=g_mapRiskLevelInfo2.end())
                         {
                             if(fRiskValue<=(it2->second))
                             {
                                 MessageBox::Show("阀值的大小要按风险方案设置(倒序)，请参考相关说明文档");
                                 return;
                             }
                         }

                     }
                 }
                 break;
             case RI_FundNetValue:
                 if(*m_pType == Oper_Add) 
                 {
                     if (!g_mapRiskLevelInfo2.empty())
                     {                    
                         map<int,double>::iterator it = g_mapRiskLevelInfo2.find(m_nRiskLevel-1);
                         if(it!=g_mapRiskLevelInfo2.end())
                         {
                             if(fRiskValue<=it->second)
                             {
                                 MessageBox::Show("阀值的大小要按风险方案设置(正序)，请参考相关说明文档");
                                 return;
                             }
                         }
                     }
                 }
                 else if(*m_pType == Oper_Modify) 
                 {
                     if (m_nRiskLevel == 1 && nLeveLCount > 1)
                     {       
                         map<int,double>::iterator it = g_mapRiskLevelInfo2.find(m_nRiskLevel+1);
                         if(it!=g_mapRiskLevelInfo2.end())
                         {
                             if(fRiskValue>=it->second)
                             {
                                 MessageBox::Show("阀值的大小要按风险方案设置(正序)，请参考相关说明文档");
                                 return;
                             }
                         }
                     }else if(m_nRiskLevel == nLeveLCount && nLeveLCount > 1)
                     {
                         map<int,double>::iterator it = g_mapRiskLevelInfo2.find(m_nRiskLevel-1);
                         if(it!=g_mapRiskLevelInfo2.end())
                         {
                             if(fRiskValue<=it->second)
                             {
                                 MessageBox::Show("阀值的大小要按风险方案设置(正序)，请参考相关说明文档");
                                 return;
                             }
                         }
                     }else if(nLeveLCount > 1)
                     {
                         map<int,double>::iterator it = g_mapRiskLevelInfo2.find(m_nRiskLevel-1);
                         if(it!=g_mapRiskLevelInfo2.end())
                         {
                             if(fRiskValue<=it->second)
                             {
                                 MessageBox::Show("阀值的大小要按风险方案设置(正序)，请参考相关说明文档");
                                 return;
                             }
                         }
                         map<int,double>::iterator it2 = g_mapRiskLevelInfo2.find(m_nRiskLevel+1);
                         if(it2!=g_mapRiskLevelInfo2.end())
                         {
                             if(fRiskValue>=it2->second)
                             {
                                 MessageBox::Show("阀值的大小要按风险方案设置(正序)，请参考相关说明文档");
                                 return;
                             }
                         }
                     }
                 }
                 break;
             }

             this->DialogResult = ::DialogResult::OK;
         }
private: System::Void button_Cancel_Click(System::Object^  sender, System::EventArgs^  e) {
             Close();
         }
private: System::Void textBox_LevelValue_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             if (e->KeyChar < '0' || e->KeyChar > '9')   
                 if (e->KeyChar != (char)8 && e->KeyChar != '.')    
                     e->Handled = true;

             //避免多次输入'.'
             if ((e->KeyChar == '.') && (textBox_LevelValue->Text->IndexOf('.') > -1))
                 e->Handled = true;

             //保留2为小数
             if(e->KeyChar != 8 && (textBox_LevelValue->Text->IndexOf('.') > -1))
             {
                 int nLen = textBox_LevelValue->Text->Length;
                 int nSignLen = textBox_LevelValue->Text->IndexOf('.');
                 if((nLen-nSignLen)==3)
                     e->Handled = true;
             }

         }
};
}

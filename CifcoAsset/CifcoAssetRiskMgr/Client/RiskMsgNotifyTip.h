#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Tools.h"
#include "RiskManageStruct.h"
#include "RiskMsgNotifyTip2.h"

namespace Client {

	/// <summary>
	/// Summary for RiskMsgNotifyTip
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
    public ref class RiskMsgNotifyTip : public System::Windows::Forms::Form
    {
    public:
        RiskMsgNotifyTip(void)
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //
            m_nMsgCount = 0;
            m_strContent = nullptr;
            m_pRiskMsgNotifyTip2 = CRiskMsgNotifyTip2::NewInstance();
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~RiskMsgNotifyTip()
        {
            if (components)
            {
                delete components;
            }

            if(m_pRiskMsgNotifyTip2 != NULL)
            {
                CRiskMsgNotifyTip2::DestroyInstance(m_pRiskMsgNotifyTip2);
            }
        }
    private: System::Windows::Forms::Button^  Previous_button;
    private: System::Windows::Forms::Button^  Next_button;
    private: System::Windows::Forms::Label^  Content_label;
    private: System::Windows::Forms::Button^  Ok_button;



    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::Container ^components;
        CRiskMsgNotifyTip2* m_pRiskMsgNotifyTip2;
        int m_nMsgCount;
    private: System::Windows::Forms::GroupBox^  groupBox1;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  Title_label;
             String^ m_strContent;
#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->Previous_button = (gcnew System::Windows::Forms::Button());
            this->Next_button = (gcnew System::Windows::Forms::Button());
            this->Content_label = (gcnew System::Windows::Forms::Label());
            this->Ok_button = (gcnew System::Windows::Forms::Button());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->Title_label = (gcnew System::Windows::Forms::Label());
            this->groupBox1->SuspendLayout();
            this->SuspendLayout();
            // 
            // Previous_button
            // 
            this->Previous_button->Location = System::Drawing::Point(53, 231);
            this->Previous_button->Name = L"Previous_button";
            this->Previous_button->Size = System::Drawing::Size(75, 23);
            this->Previous_button->TabIndex = 0;
            this->Previous_button->Text = L"上一条";
            this->Previous_button->UseVisualStyleBackColor = true;
            this->Previous_button->Visible = false;
            this->Previous_button->Click += gcnew System::EventHandler(this, &RiskMsgNotifyTip::Previous_button_Click);
            // 
            // Next_button
            // 
            this->Next_button->Location = System::Drawing::Point(150, 231);
            this->Next_button->Name = L"Next_button";
            this->Next_button->Size = System::Drawing::Size(75, 23);
            this->Next_button->TabIndex = 1;
            this->Next_button->Text = L"下一条";
            this->Next_button->UseVisualStyleBackColor = true;
            this->Next_button->Visible = false;
            this->Next_button->Click += gcnew System::EventHandler(this, &RiskMsgNotifyTip::Next_button_Click);
            // 
            // Content_label
            // 
            this->Content_label->Dock = System::Windows::Forms::DockStyle::Fill;
            this->Content_label->Location = System::Drawing::Point(3, 17);
            this->Content_label->Name = L"Content_label";
            this->Content_label->Size = System::Drawing::Size(351, 172);
            this->Content_label->TabIndex = 2;
            this->Content_label->Text = L"label";
            // 
            // Ok_button
            // 
            this->Ok_button->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Ok_button->Location = System::Drawing::Point(260, 231);
            this->Ok_button->Name = L"Ok_button";
            this->Ok_button->Size = System::Drawing::Size(75, 23);
            this->Ok_button->TabIndex = 3;
            this->Ok_button->Text = L"确定";
            this->Ok_button->UseVisualStyleBackColor = true;
            this->Ok_button->Click += gcnew System::EventHandler(this, &RiskMsgNotifyTip::Ok_button_Click);
            // 
            // groupBox1
            // 
            this->groupBox1->BackColor = System::Drawing::Color::Transparent;
            this->groupBox1->Controls->Add(this->Content_label);
            this->groupBox1->ForeColor = System::Drawing::Color::Black;
            this->groupBox1->Location = System::Drawing::Point(13, 30);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(357, 192);
            this->groupBox1->TabIndex = 4;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"内容：";
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(19, 10);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(41, 12);
            this->label1->TabIndex = 5;
            this->label1->Text = L"标题：";
            // 
            // Title_label
            // 
            this->Title_label->AutoSize = true;
            this->Title_label->Location = System::Drawing::Point(67, 9);
            this->Title_label->Name = L"Title_label";
            this->Title_label->Size = System::Drawing::Size(41, 12);
            this->Title_label->TabIndex = 6;
            this->Title_label->Text = L"label2";
            // 
            // RiskMsgNotifyTip
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(383, 265);
            this->ControlBox = false;
            this->Controls->Add(this->Title_label);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->Ok_button);
            this->Controls->Add(this->Next_button);
            this->Controls->Add(this->Previous_button);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"RiskMsgNotifyTip";
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            this->Text = L"风险消息通知提示";
            this->TopMost = true;
            this->groupBox1->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    public:System::Void UpdateContent(MessageInfo& oMessageInfo){              
               m_nMsgCount++;

               Title_label->Text = Tools::string2String(oMessageInfo.szTitle);
               Content_label->Text = Tools::string2String(oMessageInfo.szContent);
               m_pRiskMsgNotifyTip2->SetMessageIndex(m_nMsgCount);
               m_pRiskMsgNotifyTip2->SetMessageInfo(oMessageInfo);                

               if (1 < m_pRiskMsgNotifyTip2->GetMessageInfoCount())
               {
                   Previous_button->Visible = true;
                   Next_button->Visible = false;
               }
           }
    private: System::Void Previous_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 MessageInfo* pMessageInfo = m_pRiskMsgNotifyTip2->GetPreMessageInfo();
                 if (pMessageInfo != NULL)
                 {             
                     Title_label->Text = Tools::string2String(pMessageInfo->szTitle);
                     Content_label->Text = Tools::string2String(pMessageInfo->szContent);
                     if (1 < m_pRiskMsgNotifyTip2->GetMessageInfoCount() && m_nMsgCount > m_pRiskMsgNotifyTip2->GetMessageInfoCount())
                     {
                         Previous_button->Visible = true;
                         Next_button->Visible = true;
                     }else if(1 == m_pRiskMsgNotifyTip2->GetMessageInfoCount())
                     {
                        Previous_button->Visible = false;
                        Next_button->Visible = true;
                     }else if (m_nMsgCount == m_pRiskMsgNotifyTip2->GetMessageInfoCount())
                     {
                         Previous_button->Visible = true;
                         Next_button->Visible = false;
                     }
                 }
             }
    private: System::Void Next_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 MessageInfo* pMessageInfo = m_pRiskMsgNotifyTip2->GetNextmessageInfo();
                 if (pMessageInfo != NULL)
                 {
                     Title_label->Text = Tools::string2String(pMessageInfo->szTitle);
                     Content_label->Text = Tools::string2String(pMessageInfo->szContent);
                     if (1 < m_pRiskMsgNotifyTip2->GetMessageInfoCount() && m_nMsgCount > m_pRiskMsgNotifyTip2->GetMessageInfoCount())
                     {
                         Previous_button->Visible = true;
                         Next_button->Visible = true;
                     }else if(1 == m_pRiskMsgNotifyTip2->GetMessageInfoCount())
                     {
                         Previous_button->Visible = false;
                         Next_button->Visible = true;
                     }else if (m_nMsgCount == m_pRiskMsgNotifyTip2->GetMessageInfoCount())
                     {
                         Previous_button->Visible = true;
                         Next_button->Visible = false;
                     }
                 }
             }
    private: System::Void Ok_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 this->Hide();
             }
};
}

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


#include <map>
using namespace std;

#include "Tools.h"
#include "CDataInfo.h"

namespace CltServerMgr {

	/// <summary>
	/// Summary for FinanceProductForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FinanceProductForm : public System::Windows::Forms::Form
	{
	public:
        FinanceProductForm()
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //
            m_pType = new OperType;
            m_pCurFinancialProduct = new FinancialProduct;
            m_pDataInfo = CDataInfo::NewInstance();
        }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FinanceProductForm()
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

            if ( NULL != m_pCurFinancialProduct )
            {
                delete m_pCurFinancialProduct;
                m_pCurFinancialProduct = NULL;
            }
		}

    
    private: CDataInfo* m_pDataInfo;
    private: OperType* m_pType;
    private: FinancialProduct* m_pCurFinancialProduct;    

    private: System::Windows::Forms::Button^  close_button;
	private: System::Windows::Forms::Button^  confirm_button;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::TextBox^  Name_textBox;
    private: System::Windows::Forms::TextBox^  Desc_textBox;

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
        void SetCurFinancialProduct(const FinancialProduct& financialProduct)
        {
            *m_pCurFinancialProduct = financialProduct;
        }

        FinancialProduct* GetCurFinancialProduct()
        {
            return m_pCurFinancialProduct;
        }

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->Name_textBox = (gcnew System::Windows::Forms::TextBox());
            this->close_button = (gcnew System::Windows::Forms::Button());
            this->confirm_button = (gcnew System::Windows::Forms::Button());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->Desc_textBox = (gcnew System::Windows::Forms::TextBox());
            this->SuspendLayout();
            // 
            // Name_textBox
            // 
            this->Name_textBox->Location = System::Drawing::Point(100, 23);
            this->Name_textBox->MaxLength = 20;
            this->Name_textBox->Name = L"Name_textBox";
            this->Name_textBox->Size = System::Drawing::Size(163, 21);
            this->Name_textBox->TabIndex = 11;
            // 
            // close_button
            // 
            this->close_button->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->close_button->Location = System::Drawing::Point(160, 203);
            this->close_button->Name = L"close_button";
            this->close_button->Size = System::Drawing::Size(75, 23);
            this->close_button->TabIndex = 10;
            this->close_button->Text = L"关闭";
            this->close_button->UseVisualStyleBackColor = true;
            this->close_button->Click += gcnew System::EventHandler(this, &FinanceProductForm::close_button_Click);
            // 
            // confirm_button
            // 
            this->confirm_button->Location = System::Drawing::Point(58, 203);
            this->confirm_button->Name = L"confirm_button";
            this->confirm_button->Size = System::Drawing::Size(75, 23);
            this->confirm_button->TabIndex = 13;
            this->confirm_button->Text = L"确定";
            this->confirm_button->UseVisualStyleBackColor = true;
            this->confirm_button->Click += gcnew System::EventHandler(this, &FinanceProductForm::confirm_button_Click);
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(56, 59);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(29, 12);
            this->label2->TabIndex = 7;
            this->label2->Text = L"描述";
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(32, 28);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(53, 12);
            this->label1->TabIndex = 6;
            this->label1->Text = L"产品名称";
            // 
            // Desc_textBox
            // 
            this->Desc_textBox->Location = System::Drawing::Point(100, 59);
            this->Desc_textBox->MaxLength = 128;
            this->Desc_textBox->Multiline = true;
            this->Desc_textBox->Name = L"Desc_textBox";
            this->Desc_textBox->ScrollBars = System::Windows::Forms::ScrollBars::Horizontal;
            this->Desc_textBox->Size = System::Drawing::Size(163, 103);
            this->Desc_textBox->TabIndex = 12;
            // 
            // FinanceProductForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(292, 241);
            this->Controls->Add(this->Desc_textBox);
            this->Controls->Add(this->Name_textBox);
            this->Controls->Add(this->close_button);
            this->Controls->Add(this->confirm_button);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"FinanceProductForm";
            this->Text = L"理财产品";
            this->Load += gcnew System::EventHandler(this, &FinanceProductForm::FinanceProductForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
private: System::Void FinanceProductForm_Load(System::Object^  sender, System::EventArgs^  e){             
             this->CenterToParent();

             if ( *m_pType == Oper_Add )
             {
                 this->Text = "新增理财产品";
             }
             else
             {
                 this->Text = "修改理财产品";

                 Name_textBox->Text  = Tools::string2String(m_pCurFinancialProduct->szName);                 
                 Desc_textBox->Text  = Tools::string2String(m_pCurFinancialProduct->szDesc);
             }
         }
private: System::Void confirm_button_Click(System::Object^  sender, System::EventArgs^  e) {
             if(String::IsNullOrEmpty(Name_textBox->Text)) 
             {
                 MessageBox::Show("请输入产品名称!");  
                 return;
             }

             FinancialProduct oFP;
             if(*m_pType == Oper_Modify) oFP.nID = m_pCurFinancialProduct->nID;
             strcpy(oFP.szName,Tools::String2string(Name_textBox->Text).c_str());
             strcpy(oFP.szDesc,Tools::String2string(Desc_textBox->Text).c_str()); 

             if(*m_pType==Oper_Add)
             {
                 if(!m_pDataInfo->IsValidOfProductName(Tools::String2string(Name_textBox->Text)))    
                 {
                     MessageBox::Show("理财产品名称重名,请重新输入!");                 
                     return;
                 }

                 memcpy(m_pCurFinancialProduct,&oFP,sizeof(FinancialProduct));

                 this->DialogResult = ::DialogResult::OK;
                 this->Close();
             }
             
             if(*m_pType==Oper_Modify)
             {
                 if(*m_pCurFinancialProduct == oFP)
                     this->DialogResult = ::DialogResult::Cancel;
                 else
                 {
                     if(0==strcmp(m_pCurFinancialProduct->szName,oFP.szName))
                     {
                        this->DialogResult = ::DialogResult::OK;
                        memcpy(m_pCurFinancialProduct,&oFP,sizeof(FinancialProduct));
                        this->Close();
                     }
                     else
                     {
                         if(!m_pDataInfo->IsValidOfProductName(oFP.szName))
                         {                             
                             MessageBox::Show("理财产品名称重名,请重新输入!"); 
                         }
                         else
                         {
                             this->DialogResult = ::DialogResult::OK;
                             memcpy(m_pCurFinancialProduct,&oFP,sizeof(FinancialProduct));
                             this->Close();
                         }
                     }
                 }                 
             }
         }

private: System::Void close_button_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }



};
}

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
	/// Summary for ProductCheckedListBoxForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ProductCheckedListBoxForm : public AnywndComboBox::CAnywndComboBoxControlContainter
	{
	public:
        ProductCheckedListBoxForm(String^ strProduct)
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //
            m_pDataInfo = CDataInfo::NewInstance();
            LoadProduct(strProduct);
        }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ProductCheckedListBoxForm()
		{
			if (components)
			{
				delete components;
			}
		}

    private: CDataInfo* m_pDataInfo;

    private: System::Windows::Forms::ComboBox^  comboRole;
    private: System::Windows::Forms::CheckedListBox^  checkedListBox1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

    public:
        void ResetCombo(System::Windows::Forms::ComboBox^ comboRelate)
        {
            this->comboRole = comboRelate;
        };

        String^ GetCurUserRole()
        {
            return comboRole->Text;
        }

        void LoadProduct(String^ strProduct)
        {
            std::vector<int> vecRole;
            cli::array<System::String^>^ sArray = strProduct->Split(',');

            checkedListBox1->Items->Clear();

            std::map<int, FinancialProduct> mapFinancialProduct;
            m_pDataInfo->GetFinancialProduct(mapFinancialProduct);

            std::map<int, FinancialProduct>::iterator it = mapFinancialProduct.begin();
            for (int i=0; it != mapFinancialProduct.end(); it++,i++)            
            {
               String^ name = Tools::string2String(it->second.szName);
               checkedListBox1->Items->Add(Tools::string2String(it->second.szName));   

               for each (String^ item in sArray)  
               {
                   String^ sProduct = item->ToString()->Trim();
                   if ( 0==sProduct->CompareTo(name))
                        checkedListBox1->SetItemChecked(i,true);
               }
            }
        }

        String^ GetCurProduct()
        {
            return comboRole->Text;
        }

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->checkedListBox1 = (gcnew System::Windows::Forms::CheckedListBox());
            this->SuspendLayout();
            // 
            // checkedListBox1
            // 
            this->checkedListBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->checkedListBox1->CheckOnClick = true;
            this->checkedListBox1->FormattingEnabled = true;
            this->checkedListBox1->Location = System::Drawing::Point(0, 0);
            this->checkedListBox1->Name = L"checkedListBox1";
            this->checkedListBox1->Size = System::Drawing::Size(147, 256);
            this->checkedListBox1->TabIndex = 0;
            // 
            // ProductCheckedListBoxForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->Controls->Add(this->checkedListBox1);
            this->Name = L"ProductCheckedListBoxForm";
            this->Size = System::Drawing::Size(147, 248);
            this->Load += gcnew System::EventHandler(this, &ProductCheckedListBoxForm::ProductCheckedListBoxForm_Load);
            this->ResumeLayout(false);

        }
#pragma endregion
    private: System::Void ProductCheckedListBoxForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 checkedListBox1->Location = Point(0,0);
             };
    public: virtual Void ProcessData() override
            {
                std::string strText;
                int nCount = checkedListBox1->Items->Count;
                for(int i=0;i<nCount;i++) 
                {
                    if (checkedListBox1->GetItemChecked(i))
                    {
                        String^ strKey = checkedListBox1->GetItemText(checkedListBox1->Items[i]);
                        strText += Tools::String2string(strKey);
                        strText += ", ";
                    }                    
                }  

                int nLen = strText.length();
                if(nLen > 2)
                    strText[nLen-2] = '\0';

                comboRole->Text = Tools::string2String(strText.c_str());
            }
    };
}

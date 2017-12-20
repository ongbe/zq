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
	/// Summary for RoleCheckedListBoxForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class RoleCheckedListBoxForm : public AnywndComboBox::CAnywndComboBoxControlContainter
	{
	public:
		RoleCheckedListBoxForm()
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RoleCheckedListBoxForm()
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

        void SetRoleType(Int32 nType,Int32 nUserID)
        {
            checkedListBox1->Items->Clear();

            std::map<int,std::vector<RoleInfo>> mapUserType2Role;
            m_pDataInfo->GetUserType2RoleRelation(mapUserType2Role);

            std::map<int, std::vector<UserRole>> mapUserRole;
            m_pDataInfo->GetUserRoleRelation(mapUserRole);

            std::map<int,std::vector<RoleInfo>>::iterator it = mapUserType2Role.find(nType);
            if ( it != mapUserType2Role.end() )
            {
                std::vector<RoleInfo>::iterator itRole = it->second.begin();
                for (int i=0; itRole != it->second.end(); itRole++,i++)         
                {
                    checkedListBox1->Items->Add(Tools::string2String(itRole->szName)); 

                    if(!mapUserRole.empty())
                    {
                        std::map<int, std::vector<UserRole>>::iterator it_role = mapUserRole.find(nUserID);
                        if(it_role!=mapUserRole.end())
                        {
                            std::vector<UserRole>::iterator  it_rele = it_role->second.begin();
                            for(;it_rele!=it_role->second.end();it_rele++)
                                if(m_pDataInfo->GetRoleIDByName((string)(itRole->szName))== it_rele->nRoleID)
                                    checkedListBox1->SetItemChecked(i,true);
                        }
                    }
                }
            } 
        }

        String^ GetCurUserRole()
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
            this->checkedListBox1->Size = System::Drawing::Size(114, 160);
            this->checkedListBox1->TabIndex = 0;
            this->checkedListBox1->ItemCheck += gcnew System::Windows::Forms::ItemCheckEventHandler(this, &RoleCheckedListBoxForm::checkedListBox1_ItemCheck);
            // 
            // RoleCheckedListBoxForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->Controls->Add(this->checkedListBox1);
            this->Name = L"RoleCheckedListBoxForm";
            this->Size = System::Drawing::Size(114, 156);
            this->Load += gcnew System::EventHandler(this, &RoleCheckedListBoxForm::RoleCheckedListBoxForm_Load);
            this->ResumeLayout(false);

        }
#pragma endregion


    private: System::Void RoleCheckedListBoxForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 checkedListBox1->Location = Point(0,0);
             }

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
    private: System::Void checkedListBox1_ItemCheck(System::Object^  sender, System::Windows::Forms::ItemCheckEventArgs^  e) {
                 if (checkedListBox1->CheckedItems->Count>0)
                 {
                     for(int i=0;i<checkedListBox1->Items->Count;i++)
                     {
                         if(i!=e->Index)
                             checkedListBox1->SetItemCheckState(i,System::Windows::Forms::CheckState::Unchecked);
                     }
                 }
             }
};
}

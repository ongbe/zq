#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Tools.h"
#include "RiskData.h"
#include "CommonStruct.h"

namespace CltRisk {

	/// <summary>
	/// Summary for FormOrganizeCheckedTreeList
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormOrganizeCheckedTreeList : public AnywndComboBox::CAnywndComboBoxControlContainter
	{
	public:
		FormOrganizeCheckedTreeList(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            ResetCheckAllShow(false, false);
            LoadAssertMgmtOrgan();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormOrganizeCheckedTreeList()
		{
			if (components)
			{
				delete components;
			}
		}

    private: System::Windows::Forms::ComboBox^  comboRelate;
    private: System::Windows::Forms::CheckBox^  checkHasAll;
    private: System::Windows::Forms::TreeView^  treeMgmtOrgan;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

    public:
        String^ GetCurOrg()
        {
            return comboRelate->Text;
        }

        void ResetCheckAllShow(bool bShow, bool bValue) 
        {
            System::Drawing::Size size = this->Size;
            checkHasAll->Checked = bValue;
            if(bShow) 
            {
                checkHasAll->Show();
                treeMgmtOrgan->Location = Point(0, 28);
                size.Height -= 28;
                treeMgmtOrgan->Size = size;
            }
            else 
            {
                checkHasAll->Hide();
                treeMgmtOrgan->Location = Point(0, 0);
                treeMgmtOrgan->Size = size;
            }
        }

        bool CheckAllShown() 
        {
            return checkHasAll->Visible;
        }

        bool HasCheckAll()
        {
            return checkHasAll->Checked;
        }

        void ResetRelateCombo(System::Windows::Forms::ComboBox^ comboRelate) 
        {
            this->comboRelate = comboRelate;
        };

        void Clear()
        {
            comboRelate->Text = Tools::string2String("");
            if(treeMgmtOrgan->Nodes->Count>0)
                UncheckTreeNode(treeMgmtOrgan->Nodes[0]);
        }

        bool GetSelectID(std::set<int>& setMgmtOrganID, bool& bNoChecked)
        {
            if(treeMgmtOrgan->Nodes->Count==0)
                return false;

            bNoChecked = true;
            int nID = 0;
            for(int i=0; i<treeMgmtOrgan->Nodes->Count; i++) 
            {
                TreeNode^ node = treeMgmtOrgan->Nodes[i];
                if(node->Checked) {
                    String^ strKey = treeMgmtOrgan->Nodes[0]->Name;
                    nID = atoi(Tools::String2string(strKey).c_str());
                    setMgmtOrganID.insert(nID);
                    bNoChecked = false;
                }

                GetNodeID(node, setMgmtOrganID, bNoChecked);
            }

            if(bNoChecked) 
            {
                TreeNode^ node = treeMgmtOrgan->Nodes[0];
                String^ strKey = treeMgmtOrgan->Nodes[0]->Name;
                nID = atoi(Tools::String2string(strKey).c_str());
                setMgmtOrganID.insert(nID);
            }

            return setMgmtOrganID.size()>0;
        }

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->checkHasAll = (gcnew System::Windows::Forms::CheckBox());
            this->treeMgmtOrgan = (gcnew System::Windows::Forms::TreeView());
            this->SuspendLayout();
            // 
            // checkHasAll
            // 
            this->checkHasAll->AutoSize = true;
            this->checkHasAll->Location = System::Drawing::Point(0, 3);
            this->checkHasAll->Name = L"checkHasAll";
            this->checkHasAll->Size = System::Drawing::Size(120, 16);
            this->checkHasAll->TabIndex = 0;
            this->checkHasAll->Text = L"统计包含下级部门";
            this->checkHasAll->UseVisualStyleBackColor = true;
            // 
            // treeMgmtOrgan
            // 
            this->treeMgmtOrgan->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->treeMgmtOrgan->FullRowSelect = true;
            this->treeMgmtOrgan->Location = System::Drawing::Point(0, 25);
            this->treeMgmtOrgan->Name = L"treeMgmtOrgan";
            this->treeMgmtOrgan->Size = System::Drawing::Size(138, 218);
            this->treeMgmtOrgan->TabIndex = 1;
            this->treeMgmtOrgan->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &FormOrganizeCheckedTreeList::treeMgmtOrgan_AfterSelect);
            // 
            // FormOrganizeCheckedTreeList
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->Controls->Add(this->treeMgmtOrgan);
            this->Controls->Add(this->checkHasAll);
            this->Name = L"FormOrganizeCheckedTreeList";
            this->Size = System::Drawing::Size(141, 243);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

        private: void LoasSubAssertMgmtOrgan(TreeNode^ nodeMain,std::map<int, std::map<int, Organization>>& mapMain,std::map<int, Organization>& mapSub,vector<int> vecOrgID) 
                 {
                     std::map<int, std::map<int, Organization>>::iterator it;
                     std::map<int, Organization>::iterator itSub;

                     TreeNode^ subNode;

                     itSub = mapSub.begin();
                     for(; itSub != mapSub.end(); itSub++) 
                     {
                         for (vector<int>::iterator it1=vecOrgID.begin();it1!=vecOrgID.end();it1++)
                         {
                             int^ nID = itSub->first;
                             String^ strText = nID->ToString();
                             String^ strValue = Tools::string2String(itSub->second.szName);
                             subNode = nodeMain->Nodes->Add(strText, strValue);
                         }

                         it = mapMain.find((int)(itSub->first));
                         if(it != mapMain.end())
                             LoasSubAssertMgmtOrgan(subNode, mapMain, it->second,vecOrgID);
                     }
                 }

private: void LoadAssertMgmtOrgan() 
         {
             sLoginRsp loginRsp;
             memset(&loginRsp, 0, sizeof(loginRsp));
             CRiskData::GetInstance()->GetLogonInfo(loginRsp);

             vector<int> vecOrgID;
             CRiskData::GetInstance()->GetOrgIDByUserID(loginRsp.mnUserID,USER_ORG_RELATION_ATTACH,vecOrgID);
             if(vecOrgID.empty())
                 return;

             int nOrgID = vecOrgID[0];

             Organization assertOrg;
             memset(&assertOrg, 0, sizeof(assertOrg));
             if(!CRiskData::GetInstance()->GetOrganizationByID(nOrgID, assertOrg))
                 return;

             TreeNode^ node;

             int^ nID = assertOrg.nAssetMgmtOrgID;
             String^ strText = nID->ToString();
             String^ strValue = Tools::string2String(assertOrg.szName);
             treeMgmtOrgan->Nodes->Clear();
             node = treeMgmtOrgan->Nodes->Add(strText, strValue);

             std::map<int, std::map<int, Organization>> mapAssetMgmtOrgTree;
             CRiskData::GetInstance()->GetOrganization(mapAssetMgmtOrgTree);
             std::map<int, std::map<int, Organization>>::iterator it = mapAssetMgmtOrgTree.find(nOrgID);
             if(it == mapAssetMgmtOrgTree.end())
                 return;

             std::map<int, Organization>& mapSub = it->second;
             if(mapSub.begin()==mapSub.end())
                 return;

             LoasSubAssertMgmtOrgan(node, mapAssetMgmtOrgTree, mapSub,vecOrgID);

             node->Expand();
         }

private: void GetNodeID(TreeNode^ node, std::set<int>& setMgmtOrganID, bool& bNoChecked) 
         {
             for(int i=0; i<node->Nodes->Count; i++) 
             {
                 if(node->Nodes[i]->Checked) 
                 {
                     String^ strKey = node->Nodes[i]->Name;
                     int nID = atoi(Tools::String2string(strKey).c_str());
                     setMgmtOrganID.insert(nID);
                     bNoChecked = false;
                 }

                 GetNodeID(node->Nodes[i], setMgmtOrganID, bNoChecked);
             }
         }

private: bool HasTreeChecked(TreeNode^ node, bool& bNoChecked)
         {
             for(int i=0; i<node->Nodes->Count; i++) 
             {
                 if(node->Nodes[i]->Checked)
                 {
                     bNoChecked = false;
                 }

                 HasTreeChecked(node->Nodes[i], bNoChecked);
             }
             return bNoChecked;
         }

private: void GetTreeItemText(TreeNode^ node, std::string& strText, bool bNoChecked) 
         {
             for(int i=0; i<node->Nodes->Count; i++) 
             {
                 if(bNoChecked || (!bNoChecked && node->Nodes[i]->Checked))
                 {
                     String^ strKey = node->Nodes[i]->Text;
                     strText += Tools::String2string(strKey);
                     //strText += ", ";
                     break;
                 }

                 GetTreeItemText(node->Nodes[i], strText, bNoChecked);
             }
         }

private: void UncheckTreeNode(TreeNode^ node)
         {
             node->Checked = false;
             for(int i=0; i<node->Nodes->Count; i++) 
             {
                 UncheckTreeNode(node->Nodes[i]);
             }
         }
private: System::Void treeMgmtOrgan_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
             if(treeMgmtOrgan->Nodes->Count==0)
                 return;

             UncheckTreeNode(treeMgmtOrgan->Nodes[0]);

             e->Node->Checked = true;
             e->Node->Expand();
         }
public: virtual Void ProcessData() override
        {
            std::string strText;
            bool bNoChecked = true;
            if(treeMgmtOrgan->Nodes->Count>0) 
            {
                bNoChecked = treeMgmtOrgan->Nodes[0]->Checked;

                if(bNoChecked)
                {
                    String^ strKey = treeMgmtOrgan->Nodes[0]->Text;
                    strText += Tools::String2string(strKey);
                }else
                {
                    GetTreeItemText(treeMgmtOrgan->Nodes[0], strText, bNoChecked);
                }

                comboRelate->Text = Tools::string2String(strText.c_str());
            }
            else
                comboRelate->Text = Tools::string2String("");
        }
};
}

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "CDataInfo.h"
#include "Tools.h"
#include "CCommonFunc.h"
#include "OrganizeCheckedTreeListForm.h"

namespace CltServerMgr {

	/// <summary>
	/// Summary for RiskAddOrganizeForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class RiskAddOrganizeForm : public System::Windows::Forms::Form
	{
	public:
		RiskAddOrganizeForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_strCurOrg = nullptr;
            m_pCurUserInfo = new UserInfo;
            m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RiskAddOrganizeForm()
		{
			if (components)
			{
				delete components;
			}
		}

    private: Point^  m_point;
    private: String^ m_strCurOrg;
    private: CDataInfo* m_pDataInfo;
    private: UserInfo* m_pCurUserInfo;
    private: OrganizeCheckedTreeListForm^ m_OrganizeCheckedTreeListForm;

    public:

        void SetCurUserInfo(const UserInfo& userInfo)
        {
            *m_pCurUserInfo = userInfo;
        }

        void SetCurOrg(String^ strOrg)
        {
            m_strCurOrg = strOrg;
        }

        String^ GetCurOrg()
        {
            return m_strCurOrg;
        }

        bool GetListenOrg(std::vector<int>& vecOrg)
        {
            vecOrg.clear();
            GetSelectedTreeViewList(Organize_treeView->Nodes[0],vecOrg);

            return vecOrg.size()>0;
        }

    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  Account_label;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::GroupBox^  groupBox1;
    private: System::Windows::Forms::TreeView^  Organize_treeView;
    private: System::Windows::Forms::Button^  OK_button;
    private: System::Windows::Forms::Button^  Cancel_button;
    private: AnywndComboBox::AnywndComboBoxControl^  anywndComboBoxControl1;
	
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
            this->Account_label = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->Organize_treeView = (gcnew System::Windows::Forms::TreeView());
            this->OK_button = (gcnew System::Windows::Forms::Button());
            this->Cancel_button = (gcnew System::Windows::Forms::Button());
            this->anywndComboBoxControl1 = (gcnew AnywndComboBox::AnywndComboBoxControl());
            this->groupBox1->SuspendLayout();
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(35, 22);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(29, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"账号";
            // 
            // Account_label
            // 
            this->Account_label->AutoSize = true;
            this->Account_label->Location = System::Drawing::Point(98, 22);
            this->Account_label->Name = L"Account_label";
            this->Account_label->Size = System::Drawing::Size(0, 12);
            this->Account_label->TabIndex = 1;
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(35, 53);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(53, 12);
            this->label3->TabIndex = 2;
            this->label3->Text = L"所属部门";
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->Organize_treeView);
            this->groupBox1->Location = System::Drawing::Point(35, 83);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(200, 258);
            this->groupBox1->TabIndex = 4;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"监控部门设置";
            // 
            // Organize_treeView
            // 
            this->Organize_treeView->CheckBoxes = true;
            this->Organize_treeView->Dock = System::Windows::Forms::DockStyle::Fill;
            this->Organize_treeView->Location = System::Drawing::Point(3, 17);
            this->Organize_treeView->Name = L"Organize_treeView";
            this->Organize_treeView->Size = System::Drawing::Size(194, 238);
            this->Organize_treeView->TabIndex = 0;
            this->Organize_treeView->DoubleClick += gcnew System::EventHandler(this, &RiskAddOrganizeForm::Organize_treeView_DoubleClick);
                        this->Organize_treeView->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &RiskAddOrganizeForm::Organize_treeView_MouseDown);
            // 
            // OK_button
            // 
            this->OK_button->Location = System::Drawing::Point(43, 348);
            this->OK_button->Name = L"OK_button";
            this->OK_button->Size = System::Drawing::Size(75, 23);
            this->OK_button->TabIndex = 5;
            this->OK_button->Text = L"确定";
            this->OK_button->UseVisualStyleBackColor = true;
            this->OK_button->Click += gcnew System::EventHandler(this, &RiskAddOrganizeForm::OK_button_Click);
            // 
            // Cancel_button
            // 
            this->Cancel_button->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->Cancel_button->Location = System::Drawing::Point(151, 347);
            this->Cancel_button->Name = L"Cancel_button";
            this->Cancel_button->Size = System::Drawing::Size(75, 23);
            this->Cancel_button->TabIndex = 6;
            this->Cancel_button->Text = L"取消";
            this->Cancel_button->UseVisualStyleBackColor = true;
            this->Cancel_button->Click += gcnew System::EventHandler(this, &RiskAddOrganizeForm::Cancel_button_Click);
            // 
            // anywndComboBoxControl1
            // 
            this->anywndComboBoxControl1->BackColor = System::Drawing::Color::White;
            this->anywndComboBoxControl1->ChildControl = nullptr;
            this->anywndComboBoxControl1->DropDownHeight = 1;
            this->anywndComboBoxControl1->DropDownWidth = 1;
            this->anywndComboBoxControl1->FormattingEnabled = true;
            this->anywndComboBoxControl1->IntegralHeight = false;
            this->anywndComboBoxControl1->Location = System::Drawing::Point(93, 50);
            this->anywndComboBoxControl1->MaxDropDownItems = 1;
            this->anywndComboBoxControl1->Name = L"anywndComboBoxControl1";
            this->anywndComboBoxControl1->Size = System::Drawing::Size(142, 20);
            this->anywndComboBoxControl1->TabIndex = 7;
            this->anywndComboBoxControl1->SelectedIndexChanged += gcnew System::EventHandler(this, &RiskAddOrganizeForm::anywndComboBoxControl1_SelectedIndexChanged);
            // 
            // RiskAddOrganizeForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(284, 383);
            this->Controls->Add(this->anywndComboBoxControl1);
            this->Controls->Add(this->Cancel_button);
            this->Controls->Add(this->OK_button);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->Account_label);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"RiskAddOrganizeForm";
            this->Text = L"风控管理设置";
            this->Load += gcnew System::EventHandler(this, &RiskAddOrganizeForm::RiskAddOrganizeForm_Load);
            this->groupBox1->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
private: System::Void RiskAddOrganizeForm_Load(System::Object^  sender, System::EventArgs^  e) {
             this->CenterToScreen();

             m_OrganizeCheckedTreeListForm = gcnew OrganizeCheckedTreeListForm();             
             m_OrganizeCheckedTreeListForm->ResetRelateCombo(anywndComboBoxControl1);
             anywndComboBoxControl1->ChildControl = m_OrganizeCheckedTreeListForm; 

             ResetComboBoxTextReadOnly(anywndComboBoxControl1);

             Account_label->Text = Tools::string2String(m_pCurUserInfo->szAccount);
             anywndComboBoxControl1->Text = m_strCurOrg;    

             int nOrgID = m_pDataInfo->GetOrganizationIDByName(Tools::String2string(m_strCurOrg));  
             LoadAssertMgmtOrgan(nOrgID);
         }
private: System::Void anywndComboBoxControl1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             int nOrgID = m_pDataInfo->GetOrganizationIDByName(Tools::String2string(anywndComboBoxControl1->Text));
             LoadAssertMgmtOrgan(nOrgID);
         }
private: System::Void OK_button_Click(System::Object^  sender, System::EventArgs^  e) {  
             m_strCurOrg = anywndComboBoxControl1->Text;
             if(String::IsNullOrEmpty(m_strCurOrg))
             {
                 MessageBox::Show("所属部门不能为空!");
                 return;
             }
             else
             {
                 this->DialogResult = ::DialogResult::OK;
                 this->Close();
             }

         }
private: System::Void Cancel_button_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
private: void LoasSubAssertMgmtOrgan(TreeNode^ nodeMain,std::map<int, std::map<int, Organization>>& mapMain,std::map<int, Organization>& mapSub,std::vector<UserAndOrgRelation> vecUserAndOrgRelation) 
         {
             std::map<int, std::map<int, Organization>>::iterator it;
             std::map<int, Organization>::iterator itSub;

             TreeNode^ subNode;

             itSub = mapSub.begin();
             for(; itSub != mapSub.end(); itSub++) 
             {
                 int^ nID = itSub->first;
                 String^ strText = nID->ToString();
                 String^ strValue = Tools::string2String(itSub->second.szName);
                 subNode = nodeMain->Nodes->Add(strText, strValue);
                 subNode->Tag = itSub->first;

                 std::vector<UserAndOrgRelation>::iterator it_relate = vecUserAndOrgRelation.begin();
                 for (;it_relate!=vecUserAndOrgRelation.end();it_relate++)
                 {
                     if (it_relate->nRelationType)//监管
                     {
                         if(it_relate->nUserID == m_pCurUserInfo->nUserID && it_relate->nOrgID == (int)subNode->Tag)
                         {
                             subNode->Checked = true;
                             break;
                         }
                     }
                 }

                 it = mapMain.find(itSub->first);
                 if(it != mapMain.end())
                     LoasSubAssertMgmtOrgan(subNode, mapMain, it->second, vecUserAndOrgRelation);
             }
         }
private: void LoadAssertMgmtOrgan(int nOrgID) 
         {
             Organization assertOrg;
             memset(&assertOrg, 0, sizeof(assertOrg));
             if(!m_pDataInfo->GetOrganizationByID(nOrgID, assertOrg))
                 return;


             TreeNode^ node;

             int^ nID = assertOrg.nAssetMgmtOrgID;
             String^ strText = nID->ToString();
             String^ strValue = Tools::string2String(assertOrg.szName);
             Organize_treeView->Nodes->Clear();
             node = Organize_treeView->Nodes->Add(strText, strValue);
             node->Tag = assertOrg.nAssetMgmtOrgID;

             std::vector<UserAndOrgRelation> vecUserAndOrgRelation;
             m_pDataInfo->GetUserAndOrgRelation(vecUserAndOrgRelation);
             std::vector<UserAndOrgRelation>::iterator it_relate = vecUserAndOrgRelation.begin();
             for (;it_relate!=vecUserAndOrgRelation.end();it_relate++)
             {
                if (it_relate->nRelationType)//监管
                {
                      if(it_relate->nUserID == m_pCurUserInfo->nUserID && it_relate->nOrgID == (int)node->Tag)
                      {
                          node->Checked = true;
                          break;
                      }
                }
             }

             std::map<int, std::map<int, Organization>> mapAssetMgmtOrgTree;
             m_pDataInfo->GetOrganization(mapAssetMgmtOrgTree);
             std::map<int, std::map<int, Organization>>::iterator it = mapAssetMgmtOrgTree.find(nOrgID);
             if(it == mapAssetMgmtOrgTree.end())
                 return;

             std::map<int, Organization>& mapSub = it->second;
             if(mapSub.begin()==mapSub.end())
                 return;

             LoasSubAssertMgmtOrgan(node, mapAssetMgmtOrgTree, mapSub,vecUserAndOrgRelation);

             node->Expand();
         }
private: void GetSelectedTreeViewList(TreeNode^ nodes, std::vector<int>& vecOrg)
         {
             //防止节点没有子节点被忽略
             if (nodes->Checked)
             {
                 vecOrg.push_back(atoi(Tools::String2string(nodes->Name).c_str()));                 
             }
             //有子节点
             for each (TreeNode^ newNode in nodes->Nodes)
             {
                 if (newNode->Nodes->Count > 0)
                 {
                     GetSelectedTreeViewList(newNode,vecOrg);
                 }
                 //判断孩子节点
                 else
                 {
                     if (newNode->Checked)
                     {
                          vecOrg.push_back(atoi(Tools::String2string(newNode->Name).c_str()));
                     }
                 }
             }
         }
private: System::Void Organize_treeView_DoubleClick(System::Object^  sender, System::EventArgs^  e) {
             TreeNode^ node = Organize_treeView->GetNodeAt(m_point->X,m_point->Y); 
             node->Checked = !(node->Checked);
         }
private: System::Void Organize_treeView_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
             m_point = Point(e->X,e->Y);   
         }
};
}

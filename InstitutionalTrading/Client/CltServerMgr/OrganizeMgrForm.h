#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "TcpLayer.h"
#include "CommonDef.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "CDataInfo.h"
#include "Tools.h"
#include "OrganizeForm.h"
#include "CommonUserOpr.h"


namespace CltServerMgr {

    /// <summary>
    /// Summary for OrganizeMgrForm
    ///
    /// WARNING: If you change the name of this class, you will need to change the
    ///          'Resource File Name' property for the managed resource compiler tool
    ///          associated with all .resx files this class depends on.  Otherwise,
    ///          the designers will not be able to interact properly with localized
    ///          resources associated with this form.
    /// </summary>
    public ref class OrganizeMgrForm : public System::Windows::Forms::Form
    {
    public:
        OrganizeMgrForm(void)
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //
            m_pLogin = new sLoginRsp;
            m_strSelectedNodeName = nullptr;
            m_pDataInfo = CDataInfo::NewInstance();
            m_pCurOrganize = new Organization;
            m_strUpperOrganize = "总部";
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~OrganizeMgrForm()
        {
            if (components)
            {
                delete components;
            }
            if (m_pLogin)
            {
                delete m_pLogin;
                m_pLogin = NULL;
            }
        }

    private: String^ m_strSelectedNodeName;
    private: String^ m_strUpperOrganize;
    private: CDataInfo* m_pDataInfo;
    private: sLoginRsp* m_pLogin;
    private: Organization* m_pCurOrganize;

    private: System::Windows::Forms::Button^  Add_button;
    private: System::Windows::Forms::Button^  Modify_button;
    private: System::Windows::Forms::Button^  Delete_button;
    private: System::Windows::Forms::Button^  LeftAll_button;
    private: System::Windows::Forms::Button^  Left_button;
    private: System::Windows::Forms::Button^  Right_button;
    private: System::Windows::Forms::Button^  RightAll_button;
    private: System::Windows::Forms::Button^  Save_button;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::ListView^  InGroup_listView;
    private: System::Windows::Forms::ListView^  OutGroup_listView;
    private: System::Windows::Forms::TreeView^  Organize_treeView;
    private: System::Windows::Forms::ColumnHeader^  columnHeader1;
    private: System::Windows::Forms::ColumnHeader^  columnHeader2;
    private: System::Windows::Forms::ColumnHeader^  columnHeader3;
    private: System::Windows::Forms::ColumnHeader^  columnHeader4;


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
            this->Add_button = (gcnew System::Windows::Forms::Button());
            this->Modify_button = (gcnew System::Windows::Forms::Button());
            this->Delete_button = (gcnew System::Windows::Forms::Button());
            this->LeftAll_button = (gcnew System::Windows::Forms::Button());
            this->Left_button = (gcnew System::Windows::Forms::Button());
            this->Right_button = (gcnew System::Windows::Forms::Button());
            this->RightAll_button = (gcnew System::Windows::Forms::Button());
            this->Save_button = (gcnew System::Windows::Forms::Button());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->InGroup_listView = (gcnew System::Windows::Forms::ListView());
            this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
            this->OutGroup_listView = (gcnew System::Windows::Forms::ListView());
            this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
            this->Organize_treeView = (gcnew System::Windows::Forms::TreeView());
            this->SuspendLayout();
            // 
            // Add_button
            // 
            this->Add_button->Location = System::Drawing::Point(12, 262);
            this->Add_button->Name = L"Add_button";
            this->Add_button->Size = System::Drawing::Size(60, 23);
            this->Add_button->TabIndex = 0;
            this->Add_button->Text = L"新增";
            this->Add_button->UseVisualStyleBackColor = true;
            this->Add_button->Click += gcnew System::EventHandler(this, &OrganizeMgrForm::Add_button_Click);
            // 
            // Modify_button
            // 
            this->Modify_button->Location = System::Drawing::Point(78, 262);
            this->Modify_button->Name = L"Modify_button";
            this->Modify_button->Size = System::Drawing::Size(60, 23);
            this->Modify_button->TabIndex = 1;
            this->Modify_button->Text = L"修改";
            this->Modify_button->UseVisualStyleBackColor = true;
            this->Modify_button->Click += gcnew System::EventHandler(this, &OrganizeMgrForm::Modify_button_Click);
            // 
            // Delete_button
            // 
            this->Delete_button->Location = System::Drawing::Point(144, 262);
            this->Delete_button->Name = L"Delete_button";
            this->Delete_button->Size = System::Drawing::Size(60, 23);
            this->Delete_button->TabIndex = 2;
            this->Delete_button->Text = L"删除";
            this->Delete_button->UseVisualStyleBackColor = true;
            this->Delete_button->Click += gcnew System::EventHandler(this, &OrganizeMgrForm::Delete_button_Click);
            // 
            // LeftAll_button
            // 
            this->LeftAll_button->Location = System::Drawing::Point(378, 66);
            this->LeftAll_button->Name = L"LeftAll_button";
            this->LeftAll_button->Size = System::Drawing::Size(60, 23);
            this->LeftAll_button->TabIndex = 3;
            this->LeftAll_button->Text = L"<<";
            this->LeftAll_button->UseVisualStyleBackColor = true;
            this->LeftAll_button->Click += gcnew System::EventHandler(this, &OrganizeMgrForm::LeftAll_button_Click);
            // 
            // Left_button
            // 
            this->Left_button->Location = System::Drawing::Point(378, 106);
            this->Left_button->Name = L"Left_button";
            this->Left_button->Size = System::Drawing::Size(60, 23);
            this->Left_button->TabIndex = 4;
            this->Left_button->Text = L"<";
            this->Left_button->UseVisualStyleBackColor = true;
            this->Left_button->Click += gcnew System::EventHandler(this, &OrganizeMgrForm::Left_button_Click);
            // 
            // Right_button
            // 
            this->Right_button->Location = System::Drawing::Point(378, 146);
            this->Right_button->Name = L"Right_button";
            this->Right_button->Size = System::Drawing::Size(60, 23);
            this->Right_button->TabIndex = 5;
            this->Right_button->Text = L">";
            this->Right_button->UseVisualStyleBackColor = true;
            this->Right_button->Click += gcnew System::EventHandler(this, &OrganizeMgrForm::Right_button_Click);
            // 
            // RightAll_button
            // 
            this->RightAll_button->Location = System::Drawing::Point(378, 186);
            this->RightAll_button->Name = L"RightAll_button";
            this->RightAll_button->Size = System::Drawing::Size(60, 23);
            this->RightAll_button->TabIndex = 6;
            this->RightAll_button->Text = L">>";
            this->RightAll_button->UseVisualStyleBackColor = true;
            this->RightAll_button->Click += gcnew System::EventHandler(this, &OrganizeMgrForm::RightAll_button_Click);
            // 
            // Save_button
            // 
            this->Save_button->Location = System::Drawing::Point(372, 262);
            this->Save_button->Name = L"Save_button";
            this->Save_button->Size = System::Drawing::Size(75, 23);
            this->Save_button->TabIndex = 7;
            this->Save_button->Text = L"保存";
            this->Save_button->UseVisualStyleBackColor = true;
            this->Save_button->Click += gcnew System::EventHandler(this, &OrganizeMgrForm::Save_button_Click);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(228, 12);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(101, 12);
            this->label1->TabIndex = 8;
            this->label1->Text = L"部门内交易员列表";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(449, 12);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(101, 12);
            this->label2->TabIndex = 9;
            this->label2->Text = L"总部内交易员列表";
            // 
            // InGroup_listView
            // 
            this->InGroup_listView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->columnHeader1, 
                this->columnHeader2});
            this->InGroup_listView->FullRowSelect = true;
            this->InGroup_listView->GridLines = true;
            this->InGroup_listView->HideSelection = false;
            this->InGroup_listView->Location = System::Drawing::Point(230, 27);
            this->InGroup_listView->Name = L"InGroup_listView";
            this->InGroup_listView->Size = System::Drawing::Size(139, 226);
            this->InGroup_listView->TabIndex = 10;
            this->InGroup_listView->UseCompatibleStateImageBehavior = false;
            this->InGroup_listView->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader1
            // 
            this->columnHeader1->Text = L"账户号";
            // 
            // columnHeader2
            // 
            this->columnHeader2->Text = L"用户名";
            // 
            // OutGroup_listView
            // 
            this->OutGroup_listView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->columnHeader3, 
                this->columnHeader4});
            this->OutGroup_listView->FullRowSelect = true;
            this->OutGroup_listView->GridLines = true;
            this->OutGroup_listView->HideSelection = false;
            this->OutGroup_listView->Location = System::Drawing::Point(451, 27);
            this->OutGroup_listView->Name = L"OutGroup_listView";
            this->OutGroup_listView->Size = System::Drawing::Size(139, 226);
            this->OutGroup_listView->TabIndex = 11;
            this->OutGroup_listView->UseCompatibleStateImageBehavior = false;
            this->OutGroup_listView->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader3
            // 
            this->columnHeader3->Text = L"账户号";
            // 
            // columnHeader4
            // 
            this->columnHeader4->Text = L"用户名";
            // 
            // Organize_treeView
            // 
            this->Organize_treeView->HideSelection = false;
            this->Organize_treeView->Location = System::Drawing::Point(12, 12);
            this->Organize_treeView->Name = L"Organize_treeView";
            this->Organize_treeView->Size = System::Drawing::Size(192, 241);
            this->Organize_treeView->TabIndex = 12;
            this->Organize_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &OrganizeMgrForm::Organize_treeView_AfterSelect);
            this->Organize_treeView->BeforeSelect += gcnew System::Windows::Forms::TreeViewCancelEventHandler(this, &OrganizeMgrForm::Organize_treeView_BeforeSelect);
            // 
            // OrganizeMgrForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(602, 298);
            this->Controls->Add(this->Organize_treeView);
            this->Controls->Add(this->OutGroup_listView);
            this->Controls->Add(this->InGroup_listView);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->Save_button);
            this->Controls->Add(this->RightAll_button);
            this->Controls->Add(this->Right_button);
            this->Controls->Add(this->Left_button);
            this->Controls->Add(this->LeftAll_button);
            this->Controls->Add(this->Delete_button);
            this->Controls->Add(this->Modify_button);
            this->Controls->Add(this->Add_button);
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"OrganizeMgrForm";
            this->Text = L"部门结构管理";
            this->Load += gcnew System::EventHandler(this, &OrganizeMgrForm::OrganizeMgrForm_Load);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &OrganizeMgrForm::OrganizeMgrForm_FormClosed);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private: void LoasSubAssertMgmtOrgan(TreeNode^ nodeMain,std::map<int, std::map<int, Organization>>& mapMain,std::map<int, Organization>& mapSub) 
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

                     it = mapMain.find(itSub->first);
                     if(it != mapMain.end())
                         LoasSubAssertMgmtOrgan(subNode, mapMain, it->second);
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

                 std::map<int, std::map<int, Organization>> mapAssetMgmtOrgTree;
                 m_pDataInfo->GetOrganization(mapAssetMgmtOrgTree);
                 std::map<int, std::map<int, Organization>>::iterator it = mapAssetMgmtOrgTree.find(nOrgID);
                 if(it == mapAssetMgmtOrgTree.end())
                     return;

                 std::map<int, Organization>& mapSub = it->second;
                 if(mapSub.begin()==mapSub.end())
                     return;

                 LoasSubAssertMgmtOrgan(node, mapAssetMgmtOrgTree, mapSub);

                 node->Expand();

                 //              Organize_treeView->Nodes[0]->ForeColor = Color::Gray; 
             }
    private: void AddOrganize(const Organization* pOrganizition)
             {             
                 if(Organize_treeView->Nodes->Count == 0)
                 {
                     TreeNode^ tn = gcnew TreeNode();
                     tn->Text = Tools::string2String(pOrganizition->szName);
                     Organize_treeView->Nodes->Add(tn);
                 }else
                 {
                     std::string strUpperOrgName = m_pDataInfo->GetOrganizationNameByID(pOrganizition->nUpperLevelOrgID);
                     AddNode(Organize_treeView->Nodes[0],strUpperOrgName,pOrganizition);
                 }

             }
    private: void AddNode(TreeNode^ nodes, string& szUpperNodeName,const Organization* pOrganizition)
             {
                 //防止节点没有子节点被忽略
                 if (nodes->Text == Tools::string2String(szUpperNodeName.c_str()))
                 {
                     TreeNode^ tn = gcnew TreeNode();
                     tn->Text = Tools::string2String(pOrganizition->szName);
                     tn->Tag = pOrganizition->nAssetMgmtOrgID;
                     nodes->Nodes->Add(tn);  
                     tn->Parent->Expand();
                     return;
                 }

                 //有子节点
                 for each (TreeNode^ newNode in nodes->Nodes)
                 {
                     if (newNode->Nodes->Count > 0)
                     {
                         AddNode(newNode,szUpperNodeName,pOrganizition);
                     }
                     //判断孩子节点
                     else
                     {
                         if (newNode->Text == Tools::string2String(szUpperNodeName.c_str()))
                         {
                             TreeNode^ tn = gcnew TreeNode();
                             tn->Text = Tools::string2String(pOrganizition->szName);
                             tn->Tag = pOrganizition->nAssetMgmtOrgID;
                             newNode->Nodes->Add(tn);
                             tn->Parent->Expand();
                             return;
                         }
                     }
                 }
             }

    private: void ModifyOrganize(String^ strSelectedNodeName, const Organization* pOrganizition)
             {
                 ModifyNode(Organize_treeView->SelectedNode, strSelectedNodeName, pOrganizition);
             }
    private: void ModifyNode(TreeNode^ nodes, String^ strSelectedNodeName, const Organization* pOrganizition)
             {
                 //防止节点没有子节点被忽略
                 if (nodes->Text == strSelectedNodeName)
                 {
                     nodes->Text = Tools::string2String(pOrganizition->szName);
                     nodes->Tag = pOrganizition->nAssetMgmtOrgID;

                     return;
                 }
                 //有子节点
                 for each (TreeNode^ newNode in nodes->Nodes)
                 {
                     if (newNode->Nodes->Count > 0)
                     {
                         ModifyNode(newNode,strSelectedNodeName,pOrganizition);
                     }
                     //判断孩子节点
                     else
                     {
                         if (nodes->Text == Tools::string2String(pOrganizition->szName))
                         {
                             nodes->Text = Tools::string2String(pOrganizition->szName);
                             nodes->Tag = pOrganizition->nAssetMgmtOrgID;
                             return;
                         }
                     }
                 }
             }
    private: void DelOrganize(int nOrgID)
             {
                 DelNode(Organize_treeView->SelectedNode,nOrgID);
             }
    private: void DelNode(TreeNode^ nodes, int nOrgID)
             {
                 //防止节点没有子节点被忽略
                 if (::Convert::ToInt32(nodes->Tag) == nOrgID)
                 {
                     //判断选定的节点是否存在下一级节点
                     if ( nodes->Nodes->Count == 0 )    
                     {     
                         //删除节点
                         Organize_treeView->Nodes->Remove(nodes);  
                     }    
                     else 
                     {
                         MessageBox::Show("请先删除此节点中的子节点！", "提示",MessageBoxButtons::OK);  
                         return;
                     }
                 }

                 //有子节点
                 for each (TreeNode^ newNode in nodes->Nodes)
                 {
                     if (newNode->Nodes->Count > 0)
                     {
                         DelNode(newNode,nOrgID);
                     }
                     //判断孩子节点
                     else
                     {
                         if (::Convert::ToInt32(nodes->Tag) == nOrgID)
                         {
                             //判断选定的节点是否存在下一级节点
                             if ( nodes->Nodes->Count == 0 )    
                             {     
                                 //删除节点
                                 Organize_treeView->Nodes->Remove(nodes);  
                             }    
                             else 
                             {
                                 MessageBox::Show("请先删除此节点中的子节点！", "提示",MessageBoxButtons::OK);  
                                 return;
                             }
                         }
                     }
                 }



             }

    private: void LoadUserInfo()
             {
                 if(m_pDataInfo == NULL)
                     return;

                 std::vector<UserAndOrgRelation> vecUserAndOrgRelation;
                 m_pDataInfo->GetUserAndOrgRelation(vecUserAndOrgRelation);
                 if(vecUserAndOrgRelation.empty())
                     return;

                 std::vector<UserAndOrgRelation>::iterator it = vecUserAndOrgRelation.begin();
                 for (int i=0;it!=vecUserAndOrgRelation.end();it++)
                 {
                     if (it->nUserType == USER_TYPE_TRADER && it->nOrgID == ORGNIZATION_ROOT)
                     {
                         UserInfo oUserInfo;
                         memset(&oUserInfo,0,sizeof(oUserInfo));
                         m_pDataInfo->GetUserInfoByUserID(it->nUserID,oUserInfo);

                         ListViewItem^ item = gcnew ListViewItem(Tools::string2String(oUserInfo.szAccount),i++);
                         item->Tag = oUserInfo.nUserID;
                         item->SubItems->Add(Tools::string2String(oUserInfo.szAccount)); 
                         OutGroup_listView->Items->Add(item); 
                     }
                 }
             }
    private: void LoadUserInfoByOrgID(int nOrgID) 
             {
                 if(nOrgID < 0 || m_pDataInfo == NULL)
                     return;

                 InGroup_listView->Items->Clear();
                 OutGroup_listView->Items->Clear();

                 std::vector<UserAndOrgRelation> vecUserAndOrgRelation;
                 m_pDataInfo->GetUserAndOrgRelation(vecUserAndOrgRelation);
                 if(vecUserAndOrgRelation.empty())
                     return;

                 std::vector<UserAndOrgRelation>::iterator it = vecUserAndOrgRelation.begin();
                 for (int i=0;it!=vecUserAndOrgRelation.end();it++)
                 {
                     if (it->nUserType == USER_TYPE_TRADER && it->nOrgID == ORGNIZATION_ROOT)
                     {
                         UserInfo oUserInfo;
                         memset(&oUserInfo,0,sizeof(oUserInfo));
                         m_pDataInfo->GetUserInfoByUserID(it->nUserID,oUserInfo);

                         ListViewItem^ item = gcnew ListViewItem(Tools::string2String(oUserInfo.szAccount),i++);
                         item->Tag = oUserInfo.nUserID;
                         item->SubItems->Add(Tools::string2String(oUserInfo.szAccount)); 
                         OutGroup_listView->Items->Add(item); 
                     }
                 }

                 if(nOrgID == ORGNIZATION_ROOT)
                 {                     
                     this->label1->Text = "总部内交易员列表";
                     this->LeftAll_button->Enabled = false;
                     this->Left_button->Enabled = false;
                     this->RightAll_button->Enabled = false;
                     this->Right_button->Enabled = false;
                     this->Save_button->Enabled = false;

                     std::vector<UserAndOrgRelation>::iterator it = vecUserAndOrgRelation.begin();
                     for (int i=0;it!=vecUserAndOrgRelation.end();it++)
                     {
                         if (it->nUserType == USER_TYPE_TRADER && it->nOrgID == ORGNIZATION_ROOT)
                         {
                             UserInfo oUserInfo;
                             memset(&oUserInfo,0,sizeof(oUserInfo));
                             m_pDataInfo->GetUserInfoByUserID(it->nUserID,oUserInfo);

                             ListViewItem^ item = gcnew ListViewItem(Tools::string2String(oUserInfo.szAccount),i++);
                             item->Tag = oUserInfo.nUserID;
                             item->SubItems->Add(Tools::string2String(oUserInfo.szAccount)); 
                             InGroup_listView->Items->Add(item);
                         }
                     }

                 }else
                 {
                     this->label1->Text = "部门内交易员列表";
                     this->LeftAll_button->Enabled = true;
                     this->Left_button->Enabled = true;
                     this->RightAll_button->Enabled = true;
                     this->Right_button->Enabled = true;
                     this->Save_button->Enabled = true;       
                     
                     
                     m_strUpperOrganize = Tools::string2String(m_pDataInfo->GetOrganizationNameByID(nOrgID).c_str());

                     std::vector<UserAndOrgRelation>::iterator it = vecUserAndOrgRelation.begin();
                     for (int i=0;it!=vecUserAndOrgRelation.end();it++)
                     {
                         if (nOrgID == it->nOrgID && USER_TYPE_TRADER == it->nUserType)
                         {
                             UserInfo oUserInfo;
                             memset(&oUserInfo,0,sizeof(oUserInfo));
                             m_pDataInfo->GetUserInfoByUserID(it->nUserID,oUserInfo);

                             ListViewItem^ item = gcnew ListViewItem(Tools::string2String(oUserInfo.szAccount),i++);
                             item->Tag = oUserInfo.nUserID;
                             item->SubItems->Add(Tools::string2String(oUserInfo.szAlias)); 
                             InGroup_listView->Items->Add(item); 
                         }
                     }
                 }                 
             }
    protected:virtual void WndProc( Message% m ) override{
                  if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival )
                  {
                      char *p = (char*)m.LParam.ToInt32();
                      Stru_UniPkgHead head;					
                      memset(&head, 0, sizeof(head));
                      memcpy(&head, p, sizeof(head));

                      String^ strError = String::Empty;
                      bool bShowMsg = true;

                      switch ( head.cmdid )
                      {	 
                      case Cmd_QueryOrg_Rsp:
                          {
                              bShowMsg = false;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pDataInfo->ClearOrganization();

                                  std::map<int, Organization> mapAssetMgmtOrg;
                                  int nCount = head.len / sizeof(Organization);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      Organization* pAssetMgmtOrg = (Organization*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(Organization));
                                      mapAssetMgmtOrg[pAssetMgmtOrg->nAssetMgmtOrgID] = *pAssetMgmtOrg;
                                  }

                                  m_pDataInfo->SetOrganization(mapAssetMgmtOrg);

                                  //加载组织机构
                                  LoadAssertMgmtOrgan(ORGNIZATION_ROOT);
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_QryUser_Rsp:
                          {         
                              bShowMsg = false;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pDataInfo->ClearUserInfo();

                                  std::map<int, UserInfo> mapUserInfo;
                                  int nCount = head.len / sizeof(UserInfo);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      UserInfo* pUserInfo = (UserInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(UserInfo));
                                      mapUserInfo[pUserInfo->nUserID] = *pUserInfo;
                                  }

                                  m_pDataInfo->SetUserInfo(mapUserInfo);
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          }
                      case Cmd_QryUserOrgRelation_Rsp:
                          {
                              bShowMsg = false;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pDataInfo->ClearUserAndOrgRelation();

                                  std::vector<UserAndOrgRelation> vecUserAndOrgRelation;
                                  int nCount = head.len / sizeof(UserAndOrgRelation);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      UserAndOrgRelation* pUserAndOrgRelation = (UserAndOrgRelation*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(UserAndOrgRelation));
                                      vecUserAndOrgRelation.push_back(*pUserAndOrgRelation);                             
                                  }

                                  m_pDataInfo->SetUserAndOrgRelation(vecUserAndOrgRelation);	

                                  //加载未分配交易员
                                  LoadUserInfo();
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_AddOrg_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pCurOrganize->nAssetMgmtOrgID = *(int*)(p+sizeof(Stru_UniPkgHead));
                                  m_pDataInfo->SetOrganization(m_pCurOrganize);

                                  AddOrganize(m_pCurOrganize);

                                  strError = "增加组织机构操作成功";
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_ModifyOrg_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int nOrgID = *(int*)(p+sizeof(Stru_UniPkgHead));
                                  if ( nOrgID == m_pCurOrganize->nAssetMgmtOrgID )
                                  {
                                      m_pDataInfo->SetOrganization(m_pCurOrganize);

                                      ModifyOrganize(m_strSelectedNodeName, m_pCurOrganize);

                                      //重新加载
                                      Organize_treeView->Nodes->Clear();
                                      LoadAssertMgmtOrgan(ORGNIZATION_ROOT);
                                  }

                                  strError = "修改组织机构操作成功";
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_DelOrg_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int nOrgID = *(int*)(p+sizeof(Stru_UniPkgHead));
                                  m_pDataInfo->DelOrganization(nOrgID);

                                  DelOrganize(nOrgID);                                  
                                  
                                  LoadUserInfoByOrgID(ORGNIZATION_ROOT);

                                  strError = "删除组织机构操作成功";
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_SaveUserOrgRelation_FromOrg_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {            
                                  UserAndOrgRelation oRelation;                                   
                                  oRelation.nRelationType = USER_ORG_RELATION_ATTACH;
                                  oRelation.nUserType = USER_TYPE_TRADER;

                                  oRelation.nOrgID = ::Convert::ToInt32(Organize_treeView->SelectedNode->Tag);
                                  for (int i=0;i<InGroup_listView->Items->Count;i++)
                                  {
                                      String^ strAccount = InGroup_listView->Items[i]->SubItems[0]->Text;
                                      int nUserID = m_pDataInfo->GetUserIDByAccount(Tools::String2string(strAccount));
                                      oRelation.nUserID = nUserID;

                                      m_pDataInfo->SetUserAndOrgRelation(&oRelation);   
                                  }

                                  oRelation.nOrgID = ORGNIZATION_ROOT;
                                  for (int i=0;i<OutGroup_listView->Items->Count;i++)
                                  {
                                      String^ strAccount = OutGroup_listView->Items[i]->SubItems[0]->Text;
                                      int nUserID = m_pDataInfo->GetUserIDByAccount(Tools::String2string(strAccount));
                                      oRelation.nUserID = nUserID;

                                      m_pDataInfo->SetUserAndOrgRelation(&oRelation);   
                                  }

                                  strError = "保存组织机构与交易员关系操作成功";
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      }

                      if(bShowMsg)
						_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
								CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
                                Tools::String2string(strError).c_str());
                  }

                  Form::WndProc ( m );
              }
    private: System::Void OrganizeMgrForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 this->CenterToScreen();

                 int nHwnd = this->Handle.ToInt32();

                 m_pDataInfo->GetLogonInfo(*m_pLogin);

                 CTcpLayer::SubscribePkg(Cmd_AddOrg_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_DelOrg_Rsp, nHwnd);             
                 CTcpLayer::SubscribePkg(Cmd_ModifyOrg_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_SaveUserOrgRelation_FromOrg_Rsp, nHwnd);  

                 CTcpLayer::SubscribePkg(Cmd_QueryOrg_Rsp, nHwnd);
                 CTcpLayer::SendData(Cmd_QueryOrg_Req, NULL,0, 0, m_pLogin->mnUserID);//组织机构               

                 CTcpLayer::SubscribePkg(Cmd_QryUser_Rsp, nHwnd);//用户
                 CTcpLayer::SendData(Cmd_QryUser_Req, NULL,0, 0, m_pLogin->mnUserID);

                 CTcpLayer::SubscribePkg(Cmd_QryUserOrgRelation_Rsp, nHwnd);//用户与组织机构关系
                 CTcpLayer::SendData(Cmd_QryUserOrgRelation_Req, NULL,0, 0, m_pLogin->mnUserID);
             }
    private: System::Void Add_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_ORGANIZE_ADD));
                 OrganizeForm^ dlg = gcnew OrganizeForm();
                 dlg->SetOperType(Oper_Add);
                 dlg->SetCurUpperOrganize(m_strUpperOrganize);

                 if (::DialogResult::OK == dlg->ShowDialog(this))
                 {
                     *m_pCurOrganize = *(dlg->GetCurOrganize());             
                     m_pCurOrganize->nUpperLevelOrgID = m_pDataInfo->GetOrganizationIDByName(Tools::String2string(dlg->GetCurUpperOrganize()));
                     CTcpLayer::SendData(Cmd_AddOrg_Req, m_pCurOrganize, sizeof(Organization), 0, m_pLogin->mnUserID);
                 }
             }
    private: System::Void Modify_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_ORGANIZE_MODIFY));
                 if(Organize_treeView->SelectedNode != nullptr ) 
                 {
                     int nCurOrgID = ::Convert::ToInt32(Organize_treeView->SelectedNode->Tag);
                     if(m_pDataInfo->GetOrganizationByID(nCurOrgID,*m_pCurOrganize))
                     {
                         m_strSelectedNodeName = Tools::string2String(m_pCurOrganize->szName);
                         m_strUpperOrganize = Tools::string2String(m_pDataInfo->GetOrganizationNameByID(m_pCurOrganize->nUpperLevelOrgID).c_str());

                         OrganizeForm^ dlg = gcnew OrganizeForm(m_pCurOrganize->nAssetMgmtOrgID);
                         dlg->SetOperType(Oper_Modify);
                         dlg->SetCurOrganize(*m_pCurOrganize);
                         dlg->SetCurUpperOrganize(m_strUpperOrganize);

                         if (::DialogResult::OK == dlg->ShowDialog(this))
                         {
                             *m_pCurOrganize = *(dlg->GetCurOrganize());             
                             m_pCurOrganize->nUpperLevelOrgID = m_pDataInfo->GetOrganizationIDByName(Tools::String2string(dlg->GetCurUpperOrganize()));
                             CTcpLayer::SendData(Cmd_ModifyOrg_Req, m_pCurOrganize, sizeof(Organization), 0, m_pLogin->mnUserID);
                         }
                     } 
                 }
                 else
                 {
                     MessageBox::Show("请先选择一个部门！", "提示",MessageBoxButtons::OK);
                     return;
                 }
             }
    private: System::Void Delete_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_ORGANIZE_DEL));
                 if(Organize_treeView->SelectedNode != nullptr ) 
                 {
                     if(::DialogResult::Cancel ==  MessageBox::Show("此操作将会同步删除部门和交易员的关系。确认吗？", "提示", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
                         return;

                     int nCurOrgID = ::Convert::ToInt32(Organize_treeView->SelectedNode->Tag);
                     CTcpLayer::SendData(Cmd_DelOrg_Req, &nCurOrgID, sizeof(int), 0, m_pLogin->mnUserID);               
                 }
                 else
                 {
                     MessageBox::Show("请先选择一个部门！", "提示",MessageBoxButtons::OK);
                     return;
                 } 
             }
    private: System::Void Save_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_ORGANIZE_SAVE));
                 if(Organize_treeView->SelectedNode != nullptr)
                 {
                     int nRelationType = USER_ORG_RELATION_ATTACH;
                     int nUserType = USER_TYPE_TRADER;
                     int nOrgID = ::Convert::ToInt32(Organize_treeView->SelectedNode->Tag);

                     std::vector<int> vecUserIn;
                     for (int i=0;i<InGroup_listView->Items->Count;i++)
                     {
                         String^ strAccount = InGroup_listView->Items[i]->SubItems[0]->Text;
                         int nUserID = m_pDataInfo->GetUserIDByAccount(Tools::String2string(strAccount));

                         vecUserIn.push_back(nUserID);
                     }

                     std::vector<int> vecUserOut;
                     for (int i=0;i<OutGroup_listView->Items->Count;i++)
                     {
                         String^ strAccount = OutGroup_listView->Items[i]->SubItems[0]->Text;
                         int nUserID = m_pDataInfo->GetUserIDByAccount(Tools::String2string(strAccount));

                         vecUserOut.push_back(nUserID);
                     }

                     int nVecSizeIn = vecUserIn.size();
                     int nVecSizeOut = vecUserOut.size();
                     int nDataLen = 4*sizeof(int)+nVecSizeIn*sizeof(int)+sizeof(int)+nVecSizeOut*sizeof(int);
                     char* pBuffer = new char[nDataLen];
                     memset(pBuffer, 0, nDataLen);
                     memcpy(pBuffer, &nUserType, sizeof(int));
                     memcpy(pBuffer+sizeof(int), &nRelationType, sizeof(int));
                     memcpy(pBuffer+2*sizeof(int), &nOrgID, sizeof(int));

                     memcpy(pBuffer+3*sizeof(int), &nVecSizeIn, sizeof(int));
                     if ( !vecUserIn.empty() )
                         memcpy(pBuffer+4*sizeof(int), &vecUserIn[0], nVecSizeIn*sizeof(int));

                     memcpy(pBuffer+4*sizeof(int)+nVecSizeIn*sizeof(int), &nVecSizeOut, sizeof(int));
                     if ( !vecUserOut.empty() )
                         memcpy(pBuffer+4*sizeof(int)+nVecSizeIn*sizeof(int)+sizeof(int), &vecUserOut[0], nVecSizeOut*sizeof(int));                 

                     CTcpLayer::SendData(Cmd_SaveUserOrgRelation_FromOrg_Req, pBuffer, nDataLen, 0, m_pLogin->mnUserID); 

                     delete [] pBuffer;
                 }
                 else
                     MessageBox::Show("请先选择一个组织机构");
             }
    private: System::Void LeftAll_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 for (int i=0;i<OutGroup_listView->Items->Count;i++)
                 {
                     String^ strAccount = OutGroup_listView->Items[i]->SubItems[0]->Text;
                     String^ strUserName = OutGroup_listView->Items[i]->SubItems[1]->Text;

                     //UI
                     int nCount = InGroup_listView->Items->Count;
                     ListViewItem^ item = gcnew ListViewItem(strAccount,nCount);
                     item->Tag = m_pDataInfo->GetUserIDByAccount(Tools::String2string(strAccount));
                     item->SubItems->Add(strUserName);  
                     InGroup_listView->Items->Add( item );   
                 }

                 OutGroup_listView->Items->Clear();             
             }
    private: System::Void Left_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 ListView::SelectedIndexCollection^ c = OutGroup_listView->SelectedIndices;
                 if(c->Count == 0)
                     return;

                 vector<int> vecIndex;

                 for (int i=0;i<c->Count;i++)
                 {
                     String^ strAccount = OutGroup_listView->Items[c[i]]->SubItems[0]->Text;  
                     String^ strUserName = OutGroup_listView->Items[c[i]]->SubItems[1]->Text;

                     //UI
                     int nCount = InGroup_listView->Items->Count;
                     ListViewItem^ item = gcnew ListViewItem(strAccount,nCount); 
                     item->Tag = m_pDataInfo->GetUserIDByAccount(Tools::String2string(strAccount));
                     item->SubItems->Add(strUserName);  
                     InGroup_listView->Items->Add( item );  

                     vecIndex.push_back(c[i]);
                 }
                    
                 for (int j=(int)vecIndex.size()-1;j>=0;j--)
                     OutGroup_listView->Items->RemoveAt(vecIndex[j]); 
                                   
             }
    private: System::Void Right_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 ListView::SelectedIndexCollection^ c = InGroup_listView->SelectedIndices;
                 if(c->Count == 0)
                     return;

                 vector<int> vecIndex;

                 for (int i=0;i<c->Count;i++)
                 {
                     String^ strAccount = InGroup_listView->Items[c[i]]->SubItems[0]->Text;  
                     String^ strUserName = InGroup_listView->Items[c[i]]->SubItems[1]->Text;

                     //UI
                     int nCount = OutGroup_listView->Items->Count;
                     ListViewItem^ item = gcnew ListViewItem(strAccount,nCount); 
                     item->Tag = m_pDataInfo->GetUserIDByAccount(Tools::String2string(strAccount));
                     item->SubItems->Add(strUserName);  
                     OutGroup_listView->Items->Add( item );

                     vecIndex.push_back(c[i]);
                 }

                 for (int j=(int)vecIndex.size()-1;j>=0;j--)
                     InGroup_listView->Items->RemoveAt(vecIndex[j]);  
             }
    private: System::Void RightAll_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 for (int i=0;i<InGroup_listView->Items->Count;i++)
                 {
                     String^ strAccount = InGroup_listView->Items[i]->SubItems[0]->Text;
                     String^ strUserName = InGroup_listView->Items[i]->SubItems[1]->Text;

                     //UI
                     int nCount = OutGroup_listView->Items->Count;
                     ListViewItem^ item = gcnew ListViewItem(strAccount,nCount); 
                     item->Tag = m_pDataInfo->GetUserIDByAccount(Tools::String2string(strAccount));
                     item->SubItems->Add(strUserName);  
                     OutGroup_listView->Items->Add( item );   
                 }

                 InGroup_listView->Items->Clear();  
             }
    private: System::Void Organize_treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
                 //LoadUserInfo();
                 int nCurOrgID = ::Convert::ToInt32(Organize_treeView->SelectedNode->Tag);
                 LoadUserInfoByOrgID(nCurOrgID);
             }
    private: System::Void Organize_treeView_BeforeSelect(System::Object^  sender, System::Windows::Forms::TreeViewCancelEventArgs^  e) {
                 if (e->Node != nullptr)
                 {
                     if (e->Node->ForeColor == Color::Gray)
                     {
                         //e->Cancel = true;  //不让选中禁用节点
                     }
                 }

             }
    private: System::Void OrganizeMgrForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
                 int nHwnd = this->Handle.ToInt32();
                 CTcpLayer::UnsubscribeAllPkg(nHwnd);
             }
    };
}

#pragma once

#include <map>
#include "CommonStruct.h"
#include "TcpLayer.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonPkg.h"
#include "Tools.h"
#include "CDataInfo.h"
#include "Privilege_AddRoleForm.h"
#include "CommonUserOpr.h"


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

    /// <summary>
    /// Summary for PrivilegeForm
    ///
    /// WARNING: If you change the name of this class, you will need to change the
    ///          'Resource File Name' property for the managed resource compiler tool
    ///          associated with all .resx files this class depends on.  Otherwise,
    ///          the designers will not be able to interact properly with localized
    ///          resources associated with this form.
    /// </summary>
    public ref class PrivilegeForm : public System::Windows::Forms::Form
    {
    public:
        PrivilegeForm(void)
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //
            m_pDataInfo = CDataInfo::NewInstance();
            m_pCurRoleInfo = new RoleInfo;
            m_pLogin = new sLoginRsp;
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~PrivilegeForm()
        {
            if (components)
            {
                delete components;
            }

            CDataInfo::DestroyInstance();

            if(m_pCurRoleInfo)
            {
                delete m_pCurRoleInfo;
                m_pCurRoleInfo = NULL;
            }

            if(m_pLogin)
            {
                delete m_pLogin;
                m_pLogin = NULL;
            }
        }


    private: CDataInfo* m_pDataInfo;
    private: RoleInfo* m_pCurRoleInfo;

    private: System::Windows::Forms::ComboBox^  comboBox1;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Button^  button_save;

    private: System::ComponentModel::IContainer^  components;
    private: System::Windows::Forms::Button^  button_add;
    private: System::Windows::Forms::Button^  button_delete;
    private: System::Windows::Forms::ListView^  listView1;
    private: System::Windows::Forms::ColumnHeader^  columnHeader1;
    private: System::Windows::Forms::GroupBox^  groupBox2;
    private: System::Windows::Forms::TreeView^  treeView1;
    private: System::Windows::Forms::Button^  button_Modify;


    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>

        sLoginRsp* m_pLogin;


#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->button_save = (gcnew System::Windows::Forms::Button());
            this->button_add = (gcnew System::Windows::Forms::Button());
            this->button_delete = (gcnew System::Windows::Forms::Button());
            this->listView1 = (gcnew System::Windows::Forms::ListView());
            this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
            this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
            this->treeView1 = (gcnew System::Windows::Forms::TreeView());
            this->button_Modify = (gcnew System::Windows::Forms::Button());
            this->groupBox2->SuspendLayout();
            this->SuspendLayout();
            // 
            // comboBox1
            // 
            this->comboBox1->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->comboBox1->FormattingEnabled = true;
            this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"管理员", L"风控员"});
            this->comboBox1->Location = System::Drawing::Point(273, 22);
            this->comboBox1->Name = L"comboBox1";
            this->comboBox1->Size = System::Drawing::Size(121, 20);
            this->comboBox1->TabIndex = 5;
            this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &PrivilegeForm::comboBox1_SelectedIndexChanged);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(214, 26);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(53, 12);
            this->label1->TabIndex = 4;
            this->label1->Text = L"用户类型";
            // 
            // button_save
            // 
            this->button_save->Location = System::Drawing::Point(90, 408);
            this->button_save->Name = L"button_save";
            this->button_save->Size = System::Drawing::Size(75, 23);
            this->button_save->TabIndex = 3;
            this->button_save->Text = L"保存";
            this->button_save->UseVisualStyleBackColor = true;
            this->button_save->Click += gcnew System::EventHandler(this, &PrivilegeForm::button_save_Click);
            // 
            // button_add
            // 
            this->button_add->Location = System::Drawing::Point(12, 462);
            this->button_add->Name = L"button_add";
            this->button_add->Size = System::Drawing::Size(53, 23);
            this->button_add->TabIndex = 1;
            this->button_add->Text = L"新增";
            this->button_add->UseVisualStyleBackColor = true;
            this->button_add->Click += gcnew System::EventHandler(this, &PrivilegeForm::button_add_Click);
            // 
            // button_delete
            // 
            this->button_delete->BackColor = System::Drawing::Color::Azure;
            this->button_delete->Location = System::Drawing::Point(128, 462);
            this->button_delete->Name = L"button_delete";
            this->button_delete->Size = System::Drawing::Size(53, 23);
            this->button_delete->TabIndex = 2;
            this->button_delete->Text = L"删除";
            this->button_delete->UseVisualStyleBackColor = false;
            this->button_delete->Click += gcnew System::EventHandler(this, &PrivilegeForm::button_delete_Click);
            // 
            // listView1
            // 
            this->listView1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(1) {this->columnHeader1});
            this->listView1->FullRowSelect = true;
            this->listView1->GridLines = true;
            this->listView1->HideSelection = false;
            this->listView1->Location = System::Drawing::Point(12, 22);
            this->listView1->MultiSelect = false;
            this->listView1->Name = L"listView1";
            this->listView1->Size = System::Drawing::Size(184, 431);
            this->listView1->TabIndex = 3;
            this->listView1->UseCompatibleStateImageBehavior = false;
            this->listView1->View = System::Windows::Forms::View::Details;
            this->listView1->AfterLabelEdit += gcnew System::Windows::Forms::LabelEditEventHandler(this, &PrivilegeForm::listView1_AfterLabelEdit);
            this->listView1->ItemSelectionChanged += gcnew System::Windows::Forms::ListViewItemSelectionChangedEventHandler(this, &PrivilegeForm::listView1_ItemSelectionChanged);
            // 
            // columnHeader1
            // 
            this->columnHeader1->Text = L"角色名";
            this->columnHeader1->Width = 172;
            // 
            // groupBox2
            // 
            this->groupBox2->Controls->Add(this->button_save);
            this->groupBox2->Controls->Add(this->treeView1);
            this->groupBox2->Location = System::Drawing::Point(215, 48);
            this->groupBox2->Name = L"groupBox2";
            this->groupBox2->Size = System::Drawing::Size(254, 437);
            this->groupBox2->TabIndex = 2;
            this->groupBox2->TabStop = false;
            this->groupBox2->Text = L"权限设置";
            // 
            // treeView1
            // 
            this->treeView1->CheckBoxes = true;
            this->treeView1->Location = System::Drawing::Point(6, 17);
            this->treeView1->Name = L"treeView1";
            this->treeView1->Size = System::Drawing::Size(242, 385);
            this->treeView1->TabIndex = 6;
            this->treeView1->AfterCheck += gcnew System::Windows::Forms::TreeViewEventHandler(this, &PrivilegeForm::treeView1_AfterCheck);
            // 
            // button_Modify
            // 
            this->button_Modify->Location = System::Drawing::Point(70, 462);
            this->button_Modify->Name = L"button_Modify";
            this->button_Modify->Size = System::Drawing::Size(53, 23);
            this->button_Modify->TabIndex = 6;
            this->button_Modify->Text = L"修改";
            this->button_Modify->UseVisualStyleBackColor = true;
            this->button_Modify->Click += gcnew System::EventHandler(this, &PrivilegeForm::button_Modify_Click);
            // 
            // PrivilegeForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(481, 495);
            this->Controls->Add(this->button_Modify);
            this->Controls->Add(this->groupBox2);
            this->Controls->Add(this->comboBox1);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->listView1);
            this->Controls->Add(this->button_delete);
            this->Controls->Add(this->button_add);
            this->Name = L"PrivilegeForm";
            this->Text = L"权限设置";
            this->Load += gcnew System::EventHandler(this, &PrivilegeForm::PrivilegeForm_Load);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &PrivilegeForm::PrivilegeForm_FormClosed);
            this->groupBox2->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void PrivilegeForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 comboBox1->SelectedIndex = 0;
                 this->CenterToParent();

                 int nHwnd = this->Handle.ToInt32();

                 m_pDataInfo->GetLogonInfo(*m_pLogin);

                 CTcpLayer::SubscribePkg(Cmd_AddRole_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_DelRole_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_ModifyRole_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_SaveRolePrivilegeRelation_Rsp, nHwnd);

                 CTcpLayer::SubscribePkg(Cmd_QryRole_Rsp, nHwnd);
                 CTcpLayer::SendData(Cmd_QryRole_Req, NULL,0, 0, m_pLogin->mnUserID);

                 CTcpLayer::SubscribePkg(Cmd_QryPrivilege_Rsp, nHwnd);
                 CTcpLayer::SendData(Cmd_QryPrivilege_Req, NULL,0, 0, m_pLogin->mnUserID);

                 CTcpLayer::SubscribePkg(Cmd_QueryRolePrivilegeRelation_Rsp, nHwnd);
                 CTcpLayer::SendData(Cmd_QueryRolePrivilegeRelation_Req, NULL,0, 0, m_pLogin->mnUserID);

                 CTcpLayer::SubscribePkg(Cmd_QryUserRoleRelation_Rsp, nHwnd);
                 CTcpLayer::SendData(Cmd_QryUserRoleRelation_Req, NULL,0, 0, m_pLogin->mnUserID);
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
                      case Cmd_QryRole_Rsp:
                          {
                              bShowMsg = false;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pDataInfo->ClearRole();
                                  listView1->Items->Clear();

                                  std::map<int, RoleInfo> mapRoleInfo;
                                  int nCount = head.len / sizeof(RoleInfo);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      RoleInfo* pRoInfo = (RoleInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(RoleInfo));
                                      mapRoleInfo[pRoInfo->nRoleID] = *pRoInfo;

                                      if ( USER_TYPE_MANAGE == pRoInfo->nUserTypeID)
                                      {
                                          ListViewItem^ item = gcnew ListViewItem( Tools::string2String(pRoInfo->szName),i);   
                                          item->Tag = pRoInfo->nRoleID;
                                          listView1->Items->Add( item );
                                      }
                                  }

                                  m_pDataInfo->SetRole(mapRoleInfo);
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_QryPrivilege_Rsp:
                          {
                              bShowMsg = false;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  std::map<int, Privilege> mapPrivilege;
                                  std::map<int,std::vector<Privilege>> mapUserType2Privilege;
                                  std::map<int,std::vector<Privilege>> mapParentID2Privilege;

                                  int nCount = head.len / sizeof(Privilege);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      Privilege* pPrivilege = (Privilege*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(Privilege));
                                      mapPrivilege[pPrivilege->nPrivilegeID] = *pPrivilege;

                                      if ( pPrivilege->nUserTypeID >= 0 )
                                          mapUserType2Privilege[pPrivilege->nUserTypeID].push_back(*pPrivilege);
                                      if ( pPrivilege->nParentID != -1 )
                                          mapParentID2Privilege[pPrivilege->nParentID].push_back(*pPrivilege);
                                  }

                                  m_pDataInfo->SetPrivilege(mapPrivilege);
                                  m_pDataInfo->SetUserType2Privilege(mapUserType2Privilege);
                                  m_pDataInfo->SetParentID2Privilege(mapParentID2Privilege);

                                  //LoadPrivilege(comboBox1->SelectedIndex);                         

                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_QueryRolePrivilegeRelation_Rsp:
                          {
                              bShowMsg = false;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pDataInfo->ClearRolePrivilegeRelation();                           

                                  std::map<int, std::vector<int>> mapRole2Privilege;
                                  int nCount = head.len / sizeof(RoleAndPrivilegeRelation);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      RoleAndPrivilegeRelation* pRelation = (RoleAndPrivilegeRelation*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(RoleAndPrivilegeRelation));
                                      mapRole2Privilege[pRelation->nRoleID].push_back(pRelation->nPrivilegeID);
                                  }

                                  m_pDataInfo->SetRolePrivilegeRelation(mapRole2Privilege);
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_ModifyRole_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int nRoleID = *(int*)(p+sizeof(Stru_UniPkgHead));
                                  m_pDataInfo->SetRole(*m_pCurRoleInfo);

                                  listView1->SelectedItems[0]->SubItems[0]->Text = Tools::string2String(m_pCurRoleInfo->szName);

                                  strError = "修改角色操作成功";
                              }
                              else
                              {
                                  strError = "修改角色操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_AddRole_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pCurRoleInfo->nRoleID = *(int*)(p+sizeof(Stru_UniPkgHead));
                                  m_pDataInfo->SetRole(*m_pCurRoleInfo);

                                  int nCount = listView1->Items->Count;
                                  ListViewItem^ item = gcnew ListViewItem( Tools::string2String(m_pCurRoleInfo->szName),nCount);
                                  item->Tag = m_pCurRoleInfo->nRoleID;
                                  item->SubItems->Add( Tools::string2String(m_pCurRoleInfo->szName));

                                  //UI
                                  listView1->Items->Add( item );

                                  strError = "增加角色操作成功";
                              }
                              else
                              {
                                  strError = "增加角色操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_DelRole_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int nRoleID = *(int*)(p+sizeof(Stru_UniPkgHead));
                                  m_pDataInfo->DelRole(nRoleID);

                                  ListView::SelectedIndexCollection^ c = listView1->SelectedIndices;
                                  if(c != nullptr)
                                      listView1->Items->RemoveAt(c[0]);  

                                  //遍历树
                                  for each (TreeNode^ CNode in treeView1->Nodes)                           
                                      InitPrivilege(CNode);

                                  strError = "删除角色操作成功";
                              }
                              else
                              {
                                  strError = "删除角色操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_SaveRolePrivilegeRelation_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int nRoleID = *(int*)(p+sizeof(Stru_UniPkgHead));

                                  std::vector<int> vec;
                                  for each (TreeNode^ CNode in treeView1->Nodes)    
                                      GetSelectedPrivilege(CNode, vec);

                                  m_pDataInfo->SetRolePrivilegeRelation(nRoleID, vec);

                                  strError = "保存角色和权限关系操作成功";
                              }
                              else
                              {
                                  strError = "保存角色和权限关系操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));

                                  LoadPrivilege(comboBox1->SelectedIndex);
                                  ListView::SelectedIndexCollection^ c = listView1->SelectedIndices;
                                  ListViewItem^ item = listView1->Items[c[0]];
                                  int nRoleID = (int)(item->Tag);
                                  UpdataPrivilege(nRoleID); 
                              }

                              break;
                          }
                      case Cmd_QryUserRoleRelation_Rsp:
                          {                  
                              bShowMsg = false;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pDataInfo->ClearUserRoleRelation();

                                  std::map<int, std::vector<UserRole>> mapUserRole;
                                  int nCount = head.len / sizeof(UserRole);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      UserRole* pUserRole = (UserRole*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(UserRole));
                                      mapUserRole[pUserRole->nUserID].push_back(*pUserRole);
                                  }

                                  m_pDataInfo->SetUserRoleRelation(mapUserRole);

                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          }

                      default:
                          break;
                      }
                    
                      if(bShowMsg)
						_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
								CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
                                Tools::String2string(strError).c_str());
                  }

                  Form::WndProc ( m );
              }
    private: System::Void button_add_Click(System::Object^  sender, System::EventArgs^  e) { 
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_PRIVILEGE_ADD));
                 eUserType nUserType = (0==comboBox1->SelectedIndex)?eManagerType:eRiskerType;
                 Privilege_AddRoleForm^ dlg = gcnew Privilege_AddRoleForm();
                 dlg->SetUserType(nUserType);
                 dlg->SetOperType(Oper_Add);
                 if (::DialogResult::OK == dlg->ShowDialog(this))
                 {
                     *m_pCurRoleInfo = *(dlg->GetCurRole());             
                     CTcpLayer::SendData(Cmd_AddRole_Req, m_pCurRoleInfo, sizeof(RoleInfo), 0, m_pLogin->mnUserID);
                 }
             }
    private: System::Void button_Modify_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_PRIVILEGE_MODIFY));
                 if(listView1->SelectedIndices != nullptr && listView1->SelectedIndices->Count>0)
                 {
                     String^ strName = listView1->SelectedItems[0]->SubItems[0]->Text;
                     int nRoleID = m_pDataInfo->GetRoleIDByName(Tools::String2string(strName));
                     if(!m_pDataInfo->GetRoleInfoByID(nRoleID,*m_pCurRoleInfo))
                         return;

                     Privilege_AddRoleForm^ dlg = gcnew Privilege_AddRoleForm();
                     dlg->SetOperType(Oper_Modify);
                     dlg->SetCurRole(*m_pCurRoleInfo);
                     if (::DialogResult::OK == dlg->ShowDialog(this))
                     {
                         *m_pCurRoleInfo = *(dlg->GetCurRole());             
                         CTcpLayer::SendData(Cmd_ModifyRole_Req, m_pCurRoleInfo, sizeof(RoleInfo), 0, m_pLogin->mnUserID);
                     }
                 }
                 else
                     MessageBox::Show("请选择内容");
             }
    private: System::Void button_delete_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_PRIVILEGE_DEL));
                 if(::DialogResult::Cancel ==  MessageBox::Show("此操作将会同步删除和用户的关系，角色和权限的关系。确认吗？", "提示", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
                     return;

                 if(listView1->SelectedIndices != nullptr && listView1->SelectedIndices->Count>0)
                 {
                     String^ strName = listView1->SelectedItems[0]->SubItems[0]->Text;
                     int nRoleID = m_pDataInfo->GetRoleIDByName(Tools::String2string(strName));

                     CTcpLayer::SendData(Cmd_DelRole_Req,&nRoleID,sizeof(int),0, m_pLogin->mnUserID);
                 }
                 else
                     MessageBox::Show("请选择内容");
             }
    private: System::Void button_save_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_PRIVILEGE_SAVE));
                 if(listView1->SelectedIndices != nullptr && listView1->SelectedIndices->Count>0)
                 {
                     ListView::SelectedIndexCollection^ c = listView1->SelectedIndices;
                     ListViewItem^ item = listView1->Items[c[0]];
                     int nRoleID = Convert::ToInt32(item->Tag);
                     if ( nRoleID <= 0 )
                     {
                         MessageBox::Show("请先选择一条记录！", "提示",MessageBoxButtons::OK);
                     }
                     else
                     {
                         std::vector<int> vValue;
                         //遍历树
                         for each (TreeNode^ CNode in treeView1->Nodes)    
                             GetSelectedPrivilege(CNode, vValue);

                         int nDataLen = sizeof(int)+sizeof(int)*vValue.size();
                         char* pBuffer = new char[nDataLen];
                         memset(pBuffer, 0, nDataLen);
                         memcpy(pBuffer, &nRoleID, sizeof(int));
                         if ( !vValue.empty() )
                         {
                             memcpy(pBuffer+sizeof(int), &vValue[0], sizeof(int)*vValue.size());
                         }

                         CTcpLayer::SendData(Cmd_SaveRolePrivilegeRelation_Req, pBuffer, nDataLen, 0, m_pLogin->mnUserID);

                         delete [] pBuffer;
                     }
                 }
                 else
                 {
                     MessageBox::Show("请先选择一条记录！", "提示",MessageBoxButtons::OK);
                 }
             }
    private: System::Void listView1_AfterLabelEdit(System::Object^  sender, System::Windows::Forms::LabelEditEventArgs^  e) {
//                  String^ strNewLabel;
//                  if ( e->Label == nullptr)
//                  {
//                      strNewLabel = listView1->Items[e->Item]->Text;
//                  }
//                  else
//                  {
//                      strNewLabel = e->Label;
//                  }
// 
//                  ListViewItem^ item = listView1->Items[e->Item];
// 
//                  if ( strNewLabel->Length == 0 )
//                  {      
//                      MessageBox::Show("角色名不能为空，请重新添加", "提示",MessageBoxButtons::OK);
//                      e->CancelEdit = true;   
//                      listView1->Items->Remove(item);
//                      //item->BeginEdit();                     
//                      return;
//                  }
//                  if (strNewLabel->Length > MAX_PERMISSION_NAME_LENGTH)
//                  {
//                      MessageBox::Show("角色名太长，请重新添加", "提示",MessageBoxButtons::OK);
//                      e->CancelEdit = true;
//                      listView1->Items->Remove(item);
//                      //item->BeginEdit();
//                      return;
//                  }
// 
//                  int nRoleID = Convert::ToInt32(item->Tag);
//                  int nUserType = comboBox1->SelectedIndex;
// 
//                  if ( nRoleID <= 0 )
//                  {
//                      //添加		
//                      int nOtherRoleID;
//                      if(!m_pDataInfo->IsValidOfRoleName(Tools::String2string(strNewLabel),nOtherRoleID))
//                      {
//                          MessageBox::Show("角色名重复，请重新添加", "提示",MessageBoxButtons::OK);
//                          e->CancelEdit = true;
//                          listView1->Items->Remove(item);
//                          //item->BeginEdit();
//                          return;
//                      } 
// 
//                      strcpy(m_pCurRoleInfo->szName, Tools::String2string(strNewLabel).c_str());
//                      m_pCurRoleInfo->nUserTypeID = nUserType;
//                      CTcpLayer::SendData(Cmd_AddRole_Req, m_pCurRoleInfo,sizeof(RoleInfo), 0, m_pLogin->mnUserID);
//                  }
//                  else
//                  {
//                      //修改
//                      std::map<int, RoleInfo> mapRoleInfo;
//                      m_pDataInfo->GetRole(mapRoleInfo);
//                      std::map<int, RoleInfo>::iterator it = mapRoleInfo.find(nRoleID);
//                      if ( it != mapRoleInfo.end())
//                      {
//                          if ( Tools::String2string(strNewLabel).c_str() == it->second.szName )
//                          {
//                              //没有改动，直接返回
//                              return;
//                          }
// 
//                          int nOtherRoleID=nRoleID;
//                          if(!m_pDataInfo->IsValidOfRoleName(Tools::String2string(strNewLabel),nOtherRoleID))
//                          {
//                              if(nOtherRoleID != nRoleID)
//                              {
//                                  MessageBox::Show("角色名与列表中其它名称重复，请重新修改", "提示",MessageBoxButtons::OK);
//                                  e->CancelEdit = true;
//                                  return;
//                              }
//                          }
// 
//                          memcpy(m_pCurRoleInfo,&(it->second),sizeof(RoleInfo));
//                          strcpy(m_pCurRoleInfo->szName, Tools::String2string(strNewLabel).c_str());
//                          CTcpLayer::SendData(Cmd_ModifyRole_Req, m_pCurRoleInfo,sizeof(RoleInfo), 0, m_pLogin->mnUserID);
//                      }else
//                      {
//                          int nOtherRoleID=nRoleID;
//                          if(!m_pDataInfo->IsValidOfRoleName(Tools::String2string(strNewLabel),nOtherRoleID))
//                          {
//                              if(nOtherRoleID != nRoleID)
//                              {
//                                  MessageBox::Show("角色名与列表中其它名称重复，请重新添加", "提示",MessageBoxButtons::OK);
//                                  e->CancelEdit = true;
//                                  return;
//                              }
//                          } 
//                      }
//                  }
             }

    private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
                 LoadRole(comboBox1->SelectedIndex);
                 treeView1->Nodes->Clear();
                 //LoadPrivilege(comboBox1->SelectedIndex);
             }
    private: System::Void listView1_ItemSelectionChanged(System::Object^  sender, System::Windows::Forms::ListViewItemSelectionChangedEventArgs^  e) {
                 if ( e->IsSelected )
                 {
                     LoadPrivilege(comboBox1->SelectedIndex);
                     int nRoleID = (int)(listView1->Items[e->ItemIndex]->Tag);
                     UpdataPrivilege(nRoleID);                     
                 }
                 else
                 {
                     //遍历树
                     //for each (TreeNode^ CNode in treeView1->Nodes)                           
                     //    InitPrivilege(CNode);
                     treeView1->Nodes->Clear();
                 }
             }
    private: System::Void treeView1_AfterCheck(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
                 if (e->Action != TreeViewAction::Unknown)
                 {
                     SetNodeCheckedStatus(e->Node, e->Node->Checked);
                 }
             }
    private: System::Void PrivilegeForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
                 int nHwnd = this->Handle.ToInt32();
                 CTcpLayer::UnsubscribeAllPkg(nHwnd);
             }
    private: void LoadRole(int nUserType)
             {
                 listView1->Items->Clear();

                 std::map<int,RoleInfo> mapRoleInfo;
                 m_pDataInfo->GetRole(mapRoleInfo);

                 std::map<int,RoleInfo>::iterator it = mapRoleInfo.begin();
                 for ( int i=0;it!=mapRoleInfo.end(); it++,i++ )
                 {
                     if ( nUserType == it->second.nUserTypeID )
                     {
                         ListViewItem^ item = gcnew ListViewItem( Tools::string2String(it->second.szName),i);   
                         item->Tag = it->second.nRoleID;
                         listView1->Items->Add( item );
                     }
                 }

             }
    private: void LoadPrivilege(int nUserType)
             {
                 treeView1->Nodes->Clear();

                 std::map<int,std::vector<Privilege>> mapUserType2Privilege;
                 std::map<int,std::vector<Privilege>> mapParentID2Privilege;

                 m_pDataInfo->GetUserType2Privilege(mapUserType2Privilege);
                 m_pDataInfo->GetParentID2Privilege(mapParentID2Privilege);

                 std::map<int,std::vector<Privilege>>::iterator it_root = mapUserType2Privilege.find(nUserType);
                 if ( it_root != mapUserType2Privilege.end() )
                 {
                     std::vector<Privilege> vecRootNode = it_root->second; 
                     std::vector<Privilege>::iterator it_node = vecRootNode.begin();
                     for ( ;it_node != vecRootNode.end(); it_node++ )
                     {
                         //一级
                         TreeNode^ tn = gcnew TreeNode();
                         tn->Text = Tools::string2String(it_node->szName);
                         tn->Tag = it_node->nPrivilegeID;
                         //if(nUserType == it_node->nUserTypeID) 
                            treeView1->Nodes->Add(tn);
          
                         std::map<int,std::vector<Privilege>>::iterator it_root2 = mapParentID2Privilege.find(it_node->nPrivilegeID);
                         if ( it_root2 != mapParentID2Privilege.end() )
                         {
                             std::vector<Privilege> vecRootNode2 = it_root2->second;
                             std::vector<Privilege>::iterator it_node2 = vecRootNode2.begin();
                             for ( ;it_node2 != vecRootNode2.end();it_node2++ )
                             {
                                 //二级
                                 TreeNode^ tn2 = gcnew TreeNode();
                                 tn2->Text = Tools::string2String(it_node2->szName);
                                 tn2->Tag = it_node2->nPrivilegeID;
                                 //if(nUserType == it_node2->nUserTypeID) 
                                     tn->Nodes->Add(tn2);
             

                                 std::map<int,std::vector<Privilege>>::iterator it_root3 = mapParentID2Privilege.find(it_node2->nPrivilegeID);
                                 if ( it_root3 != mapParentID2Privilege.end() )
                                 {
                                     std::vector<Privilege> vecRootNode3 = it_root3->second;
                                     std::vector<Privilege>::iterator it_node3 = vecRootNode3.begin();
                                     for ( ;it_node3 != vecRootNode3.end();it_node3++ )
                                     {
                                         //三级
                                         TreeNode^ tn3 = gcnew TreeNode();
                                         tn3->Text = Tools::string2String(it_node3->szName);
                                         tn3->Tag = it_node3->nPrivilegeID;
                                         //if(nUserType == it_node3->nUserTypeID) 
                                             tn2->Nodes->Add(tn3);
                             
                                     }
                                 }
                             }
                         }

                         tn->Expand();
                     }

                 }
             }
    private: void GetSelectedPrivilege(TreeNode^ node, std::vector<int>& vec)
             {
                 if (node->Checked) 
                     vec.push_back(::Convert::ToInt32(node->Tag));

                 for each(TreeNode^ childNode in node->Nodes)                                  
                     GetSelectedPrivilege(childNode, vec);// 递归
             }
    private: void InitPrivilege(TreeNode^ node)
             {
                 if (node->Checked)
                     node->Checked = false;        

                 for each(TreeNode^ childNode in node->Nodes)                                 
                     InitPrivilege(childNode);// 递归            
             }
    private: void InitPrivilege(TreeNode^ node, std::vector<int>& vec)
             {
                 std::vector<int>::iterator it = vec.begin();
                 for ( ;it!=vec.end(); it++)
                 {
                     if (::Convert::ToInt32(node->Tag) == (*it))
                     {
                         node->Checked = true;   
                         break;
                     }
                 }       

                 for each(TreeNode^ childNode in node->Nodes)                                  
                     InitPrivilege(childNode, vec);// 递归             
             }
    private: void UpdataPrivilege(int nRoleID)
             {
                 std::map<int,std::vector<int>> mapRoleID2Privilege;
                 m_pDataInfo->GetRolePrivilegeRelation(mapRoleID2Privilege);

                 std::map<int,std::vector<int>>::iterator it = mapRoleID2Privilege.find(nRoleID);
                 if( it!=mapRoleID2Privilege.end() )
                 {
                     //遍历树
                     for each (TreeNode^ CNode in treeView1->Nodes)                 
                         InitPrivilege(CNode,it->second);                 
                 }
             }
    private: void SetNodeCheckedStatus(TreeNode^ tn, bool bChecked)
             {
                 if (tn == nullptr) return;

                 // 设置子节点状态
                 for each (TreeNode^ tnChild in tn->Nodes)
                 {
                     tnChild->Checked = bChecked;

                     //递归设置字节点状态与父节点状态一致
                     SetNodeCheckedStatus(tnChild, bChecked);
                 }
                 // 设置父节点状态
                 TreeNode^ tnParent = tn;
                 int nNodeCount = 0;

                 //循环设置父节点状态
                 while (tnParent->Parent != nullptr)
                 {
                     tnParent = (TreeNode^)(tnParent->Parent);
                     nNodeCount = 0;
                     bool isChecked = false;//记录父节点中是否有选中的子节点
                     for each (TreeNode^ tnTemp in tnParent->Nodes)
                     {
                         if (tnTemp->Checked == bChecked)
                         {
                             nNodeCount++;
                         }

                         if (tnTemp->Checked)
                         {
                             isChecked = true;
                         }
                     }

                     if (nNodeCount > 0 && isChecked)
                     {
                         tnParent->Checked = true;//若有则选中父节点
                     }
                     else
                     {
                         tnParent->Checked = bChecked;
                     }                
                 }
             }
};
}

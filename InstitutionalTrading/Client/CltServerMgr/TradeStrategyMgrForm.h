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
#include "TradeStrategyForm.h"
#include "CommonUserOpr.h"


namespace CltServerMgr {

	/// <summary>
	/// Summary for TradeStrategyMgrForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class TradeStrategyMgrForm : public System::Windows::Forms::Form
	{
	public:
		TradeStrategyMgrForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pLogin = new sLoginRsp();
            m_pCurTradeStrategy = new TradeStrategy;
            m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TradeStrategyMgrForm()
		{
			if (components)
			{
				delete components;
			}

            if(m_pLogin)
            {
                delete m_pLogin;
                m_pLogin = nullptr;
            }
            if (m_pCurTradeStrategy)
            {
                delete m_pCurTradeStrategy;
                m_pCurTradeStrategy = NULL;
            }
		}

    private: sLoginRsp*  m_pLogin;
    private: CDataInfo*  m_pDataInfo;
    private: TradeStrategy* m_pCurTradeStrategy;


    private: System::Windows::Forms::ListView^  Strategy_listView;
    private: System::Windows::Forms::Button^  Add_button;
    private: System::Windows::Forms::Button^  Modify_button;
    private: System::Windows::Forms::Button^  Delete_button;
    private: System::Windows::Forms::ColumnHeader^  columnHeader1;
    private: System::Windows::Forms::GroupBox^  groupBox1;
    private: System::Windows::Forms::TreeView^  Org_treeView;
    private: System::Windows::Forms::Button^  Save_button;
    private: System::Windows::Forms::ColumnHeader^  columnHeader2;
    private: System::Windows::Forms::ColumnHeader^  columnHeader3;


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
            this->Strategy_listView = (gcnew System::Windows::Forms::ListView());
            this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
            this->Add_button = (gcnew System::Windows::Forms::Button());
            this->Modify_button = (gcnew System::Windows::Forms::Button());
            this->Delete_button = (gcnew System::Windows::Forms::Button());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->Org_treeView = (gcnew System::Windows::Forms::TreeView());
            this->Save_button = (gcnew System::Windows::Forms::Button());
            this->groupBox1->SuspendLayout();
            this->SuspendLayout();
            // 
            // Strategy_listView
            // 
            this->Strategy_listView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->columnHeader1, 
                this->columnHeader2, this->columnHeader3});
            this->Strategy_listView->FullRowSelect = true;
            this->Strategy_listView->GridLines = true;
            this->Strategy_listView->HideSelection = false;
            this->Strategy_listView->Location = System::Drawing::Point(-1, -2);
            this->Strategy_listView->Name = L"Strategy_listView";
            this->Strategy_listView->Size = System::Drawing::Size(342, 211);
            this->Strategy_listView->TabIndex = 0;
            this->Strategy_listView->UseCompatibleStateImageBehavior = false;
            this->Strategy_listView->View = System::Windows::Forms::View::Details;
            this->Strategy_listView->ItemSelectionChanged += gcnew System::Windows::Forms::ListViewItemSelectionChangedEventHandler(this, &TradeStrategyMgrForm::Strategy_listView_ItemSelectionChanged);
            // 
            // columnHeader1
            // 
            this->columnHeader1->Text = L"策略名称";
            this->columnHeader1->Width = 90;
            // 
            // columnHeader2
            // 
            this->columnHeader2->Text = L"频率";
            this->columnHeader2->Width = 48;
            // 
            // columnHeader3
            // 
            this->columnHeader3->Text = L"策略文件绝对路径";
            this->columnHeader3->Width = 200;
            // 
            // Add_button
            // 
            this->Add_button->Location = System::Drawing::Point(43, 223);
            this->Add_button->Name = L"Add_button";
            this->Add_button->Size = System::Drawing::Size(75, 23);
            this->Add_button->TabIndex = 1;
            this->Add_button->Text = L"新增";
            this->Add_button->UseVisualStyleBackColor = true;
            this->Add_button->Click += gcnew System::EventHandler(this, &TradeStrategyMgrForm::Add_button_Click);
            // 
            // Modify_button
            // 
            this->Modify_button->Location = System::Drawing::Point(124, 223);
            this->Modify_button->Name = L"Modify_button";
            this->Modify_button->Size = System::Drawing::Size(75, 23);
            this->Modify_button->TabIndex = 2;
            this->Modify_button->Text = L"修改";
            this->Modify_button->UseVisualStyleBackColor = true;
            this->Modify_button->Click += gcnew System::EventHandler(this, &TradeStrategyMgrForm::Modify_button_Click);
            // 
            // Delete_button
            // 
            this->Delete_button->Location = System::Drawing::Point(205, 223);
            this->Delete_button->Name = L"Delete_button";
            this->Delete_button->Size = System::Drawing::Size(75, 23);
            this->Delete_button->TabIndex = 3;
            this->Delete_button->Text = L"删除";
            this->Delete_button->UseVisualStyleBackColor = true;
            this->Delete_button->Click += gcnew System::EventHandler(this, &TradeStrategyMgrForm::Delete_button_Click);
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->Org_treeView);
            this->groupBox1->Location = System::Drawing::Point(357, 3);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(220, 206);
            this->groupBox1->TabIndex = 4;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"关联部门";
            // 
            // Org_treeView
            // 
            this->Org_treeView->CheckBoxes = true;
            this->Org_treeView->Dock = System::Windows::Forms::DockStyle::Fill;
            this->Org_treeView->Location = System::Drawing::Point(3, 17);
            this->Org_treeView->Name = L"Org_treeView";
            this->Org_treeView->Size = System::Drawing::Size(214, 186);
            this->Org_treeView->TabIndex = 4;
            // 
            // Save_button
            // 
            this->Save_button->Location = System::Drawing::Point(432, 223);
            this->Save_button->Name = L"Save_button";
            this->Save_button->Size = System::Drawing::Size(75, 23);
            this->Save_button->TabIndex = 5;
            this->Save_button->Text = L"保存";
            this->Save_button->UseVisualStyleBackColor = true;
            this->Save_button->Click += gcnew System::EventHandler(this, &TradeStrategyMgrForm::Save_button_Click);
            // 
            // TradeStrategyMgrForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(589, 257);
            this->Controls->Add(this->Save_button);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->Delete_button);
            this->Controls->Add(this->Modify_button);
            this->Controls->Add(this->Add_button);
            this->Controls->Add(this->Strategy_listView);
            this->Name = L"TradeStrategyMgrForm";
            this->Text = L"交易策略管理";
            this->Load += gcnew System::EventHandler(this, &TradeStrategyMgrForm::TradeStrategyMgrForm_Load);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &TradeStrategyMgrForm::TradeStrategyMgrForm_FormClosed);
            this->groupBox1->ResumeLayout(false);
            this->ResumeLayout(false);

        }
#pragma endregion
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
                      case Cmd_AddTradeStrategy_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pCurTradeStrategy->nID = *(int*)((char*)(p+sizeof(Stru_UniPkgHead)));
                                  m_pDataInfo->SetTradeStrategy(m_pCurTradeStrategy);
                                  
                                  //UI
                                  char temp[20] = {0};
                                  ::itoa(m_pCurTradeStrategy->nFrequency,temp,10);
                                  
                                  int nCount = Strategy_listView->Items->Count;
                                  ListViewItem^ item = gcnew ListViewItem( Tools::string2String(m_pCurTradeStrategy->szName),nCount);
                                  item->Tag = m_pCurTradeStrategy->nID;
                                  item->SubItems->Add( Tools::string2String(temp) );
                                  item->SubItems->Add( Tools::string2String(m_pCurTradeStrategy->szAddr) );                                  
                                  Strategy_listView->Items->Add( item );

                                  strError = "增加交易策略操作成功";
                              }
                              else
                              {
                                  strError = "增加交易策略操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          }
                      case Cmd_DeleteTradeStrategy_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int* pStrategyID = (int*)((char*)(p+sizeof(Stru_UniPkgHead)));
                                  m_pDataInfo->DeleteTradeStrategy(*pStrategyID);

                                  //UI
                                  Strategy_listView->Items->Remove(Strategy_listView->SelectedItems[0]);

                                  strError = "删除交易策略操作成功";
                              }
                              else
                              {
                                  strError = "删除交易策略操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          }
                      case Cmd_ModifyTradeStrategy_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int* pStrategyID = (int*)((char*)(p+sizeof(Stru_UniPkgHead)));
                                  m_pDataInfo->SetTradeStrategy(m_pCurTradeStrategy);
                                  
                                  //UI
                                  char temp[20] = {0};
                                  ::itoa(m_pCurTradeStrategy->nFrequency,temp,10);

                                  Strategy_listView->SelectedItems[0]->SubItems[0]->Text = Tools::string2String(m_pCurTradeStrategy->szName);
                                  Strategy_listView->SelectedItems[0]->SubItems[1]->Text = Tools::string2String(temp);
                                  Strategy_listView->SelectedItems[0]->SubItems[2]->Text = Tools::string2String(m_pCurTradeStrategy->szAddr);

                                  strError = "修改交易策略操作成功";
                              }
                              else
                              {
                                  strError = "修改交易策略操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          }
                      case Cmd_QueryTradeStrategy_Rsp:
                          {
                              bShowMsg = false;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int nCount = head.len / sizeof(TradeStrategy);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      TradeStrategy* pTradeStrategy = (TradeStrategy*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(TradeStrategy));
                                      m_pDataInfo->SetTradeStrategy(pTradeStrategy);									
                                  }

                                  //加载交易策略
                                  if(nCount>0)
                                    LoadTradeStrategy();
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          }
                      case Cmd_SaveStrategyOrgRelation_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )  
                              {                                  
                                  int nStrategyID = *(int*)((char*)(p+sizeof(Stru_UniPkgHead)));

                                  std::vector<int> vec;
                                  for each (TreeNode^ CNode in Org_treeView->Nodes)    
                                      GetSelectedOrganization(CNode, vec);

                                  m_pDataInfo->SetStrategyOrgRelation(nStrategyID, vec);

                                  strError = "保存交易策略和组织机构关系操作成功";
                              }
                              else
                              {
                                  strError = "保存交易策略和组织机构关系操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
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

                                  //加载部门
                                  LoadAssertMgmtOrgan();
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_QueryStrategyOrgRelation_Rsp:
                          {
                              bShowMsg = false;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pDataInfo->ClearStrategyOrgRelation();

                                  std::map<int,std::vector<int>> mapStrategyID2OrgID;
                                  int nCount = head.len / sizeof(AssetOrgStrategyRelation);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      AssetOrgStrategyRelation* pOrgStrategyRelation = (AssetOrgStrategyRelation*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(AssetOrgStrategyRelation));
                                      mapStrategyID2OrgID[pOrgStrategyRelation->nStrategyID].push_back(pOrgStrategyRelation->nAssetOrgID);
                                  }

                                  m_pDataInfo->SetStrategyOrgRelation(mapStrategyID2OrgID);

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
    private: System::Void TradeStrategyMgrForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 this->CenterToScreen();
                 
                 m_pDataInfo->ClearTradeStrategy();
                 m_pDataInfo->ClearStrategyOrgRelation();

                 int nHwnd = this->Handle.ToInt32();

                 m_pDataInfo->GetLogonInfo(*m_pLogin);
                 //订阅信息                                
                 CTcpLayer::SubscribePkg(Cmd_AddTradeStrategy_Rsp, nHwnd);                
                 CTcpLayer::SubscribePkg(Cmd_DeleteTradeStrategy_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_ModifyTradeStrategy_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_SaveStrategyOrgRelation_Rsp, nHwnd);

                 //组织机构    
                 CTcpLayer::SubscribePkg(Cmd_QueryOrg_Rsp, nHwnd);
                 CTcpLayer::SendData(Cmd_QueryOrg_Req, NULL,0, 0, m_pLogin->mnUserID); 

                 //查询交易策略 
                 CTcpLayer::SubscribePkg(Cmd_QueryTradeStrategy_Rsp, nHwnd);
                 CTcpLayer::SendData(Cmd_QueryTradeStrategy_Req, NULL,0, 0, m_pLogin->mnUserID);

                 //查询交易策略与组织机构关系
                 CTcpLayer::SubscribePkg(Cmd_QueryStrategyOrgRelation_Rsp, nHwnd);
                 CTcpLayer::SendData(Cmd_QueryStrategyOrgRelation_Req, NULL,0, 0, m_pLogin->mnUserID); 

                 //用户与组织机构关系
                 CTcpLayer::SubscribePkg(Cmd_QryUserOrgRelation_Rsp, nHwnd);
                 CTcpLayer::SendData(Cmd_QryUserOrgRelation_Req, NULL,0, 0, m_pLogin->mnUserID);
             }
    private: System::Void Add_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_STRATEGY_ADD));
                 TradeStrategyForm^ dlg = gcnew TradeStrategyForm();
                 dlg->SetOperType(Oper_Add);
                 if (::DialogResult::OK == dlg->ShowDialog(this))
                 {
                     *m_pCurTradeStrategy = *(dlg->GetCurTradeStrategy());             
                     CTcpLayer::SendData(Cmd_AddTradeStrategy_Req, m_pCurTradeStrategy, sizeof(TradeStrategy), 0, m_pLogin->mnUserID);
                 }
             }
    private: System::Void Modify_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_STRATEGY_MODIFY));
                 if(Strategy_listView->SelectedIndices != nullptr && Strategy_listView->SelectedIndices->Count>0)
                 {
                     String^ strName = Strategy_listView->SelectedItems[0]->SubItems[0]->Text;
                     int nStrategyID = m_pDataInfo->GetTradeStrategyIDByName(Tools::String2string(strName));
                     if(!m_pDataInfo->GetTradeStrategyByID(nStrategyID,*m_pCurTradeStrategy))
                         return;

                     TradeStrategyForm^ dlg = gcnew TradeStrategyForm();
                     dlg->SetOperType(Oper_Modify);
                     dlg->SetCurTradeStrategy(*m_pCurTradeStrategy);

                     if (System::Windows::Forms::DialogResult::OK == dlg->ShowDialog(this))
                     {
                         *m_pCurTradeStrategy = *(dlg->GetCurTradeStrategy());     
                         CTcpLayer::SendData(Cmd_ModifyTradeStrategy_Req, m_pCurTradeStrategy, sizeof(TradeStrategy), 0, m_pLogin->mnUserID);
                     }
                 }
                 else
                     MessageBox::Show("请选择内容");
             }
    private: System::Void Delete_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_STRATEGY_DEL));
                 if(Strategy_listView->SelectedIndices != nullptr && Strategy_listView->SelectedIndices->Count>0)
                 {
                     if(::DialogResult::Cancel ==  MessageBox::Show("此操作将会同步删除交易策略和部门之间的关系。确认吗？", "提示", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
                         return;

                     String^ strName = Strategy_listView->SelectedItems[0]->SubItems[0]->Text;
                     int nStrategyID = m_pDataInfo->GetTradeStrategyIDByName(Tools::String2string(strName));

                     CTcpLayer::SendData(Cmd_DeleteTradeStrategy_Req,&nStrategyID,sizeof(int),0, m_pLogin->mnUserID);
                 }
                 else
                     MessageBox::Show("请选择内容");
             }
    private: System::Void Save_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_STRATEGY_SAVE));
                 if(Strategy_listView->SelectedIndices != nullptr && Strategy_listView->SelectedIndices->Count>0)
                 {
                     ListView::SelectedIndexCollection^ c = Strategy_listView->SelectedIndices;
                     ListViewItem^ item = Strategy_listView->Items[c[0]];
                     int nStrategyID = Convert::ToInt32(item->Tag);
                     if ( nStrategyID <= 0 )
                     {
                         MessageBox::Show("请先选择一条记录！", "提示",MessageBoxButtons::OK);
                     }
                     else
                     {
                         std::vector<int> vValue;
                         //遍历树
                         for each (TreeNode^ CNode in Org_treeView->Nodes)    
                             GetSelectedOrganization(CNode, vValue);

                         int nDataLen = sizeof(int)+sizeof(int)*vValue.size();
                         char* pBuffer = new char[nDataLen];
                         memset(pBuffer, 0, nDataLen);
                         memcpy(pBuffer, &nStrategyID, sizeof(int));
                         if ( !vValue.empty() )
                         {
                             memcpy(pBuffer+sizeof(int), &vValue[0], sizeof(int)*vValue.size());
                         }

                         CTcpLayer::SendData(Cmd_SaveStrategyOrgRelation_Req, pBuffer, nDataLen, 0, m_pLogin->mnUserID);

                         delete [] pBuffer;
                     }
                 }
                 else
                 {
                     MessageBox::Show("请先选择一条记录！", "提示",MessageBoxButtons::OK);
                 }
             }

    private: System::Void Strategy_listView_ItemSelectionChanged(System::Object^  sender, System::Windows::Forms::ListViewItemSelectionChangedEventArgs^  e) {
                 if(Strategy_listView->SelectedIndices != nullptr && Strategy_listView->SelectedIndices->Count>0)
                 {
                     ListView::SelectedIndexCollection^ c = Strategy_listView->SelectedIndices;
                     String^ strName = Strategy_listView->Items[c[0]]->Text;

                     TradeStrategy oTradeStrategy;
                     if(m_pDataInfo->GetTradeStrategyByName(oTradeStrategy,Tools::String2string(strName)))
                         LoadOrgByStrategyID(oTradeStrategy.nID);
                 } 
                 else
                 {
                     LoadAssertMgmtOrgan();
                 }

             }
private: void LoadTradeStrategy()
         {
             if(m_pDataInfo == NULL)
                 return;
             
             std::map<int, TradeStrategy>    mapTradeStrategy;
             m_pDataInfo->GetTradeStrategy(mapTradeStrategy);

             std::map<int, TradeStrategy>::iterator it=mapTradeStrategy.begin();
             for (int i=0;it!=mapTradeStrategy.end();it++)
             {
                 ListViewItem^ item = gcnew ListViewItem(Tools::string2String(it->second.szName),i++); 
                 item->Tag = it->first;
                 int^ nFreID = it->second.nFrequency;
                 item->SubItems->Add(nFreID->ToString());  
                 item->SubItems->Add(Tools::string2String(it->second.szAddr));
                 Strategy_listView->Items->Add( item );
             }  
         }

private: void LoasSubAssertMgmtOrgan(TreeNode^ nodeMain,std::map<int, std::map<int, Organization>>& mapMain,std::map<int, Organization>& mapSub) 
         {
             std::map<int, std::map<int, Organization>>::iterator it;
             std::map<int, Organization>::iterator itSub = mapSub.begin();;

             TreeNode^ subNode;
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
private: void LoadAssertMgmtOrgan() 
         {
             Organization assertOrg;
             memset(&assertOrg, 0, sizeof(assertOrg));
             if(!m_pDataInfo->GetOrganizationByID(ORGNIZATION_ROOT, assertOrg))
                 return;     

             std::map<int, std::map<int, Organization>> mapAssetMgmtOrgTree;
             m_pDataInfo->GetOrganization(mapAssetMgmtOrgTree);
             std::map<int, std::map<int, Organization>>::iterator it = mapAssetMgmtOrgTree.find(ORGNIZATION_ROOT);
             if(it == mapAssetMgmtOrgTree.end())
                 return;

             int^ nID = assertOrg.nAssetMgmtOrgID;
             String^ strText = nID->ToString();
             String^ strValue = Tools::string2String(assertOrg.szName);

             Org_treeView->Nodes->Clear();
             
             TreeNode^ node;
             node = Org_treeView->Nodes->Add(strText, strValue);
             node->Tag = assertOrg.nAssetMgmtOrgID;

             std::map<int, Organization>& mapSub = it->second;
             if(mapSub.begin() == mapSub.end())
                 return;

             LoasSubAssertMgmtOrgan(node, mapAssetMgmtOrgTree, mapSub);

             node->Expand();
         }
private: void LoadOrgByStrategyID(int nStrategyID)
         {
             if(nStrategyID == -1 || m_pDataInfo == NULL || m_pDataInfo == NULL)
                 return;

             std::vector<AssetOrgStrategyRelation> vecOrgStrategyRelation;
             m_pDataInfo->GetStrategyOrgRelation(vecOrgStrategyRelation);
             if(vecOrgStrategyRelation.empty())
                 return;

             std::vector<AssetOrgStrategyRelation>::iterator it = vecOrgStrategyRelation.begin();
             for (int i=0;it!=vecOrgStrategyRelation.end();it++)
             {
                 if (nStrategyID == it->nStrategyID)
                 {
                     int nOrgID = it->nAssetOrgID;
                     for each (TreeNode^ CNode in Org_treeView->Nodes)                         
                        SetSelectCheck(nOrgID,CNode);
                 }
             }
         }
private: void SetSelectCheck(int OrgID,TreeNode^ node)
         {
             if (true)
             {
                 String^ strName = node->Text; 
                 int nOrgID = m_pDataInfo->GetOrganizationIDByName(Tools::String2string(strName));

                 if(nOrgID == OrgID)
                     node->Checked = true;                 
             }

             if (node->Nodes->Count > 0)  //如果节点有子节点
                 for each(TreeNode^ childNode in node->Nodes) 
                     SetSelectCheck(OrgID,childNode);// 递归
         }
private: void GetSelectedOrganization(TreeNode^ node, std::vector<int>& vec)
             {
                 if (node->Checked) 
                     vec.push_back(::Convert::ToInt32(node->Tag));

                 for each(TreeNode^ childNode in node->Nodes)                                  
                     GetSelectedOrganization(childNode, vec);// 递归
             }

private: System::Void TradeStrategyMgrForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
             int nHwnd = this->Handle.ToInt32();
             CTcpLayer::UnsubscribeAllPkg(nHwnd);
         }
};
}

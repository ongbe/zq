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
#include "TradeAccountForm.h"
#include "CommonUserOpr.h"
#include "TradeAccountSetOrgForm.h"

namespace CltServerMgr {

	/// <summary>
	/// Summary for TradeAccountMgrForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class TradeAccountMgrForm : public System::Windows::Forms::Form
	{
	public:
		TradeAccountMgrForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pLogin = new sLoginRsp();
            m_pCurTradeAccount = new TradeAccount;
            m_TradeAccountAndOrgRelation = new TradeAccountAndOrg;
            m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TradeAccountMgrForm()
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
            if (m_pCurTradeAccount)
            {
                delete m_pCurTradeAccount;
                m_pCurTradeAccount = NULL;
            }
            if (m_TradeAccountAndOrgRelation)
            {
                delete m_TradeAccountAndOrgRelation;
                m_TradeAccountAndOrgRelation = NULL;
            }
		}

    private: sLoginRsp *m_pLogin;
    private: System::Windows::Forms::Button^  Add_button;
    private: System::Windows::Forms::Button^  Modify_button;
    private: System::Windows::Forms::Button^  Delete_button;
    private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


    private: CDataInfo* m_pDataInfo;
    private: TradeAccount* m_pCurTradeAccount;
    private: TradeAccountAndOrg* m_TradeAccountAndOrgRelation;


    private: System::Windows::Forms::ListView^  TradeAccount_listView;
    private: System::Windows::Forms::ColumnHeader^  columnHeader10;
    private: System::Windows::Forms::ColumnHeader^  columnHeader11;
    private: System::Windows::Forms::ColumnHeader^  columnHeader12;
//     private: System::Windows::Forms::ColumnHeader^  columnHeader13;
    private: System::Windows::Forms::ColumnHeader^  columnHeader14;
    private: System::Windows::Forms::ColumnHeader^  columnHeader15;
    private: System::Windows::Forms::ColumnHeader^  columnHeader16;
    private: System::Windows::Forms::ColumnHeader^  columnHeader17;
    private: System::Windows::Forms::Button^  button_Login;
    private: System::Windows::Forms::ColumnHeader^  columnHeader19;

    private: System::Windows::Forms::Button^  button_LoginStatusQuery;
    private: System::Windows::Forms::ColumnHeader^  columnHeader20;
    private: System::Windows::Forms::Button^  button_Org;

    private: System::Windows::Forms::ColumnHeader^  columnHeader18;

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
            this->TradeAccount_listView = (gcnew System::Windows::Forms::ListView());
            this->columnHeader10 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader11 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader12 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader14 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader15 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader16 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader17 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader18 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader19 = (gcnew System::Windows::Forms::ColumnHeader());
            this->button_Login = (gcnew System::Windows::Forms::Button());
            this->button_LoginStatusQuery = (gcnew System::Windows::Forms::Button());
            this->columnHeader20 = (gcnew System::Windows::Forms::ColumnHeader());
            this->button_Org = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // Add_button
            // 
            this->Add_button->Location = System::Drawing::Point(110, 234);
            this->Add_button->Name = L"Add_button";
            this->Add_button->Size = System::Drawing::Size(75, 23);
            this->Add_button->TabIndex = 1;
            this->Add_button->Text = L"新增";
            this->Add_button->UseVisualStyleBackColor = true;
            this->Add_button->Click += gcnew System::EventHandler(this, &TradeAccountMgrForm::Add_button_Click);
            // 
            // Modify_button
            // 
            this->Modify_button->Location = System::Drawing::Point(215, 234);
            this->Modify_button->Name = L"Modify_button";
            this->Modify_button->Size = System::Drawing::Size(75, 23);
            this->Modify_button->TabIndex = 2;
            this->Modify_button->Text = L"修改";
            this->Modify_button->UseVisualStyleBackColor = true;
            this->Modify_button->Click += gcnew System::EventHandler(this, &TradeAccountMgrForm::Modify_button_Click);
            // 
            // Delete_button
            // 
            this->Delete_button->Location = System::Drawing::Point(320, 234);
            this->Delete_button->Name = L"Delete_button";
            this->Delete_button->Size = System::Drawing::Size(75, 23);
            this->Delete_button->TabIndex = 3;
            this->Delete_button->Text = L"删除";
            this->Delete_button->UseVisualStyleBackColor = true;
            this->Delete_button->Click += gcnew System::EventHandler(this, &TradeAccountMgrForm::Delete_button_Click);
            // 
            // TradeAccount_listView
            // 
            this->TradeAccount_listView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(10) {this->columnHeader10, 
                this->columnHeader11, this->columnHeader12, this->columnHeader14, this->columnHeader15, this->columnHeader16, this->columnHeader17, 
                this->columnHeader18, this->columnHeader19, this->columnHeader20});
            this->TradeAccount_listView->FullRowSelect = true;
            this->TradeAccount_listView->GridLines = true;
            this->TradeAccount_listView->HideSelection = false;
            this->TradeAccount_listView->Location = System::Drawing::Point(0, 0);
            this->TradeAccount_listView->MultiSelect = false;
            this->TradeAccount_listView->Name = L"TradeAccount_listView";
            this->TradeAccount_listView->Size = System::Drawing::Size(869, 228);
            this->TradeAccount_listView->TabIndex = 5;
            this->TradeAccount_listView->UseCompatibleStateImageBehavior = false;
            this->TradeAccount_listView->View = System::Windows::Forms::View::Details;
            this->TradeAccount_listView->SelectedIndexChanged += gcnew System::EventHandler(this, &TradeAccountMgrForm::TradeAccount_listView_SelectedIndexChanged);
            // 
            // columnHeader10
            // 
            this->columnHeader10->Text = L"账户号";
            // 
            // columnHeader11
            // 
            this->columnHeader11->Text = L"经纪公司";
            this->columnHeader11->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader11->Width = 80;
            // 
            // columnHeader12
            // 
            this->columnHeader12->Text = L"服务器组";
            this->columnHeader12->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader12->Width = 80;
            // 
            // columnHeader14
            // 
            this->columnHeader14->Text = L"别名";
            this->columnHeader14->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader15
            // 
            this->columnHeader15->Text = L"真实姓名";
            this->columnHeader15->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader15->Width = 69;
            // 
            // columnHeader16
            // 
            this->columnHeader16->Text = L"联系方式";
            this->columnHeader16->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader16->Width = 80;
            // 
            // columnHeader17
            // 
            this->columnHeader17->Text = L"证件类型";
            this->columnHeader17->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader17->Width = 66;
            // 
            // columnHeader18
            // 
            this->columnHeader18->Text = L"证件号码";
            this->columnHeader18->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader18->Width = 80;
            // 
            // columnHeader19
            // 
            this->columnHeader19->Text = L"登录状态";
            this->columnHeader19->Width = 200;
            // 
            // button_Login
            // 
            this->button_Login->Enabled = false;
            this->button_Login->Location = System::Drawing::Point(628, 234);
            this->button_Login->Name = L"button_Login";
            this->button_Login->Size = System::Drawing::Size(75, 23);
            this->button_Login->TabIndex = 6;
            this->button_Login->Text = L"登录";
            this->button_Login->UseVisualStyleBackColor = true;
            this->button_Login->Click += gcnew System::EventHandler(this, &TradeAccountMgrForm::button_Login_Click);
            // 
            // button_LoginStatusQuery
            // 
            this->button_LoginStatusQuery->Location = System::Drawing::Point(728, 234);
            this->button_LoginStatusQuery->Name = L"button_LoginStatusQuery";
            this->button_LoginStatusQuery->Size = System::Drawing::Size(90, 23);
            this->button_LoginStatusQuery->TabIndex = 7;
            this->button_LoginStatusQuery->Text = L"登录状态查询";
            this->button_LoginStatusQuery->UseVisualStyleBackColor = true;
            this->button_LoginStatusQuery->Click += gcnew System::EventHandler(this, &TradeAccountMgrForm::button_LoginStatusQuery_Click);
            // 
            // columnHeader20
            // 
            this->columnHeader20->Text = L"组织机构";
            this->columnHeader20->Width = 80;
            // 
            // button_Org
            // 
            this->button_Org->Location = System::Drawing::Point(456, 235);
            this->button_Org->Name = L"button_Org";
            this->button_Org->Size = System::Drawing::Size(75, 23);
            this->button_Org->TabIndex = 8;
            this->button_Org->Text = L"组织机构";
            this->button_Org->UseVisualStyleBackColor = true;
            this->button_Org->Click += gcnew System::EventHandler(this, &TradeAccountMgrForm::button_Org_Click);
            // 
            // TradeAccountMgrForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(868, 266);
            this->Controls->Add(this->button_Org);
            this->Controls->Add(this->button_LoginStatusQuery);
            this->Controls->Add(this->button_Login);
            this->Controls->Add(this->TradeAccount_listView);
            this->Controls->Add(this->Delete_button);
            this->Controls->Add(this->Modify_button);
            this->Controls->Add(this->Add_button);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"TradeAccountMgrForm";
            this->Text = L"委托交易账号管理";
            this->Load += gcnew System::EventHandler(this, &TradeAccountMgrForm::TradeAccountMgrForm_Load);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &TradeAccountMgrForm::TradeAccountMgrForm_FormClosed);
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
               case Cmd_QueryBrokerInfo_Rsp:
                   {
                       bShowMsg = false;
                       if ( head.userdata1 == CF_ERROR_SUCCESS )
                       {
                           m_pDataInfo->ClearBrokerInfos();
                           std::map<int, BrokerInfo> mapBrokerInfo;
                           int nCount = head.len / sizeof(BrokerInfo);
                           for ( int i = 0; i < nCount; i++ )
                           {
                               BrokerInfo* pBrokerInfo = (BrokerInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(BrokerInfo));
                               mapBrokerInfo[pBrokerInfo->nBrokerID] = *pBrokerInfo;
                           }

                           m_pDataInfo->SetBrokerInfos(mapBrokerInfo);
                       }
                       else
                       {
                           strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                       }

                       break;
                   }
               case Cmd_QueryServerGroup_Rsp:
                   {          
                       bShowMsg = false;
                       if( head.userdata1 == CF_ERROR_SUCCESS )
                       {
                           m_pDataInfo->ClearServerGroups();

                           int nCount = head.len / sizeof(ServerGroup);
                           for(int i=0;i<nCount;i++)
                           {
                               ServerGroup* pServerGroup = (ServerGroup*)((char*)(p+sizeof(Stru_UniPkgHead))+i*sizeof(ServerGroup));
                               m_pDataInfo->SetServerGroup(pServerGroup->nBrokerID,*pServerGroup);
                           }
                       }
                       else
                       {
                           strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                       }

                       break;
                   }
			   case Cmd_QueryTradeAccount_Rsp:
				   {		
                       bShowMsg = false;
					   if ( head.userdata1 == CF_ERROR_SUCCESS )
					   {  
                           m_pDataInfo->ClearTradeAccount();
                           TradeAccount_listView->Items->Clear();

						   std::map<int, TradeAccount> mapTradeAccount;
						   int nCount = head.len / sizeof(TradeAccount);
						   for ( int i = 0; i < nCount; i++ )
						   {
							   TradeAccount* pTradeAccount = (TradeAccount*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(TradeAccount));
							   mapTradeAccount[pTradeAccount->nTradeAccountID] = *pTradeAccount;

                               String^ strBroker = nullptr;
                               String^ strServerGroup = nullptr;
                               String^ strIDCardType = nullptr;


                               strBroker = Tools::string2String(m_pDataInfo->GetBrokerNameByID(pTradeAccount->nBrokerID).c_str());

                                
                               switch((int)(pTradeAccount->nIDCardType))
                               {
                               case 0:
                                   strIDCardType = "身份证";
                                   break;
                               case 1:
                                   strIDCardType = "军官证";
                                   break;
                               case 2:
                                   strIDCardType = "教师证";
                                   break;
                               case 3:
                                   strIDCardType = "其他";
                                   break;
                               }

                               string strOrg = m_pDataInfo->GetOrgByTradeAccountID(pTradeAccount->nTradeAccountID);

                               int nCount = TradeAccount_listView->Items->Count;
                               ListViewItem^ item = gcnew ListViewItem(Tools::string2String(pTradeAccount->szTradeAccount),nCount++);
                               item->Tag = pTradeAccount->nTradeAccountID;
                               item->SubItems->Add(strBroker); 
                               std::string strGroupName = m_pDataInfo->GetServerGroupNameByID(pTradeAccount->nBrokerID,pTradeAccount->nServerGroupID);
                               item->SubItems->Add(Tools::string2String(strGroupName.c_str())); 
                               //item->SubItems->Add(pTradeAccount->dProfit.ToString()); 
                               item->SubItems->Add(Tools::string2String(pTradeAccount->szAlias)); 
                               item->SubItems->Add(Tools::string2String(pTradeAccount->szRealName)); 
                               item->SubItems->Add(Tools::string2String(pTradeAccount->szContact));
                               item->SubItems->Add(strIDCardType); 
                               item->SubItems->Add(Tools::string2String(pTradeAccount->szIDCardNo));
                               item->SubItems->Add(String::Empty);
                               item->SubItems->Add(Tools::string2String(strOrg.c_str()));
                               TradeAccount_listView->Items->Add( item );
						   }

						   m_pDataInfo->SetTradeAccount(mapTradeAccount);
					   }
					   else
					   {
						   strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
					   }

					   break;
				   }
			   case Cmd_AddTradeAccount_Rsp:
				   {
					   if ( head.userdata1 == CF_ERROR_SUCCESS )
					   {
                           m_pCurTradeAccount->nTradeAccountID = *(int*)((char*)(p+sizeof(Stru_UniPkgHead)));
                           m_pDataInfo->SetTradeAccount(m_pCurTradeAccount);

                           //UI
                           array<String^>^ strCardType = {"身份证","军官证","教师证","其他"};

                           int nCount = TradeAccount_listView->Items->Count;
                           ListViewItem^ item = gcnew ListViewItem( Tools::string2String(m_pCurTradeAccount->szTradeAccount),nCount );
                           item->Tag = m_pCurTradeAccount->nTradeAccountID;
                           item->SubItems->Add( Tools::string2String(m_pDataInfo->GetBrokerNameByID(m_pCurTradeAccount->nBrokerID).c_str()) );
                           item->SubItems->Add( Tools::string2String(m_pDataInfo->GetServerGroupNameByID(m_pCurTradeAccount->nBrokerID,m_pCurTradeAccount->nServerGroupID).c_str()) );
//                            item->SubItems->Add( ::Convert::ToString(m_pCurTradeAccount->dProfit) );
                           item->SubItems->Add( Tools::string2String(m_pCurTradeAccount->szAlias) );
                           item->SubItems->Add( Tools::string2String(m_pCurTradeAccount->szRealName) );
                           item->SubItems->Add( Tools::string2String(m_pCurTradeAccount->szContact) );
                           item->SubItems->Add( strCardType[m_pCurTradeAccount->nIDCardType] );
                           item->SubItems->Add( Tools::string2String(m_pCurTradeAccount->szIDCardNo) );
                           item->SubItems->Add( "未初始化" );
                           item->SubItems->Add(String::Empty);

                           //UI
                           TradeAccount_listView->Items->Add( item );

                           strError = "增加委托交易账号操作成功";
					   }
					   else
					   {
						   strError = "增加委托交易账号操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
					   }

					   break;
				   }
			   case Cmd_ModifyTradeAccount_Rsp:
				   {
					   if ( head.userdata1 == CF_ERROR_SUCCESS )
					   {
                           int* pStrategyID = (int*)((char*)(p+sizeof(Stru_UniPkgHead)));
                           m_pDataInfo->SetTradeAccount(m_pCurTradeAccount);

                           //UI
                           array<String^>^ strCardType = {"身份证","军官证","教师证","其他"};
                           TradeAccount_listView->SelectedItems[0]->SubItems[0]->Text = Tools::string2String(m_pCurTradeAccount->szTradeAccount);
                           TradeAccount_listView->SelectedItems[0]->SubItems[1]->Text = Tools::string2String(m_pDataInfo->GetBrokerCodeByID(m_pCurTradeAccount->nBrokerID).c_str());
                           TradeAccount_listView->SelectedItems[0]->SubItems[2]->Text = Tools::string2String(m_pDataInfo->GetServerGroupNameByID(m_pCurTradeAccount->nBrokerID,m_pCurTradeAccount->nServerGroupID).c_str());
//                            TradeAccount_listView->SelectedItems[0]->SubItems[3]->Text = ::Convert::ToString(m_pCurTradeAccount->dProfit);
                           TradeAccount_listView->SelectedItems[0]->SubItems[3]->Text = Tools::string2String(m_pCurTradeAccount->szAlias);
                           TradeAccount_listView->SelectedItems[0]->SubItems[4]->Text = Tools::string2String(m_pCurTradeAccount->szRealName);
                           TradeAccount_listView->SelectedItems[0]->SubItems[5]->Text = Tools::string2String(m_pCurTradeAccount->szContact);
                           TradeAccount_listView->SelectedItems[0]->SubItems[6]->Text = strCardType[m_pCurTradeAccount->nIDCardType];
                           TradeAccount_listView->SelectedItems[0]->SubItems[7]->Text = Tools::string2String(m_pCurTradeAccount->szIDCardNo);

                           strError = "修改委托交易账号操作成功";
					   }
					   else
					   {
						   strError = "修改委托交易账号操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
					   }

					   break;
				   }
			   case Cmd_DelTradeAccount_Rsp:
				   {
					   if ( head.userdata1 == CF_ERROR_SUCCESS )
					   {
						   int nTradeAccountID = *(int*)(p+sizeof(Stru_UniPkgHead));
						   m_pDataInfo->DelTradeAccount(nTradeAccountID);

                           //UI
                           TradeAccount_listView->Items->Remove(TradeAccount_listView->SelectedItems[0]);

                           strError = "删除委托交易账号操作成功";
					   }
					   else
					   {
						   strError = "删除委托交易账号操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
					   }

					   break;
				   }	
               case Cmd_QryUserTradeAcctRelation_Rsp:
                   {
                       bShowMsg = false;
                       if ( head.userdata1 == CF_ERROR_SUCCESS )
                       {
                           m_pDataInfo->ClearUserAndTraderAccountRelation();
                           std::map<int, UserAndTradeAccountRelation> mapUserAndTradeAccountRelation;
                           int nCount = head.len / sizeof(UserAndOrgRelation);
                           for ( int i = 0; i < nCount; i++ )
                           {
                               UserAndTradeAccountRelation* pRelation = (UserAndTradeAccountRelation*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(UserAndTradeAccountRelation));
                               mapUserAndTradeAccountRelation.insert(make_pair(pRelation->nUserID, *pRelation));                             
                           }

                           m_pDataInfo->SetUserAndTraderAccountRelation(mapUserAndTradeAccountRelation);	

                       }
                       else
                       {
                           strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                       }

                       break;
                   } 
               case Cmd_TradeDataManager_LoginAccount_Rsp:
                   {
                       if ( head.userdata3 != CF_ERROR_SUCCESS )
                       {                          
                           strError = "登录委托交易账号操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                       }
                       else
                       {
                           button_Login->Enabled = true;

                           strError = "登录委托交易账号操作成功";
                       }

                       break;
                   }

               case Cmd_TradeDataManager_QryAccountStatus_Rsp:
                   {
                       bShowMsg = false;
                       if ( head.userdata3 == CF_ERROR_SUCCESS )
                       {
                           int nCount = head.len / sizeof(sAccountStatus);
                           for ( int i = 0; i < nCount; i++ )
                           {
                               sAccountStatus accountStatus = *(sAccountStatus*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(sAccountStatus));
                               m_pDataInfo->SetAccountStatus(accountStatus);

                               int nListCount = TradeAccount_listView->Items->Count;
                               for (int j=0;j<nListCount;j++)
                               {
                                   String^ szAccount = TradeAccount_listView->Items[j]->SubItems[0]->Text->ToString();
                                   String^ szBrokerCode = TradeAccount_listView->Items[j]->SubItems[1]->Text->ToString();

                                   String^ szStatus = String::Empty;
                                   string strBrokenName = m_pDataInfo->GetBrokerNameByID(m_pDataInfo->GetBrokerIDByCode(accountStatus.mBrokerAccount.BrokerID));
                                   if(0==strcmp(Tools::String2string(szAccount).c_str(), accountStatus.mBrokerAccount.AccountID) && 
                                      0==strcmp(Tools::String2string(szBrokerCode).c_str(), strBrokenName.c_str()))
                                   {
                                       switch(accountStatus.meStatus)
                                       {
                                       case ACCOUNT_STATUS_UnInit:
                                           szStatus  = "未初始化";
                                           break;
                                       case ACCOUNT_STATUS_CannotConnect:
                                           szStatus = "连接不上";
                                           break;
                                       case ACCOUNT_STATUS_Connected:
                                           szStatus = "已连接";
                                           break;
                                       case ACCOUNT_STATUS_Disconnected:
                                           szStatus = "断开连接";
                                           break;
                                       case ACCOUNT_STATUS_Login:
                                           szStatus = "登录成功";
                                           break;
                                       case ACCOUNT_STATUS_LoginFailure:
                                           szStatus = "登录失败";
                                           break;
                                       case ACCOUNT_STATUS_TradeSettlementInfoConfirming:
                                           szStatus = "正在进行交易结算结果确认";
                                           break;
                                       }   

									   if(accountStatus.meStatus != ACCOUNT_STATUS_UnInit)
									   {
										   if(accountStatus.meCommissionAndMarginReady & CommissionMargin_ComissionReady)
											 szStatus += "(费率查询完成)";
										  if(accountStatus.meCommissionAndMarginReady & CommissionMargin_MarginReady)
											  szStatus += "(保证金率查询完成)";
									   }

                                       TradeAccount_listView->Items[j]->SubItems[8]->Text = szStatus;    
                                   }    
                               }
                           }
                       }
                       else
                       {
						   int nListCount = TradeAccount_listView->Items->Count;
						   for (int j=0;j<nListCount;j++)
						   { 
							   String^ szStatus = String::Empty;
							   szStatus = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
							   TradeAccount_listView->Items[j]->SubItems[8]->Text = szStatus;
						   }
                         
                       }

                       break;
                   }
               case Cmd_QryTradeAccountAndOrgRelation_Rsp:
                   {
                       bShowMsg = false;
                       if ( head.userdata1 == CF_ERROR_SUCCESS )
                       {
                           m_pDataInfo->ClearTradeAccountAndOrgRelation();
                           std::map<int, TradeAccountAndOrg> mapTradeAccountAndOrgRelation;
                           int nCount = head.len / sizeof(TradeAccountAndOrg);
                           for ( int i = 0; i < nCount; i++ )
                           {
                               TradeAccountAndOrg* pRelation = (TradeAccountAndOrg*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(TradeAccountAndOrg));
                               mapTradeAccountAndOrgRelation.insert(make_pair(pRelation->nTradeAccountID, *pRelation));                             
                           }

                           m_pDataInfo->SetTradeAccountAndOrgRelation(mapTradeAccountAndOrgRelation);	

                       }
                       else
                       {
                           strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                       }

                       break;
                   }
               case Cmd_SaveTradeAccountAndOrgRelation_Rsp:
                   {
                       if ( head.userdata1 == CF_ERROR_SUCCESS )
                       {  
                           string strAccount = m_pDataInfo->GetTradeAccountByID(m_TradeAccountAndOrgRelation->nTradeAccountID);
                           string strOrg = m_pDataInfo->GetOrganizationNameByID(m_TradeAccountAndOrgRelation->nOrgID);

                           int nListCount = TradeAccount_listView->Items->Count;
                           for (int j=0;j<nListCount;j++)
                           {
                               String^ szAccount = TradeAccount_listView->Items[j]->SubItems[0]->Text->ToString();
                               
                               if(0==strcmp(Tools::String2string(szAccount).c_str(), strAccount.c_str()))
                                   TradeAccount_listView->Items[j]->SubItems[9]->Text = Tools::string2String(strOrg.c_str());
                           }

                           m_pDataInfo->SetTradeAccountAndOrgRelation(*m_TradeAccountAndOrgRelation);
                           strError = "保存委托交易账号与组织机构关系关系操作成功";
                       }
                       else
                       {
                           strError = "保存委托交易账号与组织机构操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
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
private: System::Void TradeAccountMgrForm_Load(System::Object^  sender, System::EventArgs^  e) {
             this->CenterToScreen();

             int nHwnd = this->Handle.ToInt32();

             CTcpLayer::SubscribePkg(Cmd_AddTradeAccount_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_ModifyTradeAccount_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_DelTradeAccount_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_TradeDataManager_LoginAccount_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_SaveTradeAccountAndOrgRelation_Rsp, nHwnd);
              

             m_pDataInfo->GetLogonInfo(*m_pLogin);

             //委托交易账号与组织机构关系
             CTcpLayer::SubscribePkg(Cmd_QryTradeAccountAndOrgRelation_Rsp, nHwnd); 
             CTcpLayer::SendData(Cmd_QryTradeAccountAndOrgRelation_Req, NULL, 0, 0, m_pLogin->mnUserID); 

             //经纪公司
             CTcpLayer::SubscribePkg(Cmd_QueryBrokerInfo_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_QueryBrokerInfo_Req, NULL,0, 0, m_pLogin->mnUserID);

             //服务器组
             CTcpLayer::SubscribePkg(Cmd_QueryServerGroup_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_QueryServerGroup_Req, NULL, 0, 0, m_pLogin->mnUserID);
             
             //委托交易账号
             CTcpLayer::SubscribePkg(Cmd_QueryTradeAccount_Rsp, nHwnd); 
             CTcpLayer::SendData(Cmd_QueryTradeAccount_Req, NULL, 0, 0, m_pLogin->mnUserID);   

             //委托交易账号与交易员关系
             CTcpLayer::SubscribePkg(Cmd_QryUserTradeAcctRelation_Rsp, nHwnd); 
             CTcpLayer::SendData(Cmd_QryUserTradeAcctRelation_Req, NULL, 0, 0, m_pLogin->mnUserID);   

             //登录状态
             CTcpLayer::SubscribePkg(Cmd_TradeDataManager_QryAccountStatus_Rsp, nHwnd);
             CTcpLayer::SendData(Cmd_TradeDataManager_QryAccountStatus_Req, NULL, 0, 0, m_pLogin->mnUserID);  
         }
private: System::Void Add_button_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_ACCOUNT_ADD));
             TradeAccountForm^ dlg = gcnew TradeAccountForm();
             dlg->SetOperType(Oper_Add);
             if (::DialogResult::OK == dlg->ShowDialog(this))
             {
                 *m_pCurTradeAccount = *(dlg->GetCurTradeAccount());             
                 CTcpLayer::SendData(Cmd_AddTradeAccount_Req, m_pCurTradeAccount, sizeof(TradeAccount), 0, m_pLogin->mnUserID);
             }
         }
private: System::Void Modify_button_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_ACCOUNT_MODIFY));
             if(TradeAccount_listView->SelectedIndices != nullptr && TradeAccount_listView->SelectedIndices->Count>0)
             {
                 String^ strName = TradeAccount_listView->SelectedItems[0]->SubItems[0]->Text;
                 if(!m_pDataInfo->GetTradeAccountByAccount(Tools::String2string(strName),*m_pCurTradeAccount))
                     return;

                 if(m_pDataInfo->GetAccountStatus(Tools::String2string(strName)) == ACCOUNT_STATUS_Login)
                 {
                     MessageBox::Show("用户已登录，无法修改！", "提示",MessageBoxButtons::OK);
                     return;
                 }

                 TradeAccountForm^ dlg = gcnew TradeAccountForm();
                 dlg->SetOperType(Oper_Modify);
                 dlg->SetCurTradeAccount(*m_pCurTradeAccount);
                 if (::DialogResult::OK == dlg->ShowDialog(this))
                 {
                     *m_pCurTradeAccount = *(dlg->GetCurTradeAccount());     
                     CTcpLayer::SendData(Cmd_ModifyTradeAccount_Req, m_pCurTradeAccount, sizeof(TradeAccount), 0, m_pLogin->mnUserID);
                 }
             }
             else
                 MessageBox::Show("请选择内容");
         }
private: System::Void Delete_button_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_ACCOUNT_DEL));
             if(::DialogResult::Cancel ==  MessageBox::Show("此操作将会会同步删除和经纪公司和服务器组的关系。确认吗？", "提示", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
                 return;


             if(TradeAccount_listView->SelectedIndices != nullptr && TradeAccount_listView->SelectedIndices->Count>0)
             {
                 String^ strName = TradeAccount_listView->SelectedItems[0]->SubItems[0]->Text;
                 int nAccountID = m_pDataInfo->GetTradeAccountIDByAccount(Tools::String2string(strName));

                 CTcpLayer::SendData(Cmd_DelTradeAccount_Req,&nAccountID,sizeof(int),0, m_pLogin->mnUserID);
             }
             else
                 MessageBox::Show("请选择内容");             
         }

private: System::Void TradeAccountMgrForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
             int nHwnd = this->Handle.ToInt32();
             CTcpLayer::UnsubscribeAllPkg(nHwnd);
         }
private: System::Void button_Login_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_ACCOUNT_LOGIN));          
             if(TradeAccount_listView->SelectedIndices != nullptr && TradeAccount_listView->SelectedIndices->Count>0)
             {    
                 button_Login->Enabled = false;
                 int nTradeAccountID = (int)(TradeAccount_listView->SelectedItems[0]->Tag);
                 CTcpLayer::SendData(Cmd_TradeDataManager_LoginAccount_Req, &nTradeAccountID, sizeof(int), 0, m_pLogin->mnUserID);
             }
          }
private: System::Void TradeAccount_listView_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             if(TradeAccount_listView->SelectedIndices != nullptr && TradeAccount_listView->SelectedIndices->Count>0)
             {
                 String^ strName = TradeAccount_listView->SelectedItems[0]->SubItems[0]->Text;
                 if(!m_pDataInfo->GetTradeAccountByAccount(Tools::String2string(strName),*m_pCurTradeAccount))
                     return;

                 if(m_pDataInfo->GetAccountStatus(Tools::String2string(strName)) == ACCOUNT_STATUS_Login)
                 {
                     button_Login->Enabled = false;
                     Delete_button->Enabled = false;
                     Modify_button->Enabled = false;

                     return;
                 }

                 button_Login->Enabled = true;
                 Delete_button->Enabled = true;
                 Modify_button->Enabled = true;
             }           
         }
private: System::Void button_LoginStatusQuery_Click(System::Object^  sender, System::EventArgs^  e) {
             CTcpLayer::SendData(Cmd_TradeDataManager_QryAccountStatus_Req, NULL, 0, 0, m_pLogin->mnUserID);
         }
private: System::Void button_Org_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_ACCOUNT_MODIFY));
             if(TradeAccount_listView->SelectedIndices != nullptr && TradeAccount_listView->SelectedIndices->Count>0)
             {
                 if(!m_pCurTradeAccount)
                     return;

                 TradeAccountSetOrgForm^ dlg = gcnew TradeAccountSetOrgForm;
                 dlg->SetTradeAccountInfo(*m_pCurTradeAccount);

                 if (::DialogResult::OK == dlg->ShowDialog(this))
                 {                     
                     dlg->GetTradeAccountAndOrgRelation(*m_TradeAccountAndOrgRelation);  
                     if(m_TradeAccountAndOrgRelation->nOrgID == m_TradeAccountAndOrgRelation->nTradeAccountID && m_TradeAccountAndOrgRelation->nOrgID==0)
                         return;

                     CTcpLayer::SendData(Cmd_SaveTradeAccountAndOrgRelation_Req, m_TradeAccountAndOrgRelation, sizeof(TradeAccountAndOrg), 0, m_pLogin->mnUserID);
                 }
             }
             else
                 MessageBox::Show("请选择内容");
         }
};
}

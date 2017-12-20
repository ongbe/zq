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
#include "FinanceProductForm.h"
#include "..\..\..\InstitutionalTrading\Server\SvrFinanceProduct\DataWrap.h"
#include "popmsgwin.h"
#include "CommonUserOpr.h"


namespace CltServerMgr {

    /// <summary>
    /// Summary for FinanceProductMgrForm
    ///
    /// WARNING: If you change the name of this class, you will need to change the
    ///          'Resource File Name' property for the managed resource compiler tool
    ///          associated with all .resx files this class depends on.  Otherwise,
    ///          the designers will not be able to interact properly with localized
    ///          resources associated with this form.
    /// </summary>
    public ref class FinanceProductMgrForm : public System::Windows::Forms::Form
    {
    public:
        FinanceProductMgrForm(void)
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //
            m_pLogin = new sLoginRsp();
            m_pDataInfo = CDataInfo::NewInstance();
            m_pCurFinancialProduct = new FinancialProduct;
            
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~FinanceProductMgrForm()
        {
            if (components)
            {
                delete components;
            }

            if(m_pLogin)
            {
                delete m_pLogin;
                m_pLogin = NULL;
            }

            if (m_pCurFinancialProduct)
            {
                delete m_pCurFinancialProduct;
                m_pCurFinancialProduct = NULL;
            }
            if (m_pCurFinancialProduct)
            {
                delete m_pCurFinancialProduct;
                m_pCurFinancialProduct = NULL;
            }
        }

    private: sLoginRsp*     m_pLogin;
    private: String^        mProductOrg;
    private: CDataInfo*     m_pDataInfo;
    private: FinancialProduct* m_pCurFinancialProduct;
    private: FinanceProductForm^ m_FinanceProductForm;


    private: System::Windows::Forms::ListView^  FProduct_listView;
    private: System::Windows::Forms::ColumnHeader^  columnHeader1;
    private: System::Windows::Forms::ColumnHeader^  columnHeader2;
    private: System::Windows::Forms::GroupBox^  groupBox1;
    private: System::Windows::Forms::Button^  add_button;
    private: System::Windows::Forms::Button^  modify_button;
    private: System::Windows::Forms::Button^  delete_button;
    private: System::Windows::Forms::GroupBox^  groupBox2;
    private: System::Windows::Forms::Button^  AllLeftMove_button;
    private: System::Windows::Forms::Button^  LeftMove_button;
    private: System::Windows::Forms::Button^  RightMove_button;
    private: System::Windows::Forms::Button^  AllRightMove_button;
    private: System::Windows::Forms::ListView^  InGroup_listView;
    private: System::Windows::Forms::ColumnHeader^  columnHeader3;
    private: System::Windows::Forms::ColumnHeader^  columnHeader4;
    private: System::Windows::Forms::ListView^  OutGroup_listView;
    private: System::Windows::Forms::ColumnHeader^  columnHeader5;
    private: System::Windows::Forms::ColumnHeader^  columnHeader6;
    private: System::Windows::Forms::Button^  Save_button;





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
            this->delete_button = (gcnew System::Windows::Forms::Button());
            this->modify_button = (gcnew System::Windows::Forms::Button());
            this->add_button = (gcnew System::Windows::Forms::Button());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->InGroup_listView = (gcnew System::Windows::Forms::ListView());
            this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
            this->FProduct_listView = (gcnew System::Windows::Forms::ListView());
            this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
            this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
            this->OutGroup_listView = (gcnew System::Windows::Forms::ListView());
            this->columnHeader5 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader6 = (gcnew System::Windows::Forms::ColumnHeader());
            this->AllLeftMove_button = (gcnew System::Windows::Forms::Button());
            this->LeftMove_button = (gcnew System::Windows::Forms::Button());
            this->RightMove_button = (gcnew System::Windows::Forms::Button());
            this->AllRightMove_button = (gcnew System::Windows::Forms::Button());
            this->Save_button = (gcnew System::Windows::Forms::Button());
            this->groupBox1->SuspendLayout();
            this->groupBox2->SuspendLayout();
            this->SuspendLayout();
            // 
            // delete_button
            // 
            this->delete_button->Location = System::Drawing::Point(174, 278);
            this->delete_button->Name = L"delete_button";
            this->delete_button->Size = System::Drawing::Size(75, 23);
            this->delete_button->TabIndex = 11;
            this->delete_button->Text = L"删除";
            this->delete_button->UseVisualStyleBackColor = true;
            this->delete_button->Click += gcnew System::EventHandler(this, &FinanceProductMgrForm::delete_button_Click);
            // 
            // modify_button
            // 
            this->modify_button->Location = System::Drawing::Point(93, 278);
            this->modify_button->Name = L"modify_button";
            this->modify_button->Size = System::Drawing::Size(75, 23);
            this->modify_button->TabIndex = 9;
            this->modify_button->Text = L"修改";
            this->modify_button->UseVisualStyleBackColor = true;
            this->modify_button->Click += gcnew System::EventHandler(this, &FinanceProductMgrForm::modify_button_Click);
            // 
            // add_button
            // 
            this->add_button->Location = System::Drawing::Point(12, 278);
            this->add_button->Name = L"add_button";
            this->add_button->Size = System::Drawing::Size(75, 23);
            this->add_button->TabIndex = 8;
            this->add_button->Text = L"新增";
            this->add_button->UseVisualStyleBackColor = true;
            this->add_button->Click += gcnew System::EventHandler(this, &FinanceProductMgrForm::add_button_Click);
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->InGroup_listView);
            this->groupBox1->Location = System::Drawing::Point(288, 2);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(203, 267);
            this->groupBox1->TabIndex = 7;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"组内交易员列表";
            // 
            // InGroup_listView
            // 
            this->InGroup_listView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->columnHeader3, 
                this->columnHeader4});
            this->InGroup_listView->Dock = System::Windows::Forms::DockStyle::Fill;
            this->InGroup_listView->FullRowSelect = true;
            this->InGroup_listView->GridLines = true;
            this->InGroup_listView->HideSelection = false;
            this->InGroup_listView->Location = System::Drawing::Point(3, 17);
            this->InGroup_listView->Name = L"InGroup_listView";
            this->InGroup_listView->Size = System::Drawing::Size(197, 247);
            this->InGroup_listView->TabIndex = 0;
            this->InGroup_listView->UseCompatibleStateImageBehavior = false;
            this->InGroup_listView->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader3
            // 
            this->columnHeader3->Text = L"账户号";
            this->columnHeader3->Width = 80;
            // 
            // columnHeader4
            // 
            this->columnHeader4->Text = L"用户名";
            this->columnHeader4->Width = 100;
            // 
            // columnHeader2
            // 
            this->columnHeader2->Text = L"描述";
            this->columnHeader2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader2->Width = 140;
            // 
            // FProduct_listView
            // 
            this->FProduct_listView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->columnHeader1, 
                this->columnHeader2});
            this->FProduct_listView->FullRowSelect = true;
            this->FProduct_listView->GridLines = true;
            this->FProduct_listView->HideSelection = false;
            this->FProduct_listView->Location = System::Drawing::Point(1, 2);
            this->FProduct_listView->Name = L"FProduct_listView";
            this->FProduct_listView->Size = System::Drawing::Size(264, 267);
            this->FProduct_listView->TabIndex = 6;
            this->FProduct_listView->UseCompatibleStateImageBehavior = false;
            this->FProduct_listView->View = System::Windows::Forms::View::Details;
            this->FProduct_listView->ItemSelectionChanged += gcnew System::Windows::Forms::ListViewItemSelectionChangedEventHandler(this, &FinanceProductMgrForm::FProduct_listView_ItemSelectionChanged);
            // 
            // columnHeader1
            // 
            this->columnHeader1->Text = L"名称";
            this->columnHeader1->Width = 93;
            // 
            // groupBox2
            // 
            this->groupBox2->Controls->Add(this->OutGroup_listView);
            this->groupBox2->Location = System::Drawing::Point(599, 2);
            this->groupBox2->Name = L"groupBox2";
            this->groupBox2->Size = System::Drawing::Size(203, 267);
            this->groupBox2->TabIndex = 12;
            this->groupBox2->TabStop = false;
            this->groupBox2->Text = L"组外交易员列表";
            // 
            // OutGroup_listView
            // 
            this->OutGroup_listView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->columnHeader5, 
                this->columnHeader6});
            this->OutGroup_listView->Dock = System::Windows::Forms::DockStyle::Fill;
            this->OutGroup_listView->FullRowSelect = true;
            this->OutGroup_listView->GridLines = true;
            this->OutGroup_listView->HideSelection = false;
            this->OutGroup_listView->Location = System::Drawing::Point(3, 17);
            this->OutGroup_listView->Name = L"OutGroup_listView";
            this->OutGroup_listView->Size = System::Drawing::Size(197, 247);
            this->OutGroup_listView->TabIndex = 0;
            this->OutGroup_listView->UseCompatibleStateImageBehavior = false;
            this->OutGroup_listView->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader5
            // 
            this->columnHeader5->Text = L"账户号";
            this->columnHeader5->Width = 80;
            // 
            // columnHeader6
            // 
            this->columnHeader6->Text = L"用户名";
            this->columnHeader6->Width = 100;
            // 
            // AllLeftMove_button
            // 
            this->AllLeftMove_button->Location = System::Drawing::Point(508, 31);
            this->AllLeftMove_button->Name = L"AllLeftMove_button";
            this->AllLeftMove_button->Size = System::Drawing::Size(75, 23);
            this->AllLeftMove_button->TabIndex = 13;
            this->AllLeftMove_button->Text = L"<<";
            this->AllLeftMove_button->UseVisualStyleBackColor = true;
            this->AllLeftMove_button->Click += gcnew System::EventHandler(this, &FinanceProductMgrForm::AllLeftMove_button_Click);
            // 
            // LeftMove_button
            // 
            this->LeftMove_button->Location = System::Drawing::Point(508, 92);
            this->LeftMove_button->Name = L"LeftMove_button";
            this->LeftMove_button->Size = System::Drawing::Size(75, 23);
            this->LeftMove_button->TabIndex = 14;
            this->LeftMove_button->Text = L"<";
            this->LeftMove_button->UseVisualStyleBackColor = true;
            this->LeftMove_button->Click += gcnew System::EventHandler(this, &FinanceProductMgrForm::LeftMove_button_Click);
            // 
            // RightMove_button
            // 
            this->RightMove_button->Location = System::Drawing::Point(508, 153);
            this->RightMove_button->Name = L"RightMove_button";
            this->RightMove_button->Size = System::Drawing::Size(75, 23);
            this->RightMove_button->TabIndex = 15;
            this->RightMove_button->Text = L">";
            this->RightMove_button->UseVisualStyleBackColor = true;
            this->RightMove_button->Click += gcnew System::EventHandler(this, &FinanceProductMgrForm::RightMove_button_Click);
            // 
            // AllRightMove_button
            // 
            this->AllRightMove_button->Location = System::Drawing::Point(508, 214);
            this->AllRightMove_button->Name = L"AllRightMove_button";
            this->AllRightMove_button->Size = System::Drawing::Size(75, 23);
            this->AllRightMove_button->TabIndex = 16;
            this->AllRightMove_button->Text = L">>";
            this->AllRightMove_button->UseVisualStyleBackColor = true;
            this->AllRightMove_button->Click += gcnew System::EventHandler(this, &FinanceProductMgrForm::AllRightMove_button_Click);
            // 
            // Save_button
            // 
            this->Save_button->Location = System::Drawing::Point(508, 278);
            this->Save_button->Name = L"Save_button";
            this->Save_button->Size = System::Drawing::Size(75, 23);
            this->Save_button->TabIndex = 17;
            this->Save_button->Text = L"保存";
            this->Save_button->UseVisualStyleBackColor = true;
            this->Save_button->Click += gcnew System::EventHandler(this, &FinanceProductMgrForm::Save_button_Click);
            // 
            // FinanceProductMgrForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->AutoSize = true;
            this->ClientSize = System::Drawing::Size(806, 311);
            this->Controls->Add(this->Save_button);
            this->Controls->Add(this->AllRightMove_button);
            this->Controls->Add(this->RightMove_button);
            this->Controls->Add(this->LeftMove_button);
            this->Controls->Add(this->AllLeftMove_button);
            this->Controls->Add(this->groupBox2);
            this->Controls->Add(this->delete_button);
            this->Controls->Add(this->modify_button);
            this->Controls->Add(this->add_button);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->FProduct_listView);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"FinanceProductMgrForm";
            this->Text = L"理财产品管理";
            this->Load += gcnew System::EventHandler(this, &FinanceProductMgrForm::FinanceProductMgrForm_Load);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FinanceProductMgrForm::FinanceProductMgrForm_FormClosed);
            this->groupBox1->ResumeLayout(false);
            this->groupBox2->ResumeLayout(false);
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
                      bool bQuery = false;

                      switch ( head.cmdid )
                      {	 
                      case Cmd_AddFinicialProduct_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int nFinancialProductID = *(int*)((char*)(p+sizeof(Stru_UniPkgHead)));
                                  m_pCurFinancialProduct->nID = nFinancialProductID;
                                  m_pDataInfo->SetFinancialProduct(m_pCurFinancialProduct);

                                  int nCount = FProduct_listView->Items->Count;
                                  ListViewItem^ item = gcnew ListViewItem( Tools::string2String(m_pCurFinancialProduct->szName),nCount);
                                  item->SubItems->Add( Tools::string2String(m_pCurFinancialProduct->szDesc));

                                  //UI
                                  FProduct_listView->Items->Add( item );

                                  strError = "增加理财产品操作成功";
                              }
                              else
                              {
                                  strError = "增加理财产品操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          }
                      case Cmd_DeleteFinicialProduct_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int nProductID = *(int*)((char*)(p+sizeof(Stru_UniPkgHead)));
                                  m_pDataInfo->DeleteFinanceProduct(nProductID);

                                  ListView::SelectedIndexCollection^ c = FProduct_listView->SelectedIndices;
                                  if(c != nullptr)
                                      FProduct_listView->Items->RemoveAt(c[0]);  

                                  strError = "删除理财产品操作成功";
                              }
                              else
                              {
                                  strError = "删除理财产品操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          }
                      case Cmd_ModifyFinicialProduct_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  FinancialProduct* pFinancialProduct = (FinancialProduct*)((char*)(p+sizeof(Stru_UniPkgHead)));
                                  m_pDataInfo->SetFinancialProduct(pFinancialProduct);

                                  FProduct_listView->SelectedItems[0]->SubItems[0]->Text = Tools::string2String(pFinancialProduct->szName);
                                  FProduct_listView->SelectedItems[0]->SubItems[1]->Text = Tools::string2String(pFinancialProduct->szDesc);

                                  strError = "修改理财产品操作成功";
                              }
                              else
                              {
                                  strError = "修改理财产品操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));   
                              }
                              break;
                          }
                      case Cmd_QryUser_Rsp:
                          {         
                              bQuery = true;
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

                                  //加载用户
                                  LoadUserInfo();  
                              }                         
                              break;
                          }
                      case Cmd_QueryFinicialProduct_Rsp:
                          {               
                              bQuery = true;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pDataInfo->ClearFinanceProduct();

                                  int nCount = head.len / sizeof(FinancialProduct);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      FinancialProduct* pFinancialProduct = (FinancialProduct*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(FinancialProduct));
                                      m_pDataInfo->SetFinancialProduct(pFinancialProduct);									
                                  }

                                  //加载理财产品
                                  LoadFinanceProduct();
                              }
                              break;
                          }
                      case Cmd_SaveProductTraderRelation_FromProduct_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {  
                                  int nType = *(int*)((char*)(p+sizeof(Stru_UniPkgHead)));

                                  String^ strName = FProduct_listView->SelectedItems[0]->SubItems[0]->Text;
                                  int nFPID = m_pDataInfo->GetFinancialProductIDByName(Tools::String2string(strName));

                                  TraderProRelation oRelation;
                                  oRelation.nFinancialProductID = nFPID;

                                  m_pDataInfo->ClearProductTraderRelation(nType, oRelation.nFinancialProductID);

                                  std::vector<int> vec;
                                  for (int i=0;i<InGroup_listView->Items->Count;i++)
                                  {
                                      String^ strAccount = InGroup_listView->Items[i]->SubItems[0]->Text;
                                      oRelation.nTraderID = m_pDataInfo->GetUserIDByAccount(Tools::String2string(strAccount));

                                      m_pDataInfo->SetProductTraderRelation(&oRelation);
                                  }

                                  strError = "保存理财产品与交易员关系关系操作成功";
                              }
                              else
                              {
                                  strError =  "保存理财产品操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          }
                      case Cmd_QueryProductTraderRelation_Rsp:
                          {
                              bQuery = true;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pDataInfo->ClearProductTraderRelation();

                                  int nCount = head.len / sizeof(TraderProRelation);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      TraderProRelation* pTraderProRelation = (TraderProRelation*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(TraderProRelation));
                                      m_pDataInfo->SetProductTraderRelation(pTraderProRelation);
                                  }
                              }
                              break;
                          }
                      }

                      if(!bQuery)
						_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
								CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
                                Tools::String2string(strError).c_str());
                  }
                  Form::WndProc ( m );
              }

    private: System::Void FinanceProductMgrForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 this->CenterToParent(); 

                 int nHwnd = this->Handle.ToInt32();                 
                 m_pDataInfo->GetLogonInfo(*m_pLogin);

                 //订阅信息                                
                 CTcpLayer::SubscribePkg(Cmd_AddFinicialProduct_Rsp, nHwnd);                
                 CTcpLayer::SubscribePkg(Cmd_DeleteFinicialProduct_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_ModifyFinicialProduct_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_SaveProductTraderRelation_FromProduct_Rsp, nHwnd);

                 //用户
                 CTcpLayer::SubscribePkg(Cmd_QryUser_Rsp, nHwnd);
                 CTcpLayer::SendData(Cmd_QryUser_Req, NULL,0, 0, m_pLogin->mnUserID);

                 //查询理财产品
                 CTcpLayer::SubscribePkg(Cmd_QueryFinicialProduct_Rsp, nHwnd);
                 CTcpLayer::SendData(Cmd_QueryFinicialProduct_Req, NULL,0, 0, m_pLogin->mnUserID);

                 //查询理财产品与交易员关系   
                 CTcpLayer::SubscribePkg(Cmd_QueryProductTraderRelation_Rsp, nHwnd);
                 CTcpLayer::SendData(Cmd_QueryProductTraderRelation_Req, NULL,0, 0, m_pLogin->mnUserID);       
             }

    private: System::Void add_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_PRODUCT_ADD));
                 FinanceProductForm^ dlg = gcnew FinanceProductForm();
                 dlg->SetOperType(Oper_Add);
                 if (::DialogResult::OK == dlg->ShowDialog(this))
                 {
                     *m_pCurFinancialProduct = *(dlg->GetCurFinancialProduct());             
                     CTcpLayer::SendData(Cmd_AddFinicialProduct_Req, m_pCurFinancialProduct, sizeof(FinancialProduct), 0, m_pLogin->mnUserID);
                 }
             }

    private: System::Void modify_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_PRODUCT_MODIFY));
                 if(FProduct_listView->SelectedIndices != nullptr && FProduct_listView->SelectedIndices->Count>0)
                 {
                     String^ strName = FProduct_listView->SelectedItems[0]->SubItems[0]->Text;
                     int nFPID = m_pDataInfo->GetFinancialProductIDByName(Tools::String2string(strName));
                     if(!m_pDataInfo->GetFinancialProduct(nFPID,*m_pCurFinancialProduct))
                         return;

                     FinanceProductForm^ dlg = gcnew FinanceProductForm();
                     dlg->SetOperType(Oper_Modify);
                     dlg->SetCurFinancialProduct(*m_pCurFinancialProduct);
                     if (::DialogResult::OK == dlg->ShowDialog(this))
                     {
                         *m_pCurFinancialProduct = *(dlg->GetCurFinancialProduct());             
                         CTcpLayer::SendData(Cmd_ModifyFinicialProduct_Req, m_pCurFinancialProduct, sizeof(FinancialProduct), 0, m_pLogin->mnUserID);
                     }
                 }
                 else
                     MessageBox::Show("请选择内容");
             }

    private: System::Void delete_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_PRODUCT_DEL));
                 if(FProduct_listView->SelectedIndices != nullptr && FProduct_listView->SelectedIndices->Count>0)
                 {
                     if(::DialogResult::Cancel ==  MessageBox::Show("此操作将会同步删除理财产品和交易员之间的关系。确认吗？", "提示", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
                         return;

                     String^ strName = FProduct_listView->SelectedItems[0]->SubItems[0]->Text;
                     int nFPID = m_pDataInfo->GetFinancialProductIDByName(Tools::String2string(strName));

                     CTcpLayer::SendData(Cmd_DeleteFinicialProduct_Req,&nFPID,sizeof(int),0, m_pLogin->mnUserID);
                 }
                 else
                     MessageBox::Show("请选择内容");
             }

    private: System::Void FProduct_listView_ItemSelectionChanged(System::Object^  sender, System::Windows::Forms::ListViewItemSelectionChangedEventArgs^  e) {
                 if(FProduct_listView->SelectedIndices != nullptr && FProduct_listView->SelectedIndices->Count>0)
                 {
                     ListView::SelectedIndexCollection^ c = FProduct_listView->SelectedIndices;
                     String^ strName = FProduct_listView->Items[c[0]]->Text;

                     int nFPID = m_pDataInfo->GetFinancialProductIDByName(Tools::String2string(strName));
                     LoadTraderByProductID(nFPID);
                 } 
                 else 
                     LoadUserInfo();
             }
    private: System::Void Save_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_PRODUCT_SAVE));
                 if(FProduct_listView->SelectedIndices != nullptr && FProduct_listView->SelectedIndices->Count>0)
                 {
                     String^ strName = FProduct_listView->SelectedItems[0]->SubItems[0]->Text;
                     int nFPID = m_pDataInfo->GetFinancialProductIDByName(Tools::String2string(strName));

                     std::vector<int> vec;
                     for (int i=0;i<InGroup_listView->Items->Count;i++)
                     {
                         String^ strAccount = InGroup_listView->Items[i]->SubItems[0]->Text;
                         int nTraderID = m_pDataInfo->GetUserIDByAccount(Tools::String2string(strAccount));

                         vec.push_back(nTraderID);
                     }

                     int nTraderProRelationType = PRODUCT_ASSGIN_TRADER_TYPE;
                     int nDataLen = 2*sizeof(int) + vec.size()*sizeof(int);                         
                     char* pBuffer = new char[nDataLen];
                     memset(pBuffer, 0, nDataLen);
                     memcpy(pBuffer, &nTraderProRelationType, sizeof(int));
                     memcpy(pBuffer+sizeof(int), &nFPID, sizeof(int));

                     if ( !vec.empty() )                     
                         memcpy(pBuffer+2*sizeof(int), &vec[0], sizeof(int)*vec.size());                     

                     CTcpLayer::SendData(Cmd_SaveProductTraderRelation_FromProduct_Req, pBuffer, nDataLen, 0, m_pLogin->mnUserID);        

                     delete []pBuffer;
                 }
                 else
                     MessageBox::Show("请先选择理财产品");

             }
    private: System::Void AllLeftMove_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 if(FProduct_listView->SelectedIndices != nullptr && FProduct_listView->SelectedIndices->Count>0)
                 {
                     for (int i=0;i<OutGroup_listView->Items->Count;i++)
                     {
                         String^ strAccount  = OutGroup_listView->Items[i]->SubItems[0]->Text;
                         String^ strUserName = OutGroup_listView->Items[i]->SubItems[1]->Text;

                         //UI
                         int nCount = InGroup_listView->Items->Count;
                         ListViewItem^ item = gcnew ListViewItem(strAccount,nCount); 
                         item->SubItems->Add(strUserName);  
                         InGroup_listView->Items->Add( item );    
                     }

                     OutGroup_listView->Items->Clear();
                 }
                 else
                     MessageBox::Show("请先选择理财产品");
             }
    private: System::Void LeftMove_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 ListView::SelectedIndexCollection^ c = OutGroup_listView->SelectedIndices;
                 if(c->Count == 0)
                     return;

                 String^ strAccount = OutGroup_listView->Items[c[0]]->SubItems[0]->Text;  
                 String^ strUserName = OutGroup_listView->Items[c[0]]->SubItems[1]->Text;

                 //UI
                 int nCount = InGroup_listView->Items->Count;
                 ListViewItem^ item = gcnew ListViewItem(strAccount,nCount); 
                 item->SubItems->Add(strUserName);  
                 InGroup_listView->Items->Add( item );

                 OutGroup_listView->Items->RemoveAt(c[0]);  
             }
    private: System::Void RightMove_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 ListView::SelectedIndexCollection^ c = InGroup_listView->SelectedIndices;
                 if(c->Count == 0)
                     return;

                 String^ strAccount = InGroup_listView->Items[c[0]]->SubItems[0]->Text;  
                 String^ strUserName = InGroup_listView->Items[c[0]]->SubItems[1]->Text;

                 //UI
                 int nCount = OutGroup_listView->Items->Count;
                 ListViewItem^ item = gcnew ListViewItem(strAccount,nCount); 
                 item->SubItems->Add(strUserName);  
                 OutGroup_listView->Items->Add( item );

                 InGroup_listView->Items->RemoveAt(c[0]);  
             }
    private: System::Void AllRightMove_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 for (int i=0;i<InGroup_listView->Items->Count;i++)
                 {
                     String^ strAccount = InGroup_listView->Items[i]->SubItems[0]->Text;
                     String^ strUserName = InGroup_listView->Items[i]->SubItems[1]->Text;

                     //UI
                     int nCount = OutGroup_listView->Items->Count;
                     ListViewItem^ item = gcnew ListViewItem(strAccount,nCount); 
                     item->SubItems->Add(strUserName);  
                     OutGroup_listView->Items->Add( item );   
                 }

                 InGroup_listView->Items->Clear();  
             }

    private: void LoadFinanceProduct()
             {
                 if(m_pDataInfo == NULL)
                     return;

                 FProduct_listView->Items->Clear();

                 std::map<int, FinancialProduct>    mapFinancialProduct;
                 m_pDataInfo->GetFinancialProduct(mapFinancialProduct);

                 std::map<int, FinancialProduct>::iterator it=mapFinancialProduct.begin();
                 for (int i=0;it!=mapFinancialProduct.end();it++)
                 {
                     ListViewItem^ item = gcnew ListViewItem(Tools::string2String(it->second.szName),i++); 
                     item->SubItems->Add(Tools::string2String(it->second.szDesc));  
                     FProduct_listView->Items->Add( item );
                 }  
             }
    private: void LoadUserInfo()
             {
                 if(m_pDataInfo == NULL)
                     return;

                 InGroup_listView->Items->Clear();
                 OutGroup_listView->Items->Clear();

                 std::map<int, UserInfo> mapUserInfo;
                 m_pDataInfo->GetUserInfo(mapUserInfo);

                 std::map<int, UserInfo>::iterator it = mapUserInfo.begin();
                 for (int i=0;it!=mapUserInfo.end();it++)
                 {
                     if (it->second.nUserType == USER_TYPE_TRADER)
                     {
                         ListViewItem^ item = gcnew ListViewItem(Tools::string2String(it->second.szAccount),i++);      
                         item->SubItems->Add(Tools::string2String(it->second.szAlias));  
                         OutGroup_listView->Items->Add(item);
                     }
                 }

             }
    private: void LoadTraderByProductID(int nFPID) 
             {
                 if(nFPID == -1)
                     return;

                 std::vector<TraderProRelation> vecTraderProRelation;
                 m_pDataInfo->GetProTraderRelation(vecTraderProRelation);
                 if(vecTraderProRelation.empty())
                     return;

                 InGroup_listView->Items->Clear();

                 bool bHaveRelate = false;
                 std::vector<TraderProRelation>::iterator it = vecTraderProRelation.begin();
                 for (int i=0;it!=vecTraderProRelation.end();it++)
                 {
                     if (nFPID == it->nFinancialProductID)
                     {
                         UserInfo oUserInfo;
                         memset(&oUserInfo,0,sizeof(oUserInfo));

                         m_pDataInfo->GetUserInfoByUserID(it->nTraderID,oUserInfo);

                         ListViewItem^ item = gcnew ListViewItem(Tools::string2String(oUserInfo.szAccount),i++);
                         item->SubItems->Add(Tools::string2String(oUserInfo.szAlias)); 

                         InGroup_listView->Items->Add(item);                      

                         for (int i=0;i<OutGroup_listView->Items->Count;i++)
                         {
                             String^ strAccount = OutGroup_listView->Items[i]->SubItems[0]->Text;
                             if ( 0 == strcmp(Tools::String2string(strAccount).c_str(),oUserInfo.szAccount) )                         
                                 OutGroup_listView->Items->RemoveAt(i);                         
                         }
                     }
                 }
             }

    private: System::Void FinanceProductMgrForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
                 int nHwnd = this->Handle.ToInt32();
                 CTcpLayer::UnsubscribeAllPkg(nHwnd);
             }
    };
}

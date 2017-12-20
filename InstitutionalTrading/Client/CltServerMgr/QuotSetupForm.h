#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "CommonStruct.h"
#include "TcpLayer.h"
#include "CommonDef.h"
#include "CommonErrorCode.h"
#include "CommonPkg.h"
#include "Tools.h"
#include "CDataInfo.h"
#include "ServerAddrForm.h"
#include "RefOtherClass.h"
#include "CommonUserOpr.h"


namespace CltServerMgr {

	/// <summary>
	/// Summary for QuotSetupForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
    /// </summary> 

	public ref class QuotSetupForm : public System::Windows::Forms::Form
	{
	public:
		QuotSetupForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pLogin = new sLoginRsp();
            m_pDataInfo = CDataInfo::NewInstance();
            m_pCurBrokerInfoQuot = new BrokerInfo;
            m_pCurServerAddrQuot = new ServerAddressQuot;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~QuotSetupForm()
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

            if ( NULL != m_pCurBrokerInfoQuot )
            {
                delete m_pCurBrokerInfoQuot;
                m_pCurBrokerInfoQuot = NULL;
            }

            if ( NULL != m_pCurServerAddrQuot )
            {
                delete m_pCurServerAddrQuot;
                m_pCurServerAddrQuot = NULL;
            }
		}


    private: sLoginRsp* m_pLogin;
    private: CDataInfo* m_pDataInfo;
    private: BrokerInfo* m_pCurBrokerInfoQuot;
    private: ServerAddressQuot* m_pCurServerAddrQuot;

    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::GroupBox^  groupBox1;
    private: System::Windows::Forms::Button^  buttonDel;
    private: System::Windows::Forms::ListView^  listViewQuotAddr;
    private: System::Windows::Forms::Button^  buttonAdd;
    private: System::Windows::Forms::Button^  buttonModify;
    private: System::Windows::Forms::Button^  buttonOK;
    private: System::Windows::Forms::Button^  buttonCancel;
    private: System::Windows::Forms::ComboBox^  comboBoxBroker;
    private: System::Windows::Forms::TextBox^  textBoxAccount;
    private: System::Windows::Forms::TextBox^  textBoxPSW;
    private: System::Windows::Forms::ColumnHeader^  columnHeader1;
    private: System::Windows::Forms::ColumnHeader^  columnHeader2;




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
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->buttonDel = (gcnew System::Windows::Forms::Button());
            this->listViewQuotAddr = (gcnew System::Windows::Forms::ListView());
            this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
            this->buttonAdd = (gcnew System::Windows::Forms::Button());
            this->buttonModify = (gcnew System::Windows::Forms::Button());
            this->buttonOK = (gcnew System::Windows::Forms::Button());
            this->buttonCancel = (gcnew System::Windows::Forms::Button());
            this->comboBoxBroker = (gcnew System::Windows::Forms::ComboBox());
            this->textBoxAccount = (gcnew System::Windows::Forms::TextBox());
            this->textBoxPSW = (gcnew System::Windows::Forms::TextBox());
            this->groupBox1->SuspendLayout();
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(34, 23);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(65, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"经纪公司：";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(34, 57);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(65, 12);
            this->label2->TabIndex = 1;
            this->label2->Text = L"登录账号：";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(34, 87);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(41, 12);
            this->label3->TabIndex = 2;
            this->label3->Text = L"密码：";
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->buttonDel);
            this->groupBox1->Controls->Add(this->listViewQuotAddr);
            this->groupBox1->Controls->Add(this->buttonAdd);
            this->groupBox1->Controls->Add(this->buttonModify);
            this->groupBox1->Location = System::Drawing::Point(34, 121);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(338, 300);
            this->groupBox1->TabIndex = 3;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"服务器地址";
            // 
            // buttonDel
            // 
            this->buttonDel->Location = System::Drawing::Point(226, 266);
            this->buttonDel->Name = L"buttonDel";
            this->buttonDel->Size = System::Drawing::Size(75, 23);
            this->buttonDel->TabIndex = 7;
            this->buttonDel->Text = L"删除";
            this->buttonDel->UseVisualStyleBackColor = true;
            this->buttonDel->Click += gcnew System::EventHandler(this, &QuotSetupForm::buttonDel_Click);
            // 
            // listViewQuotAddr
            // 
            this->listViewQuotAddr->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->columnHeader1, 
                this->columnHeader2});
            this->listViewQuotAddr->FullRowSelect = true;
            this->listViewQuotAddr->GridLines = true;
            this->listViewQuotAddr->Location = System::Drawing::Point(6, 20);
            this->listViewQuotAddr->Name = L"listViewQuotAddr";
            this->listViewQuotAddr->Size = System::Drawing::Size(326, 240);
            this->listViewQuotAddr->TabIndex = 4;
            this->listViewQuotAddr->UseCompatibleStateImageBehavior = false;
            this->listViewQuotAddr->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader1
            // 
            this->columnHeader1->Text = L"行情服务器地址";
            this->columnHeader1->Width = 260;
            // 
            // columnHeader2
            // 
            this->columnHeader2->Text = L"端口";
            this->columnHeader2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // buttonAdd
            // 
            this->buttonAdd->Location = System::Drawing::Point(33, 266);
            this->buttonAdd->Name = L"buttonAdd";
            this->buttonAdd->Size = System::Drawing::Size(75, 23);
            this->buttonAdd->TabIndex = 5;
            this->buttonAdd->Text = L"添加";
            this->buttonAdd->UseVisualStyleBackColor = true;
            this->buttonAdd->Click += gcnew System::EventHandler(this, &QuotSetupForm::buttonAdd_Click);
            // 
            // buttonModify
            // 
            this->buttonModify->Location = System::Drawing::Point(131, 266);
            this->buttonModify->Name = L"buttonModify";
            this->buttonModify->Size = System::Drawing::Size(75, 23);
            this->buttonModify->TabIndex = 6;
            this->buttonModify->Text = L"修改";
            this->buttonModify->UseVisualStyleBackColor = true;
            this->buttonModify->Click += gcnew System::EventHandler(this, &QuotSetupForm::buttonModify_Click);
            // 
            // buttonOK
            // 
            this->buttonOK->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->buttonOK->Location = System::Drawing::Point(112, 454);
            this->buttonOK->Name = L"buttonOK";
            this->buttonOK->Size = System::Drawing::Size(75, 23);
            this->buttonOK->TabIndex = 8;
            this->buttonOK->Text = L"确定";
            this->buttonOK->UseVisualStyleBackColor = true;
            this->buttonOK->Click += gcnew System::EventHandler(this, &QuotSetupForm::buttonOK_Click);
            // 
            // buttonCancel
            // 
            this->buttonCancel->Location = System::Drawing::Point(218, 454);
            this->buttonCancel->Name = L"buttonCancel";
            this->buttonCancel->Size = System::Drawing::Size(75, 23);
            this->buttonCancel->TabIndex = 9;
            this->buttonCancel->Text = L"取消";
            this->buttonCancel->UseVisualStyleBackColor = true;
            this->buttonCancel->Click += gcnew System::EventHandler(this, &QuotSetupForm::buttonCancel_Click);
            // 
            // comboBoxBroker
            // 
            this->comboBoxBroker->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->comboBoxBroker->FormattingEnabled = true;
            this->comboBoxBroker->Location = System::Drawing::Point(105, 20);
            this->comboBoxBroker->Name = L"comboBoxBroker";
            this->comboBoxBroker->Size = System::Drawing::Size(121, 20);
            this->comboBoxBroker->TabIndex = 1;
            this->comboBoxBroker->SelectedIndexChanged += gcnew System::EventHandler(this, &QuotSetupForm::comboBoxBroker_SelectedIndexChanged);
            // 
            // textBoxAccount
            // 
            this->textBoxAccount->Location = System::Drawing::Point(105, 52);
            this->textBoxAccount->Name = L"textBoxAccount";
            this->textBoxAccount->Size = System::Drawing::Size(121, 21);
            this->textBoxAccount->TabIndex = 2;
            // 
            // textBoxPSW
            // 
            this->textBoxPSW->Location = System::Drawing::Point(105, 84);
            this->textBoxPSW->Name = L"textBoxPSW";
            this->textBoxPSW->PasswordChar = '*';
            this->textBoxPSW->Size = System::Drawing::Size(121, 21);
            this->textBoxPSW->TabIndex = 3;
            // 
            // QuotSetupForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(412, 494);
            this->Controls->Add(this->textBoxPSW);
            this->Controls->Add(this->textBoxAccount);
            this->Controls->Add(this->comboBoxBroker);
            this->Controls->Add(this->buttonOK);
            this->Controls->Add(this->buttonCancel);
            this->Controls->Add(this->groupBox1);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"QuotSetupForm";
            this->Text = L"行情服务器配置";
            this->Load += gcnew System::EventHandler(this, &QuotSetupForm::QuotSetupForm_Load);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &QuotSetupForm::QuotSetupForm_FormClosed);
            this->groupBox1->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

        public:String^ GetAccount()
               {
                   return textBoxAccount->Text;
               }
        public:String^ GetPsw()
               {
                   return textBoxPSW->Text;
               }
    private: System::Void QuotSetupForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 this->CenterToScreen();

                 int nHwnd = this->Handle.ToInt32();
                 m_pDataInfo->GetLogonInfo(*m_pLogin);
                 
//                  CTcpLayer::SubscribePkg(Cmd_ModifyBrokerInfoQuot_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_AddServerAddressQuot_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_ModifyServerAddressQuot_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_DelServerAddressQuot_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_QueryServerAddressQuot_Rsp, nHwnd);

                 if (!CTcpLayer::SendData(Cmd_QueryServerAddressQuot_Req, NULL,0, 0, m_pLogin->mnUserID))
                 {
                     MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
                 }   
                 
                 LoadBrokerInfo();
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
                  case Cmd_QueryServerAddressQuot_Rsp:
                      {		
                          bShowMsg = false;
                          listViewQuotAddr->Items->Clear();
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              int j=0;
                              std::map<int, ServerAddressQuot> mapServerAddressQuot;
                              int nCount = head.len / sizeof(ServerAddressQuot);
                              for ( int i = 0,j=0; i < nCount; i++ )
                              {
                                  ServerAddressQuot* pServerAddressQuot = (ServerAddressQuot*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(ServerAddressQuot));
                                  mapServerAddressQuot[pServerAddressQuot->nAddrID] = *pServerAddressQuot;

                                  BrokerData^ data = (BrokerData^)comboBoxBroker->SelectedItem;
                                  if(data != nullptr && data->GetBrokerCode() == Tools::string2String(pServerAddressQuot->szBrokerCode))
                                  {
                                      ListViewItem^ item = gcnew ListViewItem(Tools::string2String(pServerAddressQuot->szIP),j++); 
                                      item->Tag = pServerAddressQuot->nAddrID;
                                      item->SubItems->Add(pServerAddressQuot->nPort.ToString());  
                                      listViewQuotAddr->Items->Add( item ); 
                                  }
                              }
                              
                              m_pDataInfo->SetServerAddressQuot(mapServerAddressQuot);
                          }
                          else
                          {
                              strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }

                  case Cmd_AddServerAddressQuot_Rsp:
                      {
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              m_pCurServerAddrQuot->nAddrID = *(int*)(p+sizeof(Stru_UniPkgHead));
                              m_pDataInfo->SetServerAddressQuot(*m_pCurServerAddrQuot);

                              int nCount = listViewQuotAddr->Items->Count;
                              ListViewItem^ item = gcnew ListViewItem( Tools::string2String(m_pCurServerAddrQuot->szIP),nCount);
                              item->Tag = m_pCurServerAddrQuot->nAddrID;
                              item->SubItems->Add( m_pCurServerAddrQuot->nPort.ToString());
                              listViewQuotAddr->Items->Add( item );

                              strError = "增加行情服务器地址操作成功,需要重启行情服务器才可以生效。";
                          }
                          else
                          {
                              strError = "增加行情服务器地址操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }
                  case Cmd_DelServerAddressQuot_Rsp:
                      {
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              int nAddrID = *(int*)(p+sizeof(Stru_UniPkgHead));
                              if ( nAddrID == m_pCurServerAddrQuot->nAddrID )  
                                 m_pDataInfo->DelServerAddressQuot(nAddrID);

                              ListView::SelectedIndexCollection^ c = listViewQuotAddr->SelectedIndices;
                              if(c != nullptr)
                                  listViewQuotAddr->Items->RemoveAt(c[0]);  

                              strError = "删除行情服务器地址操作成功,需要重启行情服务器才可以生效。";
                          }
                          else
                          {
                              strError = "删除行情服务器地址操作失败,失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                          }

                          break;
                      }
                  case Cmd_ModifyServerAddressQuot_Rsp:
                      {
                          if ( head.userdata1 == CF_ERROR_SUCCESS )
                          {
                              int nAddrID = *(int*)(p+sizeof(Stru_UniPkgHead));
                              if ( nAddrID == m_pCurServerAddrQuot->nAddrID )                              
                                  m_pDataInfo->SetServerAddressQuot(*m_pCurServerAddrQuot);                              

                              listViewQuotAddr->SelectedItems[0]->SubItems[0]->Text = Tools::string2String(m_pCurServerAddrQuot->szIP);
                              listViewQuotAddr->SelectedItems[0]->SubItems[1]->Text = m_pCurServerAddrQuot->nPort.ToString();

                              strError = "修改行情服务器地址操作成功,需要重启行情服务器才可以生效。";
                          }
                          else
                          {
                              strError = "修改行情服务器地址操作失败,失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
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

private: System::Void buttonAdd_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_QUOTADDR_ADD));
             ServerAddrForm^ dlg = gcnew ServerAddrForm();
             dlg->SetOperType(Oper_Add_Quot);
             if (::DialogResult::OK == dlg->ShowDialog(this))
             {
                 BrokerData^ data = (BrokerData^)comboBoxBroker->SelectedItem;
                 if(data != nullptr)
                 {
                     int nBrokerID = data->GetBrokerID();

                     *m_pCurServerAddrQuot = *(dlg->GetCurServerAddressQuot());             
                     strcpy(m_pCurServerAddrQuot->szBrokerCode,m_pDataInfo->GetBrokerCodeByID(nBrokerID).c_str());
                     CTcpLayer::SendData(Cmd_AddServerAddressQuot_Req, m_pCurServerAddrQuot,sizeof(ServerAddressQuot), 0, m_pLogin->mnUserID);
                 }
             }
         }
private: System::Void buttonModify_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_QUOTADDR_MODIFY));
//              if(m_pDataInfo->GetTradeDataStatus() != TradeDataStatusSettlemtEnd)
//              {
//                  MessageBox::Show("只有结算完成后才可以做此操作");
//                  return;
//              }

//              if(::DialogResult::Cancel ==  MessageBox::Show("需要重启行情服务器才可以生效。确认吗？", "提示", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
//                  return;

             if(listViewQuotAddr->SelectedIndices != nullptr && listViewQuotAddr->SelectedIndices->Count>0)
             {
                 BrokerData^ data = (BrokerData^)comboBoxBroker->SelectedItem;
                 if(data==nullptr)
                     return;

                 int nBrokerID = data->GetBrokerID();

                 ListView::SelectedIndexCollection^ ca = listViewQuotAddr->SelectedIndices;
                 int nAddrID = (int)(listViewQuotAddr->Items[ca[0]]->Tag);

                 if(!m_pDataInfo->GetServerAddrQuotByID(nAddrID, *m_pCurServerAddrQuot))
                     return;

                 ServerAddrForm^ dlg = gcnew ServerAddrForm();
                 dlg->SetOperType(Oper_Modify_Quot);
                 dlg->SetCurServerAddressQuot(*m_pCurServerAddrQuot);
                 if (::DialogResult::OK == dlg->ShowDialog(this))
                 {
                     if(*m_pCurServerAddrQuot == *(dlg->GetCurServerAddressQuot()))
                         return;
                     else
                     {
                         *m_pCurServerAddrQuot = *(dlg->GetCurServerAddressQuot()); 
                         CTcpLayer::SendData(Cmd_ModifyServerAddressQuot_Req, m_pCurServerAddrQuot,sizeof(ServerAddressQuot), 0, m_pLogin->mnUserID);
                     }
                 }
             }
             else
                 MessageBox::Show("请选择内容"); 
         }
private: System::Void buttonDel_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_QUOTADDR_DEL));
//              if(m_pDataInfo->GetTradeDataStatus() != TradeDataStatusSettlemtEnd)
//              {
//                  MessageBox::Show("只有结算完成后才可以做此操作");
//                  return;
//              }

//              if(::DialogResult::Cancel ==  MessageBox::Show("需要重启行情服务器才可以生效。确认吗？", "提示", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
//                  return;

             if(listViewQuotAddr->SelectedIndices != nullptr && listViewQuotAddr->SelectedIndices->Count>0)
             {        
                 ListViewItem^ item   = listViewQuotAddr->Items[listViewQuotAddr->SelectedIndices[0]];
                 int nAddrID = (int)(item->Tag);

                 CTcpLayer::SendData(Cmd_DelServerAddressQuot_Req, &nAddrID, sizeof(int), 0, m_pLogin->mnUserID);              
             }
             else
             {
                 MessageBox::Show("请选择一个服务器组地址！", "提示",MessageBoxButtons::OK);
                 return;
             }

         }
private: System::Void buttonOK_Click(System::Object^  sender, System::EventArgs^  e) {
             if(String::IsNullOrEmpty(textBoxAccount->Text) ||
                String::IsNullOrEmpty(textBoxPSW->Text))
             {
                MessageBox::Show("账号或密码不能为空！");
                return;
             }

//              MessageBox::Show("本次新增或修改内容将在行情服务器重新初始化后生效");

             //修改行情经纪公司      
             BrokerInfo brokerInfo;

             BrokerData^ data = (BrokerData^)comboBoxBroker->SelectedItem;
             if(data==nullptr)
                 return;

             brokerInfo.nBrokerID = data->GetBrokerID();
             brokerInfo.nType = 1;

             strcpy(brokerInfo.szAccount,Tools::String2string(textBoxAccount->Text).c_str());
             strcpy(brokerInfo.szPassword,Tools::String2string(textBoxPSW->Text).c_str());
             strcpy(brokerInfo.szBrokerCode,m_pCurBrokerInfoQuot->szBrokerCode);
             strcpy(brokerInfo.szCompany,m_pCurBrokerInfoQuot->szCompany);

             if(*m_pCurBrokerInfoQuot == brokerInfo)
             {
                 return;
             }

             MessageBox::Show("需要重启行情服务器才可以生效。");

             CTcpLayer::SendData(Cmd_ModifyBrokerInfoQuot_Req, &brokerInfo, sizeof(BrokerInfo), 0, m_pLogin->mnUserID); 
         }
private: System::Void buttonCancel_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
private: System::Void QuotSetupForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
             int nHwnd = this->Handle.ToInt32();
             CTcpLayer::UnsubscribeAllPkg(nHwnd);
         }
private: System::Void comboBoxBroker_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             listViewQuotAddr->Items->Clear();

             BrokerData^ data = (BrokerData^)comboBoxBroker->SelectedItem;
             if(data==nullptr)
                 return;

             m_pCurBrokerInfoQuot->nBrokerID = data->GetBrokerID();
             strcpy(m_pCurBrokerInfoQuot->szBrokerCode,Tools::String2string(data->GetBrokerCode()).c_str());
             strcpy(m_pCurBrokerInfoQuot->szCompany,Tools::String2string(data->GetBrokerCompany()).c_str());

             textBoxAccount->Text = data->GetBrokerAccount();
             textBoxPSW->Text = data->GetBrokerPSW();

             std::map<int,ServerAddressQuot> mapServerAddr;
             m_pDataInfo->GetServerAddressQuot(mapServerAddr);

             int i=0;
             for (std::map<int,ServerAddressQuot>::iterator it=mapServerAddr.begin();it!=mapServerAddr.end();it++)
             {
                 if(data->GetBrokerCode() == Tools::string2String(it->second.szBrokerCode))
                 {
                     ListViewItem^ item = gcnew ListViewItem(Tools::string2String(it->second.szIP),i++); 
                     item->Tag = it->second.nAddrID;
                     item->SubItems->Add(it->second.nPort.ToString());  
                     listViewQuotAddr->Items->Add( item );
                 }
             }
         }

         private: void LoadBrokerInfo()
                  {
                      map<int,BrokerInfo> mapBrokerInfo;
                      m_pDataInfo->GetBrokerInfos(mapBrokerInfo);
                      if(!mapBrokerInfo.empty())
                      {
                          map<int,BrokerInfo>::iterator it = mapBrokerInfo.begin();
                          for(int i=0;it!=mapBrokerInfo.end();it++,i++)
                          {
                              BrokerData^ data = gcnew BrokerData(it->second.nBrokerID,it->second.nType,
                                  Tools::string2String(it->second.szBrokerCode),
                                  Tools::string2String(it->second.szCompany),
                                  Tools::string2String(it->second.szAccount),
                                  Tools::string2String(it->second.szPassword));
                              comboBoxBroker->Items->Add(data);

                              if(it->second.nType)
                              {
                                  textBoxAccount->Text = Tools::string2String(it->second.szAccount);
                                  textBoxPSW->Text = Tools::string2String(it->second.szPassword);
                                  comboBoxBroker->SelectedIndex = i;

                                  m_pCurBrokerInfoQuot->nBrokerID = data->GetBrokerID();
                                  m_pCurBrokerInfoQuot->nType = it->second.nType;
                                  strcpy(m_pCurBrokerInfoQuot->szBrokerCode,Tools::String2string(data->GetBrokerCode()).c_str());
                                  strcpy(m_pCurBrokerInfoQuot->szCompany,Tools::String2string(data->GetBrokerCompany()).c_str());
                                  strcpy(m_pCurBrokerInfoQuot->szAccount,Tools::String2string(data->GetBrokerAccount()).c_str());
                                  strcpy(m_pCurBrokerInfoQuot->szPassword,Tools::String2string(data->GetBrokerPSW()).c_str());
                              }
                          }

                      }

                  }
};
}

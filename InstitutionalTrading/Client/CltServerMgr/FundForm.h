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
#include "FundOprForm.h"
#include "CommonUserOpr.h"


namespace CltServerMgr {

    /// <summary>
    /// Summary for FundForm
    ///
    /// WARNING: If you change the name of this class, you will need to change the
    ///          'Resource File Name' property for the managed resource compiler tool
    ///          associated with all .resx files this class depends on.  Otherwise,
    ///          the designers will not be able to interact properly with localized
    ///          resources associated with this form.
    /// </summary>
    public ref class FundForm : public System::Windows::Forms::Form
    {
    public:
        FundForm(void)
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //
            m_pCurFundInOut = new sFundInOut;
            m_pLogin = new sLoginRsp;
            m_pDataInfo = CDataInfo::NewInstance();
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~FundForm()
        {
            if (components)
            {
                delete components;
            }

            if (m_pCurFundInOut)
            {
                delete m_pCurFundInOut;
                m_pCurFundInOut = NULL;
            }

            if (m_pLogin)
            {
                delete m_pLogin;
                m_pLogin = NULL;
            }
        }


    private: sFundInOut* m_pCurFundInOut;
    private: sLoginRsp* m_pLogin;
    private: CDataInfo* m_pDataInfo;


    private: System::Windows::Forms::SplitContainer^  splitContainer1;
    protected: 
    private: System::Windows::Forms::ListView^  listView1;

    private: System::Windows::Forms::ColumnHeader^  columnHeader2;
    private: System::Windows::Forms::ColumnHeader^  columnHeader3;
    private: System::Windows::Forms::ColumnHeader^  columnHeader4;
    private: System::Windows::Forms::ColumnHeader^  columnHeader5;
    private: System::Windows::Forms::ColumnHeader^  columnHeader6;
    private: System::Windows::Forms::ColumnHeader^  columnHeader7;
    private: System::Windows::Forms::ColumnHeader^  columnHeader8;
    private: System::Windows::Forms::Button^  button_outFund;
    private: System::Windows::Forms::Button^  button_inFund;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
    private: System::Windows::Forms::DateTimePicker^  dateTimePicker2;
    private: System::Windows::Forms::Button^  button_Qry;
    private: System::Windows::Forms::ComboBox^  comboBox_ID;

    private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  button1;



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
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->comboBox_ID = (gcnew System::Windows::Forms::ComboBox());
			this->button_Qry = (gcnew System::Windows::Forms::Button());
			this->dateTimePicker2 = (gcnew System::Windows::Forms::DateTimePicker());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader5 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader6 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader7 = (gcnew System::Windows::Forms::ColumnHeader());
			this->columnHeader8 = (gcnew System::Windows::Forms::ColumnHeader());
			this->button_outFund = (gcnew System::Windows::Forms::Button());
			this->button_inFund = (gcnew System::Windows::Forms::Button());			
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->IsSplitterFixed = true;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			this->splitContainer1->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->label3);
			this->splitContainer1->Panel1->Controls->Add(this->comboBox_ID);
			this->splitContainer1->Panel1->Controls->Add(this->button_Qry);
			this->splitContainer1->Panel1->Controls->Add(this->dateTimePicker2);
			this->splitContainer1->Panel1->Controls->Add(this->dateTimePicker1);
			this->splitContainer1->Panel1->Controls->Add(this->label2);
			this->splitContainer1->Panel1->Controls->Add(this->label1);
			this->splitContainer1->Panel1->Controls->Add(this->listView1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->button1);
			this->splitContainer1->Panel2->Controls->Add(this->button_outFund);
			this->splitContainer1->Panel2->Controls->Add(this->button_inFund);
			this->splitContainer1->Size = System::Drawing::Size(858, 292);
			this->splitContainer1->SplitterDistance = 256;
			this->splitContainer1->TabIndex = 0;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(498, 13);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(77, 12);
			this->label3->TabIndex = 7;
			this->label3->Text = L"交易员账号：";
			// 
			// comboBox_ID
			// 
			this->comboBox_ID->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox_ID->FormattingEnabled = true;
			this->comboBox_ID->Location = System::Drawing::Point(584, 9);
			this->comboBox_ID->Name = L"comboBox_ID";
			this->comboBox_ID->Size = System::Drawing::Size(121, 20);
			this->comboBox_ID->TabIndex = 6;
			// 
			// button_Qry
			// 
			this->button_Qry->Location = System::Drawing::Point(738, 9);
			this->button_Qry->Name = L"button_Qry";
			this->button_Qry->Size = System::Drawing::Size(75, 23);
			this->button_Qry->TabIndex = 5;
			this->button_Qry->Text = L"查询";
			this->button_Qry->UseVisualStyleBackColor = true;
			this->button_Qry->Click += gcnew System::EventHandler(this, &FundForm::button_Qry_Click);
			// 
			// dateTimePicker2
			// 
			this->dateTimePicker2->Format = System::Windows::Forms::DateTimePickerFormat::Short;
			this->dateTimePicker2->Location = System::Drawing::Point(318, 9);
			this->dateTimePicker2->Name = L"dateTimePicker2";
			this->dateTimePicker2->Size = System::Drawing::Size(125, 21);
			this->dateTimePicker2->TabIndex = 4;
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Short;
			this->dateTimePicker1->Location = System::Drawing::Point(84, 9);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(125, 21);
			this->dateTimePicker1->TabIndex = 3;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(247, 13);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(65, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"结束时间：";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(65, 12);
			this->label1->TabIndex = 1;
			this->label1->Text = L"起始时间：";
			// 
			// listView1
			// 
			this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(7) {this->columnHeader2, this->columnHeader3, 
				this->columnHeader4, this->columnHeader5, this->columnHeader6, this->columnHeader7, this->columnHeader8});
			this->listView1->FullRowSelect = true;
			this->listView1->GridLines = true;
			this->listView1->Location = System::Drawing::Point(0, 38);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(858, 218);
			this->listView1->TabIndex = 0;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::Details;
			// 
			// columnHeader2
			// 
			this->columnHeader2->Text = L"交易员账号";
			this->columnHeader2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader2->Width = 100;
			// 
			// columnHeader3
			// 
			this->columnHeader3->Text = L"出金/入金";
			this->columnHeader3->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader3->Width = 80;
			// 
			// columnHeader4
			// 
			this->columnHeader4->Text = L"金额";
			this->columnHeader4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader4->Width = 80;
			// 
			// columnHeader5
			// 
			this->columnHeader5->Text = L"时间";
			this->columnHeader5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader5->Width = 100;
			// 
			// columnHeader6
			// 
			this->columnHeader6->Text = L"日期";
			this->columnHeader6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader6->Width = 80;
			// 
			// columnHeader7
			// 
			this->columnHeader7->Text = L"操作员账号";
			this->columnHeader7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader7->Width = 100;
			// 
			// columnHeader8
			// 
			this->columnHeader8->Text = L"备注";
			this->columnHeader8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			this->columnHeader8->Width = 250;
			// 
			// button_outFund
			// 
			this->button_outFund->Location = System::Drawing::Point(155, 3);
			this->button_outFund->Name = L"button_outFund";
			this->button_outFund->Size = System::Drawing::Size(75, 23);
			this->button_outFund->TabIndex = 1;
			this->button_outFund->Text = L"出金";
			this->button_outFund->UseVisualStyleBackColor = true;
			this->button_outFund->Click += gcnew System::EventHandler(this, &FundForm::button_outFund_Click);
			// 
			// button_inFund
			// 
			this->button_inFund->Location = System::Drawing::Point(50, 3);
			this->button_inFund->Name = L"button_inFund";
			this->button_inFund->Size = System::Drawing::Size(75, 23);
			this->button_inFund->TabIndex = 0;
			this->button_inFund->Text = L"入金";
			this->button_inFund->UseVisualStyleBackColor = true;
			this->button_inFund->Click += gcnew System::EventHandler(this, &FundForm::button_inFund_Click);
			
			// 
			// FundForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(858, 292);
			this->Controls->Add(this->splitContainer1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"FundForm";
			this->Text = L"出入金";
			this->Load += gcnew System::EventHandler(this, &FundForm::FundForm_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FundForm::FundForm_FormClosed);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
    private: void LoadUser()
             {
                 comboBox_ID->Items->Clear();

                 std::map<int, UserInfo> mapUserInfo;
                 m_pDataInfo->GetUserInfo(mapUserInfo);

                 std::map<int, UserInfo>::iterator itUserInfo = mapUserInfo.begin();
                 for (;itUserInfo!=mapUserInfo.end();itUserInfo++)
                 {
                     if (itUserInfo->second.nUserType == USER_TYPE_TRADER)
                         comboBox_ID->Items->Add(Tools::string2String(itUserInfo->second.szAccount));   
                 }

                 comboBox_ID->Items->Add("*"); 
             }
    protected:virtual void WndProc( Message% m ) override{
                  if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival )
                  {
                      char *p = (char*)m.LParam.ToInt32();
                      Stru_UniPkgHead head;					
                      memset(&head, 0, sizeof(head));
                      memcpy(&head, p, sizeof(head));

                      char *strError = "操作成功";
                      bool bQuery = false;

                      switch ( head.cmdid )
                      {	 
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

                                  LoadUser();
                              }
                              else
                              {
                                  strError = (char*)(p+sizeof(Stru_UniPkgHead));
                              }
                              break;
                          }
                      case Cmd_TradeDataManager_QryFundInOut_Rsp:
                          {			
                              bQuery = true;
                              if ( head.userdata3 == CF_ERROR_SUCCESS )
                              {  
                                  m_pDataInfo->ClearFundInOut();
                                  listView1->Items->Clear();

                                  std::map<int, sFundInOut> mapFundInOut;
                                  int nCount = head.len / sizeof(sFundInOut);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      sFundInOut oFundInOut = *(sFundInOut*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(sFundInOut));

                                      int nCount = listView1->Items->Count;
                                      ListViewItem^ item = gcnew ListViewItem(Tools::string2String(oFundInOut.mUserID),nCount++); 
                                      item->SubItems->Add((0 == oFundInOut.meInOut)?"入金":"出金"); 
                                      item->SubItems->Add(oFundInOut.mdbVolume.ToString("N")); 
                                      item->SubItems->Add(Tools::string2String(oFundInOut.msTime)); 
                                      item->SubItems->Add(Tools::string2String(oFundInOut.msDay)); 
                                      item->SubItems->Add(Tools::string2String(oFundInOut.mOpAdminID));
                                      item->SubItems->Add(Tools::string2String(oFundInOut.msDesc));
                                      listView1->Items->Add( item );

                                      m_pDataInfo->SetFundInOut(oFundInOut);
                                  }                                 
                              }
                              else
                              {
                                  strError = (char*)(p+sizeof(Stru_UniPkgHead));
                              }

                              break;
                          }
                      case Cmd_TradeDataManager_FundInOut_Rsp:
                          {			
                              if ( head.userdata3 == CF_ERROR_SUCCESS )
                              {  
                                  int nCount = listView1->Items->Count;
                                  ListViewItem^ item = gcnew ListViewItem(Tools::string2String(m_pCurFundInOut->mUserID),nCount++); 
                                  item->SubItems->Add((0 == m_pCurFundInOut->meInOut)?"入金":"出金"); 
                                  item->SubItems->Add(m_pCurFundInOut->mdbVolume.ToString("N")); 
                                  item->SubItems->Add(Tools::string2String(m_pCurFundInOut->msTime)); 
                                  item->SubItems->Add(Tools::string2String(m_pCurFundInOut->msDay)); 
                                  item->SubItems->Add(Tools::string2String(m_pCurFundInOut->mOpAdminID));
                                  item->SubItems->Add(Tools::string2String(m_pCurFundInOut->msDesc));
                                  listView1->Items->Add( item );

                                  m_pDataInfo->SetFundInOut(*m_pCurFundInOut);

                              }
                              else
                              {
                                  strError = (char*)(p+sizeof(Stru_UniPkgHead));
                              }

                              break;
                          }
                      }

                      if(!bQuery)
						_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
								CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
								strError);
                  }

                  Form::WndProc ( m );
              }

    private: System::Void FundForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 this->CenterToScreen();

                 int nHwnd = this->Handle.ToInt32();

                 m_pDataInfo->GetLogonInfo(*m_pLogin);

                 CTcpLayer::SubscribePkg(Cmd_TradeDataManager_FundInOut_Rsp, nHwnd);

                 CTcpLayer::SubscribePkg(Cmd_QryUser_Rsp, nHwnd);
                 CTcpLayer::SendData(Cmd_QryUser_Req, NULL,0, 0, m_pLogin->mnUserID);

                 CTcpLayer::SubscribePkg(Cmd_TradeDataManager_QryFundInOut_Rsp, nHwnd);
             }
    private: System::Void button_inFund_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_INOUTFUND_IN));
                 FundOprForm^ dlg = gcnew FundOprForm();
                 dlg->SetOperType(FundIN);
                 if(::DialogResult::OK == dlg->ShowDialog())
                 {
                     UserInfo sUserInfo;
                     m_pDataInfo->GetUserInfoByUserID(m_pLogin->mnUserID,sUserInfo);

                     m_pCurFundInOut = dlg->GetCurFundInOut();
                     strcpy(m_pCurFundInOut->mOpAdminID,sUserInfo.szAccount);

                     CTcpLayer::SendData(Cmd_TradeDataManager_FundInOut_Req, m_pCurFundInOut, sizeof(sFundInOut), 0, m_pLogin->mnUserID);
                 }
             }
    private: System::Void button_outFund_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_INOUTFUND_OUT));
                 FundOprForm^ dlg = gcnew FundOprForm();
                 dlg->SetOperType(FundOut);
                 if(::DialogResult::OK == dlg->ShowDialog())
                 {
                     UserInfo sUserInfo;
                     m_pDataInfo->GetUserInfoByUserID(m_pLogin->mnUserID,sUserInfo);

                     m_pCurFundInOut = dlg->GetCurFundInOut();
                     strcpy(m_pCurFundInOut->mOpAdminID,sUserInfo.szAccount);

                     CTcpLayer::SendData(Cmd_TradeDataManager_FundInOut_Req, m_pCurFundInOut, sizeof(sFundInOut), 0, m_pLogin->mnUserID);
                 }
             }
    private: System::Void button_Qry_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_INOUTFUND_QUERY));
                 listView1->Items->Clear();

                 DateTime time1 = Convert::ToDateTime(dateTimePicker1->Value.Date.ToString("yyyy-MM-dd"));
                 DateTime time2 = Convert::ToDateTime(dateTimePicker2->Value.Date.ToString("yyyy-MM-dd"));    

                 if (DateTime::Compare(time1,time2)>0)
                 {
                     MessageBox::Show("查询起始的日期不能大于终止的日期");
                     return;
                 }

                 string  timeStart = Tools::String2string(dateTimePicker1->Value.Date.ToString("yyyy-MM-dd"));
                 string  timeEnd   = Tools::String2string(dateTimePicker2->Value.Date.ToString("yyyy-MM-dd"));

                 sQryFundInOut oQryFundInOut;
                 memset(&oQryFundInOut,0,sizeof(oQryFundInOut));
                 if(String::IsNullOrEmpty(comboBox_ID->Text))
                     strcpy(oQryFundInOut.mUserID,"*");
                 else
                     strcpy(oQryFundInOut.mUserID,Tools::String2string(comboBox_ID->Text).c_str());
                 strcpy(oQryFundInOut.msStartDay,timeStart.c_str());
                 strcpy(oQryFundInOut.msEndDay,timeEnd.c_str());

                 CTcpLayer::SendData(Cmd_TradeDataManager_QryFundInOut_Req, &oQryFundInOut, sizeof(sQryFundInOut), 0, m_pLogin->mnUserID);
             }
    private: System::Void FundForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
                 int nHwnd = this->Handle.ToInt32();
                 CTcpLayer::UnsubscribeAllPkg(nHwnd);
             }
	
};
}

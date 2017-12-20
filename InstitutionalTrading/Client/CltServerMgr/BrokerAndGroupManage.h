#pragma once

#include <map>
#include "BrokerInfoOper.h"
#include "ServerGroupForm.h"
#include "QuotSetupForm.h"
#include "RefOtherClass.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

	/// <summary>
	/// Summary for BrokerAndGroupManage
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class BrokerAndGroupManage : public System::Windows::Forms::Form
	{
	public:
		BrokerAndGroupManage(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pLogin = new sLoginRsp();
			m_pDataInfo = CDataInfo::NewInstance();
			m_pCurBrokerInfo = new BrokerInfo;
			m_pCurServerGroup = new ServerGroup;
			m_pCurServerAddr = new ServerAddress;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~BrokerAndGroupManage()
		{
			if (components)
			{
				delete components;
			}

			CDataInfo::DestroyInstance();
			if ( NULL != m_pCurBrokerInfo )
			{
				delete m_pCurBrokerInfo;
				m_pCurBrokerInfo = NULL;
			}

			if ( NULL != m_pCurServerGroup )
			{
				delete m_pCurServerGroup;
				m_pCurServerGroup = NULL;
			}

			if ( NULL != m_pCurServerAddr )
			{
				delete m_pCurServerAddr;
				m_pCurServerAddr = NULL;
			}
            if(m_pLogin)
            {
                delete m_pLogin;
                m_pLogin = nullptr;
            }
		}

    private: String^ m_szQuotAccout;
    private: String^ m_szQuotPsw;
    private: sLoginRsp *m_pLogin;
    private: CDataInfo* m_pDataInfo;  
    private: BrokerInfo* m_pCurBrokerInfo;
    private: ServerGroup* m_pCurServerGroup;
    private: ServerAddress* m_pCurServerAddr;
    private: eTradeDataStatus m_tradeDataStatus;


    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::ComboBox^  comboBoxBrokerList;
    private: System::Windows::Forms::Button^  buttonAddBroker;
    private: System::Windows::Forms::Button^  buttonModifyBroker;
    private: System::Windows::Forms::Button^  buttonDelBroker;
    private: System::Windows::Forms::Button^  buttonAddServerAddr;
    private: System::Windows::Forms::Button^  buttonDelServerAddr;
    private: System::Windows::Forms::Button^  buttonDelServerGroup;
    private: System::Windows::Forms::Button^  buttonAddServerGroup;
    private: System::Windows::Forms::DataGridView^  dataGridViewAddr;
    private: System::Windows::Forms::DataGridView^  dataGridViewGroup;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn1;
    private: System::Windows::Forms::DataGridViewCheckBoxColumn^  dataGridViewCheckBoxColumn1;
    private: System::Windows::Forms::ListView^  listViewGroup;
    private: System::Windows::Forms::ColumnHeader^  columnHeader1;
    private: System::Windows::Forms::ColumnHeader^  columnHeader2;
    private: System::Windows::Forms::Button^  button_Modify;
    private: System::Windows::Forms::ListView^  listViewAddr;
    private: System::Windows::Forms::ColumnHeader^  columnHeader3;
    private: System::Windows::Forms::ColumnHeader^  columnHeader4;
    private: System::Windows::Forms::Button^  buttonModify;
    private: System::Windows::Forms::Button^  buttonQuotSetup;
private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->comboBoxBrokerList = (gcnew System::Windows::Forms::ComboBox());
            this->buttonAddBroker = (gcnew System::Windows::Forms::Button());
            this->buttonModifyBroker = (gcnew System::Windows::Forms::Button());
            this->buttonDelBroker = (gcnew System::Windows::Forms::Button());
            this->buttonAddServerAddr = (gcnew System::Windows::Forms::Button());
            this->buttonDelServerAddr = (gcnew System::Windows::Forms::Button());
            this->buttonDelServerGroup = (gcnew System::Windows::Forms::Button());
            this->buttonAddServerGroup = (gcnew System::Windows::Forms::Button());
            this->dataGridViewAddr = (gcnew System::Windows::Forms::DataGridView());
            this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->dataGridViewGroup = (gcnew System::Windows::Forms::DataGridView());
            this->dataGridViewTextBoxColumn1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->dataGridViewCheckBoxColumn1 = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
            this->listViewGroup = (gcnew System::Windows::Forms::ListView());
            this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
            this->button_Modify = (gcnew System::Windows::Forms::Button());
            this->listViewAddr = (gcnew System::Windows::Forms::ListView());
            this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
            this->buttonModify = (gcnew System::Windows::Forms::Button());
            this->buttonQuotSetup = (gcnew System::Windows::Forms::Button());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewAddr))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewGroup))->BeginInit();
            this->SuspendLayout();
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(15, 22);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(53, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"经纪公司";
            // 
            // comboBoxBrokerList
            // 
            this->comboBoxBrokerList->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->comboBoxBrokerList->FormattingEnabled = true;
            this->comboBoxBrokerList->Location = System::Drawing::Point(73, 17);
            this->comboBoxBrokerList->Name = L"comboBoxBrokerList";
            this->comboBoxBrokerList->Size = System::Drawing::Size(145, 20);
            this->comboBoxBrokerList->TabIndex = 1;
            this->comboBoxBrokerList->SelectedIndexChanged += gcnew System::EventHandler(this, &BrokerAndGroupManage::comboBoxBrokerList_SelectedIndexChanged);
            // 
            // buttonAddBroker
            // 
            this->buttonAddBroker->Location = System::Drawing::Point(251, 17);
            this->buttonAddBroker->Name = L"buttonAddBroker";
            this->buttonAddBroker->Size = System::Drawing::Size(48, 23);
            this->buttonAddBroker->TabIndex = 2;
            this->buttonAddBroker->Text = L"增加";
            this->buttonAddBroker->UseVisualStyleBackColor = true;
            this->buttonAddBroker->Click += gcnew System::EventHandler(this, &BrokerAndGroupManage::buttonAddBroker_Click);
            // 
            // buttonModifyBroker
            // 
            this->buttonModifyBroker->Location = System::Drawing::Point(305, 17);
            this->buttonModifyBroker->Name = L"buttonModifyBroker";
            this->buttonModifyBroker->Size = System::Drawing::Size(48, 23);
            this->buttonModifyBroker->TabIndex = 3;
            this->buttonModifyBroker->Text = L"修改";
            this->buttonModifyBroker->UseVisualStyleBackColor = true;
            this->buttonModifyBroker->Click += gcnew System::EventHandler(this, &BrokerAndGroupManage::buttonModifyBroker_Click);
            // 
            // buttonDelBroker
            // 
            this->buttonDelBroker->Location = System::Drawing::Point(359, 17);
            this->buttonDelBroker->Name = L"buttonDelBroker";
            this->buttonDelBroker->Size = System::Drawing::Size(48, 23);
            this->buttonDelBroker->TabIndex = 4;
            this->buttonDelBroker->Text = L"删除";
            this->buttonDelBroker->UseVisualStyleBackColor = true;
            this->buttonDelBroker->Click += gcnew System::EventHandler(this, &BrokerAndGroupManage::buttonDelBroker_Click);
            // 
            // buttonAddServerAddr
            // 
            this->buttonAddServerAddr->Location = System::Drawing::Point(334, 312);
            this->buttonAddServerAddr->Name = L"buttonAddServerAddr";
            this->buttonAddServerAddr->Size = System::Drawing::Size(48, 23);
            this->buttonAddServerAddr->TabIndex = 8;
            this->buttonAddServerAddr->Text = L"增加";
            this->buttonAddServerAddr->UseVisualStyleBackColor = true;
            this->buttonAddServerAddr->Click += gcnew System::EventHandler(this, &BrokerAndGroupManage::buttonAddServerAddr_Click);
            // 
            // buttonDelServerAddr
            // 
            this->buttonDelServerAddr->Location = System::Drawing::Point(458, 312);
            this->buttonDelServerAddr->Name = L"buttonDelServerAddr";
            this->buttonDelServerAddr->Size = System::Drawing::Size(48, 23);
            this->buttonDelServerAddr->TabIndex = 9;
            this->buttonDelServerAddr->Text = L"删除";
            this->buttonDelServerAddr->UseVisualStyleBackColor = true;
            this->buttonDelServerAddr->Click += gcnew System::EventHandler(this, &BrokerAndGroupManage::buttonDelServerAddr_Click);
            // 
            // buttonDelServerGroup
            // 
            this->buttonDelServerGroup->Location = System::Drawing::Point(150, 312);
            this->buttonDelServerGroup->Name = L"buttonDelServerGroup";
            this->buttonDelServerGroup->Size = System::Drawing::Size(48, 23);
            this->buttonDelServerGroup->TabIndex = 11;
            this->buttonDelServerGroup->Text = L"删除";
            this->buttonDelServerGroup->UseVisualStyleBackColor = true;
            this->buttonDelServerGroup->Click += gcnew System::EventHandler(this, &BrokerAndGroupManage::buttonDelServerGroup_Click);
            // 
            // buttonAddServerGroup
            // 
            this->buttonAddServerGroup->Location = System::Drawing::Point(30, 312);
            this->buttonAddServerGroup->Name = L"buttonAddServerGroup";
            this->buttonAddServerGroup->Size = System::Drawing::Size(48, 23);
            this->buttonAddServerGroup->TabIndex = 10;
            this->buttonAddServerGroup->Text = L"增加";
            this->buttonAddServerGroup->UseVisualStyleBackColor = true;
            this->buttonAddServerGroup->Click += gcnew System::EventHandler(this, &BrokerAndGroupManage::buttonAddServerGroup_Click);
            // 
            // dataGridViewAddr
            // 
            this->dataGridViewAddr->AllowUserToAddRows = false;
            this->dataGridViewAddr->AllowUserToDeleteRows = false;
            this->dataGridViewAddr->AllowUserToResizeRows = false;
            this->dataGridViewAddr->BackgroundColor = System::Drawing::SystemColors::ActiveCaptionText;
            dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle1->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle1->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle1->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle1->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridViewAddr->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            this->dataGridViewAddr->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridViewAddr->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {this->Column1, 
                this->Column2});
            this->dataGridViewAddr->Location = System::Drawing::Point(191, 211);
            this->dataGridViewAddr->MultiSelect = false;
            this->dataGridViewAddr->Name = L"dataGridViewAddr";
            this->dataGridViewAddr->RowHeadersVisible = false;
            this->dataGridViewAddr->RowTemplate->Height = 18;
            this->dataGridViewAddr->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridViewAddr->Size = System::Drawing::Size(234, 22);
            this->dataGridViewAddr->TabIndex = 12;
            this->dataGridViewAddr->Visible = false;
            // 
            // Column1
            // 
            this->Column1->HeaderText = L"交易服务器地址";
            this->Column1->MaxInputLength = 60;
            this->Column1->Name = L"Column1";
            this->Column1->Width = 145;
            // 
            // Column2
            // 
            this->Column2->HeaderText = L"端口号";
            this->Column2->MaxInputLength = 6;
            this->Column2->Name = L"Column2";
            this->Column2->Width = 85;
            // 
            // dataGridViewGroup
            // 
            this->dataGridViewGroup->AllowUserToAddRows = false;
            this->dataGridViewGroup->AllowUserToDeleteRows = false;
            this->dataGridViewGroup->AllowUserToResizeRows = false;
            this->dataGridViewGroup->BackgroundColor = System::Drawing::SystemColors::ActiveCaptionText;
            dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle2->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle2->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridViewGroup->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
            this->dataGridViewGroup->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridViewGroup->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {this->dataGridViewTextBoxColumn1, 
                this->dataGridViewCheckBoxColumn1});
            this->dataGridViewGroup->Location = System::Drawing::Point(91, 146);
            this->dataGridViewGroup->MultiSelect = false;
            this->dataGridViewGroup->Name = L"dataGridViewGroup";
            this->dataGridViewGroup->RowHeadersVisible = false;
            this->dataGridViewGroup->RowTemplate->Height = 18;
            this->dataGridViewGroup->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridViewGroup->Size = System::Drawing::Size(270, 22);
            this->dataGridViewGroup->StandardTab = true;
            this->dataGridViewGroup->TabIndex = 12;
            this->dataGridViewGroup->Visible = false;
            // 
            // dataGridViewTextBoxColumn1
            // 
            this->dataGridViewTextBoxColumn1->HeaderText = L"交易服务器组名称";
            this->dataGridViewTextBoxColumn1->MaxInputLength = 20;
            this->dataGridViewTextBoxColumn1->Name = L"dataGridViewTextBoxColumn1";
            this->dataGridViewTextBoxColumn1->Width = 132;
            // 
            // dataGridViewCheckBoxColumn1
            // 
            this->dataGridViewCheckBoxColumn1->HeaderText = L"结算使用";
            this->dataGridViewCheckBoxColumn1->Name = L"dataGridViewCheckBoxColumn1";
            this->dataGridViewCheckBoxColumn1->Width = 132;
            // 
            // listViewGroup
            // 
            this->listViewGroup->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->columnHeader1, 
                this->columnHeader2});
            this->listViewGroup->FullRowSelect = true;
            this->listViewGroup->GridLines = true;
            this->listViewGroup->Location = System::Drawing::Point(12, 46);
            this->listViewGroup->MultiSelect = false;
            this->listViewGroup->Name = L"listViewGroup";
            this->listViewGroup->Size = System::Drawing::Size(206, 259);
            this->listViewGroup->TabIndex = 13;
            this->listViewGroup->UseCompatibleStateImageBehavior = false;
            this->listViewGroup->View = System::Windows::Forms::View::Details;
            this->listViewGroup->SelectedIndexChanged += gcnew System::EventHandler(this, &BrokerAndGroupManage::listViewGroup_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this->columnHeader1->Text = L"交易服务器组名称";
            this->columnHeader1->Width = 120;
            // 
            // columnHeader2
            // 
            this->columnHeader2->Text = L"结算使用";
            this->columnHeader2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader2->Width = 70;
            // 
            // button_Modify
            // 
            this->button_Modify->Location = System::Drawing::Point(89, 312);
            this->button_Modify->Name = L"button_Modify";
            this->button_Modify->Size = System::Drawing::Size(48, 23);
            this->button_Modify->TabIndex = 14;
            this->button_Modify->Text = L"修改";
            this->button_Modify->UseVisualStyleBackColor = true;
            this->button_Modify->Click += gcnew System::EventHandler(this, &BrokerAndGroupManage::button_ModifyServerGroup_Click);
            // 
            // listViewAddr
            // 
            this->listViewAddr->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->columnHeader3, 
                this->columnHeader4});
            this->listViewAddr->FullRowSelect = true;
            this->listViewAddr->GridLines = true;
            this->listViewAddr->Location = System::Drawing::Point(238, 47);
            this->listViewAddr->MultiSelect = false;
            this->listViewAddr->Name = L"listViewAddr";
            this->listViewAddr->Size = System::Drawing::Size(360, 259);
            this->listViewAddr->TabIndex = 15;
            this->listViewAddr->UseCompatibleStateImageBehavior = false;
            this->listViewAddr->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader3
            // 
            this->columnHeader3->Text = L"交易服务器地址";
            this->columnHeader3->Width = 260;
            // 
            // columnHeader4
            // 
            this->columnHeader4->Text = L"端口号";
            this->columnHeader4->Width = 80;
            // 
            // buttonModify
            // 
            this->buttonModify->Location = System::Drawing::Point(394, 312);
            this->buttonModify->Name = L"buttonModify";
            this->buttonModify->Size = System::Drawing::Size(48, 23);
            this->buttonModify->TabIndex = 16;
            this->buttonModify->Text = L"修改";
            this->buttonModify->UseVisualStyleBackColor = true;
            this->buttonModify->Click += gcnew System::EventHandler(this, &BrokerAndGroupManage::buttonModifyServerAddr_Click);
            // 
            // buttonQuotSetup
            // 
            this->buttonQuotSetup->Location = System::Drawing::Point(443, 17);
            this->buttonQuotSetup->Name = L"buttonQuotSetup";
            this->buttonQuotSetup->Size = System::Drawing::Size(120, 23);
            this->buttonQuotSetup->TabIndex = 17;
            this->buttonQuotSetup->Text = L"配置行情服务器";
            this->buttonQuotSetup->UseVisualStyleBackColor = true;
            this->buttonQuotSetup->Click += gcnew System::EventHandler(this, &BrokerAndGroupManage::buttonQuotSetup_Click);
            // 
            // BrokerAndGroupManage
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(610, 347);
            this->Controls->Add(this->buttonQuotSetup);
            this->Controls->Add(this->buttonModify);
            this->Controls->Add(this->listViewAddr);
            this->Controls->Add(this->button_Modify);
            this->Controls->Add(this->listViewGroup);
            this->Controls->Add(this->buttonDelServerGroup);
            this->Controls->Add(this->buttonAddServerGroup);
            this->Controls->Add(this->buttonDelServerAddr);
            this->Controls->Add(this->buttonAddServerAddr);
            this->Controls->Add(this->buttonDelBroker);
            this->Controls->Add(this->buttonModifyBroker);
            this->Controls->Add(this->buttonAddBroker);
            this->Controls->Add(this->comboBoxBrokerList);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->dataGridViewGroup);
            this->Controls->Add(this->dataGridViewAddr);
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"BrokerAndGroupManage";
            this->Text = L"经纪公司和服务器组管理";
            this->Load += gcnew System::EventHandler(this, &BrokerAndGroupManage::BrokerAndGroupManage_Load);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &BrokerAndGroupManage::BrokerAndGroupManage_FormClosed);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewAddr))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridViewGroup))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

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
               case Cmd_ModifyBrokerInfoQuot_Rsp:
                   {
                       if ( head.userdata1 == CF_ERROR_SUCCESS )
                       {
                           int nBrokerID = *(int*)(p+sizeof(Stru_UniPkgHead));
                           m_pDataInfo->SetBrokerInfoQuot(nBrokerID,
                               Tools::String2string(m_szQuotAccout).c_str(),
                               Tools::String2string(m_szQuotPsw).c_str());   
                       }
                       else
                       {
                           strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                       }

                       break;
                   }

			   case Cmd_QueryBrokerInfo_Rsp:
				   {
                       bShowMsg = false;
					   if ( head.userdata1 == CF_ERROR_SUCCESS )
                       {                
                           m_pDataInfo->ClearBrokerInfos();
                           comboBoxBrokerList->Items->Clear();

						   std::map<int, BrokerInfo> mapBrokerInfo;
						   int nCount = head.len / sizeof(BrokerInfo);
						   for ( int i = 0; i < nCount; i++ )
						   {
							   BrokerInfo* pBrokerInfo = (BrokerInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(BrokerInfo));
							   mapBrokerInfo[pBrokerInfo->nBrokerID] = *pBrokerInfo;

							   BrokerData^ data = gcnew BrokerData(pBrokerInfo->nBrokerID,pBrokerInfo->nType,
                                                                   Tools::string2String(pBrokerInfo->szBrokerCode),
								                                   Tools::string2String(pBrokerInfo->szCompany),
                                                                   Tools::string2String(pBrokerInfo->szAccount),
                                                                   Tools::string2String(pBrokerInfo->szPassword));
							   comboBoxBrokerList->Items->Add(data);
						   }

                           //默认选中第一个
                           if(nCount>0)
                           {
                               comboBoxBrokerList->SelectedIndex = 0;
    						   m_pDataInfo->SetBrokerInfos(mapBrokerInfo);
                           }
					   }
					   else
					   {
                           strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
					   }

					   break;
				   }
               case Cmd_AddBrokerInfo_Rsp:
				   {
					   if ( head.userdata1 == CF_ERROR_SUCCESS )
					   {
						   m_pCurBrokerInfo->nBrokerID = *(int*)(p+sizeof(Stru_UniPkgHead));
						   m_pDataInfo->SetBrokerInfo(*m_pCurBrokerInfo);

						   BrokerData^ data = gcnew BrokerData(m_pCurBrokerInfo->nBrokerID, m_pCurBrokerInfo->nType,
                                                               Tools::string2String(m_pCurBrokerInfo->szBrokerCode),
                                                               Tools::string2String(m_pCurBrokerInfo->szCompany),
                                                               Tools::string2String(m_pCurBrokerInfo->szAccount),
                                                               Tools::string2String(m_pCurBrokerInfo->szPassword));
						   comboBoxBrokerList->Items->Add(data);
                           comboBoxBrokerList->SelectedItem = data;

                           strError = "增加经纪公司操作成功";
					   }
					   else
					   {
                           strError = "增加经纪公司操作失败,失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
					   }

					   break;
				   }
			   case Cmd_ModifyBrokerInfo_Rsp:
				   {
					   if ( head.userdata1 == CF_ERROR_SUCCESS )
					   {
						   int nBrokerID = *(int*)(p+sizeof(Stru_UniPkgHead));
						   if ( nBrokerID == m_pCurBrokerInfo->nBrokerID )
						   {
							   m_pDataInfo->SetBrokerInfo(*m_pCurBrokerInfo);

							   int nCount = comboBoxBrokerList->Items->Count;
							   int nSelectIndex = comboBoxBrokerList->SelectedIndex;
							   for ( int i = 0; i < nCount; i++ )
							   {
								   BrokerData^ data = (BrokerData^)comboBoxBrokerList->Items[i];
								   if ( data->GetBrokerID() == nBrokerID )
								   {
                                       data->m_strCode = Tools::string2String(m_pCurBrokerInfo->szBrokerCode);
									   data->m_strCompany = Tools::string2String(m_pCurBrokerInfo->szCompany);
									   comboBoxBrokerList->Items->RemoveAt(i);
									   comboBoxBrokerList->Items->Insert(i, data);
									   break;
								   }
							   }

							   comboBoxBrokerList->SelectedIndex = nSelectIndex;
						   }

                           strError = "修改经纪公司操作成功";
					   }
					   else
					   {
						   strError = "修改经纪公司操作失败,失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
					   }

					   break;
				   }
			   case Cmd_DelBrokerInfo_Rsp:
				   {
					   if ( head.userdata1 == CF_ERROR_SUCCESS )
					   {
                           listViewGroup->Items->Clear();
                           listViewAddr->Items->Clear();

						   int nBrokerID = *(int*)(p+sizeof(Stru_UniPkgHead));
						   m_pDataInfo->DelBrokerInfo(nBrokerID);

						   int nCount = comboBoxBrokerList->Items->Count;
						   for ( int i = 0; i < nCount; i++ )
						   {
							   BrokerData^ data = (BrokerData^)comboBoxBrokerList->Items[i];
							   if ( data->GetBrokerID() == nBrokerID )
							   {
								   comboBoxBrokerList->Items->RemoveAt(i);
								   break;
							   }
						   }

                           nCount = comboBoxBrokerList->Items->Count;
                           if(nCount>0)
                                comboBoxBrokerList->SelectedIndex = 0;

                           strError = "删除经纪公司操作成功";
					   }
					   else
					   {
						   strError = "删除经纪公司操作失败,失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
					   }

					   break;
				   }
               case Cmd_QueryServerGroup_Rsp:
				   {
                       bShowMsg = false;
                       if( head.userdata1 == CF_ERROR_SUCCESS )
                       {
                           m_pDataInfo->ClearServerGroups();
                           listViewGroup->Items->Clear();

                           int nCount = head.len / sizeof(ServerGroup);
                           for(int i=0,j=0;i<nCount;i++)
                           {
                                ServerGroup* pServerGroup = (ServerGroup*)((char*)(p+sizeof(Stru_UniPkgHead))+i*sizeof(ServerGroup));
                                m_pDataInfo->SetServerGroup(pServerGroup->nBrokerID,*pServerGroup);

                                
                                if(comboBoxBrokerList->SelectedIndex >= 0)
                                {
                                    BrokerData^ data = (BrokerData^)comboBoxBrokerList->SelectedItem;
                                    if(data->GetBrokerID() == pServerGroup->nBrokerID)
                                    {
                                        string strSettle = (pServerGroup->nIsSettle)?"是":"否";

                                        ListViewItem^ item = gcnew ListViewItem(Tools::string2String(pServerGroup->szServerName),j++); 
                                        item->Tag = pServerGroup->nServerID;
                                        item->SubItems->Add(Tools::string2String(strSettle.c_str()));  
                                        listViewGroup->Items->Add( item );                                        
                                    }
                                }
                           }

                           if(nCount>0)
                                listViewGroup->Items[0]->Selected = true;
                       }
					   else
					   {
                           strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
    				   }

					   break;
				   }
			   case Cmd_ModifyServerGroup_Rsp:
				   {
					   if ( head.userdata1 == CF_ERROR_SUCCESS )
					   {
						   int nGroupID = *(int*)(p+sizeof(Stru_UniPkgHead));
						   if ( nGroupID == m_pCurServerGroup->nServerID )
						   {
							   m_pDataInfo->SetServerGroup(m_pCurServerGroup->nBrokerID, *m_pCurServerGroup);
						   }

                           listViewGroup->SelectedItems[0]->SubItems[0]->Text = Tools::string2String(m_pCurServerGroup->szServerName);
                           listViewGroup->SelectedItems[0]->SubItems[1]->Text = (m_pCurServerGroup->nIsSettle)?"是":"否";

                           strError = "修改服务器组操作成功";
					   }
					   else
					   {
						   strError = "修改服务器组操作失败,失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
					   }

					   break;
				   }
			   case Cmd_AddServerGroup_Rsp:
				   {
					   if ( head.userdata1 == CF_ERROR_SUCCESS )
					   {
						   m_pCurServerGroup->nServerID = *(int*)(p+sizeof(Stru_UniPkgHead));
						   m_pDataInfo->SetServerGroup(m_pCurServerGroup->nBrokerID, *m_pCurServerGroup);

                           int nCount = listViewGroup->Items->Count;
                           ListViewItem^ item = gcnew ListViewItem( Tools::string2String(m_pCurServerGroup->szServerName),nCount);
                           item->Tag = m_pCurServerGroup->nServerID;
                           item->SubItems->Add( Tools::string2String((m_pCurServerGroup->nIsSettle)?"是":"否"));
                           listViewGroup->Items->Add( item );

                           listViewGroup->Focus();
                           listViewGroup->Items[nCount]->Selected= true;

                           strError = "增加服务器组操作成功";
					   }
					   else
					   {
						   strError = "增加服务器组操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
					   }

					   break;
				   }
			   case Cmd_DelServerGroup_Rsp:
				   {
					   if ( head.userdata1 == CF_ERROR_SUCCESS )
					   {
						   int nServerID = *(int*)(p+sizeof(Stru_UniPkgHead));
						   if ( nServerID == m_pCurServerGroup->nServerID)
						   {
							   m_pDataInfo->DelServerGroup(m_pCurServerGroup->nBrokerID, m_pCurServerGroup->nServerID);
						   }

                           ListView::SelectedIndexCollection^ c = listViewGroup->SelectedIndices;
                           if(c != nullptr)
                               listViewGroup->Items->RemoveAt(c[0]);  

						   dataGridViewAddr->Rows->Clear();
					   }
					   else
					   {
                           strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
					   }

					   break;
				   }
			   case Cmd_QueryServerAddress_Rsp:
				   {
                       bShowMsg = false;
                       listViewAddr->Items->Clear();
					   if ( head.userdata1 == CF_ERROR_SUCCESS )
					   {
                           int j=0;
						   int nCount = head.len / sizeof(ServerAddress);
						   for ( int i = 0,j=0; i < nCount; i++ )
						   {
							   ServerAddress* pServerAddr = (ServerAddress*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(ServerAddress));
							   m_pDataInfo->SetServerAddress(pServerAddr->nServerID, *pServerAddr);

                               ListViewItem^ item   = listViewGroup->Items[listViewGroup->SelectedIndices[0]];
                               if((int)( item->Tag) == pServerAddr->nServerID)                               
                               {
                                   ListViewItem^ item = gcnew ListViewItem(Tools::string2String(pServerAddr->szIP),j++); 
                                   item->Tag = pServerAddr->nAddrID;
                                   item->SubItems->Add(pServerAddr->nPort.ToString());  
                                   listViewAddr->Items->Add( item );    
                               }                               
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

                           std::map<int, TradeAccount> mapTradeAccount;
                           int nCount = head.len / sizeof(TradeAccount);
                           for ( int i = 0; i < nCount; i++ )
                           {
                               TradeAccount* pTradeAccount = (TradeAccount*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(TradeAccount));
                               mapTradeAccount[pTradeAccount->nTradeAccountID] = *pTradeAccount;                                     
                           }
                           
                           if(nCount>0)
                                m_pDataInfo->SetTradeAccount(mapTradeAccount);
                       }
                       else
                       {
                           strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                       }

                       break;
                   }
			   case Cmd_ModifyServerAddress_Rsp:
                   {
                       if ( head.userdata1 == CF_ERROR_SUCCESS )
                       {
                           int nAddrID = *(int*)(p+sizeof(Stru_UniPkgHead));
                           if ( nAddrID == m_pCurServerAddr->nAddrID )
                           {
                               m_pDataInfo->SetServerAddress(m_pCurServerAddr->nServerID, *m_pCurServerAddr);
                           }

                           listViewAddr->SelectedItems[0]->SubItems[0]->Text = Tools::string2String(m_pCurServerAddr->szIP);
                           listViewAddr->SelectedItems[0]->SubItems[1]->Text = m_pCurServerAddr->nPort.ToString();

                           strError = "修改服务器地址成功";
                       }
                       else
                       {
                           strError = "修改服务器地址失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                       }

                       break;
                   }
			   case Cmd_AddServerAddress_Rsp:
				   {
					   if ( head.userdata1 == CF_ERROR_SUCCESS )
					   {
						   m_pCurServerAddr->nAddrID = *(int*)(p+sizeof(Stru_UniPkgHead));
						   m_pDataInfo->SetServerAddress(m_pCurServerAddr->nServerID, *m_pCurServerAddr);

                           int nCount = listViewAddr->Items->Count;
                           ListViewItem^ item = gcnew ListViewItem( Tools::string2String(m_pCurServerAddr->szIP),nCount);
                           item->Tag = m_pCurServerAddr->nAddrID;
                           item->SubItems->Add( m_pCurServerAddr->nPort.ToString());
                           listViewAddr->Items->Add( item );

                           listViewAddr->Focus();                           
                           listViewAddr->Items[nCount]->Selected= true;

                           strError = "增加服务器地址成功";
				       }
					   else
					   {
						   strError = "增加服务器地址失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));

						   int nCount = dataGridViewAddr->Rows->Count;
						   if ( nCount > 0 )
						   {
							   DataGridViewRow^ dataGridViewRow = dataGridViewAddr->Rows[nCount-1];
							   dataGridViewRow->Selected = true;
						   }
					   }

					   break;
				   }
               case Cmd_DelServerAddress_Rsp:
				   {
					   if ( head.userdata1 == CF_ERROR_SUCCESS )
					   {
						   int nAddrID = *(int*)(p+sizeof(Stru_UniPkgHead));
						   if ( nAddrID == m_pCurServerAddr->nAddrID)
						   {
							   m_pDataInfo->DelServerAddress(m_pCurServerAddr->nServerID,
								   m_pCurServerAddr->nAddrID);
						   }

                           ListView::SelectedIndexCollection^ c = listViewAddr->SelectedIndices;
                           if(c != nullptr)
                               listViewAddr->Items->RemoveAt(c[0]); 

                           strError = "删除服务器地址成功";
					   }
					   else
					   {
                           strError = "删除服务器地址失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
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
private: System::Void BrokerAndGroupManage_Load(System::Object^  sender, System::EventArgs^  e) {
             this->CenterToParent();

             int nHwnd = this->Handle.ToInt32();

             m_pDataInfo->GetLogonInfo(*m_pLogin);

             CTcpLayer::SubscribePkg(Cmd_AddBrokerInfo_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_ModifyBrokerInfo_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_DelBrokerInfo_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_QueryBrokerInfo_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_AddServerGroup_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_ModifyServerGroup_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_DelServerGroup_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_QueryServerGroup_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_AddServerAddress_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_ModifyServerAddress_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_DelServerAddress_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_QueryServerAddress_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_QueryTradeAccount_Rsp, nHwnd);  
             CTcpLayer::SubscribePkg(Cmd_ModifyBrokerInfoQuot_Rsp, nHwnd);

             if ( !CTcpLayer::SendData(Cmd_QueryBrokerInfo_Req, NULL, 0, 0, m_pLogin->mnUserID)
                 || (!CTcpLayer::SendData(Cmd_QueryServerGroup_Req, NULL, 0, 0, m_pLogin->mnUserID))
                 || (!CTcpLayer::SendData(Cmd_QueryServerAddress_Req, NULL, 0, 0, m_pLogin->mnUserID))
                 || (!CTcpLayer::SendData(Cmd_QueryTradeAccount_Req, NULL, 0, 0, m_pLogin->mnUserID)))
             {
                 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
             }
         }
private: System::Void BrokerAndGroupManage_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 int nHwnd = this->Handle.ToInt32();
			 CTcpLayer::UnsubscribeAllPkg(nHwnd);
    	 }

private: System::Void buttonAddBroker_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_BROKER_ADD));
			 BrokerInfoOper^ dlg = gcnew BrokerInfoOper();
			 dlg->SetOperType(Oper_Add_Trade);
			 if( ::DialogResult::OK == dlg->ShowDialog(this))
			 {
				 *m_pCurBrokerInfo = *(dlg->GetCurBrokerInfo());
				 if(!CTcpLayer::SendData(Cmd_AddBrokerInfo_Req, m_pCurBrokerInfo, sizeof(BrokerInfo), 0, m_pLogin->mnUserID))
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }
			 }
		 }
private: System::Void buttonModifyBroker_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_BROKER_MODIFY));
			 if ( comboBoxBrokerList->SelectedIndex < 0 )
			 {
				 MessageBox::Show("请先选择一个经纪公司！", "提示",MessageBoxButtons::OK);
				 return;
			 }

			 BrokerData^ data = (BrokerData^)comboBoxBrokerList->SelectedItem;
			 m_pCurBrokerInfo->nBrokerID = data->GetBrokerID();
			 strcpy(m_pCurBrokerInfo->szBrokerCode, Tools::String2string(data->m_strCode).c_str());
			 strcpy(m_pCurBrokerInfo->szCompany, Tools::String2string(data->m_strCompany).c_str());
			 BrokerInfoOper^ dlg = gcnew BrokerInfoOper();
			 dlg->SetOperType(Oper_Modify_Trade);
			 dlg->SetCurBrokerInfo(*m_pCurBrokerInfo);
			 if( ::DialogResult::OK == dlg->ShowDialog(this))
			 {
                 if(*m_pCurBrokerInfo == *(dlg->GetCurBrokerInfo()))
                     return;
                 else
                 {
				     *m_pCurBrokerInfo = *(dlg->GetCurBrokerInfo());
				     CTcpLayer::SendData(Cmd_ModifyBrokerInfo_Req, m_pCurBrokerInfo, sizeof(BrokerInfo), 0, m_pLogin->mnUserID);
				 }
			 }
		 }
private: System::Void buttonDelBroker_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_BROKER_DEL));
             if(::DialogResult::Cancel ==  MessageBox::Show("此操作将会同步删除经纪公司和委托交易账号的关系，经纪公司和服务器组的关系，服务器组和服务器地址的关系，使用的行情服务器。确认吗？", "提示", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
                 return;

			 if ( comboBoxBrokerList->SelectedIndex < 0 )
			 {
				 MessageBox::Show("请先选择一个经纪公司！", "提示",MessageBoxButtons::OK);
				 return;
			 }

             BrokerData^ data = (BrokerData^)comboBoxBrokerList->SelectedItem;
             int nBrokerID = data->GetBrokerID();

			 if(!CTcpLayer::SendData(Cmd_DelBrokerInfo_Req, &nBrokerID, sizeof(int), 0, m_pLogin->mnUserID))
			 {
				 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			 }
		 }
private: System::Void buttonQuotSetup_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_QUOTSERVER_CONFIG));
             QuotSetupForm^ dlg = gcnew QuotSetupForm;
             if (::DialogResult::OK == dlg->ShowDialog(this))
             {
                 m_szQuotAccout = dlg->GetAccount();
                 m_szQuotPsw = dlg->GetPsw();
             }
         }
private: System::Void comboBoxBrokerList_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             listViewGroup->Items->Clear();	
             listViewAddr->Items->Clear();

			 BrokerData^ data = (BrokerData^)comboBoxBrokerList->SelectedItem;

			 std::map<int, ServerGroup> mapServerGroup;
			 m_pDataInfo->GetServerGroups(data->GetBrokerID(), mapServerGroup);
             if(mapServerGroup.empty())             
                 return;             

			 std::map<int, ServerGroup>::iterator it = mapServerGroup.begin();
			 for ( int i = 0; it != mapServerGroup.end(); it++,i++ )
			 {
                 ListViewItem^ item = gcnew ListViewItem( Tools::string2String(it->second.szServerName),i);
                 item->Tag = it->second.nServerID;
                 item->SubItems->Add( Tools::string2String((it->second.nIsSettle)?"是":"否"));
                 listViewGroup->Items->Add( item ); 
			 }

             listViewGroup->Focus();
             listViewGroup->Items[0]->Selected = true;
            
             ListViewItem^ item   = listViewGroup->Items[listViewGroup->SelectedIndices[0]];
             int nServerID = (int)(item->Tag);
             std::map<int,ServerAddress> mapServerAddress;
             m_pDataInfo->GetServerAddress(nServerID, mapServerAddress);
             if(mapServerAddress.empty())
                 return;

             listViewAddr->Items->Clear();
             std::map<int, ServerAddress>::iterator itAddr = mapServerAddress.begin();
             for(int i=0; itAddr != mapServerAddress.end(); itAddr++,i++)
             {
                 ListViewItem^ item = gcnew ListViewItem( Tools::string2String(itAddr->second.szIP),i);
                 item->Tag = itAddr->second.nAddrID;
                 item->SubItems->Add( itAddr->second.nPort.ToString());
                 listViewAddr->Items->Add( item ); 
             }
		 }
private: System::Void buttonAddServerGroup_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_SERVERGROUP_ADD));
			 if ( comboBoxBrokerList->SelectedIndex < 0 )
			 {
				 MessageBox::Show("请先选择一个经纪公司！", "提示",MessageBoxButtons::OK);
				 return;
			 }

             BrokerData^ data = (BrokerData^)comboBoxBrokerList->SelectedItem;
             int nBrokerID = data->GetBrokerID();

             ServerGroupForm^ dlg = gcnew ServerGroupForm();
             dlg->SetOperType(Oper_Add);
             dlg->SetBrokerID(nBrokerID);
             if (::DialogResult::OK == dlg->ShowDialog(this))
             {
                 *m_pCurServerGroup = *(dlg->GetCurServerGroup());             
                 m_pCurServerGroup->nBrokerID = nBrokerID;
                 CTcpLayer::SendData(Cmd_AddServerGroup_Req, m_pCurServerGroup,sizeof(ServerGroup), 0, m_pLogin->mnUserID);
             }
		 }
private: System::Void buttonDelServerGroup_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_SERVERGROUP_DEL));
             if(::DialogResult::Cancel ==  MessageBox::Show("此操作将会同步删除服务器组和委托交易账号的关系，服务器组和经纪公司的关系，服务器组和服务器地址的关系。确认吗？", "提示", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
                 return;

             if(listViewGroup->SelectedIndices != nullptr && listViewGroup->SelectedIndices->Count>0)
             {            
                 ListViewItem^ item   = listViewGroup->Items[listViewGroup->SelectedIndices[0]];
                 int nServerID = (int)(item->Tag);

                 BrokerData^ data = (BrokerData^)comboBoxBrokerList->SelectedItem;
                 m_pCurServerGroup->nBrokerID = data->GetBrokerID();
                 m_pCurServerGroup->nServerID = nServerID;
                 CTcpLayer::SendData(Cmd_DelServerGroup_Req, &nServerID, sizeof(nServerID), 0, m_pLogin->mnUserID);                 
             }
             else
             {
                 MessageBox::Show("请选择一个服务器组！", "提示",MessageBoxButtons::OK);
                 return;
             }
         }
private: System::Void button_ModifyServerGroup_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_SERVERGROUP_MODIFY));
             if(listViewGroup->SelectedIndices != nullptr && listViewGroup->SelectedIndices->Count>0)
             {
                 BrokerData^ data = (BrokerData^)comboBoxBrokerList->SelectedItem;
                 int nBrokerID = data->GetBrokerID();

                 ListView::SelectedIndexCollection^ c = listViewGroup->SelectedIndices;
                 int nServerID = (int)(listViewGroup->Items[c[0]]->Tag);

                 std::string str = Tools::String2string(listViewGroup->SelectedItems[0]->SubItems[0]->Text);
                 int nSettle = (listViewGroup->SelectedItems[0]->SubItems[1]->Text->ToString() == "True")?1:0;

                 if(!m_pDataInfo->GetServerGroupByID(nBrokerID, nServerID,*m_pCurServerGroup))
                     return;

                 ServerGroupForm^ dlg = gcnew ServerGroupForm();
                 dlg->SetOperType(Oper_Modify);
                 dlg->SetBrokerID(nBrokerID);
                 dlg->SetCurServerGroup(*m_pCurServerGroup);
                 if (::DialogResult::OK == dlg->ShowDialog(this))
                 {
                     *m_pCurServerGroup = *(dlg->GetCurServerGroup()); 
                     CTcpLayer::SendData(Cmd_ModifyServerGroup_Req, m_pCurServerGroup,sizeof(ServerGroup), 0, m_pLogin->mnUserID);
                 }
             }
             else
                 MessageBox::Show("请选择内容");
         }
private: System::Void buttonModifyServerAddr_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_SERVERADDR_MODIFY));
             if(listViewAddr->SelectedIndices != nullptr && listViewAddr->SelectedIndices->Count>0)
             {
                 ListView::SelectedIndexCollection^ cg = listViewGroup->SelectedIndices;
                 int nServerID = (int)(listViewGroup->Items[cg[0]]->Tag);

                 ListView::SelectedIndexCollection^ ca = listViewAddr->SelectedIndices;
                 int nAddrID = (int)(listViewAddr->Items[ca[0]]->Tag);

                 if(!m_pDataInfo->GetServerAddrByID(nServerID, nAddrID, *m_pCurServerAddr))
                     return;

                 ServerAddrForm^ dlg = gcnew ServerAddrForm();
                 dlg->SetOperType(Oper_Modify_Trade);
                 dlg->SetCurServerAddress(*m_pCurServerAddr);
                if (::DialogResult::OK == dlg->ShowDialog(this))
                 {
                     if(*m_pCurServerAddr == *(dlg->GetCurServerAddress()))
                         return;
                     else
                     {
                         *m_pCurServerAddr = *(dlg->GetCurServerAddress()); 
                         CTcpLayer::SendData(Cmd_ModifyServerAddress_Req, m_pCurServerAddr,sizeof(ServerAddress), 0, m_pLogin->mnUserID);
                     }
                 }
             }
             else
                 MessageBox::Show("请选择内容"); 
         }    
private: System::Void buttonAddServerAddr_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_SERVERADDR_ADD));
             if(listViewGroup->SelectedIndices != nullptr && listViewGroup->SelectedIndices->Count>0)
             {
                 ServerAddrForm^ dlg = gcnew ServerAddrForm();
                 dlg->SetOperType(Oper_Add_Trade);
                 if (::DialogResult::OK == dlg->ShowDialog(this))
                 {
                     ListViewItem^ item   = listViewGroup->Items[listViewGroup->SelectedIndices[0]];
                     int nServerID = (int)(item->Tag);

                     *m_pCurServerAddr = *(dlg->GetCurServerAddress());             
                     m_pCurServerAddr->nServerID = nServerID;
                     CTcpLayer::SendData(Cmd_AddServerAddress_Req, m_pCurServerAddr,sizeof(ServerAddress), 0, m_pLogin->mnUserID);
                 }
             }
             else
                 MessageBox::Show("请先选择一个服务器组！", "提示",MessageBoxButtons::OK);
		 }        
private: System::Void buttonDelServerAddr_Click(System::Object^  sender, System::EventArgs^  e) {
             LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_MANAGE_SERVERADDR_DEL));
             if(listViewAddr->SelectedIndices != nullptr && listViewAddr->SelectedIndices->Count>0)
             {        
                 ListViewItem^ itemGroup   = listViewAddr->Items[listViewGroup->SelectedIndices[0]];
                 int nServerID = (int)(itemGroup->Tag);

                 ListViewItem^ item   = listViewAddr->Items[listViewAddr->SelectedIndices[0]];
                 int nAddrID = (int)(item->Tag);

                 m_pCurServerGroup->nServerID = nServerID;
                 m_pCurServerAddr->nAddrID = nAddrID;
                 CTcpLayer::SendData(Cmd_DelServerAddress_Req, &nAddrID, sizeof(nAddrID), 0, m_pLogin->mnUserID);              
             }
             else
             {
                 MessageBox::Show("请选择一个服务器组地址！", "提示",MessageBoxButtons::OK);
                 return;
             }
		 }
private: System::Void listViewGroup_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             if(listViewGroup->SelectedIndices != nullptr && listViewGroup->SelectedIndices->Count>0)
             {
                 ListView::SelectedIndexCollection^ c = listViewGroup->SelectedIndices;
                 int nServerID = (int)(listViewGroup->Items[c[0]]->Tag);
                 if( nServerID<= 0 )
                     return;

                 std::map<int, ServerAddress> mapServerAddr;
                 m_pDataInfo->GetServerAddress(nServerID,mapServerAddr);
                 if(mapServerAddr.empty())
                 {
                     listViewAddr->Items->Clear();
                     return;
                 }

                 listViewAddr->Items->Clear();

                 std::map<int, ServerAddress>::iterator it = mapServerAddr.begin();
                 for(int i=0;it != mapServerAddr.end();it++,i++)
                 {
                     ListViewItem^ item = gcnew ListViewItem( Tools::string2String(it->second.szIP),i);
                     item->Tag = it->second.nAddrID;
                     item->SubItems->Add( it->second.nPort.ToString());
                     listViewAddr->Items->Add( item ); 
                 }  
             }
         }
};
}

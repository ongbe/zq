#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include <cliext/set>
#include "TcpLayer.h"
#include "CommonDef.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "CDataInfo.h"
#include "SystemUserForm.h"
#include "TraderAddOrgForm.h"
#include "TraderAddProductForm.h"
#include "TraderAddTradeAccountForm.h"
#include "RiskAddOrganizeForm.h"
#include "CommonUserOpr.h"

namespace CltServerMgr {

    /// <summary>
    /// Summary for SystemUserMgrForm
    ///
    /// WARNING: If you change the name of this class, you will need to change the
    ///          'Resource File Name' property for the managed resource compiler tool
    ///          associated with all .resx files this class depends on.  Otherwise,
    ///          the designers will not be able to interact properly with localized
    ///          resources associated with this form.
    /// </summary>
    public ref class SystemUserMgrForm : public System::Windows::Forms::Form
    {
    public:
        SystemUserMgrForm(void)
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //
            m_nUserType = 2;
            m_strUserType = "交易员";            
            m_pRiskAddOrganizeForm = nullptr;
            m_pTraderAddOrgForm = nullptr;
            m_pTraderAddProductForm = nullptr;
            m_pTraderAddTradeAccountForm = nullptr;
            m_bQryUser = false;
            m_bQryUserRoleRelation = false;
            m_bQryUserOrgRelation = false;
            m_bQryTraderProRelation = false;
            m_bQryUserTradeAccountRelation = false;
            m_pCurUserInfo = new UserInfo;
            m_pDataInfo = CDataInfo::NewInstance();
            m_pLogin = new sLoginRsp;
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~SystemUserMgrForm()
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
        }

    private: bool m_bQryUser;
    private: bool m_bQryUserRoleRelation;
    private: bool m_bQryUserOrgRelation;
    private: bool m_bQryTraderProRelation;
    private: bool m_bQryUserTradeAccountRelation;
             sLoginRsp* m_pLogin;
    private: UserInfo* m_pCurUserInfo;
    private: CDataInfo* m_pDataInfo;
    private: String^ m_strUserRole;
    private: String^ m_strUserType;
    private: int m_nUserType;
             RiskAddOrganizeForm^ m_pRiskAddOrganizeForm;
             TraderAddOrgForm^ m_pTraderAddOrgForm;
             TraderAddProductForm^ m_pTraderAddProductForm;
             TraderAddTradeAccountForm^ m_pTraderAddTradeAccountForm;
             
             


    private: System::Windows::Forms::TabControl^  tabControl;
    private: System::Windows::Forms::TabPage^  tabPage1;
    private: System::Windows::Forms::TabPage^  tabPage2;

    private: System::Windows::Forms::TabPage^  tabPage3;
    private: System::Windows::Forms::TabPage^  tabPage4;








































    private: System::Windows::Forms::Button^  Add_button;
    private: System::Windows::Forms::Button^  Modify_button;
    private: System::Windows::Forms::Button^  Status_button;

    private: System::Windows::Forms::Button^  button1;
    private: System::Windows::Forms::Button^  button2;
    private: System::Windows::Forms::Button^  button3;

    private: System::Windows::Forms::Timer^  timer_Init;









    private: System::Windows::Forms::Timer^  timer_LoginStatus;
    private: System::Windows::Forms::ListView^  Trader_listView;

    private: System::Windows::Forms::ColumnHeader^  columnHeader1;
    private: System::Windows::Forms::ColumnHeader^  columnHeader2;
    private: System::Windows::Forms::ColumnHeader^  columnHeader3;
    private: System::Windows::Forms::ColumnHeader^  columnHeader4;
    private: System::Windows::Forms::ColumnHeader^  columnHeader5;
    private: System::Windows::Forms::ColumnHeader^  columnHeader6;
    private: System::Windows::Forms::ColumnHeader^  columnHeader7;
    private: System::Windows::Forms::ColumnHeader^  columnHeader8;
    private: System::Windows::Forms::ColumnHeader^  columnHeader9;
    private: System::Windows::Forms::ListView^  Admin_listView;

    private: System::Windows::Forms::ColumnHeader^  columnHeader10;

    private: System::Windows::Forms::ColumnHeader^  columnHeader12;
    private: System::Windows::Forms::ColumnHeader^  columnHeader13;
    private: System::Windows::Forms::ColumnHeader^  columnHeader14;
    private: System::Windows::Forms::ColumnHeader^  columnHeader15;
    private: System::Windows::Forms::ColumnHeader^  columnHeader16;
    private: System::Windows::Forms::ColumnHeader^  columnHeader17;
    private: System::Windows::Forms::ListView^  Risk_listView;

    private: System::Windows::Forms::ColumnHeader^  columnHeader18;
    private: System::Windows::Forms::ColumnHeader^  columnHeader19;
    private: System::Windows::Forms::ColumnHeader^  columnHeader20;
    private: System::Windows::Forms::ColumnHeader^  columnHeader21;
    private: System::Windows::Forms::ColumnHeader^  columnHeader22;
    private: System::Windows::Forms::ColumnHeader^  columnHeader23;
    private: System::Windows::Forms::ColumnHeader^  columnHeader24;
    private: System::Windows::Forms::ColumnHeader^  columnHeader25;
    private: System::Windows::Forms::ColumnHeader^  columnHeader26;
    private: System::Windows::Forms::ListView^  Viewer_listView;

    private: System::Windows::Forms::ColumnHeader^  columnHeader27;
    private: System::Windows::Forms::ColumnHeader^  columnHeader28;
    private: System::Windows::Forms::ColumnHeader^  columnHeader29;
    private: System::Windows::Forms::ColumnHeader^  columnHeader30;
    private: System::Windows::Forms::ColumnHeader^  columnHeader31;
    private: System::Windows::Forms::ColumnHeader^  columnHeader32;
    private: System::Windows::Forms::ColumnHeader^  columnHeader33;
    private: System::Windows::Forms::Button^  button_UserInitStatus;
private: System::Windows::Forms::Button^  button_LoginStatusQuery;

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
            this->tabControl = (gcnew System::Windows::Forms::TabControl());
            this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
            this->Trader_listView = (gcnew System::Windows::Forms::ListView());
            this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader5 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader6 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader7 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader8 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader9 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader33 = (gcnew System::Windows::Forms::ColumnHeader());
            this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
            this->Admin_listView = (gcnew System::Windows::Forms::ListView());
            this->columnHeader10 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader12 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader13 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader14 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader15 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader16 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader17 = (gcnew System::Windows::Forms::ColumnHeader());
            this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
            this->Risk_listView = (gcnew System::Windows::Forms::ListView());
            this->columnHeader18 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader19 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader20 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader21 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader22 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader23 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader24 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader25 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader26 = (gcnew System::Windows::Forms::ColumnHeader());
            this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
            this->Viewer_listView = (gcnew System::Windows::Forms::ListView());
            this->columnHeader27 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader28 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader29 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader30 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader31 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader32 = (gcnew System::Windows::Forms::ColumnHeader());
            this->Add_button = (gcnew System::Windows::Forms::Button());
            this->Modify_button = (gcnew System::Windows::Forms::Button());
            this->Status_button = (gcnew System::Windows::Forms::Button());
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->button2 = (gcnew System::Windows::Forms::Button());
            this->button3 = (gcnew System::Windows::Forms::Button());
            this->timer_Init = (gcnew System::Windows::Forms::Timer(this->components));
            this->timer_LoginStatus = (gcnew System::Windows::Forms::Timer(this->components));
            this->button_UserInitStatus = (gcnew System::Windows::Forms::Button());
            this->button_LoginStatusQuery = (gcnew System::Windows::Forms::Button());
            this->tabControl->SuspendLayout();
            this->tabPage1->SuspendLayout();
            this->tabPage2->SuspendLayout();
            this->tabPage3->SuspendLayout();
            this->tabPage4->SuspendLayout();
            this->SuspendLayout();
            // 
            // tabControl
            // 
            this->tabControl->Controls->Add(this->tabPage1);
            this->tabControl->Controls->Add(this->tabPage2);
            this->tabControl->Controls->Add(this->tabPage3);
            this->tabControl->Controls->Add(this->tabPage4);
            this->tabControl->Location = System::Drawing::Point(-1, 2);
            this->tabControl->Name = L"tabControl";
            this->tabControl->SelectedIndex = 0;
            this->tabControl->Size = System::Drawing::Size(939, 259);
            this->tabControl->TabIndex = 0;
            this->tabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &SystemUserMgrForm::tabControl_SelectedIndexChanged);
            // 
            // tabPage1
            // 
            this->tabPage1->Controls->Add(this->Trader_listView);
            this->tabPage1->Location = System::Drawing::Point(4, 21);
            this->tabPage1->Name = L"tabPage1";
            this->tabPage1->Padding = System::Windows::Forms::Padding(3);
            this->tabPage1->Size = System::Drawing::Size(931, 234);
            this->tabPage1->TabIndex = 0;
            this->tabPage1->Text = L"交易员";
            this->tabPage1->UseVisualStyleBackColor = true;
            // 
            // Trader_listView
            // 
            this->Trader_listView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(10) {this->columnHeader1, 
                this->columnHeader2, this->columnHeader3, this->columnHeader4, this->columnHeader5, this->columnHeader6, this->columnHeader7, 
                this->columnHeader8, this->columnHeader9, this->columnHeader33});
            this->Trader_listView->Dock = System::Windows::Forms::DockStyle::Fill;
            this->Trader_listView->FullRowSelect = true;
            this->Trader_listView->GridLines = true;
            this->Trader_listView->HideSelection = false;
            this->Trader_listView->Location = System::Drawing::Point(3, 3);
            this->Trader_listView->Name = L"Trader_listView";
            this->Trader_listView->Size = System::Drawing::Size(925, 228);
            this->Trader_listView->TabIndex = 0;
            this->Trader_listView->UseCompatibleStateImageBehavior = false;
            this->Trader_listView->View = System::Windows::Forms::View::Details;
            this->Trader_listView->SelectedIndexChanged += gcnew System::EventHandler(this, &SystemUserMgrForm::Trader_listView_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this->columnHeader1->Text = L"用户号";
            // 
            // columnHeader2
            // 
            this->columnHeader2->Text = L"所属部门";
            this->columnHeader2->Width = 80;
            // 
            // columnHeader3
            // 
            this->columnHeader3->Text = L"理财产品";
            this->columnHeader3->Width = 80;
            // 
            // columnHeader4
            // 
            this->columnHeader4->Text = L"委托交易账号";
            this->columnHeader4->Width = 90;
            // 
            // columnHeader5
            // 
            this->columnHeader5->Text = L"账号状态";
            this->columnHeader5->Width = 80;
            // 
            // columnHeader6
            // 
            this->columnHeader6->Text = L"用户类型";
            this->columnHeader6->Width = 80;
            // 
            // columnHeader7
            // 
            this->columnHeader7->Text = L"用户名";
            // 
            // columnHeader8
            // 
            this->columnHeader8->Text = L"联系方式";
            this->columnHeader8->Width = 80;
            // 
            // columnHeader9
            // 
            this->columnHeader9->Text = L"登录状态";
            this->columnHeader9->Width = 72;
            // 
            // columnHeader33
            // 
            this->columnHeader33->Text = L"用户初始化状态";
            this->columnHeader33->Width = 235;
            // 
            // tabPage2
            // 
            this->tabPage2->Controls->Add(this->Admin_listView);
            this->tabPage2->Location = System::Drawing::Point(4, 21);
            this->tabPage2->Name = L"tabPage2";
            this->tabPage2->Padding = System::Windows::Forms::Padding(3);
            this->tabPage2->Size = System::Drawing::Size(931, 234);
            this->tabPage2->TabIndex = 1;
            this->tabPage2->Text = L"管理员";
            this->tabPage2->UseVisualStyleBackColor = true;
            // 
            // Admin_listView
            // 
            this->Admin_listView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(7) {this->columnHeader10, 
                this->columnHeader12, this->columnHeader13, this->columnHeader14, this->columnHeader15, this->columnHeader16, this->columnHeader17});
            this->Admin_listView->Dock = System::Windows::Forms::DockStyle::Fill;
            this->Admin_listView->FullRowSelect = true;
            this->Admin_listView->GridLines = true;
            this->Admin_listView->HideSelection = false;
            this->Admin_listView->Location = System::Drawing::Point(3, 3);
            this->Admin_listView->Name = L"Admin_listView";
            this->Admin_listView->Size = System::Drawing::Size(925, 228);
            this->Admin_listView->TabIndex = 0;
            this->Admin_listView->UseCompatibleStateImageBehavior = false;
            this->Admin_listView->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader10
            // 
            this->columnHeader10->Text = L"用户号";
            // 
            // columnHeader12
            // 
            this->columnHeader12->Text = L"账号状态";
            this->columnHeader12->Width = 80;
            // 
            // columnHeader13
            // 
            this->columnHeader13->Text = L"用户类型";
            this->columnHeader13->Width = 80;
            // 
            // columnHeader14
            // 
            this->columnHeader14->Text = L"角色";
            // 
            // columnHeader15
            // 
            this->columnHeader15->Text = L"用户名";
            // 
            // columnHeader16
            // 
            this->columnHeader16->Text = L"联系方式";
            this->columnHeader16->Width = 80;
            // 
            // columnHeader17
            // 
            this->columnHeader17->Text = L"登录状态";
            this->columnHeader17->Width = 80;
            // 
            // tabPage3
            // 
            this->tabPage3->Controls->Add(this->Risk_listView);
            this->tabPage3->Location = System::Drawing::Point(4, 21);
            this->tabPage3->Name = L"tabPage3";
            this->tabPage3->Padding = System::Windows::Forms::Padding(3);
            this->tabPage3->Size = System::Drawing::Size(931, 234);
            this->tabPage3->TabIndex = 2;
            this->tabPage3->Text = L"风控员";
            this->tabPage3->UseVisualStyleBackColor = true;
            // 
            // Risk_listView
            // 
            this->Risk_listView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(9) {this->columnHeader18, 
                this->columnHeader19, this->columnHeader20, this->columnHeader21, this->columnHeader22, this->columnHeader23, this->columnHeader24, 
                this->columnHeader25, this->columnHeader26});
            this->Risk_listView->Dock = System::Windows::Forms::DockStyle::Fill;
            this->Risk_listView->FullRowSelect = true;
            this->Risk_listView->GridLines = true;
            this->Risk_listView->HideSelection = false;
            this->Risk_listView->Location = System::Drawing::Point(3, 3);
            this->Risk_listView->Name = L"Risk_listView";
            this->Risk_listView->Size = System::Drawing::Size(925, 228);
            this->Risk_listView->TabIndex = 0;
            this->Risk_listView->UseCompatibleStateImageBehavior = false;
            this->Risk_listView->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader18
            // 
            this->columnHeader18->Text = L"用户号";
            // 
            // columnHeader19
            // 
            this->columnHeader19->Text = L"所属部门";
            this->columnHeader19->Width = 80;
            // 
            // columnHeader20
            // 
            this->columnHeader20->Text = L"风控部门";
            this->columnHeader20->Width = 80;
            // 
            // columnHeader21
            // 
            this->columnHeader21->Text = L"账号状态";
            this->columnHeader21->Width = 80;
            // 
            // columnHeader22
            // 
            this->columnHeader22->Text = L"用户类型";
            this->columnHeader22->Width = 80;
            // 
            // columnHeader23
            // 
            this->columnHeader23->Text = L"角色";
            // 
            // columnHeader24
            // 
            this->columnHeader24->Text = L"用户名";
            // 
            // columnHeader25
            // 
            this->columnHeader25->Text = L"联系方式";
            this->columnHeader25->Width = 80;
            // 
            // columnHeader26
            // 
            this->columnHeader26->Text = L"登录状态";
            this->columnHeader26->Width = 80;
            // 
            // tabPage4
            // 
            this->tabPage4->Controls->Add(this->Viewer_listView);
            this->tabPage4->Location = System::Drawing::Point(4, 21);
            this->tabPage4->Name = L"tabPage4";
            this->tabPage4->Padding = System::Windows::Forms::Padding(3);
            this->tabPage4->Size = System::Drawing::Size(931, 234);
            this->tabPage4->TabIndex = 3;
            this->tabPage4->Text = L"展现员";
            this->tabPage4->UseVisualStyleBackColor = true;
            // 
            // Viewer_listView
            // 
            this->Viewer_listView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(6) {this->columnHeader27, 
                this->columnHeader28, this->columnHeader29, this->columnHeader30, this->columnHeader31, this->columnHeader32});
            this->Viewer_listView->Dock = System::Windows::Forms::DockStyle::Fill;
            this->Viewer_listView->FullRowSelect = true;
            this->Viewer_listView->GridLines = true;
            this->Viewer_listView->HideSelection = false;
            this->Viewer_listView->Location = System::Drawing::Point(3, 3);
            this->Viewer_listView->Name = L"Viewer_listView";
            this->Viewer_listView->Size = System::Drawing::Size(925, 228);
            this->Viewer_listView->TabIndex = 0;
            this->Viewer_listView->UseCompatibleStateImageBehavior = false;
            this->Viewer_listView->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader27
            // 
            this->columnHeader27->Text = L"用户号";
            // 
            // columnHeader28
            // 
            this->columnHeader28->Text = L"账号状态";
            this->columnHeader28->Width = 80;
            // 
            // columnHeader29
            // 
            this->columnHeader29->Text = L"用户类型";
            this->columnHeader29->Width = 80;
            // 
            // columnHeader30
            // 
            this->columnHeader30->Text = L"用户名";
            // 
            // columnHeader31
            // 
            this->columnHeader31->Text = L"联系方式";
            this->columnHeader31->Width = 80;
            // 
            // columnHeader32
            // 
            this->columnHeader32->Text = L"登录状态";
            this->columnHeader32->Width = 80;
            // 
            // Add_button
            // 
            this->Add_button->Location = System::Drawing::Point(37, 267);
            this->Add_button->Name = L"Add_button";
            this->Add_button->Size = System::Drawing::Size(75, 23);
            this->Add_button->TabIndex = 1;
            this->Add_button->Text = L"新增用户";
            this->Add_button->UseVisualStyleBackColor = true;
            this->Add_button->Click += gcnew System::EventHandler(this, &SystemUserMgrForm::Add_button_Click);
            // 
            // Modify_button
            // 
            this->Modify_button->Location = System::Drawing::Point(128, 267);
            this->Modify_button->Name = L"Modify_button";
            this->Modify_button->Size = System::Drawing::Size(75, 23);
            this->Modify_button->TabIndex = 1;
            this->Modify_button->Text = L"修改用户";
            this->Modify_button->UseVisualStyleBackColor = true;
            this->Modify_button->Click += gcnew System::EventHandler(this, &SystemUserMgrForm::Modify_button_Click);
            // 
            // Status_button
            // 
            this->Status_button->Location = System::Drawing::Point(268, 267);
            this->Status_button->Name = L"Status_button";
            this->Status_button->Size = System::Drawing::Size(75, 23);
            this->Status_button->TabIndex = 1;
            this->Status_button->Text = L"启用";
            this->Status_button->UseVisualStyleBackColor = true;
            this->Status_button->Click += gcnew System::EventHandler(this, &SystemUserMgrForm::Status_button_Click);
            // 
            // button1
            // 
            this->button1->Location = System::Drawing::Point(533, 267);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(110, 23);
            this->button1->TabIndex = 1;
            this->button1->Text = L"分配组织机构";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Click += gcnew System::EventHandler(this, &SystemUserMgrForm::button1_Click);
            // 
            // button2
            // 
            this->button2->Location = System::Drawing::Point(653, 267);
            this->button2->Name = L"button2";
            this->button2->Size = System::Drawing::Size(110, 23);
            this->button2->TabIndex = 1;
            this->button2->Text = L"分配理财产品";
            this->button2->UseVisualStyleBackColor = true;
            this->button2->Click += gcnew System::EventHandler(this, &SystemUserMgrForm::button2_Click);
            // 
            // button3
            // 
            this->button3->Location = System::Drawing::Point(773, 267);
            this->button3->Name = L"button3";
            this->button3->Size = System::Drawing::Size(110, 23);
            this->button3->TabIndex = 1;
            this->button3->Text = L"指定委托交易账号";
            this->button3->UseVisualStyleBackColor = true;
            this->button3->Click += gcnew System::EventHandler(this, &SystemUserMgrForm::button3_Click);
            // 
            // timer_Init
            // 
            this->timer_Init->Interval = 500;
            this->timer_Init->Tick += gcnew System::EventHandler(this, &SystemUserMgrForm::timer_Init_Tick);
            // 
            // timer_LoginStatus
            // 
            this->timer_LoginStatus->Interval = 5000;
            this->timer_LoginStatus->Tick += gcnew System::EventHandler(this, &SystemUserMgrForm::timer_LoginStatus_Tick);
            // 
            // button_UserInitStatus
            // 
            this->button_UserInitStatus->Location = System::Drawing::Point(448, 267);
            this->button_UserInitStatus->Name = L"button_UserInitStatus";
            this->button_UserInitStatus->Size = System::Drawing::Size(75, 23);
            this->button_UserInitStatus->TabIndex = 2;
            this->button_UserInitStatus->Text = L"用户初始化";
            this->button_UserInitStatus->UseVisualStyleBackColor = true;
            this->button_UserInitStatus->Click += gcnew System::EventHandler(this, &SystemUserMgrForm::button_UserInitStatus_Click);
            // 
            // button_LoginStatusQuery
            // 
            this->button_LoginStatusQuery->Location = System::Drawing::Point(353, 267);
            this->button_LoginStatusQuery->Name = L"button_LoginStatusQuery";
            this->button_LoginStatusQuery->Size = System::Drawing::Size(85, 23);
            this->button_LoginStatusQuery->TabIndex = 3;
            this->button_LoginStatusQuery->Text = L"登录状态查询";
            this->button_LoginStatusQuery->UseVisualStyleBackColor = true;
            this->button_LoginStatusQuery->Click += gcnew System::EventHandler(this, &SystemUserMgrForm::button_LoginStatusQuery_Click);
            // 
            // SystemUserMgrForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(939, 298);
            this->Controls->Add(this->button_LoginStatusQuery);
            this->Controls->Add(this->button_UserInitStatus);
            this->Controls->Add(this->button3);
            this->Controls->Add(this->button2);
            this->Controls->Add(this->Status_button);
            this->Controls->Add(this->button1);
            this->Controls->Add(this->Modify_button);
            this->Controls->Add(this->Add_button);
            this->Controls->Add(this->tabControl);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"SystemUserMgrForm";
            this->Text = L"系统用户管理";
            this->Load += gcnew System::EventHandler(this, &SystemUserMgrForm::SystemUserMgrForm_Load);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &SystemUserMgrForm::SystemUserMgrForm_FormClosed);
            this->tabControl->ResumeLayout(false);
            this->tabPage1->ResumeLayout(false);
            this->tabPage2->ResumeLayout(false);
            this->tabPage3->ResumeLayout(false);
            this->tabPage4->ResumeLayout(false);
            this->ResumeLayout(false);

        }
#pragma endregion
    private: String^ ProcessStautsText(int nStautsValue,double dTime)
             {
                 String^ szInitStatusText = String::Empty;

                 if(nStautsValue == USER_STATUS_Inited)
                     szInitStatusText = "初始化完成";
                 else if(nStautsValue & USER_STATUS_InitQryPositionDetailReady)
                     szInitStatusText = "初始化中:日初的持仓明细查询完成";
                 else if(nStautsValue & USER_STATUS_InitQryPositionReady)
                     szInitStatusText = "初始化中:日初的持仓查询完成";
                 else if(nStautsValue & USER_STATUS_InitQryFundInfoReady)
                     szInitStatusText = "初始化中:日初的资金查询完成";
                 else if(nStautsValue & USER_STATUS_InitQryOrdersReady)
                     szInitStatusText = "初始化中:报单查询完成";
                 else if(nStautsValue & USER_STATUS_InitQryTradesReady)
                     szInitStatusText = "初始化中:成交查询完成";
                 else if(nStautsValue & USER_STATUS_InitQryRatesReady)
                     szInitStatusText = "初始化中:费率和保证金率查询完成";
                 else if(nStautsValue & USER_STATUS_InitQryInstrumentsReady)
                     szInitStatusText = "初始化中:合约数据查询完成";
                 else if(nStautsValue & USER_STATUS_UnInit)
                     szInitStatusText = "未初始化";

                 if(dTime>5*60 && nStautsValue != USER_STATUS_Inited)
                     szInitStatusText = "初始化失败，请检查用户和其相关的委托交易账号配置";

                 return szInitStatusText;
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

                                  m_bQryUser = true;
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          }
                      case Cmd_QryRole_Rsp:
                          {         
                              bShowMsg = false;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pDataInfo->ClearRole();

                                  std::map<int, RoleInfo> mapRoleInfo;
                                  int nCount = head.len / sizeof(RoleInfo);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      RoleInfo* pRoleInfo = (RoleInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(RoleInfo));
                                      mapRoleInfo[pRoleInfo->nRoleID] = *pRoleInfo;
                                  }

                                  m_pDataInfo->SetRole(mapRoleInfo);

                                  m_bQryUserRoleRelation = true;
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
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

                                  m_bQryUserRoleRelation = true;
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

                                  m_bQryUserOrgRelation = true;
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

                                  m_pDataInfo->SetTradeAccount(mapTradeAccount);
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
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
                                  int nCount = head.len / sizeof(UserAndTradeAccountRelation);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      UserAndTradeAccountRelation* pRelation = (UserAndTradeAccountRelation*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(UserAndTradeAccountRelation));
                                      mapUserAndTradeAccountRelation.insert(make_pair(pRelation->nUserID, *pRelation));                             
                                  }

                                  m_pDataInfo->SetUserAndTraderAccountRelation(mapUserAndTradeAccountRelation);	

                                  m_bQryUserTradeAccountRelation = true;
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          } 
                      case Cmd_QueryFinicialProduct_Rsp:
                          {
                              bShowMsg = false;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pDataInfo->ClearFinanceProduct();

                                  int nCount = head.len / sizeof(FinancialProduct);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      FinancialProduct* pFinancialProduct = (FinancialProduct*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(FinancialProduct));
                                      m_pDataInfo->SetFinancialProduct(pFinancialProduct);									
                                  }
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_QueryProductTraderRelation_Rsp:
                          {
                              bShowMsg = false;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pDataInfo->ClearProductTraderRelation();

                                  int nCount = head.len / sizeof(TraderProRelation);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      TraderProRelation* pTraderProRelation = (TraderProRelation*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(TraderProRelation));
                                      m_pDataInfo->SetProductTraderRelation(pTraderProRelation);
                                  }

                                  m_bQryTraderProRelation = true;
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          }
                      case Cmd_AddUser_Rsp:
                          {                              
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {                                  
                                  m_pCurUserInfo->nUserID = *(int*)(p+sizeof(Stru_UniPkgHead));
                                  m_pDataInfo->AddUserInfo(m_pCurUserInfo);

                                  if(!String::IsNullOrEmpty(m_strUserRole))
                                  {
                                      std::vector<int> vecRole;
                                      cli::array<System::String^>^ sArray = m_strUserRole->Split(',');  
                                      for each (String^ item in sArray)  
                                      {
                                          String^ sRole = item->ToString();
                                          int nRoleID = m_pDataInfo->GetRoleIDByName(Tools::String2string(sRole));
                                          vecRole.push_back(nRoleID);
                                      }

                                      int nDataLen = sizeof(int)+sizeof(int)*vecRole.size();
                                      char* pBuffer = new char[nDataLen];
                                      memset(pBuffer, 0, nDataLen);
                                      memcpy(pBuffer, &(m_pCurUserInfo->nUserID), sizeof(int));
                                      if ( !vecRole.empty() )                                  
                                          memcpy(pBuffer+sizeof(int), &vecRole[0], sizeof(int)*vecRole.size());

                                      CTcpLayer::SendData(Cmd_SaveUserRoleRelation_Req, pBuffer, nDataLen, 0, m_pLogin->mnUserID);                                      
                                  }

                                  int nCount = -1;
                                  if( USER_TYPE_MANAGE == m_pCurUserInfo->nUserType)
                                  {

                                      nCount = Admin_listView->Items->Count;
                                      ListViewItem^ item = gcnew ListViewItem(Tools::string2String(m_pCurUserInfo->szAccount),nCount++);  
                                      item->Tag = m_pCurUserInfo->nUserID;                                       
                                      item->SubItems->Add((m_pCurUserInfo->nStatus)?"启用":"停用");  
                                      item->SubItems->Add("管理员");  
                                      item->SubItems->Add(m_strUserRole);  
                                      item->SubItems->Add(Tools::string2String(m_pCurUserInfo->szAlias));  
                                      item->SubItems->Add(Tools::string2String(m_pCurUserInfo->szContactInfo)); 
                                      item->SubItems->Add(String::Empty); 

                                      Admin_listView->Items->Add(item);
                                  }
                                  else if( USER_TYPE_RISK_CONTROL == m_pCurUserInfo->nUserType)
                                  {
                                      nCount = Risk_listView->Items->Count;

                                      ListViewItem^ item = gcnew ListViewItem(Tools::string2String(m_pCurUserInfo->szAccount),nCount++); 
                                      item->Tag = m_pCurUserInfo->nUserID;
                                      item->SubItems->Add("总部");  
                                      item->SubItems->Add(String::Empty);  
                                      item->SubItems->Add((m_pCurUserInfo->nStatus)?"启用":"停用");  
                                      item->SubItems->Add("风控员");  
                                      item->SubItems->Add(m_strUserRole);  
                                      item->SubItems->Add(Tools::string2String(m_pCurUserInfo->szAlias));  
                                      item->SubItems->Add(Tools::string2String(m_pCurUserInfo->szContactInfo)); 
                                      item->SubItems->Add(String::Empty); 

                                      Risk_listView->Items->Add(item);

                                      //用户默认分配到总部下边
                                      UserAndOrgRelation relation;
                                      relation.nUserID = m_pCurUserInfo->nUserID;
                                      relation.nOrgID = ORGNIZATION_ROOT;
                                      relation.nUserType = m_pCurUserInfo->nUserType;
                                      relation.nRelationType = USER_ORG_RELATION_ATTACH;

                                      m_pDataInfo->SetUserAndOrgRelation(&relation); 
  
                                  }
                                  else if( USER_TYPE_TRADER == m_pCurUserInfo->nUserType)
                                  {
                                      std::vector<TraderProRelation> vec;
                                      m_pDataInfo->GetProTraderRelation(vec);

                                      string strProductName = " ",strProduct[30];
                                      std::vector<TraderProRelation>::iterator it = vec.begin();
                                      for (int i=0; it != vec.end(); i++,it++)
                                      {
                                          if (it->nTraderID == m_pCurUserInfo->nUserID)
                                          { 
                                              strProduct[i] = m_pDataInfo->GetFinancialProductNameByID(it->nFinancialProductID);;
                                              strProductName += strProduct[i]+",";
                                          }
                                      }

                                      int nLen = strProductName.length();
                                      if(nLen>2)
                                          strProductName[nLen-1] = '\0';


                                      std::map<int, UserAndTradeAccountRelation> mapUserAndOrgRelation;
                                      m_pDataInfo->GetUserAndTraderAccountRelation(mapUserAndOrgRelation);

                                      string strTradeAcc;
                                      std::map<int, UserAndTradeAccountRelation>::iterator it2 = mapUserAndOrgRelation.find(m_pCurUserInfo->nUserID);
                                      if(it2!=mapUserAndOrgRelation.end())
                                          strTradeAcc = m_pDataInfo->GetTradeAccountByID(it2->second.nTradeAccountID);

                                      nCount = Trader_listView->Items->Count;

                                      ListViewItem^ item = gcnew ListViewItem(Tools::string2String(m_pCurUserInfo->szAccount),nCount++);  
                                      item->Tag = m_pCurUserInfo->nUserID;
                                      item->SubItems->Add("总部");  
                                      item->SubItems->Add(Tools::string2String(strProductName.c_str()));  
                                      item->SubItems->Add(Tools::string2String(strTradeAcc.c_str()));  
                                      item->SubItems->Add((m_pCurUserInfo->nStatus)?"启用":"停用");  
                                      item->SubItems->Add("交易员");  
                                      item->SubItems->Add(Tools::string2String(m_pCurUserInfo->szAlias));  
                                      item->SubItems->Add(Tools::string2String(m_pCurUserInfo->szContactInfo)); 
                                      item->SubItems->Add(String::Empty);
                                      item->SubItems->Add(String::Empty);

                                      Trader_listView->Items->Add(item);

                                      //用户默认分配到总部下边
                                      UserAndOrgRelation relation;
                                      relation.nUserID = m_pCurUserInfo->nUserID;
                                      relation.nOrgID = ORGNIZATION_ROOT;
                                      relation.nUserType = m_pCurUserInfo->nUserType;
                                      relation.nRelationType = USER_ORG_RELATION_ATTACH;

                                      m_pDataInfo->SetUserAndOrgRelation(&relation); 
                                  }
                                  else if( USER_TYPE_VIEWER == m_pCurUserInfo->nUserType)
                                  {
                                      nCount = Viewer_listView->Items->Count;

                                      ListViewItem^ item = gcnew ListViewItem(Tools::string2String(m_pCurUserInfo->szAccount),nCount++);    
                                      item->Tag = m_pCurUserInfo->nUserID;
                                      item->SubItems->Add((m_pCurUserInfo->nStatus)?"启用":"停用");  
                                      item->SubItems->Add("展现员");  
                                      item->SubItems->Add(Tools::string2String(m_pCurUserInfo->szAlias));  
                                      item->SubItems->Add(Tools::string2String(m_pCurUserInfo->szContactInfo)); 
                                      item->SubItems->Add(String::Empty); 

                                      Viewer_listView->Items->Add(item);
                                  } 

                                  strError = "增加用户操作成功";
                              }
                              else
                              {
                                  strError = "增加用户操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          }
                      case Cmd_DelUser_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int nUserID = *(int*)((char*)(p+sizeof(Stru_UniPkgHead))); 

                                  if (m_strUserType == "交易员")
                                  {         
                                      ListView::SelectedIndexCollection^ c = Trader_listView->SelectedIndices;
                                      if(c != nullptr)
                                          Trader_listView->Items->RemoveAt(c[0]);          
                                  }else if (m_strUserType == "管理员")
                                  {
                                      ListView::SelectedIndexCollection^ c = Admin_listView->SelectedIndices;
                                      if(c != nullptr)
                                          Admin_listView->Items->RemoveAt(c[0]);  
                                  }else if (m_strUserType == "风控员")
                                  {
                                      ListView::SelectedIndexCollection^ c = Risk_listView->SelectedIndices;
                                      if(c != nullptr)
                                          Risk_listView->Items->RemoveAt(c[0]); 
                                  }else if (m_strUserType == "展现员")
                                  {
                                      ListView::SelectedIndexCollection^ c = Viewer_listView->SelectedIndices;
                                      if(c != nullptr)
                                          Viewer_listView->Items->RemoveAt(c[0]); 
                                  }

                                  m_pDataInfo->DelUserInfo(nUserID);

                                  strError = "删除用户操作成功";
                              }
                              else
                              {
                                  strError = "删除用户操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_ModifyUser_Rsp:
                          {                              
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {                                  
                                  m_pCurUserInfo->nUserID = *(int*)(p+sizeof(Stru_UniPkgHead));
                                  m_pDataInfo->AddUserInfo(m_pCurUserInfo);

                                  if( USER_TYPE_MANAGE == m_pCurUserInfo->nUserType)
                                  {
                                      Admin_listView->SelectedItems[0]->SubItems[0]->Text = Tools::string2String(m_pCurUserInfo->szAccount);
                                      Admin_listView->SelectedItems[0]->SubItems[1]->Text = (m_pCurUserInfo->nStatus)?"启用":"停用";
                                      Admin_listView->SelectedItems[0]->SubItems[2]->Text = "管理员";
                                      Admin_listView->SelectedItems[0]->SubItems[3]->Text = m_strUserRole;
                                      Admin_listView->SelectedItems[0]->SubItems[4]->Text = Tools::string2String(m_pCurUserInfo->szAlias);
                                      Admin_listView->SelectedItems[0]->SubItems[5]->Text = Tools::string2String(m_pCurUserInfo->szContactInfo);
                                  }
                                  else if( USER_TYPE_RISK_CONTROL == m_pCurUserInfo->nUserType)
                                  {
                                      Risk_listView->SelectedItems[0]->SubItems[0]->Text = Tools::string2String(m_pCurUserInfo->szAccount);
                                      Risk_listView->SelectedItems[0]->SubItems[3]->Text = (m_pCurUserInfo->nStatus)?"启用":"停用";
                                      Risk_listView->SelectedItems[0]->SubItems[4]->Text = "风控员";
                                      Risk_listView->SelectedItems[0]->SubItems[5]->Text = m_strUserRole;
                                      Risk_listView->SelectedItems[0]->SubItems[6]->Text = Tools::string2String(m_pCurUserInfo->szAlias);
                                      Risk_listView->SelectedItems[0]->SubItems[7]->Text = Tools::string2String(m_pCurUserInfo->szContactInfo);
                                  }
                                  else if( USER_TYPE_TRADER == m_pCurUserInfo->nUserType)
                                  {
                                      Trader_listView->SelectedItems[0]->SubItems[0]->Text = Tools::string2String(m_pCurUserInfo->szAccount);
                                      Trader_listView->SelectedItems[0]->SubItems[4]->Text = (m_pCurUserInfo->nStatus)?"启用":"停用";
                                      Trader_listView->SelectedItems[0]->SubItems[5]->Text = "交易员";
                                      Trader_listView->SelectedItems[0]->SubItems[6]->Text = Tools::string2String(m_pCurUserInfo->szAlias);
                                      Trader_listView->SelectedItems[0]->SubItems[7]->Text = Tools::string2String(m_pCurUserInfo->szContactInfo);
                                  }
                                  else if( USER_TYPE_VIEWER == m_pCurUserInfo->nUserType)
                                  {
                                      Viewer_listView->SelectedItems[0]->SubItems[0]->Text = Tools::string2String(m_pCurUserInfo->szAccount);
                                      Viewer_listView->SelectedItems[0]->SubItems[1]->Text = (m_pCurUserInfo->nStatus)?"启用":"停用";
                                      Viewer_listView->SelectedItems[0]->SubItems[2]->Text = "展现员";
                                      Viewer_listView->SelectedItems[0]->SubItems[3]->Text = Tools::string2String(m_pCurUserInfo->szAlias);
                                      Viewer_listView->SelectedItems[0]->SubItems[4]->Text = Tools::string2String(m_pCurUserInfo->szContactInfo);
                                  } 

                                  strError = "修改用户操作成功";
                              }
                              else
                              {
                                  strError = "修改用户操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          }
                      case Cmd_SaveUserOrgRelation_FromUser_Rsp:
                      case Cmd_SaveUserOrgRelation_FromOrg_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int nUserType = *(int*)(p+sizeof(Stru_UniPkgHead));
                                  int nUserID = m_pCurUserInfo->nUserID;

                                  //将所有的关系删除
                                  m_pDataInfo->DelUserAndOrgRelation(nUserID,nUserType);

                                  string strSubOrgName = " ",strSubOrg[30];
                                  std::vector<int> vecOrg;

                                  UserAndOrgRelation oRelation; 
                                  oRelation.nUserID = nUserID;
                                  oRelation.nUserType = nUserType;

                                  vector<UserAndOrgRelation> vecRelation; 

                                  if(nUserType == USER_TYPE_RISK_CONTROL)
                                  {
                                      //从属
                                      oRelation.nRelationType = USER_ORG_RELATION_ATTACH;
                                      oRelation.nOrgID  = m_pDataInfo->GetOrganizationIDByName(Tools::String2string(m_pRiskAddOrganizeForm->GetCurOrg()));
                                      m_pDataInfo->SetUserAndOrgRelation(&oRelation);

                                      if(m_pRiskAddOrganizeForm->GetListenOrg(vecOrg))
                                      {
                                          //监控
                                          oRelation.nRelationType = USER_ORG_RELATION_MONITOR;
                                          std::vector<int>::iterator it = vecOrg.begin();
                                          for (int index=0; it!=vecOrg.end(); it++) 
                                          {
                                              oRelation.nOrgID = (*it);                                             
                                              vecRelation.push_back(oRelation);

                                              strSubOrg[index] = m_pDataInfo->GetOrganizationNameByID(oRelation.nOrgID);
                                              strSubOrgName += strSubOrg[index]+" "; 
                                              index++;
                                          }
                                      }

                                      m_pDataInfo->SetUserAndOrgListenRelation(vecRelation);

                                      Risk_listView->SelectedItems[0]->SubItems[1]->Text = m_pRiskAddOrganizeForm->GetCurOrg();
                                      Risk_listView->SelectedItems[0]->SubItems[2]->Text = Tools::string2String(strSubOrgName.c_str());
                                  }
                                  else if(nUserType == USER_TYPE_TRADER)
                                  {                       
                                      oRelation.nRelationType = USER_ORG_RELATION_ATTACH;
                                      oRelation.nOrgID  = m_pDataInfo->GetOrganizationIDByName(Tools::String2string(m_pTraderAddOrgForm->GetCurOrg()));
                                      m_pDataInfo->SetUserAndOrgRelation(&oRelation);

                                      Trader_listView->SelectedItems[0]->SubItems[1]->Text = m_pTraderAddOrgForm->GetCurOrg();
                                  }

                                  strError = "保存用户和组织机构关系操作成功";
                              }
                              else
                              {
                                  strError = "保存用户和组织机构关系操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_SaveUserRoleRelation_Rsp:
                          {
                              bShowMsg = false;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  m_pDataInfo->ClearUserRoleRelationByUserID(m_pCurUserInfo->nUserID);

                                  UserRole oUserRole;
                                  cli::array<System::String^>^ sArray = m_strUserRole->Split(',');  
                                  for each (String^ item in sArray)  
                                  {
                                      String^ sRole = item->ToString()->Trim();
                                      int nRoleID = m_pDataInfo->GetRoleIDByName(Tools::String2string(sRole));
                                      if(nRoleID<0)
                                          continue;

                                      oUserRole.nUserID = m_pCurUserInfo->nUserID;
                                      oUserRole.nRoleID = nRoleID;

                                      m_pDataInfo->SetUserRoleRelation(&oUserRole);	
                                  }

                                  if( USER_TYPE_MANAGE == m_pCurUserInfo->nUserType)   
                                  {
                                      int nCount = Admin_listView->Items->Count;
                                      for (int i=0;i<nCount;i++)
                                      {
                                          ListViewItem^ item   = Admin_listView->Items[i];
                                          if((int)( item->Tag) == m_pCurUserInfo->nUserID)
                                          {
                                              Admin_listView->Items[i]->SubItems[3]->Text = m_strUserRole;
                                              break;
                                          }
                                      }

                                  }
                                  else if( USER_TYPE_RISK_CONTROL == m_pCurUserInfo->nUserType)
                                  {
                                      int nCount = Risk_listView->Items->Count;
                                      for (int i=0;i<nCount;i++)
                                      {
                                          ListViewItem^ item   = Risk_listView->Items[i];
                                          if((int)( item->Tag) == m_pCurUserInfo->nUserID)
                                          {
                                              Risk_listView->Items[i]->SubItems[5]->Text = m_strUserRole;
                                              break;
                                          }
                                      }
                                  }

                                  strError = "保存用户和角色关系操作成功";
                              }
                              else
                              {
                                  strError = "保存用户和角色关系操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_SaveUserTradeAcctRelation_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  UserAndTradeAccountRelation* pRelation = (UserAndTradeAccountRelation*)((char*)(p+sizeof(Stru_UniPkgHead)));                                      
                                  m_pDataInfo->AddUserAndTraderAccountRelation(pRelation);	

                                  TradeAccount oAcc;
                                  if(m_pDataInfo->GetTradeAccountByAccount(Tools::String2string(m_pTraderAddTradeAccountForm->GetCurTradeAccount()),oAcc))
                                      Trader_listView->SelectedItems[0]->SubItems[3]->Text = Tools::string2String(oAcc.szTradeAccount);
                                  else
                                      Trader_listView->SelectedItems[0]->SubItems[3]->Text = "";

                                  strError = "保存用户和委托交易账号关系操作成功";
                              }
                              else
                              {
                                  strError = "保存用户和委托交易账号关系操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_SaveProductTraderRelation_FromUser_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int nType = *(int*)((char*)(p+sizeof(Stru_UniPkgHead)));

                                  int nUserID = m_pDataInfo->GetUserIDByAccount(Tools::String2string(Trader_listView->SelectedItems[0]->SubItems[0]->Text));
                                  m_pDataInfo->ClearProductTraderRelation(nType, nUserID);

                                  TraderProRelation oRelation;
                                  oRelation.nTraderID = nUserID;

                                  std::vector<int> vecProductID;
                                  String^ sProductID  = m_pTraderAddProductForm->GetCurProduct();
                                  Trader_listView->SelectedItems[0]->SubItems[2]->Text = sProductID; 
                                  cli::array<System::String^>^ sArray = sProductID->Split(',');  
                                  for each (String^ item in sArray)  
                                  {
                                      String^ sProduct = item->ToString()->Trim();                                      
                                      int nProductID = m_pDataInfo->GetFinancialProductIDByName(Tools::String2string(sProduct));
                                      if( nProductID < 0 )
                                          break;
                                      else                                        
                                      {
                                          oRelation.nFinancialProductID = nProductID;
                                          m_pDataInfo->SetProductTraderRelation(&oRelation);
                                      }
                                  }

                                  strError = "保存交易员和理财产品关系操作成功";
                              }
                              else
                              {
                                  strError = "保存交易员和理财产品关系操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_UserStatus_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int nUserID = *(int*)((char*)(p+sizeof(Stru_UniPkgHead)));
                                  m_pDataInfo->SetUserStatus(nUserID);

                                  Trader_listView->SelectedItems[0]->SubItems[4]->Text = (m_pCurUserInfo->nStatus)?"停用":"启用";
                                  Status_button->Text = (m_pCurUserInfo->nStatus)?"启用":"停用";

                                  m_pCurUserInfo->nStatus = (0==m_pCurUserInfo->nStatus)?1:0;

                                  strError = "用户状态操作成功";
                              }
                              else
                              {
                                  strError = "用户状态操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case CMDID_SubscriberAllUserLoginStatus_Rsp:
                      case CMDID_AllUserLoginStatus_Push:
                          {
                              bShowMsg = false;
                              if ( head.userdata3 == CF_ERROR_SUCCESS )
                              {
                                  cliext::set<int> setLoginUser;
                                  int nCount = head.len / sizeof(int);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      int nUserID = *(int*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(int));
                                      setLoginUser.insert(nUserID);
                                  }

                                  int nListCount = Admin_listView->Items->Count;
                                  for (int i=0;i<nListCount;i++)
                                  {
                                      
                                      int nUserID = (int)Admin_listView->Items[i]->Tag;
                                      cliext::set<int>::iterator it = setLoginUser.find(nUserID);
                                      if(it!=setLoginUser.end())
                                          Admin_listView->Items[i]->SubItems[6]->Text = STATUS_LOGIN;
                                      else
                                          Admin_listView->Items[i]->SubItems[6]->Text = STATUS_LOGINOUT;

                                  }

                                  nListCount = Risk_listView->Items->Count;
                                  for (int i=0;i<nListCount;i++)
                                  {
                                      int nUserID = (int)Risk_listView->Items[i]->Tag;
                                      cliext::set<int>::iterator it = setLoginUser.find(nUserID);
                                      if(it!=setLoginUser.end())
                                          Risk_listView->Items[i]->SubItems[8]->Text = STATUS_LOGIN;
                                      else
                                          Risk_listView->Items[i]->SubItems[8]->Text = STATUS_LOGINOUT;
                                  }

                                  nListCount = Trader_listView->Items->Count;
                                  for (int i=0;i<nListCount;i++)
                                  {
                                      int nUserID = (int)Trader_listView->Items[i]->Tag;
                                      cliext::set<int>::iterator it = setLoginUser.find(nUserID);
                                      if(it!=setLoginUser.end())
                                          Trader_listView->Items[i]->SubItems[8]->Text = STATUS_LOGIN;
                                      else
                                          Trader_listView->Items[i]->SubItems[8]->Text = STATUS_LOGINOUT;
                                  }

                                  nListCount = Viewer_listView->Items->Count;
                                  for (int i=0;i<nListCount;i++)
                                  {
                                      int nUserID = (int)Viewer_listView->Items[i]->Tag;
                                      cliext::set<int>::iterator it = setLoginUser.find(nUserID);
                                      if(it!=setLoginUser.end())
                                          Viewer_listView->Items[i]->SubItems[5]->Text = STATUS_LOGIN;
                                      else
                                          Viewer_listView->Items[i]->SubItems[5]->Text = STATUS_LOGINOUT;                                     
                                  }
                                  
                              }
                              else
                              {
                                  strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }

                              break;
                          }
                      case Cmd_TradeDataManager_QryUserInitStatus_Rsp:
                          {
                              bShowMsg = false;
                              if ( head.userdata3 == CF_ERROR_SUCCESS )
                              {
                                  int nCount = head.len / sizeof(sUserInitStatus);
                                  for ( int i = 0; i < nCount; i++ )
                                  {
                                      sUserInitStatus initStatus = *(sUserInitStatus*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(sUserInitStatus));

                                      int nUserID = m_pDataInfo->GetUserIDByAccount(initStatus.szUserAccount);
                                      int nListCount = Trader_listView->Items->Count;

                                      String^ szStatus = ProcessStautsText(initStatus.meStatus,initStatus.ldbDuration);

                                      for (int j=0;j<nListCount;j++)
                                      {
                                          if((int)Trader_listView->Items[j]->Tag == nUserID)
                                          {
                                              Trader_listView->Items[j]->SubItems[9]->Text = szStatus;
                                              break;
                                          }
                                      }
                                  }
                              }
                              else
                              {
                                 // strError = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
								  int nListCount = Trader_listView->Items->Count;
								  for (int j=0;j<nListCount;j++)
								  { 
									  String^ szStatus = String::Empty;
									  szStatus = Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
									  Trader_listView->Items[j]->SubItems[9]->Text = szStatus;
								  }
                              }

                              break;
                          }
                      case Cmd_TradeDataManager_InitUser_Rsp:
                          {
                              if ( head.userdata3 != CF_ERROR_SUCCESS )
                              {
                                  strError = "初始化用户操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }else
                                  strError = "初始化用户操作成功";

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
    private: System::Void SystemUserMgrForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 this->CenterToScreen();

                 int nHwnd = this->Handle.ToInt32();
                 m_pDataInfo->GetLogonInfo(*m_pLogin);


                 CTcpLayer::SubscribePkg(Cmd_AddUser_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_DelUser_Rsp, nHwnd);             
                 CTcpLayer::SubscribePkg(Cmd_ModifyUser_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_SaveUserOrgRelation_FromUser_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_SaveUserTradeAcctRelation_Rsp, nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_SaveUserRoleRelation_Rsp, nHwnd); 
                 CTcpLayer::SubscribePkg(Cmd_SaveProductTraderRelation_FromUser_Rsp, nHwnd);  
                 CTcpLayer::SubscribePkg(Cmd_UserStatus_Rsp,nHwnd);                 
                 CTcpLayer::SubscribePkg(Cmd_TradeDataManager_QryUserInitStatus_Rsp,nHwnd);
                 CTcpLayer::SubscribePkg(Cmd_TradeDataManager_InitUser_Rsp,nHwnd);


                 CTcpLayer::SubscribePkg(Cmd_QueryOrg_Rsp, nHwnd);//组织机构     
                 CTcpLayer::SendData(Cmd_QueryOrg_Req, NULL,0, 0, m_pLogin->mnUserID);          

                 CTcpLayer::SubscribePkg(Cmd_QryUser_Rsp, nHwnd);//用户
                 CTcpLayer::SendData(Cmd_QryUser_Req, NULL,0, 0, m_pLogin->mnUserID);

                 CTcpLayer::SubscribePkg(Cmd_QryRole_Rsp, nHwnd);//角色(仅显示分类 选择用户类型后对应的角色 对应RoleInfo)
                 CTcpLayer::SendData(Cmd_QryRole_Req, NULL,0, 0, m_pLogin->mnUserID);

                 CTcpLayer::SubscribePkg(Cmd_QryUserRoleRelation_Rsp, nHwnd);//用户与角色关系(对应UserRole)
                 CTcpLayer::SendData(Cmd_QryUserRoleRelation_Req, NULL,0, 0, m_pLogin->mnUserID);

                 CTcpLayer::SubscribePkg(Cmd_QryUserOrgRelation_Rsp, nHwnd);//用户与组织机构关系
                 CTcpLayer::SendData(Cmd_QryUserOrgRelation_Req, NULL,0, 0, m_pLogin->mnUserID);

                 CTcpLayer::SubscribePkg(Cmd_QueryTradeAccount_Rsp, nHwnd);//委托交易账户
                 CTcpLayer::SendData(Cmd_QueryTradeAccount_Req, NULL,0, 0, m_pLogin->mnUserID);

                 CTcpLayer::SubscribePkg(Cmd_QryUserTradeAcctRelation_Rsp, nHwnd);//用户与委托交易账号关系
                 CTcpLayer::SendData(Cmd_QryUserTradeAcctRelation_Req, NULL,0, 0, m_pLogin->mnUserID);

                 CTcpLayer::SubscribePkg(Cmd_QueryFinicialProduct_Rsp, nHwnd);//理财产品
                 CTcpLayer::SendData(Cmd_QueryFinicialProduct_Req, NULL,0, 0, m_pLogin->mnUserID);

                 CTcpLayer::SubscribePkg(Cmd_QueryProductTraderRelation_Rsp, nHwnd);//理财产品与交易员关系
                 CTcpLayer::SendData(Cmd_QueryProductTraderRelation_Req, NULL,0, 0, m_pLogin->mnUserID);

                 CTcpLayer::SubscribePkg(CMDID_AllUserLoginStatus_Push, nHwnd);//用户登录状态  
                 CTcpLayer::SubscribePkg(CMDID_SubscriberAllUserLoginStatus_Rsp, nHwnd);                 
                 
                 timer_Init->Start();
             }
    private: System::Void Add_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_SYSTEMUSER_ADD));

                 SystemUserForm^ dlg = gcnew SystemUserForm();
                 dlg->SetOperType(Oper_Add);
                 dlg->SetEnable(true);
                 dlg->SetUserType(m_nUserType);
                 if( ::DialogResult::OK == dlg->ShowDialog(this))
                 {
                     *m_pCurUserInfo = *(dlg->GetCurUserInfo());
                     m_strUserRole = dlg->GetCurUserRole();
                     CTcpLayer::SendData(Cmd_AddUser_Req, m_pCurUserInfo, sizeof(UserInfo), 0, m_pLogin->mnUserID);
                 }
             }
    private: System::Void Modify_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_SYSTEMUSER_MODIFY));
                 String^ strUserAccount;
                 String^ strLoginStatus;
                 if (m_strUserType == "交易员")                 
                 {
                     if(Trader_listView->SelectedIndices != nullptr && Trader_listView->SelectedIndices->Count>0) 
                     {
                         strUserAccount = Trader_listView->SelectedItems[0]->SubItems[0]->Text;
                         strLoginStatus = Trader_listView->SelectedItems[0]->SubItems[8]->Text;
                         if(0==strLoginStatus->CompareTo("已登录"))
                         {
                             MessageBox::Show("交易员已登录，无法修改！", "提示",MessageBoxButtons::OK);
                             return;
                         }

                     }
                     else
                     {
                         MessageBox::Show("请先选择一个交易员！", "提示",MessageBoxButtons::OK);
                         return;
                     }
                 }else if (m_strUserType == "管理员")
                 {
                     if(Admin_listView->SelectedIndices != nullptr && Admin_listView->SelectedIndices->Count>0) 
                     {
                         strUserAccount = Admin_listView->SelectedItems[0]->SubItems[0]->Text;
                         strLoginStatus = Admin_listView->SelectedItems[0]->SubItems[6]->Text;
                         if(0==strLoginStatus->CompareTo("已登录"))
                         {
                             MessageBox::Show("管理员已登录，无法修改！", "提示",MessageBoxButtons::OK);
                             return;
                         }
                     }
                     else
                     {
                         MessageBox::Show("请先选择一个管理员！", "提示",MessageBoxButtons::OK);
                         return;
                     }
                 }else if (m_strUserType == "风控员")
                 {
                     if(Risk_listView->SelectedIndices != nullptr && Risk_listView->SelectedIndices->Count>0) 
                     {
                         strUserAccount = Risk_listView->SelectedItems[0]->SubItems[0]->Text;
                         strLoginStatus = Risk_listView->SelectedItems[0]->SubItems[8]->Text;
                         if(0==strLoginStatus->CompareTo("已登录"))
                         {
                             MessageBox::Show("风控员已登录，无法修改！", "提示",MessageBoxButtons::OK);
                             return;
                         }
                     }
                     else
                     {
                         MessageBox::Show("请先选择一个风控员！", "提示",MessageBoxButtons::OK);
                         return;
                     }
                 }else if (m_strUserType == "展现员")
                 {
                     if(Viewer_listView->SelectedIndices != nullptr && Viewer_listView->SelectedIndices->Count>0) 
                     {
                         strUserAccount = Viewer_listView->SelectedItems[0]->SubItems[0]->Text;
                         strLoginStatus = Viewer_listView->SelectedItems[0]->SubItems[5]->Text;
                         if(0==strLoginStatus->CompareTo("已登录"))
                         {
                             MessageBox::Show("展现员已登录，无法修改！", "提示",MessageBoxButtons::OK);
                             return;
                         }
                     }
                     else
                     {
                         MessageBox::Show("请先选择一个展现员！", "提示",MessageBoxButtons::OK);
                         return;
                     }                 
                 }

                 if(!m_pDataInfo->GetUserInfoByAccount(Tools::String2string(strUserAccount),*m_pCurUserInfo))
                     return;

                 string StrRole = m_pDataInfo->GetRoleByUserID(m_pCurUserInfo->nUserID);

                 SystemUserForm^ dlg = gcnew SystemUserForm();
                 dlg->SetOperType(Oper_Modify);
                 dlg->SetEnable(false);
                 dlg->SetCurUserRole(Tools::string2String(StrRole.c_str()));
                 dlg->SetCurUserInfo(*m_pCurUserInfo);
                 dlg->SetUserType(m_nUserType);
                 if( ::DialogResult::OK == dlg->ShowDialog(this))
                 {
                     m_strUserRole = dlg->GetCurUserRole();
                     //修改角色
                     std::vector<int> vecRole;
                     cli::array<System::String^>^ sArray = m_strUserRole->Split(',');  
                     for each (String^ item in sArray)  
                     {
                         String^ sRole = item->ToString()->Trim();
                         int nRoleID = m_pDataInfo->GetRoleIDByName(Tools::String2string(sRole));
                         if(nRoleID<0)
                             continue;

                         vecRole.push_back(nRoleID);
                     }

                     int nDataLen = sizeof(int)+sizeof(int)*vecRole.size();
                     char* pBuffer = new char[nDataLen];
                     memset(pBuffer, 0, nDataLen);
                     memcpy(pBuffer, &(m_pCurUserInfo->nUserID), sizeof(int));
                     if (!vecRole.empty())                            
                         memcpy(pBuffer+sizeof(int), &vecRole[0], sizeof(int)*vecRole.size());  

                     CTcpLayer::SendData(Cmd_SaveUserRoleRelation_Req, pBuffer, nDataLen, 0, m_pLogin->mnUserID); 

                     if(*m_pCurUserInfo == *(dlg->GetCurUserInfo()))
                         return;
                     else
                     {
                         *m_pCurUserInfo = *(dlg->GetCurUserInfo());
                         CTcpLayer::SendData(Cmd_ModifyUser_Req, m_pCurUserInfo, sizeof(UserInfo), 0, m_pLogin->mnUserID);    
                     }
                 }
             }
    private: System::Void Status_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 String^ strUserAccount,^strLoginStatus;
                 bool bAdminType = false;
                 if (m_strUserType == "交易员")                 
                 {
                     LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_SYSTEMUSER_USE));
                     if(Trader_listView->SelectedIndices != nullptr && Trader_listView->SelectedIndices->Count>0) 
                     {
                         strUserAccount = Trader_listView->SelectedItems[0]->SubItems[0]->Text;

                         UserStatus userStatus;
                         userStatus.nUserID = m_pCurUserInfo->nUserID;
                         userStatus.nStatus = (0==m_pCurUserInfo->nStatus)?1:0;
                         CTcpLayer::SendData(Cmd_UserStatus_Req, &userStatus, sizeof(UserStatus), 0, m_pLogin->mnUserID);
                         return;
                     }
                     else
                     {
                         MessageBox::Show("请先选择一个交易员！", "提示",MessageBoxButtons::OK);
                         return;
                     }
                 }else if (m_strUserType == "管理员")
                 {
                     LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_SYSTEMUSER_DEL));
                     bAdminType = true;
                     if(Admin_listView->SelectedIndices != nullptr && Admin_listView->SelectedIndices->Count>0) 
                     {
                         strUserAccount = Admin_listView->SelectedItems[0]->SubItems[0]->Text;
                         strLoginStatus = Admin_listView->SelectedItems[0]->SubItems[6]->Text;
                     }
                     else
                     {
                         MessageBox::Show("请先选择一个管理员！", "提示",MessageBoxButtons::OK);
                         return;
                     }
                 }else if (m_strUserType == "风控员")
                 {
                     LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_SYSTEMUSER_DEL));
                     if(Risk_listView->SelectedIndices != nullptr && Risk_listView->SelectedIndices->Count>0) 
                     {
                         strUserAccount = Risk_listView->SelectedItems[0]->SubItems[0]->Text;
                         strLoginStatus = Risk_listView->SelectedItems[0]->SubItems[8]->Text;
                     }
                     else
                     {
                         MessageBox::Show("请先选择一个风控员！", "提示",MessageBoxButtons::OK);
                         return;
                     }
                 }else if (m_strUserType == "展现员")
                 {
                     LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_SYSTEMUSER_DEL));
                     if(Viewer_listView->SelectedIndices != nullptr && Viewer_listView->SelectedIndices->Count>0) 
                     {
                         strUserAccount = Viewer_listView->SelectedItems[0]->SubItems[0]->Text;
                         strLoginStatus = Viewer_listView->SelectedItems[0]->SubItems[5]->Text;
                     }
                     else
                     {
                         MessageBox::Show("请先选择一个展现员！", "提示",MessageBoxButtons::OK);
                         return;
                     }                 
                 }                 

                 int nUserID = m_pDataInfo->GetUserIDByAccount(Tools::String2string(strUserAccount));
                 if( USER_ADMIN == nUserID)//默认管理员
                 {
                     MessageBox::Show("默认管理员不能删除!");
                     return;
                 }

                 if(0==strLoginStatus->CompareTo(STATUS_LOGIN))
                 {
                     MessageBox::Show("用户已登录，无法删除！", "提示",MessageBoxButtons::OK);
                     return;
                 }

                 if(bAdminType)
                 {
                     if(::DialogResult::Cancel ==  MessageBox::Show("此操作将会同步删除和角色的关系。确认吗？", "提示", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
                     return;
                 }else
                 {
                     if(::DialogResult::Cancel ==  MessageBox::Show("此操作将会同步删除和角色的关系，同步删除和部门的关系。确认吗？", "提示", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
                         return;
                 }

                 CTcpLayer::SendData(Cmd_DelUser_Req, &nUserID, sizeof(int), 0, m_pLogin->mnUserID);
             }
    private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNORG));
                 if(Trader_listView->SelectedIndices != nullptr && Trader_listView->SelectedIndices->Count>0)
                 {
                     String^ strUserAccount = Trader_listView->SelectedItems[0]->SubItems[0]->Text;
                     if(!m_pDataInfo->GetUserInfoByAccount(Tools::String2string(strUserAccount),*m_pCurUserInfo))
                         return;

                     string strOrg = m_pDataInfo->GetOrgNameByUserID(m_pCurUserInfo->nUserID);

                     TraderAddOrgForm^ dlg = gcnew TraderAddOrgForm();
                     m_pTraderAddOrgForm = dlg;
                     dlg->SetCurUserInfo(*m_pCurUserInfo);   
                     dlg->SetCurOrg(Tools::string2String(strOrg.c_str()));
                     if( ::DialogResult::OK == dlg->ShowDialog(this))
                     {
                         int nUserID = m_pDataInfo->GetUserIDByAccount(Tools::String2string(Trader_listView->SelectedItems[0]->SubItems[0]->Text));
                         int nOrgID  = m_pDataInfo->GetOrganizationIDByName(Tools::String2string(dlg->GetCurOrg()));
                         if(nOrgID<0)
                         {
                             MessageBox::Show("请选择组织机构部门");
                             return;
                         }

                         std::vector<int> vecUserIn;
                         vecUserIn.push_back(nUserID);

                         std::vector<int> vecUserOut;

                         int nVecSizeIn = 1;
                         int nVecSizeOut = 0;

                         int nRelation = USER_ORG_RELATION_ATTACH;

                         int nDataLen = 4*sizeof(int);
                         char* pBuffer = new char[nDataLen];
                         memset(pBuffer, 0, nDataLen);
                         memcpy(pBuffer, &(m_pCurUserInfo->nUserType), sizeof(int));
                         memcpy(pBuffer+sizeof(int), &nRelation, sizeof(int));
                         memcpy(pBuffer+2*sizeof(int), &nOrgID, sizeof(int));
                         memcpy(pBuffer+3*sizeof(int), &nUserID, sizeof(int));

                         CTcpLayer::SendData(Cmd_SaveUserOrgRelation_FromUser_Req, pBuffer, nDataLen, 0, m_pLogin->mnUserID); 

                         delete [] pBuffer;
                     }
                 }
                 else
                     MessageBox::Show("请先选择一个交易员！", "提示",MessageBoxButtons::OK);
             }
    private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNPRODUCT));
                 if(Trader_listView->SelectedIndices != nullptr && Trader_listView->SelectedIndices->Count>0)
                 {
                     String^ strUserAccount = Trader_listView->SelectedItems[0]->SubItems[0]->Text;
                     if(!m_pDataInfo->GetUserInfoByAccount(Tools::String2string(strUserAccount),*m_pCurUserInfo))
                         return;


                     string strProduct = m_pDataInfo->GetProductByUserID(m_pCurUserInfo->nUserID);

                     TraderAddProductForm^ dlg = gcnew TraderAddProductForm();
                     m_pTraderAddProductForm = dlg;
                     dlg->SetCurUserInfo(*m_pCurUserInfo);         
                     dlg->SetCurProduct(Tools::string2String(strProduct.c_str()));
                     if( ::DialogResult::OK == dlg->ShowDialog(this))
                     {
                         int nUserID = m_pDataInfo->GetUserIDByAccount(Tools::String2string(Trader_listView->SelectedItems[0]->SubItems[0]->Text));
                         String^ sProductID  = dlg->GetCurProduct();                     

                         std::vector<int> vecProductID;
                         cli::array<System::String^>^ sArray = sProductID->Split(',');  
                         for each (String^ item in sArray)  
                         {
                             String^ sProduct = item->ToString()->Trim();
                             int nProductID = m_pDataInfo->GetFinancialProductIDByName(Tools::String2string(sProduct));
                             if( nProductID < 0 )
                                 break;
                             else                                        
                             {
                                 vecProductID.push_back(nProductID);
                             }
                         }

                         int nTraderProRelationType = TRADER_ASSGIN_PRODUCT_TYPE;
                         int nDataLen = 2*sizeof(int)+vecProductID.size()*sizeof(int);
                         char* pBuffer = new char[nDataLen];
                         memset(pBuffer, 0, nDataLen);
                         memcpy(pBuffer, &nTraderProRelationType, sizeof(int));
                         memcpy(pBuffer+sizeof(int), &nUserID, sizeof(int));

                         std::vector<int>::iterator it = vecProductID.begin();
                         for (; it!=vecProductID.end(); it++)                            
                             memcpy(pBuffer+2*sizeof(int), &vecProductID[0], sizeof(int)*vecProductID.size());                             

                         CTcpLayer::SendData(Cmd_SaveProductTraderRelation_FromUser_Req, pBuffer, nDataLen, 0, m_pLogin->mnUserID); 

                         delete [] pBuffer;  
                     }
                 }
                 else
                     MessageBox::Show("请先选择一个交易员！", "提示",MessageBoxButtons::OK);
             }
    private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
                 String^ sText = button3->Text;
                 if ( sText->CompareTo("指定委托交易账号") == 0 )
                 {          
                     LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNACCOUNT));
                     if(Trader_listView->SelectedIndices != nullptr && Trader_listView->SelectedIndices->Count>0)
                     {
                         String^ strUserAccount = Trader_listView->SelectedItems[0]->SubItems[0]->Text;
                         m_pDataInfo->GetUserInfoByAccount(Tools::String2string(strUserAccount),*m_pCurUserInfo);

                         TraderAddTradeAccountForm^ dlg = gcnew TraderAddTradeAccountForm();
                         m_pTraderAddTradeAccountForm = dlg;
                         dlg->SetCurUserInfo(*m_pCurUserInfo); 


                         std::map<int, UserAndTradeAccountRelation> mapUserAndOrgRelation;
                         m_pDataInfo->GetUserAndTraderAccountRelation(mapUserAndOrgRelation);
                         std::map<int, UserAndTradeAccountRelation>::iterator it2 = mapUserAndOrgRelation.find(m_pCurUserInfo->nUserID);
                         if(it2!=mapUserAndOrgRelation.end())
                             dlg->SetCurRelation(it2->second);  

                         if( ::DialogResult::OK == dlg->ShowDialog(this))
                         {
                             int nUserID = m_pCurUserInfo->nUserID;
                             int nTradeAccountID  = m_pDataInfo->GetTradeAccountIDByAccount(Tools::String2string(dlg->GetCurTradeAccount()));

                             UserAndTradeAccountRelation oRelation;
                             oRelation.nUserID = nUserID;
                             oRelation.nTradeAccountID  = nTradeAccountID;
                             oRelation.nCanInputOrder = dlg->GetBoolOrder();
                             oRelation.nNeedVerify = dlg->GetBoolVerify();

                             CTcpLayer::SendData(Cmd_SaveUserTradeAcctRelation_Req, &oRelation, sizeof(UserAndTradeAccountRelation), 0, m_pLogin->mnUserID);
                         }
                     }
                     else
                         MessageBox::Show("请先选择一个交易员！", "提示",MessageBoxButtons::OK);
                 }else if ( sText->CompareTo("组织机构设置") == 0 )
                 {
                     if(Risk_listView->SelectedIndices != nullptr && Risk_listView->SelectedIndices->Count>0)
                     {
                         LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_SYSTEMUSER_ASSIGNORG));
                         String^ strUserAccount = Risk_listView->SelectedItems[0]->SubItems[0]->Text;
                         m_pDataInfo->GetUserInfoByAccount(Tools::String2string(strUserAccount),*m_pCurUserInfo);
                         string strName = m_pDataInfo->GetOrgNameByUserID(m_pCurUserInfo->nUserID);

                         RiskAddOrganizeForm^ dlg = gcnew RiskAddOrganizeForm();
                         m_pRiskAddOrganizeForm = dlg;
                         dlg->SetCurUserInfo(*m_pCurUserInfo);   
                         dlg->SetCurOrg(Tools::string2String(strName.c_str()));

                         if( ::DialogResult::OK == dlg->ShowDialog(this))
                         {   
                             int nUserType = USER_TYPE_RISK_CONTROL;
                             int nRelationType = USER_ORG_RELATION_ATTACH;
                             int nOrgID = m_pDataInfo->GetOrganizationIDByName(Tools::String2string(dlg->GetCurOrg())); 

                             std::vector<int> vecOrg;
                             dlg->GetListenOrg(vecOrg);

                             int nVecSize = vecOrg.size();

                             int nDataLen = 4*sizeof(int)+sizeof(int)+sizeof(int)+nVecSize*sizeof(int);
                             char* pBuffer = new char[nDataLen];
                             memset(pBuffer, 0, nDataLen);
                             memcpy(pBuffer, &nUserType, sizeof(int));
                             memcpy(pBuffer+sizeof(int), &(m_pCurUserInfo->nUserID), sizeof(int));
                             memcpy(pBuffer+2*sizeof(int), &nRelationType, sizeof(int));
                             memcpy(pBuffer+3*sizeof(int), &nOrgID, sizeof(int));

                             nRelationType = USER_ORG_RELATION_MONITOR;
                             memcpy(pBuffer+4*sizeof(int), &nRelationType, sizeof(int));
                             memcpy(pBuffer+5*sizeof(int), &nVecSize, sizeof(int));

                             std::vector<int>::iterator it = vecOrg.begin();
                             if ( !vecOrg.empty())                            
                                 memcpy(pBuffer+6*sizeof(int), &vecOrg[0], sizeof(int)*nVecSize);  
                          

                             CTcpLayer::SendData(Cmd_SaveUserOrgRelation_FromUser_Req, pBuffer, nDataLen, 0, m_pLogin->mnUserID); 

                             delete [] pBuffer;  
                         }
                     }
                     else
                         MessageBox::Show("请先选择一个风控员！", "提示",MessageBoxButtons::OK);
                 }                 
             }
    private: void LoadUserInfo()
             {
                 if(m_pDataInfo == NULL)
                     return;

                 Trader_listView->Items->Clear();

                 std::map<int, UserInfo> mapUserInfo;
                 m_pDataInfo->GetUserInfo(mapUserInfo);

                 std::vector<UserAndOrgRelation> vecUserAndOrgRelation;
                 m_pDataInfo->GetUserAndOrgRelation(vecUserAndOrgRelation);

                 std::vector<TraderProRelation> vecTraderProRelation;
                 m_pDataInfo->GetProTraderRelation(vecTraderProRelation);

                 std::map<int, UserAndTradeAccountRelation> mapUserAndTradeAccountRelation;
                 m_pDataInfo->GetUserAndTraderAccountRelation(mapUserAndTradeAccountRelation);

                 std::map<int, std::vector<UserRole>> mapUserRole;
                 m_pDataInfo->GetUserRoleRelation(mapUserRole);


                 string strUserStatus;
                 string strUserType;

                 int i=0,j=0,m=0,n=0;
                 std::map<int, UserInfo>::iterator itUserInfo = mapUserInfo.begin();
                 for (;itUserInfo!=mapUserInfo.end();itUserInfo++)
                 {
                     switch(itUserInfo->second.nStatus)
                     {
                     case 0:
                         strUserStatus = "停用";
                         break;
                     case 1:
                         strUserStatus = "启用";
                         break;
                     default:
                         break;
                     }


                     switch(itUserInfo->second.nUserType)
                     {
                     case USER_TYPE_MANAGE:
                         strUserType = "管理员";
                         break;
                     case USER_TYPE_RISK_CONTROL:
                         strUserType = "风控员";
                         break;
                     case USER_TYPE_TRADER:
                         strUserType = "交易员";
                         break;
                     case USER_TYPE_VIEWER:
                         strUserType = "展现员";
                         break;
                     default:
                         break;
                     }

                     //部门
                     string strOrgName;
                     string strSubOrgName = " ",strSubOrg[30];
                     string strTradeAccount;
                     std::vector<UserAndOrgRelation>::iterator itUserAndOrgRelation = vecUserAndOrgRelation.begin();
                     for (;itUserAndOrgRelation != vecUserAndOrgRelation.end(); itUserAndOrgRelation++)
                     {
                         if (itUserAndOrgRelation->nUserID == itUserInfo->first && itUserAndOrgRelation->nRelationType == 0)
                         {
                             strOrgName = m_pDataInfo->GetOrganizationNameByID(itUserAndOrgRelation->nOrgID);
                             break;
                         }
                     }

                     //角色
                     string strRoleName = " ",strRole[30];
                     std::map<int, std::vector<UserRole>>::iterator itUserRole = mapUserRole.find(itUserInfo->first);
                     if (itUserRole != mapUserRole.end())
                     {
                         std::vector<UserRole> vecRole = itUserRole->second;
                         std::vector<UserRole>::iterator itRole = vecRole.begin();
                         for(int index=0; itRole != vecRole.end(); itRole++,index++)
                         {
                             if(index!=0)
                                 strRoleName += ",";

                             strRole[index] = m_pDataInfo->GetRoleNameByID(itRole->nRoleID);

                             strRoleName += strRole[index];
                         }
                     }


                     if (itUserInfo->second.nUserType == USER_TYPE_TRADER)
                     {
                         //理财产品
                         string strProductName = " ",strProduct[30];

                         std::vector<TraderProRelation>::iterator itTraderProRelation = vecTraderProRelation.begin();
                         for (int index=0; itTraderProRelation != vecTraderProRelation.end(); itTraderProRelation++,index++)
                         {
                             if (itTraderProRelation->nTraderID == itUserInfo->first)
                             {
                                 strProduct[index] = m_pDataInfo->GetFinancialProductNameByID(itTraderProRelation->nFinancialProductID);
                                 strProductName += strProduct[index]+",";
                             }
                         }

                         int nLen = strProductName.length();
                         strProductName[nLen-1] = '\0';

                         std::map<int, UserAndTradeAccountRelation>::iterator itUserAndTradeAccountRelation = mapUserAndTradeAccountRelation.find(itUserInfo->first);
                         if (itUserAndTradeAccountRelation != mapUserAndTradeAccountRelation.end())
                         {
                             strTradeAccount = m_pDataInfo->GetTradeAccountByID(itUserAndTradeAccountRelation->second.nTradeAccountID);
                         }

                         ListViewItem^ item = gcnew ListViewItem(Tools::string2String(itUserInfo->second.szAccount),i++);  
                         item->Tag = itUserInfo->first;
                         item->SubItems->Add(Tools::string2String(strOrgName.c_str()));  
                         item->SubItems->Add(Tools::string2String(strProductName.c_str()));  
                         item->SubItems->Add(Tools::string2String(strTradeAccount.c_str()));  
                         item->SubItems->Add(Tools::string2String(strUserStatus.c_str()));  
                         item->SubItems->Add(Tools::string2String(strUserType.c_str()));  
                         item->SubItems->Add(Tools::string2String(itUserInfo->second.szAlias));  
                         item->SubItems->Add(Tools::string2String(itUserInfo->second.szContactInfo));
                         item->SubItems->Add(String::Empty);
                         item->SubItems->Add(String::Empty);

                         Trader_listView->Items->Add(item);
                     }
                     else if (itUserInfo->second.nUserType == USER_TYPE_MANAGE)
                     {
                         ListViewItem^ item = gcnew ListViewItem(Tools::string2String(itUserInfo->second.szAccount),j++);
                         item->Tag = itUserInfo->first;
                         item->SubItems->Add(Tools::string2String(strUserStatus.c_str()));  
                         item->SubItems->Add(Tools::string2String(strUserType.c_str()));  
                         item->SubItems->Add(Tools::string2String(strRoleName.c_str()));  
                         item->SubItems->Add(Tools::string2String(itUserInfo->second.szAlias));  
                         item->SubItems->Add(Tools::string2String(itUserInfo->second.szContactInfo));  
                         item->SubItems->Add(String::Empty); 

                         Admin_listView->Items->Add(item);
                     }
                     else if (itUserInfo->second.nUserType == USER_TYPE_RISK_CONTROL)
                     {
                         //部门
                         std::vector<UserAndOrgRelation>::iterator itUserAndOrgRelation = vecUserAndOrgRelation.begin();
                         for (int index=0; itUserAndOrgRelation != vecUserAndOrgRelation.end(); itUserAndOrgRelation++)
                         {
                             if (itUserAndOrgRelation->nUserID == itUserInfo->first)
                             {
                                 if(itUserAndOrgRelation->nRelationType == 1)                             
                                 {
                                     strSubOrg[index] = m_pDataInfo->GetOrganizationNameByID(itUserAndOrgRelation->nOrgID);
                                     strSubOrgName += strSubOrg[index]+" "; 
                                     index++;
                                 }
                             }
                         }

                         ListViewItem^ item = gcnew ListViewItem(Tools::string2String(itUserInfo->second.szAccount),m++);  
                         item->Tag = itUserInfo->first;
                         item->SubItems->Add(Tools::string2String(strOrgName.c_str()));  
                         item->SubItems->Add(Tools::string2String(strSubOrgName.c_str()));  
                         item->SubItems->Add(Tools::string2String(strUserStatus.c_str()));  
                         item->SubItems->Add(Tools::string2String(strUserType.c_str()));  
                         item->SubItems->Add(Tools::string2String(strRoleName.c_str()));  
                         item->SubItems->Add(Tools::string2String(itUserInfo->second.szAlias));  
                         item->SubItems->Add(Tools::string2String(itUserInfo->second.szContactInfo));  
                         item->SubItems->Add(String::Empty);

                         Risk_listView->Items->Add(item);
                     }
                     else if (itUserInfo->second.nUserType == USER_TYPE_VIEWER)
                     {
                         int nCount = Viewer_listView->Items->Count;

                         ListViewItem^ item = gcnew ListViewItem(Tools::string2String(itUserInfo->second.szAccount),nCount++);    
                         item->Tag = itUserInfo->first;
                         item->SubItems->Add(Tools::string2String(strUserStatus.c_str()));  
                         item->SubItems->Add(Tools::string2String(strUserType.c_str()));  
                         item->SubItems->Add(Tools::string2String(itUserInfo->second.szAlias));   
                         item->SubItems->Add(Tools::string2String(itUserInfo->second.szContactInfo));
                         item->SubItems->Add(String::Empty);

                         Viewer_listView->Items->Add(item);
                     }
                 }
             }

    private: System::Void tabControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
                 switch(tabControl->SelectedIndex)
                 {
                 case 0:
                     m_nUserType = USER_TYPE_TRADER;
                     m_strUserType = "交易员";
                     button1->Text = "分配组织机构";
                     button2->Text = "分配理财产品";
                     button3->Text = "指定委托交易账号";
                     button_UserInitStatus->Visible = true;
                     button1->Visible = true;
                     button2->Visible = true;
                     button3->Visible = true;
                     button3->Enabled = true;
                     if(Trader_listView->SelectedIndices != nullptr && Trader_listView->SelectedIndices->Count>0)
                     {
                         String^ strUserAccount = Trader_listView->SelectedItems[0]->SubItems[0]->Text;
                         if(!m_pDataInfo->GetUserInfoByAccount(Tools::String2string(strUserAccount),*m_pCurUserInfo))
                             return;

                         if(m_pCurUserInfo->nStatus)
                             Status_button->Text = "停用";
                         else
                             Status_button->Text = "启用";
                     }
                     break;
                 case 1:
                     m_nUserType = USER_TYPE_MANAGE;
                     m_strUserType = "管理员"; 
                     button1->Visible = false;
                     button2->Visible = false;
                     button3->Visible = false;
                     Status_button->Text = "删除用户";
                     button_UserInitStatus->Visible = false;
                     break;
                 case 2:
                     m_nUserType = USER_TYPE_RISK_CONTROL;
                     m_strUserType = "风控员";
                     button3->Text = "组织机构设置";
                     button1->Visible = false;
                     button2->Visible = false;
                     button3->Visible = true;
                     button3->Enabled = true;
                     Status_button->Text = "删除用户";
                     button_UserInitStatus->Visible = false;
                     break;
                 case 3:
                     m_nUserType = USER_TYPE_VIEWER;
                     m_strUserType = "展现员";
                     button1->Visible = false;
                     button2->Visible = false;
                     button3->Visible = false;
                     Status_button->Text = "删除用户";
                     button_UserInitStatus->Visible = false;
                     break;
                 default:
                     m_nUserType = USER_TYPE_TRADER;
                     m_strUserType = "交易员";
                     button1->Text = "分配组织机构";
                     button2->Text = "分配理财产品";
                     button3->Text = "指定委托交易账号";
                     button1->Visible = true;
                     button2->Visible = true;
                     button3->Visible = true;
                     button_UserInitStatus->Visible = true;
                     break;
                 }
             }

    private: System::Void timer_Init_Tick(System::Object^  sender, System::EventArgs^  e) {
                 //加载用户
                 if( m_bQryUser && 
                     m_bQryUserRoleRelation && 
                     m_bQryUserOrgRelation && 
                     m_bQryTraderProRelation && 
                     m_bQryUserTradeAccountRelation )
                 {
                     timer_Init->Stop();
                     LoadUserInfo();

                     CTcpLayer::SendData(CMDID_SubscriberAllUserLoginStatus_Req, NULL,0, 0, m_pLogin->mnUserID);

                     timer_LoginStatus->Start();
                 }
             }
    private: System::Void timer_LoginStatus_Tick(System::Object^  sender, System::EventArgs^  e) {                 
                 CTcpLayer::SendData(Cmd_TradeDataManager_QryUserInitStatus_Req, NULL, 0, 0, m_pLogin->mnUserID);
             }
    private: System::Void SystemUserMgrForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
                 timer_LoginStatus->Stop();
                 int nHwnd = this->Handle.ToInt32();
                 CTcpLayer::UnsubscribeAllPkg(nHwnd);
             }
    private: System::Void Trader_listView_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
                 if(Trader_listView->SelectedIndices != nullptr && Trader_listView->SelectedIndices->Count>0)
                 {
                     String^ strUserAccount = Trader_listView->SelectedItems[0]->SubItems[0]->Text;
                     String^ strOrg = Trader_listView->SelectedItems[0]->SubItems[1]->Text->Trim();
                     if(!m_pDataInfo->GetUserInfoByAccount(Tools::String2string(strUserAccount),*m_pCurUserInfo))
                         return;

                     if(m_pCurUserInfo->nStatus)
                         Status_button->Text = "停用";
                     else
                         Status_button->Text = "启用";
                 }
             }
    private: System::Void button_UserInitStatus_Click(System::Object^  sender, System::EventArgs^  e) {
                 LOG_INFO("远程服务器地址<%s> 用户<%d> 操作<%s>",CConfigManager::GetSvrAddr().c_str(),m_pLogin->mnUserID,FormatOprCode(USER_OPR_ACCOUNT_SYSTEMUSER_INIT));
                 if(Trader_listView->SelectedIndices != nullptr && Trader_listView->SelectedIndices->Count>0) 
                 {
                     int nUserID = (int)(Trader_listView->SelectedItems[0]->Tag);
                     CTcpLayer::SendData(Cmd_TradeDataManager_InitUser_Req, &nUserID, sizeof(int), 0, m_pLogin->mnUserID);
                 }
                 else
                 {
                     MessageBox::Show("请先选择一个交易员！", "提示",MessageBoxButtons::OK);
                     return;
                 }
             }
    private: System::Void button_LoginStatusQuery_Click(System::Object^  sender, System::EventArgs^  e) {
                 CTcpLayer::SendData(CMDID_SubscriberAllUserLoginStatus_Req, NULL,0, 0, m_pLogin->mnUserID);
             }
};
}

#pragma once

#include <cliext/vector>
#include "CommonDef.h"
#include "CCommonFunc.h"
#include "RiskData.h"
#include "FormOrganizeCheckedTreeList.h"
#include "FormInstrumentIDTreeList.h"
#include "FormRiskPlanSetup_Add_RiskLevel.h"
#include "FormFundPerValue.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

extern string           g_riskPlanOrg;
extern Key_RiskPlan     g_riskPlanKey;
extern vector<RiskPlan> g_vecRiskPlan;
extern map<int,double>  g_mapRiskLevelInfo2;

namespace CltRisk {

    /// <summary>
    /// Summary for FormRiskPlanSetup_Add
    ///
    /// WARNING: If you change the name of this class, you will need to change the
    ///          'Resource File Name' property for the managed resource compiler tool
    ///          associated with all .resx files this class depends on.  Otherwise,
    ///          the designers will not be able to interact properly with localized
    ///          resources associated with this form.
    /// </summary>
    public ref class FormRiskPlanSetup_Add : public System::Windows::Forms::Form
    {
    public:
        FormRiskPlanSetup_Add(void)
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //
            m_pLoginInfo = new sLoginRsp;
            CRiskData::GetInstance()->GetLogonInfo(*m_pLoginInfo);
            m_pType = new OperType;
            m_pCurRiskWaring = new RiskWaring;
            memset(m_pCurRiskWaring,0,sizeof(RiskWaring));
            m_pNetFundParam = new NetFundParam;
            memset(m_pNetFundParam,0,sizeof(NetFundParam));
            m_bOkClicked = false;
            m_bUse = true;
		}

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~FormRiskPlanSetup_Add()
        {
            if (components)
            {
                delete components;
            }
            if ( NULL != m_pCurRiskWaring )
            {
                delete m_pCurRiskWaring;
                m_pCurRiskWaring = NULL;
            }
            if(m_pNetFundParam)
            {
                delete m_pNetFundParam;
                m_pNetFundParam = NULL;
            }
            if ( NULL != m_pType )
            {
                delete m_pType;
                m_pType = NULL;
            }
            if ( NULL != m_pLoginInfo )
            {
                delete m_pLoginInfo;
                m_pLoginInfo = NULL;
            }
        }

    private: FormOrganizeCheckedTreeList^ m_FormOrganizeCheckedTreeList;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
    private: System::Windows::Forms::Button^  button_Modify;



    private: FormInstrumentIDTreeList^ m_FormInstrumentIDTreeList;



//     private: AnywndComboBox::AnywndComboBoxControl^  anywndComboBoxControl_Org;
    protected: 
    private: AnywndComboBox::AnywndComboBoxControl^  anywndComboBoxControl_Product;
    private: System::Windows::Forms::ComboBox^  comboBox_RiskIndicator;
    private: System::Windows::Forms::RadioButton^  radioButton_AllProduct;
    private: System::Windows::Forms::RadioButton^  radioButton_SingleProduct;
    private: System::Windows::Forms::Button^  button_AddRiskLevel;
    private: System::Windows::Forms::Button^  button_DelRiskLevel;
    private: System::Windows::Forms::Button^  button_OK;




    private: System::Windows::Forms::Button^  button_Cancel;

    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;

    private: System::Windows::Forms::ListView^  listView_RiskPlan;


    private: System::Windows::Forms::ColumnHeader^  columnHeader1;
    private: System::Windows::Forms::ColumnHeader^  columnHeader2;
    private: System::Windows::Forms::ColumnHeader^  columnHeader3;
    private: System::Windows::Forms::Button^  button_FundPerValue;

    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>
        System::ComponentModel::Container ^components;

    private:
        RiskWaring* m_pCurRiskWaring;
        int m_nCurActionValue;
        NetFundParam* m_pNetFundParam;
        OperType* m_pType;
        bool m_bUse;
    private: System::Windows::Forms::ComboBox^  comboBox_Org;

    private: System::Windows::Forms::GroupBox^  groupBox1;
             sLoginRsp* m_pLoginInfo;
         bool m_bOkClicked;
         String^ m_szOrgText;

#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
            this->comboBox_RiskIndicator = (gcnew System::Windows::Forms::ComboBox());
            this->radioButton_AllProduct = (gcnew System::Windows::Forms::RadioButton());
            this->radioButton_SingleProduct = (gcnew System::Windows::Forms::RadioButton());
            this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->button_AddRiskLevel = (gcnew System::Windows::Forms::Button());
            this->button_DelRiskLevel = (gcnew System::Windows::Forms::Button());
            this->button_OK = (gcnew System::Windows::Forms::Button());
            this->button_Cancel = (gcnew System::Windows::Forms::Button());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->anywndComboBoxControl_Product = (gcnew AnywndComboBox::AnywndComboBoxControl());
            this->button_Modify = (gcnew System::Windows::Forms::Button());
            this->listView_RiskPlan = (gcnew System::Windows::Forms::ListView());
            this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
            this->button_FundPerValue = (gcnew System::Windows::Forms::Button());
            this->comboBox_Org = (gcnew System::Windows::Forms::ComboBox());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->SuspendLayout();
            // 
            // comboBox_RiskIndicator
            // 
            this->comboBox_RiskIndicator->DisplayMember = L"0";
            this->comboBox_RiskIndicator->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->comboBox_RiskIndicator->FormattingEnabled = true;
            this->comboBox_RiskIndicator->Items->AddRange(gcnew cli::array< System::Object^  >(8) {L"合约最大持仓", L"市场冲击风险", L"账户限亏", L"今日限亏", 
                L"隔夜跳空", L"保证金比例风险", L"单笔持仓时间", L"基金净值风险"});
            this->comboBox_RiskIndicator->Location = System::Drawing::Point(83, 72);
            this->comboBox_RiskIndicator->Name = L"comboBox_RiskIndicator";
            this->comboBox_RiskIndicator->Size = System::Drawing::Size(121, 20);
            this->comboBox_RiskIndicator->TabIndex = 2;
            this->comboBox_RiskIndicator->SelectedIndexChanged += gcnew System::EventHandler(this, &FormRiskPlanSetup_Add::comboBox_RiskIndicator_SelectedIndexChanged);
            // 
            // radioButton_AllProduct
            // 
            this->radioButton_AllProduct->AutoSize = true;
            this->radioButton_AllProduct->Location = System::Drawing::Point(304, 26);
            this->radioButton_AllProduct->Name = L"radioButton_AllProduct";
            this->radioButton_AllProduct->Size = System::Drawing::Size(71, 16);
            this->radioButton_AllProduct->TabIndex = 3;
            this->radioButton_AllProduct->TabStop = true;
            this->radioButton_AllProduct->Text = L"全部产品";
            this->radioButton_AllProduct->UseVisualStyleBackColor = true;
            this->radioButton_AllProduct->CheckedChanged += gcnew System::EventHandler(this, &FormRiskPlanSetup_Add::radioButton_AllProduct_CheckedChanged);
            // 
            // radioButton_SingleProduct
            // 
            this->radioButton_SingleProduct->AutoSize = true;
            this->radioButton_SingleProduct->Enabled = false;
            this->radioButton_SingleProduct->Location = System::Drawing::Point(304, 45);
            this->radioButton_SingleProduct->Name = L"radioButton_SingleProduct";
            this->radioButton_SingleProduct->Size = System::Drawing::Size(71, 16);
            this->radioButton_SingleProduct->TabIndex = 4;
            this->radioButton_SingleProduct->TabStop = true;
            this->radioButton_SingleProduct->Text = L"指定产品";
            this->radioButton_SingleProduct->UseVisualStyleBackColor = true;
            this->radioButton_SingleProduct->CheckedChanged += gcnew System::EventHandler(this, &FormRiskPlanSetup_Add::radioButton_SingleProduct_CheckedChanged);
            // 
            // Column1
            // 
            this->Column1->HeaderText = L"风险等级";
            this->Column1->Name = L"Column1";
            // 
            // Column2
            // 
            this->Column2->HeaderText = L"阀值";
            this->Column2->Name = L"Column2";
            // 
            // Column3
            // 
            this->Column3->HeaderText = L"动作";
            this->Column3->Name = L"Column3";
            this->Column3->Resizable = System::Windows::Forms::DataGridViewTriState::True;
            this->Column3->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // button_AddRiskLevel
            // 
            this->button_AddRiskLevel->Location = System::Drawing::Point(63, 328);
            this->button_AddRiskLevel->Name = L"button_AddRiskLevel";
            this->button_AddRiskLevel->Size = System::Drawing::Size(75, 23);
            this->button_AddRiskLevel->TabIndex = 6;
            this->button_AddRiskLevel->Text = L"新增";
            this->button_AddRiskLevel->UseVisualStyleBackColor = true;
            this->button_AddRiskLevel->Click += gcnew System::EventHandler(this, &FormRiskPlanSetup_Add::button_AddRiskLevel_Click);
            // 
            // button_DelRiskLevel
            // 
            this->button_DelRiskLevel->Location = System::Drawing::Point(225, 328);
            this->button_DelRiskLevel->Name = L"button_DelRiskLevel";
            this->button_DelRiskLevel->Size = System::Drawing::Size(75, 23);
            this->button_DelRiskLevel->TabIndex = 7;
            this->button_DelRiskLevel->Text = L"删除";
            this->button_DelRiskLevel->UseVisualStyleBackColor = true;
            this->button_DelRiskLevel->Click += gcnew System::EventHandler(this, &FormRiskPlanSetup_Add::button_DelRiskLevel_Click);
            // 
            // button_OK
            // 
            this->button_OK->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->button_OK->Location = System::Drawing::Point(373, 328);
            this->button_OK->Name = L"button_OK";
            this->button_OK->Size = System::Drawing::Size(75, 23);
            this->button_OK->TabIndex = 8;
            this->button_OK->Text = L"确定";
            this->button_OK->UseVisualStyleBackColor = true;
            this->button_OK->Click += gcnew System::EventHandler(this, &FormRiskPlanSetup_Add::button_OK_Click);
            // 
            // button_Cancel
            // 
            this->button_Cancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->button_Cancel->Location = System::Drawing::Point(454, 328);
            this->button_Cancel->Name = L"button_Cancel";
            this->button_Cancel->Size = System::Drawing::Size(75, 23);
            this->button_Cancel->TabIndex = 9;
            this->button_Cancel->Text = L"取消";
            this->button_Cancel->UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(35, 26);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(29, 12);
            this->label1->TabIndex = 10;
            this->label1->Text = L"部门";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(25, 75);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(53, 12);
            this->label2->TabIndex = 11;
            this->label2->Text = L"风控方案";
            // 
            // anywndComboBoxControl_Product
            // 
            this->anywndComboBoxControl_Product->BackColor = System::Drawing::Color::White;
            this->anywndComboBoxControl_Product->ChildControl = nullptr;
            this->anywndComboBoxControl_Product->DropDownHeight = 1;
            this->anywndComboBoxControl_Product->DropDownWidth = 1;
            this->anywndComboBoxControl_Product->Enabled = false;
            this->anywndComboBoxControl_Product->FormattingEnabled = true;
            this->anywndComboBoxControl_Product->IntegralHeight = false;
            this->anywndComboBoxControl_Product->Location = System::Drawing::Point(390, 42);
            this->anywndComboBoxControl_Product->MaxDropDownItems = 1;
            this->anywndComboBoxControl_Product->Name = L"anywndComboBoxControl_Product";
            this->anywndComboBoxControl_Product->Size = System::Drawing::Size(121, 20);
            this->anywndComboBoxControl_Product->TabIndex = 5;
            // 
            // button_Modify
            // 
            this->button_Modify->Location = System::Drawing::Point(144, 328);
            this->button_Modify->Name = L"button_Modify";
            this->button_Modify->Size = System::Drawing::Size(75, 23);
            this->button_Modify->TabIndex = 13;
            this->button_Modify->Text = L"修改";
            this->button_Modify->UseVisualStyleBackColor = true;
            this->button_Modify->Click += gcnew System::EventHandler(this, &FormRiskPlanSetup_Add::button_Modify_Click);
            // 
            // listView_RiskPlan
            // 
            this->listView_RiskPlan->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->columnHeader1, 
                this->columnHeader2, this->columnHeader3});
            this->listView_RiskPlan->FullRowSelect = true;
            this->listView_RiskPlan->GridLines = true;
            this->listView_RiskPlan->Location = System::Drawing::Point(27, 110);
            this->listView_RiskPlan->MultiSelect = false;
            this->listView_RiskPlan->Name = L"listView_RiskPlan";
            this->listView_RiskPlan->Size = System::Drawing::Size(550, 203);
            this->listView_RiskPlan->TabIndex = 14;
            this->listView_RiskPlan->UseCompatibleStateImageBehavior = false;
            this->listView_RiskPlan->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader1
            // 
            this->columnHeader1->Text = L"风险等级";
            this->columnHeader1->Width = 65;
            // 
            // columnHeader2
            // 
            this->columnHeader2->Text = L"阀值";
            this->columnHeader2->Width = 75;
            // 
            // columnHeader3
            // 
            this->columnHeader3->Text = L"动作";
            this->columnHeader3->Width = 427;
            // 
            // button_FundPerValue
            // 
            this->button_FundPerValue->Location = System::Drawing::Point(304, 74);
            this->button_FundPerValue->Name = L"button_FundPerValue";
            this->button_FundPerValue->Size = System::Drawing::Size(134, 23);
            this->button_FundPerValue->TabIndex = 6;
            this->button_FundPerValue->Text = L"资金净值参数设置";
            this->button_FundPerValue->UseVisualStyleBackColor = true;
            this->button_FundPerValue->Visible = false;
            this->button_FundPerValue->Click += gcnew System::EventHandler(this, &FormRiskPlanSetup_Add::button_FundPerValue_Click);
            // 
            // comboBox_Org
            // 
            this->comboBox_Org->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->comboBox_Org->FormattingEnabled = true;
            this->comboBox_Org->Location = System::Drawing::Point(83, 23);
            this->comboBox_Org->Name = L"comboBox_Org";
            this->comboBox_Org->Size = System::Drawing::Size(121, 20);
            this->comboBox_Org->TabIndex = 1;
            this->comboBox_Org->SelectedIndexChanged += gcnew System::EventHandler(this, &FormRiskPlanSetup_Add::comboBox_Org_SelectedIndexChanged);
            // 
            // groupBox1
            // 
            this->groupBox1->Location = System::Drawing::Point(296, 10);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(226, 59);
            this->groupBox1->TabIndex = 15;
            this->groupBox1->TabStop = false;
            // 
            // FormRiskPlanSetup_Add
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(602, 363);
            this->Controls->Add(this->comboBox_Org);
            this->Controls->Add(this->button_FundPerValue);
            this->Controls->Add(this->listView_RiskPlan);
            this->Controls->Add(this->button_Modify);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->button_Cancel);
            this->Controls->Add(this->button_OK);
            this->Controls->Add(this->button_DelRiskLevel);
            this->Controls->Add(this->button_AddRiskLevel);
            this->Controls->Add(this->radioButton_SingleProduct);
            this->Controls->Add(this->radioButton_AllProduct);
            this->Controls->Add(this->comboBox_RiskIndicator);
            this->Controls->Add(this->anywndComboBoxControl_Product);
            this->Controls->Add(this->groupBox1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"FormRiskPlanSetup_Add";
            this->Text = L"部门风险方案设置";
            this->Load += gcnew System::EventHandler(this, &FormRiskPlanSetup_Add::FormRiskPlanSetup_Add_Load);
            this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormRiskPlanSetup_Add::FormRiskPlanSetup_Add_FormClosing);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    public: void SetOperType(OperType eType)
            {
                *m_pType = eType;
            }
    public: void SetOrgText(String^ szOrgText)
            {
            m_szOrgText = szOrgText;
            comboBox_Org->Enabled = (*m_pType == Oper_Add)?true:false;            
            }
    public: String^ GetOrgText()
            {
            return comboBox_Org->Text;
            }
    public: void SetRiskPlanText(String^ szRiskPlanText)
            {
                comboBox_RiskIndicator->Text = szRiskPlanText;
                comboBox_RiskIndicator->Enabled = false;
            }
    public: String^ GetRiskPlanText()
            {
                return comboBox_RiskIndicator->Text;
            }
    public: String^ GetRiskIndicatorText()
            {
                return comboBox_RiskIndicator->Text;
            }
    private: void LoadRiskIndicator()
             {
                 comboBox_RiskIndicator->Items->Clear();
                 map<RiskIndicatorType, RiskIndicator> mapRiskIndicator;
                 CRiskData::GetInstance()->GetRiskIndicator(mapRiskIndicator);
                 for (map<RiskIndicatorType, RiskIndicator>::iterator it = mapRiskIndicator.begin();it!=mapRiskIndicator.end();it++)
                 {
                     comboBox_RiskIndicator->Items->Add(Tools::string2String(it->second.szName));
                 }            
             }
     void SetInstProductVaule(String^ szInstText)
     {
         array<System::String^>^ sArray = szInstText->Split(';');
         if(sArray->Length == 0)
             return;

         String^ szProduct = String::Empty;
         for each (String^ item in sArray)  
         {
             szProduct = item->ToString()->Trim();
             m_FormInstrumentIDTreeList->SetCheckTreeNode(szProduct);
         }

     }
    public: void LoadData()
            {
                listView_RiskPlan->Items->Clear();

                int nOrgID;
                RiskIndicatorType nRiskIndicatorID;
                if(!CRiskData::GetInstance()->GetOrganizationIDByName(Tools::String2string(comboBox_Org->Text),nOrgID))
                    return;
                if(!CRiskData::GetInstance()->GetIndicatorIDByName(Tools::String2string(comboBox_RiskIndicator->Text),nRiskIndicatorID))
                    return;

                button_FundPerValue->Visible = false;
                switch(nRiskIndicatorID)
                {
                case RI_MaxPosition:
                case RI_MarketShock:
                case RI_SingleHoldTime:
                    radioButton_SingleProduct->Enabled = true;
                    anywndComboBoxControl_Product->Enabled = false;
                    break;
                case RI_AccountLoss:
                case RI_TodayLoss:
                case RI_GappedMarket:
                case RI_MarginRatio:
                    radioButton_SingleProduct->Enabled = false;
                    anywndComboBoxControl_Product->Enabled = false;
                    break;
                case RI_FundNetValue:
                    button_FundPerValue->Visible = true;
                    radioButton_SingleProduct->Enabled = false;
                    anywndComboBoxControl_Product->Enabled = false;
                    break;
                }  

                vector<RiskPlan> vecRiskPlan;
                Key_RiskPlan key(nOrgID,nRiskIndicatorID);
                if(!CRiskData::GetInstance()->GetKey2RiskPlan(key,vecRiskPlan))
                {                 
                    radioButton_AllProduct->Checked = true;
                    radioButton_SingleProduct->Checked = false;
                    anywndComboBoxControl_Product->Text = String::Empty;
                    return;
                }

                RiskPlan riskPlan = vecRiskPlan[0];

                if(riskPlan.OrgIDPlanRelation.bAllProduct)
                {
                    radioButton_AllProduct->Checked = true;
                    radioButton_SingleProduct->Checked = false;
                    anywndComboBoxControl_Product->Text = String::Empty;
                }
                else
                {
                    radioButton_AllProduct->Checked = false;
                    radioButton_SingleProduct->Checked = true;
                    anywndComboBoxControl_Product->Enabled = true;
                    anywndComboBoxControl_Product->Text = Tools::string2String(riskPlan.OrgIDPlanRelation.ProductID);
                    SetInstProductVaule(Tools::string2String(riskPlan.OrgIDPlanRelation.ProductID));
                }

                vector<RiskPlan>::iterator it = vecRiskPlan.begin();
                for (;it!=vecRiskPlan.end();it++)
                {
                    RiskPlan riskPlan = *it;

                    int nCount = listView_RiskPlan->Items->Count;
                    int nRiskIndicatorID = (int)(riskPlan.WaringLevel.nRiskLevelID);
                    String^ szRiskLevel = "第"+nRiskIndicatorID.ToString()+"级";
                    ListViewItem^ item = gcnew ListViewItem(szRiskLevel,nCount);
                    item->Tag = riskPlan.WaringLevel.nRiskLevelID;

                    switch(riskPlan.OrgIDPlanRelation.nRiskIndicatorID)
                    {
                    case RI_MarketShock://市场冲击风险
                    case RI_AccountLoss://账户限亏
                    case RI_TodayLoss://今日限亏
                    case RI_GappedMarket://隔夜跳空
                    case RI_MarginRatio://保证金比例风险
                        item->SubItems->Add((riskPlan.WaringLevel.fThresholdValue*100).ToString("N2"));
                        break;
                    case RI_MaxPosition://合约最大持仓
                    case RI_SingleHoldTime://单笔持仓时间
                    case RI_FundNetValue://基金净值风险
                        item->SubItems->Add((riskPlan.WaringLevel.fThresholdValue).ToString());
                        break;
                    }

                    item->SubItems->Add(ProcessRiskAction(riskPlan.WaringLevel.nResponseType));
                    listView_RiskPlan->Items->Add( item );
                }
            }
    private: String^ ProcessRiskAction(int nActionValue)
             {
                 int RiskWarningTypeCount = 11;
                 String^ strActionText = String::Empty;
                 for (int i = 0; i < RiskWarningTypeCount; i++)
                 {
                     switch(i)
                     {
                     case 0:
                         if(nActionValue & WarningType_ForbidOpen_Single)
                         {
                             if (strActionText == String::Empty)                     
                                 strActionText = "限制开仓(单合约)";
                         }
                         break;
                     case 1:
                         if(nActionValue & WarningType_ForbidOpen)
                         {
                             if (strActionText == String::Empty)                     
                                 strActionText = "限制开仓";
                             else                     
                                 strActionText = strActionText + ";" + "限制开仓";

                         }
                         break;
                     case 2:
                         if(nActionValue & WarningType_ForbidOrder_Single)
                         {
                             if (strActionText == String::Empty)                     
                                 strActionText = "限制下单(单合约)";
                             else                     
                                 strActionText = strActionText + ";" + "限制下单(单合约)";

                         }
                         break;
                     case 3:
                         if(nActionValue & WarningType_ForbidOrder)
                         {
                             if (strActionText == String::Empty)                     
                                 strActionText = "限制下单";
                             else                     
                                 strActionText = strActionText + ";" + "限制下单";

                         }
                         break;
                     case 4:
                         if(nActionValue & WarningType_ForceClose_Single)
                         {
                             if (strActionText == String::Empty)                     
                                 strActionText = "市价强平(单合约)";
                             else                     
                                 strActionText = strActionText + ";" + "市价强平(单合约)";

                         }
                         break;
                     case 5:
                         if(nActionValue & WarningType_ForceClose)
                         {
                             if (strActionText == String::Empty)                     
                                 strActionText = "市价强平";
                             else                     
                                 strActionText = strActionText + ";" + "市价强平";

                         }
                         break;
                     case 6:
                         if(nActionValue & WarningType_Beep)
                         {
                             if (strActionText == String::Empty)                     
                                 strActionText = "鸣笛";
                             else                     
                                 strActionText = strActionText + ";" + "鸣笛";

                         }
                         break;
                     case 7:
                         if(nActionValue & WarningType_Vibrate)
                         {
                             if (strActionText == String::Empty)                     
                                 strActionText = "震动";
                             else                     
                                 strActionText = strActionText + ";" + "震动";

                         }
                         break;
                     case 8:
                         if(nActionValue & WarningType_Both)
                         {
                             if (strActionText == String::Empty)                     
                                 strActionText = "鸣笛加震动";
                             else                     
                                 strActionText = strActionText + ";" + "鸣笛加震动";

                         }
                         break;
                     case 9:
                         if(nActionValue & WarningType_ShowMsg)
                         {
                             if (strActionText == String::Empty)                     
                                 strActionText = "消息提示";
                             else                     
                                 strActionText = strActionText + ";" + "消息提示";

                         }
                         break;
                     case 10:
                         if(nActionValue & WarningType_ForceClose_SingleOpen)
                         {
                             if (strActionText == String::Empty)                     
                                 strActionText = "市价强平(单合约单笔)";
                             else                     
                                 strActionText = strActionText + ";" + "市价强平(单合约单笔)";

                         }
                         break;
                     }                 
                 }

                 return strActionText;
             }
    private: int GetActionTextValue(String^ szActionText)
             {    
                 int nRiskWaringType = 0;
                 int nActionTextValue = 0;
                 array<System::String^>^ sArray = szActionText->Split(';');

                 String^ szAction = String::Empty;
                 for each (String^ item in sArray)  
                 {
                     szAction = item->ToString()->Trim();
                     if (0==szAction->CompareTo("限制开仓(单合约)"))
                         nRiskWaringType = WarningType_ForbidOpen_Single;
                     else if(0==szAction->CompareTo("限制开仓"))
                         nRiskWaringType = WarningType_ForbidOpen;
                     else if(0==szAction->CompareTo("限制下单(单合约)"))
                         nRiskWaringType = WarningType_ForbidOrder_Single;
                     else if(0==szAction->CompareTo("限制下单"))
                         nRiskWaringType = WarningType_ForbidOrder;
                     else if(0==szAction->CompareTo("市价强平(单合约)"))
                         nRiskWaringType = WarningType_ForceClose_Single;
                     else if(0==szAction->CompareTo("市价强平"))
                         nRiskWaringType = WarningType_ForceClose;
                     else if(0==szAction->CompareTo("鸣笛"))
                         nRiskWaringType = WarningType_Beep;
                     else if(0==szAction->CompareTo("震动"))
                         nRiskWaringType = WarningType_Vibrate;
                     else if(0==szAction->CompareTo("鸣笛加震动"))
                         nRiskWaringType = WarningType_Both;
                     else if(0==szAction->CompareTo("消息提示"))
                         nRiskWaringType = WarningType_ShowMsg;
                     else if(0==szAction->CompareTo("市价强平(单合约单笔)"))
                         nRiskWaringType = WarningType_ForceClose_SingleOpen;

                     nActionTextValue |= nRiskWaringType;
                 }

                 return nActionTextValue;
             }
private: void LoadOrgToComboBox(vector<int> vecOrgID)
         {
             comboBox_Org->Items->Clear();

             int i=0;
             int nOrgID = -1;
             Organization assertOrg;
             for (vector<int>::iterator it = vecOrgID.begin();it!=vecOrgID.end();i++,it++)
             {
                 nOrgID = *it;                 
                 if(!CRiskData::GetInstance()->GetOrganizationByID(nOrgID, assertOrg))
                     continue;

                 comboBox_Org->Items->Add(Tools::string2String(assertOrg.szName));
                 if((*m_pType == Oper_Modify) && m_szOrgText->CompareTo(Tools::string2String(assertOrg.szName)) == 0)
                     comboBox_Org->SelectedIndex = i; 
             }
         }
             //////////////////////////////////////////////////////////////////////////
    private: System::Void FormRiskPlanSetup_Add_Load(System::Object^  sender, System::EventArgs^  e) {
                 this->CenterToParent();

//                  m_FormOrganizeCheckedTreeList = gcnew FormOrganizeCheckedTreeList();             
//                  m_FormOrganizeCheckedTreeList->ResetRelateCombo(anywndComboBoxControl_Org);
//                  anywndComboBoxControl_Org->ChildControl = m_FormOrganizeCheckedTreeList; 
//                  ResetComboBoxTextReadOnly(anywndComboBoxControl_Org);

                 m_FormInstrumentIDTreeList = gcnew FormInstrumentIDTreeList();
                 m_FormInstrumentIDTreeList->ResetRelateCombo(anywndComboBoxControl_Product);
                 anywndComboBoxControl_Product->ChildControl = m_FormInstrumentIDTreeList;
                 ResetComboBoxTextReadOnly(anywndComboBoxControl_Product);
                 anywndComboBoxControl_Product->Enabled = false;

                 sLoginRsp loginRsp;
                 memset(&loginRsp, 0, sizeof(loginRsp));
                 CRiskData::GetInstance()->GetLogonInfo(loginRsp);

                 vector<int> vecOrgID;
                 CRiskData::GetInstance()->GetOrgIDByUserID(loginRsp.mnUserID,USER_ORG_RELATION_MONITOR,vecOrgID);
                 if(!vecOrgID.empty())
                     LoadOrgToComboBox(vecOrgID);  


                 //阀值单位设置
                 int nOrgID;
                 RiskIndicatorType nRiskIndicatorID;
                 if(!CRiskData::GetInstance()->GetOrganizationIDByName(Tools::String2string(comboBox_Org->Text),nOrgID))
                     return;
                 if(!CRiskData::GetInstance()->GetIndicatorIDByName(Tools::String2string(comboBox_RiskIndicator->Text),nRiskIndicatorID))
                     return;

                 RiskPlan riskPlan;
                 vector<RiskPlan> vecRiskPlan;
                 Key_RiskPlan key(nOrgID,nRiskIndicatorID);
                 if(CRiskData::GetInstance()->GetKey2RiskPlan(key,vecRiskPlan))
                 {                 
                    riskPlan = vecRiskPlan[0];
                    m_bUse = riskPlan.OrgIDPlanRelation.bUse;
                 }

                 switch(nRiskIndicatorID)
                 {
                 case RI_MaxPosition://合约最大持仓
                     listView_RiskPlan->Columns[1]->Text = "阀值(手数)";
                     break;
                 case RI_MarketShock://市场冲击风险
                 case RI_AccountLoss://账户限亏
                 case RI_TodayLoss://今日限亏
                 case RI_GappedMarket://隔夜跳空
                 case RI_MarginRatio://保证金比例风险
                     listView_RiskPlan->Columns[1]->Text = "阀值(%)";
                     break;
                 case RI_SingleHoldTime://单笔持仓时间
                     listView_RiskPlan->Columns[1]->Text = "阀值(秒数)";
                     break;
                 case RI_FundNetValue://基金净值风险
                     break;
                 }                 

                 if ( *m_pType == Oper_Add )
                 {
                     this->Text = "新增风险方案";

                     radioButton_AllProduct->Checked = true;             
                     LoadRiskIndicator();
                 }
                 else 
                 {
                     this->Text = "修改风险方案";

                     LoadData();
                 }
             }
    private: System::Void button_AddRiskLevel_Click(System::Object^  sender, System::EventArgs^  e) {
                 int nOrgID;
                 RiskIndicatorType nRiskIndicatorID;
                 if(!CRiskData::GetInstance()->GetOrganizationIDByName(Tools::String2string(comboBox_Org->Text),nOrgID))
                 {
                     MessageBox::Show("请选择组织机构");
                     return;
                 }
                 if(!CRiskData::GetInstance()->GetIndicatorIDByName(Tools::String2string(comboBox_RiskIndicator->Text),nRiskIndicatorID))
                 {
                     MessageBox::Show("请选择风险方案");
                     return;
                 }

                 int nLevel = 0;
                 double dLevelValue = 0.0;

                 g_mapRiskLevelInfo2.clear();
                 for (int i=0;i<listView_RiskPlan->Items->Count;i++)
                 {
                     nLevel = i+1;
                     dLevelValue = (float)::Convert::ToDouble(listView_RiskPlan->Items[i]->SubItems[1]->Text);
                     g_mapRiskLevelInfo2.insert(make_pair(nLevel,dLevelValue));
                 } 

                 int nCount = listView_RiskPlan->Items->Count + 1;
                 String^ szRiskLevel = "第"+nCount.ToString()+"级";

                 float fLevelValue;
                 int nLevelCount = listView_RiskPlan->Items->Count;
                 if(nLevelCount>0)
                 {
                     String^ szValue = listView_RiskPlan->Items[nLevelCount-1]->SubItems[1]->Text;
                     fLevelValue = (float)Convert::ToDouble(szValue);
                 }

                 FormRiskPlanSetup_Add_RiskLevel^ dlg = gcnew FormRiskPlanSetup_Add_RiskLevel(nOrgID,nRiskIndicatorID);
                 dlg->SetDialogTitle("新增风险等级");
                 dlg->SetRiskLevelNumber(nCount);
                 dlg->SetRiskValue(fLevelValue);
                 dlg->SetEmpty(nLevelCount==0);
                 dlg->SetOperType(Oper_Add);
                 dlg->SetRiskLevelText(szRiskLevel);
                 if (::DialogResult::OK == dlg->ShowDialog())
                 {
                     m_nCurActionValue = dlg->GetActionValue();

                     ListViewItem^ item = gcnew ListViewItem( szRiskLevel,listView_RiskPlan->Items->Count);
                     item->Tag = -1;
                     item->SubItems->Add( dlg->GetLevelValueText());
                     item->SubItems->Add( dlg->GetActionText());

                     listView_RiskPlan->Items->Add( item );
                 }
             }
    private: System::Void button_Modify_Click(System::Object^  sender, System::EventArgs^  e) {
                 if(listView_RiskPlan->SelectedIndices != nullptr && listView_RiskPlan->SelectedIndices->Count>0)
                 {
                     String^ szRiskLevelText = listView_RiskPlan->SelectedItems[0]->SubItems[0]->Text;
                     int nLen = szRiskLevelText->Length;
                     String^ szItem = szRiskLevelText->Substring(1,1);
                     int nCount = Convert::ToInt32(szItem);

                     int nOrgID;
                     RiskIndicatorType nRiskIndicatorID;
                     if(!CRiskData::GetInstance()->GetOrganizationIDByName(Tools::String2string(comboBox_Org->Text),nOrgID))
                     {
                         MessageBox::Show("请选择组织机构");
                         return;
                     }
                     if(!CRiskData::GetInstance()->GetIndicatorIDByName(Tools::String2string(comboBox_RiskIndicator->Text),nRiskIndicatorID))
                     {
                         MessageBox::Show("请选择风险方案");
                         return;
                     }

                     int nLevel = 0;
                     double dLevelValue = 0.0;
                     
                     g_mapRiskLevelInfo2.clear();
                     for (int i=0;i<listView_RiskPlan->Items->Count;i++)
                     {
                         nLevel = i+1;
                         dLevelValue = (float)::Convert::ToDouble(listView_RiskPlan->Items[i]->SubItems[1]->Text);
                         g_mapRiskLevelInfo2.insert(make_pair(nLevel,dLevelValue));
                     } 

                     FormRiskPlanSetup_Add_RiskLevel^ dlg = gcnew FormRiskPlanSetup_Add_RiskLevel(nOrgID,nRiskIndicatorID);
                     dlg->SetDialogTitle("修改风险等级");
                     dlg->SetRiskLevelNumber(nCount);
                     dlg->SetOperType(Oper_Modify);
                     dlg->SetRiskLevelText(listView_RiskPlan->SelectedItems[0]->SubItems[0]->Text);
                     dlg->SetLevelValueText(listView_RiskPlan->SelectedItems[0]->SubItems[1]->Text);
                     dlg->SetActionText(listView_RiskPlan->SelectedItems[0]->SubItems[2]->Text);
                     if (::DialogResult::OK == dlg->ShowDialog(this))
                     {
                         m_nCurActionValue = dlg->GetActionValue();

                         listView_RiskPlan->SelectedItems[0]->SubItems[1]->Text = dlg->GetLevelValueText();
                         listView_RiskPlan->SelectedItems[0]->SubItems[2]->Text = dlg->GetActionText();
                     }
                 }
                 else
                     MessageBox::Show("请选择内容");
             }
    private: System::Void button_DelRiskLevel_Click(System::Object^  sender, System::EventArgs^  e) {
                 if(listView_RiskPlan->SelectedIndices != nullptr && listView_RiskPlan->SelectedIndices->Count>0)
                 {
                     if(::DialogResult::OK == MessageBox::Show(this,"是否确认删除此风险等级？", "提示", MessageBoxButtons::OKCancel))
                     {                         
                         ListView::SelectedIndexCollection^ c = listView_RiskPlan->SelectedIndices;
                         if(c != nullptr)
                             listView_RiskPlan->Items->RemoveAt(c[0]);

                         //删除后更新一下
                         int nCount = listView_RiskPlan->Items->Count;
                         for (int i=0;i<nCount;i++)
                         {
                             String^ szRiskLevel = "第"+(i+1).ToString()+"级";
                             listView_RiskPlan->Items[i]->SubItems[0]->Text = szRiskLevel;
                         }
                     }
                 }
                 else
                     MessageBox::Show("请选择内容");
             }
    private: System::Void button_OK_Click(System::Object^  sender, System::EventArgs^  e) {
                 int nOrgID;
                 RiskIndicatorType nRiskIndicatorID;
                 if(!CRiskData::GetInstance()->GetOrganizationIDByName(Tools::String2string(comboBox_Org->Text),nOrgID))
                 {
                     MessageBox::Show("请选择组织机构");
                     return;
                 }
                 if(!CRiskData::GetInstance()->GetIndicatorIDByName(Tools::String2string(comboBox_RiskIndicator->Text),nRiskIndicatorID))
                 {
                     MessageBox::Show("请选择风险方案");
                     return;
                 }

                 Key_RiskPlan key;
                 key.first = nOrgID;
                 key.second = nRiskIndicatorID;

                 vector<RiskPlan> vecRiskPlan;

                 if(CRiskData::GetInstance()->GetKey2RiskPlan(key,vecRiskPlan))
                     m_pNetFundParam = &(vecRiskPlan[0].netFundParam);

                 if(listView_RiskPlan->Items->Count == 0)
                 {
                     if(::DialogResult::OK == MessageBox::Show(this,"如果当前方案中没有风险等级，则删除此风控方案，是否确定？", "提示", MessageBoxButtons::OKCancel))
                     {
                         g_riskPlanKey = key;
                         g_riskPlanOrg = Tools::String2string(comboBox_Org->Text);
                         g_vecRiskPlan = vecRiskPlan;

                         CTcpLayer::SendData(Cmd_RM_DeleteRiskPlan_Req, &key, sizeof(RiskChange), 0, m_pLoginInfo->mnUserID); 
                     }

                     return;
                 }


                 RiskPlan riskPlan;

                 RiskOrgIDPlanRelation riskOrgIDPlan;
                 riskOrgIDPlan.nOrgID = nOrgID;
                 riskOrgIDPlan.nRiskIndicatorID = nRiskIndicatorID;
                 riskOrgIDPlan.bAllProduct = radioButton_AllProduct->Checked;
                 strcpy(riskOrgIDPlan.ProductID, Tools::String2string(anywndComboBoxControl_Product->Text).c_str());
                 riskOrgIDPlan.bUse = m_bUse;

                 vecRiskPlan.clear();

                 float fValue = 0.00f;
                 RiskWaring riskWaring;
                 riskWaring.nOrgID = nOrgID;
                 riskWaring.nRiskIndicatorID = nRiskIndicatorID;
                 for (int i=0;i<listView_RiskPlan->Items->Count;i++)
                 {
                     riskWaring.nRiskLevelID = i+1;
                     switch(nRiskIndicatorID)
                     {
                     case RI_MarketShock://市场冲击风险
                     case RI_AccountLoss://账户限亏
                     case RI_TodayLoss://今日限亏
                     case RI_GappedMarket://隔夜跳空
                     case RI_MarginRatio://保证金比例风险
                         fValue = ((float)::Convert::ToDouble(listView_RiskPlan->Items[i]->SubItems[1]->Text))/100.0f;
                         break;
                     case RI_MaxPosition://合约最大持仓
                     case RI_SingleHoldTime://单笔持仓时间
                     case RI_FundNetValue://基金净值风险
                         fValue = (float)::Convert::ToDouble(listView_RiskPlan->Items[i]->SubItems[1]->Text);
                         break;
                     } 
                     riskWaring.fThresholdValue = fValue;
                     riskWaring.nResponseType = GetActionTextValue(listView_RiskPlan->Items[i]->SubItems[2]->Text);
                     riskWaring.nColor = ColorTranslator::ToWin32(Color::FromArgb(0, 0, 255));//Color::Black


                     riskPlan.OrgIDPlanRelation = riskOrgIDPlan;
                     riskPlan.WaringLevel = riskWaring;
                     if(nRiskIndicatorID == RI_FundNetValue)    riskPlan.netFundParam = *m_pNetFundParam;
                     vecRiskPlan.push_back(riskPlan);
                 } 

                 g_riskPlanKey = key;
                 g_riskPlanOrg = Tools::String2string(comboBox_Org->Text);
                 g_vecRiskPlan = vecRiskPlan;


                 int nDataLen = vecRiskPlan.size()*sizeof(RiskPlan);
                 char* pBuffer = new char[nDataLen];
                 memset(pBuffer, 0, nDataLen);
                 if ( !vecRiskPlan.empty() )
                     memcpy(pBuffer, &vecRiskPlan[0], nDataLen);

                 CTcpLayer::SendData(Cmd_RM_AddRiskPlan_Req, pBuffer, nDataLen, 0, m_pLoginInfo->mnUserID); 

                 delete [] pBuffer;

             m_bOkClicked = true;
             }

private: System::Void comboBox_Org_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             listView_RiskPlan->Items->Clear();

             int nOrgID;
             RiskIndicatorType nRiskIndicatorID;
             if(!CRiskData::GetInstance()->GetOrganizationIDByName(Tools::String2string(comboBox_Org->Text),nOrgID))
                 return;
             if(!CRiskData::GetInstance()->GetIndicatorIDByName(Tools::String2string(comboBox_RiskIndicator->Text),nRiskIndicatorID))
                 return;

             button_FundPerValue->Visible = false;
             switch(nRiskIndicatorID)
             {
             case RI_MaxPosition:
             case RI_MarketShock:
             case RI_SingleHoldTime:
                 if(!radioButton_AllProduct->Checked)
                 {
                     radioButton_SingleProduct->Enabled = true;
                     anywndComboBoxControl_Product->Enabled = true;
                 }
                 break;
             case RI_AccountLoss:
             case RI_TodayLoss:
             case RI_GappedMarket:
             case RI_MarginRatio:
                 radioButton_SingleProduct->Enabled = false;
                 anywndComboBoxControl_Product->Enabled = false;
                 break;
             case RI_FundNetValue:
                 button_FundPerValue->Visible = true;
                 radioButton_SingleProduct->Enabled = false;
                 anywndComboBoxControl_Product->Enabled = false;
                 break;
             }  

             switch(nRiskIndicatorID)
             {
             case RI_MaxPosition://合约最大持仓
                 listView_RiskPlan->Columns[1]->Text = "阀值(手数)";
                 break;
             case RI_MarketShock://市场冲击风险
             case RI_AccountLoss://账户限亏
             case RI_TodayLoss://今日限亏
             case RI_GappedMarket://隔夜跳空
             case RI_MarginRatio://保证金比例风险
                 listView_RiskPlan->Columns[1]->Text = "阀值(%)";
                 break;             
             case RI_SingleHoldTime://单笔持仓时间
                 listView_RiskPlan->Columns[1]->Text = "阀值(秒数)";
                 break;     
             case RI_FundNetValue://基金净值风险
                 break;
             } 


             vector<RiskPlan> vecRiskPlan;
             Key_RiskPlan key(nOrgID,nRiskIndicatorID);
             if(!CRiskData::GetInstance()->GetKey2RiskPlan(key,vecRiskPlan))
             {                 
                 radioButton_AllProduct->Checked = true;
                 radioButton_SingleProduct->Checked = false;
                 anywndComboBoxControl_Product->Text = String::Empty;
                 return;
             }

             RiskPlan riskPlan = vecRiskPlan[0];

             if(riskPlan.OrgIDPlanRelation.bAllProduct)
             {
                 radioButton_AllProduct->Checked = true;
                 radioButton_SingleProduct->Checked = false;
                 anywndComboBoxControl_Product->Text = String::Empty;
             }
             else
             {
                 radioButton_AllProduct->Checked = false;
                 radioButton_SingleProduct->Checked = true;
                 anywndComboBoxControl_Product->Text = Tools::string2String(riskPlan.OrgIDPlanRelation.ProductID);
             }

             vector<RiskPlan>::iterator it = vecRiskPlan.begin();
             for (;it!=vecRiskPlan.end();it++)
             {
                 RiskPlan riskPlan = *it;

                 int nCount = listView_RiskPlan->Items->Count;
                 int nRiskIndicatorID = (int)(riskPlan.WaringLevel.nRiskLevelID);
                 String^ szRiskLevel = "第"+nRiskIndicatorID.ToString()+"级";
                 ListViewItem^ item = gcnew ListViewItem(szRiskLevel,nCount);
                 item->Tag = riskPlan.WaringLevel.nRiskLevelID;
                 item->SubItems->Add(riskPlan.WaringLevel.fThresholdValue.ToString());
                 item->SubItems->Add(ProcessRiskAction(riskPlan.WaringLevel.nResponseType));
                 listView_RiskPlan->Items->Add( item );
             }
         }
    private: System::Void comboBox_RiskIndicator_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
                 listView_RiskPlan->Items->Clear();

                 int nOrgID;
                 RiskIndicatorType nRiskIndicatorID;
                 if(!CRiskData::GetInstance()->GetOrganizationIDByName(Tools::String2string(comboBox_Org->Text),nOrgID))
                     return;
                 if(!CRiskData::GetInstance()->GetIndicatorIDByName(Tools::String2string(comboBox_RiskIndicator->Text),nRiskIndicatorID))
                     return;

                 button_FundPerValue->Visible = false;
                 switch(nRiskIndicatorID)
                 {
                 case RI_MaxPosition:
                 case RI_MarketShock:
                 case RI_SingleHoldTime:
                     radioButton_SingleProduct->Enabled = true;
                     anywndComboBoxControl_Product->Enabled = false;
                     break;
                 case RI_AccountLoss:
                 case RI_TodayLoss:
                 case RI_GappedMarket:
                 case RI_MarginRatio:
                     radioButton_SingleProduct->Enabled = false;
                     anywndComboBoxControl_Product->Enabled = false;
                     break;
                 case RI_FundNetValue:
                     button_FundPerValue->Visible = true;
                     radioButton_SingleProduct->Enabled = false;
                     anywndComboBoxControl_Product->Enabled = false;
                     break;
                 }  

                 switch(nRiskIndicatorID)
                 {
                 case RI_MaxPosition://合约最大持仓
                     listView_RiskPlan->Columns[1]->Text = "阀值(手数)";
                     break;
                 case RI_MarketShock://市场冲击风险
                 case RI_AccountLoss://账户限亏
                 case RI_TodayLoss://今日限亏
                 case RI_GappedMarket://隔夜跳空
                 case RI_MarginRatio://保证金比例风险
                     listView_RiskPlan->Columns[1]->Text = "阀值(%)";
                     break;             
                 case RI_SingleHoldTime://单笔持仓时间
                     listView_RiskPlan->Columns[1]->Text = "阀值(秒数)";
                     break;     
                 case RI_FundNetValue://基金净值风险
                     listView_RiskPlan->Columns[1]->Text = "阀值";
                     break;
                 } 

                 vector<RiskPlan> vecRiskPlan;
                 Key_RiskPlan key(nOrgID,nRiskIndicatorID);
                 if(!CRiskData::GetInstance()->GetKey2RiskPlan(key,vecRiskPlan))
                 {                 
                     radioButton_AllProduct->Checked = true;
                     radioButton_SingleProduct->Checked = false;
                     anywndComboBoxControl_Product->Text = String::Empty;
                     return;
                 }

                 RiskPlan riskPlan = vecRiskPlan[0];

                 if(riskPlan.OrgIDPlanRelation.bAllProduct)
                 {
                     radioButton_AllProduct->Checked = true;
                     radioButton_SingleProduct->Checked = false;
                     anywndComboBoxControl_Product->Text = String::Empty;
                 }
                 else
                 {
                     radioButton_AllProduct->Checked = false;
                     radioButton_SingleProduct->Checked = true;
                     anywndComboBoxControl_Product->Text = Tools::string2String(riskPlan.OrgIDPlanRelation.ProductID);
                 }

                 vector<RiskPlan>::iterator it = vecRiskPlan.begin();
                 for (;it!=vecRiskPlan.end();it++)
                 {
                     RiskPlan riskPlan = *it;

                     int nCount = listView_RiskPlan->Items->Count;
                     int nRiskIndicatorID = (int)(riskPlan.WaringLevel.nRiskLevelID);
                     String^ szRiskLevel = "第"+nRiskIndicatorID.ToString()+"级";
                     ListViewItem^ item = gcnew ListViewItem(szRiskLevel,nCount);
                     item->Tag = riskPlan.WaringLevel.nRiskLevelID;
                     item->SubItems->Add(riskPlan.WaringLevel.fThresholdValue.ToString());
                     item->SubItems->Add(ProcessRiskAction(riskPlan.WaringLevel.nResponseType));
                     listView_RiskPlan->Items->Add( item );
                 }
             }
    private: System::Void button_FundPerValue_Click(System::Object^  sender, System::EventArgs^  e) {
                 int nOrgID;
                 RiskIndicatorType nRiskIndicatorID;
             if(!CRiskData::GetInstance()->GetOrganizationIDByName(Tools::String2string(comboBox_Org->Text),nOrgID))
                     return;
                 if(!CRiskData::GetInstance()->GetIndicatorIDByName(Tools::String2string(comboBox_RiskIndicator->Text),nRiskIndicatorID))
                     return;

                 Key_RiskPlan key;
                 key.first = nOrgID;
                 key.second = nRiskIndicatorID;

                 vector<RiskPlan> vecRiskPlan;             

                 FormFundPerValue^ dlg = gcnew FormFundPerValue;
                 if(CRiskData::GetInstance()->GetKey2RiskPlan(key,vecRiskPlan))
                 {
                     dlg->SetNetFundParam(&(vecRiskPlan[0].netFundParam));

                     if (::DialogResult::OK == dlg->ShowDialog())
                     {
                         dlg->GetNetFundParam(*m_pNetFundParam);
                         CRiskData::GetInstance()->SetNetFundParam(key,m_pNetFundParam);
                     }
                 }
                 else
                 {
                     dlg->SetNetFundParam(m_pNetFundParam);

                     if (::DialogResult::OK == dlg->ShowDialog())
                     {
                         dlg->GetNetFundParam(*m_pNetFundParam);
                     }
                 }
             }
private: System::Void FormRiskPlanSetup_Add_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
             if(!m_bOkClicked && ::DialogResult::Cancel ==  MessageBox::Show("是否关闭。确认吗？\n确认:窗口退出,不保存\n取消:窗口不退出", "提示", MessageBoxButtons::OKCancel, MessageBoxIcon::Warning))
                 e->Cancel = true;
         }
private: System::Void radioButton_AllProduct_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
             anywndComboBoxControl_Product->Enabled = false;
             m_FormInstrumentIDTreeList->Clear();
         }
private: System::Void radioButton_SingleProduct_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
             anywndComboBoxControl_Product->Enabled = true;
         }
    };
}
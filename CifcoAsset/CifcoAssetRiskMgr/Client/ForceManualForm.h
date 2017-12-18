#pragma once

#include "ForceManualForm2.h"
#include "ForceShortcutForm.h"
#include "RiskManageCmd.h"
#include "RiskManageStruct.h"
#include "CommonPkg.h"
#include "TcpLayer.h"
#include "md5.h"
#include "CDataInfo.h"
#include "ForceCloseInfoData.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Client {

	/// <summary>
	/// Summary for ForceManualForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ForceManualForm : public System::Windows::Forms::Form
	{
	public:
		ForceManualForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_nCurRowIndex = -1;
            m_pAllFundInfo = NULL;
            m_pDataInfo = CDataInfo::NewInstance();
            m_pForceCloseInfoData = CForceCloseInfoData::GetInstance();   
            m_pForceManualForm2 = ForceManualForm2::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ForceManualForm()
		{
			if (components)
			{
				delete components;
			}

            if (m_pDataInfo != NULL)
            {
                CDataInfo::DestroyInstance();
            }

            if (m_pForceCloseInfoData)
            {
                CForceCloseInfoData::DestroyInstance();
            }
            if(m_pForceManualForm2 != NULL)
                ForceManualForm2::DestroyInstance(m_pForceManualForm2);
		}
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	protected: 
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::SplitContainer^  splitContainer3;
    private: System::Windows::Forms::Button^  CalculateTest_button;
	private: System::Windows::Forms::GroupBox^  groupBox1;
    private: System::Windows::Forms::Label^  DynamicInterest_label;
	private: System::Windows::Forms::Label^  label4;
    private: System::Windows::Forms::Label^  FreezeFund_label;
	private: System::Windows::Forms::Label^  label7;
    private: System::Windows::Forms::Label^  EnsureFund_label;
	private: System::Windows::Forms::Label^  label6;
    private: System::Windows::Forms::Label^  IndicatorLevel_label;
	private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Label^  DiscrepancyFund_label;
	private: System::Windows::Forms::Label^  label5;
    private: System::Windows::Forms::Label^  Name_label;
    private: System::Windows::Forms::Label^  Account_label;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
    private: System::Windows::Forms::DataGridView^  TodayHold_dataGridView;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::SplitContainer^  splitContainer4;
    private: System::Windows::Forms::DataGridView^  ForceTable_dataGridView;
    private: System::Windows::Forms::Button^  Order_button;
    private: System::Windows::Forms::Label^  AllEnsureFund_label;
	private: System::Windows::Forms::GroupBox^  groupBox3;
    private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label12;
    private: System::Windows::Forms::ComboBox^  PriceType_comboBox;
    private: System::Windows::Forms::TextBox^  CommissPrice_textBox;
    private: System::Windows::Forms::TextBox^  Spread_textBox;
    private: System::Windows::Forms::TextBox^  ForceCount_textBox;
    private: System::Windows::Forms::ComboBox^  ForceDirect_comboBox;
    private: System::Windows::Forms::TextBox^  Contract_textBox;
    private: System::Windows::Forms::Button^  Cancel_button;
    private: System::Windows::Forms::Button^  Save_button;
    private: System::Windows::Forms::CheckBox^  CommissPrice_checkBox;
    private: System::Windows::Forms::CheckBox^  Spread_checkBox;
    private: System::Windows::Forms::Label^  label15;
    private: System::Windows::Forms::Label^  label14;
    private: System::Windows::Forms::SplitContainer^  splitContainer5;
    private: System::Windows::Forms::Button^  Quit_button;
    private: System::Windows::Forms::Panel^  panel1;
    private: System::Windows::Forms::GroupBox^  groupBox4;
    private: System::Windows::Forms::DataGridView^  OrderTable_dataGridView;
    private: System::Windows::Forms::SplitContainer^  splitContainer6;
    private: System::Windows::Forms::ContextMenuStrip^  TodayHoldDataGrid_contextMenuStrip;
    private: System::Windows::Forms::ToolStripMenuItem^  ForceShortcut_ToolStripMenuItem;
    private: System::ComponentModel::IContainer^  components;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column4;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column5;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column6;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column7;
    private: System::Windows::Forms::DataGridViewCheckBoxColumn^  Column35;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column8;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column9;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column10;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column11;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column12;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column13;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column14;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column15;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column16;
    

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
        CDataInfo* m_pDataInfo;
        CForceCloseInfoData* m_pForceCloseInfoData; 
        ForceManualForm2* m_pForceManualForm2;
        DepthMarketDataField* m_pDepthMarketDataField;//深度行情
        RiskAllAccountField* m_pAllFundInfo;//资金

        String^ m_strAccountID;
        String^ m_strIndicatorLevel;
        String^ m_strInstrumentID;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column17;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column18;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column19;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column20;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column21;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column22;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column23;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column24;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column27;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column28;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column29;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column30;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column31;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column32;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column33;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column34;

















         int m_nCurRowIndex;

public:void SetTradeAccount(int TradeAccount)
       {
           m_strAccountID = Convert::ToString(TradeAccount);
       }
public:void SetIndicatorLevel(int IndicatorLevel)
      {
          m_strIndicatorLevel = Convert::ToString(IndicatorLevel);
      }
public:void SetInstrument(int InstrumentID)
      {
          m_strInstrumentID = Convert::ToString(InstrumentID);
      }
       

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
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle3 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle4 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle5 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle6 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->DynamicInterest_label = (gcnew System::Windows::Forms::Label());
            this->label4 = (gcnew System::Windows::Forms::Label());
            this->FreezeFund_label = (gcnew System::Windows::Forms::Label());
            this->label7 = (gcnew System::Windows::Forms::Label());
            this->EnsureFund_label = (gcnew System::Windows::Forms::Label());
            this->label6 = (gcnew System::Windows::Forms::Label());
            this->IndicatorLevel_label = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->DiscrepancyFund_label = (gcnew System::Windows::Forms::Label());
            this->label5 = (gcnew System::Windows::Forms::Label());
            this->Name_label = (gcnew System::Windows::Forms::Label());
            this->Account_label = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->splitContainer6 = (gcnew System::Windows::Forms::SplitContainer());
            this->panel1 = (gcnew System::Windows::Forms::Panel());
            this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
            this->splitContainer3 = (gcnew System::Windows::Forms::SplitContainer());
            this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
            this->TodayHold_dataGridView = (gcnew System::Windows::Forms::DataGridView());
            this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column4 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column5 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column6 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column7 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->splitContainer5 = (gcnew System::Windows::Forms::SplitContainer());
            this->splitContainer4 = (gcnew System::Windows::Forms::SplitContainer());
            this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
            this->ForceTable_dataGridView = (gcnew System::Windows::Forms::DataGridView());
            this->Column35 = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
            this->Column8 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column9 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column10 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column11 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column12 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column13 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column14 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column15 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column16 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->ForceDirect_comboBox = (gcnew System::Windows::Forms::ComboBox());
            this->PriceType_comboBox = (gcnew System::Windows::Forms::ComboBox());
            this->CommissPrice_textBox = (gcnew System::Windows::Forms::TextBox());
            this->Spread_textBox = (gcnew System::Windows::Forms::TextBox());
            this->ForceCount_textBox = (gcnew System::Windows::Forms::TextBox());
            this->Contract_textBox = (gcnew System::Windows::Forms::TextBox());
            this->Cancel_button = (gcnew System::Windows::Forms::Button());
            this->Save_button = (gcnew System::Windows::Forms::Button());
            this->CommissPrice_checkBox = (gcnew System::Windows::Forms::CheckBox());
            this->Spread_checkBox = (gcnew System::Windows::Forms::CheckBox());
            this->label15 = (gcnew System::Windows::Forms::Label());
            this->label14 = (gcnew System::Windows::Forms::Label());
            this->label13 = (gcnew System::Windows::Forms::Label());
            this->label12 = (gcnew System::Windows::Forms::Label());
            this->AllEnsureFund_label = (gcnew System::Windows::Forms::Label());
            this->Order_button = (gcnew System::Windows::Forms::Button());
            this->CalculateTest_button = (gcnew System::Windows::Forms::Button());
            this->label8 = (gcnew System::Windows::Forms::Label());
            this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
            this->OrderTable_dataGridView = (gcnew System::Windows::Forms::DataGridView());
            this->Quit_button = (gcnew System::Windows::Forms::Button());
            this->TodayHoldDataGrid_contextMenuStrip = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
            this->ForceShortcut_ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->Column17 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column18 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column19 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column20 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column21 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column22 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column23 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column24 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column27 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column28 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column29 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column30 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column31 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column32 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column33 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column34 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->splitContainer1->Panel1->SuspendLayout();
            this->splitContainer1->Panel2->SuspendLayout();
            this->splitContainer1->SuspendLayout();
            this->groupBox1->SuspendLayout();
            this->splitContainer6->Panel1->SuspendLayout();
            this->splitContainer6->Panel2->SuspendLayout();
            this->splitContainer6->SuspendLayout();
            this->panel1->SuspendLayout();
            this->splitContainer2->Panel1->SuspendLayout();
            this->splitContainer2->Panel2->SuspendLayout();
            this->splitContainer2->SuspendLayout();
            this->splitContainer3->Panel1->SuspendLayout();
            this->splitContainer3->Panel2->SuspendLayout();
            this->splitContainer3->SuspendLayout();
            this->groupBox2->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TodayHold_dataGridView))->BeginInit();
            this->splitContainer5->Panel1->SuspendLayout();
            this->splitContainer5->Panel2->SuspendLayout();
            this->splitContainer5->SuspendLayout();
            this->splitContainer4->Panel1->SuspendLayout();
            this->splitContainer4->Panel2->SuspendLayout();
            this->splitContainer4->SuspendLayout();
            this->groupBox3->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ForceTable_dataGridView))->BeginInit();
            this->groupBox4->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->OrderTable_dataGridView))->BeginInit();
            this->TodayHoldDataGrid_contextMenuStrip->SuspendLayout();
            this->SuspendLayout();
            // 
            // splitContainer1
            // 
            this->splitContainer1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
            this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer1->Location = System::Drawing::Point(0, 0);
            this->splitContainer1->Name = L"splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this->splitContainer1->Panel1->Controls->Add(this->groupBox1);
            // 
            // splitContainer1.Panel2
            // 
            this->splitContainer1->Panel2->Controls->Add(this->splitContainer6);
            this->splitContainer1->Size = System::Drawing::Size(1216, 603);
            this->splitContainer1->SplitterDistance = 150;
            this->splitContainer1->TabIndex = 0;
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->DynamicInterest_label);
            this->groupBox1->Controls->Add(this->label4);
            this->groupBox1->Controls->Add(this->FreezeFund_label);
            this->groupBox1->Controls->Add(this->label7);
            this->groupBox1->Controls->Add(this->EnsureFund_label);
            this->groupBox1->Controls->Add(this->label6);
            this->groupBox1->Controls->Add(this->IndicatorLevel_label);
            this->groupBox1->Controls->Add(this->label3);
            this->groupBox1->Controls->Add(this->DiscrepancyFund_label);
            this->groupBox1->Controls->Add(this->label5);
            this->groupBox1->Controls->Add(this->Name_label);
            this->groupBox1->Controls->Add(this->Account_label);
            this->groupBox1->Controls->Add(this->label2);
            this->groupBox1->Controls->Add(this->label1);
            this->groupBox1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->groupBox1->Location = System::Drawing::Point(0, 0);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(146, 599);
            this->groupBox1->TabIndex = 0;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"资金情况";
            // 
            // DynamicInterest_label
            // 
            this->DynamicInterest_label->AutoSize = true;
            this->DynamicInterest_label->Location = System::Drawing::Point(77, 115);
            this->DynamicInterest_label->Name = L"DynamicInterest_label";
            this->DynamicInterest_label->Size = System::Drawing::Size(0, 12);
            this->DynamicInterest_label->TabIndex = 0;
            // 
            // label4
            // 
            this->label4->AutoSize = true;
            this->label4->Location = System::Drawing::Point(6, 115);
            this->label4->Name = L"label4";
            this->label4->Size = System::Drawing::Size(65, 12);
            this->label4->TabIndex = 0;
            this->label4->Text = L"动态权益：";
            // 
            // FreezeFund_label
            // 
            this->FreezeFund_label->AutoSize = true;
            this->FreezeFund_label->Location = System::Drawing::Point(77, 205);
            this->FreezeFund_label->Name = L"FreezeFund_label";
            this->FreezeFund_label->Size = System::Drawing::Size(0, 12);
            this->FreezeFund_label->TabIndex = 0;
            // 
            // label7
            // 
            this->label7->AutoSize = true;
            this->label7->Location = System::Drawing::Point(6, 205);
            this->label7->Name = L"label7";
            this->label7->Size = System::Drawing::Size(65, 12);
            this->label7->TabIndex = 0;
            this->label7->Text = L"冻结资金：";
            // 
            // EnsureFund_label
            // 
            this->EnsureFund_label->AutoSize = true;
            this->EnsureFund_label->Location = System::Drawing::Point(77, 175);
            this->EnsureFund_label->Name = L"EnsureFund_label";
            this->EnsureFund_label->Size = System::Drawing::Size(0, 12);
            this->EnsureFund_label->TabIndex = 0;
            // 
            // label6
            // 
            this->label6->AutoSize = true;
            this->label6->Location = System::Drawing::Point(6, 175);
            this->label6->Name = L"label6";
            this->label6->Size = System::Drawing::Size(53, 12);
            this->label6->TabIndex = 0;
            this->label6->Text = L"保证金：";
            // 
            // IndicatorLevel_label
            // 
            this->IndicatorLevel_label->AutoSize = true;
            this->IndicatorLevel_label->Location = System::Drawing::Point(77, 85);
            this->IndicatorLevel_label->Name = L"IndicatorLevel_label";
            this->IndicatorLevel_label->Size = System::Drawing::Size(0, 12);
            this->IndicatorLevel_label->TabIndex = 0;
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(6, 85);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(65, 12);
            this->label3->TabIndex = 0;
            this->label3->Text = L"指标等级：";
            // 
            // DiscrepancyFund_label
            // 
            this->DiscrepancyFund_label->AutoSize = true;
            this->DiscrepancyFund_label->Location = System::Drawing::Point(77, 145);
            this->DiscrepancyFund_label->Name = L"DiscrepancyFund_label";
            this->DiscrepancyFund_label->Size = System::Drawing::Size(0, 12);
            this->DiscrepancyFund_label->TabIndex = 0;
            // 
            // label5
            // 
            this->label5->AutoSize = true;
            this->label5->Location = System::Drawing::Point(6, 145);
            this->label5->Name = L"label5";
            this->label5->Size = System::Drawing::Size(53, 12);
            this->label5->TabIndex = 0;
            this->label5->Text = L"出入金：";
            // 
            // Name_label
            // 
            this->Name_label->AutoSize = true;
            this->Name_label->Location = System::Drawing::Point(77, 55);
            this->Name_label->Name = L"Name_label";
            this->Name_label->Size = System::Drawing::Size(0, 12);
            this->Name_label->TabIndex = 0;
            // 
            // Account_label
            // 
            this->Account_label->AutoSize = true;
            this->Account_label->Location = System::Drawing::Point(77, 25);
            this->Account_label->Name = L"Account_label";
            this->Account_label->Size = System::Drawing::Size(0, 12);
            this->Account_label->TabIndex = 0;
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(6, 55);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(41, 12);
            this->label2->TabIndex = 0;
            this->label2->Text = L"姓名：";
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(6, 25);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(41, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"帐号：";
            // 
            // splitContainer6
            // 
            this->splitContainer6->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
            this->splitContainer6->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer6->Location = System::Drawing::Point(0, 0);
            this->splitContainer6->Name = L"splitContainer6";
            this->splitContainer6->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer6.Panel1
            // 
            this->splitContainer6->Panel1->Controls->Add(this->panel1);
            // 
            // splitContainer6.Panel2
            // 
            this->splitContainer6->Panel2->Controls->Add(this->Quit_button);
            this->splitContainer6->Size = System::Drawing::Size(1062, 603);
            this->splitContainer6->SplitterDistance = 567;
            this->splitContainer6->TabIndex = 0;
            // 
            // panel1
            // 
            this->panel1->Controls->Add(this->splitContainer2);
            this->panel1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->panel1->Location = System::Drawing::Point(0, 0);
            this->panel1->Name = L"panel1";
            this->panel1->Size = System::Drawing::Size(1058, 563);
            this->panel1->TabIndex = 0;
            // 
            // splitContainer2
            // 
            this->splitContainer2->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
            this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer2->Location = System::Drawing::Point(0, 0);
            this->splitContainer2->Name = L"splitContainer2";
            this->splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this->splitContainer2->Panel1->Controls->Add(this->splitContainer3);
            // 
            // splitContainer2.Panel2
            // 
            this->splitContainer2->Panel2->Controls->Add(this->groupBox4);
            this->splitContainer2->Size = System::Drawing::Size(1058, 563);
            this->splitContainer2->SplitterDistance = 396;
            this->splitContainer2->TabIndex = 0;
            // 
            // splitContainer3
            // 
            this->splitContainer3->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
            this->splitContainer3->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer3->Location = System::Drawing::Point(0, 0);
            this->splitContainer3->Name = L"splitContainer3";
            this->splitContainer3->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer3.Panel1
            // 
            this->splitContainer3->Panel1->Controls->Add(this->groupBox2);
            this->splitContainer3->Panel1->ImeMode = System::Windows::Forms::ImeMode::NoControl;
            // 
            // splitContainer3.Panel2
            // 
            this->splitContainer3->Panel2->Controls->Add(this->splitContainer5);
            this->splitContainer3->Size = System::Drawing::Size(1058, 396);
            this->splitContainer3->SplitterDistance = 156;
            this->splitContainer3->TabIndex = 0;
            // 
            // groupBox2
            // 
            this->groupBox2->Controls->Add(this->TodayHold_dataGridView);
            this->groupBox2->Dock = System::Windows::Forms::DockStyle::Fill;
            this->groupBox2->Location = System::Drawing::Point(0, 0);
            this->groupBox2->Name = L"groupBox2";
            this->groupBox2->Size = System::Drawing::Size(1054, 152);
            this->groupBox2->TabIndex = 0;
            this->groupBox2->TabStop = false;
            this->groupBox2->Text = L"当前持仓";
            // 
            // TodayHold_dataGridView
            // 
            this->TodayHold_dataGridView->AllowUserToAddRows = false;
            this->TodayHold_dataGridView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->TodayHold_dataGridView->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
            dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle1->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle1->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle1->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle1->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->TodayHold_dataGridView->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            this->TodayHold_dataGridView->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->TodayHold_dataGridView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(7) {this->Column1, 
                this->Column2, this->Column3, this->Column4, this->Column5, this->Column6, this->Column7});
            dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle2->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle2->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->TodayHold_dataGridView->DefaultCellStyle = dataGridViewCellStyle2;
            this->TodayHold_dataGridView->Location = System::Drawing::Point(3, 17);
            this->TodayHold_dataGridView->MultiSelect = false;
            this->TodayHold_dataGridView->Name = L"TodayHold_dataGridView";
            this->TodayHold_dataGridView->ReadOnly = true;
            this->TodayHold_dataGridView->RowHeadersVisible = false;
            this->TodayHold_dataGridView->RowTemplate->Height = 23;
            this->TodayHold_dataGridView->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->TodayHold_dataGridView->Size = System::Drawing::Size(1048, 125);
            this->TodayHold_dataGridView->TabIndex = 0;
            this->TodayHold_dataGridView->CellMouseDown += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &ForceManualForm::TodayHold_dataGridView_CellMouseDown);
            // 
            // Column1
            // 
            this->Column1->HeaderText = L"合约";
            this->Column1->Name = L"Column1";
            this->Column1->ReadOnly = true;
            // 
            // Column2
            // 
            this->Column2->HeaderText = L"持仓方向";
            this->Column2->Name = L"Column2";
            this->Column2->ReadOnly = true;
            // 
            // Column3
            // 
            this->Column3->HeaderText = L"总持仓";
            this->Column3->Name = L"Column3";
            this->Column3->ReadOnly = true;
            // 
            // Column4
            // 
            this->Column4->HeaderText = L"可平今数";
            this->Column4->Name = L"Column4";
            this->Column4->ReadOnly = true;
            // 
            // Column5
            // 
            this->Column5->HeaderText = L"可平昨数";
            this->Column5->Name = L"Column5";
            this->Column5->ReadOnly = true;
            // 
            // Column6
            // 
            this->Column6->HeaderText = L"投保";
            this->Column6->Name = L"Column6";
            this->Column6->ReadOnly = true;
            // 
            // Column7
            // 
            this->Column7->HeaderText = L"保证金";
            this->Column7->Name = L"Column7";
            this->Column7->ReadOnly = true;
            // 
            // splitContainer5
            // 
            this->splitContainer5->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
            this->splitContainer5->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer5->Location = System::Drawing::Point(0, 0);
            this->splitContainer5->Name = L"splitContainer5";
            this->splitContainer5->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer5.Panel1
            // 
            this->splitContainer5->Panel1->Controls->Add(this->splitContainer4);
            // 
            // splitContainer5.Panel2
            // 
            this->splitContainer5->Panel2->Controls->Add(this->AllEnsureFund_label);
            this->splitContainer5->Panel2->Controls->Add(this->Order_button);
            this->splitContainer5->Panel2->Controls->Add(this->CalculateTest_button);
            this->splitContainer5->Panel2->Controls->Add(this->label8);
            this->splitContainer5->Size = System::Drawing::Size(1058, 236);
            this->splitContainer5->SplitterDistance = 202;
            this->splitContainer5->TabIndex = 0;
            // 
            // splitContainer4
            // 
            this->splitContainer4->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
            this->splitContainer4->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer4->Location = System::Drawing::Point(0, 0);
            this->splitContainer4->Name = L"splitContainer4";
            // 
            // splitContainer4.Panel1
            // 
            this->splitContainer4->Panel1->Controls->Add(this->groupBox3);
            // 
            // splitContainer4.Panel2
            // 
            this->splitContainer4->Panel2->Controls->Add(this->ForceDirect_comboBox);
            this->splitContainer4->Panel2->Controls->Add(this->PriceType_comboBox);
            this->splitContainer4->Panel2->Controls->Add(this->CommissPrice_textBox);
            this->splitContainer4->Panel2->Controls->Add(this->Spread_textBox);
            this->splitContainer4->Panel2->Controls->Add(this->ForceCount_textBox);
            this->splitContainer4->Panel2->Controls->Add(this->Contract_textBox);
            this->splitContainer4->Panel2->Controls->Add(this->Cancel_button);
            this->splitContainer4->Panel2->Controls->Add(this->Save_button);
            this->splitContainer4->Panel2->Controls->Add(this->CommissPrice_checkBox);
            this->splitContainer4->Panel2->Controls->Add(this->Spread_checkBox);
            this->splitContainer4->Panel2->Controls->Add(this->label15);
            this->splitContainer4->Panel2->Controls->Add(this->label14);
            this->splitContainer4->Panel2->Controls->Add(this->label13);
            this->splitContainer4->Panel2->Controls->Add(this->label12);
            this->splitContainer4->Size = System::Drawing::Size(1058, 202);
            this->splitContainer4->SplitterDistance = 850;
            this->splitContainer4->TabIndex = 0;
            // 
            // groupBox3
            // 
            this->groupBox3->Controls->Add(this->ForceTable_dataGridView);
            this->groupBox3->Dock = System::Windows::Forms::DockStyle::Fill;
            this->groupBox3->Location = System::Drawing::Point(0, 0);
            this->groupBox3->Name = L"groupBox3";
            this->groupBox3->Size = System::Drawing::Size(846, 198);
            this->groupBox3->TabIndex = 0;
            this->groupBox3->TabStop = false;
            this->groupBox3->Text = L"强平单列表";
            // 
            // ForceTable_dataGridView
            // 
            this->ForceTable_dataGridView->AllowUserToAddRows = false;
            this->ForceTable_dataGridView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->ForceTable_dataGridView->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
            dataGridViewCellStyle3->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle3->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle3->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle3->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle3->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle3->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle3->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->ForceTable_dataGridView->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle3;
            this->ForceTable_dataGridView->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->ForceTable_dataGridView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(10) {this->Column35, 
                this->Column8, this->Column9, this->Column10, this->Column11, this->Column12, this->Column13, this->Column14, this->Column15, 
                this->Column16});
            dataGridViewCellStyle4->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle4->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle4->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle4->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle4->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle4->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle4->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->ForceTable_dataGridView->DefaultCellStyle = dataGridViewCellStyle4;
            this->ForceTable_dataGridView->Location = System::Drawing::Point(3, 17);
            this->ForceTable_dataGridView->MultiSelect = false;
            this->ForceTable_dataGridView->Name = L"ForceTable_dataGridView";
            this->ForceTable_dataGridView->RowHeadersVisible = false;
            this->ForceTable_dataGridView->RowTemplate->Height = 23;
            this->ForceTable_dataGridView->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->ForceTable_dataGridView->Size = System::Drawing::Size(840, 178);
            this->ForceTable_dataGridView->TabIndex = 0;
            this->ForceTable_dataGridView->CellMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &ForceManualForm::ForceTable_dataGridView_CellMouseClick);
            this->ForceTable_dataGridView->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &ForceManualForm::ForceTable_dataGridView_CellContentClick);
            // 
            // Column35
            // 
            this->Column35->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::DisplayedCells;
            this->Column35->HeaderText = L"";
            this->Column35->Name = L"Column35";
            this->Column35->Width = 5;
            // 
            // Column8
            // 
            this->Column8->HeaderText = L"合约";
            this->Column8->Name = L"Column8";
            this->Column8->ReadOnly = true;
            this->Column8->Resizable = System::Windows::Forms::DataGridViewTriState::True;
            // 
            // Column9
            // 
            this->Column9->HeaderText = L"强平方向";
            this->Column9->Name = L"Column9";
            this->Column9->ReadOnly = true;
            this->Column9->Resizable = System::Windows::Forms::DataGridViewTriState::True;
            // 
            // Column10
            // 
            this->Column10->HeaderText = L"投保";
            this->Column10->Name = L"Column10";
            this->Column10->ReadOnly = true;
            // 
            // Column11
            // 
            this->Column11->HeaderText = L"平今/平仓";
            this->Column11->Name = L"Column11";
            this->Column11->ReadOnly = true;
            // 
            // Column12
            // 
            this->Column12->HeaderText = L"可平数量";
            this->Column12->Name = L"Column12";
            this->Column12->ReadOnly = true;
            // 
            // Column13
            // 
            this->Column13->HeaderText = L"强平数量";
            this->Column13->Name = L"Column13";
            this->Column13->ReadOnly = true;
            // 
            // Column14
            // 
            this->Column14->HeaderText = L"价格类型";
            this->Column14->Name = L"Column14";
            this->Column14->ReadOnly = true;
            // 
            // Column15
            // 
            this->Column15->HeaderText = L"偏移价位";
            this->Column15->Name = L"Column15";
            this->Column15->ReadOnly = true;
            // 
            // Column16
            // 
            this->Column16->HeaderText = L"释放保证金";
            this->Column16->Name = L"Column16";
            this->Column16->ReadOnly = true;
            // 
            // ForceDirect_comboBox
            // 
            this->ForceDirect_comboBox->FormattingEnabled = true;
            this->ForceDirect_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"买", L"卖"});
            this->ForceDirect_comboBox->Location = System::Drawing::Point(85, 36);
            this->ForceDirect_comboBox->Name = L"ForceDirect_comboBox";
            this->ForceDirect_comboBox->Size = System::Drawing::Size(99, 20);
            this->ForceDirect_comboBox->TabIndex = 7;
            // 
            // PriceType_comboBox
            // 
            this->PriceType_comboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->PriceType_comboBox->FormattingEnabled = true;
            this->PriceType_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"反向涨跌停", L"最新价", L"买价", L"卖价", L"限价"});
            this->PriceType_comboBox->Location = System::Drawing::Point(85, 86);
            this->PriceType_comboBox->Name = L"PriceType_comboBox";
            this->PriceType_comboBox->Size = System::Drawing::Size(99, 20);
            this->PriceType_comboBox->TabIndex = 6;
            this->PriceType_comboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &ForceManualForm::PriceType_comboBox_SelectedIndexChanged);
            // 
            // CommissPrice_textBox
            // 
            this->CommissPrice_textBox->Enabled = false;
            this->CommissPrice_textBox->Location = System::Drawing::Point(85, 134);
            this->CommissPrice_textBox->Name = L"CommissPrice_textBox";
            this->CommissPrice_textBox->Size = System::Drawing::Size(100, 21);
            this->CommissPrice_textBox->TabIndex = 5;
            this->CommissPrice_textBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ForceManualForm::CommissPrice_textBox_KeyPress);
            // 
            // Spread_textBox
            // 
            this->Spread_textBox->Enabled = false;
            this->Spread_textBox->Location = System::Drawing::Point(85, 109);
            this->Spread_textBox->Name = L"Spread_textBox";
            this->Spread_textBox->Size = System::Drawing::Size(100, 21);
            this->Spread_textBox->TabIndex = 5;
            this->Spread_textBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ForceManualForm::Spread_textBox_KeyPress);
            // 
            // ForceCount_textBox
            // 
            this->ForceCount_textBox->Location = System::Drawing::Point(85, 61);
            this->ForceCount_textBox->Name = L"ForceCount_textBox";
            this->ForceCount_textBox->Size = System::Drawing::Size(100, 21);
            this->ForceCount_textBox->TabIndex = 5;
            this->ForceCount_textBox->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ForceManualForm::ForceCount_textBox_KeyPress);
            // 
            // Contract_textBox
            // 
            this->Contract_textBox->Location = System::Drawing::Point(85, 10);
            this->Contract_textBox->Name = L"Contract_textBox";
            this->Contract_textBox->Size = System::Drawing::Size(100, 21);
            this->Contract_textBox->TabIndex = 5;
            // 
            // Cancel_button
            // 
            this->Cancel_button->Location = System::Drawing::Point(101, 169);
            this->Cancel_button->Name = L"Cancel_button";
            this->Cancel_button->Size = System::Drawing::Size(75, 23);
            this->Cancel_button->TabIndex = 4;
            this->Cancel_button->Text = L"取消";
            this->Cancel_button->UseVisualStyleBackColor = true;
            this->Cancel_button->Click += gcnew System::EventHandler(this, &ForceManualForm::Cancel_button_Click);
            // 
            // Save_button
            // 
            this->Save_button->Location = System::Drawing::Point(20, 169);
            this->Save_button->Name = L"Save_button";
            this->Save_button->Size = System::Drawing::Size(75, 23);
            this->Save_button->TabIndex = 4;
            this->Save_button->Text = L"保存";
            this->Save_button->UseVisualStyleBackColor = true;
            this->Save_button->Click += gcnew System::EventHandler(this, &ForceManualForm::Save_button_Click);
            // 
            // CommissPrice_checkBox
            // 
            this->CommissPrice_checkBox->AutoSize = true;
            this->CommissPrice_checkBox->Enabled = false;
            this->CommissPrice_checkBox->Location = System::Drawing::Point(13, 137);
            this->CommissPrice_checkBox->Name = L"CommissPrice_checkBox";
            this->CommissPrice_checkBox->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
            this->CommissPrice_checkBox->Size = System::Drawing::Size(60, 16);
            this->CommissPrice_checkBox->TabIndex = 3;
            this->CommissPrice_checkBox->Text = L"委托价";
            this->CommissPrice_checkBox->UseVisualStyleBackColor = true;
            // 
            // Spread_checkBox
            // 
            this->Spread_checkBox->AutoSize = true;
            this->Spread_checkBox->Enabled = false;
            this->Spread_checkBox->Location = System::Drawing::Point(25, 113);
            this->Spread_checkBox->Name = L"Spread_checkBox";
            this->Spread_checkBox->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
            this->Spread_checkBox->Size = System::Drawing::Size(48, 16);
            this->Spread_checkBox->TabIndex = 3;
            this->Spread_checkBox->Text = L"价差";
            this->Spread_checkBox->UseVisualStyleBackColor = true;
            // 
            // label15
            // 
            this->label15->AutoSize = true;
            this->label15->Location = System::Drawing::Point(20, 91);
            this->label15->Name = L"label15";
            this->label15->Size = System::Drawing::Size(65, 12);
            this->label15->TabIndex = 2;
            this->label15->Text = L"价格类型：";
            // 
            // label14
            // 
            this->label14->AutoSize = true;
            this->label14->Location = System::Drawing::Point(20, 67);
            this->label14->Name = L"label14";
            this->label14->Size = System::Drawing::Size(65, 12);
            this->label14->TabIndex = 2;
            this->label14->Text = L"强平数量：";
            // 
            // label13
            // 
            this->label13->AutoSize = true;
            this->label13->Location = System::Drawing::Point(20, 41);
            this->label13->Name = L"label13";
            this->label13->Size = System::Drawing::Size(65, 12);
            this->label13->TabIndex = 2;
            this->label13->Text = L"强平方向：";
            // 
            // label12
            // 
            this->label12->AutoSize = true;
            this->label12->Location = System::Drawing::Point(44, 16);
            this->label12->Name = L"label12";
            this->label12->Size = System::Drawing::Size(41, 12);
            this->label12->TabIndex = 2;
            this->label12->Text = L"合约：";
            // 
            // AllEnsureFund_label
            // 
            this->AllEnsureFund_label->AutoSize = true;
            this->AllEnsureFund_label->Location = System::Drawing::Point(104, 6);
            this->AllEnsureFund_label->Name = L"AllEnsureFund_label";
            this->AllEnsureFund_label->Size = System::Drawing::Size(41, 12);
            this->AllEnsureFund_label->TabIndex = 2;
            this->AllEnsureFund_label->Text = L"label8";
            // 
            // Order_button
            // 
            this->Order_button->Location = System::Drawing::Point(467, 1);
            this->Order_button->Name = L"Order_button";
            this->Order_button->Size = System::Drawing::Size(75, 23);
            this->Order_button->TabIndex = 0;
            this->Order_button->Text = L"下单";
            this->Order_button->UseVisualStyleBackColor = true;
            this->Order_button->Click += gcnew System::EventHandler(this, &ForceManualForm::Order_button_Click);
            // 
            // CalculateTest_button
            // 
            this->CalculateTest_button->Location = System::Drawing::Point(369, 1);
            this->CalculateTest_button->Name = L"CalculateTest_button";
            this->CalculateTest_button->Size = System::Drawing::Size(75, 23);
            this->CalculateTest_button->TabIndex = 0;
            this->CalculateTest_button->Text = L"试算";
            this->CalculateTest_button->UseVisualStyleBackColor = true;
            // 
            // label8
            // 
            this->label8->AutoSize = true;
            this->label8->Location = System::Drawing::Point(9, 6);
            this->label8->Name = L"label8";
            this->label8->Size = System::Drawing::Size(89, 12);
            this->label8->TabIndex = 2;
            this->label8->Text = L"总释放保证金：";
            // 
            // groupBox4
            // 
            this->groupBox4->Controls->Add(this->OrderTable_dataGridView);
            this->groupBox4->Dock = System::Windows::Forms::DockStyle::Fill;
            this->groupBox4->Location = System::Drawing::Point(0, 0);
            this->groupBox4->Name = L"groupBox4";
            this->groupBox4->Size = System::Drawing::Size(1054, 159);
            this->groupBox4->TabIndex = 0;
            this->groupBox4->TabStop = false;
            this->groupBox4->Text = L"当前报单";
            // 
            // OrderTable_dataGridView
            // 
            this->OrderTable_dataGridView->AllowUserToAddRows = false;
            this->OrderTable_dataGridView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->OrderTable_dataGridView->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
            dataGridViewCellStyle5->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle5->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle5->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle5->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle5->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle5->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle5->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->OrderTable_dataGridView->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle5;
            this->OrderTable_dataGridView->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->OrderTable_dataGridView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(16) {this->Column17, 
                this->Column18, this->Column19, this->Column20, this->Column21, this->Column22, this->Column23, this->Column24, this->Column27, 
                this->Column28, this->Column29, this->Column30, this->Column31, this->Column32, this->Column33, this->Column34});
            dataGridViewCellStyle6->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle6->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle6->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle6->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle6->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle6->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle6->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->OrderTable_dataGridView->DefaultCellStyle = dataGridViewCellStyle6;
            this->OrderTable_dataGridView->Location = System::Drawing::Point(3, 17);
            this->OrderTable_dataGridView->MultiSelect = false;
            this->OrderTable_dataGridView->Name = L"OrderTable_dataGridView";
            this->OrderTable_dataGridView->ReadOnly = true;
            this->OrderTable_dataGridView->RowHeadersVisible = false;
            this->OrderTable_dataGridView->RowTemplate->Height = 23;
            this->OrderTable_dataGridView->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->OrderTable_dataGridView->Size = System::Drawing::Size(1048, 133);
            this->OrderTable_dataGridView->TabIndex = 0;
            // 
            // Quit_button
            // 
            this->Quit_button->Location = System::Drawing::Point(420, 1);
            this->Quit_button->Name = L"Quit_button";
            this->Quit_button->Size = System::Drawing::Size(75, 23);
            this->Quit_button->TabIndex = 1;
            this->Quit_button->Text = L"退出";
            this->Quit_button->UseVisualStyleBackColor = true;
            this->Quit_button->Click += gcnew System::EventHandler(this, &ForceManualForm::Quit_button_Click);
            // 
            // TodayHoldDataGrid_contextMenuStrip
            // 
            this->TodayHoldDataGrid_contextMenuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->ForceShortcut_ToolStripMenuItem});
            this->TodayHoldDataGrid_contextMenuStrip->Name = L"TodayHoldDataGrid_contextMenuStrip";
            this->TodayHoldDataGrid_contextMenuStrip->Size = System::Drawing::Size(119, 26);
            // 
            // ForceShortcut_ToolStripMenuItem
            // 
            this->ForceShortcut_ToolStripMenuItem->Name = L"ForceShortcut_ToolStripMenuItem";
            this->ForceShortcut_ToolStripMenuItem->Size = System::Drawing::Size(118, 22);
            this->ForceShortcut_ToolStripMenuItem->Text = L"快捷强平";
            // 
            // Column17
            // 
            this->Column17->HeaderText = L"保单编号";
            this->Column17->Name = L"Column17";
            this->Column17->ReadOnly = true;
            this->Column17->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column18
            // 
            this->Column18->HeaderText = L"合约";
            this->Column18->Name = L"Column18";
            this->Column18->ReadOnly = true;
            this->Column18->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column19
            // 
            this->Column19->HeaderText = L"买卖";
            this->Column19->Name = L"Column19";
            this->Column19->ReadOnly = true;
            this->Column19->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column20
            // 
            this->Column20->HeaderText = L"开平";
            this->Column20->Name = L"Column20";
            this->Column20->ReadOnly = true;
            this->Column20->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column21
            // 
            this->Column21->HeaderText = L"报单手数";
            this->Column21->Name = L"Column21";
            this->Column21->ReadOnly = true;
            this->Column21->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column22
            // 
            this->Column22->HeaderText = L"未成交手数";
            this->Column22->Name = L"Column22";
            this->Column22->ReadOnly = true;
            this->Column22->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column23
            // 
            this->Column23->HeaderText = L"报单价格";
            this->Column23->Name = L"Column23";
            this->Column23->ReadOnly = true;
            this->Column23->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column24
            // 
            this->Column24->HeaderText = L"报单时间";
            this->Column24->Name = L"Column24";
            this->Column24->ReadOnly = true;
            this->Column24->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column27
            // 
            this->Column27->HeaderText = L"序列号";
            this->Column27->Name = L"Column27";
            this->Column27->ReadOnly = true;
            this->Column27->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column28
            // 
            this->Column28->HeaderText = L"投保";
            this->Column28->Name = L"Column28";
            this->Column28->ReadOnly = true;
            this->Column28->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column29
            // 
            this->Column29->HeaderText = L"报单类型";
            this->Column29->Name = L"Column29";
            this->Column29->ReadOnly = true;
            this->Column29->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column30
            // 
            this->Column30->HeaderText = L"是否强平";
            this->Column30->Name = L"Column30";
            this->Column30->ReadOnly = true;
            this->Column30->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column31
            // 
            this->Column31->HeaderText = L"强平原因";
            this->Column31->Name = L"Column31";
            this->Column31->ReadOnly = true;
            this->Column31->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column32
            // 
            this->Column32->HeaderText = L"指令结果";
            this->Column32->Name = L"Column32";
            this->Column32->ReadOnly = true;
            this->Column32->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column33
            // 
            this->Column33->HeaderText = L"挂单状态";
            this->Column33->Name = L"Column33";
            this->Column33->ReadOnly = true;
            this->Column33->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // Column34
            // 
            this->Column34->HeaderText = L"详细状态";
            this->Column34->Name = L"Column34";
            this->Column34->ReadOnly = true;
            this->Column34->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // ForceManualForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->AutoScroll = true;
            this->ClientSize = System::Drawing::Size(1216, 603);
            this->Controls->Add(this->splitContainer1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->Name = L"ForceManualForm";
            this->Text = L"手动强平";
            this->Load += gcnew System::EventHandler(this, &ForceManualForm::ForceManualForm_Load);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &ForceManualForm::ForceManualForm_FormClosed);
            this->splitContainer1->Panel1->ResumeLayout(false);
            this->splitContainer1->Panel2->ResumeLayout(false);
            this->splitContainer1->ResumeLayout(false);
            this->groupBox1->ResumeLayout(false);
            this->groupBox1->PerformLayout();
            this->splitContainer6->Panel1->ResumeLayout(false);
            this->splitContainer6->Panel2->ResumeLayout(false);
            this->splitContainer6->ResumeLayout(false);
            this->panel1->ResumeLayout(false);
            this->splitContainer2->Panel1->ResumeLayout(false);
            this->splitContainer2->Panel2->ResumeLayout(false);
            this->splitContainer2->ResumeLayout(false);
            this->splitContainer3->Panel1->ResumeLayout(false);
            this->splitContainer3->Panel2->ResumeLayout(false);
            this->splitContainer3->ResumeLayout(false);
            this->groupBox2->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->TodayHold_dataGridView))->EndInit();
            this->splitContainer5->Panel1->ResumeLayout(false);
            this->splitContainer5->Panel2->ResumeLayout(false);
            this->splitContainer5->Panel2->PerformLayout();
            this->splitContainer5->ResumeLayout(false);
            this->splitContainer4->Panel1->ResumeLayout(false);
            this->splitContainer4->Panel2->ResumeLayout(false);
            this->splitContainer4->Panel2->PerformLayout();
            this->splitContainer4->ResumeLayout(false);
            this->groupBox3->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->ForceTable_dataGridView))->EndInit();
            this->groupBox4->ResumeLayout(false);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->OrderTable_dataGridView))->EndInit();
            this->TodayHoldDataGrid_contextMenuStrip->ResumeLayout(false);
            this->ResumeLayout(false);

        }
#pragma endregion
    //重载WndProc，以处理WM_COMMAND消息
    protected:virtual void WndProc( Message% m ) override
              {
                  if (  m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival )
                  {
                      string strs[9];
                      char *p = (char*)m.LParam.ToInt32();
                      Stru_UniPkgHead head;					
                      memset(&head, 0, sizeof(head));
                      memcpy(&head, p, sizeof(head));

                      switch(head.cmdid)
                      {
                      case Cmd_RM_SubscribeOrder_Rsp://订阅报单返回
                          if ( head.userdata1 == OPERATOR_SUCCESS )
                          {	
                              std::string strOrderRef;
                              std::string strOrderSysID;
                              std::string strDirection;
                              std::string strCombOffsetFlag;                             
                              std::string strHedgeFlag;
                              std::string strOrderType;
                              std::string strUserForceClose;
                              std::string strForceCloseReason;
                              std::string strOrderSubmitStatus;
                              std::string strOrderStatus;
                              char strVolumeTotalOriginal[128];
                              char strVolumeTotalRest[128];
                              char strLimitPrice[128];
                              char strInsertTime[128];
                              //char strFrozenMargin[128];
                              //char strCommission[128];
                              char strSequenceNo[128];
                              char strStatusMsg[128];

                              SequencialOrderField* pOrderInfo = NULL;
                              int nItemCount = head.len/sizeof(SequencialOrderField);

                              m_pForceCloseInfoData->LockObject();
                              for(int i=0; i<nItemCount; i++) 
                              {
                                  pOrderInfo = (SequencialOrderField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialOrderField));

                                  if (pOrderInfo->OrderStatus == T_OST_PartTradedQueueing || pOrderInfo->OrderStatus ==T_OST_NoTradeQueueing)
                                  {
                                      switch(pOrderInfo->CombOffsetFlag[0])
                                      {
                                      case T_OF_Open:
                                          strCombOffsetFlag = "开仓";
                                          break;
                                      case T_OF_Close:
                                          strCombOffsetFlag = "平仓";
                                          break;
                                      case T_OF_ForceClose:
                                          strCombOffsetFlag = "强平";
                                          break;
                                      case T_OF_CloseToday:
                                          strCombOffsetFlag = "平今";
                                          break;
                                      case T_OF_CloseYesterday:
                                          strCombOffsetFlag = "平昨";
                                          break;
                                      case T_OF_ForceOff:
                                          strCombOffsetFlag = "强减";
                                          break;
                                      case T_OF_LocalForceClose:
                                          strCombOffsetFlag = "本地强平";
                                          break;
                                      }
                                      switch(pOrderInfo->CombHedgeFlag[0])
                                      {
                                      case T_HF_Speculation:
                                          strHedgeFlag = "投机";
                                          break;
                                      case T_HF_Arbitrage:
                                          strHedgeFlag = "套利";
                                          break;
                                      case T_HF_Hedge:
                                          strHedgeFlag = "套保";
                                          break;
                                      }

                                      switch(pOrderInfo->OrderType)
                                      {
                                      case T_ORDT_Normal:
                                          strOrderType = "正常";
                                          break;
                                      case T_ORDT_DeriveFromQuote:
                                          strOrderType = "报价衍生";
                                          break;
                                      case T_ORDT_DeriveFromCombination:
                                          strOrderType = "组合衍生";
                                          break;
                                      case T_ORDT_Combination:
                                          strOrderType = "组合报单";
                                          break;
                                      case T_ORDT_ConditionalOrder:
                                          strOrderType = "条件单";
                                          break;
                                      case T_ORDT_Swap:
                                          strOrderType = "互换单";
                                          break;
                                      }

                                      switch(pOrderInfo->ForceCloseReason)
                                      {
                                      case T_FCC_NotForceClose:
                                          strForceCloseReason = "非强平";
                                          break;
                                      case T_FCC_LackDeposit:
                                          strForceCloseReason = "资金不足";
                                          break;
                                      case T_FCC_ClientOverPositionLimit:
                                          strForceCloseReason = "客户超仓";
                                          break;
                                      case T_FCC_MemberOverPositionLimit:
                                          strForceCloseReason = "会员超仓";
                                          break;
                                      case T_FCC_NotMultiple:
                                          strForceCloseReason = "持仓非整数倍";
                                          break;
                                      case T_FCC_Violation:
                                          strForceCloseReason = "违规";
                                          break;
                                      case T_FCC_Other:
                                          strForceCloseReason = "其它";
                                          break;
                                      case T_FCC_PersonDeliv:
                                          strForceCloseReason = "自然人临近交割";
                                          break;
                                      }
                                      switch(pOrderInfo->OrderSubmitStatus)
                                      {
                                      case T_OSS_InsertSubmitted:
                                          strOrderSubmitStatus = "已经提交";
                                          break;
                                      case T_OSS_CancelSubmitted:
                                          strOrderSubmitStatus = "撤单已经提交";
                                          break;
                                      case T_OSS_ModifySubmitted:
                                          strOrderSubmitStatus = "修改已经提交";
                                          break;
                                      case T_OSS_Accepted:
                                          strOrderSubmitStatus = "已经接受";
                                          break;
                                      case T_OSS_InsertRejected:
                                          strOrderSubmitStatus = "报单已经被拒绝";
                                          break;
                                      case T_OSS_CancelRejected:
                                          strOrderSubmitStatus = "撤单已经被拒绝";
                                          break;
                                      case T_OSS_ModifyRejected:
                                          strOrderSubmitStatus = "改单已经被拒绝";
                                          break;
                                      }

                                      switch(pOrderInfo->OrderStatus)
                                      {
                                      case T_OST_AllTraded:
                                          strOrderStatus = "全部成交";
                                          break;
                                      case T_OST_PartTradedQueueing:
                                          strOrderStatus = "部分成交还在队列中";
                                          break;
                                      case T_OST_PartTradedNotQueueing:
                                          strOrderStatus = "部分成交不在队列中";
                                          break;
                                      case T_OST_NoTradeQueueing:
                                          strOrderStatus = "未成交还在队列中";
                                          break;
                                      case T_OST_NoTradeNotQueueing:
                                          strOrderStatus = "未成交不在队列中";
                                          break;
                                      case T_OST_Canceled:
                                          strOrderStatus = "撤单";
                                          break;
                                      case T_OST_Unknown:
                                          strOrderStatus = "未知";
                                          break;
                                      case T_OST_NotTouched:
                                          strOrderStatus = "尚未触发";
                                          break;
                                      case T_OST_Touched:
                                          strOrderStatus = "已触发";
                                          break;
                                      }


                                      switch(pOrderInfo->OrderType)
                                      {
                                      case T_ORDT_Normal:
                                          strOrderType = "正常";
                                              break;
                                      case T_ORDT_DeriveFromQuote:
                                          strOrderType = "报价衍生";
                                              break;
                                      case T_ORDT_DeriveFromCombination:
                                          strOrderType = "组合衍生";
                                              break;
                                      case T_ORDT_Combination:
                                          strOrderType = "组合报单";
                                              break;
                                      case T_ORDT_ConditionalOrder:
                                          strOrderType = "条件单";
                                              break;
                                      case T_ORDT_Swap:
                                          strOrderType = "互换单";
                                              break;
                                      default:
                                          strOrderType = "未知";
                                          break;
                                      }
                                      strOrderRef = pOrderInfo->OrderRef;
                                      strOrderSysID = pOrderInfo->OrderSysID;                                    
                                      strDirection = (pOrderInfo->Direction == T_D_Buy?"买":"卖");
                                      sprintf(strVolumeTotalOriginal, "%d", pOrderInfo->VolumeTotalOriginal); 
                                      sprintf(strVolumeTotalRest, "%d", pOrderInfo->VolumeTotal);
                                      sprintf(strLimitPrice, "%0.2f", pOrderInfo->LimitPrice);
                                      sprintf(strInsertTime, "%s", pOrderInfo->InsertTime);
                                      //sprintf(strFrozenMargin,"%0.2f",0.0001/*m_pAllFundInfo->cur.CurrMargin*/);
                                      //sprintf(strCommission,"%0.2f",0.0002/*m_pAllFundInfo->cur.Commission*/);
                                      sprintf(strSequenceNo, "%d", pOrderInfo->BrokerOrderSeq);
                                      strUserForceClose = (pOrderInfo->UserForceClose == 1?"是":"否");
                                      sprintf(strStatusMsg, "%s", pOrderInfo->StatusMsg);

                                      array<String^>^row = {Tools::string2String(strOrderSysID.c_str()),
                                                              Tools::string2String(pOrderInfo->InstrumentID),
                                                              Tools::string2String(strDirection.c_str()),
                                                              Tools::string2String(strCombOffsetFlag.c_str()),
                                                              Tools::string2String(strVolumeTotalOriginal),
                                                              Tools::string2String(strVolumeTotalRest),
                                                              Tools::string2String(strLimitPrice),
                                                              Tools::string2String(strInsertTime),
                                                              //Tools::string2String(strFrozenMargin),
                                                              //Tools::string2String(strCommission),
                                                              Tools::string2String(strSequenceNo),
                                                              Tools::string2String(strHedgeFlag.c_str()),
                                                              Tools::string2String(strOrderType.c_str()),
                                                              Tools::string2String(strUserForceClose.c_str()),
                                                              Tools::string2String(strForceCloseReason.c_str()),
                                                              Tools::string2String(strOrderSubmitStatus.c_str()),
                                                              Tools::string2String(strOrderStatus.c_str()),
                                                              Tools::string2String(strStatusMsg)};                                          

                                      this->OrderTable_dataGridView->Rows->Add(row);
                                      m_pForceManualForm2->SetOrderRefToId(pOrderInfo->OrderRef,this->OrderTable_dataGridView->Rows->Count);

                                  }
                              }

                              m_pForceCloseInfoData->UnlockObject(); 
                          }
                          else
                          {
                              string str = (char*)(p+sizeof(Stru_UniPkgHead));
                              String^ strtemp = Tools::string2String(str.c_str());
                              MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);				
                          }
                          break;
                      case Cmd_RM_Order_Push://报单推送
                          if ( head.userdata1 == OPERATOR_SUCCESS )
                          {	
                              std::string strOrderSysID;
                              std::string strDirection;
                              std::string strCombOffsetFlag;                             
                              std::string strHedgeFlag;
                              std::string strOrderType;
                              std::string strUserForceClose;
                              std::string strForceCloseReason;
                              std::string strOrderSubmitStatus;
                              std::string strOrderStatus;
                              char strVolumeTotalOriginal[128];
                              char strVolumeTotalRest[128];
                              char strLimitPrice[128];
                              char strInsertTime[128];
                              char strTodayPosition[128];
                              char strYdPosition[128];
                              //char strFrozenMargin[128];
                              //char strCommission[128];
                              char strSequenceNo[128];
                              char strStatusMsg[128];

                              m_pForceCloseInfoData->LockObject();
                              SequencialOrderField* pOrderInfo = (SequencialOrderField*)(p+sizeof(Stru_UniPkgHead));

                              switch(pOrderInfo->CombOffsetFlag[0])
                              {
                              case T_OF_Open:
                                  strCombOffsetFlag = "开仓";
                                  break;
                              case T_OF_Close:
                                  strCombOffsetFlag = "平仓";
                                  break;
                              case T_OF_ForceClose:
                                  strCombOffsetFlag = "强平";
                                  break;
                              case T_OF_CloseToday:
                                  strCombOffsetFlag = "平今";
                                  break;
                              case T_OF_CloseYesterday:
                                  strCombOffsetFlag = "平昨";
                                  break;
                              case T_OF_ForceOff:
                                  strCombOffsetFlag = "强减";
                                  break;
                              case T_OF_LocalForceClose:
                                  strCombOffsetFlag = "本地强平";
                                  break;
                              }
                              switch(pOrderInfo->CombHedgeFlag[0])
                              {
                              case T_HF_Speculation:
                                  strHedgeFlag = "投机";
                                  break;
                              case T_HF_Arbitrage:
                                  strHedgeFlag = "套利";
                                  break;
                              case T_HF_Hedge:
                                  strHedgeFlag = "套保";
                                  break;
                              }

                              switch(pOrderInfo->OrderType)
                              {
                              case T_ORDT_Normal:
                                  strOrderType = "正常";
                                  break;
                              case T_ORDT_DeriveFromQuote:
                                  strOrderType = "报价衍生";
                                  break;
                              case T_ORDT_DeriveFromCombination:
                                  strOrderType = "组合衍生";
                                  break;
                              case T_ORDT_Combination:
                                  strOrderType = "组合报单";
                                  break;
                              case T_ORDT_ConditionalOrder:
                                  strOrderType = "条件单";
                                  break;
                              case T_ORDT_Swap:
                                  strOrderType = "互换单";
                                  break;
                              }

                              switch(pOrderInfo->ForceCloseReason)
                              {
                              case T_FCC_NotForceClose:
                                  strForceCloseReason = "非强平";
                                  break;
                              case T_FCC_LackDeposit:
                                  strForceCloseReason = "资金不足";
                                  break;
                              case T_FCC_ClientOverPositionLimit:
                                  strForceCloseReason = "客户超仓";
                                  break;
                              case T_FCC_MemberOverPositionLimit:
                                  strForceCloseReason = "会员超仓";
                                  break;
                              case T_FCC_NotMultiple:
                                  strForceCloseReason = "持仓非整数倍";
                                  break;
                              case T_FCC_Violation:
                                  strForceCloseReason = "违规";
                                  break;
                              case T_FCC_Other:
                                  strForceCloseReason = "其它";
                                  break;
                              case T_FCC_PersonDeliv:
                                  strForceCloseReason = "自然人临近交割";
                                  break;
                              }

                              switch(pOrderInfo->OrderSubmitStatus)
                              {
                              case T_OSS_InsertSubmitted:
                                  strOrderSubmitStatus = "已经提交";
                                  break;
                              case T_OSS_CancelSubmitted:
                                  strOrderSubmitStatus = "撤单已经提交";
                                  break;
                              case T_OSS_ModifySubmitted:
                                  strOrderSubmitStatus = "修改已经提交";
                                  break;
                              case T_OSS_Accepted:
                                  strOrderSubmitStatus = "已经接受";
                                  break;
                              case T_OSS_InsertRejected:
                                  strOrderSubmitStatus = "报单已经被拒绝";
                                  break;
                              case T_OSS_CancelRejected:
                                  strOrderSubmitStatus = "撤单已经被拒绝";
                                  break;
                              case T_OSS_ModifyRejected:
                                  strOrderSubmitStatus = "改单已经被拒绝";
                                  break;
                              }

                              switch(pOrderInfo->OrderStatus)
                              {
                              case T_OST_AllTraded:
                                  strOrderStatus = "全部成交";
                                  break;
                              case T_OST_PartTradedQueueing:
                                  strOrderStatus = "部分成交还在队列中";
                                  break;
                              case T_OST_PartTradedNotQueueing:
                                  strOrderStatus = "部分成交不在队列中";
                                  break;
                              case T_OST_NoTradeQueueing:
                                  strOrderStatus = "未成交还在队列中";
                                  break;
                              case T_OST_NoTradeNotQueueing:
                                  strOrderStatus = "未成交不在队列中";
                                  break;
                              case T_OST_Canceled:
                                  strOrderStatus = "撤单";
                                  break;
                              case T_OST_Unknown:
                                  strOrderStatus = "未知";
                                  break;
                              case T_OST_NotTouched:
                                  strOrderStatus = "尚未触发";
                                  break;
                              case T_OST_Touched:
                                  strOrderStatus = "已触发";
                                  break;
                              }

                              switch(pOrderInfo->OrderType)
                              {
                              case T_ORDT_Normal:
                                  strOrderType = "正常";
                                      break;
                              case T_ORDT_DeriveFromQuote:
                                  strOrderType = "报价衍生";
                                      break;
                              case T_ORDT_DeriveFromCombination:
                                  strOrderType = "组合衍生";
                                      break;
                              case T_ORDT_Combination:
                                  strOrderType = "组合报单";
                                      break;
                              case T_ORDT_ConditionalOrder:
                                  strOrderType = "条件单";
                                      break;
                              case T_ORDT_Swap:
                                  strOrderType = "互换单";
                                      break;
                              default:
                                  strOrderType = "未知";
                                  break;
                              }
                              strOrderSysID = pOrderInfo->OrderSysID;                                    
                              strDirection = (pOrderInfo->Direction == T_D_Buy?"买":"卖");
                              sprintf(strVolumeTotalOriginal, "%d", pOrderInfo->VolumeTotalOriginal); 
                              sprintf(strVolumeTotalRest, "%d", pOrderInfo->VolumeTotal);
                              sprintf(strLimitPrice, "%0.2f", pOrderInfo->LimitPrice);
                              sprintf(strInsertTime, "%s", pOrderInfo->InsertTime);
                              //sprintf(strFrozenMargin,"%0.2f",pOrderInfo->FrozenMargin);
                              //sprintf(strCommission,"%0.2f",pOrderInfo->FrozenCommission);
                              sprintf(strSequenceNo, "%d", pOrderInfo->BrokerOrderSeq);
                              strUserForceClose = (pOrderInfo->UserForceClose == 1?"是":"否");
                              sprintf(strStatusMsg, "%s", pOrderInfo->StatusMsg);
                           
                              array<String^>^row = {Tools::string2String(strOrderSysID.c_str()),
                                                      Tools::string2String(pOrderInfo->InstrumentID),
                                                      Tools::string2String(strDirection.c_str()),
                                                      Tools::string2String(strCombOffsetFlag.c_str()),
                                                      Tools::string2String(strVolumeTotalOriginal),
                                                      Tools::string2String(strVolumeTotalRest),
                                                      Tools::string2String(strLimitPrice),
                                                      Tools::string2String(strInsertTime),
                                                      //Tools::string2String(strFrozenMargin),
                                                      //Tools::string2String(strCommission),
                                                      Tools::string2String(strSequenceNo),
                                                      Tools::string2String(strHedgeFlag.c_str()),
                                                      Tools::string2String(strOrderType.c_str()),
                                                      Tools::string2String(strUserForceClose.c_str()),
                                                      Tools::string2String(strForceCloseReason.c_str()),
                                                      Tools::string2String(strOrderSubmitStatus.c_str()),
                                                      Tools::string2String(strOrderStatus.c_str()),
                                                      Tools::string2String(strStatusMsg)};
                              
                              if (pOrderInfo->OrderStatus ==T_OST_NoTradeQueueing)
                              {
                                  this->OrderTable_dataGridView->Rows->Add(row);                                
                                  m_pForceManualForm2->SetOrderRefToId(pOrderInfo->OrderRef,this->OrderTable_dataGridView->Rows->Count-1);

                                  //更新持仓可平view
                                  for (int i=0;i<this->TodayHold_dataGridView->Rows->Count;i++)
                                  {
                                      DataGridViewRow^ dataGridViewRow = TodayHold_dataGridView->Rows[i];

                                      if (Tools::string2String(pOrderInfo->InstrumentID) == dataGridViewRow->Cells[0]->Value &&
                                          Tools::string2String(strDirection.c_str()) == dataGridViewRow->Cells[1]->Value)
                                      {
                                          if(pOrderInfo->CombOffsetFlag[0] == T_OF_CloseToday)
                                          {
                                              sprintf(strTodayPosition, "%d", Convert::ToInt32(dataGridViewRow->Cells[3]->Value) - pOrderInfo->VolumeTotal);
                                              dataGridViewRow->Cells[3]->Value = Tools::string2String(strTodayPosition);
                                          }
                                          else if(pOrderInfo->CombOffsetFlag[0] == T_OF_CloseYesterday)
                                          {
                                              sprintf(strYdPosition, "%d", Convert::ToInt32(dataGridViewRow->Cells[4]->Value) - pOrderInfo->VolumeTotal);
                                              dataGridViewRow->Cells[4]->Value = Tools::string2String(strYdPosition);
                                          }
                                      }
                                  }                                  
                              }
                              else if (pOrderInfo->OrderStatus == T_OST_AllTraded || pOrderInfo->OrderStatus ==T_OST_Canceled)
                              {
                                  int nID=-1;
                                  std::map<std::string,int> mapOrderRefToId;
                                  m_pForceManualForm2->GetOrderRefToId(mapOrderRefToId);
                                  std::map<std::string,int>::iterator it = mapOrderRefToId.find(pOrderInfo->OrderRef);
                                  if(it != mapOrderRefToId.end())
                                  {
                                      nID = it->second;
                                      //删掉行
                                      this->OrderTable_dataGridView->Rows->RemoveAt(nID);
                                      //清除记录                                      
                                      m_pForceManualForm2->DeleteOrderRefToId((string)(pOrderInfo->OrderRef));
                                  }

                                  //m_mapOrderRefToId重新编号
                                  m_pForceManualForm2->ResetOrderRefToId(nID);
                              }

                              m_pForceCloseInfoData->UnlockObject(); 

                              //再次订阅资金
                              IntPtr hWnd=this->Handle; 
                              ForceManualForm2::ReqSubscribeFund(Convert::ToInt32(m_strAccountID));
                          }
                          else
                          {
                              string str = (char*)(p+sizeof(Stru_UniPkgHead));
                              String^ strtemp = Tools::string2String(str.c_str());
                              MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);				
                          }
                          break;
                      case Cmd_RM_SubscribeQuot_Rsp://订阅行情返回
                          if ( head.userdata1 == OPERATOR_SUCCESS )
                          {	
                              DepthMarketDataField* pDepthMarketDataField = (DepthMarketDataField*)(p+sizeof(Stru_UniPkgHead));  
                              if (0 == m_pForceCloseInfoData->vecDepthMarketDataInfo.size())
                              {
                                    m_pForceCloseInfoData->vecDepthMarketDataInfo.push_back(*pDepthMarketDataField);
                              }else
                              {
                                  bool bHave = false;
                                  std::vector<DepthMarketDataField>::iterator it;
                                  m_pForceCloseInfoData->LockObject(); 
                                  for (it=m_pForceCloseInfoData->vecDepthMarketDataInfo.begin();it!=m_pForceCloseInfoData->vecDepthMarketDataInfo.end();it++)
                                  {
                                      if (strcmp(pDepthMarketDataField->InstrumentID , it->InstrumentID) == 0)
                                      {
                                          bHave = true;
                                          (*it) = (*pDepthMarketDataField);
                                          break;
                                      }
                                  }
                                  
                                  if (!bHave)
                                  {
                                        m_pForceCloseInfoData->vecDepthMarketDataInfo.push_back(*pDepthMarketDataField);
                                  }
                                  m_pForceCloseInfoData->UnlockObject(); 
                              }
                          }
                          else
                          {
                              string str = (char*)(p+sizeof(Stru_UniPkgHead));
                              String^ strtemp = Tools::string2String(str.c_str());
                              MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);				
                          }
                          break;
                      case Cmd_RM_Quot_Push://行情推送
                          if ( head.userdata1 == OPERATOR_SUCCESS )
                          {	
                              DepthMarketDataField* pDepthMarketDataField = (DepthMarketDataField*)(p+sizeof(Stru_UniPkgHead));  
                              if (0 == m_pForceCloseInfoData->vecDepthMarketDataInfo.size())
                              {
                                  m_pForceCloseInfoData->vecDepthMarketDataInfo.push_back(*pDepthMarketDataField);
                              }else
                              {
                                  bool bHave = false;
                                  std::vector<DepthMarketDataField>::iterator it;
                                  m_pForceCloseInfoData->LockObject(); 
                                  for (it=m_pForceCloseInfoData->vecDepthMarketDataInfo.begin();it!=m_pForceCloseInfoData->vecDepthMarketDataInfo.end();it++)
                                  {
                                      if (pDepthMarketDataField->InstrumentID == it->InstrumentID)
                                      {
                                          bHave = true;
                                          (*it) = (*pDepthMarketDataField);
                                          break;
                                      }
                                  }

                                  if (!bHave)
                                  {
                                      m_pForceCloseInfoData->vecDepthMarketDataInfo.push_back(*pDepthMarketDataField);
                                  }
                                  m_pForceCloseInfoData->UnlockObject(); 
                              } 
                          }
                          else
                          {
                              string str = (char*)(p+sizeof(Stru_UniPkgHead));
                              String^ strtemp = Tools::string2String(str.c_str());
                              MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);				
                          }
                          break;
                      case Cmd_RM_SubscribePosition_Rsp://订阅持仓返回
                          {
                              if(head.userdata1 == OPERATOR_SUCCESS) 
                              {
                                  std::string strDirection;
                                  std::string strHedgeFlag;
                                  char strPosition[128];
                                  char strTodayPosition[128];
                                  char strYdPosition[128];
                                  char strExchangeMargin[128];

                                  SequencialPositionField* pPosiInfo = NULL;
                                  int nItemCount = head.len/sizeof(SequencialPositionField);

                                  m_pForceCloseInfoData->LockObject();
                                  m_pForceCloseInfoData->vecPositionInfo.clear();
                                  this->TodayHold_dataGridView->Rows->Clear();
                                  for(int i=0; i<nItemCount; i++) 
                                  {
                                      pPosiInfo = (SequencialPositionField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialPositionField)); 

                                      if (pPosiInfo->Position == 0)
                                      {
                                          continue;
                                      }else
                                      {
                                          TrustTradeAccount oTrustTradeAccount;
                                          m_pDataInfo->GetTrustTradeAccount(pPosiInfo->InvestorID, oTrustTradeAccount);

                                          if (oTrustTradeAccount.nTradeAccountID != Convert::ToInt32(m_strAccountID))
                                              continue;

                                          m_pForceCloseInfoData->vecPositionInfo.push_back(*pPosiInfo);
                                          m_pForceCloseInfoData->queuePositionInfo.push(*pPosiInfo);
                                      }
                                  }     

                                  while (!m_pForceCloseInfoData->queuePositionInfo.empty())
                                  {
                                      SequencialPositionField positionField = m_pForceCloseInfoData->queuePositionInfo.front();
                                      m_pForceCloseInfoData->queuePositionInfo.pop();

                                      switch(positionField.PosiDirection)
                                      {
                                      case '1':
                                          strDirection = "净";
                                          break;
                                      case '2':
                                          strDirection = "买";
                                          break;
                                      case '3':
                                          strDirection = "卖";
                                          break;
                                      }

                                      switch(positionField.HedgeFlag)
                                      {
                                      case '1':
                                          strHedgeFlag = "投机";
                                          break;
                                      case '2':
                                          strHedgeFlag = "套利";
                                          break;
                                      case '3':
                                          strHedgeFlag = "套保";
                                          break;
                                      }

                                      sprintf(strPosition, "%d", positionField.Position);
                                      sprintf(strTodayPosition, "%d", positionField.TodayPosition);  
                                      sprintf(strYdPosition, "%d",positionField.Position - positionField.TodayPosition);
                                      sprintf(strExchangeMargin, "%0.2f", positionField.UseMargin);

                                      array<String^>^row = {Tools::string2String(positionField.InstrumentID),
                                                          Tools::string2String(strDirection.c_str()),
                                                          Tools::string2String(strPosition),
                                                          Tools::string2String(strTodayPosition),
                                                          Tools::string2String(strYdPosition),
                                                          Tools::string2String(strHedgeFlag.c_str()),
                                                          Tools::string2String(strExchangeMargin)};

                                      this->TodayHold_dataGridView->Rows->Add(row);  
                                      //取消默认选择
                                      this->TodayHold_dataGridView->Rows[0]->Selected = false;
                                  }

                                  m_pForceCloseInfoData->UnlockObject();
                              }
                              else 
                              {
                                  p[sizeof(head)+head.len-1]=0;
                                  //TRACE(p+sizeof(head));
                              }
                          }
                          break;
                      case Cmd_RM_UnSubscribePosition_Rsp://退订持仓返回
                          if(head.userdata1 == OPERATOR_SUCCESS) 
                          {
                          }
                          else 
                          {
                              p[sizeof(head)+head.len-1]=0;
                              //TRACE(p+sizeof(head));
                          }
                          break;
                      case Cmd_RM_Position_Push://持仓推送
                          {
                              if(head.userdata1 == OPERATOR_SUCCESS) 
                              {
                                  std::string strDirection;
                                  std::string strHedgeFlag;
                                  char strPosition[128];
                                  char strTodayPosition[128];
                                  char strYdPosition[128];
                                  char strUseMargin[128];

                                  SequencialPositionField* pPosiInfo = NULL;
                                  int nItemCount = head.len/sizeof(SequencialPositionField);

                                  m_pForceCloseInfoData->LockObject();
                                  for(int i=0; i<nItemCount; i++) 
                                  {
                                      pPosiInfo = (SequencialPositionField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialPositionField));

                                      if (pPosiInfo->Position == 0)
                                      {
                                          for (i = 0; i < TodayHold_dataGridView->Rows->Count; i++)
                                          {
                                              String^ strID = TodayHold_dataGridView->Rows[i]->Cells[0]->Value->ToString();
                                              String^ strDirect = TodayHold_dataGridView->Rows[i]->Cells[1]->Value->ToString();

                                              switch(pPosiInfo->PosiDirection)
                                              {
                                              case '1':
                                                  strDirection = "净";
                                                  break;
                                              case '2':
                                                  strDirection = "买";
                                                  break;
                                              case '3':
                                                  strDirection = "卖";
                                                  break;
                                              }

                                              if (strID == Tools::string2String(pPosiInfo->InstrumentID) && strDirect == Tools::string2String(strDirection.c_str()))
                                                 TodayHold_dataGridView->Rows->RemoveAt(i);
                                          }                                          	
                                          continue;
                                      }else
                                      {
                                          TrustTradeAccount oTrustTradeAccount;
                                          m_pDataInfo->GetTrustTradeAccount(pPosiInfo->InvestorID, oTrustTradeAccount);

                                          if (oTrustTradeAccount.nTradeAccountID != Convert::ToInt32(m_strAccountID))
                                              continue;

                                          m_pForceCloseInfoData->vecPositionInfo.push_back(*pPosiInfo);
                                          m_pForceCloseInfoData->queuePositionInfo.push(*pPosiInfo);
                                        
                                      }
                                  }


                                  if (0 == TodayHold_dataGridView->RowCount)
                                  {
                                      while (!m_pForceCloseInfoData->queuePositionInfo.empty())
                                      {
                                          SequencialPositionField positionField = m_pForceCloseInfoData->queuePositionInfo.front();
                                          m_pForceCloseInfoData->queuePositionInfo.pop();
                                      
                                          switch(positionField.PosiDirection)
                                          {
                                          case '1':
                                              strDirection = "净";
                                              break;
                                          case '2':
                                              strDirection = "买";
                                              break;
                                          case '3':
                                              strDirection = "卖";
                                              break;
                                          }

                                          switch(positionField.HedgeFlag)
                                          {
                                          case '1':
                                              strHedgeFlag = "投机";
                                              break;
                                          case '2':
                                              strHedgeFlag = "套利";
                                              break;
                                          case '3':
                                              strHedgeFlag = "套保";
                                              break;
                                          }

                                          sprintf(strPosition, "%d", positionField.Position);
                                          sprintf(strTodayPosition, "%d", positionField.TodayPosition);  
                                          sprintf(strYdPosition, "%d", positionField.Position - positionField.TodayPosition);
                                          sprintf(strUseMargin, "%0.2f", positionField.UseMargin);

                                          array<String^>^row = {Tools::string2String(positionField.InstrumentID),
                                                                Tools::string2String(strDirection.c_str()),
                                                                Tools::string2String(strPosition),
                                                                Tools::string2String(strTodayPosition),
                                                                Tools::string2String(strYdPosition),
                                                                Tools::string2String(strHedgeFlag.c_str()),
                                                                Tools::string2String(strUseMargin)};

                                          this->TodayHold_dataGridView->Rows->Add(row);
                                      }
                                  }
                                  else
                                  {
                                      while (!m_pForceCloseInfoData->queuePositionInfo.empty())
                                      {
                                          SequencialPositionField positionField = m_pForceCloseInfoData->queuePositionInfo.front();
                                          m_pForceCloseInfoData->queuePositionInfo.pop();

                                          switch(positionField.PosiDirection)
                                          {
                                          case '1':
                                              strDirection = "净";
                                              break;
                                          case '2':
                                              strDirection = "买";
                                              break;
                                          case '3':
                                              strDirection = "卖";
                                              break;
                                          }

                                          switch(positionField.HedgeFlag)
                                          {
                                          case '1':
                                              strHedgeFlag = "投机";
                                              break;
                                          case '2':
                                              strHedgeFlag = "套利";
                                              break;
                                          case '3':
                                              strHedgeFlag = "套保";
                                              break;
                                          }

                                          //更新
                                          bool bFind = false;
                                          int nRowCount = this->TodayHold_dataGridView->Rows->Count;
                                          for (int i=0; i<nRowCount; i++)
                                          {  
                                              DataGridViewRow^ dataGridViewRow = TodayHold_dataGridView->Rows[i];

                                              if (Tools::string2String(positionField.InstrumentID) == dataGridViewRow->Cells[0]->Value->ToString() &&
                                                  Tools::string2String(strDirection.c_str()) == dataGridViewRow->Cells[1]->Value->ToString() &&
                                                  Tools::string2String(strHedgeFlag.c_str()) == dataGridViewRow->Cells[5]->Value->ToString())  
                                              {  
                                                  bFind = true;

                                                  sprintf(strPosition, "%d", positionField.Position);
                                                  sprintf(strTodayPosition, "%d", positionField.TodayPosition);
                                                  sprintf(strYdPosition, "%d", positionField.Position - positionField.TodayPosition);
                                                  sprintf(strUseMargin, "%0.2f", positionField.UseMargin);

                                                  dataGridViewRow->Cells[0]->Value = Tools::string2String(positionField.InstrumentID);
                                                  dataGridViewRow->Cells[1]->Value = Tools::string2String(strDirection.c_str());
                                                  dataGridViewRow->Cells[2]->Value = Tools::string2String(strPosition);
                                                  dataGridViewRow->Cells[3]->Value = Tools::string2String(strTodayPosition);
                                                  dataGridViewRow->Cells[4]->Value = Tools::string2String(strYdPosition);
                                                  dataGridViewRow->Cells[5]->Value = Tools::string2String(strHedgeFlag.c_str());
                                                  dataGridViewRow->Cells[6]->Value = Tools::string2String(strUseMargin);
                                              } 
                                          } 

                                          if(!bFind)
                                          {
                                              sprintf(strPosition, "%d", positionField.Position);
                                              sprintf(strTodayPosition, "%d", positionField.TodayPosition);  
                                              sprintf(strYdPosition, "%d", positionField.Position - positionField.TodayPosition);
                                              sprintf(strUseMargin, "%0.2f", positionField.UseMargin);

                                              array<String^>^row = {Tools::string2String(positionField.InstrumentID),
                                                                    Tools::string2String(strDirection.c_str()),
                                                                    Tools::string2String(strPosition),
                                                                    Tools::string2String(strTodayPosition),
                                                                    Tools::string2String(strYdPosition),
                                                                    Tools::string2String(strHedgeFlag.c_str()),
                                                                    Tools::string2String(strUseMargin)};

                                              this->TodayHold_dataGridView->Rows->Add(row);
                                          }
                                      }
                                  }

                                  m_pForceCloseInfoData->UnlockObject(); 
                              }
                          }
                          break; 
                      case Cmd_RM_SubscribeFund_Rsp://订阅资金返回
                          {
                              if(head.userdata1 == OPERATOR_SUCCESS) 
                              {                 
                                  char strTemp[128] = {0};

                                  RiskAllAccountField* pAllFundInfo = (RiskAllAccountField*)(p+sizeof(Stru_UniPkgHead));
                                  RiskSyncAccountField accountField = pAllFundInfo->cur;

                                  TrustTradeAccount oTrustTradeAccount;
                                  m_pDataInfo->GetTrustTradeAccount(accountField.InvestorID, oTrustTradeAccount);

                                  if (oTrustTradeAccount.nTradeAccountID != Convert::ToInt32(m_strAccountID))
                                      break;

                                  itoa(oTrustTradeAccount.nTradeAccountID,strTemp,10);
                                  this->Account_label->Text = Tools::string2String(oTrustTradeAccount.szAccount);
                                  this->Name_label->Text = Tools::string2String(oTrustTradeAccount.szName);
                                  this->IndicatorLevel_label->Text = m_strIndicatorLevel;
                                  sprintf(strTemp, "%0.2f", accountField.Balance);
                                  this->DynamicInterest_label->Text = Tools::string2String(strTemp);
                                  sprintf(strTemp, "%0.2f", accountField.Withdraw);
                                  this->DiscrepancyFund_label->Text = Tools::string2String(strTemp);
                                  sprintf(strTemp, "%0.2f", accountField.CurrMargin);
                                  this->EnsureFund_label->Text = Tools::string2String(strTemp);
                                  sprintf(strTemp, "%0.2f", accountField.FrozenMargin);
                                  this->FreezeFund_label->Text = Tools::string2String(strTemp);


                                  m_pAllFundInfo = pAllFundInfo;
                              }
                              else 
                              {
                                  p[sizeof(head)+head.len-1]=0;
                                  //TRACE(p+sizeof(head));
                              }
                          }
                          break;
                      case Cmd_RM_UnSubscribeFund_Rsp://退订资金返回
                          if(head.userdata1 == OPERATOR_SUCCESS) 
                          {
                          }
                          else 
                          {
                              p[sizeof(head)+head.len-1]=0;
                              //TRACE(p+sizeof(head));
                          }
                          break;
                      case Cmd_RM_Fund_Push://资金推送
                          {
                              if(head.userdata1 == OPERATOR_SUCCESS) 
                              {
                                  char strTemp[128] = {0};

                                  RiskSyncAccountField* accountField = NULL;
                                  int nItemCount = head.len/sizeof(RiskSyncAccountField);

                                  if(nItemCount != 0) {
                                      accountField = (RiskSyncAccountField*)(p+sizeof(Stru_UniPkgHead));

                                      TrustTradeAccount oTrustTradeAccount;
                                      m_pDataInfo->GetTrustTradeAccount(accountField->InvestorID, oTrustTradeAccount);

                                      if (oTrustTradeAccount.nTradeAccountID != Convert::ToInt32(m_strAccountID))
                                          break;

                                      sprintf(strTemp, "%0.2f", accountField->Balance);
                                      this->DynamicInterest_label->Text = Tools::string2String(strTemp);
                                      sprintf(strTemp, "%0.2f", accountField->Withdraw);
                                      this->DiscrepancyFund_label->Text = Tools::string2String(strTemp);
                                      sprintf(strTemp, "%0.2f", accountField->CurrMargin);
                                      this->EnsureFund_label->Text = Tools::string2String(strTemp);
                                      sprintf(strTemp, "%0.2f", accountField->FrozenMargin);
                                      this->FreezeFund_label->Text = Tools::string2String(strTemp);

                                      if(NULL != m_pAllFundInfo)
                                        m_pAllFundInfo->cur = *accountField;        
                                  }                                  
                              }
                          }
                          break;  
                      case Cmd_RM_SubscribeTrade_Rsp://订阅成交返回
                          if ( head.userdata1 == OPERATOR_SUCCESS )
                          {	
                              SequencialOrderField* pSequencialOrderField = NULL; 
                              int nItemCount = head.len/sizeof(SequencialOrderField);                
                              for(int i=0; i<nItemCount; i++) 
                              {
                                  pSequencialOrderField = (SequencialOrderField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialOrderField));  
                              }    
                          }
                          else
                          {
                              string str = (char*)(p+sizeof(Stru_UniPkgHead));
                              String^ strtemp = Tools::string2String(str.c_str());
                              MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);				
                          }
                          break;
                      case Cmd_RM_Trade_Push://成交推送
                          if ( head.userdata1 == OPERATOR_SUCCESS )
                          {	
                              SequencialOrderField* pSequencialOrderField = NULL; 
                              int nItemCount = head.len/sizeof(SequencialOrderField);
                              for(int i=0; i<nItemCount; i++) 
                              {
                                  pSequencialOrderField = (SequencialOrderField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(SequencialOrderField));  
                              }     
                          }
                          else
                          {
                              string str = (char*)(p+sizeof(Stru_UniPkgHead));
                              String^ strtemp = Tools::string2String(str.c_str());
                              MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);				
                          }
                          break;
                      case Cmd_RM_ForceCloseOrderInsert_Rsp://强平报单录入返回
                          if ( head.userdata1 == OPERATOR_SUCCESS )
                          {	
                              int nItemCount = head.len/sizeof(InputOrderField);
                              for(int i=0; i<nItemCount; i++) 
                              {
                                  InputOrderField* pInputOrderField = (InputOrderField*)(p+sizeof(Stru_UniPkgHead)+i*sizeof(InputOrderField));  
                              }
                          }
                          else
                          {
                              string str = (char*)(p+sizeof(Stru_UniPkgHead));
                              String^ strtemp = Tools::string2String(str.c_str());
                              MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);				
                          }
                          break;

                       }
                  }
                  Form::WndProc ( m );
              }
    private: System::Void ForceManualForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 this->PriceType_comboBox->SelectedIndex = 0;
                 this->AllEnsureFund_label->Text = "0";

                 m_pForceCloseInfoData->vecPositionInfo.clear();
                 m_pForceCloseInfoData->vecDepthMarketDataInfo.clear();

                 IntPtr hWnd=this->Handle; 
                 //订阅资金
                 ForceManualForm2::SubscribeFundRsp((int)hWnd);
                 ForceManualForm2::SubscribeFundPush((int)hWnd);
                 ForceManualForm2::UnsubscribeFundRsp((int)hWnd);
                 //订阅报单
                 ForceManualForm2::SubscribeOrderRsp((int)hWnd);
                 ForceManualForm2::SubscribeOrderPush((int)hWnd);
                 ForceManualForm2::UnsubscribeOrderRsp((int)hWnd);
                 //订阅行情
                 ForceShortcutForm2::SubscribeQuotRsp((int)hWnd);
                 ForceShortcutForm2::SubscribeQuotPush((int)hWnd);
                 ForceShortcutForm2::UnsubscribeQuotRsp((int)hWnd);
                 //订阅持仓
                 ForceManualForm2::SubscribePositionRsp((int)hWnd);
                 ForceManualForm2::SubscribePositionPush((int)hWnd);
                 ForceManualForm2::UnsubscribePositionRsp((int)hWnd);
                 //订阅成交
                 ForceManualForm2::SubscribeTradeRsp((int)hWnd);
                 ForceManualForm2::SubscribeTradePush((int)hWnd);
                 ForceManualForm2::UnsubscribeTradeRsp((int)hWnd);
                 //订阅强平
                 ForceManualForm2::SubscribeForceCloseOrderInsertRsp((int)hWnd);


                 //TInstrumentIDType InstrumentID = {0};
                 //strcpy(InstrumentID,Tools::String2string(m_strInstrumentID).c_str()); 

    
                 //TrustTradeAccount fp;
                 //m_pDataInfo->GetTrustTradeAccount(/*Tools::String2string(m_strAccountID)*/"014813",fp);

                 //订阅报单请求
                 ForceManualForm2::ReqSubscribeOrder(Convert::ToInt32(m_strAccountID));
                 //订阅深度行情请求
                 //ForceManualForm2::ReqSubscribeQuot(InstrumentID);
                 //订阅持仓请求
                 ForceManualForm2::ReqSubscribePosition(Convert::ToInt32(m_strAccountID));
                 //订阅资金请求
                 ForceManualForm2::ReqSubscribeFund(Convert::ToInt32(m_strAccountID));
                 //订阅成交请求   
                 ForceShortcutForm2::ReqSubscribeTrade(Convert::ToInt32(m_strAccountID));
             }
    private: System::Void TodayHold_dataGridView_CellMouseDown(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
                 /*
                 if (e->Button == System::Windows::Forms::MouseButtons::Right)
                 {
                     if (e->RowIndex >= 0 && e->RowIndex < this->TodayHold_dataGridView->RowCount)
                     {                         
                         //若行已是选中状态就不再进行设置
                         if (this->TodayHold_dataGridView->Rows[e->RowIndex]->Selected == false)
                         {
                             this->TodayHold_dataGridView->ClearSelection();
                             this->TodayHold_dataGridView->Rows[e->RowIndex]->Selected = true;
                         }
                         //只选中一行时设置活动单元格
                         if (this->TodayHold_dataGridView->SelectedRows->Count == 1)
                         {
                             this->TodayHold_dataGridView->CurrentCell = this->TodayHold_dataGridView->Rows[e->RowIndex]->Cells[e->ColumnIndex];
                         }
                         //弹出操作菜单
                         this->TodayHoldDataGrid_contextMenuStrip->Show(MousePosition.X, MousePosition.Y);
                     }
                 }
                 else 
                 */
                 if (e->Button == System::Windows::Forms::MouseButtons::Left)
                 {
                     if (e->RowIndex >= 0 && e->ColumnIndex >= 0 && e->RowIndex < this->TodayHold_dataGridView->RowCount)
                     {   
                        this->TodayHold_dataGridView->CurrentCell = this->TodayHold_dataGridView->Rows[e->RowIndex]->Cells[e->ColumnIndex];
                        //取出选中项的信息,将信息添加到下单窗口
                        std::string strInstrumentID = Tools::String2string(this->TodayHold_dataGridView->CurrentRow->Cells[0]->Value->ToString());
                        TInstrumentIDType InstrumentID = {0};
                        strcpy(InstrumentID,strInstrumentID.c_str()); 
                        //订阅深度行情请求
                        ForceManualForm2::ReqSubscribeQuot(InstrumentID);

                        std::string strDirect = Tools::String2string(this->TodayHold_dataGridView->CurrentRow->Cells[1]->Value->ToString());
                        if(strDirect == "买")
                            this->ForceDirect_comboBox->Text = "卖";
                        else if(strDirect == "卖")
                            this->ForceDirect_comboBox->Text = "买";

                        this->PriceType_comboBox->SelectedIndex = 0;

                        this->Contract_textBox->Text = this->TodayHold_dataGridView->CurrentRow->Cells[0]->Value->ToString();                        
                        this->ForceCount_textBox->Text = this->TodayHold_dataGridView->CurrentRow->Cells[2]->Value->ToString(); 
                        m_pForceCloseInfoData->GetPositionFieldByInstrumentIDandDirect(Tools::String2string(this->Contract_textBox->Text),
                                                                                    Tools::String2string(this->TodayHold_dataGridView->CurrentRow->Cells[1]->Value->ToString()));
                     }
                     else
                     {
                         //清空下单版数据
                         ClearData();
                     }
                 }
             }
    private: System::Void ForceTable_dataGridView_CellMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
                  if (e->Button == System::Windows::Forms::MouseButtons::Left)
                  {
                      if(e->RowIndex >= 0 && e->RowIndex < this->ForceTable_dataGridView->RowCount)
                      {
                          //取出选中项的信息,将信息添加到下单窗口
                          this->Contract_textBox->Text = this->ForceTable_dataGridView->CurrentRow->Cells[1]->Value->ToString();
                          if(this->ForceTable_dataGridView->CurrentRow->Cells[2]->Value->ToString() == "卖")
                              this->ForceDirect_comboBox->SelectedIndex = 1;
                          else if(this->ForceTable_dataGridView->CurrentRow->Cells[2]->Value->ToString() == "买")
                              this->ForceDirect_comboBox->SelectedIndex = 0;
                          

                          this->ForceCount_textBox->Text = this->ForceTable_dataGridView->CurrentRow->Cells[6]->Value->ToString();

                          if(this->ForceTable_dataGridView->CurrentRow->Cells[7]->Value->ToString() == "反向涨跌停")
                              this->PriceType_comboBox->SelectedIndex = 0;
                          else if(this->ForceTable_dataGridView->CurrentRow->Cells[7]->Value->ToString() == "最新价")
                              this->PriceType_comboBox->SelectedIndex = 1;
                          else if(this->ForceTable_dataGridView->CurrentRow->Cells[7]->Value->ToString() == "买价")
                              this->PriceType_comboBox->SelectedIndex = 2;
                          else if(this->ForceTable_dataGridView->CurrentRow->Cells[7]->Value->ToString() == "卖价")
                              this->PriceType_comboBox->SelectedIndex = 3;
                          else if(this->ForceTable_dataGridView->CurrentRow->Cells[7]->Value->ToString() == "限价")
                              this->PriceType_comboBox->SelectedIndex = 4; 
                      }else
                      {
                          //清空下单版数据
                          ClearData();
                      }
                  }
              }
    private: System::Void ForceTable_dataGridView_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
                  //统计选中的保证金
                  if (e->ColumnIndex == 0 && e->RowIndex >= 0 && e->RowIndex < this->ForceTable_dataGridView->Rows->Count)
                  {
                      //获取控件的值
                      double fEnsureFund,fEnsureFundTotal = 0.0f;                     
                      for (int i = 0; i < this->ForceTable_dataGridView->Rows->Count; i++)
                      {    
                          TInstrumentIDType InstrumentID = {0};
                          strcpy(InstrumentID,Tools::String2string(this->ForceTable_dataGridView->Rows[i]->Cells[1]->Value->ToString()).c_str()); 

                          if (this->ForceTable_dataGridView->Rows[i]->Cells[0]->EditedFormattedValue->ToString() == "True")
                          {
                              //订阅深度行情请求                              
                              ForceManualForm2::ReqSubscribeQuot(InstrumentID);

                              fEnsureFund = Convert::ToDouble(this->ForceTable_dataGridView->Rows[i]->Cells[9]->Value->ToString());
                              fEnsureFundTotal += fEnsureFund;
                          }
                          else
                          {
                                ForceManualForm2::UnreqSubscribeQuot(InstrumentID);
                          }
                      }

                      this->AllEnsureFund_label->Text = fEnsureFundTotal.ToString();
                  }             
              }
    private: System::Void PriceType_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
                 switch(this->PriceType_comboBox->SelectedIndex)
                 {
                 case 0://反向涨跌停
                     this->Spread_checkBox->Enabled = false;
                     this->Spread_textBox->Enabled = false;
                     this->CommissPrice_checkBox->Enabled = false;                 
                     this->CommissPrice_textBox->Enabled = false;
                     this->Spread_checkBox->Checked = false;
                     this->Spread_textBox->Clear();
                     this->CommissPrice_checkBox->Checked = false;                 
                     this->CommissPrice_textBox->Clear();
                     break;
                 case 1://最新价
                 case 2://买价
                 case 3://卖价
                     this->Spread_checkBox->Enabled = true;
                     this->Spread_textBox->Enabled = true;
                     this->CommissPrice_checkBox->Enabled = false;
                     this->CommissPrice_textBox->Enabled = false;
                     break;
                 case 4://限价
                     this->Spread_checkBox->Enabled = false;
                     this->Spread_textBox->Enabled = false;
                     this->CommissPrice_checkBox->Enabled = true;
                     this->CommissPrice_textBox->Enabled = true;
                     this->Spread_checkBox->Checked = false;
                     this->Spread_textBox->Clear();
                     break;
                 default:
                     break;
                 }
                }
    private: System::Void Save_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 //将下单板的数据保存或修改到左侧列表中
                 std::string strDirection;
                 std::string strHedgeFlag;
                 std::string strPirceType;
                 std::string strCloseType;
                 std::string strSpread;
                 char strPosition[128];
                 char strUseMargin[128];              

                 strDirection = Tools::String2string(this->ForceDirect_comboBox->Text);

                 switch(m_pForceCloseInfoData->curPositionField.HedgeFlag)
                 {
                 case '1':
                     strHedgeFlag = "投机";
                     break;
                 case '2':
                     strHedgeFlag = "套利";
                     break;
                 case '3':
                     strHedgeFlag = "套保";
                     break;
                 }

                 sprintf(strPosition, "%d", m_pForceCloseInfoData->curPositionField.Position);

                 DataGridViewRow^ dataGridViewRow = TodayHold_dataGridView->CurrentRow;

                 int nCount = Convert::ToInt32(this->ForceCount_textBox->Text->ToString());
                 int nAll = Convert::ToInt32(TodayHold_dataGridView->SelectedCells[2]->Value->ToString());
                 double dPrice = Convert::ToDouble(TodayHold_dataGridView->SelectedCells[6]->Value->ToString());
                 double dUseMargin = nCount*dPrice/nAll;
                 sprintf(strUseMargin, "%0.2f", dUseMargin);

                 switch(this->PriceType_comboBox->SelectedIndex)
                 {
                 case 0:
                     strPirceType = "反向涨跌停";
                     break;
                 case 1:
                     strPirceType = "最新价";
                     break;
                 case 2:
                     strPirceType = "买价";
                     break;
                 case 3:
                     strPirceType = "卖价";
                     break;
                 case 4:
                     strPirceType = "限价";
                     break;     
                 }

                 if(m_pForceCloseInfoData->curPositionField.TodayPosition > 0)
                     strCloseType = "平今";
                 else if(m_pForceCloseInfoData->curPositionField.YdPosition > 0) 
                     strCloseType = "平仓";
                 else
                     strCloseType = "强平数量不足";

                 strSpread = (0 == this->Spread_textBox->TextLength)?" ":Tools::String2string(this->Spread_textBox->Text->ToString());

                 array<String^>^row = {"false",
                                      Tools::string2String(m_pForceCloseInfoData->curPositionField.InstrumentID),
                                      Tools::string2String(strDirection.c_str()),
                                      Tools::string2String(strHedgeFlag.c_str()),
                                      Tools::string2String(strCloseType.c_str()),
                                      Tools::string2String(strPosition),
                                      this->ForceCount_textBox->Text->ToString(),
                                      Tools::string2String(strPirceType.c_str()),
                                      Tools::string2String(strSpread.c_str()),
                                      Tools::string2String(strUseMargin)};

                 if (0 == this->ForceTable_dataGridView->RowCount)
                 {
                     this->ForceTable_dataGridView->Rows->Add(row); 
                 }
                 else
                 {
                     //更新
                     bool bFind = false;
                     int nRowCount = this->ForceTable_dataGridView->Rows->Count;
                     for (int i=0; i<nRowCount; i++)
                     {  
                         DataGridViewRow^ dataGridViewRow = ForceTable_dataGridView->Rows[i];

                         if (Tools::string2String(m_pForceCloseInfoData->curPositionField.InstrumentID) == dataGridViewRow->Cells[1]->Value->ToString() &&
                             Tools::string2String(strDirection.c_str()) == dataGridViewRow->Cells[2]->Value->ToString())  
                         {  
                             bFind = true;

                             dataGridViewRow->Cells[6]->Value = this->ForceCount_textBox->Text;
                             dataGridViewRow->Cells[7]->Value = Tools::string2String(strPirceType.c_str());
                             dataGridViewRow->Cells[8]->Value = Tools::string2String(strSpread.c_str());
                             dataGridViewRow->Cells[9]->Value = Tools::string2String(strUseMargin);
                         } 
                     } 

                     if(!bFind)
                     {
                        this->ForceTable_dataGridView->Rows->Add(row); 
                     }
                 }
             }
    private: System::Void Cancel_button_Click(System::Object^  sender, System::EventArgs^  e) {
                //清空下单版数据
                ClearData();
             }
    private: System::Void Order_button_Click(System::Object^  sender, System::EventArgs^  e) 
             {
                 String^ strContent;
                 std::string strTemp;
                 strTemp = "总释放保证金:   "+ Tools::String2string(this->AllEnsureFund_label->Text)+"\n";
                 strTemp += "预算释放后动态权益:    "+ Tools::String2string(this->DynamicInterest_label->Text)+"\n";
                 strTemp += "预算释放后保证金:    "+ Tools::String2string(this->EnsureFund_label->Text);
                 strContent = Tools::string2String(strTemp.c_str());
                 if(System::Windows::Forms::DialogResult::OK == MessageBox::Show(strContent,"强平确认",MessageBoxButtons::OKCancel,MessageBoxIcon::Warning,MessageBoxDefaultButton::Button2))
                 {
                     for (int i = 0; i < this->ForceTable_dataGridView->Rows->Count;i++)
                     {
                         if (this->ForceTable_dataGridView->Rows[i]->Cells[0]->EditedFormattedValue->ToString() == "True")
                         {
                             m_nCurRowIndex = i;
                             

                             String^ strDirect;
                             if(this->ForceTable_dataGridView->Rows[i]->Cells[2]->Value->ToString() == "卖")
                                 strDirect = "买";
                             else if(this->ForceTable_dataGridView->Rows[i]->Cells[2]->Value->ToString() == "买")
                                 strDirect = "卖";


                             m_pForceCloseInfoData->GetPositionFieldByInstrumentIDandDirect(
                                                     Tools::String2string(this->ForceTable_dataGridView->Rows[i]->Cells[1]->Value->ToString()),
                                                     Tools::String2string(strDirect));

                             m_pForceCloseInfoData->GetDepthMarketDataFieldByInstrumentID(
                                                     Tools::String2string(this->ForceTable_dataGridView->Rows[i]->Cells[1]->Value->ToString()));

                             RiskForceCloseOrderField riskForceCloseOrderField;
                             memset(&riskForceCloseOrderField,0,sizeof(RiskForceCloseOrderField));
                             DataPackage(riskForceCloseOrderField);
                             ForceManualForm2::ReqForceCloseOrderInsert(riskForceCloseOrderField);

                             //将已下单从强平报单列表删除
                             this->ForceTable_dataGridView->Rows->RemoveAt(i);                             
                         }
                     }
                 }
             }
    private: System::Void Quit_button_Click(System::Object^  sender, System::EventArgs^  e) {
                 this->Close();
             }
    private: System::Void ClearData()
             {
                 this->Contract_textBox->Clear();
                 this->ForceDirect_comboBox->SelectedIndex = -1;
                 this->ForceCount_textBox->Clear();
                 this->PriceType_comboBox->SelectedIndex = -1;
                 this->Spread_checkBox->AutoCheck = false;
                 this->Spread_textBox->Clear();
                 this->CommissPrice_checkBox->AutoCheck = false;
                 this->CommissPrice_textBox->Clear();
             }
    private: System::Void DataPackage(RiskForceCloseOrderField& oRiskForceCloseOrderField)
             {
                 /************************************************************************/
                 /* 因强平业务改为普通平仓下单，故下单结构体只填充必要的项即可           */
                 /************************************************************************/
                 char szTemp[128] = {0};
                 std::string strTemp;
                 char strChar[256]={0};
                 char szDataTemp[128]={0};
                 struct tm *p;
                 time_t timeData = time(NULL);
                 p = localtime(&timeData);
                 p->tm_year = p->tm_year + 1900;
                 p->tm_mon = p->tm_mon + 1;

                 sprintf(szDataTemp,"%02d:%02d:%02d",p->tm_hour,p->tm_min,p->tm_sec);

                 ///风控强平类型
                 /*//手工强平
                 #define T_FCT_Manual '0'
                 ///单一投资者辅助强平
                 #define T_FCT_Single '1'
                 ///批量投资者辅助强平
                 #define T_FCT_Group '2'
                 */
                 oRiskForceCloseOrderField.FCType = T_FCT_Single;
                 ///辅助强平单的生成时间
                 strcpy(oRiskForceCloseOrderField.Time1,szDataTemp);
                 ///辅助强平单的生成时间（毫秒）
                 oRiskForceCloseOrderField.Millisec1 = 225;
                 ///强平单的提交时间
                 //strcpy(oRiskForceCloseOrderField.Time2,szDataTemp);
                 ///强平单的提交时间（毫秒）
                 //oRiskForceCloseOrderField.Millisec2 = (p->tm_hour*60*60*1000)+(p->tm_min*60*1000)+(p->tm_sec*1000);;
                 ///强平场景编号
                 //sprintf(szTemp,"%d",00001);
                 //strcpy(oRiskForceCloseOrderField.FCSceneId,szTemp);

                 ///输入报单结构
                 ///经纪公司代码
                 strcpy(oRiskForceCloseOrderField.BrokerID,m_pForceCloseInfoData->curPositionField.BrokerID);
                 ///投资者代码
                 strcpy(oRiskForceCloseOrderField.InvestorID,m_pForceCloseInfoData->curPositionField.InvestorID);
                 ///合约代码
                 strcpy(oRiskForceCloseOrderField.InstrumentID,m_pForceCloseInfoData->curPositionField.InstrumentID);
                 ///报单引用 服务端去解决
                 //oRiskForceCloseOrderField.OrderRef[0] = '0';
                 ///用户代码
				 strcpy(oRiskForceCloseOrderField.UserID,m_pForceCloseInfoData->curPositionField.InvestorID);
                    /*//报单价格条件
                    ///任意价
                    #define T_OPT_AnyPrice '1'
                    ///限价
                    #define T_OPT_LimitPrice '2'
                    ///最优价
                    #define T_OPT_BestPrice '3'
                    ///最新价
                    #define T_OPT_LastPrice '4'
                    ///最新价浮动上浮1个ticks
                    #define T_OPT_LastPricePlusOneTicks '5'
                    ///最新价浮动上浮2个ticks
                    #define T_OPT_LastPricePlusTwoTicks '6'
                    ///最新价浮动上浮3个ticks
                    #define T_OPT_LastPricePlusThreeTicks '7'
                    ///卖一价
                    #define T_OPT_AskPrice1 '8'
                    ///卖一价浮动上浮1个ticks
                    #define T_OPT_AskPrice1PlusOneTicks '9'
                    ///卖一价浮动上浮2个ticks
                    #define T_OPT_AskPrice1PlusTwoTicks 'A'
                    ///卖一价浮动上浮3个ticks
                    #define T_OPT_AskPrice1PlusThreeTicks 'B'
                    ///买一价
                    #define T_OPT_BidPrice1 'C'
                    ///买一价浮动上浮1个ticks
                    #define T_OPT_BidPrice1PlusOneTicks 'D'
                    ///买一价浮动上浮2个ticks
                    #define T_OPT_BidPrice1PlusTwoTicks 'E'
                    ///买一价浮动上浮3个ticks
                    #define T_OPT_BidPrice1PlusThreeTicks 'F'
                    */
                 oRiskForceCloseOrderField.OrderPriceType = T_OPT_LimitPrice;
                 ///买卖方向
                 /*
                 //买
                 #define T_D_Buy '0'
                 ///卖
                 #define T_D_Sell '1'
                 */
                 if(this->ForceTable_dataGridView->Rows[m_nCurRowIndex]->Cells[2]->Value->ToString() == "卖")
                     oRiskForceCloseOrderField.Direction = T_D_Sell;
                 else
                     oRiskForceCloseOrderField.Direction = T_D_Buy;

                 ///组合开平标志
                 /*
                 ///开仓
                 #define T_OF_Open '0'
                 ///平仓
                 #define T_OF_Close '1'
                 ///强平
                 #define T_OF_ForceClose '2'
                 ///平今
                 #define T_OF_CloseToday '3'
                 ///平昨
                 #define T_OF_CloseYesterday '4'
                 ///强减
                 #define T_OF_ForceOff '5'
                 ///本地强平
                 #define T_OF_LocalForceClose '6'
                 */
                 if(m_pForceCloseInfoData->curPositionField.TodayPosition > 0 )
                    oRiskForceCloseOrderField.CombOffsetFlag[0] = T_OF_CloseToday;
                 else if(m_pForceCloseInfoData->curPositionField.Position > 0)
                     oRiskForceCloseOrderField.CombOffsetFlag[0] = T_OF_CloseYesterday;

                 ///组合投机套保标志
				 ///投机
				//#define THOST_FTDC_BHF_Speculation '1'
				/////套利
				//#define THOST_FTDC_BHF_Arbitrage '2'
				/////套保
				//#define THOST_FTDC_BHF_Hedge '3'
                 oRiskForceCloseOrderField.CombHedgeFlag[0] = '1';
                 ///价格
                 switch(this->PriceType_comboBox->SelectedIndex)
                 {
                 case 0://涨跌停
                     {
                         switch(m_pForceCloseInfoData->curPositionField.PosiDirection)
                         {
                         case '1'://净
                             break;
                         case '2'://买 
                             sprintf(strChar, "%.2f",m_pForceCloseInfoData->curDepthMarketDataField.LowerLimitPrice);
                             break;
                         case '3'://卖
                             sprintf(strChar, "%.2f",m_pForceCloseInfoData->curDepthMarketDataField.UpperLimitPrice);
                             break;
                         }

                         break;
                     }
                 case 1://最新价
                 case 2://买价
                 case 3://卖价
                     {
                         if(Spread_checkBox->Checked)
                             sprintf(strChar, "%.2f",m_pForceCloseInfoData->curDepthMarketDataField.LastPrice + Convert::ToDouble(String::IsNullOrEmpty(Spread_textBox->Text)?"0":Spread_textBox->Text));
                         else
                             sprintf(strChar, "%.2f",m_pForceCloseInfoData->curDepthMarketDataField.LastPrice);

                         break;
                     }                   
                 case 4://限价
                     {
                         if(CommissPrice_checkBox->Checked)
                             sprintf(strChar, "%.2f",Convert::ToDouble(String::IsNullOrEmpty(CommissPrice_textBox->Text)?"0":CommissPrice_textBox->Text));
                         else
                             sprintf(strChar, "%.2f",m_pForceCloseInfoData->curDepthMarketDataField.LastPrice);
                         break;
                     }
                 }
                 oRiskForceCloseOrderField.LimitPrice = Convert::ToDouble(Tools::string2String(strChar));
                 ///数量
                 oRiskForceCloseOrderField.VolumeTotalOriginal = Convert::ToInt32(this->ForceTable_dataGridView->Rows[m_nCurRowIndex]->Cells[6]->Value->ToString());
                 ///有效期类型
                 /*
                 ///立即完成，否则撤销
                 #define T_TC_IOC '1'
                 ///本节有效
                 #define T_TC_GFS '2'
                 ///当日有效
                 #define T_TC_GFD '3'
                 ///指定日期前有效
                 #define T_TC_GTD '4'
                 ///撤销前有效
                 #define T_TC_GTC '5'
                 ///集合竞价有效
                 #define T_TC_GFA '6'
                 */
                 oRiskForceCloseOrderField.TimeCondition = T_TC_GFD;
                 ///GTD日期
                 //strcpy(oRiskForceCloseOrderField.GTDDate,szDataTemp);
                 /*
                 ///任何数量
                 #define T_VC_AV '1'
                 ///最小数量
                 #define T_VC_MV '2'
                 ///全部数量
                 #define T_VC_CV '3'
                 */
                 ///成交量类型
                 oRiskForceCloseOrderField.VolumeCondition = T_VC_AV;
                 ///最小成交量
                 oRiskForceCloseOrderField.MinVolume = 1;
                 ///触发条件
                 /*
                 ///立即
                 #define T_CC_Immediately '1'
                 ///止损
                 #define T_CC_Touch '2'
                 ///止赢
                 #define T_CC_TouchProfit '3'
                 ///预埋单
                 #define T_CC_ParkedOrder '4'
                 ///最新价大于条件价
                 #define T_CC_LastPriceGreaterThanStopPrice '5'
                 ///最新价大于等于条件价
                 #define T_CC_LastPriceGreaterEqualStopPrice '6'
                 ///最新价小于条件价
                 #define T_CC_LastPriceLesserThanStopPrice '7'
                 ///最新价小于等于条件价
                 #define T_CC_LastPriceLesserEqualStopPrice '8'
                 ///卖一价大于条件价
                 #define T_CC_AskPriceGreaterThanStopPrice '9'
                 ///卖一价大于等于条件价
                 #define T_CC_AskPriceGreaterEqualStopPrice 'A'
                 ///卖一价小于条件价
                 #define T_CC_AskPriceLesserThanStopPrice 'B'
                 ///卖一价小于等于条件价
                 #define T_CC_AskPriceLesserEqualStopPrice 'C'
                 ///买一价大于条件价
                 #define T_CC_BidPriceGreaterThanStopPrice 'D'
                 ///买一价大于等于条件价
                 #define T_CC_BidPriceGreaterEqualStopPrice 'E'
                 ///买一价小于条件价
                 #define T_CC_BidPriceLesserThanStopPrice 'F'
                 ///买一价小于等于条件价
                 #define T_CC_BidPriceLesserEqualStopPrice 'H'
                 */
                 oRiskForceCloseOrderField.ContingentCondition = T_CC_Immediately;
                 ///止损价
                 //oRiskForceCloseOrderField.StopPrice = Convert::ToDouble(m_strPrice);;
                 ///强平原因
                 /*
                 ///非强平
                 #define T_FCC_NotForceClose '0'
                 ///资金不足
                 #define T_FCC_LackDeposit '1'
                 ///客户超仓
                 #define T_FCC_ClientOverPositionLimit '2'
                 ///会员超仓
                 #define T_FCC_MemberOverPositionLimit '3'
                 ///持仓非整数倍
                 #define T_FCC_NotMultiple '4'
                 ///违规
                 #define T_FCC_Violation '5'
                 ///其它
                 #define T_FCC_Other '6'
                 ///自然人临近交割
                 #define T_FCC_PersonDeliv '7'
                 */
                 oRiskForceCloseOrderField.ForceCloseReason = T_FCC_NotForceClose;
                 ///自动挂起标志
                 //oRiskForceCloseOrderField.IsAutoSuspend;
                 ///业务单元
                 //oRiskForceCloseOrderField.BusinessUnit;
                 ///请求编号
                 //oRiskForceCloseOrderField.RequestID;
                 ///用户强评标志
                 //oRiskForceCloseOrderField.UserForceClose = true;
                 ///前置编号
                 //oRiskForceCloseOrderField.FrontID;
                 ///会话编号
                 //oRiskForceCloseOrderField.SessionID;
             }
//数据有效性约束
private: System::Void ForceCount_textBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             if(e->KeyChar=='.' && ForceCount_textBox->Text->IndexOf(".")!=-1)  
             {  
                 e->Handled=true;  
             }  

             if(!((e->KeyChar>=48 && e->KeyChar<=57) || e->KeyChar=='.' || e->KeyChar==8))  
             {  
                 e->Handled=true;  
             } 
         }
private: System::Void Spread_textBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             if(e->KeyChar=='.' && Spread_textBox->Text->IndexOf(".")!=-1)  
             {  
                 e->Handled=true;  
             }  

             if(!((e->KeyChar>=48 && e->KeyChar<=57) || e->KeyChar=='.' || e->KeyChar==8))  
             {  
                 e->Handled=true;  
             } 
         }
private: System::Void CommissPrice_textBox_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
             if(e->KeyChar=='.' && CommissPrice_textBox->Text->IndexOf(".")!=-1)  
             {  
                 e->Handled=true;  
             }  

             if(!((e->KeyChar>=48 && e->KeyChar<=57) || e->KeyChar=='.' || e->KeyChar==8))  
             {  
                 e->Handled=true;  
             } 
         }
private: System::Void ForceManualForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
             IntPtr hWnd=this->Handle;
             ForceManualForm2::UnsubscribeAllEvt((int)hWnd);
         }
};
}
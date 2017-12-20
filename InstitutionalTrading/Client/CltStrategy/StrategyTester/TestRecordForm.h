#pragma once

#include "..\CommonClasses\DataEntity.h"
#include "HistoryQueryForm.h"


using namespace System;
using namespace System::Diagnostics;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Windows::Forms::DataVisualization::Charting;


namespace StrategyTester {

    enum MSCHART_TYPE{MSCHART_TYPE_PROFITLOSS,MSCHART_TYPE_REPORT_MONTH,MSCHART_TYPE_REPORT_YEAR,MSCHART_TYPE_PROFIT,MSCHART_TYPE_INDICATOR};

	/// <summary>
	/// Summary for TestRecordForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class TestRecordForm : public System::Windows::Forms::Form
	{
	public:
		TestRecordForm(bool bHistory)
		{
			InitializeComponent();
            chart_ProfitLoss = nullptr;
            chart_ReportMonth = nullptr;
            chart_ReportYear = nullptr;
            chart_Profit = nullptr;
            chart_Indicator = nullptr;
            chart_Show = chart_ProfitLoss;
            m_bHistory = bHistory;
            m_itemRoot = nullptr;
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TestRecordForm()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::SplitContainer^  splitContainer_Main;
    protected: 

    protected: 
    private: System::Windows::Forms::TabControl^  tabControl1;
    private: System::Windows::Forms::TabPage^  tabPage1;
    private: System::Windows::Forms::TabPage^  tabPage2;
    private: System::Windows::Forms::TabPage^  tabPage3;
    private: System::Windows::Forms::TabPage^  tabPage4;
    private: System::Windows::Forms::TabPage^  tabPage5;
    private: System::Windows::Forms::TreeListView^  treeListView1;
    private: System::Windows::Forms::ColumnHeader^  columnHeader1;
    private: System::Windows::Forms::ColumnHeader^  columnHeader2;
    private: System::Windows::Forms::ColumnHeader^  columnHeader3;
    private: System::Windows::Forms::SplitContainer^  splitContainer_Indicator;

    private: System::Windows::Forms::GroupBox^  groupBox_Indicator;
    private: System::Windows::Forms::GroupBox^  groupBox_IndicatorDesp;
    private: System::Windows::Forms::ListView^  listView_Indicator;



    private: System::Windows::Forms::ColumnHeader^  columnHeader4;
    private: System::Windows::Forms::ColumnHeader^  columnHeader5;
    private: System::Windows::Forms::ColumnHeader^  columnHeader6;
    private: System::Windows::Forms::ColumnHeader^  columnHeader7;
    private: System::Windows::Forms::ColumnHeader^  columnHeader8;
    private: System::Windows::Forms::ColumnHeader^  columnHeader9;
    private: System::Windows::Forms::ColumnHeader^  columnHeader10;
    private: System::Windows::Forms::GroupBox^  groupBox3;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::ListView^  listView_StrategyInfo;
    private: System::Windows::Forms::ColumnHeader^  columnHeader11;
    private: System::Windows::Forms::ColumnHeader^  columnHeader12;
    private: System::Windows::Forms::ColumnHeader^  columnHeader13;
    private: System::Windows::Forms::GroupBox^  groupBox_DelegateDetail;
    private: System::Windows::Forms::ListView^  listView_delegateDetail;
    private: System::Windows::Forms::ColumnHeader^  columnHeader14;
    private: System::Windows::Forms::ColumnHeader^  columnHeader15;
    private: System::Windows::Forms::ColumnHeader^  columnHeader16;
    private: System::Windows::Forms::ColumnHeader^  columnHeader17;
    private: System::Windows::Forms::ColumnHeader^  columnHeader18;
    private: System::Windows::Forms::ColumnHeader^  columnHeader19;
    private: System::Windows::Forms::ColumnHeader^  columnHeader20;
    private: System::Windows::Forms::ColumnHeader^  columnHeader21;
    private: System::Windows::Forms::ColumnHeader^  columnHeader22;
    private: System::Windows::Forms::ColumnHeader^  columnHeader23;
    private: System::Windows::Forms::ColumnHeader^  columnHeader24;
    private: System::Windows::Forms::ColumnHeader^  columnHeader25;
    private: System::Windows::Forms::ColumnHeader^  columnHeader26;
    private: System::Windows::Forms::ColumnHeader^  columnHeader27;
    private: System::Windows::Forms::ColumnHeader^  columnHeader28;
    private: System::Windows::Forms::ColumnHeader^  columnHeader29;
    private: System::Windows::Forms::ColumnHeader^  columnHeader30;
    private: System::Windows::Forms::ColumnHeader^  columnHeader31;
    private: System::Windows::Forms::ColumnHeader^  columnHeader32;
    private: System::Windows::Forms::ColumnHeader^  columnHeader33;
    private: System::Windows::Forms::ColumnHeader^  columnHeader34;
    private: System::Windows::Forms::ColumnHeader^  columnHeader35;
    private: System::Windows::Forms::ColumnHeader^  columnHeader36;
    private: System::Windows::Forms::GroupBox^  groupBox_TradeDetail;
    private: System::Windows::Forms::ListView^  listView_tradeDetail;
    private: System::Windows::Forms::ColumnHeader^  columnHeader37;
    private: System::Windows::Forms::ColumnHeader^  columnHeader38;
    private: System::Windows::Forms::ColumnHeader^  columnHeader39;
    private: System::Windows::Forms::ColumnHeader^  columnHeader40;
    private: System::Windows::Forms::ColumnHeader^  columnHeader41;
    private: System::Windows::Forms::ColumnHeader^  columnHeader42;
    private: System::Windows::Forms::ColumnHeader^  columnHeader43;
    private: System::Windows::Forms::ColumnHeader^  columnHeader44;
    private: System::Windows::Forms::ColumnHeader^  columnHeader45;
    private: System::Windows::Forms::ColumnHeader^  columnHeader46;
    private: System::Windows::Forms::ColumnHeader^  columnHeader47;
    private: System::Windows::Forms::ColumnHeader^  columnHeader48;
    private: System::Windows::Forms::ColumnHeader^  columnHeader49;
    private: System::Windows::Forms::SplitContainer^  splitContainer_ChartSummary;
    private: System::Windows::Forms::ListBox^  listBox1;
    private: System::Windows::Forms::SplitContainer^  splitContainer_ChartSummary2;
    private: System::Windows::Forms::ListView^  listView_chart;
    private: System::Windows::Forms::ColumnHeader^  columnHeader50;
    private: System::Windows::Forms::ColumnHeader^  columnHeader51;
    private: System::Windows::Forms::ColumnHeader^  columnHeader52;
    private: System::Windows::Forms::ColumnHeader^  columnHeader53;
    private: System::Windows::Forms::ColumnHeader^  columnHeader54;
    private: System::Windows::Forms::ColumnHeader^  columnHeader55;
    private: System::Windows::Forms::ColumnHeader^  columnHeader56;
    private: System::Windows::Forms::ColumnHeader^  columnHeader57;
    private: System::Windows::Forms::ColumnHeader^  columnHeader58;
    private: System::Windows::Forms::ColumnHeader^  columnHeader59;
    private: System::Windows::Forms::ColumnHeader^  columnHeader60;
    private: System::Windows::Forms::ColumnHeader^  columnHeader61;
    private: System::Windows::Forms::ColumnHeader^  columnHeader62;
    private: System::Windows::Forms::ColumnHeader^  columnHeader63;
    private: System::Windows::Forms::ColumnHeader^  columnHeader64;
private: System::Windows::Forms::ImageList^  imageList1;






	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
        private: System::ComponentModel::IContainer^  components;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->components = (gcnew System::ComponentModel::Container());
            System::Windows::Forms::ListViewItem^  listViewItem1 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(8) {L"1", 
                L"净利润", L"0", L"平均盈利", L"0", L"总盈利", L"0", L""}, -1));
            System::Windows::Forms::ListViewItem^  listViewItem2 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(7) {L"2", 
                L"最大亏损", L"0", L"总亏损", L"0", L"平均亏损", L"0"}, -1));
            System::Windows::Forms::ListViewItem^  listViewItem3 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(7) {L"3", 
                L"初始权益", L"0", L"总盈利/总亏损(%)", L"0", L"最小权益", L"0"}, -1));
            System::Windows::Forms::ListViewItem^  listViewItem4 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(7) {L"4", 
                L"毛利润", L"0", L"净利润/最大盈利(%)", L"0", L"最大权益", L"0"}, -1));
            System::Windows::Forms::ListViewItem^  listViewItem5 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(7) {L"5", 
                L"平均利润", L"0", L"净利润/最大亏损(%)", L"0", L"最大使用资金", L"0"}, -1));
            System::Windows::Forms::ListViewItem^  listViewItem6 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(7) {L"6", 
                L"手续费/毛利润(%)", L"0", L"最大盈利/最大亏损(%)", L"0", L"结束总权益", L"0"}, -1));
            System::Windows::Forms::ListViewItem^  listViewItem7 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(7) {L"7", 
                L"未成交手数", L"0", L"胜率(%)", L"0", L"盈利次数", L"0"}, -1));
            System::Windows::Forms::ListViewItem^  listViewItem8 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(7) {L"8", 
                L"最大盈利/总盈利(%)", L"0", L"收益率(%)", L"0", L"亏损次数", L"0"}, -1));
            System::Windows::Forms::ListViewItem^  listViewItem9 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(7) {L"9", 
                L"最大亏损/总亏损(%)", L"0", L"有效收益率(%)", L"0", L"年度收益率(%)", L"0"}, -1));
            System::Windows::Forms::ListViewItem^  listViewItem10 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(7) {L"10", 
                L"盈利次数/总交易次数(%)", L"0", L"月度平均盈利", L"0", L"亏损次数/盈利次数(%)", L"0"}, -1));
            System::Windows::Forms::ListViewItem^  listViewItem11 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(7) {L"11", 
                L"年化收益", L"0", L"亏损次数/总交易次数(%)", L"0", L"最大连续盈利次数", L"0"}, -1));
            System::Windows::Forms::ListViewItem^  listViewItem12 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(7) {L"12", 
                L"最大连续亏损次数", L"0", L"总交易次数", L"0", L"最大持仓", L"0"}, -1));
            System::Windows::Forms::ListViewItem^  listViewItem13 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(7) {L"13", 
                L"置信度", L"0", L"总交易委托数", L"0", L"最大盈利", L"0"}, -1));
            System::Windows::Forms::ListViewItem^  listViewItem14 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(7) {L"14", 
                L"策略开始时间", L"2014/06/06 09:32:11", L"策略停止时间", L"2014/06/06 13:33:12"}, -1));
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(TestRecordForm::typeid));
            this->splitContainer_Main = (gcnew System::Windows::Forms::SplitContainer());
            this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
            this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
            this->splitContainer_Indicator = (gcnew System::Windows::Forms::SplitContainer());
            this->groupBox_Indicator = (gcnew System::Windows::Forms::GroupBox());
            this->listView_Indicator = (gcnew System::Windows::Forms::ListView());
            this->columnHeader4 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader5 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader6 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader7 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader8 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader9 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader10 = (gcnew System::Windows::Forms::ColumnHeader());
            this->groupBox_IndicatorDesp = (gcnew System::Windows::Forms::GroupBox());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
            this->splitContainer_ChartSummary = (gcnew System::Windows::Forms::SplitContainer());
            this->listBox1 = (gcnew System::Windows::Forms::ListBox());
            this->splitContainer_ChartSummary2 = (gcnew System::Windows::Forms::SplitContainer());
            this->listView_chart = (gcnew System::Windows::Forms::ListView());
            this->columnHeader50 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader51 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader52 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader53 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader54 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader55 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader56 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader57 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader58 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader59 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader60 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader61 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader62 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader63 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader64 = (gcnew System::Windows::Forms::ColumnHeader());
            this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
            this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
            this->listView_StrategyInfo = (gcnew System::Windows::Forms::ListView());
            this->columnHeader11 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader12 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader13 = (gcnew System::Windows::Forms::ColumnHeader());
            this->tabPage4 = (gcnew System::Windows::Forms::TabPage());
            this->groupBox_DelegateDetail = (gcnew System::Windows::Forms::GroupBox());
            this->listView_delegateDetail = (gcnew System::Windows::Forms::ListView());
            this->columnHeader14 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader15 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader16 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader17 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader18 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader19 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader20 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader21 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader22 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader23 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader24 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader25 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader26 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader27 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader28 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader29 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader30 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader31 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader32 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader33 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader34 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader35 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader36 = (gcnew System::Windows::Forms::ColumnHeader());
            this->tabPage5 = (gcnew System::Windows::Forms::TabPage());
            this->groupBox_TradeDetail = (gcnew System::Windows::Forms::GroupBox());
            this->listView_tradeDetail = (gcnew System::Windows::Forms::ListView());
            this->columnHeader37 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader38 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader39 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader40 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader41 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader42 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader43 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader44 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader45 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader46 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader47 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader48 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader49 = (gcnew System::Windows::Forms::ColumnHeader());
            this->treeListView1 = (gcnew System::Windows::Forms::TreeListView());
            this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader3 = (gcnew System::Windows::Forms::ColumnHeader());
            this->imageList1 = (gcnew System::Windows::Forms::ImageList(this->components));
            this->splitContainer_Main->Panel1->SuspendLayout();
            this->splitContainer_Main->Panel2->SuspendLayout();
            this->splitContainer_Main->SuspendLayout();
            this->tabControl1->SuspendLayout();
            this->tabPage1->SuspendLayout();
            this->splitContainer_Indicator->Panel1->SuspendLayout();
            this->splitContainer_Indicator->Panel2->SuspendLayout();
            this->splitContainer_Indicator->SuspendLayout();
            this->groupBox_Indicator->SuspendLayout();
            this->groupBox_IndicatorDesp->SuspendLayout();
            this->tabPage2->SuspendLayout();
            this->splitContainer_ChartSummary->Panel1->SuspendLayout();
            this->splitContainer_ChartSummary->Panel2->SuspendLayout();
            this->splitContainer_ChartSummary->SuspendLayout();
            this->splitContainer_ChartSummary2->Panel2->SuspendLayout();
            this->splitContainer_ChartSummary2->SuspendLayout();
            this->tabPage3->SuspendLayout();
            this->groupBox3->SuspendLayout();
            this->tabPage4->SuspendLayout();
            this->groupBox_DelegateDetail->SuspendLayout();
            this->tabPage5->SuspendLayout();
            this->groupBox_TradeDetail->SuspendLayout();
            this->SuspendLayout();
            // 
            // splitContainer_Main
            // 
            this->splitContainer_Main->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer_Main->Location = System::Drawing::Point(0, 0);
            this->splitContainer_Main->Name = L"splitContainer_Main";
            this->splitContainer_Main->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer_Main.Panel1
            // 
            this->splitContainer_Main->Panel1->Controls->Add(this->tabControl1);
            // 
            // splitContainer_Main.Panel2
            // 
            this->splitContainer_Main->Panel2->Controls->Add(this->treeListView1);
            this->splitContainer_Main->Size = System::Drawing::Size(1331, 657);
            this->splitContainer_Main->SplitterDistance = 516;
            this->splitContainer_Main->TabIndex = 0;
            // 
            // tabControl1
            // 
            this->tabControl1->Alignment = System::Windows::Forms::TabAlignment::Bottom;
            this->tabControl1->Controls->Add(this->tabPage1);
            this->tabControl1->Controls->Add(this->tabPage2);
            this->tabControl1->Controls->Add(this->tabPage3);
            this->tabControl1->Controls->Add(this->tabPage4);
            this->tabControl1->Controls->Add(this->tabPage5);
            this->tabControl1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->tabControl1->Location = System::Drawing::Point(0, 0);
            this->tabControl1->Name = L"tabControl1";
            this->tabControl1->SelectedIndex = 0;
            this->tabControl1->Size = System::Drawing::Size(1331, 516);
            this->tabControl1->TabIndex = 0;
            // 
            // tabPage1
            // 
            this->tabPage1->Controls->Add(this->splitContainer_Indicator);
            this->tabPage1->Location = System::Drawing::Point(4, 4);
            this->tabPage1->Name = L"tabPage1";
            this->tabPage1->Padding = System::Windows::Forms::Padding(3);
            this->tabPage1->Size = System::Drawing::Size(1323, 491);
            this->tabPage1->TabIndex = 0;
            this->tabPage1->Text = L"指标概要";
            this->tabPage1->UseVisualStyleBackColor = true;
            // 
            // splitContainer_Indicator
            // 
            this->splitContainer_Indicator->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer_Indicator->IsSplitterFixed = true;
            this->splitContainer_Indicator->Location = System::Drawing::Point(3, 3);
            this->splitContainer_Indicator->Name = L"splitContainer_Indicator";
            this->splitContainer_Indicator->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer_Indicator.Panel1
            // 
            this->splitContainer_Indicator->Panel1->Controls->Add(this->groupBox_Indicator);
            // 
            // splitContainer_Indicator.Panel2
            // 
            this->splitContainer_Indicator->Panel2->Controls->Add(this->groupBox_IndicatorDesp);
            this->splitContainer_Indicator->Size = System::Drawing::Size(1317, 485);
            this->splitContainer_Indicator->SplitterDistance = 392;
            this->splitContainer_Indicator->TabIndex = 0;
            // 
            // groupBox_Indicator
            // 
            this->groupBox_Indicator->Controls->Add(this->listView_Indicator);
            this->groupBox_Indicator->Dock = System::Windows::Forms::DockStyle::Fill;
            this->groupBox_Indicator->Location = System::Drawing::Point(0, 0);
            this->groupBox_Indicator->Name = L"groupBox_Indicator";
            this->groupBox_Indicator->Size = System::Drawing::Size(1317, 392);
            this->groupBox_Indicator->TabIndex = 0;
            this->groupBox_Indicator->TabStop = false;
            this->groupBox_Indicator->Text = L"详细指标";
            // 
            // listView_Indicator
            // 
            this->listView_Indicator->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(7) {this->columnHeader4, 
                this->columnHeader5, this->columnHeader6, this->columnHeader7, this->columnHeader8, this->columnHeader9, this->columnHeader10});
            this->listView_Indicator->Dock = System::Windows::Forms::DockStyle::Fill;
            this->listView_Indicator->FullRowSelect = true;
            this->listView_Indicator->GridLines = true;
            this->listView_Indicator->HideSelection = false;
            this->listView_Indicator->Items->AddRange(gcnew cli::array< System::Windows::Forms::ListViewItem^  >(14) {listViewItem1, listViewItem2, 
                listViewItem3, listViewItem4, listViewItem5, listViewItem6, listViewItem7, listViewItem8, listViewItem9, listViewItem10, listViewItem11, 
                listViewItem12, listViewItem13, listViewItem14});
            this->listView_Indicator->Location = System::Drawing::Point(3, 17);
            this->listView_Indicator->Name = L"listView_Indicator";
            this->listView_Indicator->Size = System::Drawing::Size(1311, 372);
            this->listView_Indicator->TabIndex = 0;
            this->listView_Indicator->UseCompatibleStateImageBehavior = false;
            this->listView_Indicator->View = System::Windows::Forms::View::Details;
            this->listView_Indicator->SelectedIndexChanged += gcnew System::EventHandler(this, &TestRecordForm::listView_Indicator_SelectedIndexChanged);
            // 
            // columnHeader4
            // 
            this->columnHeader4->Text = L"序号";
            // 
            // columnHeader5
            // 
            this->columnHeader5->Text = L"指标名称";
            this->columnHeader5->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader5->Width = 160;
            // 
            // columnHeader6
            // 
            this->columnHeader6->Text = L"指标值";
            this->columnHeader6->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader6->Width = 160;
            // 
            // columnHeader7
            // 
            this->columnHeader7->Text = L"指标名称";
            this->columnHeader7->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader7->Width = 160;
            // 
            // columnHeader8
            // 
            this->columnHeader8->Text = L"指标值";
            this->columnHeader8->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader8->Width = 160;
            // 
            // columnHeader9
            // 
            this->columnHeader9->Text = L"指标名称";
            this->columnHeader9->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader9->Width = 160;
            // 
            // columnHeader10
            // 
            this->columnHeader10->Text = L"指标值";
            this->columnHeader10->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader10->Width = 120;
            // 
            // groupBox_IndicatorDesp
            // 
            this->groupBox_IndicatorDesp->Controls->Add(this->label3);
            this->groupBox_IndicatorDesp->Controls->Add(this->label2);
            this->groupBox_IndicatorDesp->Controls->Add(this->label1);
            this->groupBox_IndicatorDesp->Dock = System::Windows::Forms::DockStyle::Fill;
            this->groupBox_IndicatorDesp->Location = System::Drawing::Point(0, 0);
            this->groupBox_IndicatorDesp->Name = L"groupBox_IndicatorDesp";
            this->groupBox_IndicatorDesp->Size = System::Drawing::Size(1317, 89);
            this->groupBox_IndicatorDesp->TabIndex = 0;
            this->groupBox_IndicatorDesp->TabStop = false;
            this->groupBox_IndicatorDesp->Text = L"指标描述";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(29, 63);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(0, 12);
            this->label3->TabIndex = 2;
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(29, 45);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(0, 12);
            this->label2->TabIndex = 1;
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(29, 27);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(0, 12);
            this->label1->TabIndex = 0;
            // 
            // tabPage2
            // 
            this->tabPage2->Controls->Add(this->splitContainer_ChartSummary);
            this->tabPage2->Location = System::Drawing::Point(4, 4);
            this->tabPage2->Name = L"tabPage2";
            this->tabPage2->Padding = System::Windows::Forms::Padding(3);
            this->tabPage2->Size = System::Drawing::Size(1323, 491);
            this->tabPage2->TabIndex = 1;
            this->tabPage2->Text = L"图表概要";
            this->tabPage2->UseVisualStyleBackColor = true;
            // 
            // splitContainer_ChartSummary
            // 
            this->splitContainer_ChartSummary->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer_ChartSummary->FixedPanel = System::Windows::Forms::FixedPanel::Panel1;
            this->splitContainer_ChartSummary->IsSplitterFixed = true;
            this->splitContainer_ChartSummary->Location = System::Drawing::Point(3, 3);
            this->splitContainer_ChartSummary->Name = L"splitContainer_ChartSummary";
            // 
            // splitContainer_ChartSummary.Panel1
            // 
            this->splitContainer_ChartSummary->Panel1->Controls->Add(this->listBox1);
            // 
            // splitContainer_ChartSummary.Panel2
            // 
            this->splitContainer_ChartSummary->Panel2->Controls->Add(this->splitContainer_ChartSummary2);
            this->splitContainer_ChartSummary->Size = System::Drawing::Size(1317, 485);
            this->splitContainer_ChartSummary->SplitterDistance = 162;
            this->splitContainer_ChartSummary->TabIndex = 0;
            // 
            // listBox1
            // 
            this->listBox1->Dock = System::Windows::Forms::DockStyle::Fill;
            this->listBox1->FormattingEnabled = true;
            this->listBox1->ItemHeight = 12;
            this->listBox1->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"累计盈亏图", L"月度报表", L"年度报表", L"净利润图", L"指标图"});
            this->listBox1->Location = System::Drawing::Point(0, 0);
            this->listBox1->Name = L"listBox1";
            this->listBox1->Size = System::Drawing::Size(162, 484);
            this->listBox1->TabIndex = 0;
            this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &TestRecordForm::listBox1_SelectedIndexChanged);
            // 
            // splitContainer_ChartSummary2
            // 
            this->splitContainer_ChartSummary2->Dock = System::Windows::Forms::DockStyle::Fill;
            this->splitContainer_ChartSummary2->IsSplitterFixed = true;
            this->splitContainer_ChartSummary2->Location = System::Drawing::Point(0, 0);
            this->splitContainer_ChartSummary2->Name = L"splitContainer_ChartSummary2";
            this->splitContainer_ChartSummary2->Orientation = System::Windows::Forms::Orientation::Horizontal;
            // 
            // splitContainer_ChartSummary2.Panel1
            // 
            this->splitContainer_ChartSummary2->Panel1->SizeChanged += gcnew System::EventHandler(this, &TestRecordForm::splitContainer_ChartSummary2_Panel1_SizeChanged);
            // 
            // splitContainer_ChartSummary2.Panel2
            // 
            this->splitContainer_ChartSummary2->Panel2->Controls->Add(this->listView_chart);
            this->splitContainer_ChartSummary2->Size = System::Drawing::Size(1151, 485);
            this->splitContainer_ChartSummary2->SplitterDistance = 373;
            this->splitContainer_ChartSummary2->TabIndex = 0;
            // 
            // listView_chart
            // 
            this->listView_chart->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(15) {this->columnHeader50, this->columnHeader51, 
                this->columnHeader52, this->columnHeader53, this->columnHeader54, this->columnHeader55, this->columnHeader56, this->columnHeader57, 
                this->columnHeader58, this->columnHeader59, this->columnHeader60, this->columnHeader61, this->columnHeader62, this->columnHeader63, 
                this->columnHeader64});
            this->listView_chart->Dock = System::Windows::Forms::DockStyle::Fill;
            this->listView_chart->FullRowSelect = true;
            this->listView_chart->GridLines = true;
            this->listView_chart->Location = System::Drawing::Point(0, 0);
            this->listView_chart->Name = L"listView_chart";
            this->listView_chart->Size = System::Drawing::Size(1151, 108);
            this->listView_chart->TabIndex = 0;
            this->listView_chart->UseCompatibleStateImageBehavior = false;
            this->listView_chart->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader50
            // 
            this->columnHeader50->Text = L"序号";
            // 
            // columnHeader51
            // 
            this->columnHeader51->Text = L"合约";
            this->columnHeader51->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader52
            // 
            this->columnHeader52->Text = L"买卖";
            this->columnHeader52->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader53
            // 
            this->columnHeader53->Text = L"开平";
            this->columnHeader53->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader54
            // 
            this->columnHeader54->Text = L"盈亏";
            this->columnHeader54->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader55
            // 
            this->columnHeader55->Text = L"总盈亏";
            this->columnHeader55->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader56
            // 
            this->columnHeader56->Text = L"账号";
            this->columnHeader56->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader56->Width = 80;
            // 
            // columnHeader57
            // 
            this->columnHeader57->Text = L"成交编号";
            this->columnHeader57->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader57->Width = 80;
            // 
            // columnHeader58
            // 
            this->columnHeader58->Text = L"成交价格";
            this->columnHeader58->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader58->Width = 80;
            // 
            // columnHeader59
            // 
            this->columnHeader59->Text = L"成交手数";
            this->columnHeader59->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader59->Width = 80;
            // 
            // columnHeader60
            // 
            this->columnHeader60->Text = L"成交时间";
            this->columnHeader60->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader60->Width = 100;
            // 
            // columnHeader61
            // 
            this->columnHeader61->Text = L"成交行情时间";
            this->columnHeader61->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader61->Width = 100;
            // 
            // columnHeader62
            // 
            this->columnHeader62->Text = L"报单编号";
            this->columnHeader62->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader62->Width = 80;
            // 
            // columnHeader63
            // 
            this->columnHeader63->Text = L"投保";
            this->columnHeader63->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader64
            // 
            this->columnHeader64->Text = L"交易所";
            this->columnHeader64->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader64->Width = 80;
            // 
            // tabPage3
            // 
            this->tabPage3->Controls->Add(this->groupBox3);
            this->tabPage3->Location = System::Drawing::Point(4, 4);
            this->tabPage3->Name = L"tabPage3";
            this->tabPage3->Size = System::Drawing::Size(1323, 491);
            this->tabPage3->TabIndex = 2;
            this->tabPage3->Text = L"策略基本信息";
            this->tabPage3->UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this->groupBox3->Controls->Add(this->listView_StrategyInfo);
            this->groupBox3->Dock = System::Windows::Forms::DockStyle::Fill;
            this->groupBox3->Location = System::Drawing::Point(0, 0);
            this->groupBox3->Name = L"groupBox3";
            this->groupBox3->Size = System::Drawing::Size(1323, 491);
            this->groupBox3->TabIndex = 0;
            this->groupBox3->TabStop = false;
            this->groupBox3->Text = L"策略基本信息";
            // 
            // listView_StrategyInfo
            // 
            this->listView_StrategyInfo->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->columnHeader11, this->columnHeader12, 
                this->columnHeader13});
            this->listView_StrategyInfo->Dock = System::Windows::Forms::DockStyle::Fill;
            this->listView_StrategyInfo->FullRowSelect = true;
            this->listView_StrategyInfo->GridLines = true;
            this->listView_StrategyInfo->Location = System::Drawing::Point(3, 17);
            this->listView_StrategyInfo->Name = L"listView_StrategyInfo";
            this->listView_StrategyInfo->Size = System::Drawing::Size(1317, 471);
            this->listView_StrategyInfo->TabIndex = 0;
            this->listView_StrategyInfo->UseCompatibleStateImageBehavior = false;
            this->listView_StrategyInfo->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader11
            // 
            this->columnHeader11->Text = L"序号";
            // 
            // columnHeader12
            // 
            this->columnHeader12->Text = L"参数名称";
            this->columnHeader12->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader12->Width = 160;
            // 
            // columnHeader13
            // 
            this->columnHeader13->Text = L"参数内容";
            this->columnHeader13->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader13->Width = 160;
            // 
            // tabPage4
            // 
            this->tabPage4->Controls->Add(this->groupBox_DelegateDetail);
            this->tabPage4->Location = System::Drawing::Point(4, 4);
            this->tabPage4->Name = L"tabPage4";
            this->tabPage4->Size = System::Drawing::Size(1323, 491);
            this->tabPage4->TabIndex = 3;
            this->tabPage4->Text = L"委托明细";
            this->tabPage4->UseVisualStyleBackColor = true;
            // 
            // groupBox_DelegateDetail
            // 
            this->groupBox_DelegateDetail->Controls->Add(this->listView_delegateDetail);
            this->groupBox_DelegateDetail->Dock = System::Windows::Forms::DockStyle::Fill;
            this->groupBox_DelegateDetail->Location = System::Drawing::Point(0, 0);
            this->groupBox_DelegateDetail->Name = L"groupBox_DelegateDetail";
            this->groupBox_DelegateDetail->Size = System::Drawing::Size(1323, 491);
            this->groupBox_DelegateDetail->TabIndex = 0;
            this->groupBox_DelegateDetail->TabStop = false;
            this->groupBox_DelegateDetail->Text = L"委托明细";
            // 
            // listView_delegateDetail
            // 
            this->listView_delegateDetail->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(23) {this->columnHeader14, this->columnHeader15, 
                this->columnHeader16, this->columnHeader17, this->columnHeader18, this->columnHeader19, this->columnHeader20, this->columnHeader21, 
                this->columnHeader22, this->columnHeader23, this->columnHeader24, this->columnHeader25, this->columnHeader26, this->columnHeader27, 
                this->columnHeader28, this->columnHeader29, this->columnHeader30, this->columnHeader31, this->columnHeader32, this->columnHeader33, 
                this->columnHeader34, this->columnHeader35, this->columnHeader36});
            this->listView_delegateDetail->Dock = System::Windows::Forms::DockStyle::Fill;
            this->listView_delegateDetail->FullRowSelect = true;
            this->listView_delegateDetail->GridLines = true;
            this->listView_delegateDetail->Location = System::Drawing::Point(3, 17);
            this->listView_delegateDetail->Name = L"listView_delegateDetail";
            this->listView_delegateDetail->Size = System::Drawing::Size(1317, 471);
            this->listView_delegateDetail->TabIndex = 0;
            this->listView_delegateDetail->UseCompatibleStateImageBehavior = false;
            this->listView_delegateDetail->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader14
            // 
            this->columnHeader14->Text = L"序号";
            // 
            // columnHeader15
            // 
            this->columnHeader15->Text = L"账号";
            this->columnHeader15->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader16
            // 
            this->columnHeader16->Text = L"报单编号";
            this->columnHeader16->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader16->Width = 80;
            // 
            // columnHeader17
            // 
            this->columnHeader17->Text = L"合约";
            this->columnHeader17->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader18
            // 
            this->columnHeader18->Text = L"买卖";
            this->columnHeader18->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader19
            // 
            this->columnHeader19->Text = L"开平";
            this->columnHeader19->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader20
            // 
            this->columnHeader20->Text = L"最终状态";
            this->columnHeader20->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader20->Width = 100;
            // 
            // columnHeader21
            // 
            this->columnHeader21->Text = L"报单价格";
            this->columnHeader21->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader21->Width = 80;
            // 
            // columnHeader22
            // 
            this->columnHeader22->Text = L"报单手数";
            this->columnHeader22->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader22->Width = 80;
            // 
            // columnHeader23
            // 
            this->columnHeader23->Text = L"成交手数";
            this->columnHeader23->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader23->Width = 80;
            // 
            // columnHeader24
            // 
            this->columnHeader24->Text = L"未成交手数";
            this->columnHeader24->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader24->Width = 80;
            // 
            // columnHeader25
            // 
            this->columnHeader25->Text = L"详细状态";
            this->columnHeader25->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader25->Width = 80;
            // 
            // columnHeader26
            // 
            this->columnHeader26->Text = L"报单时间";
            this->columnHeader26->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader26->Width = 100;
            // 
            // columnHeader27
            // 
            this->columnHeader27->Text = L"报单行情时间";
            this->columnHeader27->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader27->Width = 100;
            // 
            // columnHeader28
            // 
            this->columnHeader28->Text = L"成交均价";
            this->columnHeader28->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader28->Width = 80;
            // 
            // columnHeader29
            // 
            this->columnHeader29->Text = L"投保";
            this->columnHeader29->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader30
            // 
            this->columnHeader30->Text = L"订单类型";
            this->columnHeader30->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader30->Width = 80;
            // 
            // columnHeader31
            // 
            this->columnHeader31->Text = L"订单属性";
            this->columnHeader31->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader31->Width = 80;
            // 
            // columnHeader32
            // 
            this->columnHeader32->Text = L"止损/止盈价";
            this->columnHeader32->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader32->Width = 100;
            // 
            // columnHeader33
            // 
            this->columnHeader33->Text = L"申报标志";
            this->columnHeader33->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader33->Width = 80;
            // 
            // columnHeader34
            // 
            this->columnHeader34->Text = L"订单类别";
            this->columnHeader34->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader34->Width = 80;
            // 
            // columnHeader35
            // 
            this->columnHeader35->Text = L"最后成交时间";
            this->columnHeader35->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader35->Width = 100;
            // 
            // columnHeader36
            // 
            this->columnHeader36->Text = L"交易所";
            this->columnHeader36->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader36->Width = 100;
            // 
            // tabPage5
            // 
            this->tabPage5->Controls->Add(this->groupBox_TradeDetail);
            this->tabPage5->Location = System::Drawing::Point(4, 4);
            this->tabPage5->Name = L"tabPage5";
            this->tabPage5->Size = System::Drawing::Size(1323, 491);
            this->tabPage5->TabIndex = 4;
            this->tabPage5->Text = L"成交明细";
            this->tabPage5->UseVisualStyleBackColor = true;
            // 
            // groupBox_TradeDetail
            // 
            this->groupBox_TradeDetail->Controls->Add(this->listView_tradeDetail);
            this->groupBox_TradeDetail->Dock = System::Windows::Forms::DockStyle::Fill;
            this->groupBox_TradeDetail->Location = System::Drawing::Point(0, 0);
            this->groupBox_TradeDetail->Name = L"groupBox_TradeDetail";
            this->groupBox_TradeDetail->Size = System::Drawing::Size(1323, 491);
            this->groupBox_TradeDetail->TabIndex = 0;
            this->groupBox_TradeDetail->TabStop = false;
            this->groupBox_TradeDetail->Text = L"成交明细";
            // 
            // listView_tradeDetail
            // 
            this->listView_tradeDetail->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(13) {this->columnHeader37, this->columnHeader38, 
                this->columnHeader39, this->columnHeader40, this->columnHeader41, this->columnHeader42, this->columnHeader43, this->columnHeader44, 
                this->columnHeader45, this->columnHeader46, this->columnHeader47, this->columnHeader48, this->columnHeader49});
            this->listView_tradeDetail->Dock = System::Windows::Forms::DockStyle::Fill;
            this->listView_tradeDetail->FullRowSelect = true;
            this->listView_tradeDetail->GridLines = true;
            this->listView_tradeDetail->Location = System::Drawing::Point(3, 17);
            this->listView_tradeDetail->Name = L"listView_tradeDetail";
            this->listView_tradeDetail->Size = System::Drawing::Size(1317, 471);
            this->listView_tradeDetail->TabIndex = 0;
            this->listView_tradeDetail->UseCompatibleStateImageBehavior = false;
            this->listView_tradeDetail->View = System::Windows::Forms::View::Details;
            // 
            // columnHeader37
            // 
            this->columnHeader37->Text = L"序号";
            // 
            // columnHeader38
            // 
            this->columnHeader38->Text = L"账号";
            this->columnHeader38->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader38->Width = 80;
            // 
            // columnHeader39
            // 
            this->columnHeader39->Text = L"成交编号";
            this->columnHeader39->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader39->Width = 80;
            // 
            // columnHeader40
            // 
            this->columnHeader40->Text = L"合约";
            this->columnHeader40->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader41
            // 
            this->columnHeader41->Text = L"买卖";
            this->columnHeader41->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader42
            // 
            this->columnHeader42->Text = L"开平";
            this->columnHeader42->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader43
            // 
            this->columnHeader43->Text = L"成交价格";
            this->columnHeader43->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader43->Width = 80;
            // 
            // columnHeader44
            // 
            this->columnHeader44->Text = L"成交手数";
            this->columnHeader44->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader44->Width = 80;
            // 
            // columnHeader45
            // 
            this->columnHeader45->Text = L"成交时间";
            this->columnHeader45->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader45->Width = 160;
            // 
            // columnHeader46
            // 
            this->columnHeader46->Text = L"成交行情时间";
            this->columnHeader46->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader46->Width = 160;
            // 
            // columnHeader47
            // 
            this->columnHeader47->Text = L"报单编号";
            this->columnHeader47->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader47->Width = 80;
            // 
            // columnHeader48
            // 
            this->columnHeader48->Text = L"投保";
            this->columnHeader48->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // columnHeader49
            // 
            this->columnHeader49->Text = L"交易所";
            this->columnHeader49->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->columnHeader49->Width = 80;
            // 
            // treeListView1
            // 
            this->treeListView1->AllowColumnReorder = true;
            this->treeListView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->treeListView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(3) {this->columnHeader1, 
                this->columnHeader2, this->columnHeader3});
            this->treeListView1->GridLines = true;
            this->treeListView1->HideSelection = false;
            this->treeListView1->LabelWrap = false;
            this->treeListView1->Location = System::Drawing::Point(0, 0);
            this->treeListView1->Name = L"treeListView1";
            this->treeListView1->Size = System::Drawing::Size(1331, 137);
            this->treeListView1->SmallImageList = this->imageList1;
            this->treeListView1->TabIndex = 0;
            this->treeListView1->UseCompatibleStateImageBehavior = false;
            this->treeListView1->DoubleClick += gcnew System::EventHandler(this, &TestRecordForm::treeListView1_DoubleClick);
            // 
            // columnHeader1
            // 
            this->columnHeader1->Text = L"描述";
            this->columnHeader1->Width = 300;
            // 
            // columnHeader2
            // 
            this->columnHeader2->Text = L"总交易次数";
            this->columnHeader2->Width = 160;
            // 
            // columnHeader3
            // 
            this->columnHeader3->Text = L"净利润";
            this->columnHeader3->Width = 160;
            // 
            // imageList1
            // 
            this->imageList1->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"imageList1.ImageStream")));
            this->imageList1->TransparentColor = System::Drawing::Color::Transparent;
            this->imageList1->Images->SetKeyName(0, L"");
            this->imageList1->Images->SetKeyName(1, L"");
            this->imageList1->Images->SetKeyName(2, L"");
            // 
            // TestRecordForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(1331, 657);
            this->Controls->Add(this->splitContainer_Main);
            this->Name = L"TestRecordForm";
            this->Text = L"策略测试结果";
            this->Load += gcnew System::EventHandler(this, &TestRecordForm::TestRecordForm_Load);
            this->splitContainer_Main->Panel1->ResumeLayout(false);
            this->splitContainer_Main->Panel2->ResumeLayout(false);
            this->splitContainer_Main->ResumeLayout(false);
            this->tabControl1->ResumeLayout(false);
            this->tabPage1->ResumeLayout(false);
            this->splitContainer_Indicator->Panel1->ResumeLayout(false);
            this->splitContainer_Indicator->Panel2->ResumeLayout(false);
            this->splitContainer_Indicator->ResumeLayout(false);
            this->groupBox_Indicator->ResumeLayout(false);
            this->groupBox_IndicatorDesp->ResumeLayout(false);
            this->groupBox_IndicatorDesp->PerformLayout();
            this->tabPage2->ResumeLayout(false);
            this->splitContainer_ChartSummary->Panel1->ResumeLayout(false);
            this->splitContainer_ChartSummary->Panel2->ResumeLayout(false);
            this->splitContainer_ChartSummary->ResumeLayout(false);
            this->splitContainer_ChartSummary2->Panel2->ResumeLayout(false);
            this->splitContainer_ChartSummary2->ResumeLayout(false);
            this->tabPage3->ResumeLayout(false);
            this->groupBox3->ResumeLayout(false);
            this->tabPage4->ResumeLayout(false);
            this->groupBox_DelegateDetail->ResumeLayout(false);
            this->tabPage5->ResumeLayout(false);
            this->groupBox_TradeDetail->ResumeLayout(false);
            this->ResumeLayout(false);

        }
#pragma endregion

        private:
            Chart^ chart_ProfitLoss;
            Chart^ chart_ReportMonth;
            Chart^ chart_ReportYear;
            Chart^ chart_Profit;
            Chart^ chart_Indicator;
            Chart^ chart_Show;  //默认显示的图表
            bool   m_bHistory;
            String^ m_DataTime;

            TreeListViewItem^ m_itemRoot;


    private: Chart^ CreateChart(MSCHART_TYPE type,
                                const std::list<PlatformStru_TradingAccountInfo>& listTradingAccount,
                                const std::list<PlatformStru_TradeInfo>& listTrade)
             { 
                 Chart^  chart = gcnew Chart(); 
                 chart->Name = "chart";
                 chart->Parent = this->splitContainer_ChartSummary2->Panel1;
                 chart->Width = this->splitContainer_ChartSummary2->Panel1->Width;
                 chart->Height = this->splitContainer_ChartSummary2->Panel1->Height;

                 chart->BackColor = Color::FromArgb(211, 223, 240);
                 //图表背景色的渐变方式
                 chart->BackGradientStyle = GradientStyle::TopBottom;
                 //图表的边框颜色、
                 chart->BorderlineColor = Color::FromArgb(26, 59, 105);
                 //图表的边框线条样式
                 chart->BorderlineDashStyle = ChartDashStyle::Solid;
                 //图表边框线条的宽度
                 chart->BorderlineWidth = 2;
                 //图表边框的皮肤
                 chart->BorderSkin->SkinStyle = BorderSkinStyle::Emboss;


                 //设置图表的Title
                 Title^ title = gcnew Title();
                 //标题内容
                 if(MSCHART_TYPE_PROFITLOSS == type)
                    title->Text = "盈亏变化图";
                 else if(MSCHART_TYPE_REPORT_MONTH == type)
                     title->Text = "月度报表";
                 else if(MSCHART_TYPE_REPORT_YEAR == type)
                     title->Text = "年度报表";
                 else if(MSCHART_TYPE_PROFIT == type)
                     title->Text = "净利润图";
                 else if(MSCHART_TYPE_INDICATOR == type)
                     title->Text = "指标图"; 
                 //标题的字体
                 title->Font = gcnew System::Drawing::Font("Microsoft Sans Serif",12,FontStyle::Bold);
                 //标题字体颜色
                 title->ForeColor = Color::FromArgb(26, 59, 105);
                 //标题阴影颜色
                 title->ShadowColor = Color::FromArgb(32, 0, 0, 0);
                 //标题阴影偏移量
                 title->ShadowOffset = 0;

                 chart->Titles->Add(title);


                 //设置图表区属性
                 //图表区的名字
                 ChartArea^ chartArea = gcnew ChartArea("chartArea");
                 //背景色
                 chartArea->BackColor = Color::FromArgb(64, 165, 191, 228);
                 //背景渐变方式
                 chartArea->BackGradientStyle = GradientStyle::None;
                 //渐变和阴影的辅助背景色
                 chartArea->BackSecondaryColor = Color::White;
                 //边框颜色
                 chartArea->BorderColor = Color::FromArgb(64, 64, 64, 64);
                 //阴影颜色
                 chartArea->ShadowColor = Color::Transparent;

                 //设置X轴和Y轴线条的颜色和宽度
                 chartArea->AxisX->LineColor = Color::FromArgb(64, 64, 64, 64);
                 chartArea->AxisX->LineWidth = 1;
                 chartArea->AxisY->LineColor = Color::FromArgb(64, 64, 64, 64);
                 chartArea->AxisY->LineWidth = 1;

                 //设置X轴和Y轴的标题
                 if(MSCHART_TYPE_PROFITLOSS == type)
                 {
                     chartArea->AxisX->Title = "交易编号";
                     chartArea->AxisY->Title = "盈亏";
                 }
                 else if(MSCHART_TYPE_REPORT_MONTH == type)
                 {
                     chartArea->AxisX->Title = "日期";
                     chartArea->AxisY->Title = "盈亏";
                 }
                 else if(MSCHART_TYPE_REPORT_YEAR == type)
                 {
                     chartArea->AxisX->Title = "日期";
                     chartArea->AxisY->Title = "盈亏";
                 }
                 else if(MSCHART_TYPE_PROFIT == type)
                 {
                     chartArea->AxisX->Title = "时间";
                     chartArea->AxisY->Title = "盈亏";
                 }
                 else if(MSCHART_TYPE_INDICATOR == type)
                 {
                     chartArea->AxisX->Title = "时间";
                     chartArea->AxisY->Title = "盈亏";
                 }

                 //设置图表区网格横纵线条的颜色和宽度
                 chartArea->AxisX->MajorGrid->LineColor = Color::AliceBlue;
                 chartArea->AxisX->MajorGrid->LineWidth = 2;
                 chartArea->AxisY->MajorGrid->LineColor = Color::AliceBlue;
                 chartArea->AxisY->MajorGrid->LineWidth = 2;

                 chart->ChartAreas->Add(chartArea);


                 //图例及图例的位置
                 Legend^ legend = gcnew Legend();
                 legend->Alignment = StringAlignment::Center;
                 legend->Docking = Docking::Bottom;

                 chart->Legends->Add(legend);



                 Series^ series = gcnew Series("series");
                 
                 //图表所属的绘图区域名称
                 series->ChartArea = "chartArea";
                 //Series的名称
                 //Series的类型
                 if(MSCHART_TYPE_PROFITLOSS == type)
                 {
                     series->Name = "盈亏线";
                     series->ChartType = SeriesChartType::Line;
                 }
                 else if(MSCHART_TYPE_REPORT_MONTH == type)
                 {
                     series->Name = "盈亏";
                     series->ChartType = SeriesChartType::Column;
                 }
                 else if(MSCHART_TYPE_REPORT_YEAR == type)
                 {
                     series->Name = "盈亏";                  
                     series->ChartType = SeriesChartType::Column;
                 }
                 else if(MSCHART_TYPE_PROFIT == type)
                 {
                     series->Name = "净利润";
                     series->ChartType = SeriesChartType::Spline;
                 }
                 else if(MSCHART_TYPE_INDICATOR == type)
                 {
                     series->Name = "MA";
                     series->ChartType = SeriesChartType::Spline;                 
                 }
                 //Series的边框颜色
                 series->BorderColor = Color::FromArgb(180, 26, 59, 105);
                 //线条宽度
                 series->BorderWidth = 2;
                 //线条阴影颜色
                 series->ShadowColor = Color::Black;
                 //阴影宽度
                 series->ShadowOffset = 1;
                 //是否显示数据说明
                 series->IsVisibleInLegend = true;
                 //线条上数据点上是否有数据显示
                 series->IsValueShownAsLabel = false;
                 //线条上的数据点标志类型
                 series->MarkerStyle = MarkerStyle::Circle;
                 //线条数据点的大小
                 series->MarkerSize = 6;
                 //线条颜色
                 series->Color = Color::FromArgb(220, 65, 140, 240);


                 cli::array<int^>^ arr = gcnew cli::array<int^>(7){ 20, 40, 50, 80, 30, 10, 60 };   

                 
                 int nYear = Convert::ToInt32(m_DataTime->Substring(0,4));
                 int nMonth = Convert::ToInt32(m_DataTime->Substring(4,2));
                 int nDay = Convert::ToInt32(m_DataTime->Substring(6,2));
                 DateTime^ dt = gcnew DateTime(nYear,nMonth,nDay);// DateTime::Now;

                 static double dProfitLoss = 0;
                 if(MSCHART_TYPE_PROFITLOSS == type)
                 {                     
                     std::list<PlatformStru_TradeInfo>::const_iterator it = listTrade.begin();
                     for (int i=1;it!=listTrade.end();it++,i++) 
                     {
                         dProfitLoss += it->CloseProfitByTrade;
                         series->Points->AddXY(i, dProfitLoss);  
                     }
                 }
                 else if(MSCHART_TYPE_REPORT_MONTH == type)
                 {
                     for (int i = 0; i < 1; i++)  
                     {  
                         Object^ obj = gcnew Object;
                         obj = nMonth.ToString();
                         series->Points->AddXY(obj, dProfitLoss); 
                         //series->Points->AddXY(dt, arr[i]); 
                         //dt = dt->AddMonths(1); 
                     }  
                 }
                 else if(MSCHART_TYPE_REPORT_YEAR == type)
                 {
                     for (int i = 0; i < 1; i++)  
                     {  
                         Object^ obj = gcnew Object;
                         obj = nYear.ToString();
                         series->Points->AddXY(obj, dProfitLoss); 
                         //series->Points->AddXY(dt, arr[i]); 
                         //dt = dt->AddYears(1);  
                     }  
                 }
                 else if(MSCHART_TYPE_PROFIT == type)
                 {
                     int nProfit = 0;
                     std::list<PlatformStru_TradingAccountInfo>::const_iterator it = listTradingAccount.begin();
                     for (int i=1;it!=listTradingAccount.end();it++,i++)                      
                     {
                         nProfit = it->PositionProfit+it->CloseProfit-it->Commission;
                         Object^ obj = gcnew Object;
                         obj = String(it->Account).ToString();
                         series->Points->AddXY(obj, nProfit);
                         //dt = dt->AddHours(1);  
                     }
                 }
                 else if(MSCHART_TYPE_INDICATOR == type)
                 {
                     for (int i = 0; i < arr->Length; i++)  
                     {
                         series->Points->AddXY(dt, arr[i]);         
                         dt = dt->AddDays(1);  
                     }
                 }
                
                 chart->Series->Add(series);

                 return chart;                 
             }

    private: void InitIndicatorListView(const TestIndicator& indicator)
             {
                 //净利润
                 listView_Indicator->Items[0]->SubItems[2]->Text = indicator.NetProfit.ToString();
                 //平均盈利
                 listView_Indicator->Items[0]->SubItems[4]->Text = indicator.AvgProfit.ToString();
                 //总盈利
                 listView_Indicator->Items[0]->SubItems[6]->Text = indicator.TotalProfit.ToString();
                 //最大亏损
                 listView_Indicator->Items[1]->SubItems[2]->Text = indicator.MaxLoss.ToString();
                  //总亏损
                 listView_Indicator->Items[1]->SubItems[4]->Text = indicator.TotalLoss.ToString();
                 //平均亏损
                 listView_Indicator->Items[1]->SubItems[6]->Text = indicator.AvgLoss.ToString();
                 //初始权益
                 listView_Indicator->Items[2]->SubItems[2]->Text = indicator.InitialProfit.ToString();
                 //总盈利/总亏损(%)
                 listView_Indicator->Items[2]->SubItems[4]->Text = (indicator.TotalProfit*100/indicator.TotalLoss).ToString();
                 //最小权益
                 listView_Indicator->Items[2]->SubItems[6]->Text = indicator.MiniDynamicProfit.ToString();
                 //毛利润
                 listView_Indicator->Items[3]->SubItems[2]->Text = indicator.GrossProfit.ToString();
                 //净利润/最大盈利(%)
                 listView_Indicator->Items[3]->SubItems[4]->Text = (indicator.NetProfit*100/indicator.MaxProfit).ToString();
                 //最大权益
                 listView_Indicator->Items[3]->SubItems[6]->Text = indicator.MaxDynamicProfit.ToString();
                 //平均利润
                 listView_Indicator->Items[4]->SubItems[2]->Text = (indicator.NetProfit*100/indicator.TotalTradeCnt).ToString();
                 //净利润/最大亏损(%)
                 listView_Indicator->Items[4]->SubItems[4]->Text = (indicator.NetProfit*100/indicator.MaxLoss).ToString();
                 //最大使用资金
                 listView_Indicator->Items[4]->SubItems[6]->Text = indicator.MaxUsedFund.ToString();
                 //手续费/毛利润(%)
                 listView_Indicator->Items[5]->SubItems[2]->Text = (indicator.TotalCommission/indicator.GrossProfit).ToString();
                 //最大盈利/最大亏损(%)
                 listView_Indicator->Items[5]->SubItems[4]->Text = (indicator.MaxProfit*100/indicator.MaxLoss).ToString();
                 //结束总权益
                 listView_Indicator->Items[5]->SubItems[6]->Text = indicator.FinishProfit.ToString();
                 //未成交手数
                 listView_Indicator->Items[6]->SubItems[2]->Text = indicator.OpenOrderCnt.ToString();
                 //胜率(%)
                 listView_Indicator->Items[6]->SubItems[4]->Text = indicator.WinRate.ToString();
                 //盈利次数
                 listView_Indicator->Items[6]->SubItems[6]->Text = indicator.TotalProfitCnt.ToString();
                 //最大盈利/总盈利(%)
                 listView_Indicator->Items[7]->SubItems[2]->Text = (indicator.MaxProfit*100/indicator.TotalProfit).ToString();
                 //收益率(%)
                 listView_Indicator->Items[7]->SubItems[4]->Text = indicator.YieldRate.ToString();
                 //亏损次数
                 listView_Indicator->Items[7]->SubItems[6]->Text = indicator.TotalLossCnt.ToString();
                 //最大亏损/总亏损(%)
                 listView_Indicator->Items[8]->SubItems[2]->Text = (indicator.MaxLoss*100/indicator.TotalLoss).ToString();
                 //有效收益率(%)
                 listView_Indicator->Items[8]->SubItems[4]->Text = indicator.EffectiveYield.ToString();
                 //年度收益率(%)
                 listView_Indicator->Items[8]->SubItems[6]->Text = indicator.Annualyield.ToString();
                 //盈利次数/总交易次数(%)
                 listView_Indicator->Items[9]->SubItems[2]->Text = (indicator.TotalProfitCnt/indicator.TotalTradingTime).ToString();
                 //月度平均盈利
                 listView_Indicator->Items[9]->SubItems[4]->Text = indicator.MonthlyAvgProfit.ToString();
                 //亏损次数/盈利次数(%)
                 listView_Indicator->Items[9]->SubItems[6]->Text = (indicator.TotalProfitCnt==0)?System::String::Empty:(indicator.TotalLossCnt*100/indicator.TotalProfitCnt).ToString();
                 //年化收益
                 listView_Indicator->Items[10]->SubItems[2]->Text = indicator.AnnualProfit.ToString();
                 //亏损次数/总交易次数(%)
                 listView_Indicator->Items[10]->SubItems[4]->Text = (indicator.TotalLossCnt*100/indicator.TotalTradingTime).ToString();
                 //最大连续盈利次数
                 listView_Indicator->Items[10]->SubItems[6]->Text = indicator.MaxContinuousProfit.ToString();
                 //最大连续亏损次数
                 listView_Indicator->Items[11]->SubItems[2]->Text = indicator.MaxContinuousLoss.ToString();
                 //总交易次数
                 listView_Indicator->Items[11]->SubItems[4]->Text = indicator.TotalTradeCnt.ToString();
                 //最大持仓
                 listView_Indicator->Items[11]->SubItems[6]->Text = indicator.MaxPosition.ToString();
                 //置信度
                 listView_Indicator->Items[12]->SubItems[2]->Text = indicator.ConfidenceLevel.ToString();
                 //总交易委托数
                 listView_Indicator->Items[12]->SubItems[4]->Text = indicator.TotalOrderCnt.ToString();
                 //最大盈利
                 listView_Indicator->Items[12]->SubItems[6]->Text = indicator.MaxProfit.ToString();
                 //策略开始时间
                 listView_Indicator->Items[13]->SubItems[2]->Text = String(indicator.szStartTime).ToString();//策略开始时间:
                 //策略停止时间
                 listView_Indicator->Items[13]->SubItems[4]->Text = String(indicator.szEndTime).ToString();//策略停止时间:              
             }

    private: void InitStrategyInfoListView(CDataEntity* pEntity,CStrategyDataEx& strategy)
             {      
                 string strInsts,strVersion;
                 std::map<std::string, PlatformStru_InstrumentInfo> mapInstruments = pEntity->GetInstrument();
                 std::map<std::string, PlatformStru_InstrumentInfo>::iterator it = mapInstruments.begin();
                 for (;it!=mapInstruments.end();it++)
                 {
                    strInsts += it->first;
                    strInsts += " ";
                 }

                 TestIndicator outData;
                 pEntity->GetTestIndicator(outData);
                 strVersion = pEntity->GetStrategy().GetStrategyVersion();



                 int nIndex = 0;              

                 ListViewItem^ item1 = gcnew ListViewItem((nIndex++).ToString());
                 item1->SubItems->Add("策略名称");
                 item1->SubItems->Add(String(strategy.GetStrategyNickName().data()).ToString());
                 ListViewItem^ item2 = gcnew ListViewItem((nIndex++).ToString());
                 item2->SubItems->Add("合约信息");
                 item2->SubItems->Add(String(strInsts.data()).ToString());
                 ListViewItem^ item3 = gcnew ListViewItem((nIndex++).ToString());
                 item3->SubItems->Add("创建时间");
                 item3->SubItems->Add(String(outData.szCreatTime).ToString());
                 ListViewItem^ item4 = gcnew ListViewItem((nIndex++).ToString());
                 item4->SubItems->Add("更新时间");
                 item4->SubItems->Add(String(outData.szModifyTime).ToString());
                 ListViewItem^ item5 = gcnew ListViewItem((nIndex++).ToString());
                 item5->SubItems->Add("版本号");
                 item5->SubItems->Add(String(strVersion.data()).ToString());
                 ListViewItem^ item6 = gcnew ListViewItem((nIndex++).ToString());
                 item6->SubItems->Add("交易源");
                 item6->SubItems->Add("需求未明确");
                 ListViewItem^ item7 = gcnew ListViewItem((nIndex++).ToString());
                 item7->SubItems->Add("行情源");
                 item7->SubItems->Add("需求未明确");
                 ListViewItem^ item8 = gcnew ListViewItem((nIndex++).ToString());
                 item8->SubItems->Add("合约");
                 item8->SubItems->Add(String(strInsts.data()).ToString());

                 listView_StrategyInfo->Items->Add(item1);
                 listView_StrategyInfo->Items->Add(item2);
                 listView_StrategyInfo->Items->Add(item3);
                 listView_StrategyInfo->Items->Add(item4);
                 listView_StrategyInfo->Items->Add(item5);
                 listView_StrategyInfo->Items->Add(item6);
                 listView_StrategyInfo->Items->Add(item7);
                 listView_StrategyInfo->Items->Add(item8);


                 std::map<std::string,StrategyPara> mapPara = pEntity->GetStrategy().GetStrategyParams();
                 std::map<std::string,StrategyPara>::iterator itPara = mapPara.begin();
                 for (;itPara!=mapPara.end();itPara++)
                 {
                    string  strTitle = itPara->first;
                    StrategyPara para = itPara->second;
                    ListViewItem^ itemxx = gcnew ListViewItem((nIndex++).ToString());
                    itemxx->SubItems->Add(String(strTitle.data()).ToString());
                    itemxx->SubItems->Add(String(para.szDefault).ToString());

                    listView_StrategyInfo->Items->Add(itemxx);
                 }

                 //ListViewItem^ item11 = gcnew ListViewItem("11");
                 //item11->SubItems->Add("浮动盈亏计数");
                 //item11->SubItems->Add(String(strategy.GetStrategyNickName().data()).ToString());
                 //ListViewItem^ item12 = gcnew ListViewItem("12");
                 //item12->SubItems->Add("开仓手数");
                 //item12->SubItems->Add(String(strategy.GetStrategyNickName().data()).ToString());
                 //ListViewItem^ item13 = gcnew ListViewItem("13");
                 //item13->SubItems->Add("均线偏离(tick)");
                 //item13->SubItems->Add(String(strategy.GetStrategyNickName().data()).ToString());
                 //ListViewItem^ item14 = gcnew ListViewItem("14");
                 //item14->SubItems->Add("采样个数1");
                 //item14->SubItems->Add(String(strategy.GetStrategyNickName().data()).ToString());
                 //ListViewItem^ item15 = gcnew ListViewItem("15");
                 //item15->SubItems->Add("采样个数2");
                 //item15->SubItems->Add(String(strategy.GetStrategyNickName().data()).ToString());
                 //ListViewItem^ item16 = gcnew ListViewItem("16");
                 //item16->SubItems->Add("止盈百分比");
                 //item16->SubItems->Add(String(strategy.GetStrategyNickName().data()).ToString());
                 //ListViewItem^ item17 = gcnew ListViewItem("17");
                 //item17->SubItems->Add("止损tick");
                 //item17->SubItems->Add(String(strategy.GetStrategyNickName().data()).ToString());
                 //ListViewItem^ item18 = gcnew ListViewItem("18");
                 //item18->SubItems->Add("开仓方向(0:多空;1:多;-1:空)");
                 //item18->SubItems->Add(String(strategy.GetStrategyNickName().data()).ToString());


                 //listView_StrategyInfo->Items->Add(item11);
                 //listView_StrategyInfo->Items->Add(item12);
                 //listView_StrategyInfo->Items->Add(item13);
                 //listView_StrategyInfo->Items->Add(item14);
                 //listView_StrategyInfo->Items->Add(item15);
                 //listView_StrategyInfo->Items->Add(item16);
                 //listView_StrategyInfo->Items->Add(item17);
                 //listView_StrategyInfo->Items->Add(item18);
             }             
    private: void InitTestRecordListView(TestSimpleData& data)
             {
                 TreeListViewItem^ itemRoot = gcnew TreeListViewItem("测试记录", 0);
                 treeListView1->Items->Add(itemRoot);

                 String^ strData = String(data.szDate).ToString();
                 String^ strName = String(data.szName).ToString();
                 String^ strTime = String(data.szTime).ToString();
                 String^ strCount = data.TotalTradeCnt.ToString();
                 String^ strProfit = data.NetProfit.ToString();
                 strName += " ("+strTime+")"; 

                 TreeListViewItem^ itemFirst = gcnew TreeListViewItem(strData, 1);                      
                 itemRoot->Items->Add(itemFirst);

                 TreeListViewItem^ itemSecond = gcnew TreeListViewItem(strName, 2);
                 itemFirst->Items->Add(itemSecond);

                 itemSecond->SubItems->Add(strCount);
                 itemSecond->SubItems->Add(strProfit);

                 itemSecond->Tag = strData;

                 itemRoot->Expand();
             }  
    private: void InitTestRecordListView_history()
             {
                 TreeListViewItem^ itemRoot = gcnew TreeListViewItem("测试记录", 0);
                 treeListView1->Items->Add(itemRoot);                

                 m_itemRoot = itemRoot;
             }
    private: TreeListViewItem^ InitTestRecordListView_history3(TreeListViewItem^ itemRoot,String^ data)
             {
                 if(itemRoot==nullptr)
                     return itemRoot;

                 TreeListViewItem^ itemFirst = gcnew TreeListViewItem(data, 1);                      
                 itemRoot->Items->Add(itemFirst);                    

                 itemRoot->Expand();

                 return itemFirst;
             }
    private: void InitTestRecordListView_history4(TreeListViewItem^ itemRoot,TestSimpleData& data)
             {
                 if(itemRoot==nullptr)
                     return;

                 String^ strName = String(data.szName).ToString();
                 String^ strTime = String(data.szTime).ToString();
                 String^ strCount = data.TotalTradeCnt.ToString();
                 String^ strProfit = data.NetProfit.ToString();
                 strName += " ("+strTime+")"; 

                 TreeListViewItem^ itemSecond = gcnew TreeListViewItem(strName, 2);
                 itemRoot->Items->Add(itemSecond);

                 itemSecond->SubItems->Add(strCount);
                 itemSecond->SubItems->Add(strProfit);                  

                 itemRoot->Expand();
             }
    private: void InitDelegateDetailListView(const std::list<PlatformStru_OrderInfo>& orderlist)
             {
                 std::list<PlatformStru_OrderInfo>::const_iterator it = orderlist.begin();
                 for(int i=0,nSeq = 1;it!=orderlist.end();it++,i++,nSeq++)
                 {
                     PlatformStru_OrderInfo info = *it;

                     std::string strCombOffsetFlag,strHedgeFlag,strOrderStatus;
                     switch(info.CombOffsetFlag[0])
                     {
                     case THOST_FTDC_OF_Open:
                         strCombOffsetFlag = "开仓";
                         break;
                     case THOST_FTDC_OF_Close:
                         strCombOffsetFlag = "平仓";
                         break;
                     case THOST_FTDC_OF_ForceClose:
                         strCombOffsetFlag = "强平";
                         break;
                     case THOST_FTDC_OF_CloseToday:
                         strCombOffsetFlag = "平今";
                         break;
                     case THOST_FTDC_OF_CloseYesterday:
                         strCombOffsetFlag = "平昨";
                         break;
                     case THOST_FTDC_OF_ForceOff:
                         strCombOffsetFlag = "强减";
                         break;
                     case THOST_FTDC_OF_LocalForceClose:
                         strCombOffsetFlag = "本地强平";
                         break;
                     }
                     switch(info.CombHedgeFlag[0])
                     {
                     case THOST_FTDC_HF_Speculation: 
                         strHedgeFlag = "投机";
                         break;
                     case THOST_FTDC_HF_Arbitrage:
                         strHedgeFlag = "套利";
                         break;
                     case THOST_FTDC_HF_Hedge:
                         strHedgeFlag = "套保";
                         break;
                     }

                     switch(info.OrderStatus)
                     {
                     case THOST_FTDC_OST_AllTraded:
                         strOrderStatus = "全部成交";
                         break;
                     case THOST_FTDC_OST_PartTradedQueueing:
                         strOrderStatus = "部分成交还在队列中";
                         break;
                     case THOST_FTDC_OST_PartTradedNotQueueing:
                         strOrderStatus = "部分成交不在队列中";
                         break;
                     case THOST_FTDC_OST_NoTradeQueueing:
                         strOrderStatus = "未成交还在队列中";
                         break;
                     case THOST_FTDC_OST_NoTradeNotQueueing:
                         strOrderStatus = "未成交不在队列中";
                         break;
                     case THOST_FTDC_OST_Canceled:
                         strOrderStatus = "撤单";
                         break;
                     case THOST_FTDC_OST_Unknown:
                         strOrderStatus = "未知";
                         break;
                     case THOST_FTDC_OST_NotTouched:
                         strOrderStatus = "尚未触发";
                         break;
                     case THOST_FTDC_OST_Touched:
                         strOrderStatus = "已触发";
                         break;
                     }

                     int nIndex = 0;
                     ListViewItem^ item = gcnew ListViewItem(nSeq.ToString());
                     item->SubItems->Add(String(info.Account).ToString());
                     item->SubItems->Add(String(info.OrderSysID).ToString());
                     item->SubItems->Add(String(info.InstrumentID).ToString());
                     item->SubItems->Add((info.Direction == THOST_FTDC_D_Buy?"买":"卖"));
                     item->SubItems->Add(String(strCombOffsetFlag.data()).ToString());
                     item->SubItems->Add(String(strOrderStatus.data()).ToString());
                     item->SubItems->Add(info.LimitPrice.ToString());
                     item->SubItems->Add(info.VolumeTotalOriginal.ToString());
                     item->SubItems->Add((info.VolumeTotalOriginal-info.VolumeTotal).ToString());
                     item->SubItems->Add(info.VolumeTotal.ToString());
                     item->SubItems->Add(String(info.StatusMsg).ToString());
                     item->SubItems->Add(String(info.InsertTime).ToString());
                     item->SubItems->Add(nSeq.ToString());//报单行情时间
                     item->SubItems->Add(nSeq.ToString());//成交均价
                     item->SubItems->Add(String(strHedgeFlag.data()).ToString());
                     item->SubItems->Add(nSeq.ToString());//订单类型
                     item->SubItems->Add(nSeq.ToString());//订单属性
                     item->SubItems->Add(nSeq.ToString());//止损止盈价
                     item->SubItems->Add(nSeq.ToString());//申报标志
                     item->SubItems->Add(nSeq.ToString());//订单类别
                     item->SubItems->Add(nSeq.ToString());//最后成交时间
                     item->SubItems->Add(String(info.ExchangeID).ToString());//

                     listView_delegateDetail->Items->Add(item);
                 }

             }
    private: void InitTradeDetailListView(const std::list<PlatformStru_TradeInfo>& tradelist)
             {
                 std::list<PlatformStru_TradeInfo>::const_iterator it = tradelist.begin();
                 for(int i=0,nSeq = 1;it!=tradelist.end();it++,i++,nSeq++)
                 {
                     PlatformStru_TradeInfo info = *it;

                     std::string strOffsetFlag,strHedgeFlag,strTradeType;
                     switch(info.OffsetFlag)
                     {
                     case THOST_FTDC_OF_Open:
                         strOffsetFlag = "开仓";
                         break;
                     case THOST_FTDC_OF_Close:
                         strOffsetFlag = "平仓";
                         break;
                     case THOST_FTDC_OF_ForceClose:
                         strOffsetFlag = "强平";
                         break;
                     case THOST_FTDC_OF_CloseToday:
                         strOffsetFlag = "平今";
                         break;
                     case THOST_FTDC_OF_CloseYesterday:
                         strOffsetFlag = "平昨";
                         break;
                     case THOST_FTDC_OF_ForceOff:
                         strOffsetFlag = "强减";
                         break;
                     case THOST_FTDC_OF_LocalForceClose:
                         strOffsetFlag = "本地强平";
                         break;
                     }
                     switch(info.HedgeFlag)
                     {
                     case THOST_FTDC_HF_Speculation: 
                         strHedgeFlag = "投机";
                         break;
                     case THOST_FTDC_HF_Arbitrage:
                         strHedgeFlag = "套利";
                         break;
                     case THOST_FTDC_HF_Hedge:
                         strHedgeFlag = "套保";
                         break;
                     }
                     switch(info.TradeType)
                     {
                     case THOST_FTDC_TRDT_Common:
                         strTradeType = "普通成交";
                         break;
                     case THOST_FTDC_TRDT_OptionsExecution:
                         strTradeType = "期权执行";
                         break;
                     case THOST_FTDC_TRDT_OTC:
                         strTradeType = "OTC成交";
                         break;
                     case THOST_FTDC_TRDT_EFPDerived:
                         strTradeType = "期转现衍生成交";
                         break;
                     case THOST_FTDC_TRDT_CombinationDerived:
                         strTradeType = "组合衍生成交";
                         break;
                     default:
                         strTradeType = "普通成交";
                         break;
                     }

                     ListViewItem^ item = gcnew ListViewItem(nSeq.ToString());
                     item->SubItems->Add(String(info.Account).ToString());
                     item->SubItems->Add(String(info.TradeID).ToString());
                     item->SubItems->Add(String(info.InstrumentID).ToString());
                     item->SubItems->Add((info.Direction == THOST_FTDC_D_Buy?"买":"卖"));
                     item->SubItems->Add(String(strOffsetFlag.data()).ToString());
                     item->SubItems->Add(info.Price.ToString());
                     item->SubItems->Add(info.Volume.ToString());
                     item->SubItems->Add(String(info.TradeTime).ToString());
                     item->SubItems->Add(String(info.TradeTime).ToString());//成交行情时间
                     item->SubItems->Add(String(info.OrderSysID).ToString());
                     item->SubItems->Add(String(strHedgeFlag.data()).ToString());
                     item->SubItems->Add(String(info.ExchangeID).ToString());

                     listView_tradeDetail->Items->Add(item);
                 }
             }

    private: void InitChartListView(const std::list<PlatformStru_TradeInfo>& tradelist)
             {
                 std::list<PlatformStru_TradeInfo>::const_iterator it = tradelist.begin();
                 for(int i=0,nSeq = 1;it!=tradelist.end();it++,i++,nSeq++)
                 {
                     PlatformStru_TradeInfo info = *it;

                     std::string strOffsetFlag,strHedgeFlag,strTradeType;
                     switch(info.OffsetFlag)
                     {
                     case THOST_FTDC_OF_Open:
                         strOffsetFlag = "开仓";
                         break;
                     case THOST_FTDC_OF_Close:
                         strOffsetFlag = "平仓";
                         break;
                     case THOST_FTDC_OF_ForceClose:
                         strOffsetFlag = "强平";
                         break;
                     case THOST_FTDC_OF_CloseToday:
                         strOffsetFlag = "平今";
                         break;
                     case THOST_FTDC_OF_CloseYesterday:
                         strOffsetFlag = "平昨";
                         break;
                     case THOST_FTDC_OF_ForceOff:
                         strOffsetFlag = "强减";
                         break;
                     case THOST_FTDC_OF_LocalForceClose:
                         strOffsetFlag = "本地强平";
                         break;
                     }
                     switch(info.HedgeFlag)
                     {
                     case THOST_FTDC_HF_Speculation: 
                         strHedgeFlag = "投机";
                         break;
                     case THOST_FTDC_HF_Arbitrage:
                         strHedgeFlag = "套利";
                         break;
                     case THOST_FTDC_HF_Hedge:
                         strHedgeFlag = "套保";
                         break;
                     }
                     switch(info.TradeType)
                     {
                     case THOST_FTDC_TRDT_Common:
                         strTradeType = "普通成交";
                         break;
                     case THOST_FTDC_TRDT_OptionsExecution:
                         strTradeType = "期权执行";
                         break;
                     case THOST_FTDC_TRDT_OTC:
                         strTradeType = "OTC成交";
                         break;
                     case THOST_FTDC_TRDT_EFPDerived:
                         strTradeType = "期转现衍生成交";
                         break;
                     case THOST_FTDC_TRDT_CombinationDerived:
                         strTradeType = "组合衍生成交";
                         break;
                     default:
                         strTradeType = "普通成交";
                         break;
                     }

                     ListViewItem^ item = gcnew ListViewItem(nSeq.ToString());
                     item->SubItems->Add(String(info.InstrumentID).ToString());
                     item->SubItems->Add((info.Direction == THOST_FTDC_D_Buy?"买":"卖"));
                     item->SubItems->Add(String(strOffsetFlag.data()).ToString());

                     item->SubItems->Add(info.CloseProfitByDate.ToString());//盈亏
                     item->SubItems->Add(info.CloseProfitByTrade.ToString());//总盈亏

                     item->SubItems->Add(String(info.Account).ToString());
                     item->SubItems->Add(String(info.TradeID).ToString());   
                     item->SubItems->Add(info.Price.ToString());
                     item->SubItems->Add(info.Volume.ToString());
                     item->SubItems->Add(String(info.TradeTime).ToString());
                     item->SubItems->Add(String(info.TradeTime).ToString());//成交行情时间
                     item->SubItems->Add(String(info.OrderSysID).ToString());
                     item->SubItems->Add(String(strHedgeFlag.data()).ToString());
                     item->SubItems->Add(String(info.ExchangeID).ToString());

                     listView_chart->Items->Add(item);
                 }
             }
    //用于历史查询 主要生成树
    private:void LoadHistoryData(sQueryCondition* sqd)
            {
                if(sqd->m_bSameDay)//同一天
                {
                    string hisPathRoot = _GetHistoryDataPath();   
                    //String^ hisPath = "F:\\strategy\\HistoryDatas\\"+(String(sqd->m_start_date.data()).ToString());
                    String^ hisPath = String(_GetHistoryDataPath().data()).ToString()+"\\"+(String(sqd->m_start_date.data()).ToString());
                    if(!System::IO::Directory::Exists(hisPath))
                    {
                        MessageBox::Show("加载历史数据：没有根目录"+hisPath, "提示", MessageBoxButtons::OK);
                        return;
                    }
                        

                    cli::array<String^>^ MyFolders = System::IO::Directory::GetDirectories(hisPath);
                    if(0==MyFolders->Length)
                    {
                        MessageBox::Show("加载历史数据：根目录"+hisPath+"下没有历史文件。", "提示", MessageBoxButtons::OK);
                        return;
                    }

                    TreeListViewItem^ node = InitTestRecordListView_history3(m_itemRoot,String(sqd->m_start_date.data()).ToString());

                    TestSimpleData data;
                    for each(String^ path in MyFolders)
                    {  
                        CDataEntity *pDataEntity = new CDataEntity();

                        string szPath = (char*)(Marshal::StringToHGlobalAnsi(path+"\\")).ToPointer();
                        pDataEntity->SetSavePath(szPath); 
                        if(pDataEntity->LoadAll())
                        {
                            //初始化树                                
                            memset(&data,0,sizeof(TestSimpleData));
                            if(pDataEntity->LoadSimple(data))
                                InitTestRecordListView_history4(node,data);
                        }

                        if(pDataEntity)
                        {
                            delete pDataEntity;
                            pDataEntity = NULL;
                        }
                    }
                }
                else
                {
                    String^ hisPath = String(_GetHistoryDataPath().data()).ToString();                    
                    if(!System::IO::Directory::Exists(hisPath))
                    {
                        MessageBox::Show("加载历史数据：没有根目录"+hisPath, "提示", MessageBoxButtons::OK);
                        return;
                    }

                    TestSimpleData data;
                    cli::array<String^>^ MyFolders = System::IO::Directory::GetDirectories(hisPath);// F:\strategy\HistoryDatas\20140627
                    if(0==MyFolders->Length)
                    {
                        MessageBox::Show("加载历史数据：根目录"+hisPath+"下没有历史文件。", "提示", MessageBoxButtons::OK);
                        return;
                    }

                    for each(String^ path in MyFolders)
                    {  
                        String^ fname = path->Substring(path->Length-8,8);
                        string sFname = (char*)(Marshal::StringToHGlobalAnsi(fname)).ToPointer();
                        unsigned int uiTime = atoi(sFname.c_str());
                        unsigned int uiStartTime = atoi(sqd->m_start_date.c_str());
                        unsigned int uiEndTime = atoi(sqd->m_end_date.c_str());

                        if (uiTime>=uiStartTime && uiTime<=uiEndTime) //判断日期大小                               
                        {
                            TreeListViewItem^ node = InitTestRecordListView_history3(m_itemRoot,fname);

                            cli::array<String^>^ subFolders = System::IO::Directory::GetDirectories(path); //F:\strategy\HistoryDatas\20140627\085144
                            if(0==subFolders->Length)
                            {
                                MessageBox::Show("加载历史数据：目录"+path+"下没有文件。", "提示", MessageBoxButtons::OK);
                                return;
                            }

                            for each(String^ subPath in subFolders)
                            {
                                CDataEntity *pDataEntity = new CDataEntity();

                                string szPath = (char*)(Marshal::StringToHGlobalAnsi(subPath+"\\")).ToPointer();
                                pDataEntity->SetSavePath(szPath); 
                                if(pDataEntity->LoadAll())
                                {
                                    //初始化树                                
                                    memset(&data,0,sizeof(TestSimpleData));
                                    if(pDataEntity->LoadSimple(data))
                                        InitTestRecordListView_history4(node,data);
                                }

                                if(pDataEntity)
                                {
                                    delete pDataEntity;
                                    pDataEntity = NULL;
                                }   
                            }                            
                        }
                    }
                }
            }
    private:void LoadHistoryData(string sPath,bool bInitTreelistView)
            {
                //test data from local file
                CDataEntity *pDataEntity = new CDataEntity();
                pDataEntity->SetSavePath(sPath); 
                if(pDataEntity->LoadAll())
                {                     
                    //初始化树
                    if(bInitTreelistView)
                    {
                        TestSimpleData data;
                        memset(&data,0,sizeof(TestSimpleData));
                        if(pDataEntity->LoadSimple(data))
                            InitTestRecordListView(data);


                        string sData(data.szDate);
                        sData = sData.substr(0,4)+sData.substr(5,2)+sData.substr(8,2);
                        m_DataTime = String(sData.data()).ToString();
                    }

                    //初始化指标列表
                    TestIndicator indicator;
                    memset(&indicator,0,sizeof(TestIndicator));
                    pDataEntity->GetTestIndicator(indicator);
                    InitIndicatorListView(indicator);

                    //初始化策略信息列表
                    CStrategyDataEx strategy = pDataEntity->GetStrategy();
                    InitStrategyInfoListView(pDataEntity,strategy);

                    //初始化委托明细列表                 
                    const std::list<PlatformStru_OrderInfo> orderlist = pDataEntity->GetOrderList();
                    InitDelegateDetailListView(orderlist);                     

                    //初始化成交明细列表
                    const std::list<PlatformStru_TradeInfo> traderlist = pDataEntity->GetTradeList();
                    InitTradeDetailListView(traderlist);

                    //初始化图表下的列表
                    InitChartListView(traderlist);

                    //初始化图表
                    const std::list<PlatformStru_TradingAccountInfo> tradingAccountlist = pDataEntity->GetTradingAccountList();
                    
                    chart_ProfitLoss = CreateChart(MSCHART_TYPE_PROFITLOSS,tradingAccountlist,traderlist);
                    chart_ReportMonth = CreateChart(MSCHART_TYPE_REPORT_MONTH,tradingAccountlist,traderlist);
                    chart_ReportYear = CreateChart(MSCHART_TYPE_REPORT_YEAR,tradingAccountlist,traderlist);
                    chart_Profit = CreateChart(MSCHART_TYPE_PROFIT,tradingAccountlist,traderlist);
                    chart_Indicator = CreateChart(MSCHART_TYPE_INDICATOR,tradingAccountlist,traderlist);

                    chart_Show = chart_ProfitLoss;
                    listBox1->SetSelected(0,true);

                    if(pDataEntity)
                    {
                        delete pDataEntity;
                        pDataEntity = NULL;
                    }
                }
            }
private: System::Void TestRecordForm_Load(System::Object^  sender, System::EventArgs^  e) {             

             if(m_bHistory)
                 InitTestRecordListView_history();
             else
                 LoadHistoryData(_GetHistoryDataPath(),true);                 
         }
private: System::Void listView_Indicator_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             if(listView_Indicator->SelectedIndices != nullptr && listView_Indicator->SelectedIndices->Count>0)
             {

                 switch(listView_Indicator->SelectedItems[0]->Index)
                 {
                 case 0:
                     this->label1->Text = listView_Indicator->SelectedItems[0]->SubItems[1]->Text+":"+"总盈利+总亏损";//净利润
                     this->label2->Text = listView_Indicator->SelectedItems[0]->SubItems[3]->Text+":"+"总盈利/总盈利次数（平仓盈亏）";//平均盈利
                     this->label3->Text = listView_Indicator->SelectedItems[0]->SubItems[5]->Text+":"+"所有盈利交易的净盈利之和 （平仓盈亏）";//总盈利
                     break;
                 case 1:
                     this->label1->Text = listView_Indicator->SelectedItems[0]->SubItems[1]->Text+":"+"所有亏损交易的净亏损之和";//最大亏损
                     this->label2->Text = listView_Indicator->SelectedItems[0]->SubItems[3]->Text+":"+"所有亏损交易的净亏损之和";//总亏损
                     this->label3->Text = listView_Indicator->SelectedItems[0]->SubItems[5]->Text+":"+"总亏损/亏损次数";//平均亏损
                     break;
                 case 2:
                     this->label1->Text = listView_Indicator->SelectedItems[0]->SubItems[1]->Text+":"+"账号初始权益";//初始权益
                     this->label2->Text = listView_Indicator->SelectedItems[0]->SubItems[3]->Text+":"+"";//总盈利/总亏损(%)
                     this->label3->Text = listView_Indicator->SelectedItems[0]->SubItems[5]->Text+":"+"交易过程中权益的最小值";//最小权益
                     break;
                 case 3:
                     this->label1->Text = listView_Indicator->SelectedItems[0]->SubItems[1]->Text+":"+"净利润+手续费";//毛利润
                     this->label2->Text = listView_Indicator->SelectedItems[0]->SubItems[3]->Text+":"+"";//净利润/最大盈利(%)
                     this->label3->Text = listView_Indicator->SelectedItems[0]->SubItems[5]->Text+":"+"交易过程中权益的最大值";//最大权益
                     break;
                 case 4:
                     this->label1->Text = listView_Indicator->SelectedItems[0]->SubItems[1]->Text+":"+"净利润/总交易次数（平仓）";//平均利润
                     this->label2->Text = listView_Indicator->SelectedItems[0]->SubItems[3]->Text+":"+"";//净利润/最大亏损(%)
                     this->label3->Text = listView_Indicator->SelectedItems[0]->SubItems[5]->Text+":"+"保证金占用最大值";//最大使用资金
                     break;
                 case 5:
                     this->label1->Text = listView_Indicator->SelectedItems[0]->SubItems[1]->Text+":"+"";//手续费/毛利润(%)
                     this->label2->Text = listView_Indicator->SelectedItems[0]->SubItems[3]->Text+":"+"";//最大盈利/最大亏损(%)
                     this->label3->Text = listView_Indicator->SelectedItems[0]->SubItems[5]->Text+":"+"结束后用户权益";//结束总权益
                     break;
                 case 6:
                     this->label1->Text = listView_Indicator->SelectedItems[0]->SubItems[1]->Text+":"+"未成交挂单的手数";//未成交手数
                     this->label2->Text = listView_Indicator->SelectedItems[0]->SubItems[3]->Text+":"+"盈利次数/总交易次数";//胜率(%)
                     this->label3->Text = listView_Indicator->SelectedItems[0]->SubItems[5]->Text+":"+"";//盈利次数
                     break;
                 case 7:
                     this->label1->Text = listView_Indicator->SelectedItems[0]->SubItems[1]->Text+":"+"";//最大盈利/总盈利(%)
                     this->label2->Text = listView_Indicator->SelectedItems[0]->SubItems[3]->Text+":"+"净利润/初始资金";//收益率(%)
                     this->label3->Text = listView_Indicator->SelectedItems[0]->SubItems[5]->Text+":"+"";//亏损次数
                     break;
                 case 8:
                     this->label1->Text = listView_Indicator->SelectedItems[0]->SubItems[1]->Text+":"+"";//最大亏损/总亏损(%)
                     this->label2->Text = listView_Indicator->SelectedItems[0]->SubItems[3]->Text+":"+"净利润/最大使用资金";//有效收益率(%)
                     this->label3->Text = listView_Indicator->SelectedItems[0]->SubItems[5]->Text+":"+"（净利润/最大使用资金）/（总交易的天数/365）";//年度收益率(%)
                     break;
                 case 9:
                     this->label1->Text = listView_Indicator->SelectedItems[0]->SubItems[1]->Text+":"+"";//盈利次数/总交易次数(%)
                     this->label2->Text = listView_Indicator->SelectedItems[0]->SubItems[3]->Text+":"+"净利润/总交易的天数*30.5";//月度平均盈利
                     this->label3->Text = listView_Indicator->SelectedItems[0]->SubItems[5]->Text+":"+"";//亏损次数/盈利次数(%)
                     break;
                 case 10:
                     this->label1->Text = listView_Indicator->SelectedItems[0]->SubItems[1]->Text+":"+"净利润/总交易的天数*365";//年化收益
                     this->label2->Text = listView_Indicator->SelectedItems[0]->SubItems[3]->Text+":"+"";//亏损次数/总交易次数(%)
                     this->label3->Text = listView_Indicator->SelectedItems[0]->SubItems[5]->Text+":"+"最大连续盈利交易的次数";//最大连续盈利次数
                     break;
                 case 11:
                     this->label1->Text = listView_Indicator->SelectedItems[0]->SubItems[1]->Text+":"+"最大的连续交易亏损数";//最大连续亏损次数
                     this->label2->Text = listView_Indicator->SelectedItems[0]->SubItems[3]->Text+":"+"盈利次数+亏损次数+持平次数";//总交易次数
                     this->label3->Text = listView_Indicator->SelectedItems[0]->SubItems[5]->Text+":"+"交易过程中最大持有仓量";//最大持仓
                     break;
                 case 12:
                     this->label1->Text = listView_Indicator->SelectedItems[0]->SubItems[1]->Text+":"+"1—1/sqrt（交易次数）";//置信度
                     this->label2->Text = listView_Indicator->SelectedItems[0]->SubItems[3]->Text+":"+"报单总数";//总交易委托数
                     this->label3->Text = listView_Indicator->SelectedItems[0]->SubItems[5]->Text+":"+"单笔交易最大盈利";//最大盈利
                     break;
                 case 13:
                     this->label1->Text = listView_Indicator->SelectedItems[0]->SubItems[1]->Text+":"+"";//策略开始时间
                     this->label2->Text = listView_Indicator->SelectedItems[0]->SubItems[3]->Text+":"+"";//策略停止时间
                     break;
                 }
             }
         }
private: System::Void listBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             if(chart_Show==nullptr)
                 return;

             String^ szItem = listBox1->SelectedItem->ToString();
             if(szItem->CompareTo("累计盈亏图")==0)
             {                  
                  chart_Show = chart_ProfitLoss;
                  splitContainer_ChartSummary2->Panel2Collapsed = false;
                  
                  chart_ReportMonth->Hide();
                  chart_ReportYear->Hide();
                  chart_Profit->Hide();
                  chart_Indicator->Hide();
                  chart_ProfitLoss->Show();
             }
             else if(szItem->CompareTo("月度报表")==0)
             {
                 chart_Show = chart_ReportMonth;     
                 splitContainer_ChartSummary2->Panel2Collapsed = false;

                 chart_ReportYear->Hide();
                 chart_Profit->Hide();
                 chart_Indicator->Hide();
                 chart_ProfitLoss->Hide();
                 chart_ReportMonth->Show();      
             }
             else if(szItem->CompareTo("年度报表")==0)
             {
                 chart_Show = chart_ReportYear;
                 splitContainer_ChartSummary2->Panel2Collapsed = false;

                 chart_ReportMonth->Hide();
                 chart_Profit->Hide();
                 chart_Indicator->Hide();
                 chart_ProfitLoss->Hide();
                 chart_ReportYear->Show();
             }
             else if(szItem->CompareTo("净利润图")==0)
             {  
                 chart_Show = chart_Profit;
                 splitContainer_ChartSummary2->Panel2Collapsed = true;
                 
                 chart_ReportMonth->Hide();
                 chart_ReportYear->Hide();
                 chart_Indicator->Hide();
                 chart_ProfitLoss->Hide();

                 chart_Profit->Height = splitContainer_ChartSummary2->Panel1->Height;
                 chart_Profit->Show();
                 //chart_Profit->Update();
             }
             else if(szItem->CompareTo("指标图")==0)
             {
                 chart_Show = chart_Indicator;
                 splitContainer_ChartSummary2->Panel2Collapsed = true;

                 chart_ReportMonth->Hide();
                 chart_ReportYear->Hide();
                 chart_Profit->Hide();
                 chart_ProfitLoss->Hide();

                 chart_Indicator->Height = splitContainer_ChartSummary2->Panel1->Height;
                 chart_Indicator->Show();
                 //chart_Indicator->Update();
             }
         }
private: System::Void splitContainer_ChartSummary2_Panel1_SizeChanged(System::Object^  sender, System::EventArgs^  e) {
             if(chart_Show)
             {
                 chart_Show->Height = splitContainer_ChartSummary2->Panel1->Height;
                 chart_Show->Width = splitContainer_ChartSummary2->Panel1->Width;
                 chart_Show->Update();
             }
         }
private: System::Void treeListView1_DoubleClick(System::Object^  sender, System::EventArgs^  e) {
             if(m_bHistory && treeListView1->SelectedItems->Count>0)
             {                 
                 TreeListViewItem^ item = treeListView1->SelectedItems[0];
                 if(0==item->Level)//
                 {
                     HistoryQueryForm^ dlg = gcnew HistoryQueryForm;
                     if (System::Windows::Forms::DialogResult::OK == dlg->ShowDialog(this))
                     {
                         sQueryCondition* sd = dlg->GetQueryCondition();

                         treeListView1->Items->Remove(m_itemRoot);
                         InitTestRecordListView_history();

                         LoadHistoryData(sd);
                     }

                     m_itemRoot->Expand();
                 }
                 else if(2 == item->Level)
                 {
                     listView_delegateDetail->Items->Clear();
                     listView_tradeDetail->Items->Clear();
                     listView_chart->Items->Clear();
                     listView_StrategyInfo->Items->Clear();
                     

                     TreeListViewItem^ item_parent = item->Parent;
                     m_DataTime = item_parent->Text;

                     String^ szTitle = item->SubItems[0]->Text;
                     szTitle = szTitle->Substring(4,2)+szTitle->Substring(7,2)+szTitle->Substring(10,2);                     
                     String^ hisPath = String(_GetHistoryDataPath().data()).ToString()+"\\"+ item_parent->Text + "\\" + szTitle + "\\";

                     string path = (char*)(Marshal::StringToHGlobalAnsi(hisPath)).ToPointer();
                     LoadHistoryData(path,false);
                     
                 }

                 //String^ szTitle = treeListView1->SelectedItems[0]->SubItems[0]->Text;
                 //System::Diagnostics::Debug::WriteLine(szTitle);
             }
         }
};
}

#pragma once

#include "RiskData.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

/*
extern vector<string> g_vecTreeNodeTitle;//组织机构的名称
extern map<string,vector<string>> g_mapTreeNode2Column;//组织机构名称对应的列表名称

extern std::vector<std::string> g_vecTitleDepthMarket;
extern std::vector<std::string> g_vecTitleRiskEvent;
extern std::vector<std::string> g_vecTitleTradingAccount;
extern std::vector<std::string> g_vecTitleAllOrder;
extern std::vector<std::string> g_vecTitleTrade;
extern std::vector<std::string> g_vecTitlePosition;
extern std::vector<std::string> g_vecTitleUnTrade;
extern std::vector<std::string> g_vecTitleClosePosition;
extern std::vector<std::string> g_vecTitleNeedVerify;
*/
namespace CltRisk {

	/// <summary>
	/// Summary for FormTableColumnSetup
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormTableColumnSetup : public System::Windows::Forms::Form
	{
	public:
		FormTableColumnSetup(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_selectionIdx = 0;
            LoadData();
        }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormTableColumnSetup()
		{
			if (components)
			{
				delete components;
			}
		}
    private: int m_selectionIdx;

    private: System::Windows::Forms::TreeView^  treeView;
    private: System::Windows::Forms::DataGridView^  dataGridView;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
    private: System::Windows::Forms::DataGridViewCheckBoxColumn^  Column2;

    private: System::Windows::Forms::Button^  button_OK;
    private: System::Windows::Forms::Button^  button_Cancel;
    private: System::Windows::Forms::Button^  button_up;
    private: System::Windows::Forms::Button^  button_down;

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
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle3 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            this->treeView = (gcnew System::Windows::Forms::TreeView());
            this->dataGridView = (gcnew System::Windows::Forms::DataGridView());
            this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column2 = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
            this->button_OK = (gcnew System::Windows::Forms::Button());
            this->button_Cancel = (gcnew System::Windows::Forms::Button());
            this->button_up = (gcnew System::Windows::Forms::Button());
            this->button_down = (gcnew System::Windows::Forms::Button());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView))->BeginInit();
            this->SuspendLayout();
            // 
            // treeView
            // 
            this->treeView->HideSelection = false;
            this->treeView->Location = System::Drawing::Point(12, 12);
            this->treeView->Name = L"treeView";
            this->treeView->Size = System::Drawing::Size(144, 273);
            this->treeView->TabIndex = 0;
            this->treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &FormTableColumnSetup::treeView_AfterSelect);
            // 
            // dataGridView
            // 
            this->dataGridView->AllowDrop = true;
            this->dataGridView->AllowUserToAddRows = false;
            this->dataGridView->AllowUserToOrderColumns = true;
            this->dataGridView->AllowUserToResizeRows = false;
            this->dataGridView->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
            dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle1->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle1->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle1->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle1->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            this->dataGridView->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::DisableResizing;
            this->dataGridView->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {this->Column1, 
                this->Column2});
            dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle2->BackColor = System::Drawing::SystemColors::Window;
            dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle2->ForeColor = System::Drawing::SystemColors::ControlText;
            dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::False;
            this->dataGridView->DefaultCellStyle = dataGridViewCellStyle2;
            this->dataGridView->EnableHeadersVisualStyles = false;
            this->dataGridView->Location = System::Drawing::Point(184, 12);
            this->dataGridView->MultiSelect = false;
            this->dataGridView->Name = L"dataGridView";
            this->dataGridView->RowHeadersVisible = false;
            this->dataGridView->RowHeadersWidthSizeMode = System::Windows::Forms::DataGridViewRowHeadersWidthSizeMode::DisableResizing;
            dataGridViewCellStyle3->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            this->dataGridView->RowsDefaultCellStyle = dataGridViewCellStyle3;
            this->dataGridView->RowTemplate->Height = 23;
            this->dataGridView->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView->Size = System::Drawing::Size(166, 273);
            this->dataGridView->TabIndex = 1;
            this->dataGridView->CellMouseMove += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &FormTableColumnSetup::dataGridView_CellMouseMove);
            this->dataGridView->RowsAdded += gcnew System::Windows::Forms::DataGridViewRowsAddedEventHandler(this, &FormTableColumnSetup::dataGridView_RowsAdded);
            this->dataGridView->DragEnter += gcnew System::Windows::Forms::DragEventHandler(this, &FormTableColumnSetup::dataGridView_DragEnter);
            this->dataGridView->DragDrop += gcnew System::Windows::Forms::DragEventHandler(this, &FormTableColumnSetup::dataGridView_DragDrop);
            // 
            // Column1
            // 
            this->Column1->Frozen = true;
            this->Column1->HeaderText = L"标题";
            this->Column1->Name = L"Column1";
            this->Column1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            this->Column1->Width = 35;
            // 
            // Column2
            // 
            this->Column2->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::AllCells;
            this->Column2->HeaderText = L"显示";
            this->Column2->Name = L"Column2";
            this->Column2->Width = 35;
            // 
            // button_OK
            // 
            this->button_OK->Location = System::Drawing::Point(97, 302);
            this->button_OK->Name = L"button_OK";
            this->button_OK->Size = System::Drawing::Size(75, 23);
            this->button_OK->TabIndex = 2;
            this->button_OK->Text = L"确定";
            this->button_OK->UseVisualStyleBackColor = true;
            this->button_OK->Click += gcnew System::EventHandler(this, &FormTableColumnSetup::button_OK_Click);
            // 
            // button_Cancel
            // 
            this->button_Cancel->Location = System::Drawing::Point(190, 302);
            this->button_Cancel->Name = L"button_Cancel";
            this->button_Cancel->Size = System::Drawing::Size(75, 23);
            this->button_Cancel->TabIndex = 3;
            this->button_Cancel->Text = L"取消";
            this->button_Cancel->UseVisualStyleBackColor = true;
            this->button_Cancel->Click += gcnew System::EventHandler(this, &FormTableColumnSetup::button_Cancel_Click);
            // 
            // button_up
            // 
            this->button_up->Location = System::Drawing::Point(358, 98);
            this->button_up->Name = L"button_up";
            this->button_up->Size = System::Drawing::Size(46, 23);
            this->button_up->TabIndex = 4;
            this->button_up->Text = L"上移";
            this->button_up->UseVisualStyleBackColor = true;
            this->button_up->Click += gcnew System::EventHandler(this, &FormTableColumnSetup::button_up_Click);
            // 
            // button_down
            // 
            this->button_down->Location = System::Drawing::Point(358, 174);
            this->button_down->Name = L"button_down";
            this->button_down->Size = System::Drawing::Size(46, 23);
            this->button_down->TabIndex = 4;
            this->button_down->Text = L"下移";
            this->button_down->UseVisualStyleBackColor = true;
            this->button_down->Click += gcnew System::EventHandler(this, &FormTableColumnSetup::button_down_Click);
            // 
            // FormTableColumnSetup
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(417, 337);
            this->Controls->Add(this->button_down);
            this->Controls->Add(this->button_up);
            this->Controls->Add(this->button_Cancel);
            this->Controls->Add(this->button_OK);
            this->Controls->Add(this->dataGridView);
            this->Controls->Add(this->treeView);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->Name = L"FormTableColumnSetup";
            this->Text = L"表格列设置";
            this->Load += gcnew System::EventHandler(this, &FormTableColumnSetup::FormTableColumnSetup_Load);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView))->EndInit();
            this->ResumeLayout(false);

        }
#pragma endregion
private: void LoadTree() 
{  /*    
    if(g_vecTreeNodeTitle.empty())
        return;

    TreeNode^ node;
    for (int i=0;i<(int)g_vecTreeNodeTitle.size();i++)
    {           
        if(i==0)
        {                    
            node = treeView->Nodes->Add(i.ToString(), Tools::string2String(g_vecTreeNodeTitle[i].c_str()));
            node->Tag = i;
        }
        else
        {
            TreeNode^ subNode;
            subNode = node->Nodes->Add(i.ToString(),Tools::string2String(g_vecTreeNodeTitle[i].c_str()));
            subNode->Tag = i;
        }
    }

    node->Expand();*/
}
private: void LoadData()
      {/*
          vector<string> vecColumnText;
          
          //所有列数据           
          g_vecTreeNodeTitle.push_back("表格列设置");
          g_vecTreeNodeTitle.push_back("报价表");
          g_vecTreeNodeTitle.push_back("当前风险事件");
          g_vecTreeNodeTitle.push_back("账户及资金");
          g_vecTreeNodeTitle.push_back("委托单");
          g_vecTreeNodeTitle.push_back("未成交单");
          g_vecTreeNodeTitle.push_back("成交单");
          g_vecTreeNodeTitle.push_back("持仓");
          g_vecTreeNodeTitle.push_back("平仓");
          g_vecTreeNodeTitle.push_back("待审核单");

          
          vecColumnText.push_back("合约");
          vecColumnText.push_back("最新价");
          vecColumnText.push_back("涨跌");
          vecColumnText.push_back("涨跌幅");
          vecColumnText.push_back("买价");
          vecColumnText.push_back("买量");
          vecColumnText.push_back("卖价");
          vecColumnText.push_back("卖量");
          vecColumnText.push_back("成交量");
          vecColumnText.push_back("持仓量");
          vecColumnText.push_back("现量");
          vecColumnText.push_back("今开盘");
          vecColumnText.push_back("昨结算");
          vecColumnText.push_back("最高价");
          vecColumnText.push_back("最低价");
          vecColumnText.push_back("涨停价");
          vecColumnText.push_back("跌停价");
          vecColumnText.push_back("更新时间");
          g_mapTreeNode2Column.insert(make_pair("报价表",vecColumnText));

          vecColumnText.clear();
          vecColumnText.push_back("风险事件ID");
          vecColumnText.push_back("交易员ID");
          vecColumnText.push_back("交易员名称");
          vecColumnText.push_back("风险指标类型");
          vecColumnText.push_back("风险详情");
          vecColumnText.push_back("风险事件状态");
          g_mapTreeNode2Column.insert(make_pair("当前风险事件",vecColumnText));

          vecColumnText.clear();
          vecColumnText.push_back("交易员");
          vecColumnText.push_back("交易限制");
          vecColumnText.push_back("手动审核");
          vecColumnText.push_back("登录状态");
          vecColumnText.push_back("风险度");
          vecColumnText.push_back("动态权益");
          vecColumnText.push_back("持仓盈亏");
          vecColumnText.push_back("平仓盈亏");
          vecColumnText.push_back("占用保证金");
          vecColumnText.push_back("手续费");
          vecColumnText.push_back("冻结保证金");
          vecColumnText.push_back("冻结手续费");
          g_mapTreeNode2Column.insert(make_pair("账户及资金",vecColumnText));

          vecColumnText.clear();
          vecColumnText.push_back("交易员");
          vecColumnText.push_back("报单编号");
          vecColumnText.push_back("合约");
          vecColumnText.push_back("买卖");
          vecColumnText.push_back("开平");
          vecColumnText.push_back("挂单状态");
          vecColumnText.push_back("报单价格");
          vecColumnText.push_back("报单手数");
          vecColumnText.push_back("未成交手数");
          vecColumnText.push_back("成交手数");
          vecColumnText.push_back("详细状态");
          vecColumnText.push_back("报单时间");
          vecColumnText.push_back("冻结保证金");
          vecColumnText.push_back("冻结手续费");
          vecColumnText.push_back("投保");
          vecColumnText.push_back("交易所");
          vecColumnText.push_back("前置机");
          vecColumnText.push_back("会话机");
          vecColumnText.push_back("报单引用");
          vecColumnText.push_back("状态");
          g_mapTreeNode2Column.insert(make_pair("委托单",vecColumnText));
          g_mapTreeNode2Column.insert(make_pair("未成交单",vecColumnText));
          g_mapTreeNode2Column.insert(make_pair("待审核单",vecColumnText));

          vecColumnText.clear();
          vecColumnText.push_back("交易员");
          vecColumnText.push_back("合约");
          vecColumnText.push_back("买卖");
          vecColumnText.push_back("开平");
          vecColumnText.push_back("成交价格");
          vecColumnText.push_back("成交手数");
          vecColumnText.push_back("手续费");
          vecColumnText.push_back("成交时间");
          vecColumnText.push_back("成交类型");
          vecColumnText.push_back("投保");
          vecColumnText.push_back("成交编号");
          vecColumnText.push_back("报单编号");
          vecColumnText.push_back("交易所");
          g_mapTreeNode2Column.insert(make_pair("成交单",vecColumnText));
          g_mapTreeNode2Column.insert(make_pair("平仓",vecColumnText));

          vecColumnText.clear();
          vecColumnText.push_back("交易员");
          vecColumnText.push_back("合约");
          vecColumnText.push_back("买卖");
          vecColumnText.push_back("投保");
          vecColumnText.push_back("总持仓");
          vecColumnText.push_back("可平量");
          vecColumnText.push_back("现价");
          vecColumnText.push_back("持仓均价");
          vecColumnText.push_back("开仓均价");
          vecColumnText.push_back("持仓盈亏");
          vecColumnText.push_back("浮动盈亏");
          vecColumnText.push_back("盯市总盈亏");
          vecColumnText.push_back("占用保证金");
          vecColumnText.push_back("总多仓");
          vecColumnText.push_back("总空仓");
          vecColumnText.push_back("今多仓");
          vecColumnText.push_back("今空仓");
          vecColumnText.push_back("昨仓");
          vecColumnText.push_back("今仓");
          vecColumnText.push_back("今开仓量");
          vecColumnText.push_back("今平仓量");
          vecColumnText.push_back("总可平今量");
          vecColumnText.push_back("总可平昨量");
          vecColumnText.push_back("平今挂单量");
          vecColumnText.push_back("平仓挂单量");
          vecColumnText.push_back("可平今量");
          vecColumnText.push_back("可平昨量");
          vecColumnText.push_back("交易所");
          g_mapTreeNode2Column.insert(make_pair("持仓",vecColumnText));              


          //配置文件中的数据
          string szDataGridView = "";
          int nColumnIndex = -1;
          vector<string> vecColumn;

          map<string,vector<string>> mapDataGridView2Column = CRiskData::GetInstance()->GetConfigFile(XML_TYPE_COLUMN);
           for(map<string,vector<string>>::iterator it = mapDataGridView2Column.begin();it != mapDataGridView2Column.end();it++)
           {
               szDataGridView = it->first;
               if(0==strcmp(szDataGridView.c_str(),DataGridView_DepthMarket))
               {
                   vecColumn = it->second;                     
                   for (vector<string>::iterator it_column = vecColumn.begin();it_column != vecColumn.end();it_column++)
                   {
                       nColumnIndex = atoi((*it_column).c_str());
                       if(nColumnIndex == conCol_DepthMarket_InstrumentID)     {g_vecTitleDepthMarket.push_back("合约");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_DepthMarket_LastPrice)        {g_vecTitleDepthMarket.push_back("最新价");continue;}
                       if(nColumnIndex == conCol_DepthMarket_Mixed)            {g_vecTitleDepthMarket.push_back("涨跌");continue;}
                       if(nColumnIndex == conCol_DepthMarket_MixedRange)       {g_vecTitleDepthMarket.push_back("涨跌幅");continue;}
                       if(nColumnIndex == conCol_DepthMarket_BidPrice1)        {g_vecTitleDepthMarket.push_back("买价");continue;}
                       if(nColumnIndex == conCol_DepthMarket_BidVolume1)       {g_vecTitleDepthMarket.push_back("买量");continue;}
                       if(nColumnIndex == conCol_DepthMarket_AskPrice1)        {g_vecTitleDepthMarket.push_back("卖价");continue;}
                       if(nColumnIndex == conCol_DepthMarket_AskVolume1)       {g_vecTitleDepthMarket.push_back("卖量");continue;}
                       if(nColumnIndex == conCol_DepthMarket_Volume)           {g_vecTitleDepthMarket.push_back("成交量");continue;}
                       if(nColumnIndex == conCol_DepthMarket_OpenInterest)     {g_vecTitleDepthMarket.push_back("持仓量");continue;}
                       if(nColumnIndex == conCol_DepthMarket_CurVolume)        {g_vecTitleDepthMarket.push_back("现量");continue;}
                       if(nColumnIndex == conCol_DepthMarket_OpenPrice)        {g_vecTitleDepthMarket.push_back("今开盘");continue;}
                       if(nColumnIndex == conCol_DepthMarket_SettlementPrice)  {g_vecTitleDepthMarket.push_back("昨结算");continue;}
                       if(nColumnIndex == conCol_DepthMarket_HighestPrice)     {g_vecTitleDepthMarket.push_back("最高价");continue;}
                       if(nColumnIndex == conCol_DepthMarket_LowestPrice)      {g_vecTitleDepthMarket.push_back("最低价");continue;}
                       if(nColumnIndex == conCol_DepthMarket_UpperLimitPrice)  {g_vecTitleDepthMarket.push_back("涨停价");continue;}
                       if(nColumnIndex == conCol_DepthMarket_LowerLimitPrice)  {g_vecTitleDepthMarket.push_back("跌停价");continue;}
                       if(nColumnIndex == conCol_DepthMarket_UpdateTime)       {g_vecTitleDepthMarket.push_back("更新时间");continue;}
                   }

                   continue;
               }else if(0==strcmp(szDataGridView.c_str(),DataGridView_RiskEvent))
               {
                   vecColumn = it->second;
                   for (vector<string>::iterator it_column = vecColumn.begin();it_column != vecColumn.end();it_column++)
                   {
                       nColumnIndex = atoi((*it_column).c_str());
                       if(nColumnIndex == conCol_RiskEvent_RiskEventID)     {g_vecTitleRiskEvent.push_back("风险事件ID");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_RiskEvent_TradeInvestorID) {g_vecTitleRiskEvent.push_back("交易员ID");continue;}
                       if(nColumnIndex == conCol_RiskEvent_TraderName)      {g_vecTitleRiskEvent.push_back("交易员名称");continue;}
                       if(nColumnIndex == conCol_RiskEvent_IndicatorName)   {g_vecTitleRiskEvent.push_back("风险指标类型");continue;}
                       if(nColumnIndex == conCol_RiskEvent_RiskDesc)        {g_vecTitleRiskEvent.push_back("风险详情");continue;}
                       if(nColumnIndex == conCol_RiskEvent_EventTime)       {g_vecTitleRiskEvent.push_back("更新时间");continue;}
                       if(nColumnIndex == conCol_RiskEvent_EventStatus)     {g_vecTitleRiskEvent.push_back("风险事件状态");continue;}
                   }

                   continue;
               }else if(0==strcmp(szDataGridView.c_str(),DataGridView_TradingAccount))
               {
                   vecColumn = it->second;
                   for (vector<string>::iterator it_column = vecColumn.begin();it_column != vecColumn.end();it_column++)
                   {
                       nColumnIndex = atoi((*it_column).c_str());
                       if(nColumnIndex == conCol_TradingAccount_Account)           {g_vecTitleTradingAccount.push_back("交易员");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_TradingAccount_LimitTade)         {g_vecTitleTradingAccount.push_back("交易限制");continue;}
                       if(nColumnIndex == conCol_TradingAccount_ManuVerify)        {g_vecTitleTradingAccount.push_back("手动审核");continue;}
                       if(nColumnIndex == conCol_TradingAccount_LoginStatus)       {g_vecTitleTradingAccount.push_back("登录状态");continue;}
                       if(nColumnIndex == conCol_TradingAccount_RiskDegree)        {g_vecTitleTradingAccount.push_back("风险度(%)");continue;}
                       if(nColumnIndex == conCol_TradingAccount_DynamicProfit)     {g_vecTitleTradingAccount.push_back("动态权益");continue;}
                       if(nColumnIndex == conCol_TradingAccount_PositionProfit)    {g_vecTitleTradingAccount.push_back("持仓盈亏");continue;}
                       if(nColumnIndex == conCol_TradingAccount_CloseProfit)       {g_vecTitleTradingAccount.push_back("平仓盈亏");continue;}
                       if(nColumnIndex == conCol_TradingAccount_CurrMargin)        {g_vecTitleTradingAccount.push_back("占用保证金");continue;}
                       if(nColumnIndex == conCol_TradingAccount_Commission)        {g_vecTitleTradingAccount.push_back("手续费");continue;}
                       if(nColumnIndex == conCol_TradingAccount_FrozenMargin)      {g_vecTitleTradingAccount.push_back("冻结保证金");continue;}
                       if(nColumnIndex == conCol_TradingAccount_FrozenCommission)  {g_vecTitleTradingAccount.push_back("冻结手续费");continue;}
                   }

                   continue;
               }else if(0==strcmp(szDataGridView.c_str(),DataGridView_AllOrder))
               {
                   vecColumn = it->second;
                   for (vector<string>::iterator it_column = vecColumn.begin();it_column != vecColumn.end();it_column++)
                   {
                       nColumnIndex = atoi((*it_column).c_str());
                       if(nColumnIndex == conCol_AllOrder_InvestorID)          {g_vecTitleAllOrder.push_back("交易员");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_AllOrder_OrderSysID)          {g_vecTitleAllOrder.push_back("报单编号");continue;}
                       if(nColumnIndex == conCol_AllOrder_InstrumentID)        {g_vecTitleAllOrder.push_back("合约");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_AllOrder_Direction)           {g_vecTitleAllOrder.push_back("买卖");continue;}
                       if(nColumnIndex == conCol_AllOrder_CombOffsetFlag)      {g_vecTitleAllOrder.push_back("开平");continue;}
                       if(nColumnIndex == conCol_AllOrder_OrderStatus)         {g_vecTitleAllOrder.push_back("挂单状态");continue;}
                       if(nColumnIndex == conCol_AllOrder_LimitPrice)          {g_vecTitleAllOrder.push_back("报单价格");continue;}
                       if(nColumnIndex == conCol_AllOrder_VolumeTotalOriginal) {g_vecTitleAllOrder.push_back("报单手数");continue;}
                       if(nColumnIndex == conCol_AllOrder_VolumeTotal)         {g_vecTitleAllOrder.push_back("未成交手数");continue;}
                       if(nColumnIndex == conCol_AllOrder_Volume)              {g_vecTitleAllOrder.push_back("成交手数");continue;}
                       if(nColumnIndex == conCol_AllOrder_StatusMsg)           {g_vecTitleAllOrder.push_back("详细状态");continue;}
                       if(nColumnIndex == conCol_AllOrder_InsertTime)          {g_vecTitleAllOrder.push_back("报单时间");continue;}
                       if(nColumnIndex == conCol_AllOrder_freezeMargin)        {g_vecTitleAllOrder.push_back("冻结保证金");continue;}
                       if(nColumnIndex == conCol_AllOrder_troubleMoney)        {g_vecTitleAllOrder.push_back("冻结手续费");continue;}
                       if(nColumnIndex == conCol_AllOrder_HedgeFlag)           {g_vecTitleAllOrder.push_back("投保");continue;}
                       if(nColumnIndex == conCol_AllOrder_ExchangeID)          {g_vecTitleAllOrder.push_back("交易所");continue;}
                       if(nColumnIndex == conCol_AllOrder_FrontID)             {g_vecTitleAllOrder.push_back("前置机");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_AllOrder_SessionID)           {g_vecTitleAllOrder.push_back("会话机");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_AllOrder_OrderRef)            {g_vecTitleAllOrder.push_back("报单引用");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_AllOrder_ExStatus)            {g_vecTitleAllOrder.push_back("状态");continue;}//RowFilter 用于未成交列表和审核列表
                   }

                   continue;
               }else if(0==strcmp(szDataGridView.c_str(),DataGridView_Trade))
               {
                   vecColumn = it->second;
                   for (vector<string>::iterator it_column = vecColumn.begin();it_column != vecColumn.end();it_column++)
                   {
                       nColumnIndex = atoi((*it_column).c_str());
                       if(nColumnIndex == conCol_Trade_InvestorID)         {g_vecTitleTrade.push_back("交易员");continue;}
                       if(nColumnIndex == conCol_Trade_InstrumentID)       {g_vecTitleTrade.push_back("合约");continue;}
                       if(nColumnIndex == conCol_Trade_Direction)          {g_vecTitleTrade.push_back("买卖");continue;}
                       if(nColumnIndex == conCol_Trade_OffsetFlag)         {g_vecTitleTrade.push_back("开平");continue;}
                       if(nColumnIndex == conCol_Trade_Price)              {g_vecTitleTrade.push_back("成交价格");continue;}
                       if(nColumnIndex == conCol_Trade_Volume)             {g_vecTitleTrade.push_back("成交手数");continue;}
                       if(nColumnIndex == conCol_Trade_TradeCommission)    {g_vecTitleTrade.push_back("手续费");continue;}
                       if(nColumnIndex == conCol_Trade_TradeTime)          {g_vecTitleTrade.push_back("成交时间");continue;}
                       if(nColumnIndex == conCol_Trade_TradeType)          {g_vecTitleTrade.push_back("成交类型");continue;}
                       if(nColumnIndex == conCol_Trade_HedgeFlag)          {g_vecTitleTrade.push_back("投保");continue;}
                       if(nColumnIndex == conCol_Trade_TradeID)            {g_vecTitleTrade.push_back("成交编号");continue;}
                       if(nColumnIndex == conCol_Trade_OrderSysID)         {g_vecTitleTrade.push_back("报单编号");continue;}
                       if(nColumnIndex == conCol_Trade_ExchangeID)         {g_vecTitleTrade.push_back("交易所");continue;}
                   }

                   continue;
               }else if(0==strcmp(szDataGridView.c_str(),DataGridView_Position))
               {
                   vecColumn = it->second;
                   for (vector<string>::iterator it_column = vecColumn.begin();it_column != vecColumn.end();it_column++)
                   {
                       nColumnIndex = atoi((*it_column).c_str());
                       if(nColumnIndex == conCol_Position_InvestorID)                  {g_vecTitlePosition.push_back("交易员");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_Position_InstrumentID)                {g_vecTitlePosition.push_back("合约");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_Position_Direction)                   {g_vecTitlePosition.push_back("买卖");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_Position_HedgeFlag)                   {g_vecTitlePosition.push_back("投保");continue;}//PrimaryKey
                       if(nColumnIndex == conCol_Position_Position)                    {g_vecTitlePosition.push_back("总持仓");continue;}
                       if(nColumnIndex == conCol_Position_CanCloseVolume)              {g_vecTitlePosition.push_back("可平量");continue;}
                       if(nColumnIndex == conCol_Position_LastPrice)                   {g_vecTitlePosition.push_back("现价");continue;}
                       if(nColumnIndex == conCol_Position_AveragePositionPrice)        {g_vecTitlePosition.push_back("持仓均价");continue;}
                       if(nColumnIndex == conCol_Position_AverageOpenPrice)            {g_vecTitlePosition.push_back("开仓均价");continue;}
                       if(nColumnIndex == conCol_Position_PositionProfit)              {g_vecTitlePosition.push_back("持仓盈亏");continue;}
                       if(nColumnIndex == conCol_Position_PositionProfitByTrade)       {g_vecTitlePosition.push_back("浮动盈亏");continue;}
                       if(nColumnIndex == conCol_Position_TotalPositionProfitByDate)   {g_vecTitlePosition.push_back("盯市总盈亏");continue;}
                       if(nColumnIndex == conCol_Position_UseMargin)                   {g_vecTitlePosition.push_back("占用保证金");continue;}
                       if(nColumnIndex == conCol_Position_MultiBuyPosition)            {g_vecTitlePosition.push_back("总多仓");continue;}
                       if(nColumnIndex == conCol_Position_MultiSellPosition)           {g_vecTitlePosition.push_back("总空仓");continue;}
                       if(nColumnIndex == conCol_Position_TodayBuyPosition)            {g_vecTitlePosition.push_back("今多仓");continue;}
                       if(nColumnIndex == conCol_Position_TodaySellPosition)           {g_vecTitlePosition.push_back("今空仓");continue;}
                       if(nColumnIndex == conCol_Position_YesterdayPosition)           {g_vecTitlePosition.push_back("昨仓");continue;}
                       if(nColumnIndex == conCol_Position_TodayPosition)               {g_vecTitlePosition.push_back("今仓");continue;}
                       if(nColumnIndex == conCol_Position_OpenVolume)                  {g_vecTitlePosition.push_back("今开仓量");continue;}
                       if(nColumnIndex == conCol_Position_CloseVolume)                 {g_vecTitlePosition.push_back("今平仓量");continue;}
                       if(nColumnIndex == conCol_Position_TotalCanCloseTodayVolume)    {g_vecTitlePosition.push_back("总可平今量");continue;}
                       if(nColumnIndex == conCol_Position_TotalCanCloseYesterdayVolume){g_vecTitlePosition.push_back("总可平昨量");continue;}
                       if(nColumnIndex == conCol_Position_CloseTodayOrderVolume)       {g_vecTitlePosition.push_back("平今挂单量");continue;}
                       if(nColumnIndex == conCol_Position_CloseOrderVolume)            {g_vecTitlePosition.push_back("平仓挂单量");continue;}
                       if(nColumnIndex == conCol_Position_CanCloseTodayVolume)         {g_vecTitlePosition.push_back("可平今量");continue;}
                       if(nColumnIndex == conCol_Position_CanCloseYesterdayVolume)     {g_vecTitlePosition.push_back("可平昨量");continue;}
                       if(nColumnIndex == conCol_Position_ExchangeID)                  {g_vecTitlePosition.push_back("交易所");continue;}
                   }

                   continue;
               }
           }*/
      } 
private: int GetTagFromText(String^ szText)
         {
             if(szText->CompareTo("合约")==0)           return conCol_DepthMarket_InstrumentID;//PrimaryKey
             if(szText->CompareTo("最新价")==0)         return conCol_DepthMarket_LastPrice;
             if(szText->CompareTo("涨跌")==0)           return conCol_DepthMarket_Mixed;
             if(szText->CompareTo("涨跌幅")==0)         return conCol_DepthMarket_MixedRange;
             if(szText->CompareTo("买价")==0)           return conCol_DepthMarket_BidPrice1;
             if(szText->CompareTo("买量")==0)           return conCol_DepthMarket_BidVolume1;
             if(szText->CompareTo("卖价")==0)           return conCol_DepthMarket_AskPrice1;
             if(szText->CompareTo("卖量")==0)           return conCol_DepthMarket_AskVolume1;
             if(szText->CompareTo("成交量")==0)         return conCol_DepthMarket_Volume;
             if(szText->CompareTo("持仓量")==0)         return conCol_DepthMarket_OpenInterest;
             if(szText->CompareTo("现量")==0)           return conCol_DepthMarket_CurVolume;
             if(szText->CompareTo("今开盘")==0)         return conCol_DepthMarket_OpenPrice;
             if(szText->CompareTo("昨结算")==0)         return conCol_DepthMarket_SettlementPrice;
             if(szText->CompareTo("最高价")==0)         return conCol_DepthMarket_HighestPrice;
             if(szText->CompareTo("最低价")==0)         return conCol_DepthMarket_LowestPrice;
             if(szText->CompareTo("涨停价")==0)         return conCol_DepthMarket_UpperLimitPrice;
             if(szText->CompareTo("跌停价")==0)         return conCol_DepthMarket_LowerLimitPrice;
             if(szText->CompareTo("更新时间")==0)       return conCol_DepthMarket_UpdateTime;

             if(szText->CompareTo("风险事件ID")==0)     return conCol_RiskEvent_RiskEventID;//PrimaryKey
             if(szText->CompareTo("交易员ID")==0)       return conCol_RiskEvent_TradeInvestorID;
             if(szText->CompareTo("交易员名称")==0)     return conCol_RiskEvent_TraderName;
             if(szText->CompareTo("风险指标类型")==0)   return conCol_RiskEvent_IndicatorName;
             if(szText->CompareTo("风险详情")==0)       return conCol_RiskEvent_RiskDesc;
             if(szText->CompareTo("更新时间")==0)       return conCol_RiskEvent_EventTime;
             if(szText->CompareTo("风险事件状态")==0)   return conCol_RiskEvent_EventStatus;

             if(szText->CompareTo("交易员")==0)         return conCol_TradingAccount_Account;//PrimaryKey
             if(szText->CompareTo("交易限制")==0)       return conCol_TradingAccount_LimitTade;
             if(szText->CompareTo("手动审核")==0)       return conCol_TradingAccount_ManuVerify;
             if(szText->CompareTo("登录状态")==0)       return conCol_TradingAccount_LoginStatus;
             if(szText->CompareTo("风险度(%)")==0)      return conCol_TradingAccount_RiskDegree;
             if(szText->CompareTo("动态权益")==0)       return conCol_TradingAccount_DynamicProfit;
             if(szText->CompareTo("持仓盈亏")==0)       return conCol_TradingAccount_PositionProfit;
             if(szText->CompareTo("平仓盈亏")==0)       return conCol_TradingAccount_CloseProfit;
             if(szText->CompareTo("占用保证金")==0)     return conCol_TradingAccount_CurrMargin;
             if(szText->CompareTo("手续费")==0)         return conCol_TradingAccount_Commission;
             if(szText->CompareTo("冻结保证金")==0)     return conCol_TradingAccount_FrozenMargin;
             if(szText->CompareTo("冻结手续费")==0)     return conCol_TradingAccount_FrozenCommission;

             if(szText->CompareTo("交易员")==0)         return conCol_AllOrder_InvestorID;//PrimaryKey
             if(szText->CompareTo("报单编号")==0)       return conCol_AllOrder_OrderSysID;
             if(szText->CompareTo("合约")==0)           return conCol_AllOrder_InstrumentID;//PrimaryKey
             if(szText->CompareTo("买卖")==0)           return conCol_AllOrder_Direction;
             if(szText->CompareTo("开平")==0)           return conCol_AllOrder_CombOffsetFlag;
             if(szText->CompareTo("挂单状态")==0)       return conCol_AllOrder_OrderStatus;
             if(szText->CompareTo("报单价格")==0)       return conCol_AllOrder_LimitPrice;
             if(szText->CompareTo("报单手数")==0)       return conCol_AllOrder_VolumeTotalOriginal;
             if(szText->CompareTo("未成交手数")==0)     return conCol_AllOrder_VolumeTotal;
             if(szText->CompareTo("成交手数")==0)       return conCol_AllOrder_Volume;
             if(szText->CompareTo("成交手数")==0)       return conCol_AllOrder_Volume;
             if(szText->CompareTo("成交手数")==0)       return conCol_AllOrder_Volume;
             if(szText->CompareTo("详细状态")==0)       return conCol_AllOrder_StatusMsg;
             if(szText->CompareTo("报单时间")==0)       return conCol_AllOrder_InsertTime;
             if(szText->CompareTo("冻结保证金")==0)     return conCol_AllOrder_freezeMargin;
             if(szText->CompareTo("冻结手续费")==0)     return conCol_AllOrder_troubleMoney;
             if(szText->CompareTo("投保")==0)           return conCol_AllOrder_HedgeFlag;
             if(szText->CompareTo("交易所")==0)         return conCol_AllOrder_ExchangeID;
             if(szText->CompareTo("前置机")==0)         return conCol_AllOrder_FrontID;//PrimaryKey
             if(szText->CompareTo("会话机")==0)         return conCol_AllOrder_SessionID;//PrimaryKey
             if(szText->CompareTo("报单引用")==0)       return conCol_AllOrder_OrderRef;//PrimaryKey
             if(szText->CompareTo("状态")==0)           return conCol_AllOrder_ExStatus;//RowFilter 用于未成交列表和审核列表

             if(szText->CompareTo("交易员")==0)         return conCol_Trade_InvestorID;
             if(szText->CompareTo("合约")==0)           return conCol_Trade_InstrumentID;
             if(szText->CompareTo("买卖")==0)           return conCol_Trade_Direction;
             if(szText->CompareTo("开平")==0)           return conCol_Trade_OffsetFlag;
             if(szText->CompareTo("成交价格")==0)       return conCol_Trade_Price;
             if(szText->CompareTo("成交手数")==0)       return conCol_Trade_Volume;
             if(szText->CompareTo("手续费")==0)         return conCol_Trade_TradeCommission;
             if(szText->CompareTo("成交时间")==0)       return conCol_Trade_TradeTime;
             if(szText->CompareTo("成交类型")==0)       return conCol_Trade_TradeType;
             if(szText->CompareTo("投保")==0)           return conCol_Trade_HedgeFlag;
             if(szText->CompareTo("成交编号")==0)       return conCol_Trade_TradeID;
             if(szText->CompareTo("报单编号")==0)       return conCol_Trade_OrderSysID;
             if(szText->CompareTo("交易所")==0)         return conCol_Trade_ExchangeID;

             if(szText->CompareTo("交易员")==0)         return conCol_Position_InvestorID;//PrimaryKey
             if(szText->CompareTo("合约")==0)           return conCol_Position_InstrumentID;//PrimaryKey
             if(szText->CompareTo("买卖")==0)           return conCol_Position_Direction;//PrimaryKey
             if(szText->CompareTo("投保")==0)           return conCol_Position_HedgeFlag;//PrimaryKey
             if(szText->CompareTo("总持仓")==0)         return conCol_Position_Position;
             if(szText->CompareTo("可平量")==0)         return conCol_Position_CanCloseVolume;
             if(szText->CompareTo("现价")==0)           return conCol_Position_LastPrice;
             if(szText->CompareTo("持仓均价")==0)       return conCol_Position_AveragePositionPrice;
             if(szText->CompareTo("开仓均价")==0)       return conCol_Position_AverageOpenPrice;
             if(szText->CompareTo("持仓盈亏")==0)       return conCol_Position_PositionProfit;
             if(szText->CompareTo("浮动盈亏")==0)       return conCol_Position_PositionProfitByTrade;
             if(szText->CompareTo("盯市总盈亏")==0)     return conCol_Position_TotalPositionProfitByDate;
             if(szText->CompareTo("占用保证金")==0)     return conCol_Position_UseMargin;
             if(szText->CompareTo("总多仓")==0)         return conCol_Position_MultiBuyPosition;
             if(szText->CompareTo("总空仓")==0)         return conCol_Position_MultiSellPosition;
             if(szText->CompareTo("今多仓")==0)         return conCol_Position_TodayBuyPosition;
             if(szText->CompareTo("今空仓")==0)         return conCol_Position_TodaySellPosition;
             if(szText->CompareTo("昨仓")==0)           return conCol_Position_YesterdayPosition;
             if(szText->CompareTo("今仓")==0)           return conCol_Position_TodayPosition;
             if(szText->CompareTo("今开仓量")==0)       return conCol_Position_OpenVolume;
             if(szText->CompareTo("今平仓量")==0)       return conCol_Position_CloseVolume;
             if(szText->CompareTo("总可平今量")==0)     return conCol_Position_TotalCanCloseTodayVolume;
             if(szText->CompareTo("总可平昨量")==0)     return conCol_Position_TotalCanCloseYesterdayVolume;
             if(szText->CompareTo("平今挂单量")==0)     return conCol_Position_CloseTodayOrderVolume;
             if(szText->CompareTo("平仓挂单量")==0)     return conCol_Position_CloseOrderVolume;
             if(szText->CompareTo("可平今量")==0)       return conCol_Position_CanCloseTodayVolume;
             if(szText->CompareTo("可平昨量")==0)       return conCol_Position_CanCloseYesterdayVolume;
             if(szText->CompareTo("交易所")==0)         return conCol_Position_ExchangeID;

             return conCol_Position_None;
         }
private: System::Void FormTableColumnSetup_Load(System::Object^  sender, System::EventArgs^  e) {
             CenterToParent();
             LoadTree();
         }
private: bool IsChecked(String^ szTreeText,String^ szColumnText)
          {/*
              if(szTreeText->CompareTo("报价表") == 0)
              {
                  vector<string>::iterator it = g_vecTitleDepthMarket.begin();
                  for (;it!=g_vecTitleDepthMarket.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("当前风险事件") == 0)
              {
                  vector<string>::iterator it = g_vecTitleRiskEvent.begin();
                  for (;it!=g_vecTitleRiskEvent.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("账户及资金") == 0)
              {
                  vector<string>::iterator it = g_vecTitleTradingAccount.begin();
                  for (;it!=g_vecTitleTradingAccount.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("委托单") == 0)
              {
                  vector<string>::iterator it = g_vecTitleAllOrder.begin();
                  for (;it!=g_vecTitleAllOrder.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("未成交单") == 0)
              {
                  vector<string>::iterator it = g_vecTitleUnTrade.begin();
                  for (;it!=g_vecTitleUnTrade.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("成交单") == 0)
              {
                  vector<string>::iterator it = g_vecTitleTrade.begin();
                  for (;it!=g_vecTitleTrade.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("持仓") == 0)
              {
                  vector<string>::iterator it = g_vecTitlePosition.begin();
                  for (;it!=g_vecTitlePosition.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("平仓") == 0)
              {
                  vector<string>::iterator it = g_vecTitleClosePosition.begin();
                  for (;it!=g_vecTitleClosePosition.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }else if(szTreeText->CompareTo("待审核单") == 0)
              {
                  vector<string>::iterator it = g_vecTitleNeedVerify.begin();
                  for (;it!=g_vecTitleNeedVerify.end();it++)
                  {
                      if(szColumnText->CompareTo(Tools::string2String((*it).c_str())) == 0)
                          return true;
                  }
              }*/

              return false;
          }
private: System::Void treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
     /*        dataGridView->Rows->Clear();
             string szText = Tools::String2string(treeView->SelectedNode->Text);
             int     nTag   = (int)treeView->SelectedNode->Tag;

             map<string,vector<string>>::iterator it = g_mapTreeNode2Column.find(szText);
             if(it!=g_mapTreeNode2Column.end())
             {
                 vector<string> vec = it->second; 
                 for (int i=0;i<(int)vec.size();i++)
                 {
                     string szColumnText = vec[i];
                     array<String^>^ newRow = {Tools::string2String(szColumnText.c_str()),IsChecked(Tools::string2String(szText.c_str()),Tools::string2String(szColumnText.c_str()))?"true":"false"};    
                     dataGridView->Rows->Add(newRow);
                     dataGridView->Rows[i]->Tag = GetTagFromText(Tools::string2String(szColumnText.c_str()));                       
                 }

                     switch(nTag)
                     {
                     case 1://报价表
                     case 2://当前风险事件
                     case 3://账户及资金
                         dataGridView->Rows[0]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         break;
                     case 4://委托单
                     case 5://未成交单
                     case 9://待审核单
                         dataGridView->Rows[0]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[2]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[16]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[17]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[18]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[19]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         break;
                     case 6://成交单
                         break;
                     case 7://持仓
                     case 8://平仓
                         dataGridView->Rows[0]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[1]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[2]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         dataGridView->Rows[3]->Cells[1]->ReadOnly  = true;//PrimaryKey
                         break;
                     }
             }*/
         }
private: System::Void button_OK_Click(System::Object^  sender, System::EventArgs^  e) {
             String^ szTreeText = treeView->SelectedNode->Text;
             String^ szColumnText = String::Empty;
             String^ szColumnIndex = String::Empty;
             vector<string> vecDataGridView2Column;               
             for (int i = 0; i < dataGridView->Rows->Count; i++)
             {
                 if (dataGridView->Rows[i]->Cells[1]->FormattedValue->ToString() == "True")
                 {
                     szColumnText = dataGridView->Rows[i]->Cells[0]->Value->ToString();
                     szColumnIndex = GetTagFromText(szColumnText).ToString();
                     vecDataGridView2Column.push_back(Tools::String2string(szColumnIndex));      
                 }
             }

//              CRiskData::GetInstance()->UpdaeConfigFile(Tools::String2string(szTreeText),vecDataGridView2Column,XML_TYPE_COLUMN);

             this->DialogResult = ::DialogResult::OK;
             this->Close();
         }
private: System::Void button_Cancel_Click(System::Object^  sender, System::EventArgs^  e) {
             this->Close();
         }
private: System::Void button_up_Click(System::Object^  sender, System::EventArgs^  e) {
         }
private: System::Void button_down_Click(System::Object^  sender, System::EventArgs^  e) {
         }
/*行排序 保留以后用*/
private: System::Void dataGridView_DragEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
             e->Effect = DragDropEffects::Move;
         }
private: System::Void dataGridView_CellMouseMove(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {
             if ((e->Clicks < 2) && (e->Button == System::Windows::Forms::Control::MouseButtons/*MouseButtons::Left*/))
             {
                 if ((e->ColumnIndex == -1) && (e->RowIndex > -1))
                     dataGridView->DoDragDrop(dataGridView->Rows[e->RowIndex], DragDropEffects::Move);
             }

         }
private: System::Void dataGridView_DragDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e) {
             int idx = GetRowFromPoint(e->X, e->Y);

             if (idx < 0) return;

             //::System::Type::typeid
//              if (e->Data->GetDataPresent(DataGridViewRow))
//              {
//                  DataGridViewRow^ row = (DataGridViewRow^)e->Data->GetData(::System::Data::DataRowView);//(typeof(DataGridViewRow));
//                  dataGridView->Rows->Remove(row);
//                  m_selectionIdx = idx;
//                  dataGridView->Rows->Insert(idx, row);
//              }

         }
private: int GetRowFromPoint(int x, int y)
         {
             for (int i = 0; i < dataGridView->RowCount; i++)
             {
                 //Rectangle rec = dataGridView->GetRowDisplayRectangle(i, false);

//                  if (dataGridView->RectangleToScreen(rec)->System::Drawing::Rectangle::Contains(x, y))
//                      return i;
             }


             return -1;
         }

private: System::Void dataGridView_RowsAdded(System::Object^  sender, System::Windows::Forms::DataGridViewRowsAddedEventArgs^  e) {
             if (m_selectionIdx > -1)
             {
                 dataGridView->Rows[m_selectionIdx]->Selected = true;
                 dataGridView->CurrentCell = dataGridView->Rows[m_selectionIdx]->Cells[0];
             }
         }
};
}

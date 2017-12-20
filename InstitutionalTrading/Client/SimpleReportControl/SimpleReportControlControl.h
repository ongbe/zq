#pragma once
//#include "windows.h"
#include "ExcelExport.h"
#include "DisplaySetting.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;


namespace SimpleReportControl {

	/// <summary>
	/// Summary for SimpleReportControlControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class SimpleReportControlControl : public System::Windows::Forms::UserControl
	{
	public:
		SimpleReportControlControl(void)
		{
			InitializeComponent();
			mbFullScreen = false;
			mnPageCount = 1;
			mnPageRowCount = 50;
			mnCurrentPageIndex = 1;
			mnRowCount = 0;
			mnStartLine = 0;
			mbSimpleMode = false;
			mbShowStatistic = false;
			mrDataTable = mrDataTableStatistic = mrOrginalDataTable = mrOrginalDataTableStatistic = nullptr;
			mHideColumn = gcnew cliext::set<String^>();
			mfunShowProgress = gcnew ShowProgressCallback(this,&SimpleReportControlControl::ShowProgess);

			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SimpleReportControlControl()
		{
			if (components)
			{
				delete components;
			}
		}

	private: Boolean mbFullScreen;
	private: System::Drawing::Point mOldPoint;
	private: System::Drawing::Size mOldSize;

	private: Int32 mnPageCount,mnCurrentPageIndex,mnPageRowCount,mnRowCount,mnStartLine;
	private: System::Windows::Forms::FlowLayoutPanel^  flowLayoutPanel1;
	private: System::Windows::Forms::ToolStripLabel^  toolStripLabel1;
	private: System::Windows::Forms::ToolStripComboBox^  toolStripComboBox1;
	private: System::Data::DataTable ^ mrDataTable;        //显示表地址
	private: System::Data::DataTable ^ mrOrginalDataTable; //最初的引用数据表地址



	private: System::Windows::Forms::ToolStripButton^  toolStripButton2;
	private: Boolean mbSimpleMode ;
	private: System::Data::DataTable ^ mrDataTableStatistic;
	private: System::Data::DataTable ^ mrOrginalDataTableStatistic;
	private: cliext::set<String^>^ mHideColumn;
	private: Boolean mbShowStatistic;
	private: System::Windows::Forms::ToolStripButton^  toolStripSplitButton1;
	private: String^  mGuid;
	private: System::Windows::Forms::ToolStripProgressBar^  toolStripProgressBar1;
	private: System::Windows::Forms::ToolStripLabel^  toolStripLabel2;

	private:  ShowProgressCallback ^ mfunShowProgress;;
	public:	property Boolean SimpleMode
			{
				Boolean get() {return mbSimpleMode;}
				Void set(Boolean nbVal) 
				{
					mbSimpleMode = nbVal;
				}
			}
	public:	property String^ ConfigGuid
			{
				String^ get() {return mGuid;}
				Void set(String^ nbVal) 
				{					
					mGuid = nbVal;
				}
			}
	private: void LoadData()
			 {
				 int lStartPos = 0;   //当前页面开始记录行
				 int lEndPos = 0;     //当前页面结束记录行


				 //	this->dataGridView1->cloumns
				 //System::Data::DataTable^ dtTemp = mrDataTable->Clone();   //克隆DataTable结构框架

				 if(mnPageCount != 0)
				 {
					 if (mnCurrentPageIndex == mnPageCount)
					 {
						 lEndPos = mnRowCount;				
					 }
					 else
					 {
						 lEndPos = mnPageRowCount * mnCurrentPageIndex;				
					 }

					 lStartPos = mnStartLine;	

					 //从元数据源复制记录行
					 int nCount = 0;
					 for (int i = lStartPos; i < lEndPos; i++)
					 {
						 //dtTemp->ImportRow(mrDataTable->Rows[i]);
						 lStartPos++;
						 nCount++;
					 }
					 this->dataGridView1->RowCount = nCount;
				 }
				 //this->dataGridView1->DataSource = dtTemp;	

				 UpdateBindingNavigator();
			 }
	private:Void UpdateBindingNavigator()
			{
				bindingNavigatorMoveFirstItem->Enabled = true;
				bindingNavigatorMovePreviousItem->Enabled = true;
				bindingNavigatorMoveNextItem->Enabled = true;
				bindingNavigatorMoveLastItem->Enabled = true;
				bindingNavigatorPositionItem->Enabled = true;
				toolStripComboBox1->Enabled = true;
				if(mnCurrentPageIndex == 1) //到达第一页
				{
					bindingNavigatorMoveFirstItem->Enabled = false;
					bindingNavigatorMovePreviousItem->Enabled = false;
					bindingNavigatorMoveNextItem->Enabled = true;
					bindingNavigatorMoveLastItem->Enabled = true;
					bindingNavigatorPositionItem->Enabled = true;
				}
				if(mnCurrentPageIndex == mnPageCount) //到达最后一页
				{					
					bindingNavigatorMoveFirstItem->Enabled = true;
					bindingNavigatorMovePreviousItem->Enabled = true;
					bindingNavigatorMoveNextItem->Enabled = false;
					bindingNavigatorMoveLastItem->Enabled = false;
					bindingNavigatorPositionItem->Enabled = true;
				}
				if(mnPageCount == 1) //只有一页
				{
					bindingNavigatorMoveFirstItem->Enabled = false;
					bindingNavigatorMovePreviousItem->Enabled = false;
					bindingNavigatorMoveNextItem->Enabled = false;
					bindingNavigatorMoveLastItem->Enabled = false;
					bindingNavigatorPositionItem->Enabled = true;
				}	
				if(mnPageCount == 0)
				{
					bindingNavigatorMoveFirstItem->Enabled = false;
					bindingNavigatorMovePreviousItem->Enabled = false;
					bindingNavigatorMoveNextItem->Enabled = false;
					bindingNavigatorMoveLastItem->Enabled = false;
					bindingNavigatorPositionItem->Enabled = false;
				}
				if(mnPageCount != 0 )
				{
					bindingNavigatorPositionItem->Text = mnCurrentPageIndex.ToString();
					bindingNavigatorCountItem->Text = L"/" + mnPageCount.ToString();
					bindingNavigatorCountItem->Enabled = true;
				}
			

			}
	public:	property System::Data::DataTable ^ RealDataTable
			{
				System::Data::DataTable ^ get() {return mrDataTable;}
				Void set(System::Data::DataTable ^ nrTable) 
				{
					if(nrTable == nullptr)
						return;
					mrDataTable = nrTable;
					mrOrginalDataTable = nrTable;
					mbShowStatistic = false;
					dataGridView1->RowCount = 0;
					dataGridView1->Columns->Clear();
					dataGridView1->Rows->Clear();
					mnRowCount = nrTable->Rows->Count;
					mnPageCount = mnRowCount / mnPageRowCount;
					if ((mnRowCount % mnPageRowCount) > 0) mnPageCount++;
					mnCurrentPageIndex = 1;
				

					//添加表头

					for each (DataColumn^ column in mrDataTable->Columns)
					{
						dataGridView1->Columns->Add(
							column->ColumnName, column->ColumnName);
					}
				
				/*	for(int j = 0;j < this->dataGridView1->Columns->Count;j++)
				    {
						  this->dataGridView1->Columns[j]->SortMode = DataGridViewColumnSortMode::Automatic;					
				    }*/
					LoadData();
					ShowWithConfig();
					toolStripComboBox1->SelectedIndex = mnCurrentPageIndex-1;
					mnStartLine = 0;	
				}
			}
	public:	property System::Data::DataTable ^ StatisticDataTable
			{
				System::Data::DataTable ^ get() {return mrDataTableStatistic;}
				Void set(System::Data::DataTable ^ nrTable) 
				{
					if(nrTable == nullptr)
						return;
					mrDataTableStatistic = nrTable;
					mrOrginalDataTableStatistic = nrTable;
					//mnRowCount = nrTable->Rows->Count;
					//mnPageCount = mnRowCount / mnPageRowCount;
					//if ((mnRowCount % mnPageRowCount) > 0) mnPageCount++;
					//mnCurrentPageIndex = 1;
					//mnStartLine = 0;	

					////添加表头

					//for each (DataColumn^ column in mrDataTable->Columns)
					//{
					//	dataGridView1->Columns->Add(
					//		column->ColumnName, column->ColumnName);
					//}
					//LoadData();

				}
			}
	public:Void Clear()
		   { 

			   dataGridView1->RowCount=0;
			   if(mrDataTable != mrOrginalDataTable)
			   {
				   mrDataTable->Clear();
				   mrDataTable = mrOrginalDataTable;
			   }
			   if(mrOrginalDataTableStatistic != mrDataTableStatistic)
			   {
				   mrDataTableStatistic->Clear();
				   mrDataTableStatistic = mrOrginalDataTableStatistic;
			   }
			 /*  mrDataTable = nullptr;
			   mrDataTableStatistic = nullptr;*/
			   mnPageCount = 0;
			   mnCurrentPageIndex = 1;
			   mnStartLine = 0;	
			  /* bindingNavigatorPositionItem->Text = mnCurrentPageIndex.ToString();
			    bindingNavigatorCountItem->Text = L"/" + mnPageCount.ToString();*/

			   //RealDataTable->Clear();
			   flowLayoutPanel1->Controls->Clear();
			   flowLayoutPanel1->Update();
		   }

	public: void Refresh2()
			{
				if(mrDataTable == nullptr)
					return;

				if(mbShowStatistic == false)
				{
					if(mrDataTable != mrOrginalDataTable)
					{
						mrDataTable->Clear();
						mrDataTable = mrOrginalDataTable;
					}
					mnRowCount = mrDataTable->Rows->Count;
				}
				else
				{
					if(mrOrginalDataTableStatistic != mrDataTableStatistic)
					{
						mrDataTableStatistic->Clear();
						mrDataTableStatistic = mrOrginalDataTableStatistic;
					}
					mnRowCount = mrDataTableStatistic->Rows->Count;
				}
				mnPageCount = mnRowCount / mnPageRowCount;
				if ((mnRowCount % mnPageRowCount) > 0) 
					mnPageCount++;
				LoadData();
				this->dataGridView1->Invalidate();
				/*  mnCurrentPageIndex = 1;
				mnStartLine = 0;	*/
			}

	protected: 
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::BindingNavigator^  bindingNavigator1;
	private: System::Windows::Forms::ToolStripLabel^  bindingNavigatorCountItem;
	private: System::Windows::Forms::ToolStripButton^  bindingNavigatorMoveFirstItem;
	private: System::Windows::Forms::ToolStripButton^  bindingNavigatorMovePreviousItem;
	private: System::Windows::Forms::ToolStripSeparator^  bindingNavigatorSeparator;
	private: System::Windows::Forms::ToolStripTextBox^  bindingNavigatorPositionItem;
	private: System::Windows::Forms::ToolStripSeparator^  bindingNavigatorSeparator1;
	private: System::Windows::Forms::ToolStripButton^  bindingNavigatorMoveNextItem;
	private: System::Windows::Forms::ToolStripButton^  bindingNavigatorMoveLastItem;
	private: System::Windows::Forms::ToolStripSeparator^  bindingNavigatorSeparator2;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton3;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton1;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator2;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton6;
	private: System::Windows::Forms::ToolStripSplitButton^  toolStripSplitButton2;
	private: System::Windows::Forms::ToolStripMenuItem^  导出数据和图表ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  只导出数据ToolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  只导出图表ToolStripMenuItem;

	private: System::ComponentModel::IContainer^  components;

	public:
		property System::Windows::Forms::FlowLayoutPanel^ ChildChart
		{
			System::Windows::Forms::FlowLayoutPanel^ get() {return flowLayoutPanel1;}
			Void set(System::Windows::Forms::FlowLayoutPanel^ nc) {flowLayoutPanel1 = nc;}
		}

		property System::Windows::Forms::DataGridView^ ChildDataGridView
		{
			System::Windows::Forms::DataGridView^ get() {return dataGridView1;}
			Void set(System::Windows::Forms::DataGridView^ nc) {dataGridView1 = nc;}
		}
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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(SimpleReportControlControl::typeid));
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->bindingNavigator1 = (gcnew System::Windows::Forms::BindingNavigator(this->components));
			this->bindingNavigatorCountItem = (gcnew System::Windows::Forms::ToolStripLabel());
			this->toolStripSplitButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton3 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->bindingNavigatorMoveFirstItem = (gcnew System::Windows::Forms::ToolStripButton());
			this->bindingNavigatorMovePreviousItem = (gcnew System::Windows::Forms::ToolStripButton());
			this->bindingNavigatorSeparator = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->bindingNavigatorPositionItem = (gcnew System::Windows::Forms::ToolStripTextBox());
			this->bindingNavigatorSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->bindingNavigatorMoveNextItem = (gcnew System::Windows::Forms::ToolStripButton());
			this->bindingNavigatorMoveLastItem = (gcnew System::Windows::Forms::ToolStripButton());
			this->bindingNavigatorSeparator2 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripButton6 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripSplitButton2 = (gcnew System::Windows::Forms::ToolStripSplitButton());
			this->toolStripLabel1 = (gcnew System::Windows::Forms::ToolStripLabel());
			this->toolStripComboBox1 = (gcnew System::Windows::Forms::ToolStripComboBox());
			this->toolStripProgressBar1 = (gcnew System::Windows::Forms::ToolStripProgressBar());
			this->toolStripLabel2 = (gcnew System::Windows::Forms::ToolStripLabel());
			this->导出数据和图表ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->只导出数据ToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->只导出图表ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->bindingNavigator1))->BeginInit();
			this->bindingNavigator1->SuspendLayout();
			this->SuspendLayout();
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->AllowUserToOrderColumns = true;
			dataGridViewCellStyle1->BackColor = System::Drawing::Color::Silver;
			this->dataGridView1->AlternatingRowsDefaultCellStyle = dataGridViewCellStyle1;
			this->dataGridView1->BackgroundColor = System::Drawing::Color::White;
			this->dataGridView1->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
		    this->dataGridView1->ColumnHeadersHeight = 30;
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dataGridView1->Location = System::Drawing::Point(0, 25);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 25;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(888, 321);
			this->dataGridView1->TabIndex = 1;
			this->dataGridView1->VirtualMode = true;
			this->dataGridView1->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &SimpleReportControlControl::dataGridView1_ColumnHeaderMouseClick);
			this->dataGridView1->CellValueNeeded += gcnew System::Windows::Forms::DataGridViewCellValueEventHandler(this, &SimpleReportControlControl::dataGridView1_CellValueNeeded);
			// 
			// bindingNavigator1
			// 
			this->bindingNavigator1->AddNewItem = nullptr;
			this->bindingNavigator1->CountItem = this->bindingNavigatorCountItem;
			this->bindingNavigator1->DeleteItem = nullptr;
			this->bindingNavigator1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(20) {this->toolStripSplitButton1, 
				this->toolStripButton3, this->toolStripButton2, this->toolStripButton1, this->toolStripSeparator2, this->bindingNavigatorMoveFirstItem, 
				this->bindingNavigatorMovePreviousItem, this->bindingNavigatorSeparator, this->bindingNavigatorPositionItem, this->bindingNavigatorCountItem, 
				this->bindingNavigatorSeparator1, this->bindingNavigatorMoveNextItem, this->bindingNavigatorMoveLastItem, this->bindingNavigatorSeparator2, 
				this->toolStripButton6, this->toolStripSplitButton2, this->toolStripLabel1, this->toolStripComboBox1, this->toolStripProgressBar1, 
				this->toolStripLabel2});
			this->bindingNavigator1->Location = System::Drawing::Point(0, 0);
			this->bindingNavigator1->MoveFirstItem = this->bindingNavigatorMoveFirstItem;
			this->bindingNavigator1->MoveLastItem = this->bindingNavigatorMoveLastItem;
			this->bindingNavigator1->MoveNextItem = this->bindingNavigatorMoveNextItem;
			this->bindingNavigator1->MovePreviousItem = this->bindingNavigatorMovePreviousItem;
			this->bindingNavigator1->Name = L"bindingNavigator1";
			this->bindingNavigator1->PositionItem = this->bindingNavigatorPositionItem;
			this->bindingNavigator1->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			this->bindingNavigator1->Size = System::Drawing::Size(888, 25);
			this->bindingNavigator1->TabIndex = 3;
			this->bindingNavigator1->Text = L"bindingNavigator1";
			// 
			// bindingNavigatorCountItem
			// 
			this->bindingNavigatorCountItem->Name = L"bindingNavigatorCountItem";
			this->bindingNavigatorCountItem->Size = System::Drawing::Size(41, 22);
			this->bindingNavigatorCountItem->Text = L"of {0}";
			this->bindingNavigatorCountItem->ToolTipText = L"Total number of items";
			// 
			// toolStripSplitButton1
			// 
			this->toolStripSplitButton1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripSplitButton1.Image")));
			this->toolStripSplitButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripSplitButton1->Name = L"toolStripSplitButton1";
			this->toolStripSplitButton1->Size = System::Drawing::Size(85, 22);
			this->toolStripSplitButton1->Text = L"显示列设置";
			this->toolStripSplitButton1->Click += gcnew System::EventHandler(this, &SimpleReportControlControl::toolStripSplitButton1_Click);
			// 
			// toolStripButton3
			// 
			this->toolStripButton3->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton3.Image")));
			this->toolStripButton3->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton3->Name = L"toolStripButton3";
			this->toolStripButton3->Size = System::Drawing::Size(49, 22);
			this->toolStripButton3->Text = L"数据";
			this->toolStripButton3->Click += gcnew System::EventHandler(this, &SimpleReportControlControl::toolStripButton3_Click);
			// 
			// toolStripButton2
			// 
			this->toolStripButton2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton2.Image")));
			this->toolStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton2->Name = L"toolStripButton2";
			this->toolStripButton2->Size = System::Drawing::Size(73, 22);
			this->toolStripButton2->Text = L"汇总数据";
			this->toolStripButton2->Click += gcnew System::EventHandler(this, &SimpleReportControlControl::toolStripButton2_Click);
			// 
			// toolStripButton1
			// 
			this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton1.Image")));
			this->toolStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton1->Name = L"toolStripButton1";
			this->toolStripButton1->Size = System::Drawing::Size(49, 22);
			this->toolStripButton1->Text = L"图表";
			this->toolStripButton1->Click += gcnew System::EventHandler(this, &SimpleReportControlControl::toolStripButton1_Click);
			// 
			// toolStripSeparator2
			// 
			this->toolStripSeparator2->Name = L"toolStripSeparator2";
			this->toolStripSeparator2->Size = System::Drawing::Size(6, 25);
			// 
			// bindingNavigatorMoveFirstItem
			// 
			this->bindingNavigatorMoveFirstItem->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->bindingNavigatorMoveFirstItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"bindingNavigatorMoveFirstItem.Image")));
			this->bindingNavigatorMoveFirstItem->Name = L"bindingNavigatorMoveFirstItem";
			this->bindingNavigatorMoveFirstItem->RightToLeftAutoMirrorImage = true;
			this->bindingNavigatorMoveFirstItem->Size = System::Drawing::Size(23, 22);
			this->bindingNavigatorMoveFirstItem->Text = L"第一页";
			this->bindingNavigatorMoveFirstItem->Click += gcnew System::EventHandler(this, &SimpleReportControlControl::bindingNavigatorMoveFirstItem_Click);
			// 
			// bindingNavigatorMovePreviousItem
			// 
			this->bindingNavigatorMovePreviousItem->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->bindingNavigatorMovePreviousItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"bindingNavigatorMovePreviousItem.Image")));
			this->bindingNavigatorMovePreviousItem->Name = L"bindingNavigatorMovePreviousItem";
			this->bindingNavigatorMovePreviousItem->RightToLeftAutoMirrorImage = true;
			this->bindingNavigatorMovePreviousItem->Size = System::Drawing::Size(23, 22);
			this->bindingNavigatorMovePreviousItem->Text = L"上一页";
			this->bindingNavigatorMovePreviousItem->Click += gcnew System::EventHandler(this, &SimpleReportControlControl::bindingNavigatorMovePreviousItem_Click);
			// 
			// bindingNavigatorSeparator
			// 
			this->bindingNavigatorSeparator->Name = L"bindingNavigatorSeparator";
			this->bindingNavigatorSeparator->Size = System::Drawing::Size(6, 25);
			// 
			// bindingNavigatorPositionItem
			// 
			this->bindingNavigatorPositionItem->AccessibleName = L"Position";
			this->bindingNavigatorPositionItem->AutoSize = false;
			this->bindingNavigatorPositionItem->Name = L"bindingNavigatorPositionItem";
			this->bindingNavigatorPositionItem->Size = System::Drawing::Size(50, 21);
			this->bindingNavigatorPositionItem->Text = L"0";
			this->bindingNavigatorPositionItem->ToolTipText = L"Current position";
			this->bindingNavigatorPositionItem->TextChanged += gcnew System::EventHandler(this, &SimpleReportControlControl::OnPageChange);
			// 
			// bindingNavigatorSeparator1
			// 
			this->bindingNavigatorSeparator1->Name = L"bindingNavigatorSeparator1";
			this->bindingNavigatorSeparator1->Size = System::Drawing::Size(6, 25);
			// 
			// bindingNavigatorMoveNextItem
			// 
			this->bindingNavigatorMoveNextItem->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->bindingNavigatorMoveNextItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"bindingNavigatorMoveNextItem.Image")));
			this->bindingNavigatorMoveNextItem->Name = L"bindingNavigatorMoveNextItem";
			this->bindingNavigatorMoveNextItem->RightToLeftAutoMirrorImage = true;
			this->bindingNavigatorMoveNextItem->Size = System::Drawing::Size(23, 22);
			this->bindingNavigatorMoveNextItem->Text = L"下一页";
			this->bindingNavigatorMoveNextItem->Click += gcnew System::EventHandler(this, &SimpleReportControlControl::bindingNavigatorMoveNextItem_Click);
			// 
			// bindingNavigatorMoveLastItem
			// 
			this->bindingNavigatorMoveLastItem->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->bindingNavigatorMoveLastItem->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"bindingNavigatorMoveLastItem.Image")));
			this->bindingNavigatorMoveLastItem->Name = L"bindingNavigatorMoveLastItem";
			this->bindingNavigatorMoveLastItem->RightToLeftAutoMirrorImage = true;
			this->bindingNavigatorMoveLastItem->Size = System::Drawing::Size(23, 22);
			this->bindingNavigatorMoveLastItem->Text = L"最后一页";
			this->bindingNavigatorMoveLastItem->Click += gcnew System::EventHandler(this, &SimpleReportControlControl::bindingNavigatorMoveLastItem_Click);
			// 
			// bindingNavigatorSeparator2
			// 
			this->bindingNavigatorSeparator2->Name = L"bindingNavigatorSeparator2";
			this->bindingNavigatorSeparator2->Size = System::Drawing::Size(6, 25);
			// 
			// toolStripButton6
			// 
			this->toolStripButton6->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
			this->toolStripButton6->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton6.Image")));
			this->toolStripButton6->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton6->Name = L"toolStripButton6";
			this->toolStripButton6->Size = System::Drawing::Size(49, 22);
			this->toolStripButton6->Text = L"全屏";
			this->toolStripButton6->Click += gcnew System::EventHandler(this, &SimpleReportControlControl::toolStripButton6_Click);
			// 
			// toolStripSplitButton2
			// 
			this->toolStripSplitButton2->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
			this->toolStripSplitButton2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripSplitButton2.Image")));
			this->toolStripSplitButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripSplitButton2->Name = L"toolStripSplitButton2";
			this->toolStripSplitButton2->Size = System::Drawing::Size(61, 22);
			this->toolStripSplitButton2->Text = L"导出";
			this->toolStripSplitButton2->ButtonClick += gcnew System::EventHandler(this, &SimpleReportControlControl::toolStripSplitButton2_ButtonClick);
			// 
			// toolStripLabel1
			// 
			this->toolStripLabel1->Name = L"toolStripLabel1";
			this->toolStripLabel1->Size = System::Drawing::Size(29, 22);
			this->toolStripLabel1->Text = L"数量";
			// 
			// toolStripComboBox1
			// 
			this->toolStripComboBox1->DropDownWidth = 75;
			this->toolStripComboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(6) {L"50", L"100", L"150", L"200", L"500", 
				L"1000"});
			this->toolStripComboBox1->Name = L"toolStripComboBox1";
			this->toolStripComboBox1->Size = System::Drawing::Size(75, 25);
			this->toolStripComboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &SimpleReportControlControl::toolStripComboBox1_SelectedIndexChanged);
			// 
			// toolStripProgressBar1
			// 
			this->toolStripProgressBar1->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
			this->toolStripProgressBar1->Name = L"toolStripProgressBar1";
			this->toolStripProgressBar1->Size = System::Drawing::Size(100, 22);
			// 
			// toolStripLabel2
			// 
			this->toolStripLabel2->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
			this->toolStripLabel2->Name = L"toolStripLabel2";
			this->toolStripLabel2->Size = System::Drawing::Size(95, 12);
			this->toolStripLabel2->Text = L"toolStripLabel2";
			// 
			// 导出数据和图表ToolStripMenuItem
			// 
			this->导出数据和图表ToolStripMenuItem->Name = L"导出数据和图表ToolStripMenuItem";
			this->导出数据和图表ToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->导出数据和图表ToolStripMenuItem->Text = L"导出数据和图表";
			this->导出数据和图表ToolStripMenuItem->Click += gcnew System::EventHandler(this, &SimpleReportControlControl::导出数据和图表ToolStripMenuItem_Click);
			// 
			// 只导出数据ToolStripMenuItem1
			// 
			this->只导出数据ToolStripMenuItem1->Name = L"只导出数据ToolStripMenuItem1";
			this->只导出数据ToolStripMenuItem1->Size = System::Drawing::Size(154, 22);
			this->只导出数据ToolStripMenuItem1->Text = L"只导出数据";
			this->只导出数据ToolStripMenuItem1->Click += gcnew System::EventHandler(this, &SimpleReportControlControl::只导出数据ToolStripMenuItem1_Click);
			// 
			// 只导出图表ToolStripMenuItem
			// 
			this->只导出图表ToolStripMenuItem->Name = L"只导出图表ToolStripMenuItem";
			this->只导出图表ToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->只导出图表ToolStripMenuItem->Text = L"只导出图表";
			this->只导出图表ToolStripMenuItem->Click += gcnew System::EventHandler(this, &SimpleReportControlControl::只导出图表ToolStripMenuItem_Click);
			// 
			// flowLayoutPanel1
			// 
			this->flowLayoutPanel1->AutoScroll = true;
			this->flowLayoutPanel1->BackColor = System::Drawing::Color::White;
			this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->flowLayoutPanel1->FlowDirection = System::Windows::Forms::FlowDirection::TopDown;
			this->flowLayoutPanel1->Location = System::Drawing::Point(0, 25);
			this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
			this->flowLayoutPanel1->Size = System::Drawing::Size(888, 321);
			this->flowLayoutPanel1->TabIndex = 4;
			this->flowLayoutPanel1->WrapContents = false;
			// 
			// SimpleReportControlControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->Controls->Add(this->flowLayoutPanel1);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->bindingNavigator1);
			this->Name = L"SimpleReportControlControl";
			this->Size = System::Drawing::Size(888, 346);
			this->Load += gcnew System::EventHandler(this, &SimpleReportControlControl::OnLoad);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->bindingNavigator1))->EndInit();
			this->bindingNavigator1->ResumeLayout(false);
			this->bindingNavigator1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void OnLoad(System::Object^  sender, System::EventArgs^  e) {
				 toolStripLabel2->Visible = false;				
				 toolStripProgressBar1->Visible = false;	
				 flowLayoutPanel1->Hide();
				 if(mbSimpleMode)
				 {
					 this->toolStripButton3->Visible = false;
					 this->toolStripButton1->Visible = false;
					 this->toolStripButton6->Visible = false;
					 this->toolStripButton2->Visible = false;
					 this->toolStripSplitButton2->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
					 this->toolStripSplitButton2->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {	 this->只导出数据ToolStripMenuItem1});
					 /* this->toolStripSplitButton2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripSplitButton2.Image")));
					 this->toolStripSplitButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
					 this->toolStripSplitButton2->Name = L"toolStripSplitButton2";
					 this->toolStripSplitButton2->Size = System::Drawing::Size(61, 22);
					 this->toolStripSplitButton2->Text = L"导出";*/
				 }
				 else
				 {
					 this->toolStripSplitButton2->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
					 this->toolStripSplitButton2->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->导出数据和图表ToolStripMenuItem, 
						 this->只导出数据ToolStripMenuItem1, this->只导出图表ToolStripMenuItem});
					 /* this->toolStripSplitButton2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripSplitButton2.Image")));
					 this->toolStripSplitButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
					 this->toolStripSplitButton2->Name = L"toolStripSplitButton2";
					 this->toolStripSplitButton2->Size = System::Drawing::Size(61, 22);
					 this->toolStripSplitButton2->Text = L"导出";*/
				 }
				 //装载config
				 LoadConfig();
				
			 }

	private: System::Void toolStripButton3_Click(System::Object^  sender, System::EventArgs^  e) {
				 //数据
				 //数据
				 if(mrDataTable == nullptr)
					 return;

				 dataGridView1->RowCount = 0;
				 mbShowStatistic = false;
				 toolStripSplitButton1->Visible = true;
				 flowLayoutPanel1->Hide();
				 
				 dataGridView1->Columns->Clear();
				 dataGridView1->Rows->Clear();

				 mnRowCount = mrDataTable->Rows->Count;
				 mnPageCount = mnRowCount / mnPageRowCount;
				 if ((mnRowCount % mnPageRowCount) > 0) mnPageCount++;
				 mnCurrentPageIndex = 1;
				 mnStartLine = 0;	

				 //添加表头

				 for each (DataColumn^ column in mrDataTable->Columns)
					{
						dataGridView1->Columns->Add(
							column->ColumnName, column->ColumnName);
					}
				 ShowWithConfig();
				 LoadData();
				 dataGridView1->Show();
				 UpdateBindingNavigator();
			 }
	private: System::Void toolStripButton1_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(mrDataTable == nullptr)
					 return;
				 //图表
				 //图表
				 dataGridView1->Hide();
				 flowLayoutPanel1->Show();
				 bindingNavigatorMoveFirstItem->Enabled = false;
				 bindingNavigatorMovePreviousItem->Enabled = false;
				 bindingNavigatorMoveNextItem->Enabled = false;
				 bindingNavigatorMoveLastItem->Enabled = false;
				 bindingNavigatorPositionItem->Enabled = false;
				 bindingNavigatorCountItem->Enabled = false;
				 toolStripComboBox1->Enabled = false;
			 }
	private: System::Void toolStripButton6_Click(System::Object^  sender, System::EventArgs^  e) {
				 //全屏
				 if(mbFullScreen == false)
				 {
					 mOldSize = this->Size;
					 mOldPoint = this->Location;				
					 this->Parent->Location = System::Drawing::Point(0,0);
					 this->Parent->Size = System::Drawing::Size(Screen::PrimaryScreen->WorkingArea.Width,Screen::PrimaryScreen->WorkingArea.Height);
					 this->Dock = DockStyle::None; 
					 this->Location = System::Drawing::Point(0,0);
					 this->Size = this->Parent->ClientSize;
					 // this->Width = Screen::PrimaryScreen->WorkingArea.Width; 
					 // this->Height = Screen::PrimaryScreen->WorkingArea.Height; 
					 //::SetParent((HWND)this->Handle.ToInt32(), 0); 
					 mbFullScreen = true;
					 this->toolStripButton6->Text = L"退出全屏";
				 }
				 else
				 {

					 this->Dock = DockStyle::None; 
					 this->Location = mOldPoint;
					 this->Size = mOldSize;
					 // this->Width = Screen::PrimaryScreen->WorkingArea.Width; 
					 // this->Height = Screen::PrimaryScreen->WorkingArea.Height; 
					 //::SetParent((HWND)this->Handle.ToInt32(), mOldParent); 

					 mbFullScreen = false;
					 this->toolStripButton6->Text = L"全屏";
				 }
			 }
	private: System::Void 导出数据和图表ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(mrDataTable == nullptr)
					 return;				
				 //导出树据何图表
				 if(mbShowStatistic == false)
					Export(mrDataTable,flowLayoutPanel1);
				 else
					Export(mrDataTableStatistic,flowLayoutPanel1);

			 }
	private: System::Void 只导出数据ToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
				// Export(mrDataTable,nullptr);
				 if(mrDataTable == nullptr)
					 return;	
				 if(mbShowStatistic == false)
					 Export(mrDataTable,nullptr);
				 else
					 Export(mrDataTableStatistic,nullptr);
			 }
	private: System::Void 只导出图表ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(mrDataTable == nullptr)
					 return;					
				 Export(nullptr,flowLayoutPanel1);				
			 }
	private: System::Void bindingNavigatorMoveFirstItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 //第一页
				 mnCurrentPageIndex = 1;
				 mnStartLine = 0;
				 LoadData();
				 this->dataGridView1->Invalidate();
			 }
	private: System::Void bindingNavigatorMovePreviousItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 //上一页
				 mnCurrentPageIndex --;
				 mnStartLine = mnPageRowCount*(mnCurrentPageIndex-1);;
				 LoadData();
				 this->dataGridView1->Invalidate();
			 }
	private: System::Void bindingNavigatorMoveNextItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 //下一页
				 mnCurrentPageIndex ++;
				 mnStartLine = mnPageRowCount*(mnCurrentPageIndex-1);;
				 LoadData();
				 this->dataGridView1->Invalidate();
			 }
	private: System::Void bindingNavigatorMoveLastItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 //最后一页
				 mnStartLine = mnPageRowCount*(mnPageCount-1);
				 mnCurrentPageIndex = mnPageCount;
				 LoadData();
				 this->dataGridView1->Invalidate();
			 }

	private: System::Void OnPageChange(System::Object^  sender, System::EventArgs^  e) {
				 //用户输入页码
				 try
				 {			
					 int lnTemp = Convert::ToInt32(bindingNavigatorPositionItem->Text->Trim());
					 if(lnTemp <= 0 || lnTemp > mnPageCount)
					 {	
						 throw;						
					 }
					 mnCurrentPageIndex = lnTemp;
					 mnStartLine = mnPageRowCount*(mnCurrentPageIndex-1);;
					 LoadData();
					 this->dataGridView1->Invalidate();
				 }
				 catch(System::Exception ^ e)
				 {
					 System::Windows::Forms::MessageBox::Show("请输入正确的数字！");				 
					 bindingNavigatorPositionItem->Focus();
				 }
			 }
	private: System::Void ShowProgess(String^ nsText,Int32 nProgress)
			 {
				 toolStripLabel2->Text = nsText;
				 toolStripProgressBar1->Value = nProgress;
				 return;
			 }
	private: System::Void Export(System::Data::DataTable ^ nrTable,System::Windows::Forms::FlowLayoutPanel^ nrChartContainer)
			 {

				 Type^ type = Type::GetTypeFromProgID("Excel.Application");
				 if(type == nullptr)
				 {
					 MessageBox::Show("该机没有安装Excel软件，无法完成导出功能", "错误", MessageBoxButtons::OK, MessageBoxIcon::Error);
					 // pFunShowProgress("Excel无法启动,导出失败",100);
					 return ;
				 }

				 SaveFileDialog^ saveFileDialog = gcnew SaveFileDialog();
				 saveFileDialog->InitialDirectory = System::Environment::CurrentDirectory;

				 saveFileDialog->Filter = "Excel 2007|*.xlsx|所有文件|*.*";
				 saveFileDialog->RestoreDirectory = true;
				 saveFileDialog->FilterIndex = 1;
				 if (saveFileDialog->ShowDialog() == DialogResult::OK)
				 {
					 String^ lsPath = saveFileDialog->FileName->ToUpper();
					 if (lsPath->EndsWith("XLSX"))
					 {					 
					
						
						 toolStripLabel2->Visible =true;
						 toolStripProgressBar1->Visible =true;
						 toolStripSplitButton2->Visible =false;
						 CFileDBUtil::ExportToExcel(nrTable,nrChartContainer,saveFileDialog->FileName,mfunShowProgress);
						 toolStripSplitButton2->Visible =true;
						 toolStripLabel2->Visible =false;
						 toolStripProgressBar1->Visible =false;


					 }
				 }
			 }


	private: System::Void dataGridView1_CellValueNeeded(System::Object^  sender, System::Windows::Forms::DataGridViewCellValueEventArgs^  e) {

				 //if(mrDataTable == nullptr)
				 // return;
				 /* _cache.LoadPage( e.RowIndex );
				 int rowIndex = e.RowIndex % _cache.PageSize;
				 e.Value = _cache.CachedData[rowIndex][e.ColumnIndex];*/
				 // If this is the row for new records, no values are needed.
				 // if (e->RowIndex == this->dataGridView1->RowCount - 1) return;
				 // e->Value = mrDataTable[e->RowIndex + mnCurrentPageIndex * mnPageRowCount][e->ColumnIndex];
				 String^ columnName= this->dataGridView1->Columns[e->ColumnIndex]->HeaderText;
				 if(mbShowStatistic == false && mrDataTable != nullptr && (e->RowIndex + ( mnCurrentPageIndex - 1) * mnPageRowCount < mrDataTable->Rows->Count))
					 e->Value = mrDataTable->Rows[e->RowIndex + ( mnCurrentPageIndex - 1) * mnPageRowCount][columnName];
				

				 if(mbShowStatistic != false && mrDataTableStatistic != nullptr  && (e->RowIndex + ( mnCurrentPageIndex - 1) * mnPageRowCount < mrDataTableStatistic->Rows->Count))
					 e->Value = mrDataTableStatistic->Rows[e->RowIndex + ( mnCurrentPageIndex - 1) * mnPageRowCount][columnName];
				
				 // Int32 nRealIndex =  mrDataTable->DefaultView[e->RowIndex + ( mnCurrentPageIndex - 1) * mnPageRowCount].;
				 // e->Value = mrDataTable->Rows[nRealIndex][columnName];
				 

			 }
	private: System::Void dataGridView1_ColumnHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) {

				 if(mrDataTable == nullptr || e->Button != System::Windows::Forms::MouseButtons::Left)
					 return;		
				
				 for(int j = 0;j < this->dataGridView1->Columns->Count;j++)
				 {
					// this->dataGridView1->Columns[j]->SortMode = DataGridViewColumnSortMode::Automatic;
					 if(j != e->ColumnIndex) 
						 this->dataGridView1->Columns[j]->HeaderCell->SortGlyphDirection = SortOrder::None;
				 }
				 //this->dataGridView1->Invalidate();
				
				 String^ columnName= this->dataGridView1->Columns[e->ColumnIndex]->HeaderText;
				 if (this->dataGridView1->Columns[e->ColumnIndex]->HeaderCell->SortGlyphDirection != SortOrder::Ascending)
				 {
					 
				
					 //dt.DefaultView.Sort = "unbound asc";
					 if(mbShowStatistic == false)
					 {
						 DataView ^ lv = gcnew DataView(mrDataTable);
						 lv->Sort = columnName ;
						 mrDataTable = lv->ToTable();
					 }
					 else
					 {
						 DataView ^ lv = gcnew DataView(mrDataTableStatistic);
						 lv->Sort = columnName ;
						 mrDataTableStatistic = lv->ToTable();
					 }

					 //mrDataTable->DefaultView->Sort = columnName;
					 this->dataGridView1->Columns[e->ColumnIndex]->HeaderCell->SortGlyphDirection = SortOrder::Ascending;
					 this->dataGridView1->Invalidate();


				 }
				 else
				 {
					 this->dataGridView1->Columns[e->ColumnIndex]->HeaderCell->SortGlyphDirection = SortOrder::Descending;

					 String ^ ls = gcnew String("");
					 ls = columnName + " DESC";

					 if(mbShowStatistic == false)
					 {
						 DataView ^ lv = gcnew DataView(mrDataTable);
						 lv->Sort = ls ;
						 mrDataTable = lv->ToTable();
					 }
					 else
					 {
						 DataView ^ lv = gcnew DataView(mrDataTableStatistic);
						 lv->Sort = ls ;
						 mrDataTableStatistic = lv->ToTable();
					 }

					 this->dataGridView1->Columns[e->ColumnIndex]->HeaderCell->SortGlyphDirection = SortOrder::Descending;
					 this->dataGridView1->Invalidate();

				 }
				 //mrDataTable->Select()
			 }
	private: System::Void toolStripComboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 //每页数量发生改变50


				 switch(this->toolStripComboBox1->SelectedIndex )
				 {
				 case  0: mnPageRowCount = 50;break;
				 case  1: mnPageRowCount = 100;break;
				 case  2: mnPageRowCount = 150;break;
				 case  3: mnPageRowCount = 200;break;
				 case  4: mnPageRowCount = 500;break;
				 case  5: mnPageRowCount = 1000;break;
					 break;
				 }

				 mnPageCount = mnRowCount / mnPageRowCount;
				 if ((mnRowCount % mnPageRowCount) > 0) mnPageCount++;
				 mnCurrentPageIndex = 1;
				 mnStartLine = 0;	
				 LoadData();			
				 this->dataGridView1->Invalidate();
			 }
	private: System::Void toolStripButton2_Click(System::Object^  sender, System::EventArgs^  e) {
				 //汇总数据
				 if(mrDataTableStatistic == nullptr)
					 return;	
				 dataGridView1->RowCount = 0;
				 mbShowStatistic = true;
				 toolStripSplitButton1->Visible = false;
				 flowLayoutPanel1->Hide();

				 dataGridView1->Columns->Clear();
				 dataGridView1->Rows->Clear();

				 mnRowCount = mrDataTableStatistic->Rows->Count;
				 mnPageCount = mnRowCount / mnPageRowCount;
				 if ((mnRowCount % mnPageRowCount) > 0) mnPageCount++;
				 mnCurrentPageIndex = 1;
				 mnStartLine = 0;	

				 //添加表头

				 for each (DataColumn^ column in mrDataTableStatistic->Columns)
				 {
					dataGridView1->Columns->Add(
							column->ColumnName, column->ColumnName);
				 }
				 LoadData();

				 dataGridView1->Show();
				 UpdateBindingNavigator();
			 }
	private: System::Void toolStripSplitButton2_ButtonClick(System::Object^  sender, System::EventArgs^  e) {
				 if(mrDataTable == nullptr)
					 return;					
				 if(mbShowStatistic == false)
					 Export(mrDataTable,flowLayoutPanel1);
				 else
					 Export(mrDataTableStatistic,flowLayoutPanel1);

			 }



private: System::Void LoadConfig()
		 {
			 if(mGuid == nullptr)
				 return;

			 String^ fileName = mGuid->ToString() + ".txt";
			 try 
			 {			
				 StreamReader^ din = File::OpenText(fileName);

				 String^ str;
				 int count = 0;
				 while ((str = din->ReadLine()) != nullptr) 
				 {
					 count++;
					 
					 mHideColumn->insert(str);
				 }
			 }
			 catch (Exception^ e)
			 {
				return;
			 }


		 }

private: System::Void SaveConfig()
		 {

			 if(mGuid == nullptr)
				 return;

			 String^ fileName = mGuid + ".txt";

			 try
			 {

				 File::Delete(fileName);
				 StreamWriter^ sw = gcnew StreamWriter(fileName);
			
				 for each(String^ ls in mHideColumn)
					 sw->WriteLine(ls);
				 sw->Close();
			 }			 
			 catch (Exception^ e)
			 {
				 return;
			 }		

		 }

 private:System::Void ShowWithConfig()
		 {
			 if(mHideColumn == nullptr)
				 return;

			 for each (DataGridViewColumn^ lCol in dataGridView1->Columns)
			 {
				 if(mHideColumn->find(lCol->Name) != mHideColumn->end())
					 lCol->Visible = false;
				 else
					 lCol->Visible = true;				
			 }

		 }
private: System::Void toolStripSplitButton1_Click(System::Object^  sender, System::EventArgs^  e) {
			 if(mrDataTable == nullptr)
				 return;

			 DisplaySetting ^ ldlg = gcnew DisplaySetting();
			 ldlg->Clear();
			 for each (DataColumn^ column in mrDataTable->Columns)
			 {
				 ldlg->AddColumn(column->ColumnName);
			 }
			 if(mHideColumn != nullptr)
			 {
				 for each(String^ ls in mHideColumn)
					 ldlg->AddHideColumn(ls);
			 }
			 if(ldlg->ShowDialog() == ::DialogResult::OK)
			 {
			
				 mHideColumn->clear();
				 ldlg->GetUnSelectHideItem(mHideColumn);				
				 ShowWithConfig();
				 SaveConfig();

			 }
		 }
};
}

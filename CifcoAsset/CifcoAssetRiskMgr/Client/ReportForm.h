#pragma once
#include <set>
#include <map>
#include <queue>
#include "AbstractReportDataProvider.h"
#include "ReportOrgTreeForm.h"
#include "CInstrumentIDTreeListDlg.h"
#include "CFinancialProductIDListDlg.h"
#include "CAssetMgmtOrganIDTreeListDlg.h"
#include "COrganAndAccountIDTreeListDlg.h"
#include "CCommonFunc.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace ReportDataProvider;


namespace Client {

	//[dllimport("user32.dll")]
	//static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);
	/// <summary>
	/// Summary for ReportForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ReportForm : public System::Windows::Forms::Form
	{
	private:
		IReportDataProvider ^ mpDataProvider;
	private: SimpleReportControl::SimpleReportControlControl^  simpleReportControlControl1;


	public: 
		property IReportDataProvider^  DataProvider
		{
			IReportDataProvider^ get() { return mpDataProvider; }
			void set(IReportDataProvider^ np) { mpDataProvider = np; }
		}
	private:
		ReportOrgTreeForm ^ mOrgTreeIDForm;
	
	public:
		ReportForm(void)
		{
			InitializeComponent();
			mpDataProvider = nullptr;
			m_pdataInfo = CDataInfo::NewInstance();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ReportForm()
		{
			if (components)
			{
				delete components;
			}
		}
	//全局共享基础信息
	private: CDataInfo* m_pdataInfo;
    //combobox 下拉控件
	private: CAssetMgmtOrganIDTreeListDlg^ dlgMgmtOrganIDTreeList;
	private: CFinancialProductIDListDlg^ dlgFinancialProductIDList;
	private: CInstrumentIDTreeListDlg^ dlgInstrumentIDTreeList;
	private: COrganAndAccountIDTreeListDlg^ dlgOrganAndAccountIDTreeList;

	private: AnywndComboBox::AnywndComboBoxControl^  comboInstrument;
	protected: 
	private: System::Windows::Forms::Label^  label3;
	private: AnywndComboBox::AnywndComboBoxControl^  comboFinaProduct;
	private: System::Windows::Forms::Label^  label2;
	private: AnywndComboBox::AnywndComboBoxControl^  comboAccount;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::RadioButton^  radioAccountQuery;
	private: System::Windows::Forms::RadioButton^  radioGroupQuery;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;




	private: AnywndComboBox::AnywndComboBoxControl^  comboOrg;
	private: System::ComponentModel::IContainer^  components;







	protected: 

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
			this->comboInstrument = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->comboFinaProduct = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboAccount = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->radioAccountQuery = (gcnew System::Windows::Forms::RadioButton());
			this->radioGroupQuery = (gcnew System::Windows::Forms::RadioButton());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker2 = (gcnew System::Windows::Forms::DateTimePicker());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->comboOrg = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->simpleReportControlControl1 = (gcnew SimpleReportControl::SimpleReportControlControl());
			this->SuspendLayout();
			// 
			// comboInstrument
			// 
			this->comboInstrument->BackColor = System::Drawing::Color::White;
			this->comboInstrument->ChildControl = nullptr;
			this->comboInstrument->DropDownHeight = 1;
			this->comboInstrument->DropDownWidth = 1;
			this->comboInstrument->FormattingEnabled = true;
			this->comboInstrument->IntegralHeight = false;
			this->comboInstrument->Location = System::Drawing::Point(528, 15);
			this->comboInstrument->MaxDropDownItems = 1;
			this->comboInstrument->Name = L"comboInstrument";
			this->comboInstrument->Size = System::Drawing::Size(100, 20);
			this->comboInstrument->TabIndex = 7;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(469, 18);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(59, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"品种/合约";
			// 
			// comboFinaProduct
			// 
			this->comboFinaProduct->BackColor = System::Drawing::Color::White;
			this->comboFinaProduct->ChildControl = nullptr;
			this->comboFinaProduct->DropDownHeight = 1;
			this->comboFinaProduct->DropDownWidth = 1;
			this->comboFinaProduct->FormattingEnabled = true;
			this->comboFinaProduct->IntegralHeight = false;
			this->comboFinaProduct->Location = System::Drawing::Point(347, 15);
			this->comboFinaProduct->MaxDropDownItems = 1;
			this->comboFinaProduct->Name = L"comboFinaProduct";
			this->comboFinaProduct->Size = System::Drawing::Size(100, 20);
			this->comboFinaProduct->TabIndex = 8;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(288, 18);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"理财产品";
			// 
			// comboAccount
			// 
			this->comboAccount->BackColor = System::Drawing::Color::White;
			this->comboAccount->ChildControl = nullptr;
			this->comboAccount->DropDownHeight = 1;
			this->comboAccount->DropDownWidth = 1;
			this->comboAccount->FormattingEnabled = true;
			this->comboAccount->IntegralHeight = false;
			this->comboAccount->Location = System::Drawing::Point(173, 41);
			this->comboAccount->MaxDropDownItems = 1;
			this->comboAccount->Name = L"comboAccount";
			this->comboAccount->Size = System::Drawing::Size(274, 20);
			this->comboAccount->TabIndex = 6;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(108, 44);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(41, 12);
			this->label4->TabIndex = 11;
			this->label4->Text = L"账户号";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(109, 18);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 12;
			this->label1->Text = L"组织架构";
			// 
			// radioAccountQuery
			// 
			this->radioAccountQuery->AutoSize = true;
			this->radioAccountQuery->Location = System::Drawing::Point(12, 42);
			this->radioAccountQuery->Name = L"radioAccountQuery";
			this->radioAccountQuery->Size = System::Drawing::Size(71, 16);
			this->radioAccountQuery->TabIndex = 9;
			this->radioAccountQuery->TabStop = true;
			this->radioAccountQuery->Text = L"指定查询";
			this->radioAccountQuery->UseVisualStyleBackColor = true;
			this->radioAccountQuery->CheckedChanged += gcnew System::EventHandler(this, &ReportForm::radioAccountQuery_CheckedChanged);
			// 
			// radioGroupQuery
			// 
			this->radioGroupQuery->AutoSize = true;
			this->radioGroupQuery->Location = System::Drawing::Point(12, 16);
			this->radioGroupQuery->Name = L"radioGroupQuery";
			this->radioGroupQuery->Size = System::Drawing::Size(71, 16);
			this->radioGroupQuery->TabIndex = 10;
			this->radioGroupQuery->TabStop = true;
			this->radioGroupQuery->Text = L"组合查询";
			this->radioGroupQuery->UseVisualStyleBackColor = true;
			this->radioGroupQuery->CheckedChanged += gcnew System::EventHandler(this, &ReportForm::radioGroupQuery_CheckedChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(30, 80);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(53, 12);
			this->label5->TabIndex = 11;
			this->label5->Text = L"初始日期";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->Location = System::Drawing::Point(110, 74);
			this->dateTimePicker1->MaxDate = System::DateTime(2013, 1, 22, 0, 0, 0, 0);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(163, 21);
			this->dateTimePicker1->TabIndex = 13;
			this->dateTimePicker1->Value = System::DateTime(2013, 1, 22, 0, 0, 0, 0);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(318, 80);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(53, 12);
			this->label6->TabIndex = 11;
			this->label6->Text = L"结束日期";
			// 
			// dateTimePicker2
			// 
			this->dateTimePicker2->Location = System::Drawing::Point(398, 74);
			this->dateTimePicker2->Name = L"dateTimePicker2";
			this->dateTimePicker2->Size = System::Drawing::Size(163, 21);
			this->dateTimePicker2->TabIndex = 13;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(95, 117);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 14;
			this->button1->Text = L"统计";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ReportForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(213, 117);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 14;
			this->button2->Text = L"清空";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &ReportForm::button2_Click);
			// 
			// comboOrg
			// 
			this->comboOrg->BackColor = System::Drawing::Color::White;
			this->comboOrg->ChildControl = nullptr;
			this->comboOrg->DropDownHeight = 1;
			this->comboOrg->DropDownWidth = 1;
			this->comboOrg->IntegralHeight = false;
			this->comboOrg->Location = System::Drawing::Point(173, 15);
			this->comboOrg->MaxDropDownItems = 1;
			this->comboOrg->Name = L"comboOrg";
			this->comboOrg->Size = System::Drawing::Size(105, 20);
			this->comboOrg->TabIndex = 16;
			// 
			// simpleReportControlControl1
			// 
			this->simpleReportControlControl1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->simpleReportControlControl1->Location = System::Drawing::Point(12, 146);
			this->simpleReportControlControl1->Name = L"simpleReportControlControl1";
			this->simpleReportControlControl1->RealDataTable = nullptr;
			this->simpleReportControlControl1->Size = System::Drawing::Size(646, 280);
			this->simpleReportControlControl1->TabIndex = 17;
			// 
			// ReportForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(670, 438);
			this->Controls->Add(this->simpleReportControlControl1);
			this->Controls->Add(this->comboOrg);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->dateTimePicker2);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->radioAccountQuery);
			this->Controls->Add(this->radioGroupQuery);
			this->Controls->Add(this->comboInstrument);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboFinaProduct);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboAccount);
			this->Name = L"ReportForm";
			this->Text = L"ReportForm";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Load += gcnew System::EventHandler(this, &ReportForm::OnLoad);
			this->Resize += gcnew System::EventHandler(this, &ReportForm::OnSize);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void OnSize(System::Object^  sender, System::EventArgs^  e) {
			  Int32 lnClientWidth = this->Width;
			  Int32 lnClientHeight = this->Height;
			  //this->flowLayoutPanel1->Size = System::Drawing::Size(lnClientWidth - 2*this->flowLayoutPanel1->Location.X -10,lnClientHeight - this->flowLayoutPanel1->Location.Y - 50);

			  

			  //this->simpleReportControlControl1
			  //this->chart1->Location = Point(5,lnClientHeight  - lnClientHeight /3 + 5) ;
			  //this->chart1->Size = System::Drawing::Size(lnClientWidth - 30,lnClientHeight / 3 - 20);
			 
			  this->simpleReportControlControl1->Location = Point(5,this->simpleReportControlControl1->Location.Y);
			  this->simpleReportControlControl1->Size = System::Drawing::Size(lnClientWidth - 20,lnClientHeight - simpleReportControlControl1->Location.Y  - 35);
		}
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 //统计
			 //判断查询条件，获取数据
			 //在flowLayoutPanel里面添加一个grid 和 一个chart报表

			 bool bHasData = false;
		
			 std::set<int> setAccountID;
			 // 然后根据查询条件获得需要订阅的AccountID集合
			 if(radioGroupQuery->Checked)
			 {

				 mpDataProvider->ClearFilterInstrument();
				 mpDataProvider->ClearFilterAccount();

				 bool bNoChecked = true;
				 std::set<int> setOrganID;
				 std::set<int> setFinanProdID;
				 dlgMgmtOrganIDTreeList->GetSelectID(setOrganID, bNoChecked);
				 //if(!bNoChecked) {
				 dlgFinancialProductIDList->GetSelectID(setFinanProdID);
				 //}

				 std::set<int> setTemp;
				 std::set<int> setOrganAccntID;

				 std::set<int>::iterator it;
				 std::set<int>::iterator itTmp;
				 for(it = setOrganID.begin(); it != setOrganID.end(); it++) {
					 setTemp.clear();
					 m_pdataInfo->GetTrustTradeAccountByOrgID(*it, bNoChecked, setTemp);
					 for(itTmp = setTemp.begin(); itTmp != setTemp.end(); itTmp++) {
						 setOrganAccntID.insert(*itTmp);
					 }
				 }

				 std::set<int> setProdAccntID;

				 for(it = setFinanProdID.begin(); it != setFinanProdID.end(); it++) {
					 setTemp.clear();
					 m_pdataInfo->GetTrustTradeAccountByFinancialID(*it, setTemp);
					 for(itTmp = setTemp.begin(); itTmp != setTemp.end(); itTmp++) {
						 setProdAccntID.insert(*itTmp);
					 }
				 }

				 int nMax = max(setOrganAccntID.size(), setProdAccntID.size());
				 int* pTempID = new int[nMax];

				
				 int* pEnd = std::set_intersection(
					 setOrganAccntID.begin(), setOrganAccntID.end(), 
					 setProdAccntID.begin(), setProdAccntID.end(), pTempID);
				 int* nPos = pTempID;
				 while(nPos < pEnd) {
					 setAccountID.insert(*nPos);
					 nPos++;
				 }
				 delete pTempID;

				 bHasData = setAccountID.size()>0;

				 //获取合约
				 if(this->comboInstrument->Visible != false)
				 {
					 std::set<std::string> setFilterInstrumentID;
					 dlgInstrumentIDTreeList->GetSelectID(setFilterInstrumentID);	
					 mpDataProvider->SetFilterInstrument(setFilterInstrumentID);

				 }


			 }
			 else 
			 {
				 //清空合约和账户过滤
				 mpDataProvider->ClearFilterInstrument();
				 mpDataProvider->ClearFilterAccount();
				 // 将查询集合保存起来				
				 dlgOrganAndAccountIDTreeList->GetSelectID(setAccountID);
				 bHasData = setAccountID.size()>0;	
				
			 }
			 if(!bHasData)				
			 {
				 MessageBox::Show("所设查询条件未筛选出任何帐号。", "提示", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			     return;
			 }
			 std::set<std::string> lInvestID;
			 set<int>::iterator lIter = setAccountID.begin();
			 while(lIter != setAccountID.end())
			 {
				 TrustTradeAccount  lTraderInfo;
				 m_pdataInfo->GetTrustTradeAccount(*lIter,lTraderInfo);
				 lInvestID.insert(std::string(lTraderInfo.szAccount));
				 lIter++;
			 }

			 //判断时间，判断两个时间都不能大于今天
			 if(this->dateTimePicker1->Value.CompareTo( this->dateTimePicker2->Value) > 0)
			 {
				 MessageBox::Show("你输入的开始日期晚于或等于结束日期，请重新输入！", "提示",MessageBoxButtons::OK);
				 return;
			 }

			 //清空数据表	
			 if(mpDataProvider->RealDataTable)
			 {
				 mpDataProvider->RealDataTable->Clear();
				 simpleReportControlControl1->RealDataTable = mpDataProvider->RealDataTable;
			 }
			 simpleReportControlControl1->Clear();


			 //提交查询
			 CQueryConditions ^ lQueryCondition = gcnew CQueryConditions();
			 lQueryCondition->mstartTime = this->dateTimePicker1->Value;
			 lQueryCondition->mendTime = this->dateTimePicker2->Value;

			 IntPtr hWnd=this->Handle;
			 mpDataProvider->SetFilterAccountSet(lInvestID);
			 mpDataProvider->SubscribeEvt(lQueryCondition,hWnd);

		
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 //清空结果
			 //清空查询条件
			 if(mpDataProvider->RealDataTable)
			 {
				 mpDataProvider->RealDataTable->Clear();
				 simpleReportControlControl1->RealDataTable = mpDataProvider->RealDataTable;
			 }
			 simpleReportControlControl1->Clear();
			 
		 }

private: System::Void OnLoad(System::Object^  sender, System::EventArgs^  e) {
			 //注册事件
			 IntPtr hWnd=this->Handle;
			 if(mpDataProvider == nullptr)
				 return;			
			
			 //////////////////////////////////////////////////////
			 //设置界面
			 dlgFinancialProductIDList = gcnew CFinancialProductIDListDlg();
			 dlgFinancialProductIDList->ResetRelateCombo(comboFinaProduct);
			 comboFinaProduct->ChildControl = dlgFinancialProductIDList;

			 dlgMgmtOrganIDTreeList = gcnew CAssetMgmtOrganIDTreeListDlg();
			 dlgMgmtOrganIDTreeList->ResetFinaProductDlg(dlgFinancialProductIDList);
			 dlgMgmtOrganIDTreeList->ResetRelateCombo(comboOrg);
			 comboOrg->ChildControl = dlgMgmtOrganIDTreeList; 

			 dlgInstrumentIDTreeList = gcnew CInstrumentIDTreeListDlg();
			 dlgInstrumentIDTreeList->ResetRelateCombo(comboInstrument);
			 comboInstrument->ChildControl = dlgInstrumentIDTreeList;

			 dlgOrganAndAccountIDTreeList = gcnew COrganAndAccountIDTreeListDlg();
			 dlgOrganAndAccountIDTreeList->ResetRelateCombo(comboAccount);
			 comboAccount->ChildControl = dlgOrganAndAccountIDTreeList;
		

			 ResetComboBoxTextReadOnly(comboFinaProduct);
			 ResetComboBoxTextReadOnly(comboOrg);
			 ResetComboBoxTextReadOnly(comboAccount);
			 ResetComboBoxTextReadOnly(comboInstrument);

			 radioGroupQuery->Checked = false;
			 radioAccountQuery->Checked = true;
			 comboOrg->Enabled = false;
			 comboFinaProduct->Enabled = false;
			 comboInstrument->Enabled = false;
			 comboAccount->Enabled = true;
			

			 this->dateTimePicker1->MaxDate = DateTime::Now;
			 this->dateTimePicker2->MaxDate = DateTime::Now;
			 
			 ////////////////////////////////////////////////////////
			 // mpDataProvider->SubscribeEvt((unsigned int)(hWnd.ToInt32()));
			 // mpDataProvider->GenerateGridViewHeader();

			 //加表头

			 //array<String^>^ headers = nullptr;
			 //if(mpDataProvider->GenerateGridViewHeader(headers))
			 //{
				// dataGridView1->ColumnCount=headers->Length;
				// for(int i=0;i<headers->Length;i++)
				// {
				//	 dataGridView1->Columns[i]->Name=headers[i];
				//	 dataGridView1->Columns[i]->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::ColumnHeader;
				// }
				// //dataGridView1->Columns[headers->Length-1]->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			 //}
		

		 }
private: System::Void radioGroupQuery_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(radioGroupQuery->Checked == false)
				 return;
			 //组合查询
			 radioGroupQuery->Checked = true;
			 radioAccountQuery->Checked = false;
			 comboOrg->Enabled = true;
			 comboInstrument->Enabled = true;
			 comboFinaProduct->Enabled = true;
			 comboAccount->Enabled = false;

		 }
private: System::Void radioAccountQuery_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(radioAccountQuery->Checked == false)
				 return;
			 //指定查询
			 radioAccountQuery->Checked = true;
			 radioGroupQuery->Checked = false;
			 comboOrg->Enabled = false;
			 comboInstrument->Enabled = false;
			 comboFinaProduct->Enabled = false;
			 comboAccount->Enabled = true;
		 }

		 //重载WndProc，以处理WM_COMMAND消息
protected:virtual void WndProc( Message% m ) override
		  {

			  if(m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival) 
			  {
				  char *p = (char*)m.LParam.ToInt32();
				 
				  mpDataProvider->ProcessData(p);
				  simpleReportControlControl1->RealDataTable = mpDataProvider->RealDataTable;
				  mpDataProvider->FillChart(simpleReportControlControl1->ChildChart);
			  }

			  Form::WndProc ( m );
		  }
public: void SetInstrumentShow(bool nbVal)
		{
			if(nbVal)
			{
				this->label3->Show();
				this->comboInstrument->Show();
			}
			else
			{
				this->label3->Hide();
				this->comboInstrument->Hide();
			}
		
		}

};
}

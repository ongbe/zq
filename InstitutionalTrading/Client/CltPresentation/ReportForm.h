#pragma once
#include <set>
#include <map>
#include <queue>
#include "AbstractReportDataProvider.h"
#include "CInstrumentIDTreeListDlg.h"
#include "CFinancialProductIDListDlg.h"
#include "CAssetMgmtOrganIDTreeListDlg.h"
#include "COrganAndAccountIDTreeListDlg.h"
#include "CCommonFunc.h"
#include "RiskMapForm.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace ReportDataProvider;


namespace CltPresentation {

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






	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::ComboBox^  comboBox2;
	private: SimpleReportControl::SimpleReportControlControl^  simpleReportControlControl1;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Button^  button3;
	private: String ^ msReportConrtolConfigGuid;


	public: 
		property IReportDataProvider^  DataProvider
		{
			IReportDataProvider^ get() { return mpDataProvider; }
			void set(IReportDataProvider^ np) { mpDataProvider = np; }
		}
		property String^  ReportConrtolConfigGuid
		{
			String^ get() { return msReportConrtolConfigGuid; }
			void set(String^ np) { 
				msReportConrtolConfigGuid = np; 
				simpleReportControlControl1->ConfigGuid = msReportConrtolConfigGuid;
			}
		}
	private:
		//ReportOrgTreeForm ^ mOrgTreeIDForm;
	
	public:
		ReportForm(void)
		{
			InitializeComponent();
			mpDataProvider = nullptr;
			mnQueryMode = -1;
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
	private: int mnQueryMode;
    //combobox 下拉控件
	private: CAssetMgmtOrganIDTreeListDlg^ dlgMgmtOrganIDTreeList;
	private: CFinancialProductIDListDlg^ dlgFinancialProductIDList;
	private: CInstrumentIDTreeListDlg^ dlgInstrumentIDTreeList;
	private: COrganAndAccountIDTreeListDlg^ dlgOrganAndAccountIDTreeList;

	private: AnywndComboBox::AnywndComboBoxControl^  comboInstrument;
	protected: 
	private: System::Windows::Forms::Label^  label3;

	private: System::Windows::Forms::Label^  label2;



	private: System::Windows::Forms::Label^  label1;


	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;





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
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker2 = (gcnew System::Windows::Forms::DateTimePicker());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->simpleReportControlControl1 = (gcnew SimpleReportControl::SimpleReportControlControl());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->button3 = (gcnew System::Windows::Forms::Button());
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
			this->comboInstrument->Location = System::Drawing::Point(434, 18);
			this->comboInstrument->MaxDropDownItems = 1;
			this->comboInstrument->Name = L"comboInstrument";
			this->comboInstrument->Size = System::Drawing::Size(183, 20);
			this->comboInstrument->TabIndex = 7;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(375, 21);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(53, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"查询内容";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(194, 21);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"查询方式";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(14, 21);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 12;
			this->label1->Text = L"查询类型";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(14, 60);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(53, 12);
			this->label5->TabIndex = 11;
			this->label5->Text = L"初始日期";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->Location = System::Drawing::Point(78, 54);
			this->dateTimePicker1->MaxDate = System::DateTime(2013, 1, 22, 0, 0, 0, 0);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(163, 21);
			this->dateTimePicker1->TabIndex = 13;
			this->dateTimePicker1->Value = System::DateTime(2013, 1, 22, 0, 0, 0, 0);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(302, 60);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(53, 12);
			this->label6->TabIndex = 11;
			this->label6->Text = L"结束日期";
			// 
			// dateTimePicker2
			// 
			this->dateTimePicker2->Location = System::Drawing::Point(363, 54);
			this->dateTimePicker2->Name = L"dateTimePicker2";
			this->dateTimePicker2->Size = System::Drawing::Size(163, 21);
			this->dateTimePicker2->TabIndex = 13;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(78, 91);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 14;
			this->button1->Text = L"统计";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ReportForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(196, 91);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 14;
			this->button2->Text = L"清空";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &ReportForm::button2_Click);
		
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"汇总查询", L"指定查询"});
			this->comboBox1->Location = System::Drawing::Point(78, 18);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(110, 20);
			this->comboBox1->TabIndex = 19;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &ReportForm::comboBox1_SelectedIndexChanged);
			// 
			// comboBox2
			// 
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Location = System::Drawing::Point(253, 18);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(105, 20);
			this->comboBox2->TabIndex = 20;
			this->comboBox2->SelectedIndexChanged += gcnew System::EventHandler(this, &ReportForm::comboBox2_SelectedIndexChanged);
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(78, 120);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(238, 20);
			this->progressBar1->TabIndex = 21;
			this->progressBar1->Value = 66;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(16, 124);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(53, 12);
			this->label4->TabIndex = 22;
			this->label4->Text = L"正在查询";
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(334, 117);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 23;
			this->button3->Text = L"取消查询";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &ReportForm::button3_Click);


			// 
			// simpleReportControlControl1
			// 
			this->simpleReportControlControl1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->simpleReportControlControl1->ConfigGuid = nullptr;
			this->simpleReportControlControl1->Location = System::Drawing::Point(12, 146);
			this->simpleReportControlControl1->Name = L"simpleReportControlControl1";
			this->simpleReportControlControl1->RealDataTable = nullptr;
			this->simpleReportControlControl1->SimpleMode = false;
			this->simpleReportControlControl1->Size = System::Drawing::Size(646, 280);
			this->simpleReportControlControl1->StatisticDataTable = nullptr;
			this->simpleReportControlControl1->TabIndex = 24;
			// 
			// ReportForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(670, 438);
			this->Controls->Add(this->simpleReportControlControl1);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->comboBox2);
			this->Controls->Add(this->comboBox1);		
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->dateTimePicker2);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboInstrument);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
		
			this->Name = L"ReportForm";
			this->Text = L"统计展现";
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

			 if(mnQueryMode == -1)
			 {
				 MessageBox::Show("请选中一种查询方式。", "提示", MessageBoxButtons::OK, MessageBoxIcon::Warning);				
				 return;
			 }
			 bool bHasData = false;
		
			 std::set<int> setAccountID;
			 std::set<std::string> setOtherName;
			 // 然后根据查询条件获得需要订阅的AccountID集合
			 if(mnQueryMode == 0)
			 {
				 //按部门
				 mpDataProvider->ClearFilterInstrument();
				 mpDataProvider->ClearFilterAccount();

				 bool bNoChecked = true;
				 std::set<int> setOrganID;
				 std::set<int> setFinanProdID;
				 dlgMgmtOrganIDTreeList->GetSelectID(setOrganID, bNoChecked);
			
				 std::set<int> setTemp;				 
				 std::set<int>::iterator it;
				 std::set<int>::iterator itTmp;
				 for(it = setOrganID.begin(); it != setOrganID.end(); it++) {

					 setOtherName.insert(m_pdataInfo->GetOrganizationNameByID(*it));

					 setTemp.clear();
					 m_pdataInfo->GetDealerByOrganID(*it, setTemp);
					 for(itTmp = setTemp.begin(); itTmp != setTemp.end(); itTmp++) {
						 setAccountID.insert(*itTmp);
					 }
				 }		

				 bHasData = setAccountID.size()>0;			


			 }
			 else if(mnQueryMode == 1)
			 {
				 //按产品
				 mpDataProvider->ClearFilterInstrument();
				 mpDataProvider->ClearFilterAccount();

		
				 std::set<int> setFinanProdID;		
				 dlgFinancialProductIDList->GetSelectID(setFinanProdID);
			
				 std::vector<int> setTemp;			

				 std::set<int>::iterator it;
				 std::vector<int>::iterator itTmp;
				 for(it = setFinanProdID.begin(); it != setFinanProdID.end(); it++) {

					 setOtherName.insert(m_pdataInfo->GetFinancialProductNameByID(*it));

					 setTemp.clear();
					 m_pdataInfo->GetTraderSetByProductID(*it, setTemp);
					 for(itTmp = setTemp.begin(); itTmp != setTemp.end(); itTmp++) {
						 setAccountID.insert(*itTmp);
					 }
				 }			

				 bHasData = setAccountID.size()>0;		

			 }
			 else if(mnQueryMode == 2)
			 {
				 //按品种			
				 mpDataProvider->ClearFilterInstrument();
				 mpDataProvider->ClearFilterAccount();

				 std::map<int, UserInfo> lmapUserInfo;
				 m_pdataInfo->GetUserInfo(lmapUserInfo);
				 std::map<int, UserInfo>::iterator it;
				 for(it = lmapUserInfo.begin(); it != lmapUserInfo.end(); it++) {					
						 setAccountID.insert(it->first);					
				 }	
				 bHasData = setAccountID.size()>0;	

				 std::set<std::string> setFilterInstrumentID;
				 dlgInstrumentIDTreeList->GetSelectID(setFilterInstrumentID);	
				 mpDataProvider->SetFilterInstrument(setFilterInstrumentID);				 
				 setOtherName = setFilterInstrumentID;
			 }
			 else  if(mnQueryMode == 3)
			 {
				 //清空合约和账户过滤
				 mpDataProvider->ClearFilterInstrument();
				 mpDataProvider->ClearFilterAccount();
				 // 将查询集合保存起来				
				 dlgOrganAndAccountIDTreeList->GetSelectID(setAccountID);
				 bHasData = setAccountID.size()>0;	

				 setOtherName.clear();
				
			 }
			 if(!bHasData)				
			 {
				 MessageBox::Show("所设查询条件未筛选出任何帐号。", "提示", MessageBoxButtons::OK, MessageBoxIcon::Warning);
			     return;
			 }
			

			 //判断时间，判断两个时间都不能大于今天
			 if(this->dateTimePicker1->Value.CompareTo( this->dateTimePicker2->Value) > 0)
			 {
				 MessageBox::Show("你输入的开始日期晚于或等于结束日期，请重新输入！", "提示",MessageBoxButtons::OK);
				 return;
			 }

			 //清空数据表	
			 //清空结果	
			 simpleReportControlControl1->Clear();
			 //清空查询条件
			 if(mpDataProvider->RealDataTable)
			 {
				 mpDataProvider->RealDataTable->Clear();
				 simpleReportControlControl1->RealDataTable = mpDataProvider->RealDataTable;
			 }

	
			 //提交查询
			 CQueryConditions ^ lQueryCondition = gcnew CQueryConditions(setAccountID.size(),setOtherName.size());
			 std::set<std::string> lInvestID;
			 set<int>::iterator lIter = setAccountID.begin();
			 int i = 0;
			 while(lIter != setAccountID.end())
			 {
				 UserInfo  lTraderInfo;
				 m_pdataInfo->GetUserInfoByUserID(*lIter,lTraderInfo);
				 lInvestID.insert(std::string(lTraderInfo.szAccount));
				 lQueryCondition->marrayAccount->SetValue(Tools::string2String(lTraderInfo.szAccount),i++);
				 lIter++;
			 }
		
			 std::set<std::string>::iterator lIterOther = setOtherName.begin();
			 i = 0;
			 for(;lIterOther != setOtherName.end();lIterOther++)
			 {
				 std::string ls = *lIterOther;
				lQueryCondition->marrayOtherName->SetValue(Tools::string2String(ls.c_str()),i++);
			 }
			 lQueryCondition->mnQueryMode = mnQueryMode;
			 lQueryCondition->mstartTime = this->dateTimePicker1->Value;
			 lQueryCondition->mendTime = this->dateTimePicker2->Value;

			 IntPtr hWnd=this->Handle;
			 mpDataProvider->SetFilterAccountSet(lInvestID);
			 mpDataProvider->SetFilterOther(setOtherName);
			 if(mnQueryMode == 3)
				 mpDataProvider->SetFilterOther(lInvestID);


			 sLoginRsp loginRsp = {0};
			 CDataInfo::NewInstance()->GetLogonInfo(loginRsp);
			
		
			 mpDataProvider->SubscribeEvt(lQueryCondition,hWnd,loginRsp.mnUserID);

			 simpleReportControlControl1->Enabled = false;
			 this->button1->Enabled = false;
			 this->button2->Enabled = false;

			 simpleReportControlControl1->Enabled = false;
			 label4->Visible =true;		
			 button3->Visible = true;
			 progressBar1->Value = 0;
			 progressBar1->Visible =true;
		
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 //清空结果	
			 simpleReportControlControl1->Clear();
			 //清空查询条件
			 if(mpDataProvider->RealDataTable)
			 {
				 mpDataProvider->RealDataTable->Clear();
				// simpleReportControlControl1->RealDataTable = mpDataProvider->RealDataTable;
			 }
			 if(mpDataProvider->StatisticDataTable)
			 {
				 mpDataProvider->StatisticDataTable->Clear();
				// simpleReportControlControl1->StatisticDataTable = mpDataProvider->StatisticDataTable;
			 }
			 
		 }

private: System::Void OnLoad(System::Object^  sender, System::EventArgs^  e) {
			 //注册事件
			 IntPtr hWnd=this->Handle;
			 if(mpDataProvider == nullptr)
				 return;			
			
			 label4->Visible =false;
			 progressBar1->Visible = false;
			 button3->Visible = false;
			 mpDataProvider->GenerateGridViewHeader();
			 //////////////////////////////////////////////////////
			 //设置界面
			 dlgFinancialProductIDList = gcnew CFinancialProductIDListDlg();
			 dlgMgmtOrganIDTreeList = gcnew CAssetMgmtOrganIDTreeListDlg();
			 dlgInstrumentIDTreeList = gcnew CInstrumentIDTreeListDlg();
			 dlgOrganAndAccountIDTreeList = gcnew COrganAndAccountIDTreeListDlg();
			

			 this->dateTimePicker1->MaxDate = DateTime::Now;
			 this->dateTimePicker2->MaxDate = DateTime::Now;
			 
					
			 simpleReportControlControl1->ConfigGuid = msReportConrtolConfigGuid;

		 }


		 //重载WndProc，以处理WM_USER_Win消息
protected:virtual void WndProc( Message% m ) override
		  {

			  if(m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival) 
			  {
				 // this->button1->Enabled = true;
				  char *p = (char*)m.LParam.ToInt32();
				 
				  mpDataProvider->ProcessData(p);
				  if(mpDataProvider->QueryIsFinished())
				  {
					  simpleReportControlControl1->Enabled = true;
					  label4->Visible =false;
					  progressBar1->Visible =false;
					  button3->Visible = false;					
					  this->button1->Enabled = true;
					  this->button2->Enabled = true;
					  simpleReportControlControl1->RealDataTable = mpDataProvider->RealDataTable;
					  simpleReportControlControl1->StatisticDataTable = mpDataProvider->StatisticDataTable;
				      mpDataProvider->FillChart(simpleReportControlControl1->ChildChart);

					  if(mpDataProvider->RealDataTable->Rows->Count == 0)
					  {
						  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
							  			CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
							  			"本次查询无数据返回");
					  }
				  }
				  else
				  {
					
					  progressBar1->Value = (100 *  (mpDataProvider->GetTotalQueryCount() - mpDataProvider->GetRemindQueryCount())) / mpDataProvider->GetTotalQueryCount();
				  }
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

private: System::Void 地图展现ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 //
			
		 }
private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 //组合查询
			 if(this->comboBox1->SelectedIndex == 0)
			 {
				 this->comboBox2->ResetText();	
				 this->comboBox2->Items->Clear();
				 this->comboBox2->Items->Add("按部门汇总");
				 this->comboBox2->Items->Add("按产品汇总");
				 if(this->Text != "帐户权益绩效统计")
				 {
					 this->comboBox2->Items->Add("按品种汇总");
				 }
			
				 this->comboBox2->SelectedIndex = 0;

			 }
			 //指定查询
			 else
			 {
				 this->comboBox2->ResetText();		
				 this->comboBox2->Items->Clear();
				 this->comboBox2->Items->Add("指定交易员查询");
				 this->comboBox2->SelectedIndex = 0;
			 }
		 }
private: System::Void comboBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 //如果是组合查询
			 //按部门汇总
			 //按产品汇总
			 //按品种汇总
			 if(this->comboBox1->SelectedIndex == 0)
			 {
				 if(this->comboBox2->SelectedIndex == 0)
				 {
					 //按部门
					 dlgMgmtOrganIDTreeList->ResetRelateCombo(comboInstrument);
					 comboInstrument->ChildControl = dlgMgmtOrganIDTreeList; 
					 ResetComboBoxTextReadOnly(comboInstrument);
					 mnQueryMode = 0;
					
				 }
				 else if(this->comboBox2->SelectedIndex == 1)
				 {
					 //按产品
					 dlgFinancialProductIDList->ResetRelateCombo(comboInstrument);
					 comboInstrument->ChildControl = dlgFinancialProductIDList; 
					 ResetComboBoxTextReadOnly(comboInstrument);
					 mnQueryMode  = 1;
				 }
				 else
				 {
					 //按品种
					 dlgInstrumentIDTreeList->ResetRelateCombo(comboInstrument);
					 comboInstrument->ChildControl = dlgInstrumentIDTreeList; 
					 ResetComboBoxTextReadOnly(comboInstrument);
					 mnQueryMode = 2;
				 }
			 }

			 //如果是指定查询
			 //指定交易员查询
			 else
			 {
				 dlgOrganAndAccountIDTreeList->ResetRelateCombo(comboInstrument);
				 comboInstrument->ChildControl = dlgOrganAndAccountIDTreeList; 
				 ResetComboBoxTextReadOnly(comboInstrument);
				 mnQueryMode = 3;
			 }
		 }
private: System::Void 设置ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		 }

private: System::Void 退出ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {

			// 是否终止本次查询
			 if(::DialogResult::Cancel ==  MessageBox::Show("是否终止本次查询", "提示",MessageBoxButtons::OKCancel))
				 return;

			 mpDataProvider->SetUserCancel(true);	
			
			/* if(::DialogResult::No ==  MessageBox::Show("是否显示未完成的查询结果", "提示",MessageBoxButtons::YesNo))
				 return;				
			*/
			 simpleReportControlControl1->RealDataTable = mpDataProvider->RealDataTable;
			 simpleReportControlControl1->StatisticDataTable = mpDataProvider->StatisticDataTable;
			 mpDataProvider->FillChart(simpleReportControlControl1->ChildChart);	
			 simpleReportControlControl1->Enabled = true;
			 label4->Visible =false;
			 progressBar1->Visible =false;
			 button3->Hide();
			 this->button1->Enabled = true;
			 this->button2->Enabled = true;
		 }
};
}

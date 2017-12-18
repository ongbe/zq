#pragma once
#include <string>
#include <set>
#include <map>
#include <queue>
#include "CommonPkg.h"
#include "CCommonFunc.h"
#include "TcpLayer.h"
#include "Tools.h"
#include "QueryInvestorInfo2.h"
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "CFinancialProductIDListDlg.h"
#include "CAssetMgmtOrganIDTreeListDlg.h"
#include "COrganAndAccountIDTreeListDlg.h"
#include "CDataInfo.h"
using namespace std;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Client {

	/// <summary>
	/// Summary for QueryInvestorInfo
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class QueryInvestorInfo : public System::Windows::Forms::Form
	{
	public:
		QueryInvestorInfo(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pdataInfo = CDataInfo::NewInstance();
			m_bHasChanged_QueryParam = true;
            m_QueryInvestorInfo2 = new QueryInvestorInfo2();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~QueryInvestorInfo()
		{
			if (components)
			{
				delete components;
			}
		}
	private: QueryInvestorInfo2*                 m_QueryInvestorInfo2; 
	private: CAssetMgmtOrganIDTreeListDlg^       dlgMgmtOrganIDTreeList;
	private: CFinancialProductIDListDlg^         dlgFinancialProductIDList;
	private: COrganAndAccountIDTreeListDlg^      dlgOrganAndAccountIDTreeList;
	private: CDataInfo*                          m_pdataInfo;
	private: bool                                m_bHasChanged_QueryParam;


	private: System::Windows::Forms::RadioButton^  radioButton1;

	protected: 

	private: System::Windows::Forms::Label^  label1;


	private: System::Windows::Forms::Label^  label2;




	private: System::Windows::Forms::RadioButton^  radioButton2;
	private: System::Windows::Forms::Label^  label3;





	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::Button^  button_query;
	private: System::Windows::Forms::Button^  button_clear;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_account;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_Org;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_Finacial;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_name;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_Contact;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_idtype;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_IdNumber;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_RiskIndiModel;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_Trader;
	private: AnywndComboBox::AnywndComboBoxControl^  comboOrganization;
	private: AnywndComboBox::AnywndComboBoxControl^  comboFinaProduct;
	private: AnywndComboBox::AnywndComboBoxControl^  comboAccount;

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
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column_account = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_Org = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_Finacial = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_name = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_Contact = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_idtype = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_IdNumber = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_RiskIndiModel = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_Trader = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->button_query = (gcnew System::Windows::Forms::Button());
			this->button_clear = (gcnew System::Windows::Forms::Button());
			this->comboFinaProduct = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->comboOrganization = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->comboAccount = (gcnew AnywndComboBox::AnywndComboBoxControl());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Location = System::Drawing::Point(30, 24);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(71, 16);
			this->radioButton1->TabIndex = 0;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"组合查询";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->Click += gcnew System::EventHandler(this, &QueryInvestorInfo::radioButton1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(161, 27);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"组织架构";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(409, 26);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 4;
			this->label2->Text = L"理财产品";
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(30, 60);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(71, 16);
			this->radioButton2->TabIndex = 7;
			this->radioButton2->TabStop = true;
			this->radioButton2->Text = L"指定查询";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->Click += gcnew System::EventHandler(this, &QueryInvestorInfo::radioButton2_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(163, 63);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(41, 12);
			this->label3->TabIndex = 8;
			this->label3->Text = L"账户号";
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(9) {this->Column_account, 
				this->Column_Org, this->Column_Finacial, this->Column_name, this->Column_Contact, this->Column_idtype, this->Column_IdNumber, 
				this->Column_RiskIndiModel, this->Column_Trader});
			this->dataGridView1->Location = System::Drawing::Point(30, 144);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(925, 361);
			this->dataGridView1->TabIndex = 11;
			this->dataGridView1->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &QueryInvestorInfo::dataGridView1_CellContentClick);
			// 
			// Column_account
			// 
			this->Column_account->HeaderText = L"账号";
			this->Column_account->Name = L"Column_account";
			this->Column_account->ReadOnly = true;
			this->Column_account->Width = 51;
			// 
			// Column_Org
			// 
			this->Column_Org->HeaderText = L"账号所属资管";
			this->Column_Org->Name = L"Column_Org";
			this->Column_Org->ReadOnly = true;
			this->Column_Org->Width = 72;
			// 
			// Column_Finacial
			// 
			this->Column_Finacial->HeaderText = L"账号所属理财产品";
			this->Column_Finacial->Name = L"Column_Finacial";
			this->Column_Finacial->ReadOnly = true;
			this->Column_Finacial->Width = 83;
			// 
			// Column_name
			// 
			this->Column_name->HeaderText = L"姓名";
			this->Column_name->Name = L"Column_name";
			this->Column_name->ReadOnly = true;
			this->Column_name->Width = 51;
			// 
			// Column_Contact
			// 
			this->Column_Contact->HeaderText = L"联系方式";
			this->Column_Contact->Name = L"Column_Contact";
			this->Column_Contact->ReadOnly = true;
			this->Column_Contact->Width = 61;
			// 
			// Column_idtype
			// 
			this->Column_idtype->HeaderText = L"证件类型";
			this->Column_idtype->Name = L"Column_idtype";
			this->Column_idtype->ReadOnly = true;
			this->Column_idtype->Width = 61;
			// 
			// Column_IdNumber
			// 
			this->Column_IdNumber->HeaderText = L"证件号码";
			this->Column_IdNumber->Name = L"Column_IdNumber";
			this->Column_IdNumber->ReadOnly = true;
			this->Column_IdNumber->Width = 61;
			// 
			// Column_RiskIndiModel
			// 
			this->Column_RiskIndiModel->HeaderText = L"风险指标模型";
			this->Column_RiskIndiModel->Name = L"Column_RiskIndiModel";
			this->Column_RiskIndiModel->ReadOnly = true;
			this->Column_RiskIndiModel->Width = 72;
			// 
			// Column_Trader
			// 
			this->Column_Trader->HeaderText = L"交易员";
			this->Column_Trader->Name = L"Column_Trader";
			this->Column_Trader->ReadOnly = true;
			this->Column_Trader->Width = 61;
			// 
			// button_query
			// 
			this->button_query->Location = System::Drawing::Point(30, 101);
			this->button_query->Name = L"button_query";
			this->button_query->Size = System::Drawing::Size(75, 23);
			this->button_query->TabIndex = 12;
			this->button_query->Text = L"查询";
			this->button_query->UseVisualStyleBackColor = true;
			this->button_query->Click += gcnew System::EventHandler(this, &QueryInvestorInfo::button_query_Click);
			// 
			// button_clear
			// 
			this->button_clear->Location = System::Drawing::Point(165, 100);
			this->button_clear->Name = L"button_clear";
			this->button_clear->Size = System::Drawing::Size(75, 23);
			this->button_clear->TabIndex = 13;
			this->button_clear->Text = L"清空";
			this->button_clear->UseVisualStyleBackColor = true;
			this->button_clear->Click += gcnew System::EventHandler(this, &QueryInvestorInfo::button_clear_Click);
			// 
			// comboFinaProduct
			// 
			this->comboFinaProduct->BackColor = System::Drawing::Color::White;
			this->comboFinaProduct->ChildControl = nullptr;
			this->comboFinaProduct->DropDownHeight = 1;
			this->comboFinaProduct->DropDownWidth = 1;
			this->comboFinaProduct->FormattingEnabled = true;
			this->comboFinaProduct->IntegralHeight = false;
			this->comboFinaProduct->Location = System::Drawing::Point(495, 19);
			this->comboFinaProduct->MaxDropDownItems = 1;
			this->comboFinaProduct->Name = L"comboFinaProduct";
			this->comboFinaProduct->Size = System::Drawing::Size(121, 20);
			this->comboFinaProduct->TabIndex = 15;
			// 
			// comboOrganization
			// 
			this->comboOrganization->BackColor = System::Drawing::Color::White;
			this->comboOrganization->ChildControl = nullptr;
			this->comboOrganization->DropDownHeight = 1;
			this->comboOrganization->DropDownWidth = 1;
			this->comboOrganization->FormattingEnabled = true;
			this->comboOrganization->IntegralHeight = false;
			this->comboOrganization->Location = System::Drawing::Point(232, 20);
			this->comboOrganization->MaxDropDownItems = 1;
			this->comboOrganization->Name = L"comboOrganization";
			this->comboOrganization->Size = System::Drawing::Size(121, 20);
			this->comboOrganization->TabIndex = 14;
			// 
			// comboAccount
			// 
			this->comboAccount->BackColor = System::Drawing::Color::White;
			this->comboAccount->ChildControl = nullptr;
			this->comboAccount->DropDownHeight = 1;
			this->comboAccount->DropDownWidth = 1;
			this->comboAccount->FormattingEnabled = true;
			this->comboAccount->IntegralHeight = false;
			this->comboAccount->Location = System::Drawing::Point(232, 55);
			this->comboAccount->MaxDropDownItems = 1;
			this->comboAccount->Name = L"comboAccount";
			this->comboAccount->Size = System::Drawing::Size(121, 20);
			this->comboAccount->TabIndex = 16;
			// 
			// QueryInvestorInfo
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(980, 529);
			this->Controls->Add(this->comboAccount);
			this->Controls->Add(this->comboFinaProduct);
			this->Controls->Add(this->comboOrganization);
			this->Controls->Add(this->button_clear);
			this->Controls->Add(this->button_query);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->radioButton2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->radioButton1);
			this->Name = L"QueryInvestorInfo";
			this->Text = L"投资者信息查询";
			this->Load += gcnew System::EventHandler(this, &QueryInvestorInfo::QueryInvestorInfo_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void QueryInvestorInfo_Load(System::Object^  sender, System::EventArgs^  e) 
		 {			 
			
          //列表添加空白行
			 /*for(int i=0; i<15; i++)
			 {
				 this->dataGridView1->Rows->Add();
			 }*/

			 dlgFinancialProductIDList = gcnew CFinancialProductIDListDlg();
			 dlgFinancialProductIDList->ResetRelateCombo(comboFinaProduct);
			 comboFinaProduct->ChildControl = dlgFinancialProductIDList;

			 dlgMgmtOrganIDTreeList = gcnew CAssetMgmtOrganIDTreeListDlg();
			 dlgMgmtOrganIDTreeList->ResetFinaProductDlg(dlgFinancialProductIDList);
			 dlgMgmtOrganIDTreeList->ResetRelateCombo(comboOrganization);
			 comboOrganization->ChildControl = dlgMgmtOrganIDTreeList;

			 dlgOrganAndAccountIDTreeList = gcnew COrganAndAccountIDTreeListDlg();
			 dlgOrganAndAccountIDTreeList->ResetRelateCombo(comboAccount);
			 comboAccount->ChildControl = dlgOrganAndAccountIDTreeList;



			 ResetComboBoxTextReadOnly(comboFinaProduct);
			 ResetComboBoxTextReadOnly(comboOrganization);
			 ResetComboBoxTextReadOnly(comboAccount);

			 // comboAccount
			this->comboAccount->ChildControl = nullptr;
			this->comboAccount->DropDownHeight = 1;
			this->comboAccount->DropDownWidth = 1;
			this->comboAccount->FormattingEnabled = true;
			this->comboAccount->IntegralHeight = false;
			this->comboAccount->MaxDropDownItems = 1;
			//this->comboAccount->DropDown += gcnew System::EventHandler(this, &QueryRiskEvt::comboAccount_DropDown);

			// comboFinaProduct
			this->comboFinaProduct->ChildControl = nullptr;
			this->comboFinaProduct->DropDownHeight = 1;
			this->comboFinaProduct->DropDownWidth = 1;
			this->comboFinaProduct->FormattingEnabled = true;
			this->comboFinaProduct->IntegralHeight = false;
			this->comboFinaProduct->MaxDropDownItems = 1;
			//this->comboFinaProduct->DropDown += gcnew System::EventHandler(this, &QueryRiskEvt::comboFinaProduct_DropDown);
               
			//comboOrganization
            this->comboOrganization->ChildControl = nullptr;
			this->comboOrganization->DropDownHeight = 1;
			this->comboOrganization->DropDownWidth = 1;
			this->comboOrganization->IntegralHeight = false;

			this->comboOrganization->MaxDropDownItems = 1;

			radioButton1->Checked = true;
			comboOrganization->Enabled = true;
			comboFinaProduct->Enabled = true;
			comboAccount->Enabled = false;



		 }


private: System::Void button_query_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			  if(!m_bHasChanged_QueryParam)
				 return;

             std::set<int> setOrganID;
			 std::set<int> setFinanProdID;
			 std::map<int, TrustTradeAccount> mapAccount;
			 if(radioButton1->Checked)
			 {
				 bool bHasData = false;
				 String^ istrt=comboOrganization->Text;
				 if(String::IsNullOrEmpty(istrt))
				 {
					 String^ strtemp = NULL_QUERY_CONDITION;
					 MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);
				 }
				 else
				 {
					 mapAccount.clear();
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

					 std::set<int> setResult;
					 int* pEnd = std::set_intersection(
						 setOrganAccntID.begin(), setOrganAccntID.end(), 
						 setProdAccntID.begin(), setProdAccntID.end(), pTempID);
					 int* nPos = pTempID;
					 while(nPos < pEnd) {
						 setResult.insert(*nPos);
						 nPos++;
					 }

					 //请求查询投资者信息
					 //m_QueryInvestorInfo2->ReqInvestorInfo(strOrganization,strFinancial,mapAccount);//通过名称查询
					 //测试代码
					 /* for (int i = 0; i <3 ; i++)
					 {
					 setOrganID.insert(i);
					 setFinanProdID.insert(i);
					 }*/

					 bHasData = setResult.size()>0;
					 if(bHasData)
						 m_QueryInvestorInfo2->ReqInvestorInfo(setResult,mapAccount);//通过ID查询
					 else
						 MessageBox::Show("所设查询条件未筛选出任何帐号。", 
						 "提示", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				 }
			 }
			 if(radioButton2->Checked)
			 {
				 /* String^ istrt=textBox_Account->Text;
				 const char*pchars = (const char*)(Runtime::InteropServices::Marshal::StringToHGlobalAnsi(istrt)).ToPointer();
				 std::string strAccount(pchars);
				 Runtime::InteropServices::Marshal::FreeHGlobal(IntPtr((void*)pchars));*/
				 String^ istrt=comboAccount->Text;
				 if(String::IsNullOrEmpty(istrt))
				 {
					 String^ strtemp = NULL_QUERY_CONDITION;
					 MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);
				 }
				 else
				 {
					 mapAccount.clear();

					 std::set<int> setAccntID;
					 dlgOrganAndAccountIDTreeList->GetSelectID(setAccntID);

					 ////请求查询投资者信息
					 m_QueryInvestorInfo2->ReqInvestorInfo(setAccntID,mapAccount);
				 }
			 }

			 //在列表显示查询结果
			 dataGridView1->Rows->Clear();

			 std::map<int, TrustTradeAccount>::iterator itacct = mapAccount.begin();
			 for(itacct;itacct!=mapAccount.end();itacct++)
			 {
                 int newRowIndex=dataGridView1->Rows->Add();
				DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
				// DataGridViewRow^ dr = gcnew DataGridViewRow();
				 //dataGridView1->Rows->Add(dr);
				 dr->Cells[0]->Value = Tools::string2String(itacct->second.szAccount);//帐号

					 AssetMgmtOrganization organ;
					 memset(&organ, 0, sizeof(organ));
					 if(m_pdataInfo->GetAssetMgmtOrg(itacct->second.nAssetMgmtOrgID, organ))
						 dr->Cells[1]->Value = Tools::string2String(organ.szName);//账号所属资管
					 FinancialProduct finanProduct;
					 memset(&finanProduct, 0, sizeof(finanProduct));
					 if(m_pdataInfo->GetFinancialProduct(itacct->second.nFinancialProductID, finanProduct))
						 dr->Cells[2]->Value = Tools::string2String(finanProduct.szName);//账号所属理财产品

				 dr->Cells[3]->Value = Tools::string2String(itacct->second.szName);//姓名
				 dr->Cells[4]->Value = Tools::string2String(itacct->second.szContactInfo);//联系方式
				 String^ ContactInfo = "";
				 switch(itacct->second.nIDCardType)
				 {
				 case IDENTIFY_CARD:
					 ContactInfo=L"身份证";
					 break;
				 case MILITARY_ID:
					 ContactInfo=L"军官证";
					 break;
				 case OTHER_CARD:
					 ContactInfo=L"其他";
					 break;
				 default:
					 ContactInfo=L"其他";
					 break;

				 }

				 dr->Cells[5]->Value = ContactInfo;//证件类型
				 dr->Cells[6]->Value = Tools::string2String(itacct->second.szIDCardNo);//证件号码

				 RiskIndicatorModule riskmodule;
                 memset(&riskmodule,0,sizeof(riskmodule));
				 if(m_pdataInfo->GetRiskIndicatorModule(itacct->second.nRiskIndModuleID,riskmodule))
					 dr->Cells[7]->Value = Tools::string2String(riskmodule.szName);//风险指标模型

                 TraderInfo trader;
				 memset(&trader,0,sizeof(trader));
				 if(m_pdataInfo->GetTraderInfo(itacct->second.nTraderID,trader))
					 dr->Cells[8]->Value = Tools::string2String(trader.szName);//交易员

			 }


		 }
private: System::Void button_clear_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 /* for(int   i=0;i<dataGridView1->Rows->Count;i++)   
			 {   
				 dataGridView1->Rows->RemoveAt(i);  

			 }   */
			 dataGridView1->Rows->Clear();
		 }
private: System::Void dataGridView1_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
		 {
			  
			  
		 }
private: System::Void radioButton1_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 comboOrganization->Enabled = true;
			 comboFinaProduct->Enabled = true;
			 comboAccount->Enabled = false;
		 }
private: System::Void radioButton2_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 comboOrganization->Enabled = false;
			 comboFinaProduct->Enabled = false;
			 comboAccount->Enabled = true;
		 }
};
}

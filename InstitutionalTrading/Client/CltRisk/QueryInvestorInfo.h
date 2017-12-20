#pragma once
#include "CommonDef.h"
#include "CCommonFunc.h"
#include "COrganAndAccountIDTreeListDlg.h"
#include "RiskData.h"
#include "popmsgwin.h"
#include "QueryData.h"
#include "TcpLayer.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltRisk {

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
			mDataTable = gcnew DataTable();
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

	private:

	private: COrganAndAccountIDTreeListDlg^ m_COrganAndAccountIDTreeListDlg;
	private: System::Windows::Forms::Label^  label2;
	private: AnywndComboBox::AnywndComboBoxControl^  comboBox_user;
	private: DataTable ^ mDataTable;
	private: SimpleReportControl::SimpleReportControlControl^  viewGrid1;
	private: System::Windows::Forms::Button^  button_query;
	private: System::Windows::Forms::Button^  button_clear;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_account;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_type;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_status;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_alias;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_contactinfo;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_department;
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
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboBox_user = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->viewGrid1 = (gcnew SimpleReportControl::SimpleReportControlControl());
			this->Column_account = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_type = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_status = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_alias = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_contactinfo = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_department = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->button_query = (gcnew System::Windows::Forms::Button());
			this->button_clear = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(33, 26);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(41, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"交易员";
			// 
			// comboBox_user
			// 
			this->comboBox_user->BackColor = System::Drawing::Color::White;
			this->comboBox_user->ChildControl = nullptr;
			this->comboBox_user->DropDownHeight = 1;
			this->comboBox_user->DropDownWidth = 1;
			this->comboBox_user->FormattingEnabled = true;
			this->comboBox_user->IntegralHeight = false;
			this->comboBox_user->Location = System::Drawing::Point(80, 18);
			this->comboBox_user->MaxDropDownItems = 1;
			this->comboBox_user->Name = L"comboBox_user";
			this->comboBox_user->Size = System::Drawing::Size(121, 20);
			this->comboBox_user->TabIndex = 3;
			// 
			// viewGrid1
			// 
			this->viewGrid1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->viewGrid1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->viewGrid1->ConfigGuid = L"{04351066-E630-442a-B1C4-753846D9B4DA}";
			this->viewGrid1->Location = System::Drawing::Point(35, 65);
			this->viewGrid1->Name = L"viewGrid1";
			this->viewGrid1->RealDataTable = nullptr;
			this->viewGrid1->SimpleMode = true;
			this->viewGrid1->Size = System::Drawing::Size(711, 397);
			this->viewGrid1->StatisticDataTable = nullptr;
			this->viewGrid1->TabIndex = 4;
			// 
			// Column_account
			// 
			this->Column_account->Name = L"Column_account";
			// 
			// Column_type
			// 
			this->Column_type->Name = L"Column_type";
			// 
			// Column_status
			// 
			this->Column_status->Name = L"Column_status";
			// 
			// Column_alias
			// 
			this->Column_alias->Name = L"Column_alias";
			// 
			// Column_contactinfo
			// 
			this->Column_contactinfo->Name = L"Column_contactinfo";
			// 
			// Column_department
			// 
			this->Column_department->Name = L"Column_department";
			// 
			// button_query
			// 
			this->button_query->Location = System::Drawing::Point(541, 18);
			this->button_query->Name = L"button_query";
			this->button_query->Size = System::Drawing::Size(75, 23);
			this->button_query->TabIndex = 5;
			this->button_query->Text = L"查询";
			this->button_query->UseVisualStyleBackColor = true;
			this->button_query->Click += gcnew System::EventHandler(this, &QueryInvestorInfo::button_query_Click);
			// 
			// button_clear
			// 
			this->button_clear->Location = System::Drawing::Point(671, 18);
			this->button_clear->Name = L"button_clear";
			this->button_clear->Size = System::Drawing::Size(75, 23);
			this->button_clear->TabIndex = 6;
			this->button_clear->Text = L"清空";
			this->button_clear->UseVisualStyleBackColor = true;
			this->button_clear->Click += gcnew System::EventHandler(this, &QueryInvestorInfo::button_clear_Click);
			// 
			// QueryInvestorInfo
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(784, 500);
			this->Controls->Add(this->button_clear);
			this->Controls->Add(this->button_query);
			this->Controls->Add(this->viewGrid1);
			this->Controls->Add(this->comboBox_user);
			this->Controls->Add(this->label2);
			this->Name = L"QueryInvestorInfo";
			this->Text = L"交易员基础信息查询";
			this->Load += gcnew System::EventHandler(this, &QueryInvestorInfo::QueryInvestorInfo_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &QueryInvestorInfo::QueryInvestorInfo_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void QueryInvestorInfo_Load(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->CenterToParent();
             SetDataTableColumns();
			 m_COrganAndAccountIDTreeListDlg = gcnew COrganAndAccountIDTreeListDlg();             
			 m_COrganAndAccountIDTreeListDlg->ResetRelateCombo(comboBox_user);
			 comboBox_user->ChildControl = m_COrganAndAccountIDTreeListDlg; 
			 ResetComboBoxTextReadOnly(comboBox_user);

			 viewGrid1->RealDataTable = mDataTable;

		 }
private: System::Void comboBox_department_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
			 }

private: System::Void SetDataTableColumns()
		 {
			 mDataTable->Columns->Clear();
			 mDataTable->Columns->Add(gcnew DataColumn("交易员账号",     Type::GetType("System.String")));
			 mDataTable->Columns->Add(gcnew DataColumn("帐号类型",   Type::GetType("System.String")));
			 mDataTable->Columns->Add(gcnew DataColumn("启用状态",   Type::GetType("System.String")));
			 mDataTable->Columns->Add(gcnew DataColumn("别名",     Type::GetType("System.String")));
			 mDataTable->Columns->Add(gcnew DataColumn("联系方式",   Type::GetType("System.String")));
			 mDataTable->Columns->Add(gcnew DataColumn("所属部门",     Type::GetType("System.String")));
		 }
			
private: Void ShowData(UserInfo& userinfo)
		 {
			 DataRow ^ dr = mDataTable->NewRow();
			 
			 dr[0] = Tools::string2String(userinfo.szAccount);//帐号
			 std::string strUserType =  CRiskData::GetInstance()->UserType2String(userinfo.nUserType);
			 dr[1]= Tools::string2String(strUserType.c_str());//帐号类型
			 if ( userinfo.nStatus == 0)
			 {
				 dr[2]= L"未启用";
			 }
			 else
			 {

				 dr[2]= L"启用";
			 }
			 dr[3] = Tools::string2String(userinfo.szAlias);//别名
			 dr[4] = Tools::string2String(userinfo.szContactInfo);//联系方式


			 vector<int> vecOrgID;
			 CRiskData::GetInstance()->GetOrgIDByUserID(userinfo.nUserID,USER_ORG_RELATION_ATTACH,vecOrgID);
			 string szName;
			 CRiskData::GetInstance()->GetOrganizationNameByID(vecOrgID[0], szName);
			 dr[5] = Tools::string2String(szName.c_str());//部门

			 mDataTable->Rows->Add(dr);

		 }
private: System::Void button_query_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 String^ istrt=comboBox_user->Text;
			 if(String::IsNullOrEmpty(istrt))
			 {
				MessageBox::Show(NULL_QUERY_CONDITION, "提示",MessageBoxButtons::OK);
			 }
			 else
			 {
				 viewGrid1->Clear();
				 mDataTable->Rows->Clear();
				 std::set<int> setAccntID;
				 m_COrganAndAccountIDTreeListDlg->GetSelectID(setAccntID);
				 if(setAccntID.size()==0)
				 {
					 MessageBox::Show(ERROR_CANNOTSCREENING, "提示",MessageBoxButtons::OK);
				 }
				 std::set<int>::iterator it = setAccntID.begin();
				 while( it!= setAccntID.end())
				 {
					 UserInfo userinfo;
					 memset(&userinfo,0,sizeof(userinfo));
					 CRiskData::GetInstance()->GetUserInfoByID((*it),userinfo);
					 ShowData(userinfo);
					 it++;

				 }
				 viewGrid1->Refresh2();

			 }
		 }
private: System::Void button_clear_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 viewGrid1->Clear();
			 mDataTable->Rows->Clear();
		 }

private: System::Void QueryInvestorInfo_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
		 {

			 //IntPtr hWnd=this->Handle;
			 //CTcpLayer::UnsubscribeAllPkg((int)hWnd);
		 }
};
}

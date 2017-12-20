#pragma once
#include "Stdafx.h"
#include "DataManageEx.h"
#include "ToolsEx.h"
#include "..\CommonClasses\ConvEnum.h"
#include "StrategeParaConfig.h"
#include "StrategeIndiConfig.h"
//#include "IndicatorDataEx.h"
//#include "StrategyDataEx.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace EnvDTE;
using namespace EnvDTE80;
using namespace VSLangProj;
using namespace System::IO;
using namespace System::Text;
using namespace System::Reflection;
using namespace System::Diagnostics;
using namespace System::Collections::Generic;
using namespace System::Collections::Generic;


namespace CltStrategy {

	/// <summary>
	/// Summary for CreatStrategy
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CreatStrategy : public System::Windows::Forms::Form
	{
	public:
		CreatStrategy(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pDataInfo = CDataManageEx::NewInstance();
			m_StrategyData = new CStrategyDataEx();

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CreatStrategy()
		{
			if (components)
			{
				delete components;
			}
			CDataManageEx::DestroyInstance();

			if ( NULL != m_StrategyData )
			{
				delete m_StrategyData;
				m_StrategyData = NULL;
			}
		}
	private: CStrategyDataEx*  m_StrategyData;
	private: EnvDTE80::DTE2^ _applicationObject;
	private: CDataManageEx*     m_pDataInfo;
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::TextBox^  textBox_engName;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox_chnName;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  textBox_version;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_title;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_name;
	private: System::Windows::Forms::Button^  button_delete;
	private: System::Windows::Forms::Button^  button_add;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_type;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_inputType;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_display;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_default;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::DataGridView^  dataGridView2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_InsNumber;
	private: System::Windows::Forms::Button^  button_deleteIns;
	private: System::Windows::Forms::Button^  button_addIns;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_MDPeriod;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::DataGridView^  dataGridView3;
	private: System::Windows::Forms::Button^  button_deleteIndi;
	private: System::Windows::Forms::Button^  button_addIndi;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_indiName;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_indicator;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_para;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_output;
	private: System::Windows::Forms::Button^  button_ok;
	private: System::Windows::Forms::Button^  button_cancel;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  textBox_deacribe;


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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox_engName = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox_chnName = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox_version = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button_delete = (gcnew System::Windows::Forms::Button());
			this->button_add = (gcnew System::Windows::Forms::Button());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column_title = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_name = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_type = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_inputType = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_display = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_default = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->button_deleteIns = (gcnew System::Windows::Forms::Button());
			this->button_addIns = (gcnew System::Windows::Forms::Button());
			this->dataGridView2 = (gcnew System::Windows::Forms::DataGridView());
			this->Column_InsNumber = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_MDPeriod = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->button_deleteIndi = (gcnew System::Windows::Forms::Button());
			this->button_addIndi = (gcnew System::Windows::Forms::Button());
			this->dataGridView3 = (gcnew System::Windows::Forms::DataGridView());
			this->Column_indiName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_indicator = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_para = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_output = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->button_ok = (gcnew System::Windows::Forms::Button());
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBox_deacribe = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->BeginInit();
			this->groupBox3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView3))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(28, 25);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(89, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"策略英文名称：";
			// 
			// textBox_engName
			// 
			this->textBox_engName->ImeMode = System::Windows::Forms::ImeMode::Disable;
			this->textBox_engName->Location = System::Drawing::Point(112, 16);
			this->textBox_engName->Name = L"textBox_engName";
			this->textBox_engName->Size = System::Drawing::Size(115, 21);
			this->textBox_engName->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(271, 23);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(89, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"策略中文别名：";
			// 
			// textBox_chnName
			// 
			this->textBox_chnName->ImeMode = System::Windows::Forms::ImeMode::On;
			this->textBox_chnName->Location = System::Drawing::Point(356, 16);
			this->textBox_chnName->Name = L"textBox_chnName";
			this->textBox_chnName->Size = System::Drawing::Size(137, 21);
			this->textBox_chnName->TabIndex = 3;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(542, 24);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(41, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"版本：";
			// 
			// textBox_version
			// 
			this->textBox_version->ImeMode = System::Windows::Forms::ImeMode::Disable;
			this->textBox_version->Location = System::Drawing::Point(579, 16);
			this->textBox_version->Name = L"textBox_version";
			this->textBox_version->Size = System::Drawing::Size(100, 21);
			this->textBox_version->TabIndex = 5;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->button_delete);
			this->groupBox1->Controls->Add(this->button_add);
			this->groupBox1->Controls->Add(this->dataGridView1);
			this->groupBox1->Location = System::Drawing::Point(30, 90);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(782, 215);
			this->groupBox1->TabIndex = 6;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"参数";
			// 
			// button_delete
			// 
			this->button_delete->Location = System::Drawing::Point(682, 115);
			this->button_delete->Name = L"button_delete";
			this->button_delete->Size = System::Drawing::Size(75, 23);
			this->button_delete->TabIndex = 2;
			this->button_delete->Text = L"删除";
			this->button_delete->UseVisualStyleBackColor = true;
			this->button_delete->Click += gcnew System::EventHandler(this, &CreatStrategy::button_delete_Click);
			// 
			// button_add
			// 
			this->button_add->Location = System::Drawing::Point(682, 69);
			this->button_add->Name = L"button_add";
			this->button_add->Size = System::Drawing::Size(75, 23);
			this->button_add->TabIndex = 1;
			this->button_add->Text = L"新增";
			this->button_add->UseVisualStyleBackColor = true;
			this->button_add->Click += gcnew System::EventHandler(this, &CreatStrategy::button_add_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->AllowUserToOrderColumns = true;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(6) {this->Column_title, 
				this->Column_name, this->Column_type, this->Column_inputType, this->Column_display, this->Column_default});
			this->dataGridView1->Location = System::Drawing::Point(16, 21);
			this->dataGridView1->MultiSelect = false;
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(643, 173);
			this->dataGridView1->TabIndex = 0;
			this->dataGridView1->CellDoubleClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &CreatStrategy::dataGridView1_CellDoubleClick);
			// 
			// Column_title
			// 
			this->Column_title->HeaderText = L"标题";
			this->Column_title->Name = L"Column_title";
			this->Column_title->ReadOnly = true;
			// 
			// Column_name
			// 
			this->Column_name->HeaderText = L"名称";
			this->Column_name->Name = L"Column_name";
			this->Column_name->ReadOnly = true;
			// 
			// Column_type
			// 
			this->Column_type->HeaderText = L"类型";
			this->Column_type->Name = L"Column_type";
			this->Column_type->ReadOnly = true;
			// 
			// Column_inputType
			// 
			this->Column_inputType->HeaderText = L"输入类型";
			this->Column_inputType->Name = L"Column_inputType";
			this->Column_inputType->ReadOnly = true;
			// 
			// Column_display
			// 
			this->Column_display->HeaderText = L"显示方式";
			this->Column_display->Name = L"Column_display";
			this->Column_display->ReadOnly = true;
			// 
			// Column_default
			// 
			this->Column_default->HeaderText = L"默认值";
			this->Column_default->Name = L"Column_default";
			this->Column_default->ReadOnly = true;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->button_deleteIns);
			this->groupBox2->Controls->Add(this->button_addIns);
			this->groupBox2->Controls->Add(this->dataGridView2);
			this->groupBox2->Location = System::Drawing::Point(30, 323);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(782, 194);
			this->groupBox2->TabIndex = 7;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"合约";
			// 
			// button_deleteIns
			// 
			this->button_deleteIns->Location = System::Drawing::Point(682, 103);
			this->button_deleteIns->Name = L"button_deleteIns";
			this->button_deleteIns->Size = System::Drawing::Size(75, 23);
			this->button_deleteIns->TabIndex = 2;
			this->button_deleteIns->Text = L"删除";
			this->button_deleteIns->UseVisualStyleBackColor = true;
			this->button_deleteIns->Click += gcnew System::EventHandler(this, &CreatStrategy::button_deleteIns_Click);
			// 
			// button_addIns
			// 
			this->button_addIns->Location = System::Drawing::Point(682, 54);
			this->button_addIns->Name = L"button_addIns";
			this->button_addIns->Size = System::Drawing::Size(75, 23);
			this->button_addIns->TabIndex = 1;
			this->button_addIns->Text = L"新增";
			this->button_addIns->UseVisualStyleBackColor = true;
			this->button_addIns->Click += gcnew System::EventHandler(this, &CreatStrategy::button_addIns_Click);
			// 
			// dataGridView2
			// 
			this->dataGridView2->AllowUserToAddRows = false;
			this->dataGridView2->AllowUserToDeleteRows = false;
			this->dataGridView2->AllowUserToOrderColumns = true;
			this->dataGridView2->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView2->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {this->Column_InsNumber, 
				this->Column_MDPeriod});
			this->dataGridView2->Location = System::Drawing::Point(16, 20);
			this->dataGridView2->MultiSelect = false;
			this->dataGridView2->Name = L"dataGridView2";
			this->dataGridView2->ReadOnly = true;
			this->dataGridView2->RowHeadersVisible = false;
			this->dataGridView2->RowTemplate->Height = 23;
			this->dataGridView2->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView2->Size = System::Drawing::Size(643, 157);
			this->dataGridView2->TabIndex = 0;
			// 
			// Column_InsNumber
			// 
			this->Column_InsNumber->HeaderText = L"合约编号";
			this->Column_InsNumber->Name = L"Column_InsNumber";
			this->Column_InsNumber->ReadOnly = true;
			// 
			// Column_MDPeriod
			// 
			this->Column_MDPeriod->HeaderText = L"行情周期";
			this->Column_MDPeriod->Name = L"Column_MDPeriod";
			this->Column_MDPeriod->ReadOnly = true;
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->button_deleteIndi);
			this->groupBox3->Controls->Add(this->button_addIndi);
			this->groupBox3->Controls->Add(this->dataGridView3);
			this->groupBox3->Location = System::Drawing::Point(30, 539);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(782, 229);
			this->groupBox3->TabIndex = 8;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"指标";
			// 
			// button_deleteIndi
			// 
			this->button_deleteIndi->Location = System::Drawing::Point(682, 125);
			this->button_deleteIndi->Name = L"button_deleteIndi";
			this->button_deleteIndi->Size = System::Drawing::Size(75, 23);
			this->button_deleteIndi->TabIndex = 2;
			this->button_deleteIndi->Text = L"删除";
			this->button_deleteIndi->UseVisualStyleBackColor = true;
			this->button_deleteIndi->Click += gcnew System::EventHandler(this, &CreatStrategy::button_deleteIndi_Click);
			// 
			// button_addIndi
			// 
			this->button_addIndi->Location = System::Drawing::Point(682, 70);
			this->button_addIndi->Name = L"button_addIndi";
			this->button_addIndi->Size = System::Drawing::Size(75, 23);
			this->button_addIndi->TabIndex = 1;
			this->button_addIndi->Text = L"新增";
			this->button_addIndi->UseVisualStyleBackColor = true;
			this->button_addIndi->Click += gcnew System::EventHandler(this, &CreatStrategy::button_addIndi_Click);
			// 
			// dataGridView3
			// 
			this->dataGridView3->AllowUserToAddRows = false;
			this->dataGridView3->AllowUserToDeleteRows = false;
			this->dataGridView3->AllowUserToOrderColumns = true;
			this->dataGridView3->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView3->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(4) {this->Column_indiName, 
				this->Column_indicator, this->Column_para, this->Column_output});
			this->dataGridView3->Location = System::Drawing::Point(16, 21);
			this->dataGridView3->MultiSelect = false;
			this->dataGridView3->Name = L"dataGridView3";
			this->dataGridView3->ReadOnly = true;
			this->dataGridView3->RowHeadersVisible = false;
			this->dataGridView3->RowTemplate->Height = 23;
			this->dataGridView3->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView3->Size = System::Drawing::Size(643, 191);
			this->dataGridView3->TabIndex = 0;
			this->dataGridView3->CellDoubleClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &CreatStrategy::dataGridView3_CellDoubleClick);
			// 
			// Column_indiName
			// 
			this->Column_indiName->HeaderText = L"名称";
			this->Column_indiName->Name = L"Column_indiName";
			this->Column_indiName->ReadOnly = true;
			// 
			// Column_indicator
			// 
			this->Column_indicator->HeaderText = L"指标";
			this->Column_indicator->Name = L"Column_indicator";
			this->Column_indicator->ReadOnly = true;
			// 
			// Column_para
			// 
			this->Column_para->HeaderText = L"参数";
			this->Column_para->Name = L"Column_para";
			this->Column_para->ReadOnly = true;
			// 
			// Column_output
			// 
			this->Column_output->HeaderText = L"是否输出";
			this->Column_output->Name = L"Column_output";
			this->Column_output->ReadOnly = true;
			// 
			// button_ok
			// 
			this->button_ok->Location = System::Drawing::Point(490, 792);
			this->button_ok->Name = L"button_ok";
			this->button_ok->Size = System::Drawing::Size(120, 23);
			this->button_ok->TabIndex = 9;
			this->button_ok->Text = L"确定";
			this->button_ok->UseVisualStyleBackColor = true;
			this->button_ok->Click += gcnew System::EventHandler(this, &CreatStrategy::button_ok_Click);
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(667, 792);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(120, 23);
			this->button_cancel->TabIndex = 10;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &CreatStrategy::button_cancel_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(28, 55);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(65, 12);
			this->label4->TabIndex = 11;
			this->label4->Text = L"策略描述：";
			// 
			// textBox_deacribe
			// 
			this->textBox_deacribe->Location = System::Drawing::Point(112, 45);
			this->textBox_deacribe->Name = L"textBox_deacribe";
			this->textBox_deacribe->Size = System::Drawing::Size(567, 21);
			this->textBox_deacribe->TabIndex = 12;
			// 
			// CreatStrategy
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(861, 836);
			this->Controls->Add(this->textBox_deacribe);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->button_ok);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->textBox_version);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->textBox_chnName);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBox_engName);
			this->Controls->Add(this->label1);
			this->Name = L"CreatStrategy";
			this->Text = L"新建策略：";
			this->Load += gcnew System::EventHandler(this, &CreatStrategy::CreatStrategy_Load);
			this->groupBox1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->groupBox2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->EndInit();
			this->groupBox3->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView3))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
public: void setApplicationObject(EnvDTE80::DTE2^ app)
		{

			_applicationObject = app;
		}
public: EnvDTE80::DTE2^ getApplicationObject()
		{
			return _applicationObject;					
		}
private: System::Void CreatStrategy_Load(System::Object^  sender, System::EventArgs^  e) 
		 {
			  this->CenterToParent();
			  AddNewRowIns();
			  //m_pDataInfo->ClearStrategyMap();
		 }
private: System::Void UpdateRow(DataGridViewRow^ dr,StrategyPara& data)
		 {
			 dr->Cells[0]->Value = Tools::string2String(data.szTitle);
			 dr->Cells[1]->Value = Tools::string2String(data.szName);
			 dr->Cells[2]->Value = Tools::string2String(StrategyType2String(data.eType));
			 dr->Cells[3]->Value = Tools::string2String(DirectionType2String(data.eDirection));
			 dr->Cells[4]->Value = Tools::string2String(DisplayType2String(data.eDisplayType));
			 string strdefault(data.szDefault);
			 if( strdefault !=NULL_VALUE )
			 {
                 dr->Cells[5]->Value = Tools::string2String(data.szDefault);
			 }
			 else
			 {
				  dr->Cells[5]->Value = "";
			 }
			 

		 }
private: System::Void button_add_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 int newRowIndex=dataGridView1->Rows->Count+1;
			 StrategeParaConfig^ dlg = gcnew StrategeParaConfig(m_StrategyData,newRowIndex);

			 if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
			 {
				 StrategyPara pFieid;
				 memset(&pFieid,0,sizeof(StrategyPara));

				 pFieid=*(dlg->GetStrategyParaInfo());	
				 int newRowIndex=dataGridView1->Rows->Add();
				 DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
				 UpdateRow(dr,pFieid);
				 m_StrategyData->AddStrategyPara(pFieid);
			 }
		 }
private: System::Void button_delete_Click(System::Object^  sender, System::EventArgs^  e) 
		 {

			 DataGridViewRow^  dr = dataGridView1->CurrentRow;
			 if(dr != nullptr)
			 { 
				 string szTitle = Tools::String2string(dr->Cells[0]->Value->ToString());
				 m_StrategyData->DeleteStrategyPara(szTitle);
				 dataGridView1->Rows->RemoveAt(dr->Index);

			 }
		 }
private: System::Void AddNewRowIns()
		 {
			 int newRowIndex=dataGridView2->Rows->Add();
			 DataGridViewRow^ dr = dataGridView2->Rows[newRowIndex];
			 char strTemp[32];
			 memset(strTemp,0,sizeof(strTemp));
			 sprintf(strTemp, "%s%d","合约",newRowIndex+1);

			 dr->Cells[0]->Value = Tools::string2String(strTemp);
			 dr->Cells[1]->Value = L"TICK";


		 }
private: System::Void button_addIns_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			AddNewRowIns();

		 }
private: System::Void button_deleteIns_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 int nRowCnt=dataGridView2->Rows->Count;
			 if ( nRowCnt > 0 )
			 {
				 DataGridViewRow^  dr = dataGridView2->Rows[dataGridView2->Rows->Count-1];
				 if(dr != nullptr)
				 { 			
					 dataGridView2->Rows->RemoveAt(dr->Index);

				 }
			 }
			 
		 }
private: System::Void UpdateIndiRow(DataGridViewRow^ dr,StrategyIndicator& data)
		 {
			 dr->Cells[0]->Value = Tools::string2String(data.szTitle);
			 dr->Cells[1]->Value = Tools::string2String(data.szName);
             dr->Cells[2]->Value = Tools::string2String(data.szPara);
			 if (data.bOutput == true)
			 {
				  dr->Cells[3]->Value = "true";
			 }
			 else
			 {
                 dr->Cells[3]->Value = "false";
			 }
			

		 }
private: System::Void button_addIndi_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if( String::IsNullOrEmpty(textBox_engName->Text))  
			 {
				 MessageBox::Show("请输入策略英文名称!");    
				 return;
			 }
			 int nInsCount = dataGridView2->Rows->Count;
			 if (nInsCount == 0)
				 return;
			 //获取策略参数
			 int nCount = dataGridView1->Rows->Count;
             multimap<string,string> strategyParaType2Name;
			 for ( int i =0; i< nCount; i++)
			 {
				 std::string strtype,strname;
				 strtype = Tools::String2string(dataGridView1->Rows[i]->Cells[2]->Value->ToString());
				 strname = Tools::String2string(dataGridView1->Rows[i]->Cells[1]->Value->ToString());
				 strategyParaType2Name.insert(make_pair(strtype,strname));
			 }
             m_StrategyData->SetParaType2NameMap(strategyParaType2Name);
			 
			 StrategeIndiConfig^ dlg = gcnew StrategeIndiConfig(nInsCount,m_StrategyData);

			 if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
			 {
				 StrategyIndicator pFieid;
				 memset(&pFieid,0,sizeof(StrategyIndicator));

				 pFieid=*(dlg->GetStrategyIndicator());	
				 int newRowIndex=dataGridView3->Rows->Add();
				 DataGridViewRow^ dr = dataGridView3->Rows[newRowIndex];
				 UpdateIndiRow(dr,pFieid);
				 
				 string szStrategy = Tools::String2string(textBox_engName->Text);
				 m_StrategyData->AddStrategyIndicator(szStrategy,pFieid);
			 }
		 }
private: System::Void button_deleteIndi_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 DataGridViewRow^  dr = dataGridView3->CurrentRow;
			 if(dr != nullptr)
			 { 
				 string szStrategy = Tools::String2string(textBox_engName->Text);
				 string szTitle = Tools::String2string(dr->Cells[0]->Value->ToString());
				 m_StrategyData->DeleteStrategyIndicator(szStrategy,szTitle);
				 dataGridView3->Rows->RemoveAt(dr->Index);

			 }
		 }
 private: bool InputCheck()
		 {
			 if( String::IsNullOrEmpty(textBox_chnName->Text))  
			 {
				 MessageBox::Show("请输入策略中文名称!");    
				 return false;
			 }
			 if( String::IsNullOrEmpty(textBox_version->Text))  
			 {
				 MessageBox::Show("请输入策略版本!");    
				 return false;
			 }
			 return true;
		  }
		  Project^ GetProjectByName(EnvDTE90::Solution3^ soln, String^ projName) 
		  {
			  Project^ proj = nullptr;
			  for(int i = 0; i<soln->Projects->Count; i++) 
			  {
				  proj = soln->Projects->Item(i+1);
				  if(proj->default == projName) 
				  {
					  return proj;
				  }
				  else {
					  for(int i = 0; i<proj->ProjectItems->Count; i++)
					  {
						  ProjectItem^ proj2 = proj->ProjectItems->Item(i+1);
						  if(proj2->default == projName) 
						  {
							  return (Project^)proj2->Object;
						  }
					  }
				  }
			  }
			  return nullptr;
		  }
private: System::Void button_ok_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(InputCheck()== false)
				 return;

			 /*SStrategy strategy;
			 memset(&strategy,0,sizeof(SStrategy));*/
			 m_StrategyData->SetStrategyName(Tools::String2string(textBox_engName->Text)) ;
			 m_StrategyData->SetStrategyNickName(Tools::String2string(textBox_chnName->Text));
			 m_StrategyData->SetStrategyVersion(Tools::String2string(textBox_version->Text));
			 m_StrategyData->SetDescribe(Tools::String2string(textBox_deacribe->Text));
			 m_StrategyData->SetInstrumentCount(dataGridView2->Rows->Count);
			 m_StrategyData->SetStrategyPrivate(1);

			// m_pDataInfo->AddIndcator(indicator);

			 int protectcount = _applicationObject->Solution->Projects->Count; 

			 string folderFullName;


			 String^ p1 = (gcnew String(_GetProjectsPath().c_str())) + textBox_engName->Text;

			 String^ p3=textBox_engName->Text+".cs";
			 String^ strCodeFile = Path::Combine( p1, p3 );
			 String^ strParamsFile = Path::Combine( p1, "params.xml" );
			 Project^ proj = nullptr;

			 EnvDTE90::Solution3^ soln = (EnvDTE90::Solution3^)_applicationObject->Solution;
			 proj = GetProjectByName(soln, "策略");
			 SolutionFolder^ SF = (SolutionFolder^)proj->Object;

			 String^ classLibProjTemplatePath = soln->GetProjectTemplate("ClassLibrary.zip", "CSharp");
			 String^ domainProjName = textBox_engName->Text;
			 Project^ StrategyProj = nullptr;

			 // 需要处理proj==null的情况
			 StrategyProj = SF->AddFromTemplate(classLibProjTemplatePath, 
				 p1, domainProjName);//, false);

			 String^ strCodeTempl = gcnew String((_GetSupportFilesPath()+"CStrategyTemplate.txt").c_str());
			 m_StrategyData->MakeCodeToFile(strCodeTempl, strCodeFile);
			 m_StrategyData->MakeXmlToFile(strParamsFile);

			 bool bFound = false;

			 for(int i = 0; i<proj->ProjectItems->Count; i++) {
				 ProjectItem^ proj2 = proj->ProjectItems->Item(i+1);
				 String^ strName = proj2->default;
				 if(strName == domainProjName) {
					 StrategyProj = (Project^)proj2->Object;
					 bFound = true;
					 break;
				 }
			 }

			 try {
				 StrategyProj->ProjectItems->AddFromFile(strCodeFile);
				 StrategyProj->ProjectItems->AddFromFile(strParamsFile);
				 VSProject^ vsproj = (VSProject^)StrategyProj->Object;
				 vsproj->References->Add(gcnew String((_GetSupportFilesPath()+"CommonBizEntity.dll").c_str()));
				 StrategyProj->Save(StrategyProj->FileName);
				 StrategyProj->ProjectItems->Item("Class1.cs")->Delete();
				 Document^ doc =StrategyProj->ProjectItems->Item(p3)->Open(Constants::vsViewKindTextView)->Document;
				 doc->Activate();

				 //SolutionBuild2^ sb = (SolutionBuild2^)soln->SolutionBuild; 
				 //sb->BuildProject("Release", StrategyProj->FullName, true);

				 //soln->SaveAs(soln->FileName);

				 //std::string strRetZipFile;
				 //std::string strWinRar(_GetSupportFilesPath());
				 //std::string strWorkPath(_GetProjectsPath());
				 //std::string strProjectname = Tools::String2string(domainProjName);
				 //CToolsEx::Zip_Project(strWinRar, strWorkPath, strProjectname, strRetZipFile);

				 //m_StrategyData->SetZipFile(strRetZipFile);
				 //m_StrategyData->SetDLLFile(_GetProjectsPath()+m_StrategyData->GetStrategyName()
					// +"\\bin\\Release\\"+m_StrategyData->GetStrategyName()+".dll");

				 //m_pDataInfo->GetTraderApi()->ReqUploadStategyBaseInfo(
					//	*m_StrategyData, 0);
				 //m_pDataInfo->GetTraderApi()->ReqUploadStategyFile(
					//	m_StrategyData->GetStrategyName(), 
					//	m_StrategyData->GetDLLFile(), 
					//	0, true);
				 //m_pDataInfo->GetTraderApi()->ReqUploadStategyFile(
					//	m_StrategyData->GetStrategyName(), 
					//	m_StrategyData->GetZipFile(), 0, false);
			 }
			 catch(Exception^ ex) {
				 Debug::WriteLine("{0}", ex->ToString());
				 //Console.WriteLine("{0}{1}",v1,v2);
			 }

			 
			 m_pDataInfo->AddStrategy(*m_StrategyData);
			 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
			 this->Close();
		 }
private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->DialogResult = System::Windows::Forms::DialogResult::No;
			 this->Close();
		 }
private: System::Void dataGridView1_CellDoubleClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
		 { 

		 if(dataGridView1->SelectedRows != nullptr && dataGridView1->SelectedRows->Count == 1)
			 {
				 DataGridViewRow^  dr = dataGridView1->CurrentRow;

				 StrategyPara field;
				 memset(&field,0,sizeof(field));
				 strncpy(field.szTitle,Tools::String2string(dr->Cells[0]->Value->ToString()).c_str(),sizeof(field.szTitle)-1);
				 strncpy(field.szName,Tools::String2string(dr->Cells[1]->Value->ToString()).c_str(),sizeof(field.szName)-1);
				 field.eType = StrategyType2Enum(Tools::String2string(dr->Cells[2]->Value->ToString()));
				 field.eDirection = StrategyDirection2Enum(Tools::String2string(dr->Cells[3]->Value->ToString()));
				 field.eDisplayType = DisplayType2Enum(Tools::String2string(dr->Cells[4]->Value->ToString()));
				 if ( dr->Cells[5]->Value != nullptr )
				 {
					 strncpy(field.szDefault,Tools::String2string(dr->Cells[5]->Value->ToString()).c_str(),sizeof(field.szDefault)-1);
				 }
				 else
				 {
                     strncpy(field.szDefault,NULL_VALUE,sizeof(field.szDefault)-1);
				 }
				 
				 StrategeParaConfig^ dlg = gcnew StrategeParaConfig(m_StrategyData,1);
				 dlg->SetStrategyParaInfo(field);

				 if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
				 {
					 StrategyPara pFieid;
					 memset(&pFieid,0,sizeof(StrategyPara));

					 pFieid=*(dlg->GetStrategyParaInfo());	
					 UpdateRow(dr,pFieid);
					 m_StrategyData->AddStrategyPara(pFieid);
				 }
			 }
			 else
				 MessageBox::Show("请选择内容");
		 }
private: System::Void dataGridView3_CellDoubleClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
		 {
			 if(dataGridView3->SelectedRows != nullptr && dataGridView3->SelectedRows->Count == 1)
			 {
				 DataGridViewRow^  dr = dataGridView3->CurrentRow;

				 StrategyIndicator field;
				 memset(&field,0,sizeof(field));
				 strncpy(field.szTitle,Tools::String2string(dr->Cells[0]->Value->ToString()).c_str(),sizeof(field.szTitle)-1);
				 /*strncpy(field.szName,Tools::String2string(dr->Cells[1]->Value->ToString()).c_str(),sizeof(field.szName)-1);
				 strncpy(field.szPara,Tools::String2string(dr->Cells[2]->Value->ToString()).c_str(),sizeof(field.szName)-1);
				 field.nInstrument =
				 if (dr->Cells[3]->Value == "true")
				 {
					 field.bOutput = true;
				 }
				 else
				 {
					 field.bOutput = false;
				 }*/
				 m_StrategyData->GetStrategyIndicator(field.szTitle,field);
				 
				 int nInsCount = dataGridView2->Rows->Count;
				 if (nInsCount == 0)
					 return;
				 //获取策略参数
				 int nCount = dataGridView1->Rows->Count;
				 multimap<string,string> strategyParaType2Name;
				 for ( int i =0; i< nCount; i++)
				 {
					 std::string strtype,strname;
					 strtype = Tools::String2string(dataGridView1->Rows[i]->Cells[2]->Value->ToString());
					 strname = Tools::String2string(dataGridView1->Rows[i]->Cells[1]->Value->ToString());
					 strategyParaType2Name.insert(make_pair(strtype,strname));
				 }
				 m_StrategyData->SetParaType2NameMap(strategyParaType2Name);

				 StrategeIndiConfig^ dlg = gcnew StrategeIndiConfig(nInsCount,m_StrategyData);
                 dlg->SetStrategyIndiConfig(field);
				 if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
				 {
					 StrategyIndicator pFieid;
					 memset(&pFieid,0,sizeof(StrategyIndicator));

					 pFieid=*(dlg->GetStrategyIndicator());	
					 UpdateIndiRow(dr,pFieid);

					 string szStrategy = Tools::String2string(textBox_engName->Text);
					 m_StrategyData->AddStrategyIndicator(szStrategy,pFieid);
				 }				
			 }
			 else
				 MessageBox::Show("请选择内容");
		 }
};
}

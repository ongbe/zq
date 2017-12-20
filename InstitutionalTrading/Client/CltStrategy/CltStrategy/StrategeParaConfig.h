#pragma once
#include "DataManageEx.h"
#include "Tools.h"
#include "..\CommonClasses\ConvEnum.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Collections::Generic;



namespace CltStrategy {

	/// <summary>
	/// Summary for StrategeParaConfig
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class StrategeParaConfig : public System::Windows::Forms::Form
	{
	public:
		StrategeParaConfig( CStrategyDataEx*  pStrategyData ,int nParaCount )
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pDataInfo = CDataManageEx::NewInstance();
			m_StrategyPara = new StrategyPara;
			
			bFlag =true;//触发事件
			m_StrategyData = pStrategyData;
			m_bOprationType=true;
			m_nParaCount=nParaCount;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~StrategeParaConfig()
		{
			if (components)
			{
				delete components;
			}
			if ( NULL != m_StrategyPara )
			{
				delete m_StrategyPara;
				m_StrategyPara = NULL;
			}
			CDataManageEx::DestroyInstance();
		}
	private: int m_nParaCount;
	private: bool             m_bOprationType;//新增参数true,修改参数false
	private: CStrategyDataEx*  m_StrategyData;
	private: CDataManageEx*     m_pDataInfo;
	private: bool            bFlag;
	private: StrategyPara*   m_StrategyPara;
	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox_title;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::ComboBox^  comboBox_type;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::ComboBox^  comboBox_direction;

	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::ComboBox^  comboBox_display;

	private: System::Windows::Forms::GroupBox^  groupBox_default;
	private: System::Windows::Forms::Label^  label_readonly;
	private: System::Windows::Forms::TextBox^  textBox_default;

	private: System::Windows::Forms::Label^  label_default;
	private: System::Windows::Forms::ComboBox^  comboBox_readonly;

	private: System::Windows::Forms::Button^  button_ok;
	private: System::Windows::Forms::Button^  button_cancel;
	private: System::Windows::Forms::GroupBox^  groupBox_radio;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::TextBox^  textBox_name;
	private: System::Windows::Forms::TextBox^  textBox_value;


	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_name;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_value;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_default;
	private: System::Windows::Forms::Button^  button_default;
	private: System::Windows::Forms::Button^  button_delete;
	private: System::Windows::Forms::Button^  button_add;
	private: System::Windows::Forms::ComboBox^  comboBox_borsDirection;

	private: System::Windows::Forms::Label^  label_BOrSdirection;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;





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
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox_title = (gcnew System::Windows::Forms::TextBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->comboBox_type = (gcnew System::Windows::Forms::ComboBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->comboBox_direction = (gcnew System::Windows::Forms::ComboBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->comboBox_display = (gcnew System::Windows::Forms::ComboBox());
			this->groupBox_default = (gcnew System::Windows::Forms::GroupBox());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->comboBox_borsDirection = (gcnew System::Windows::Forms::ComboBox());
			this->label_BOrSdirection = (gcnew System::Windows::Forms::Label());
			this->textBox_default = (gcnew System::Windows::Forms::TextBox());
			this->label_default = (gcnew System::Windows::Forms::Label());
			this->comboBox_readonly = (gcnew System::Windows::Forms::ComboBox());
			this->label_readonly = (gcnew System::Windows::Forms::Label());
			this->button_ok = (gcnew System::Windows::Forms::Button());
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->groupBox_radio = (gcnew System::Windows::Forms::GroupBox());
			this->button_default = (gcnew System::Windows::Forms::Button());
			this->button_delete = (gcnew System::Windows::Forms::Button());
			this->button_add = (gcnew System::Windows::Forms::Button());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column_name = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_value = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_default = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->textBox_value = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->textBox_name = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->groupBox_default->SuspendLayout();
			this->groupBox_radio->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(38, 18);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"标题：";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(250, 18);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(41, 12);
			this->label2->TabIndex = 1;
			this->label2->Text = L"名称：";
			// 
			// textBox_title
			// 
			this->textBox_title->Location = System::Drawing::Point(86, 13);
			this->textBox_title->Name = L"textBox_title";
			this->textBox_title->Size = System::Drawing::Size(120, 21);
			this->textBox_title->TabIndex = 2;
			this->textBox_title->Text = L"参数1";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(297, 15);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(121, 21);
			this->textBox1->TabIndex = 3;
			this->textBox1->Text = L"P1";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(38, 51);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(41, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"类型：";
			// 
			// comboBox_type
			// 
			this->comboBox_type->FormattingEnabled = true;
			this->comboBox_type->ImeMode = System::Windows::Forms::ImeMode::NoControl;
			this->comboBox_type->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"int", L"double", L"bool", L"String", L"DateTime"});
			this->comboBox_type->Location = System::Drawing::Point(85, 43);
			this->comboBox_type->Name = L"comboBox_type";
			this->comboBox_type->Size = System::Drawing::Size(121, 20);
			this->comboBox_type->TabIndex = 5;
			this->comboBox_type->Text = L"int";
			this->comboBox_type->SelectedIndexChanged += gcnew System::EventHandler(this, &StrategeParaConfig::comboBox_type_SelectedIndexChanged);
			this->comboBox_type->SelectedValueChanged += gcnew System::EventHandler(this, &StrategeParaConfig::comboBox_type_SelectedValueChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(252, 50);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(41, 12);
			this->label4->TabIndex = 6;
			this->label4->Text = L"方向：";
			// 
			// comboBox_direction
			// 
			this->comboBox_direction->FormattingEnabled = true;
			this->comboBox_direction->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"IN", L"OUT", L"INOUT"});
			this->comboBox_direction->Location = System::Drawing::Point(297, 43);
			this->comboBox_direction->Name = L"comboBox_direction";
			this->comboBox_direction->Size = System::Drawing::Size(121, 20);
			this->comboBox_direction->TabIndex = 7;
			this->comboBox_direction->Text = L"IN";
			this->comboBox_direction->SelectedIndexChanged += gcnew System::EventHandler(this, &StrategeParaConfig::comboBox_direction_SelectedIndexChanged);
			this->comboBox_direction->SelectedValueChanged += gcnew System::EventHandler(this, &StrategeParaConfig::comboBox_direction_SelectedValueChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(14, 78);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(65, 12);
			this->label5->TabIndex = 8;
			this->label5->Text = L"显示方式：";
			// 
			// comboBox_display
			// 
			this->comboBox_display->FormattingEnabled = true;
			this->comboBox_display->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"文本框", L"单选框", L"下拉框", L"买卖方向", L"开平方向"});
			this->comboBox_display->Location = System::Drawing::Point(85, 75);
			this->comboBox_display->Name = L"comboBox_display";
			this->comboBox_display->Size = System::Drawing::Size(121, 20);
			this->comboBox_display->TabIndex = 9;
			this->comboBox_display->Text = L"文本框";
			this->comboBox_display->SelectedIndexChanged += gcnew System::EventHandler(this, &StrategeParaConfig::comboBox_display_SelectedIndexChanged);
			this->comboBox_display->SelectedValueChanged += gcnew System::EventHandler(this, &StrategeParaConfig::comboBox_display_SelectedValueChanged);
			// 
			// groupBox_default
			// 
			this->groupBox_default->Controls->Add(this->dateTimePicker1);
			this->groupBox_default->Controls->Add(this->comboBox_borsDirection);
			this->groupBox_default->Controls->Add(this->label_BOrSdirection);
			this->groupBox_default->Controls->Add(this->textBox_default);
			this->groupBox_default->Controls->Add(this->label_default);
			this->groupBox_default->Controls->Add(this->comboBox_readonly);
			this->groupBox_default->Controls->Add(this->label_readonly);
			this->groupBox_default->Location = System::Drawing::Point(16, 115);
			this->groupBox_default->Name = L"groupBox_default";
			this->groupBox_default->Size = System::Drawing::Size(427, 62);
			this->groupBox_default->TabIndex = 10;
			this->groupBox_default->TabStop = false;
			this->groupBox_default->Text = L"默认值";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Short;
			this->dateTimePicker1->Location = System::Drawing::Point(145, 17);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->ShowUpDown = true;
			this->dateTimePicker1->Size = System::Drawing::Size(144, 21);
			this->dateTimePicker1->TabIndex = 6;
			this->dateTimePicker1->Visible = false;
			// 
			// comboBox_borsDirection
			// 
			this->comboBox_borsDirection->FormattingEnabled = true;
			this->comboBox_borsDirection->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"买（1）", L"卖（-1）"});
			this->comboBox_borsDirection->Location = System::Drawing::Point(93, 44);
			this->comboBox_borsDirection->Name = L"comboBox_borsDirection";
			this->comboBox_borsDirection->Size = System::Drawing::Size(309, 20);
			this->comboBox_borsDirection->TabIndex = 5;
			this->comboBox_borsDirection->Text = L"买（1）";
			this->comboBox_borsDirection->Visible = false;
			// 
			// label_BOrSdirection
			// 
			this->label_BOrSdirection->AutoSize = true;
			this->label_BOrSdirection->Location = System::Drawing::Point(22, 44);
			this->label_BOrSdirection->Name = L"label_BOrSdirection";
			this->label_BOrSdirection->Size = System::Drawing::Size(65, 12);
			this->label_BOrSdirection->TabIndex = 4;
			this->label_BOrSdirection->Text = L"买卖方向：";
			this->label_BOrSdirection->Visible = false;
			// 
			// textBox_default
			// 
			this->textBox_default->Location = System::Drawing::Point(296, 21);
			this->textBox_default->Name = L"textBox_default";
			this->textBox_default->Size = System::Drawing::Size(106, 21);
			this->textBox_default->TabIndex = 3;
			this->textBox_default->Text = L"0";
			// 
			// label_default
			// 
			this->label_default->AutoSize = true;
			this->label_default->Location = System::Drawing::Point(236, 28);
			this->label_default->Name = L"label_default";
			this->label_default->Size = System::Drawing::Size(53, 12);
			this->label_default->TabIndex = 2;
			this->label_default->Text = L"默认值：";
			// 
			// comboBox_readonly
			// 
			this->comboBox_readonly->FormattingEnabled = true;
			this->comboBox_readonly->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"true", L"false"});
			this->comboBox_readonly->Location = System::Drawing::Point(70, 21);
			this->comboBox_readonly->Name = L"comboBox_readonly";
			this->comboBox_readonly->Size = System::Drawing::Size(121, 20);
			this->comboBox_readonly->TabIndex = 1;
			this->comboBox_readonly->Text = L"false";
			// 
			// label_readonly
			// 
			this->label_readonly->AutoSize = true;
			this->label_readonly->Location = System::Drawing::Point(22, 28);
			this->label_readonly->Name = L"label_readonly";
			this->label_readonly->Size = System::Drawing::Size(41, 12);
			this->label_readonly->TabIndex = 0;
			this->label_readonly->Text = L"只读：";
			// 
			// button_ok
			// 
			this->button_ok->Location = System::Drawing::Point(241, 382);
			this->button_ok->Name = L"button_ok";
			this->button_ok->Size = System::Drawing::Size(75, 23);
			this->button_ok->TabIndex = 11;
			this->button_ok->Text = L"确定";
			this->button_ok->UseVisualStyleBackColor = true;
			this->button_ok->Click += gcnew System::EventHandler(this, &StrategeParaConfig::button_ok_Click);
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(343, 382);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(75, 23);
			this->button_cancel->TabIndex = 12;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &StrategeParaConfig::button_cancel_Click);
			// 
			// groupBox_radio
			// 
			this->groupBox_radio->Controls->Add(this->button_default);
			this->groupBox_radio->Controls->Add(this->button_delete);
			this->groupBox_radio->Controls->Add(this->button_add);
			this->groupBox_radio->Controls->Add(this->dataGridView1);
			this->groupBox_radio->Controls->Add(this->textBox_value);
			this->groupBox_radio->Controls->Add(this->label7);
			this->groupBox_radio->Controls->Add(this->textBox_name);
			this->groupBox_radio->Controls->Add(this->label6);
			this->groupBox_radio->Location = System::Drawing::Point(16, 183);
			this->groupBox_radio->Name = L"groupBox_radio";
			this->groupBox_radio->Size = System::Drawing::Size(427, 184);
			this->groupBox_radio->TabIndex = 13;
			this->groupBox_radio->TabStop = false;
			this->groupBox_radio->Text = L"单选框、下拉框";
			this->groupBox_radio->Visible = false;
			// 
			// button_default
			// 
			this->button_default->Location = System::Drawing::Point(340, 139);
			this->button_default->Name = L"button_default";
			this->button_default->Size = System::Drawing::Size(62, 23);
			this->button_default->TabIndex = 7;
			this->button_default->Text = L"默认";
			this->button_default->UseVisualStyleBackColor = true;
			this->button_default->Click += gcnew System::EventHandler(this, &StrategeParaConfig::button_default_Click);
			// 
			// button_delete
			// 
			this->button_delete->Location = System::Drawing::Point(340, 98);
			this->button_delete->Name = L"button_delete";
			this->button_delete->Size = System::Drawing::Size(62, 23);
			this->button_delete->TabIndex = 6;
			this->button_delete->Text = L"删除";
			this->button_delete->UseVisualStyleBackColor = true;
			this->button_delete->Click += gcnew System::EventHandler(this, &StrategeParaConfig::button_delete_Click);
			// 
			// button_add
			// 
			this->button_add->Location = System::Drawing::Point(340, 59);
			this->button_add->Name = L"button_add";
			this->button_add->Size = System::Drawing::Size(62, 23);
			this->button_add->TabIndex = 5;
			this->button_add->Text = L"新增";
			this->button_add->UseVisualStyleBackColor = true;
			this->button_add->Click += gcnew System::EventHandler(this, &StrategeParaConfig::button_add_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->AllowUserToOrderColumns = true;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->Column_name, 
				this->Column_value, this->Column_default});
			this->dataGridView1->Location = System::Drawing::Point(24, 59);
			this->dataGridView1->MultiSelect = false;
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(303, 107);
			this->dataGridView1->TabIndex = 4;
			// 
			// Column_name
			// 
			this->Column_name->HeaderText = L"选项名称";
			this->Column_name->Name = L"Column_name";
			this->Column_name->ReadOnly = true;
			// 
			// Column_value
			// 
			this->Column_value->HeaderText = L"值";
			this->Column_value->Name = L"Column_value";
			this->Column_value->ReadOnly = true;
			// 
			// Column_default
			// 
			this->Column_default->HeaderText = L"默认";
			this->Column_default->Name = L"Column_default";
			this->Column_default->ReadOnly = true;
			// 
			// textBox_value
			// 
			this->textBox_value->Location = System::Drawing::Point(271, 23);
			this->textBox_value->Name = L"textBox_value";
			this->textBox_value->Size = System::Drawing::Size(131, 21);
			this->textBox_value->TabIndex = 3;
			this->textBox_value->TextChanged += gcnew System::EventHandler(this, &StrategeParaConfig::textBox4_TextChanged);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(236, 32);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(29, 12);
			this->label7->TabIndex = 2;
			this->label7->Text = L"值：";
			// 
			// textBox_name
			// 
			this->textBox_name->Location = System::Drawing::Point(93, 24);
			this->textBox_name->Name = L"textBox_name";
			this->textBox_name->Size = System::Drawing::Size(100, 21);
			this->textBox_name->TabIndex = 1;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(22, 33);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(65, 12);
			this->label6->TabIndex = 0;
			this->label6->Text = L"选项名称：";
			// 
			// StrategeParaConfig
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(467, 421);
			this->Controls->Add(this->groupBox_radio);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->button_ok);
			this->Controls->Add(this->groupBox_default);
			this->Controls->Add(this->comboBox_display);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->comboBox_direction);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->comboBox_type);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->textBox_title);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Name = L"StrategeParaConfig";
			this->Text = L"参数配置";
			this->Load += gcnew System::EventHandler(this, &StrategeParaConfig::StrategeParaConfig_Load);
			this->groupBox_default->ResumeLayout(false);
			this->groupBox_default->PerformLayout();
			this->groupBox_radio->ResumeLayout(false);
			this->groupBox_radio->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void StrategeParaConfig_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				this->CenterToParent();
				if ( !m_bOprationType )//修改
				{
					textBox_title->Text  = Tools::string2String(m_StrategyPara->szTitle); 
					textBox_name->Text  = Tools::string2String(m_StrategyPara->szName); 
					comboBox_type->Text = Tools::string2String(StrategyType2String(m_StrategyPara->eType));
					comboBox_direction->Text = Tools::string2String(DirectionType2String(m_StrategyPara->eDirection));
					comboBox_display->Text = Tools::string2String(DisplayType2String(m_StrategyPara->eDisplayType));
					SetDynamicZoneInf_display();
					SetDynamicZoneInf_direction();
					SetDefaultValue();


					
				}
				else //新增
				{
					char strtemp[32];
					memset(strtemp,0,sizeof(strtemp));
					sprintf( strtemp, "%s%d","参数",m_nParaCount );
					textBox_title->Text  = Tools::string2String(strtemp); 
					memset(strtemp,0,sizeof(strtemp));
					sprintf( strtemp, "%s%d","p",m_nParaCount );
					textBox1->Text  = Tools::string2String(strtemp); 
				}
 

			 }
	public:StrategyPara* GetStrategyParaInfo()
		   {

			   return m_StrategyPara;
		   }
	public: void SetStrategyParaInfo(const StrategyPara& Para)
			{
				*m_StrategyPara = Para;
				m_bOprationType=false;
				this->textBox_title->Enabled = false;
			}

private: System::Void SetEditBox()
		 {
			 groupBox_default->Visible = true;
			 groupBox_radio->Visible = false;
			 this->dateTimePicker1->Visible =false;
			 label_BOrSdirection->Visible =false;
			 comboBox_borsDirection->Visible = false;
			 this->textBox_default->Visible = true;
			 this->label_default->Visible = true;
			 this->comboBox_readonly->Visible = true;
			 this->label_readonly->Visible = true;

			 this->textBox_default->Location = System::Drawing::Point(296, 21);
			 this->textBox_default->Size = System::Drawing::Size(106, 21);
			 this->textBox_default->Text = L"0";

			 this->label_default->Location = System::Drawing::Point(236, 28);
			 this->label_default->Size = System::Drawing::Size(53, 12);
			 this->label_default->Text = L"默认值：";

			 this->comboBox_readonly->Items->Clear();
			 this->comboBox_readonly->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"true", L"false"});
			 this->comboBox_readonly->Location = System::Drawing::Point(70, 21);
			 this->comboBox_readonly->Size = System::Drawing::Size(121, 20);
			 this->comboBox_readonly->Text = L"false";

			 this->label_readonly->Location = System::Drawing::Point(22, 28);
			 this->label_readonly->Size = System::Drawing::Size(41, 12);
			 this->label_readonly->Text = L"只读：";

		 }
private: System::Void SetDirectionBox()
		 {
			 groupBox_default->Visible = true;
			 label_readonly->Visible = false;
			 comboBox_readonly->Visible = false;
			 label_default->Visible = false;
			 textBox_default->Visible  = false;
			 this->dateTimePicker1->Visible =false;

			 label_BOrSdirection->Visible = true;
			 label_BOrSdirection->Text = L"买卖方向：";
			 comboBox_borsDirection->Visible = true;
			 comboBox_borsDirection->Items->Clear();
			 comboBox_borsDirection->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"买（1）", L"卖（-1）"});
			 comboBox_borsDirection->Text = L"买（1）";
			 label_BOrSdirection->Location = System::Drawing::Point(22, 28);
			 comboBox_borsDirection->Location = System::Drawing::Point(93, 21);

			 groupBox_radio->Visible = false;
		 }
 private: System::Void SetOffsetFlagBox()
		 {
			 groupBox_default->Visible = true;
			 label_readonly->Visible = false;
			 comboBox_readonly->Visible = false;
			 label_default->Visible = false;
			 textBox_default->Visible  = false;
			 this->dateTimePicker1->Visible =false;
			 label_BOrSdirection->Visible = true;
			 label_BOrSdirection->Text = L"开平方向：";
			 comboBox_borsDirection->Visible = true;
			 comboBox_borsDirection->Items->Clear();
			 comboBox_borsDirection->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"开（1）", L"平（-1）"});
			 comboBox_borsDirection->Text = L"开（1）";
			 label_BOrSdirection->Location = System::Drawing::Point(22, 28);
			 comboBox_borsDirection->Location = System::Drawing::Point(93, 21);

			 groupBox_radio->Visible = false;
		  }
 private: System::Void SetBOOLBox()
		  {
			  groupBox_default->Visible = true;
			  label_readonly->Visible = false;
			  this->dateTimePicker1->Visible =false;
			  label_BOrSdirection->Visible =false;
			  comboBox_borsDirection->Visible = false;
			  comboBox_readonly->Visible = false;
			  label_default->Visible = false;
			  textBox_default->Visible  = false;
			  label_BOrSdirection->Visible = true;
			  label_BOrSdirection->Text = L"布尔：";
			  comboBox_borsDirection->Visible = true;
			  comboBox_borsDirection->Items->Clear();
			  comboBox_borsDirection->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"true", L"false"});
			  comboBox_borsDirection->Text = L"true";
			  label_BOrSdirection->Location = System::Drawing::Point(22, 28);
			  comboBox_borsDirection->Location = System::Drawing::Point(93, 21);

			  groupBox_radio->Visible = false;
		  }
private: System::Void SetCYCLEBox()
		 {
			 groupBox_default->Visible = true;
			 groupBox_radio->Visible = false;

		     this->dateTimePicker1->Visible =false;
			 label_BOrSdirection->Visible =false;
			 comboBox_borsDirection->Visible = false;

			 this->textBox_default->Location = System::Drawing::Point(70, 21);
			 this->textBox_default->Size = System::Drawing::Size(106, 21);
			 this->textBox_default->Text = L"5";
			 this->textBox_default->Visible = true;

			 this->label_default->Location = System::Drawing::Point(236, 28);
			 this->label_default->Size = System::Drawing::Size(53, 12);
			 this->label_default->Text = L" ";
			 this->label_default->Visible = true;

			 this->comboBox_readonly->Items->Clear();
			 this->comboBox_readonly->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"秒", L"分",L"时",L"天"});
			 this->comboBox_readonly->Location = System::Drawing::Point(200, 21);
			 this->comboBox_readonly->Size = System::Drawing::Size(121, 20);
			 this->comboBox_readonly->Text = L"秒";
			 this->comboBox_readonly->Visible = true;

			 this->label_readonly->Location = System::Drawing::Point(22, 28);
			 this->label_readonly->Size = System::Drawing::Size(41, 12);
			 this->label_readonly->Text = L"周期：";
			 this->label_readonly->Visible = true;

		 }

private: System::Void SetTimeBox()
		 {
			 groupBox_default->Visible = true;
			 groupBox_radio->Visible = false;

			 this->textBox_default->Visible = false;
			 this->label_default->Visible = false;
			 label_BOrSdirection->Visible =false;
			 comboBox_borsDirection->Visible = false;
			 this->comboBox_readonly->Visible = false;
			 this->dateTimePicker1->Visible =true;
			 this->dateTimePicker1->Location = System::Drawing::Point(70, 21);

			 
			 this->dateTimePicker1->CustomFormat = L"yyyy-MM-dd hh:mm:ss";
			 this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
			 /*this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Time;
			 this->dateTimePicker1->ShowUpDown = true;*/
			 this->label_readonly->Location = System::Drawing::Point(22, 28);
			 this->label_readonly->Size = System::Drawing::Size(41, 12);
			 this->label_readonly->Text = L"时间：";
			 this->label_readonly->Visible = true;

		 }
private: System::Void SetDynamicZoneInf_display()
		 {
			 bFlag = false;
			 int nDisplay = DisplayType2Enum(Tools::String2string(comboBox_display->Text));
			 switch(nDisplay)
			 {
			 case EditBox:	
				 {
					 SetEditBox();
				 }
				 break;
			 case RadioBox:	
			 case DropDownBox:
				 {
					 groupBox_default->Visible = false;
					 groupBox_radio->Visible = true;
					 groupBox_radio->Location = System::Drawing::Point(16, 115);
				 }
				 break;
			 case DirectionBox:
				 {

					 SetDirectionBox();

					 groupBox_default->Visible = true;
					 label_readonly->Visible = false;
					 comboBox_readonly->Visible = false;
					 label_readonly->Visible = false;
					 textBox_value->Visible  = false;
					 label_BOrSdirection->Location = System::Drawing::Point(22, 28);
					 comboBox_borsDirection->Location = System::Drawing::Point(93, 21);

					 groupBox_radio->Visible = false;

				 }
				 break;
			 case OffsetFlagBox:
				 {
					 SetOffsetFlagBox();
				 }
				 break;
			 case BOOLBox:
				 {
					 SetBOOLBox();
				 }
				 break;
			 case CYCLEBox:
				 {
					 SetCYCLEBox();
				 }
			 default:
				 break;
			 }
             bFlag = true;
		 }
private: System::Void SetDynamicZoneInf_direction()
		 {
			 if(comboBox_direction->Text != "OUT")
			 {
			 }
			 else
			 {
				 groupBox_default->Visible = false;
				 comboBox_display->Enabled = false;

			 }

		 }
private: System::Void SetDynamicZoneInf_type()
		 {
			 bFlag = false;
			 int nType=comboBox_type->SelectedIndex;
			 comboBox_display->Enabled = true;

			 switch(nType)
			 {
			 case StrategyIntegerType:	
				 {
					 comboBox_display->Items->Clear();
					 this->comboBox_display->Items->AddRange(gcnew cli::array< System::Object^  >(5){L"文本框", L"下拉框",L"单选框",L"买卖方向",L"开平方向"});
					 this->comboBox_display->Text = L"文本框";

					 if(comboBox_direction->Text != "OUT")
					 {
						 int nDisplay = DisplayType2Enum(Tools::String2string(comboBox_display->Text));
						 switch(nDisplay)
						 {
						 case EditBox:	
							 {
								 SetEditBox();
							 }
							 break;
						 case RadioBox:	
						 case DropDownBox:
							 {
								 groupBox_default->Visible = false;
								 groupBox_radio->Visible = true;
								 groupBox_radio->Location = System::Drawing::Point(16, 115);
							 }
							 break;
						 case DirectionBox:
							 {

								 SetDirectionBox();

								 groupBox_default->Visible = true;
								 label_readonly->Visible = false;
								 comboBox_readonly->Visible = false;
								 label_readonly->Visible = false;
								 textBox_value->Visible  = false;
								 label_BOrSdirection->Location = System::Drawing::Point(22, 28);
								 comboBox_borsDirection->Location = System::Drawing::Point(93, 21);

								 groupBox_radio->Visible = false;

							 }
							 break;
						 case OffsetFlagBox:
							 {
								 SetOffsetFlagBox();
							 }
							 break;
						 default:
							 break;
						 }

					 }
					 else
					 {
						 groupBox_default->Visible = false;
						 comboBox_display->Enabled = false;

					 }


				 }
				 break;
			 case StrategyDoubleType:	 
				 {
					 comboBox_display->Items->Clear();
					 this->comboBox_display->Items->AddRange(gcnew cli::array< System::Object^  >(3){L"文本框", L"下拉框",L"单选框"});
					 this->comboBox_display->Text = L"文本框";
					 if(comboBox_direction->Text != "OUT")
					 {
						 int nDisplay = DisplayType2Enum(Tools::String2string(comboBox_display->Text));
						 switch(nDisplay)
						 {
						 case EditBox:	
							 {
								 SetEditBox();
								 this->textBox_default->Text = L"0.0";
							 }
							 break;
						 case RadioBox:	
						 case DropDownBox:
							 {
								 groupBox_default->Visible = false;
								 groupBox_radio->Visible = true;
								 groupBox_radio->Location = System::Drawing::Point(16, 115);
							 }
							 break;

						 default:
							 break;
						 }

					 }
					 else
					 {
						 groupBox_default->Visible = false;
						 comboBox_display->Enabled = false;

					 }

				 }
				 break;
			 case StrategyBooleanType:	
				 {
					 if(comboBox_direction->Text != "OUT")
					 {
						 comboBox_display->Items->Clear();
						 this->comboBox_display->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"布尔"});
						 this->comboBox_display->Text = L"布尔";

						 SetBOOLBox();

					 }
					 else
					 {
						 groupBox_default->Visible = false;
						 comboBox_display->Enabled = false;

					 }
				 }
				 break;
			 case StrategyStringType:	 
				 {
					 comboBox_display->Items->Clear();
					 this->comboBox_display->Items->AddRange(gcnew cli::array< System::Object^  >(7){L"文本框", L"下拉框",L"单选框",L"布尔",L"周期",L"买卖方向",L"开平方向"});
					 this->comboBox_display->Text = L"文本框";
					 if(comboBox_direction->Text != "OUT")
					 {
						 int nDisplay = DisplayType2Enum(Tools::String2string(comboBox_display->Text));
						 switch(nDisplay)
						 {
						 case EditBox:	
							 {
								 SetEditBox();
								 this->textBox_default->Text = L"";
							 }
							 break;
						 case RadioBox:	
						 case DropDownBox:
							 {
								 groupBox_default->Visible = false;
								 groupBox_radio->Visible = true;
								 groupBox_radio->Location = System::Drawing::Point(16, 115);
							 }
							 break;
						 case DirectionBox:
							 {
								 SetDirectionBox();
							 }
							 break;
						 case OffsetFlagBox:
							 {
								 SetOffsetFlagBox();
							 }
							 break;
						 case BOOLBox:
							 {
								 SetBOOLBox();
							 }
							 break;
						 case CYCLEBox:
							 {
								 SetCYCLEBox();
							 }
							 break;
						 default:
							 break;
						 }

					 }
					 else
					 {
						 groupBox_default->Visible = false;
						 comboBox_display->Enabled = false;

					 }
				 }
				 break;
			 case StrategyDateTimeType:	
				 {
					 if(comboBox_direction->Text != "OUT")
					 {
						 comboBox_display->Items->Clear();
						 this->comboBox_display->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"日期时间输入框"});
						 this->comboBox_display->Text = L"日期时间输入框";

						 SetTimeBox();

					 }
					 else
					 {
						 groupBox_default->Visible = false;
						 comboBox_display->Enabled = false;

					 }
				 }
				 break;
			 case StrategyCycleType:	
				 {
					 if(comboBox_direction->Text != "OUT")
					 {
						 SetCYCLEBox();

					 }
					 else
					 {
						 groupBox_default->Visible = false;
						 comboBox_display->Enabled = false;

					 }
				 }
				 break;
			 case SubStrategyType:	 
				 {
					 groupBox_default->Visible = false;
					 comboBox_display->Enabled = false;

					 comboBox_display->Items->Clear();
					 this->comboBox_display->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"子策略"});
					 this->comboBox_display->Text = L"子策略";
				 }
				 break;
			 default:
				 break;
			 }
			 bFlag = true;
		 }
private: System::Void comboBox_type_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			// SetDynamicTimeZoneInf();
			
		 }
private: System::Void comboBox_direction_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //SetDynamicTimeZoneInf();
		 }
private: System::Void comboBox_display_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //SetDynamicTimeZoneInf();
		 }
private: System::Void button_add_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			if( String::IsNullOrEmpty(textBox_name->Text) || String::IsNullOrEmpty(textBox_value->Text) )  
			{
				MessageBox::Show("名称和值不能为空!");    
				return;
			}
			String ^ strName = textBox_name->Text;
			String^ strValue = textBox_value->Text;
			/*if (m_pDataInfo->IsInteger(Tools::String2string(strValue)) == false)
			{
				MessageBox::Show("输入的值与类型不符!"); 
				return;
			}*/
			if (comboBox_type->Text == "int" || comboBox_type->Text == "double" )
			{
                textBox_value->ImeMode = System::Windows::Forms::ImeMode::Disable;
			}
			
			int newRowIndex=dataGridView1->Rows->Add();
			DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
			dr->Cells[0]->Value = strName;
			dr->Cells[1]->Value = strValue;

			textBox_name->Clear();
			textBox_value->Clear();

		}


private: System::Void button_delete_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 DataGridViewRow^  dr = dataGridView1->CurrentRow;
			 if(dr != nullptr)
			 { 
				
				 dataGridView1->Rows->RemoveAt(dr->Index);

			 }
		 }
private: System::Void button_default_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 DataGridViewRow^  dr = dataGridView1->CurrentRow;
			 for(int i=0; i<dataGridView1->Rows->Count; i++) 
			 {
				 dataGridView1->Rows[i]->Cells[2]->Value = nullptr;
			 }
			 if(dr != nullptr)
			 { 

				 dr->Cells[2]->Value = "默认";

			 }
		 }
private: System::Void textBox4_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: bool  GetDefaultValue()
		 {

			 std::string strStemp;
			 if(comboBox_direction->Text != "OUT")
			 {
				 int nDisplay = DisplayType2Enum(Tools::String2string(comboBox_display->Text));
				 switch(nDisplay)
				 {
				 case EditBox:	
					 {
						 strncpy(m_StrategyPara->szDefault , Tools::String2string(textBox_default->Text).c_str(),sizeof(m_StrategyPara->szDefault)-1);
						 m_StrategyPara->bReadOnly = (comboBox_readonly->SelectedIndex == 0) ? true : false;
					 }
					 break;
				 case RadioBox:	
				 case DropDownBox:
					 {
						 if (dataGridView1->Rows->Count == 0)
						 {
							 MessageBox::Show("至少添加一个选项");
							 return false;

						 }
						 else
						 {
							 strStemp = Tools::String2string(dataGridView1->Rows[0]->Cells[1]->Value->ToString()); 
							 std::list<StrategyPrarSubItem> ListPrarSubItem;
							 for( int i = 0;i<dataGridView1->Rows->Count; i++ )
							 {
								  string strName = Tools::String2string(dataGridView1->Rows[i]->Cells[0]->Value->ToString());
								  string strValue = Tools::String2string(dataGridView1->Rows[i]->Cells[1]->Value->ToString());
								  StrategyPrarSubItem filed;
								  memset(&filed,0,sizeof(StrategyPrarSubItem));
								  strncpy(filed.szName , strName.c_str(),sizeof(filed.szName)-1);
								  strncpy(filed.szValue , strValue.c_str(),sizeof(filed.szValue)-1);
								  

								 if (dataGridView1->Rows[i]->Cells[2]->Value != nullptr)
								 {
									 filed.bDefault =true;
									 strStemp = Tools::String2string(dataGridView1->Rows[i]->Cells[1]->Value->ToString());
								 }
								 else
								 {
									 filed.bDefault =false;
								 }
								 ListPrarSubItem.push_back(filed);
							 }

                             strncpy(m_StrategyPara->szDefault , strStemp.c_str(),sizeof(m_StrategyPara->szDefault)-1);

							 string szTitle =  Tools::String2string(textBox_title->Text);							 							 
							 m_StrategyData->SetParaSubItem(szTitle,ListPrarSubItem);

							
							 
						 }
					 }
					 break;
				 case DirectionBox:
					 {

						 if (comboBox_borsDirection->Text == L"买（1）")
						 {
							 strncpy(m_StrategyPara->szDefault , "1",sizeof(m_StrategyPara->szDefault)-1);
						 }
						 else
						 {
							 strncpy(m_StrategyPara->szDefault , "-1",sizeof(m_StrategyPara->szDefault)-1);

						 }

					 }
					 break;
				 case OffsetFlagBox:
					 {
						 if (comboBox_borsDirection->Text == L"开（1）")
						 {
							 strncpy(m_StrategyPara->szDefault , "1",sizeof(m_StrategyPara->szDefault)-1);
						 }
						 else
						 {
							 strncpy(m_StrategyPara->szDefault , "-1",sizeof(m_StrategyPara->szDefault)-1);

						 }
					 }
					 break;
				 case BOOLBox:
					 {
						 if (comboBox_borsDirection->Text == L"true")
						 {
							 strncpy(m_StrategyPara->szDefault , "true",sizeof(m_StrategyPara->szDefault)-1);
						 }
						 else
						 {
							 strncpy(m_StrategyPara->szDefault , "false",sizeof(m_StrategyPara->szDefault)-1);

						 }
					 }
					 break;
				 case CYCLEBox:
					 {
						 if (comboBox_readonly->Text == L"秒")
						 {
							 strStemp = Tools::String2string((textBox_default->Text) + "s");

						 }
						 else if(comboBox_readonly->Text == L"分")
						 {
							 strStemp = Tools::String2string((textBox_default->Text) + "m");

						 }
						 else if(comboBox_readonly->Text == L"时")
						 {
							 strStemp = Tools::String2string((textBox_default->Text) + "h");

						 }
						 else if(comboBox_readonly->Text == L"天")
						 {
							 if (atoi(Tools::String2string(textBox_default->Text).c_str()) != 1)
							 {
								 MessageBox::Show("天类型K线支持一天，请修改！");
								  return false;
							 }
							 else
							 {
								 strStemp = Tools::String2string((comboBox_readonly->Text) + "d");
								 strncpy(m_StrategyPara->szDefault , "true",sizeof(m_StrategyPara->szDefault)-1);
							 }


						 }


					 }
					 break;
				 case TimeBox:
				 //case DateBox:
					 {
						 strncpy(m_StrategyPara->szDefault ,Tools::String2string(dateTimePicker1->Text).c_str(),sizeof(m_StrategyPara->szDefault)-1);
					 }
					 break;
				 default:
					 break;
				 }

			 }
			 else
			 {

				 strncpy(m_StrategyPara->szDefault ,NULL_VALUE,sizeof(m_StrategyPara->szDefault)-1);

			}

			 return true;

		 }
		 private: void  SetDefaultValue()
				  {

					  std::string strStemp;
					  if(comboBox_direction->Text != "OUT")
					  {
						  int nDisplay = DisplayType2Enum(Tools::String2string(comboBox_display->Text));
						  switch(nDisplay)
						  {
						  case EditBox:	
							  {
								  textBox_default->Text  = Tools::string2String(m_StrategyPara->szDefault);
								  comboBox_readonly->Text = (m_StrategyPara->bReadOnly == true) ? "true":"false";
							  }
							  break;
						  case RadioBox:	
						  case DropDownBox:
							  { 
								  map<string,list<StrategyPrarSubItem>> mapFiled;
								  m_StrategyData->GetParaSubItem(mapFiled);
								  map<string,list<StrategyPrarSubItem>>::iterator itmap =  mapFiled.find(Tools::String2string(textBox_title->Text));
								  if(itmap != mapFiled.end())
								  {
									  list<StrategyPrarSubItem>::iterator itList =itmap->second.begin();
									  while (itList != itmap->second.end())
									  {
										  int newRowIndex=dataGridView1->Rows->Add();
									      DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
									      dr->Cells[0]->Value = Tools::string2String(itList->szName);
									      dr->Cells[1]->Value = Tools::string2String(itList->szValue);
										  if (itList->bDefault)
										  {
											  dr->Cells[2]->Value = "默认";
										  }

										  itList++;
									  }
									  

								  }
								 
									 
							  }
							  break;
						  case DirectionBox:
							  {
								  if (m_StrategyPara->szDefault == "1")
								  {
									  comboBox_borsDirection->Text == L"买（1）";
								  } 
								  else
								  {
									  comboBox_borsDirection->Text == L"卖（-1）";
								  }
							
							  }
							  break;
						  case OffsetFlagBox:
							  {
								  if (m_StrategyPara->szDefault == "1")
								  {
									  comboBox_borsDirection->Text == L"开（1）";
								  } 
								  else
								  {
									  comboBox_borsDirection->Text == L"平（-1）";
								  }
							  }
							  break;
						  case BOOLBox:
							  {
								   comboBox_borsDirection->Text  = Tools::string2String(m_StrategyPara->szDefault);
							  }
							  break;
						  case CYCLEBox:
							  {								  

							  }
							  break;
						  case TimeBox:
							  {
								  dateTimePicker1->Text  = Tools::string2String(m_StrategyPara->szDefault);
							  }
							  break;
						  default:
							  break;
						  }

					  }
					  else
					  {
						  comboBox_display->Enabled =false;
                      }


		 }


		 private: bool InputCheck()
				  {
					  if( String::IsNullOrEmpty(textBox_title->Text))  
					  {
						  MessageBox::Show("请输入参数标题!");    
						  return false;
					  }
					  if( String::IsNullOrEmpty(textBox1->Text))  
					  {
						  MessageBox::Show("请输入参数名称!");    
						  return false;
					  } 
					 /* if( String::IsNullOrEmpty(comboBox_type->Text))  
					  {
						  MessageBox::Show("请输入参数名称!");    
						  return false;
					  }
					  if( String::IsNullOrEmpty(comboBox_direction->Text))  
					  {
						  MessageBox::Show("请输入参数名称!");    
						  return false;
					  } */
					  return true;

				  }
private: System::Void button_ok_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if( InputCheck()== false)
				 return;
			 if (m_bOprationType==true && (!m_StrategyData->CheckStrategyParaName(Tools::String2string(textBox_title->Text).c_str())))
			 {
				 MessageBox::Show("参数名称重复!");
				 return;
			 }
			 m_StrategyPara->bReadOnly = false;
			 if ( GetDefaultValue() == false )
				 return;
			 strncpy(m_StrategyPara->szTitle , Tools::String2string(textBox_title->Text).c_str(),sizeof(m_StrategyPara->szTitle)-1);
			 strncpy(m_StrategyPara->szName , Tools::String2string(textBox1->Text).c_str(),sizeof(m_StrategyPara->szName)-1);
			 m_StrategyPara->eType = StrategyType2Enum(Tools::String2string(comboBox_type->Text).c_str());
			 m_StrategyPara->eDirection = StrategyDirection2Enum(Tools::String2string(comboBox_direction->Text).c_str());
			 m_StrategyPara->eDisplayType = DisplayType2Enum(Tools::String2string(comboBox_display->Text).c_str());
			

			 

			 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
			 this->Close();
		 }
private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Close();
		 }

private: System::Void comboBox_type_SelectedValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(bFlag == false)
				 return;
			  SetDynamicZoneInf_type();
		 }
private: System::Void comboBox_display_SelectedValueChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(bFlag == false)
				 return;
			  SetDynamicZoneInf_display();
		 }
private: System::Void comboBox_direction_SelectedValueChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(bFlag == false)
				 return;
			  SetDynamicZoneInf_type();
		 }
};
}

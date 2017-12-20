#pragma once
#include "Tools.h"
#include "stdio.h"
#include "DataManageEx.h"
#include "TestManager.h"
#include "..\CommonClasses\ConvEnum.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace StrategyTester {

	/// <summary>
	/// Summary for FormStrategyTester
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormStrategyTester : public System::Windows::Forms::Form
	{
	public:
		FormStrategyTester(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pDataInfo = CDataManageEx::NewInstance();
			m_pStrategy = new CStrategyDataEx();
			m_pTestParams = new StrategyTestParams;
			memset(m_pTestParams, 0, sizeof(StrategyTestParams));
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormStrategyTester()
		{
			if (components)
			{
				delete components;
			}
			CDataManageEx::DestroyInstance();
			delete m_pStrategy;
			delete m_pTestParams;
		}

	private: CStrategyDataEx*	m_pStrategy;
	private: StrategyTestParams*	m_pTestParams;
	private: CDataManageEx*     m_pDataInfo;

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::CheckBox^  checkBox_timer;

	private: System::Windows::Forms::TextBox^  textBox_describe;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  textBox_version;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox_name;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  comboBox_timer;
	private: System::Windows::Forms::TextBox^  textBox_timer;


	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Panel^  panel_para;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  lable5;
	private: System::Windows::Forms::Panel^  panel_instrment;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::GroupBox^  groupBox4;





	private: System::Windows::Forms::Button^  button_Start;
	private: System::Windows::Forms::Button^  button_Close;
	private: System::Windows::Forms::Panel^  panel_MCRate;

	private: System::Windows::Forms::CheckBox^  checkBox_LocalOutput;

	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::CheckBox^  checkBox_SaveLog;
	private: System::Windows::Forms::CheckBox^  checkBox_SaveHistory;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::ComboBox^  comboBox_QuotSource;

	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::DateTimePicker^  dateTimePK_End;

	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::DateTimePicker^  dateTimePK_Start;

	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::TextBox^  textBox_InitFee;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::ComboBox^  comboBox_traderule;




	protected: 

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
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox_SaveHistory = (gcnew System::Windows::Forms::CheckBox());
			this->comboBox_timer = (gcnew System::Windows::Forms::ComboBox());
			this->checkBox_SaveLog = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_LocalOutput = (gcnew System::Windows::Forms::CheckBox());
			this->textBox_timer = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->checkBox_timer = (gcnew System::Windows::Forms::CheckBox());
			this->textBox_describe = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox_version = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox_name = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->panel_para = (gcnew System::Windows::Forms::Panel());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->dateTimePK_End = (gcnew System::Windows::Forms::DateTimePicker());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->dateTimePK_Start = (gcnew System::Windows::Forms::DateTimePicker());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->comboBox_QuotSource = (gcnew System::Windows::Forms::ComboBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->lable5 = (gcnew System::Windows::Forms::Label());
			this->panel_instrment = (gcnew System::Windows::Forms::Panel());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->comboBox_traderule = (gcnew System::Windows::Forms::ComboBox());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->textBox_InitFee = (gcnew System::Windows::Forms::TextBox());
			this->panel_MCRate = (gcnew System::Windows::Forms::Panel());
			this->button_Start = (gcnew System::Windows::Forms::Button());
			this->button_Close = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->checkBox_SaveHistory);
			this->groupBox1->Controls->Add(this->comboBox_timer);
			this->groupBox1->Controls->Add(this->checkBox_SaveLog);
			this->groupBox1->Controls->Add(this->checkBox_LocalOutput);
			this->groupBox1->Controls->Add(this->textBox_timer);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->checkBox_timer);
			this->groupBox1->Controls->Add(this->textBox_describe);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->textBox_version);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->textBox_name);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(10, 8);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(658, 127);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"策略信息";
			// 
			// checkBox_SaveHistory
			// 
			this->checkBox_SaveHistory->AutoSize = true;
			this->checkBox_SaveHistory->Checked = true;
			this->checkBox_SaveHistory->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox_SaveHistory->Location = System::Drawing::Point(515, 101);
			this->checkBox_SaveHistory->Name = L"checkBox_SaveHistory";
			this->checkBox_SaveHistory->Size = System::Drawing::Size(72, 16);
			this->checkBox_SaveHistory->TabIndex = 11;
			this->checkBox_SaveHistory->Text = L"记录历史";
			this->checkBox_SaveHistory->UseVisualStyleBackColor = true;
			this->checkBox_SaveHistory->Visible = false;
			// 
			// comboBox_timer
			// 
			this->comboBox_timer->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox_timer->Enabled = false;
			this->comboBox_timer->FormattingEnabled = true;
			this->comboBox_timer->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"秒", L"分", L"时"});
			this->comboBox_timer->Location = System::Drawing::Point(211, 99);
			this->comboBox_timer->Name = L"comboBox_timer";
			this->comboBox_timer->Size = System::Drawing::Size(62, 20);
			this->comboBox_timer->TabIndex = 8;
			// 
			// checkBox_SaveLog
			// 
			this->checkBox_SaveLog->AutoSize = true;
			this->checkBox_SaveLog->Checked = true;
			this->checkBox_SaveLog->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox_SaveLog->Location = System::Drawing::Point(437, 101);
			this->checkBox_SaveLog->Name = L"checkBox_SaveLog";
			this->checkBox_SaveLog->Size = System::Drawing::Size(72, 16);
			this->checkBox_SaveLog->TabIndex = 10;
			this->checkBox_SaveLog->Text = L"记录日志";
			this->checkBox_SaveLog->UseVisualStyleBackColor = true;
			this->checkBox_SaveLog->Visible = false;
			// 
			// checkBox_LocalOutput
			// 
			this->checkBox_LocalOutput->AutoSize = true;
			this->checkBox_LocalOutput->Checked = true;
			this->checkBox_LocalOutput->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox_LocalOutput->Location = System::Drawing::Point(359, 101);
			this->checkBox_LocalOutput->Name = L"checkBox_LocalOutput";
			this->checkBox_LocalOutput->Size = System::Drawing::Size(72, 16);
			this->checkBox_LocalOutput->TabIndex = 9;
			this->checkBox_LocalOutput->Text = L"本地导出";
			this->checkBox_LocalOutput->UseVisualStyleBackColor = true;
			this->checkBox_LocalOutput->Visible = false;
			// 
			// textBox_timer
			// 
			this->textBox_timer->Enabled = false;
			this->textBox_timer->Location = System::Drawing::Point(142, 99);
			this->textBox_timer->Name = L"textBox_timer";
			this->textBox_timer->Size = System::Drawing::Size(53, 21);
			this->textBox_timer->TabIndex = 7;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(288, 103);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(65, 12);
			this->label6->TabIndex = 5;
			this->label6->Text = L"其他选项：";
			this->label6->Visible = false;
			// 
			// checkBox_timer
			// 
			this->checkBox_timer->AutoSize = true;
			this->checkBox_timer->Location = System::Drawing::Point(15, 101);
			this->checkBox_timer->Name = L"checkBox_timer";
			this->checkBox_timer->Size = System::Drawing::Size(120, 16);
			this->checkBox_timer->TabIndex = 6;
			this->checkBox_timer->Text = L"启用定时器，间隔";
			this->checkBox_timer->UseVisualStyleBackColor = true;
			this->checkBox_timer->CheckedChanged += gcnew System::EventHandler(this, &FormStrategyTester::checkBox_timer_CheckedChanged);
			// 
			// textBox_describe
			// 
			this->textBox_describe->Enabled = false;
			this->textBox_describe->Location = System::Drawing::Point(72, 43);
			this->textBox_describe->Multiline = true;
			this->textBox_describe->Name = L"textBox_describe";
			this->textBox_describe->Size = System::Drawing::Size(446, 52);
			this->textBox_describe->TabIndex = 5;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(13, 48);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(53, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"策略描述";
			// 
			// textBox_version
			// 
			this->textBox_version->Enabled = false;
			this->textBox_version->Location = System::Drawing::Point(418, 16);
			this->textBox_version->Name = L"textBox_version";
			this->textBox_version->Size = System::Drawing::Size(100, 21);
			this->textBox_version->TabIndex = 3;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(359, 19);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"策略版本";
			// 
			// textBox_name
			// 
			this->textBox_name->Enabled = false;
			this->textBox_name->Location = System::Drawing::Point(72, 17);
			this->textBox_name->Name = L"textBox_name";
			this->textBox_name->Size = System::Drawing::Size(146, 21);
			this->textBox_name->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"策略名称";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->panel_para);
			this->groupBox2->Location = System::Drawing::Point(10, 297);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(658, 148);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"自定义参数";
			// 
			// panel_para
			// 
			this->panel_para->AutoScroll = true;
			this->panel_para->Location = System::Drawing::Point(7, 21);
			this->panel_para->Name = L"panel_para";
			this->panel_para->Size = System::Drawing::Size(642, 118);
			this->panel_para->TabIndex = 0;
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->label9);
			this->groupBox3->Controls->Add(this->dateTimePK_End);
			this->groupBox3->Controls->Add(this->label8);
			this->groupBox3->Controls->Add(this->dateTimePK_Start);
			this->groupBox3->Controls->Add(this->label7);
			this->groupBox3->Controls->Add(this->comboBox_QuotSource);
			this->groupBox3->Controls->Add(this->label5);
			this->groupBox3->Controls->Add(this->label4);
			this->groupBox3->Controls->Add(this->lable5);
			this->groupBox3->Controls->Add(this->panel_instrment);
			this->groupBox3->Location = System::Drawing::Point(10, 141);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(658, 150);
			this->groupBox3->TabIndex = 2;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"合约信息";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(467, 121);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(65, 12);
			this->label9->TabIndex = 14;
			this->label9->Text = L"结束时间：";
			// 
			// dateTimePK_End
			// 
			this->dateTimePK_End->Location = System::Drawing::Point(536, 117);
			this->dateTimePK_End->Name = L"dateTimePK_End";
			this->dateTimePK_End->Size = System::Drawing::Size(113, 21);
			this->dateTimePK_End->TabIndex = 13;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(277, 122);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(65, 12);
			this->label8->TabIndex = 12;
			this->label8->Text = L"开始时间：";
			// 
			// dateTimePK_Start
			// 
			this->dateTimePK_Start->Location = System::Drawing::Point(346, 118);
			this->dateTimePK_Start->Name = L"dateTimePK_Start";
			this->dateTimePK_Start->Size = System::Drawing::Size(113, 21);
			this->dateTimePK_Start->TabIndex = 11;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(13, 122);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(53, 12);
			this->label7->TabIndex = 10;
			this->label7->Text = L"行情源：";
			// 
			// comboBox_QuotSource
			// 
			this->comboBox_QuotSource->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox_QuotSource->FormattingEnabled = true;
			this->comboBox_QuotSource->Location = System::Drawing::Point(72, 118);
			this->comboBox_QuotSource->Name = L"comboBox_QuotSource";
			this->comboBox_QuotSource->Size = System::Drawing::Size(199, 20);
			this->comboBox_QuotSource->TabIndex = 9;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(409, 13);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(29, 12);
			this->label5->TabIndex = 3;
			this->label5->Text = L"合约";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(258, 13);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(29, 12);
			this->label4->TabIndex = 2;
			this->label4->Text = L"品种";
			// 
			// lable5
			// 
			this->lable5->AutoSize = true;
			this->lable5->Location = System::Drawing::Point(105, 14);
			this->lable5->Name = L"lable5";
			this->lable5->Size = System::Drawing::Size(41, 12);
			this->lable5->TabIndex = 1;
			this->lable5->Text = L"交易所";
			// 
			// panel_instrment
			// 
			this->panel_instrment->AutoScroll = true;
			this->panel_instrment->Location = System::Drawing::Point(10, 29);
			this->panel_instrment->Name = L"panel_instrment";
			this->panel_instrment->Size = System::Drawing::Size(639, 83);
			this->panel_instrment->TabIndex = 0;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->label17);
			this->groupBox4->Controls->Add(this->comboBox_traderule);
			this->groupBox4->Controls->Add(this->label14);
			this->groupBox4->Controls->Add(this->label15);
			this->groupBox4->Controls->Add(this->label16);
			this->groupBox4->Controls->Add(this->label11);
			this->groupBox4->Controls->Add(this->label12);
			this->groupBox4->Controls->Add(this->label13);
			this->groupBox4->Controls->Add(this->label10);
			this->groupBox4->Controls->Add(this->textBox_InitFee);
			this->groupBox4->Controls->Add(this->panel_MCRate);
			this->groupBox4->Location = System::Drawing::Point(10, 451);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(658, 158);
			this->groupBox4->TabIndex = 3;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"手续费保证金配置";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(230, 23);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(65, 12);
			this->label17->TabIndex = 19;
			this->label17->Text = L"成交规则：";
			// 
			// comboBox_traderule
			// 
			this->comboBox_traderule->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox_traderule->FormattingEnabled = true;
			this->comboBox_traderule->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"最新价见价成交", L"买卖价见价成交", L"买卖价按买卖量成交"});
			this->comboBox_traderule->Location = System::Drawing::Point(301, 20);
			this->comboBox_traderule->Name = L"comboBox_traderule";
			this->comboBox_traderule->Size = System::Drawing::Size(137, 20);
			this->comboBox_traderule->TabIndex = 18;
			this->comboBox_traderule->SelectedIndex = 0;
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(518, 50);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(89, 12);
			this->label14->TabIndex = 17;
			this->label14->Text = L"平今手续费(率)";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(411, 50);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(89, 12);
			this->label15->TabIndex = 16;
			this->label15->Text = L"开仓手续费(率)";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(321, 50);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(53, 12);
			this->label16->TabIndex = 15;
			this->label16->Text = L"保证金率";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(216, 50);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(53, 12);
			this->label11->TabIndex = 14;
			this->label11->Text = L"收费方式";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(115, 50);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(53, 12);
			this->label12->TabIndex = 13;
			this->label12->Text = L"合约乘数";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(13, 50);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(29, 12);
			this->label13->TabIndex = 12;
			this->label13->Text = L"合约";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(13, 24);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(65, 12);
			this->label10->TabIndex = 11;
			this->label10->Text = L"初始资金：";
			// 
			// textBox_InitFee
			// 
			this->textBox_InitFee->Location = System::Drawing::Point(82, 20);
			this->textBox_InitFee->MaxLength = 20;
			this->textBox_InitFee->Name = L"textBox_InitFee";
			this->textBox_InitFee->Size = System::Drawing::Size(124, 21);
			this->textBox_InitFee->TabIndex = 8;
			this->textBox_InitFee->Text = L"10000000.00";
			// 
			// panel_MCRate
			// 
			this->panel_MCRate->AutoScroll = true;
			this->panel_MCRate->Location = System::Drawing::Point(10, 65);
			this->panel_MCRate->Name = L"panel_MCRate";
			this->panel_MCRate->Size = System::Drawing::Size(639, 83);
			this->panel_MCRate->TabIndex = 1;
			// 
			// button_Start
			// 
			this->button_Start->Location = System::Drawing::Point(517, 618);
			this->button_Start->Name = L"button_Start";
			this->button_Start->Size = System::Drawing::Size(72, 21);
			this->button_Start->TabIndex = 7;
			this->button_Start->Text = L"开始测试";
			this->button_Start->UseVisualStyleBackColor = true;
			this->button_Start->Click += gcnew System::EventHandler(this, &FormStrategyTester::button_Start_Click);
			// 
			// button_Close
			// 
			this->button_Close->Location = System::Drawing::Point(595, 618);
			this->button_Close->Name = L"button_Close";
			this->button_Close->Size = System::Drawing::Size(72, 21);
			this->button_Close->TabIndex = 8;
			this->button_Close->Text = L"关闭";
			this->button_Close->UseVisualStyleBackColor = true;
			this->button_Close->Click += gcnew System::EventHandler(this, &FormStrategyTester::button_Close_Click);
			// 
			// FormStrategyTester
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(678, 649);
			this->Controls->Add(this->button_Close);
			this->Controls->Add(this->button_Start);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"FormStrategyTester";
			this->Text = L"策略测试配置";
			this->Load += gcnew System::EventHandler(this, &FormStrategyTester::FormStrategyTester_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void GenerateInstrumentList(int  Count)
			 {
				 for ( int i=0; i<Count; i++)
				 {
					  char strtemp[32];
					 //合约
					 Label^ lbvalue = gcnew Label();
					 lbvalue->AutoSize = true;
					 lbvalue->Location = System::Drawing::Point(10, 5+2+30*i);
					 memset(strtemp,0,sizeof(strtemp));
					 sprintf(strtemp, "%05u_label_ins", i);
					 lbvalue->Name = Tools::string2String(strtemp);
					 lbvalue->Size = System::Drawing::Size(41, 12);
					 lbvalue->TabIndex = 4*i;
					 memset(strtemp,0,sizeof(strtemp));
					 sprintf( strtemp, "%s%d","合约", i+1);
					 lbvalue->Text =  Tools::string2String(strtemp );
					 this->panel_instrment->Controls->Add(lbvalue);

					 //交易所comobox
					 ComboBox^ comboBox_exchange = gcnew ComboBox();
					 comboBox_exchange->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
					 comboBox_exchange->FormattingEnabled = true;
					 comboBox_exchange->Location = System::Drawing::Point(60, 5+30*i);
					 memset(strtemp,0,sizeof(strtemp));
					 sprintf(strtemp, "%05u_comboBox_exchange", i);
					 comboBox_exchange->Name =  Tools::string2String(strtemp);
					 comboBox_exchange->Size = System::Drawing::Size(121, 20);
					 comboBox_exchange->TabIndex = 1+4*i;
					 comboBox_exchange->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"中金所", L"大商所", L"郑商所",L"上期所"});
					 comboBox_exchange->SelectedIndexChanged += gcnew System::EventHandler(this, &FormStrategyTester::comboBox_exchange_SelectedIndexChanged);
					 this->panel_instrment->Controls->Add(comboBox_exchange);

					 //品种comobox
					 ComboBox^  comboBox_product = gcnew ComboBox();
					 comboBox_product->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
					 comboBox_product->FormattingEnabled = true;
					 comboBox_product->Location = System::Drawing::Point(210, 5+30*i);
					 memset(strtemp,0,sizeof(strtemp));
					 sprintf(strtemp, "%05u_comboBox_product", i);
					 comboBox_product->Name =  Tools::string2String(strtemp);
					 comboBox_product->Size = System::Drawing::Size(121, 20);
					 comboBox_product->TabIndex = 2+4*i;
					 comboBox_product->SelectedIndexChanged += gcnew System::EventHandler(this, &FormStrategyTester::comboBox_product_SelectedIndexChanged);
					 this->panel_instrment->Controls->Add(comboBox_product);

					 //合约comobox
					 ComboBox^  comboBox_Ins = gcnew ComboBox();
					 comboBox_Ins->FormattingEnabled = true;
					 comboBox_Ins->Location = System::Drawing::Point(360, 5+30*i);
					 memset(strtemp,0,sizeof(strtemp));
					 sprintf(strtemp, "%05u_comboBox_ins", i);
					 comboBox_Ins->Name =  Tools::string2String(strtemp);
					 comboBox_Ins->Size = System::Drawing::Size(121, 20);
					 comboBox_Ins->TabIndex = 3+4*i;
					 comboBox_Ins->TextChanged += gcnew System::EventHandler(this, &FormStrategyTester::comboBox_Instrument_TextChanged);
					 this->panel_instrment->Controls->Add(comboBox_Ins);
				 }

			 }

	private: System::Void GenerateMCRateList(int Count)
			 {
				 for ( int i=0; i<Count; i++)
				 {
					 char strtemp[32];
					 //合约
					 TextBox^ txtInstrument = gcnew System::Windows::Forms::TextBox();
					 txtInstrument->Enabled = false;
					 sprintf(strtemp, "%05u_textbox_ins", i);
					 txtInstrument->Name = gcnew String(strtemp);
					 txtInstrument->Location = System::Drawing::Point(5, 5+30*i);
					 txtInstrument->Size = System::Drawing::Size(95, 21);
					 txtInstrument->TabIndex = 6*i;
					 this->panel_MCRate->Controls->Add(txtInstrument);

					 //合约乘数
					 TextBox^ txtMultiVolume = gcnew System::Windows::Forms::TextBox();
					 txtMultiVolume->Enabled = true;
					 sprintf(strtemp, "%05u_textbox_multi", i);
					 txtMultiVolume->Name = gcnew String(strtemp);
					 txtMultiVolume->Location = System::Drawing::Point(105, 5+30*i);
					 txtMultiVolume->Size = System::Drawing::Size(95, 21);
					 txtMultiVolume->TabIndex = 1+6*i;
					 this->panel_MCRate->Controls->Add(txtMultiVolume);

					 //收费方式combobox
					 ComboBox^ comboBox_CSRateMode = gcnew ComboBox();
					 comboBox_CSRateMode->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
					 comboBox_CSRateMode->FormattingEnabled = true;
					 sprintf(strtemp, "%05u_combobox_csmode", i);
					 comboBox_CSRateMode->Name =  Tools::string2String(strtemp);
					 comboBox_CSRateMode->Location = System::Drawing::Point(205, 5+30*i);
					 comboBox_CSRateMode->Size = System::Drawing::Size(95, 20);
					 comboBox_CSRateMode->TabIndex = 2+6*i;
					 comboBox_CSRateMode->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"成交额", L"每手"});
					 comboBox_CSRateMode->SelectedIndex = 0;
					 comboBox_CSRateMode->SelectedIndexChanged += gcnew System::EventHandler(this, &FormStrategyTester::comboBox_CSRateMode_SelectedIndexChanged);
					 this->panel_MCRate->Controls->Add(comboBox_CSRateMode);

					 //保证金率
					 TextBox^ txtMarginRate = gcnew System::Windows::Forms::TextBox();
					 txtMarginRate->Enabled = true;
					 sprintf(strtemp, "%05u_textbox_margin", i);
					 txtMarginRate->Name = gcnew String(strtemp);
					 txtMarginRate->Location = System::Drawing::Point(305, 5+30*i);
					 txtMarginRate->Size = System::Drawing::Size(95, 21);
					 txtMarginRate->TabIndex = 3+6*i;
					 this->panel_MCRate->Controls->Add(txtMarginRate);

					 //开仓手续费率
					 TextBox^ txtOpenCSRate = gcnew System::Windows::Forms::TextBox();
					 txtOpenCSRate->Enabled = true;
					 sprintf(strtemp, "%05u_textbox_opencs", i);
					 txtOpenCSRate->Name = gcnew String(strtemp);
					 txtOpenCSRate->Location = System::Drawing::Point(405, 5+30*i);
					 txtOpenCSRate->Size = System::Drawing::Size(95, 21);
					 txtOpenCSRate->TabIndex = 4+6*i;
					 this->panel_MCRate->Controls->Add(txtOpenCSRate);

					 //平今手续费率
					 TextBox^ txtCTCSRate = gcnew System::Windows::Forms::TextBox();
					 txtCTCSRate->Enabled = true;
					 sprintf(strtemp, "%05u_textbox_ctcs", i);
					 txtCTCSRate->Name = gcnew String(strtemp);
					 txtCTCSRate->Location = System::Drawing::Point(505, 5+30*i);
					 txtCTCSRate->Size = System::Drawing::Size(95, 21);
					 txtCTCSRate->TabIndex = 5+6*i;
					 this->panel_MCRate->Controls->Add(txtCTCSRate);

				 }

			 }

	private: System::Void GenerateStrategyParaList(CStrategyDataEx&  strategyData)
			 {
				 char strtemp[32];
				 std::map<std::string,StrategyPara> mapPara;
				 strategyData.GetStrategyPara(mapPara);
				 std::map<std::string,StrategyPara>::iterator it = mapPara.begin();
				 for ( int i= 0; it!= mapPara.end(); it++,i++)
				 {
					 
					 //参数名
					 Label^ lbName = gcnew Label();
					 lbName->AutoSize = true;
					 lbName->Location = System::Drawing::Point(20, 5+2+30*i);
					 memset(strtemp,0,sizeof(strtemp));
					 sprintf(strtemp, "%05u_label_paraname", i);
					 lbName->Name = Tools::string2String(strtemp);
					 lbName->Size = System::Drawing::Size(41, 12);
					 lbName->TabIndex = 4*i;
					 memset(strtemp,0,sizeof(strtemp));
					 sprintf( strtemp, "%s(%s)",it->second.szTitle ,it->second.szName);
					 lbName->Text =  Tools::string2String(strtemp );
					 this->panel_para->Controls->Add(lbName);

					 //参数类型
					 Label^ lbType = gcnew Label();
					 lbType->AutoSize = true;
					 lbType->Location = System::Drawing::Point(200, 5+2+30*i);
					 memset(strtemp,0,sizeof(strtemp));
					 sprintf(strtemp, "%05u_label_paratype", i);
					 lbType->Name = Tools::string2String(strtemp);
					 lbType->Size = System::Drawing::Size(41, 12);
					 lbType->TabIndex = 4*i+1;
					 lbType->Text =  Tools::string2String(StrategyType2String(it->second.eType));
					 this->panel_para->Controls->Add(lbType);

					 //默认值
					 TextBox^  textBoxDefault= gcnew TextBox();
					 textBoxDefault->Location = System::Drawing::Point(300, 5+30*i);
					 memset(strtemp,0,sizeof(strtemp));
					 sprintf(strtemp, "%05u_textboxdefault", i);
					 textBoxDefault->Name = Tools::string2String(strtemp);
					 textBoxDefault->Size = System::Drawing::Size(100, 21);
					 textBoxDefault->TabIndex =4*i+ 2;
					 textBoxDefault->Text =  Tools::string2String(it->second.szDefault);
					 this->panel_para->Controls->Add(textBoxDefault);
				 }

			 }

	private: System::Void FormStrategyTester_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent();

				 this->comboBox_QuotSource->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"文件", gcnew String(_GetQuotServer().c_str())});
				 this->comboBox_QuotSource->SelectedIndex = 0;

				 m_pStrategy->ImportFromXmlFile(_GetStategyPath()+"params.xml");

				 textBox_name->Text = Tools::string2String(_GetStrategyID().c_str());
				 textBox_version->Text = Tools::string2String(m_pStrategy->GetStrategyVersion().c_str());		 
				 textBox_describe->Text = Tools::string2String(m_pStrategy->GetDescribe().c_str());

				 int insCount = m_pStrategy->GetInstrumentCount();
				 char strtemp[32];
				 GenerateInstrumentList(insCount);
				 GenerateStrategyParaList(*m_pStrategy);
				 GenerateMCRateList(insCount);

				 //定时器默认不勾选
				 checkBox_timer->Checked =false;
				 textBox_timer->Enabled =false ;
				 comboBox_timer->Enabled =false;

				 //修改

				 //参数

				 std::map<std::string,StrategyPara>& mapPara = m_pStrategy->GetStrategyParams();
				 for ( int i =0; i< mapPara.size(); i++)
				 {
					 memset(strtemp,0,sizeof(strtemp));
					 sprintf(strtemp, "%05u_label_paraname", i);
					 Label^  Label_para  = (Label^)panel_para->Controls->Find( Tools::string2String(strtemp),true )[0];						 
					 int nfind = Label_para->Text->IndexOf("(");
					 String^ paraTitle= Label_para->Text->Remove(nfind);
					 std::map<std::string,StrategyPara>::iterator it = mapPara.find(Tools::String2string(paraTitle));
					 if (it!=mapPara.end())
					 {
						 memset(strtemp,0,sizeof(strtemp));
						 sprintf(strtemp, "%05u_textboxdefault", i);
						 TextBox^  textBoxDefault  = (TextBox^)panel_para->Controls->Find( Tools::string2String(strtemp),true )[0];		
						 textBoxDefault->Text = Tools::string2String(it->second.szDefault);
					 }
				 }
				 
			 }

	private: System::Void comboBox_exchange_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 ComboBox^ comboBoxE = (ComboBox^)(sender);
				 String^ comboBoxName = comboBoxE->Name; 
				 String^ product = comboBoxName->Substring(0, 5) + "_combobox_product";
				 String^ instrument = comboBoxName->Substring(0, 5) + "_combobox_ins";
				 ComboBox^ comboBoxP = ((ComboBox^)panel_instrment->Controls->Find( product,true )[0]);
				 ComboBox^ comboBoxI = ((ComboBox^)panel_instrment->Controls->Find( instrument,true )[0]);

				  String^ selectedExchange = (String^)(comboBoxE->SelectedItem);
				  string exchang =ExchangeName2ID(Tools::String2string(selectedExchange));
				  std::set<std::string> setRetProductID;
				  if (m_pDataInfo->GetProductID(exchang,  setRetProductID))
				  {
					  comboBoxP->Items->Clear();
					  comboBoxP->Text ="";
					  comboBoxI->Items->Clear();
					  comboBoxI->Text ="";
					  std::set<std::string>::iterator it = setRetProductID.begin();
					  while (it!=setRetProductID.end())
					  {
						  comboBoxP->Items->Add(Tools::string2String((*it).c_str()));
						  it++;
					  }
				  }
				  
			 }

	private: System::Void comboBox_product_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 ComboBox^ comboBoxP = (ComboBox^)(sender);
				 String^ comboBoxName = comboBoxP->Name; 
				 String^ instrument = comboBoxName->Substring(0, 5) + "_combobox_ins";
				 ComboBox^ comboBoxI = ((ComboBox^)panel_instrment->Controls->Find( instrument,true )[0]);

				 String^ selectedpProduct = (String^)(comboBoxP->SelectedItem);
				 std::map<std::string, PlatformStru_InstrumentInfo> mapIns;
				 if (m_pDataInfo->GetInstrument(Tools::String2string(selectedpProduct),mapIns))
				 {
					 comboBoxI->Items->Clear();
					 comboBoxI->Text ="";
					 std::map<std::string, PlatformStru_InstrumentInfo>::iterator it =mapIns.begin();
					 while ( it != mapIns.end() )
					 {
						 comboBoxI->Items->Add(Tools::string2String(it->second.InstrumentID));
						 it++;
					 }
				 }
			 }

	protected: System::Void LoadMCRate(String^ strIndex, String^ strInstrument)
			   {
					String^ strCtrlKey = nullptr;
					std::string strInstrumentID = Tools::String2string(strInstrument);
					char strText[1024];

					PlatformStru_InstrumentInfo instInfo;
					strcpy(strText, "0");
					if(m_pDataInfo->GetInstrumentInfo(strInstrumentID, instInfo)) {
						sprintf_s(strText, sizeof(strText)-1, "%d", instInfo.VolumeMultiple);
					}
					strCtrlKey = strIndex + "_textbox_multi";
					TextBox^ textbox = ((TextBox^)panel_MCRate->Controls->Find(strCtrlKey, true)[0]);
					textbox->Text = gcnew String(strText);

					PlatformStru_InstrumentMarginRate mgRate;
					strcpy(strText, "0.0");
					if(m_pDataInfo->GetMarginRate(strInstrumentID, mgRate)) {
						sprintf_s(strText, sizeof(strText)-1, "%g", mgRate.LongMarginRatioByMoney);
					}
					strCtrlKey = strIndex + "_textbox_margin";
					textbox = ((TextBox^)panel_MCRate->Controls->Find(strCtrlKey, true)[0]);
					textbox->Text = gcnew String(strText);

					PlatformStru_InstrumentCommissionRate csRate;
					strcpy(strText, "0.0");
					strCtrlKey = strIndex + "_combobox_csmode";
					ComboBox^ combobox = ((ComboBox^)panel_MCRate->Controls->Find(strCtrlKey, true)[0]);
					if(m_pDataInfo->GetCommissionRate(strInstrumentID, csRate)) {
						if(combobox->SelectedIndex==0) 
							sprintf_s(strText, sizeof(strText)-1, "%g", csRate.OpenRatioByMoney);
						else
							sprintf_s(strText, sizeof(strText)-1, "%g", csRate.OpenRatioByVolume);
					}
					strCtrlKey = strIndex + "_textbox_opencs";
					textbox = ((TextBox^)panel_MCRate->Controls->Find(strCtrlKey, true)[0]);
					textbox->Text = gcnew String(strText);

					strcpy(strText, "0.0");
					if(m_pDataInfo->GetCommissionRate(strInstrumentID, csRate)) {
						if(combobox->SelectedIndex==0) 
							sprintf_s(strText, sizeof(strText)-1, "%g", csRate.CloseTodayRatioByMoney);
						else
							sprintf_s(strText, sizeof(strText)-1, "%g", csRate.CloseTodayRatioByVolume);
					}
					strCtrlKey = strIndex + "_textbox_ctcs";
					textbox = ((TextBox^)panel_MCRate->Controls->Find(strCtrlKey, true)[0]);
					textbox->Text = gcnew String(strText);
			   }

	private: System::Void comboBox_Instrument_TextChanged(System::Object^  sender, System::EventArgs^  e) {
				 ComboBox^ comboboxP = (ComboBox^)(sender);
				 String^ strIndex = comboboxP->Name->Substring(0, 5);
				 String^ strCtrlKey = strIndex + "_textbox_ins";
				 TextBox^ textBoxI = ((TextBox^)panel_MCRate->Controls->Find(strCtrlKey, true)[0]);
				 textBoxI->Text = comboboxP->Text;
				 LoadMCRate(strIndex, comboboxP->Text);
			 }

	private: System::Void comboBox_CSRateMode_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 ComboBox^ comboboxP = (ComboBox^)(sender);
				 String^ strIndex = comboboxP->Name->Substring(0, 5);
				 String^ strCtrlKey = strIndex + "_textbox_ins";
				 TextBox^ textBoxI = ((TextBox^)panel_MCRate->Controls->Find(strCtrlKey, true)[0]);
				 LoadMCRate(strIndex, textBoxI->Text);
			 }

	private: System::Void button_Start_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 //if(comboBox_QuotSource->SelectedIndex==0) {
					// if(m_pDataInfo->GetQuotList().size()==0) {
					//	 MessageBox::Show("未导入的行情数据，或数量为0，不能继续测试。");  
					//	 return;
					// }
				 //}
				 //else {
					// if(m_pDataInfo->GetLoginQuotStatus()!=CDataManageEx::conLogon) {
					//	 MessageBox::Show("未登录行情服务器或失败。");  
					//	 return;
					// }
				 //}

				 //重置合约数据
				 char strtemp[32];
				 int insCount = m_pStrategy->GetInstrumentCount();
				 string strVa="";
				 string strInstruments;
				 std::vector<std::string> vecInstrument;
				 std::map<std::string, PlatformStru_InstrumentInfo> mapInstrument;
				 std::map<std::string, PlatformStru_InstrumentMarginRate> mapMarginRate;
				 std::map<std::string, PlatformStru_InstrumentCommissionRate> mapCommissionRate;

				 for(int i=0; i< insCount; i++) {
					 sprintf(strtemp, "%05u_textbox_ins", i);
					 String^ value = ((TextBox^)panel_MCRate->Controls->Find( Tools::string2String(strtemp), true)[0])->Text;
					 if(String::IsNullOrEmpty(value)) {
						 MessageBox::Show("请选择合约!");  
						 return;
					 }
					 std::string strInstrumentID = Tools::String2string(value);
					 vecInstrument.push_back(strInstrumentID);
					 strInstruments += strInstrumentID;
					 if(i<insCount-1)
						strInstruments += ",";

					 PlatformStru_InstrumentInfo instInfo;
					 strncpy(instInfo.InstrumentID, strInstrumentID.c_str(), sizeof(instInfo.InstrumentID)-1);
					 m_pDataInfo->GetInstrumentInfo(strInstrumentID, instInfo);
					 sprintf(strtemp, "%05u_textbox_multi", i);
					 value = ((TextBox^)panel_MCRate->Controls->Find(Tools::string2String(strtemp), true)[0])->Text;
					 instInfo.VolumeMultiple = atoi(Tools::String2string(value).c_str());
					 mapInstrument[strInstrumentID] = instInfo;

					 sprintf(strtemp, "%05u_combobox_csmode", i);
					 ComboBox^ combobox = ((ComboBox^)panel_MCRate->Controls->Find(Tools::string2String(strtemp), true)[0]);

					 PlatformStru_InstrumentMarginRate mgRate;
					 sprintf(strtemp, "%05u_textbox_margin", i);
					 value = ((TextBox^)panel_MCRate->Controls->Find(Tools::string2String(strtemp), true)[0])->Text;

					 strncpy(mgRate.InstrumentID, strInstrumentID.c_str(), sizeof(mgRate.InstrumentID)-1);
					 mgRate.HedgeFlag = THOST_FTDC_HF_Speculation;
					 mgRate.LongMarginRatioByMoney = atof(Tools::String2string(value).c_str());
					 mgRate.ShortMarginRatioByMoney = atof(Tools::String2string(value).c_str());
					 mgRate.LongMarginRatioByVolume = 0.0;
					 mgRate.ShortMarginRatioByVolume = 0.0;
					 mapMarginRate[strInstrumentID] = mgRate;

					 PlatformStru_InstrumentCommissionRate csRate;
					 sprintf(strtemp, "%05u_textbox_opencs", i);
					 value = ((TextBox^)panel_MCRate->Controls->Find(Tools::string2String(strtemp), true)[0])->Text;
					 if(combobox->SelectedIndex==0) {
						 csRate.OpenRatioByMoney = atof(Tools::String2string(value).c_str());
						 csRate.OpenRatioByVolume = 0.0;
					 }
					 else {
						 csRate.OpenRatioByMoney = 0.0;
						 csRate.OpenRatioByVolume = atoi(Tools::String2string(value).c_str());
					 }

					 sprintf(strtemp, "%05u_textbox_ctcs", i);
					 value = ((TextBox^)panel_MCRate->Controls->Find(Tools::string2String(strtemp), true)[0])->Text;
					 if(combobox->SelectedIndex==0) {
						 csRate.CloseTodayRatioByMoney = atof(Tools::String2string(value).c_str());
						 csRate.CloseTodayRatioByVolume = 0.0;
						 csRate.CloseRatioByMoney = 0.0;
						 csRate.CloseRatioByVolume = 0.0;
					 }
					 else {
						 csRate.CloseTodayRatioByMoney = 0.0;
						 csRate.CloseTodayRatioByVolume = atof(Tools::String2string(value).c_str());
						 csRate.CloseRatioByMoney = 0.0;
						 csRate.CloseRatioByVolume = 0.0;
					 }

					 strncpy(csRate.InstrumentID, strInstrumentID.c_str(), sizeof(csRate.InstrumentID)-1);
					 mapCommissionRate[strInstrumentID] = csRate;
				 }

				 //自定义参数
				 std::map<std::string, StrategyPara>& mapPara = m_pStrategy->GetStrategyParams();
				 std::map<std::string, StrategyPara>::iterator it = mapPara.begin();
				 for ( int i= 0; it!= mapPara.end(); it++,i++) {
					 memset(strtemp,0,sizeof(strtemp));
					 sprintf(strtemp, "%05u_textboxdefault", i);
					 String^ value = ((TextBox^)panel_para->Controls->Find(Tools::string2String(strtemp), true)[0])->Text;
					 if(String::IsNullOrEmpty(value)) {
						 MessageBox::Show("请输入默认值!");  
						 return;
					 }
					 strncpy(it->second.szDefault, Tools::String2string(value).c_str(), sizeof(it->second.szDefault)-1);
				 }
				 //string para;
				 //m_pStrategy->MakeXml(para);

				 //策略控制参数
				 m_pTestParams->bLocalOutput = checkBox_LocalOutput->Checked;
				 m_pTestParams->bSaveLog = checkBox_SaveLog->Checked;
				 m_pTestParams->bSaveHistory = checkBox_SaveHistory->Checked;
				 m_pTestParams->fInitFee = atof(Tools::String2string(textBox_InitFee->Text).c_str());
				 m_pTestParams->nTradeRule = comboBox_traderule->SelectedIndex+1;

				  //定时器
				  m_pTestParams->bStartTimer = checkBox_timer->Checked;
				  if(m_pTestParams->bStartTimer) {
					  int interval = Convert::ToInt32(textBox_timer->Text);
					  m_pTestParams->nTimerSpan = (int)(interval *pow(60.0,comboBox_timer->SelectedIndex));
				  }
				  else 
					  m_pTestParams->nTimerSpan = 0;

				  CTestEntity* pTestEntity = CTestManager::CreateTestEntity(std::string(""), std::string(""));
				  CDataEntity* pDataEntity = pTestEntity->GetDataEntity();
				  pDataEntity->SetMDQuotList(m_pDataInfo->GetQuotList());
				  pDataEntity->SetInstrument(mapInstrument);
				  pDataEntity->SetMarginRate(mapMarginRate);
				  pDataEntity->SetCommissionRate(mapCommissionRate);
				  pDataEntity->SetStrategy(*m_pStrategy);
				  pDataEntity->SetInitFund(m_pTestParams->fInitFee);

				  std::string strStrategyFile(_GetStategyPath()+_GetStrategyID()+".cs");
				  String^ strPathFile = gcnew String(strStrategyFile.c_str());
				  System::DateTime^ createTime = ::System::IO::File::GetCreationTime(strPathFile);
				  System::DateTime^ modifyTime = ::System::IO::File::GetLastWriteTime(strPathFile);
				  String^ strCreateTime = createTime->ToString("yyyy-MM-dd HH:mm:ss");
				  String^ strModifyTime = modifyTime->ToString("yyyy-MM-dd HH:mm:ss");
				  pDataEntity->SetStrategyCreateTime(Tools::String2string(strCreateTime));
				  pDataEntity->SetStrategyEditTime(Tools::String2string(strModifyTime));

				  pTestEntity->SetInstrumentIDs(strInstruments);
				  pTestEntity->SetQuotFromFile(comboBox_QuotSource->SelectedIndex==0);
				  pTestEntity->SetQuotFile(_GetQuotFile());
				  pTestEntity->SetRunPath(_GetRunPath());
				  int nStartDate = dateTimePK_Start->Value.Year * 10000 
						+ dateTimePK_Start->Value.Month * 100
						+ dateTimePK_Start->Value.Day;
				  int nEndDate = dateTimePK_End->Value.Year * 10000 
						+ dateTimePK_End->Value.Month * 100
						+ dateTimePK_End->Value.Day;
				  pTestEntity->ResetQuotRange(nStartDate, nEndDate);
				  if(pTestEntity->Init(vecInstrument, *m_pStrategy, *m_pTestParams)) {
					  CTestManager::sm_pTestEntity = pTestEntity;
					  this->DialogResult = System::Windows::Forms::DialogResult::OK;
				  }
				  else {
					  MessageBox::Show("测试引擎初始化错误！");
					  this->DialogResult = System::Windows::Forms::DialogResult::Abort;
				  }
				  this->Close();

			 }

	private: System::Void button_Close_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
				 this->Close();
			 }

	private: System::Void checkBox_timer_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 if (checkBox_timer->Checked)
				 {
					 textBox_timer->Enabled =true;
					 comboBox_timer->Enabled =true;
				 }
				 else
				 {
					 textBox_timer->Enabled =false ;
					 comboBox_timer->Enabled =false;
				 }
			 }
};
}

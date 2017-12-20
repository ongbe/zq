#pragma once
#include "TcpLayer.h"
#include "CommonDef.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "CDataInfo.h"
#include "Tools.h"
#include "CInstrumentIDTreeListDlg.h"
#include "AddPDform.h"
#include "PromptForm.h"
#include "CommonMacro.h"
#include "FileOpr.h"
#include "SettleSort.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

	/// <summary>
	/// Summary for PositionConfirmForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class PositionConfirmForm : public System::Windows::Forms::Form
	{
	public:
		PositionConfirmForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pLogin = new sLoginRsp();
			m_pDataInfo = CDataInfo::NewInstance();
			m_pCurPositionDetail = new PlatformStru_PositionDetail;
			InvalideValue=util::GetDoubleInvalidValue();
			m_nRowNumPerPage = 10;
			m_nCurShowPage = 1;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PositionConfirmForm()
		{
			if (components)
			{
				delete components;
			}
			CDataInfo::DestroyInstance();
			if(m_pLogin)
			{
				delete m_pLogin;
				m_pLogin = nullptr;
			}
			if ( NULL != m_pCurPositionDetail )
			{
				delete m_pCurPositionDetail;
				m_pCurPositionDetail = NULL;
			}
		}
	private: CInstrumentIDTreeListDlg^ dlgInstrumentIDTreeList;
	private: sLoginRsp*     m_pLogin;
	private: CDataInfo*     m_pDataInfo;
	private:PlatformStru_PositionDetail* m_pCurPositionDetail;
	private: double         InvalideValue;
	private: int            m_nRowNumPerPage;
	private: int            m_nCurShowPage;



	private: System::Windows::Forms::Label^  label3;
	private: AnywndComboBox::AnywndComboBoxControl^   comboBox_Ins;

	private: System::Windows::Forms::Button^  button_query;
	private: System::Windows::Forms::Button^  button_clear;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;



	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::ComboBox^  comboBox_direction;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  comboBox_user;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::ComboBox^  comboBox_exchange;

	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::DataGridView^  dataGridView3;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::DataGridView^  dataGridView4;
	private: System::Windows::Forms::Button^  button_add;
	private: System::Windows::Forms::Button^  button_modify;
	private: System::Windows::Forms::Button^  button_delete;
	private: System::Windows::Forms::Button^  button_confirm;

	private: System::Windows::Forms::Button^  button_cancel;



	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::ComboBox^  comboBox2;
	private: System::Windows::Forms::Label^  label12;
private: System::Windows::Forms::Button^  button_pre;
private: System::Windows::Forms::Button^  button_next;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column4;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column5;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column6;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column7;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column8;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column9;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column11;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column12;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column13;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column14;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column15;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column16;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column17;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column18;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column19;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column20;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column21;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column22;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column23;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column24;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column25;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column10;




















































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
			this->comboBox_user = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboBox_exchange = (gcnew System::Windows::Forms::ComboBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->comboBox_direction = (gcnew System::Windows::Forms::ComboBox());
			this->button_query = (gcnew System::Windows::Forms::Button());
			this->button_clear = (gcnew System::Windows::Forms::Button());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->dataGridView3 = (gcnew System::Windows::Forms::DataGridView());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column4 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column5 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column6 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column7 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column8 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column9 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column10 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->dataGridView4 = (gcnew System::Windows::Forms::DataGridView());
			this->button_add = (gcnew System::Windows::Forms::Button());
			this->button_modify = (gcnew System::Windows::Forms::Button());
			this->button_delete = (gcnew System::Windows::Forms::Button());
			this->button_confirm = (gcnew System::Windows::Forms::Button());
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->comboBox_Ins = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->button_pre = (gcnew System::Windows::Forms::Button());
			this->button_next = (gcnew System::Windows::Forms::Button());
			this->Column11 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column12 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column13 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column14 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column15 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column16 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column17 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column18 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column19 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column20 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column21 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column22 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column23 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column24 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column25 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView4))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(21, 32);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"交易员";
			// 
			// comboBox_user
			// 
			this->comboBox_user->FormattingEnabled = true;
			this->comboBox_user->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"全部"});
			this->comboBox_user->Location = System::Drawing::Point(69, 23);
			this->comboBox_user->Name = L"comboBox_user";
			this->comboBox_user->Size = System::Drawing::Size(121, 20);
			this->comboBox_user->TabIndex = 1;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(222, 31);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(65, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"交易所名称";
			// 
			// comboBox_exchange
			// 
			this->comboBox_exchange->FormattingEnabled = true;
			this->comboBox_exchange->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"全部", L"上期所", L"郑商所", L"大商所", L"中金所"});
			this->comboBox_exchange->Location = System::Drawing::Point(308, 23);
			this->comboBox_exchange->Name = L"comboBox_exchange";
			this->comboBox_exchange->Size = System::Drawing::Size(89, 20);
			this->comboBox_exchange->TabIndex = 3;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(435, 31);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(59, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"品种/合约";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(643, 30);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(29, 12);
			this->label4->TabIndex = 6;
			this->label4->Text = L"方向";
			// 
			// comboBox_direction
			// 
			this->comboBox_direction->FormattingEnabled = true;
			this->comboBox_direction->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"全部", L"净", L"多头", L"空头"});
			this->comboBox_direction->Location = System::Drawing::Point(693, 22);
			this->comboBox_direction->Name = L"comboBox_direction";
			this->comboBox_direction->Size = System::Drawing::Size(53, 20);
			this->comboBox_direction->TabIndex = 7;
			// 
			// button_query
			// 
			this->button_query->Location = System::Drawing::Point(801, 21);
			this->button_query->Name = L"button_query";
			this->button_query->Size = System::Drawing::Size(75, 23);
			this->button_query->TabIndex = 8;
			this->button_query->Text = L"查询";
			this->button_query->UseVisualStyleBackColor = true;
			this->button_query->Click += gcnew System::EventHandler(this, &PositionConfirmForm::button_query_Click);
			// 
			// button_clear
			// 
			this->button_clear->Location = System::Drawing::Point(895, 21);
			this->button_clear->Name = L"button_clear";
			this->button_clear->Size = System::Drawing::Size(75, 23);
			this->button_clear->TabIndex = 9;
			this->button_clear->Text = L"清空";
			this->button_clear->UseVisualStyleBackColor = true;
			this->button_clear->Click += gcnew System::EventHandler(this, &PositionConfirmForm::button_clear_Click);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->ForeColor = System::Drawing::SystemColors::ActiveCaption;
			this->label7->Location = System::Drawing::Point(23, 60);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(53, 12);
			this->label7->TabIndex = 10;
			this->label7->Text = L"持仓汇总";
			// 
			// dataGridView3
			// 
			this->dataGridView3->AllowUserToAddRows = false;
			this->dataGridView3->AllowUserToDeleteRows = false;
			this->dataGridView3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView3->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView3->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView3->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(10) {this->Column1, 
				this->Column2, this->Column3, this->Column4, this->Column5, this->Column6, this->Column7, this->Column8, this->Column9, this->Column10});
			this->dataGridView3->Location = System::Drawing::Point(25, 75);
			this->dataGridView3->Name = L"dataGridView3";
			this->dataGridView3->ReadOnly = true;
			this->dataGridView3->RowHeadersVisible = false;
			this->dataGridView3->RowTemplate->Height = 23;
			this->dataGridView3->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView3->Size = System::Drawing::Size(945, 279);
			this->dataGridView3->TabIndex = 11;
			// 
			// Column1
			// 
			this->Column1->HeaderText = L"账户号";
			this->Column1->Name = L"Column1";
			this->Column1->ReadOnly = true;
			this->Column1->Width = 66;
			// 
			// Column2
			// 
			this->Column2->HeaderText = L"账户名称";
			this->Column2->Name = L"Column2";
			this->Column2->ReadOnly = true;
			this->Column2->Width = 78;
			// 
			// Column3
			// 
			this->Column3->HeaderText = L"合约";
			this->Column3->Name = L"Column3";
			this->Column3->ReadOnly = true;
			this->Column3->Width = 54;
			// 
			// Column4
			// 
			this->Column4->HeaderText = L"持仓方向";
			this->Column4->Name = L"Column4";
			this->Column4->ReadOnly = true;
			this->Column4->Width = 78;
			// 
			// Column5
			// 
			this->Column5->HeaderText = L"投保";
			this->Column5->Name = L"Column5";
			this->Column5->ReadOnly = true;
			this->Column5->Width = 54;
			// 
			// Column6
			// 
			this->Column6->HeaderText = L"总持仓";
			this->Column6->Name = L"Column6";
			this->Column6->ReadOnly = true;
			this->Column6->Width = 66;
			// 
			// Column7
			// 
			this->Column7->HeaderText = L"结算价";
			this->Column7->Name = L"Column7";
			this->Column7->ReadOnly = true;
			this->Column7->Width = 66;
			// 
			// Column8
			// 
			this->Column8->HeaderText = L"持仓盈亏";
			this->Column8->Name = L"Column8";
			this->Column8->ReadOnly = true;
			this->Column8->Width = 78;
			// 
			// Column9
			// 
			this->Column9->HeaderText = L"浮动盈亏";
			this->Column9->Name = L"Column9";
			this->Column9->ReadOnly = true;
			this->Column9->Width = 78;
			// 
			// Column10
			// 
			this->Column10->HeaderText = L"占用保证金";
			this->Column10->Name = L"Column10";
			this->Column10->ReadOnly = true;
			this->Column10->Width = 90;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->ForeColor = System::Drawing::SystemColors::ActiveCaption;
			this->label8->Location = System::Drawing::Point(25, 394);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(53, 12);
			this->label8->TabIndex = 12;
			this->label8->Text = L"持仓明细";
			// 
			// dataGridView4
			// 
			this->dataGridView4->AllowUserToAddRows = false;
			this->dataGridView4->AllowUserToDeleteRows = false;
			this->dataGridView4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView4->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView4->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView4->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(15) {this->Column11, 
				this->Column12, this->Column13, this->Column14, this->Column15, this->Column16, this->Column17, this->Column18, this->Column19, 
				this->Column20, this->Column21, this->Column22, this->Column23, this->Column24, this->Column25});
			this->dataGridView4->Location = System::Drawing::Point(27, 410);
			this->dataGridView4->MultiSelect = false;
			this->dataGridView4->Name = L"dataGridView4";
			this->dataGridView4->ReadOnly = true;
			this->dataGridView4->RowHeadersVisible = false;
			this->dataGridView4->RowTemplate->Height = 23;
			this->dataGridView4->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView4->Size = System::Drawing::Size(943, 337);
			this->dataGridView4->TabIndex = 13;
			this->dataGridView4->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &PositionConfirmForm::dataGridView4_ColumnHeaderMouseClick);
			// 
			// button_add
			// 
			this->button_add->Location = System::Drawing::Point(23, 754);
			this->button_add->Name = L"button_add";
			this->button_add->Size = System::Drawing::Size(75, 23);
			this->button_add->TabIndex = 14;
			this->button_add->Text = L"新增明细";
			this->button_add->UseVisualStyleBackColor = true;
			this->button_add->Visible = false;
			this->button_add->Click += gcnew System::EventHandler(this, &PositionConfirmForm::button_add_Click);
			// 
			// button_modify
			// 
			this->button_modify->Location = System::Drawing::Point(115, 754);
			this->button_modify->Name = L"button_modify";
			this->button_modify->Size = System::Drawing::Size(75, 23);
			this->button_modify->TabIndex = 15;
			this->button_modify->Text = L"修改明细";
			this->button_modify->UseVisualStyleBackColor = true;
			this->button_modify->Visible = false;
			this->button_modify->Click += gcnew System::EventHandler(this, &PositionConfirmForm::button_modify_Click);
			// 
			// button_delete
			// 
			this->button_delete->Location = System::Drawing::Point(212, 754);
			this->button_delete->Name = L"button_delete";
			this->button_delete->Size = System::Drawing::Size(75, 23);
			this->button_delete->TabIndex = 16;
			this->button_delete->Text = L"删除明细";
			this->button_delete->UseVisualStyleBackColor = true;
			this->button_delete->Visible = false;
			this->button_delete->Click += gcnew System::EventHandler(this, &PositionConfirmForm::button_delete_Click);
			// 
			// button_confirm
			// 
			this->button_confirm->Location = System::Drawing::Point(366, 781);
			this->button_confirm->Name = L"button_confirm";
			this->button_confirm->Size = System::Drawing::Size(128, 23);
			this->button_confirm->TabIndex = 17;
			this->button_confirm->Text = L"确认提交";
			this->button_confirm->UseVisualStyleBackColor = true;
			this->button_confirm->Click += gcnew System::EventHandler(this, &PositionConfirmForm::button_confirm_Click);
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(858, 781);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(75, 23);
			this->button_cancel->TabIndex = 19;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Visible = false;
			this->button_cancel->Click += gcnew System::EventHandler(this, &PositionConfirmForm::button_cancel_Click);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(128, 392);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(41, 12);
			this->label9->TabIndex = 20;
			this->label9->Text = L"查询到";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(259, 393);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(53, 12);
			this->label10->TabIndex = 21;
			this->label10->Text = L"每页显示";
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(319, 385);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(49, 20);
			this->comboBox1->TabIndex = 22;
			this->comboBox1->SelectionChangeCommitted += gcnew System::EventHandler(this, &PositionConfirmForm::comboBox1_SelectionChangeCommitted);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(424, 392);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(47, 12);
			this->label11->TabIndex = 23;
			this->label11->Text = L" 当前第";
			// 
			// comboBox2
			// 
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Location = System::Drawing::Point(477, 385);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(44, 20);
			this->comboBox2->TabIndex = 24;
			this->comboBox2->SelectionChangeCommitted += gcnew System::EventHandler(this, &PositionConfirmForm::comboBox2_SelectionChangeCommitted);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(528, 393);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(35, 12);
			this->label12->TabIndex = 25;
			this->label12->Text = L"/1页 ";
			// 
			// comboBox_Ins
			// 
			this->comboBox_Ins->BackColor = System::Drawing::Color::White;
			this->comboBox_Ins->ChildControl = nullptr;
			this->comboBox_Ins->DropDownHeight = 1;
			this->comboBox_Ins->DropDownWidth = 1;
			this->comboBox_Ins->FormattingEnabled = true;
			this->comboBox_Ins->IntegralHeight = false;
			this->comboBox_Ins->Location = System::Drawing::Point(509, 23);
			this->comboBox_Ins->MaxDropDownItems = 1;
			this->comboBox_Ins->Name = L"comboBox_Ins";
			this->comboBox_Ins->Size = System::Drawing::Size(89, 20);
			this->comboBox_Ins->TabIndex = 5;
			// 
			// button_pre
			// 
			this->button_pre->Location = System::Drawing::Point(389, 385);
			this->button_pre->Name = L"button_pre";
			this->button_pre->Size = System::Drawing::Size(39, 23);
			this->button_pre->TabIndex = 26;
			this->button_pre->Text = L"<<";
			this->button_pre->UseVisualStyleBackColor = true;
			this->button_pre->Click += gcnew System::EventHandler(this, &PositionConfirmForm::button_pre_Click);
			// 
			// button_next
			// 
			this->button_next->Location = System::Drawing::Point(569, 383);
			this->button_next->Name = L"button_next";
			this->button_next->Size = System::Drawing::Size(42, 23);
			this->button_next->TabIndex = 27;
			this->button_next->Text = L">>";
			this->button_next->UseVisualStyleBackColor = true;
			this->button_next->Click += gcnew System::EventHandler(this, &PositionConfirmForm::button_next_Click);
			// 
			// Column11
			// 
			this->Column11->HeaderText = L"账户号";
			this->Column11->Name = L"Column11";
			this->Column11->ReadOnly = true;
			this->Column11->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column11->Width = 66;
			// 
			// Column12
			// 
			this->Column12->HeaderText = L"账户名称";
			this->Column12->Name = L"Column12";
			this->Column12->ReadOnly = true;
			this->Column12->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column12->Width = 78;
			// 
			// Column13
			// 
			this->Column13->HeaderText = L"合约";
			this->Column13->Name = L"Column13";
			this->Column13->ReadOnly = true;
			this->Column13->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column13->Width = 54;
			// 
			// Column14
			// 
			this->Column14->HeaderText = L"买卖";
			this->Column14->Name = L"Column14";
			this->Column14->ReadOnly = true;
			this->Column14->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column14->Width = 54;
			// 
			// Column15
			// 
			this->Column15->HeaderText = L"投保";
			this->Column15->Name = L"Column15";
			this->Column15->ReadOnly = true;
			this->Column15->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column15->Width = 54;
			// 
			// Column16
			// 
			this->Column16->HeaderText = L"手数";
			this->Column16->Name = L"Column16";
			this->Column16->ReadOnly = true;
			this->Column16->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column16->Width = 54;
			// 
			// Column17
			// 
			this->Column17->HeaderText = L"开仓价";
			this->Column17->Name = L"Column17";
			this->Column17->ReadOnly = true;
			this->Column17->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column17->Width = 66;
			// 
			// Column18
			// 
			this->Column18->HeaderText = L"结算价";
			this->Column18->Name = L"Column18";
			this->Column18->ReadOnly = true;
			this->Column18->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column18->Width = 66;
			// 
			// Column19
			// 
			this->Column19->HeaderText = L"开仓时间";
			this->Column19->Name = L"Column19";
			this->Column19->ReadOnly = true;
			this->Column19->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column19->Width = 78;
			// 
			// Column20
			// 
			this->Column20->HeaderText = L"成交编号";
			this->Column20->Name = L"Column20";
			this->Column20->ReadOnly = true;
			this->Column20->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column20->Width = 78;
			// 
			// Column21
			// 
			this->Column21->HeaderText = L"昨结算";
			this->Column21->Name = L"Column21";
			this->Column21->ReadOnly = true;
			this->Column21->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column21->Width = 66;
			// 
			// Column22
			// 
			this->Column22->HeaderText = L"占用保证金";
			this->Column22->Name = L"Column22";
			this->Column22->ReadOnly = true;
			this->Column22->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column22->Width = 90;
			// 
			// Column23
			// 
			this->Column23->HeaderText = L"平仓量";
			this->Column23->Name = L"Column23";
			this->Column23->ReadOnly = true;
			this->Column23->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column23->Width = 66;
			// 
			// Column24
			// 
			this->Column24->HeaderText = L"持仓盈亏";
			this->Column24->Name = L"Column24";
			this->Column24->ReadOnly = true;
			this->Column24->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column24->Width = 78;
			// 
			// Column25
			// 
			this->Column25->HeaderText = L"平仓盈亏";
			this->Column25->Name = L"Column25";
			this->Column25->ReadOnly = true;
			this->Column25->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column25->Width = 78;
			// 
			// PositionConfirmForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1005, 815);
			this->Controls->Add(this->button_next);
			this->Controls->Add(this->button_pre);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->comboBox2);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->button_confirm);
			this->Controls->Add(this->button_delete);
			this->Controls->Add(this->button_modify);
			this->Controls->Add(this->button_add);
			this->Controls->Add(this->dataGridView4);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->dataGridView3);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->button_clear);
			this->Controls->Add(this->button_query);
			this->Controls->Add(this->comboBox_direction);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->comboBox_Ins);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboBox_exchange);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboBox_user);
			this->Controls->Add(this->label1);
			this->Name = L"PositionConfirmForm";
			this->Text = L"持仓和持仓明细确认";
			this->Load += gcnew System::EventHandler(this, &PositionConfirmForm::PositionConfirmForm_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &PositionConfirmForm::PositionConfirmForm_FormClosed);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView4))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void PositionConfirmForm_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent(); 
				 int nHwnd = this->Handle.ToInt32();

				 dlgInstrumentIDTreeList = gcnew CInstrumentIDTreeListDlg();
				 dlgInstrumentIDTreeList->ResetRelateCombo(comboBox_Ins);
				 comboBox_Ins->ChildControl = dlgInstrumentIDTreeList;

				 std::map<int, UserInfo>  lMapFiled;
				 m_pDataInfo->GetUserInfo(lMapFiled);
				 std::map<int, UserInfo>::iterator it=  lMapFiled.begin();
				 while (it!=lMapFiled.end())
				 {
					 if ( it->second.nUserType == USER_TYPE_TRADER )
					 {											 
						 string strTemp =it->second.szAccount;
						 string strTempN =it->second.szAlias;
						 strTemp += "+" + strTempN;
						 comboBox_user->Items->Add(Tools::string2String(strTemp.c_str()));
					 }
					 it++;
				 }
				 dlgInstrumentIDTreeList->LoadInstrumentTree();

				 m_pDataInfo->GetLogonInfo(*m_pLogin);
				 PlatformStru_QryInstrument Field;
				 //CTcpLayer::SubscribePkg(Cmd_TradeDataManager_QryInstrument_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_QueryTradeAccount_Rsp, nHwnd);
				 //CTcpLayer::SubscribePkg(Cmd_RM_QryUser_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_QueryPositionDetail_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_AddPositionDetail_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_DeletePositionDetail_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_ModifyPositionDetail_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_QueryPosition_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_AddPosition_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_DeletePosition_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_ModifyPosition_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_QryUserTradeAcctRelation_Rsp, nHwnd);//用户与委托交易账号关系

				 
				 m_pDataInfo->ClearPositionDetail();
				 m_pDataInfo->ClearPosition();	

				 std::vector<PlatformStru_PositionDetail> lvecField;
				 std::vector<PlatformStru_Position> lvecField2;

				 GetPositionVec(lvecField2,lvecField);

				 UpdatePageBar(lvecField);
				 if ( !CTcpLayer::SendData(Cmd_Settlement_QueryPositionDetail_Req, NULL, 0, 0, m_pLogin->mnUserID)
					 || (!CTcpLayer::SendData(Cmd_Settlement_QueryPosition_Req, NULL, 0, 0, m_pLogin->mnUserID)))
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }

	             button_query->Enabled = false;

	}

protected:virtual void WndProc( Message% m ) override
		  {
			  if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival )
			  {
				  char *p = (char*)m.LParam.ToInt32();
				  Stru_UniPkgHead head;					
				  memset(&head, 0, sizeof(head));
				  memcpy(&head, p, sizeof(head));

				  //OUTPUT_LOG("客户端持仓确认收到消息： %d",  head.cmdid);
				  switch ( head.cmdid )
				  {
				  case Cmd_TradeDataManager_QryInstrument_Rsp:
					  {
						  if ( head.userdata3 == CF_ERROR_SUCCESS )
						  {
							  std::map<std::string, PlatformStru_InstrumentInfo> mapField;
							  int nCount = head.len / sizeof(PlatformStru_InstrumentInfo);
							  if(nCount == 0)
								  break;
							  for ( int i = 0; i < nCount; i++ )
							  {
								  PlatformStru_InstrumentInfo* pField = (PlatformStru_InstrumentInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(PlatformStru_InstrumentInfo));
								  mapField[pField->InstrumentID] = *pField;

							  }

							  m_pDataInfo->SetInstrumentInfo(mapField);
							  dlgInstrumentIDTreeList->LoadInstrumentTree();

						  }
						  else
						  {
							  string str = (char*)(p+sizeof(Stru_UniPkgHead));
							  String^ strtemp = Tools::string2String(str.c_str());
								_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
										str.c_str());
						  }

						  break;
					  }

				  case Cmd_QueryTradeAccount_Rsp:
					  {
						  if ( head.userdata1 == CF_ERROR_SUCCESS )
						  {

							  std::map<int, TradeAccount> mapTradeAccount;
							  int nCount = head.len / sizeof(TradeAccount);
							  if(nCount == 0)
								  break;
							  for ( int i = 0; i < nCount; i++ )
							  {
								  TradeAccount* pTradeAccount = (TradeAccount*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(TradeAccount));
								  mapTradeAccount[pTradeAccount->nTradeAccountID] = *pTradeAccount;										  

							  }

							  m_pDataInfo->SetTradeAccount(mapTradeAccount);
						  }
						  else
						  {
							  string str = (char*)(p+sizeof(Stru_UniPkgHead));
							  String^ strtemp = Tools::string2String(str.c_str());
								_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
										str.c_str());
						  }

						  break;
					  }
				  case Cmd_Settlement_QueryPositionDetail_Rsp:
					  {
						  if ( head.userdata1 == CF_ERROR_SUCCESS )
						  {

							  std::multimap<std::string,PlatformStru_PositionDetail>  lMapFiled;
							  int nCount = head.len / sizeof(PlatformStru_PositionDetail);
							  if(nCount == 0)
								  break;
							  for ( int i = 0; i < nCount; i++ )
							  {
								  PlatformStru_PositionDetail* pField = (PlatformStru_PositionDetail*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(PlatformStru_PositionDetail));

								  lMapFiled.insert(make_pair(pField->InvestorID,*pField));
							  }
							  m_pDataInfo->SetPositionDetail(lMapFiled);

							  std::vector<PlatformStru_PositionDetail> lvecField;
							  std::vector<PlatformStru_Position> lvecField2;

							  GetPositionVec(lvecField2,lvecField);
							  m_pDataInfo->SaveSortedPositionDetail(lvecField);
							  std::vector<PlatformStru_PositionDetail> vecShow;
							  GetShowData(lvecField,vecShow);
							  loadPositionDetailView(vecShow);
							  UpdatePageBar(lvecField);
							  if (lvecField.size() == 0)
							  {
								  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
									  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
									  "没有持仓明细");

							  }


						  }
						  else
						  {
							  string str = (char*)(p+sizeof(Stru_UniPkgHead));
							  String^ strtemp = Tools::string2String(str.c_str());
							  //  m_pDataInfo->PopMsgShow(str.c_str());
							  /*PromptForm^ dlg = gcnew PromptForm(strtemp);
							  dlg->Show();*/
							  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
								  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
								  str.c_str());

						  }
                          button_query->Enabled = true;
						  break;
					  }
				  case Cmd_Settlement_AddPositionDetail_Rsp:
					  {
						  if ( head.userdata1 == CF_ERROR_SUCCESS )
						  {

							  PlatformStru_PositionDetail* pField = (PlatformStru_PositionDetail*)(char*)(p+sizeof(Stru_UniPkgHead)) ;
							  m_pDataInfo->SetPositionDetail(pField);
							  std::vector<PlatformStru_PositionDetail> lvecField;
							  std::vector<PlatformStru_Position> lvecField2;

							  GetPositionVec(lvecField2,lvecField);


							  loadPositionDetailView(lvecField);
							  //loadPositionView(lvecField2);

						  }
						  else
						  {
							  string str = (char*)(p+sizeof(Stru_UniPkgHead));
							  String^ strtemp = Tools::string2String(str.c_str());
								_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
										str.c_str());
						  }

						  break;
					  }
				  case Cmd_Settlement_ModifyPositionDetail_Req:
					  {
						  if ( head.userdata1 == CF_ERROR_SUCCESS )
						  {

							  PlatformStru_PositionDetail* pField = (PlatformStru_PositionDetail*)(char*)(p+sizeof(Stru_UniPkgHead)) ;
							  /*m_pDataInfo->ModifyPositionDetail(pField);*/
							  std::vector<PlatformStru_PositionDetail> lvecField;
							  std::vector<PlatformStru_Position> lvecField2;

							  GetPositionVec(lvecField2,lvecField);


							  loadPositionDetailView(lvecField);
						  }
						  else
						  {
							  string str = (char*)(p+sizeof(Stru_UniPkgHead));
							  String^ strtemp = Tools::string2String(str.c_str());
								_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
										str.c_str());
						  }

						  break;
					  }
				  case Cmd_Settlement_DeletePositionDetail_Req:
					  {
						  if ( head.userdata1 == CF_ERROR_SUCCESS )
						  {

							  PlatformStru_PositionDetail* pField = (PlatformStru_PositionDetail*)(char*)(p+sizeof(Stru_UniPkgHead)) ;
							  //m_pDataInfo->SetPositionDetail(pField);
							  DataGridViewRow^  c = dataGridView4->CurrentRow;
							  if(c != nullptr)
								  dataGridView4->Rows->RemoveAt(c->Index);  

						  }
						  else
						  {
							  string str = (char*)(p+sizeof(Stru_UniPkgHead));
							  String^ strtemp = Tools::string2String(str.c_str());
								_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
										str.c_str());
						  }

						  break;
					  }
				  case Cmd_Settlement_QueryPosition_Rsp:
					  {
						  if ( head.userdata1 == CF_ERROR_SUCCESS )
						  {

							  std::multimap<std::string,PlatformStru_Position>  lMapFiled;
							  int nCount = head.len / sizeof(PlatformStru_Position);
							  if(nCount == 0)
								  break;
							  for ( int i = 0; i < nCount; i++ )
							  {
								  PlatformStru_Position* pField = (PlatformStru_Position*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(PlatformStru_Position));

								  lMapFiled.insert(make_pair(pField->InvestorID,*pField));
							  }
							  m_pDataInfo->SetPosition(lMapFiled);	
							  CFileOpr::getObj().writelocallog("PositionConfirmForm.log","客户端收到持仓数据 %d 条",
								  lMapFiled.size());

							  std::vector<PlatformStru_PositionDetail> lvecField;
							  std::vector<PlatformStru_Position> lvecField2;
							  GetPositionVec(lvecField2,lvecField);						 
							  loadPositionView(lvecField2);
							  if (lvecField2.size() == 0)
							  {
								  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
									  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
									  "没有持仓记录");

							  }

							  

						  }
						  else
						  {
							  string str = (char*)(p+sizeof(Stru_UniPkgHead));
							  String^ strtemp = Tools::string2String(str.c_str());
								_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
										str.c_str());
							  //PromptForm^ dlg = gcnew PromptForm(strtemp);
							 // dlg->Show();
						  }

						  break;
					  }
				  case Cmd_QryUser_Req:
					  {
						  if ( head.userdata1 == CF_ERROR_SUCCESS )
						  {

							  std::map<int, UserInfo>  lMapFiled;
							  int nCount = head.len / sizeof(UserInfo);
							  if(nCount == 0)
								  break;
							  for ( int i = 0; i < nCount; i++ )
							  {
								  UserInfo* pField = (UserInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(UserInfo));

								  lMapFiled.insert(make_pair(pField->nUserID,*pField));
								  string strTemp =pField->szAccount;
								  string strTempN =pField->szAlias;
								  strTemp += "+" + strTempN;
								  comboBox_user->Items->Add(Tools::string2String(strTemp.c_str()));
							  }
							  m_pDataInfo->SetUserInfo(lMapFiled);								 

						  }
						  else
						  {
							  string str = (char*)(p+sizeof(Stru_UniPkgHead));
							  String^ strtemp = Tools::string2String(str.c_str());
								_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
										str.c_str());
						  }

						  break;
					  }
				  case Cmd_QryUserTradeAcctRelation_Rsp:
					  {
						  if ( head.userdata1 == CF_ERROR_SUCCESS )
						  {
							  m_pDataInfo->ClearUserAndTraderAccountRelation();
							  std::map<int, UserAndTradeAccountRelation> mapUserAndTradeAccountRelation;
							  int nCount = head.len / sizeof(UserAndOrgRelation);
							  if(nCount == 0)
								  break;
							  for ( int i = 0; i < nCount; i++ )
							  {
								  UserAndTradeAccountRelation* pRelation = (UserAndTradeAccountRelation*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(UserAndTradeAccountRelation));
								  mapUserAndTradeAccountRelation.insert(make_pair(pRelation->nUserID, *pRelation));                             
							  }

							  m_pDataInfo->SetUserAndTraderAccountRelation(mapUserAndTradeAccountRelation);	

						  }
						  else
						  {
							  string str = (char*)(p+sizeof(Stru_UniPkgHead));
							  String^ strtemp = Tools::string2String(str.c_str());
								_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
										CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
										str.c_str());
						  }

						  break;
					  } 

				  default:
					  break;
				  }

			  }

			  Form::WndProc ( m );
		  }
private: System::Void GetPositionVec(std::vector<PlatformStru_Position>& vecFieldP,std::vector<PlatformStru_PositionDetail>& vecFieldPD) 
		 {
			 std::vector<PlatformStru_Position> vecPosition;
			 vecPosition.clear();
			 m_pDataInfo->GetPosition(vecPosition);

			 std::vector<PlatformStru_PositionDetail> vecPositionDetail;
			 vecPositionDetail.clear();
			 m_pDataInfo->GetPositionDetail(vecPositionDetail);


			 string strUserID = Tools::String2string(comboBox_user->Text);
			 if (strUserID !="" &&  strUserID !=SELECT_ALL )
			 {
				 unsigned pos = strUserID.find("+");        
				 std::string str = strUserID.substr ( 0,pos );

				 std::vector<PlatformStru_Position>::iterator it= vecPosition.begin();
				 while (it!=vecPosition.end())
				 {

					 if ((*it).InvestorID !=str )
					 {
						 it=vecPosition.erase(it);
					 }
					 else
						 ++it;
				 }
				 std::vector<PlatformStru_PositionDetail>::iterator it2= vecPositionDetail.begin();
				 while (it2!=vecPositionDetail.end())
				 {

					 if ((*it2).InvestorID !=str )
					 {
						 it2=vecPositionDetail.erase(it2);
					 }
					 else
						 ++it2;
				 }

			 }




			 std::set<string> setFilterInstrumentID;
			 dlgInstrumentIDTreeList->GetSelectID(setFilterInstrumentID);
			 std::vector<PlatformStru_Position> vecPositionIns;
			 std::vector<PlatformStru_PositionDetail> vecPositionDetailIns;
			 if (setFilterInstrumentID.size()!=0)
			 {
				 std::set<string>::iterator it =setFilterInstrumentID.begin();
				 for (; it!=setFilterInstrumentID.end();it++)
				 {
					 for (int i=0;i<(int)vecPosition.size();i++)
					 {
						 if( strncmp((*it).c_str(),vecPosition[i].InstrumentID,(*it).length()) == 0)
							 vecPositionIns.push_back(vecPosition[i]);
					 }
					 for (int i=0;i<(int)vecPositionDetail.size();i++)
					 {
						 if( strncmp((*it).c_str(),vecPositionDetail[i].InstrumentID,(*it).length()) == 0)
							 vecPositionDetailIns.push_back(vecPositionDetail[i]);
					 }
				 }
			 }
			 else
			 {
				 vecPositionDetailIns = vecPositionDetail;
				 vecPositionIns = vecPosition;


			 }

			 //交易所
			 string strExchange = Tools::String2string(comboBox_exchange->Text);
			 if (strExchange != ""&& strExchange !=SELECT_ALL)
			 {
				 string exchange =  m_pDataInfo->ExchangeName2ID(strExchange);
				 std::vector<PlatformStru_Position>::iterator it= vecPositionIns.begin();
				 while (it!=vecPositionIns.end())
				 {

					 if ((*it).ExchangeID !=exchange )
					 {
						 it=vecPositionIns.erase(it);
					 }
					 else
						 ++it;
				 }

				 std::vector<PlatformStru_PositionDetail>::iterator it2= vecPositionDetailIns.begin();
				 while (it2!=vecPositionDetailIns.end())
				 {

					 if ((*it2).ExchangeID !=exchange )
					 {
						 it2=vecPositionDetailIns.erase(it2);
					 }
					 else
						 ++it2;
				 }

			 }

			 //买卖
			 string strbuySell = Tools::String2string(comboBox_direction->Text);
			 if (strbuySell != ""&& strbuySell !=SELECT_ALL)
			 {
				 std::vector<PlatformStru_Position>::iterator it= vecPositionIns.begin();
				 while (it!=vecPositionIns.end())
				 {
					 if (m_pDataInfo->Direction2String((*it).PosiDirection) !=strbuySell )
					 {
						 it=vecPositionIns.erase(it);
					 }
					 else
						 ++it;
				 }
				 if (strbuySell == "多头")
					 strbuySell = "买";
				 if (strbuySell == "空头")
				     strbuySell="卖";
				 std::vector<PlatformStru_PositionDetail>::iterator it2= vecPositionDetailIns.begin();
				 while (it2!=vecPositionDetailIns.end())
				 {

					 if (m_pDataInfo->TradeDirection2String((*it2).Direction) !=strbuySell )
					 {
						 it2=vecPositionDetailIns.erase(it2);
					 }
					 else
						 ++it2;
				 }

			 }

			 vecFieldP = vecPositionIns;

			 vecFieldPD = vecPositionDetailIns;

		 }

private: System::Void loadPositionView(std::vector<PlatformStru_Position>& lvecField)
		 {
			 dataGridView3->Rows->Clear();
			 std::vector<PlatformStru_Position>::iterator it;
			 for(it = lvecField.begin(); it != lvecField.end(); it++)
			 {

				 int newRowIndex=dataGridView3->Rows->Add();
				 DataGridViewRow^ dr = dataGridView3->Rows[newRowIndex];
				 dr->Cells[0]->Value = Tools::string2String((*it).InvestorID);
				 UserInfo sUserInfo;
				 m_pDataInfo->GetUserInfoByAccount((*it).InvestorID,sUserInfo);
				 dr->Cells[1]->Value = Tools::string2String(sUserInfo.szAlias);
				 dr->Cells[2]->Value = Tools::string2String((*it).InstrumentID);
				 dr->Cells[3]->Value = Tools::string2String(m_pDataInfo->Direction2String((*it).PosiDirection));
				 dr->Cells[4]->Value = Tools::string2String(m_pDataInfo->HedgeFlag2String((*it).HedgeFlag));
				 char strPrice[128];
				 memset(strPrice,0,sizeof(strPrice));
				 sprintf(strPrice, "%d",( *it).Position);
				 dr->Cells[5]->Value = Tools::string2String(strPrice);


				 if ((*it).SettlementPrice!= InvalideValue && (*it).SettlementPrice!= 0 )
				 {
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%.3f",( *it).SettlementPrice);
					 dr->Cells[6]->Value = Tools::string2String(strPrice);
				 }
				 if ((*it).PositionProfit!= InvalideValue)
				 {
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%.2f",( *it).PositionProfit);
					 dr->Cells[7]->Value = Tools::string2String(strPrice);
				 }
				 if ((*it).PositionProfitByTrade!= InvalideValue)
				 {
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%.2f",( *it).PositionProfitByTrade);
					 dr->Cells[8]->Value = Tools::string2String(strPrice);
				 }
				 if ((*it).CloseProfit!= InvalideValue)
				 {

					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%.2f",( *it).CloseProfit);
					 dr->Cells[9]->Value = Tools::string2String(strPrice);
				 }
				 if ((*it).UseMargin!= InvalideValue)
					{
						memset(strPrice,0,sizeof(strPrice));
						sprintf(strPrice, "%.2f",( *it).UseMargin);
						dr->Cells[9]->Value = Tools::string2String(strPrice);
					}

                CFileOpr::getObj().writelocallog("PositionConfirmForm.log","加载一条持仓数据 合约 %s",  (*it).InstrumentID);

			 }
		 }
private: System::Void loadPositionDetailView(std::vector<PlatformStru_PositionDetail>& lvecField)
		 {
			 dataGridView4->Rows->Clear();
			 std::vector<PlatformStru_PositionDetail>::iterator it;
			 for(it = lvecField.begin(); it != lvecField.end(); it++)
			 {

				 int newRowIndex=dataGridView4->Rows->Add();
				 DataGridViewRow^ dr = dataGridView4->Rows[newRowIndex];
				 dr->Cells[0]->Value = Tools::string2String((*it).InvestorID);
				 UserInfo sUserInfo;
				 m_pDataInfo->GetUserInfoByAccount((*it).InvestorID,sUserInfo);
				 dr->Cells[1]->Value = Tools::string2String(sUserInfo.szAlias);
				 dr->Cells[2]->Value = Tools::string2String((*it).InstrumentID);
				 dr->Cells[3]->Value = Tools::string2String(m_pDataInfo->TradeDirection2String((*it).Direction));
				 dr->Cells[4]->Value = Tools::string2String(m_pDataInfo->HedgeFlag2String((*it).HedgeFlag));
				 char strPrice[128];
				 memset(strPrice,0,sizeof(strPrice));
				 sprintf(strPrice, "%d",( *it).Volume);
				 dr->Cells[5]->Value = Tools::string2String(strPrice);

				 memset(strPrice,0,sizeof(strPrice));
				 sprintf(strPrice, "%.3f",( *it).OpenPrice);
				 dr->Cells[6]->Value = Tools::string2String(strPrice);
				 if ((*it).SettlementPrice!= InvalideValue && (*it).SettlementPrice!= 0 )
				 {
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%.3f",( *it).SettlementPrice);
					 dr->Cells[7]->Value = Tools::string2String(strPrice);
				 }

				 dr->Cells[8]->Value = Tools::string2String((*it).OpenDate);
				 dr->Cells[9]->Value = Tools::string2String((*it).TradeID);
				 if ((*it).LastSettlementPrice!= InvalideValue && (*it).LastSettlementPrice!= 0 )
				 {
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%.3f",( *it).LastSettlementPrice);
					 dr->Cells[10]->Value = Tools::string2String(strPrice);
				 }
				 if ((*it).Margin!= InvalideValue)
				 {
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%.2f",( *it).Margin);
					 dr->Cells[11]->Value = Tools::string2String(strPrice);
				 }


				 if ((*it).CloseVolume!= InvalideValue)
				 {
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%d",( *it).CloseVolume);
					 dr->Cells[12]->Value = Tools::string2String(strPrice);
				 }
				 if ((*it).PositionProfitByDate!= InvalideValue)
				 {
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%.2f",( *it).PositionProfitByDate);
					 dr->Cells[13]->Value = Tools::string2String(strPrice);
				 }
				 if ((*it).CloseProfitByDate!= InvalideValue)
				 {
					 memset(strPrice,0,sizeof(strPrice));
					 sprintf(strPrice, "%.2f",( *it).CloseProfitByDate);
					 dr->Cells[14]->Value = Tools::string2String(strPrice);
				 }



			 }

		 }

private: System::Void button_query_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			  
			 if (!CTcpLayer::SendData(Cmd_Settlement_QueryPositionDetail_Req, NULL, 0, 0, m_pLogin->mnUserID))				 
			 {
				 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			 }

			 if (!CTcpLayer::SendData(Cmd_Settlement_QueryPosition_Req, NULL, 0, 0, m_pLogin->mnUserID))				 
			 {
				 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			 }


		 }
private: System::Void button_confirm_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"是否确定确认持仓和持仓明细？", "提示", MessageBoxButtons::YesNo);
			 if ( dr == System::Windows::Forms::DialogResult::Yes)
			 { 
				 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
				 this->Close();
			 }
		 }




private: System::Void button_clear_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 m_pDataInfo->ClearPosition();
			 m_pDataInfo->ClearPositionDetail();
			 std::vector<PlatformStru_PositionDetail> lvecField;
			 std::vector<PlatformStru_Position> lvecField2;
             GetPositionVec(lvecField2,lvecField);
			 m_pDataInfo->SaveSortedPositionDetail(lvecField);
			 UpdatePageBar(lvecField);
			  dataGridView4->Rows->Clear();
			  dataGridView3->Rows->Clear();
		 }

private: System::Void button_add_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 CTcpLayer::SendData(Cmd_QryUserTradeAcctRelation_Req, NULL,0, 0,  m_pLogin->mnUserID);
			 AddPDform^ dlg = gcnew AddPDform();
			 dlg->SetOperType(Oper_Add);
			 if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
			 {
				 *m_pCurPositionDetail = *(dlg->GetPositionDetail());             
				 CTcpLayer::SendData(Cmd_Settlement_AddPositionDetail_Req, m_pCurPositionDetail, sizeof(PlatformStru_PositionDetail), 0);
			 }
		 }
private: System::Void button_modify_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(dataGridView4->SelectedRows != nullptr && dataGridView4->SelectedRows->Count == 1)
			 {
				 DataGridViewRow^ cur =   dataGridView4->CurrentRow;	
				 string  userID=Tools::String2string(cur->Cells[0]->Value->ToString());
				 string  traderID=Tools::String2string(cur->Cells[9]->Value->ToString());

				 if(!m_pDataInfo->GetPositionDetail(userID,traderID,*m_pCurPositionDetail))
					 return;

				 AddPDform^ dlg = gcnew AddPDform();
				 dlg->SetOperType(Oper_Modify);
				 dlg->SetPositionDetail(*m_pCurPositionDetail);
				 if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
				 {
					 *m_pCurPositionDetail = *(dlg->GetPositionDetail());             
					 CTcpLayer::SendData(Cmd_Settlement_ModifyPositionDetail_Req, m_pCurPositionDetail, sizeof(PlatformStru_PositionDetail), 0);
				 }
			 }
			 else
				 MessageBox::Show("请选择内容");
		 }
private: System::Void button_delete_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(dataGridView4->SelectedRows != nullptr && dataGridView4->SelectedRows->Count == 1)
			 {
				 DataGridViewRow^ cur =   dataGridView4->CurrentRow;	
				 string  userID=Tools::String2string(cur->Cells[0]->Value->ToString());
				 string  traderID=Tools::String2string(cur->Cells[9]->Value->ToString());

				 if(!m_pDataInfo->GetPositionDetail(userID,traderID,*m_pCurPositionDetail))
					 return;

				 CTcpLayer::SendData(Cmd_Settlement_DeletePositionDetail_Req, m_pCurPositionDetail, sizeof(PlatformStru_PositionDetail), 0);
			 }
			 else
				 MessageBox::Show("请选择内容");
		 }
private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Close();
		 }
 private: void GetShowData(std::vector<PlatformStru_PositionDetail>& vecAllPositionDetail,std::vector<PlatformStru_PositionDetail>& vecShowPositionDetail)
				  {

					  int nDistance = (m_nCurShowPage-1) * m_nRowNumPerPage;
					  std::vector<PlatformStru_PositionDetail>::iterator it_dis = vecAllPositionDetail.begin();
					  if ( nDistance < static_cast<int>(vecAllPositionDetail.size()))
					  {
						  std::advance(it_dis, nDistance);
						  for ( int i = 0; i < m_nRowNumPerPage && it_dis != vecAllPositionDetail.end(); i++, it_dis++ )
						  {
							  vecShowPositionDetail.push_back(*it_dis);

						  }
					  }

				  }

private:void  UpdatePageBar(std::vector<PlatformStru_PositionDetail>& vecPositionDetail)
		{

			char strVolume[32];
			memset(strVolume,0,strlen(strVolume)+1);
			sprintf(strVolume, "查询到%d条记录",vecPositionDetail.size());
			this->label9->Text = Tools::string2String(strVolume);

			int nTotalPage = vecPositionDetail.size() / m_nRowNumPerPage;
			if ( vecPositionDetail.size() % m_nRowNumPerPage != 0 )
			{
				nTotalPage += 1;
			}



			memset(strVolume,0,strlen(strVolume)+1);
			sprintf(strVolume, "/%d页",nTotalPage);
			this->label12->Text = Tools::string2String(strVolume);




			comboBox1->Items->Clear();

			for ( int i = 10; i <=  (static_cast<int>(vecPositionDetail.size())+10) && i<= 50; i+=10  )
			{
				comboBox1->Items->Add( i.ToString() );
			}
			this->comboBox1->Text = m_nRowNumPerPage.ToString();


			comboBox2->Items->Clear();

			int nCount = comboBox2->Items->Count;
			if ( nCount > nTotalPage )
			{
				for ( int i = 0; i < nCount - nTotalPage; i++ )
				{
					comboBox2->Items->Add( (nCount-1-i).ToString() );
				}
			}
			else if ( nCount < nTotalPage )
			{
				for ( int i = 0; i < nTotalPage - nCount; i++ )
				{

					comboBox2->Items->Add( (nCount+1+i).ToString() );
				}
			}
			this->comboBox2->Text = m_nCurShowPage.ToString();
			if (m_nCurShowPage > nTotalPage )
			{
				m_nCurShowPage =1;
				comboBox2->Text = L"1";

			}
			 if ( 0 == nTotalPage )
			 {
				 m_nCurShowPage =1;
				 comboBox2->Text = L"0";
				 button_pre->Enabled = false;
				 button_next->Enabled = false;

			 }
			 else if ( nTotalPage == 1 )
			 {
				button_pre->Enabled = false;
				button_next->Enabled = false;
			 }
			 else
			 {
				 if ( m_nCurShowPage == 1 )
				 {
					 button_pre->Enabled = false;
					 button_next->Enabled = true;
				 }
				 else if ( m_nCurShowPage == nTotalPage )
				 {
					 button_pre->Enabled = true;
					 button_next->Enabled = false;
				 }
				 else
				 {
					 button_pre->Enabled = true;
					 button_next->Enabled = true;
				 }
			 }
		}

private: System::Void comboBox1_SelectionChangeCommitted(System::Object^  sender, System::EventArgs^  e) 
		 {
			 m_nRowNumPerPage = int::Parse(comboBox1->SelectedItem->ToString());
			 std::vector<PlatformStru_PositionDetail> lvecField;

			 m_pDataInfo->GetSortedPositionDetail(lvecField);
			 UpdatePageBar(lvecField);
			 std::vector<PlatformStru_PositionDetail> vecShow;
			 GetShowData(lvecField,vecShow);
			 loadPositionDetailView(vecShow);
		 }

private: System::Void comboBox2_SelectionChangeCommitted(System::Object^  sender, System::EventArgs^  e)
		 {
			 m_nCurShowPage = int::Parse(comboBox2->SelectedItem->ToString());
			 std::vector<PlatformStru_PositionDetail> lvecField;
			 m_pDataInfo->GetSortedPositionDetail(lvecField);
			 std::vector<PlatformStru_PositionDetail> vecShow;
			 GetShowData(lvecField,vecShow);
			 loadPositionDetailView(vecShow);

			 UpdatePageBar(lvecField);
		 }
private: System::Void button_pre_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if ( m_nCurShowPage > 1 )
			 {
				 m_nCurShowPage -= 1;

				 std::vector<PlatformStru_PositionDetail> lvecField;
	
				 m_pDataInfo->GetSortedPositionDetail(lvecField);
				 std::vector<PlatformStru_PositionDetail> vecShow;
				 GetShowData(lvecField,vecShow);
				 loadPositionDetailView(vecShow);
				 UpdatePageBar(lvecField);
			 }
		 }

private: System::Void button_next_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 std::vector<PlatformStru_PositionDetail> lvecField;

			 m_pDataInfo->GetSortedPositionDetail(lvecField);
			 if ( m_nCurShowPage < (int)lvecField.size() )
			 {
				 m_nCurShowPage += 1;


				 std::vector<PlatformStru_PositionDetail> vecShow;
				 GetShowData(lvecField,vecShow);
				 loadPositionDetailView(vecShow);
				 UpdatePageBar(lvecField);
			 }
		 }
private: System::Void PositionConfirmForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
		 {
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
			
		 }
private: System::Void dataGridView4_ColumnHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) 
		 {	 
			 DataGridView^ dataGridView = (DataGridView^)(sender);
			 DataGridViewColumn^ SColumn = dataGridView4->Columns[e->ColumnIndex];


			 std::vector<PlatformStru_PositionDetail> lvecField;
			 std::vector<PlatformStru_Position> lvecField2;

			 GetPositionVec(lvecField2,lvecField);
			 // 排序

			 if (SColumn->HeaderCell->SortGlyphDirection == SortOrder::Ascending)
			 {
				 SColumn->HeaderCell->SortGlyphDirection = SortOrder::Descending;

				 switch(SColumn->Index)
				 {
				 case 0:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_0());
					 break;
				 case 1:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_1());
					 break;
				 case 2:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_2());
					 break;
				 case 3:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_3());
					 break;
				 case 4:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_4());
					 break;
				 case 5:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_5());
					 break;
				 case 6:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_6());
					 break;
				 case 7:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_7());
					 break;
				 case 8:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_8());
					 break;
				 case 9:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_9());
					 break;
				 case 10:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_10());
					 break;
				 case 11:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_11());
					 break;
				 case 12:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_12());
					 break;
				 case 13:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_13());
					 break;
				 case 14:
					 sort(lvecField.begin(),lvecField.end(),PDAComparer_14());
					 break;


				 }
			 }
			 else
			 {
				 SColumn->HeaderCell->SortGlyphDirection = SortOrder::Ascending;

				 switch(SColumn->Index)
				 {
				 case 0:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_0());
					 break;
				 case 1:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_1());
					 break;
				 case 2:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_2());
					 break;
				 case 3:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_3());
					 break;
				 case 4:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_4());
					 break;
				 case 5:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_5());
					 break;
				 case 6:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_6());
					 break;
				 case 7:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_7());
					 break;
				 case 8:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_8());
					 break;
				 case 9:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_9());
					 break;
				 case 10:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_10());
					 break;
				 case 11:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_11());
					 break;
				 case 12:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_12());
					 break;
				 case 13:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_13());
					 break;
				 case 14:
					 sort(lvecField.begin(),lvecField.end(),PDComparer_14());
					 break;


				 }
			 }


			 m_pDataInfo->SaveSortedPositionDetail(lvecField);


			 UpdatePageBar(lvecField);
			 std::vector<PlatformStru_PositionDetail> vecShow;
			 GetShowData(lvecField,vecShow);
			 loadPositionDetailView(vecShow);

		 }
};
}

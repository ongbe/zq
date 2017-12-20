#pragma once

#include "TcpLayer.h"
#include "CommonDef.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "CDataInfo.h"
#include "Tools.h"
#include "CommonStruct.h"
#include "LoadHistoryCommission.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

	/// <summary>
	/// Summary for CommissionMarginConfirmForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CommissionMarginConfirmForm : public System::Windows::Forms::Form
	{
	public:
		CommissionMarginConfirmForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pLogin = new sLoginRsp();
			m_pDataInfo = CDataInfo::NewInstance();
			InvalideValue=util::GetDoubleInvalidValue();
			bConfirmOrApply=false;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CommissionMarginConfirmForm()
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
		}
		bool bConfirmOrApply;
	private: sLoginRsp*     m_pLogin;
	private: CDataInfo*     m_pDataInfo;
	private: double         InvalideValue;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage_SHFE;
	private: System::Windows::Forms::TabPage^  tabPage_CZCE;
	private: System::Windows::Forms::Button^  button_confirm;
	private: System::Windows::Forms::Button^  button_apply;
	private: System::Windows::Forms::Button^  button_cancel;
	private: System::Windows::Forms::TabPage^  tabPage_DCE;
	private: System::Windows::Forms::TabPage^  tabPage_CFFEX;
	private: System::Windows::Forms::DataGridView^  dataGridView_SHFE;

	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn39;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn40;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn41;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn42;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn43;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn44;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn45;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn46;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn47;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn48;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn49;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn50;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn51;
	private: System::Windows::Forms::DataGridView^  dataGridView_CZCE;


	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn26;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn27;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn28;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn29;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn30;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn31;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn32;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn33;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn34;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn35;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn36;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn37;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn38;
	private: System::Windows::Forms::DataGridView^  dataGridView_DCE;

	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn3;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn4;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn5;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn6;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn7;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn8;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn9;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn10;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn11;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn12;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn25;
	private: System::Windows::Forms::DataGridView^  dataGridView_CFFEX;

	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn13;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn14;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn15;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn16;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn17;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn18;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn19;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn20;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn21;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn22;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn23;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn24;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  dataGridViewTextBoxColumn52;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::ComboBox^  comboBox2;
private: System::Windows::Forms::Label^  labelprompting;
private: System::Windows::Forms::Button^  button_LoadHistoryCommission;





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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage_SHFE = (gcnew System::Windows::Forms::TabPage());
			this->dataGridView_SHFE = (gcnew System::Windows::Forms::DataGridView());
			this->dataGridViewTextBoxColumn39 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn40 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn41 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn42 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn43 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn44 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn45 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn46 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn47 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn48 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn49 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn50 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn51 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->tabPage_CZCE = (gcnew System::Windows::Forms::TabPage());
			this->dataGridView_CZCE = (gcnew System::Windows::Forms::DataGridView());
			this->dataGridViewTextBoxColumn26 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn27 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn28 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn29 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn30 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn31 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn32 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn33 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn34 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn35 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn36 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn37 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn38 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->tabPage_DCE = (gcnew System::Windows::Forms::TabPage());
			this->dataGridView_DCE = (gcnew System::Windows::Forms::DataGridView());
			this->dataGridViewTextBoxColumn1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn4 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn5 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn6 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn7 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn8 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn9 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn10 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn11 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn12 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn25 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->tabPage_CFFEX = (gcnew System::Windows::Forms::TabPage());
			this->dataGridView_CFFEX = (gcnew System::Windows::Forms::DataGridView());
			this->dataGridViewTextBoxColumn13 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn14 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn15 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn16 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn17 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn18 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn19 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn20 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn21 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn22 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn23 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn24 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridViewTextBoxColumn52 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->button_confirm = (gcnew System::Windows::Forms::Button());
			this->button_apply = (gcnew System::Windows::Forms::Button());
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->labelprompting = (gcnew System::Windows::Forms::Label());
			this->button_LoadHistoryCommission = (gcnew System::Windows::Forms::Button());
			this->tabControl1->SuspendLayout();
			this->tabPage_SHFE->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_SHFE))->BeginInit();
			this->tabPage_CZCE->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_CZCE))->BeginInit();
			this->tabPage_DCE->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_DCE))->BeginInit();
			this->tabPage_CFFEX->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_CFFEX))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(257, 25);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"交易委托账号";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(496, 17);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 2;
			this->button1->Text = L"查询";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &CommissionMarginConfirmForm::button1_Click);
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"全部"});
			this->comboBox1->Location = System::Drawing::Point(340, 17);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 20);
			this->comboBox1->TabIndex = 3;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage_SHFE);
			this->tabControl1->Controls->Add(this->tabPage_CZCE);
			this->tabControl1->Controls->Add(this->tabPage_DCE);
			this->tabControl1->Controls->Add(this->tabPage_CFFEX);
			this->tabControl1->Location = System::Drawing::Point(32, 84);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(905, 441);
			this->tabControl1->TabIndex = 4;
			this->tabControl1->SelectedIndexChanged += gcnew System::EventHandler(this, &CommissionMarginConfirmForm::tabControl1_SelectedIndexChanged);
			// 
			// tabPage_SHFE
			// 
			this->tabPage_SHFE->Controls->Add(this->dataGridView_SHFE);
			this->tabPage_SHFE->Location = System::Drawing::Point(4, 21);
			this->tabPage_SHFE->Name = L"tabPage_SHFE";
			this->tabPage_SHFE->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_SHFE->Size = System::Drawing::Size(897, 416);
			this->tabPage_SHFE->TabIndex = 0;
			this->tabPage_SHFE->Text = L"上期所";
			this->tabPage_SHFE->UseVisualStyleBackColor = true;
			// 
			// dataGridView_SHFE
			// 
			this->dataGridView_SHFE->AllowUserToAddRows = false;
			this->dataGridView_SHFE->AllowUserToDeleteRows = false;
			this->dataGridView_SHFE->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView_SHFE->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView_SHFE->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(13) {this->dataGridViewTextBoxColumn39, 
				this->dataGridViewTextBoxColumn40, this->dataGridViewTextBoxColumn41, this->dataGridViewTextBoxColumn42, this->dataGridViewTextBoxColumn43, 
				this->dataGridViewTextBoxColumn44, this->dataGridViewTextBoxColumn45, this->dataGridViewTextBoxColumn46, this->dataGridViewTextBoxColumn47, 
				this->dataGridViewTextBoxColumn48, this->dataGridViewTextBoxColumn49, this->dataGridViewTextBoxColumn50, this->dataGridViewTextBoxColumn51});
			this->dataGridView_SHFE->ImeMode = System::Windows::Forms::ImeMode::Disable;
			this->dataGridView_SHFE->Location = System::Drawing::Point(3, 3);
			this->dataGridView_SHFE->Name = L"dataGridView_SHFE";
			this->dataGridView_SHFE->RowHeadersVisible = false;
			this->dataGridView_SHFE->RowTemplate->Height = 23;
			this->dataGridView_SHFE->Size = System::Drawing::Size(891, 410);
			this->dataGridView_SHFE->TabIndex = 2;
			this->dataGridView_SHFE->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &CommissionMarginConfirmForm::dataGridView_SHFE_CellEndEdit);
			// 
			// dataGridViewTextBoxColumn39
			// 
			this->dataGridViewTextBoxColumn39->HeaderText = L"交易委托账号";
			this->dataGridViewTextBoxColumn39->Name = L"dataGridViewTextBoxColumn39";
			this->dataGridViewTextBoxColumn39->Width = 72;
			// 
			// dataGridViewTextBoxColumn40
			// 
			this->dataGridViewTextBoxColumn40->HeaderText = L"经纪公司";
			this->dataGridViewTextBoxColumn40->Name = L"dataGridViewTextBoxColumn40";
			this->dataGridViewTextBoxColumn40->Width = 61;
			// 
			// dataGridViewTextBoxColumn41
			// 
			this->dataGridViewTextBoxColumn41->HeaderText = L"合约";
			this->dataGridViewTextBoxColumn41->Name = L"dataGridViewTextBoxColumn41";
			this->dataGridViewTextBoxColumn41->Width = 51;
			// 
			// dataGridViewTextBoxColumn42
			// 
			this->dataGridViewTextBoxColumn42->HeaderText = L"多头保证金率";
			this->dataGridViewTextBoxColumn42->Name = L"dataGridViewTextBoxColumn42";
			this->dataGridViewTextBoxColumn42->Width = 72;
			// 
			// dataGridViewTextBoxColumn43
			// 
			this->dataGridViewTextBoxColumn43->HeaderText = L"多头保证金费";
			this->dataGridViewTextBoxColumn43->Name = L"dataGridViewTextBoxColumn43";
			this->dataGridViewTextBoxColumn43->Width = 72;
			// 
			// dataGridViewTextBoxColumn44
			// 
			this->dataGridViewTextBoxColumn44->HeaderText = L"空头保证金率";
			this->dataGridViewTextBoxColumn44->Name = L"dataGridViewTextBoxColumn44";
			this->dataGridViewTextBoxColumn44->Width = 72;
			// 
			// dataGridViewTextBoxColumn45
			// 
			this->dataGridViewTextBoxColumn45->HeaderText = L"空头保证金费";
			this->dataGridViewTextBoxColumn45->Name = L"dataGridViewTextBoxColumn45";
			this->dataGridViewTextBoxColumn45->Width = 72;
			// 
			// dataGridViewTextBoxColumn46
			// 
			this->dataGridViewTextBoxColumn46->HeaderText = L"开仓手续费率";
			this->dataGridViewTextBoxColumn46->Name = L"dataGridViewTextBoxColumn46";
			this->dataGridViewTextBoxColumn46->Width = 72;
			// 
			// dataGridViewTextBoxColumn47
			// 
			this->dataGridViewTextBoxColumn47->HeaderText = L"开仓手续费";
			this->dataGridViewTextBoxColumn47->Name = L"dataGridViewTextBoxColumn47";
			this->dataGridViewTextBoxColumn47->Width = 72;
			// 
			// dataGridViewTextBoxColumn48
			// 
			this->dataGridViewTextBoxColumn48->HeaderText = L"平仓手续费率";
			this->dataGridViewTextBoxColumn48->Name = L"dataGridViewTextBoxColumn48";
			this->dataGridViewTextBoxColumn48->Width = 72;
			// 
			// dataGridViewTextBoxColumn49
			// 
			this->dataGridViewTextBoxColumn49->HeaderText = L"平仓手续费";
			this->dataGridViewTextBoxColumn49->Name = L"dataGridViewTextBoxColumn49";
			this->dataGridViewTextBoxColumn49->Width = 72;
			// 
			// dataGridViewTextBoxColumn50
			// 
			this->dataGridViewTextBoxColumn50->HeaderText = L"平今手续费率";
			this->dataGridViewTextBoxColumn50->Name = L"dataGridViewTextBoxColumn50";
			this->dataGridViewTextBoxColumn50->Width = 72;
			// 
			// dataGridViewTextBoxColumn51
			// 
			this->dataGridViewTextBoxColumn51->HeaderText = L"平今手续费";
			this->dataGridViewTextBoxColumn51->Name = L"dataGridViewTextBoxColumn51";
			this->dataGridViewTextBoxColumn51->Width = 72;
			// 
			// tabPage_CZCE
			// 
			this->tabPage_CZCE->Controls->Add(this->dataGridView_CZCE);
			this->tabPage_CZCE->Location = System::Drawing::Point(4, 21);
			this->tabPage_CZCE->Name = L"tabPage_CZCE";
			this->tabPage_CZCE->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_CZCE->Size = System::Drawing::Size(897, 416);
			this->tabPage_CZCE->TabIndex = 1;
			this->tabPage_CZCE->Text = L"郑商所";
			this->tabPage_CZCE->UseVisualStyleBackColor = true;
			// 
			// dataGridView_CZCE
			// 
			this->dataGridView_CZCE->AllowUserToAddRows = false;
			this->dataGridView_CZCE->AllowUserToDeleteRows = false;
			this->dataGridView_CZCE->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView_CZCE->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView_CZCE->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(13) {this->dataGridViewTextBoxColumn26, 
				this->dataGridViewTextBoxColumn27, this->dataGridViewTextBoxColumn28, this->dataGridViewTextBoxColumn29, this->dataGridViewTextBoxColumn30, 
				this->dataGridViewTextBoxColumn31, this->dataGridViewTextBoxColumn32, this->dataGridViewTextBoxColumn33, this->dataGridViewTextBoxColumn34, 
				this->dataGridViewTextBoxColumn35, this->dataGridViewTextBoxColumn36, this->dataGridViewTextBoxColumn37, this->dataGridViewTextBoxColumn38});
			this->dataGridView_CZCE->ImeMode = System::Windows::Forms::ImeMode::Disable;
			this->dataGridView_CZCE->Location = System::Drawing::Point(3, 3);
			this->dataGridView_CZCE->Name = L"dataGridView_CZCE";
			this->dataGridView_CZCE->RowHeadersVisible = false;
			this->dataGridView_CZCE->RowTemplate->Height = 23;
			this->dataGridView_CZCE->Size = System::Drawing::Size(891, 410);
			this->dataGridView_CZCE->TabIndex = 3;
			this->dataGridView_CZCE->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &CommissionMarginConfirmForm::dataGridView_CZCE_CellEndEdit);
			// 
			// dataGridViewTextBoxColumn26
			// 
			this->dataGridViewTextBoxColumn26->HeaderText = L"交易委托账号";
			this->dataGridViewTextBoxColumn26->Name = L"dataGridViewTextBoxColumn26";
			// 
			// dataGridViewTextBoxColumn27
			// 
			this->dataGridViewTextBoxColumn27->HeaderText = L"经纪公司";
			this->dataGridViewTextBoxColumn27->Name = L"dataGridViewTextBoxColumn27";
			// 
			// dataGridViewTextBoxColumn28
			// 
			this->dataGridViewTextBoxColumn28->HeaderText = L"合约";
			this->dataGridViewTextBoxColumn28->Name = L"dataGridViewTextBoxColumn28";
			// 
			// dataGridViewTextBoxColumn29
			// 
			this->dataGridViewTextBoxColumn29->HeaderText = L"多头保证金率";
			this->dataGridViewTextBoxColumn29->Name = L"dataGridViewTextBoxColumn29";
			// 
			// dataGridViewTextBoxColumn30
			// 
			this->dataGridViewTextBoxColumn30->HeaderText = L"多头保证金费";
			this->dataGridViewTextBoxColumn30->Name = L"dataGridViewTextBoxColumn30";
			// 
			// dataGridViewTextBoxColumn31
			// 
			this->dataGridViewTextBoxColumn31->HeaderText = L"空头保证金率";
			this->dataGridViewTextBoxColumn31->Name = L"dataGridViewTextBoxColumn31";
			// 
			// dataGridViewTextBoxColumn32
			// 
			this->dataGridViewTextBoxColumn32->HeaderText = L"空头保证金费";
			this->dataGridViewTextBoxColumn32->Name = L"dataGridViewTextBoxColumn32";
			// 
			// dataGridViewTextBoxColumn33
			// 
			this->dataGridViewTextBoxColumn33->HeaderText = L"开仓手续费率";
			this->dataGridViewTextBoxColumn33->Name = L"dataGridViewTextBoxColumn33";
			// 
			// dataGridViewTextBoxColumn34
			// 
			this->dataGridViewTextBoxColumn34->HeaderText = L"开仓手续费";
			this->dataGridViewTextBoxColumn34->Name = L"dataGridViewTextBoxColumn34";
			// 
			// dataGridViewTextBoxColumn35
			// 
			this->dataGridViewTextBoxColumn35->HeaderText = L"平仓手续费率";
			this->dataGridViewTextBoxColumn35->Name = L"dataGridViewTextBoxColumn35";
			// 
			// dataGridViewTextBoxColumn36
			// 
			this->dataGridViewTextBoxColumn36->HeaderText = L"平仓手续费";
			this->dataGridViewTextBoxColumn36->Name = L"dataGridViewTextBoxColumn36";
			// 
			// dataGridViewTextBoxColumn37
			// 
			this->dataGridViewTextBoxColumn37->HeaderText = L"平今手续费率";
			this->dataGridViewTextBoxColumn37->Name = L"dataGridViewTextBoxColumn37";
			// 
			// dataGridViewTextBoxColumn38
			// 
			this->dataGridViewTextBoxColumn38->HeaderText = L"平今手续费";
			this->dataGridViewTextBoxColumn38->Name = L"dataGridViewTextBoxColumn38";
			// 
			// tabPage_DCE
			// 
			this->tabPage_DCE->Controls->Add(this->dataGridView_DCE);
			this->tabPage_DCE->Location = System::Drawing::Point(4, 21);
			this->tabPage_DCE->Name = L"tabPage_DCE";
			this->tabPage_DCE->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_DCE->Size = System::Drawing::Size(897, 416);
			this->tabPage_DCE->TabIndex = 2;
			this->tabPage_DCE->Text = L"大商所";
			this->tabPage_DCE->UseVisualStyleBackColor = true;
			// 
			// dataGridView_DCE
			// 
			this->dataGridView_DCE->AllowUserToAddRows = false;
			this->dataGridView_DCE->AllowUserToDeleteRows = false;
			this->dataGridView_DCE->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView_DCE->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView_DCE->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(13) {this->dataGridViewTextBoxColumn1, 
				this->dataGridViewTextBoxColumn2, this->dataGridViewTextBoxColumn3, this->dataGridViewTextBoxColumn4, this->dataGridViewTextBoxColumn5, 
				this->dataGridViewTextBoxColumn6, this->dataGridViewTextBoxColumn7, this->dataGridViewTextBoxColumn8, this->dataGridViewTextBoxColumn9, 
				this->dataGridViewTextBoxColumn10, this->dataGridViewTextBoxColumn11, this->dataGridViewTextBoxColumn12, this->dataGridViewTextBoxColumn25});
			this->dataGridView_DCE->ImeMode = System::Windows::Forms::ImeMode::Disable;
			this->dataGridView_DCE->Location = System::Drawing::Point(3, 3);
			this->dataGridView_DCE->Name = L"dataGridView_DCE";
			this->dataGridView_DCE->RowHeadersVisible = false;
			this->dataGridView_DCE->RowTemplate->Height = 23;
			this->dataGridView_DCE->Size = System::Drawing::Size(891, 410);
			this->dataGridView_DCE->TabIndex = 3;
			this->dataGridView_DCE->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &CommissionMarginConfirmForm::dataGridView_DCE_CellEndEdit);
			// 
			// dataGridViewTextBoxColumn1
			// 
			this->dataGridViewTextBoxColumn1->HeaderText = L"交易委托账号";
			this->dataGridViewTextBoxColumn1->Name = L"dataGridViewTextBoxColumn1";
			// 
			// dataGridViewTextBoxColumn2
			// 
			this->dataGridViewTextBoxColumn2->HeaderText = L"经纪公司";
			this->dataGridViewTextBoxColumn2->Name = L"dataGridViewTextBoxColumn2";
			// 
			// dataGridViewTextBoxColumn3
			// 
			this->dataGridViewTextBoxColumn3->HeaderText = L"合约";
			this->dataGridViewTextBoxColumn3->Name = L"dataGridViewTextBoxColumn3";
			// 
			// dataGridViewTextBoxColumn4
			// 
			this->dataGridViewTextBoxColumn4->HeaderText = L"多头保证金率";
			this->dataGridViewTextBoxColumn4->Name = L"dataGridViewTextBoxColumn4";
			// 
			// dataGridViewTextBoxColumn5
			// 
			this->dataGridViewTextBoxColumn5->HeaderText = L"多头保证金费";
			this->dataGridViewTextBoxColumn5->Name = L"dataGridViewTextBoxColumn5";
			// 
			// dataGridViewTextBoxColumn6
			// 
			this->dataGridViewTextBoxColumn6->HeaderText = L"空头保证金率";
			this->dataGridViewTextBoxColumn6->Name = L"dataGridViewTextBoxColumn6";
			// 
			// dataGridViewTextBoxColumn7
			// 
			this->dataGridViewTextBoxColumn7->HeaderText = L"空头保证金费";
			this->dataGridViewTextBoxColumn7->Name = L"dataGridViewTextBoxColumn7";
			// 
			// dataGridViewTextBoxColumn8
			// 
			this->dataGridViewTextBoxColumn8->HeaderText = L"开仓手续费率";
			this->dataGridViewTextBoxColumn8->Name = L"dataGridViewTextBoxColumn8";
			// 
			// dataGridViewTextBoxColumn9
			// 
			this->dataGridViewTextBoxColumn9->HeaderText = L"开仓手续费";
			this->dataGridViewTextBoxColumn9->Name = L"dataGridViewTextBoxColumn9";
			// 
			// dataGridViewTextBoxColumn10
			// 
			this->dataGridViewTextBoxColumn10->HeaderText = L"平仓手续费率";
			this->dataGridViewTextBoxColumn10->Name = L"dataGridViewTextBoxColumn10";
			// 
			// dataGridViewTextBoxColumn11
			// 
			this->dataGridViewTextBoxColumn11->HeaderText = L"平仓手续费";
			this->dataGridViewTextBoxColumn11->Name = L"dataGridViewTextBoxColumn11";
			// 
			// dataGridViewTextBoxColumn12
			// 
			this->dataGridViewTextBoxColumn12->HeaderText = L"平今手续费率";
			this->dataGridViewTextBoxColumn12->Name = L"dataGridViewTextBoxColumn12";
			// 
			// dataGridViewTextBoxColumn25
			// 
			this->dataGridViewTextBoxColumn25->HeaderText = L"平今手续费";
			this->dataGridViewTextBoxColumn25->Name = L"dataGridViewTextBoxColumn25";
			// 
			// tabPage_CFFEX
			// 
			this->tabPage_CFFEX->Controls->Add(this->dataGridView_CFFEX);
			this->tabPage_CFFEX->Location = System::Drawing::Point(4, 21);
			this->tabPage_CFFEX->Name = L"tabPage_CFFEX";
			this->tabPage_CFFEX->Size = System::Drawing::Size(897, 416);
			this->tabPage_CFFEX->TabIndex = 3;
			this->tabPage_CFFEX->Text = L"中金所";
			this->tabPage_CFFEX->UseVisualStyleBackColor = true;
			// 
			// dataGridView_CFFEX
			// 
			this->dataGridView_CFFEX->AllowUserToAddRows = false;
			this->dataGridView_CFFEX->AllowUserToDeleteRows = false;
			this->dataGridView_CFFEX->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView_CFFEX->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView_CFFEX->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(13) {this->dataGridViewTextBoxColumn13, 
				this->dataGridViewTextBoxColumn14, this->dataGridViewTextBoxColumn15, this->dataGridViewTextBoxColumn16, this->dataGridViewTextBoxColumn17, 
				this->dataGridViewTextBoxColumn18, this->dataGridViewTextBoxColumn19, this->dataGridViewTextBoxColumn20, this->dataGridViewTextBoxColumn21, 
				this->dataGridViewTextBoxColumn22, this->dataGridViewTextBoxColumn23, this->dataGridViewTextBoxColumn24, this->dataGridViewTextBoxColumn52});
			this->dataGridView_CFFEX->ImeMode = System::Windows::Forms::ImeMode::Disable;
			this->dataGridView_CFFEX->Location = System::Drawing::Point(0, 0);
			this->dataGridView_CFFEX->Name = L"dataGridView_CFFEX";
			this->dataGridView_CFFEX->RowHeadersVisible = false;
			this->dataGridView_CFFEX->RowTemplate->Height = 23;
			this->dataGridView_CFFEX->Size = System::Drawing::Size(897, 416);
			this->dataGridView_CFFEX->TabIndex = 3;
			this->dataGridView_CFFEX->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &CommissionMarginConfirmForm::dataGridView_CFFEX_CellEndEdit);
			// 
			// dataGridViewTextBoxColumn13
			// 
			this->dataGridViewTextBoxColumn13->HeaderText = L"交易委托账号";
			this->dataGridViewTextBoxColumn13->Name = L"dataGridViewTextBoxColumn13";
			// 
			// dataGridViewTextBoxColumn14
			// 
			this->dataGridViewTextBoxColumn14->HeaderText = L"经纪公司";
			this->dataGridViewTextBoxColumn14->Name = L"dataGridViewTextBoxColumn14";
			// 
			// dataGridViewTextBoxColumn15
			// 
			this->dataGridViewTextBoxColumn15->HeaderText = L"合约";
			this->dataGridViewTextBoxColumn15->Name = L"dataGridViewTextBoxColumn15";
			// 
			// dataGridViewTextBoxColumn16
			// 
			this->dataGridViewTextBoxColumn16->HeaderText = L"多头保证金率";
			this->dataGridViewTextBoxColumn16->Name = L"dataGridViewTextBoxColumn16";
			// 
			// dataGridViewTextBoxColumn17
			// 
			this->dataGridViewTextBoxColumn17->HeaderText = L"多头保证金费";
			this->dataGridViewTextBoxColumn17->Name = L"dataGridViewTextBoxColumn17";
			// 
			// dataGridViewTextBoxColumn18
			// 
			this->dataGridViewTextBoxColumn18->HeaderText = L"空头保证金率";
			this->dataGridViewTextBoxColumn18->Name = L"dataGridViewTextBoxColumn18";
			// 
			// dataGridViewTextBoxColumn19
			// 
			this->dataGridViewTextBoxColumn19->HeaderText = L"空头保证金费";
			this->dataGridViewTextBoxColumn19->Name = L"dataGridViewTextBoxColumn19";
			// 
			// dataGridViewTextBoxColumn20
			// 
			this->dataGridViewTextBoxColumn20->HeaderText = L"开仓手续费率";
			this->dataGridViewTextBoxColumn20->Name = L"dataGridViewTextBoxColumn20";
			// 
			// dataGridViewTextBoxColumn21
			// 
			this->dataGridViewTextBoxColumn21->HeaderText = L"开仓手续费";
			this->dataGridViewTextBoxColumn21->Name = L"dataGridViewTextBoxColumn21";
			// 
			// dataGridViewTextBoxColumn22
			// 
			this->dataGridViewTextBoxColumn22->HeaderText = L"平仓手续费率";
			this->dataGridViewTextBoxColumn22->Name = L"dataGridViewTextBoxColumn22";
			// 
			// dataGridViewTextBoxColumn23
			// 
			this->dataGridViewTextBoxColumn23->HeaderText = L"平仓手续费";
			this->dataGridViewTextBoxColumn23->Name = L"dataGridViewTextBoxColumn23";
			// 
			// dataGridViewTextBoxColumn24
			// 
			this->dataGridViewTextBoxColumn24->HeaderText = L"平今手续费率";
			this->dataGridViewTextBoxColumn24->Name = L"dataGridViewTextBoxColumn24";
			// 
			// dataGridViewTextBoxColumn52
			// 
			this->dataGridViewTextBoxColumn52->HeaderText = L"平今手续费";
			this->dataGridViewTextBoxColumn52->Name = L"dataGridViewTextBoxColumn52";
			// 
			// button_confirm
			// 
			this->button_confirm->Location = System::Drawing::Point(323, 543);
			this->button_confirm->Name = L"button_confirm";
			this->button_confirm->Size = System::Drawing::Size(75, 23);
			this->button_confirm->TabIndex = 9;
			this->button_confirm->Text = L"确认提交";
			this->button_confirm->UseVisualStyleBackColor = true;
			this->button_confirm->Click += gcnew System::EventHandler(this, &CommissionMarginConfirmForm::button_confirm_Click);
			// 
			// button_apply
			// 
			this->button_apply->Location = System::Drawing::Point(545, 543);
			this->button_apply->Name = L"button_apply";
			this->button_apply->Size = System::Drawing::Size(75, 23);
			this->button_apply->TabIndex = 10;
			this->button_apply->Text = L"应用";
			this->button_apply->UseVisualStyleBackColor = true;
			this->button_apply->Click += gcnew System::EventHandler(this, &CommissionMarginConfirmForm::button_apply_Click);
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(681, 542);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(75, 23);
			this->button_cancel->TabIndex = 11;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &CommissionMarginConfirmForm::button_cancel_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(30, 26);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 12;
			this->label2->Text = L"经纪公司";
			// 
			// comboBox2
			// 
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"全部"});
			this->comboBox2->Location = System::Drawing::Point(105, 18);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(121, 20);
			this->comboBox2->TabIndex = 13;
			this->comboBox2->SelectedIndexChanged += gcnew System::EventHandler(this, &CommissionMarginConfirmForm::comboBox2_SelectedIndexChanged);
			// 
			// labelprompting
			// 
			this->labelprompting->AutoSize = true;
			this->labelprompting->Location = System::Drawing::Point(640, 25);
			this->labelprompting->Name = L"labelprompting";
			this->labelprompting->Size = System::Drawing::Size(41, 12);
			this->labelprompting->TabIndex = 14;
			this->labelprompting->Text = L"label3";
			// 
			// button_LoadHistoryCommission
			// 
			this->button_LoadHistoryCommission->Location = System::Drawing::Point(804, 64);
			this->button_LoadHistoryCommission->Name = L"button_LoadHistoryCommission";
			this->button_LoadHistoryCommission->Size = System::Drawing::Size(126, 23);
			this->button_LoadHistoryCommission->TabIndex = 15;
			this->button_LoadHistoryCommission->Text = L"导入历史费率";
			this->button_LoadHistoryCommission->UseVisualStyleBackColor = true;
			this->button_LoadHistoryCommission->Click += gcnew System::EventHandler(this, &CommissionMarginConfirmForm::button_LoadHistoryCommission_Click);
			// 
			// CommissionMarginConfirmForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(963, 590);
			this->Controls->Add(this->button_LoadHistoryCommission);
			this->Controls->Add(this->labelprompting);
			this->Controls->Add(this->comboBox2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->button_apply);
			this->Controls->Add(this->button_confirm);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label1);
			this->Name = L"CommissionMarginConfirmForm";
			this->Text = L"费率确认";
			this->Load += gcnew System::EventHandler(this, &CommissionMarginConfirmForm::CommissionMarginConfirmForm_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &CommissionMarginConfirmForm::CommissionMarginConfirmForm_FormClosed);
			this->tabControl1->ResumeLayout(false);
			this->tabPage_SHFE->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_SHFE))->EndInit();
			this->tabPage_CZCE->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_CZCE))->EndInit();
			this->tabPage_DCE->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_DCE))->EndInit();
			this->tabPage_CFFEX->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_CFFEX))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void CommissionMarginConfirmForm_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent(); 
				 int nHwnd = this->Handle.ToInt32();
				 //CTcpLayer::SetMainWndHandle(nHwnd);

				 m_pDataInfo->GetLogonInfo(*m_pLogin);
				 PlatformStru_QryInstrument Field;
				 CTcpLayer::SubscribePkg(Cmd_TradeDataManager_QryInstrument_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_QueryTradeAccount_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_QueryBrokerInfo_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_QueryCommissionMarginRate_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_ModifyCommissionMarginRate_Rsp, nHwnd);
				 if ( !CTcpLayer::SendData(Cmd_QueryTradeAccount_Req, NULL, 0, 0, m_pLogin->mnUserID)
					 || (!CTcpLayer::SendData(Cmd_Settlement_QueryCommissionMarginRate_Req, NULL, 0, 0, m_pLogin->mnUserID))
					 || (!CTcpLayer::SendData(Cmd_TradeDataManager_QryInstrument_Req, &Field, sizeof(PlatformStru_QryInstrument), 0, m_pLogin->mnUserID))
					 || (!CTcpLayer::SendData(Cmd_QueryBrokerInfo_Req, NULL, 0, 0, m_pLogin->mnUserID)))
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }
				 dataGridView_SHFE->Columns[0]->ReadOnly = true;
				 dataGridView_SHFE->Columns[1]->ReadOnly = true;
				 dataGridView_SHFE->Columns[2]->ReadOnly = true;
				 dataGridView_CZCE->Columns[0]->ReadOnly = true;
				 dataGridView_CZCE->Columns[1]->ReadOnly = true;
				 dataGridView_CZCE->Columns[2]->ReadOnly = true;
				 dataGridView_DCE->Columns[0]->ReadOnly = true;
				 dataGridView_DCE->Columns[1]->ReadOnly = true;
				 dataGridView_DCE->Columns[2]->ReadOnly = true;
				 dataGridView_CFFEX->Columns[0]->ReadOnly = true;
				 dataGridView_CFFEX->Columns[1]->ReadOnly = true;
				 dataGridView_CFFEX->Columns[2]->ReadOnly = true;

				   m_pDataInfo->ClearCommissionMarginRate();
				   m_pDataInfo->ClearModifiedRate();
				   labelprompting->Text=L"";

				 
				   
				  
				 
			 }

	protected:virtual void WndProc( Message% m ) override
			  {
				  if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival )
				  {
					  char *p = (char*)m.LParam.ToInt32();
					  Stru_UniPkgHead head;					
					  memset(&head, 0, sizeof(head));
					  memcpy(&head, p, sizeof(head));

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
							  }
							  else
							  {
								  string str = (char*)(p+sizeof(Stru_UniPkgHead));
								 // String^ strtemp = Tools::string2String(str.c_str());
									_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
											CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
											str.c_str());
							  }

							  break;
						  }
					  case Cmd_QueryBrokerInfo_Rsp:
						  {
							  if ( head.userdata1 == CF_ERROR_SUCCESS )
							  {
								  comboBox2->Items->Clear();
								  comboBox2->Items->Add(Tools::string2String(SELECT_ALL));

								  std::map<int, BrokerInfo> mapBrokerInfo;
								  int nCount = head.len / sizeof(BrokerInfo);
								  if(nCount == 0)
									  break;
								  for ( int i = 0; i < nCount; i++ )
								  {
									  BrokerInfo* pBrokerInfo = (BrokerInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(BrokerInfo));
									  mapBrokerInfo[pBrokerInfo->nBrokerID] = *pBrokerInfo;

									  /*char strBroker[32];
									  memset(strBroker,0,strlen(strBroker)+1);
									  sprintf(strBroker, "%d",pBrokerInfo->nBrokerID);*/
									  comboBox2->Items->Add(Tools::string2String(pBrokerInfo->szBrokerCode));
								  }
								  m_pDataInfo->SetBrokerInfos(mapBrokerInfo);
						
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
								  comboBox1->Items->Clear();
								  comboBox1->Items->Add(Tools::string2String(SELECT_ALL));

								  std::map<int, TradeAccount> mapTradeAccount;
								  int nCount = head.len / sizeof(TradeAccount);
								  if(nCount == 0)
									  break;
								  for ( int i = 0; i < nCount; i++ )
								  {
									  TradeAccount* pTradeAccount = (TradeAccount*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(TradeAccount));
									  mapTradeAccount[pTradeAccount->nTradeAccountID] = *pTradeAccount;
									  
									 comboBox1->Items->Add(Tools::string2String(pTradeAccount->szTradeAccount));
	
									  
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
					  case Cmd_Settlement_QueryCommissionMarginRate_Rsp:
						  {
							  if ( head.userdata1 == CF_ERROR_SUCCESS )
							  {

								m_pDataInfo->ClearCommissionMarginRate();
								m_pDataInfo->ClearModifiedRate();
								  std::map<RateKey,CommissionMarginRate>  lMapFiled;
								  int nCount = head.len / sizeof(CommissionMarginRate);
								  if(nCount == 0)
									  break;
								  for ( int i = 0; i < nCount; i++ )
								  {
									  CommissionMarginRate* pField = (CommissionMarginRate*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(CommissionMarginRate));
									  RateKey key(pField->AccountID,pField->InstrumentID,pField->BrokerID);
									  lMapFiled[key] = *pField;
								  }
								  m_pDataInfo->SetCommissionMarginRate(lMapFiled);
								  // 默认显示出上期所费率

								  string strAccountID = SELECT_ALL;
								  string strBrokerID = SELECT_ALL;
								  std::vector<CommissionMarginRate> vecRate;
								  vecRate.clear();
								  m_pDataInfo->GetCommissionMarginRate(vecRate,0,strAccountID,strBrokerID);
								  LoadCommissionMarginRate(dataGridView_SHFE,vecRate );

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
					  case Cmd_Settlement_ModifyCommissionMarginRate_Rsp:
						  {
							  if ( head.userdata1 == CF_ERROR_SUCCESS )
							  {

								 m_pDataInfo->ModifyCommissionMarginRate();			
								 m_pDataInfo->ClearModifiedRate();

								 if (bConfirmOrApply)
								 {
									 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
									 this->Close();
								 }
								

							  }
							  else
							  {
								  string str = (char*)(p+sizeof(Stru_UniPkgHead));
								  String^ strtemp = Tools::string2String(str.c_str());
									_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
											CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
											str.c_str());
							  }
							  labelprompting->Text=L"";
							  button_confirm->Enabled=true;
							  button_apply->Enabled =true;
							  button_cancel->Enabled =true;

							  break;
						  }

					  default:
						  break;
					  }

				  }

				  Form::WndProc ( m );
			  }

private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
//private: System::Void comboBox1_DropDown(System::Object^  sender, System::EventArgs^  e) 
//		 {
//			 std::map<int, TradeAccount> mapTradeAccount;
//			 m_pDataInfo->GetTradeAccount(mapTradeAccount);
//		 }
private: string Double2String(double dbData)
		 {
			 char strPriceText[512]={0};
			 memset(strPriceText, 0,sizeof(strPriceText));
			 strcpy(strPriceText, "-");
			 if(util::isInvalidValue(dbData)) 
				 return strPriceText;
			 sprintf(strPriceText, "%f", dbData);
			 for(int i=strlen(strPriceText)-1; i>=0; i--) 
			 {
				 if(strPriceText[i]=='0')
					 strPriceText[i]=0;
				 else
				 {
					 if(strPriceText[i]=='.')
						 strPriceText[i]=0;
					 break;
				 }
			 }
			 return strPriceText;
		 }
		 //获取货币的分段显示格式
private: static std::string GetAccountFormatString(double dblAmount,int precision)
		 {
			 char szBuf[256]={0};
			 char szFormat[16]={0};
			 int i,Len,ipt;

			 if(precision<0&&precision!=-1) precision=2;

			 if(precision==-1)
			 {
				 _snprintf(szBuf,255,"%f",dblAmount);
				 szBuf[sizeof(szBuf)-1]=0;
				 Len=strlen(szBuf);
				 for(i=0;i<Len&&szBuf[i]!='.';i++);
				 if(i<Len)
				 {
					 //过滤分数部分无效的零
					 while(Len>0&&szBuf[Len-1]=='0') 
					 {
						 szBuf[Len-1]=0;
						 Len--;
					 }
				 }
				 Len=strlen(szBuf);
				 if(Len>0&&szBuf[Len-1]=='.')
					 szBuf[Len-1]=0;
			 }
			 else
			 {
				 _snprintf(szFormat,15,"%%.%df",precision);
				 _snprintf(szBuf,255,szFormat,dblAmount);
				 szBuf[sizeof(szBuf)-1]=0;
			 }

			 Len=strlen(szBuf);
			 for(ipt=0;ipt<Len&&szBuf[ipt]!='.';ipt++);
			 while(ipt>3&&
				 szBuf[ipt-3-1]>='0'&&szBuf[ipt-3-1]<='9'&&
				 Len<sizeof(szBuf)-10)
			 {
				 i=ipt-3;
				 memmove(szBuf+i+1,szBuf+i,Len-i+1);
				 szBuf[i]=',';
				 Len++;
				 ipt-=3;
			 }
			 return std::string(szBuf);
		 }
 protected: System::Void LoadCommissionMarginRate(DataGridView^ dataGridView,std::vector<CommissionMarginRate>& lvecField )
			{
				dataGridView->Rows->Clear();
				/*if (lvecField.size() == 0)
				{
					_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
						CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
						"没有查询到费率");
				}*/
				std::vector<CommissionMarginRate>::iterator it;
				for(it = lvecField.begin(); it != lvecField.end(); it++)
				{

					int newRowIndex=dataGridView->Rows->Add();
					DataGridViewRow^ dr = dataGridView->Rows[newRowIndex];
					dr->Cells[0]->Value = Tools::string2String((*it).AccountID);
					dr->Cells[1]->Value = Tools::string2String((*it).BrokerID);
					dr->Cells[2]->Value = Tools::string2String((*it).InstrumentID);

					std::string strPrice="";
					if ((*it).LongMarginRatioByMoney != InvalideValue && (*it).LongMarginRatioByMoney >= 0)
					{
						strPrice = Double2String((*it).LongMarginRatioByMoney*100)+"%"; 
						dr->Cells[3]->Value = Tools::string2String(strPrice.c_str());
					}
					if ((*it).LongMarginRatioByVolume != InvalideValue && (*it).LongMarginRatioByVolume >= 0)
					{
						strPrice = GetAccountFormatString((*it).LongMarginRatioByVolume,-1);
						dr->Cells[4]->Value = Tools::string2String(strPrice.c_str());
					}
					if ((*it).ShortMarginRatioByMoney != InvalideValue && (*it).ShortMarginRatioByMoney >= 0)
					{
						strPrice = Double2String((*it).ShortMarginRatioByMoney*100)+"%";
						dr->Cells[5]->Value = Tools::string2String(strPrice.c_str());
					}
					if ((*it).ShortMarginRatioByVolume != InvalideValue && (*it).ShortMarginRatioByVolume >= 0)
					{
						strPrice = GetAccountFormatString((*it).ShortMarginRatioByVolume,-1);
						dr->Cells[6]->Value = Tools::string2String(strPrice.c_str());
					}
					if ((*it).OpenRatioByMoney != InvalideValue && (*it).OpenRatioByMoney >= 0)
					{
						strPrice = Double2String((*it).OpenRatioByMoney*100)+"%";
						dr->Cells[7]->Value = Tools::string2String(strPrice.c_str());
					}
					if ((*it).OpenRatioByVolume != InvalideValue && (*it).OpenRatioByVolume >= 0)
					{
						strPrice = Double2String((*it).OpenRatioByVolume);
						dr->Cells[8]->Value = Tools::string2String(strPrice.c_str());
					}
					if ((*it).CloseRatioByMoney != InvalideValue && (*it).CloseRatioByMoney >= 0)
					{
						strPrice = Double2String((*it).CloseRatioByMoney*100)+"%";
						dr->Cells[9]->Value = Tools::string2String(strPrice.c_str());
					}
					if ((*it).CloseRatioByVolume != InvalideValue && (*it).CloseRatioByVolume >= 0)
					{
						strPrice = Double2String((*it).CloseRatioByVolume);
						dr->Cells[10]->Value = Tools::string2String(strPrice.c_str());
					}
					if ((*it).CloseTodayRatioByMoney != InvalideValue && (*it).CloseTodayRatioByMoney >= 0)
					{
						strPrice = Double2String((*it).CloseTodayRatioByMoney*100)+"%";
						dr->Cells[11]->Value = Tools::string2String(strPrice.c_str());
					}
					if ((*it).CloseTodayRatioByVolume != InvalideValue && (*it).CloseTodayRatioByVolume >= 0)
					{
						strPrice = Double2String((*it).CloseTodayRatioByVolume);
						dr->Cells[12]->Value = Tools::string2String(strPrice.c_str());		
					}

				}
			}
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 string strAccountID = Tools::String2string(comboBox1->Text);
			 string strBrokerID = Tools::String2string(comboBox2->Text);
			 if ( strAccountID =="" )
			 {
				 strAccountID = SELECT_ALL;
			 }
			 if ( strBrokerID =="" )
			 {
				  strBrokerID = SELECT_ALL ;
			 }

			 std::vector<CommissionMarginRate> vecRate;
			 vecRate.clear();
			 m_pDataInfo->GetCommissionMarginRate(vecRate,tabControl1->SelectedIndex,strAccountID,strBrokerID);
			 switch(tabControl1->SelectedIndex)
			 {
			 case 0:
				 LoadCommissionMarginRate(dataGridView_SHFE,vecRate );
				 break;
			 case 1:
				 LoadCommissionMarginRate(dataGridView_CZCE,vecRate );
				 break;
			 case 2:
				 LoadCommissionMarginRate(dataGridView_DCE,vecRate );
				 break;
			 case 3:
				 LoadCommissionMarginRate(dataGridView_CFFEX,vecRate );
				 break;
			 default:
				 LoadCommissionMarginRate(dataGridView_SHFE,vecRate );
				 break;
			 }							 
			 
			 
		 }
private: System::Void dataGridView3_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
		 }
private: System::Void tabControl1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {

				 string strAccountID = Tools::String2string(comboBox1->Text);
				 string strBrokerID = Tools::String2string(comboBox2->Text);
				 if ( strAccountID =="" )
				 {
					 strAccountID = SELECT_ALL;
				 }
				 if ( strBrokerID =="" )
				 {
					 strBrokerID = SELECT_ALL ;
				 }


				 std::vector<CommissionMarginRate> vecRate;
				 vecRate.clear();
				 m_pDataInfo->GetCommissionMarginRate(vecRate,tabControl1->SelectedIndex,strAccountID,strBrokerID);
				 switch(tabControl1->SelectedIndex)
				 {
				 case 0:
					 LoadCommissionMarginRate(dataGridView_SHFE,vecRate );
					 break;
				 case 1:
					 LoadCommissionMarginRate(dataGridView_CZCE,vecRate );
					 break;
				 case 2:
					 LoadCommissionMarginRate(dataGridView_DCE,vecRate );
					 break;
				 case 3:
					 LoadCommissionMarginRate(dataGridView_CFFEX,vecRate );
					 break;
				 default:
					 LoadCommissionMarginRate(dataGridView_SHFE,vecRate );
					 break;
				 }							 
			
		 }
protected: System::Void SaveModifiedRate(DataGridView^ dataGridView,int rowIndex,int columnIndex)
		   {

			   double dbRate = InvalideValue;
			   CommissionMarginRate Filed;
			   memset(&Filed,0,sizeof(Filed));
			   String^ strAccount = dataGridView->Rows[rowIndex]->Cells[0]->Value->ToString();
			   strncpy(Filed.AccountID,Tools::String2string(strAccount).c_str(),sizeof(Filed.AccountID)-1);
			   String^ strBrokerID = dataGridView->Rows[rowIndex]->Cells[1]->Value->ToString();
			   strncpy(Filed.BrokerID,Tools::String2string(strBrokerID).c_str(),sizeof(Filed.BrokerID)-1);
			   String^ strInstrumentID = dataGridView->Rows[rowIndex]->Cells[2]->Value->ToString();
			   strncpy(Filed.InstrumentID,Tools::String2string(strInstrumentID).c_str(),sizeof(Filed.InstrumentID)-1);
			   if (dataGridView->Rows[rowIndex]->Cells[columnIndex]->FormattedValue == "")
			   {
				  //MessageBox::Show("请输入费率值！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				  // return;
			   }
			   else
			   {
				   String^ strRate = dataGridView->Rows[rowIndex]->Cells[columnIndex]->Value->ToString();
				   double dbRate = atof(Tools::String2string(strRate).c_str());
			   }
			   

			   if ( dbRate < 0 )
			   {
				   MessageBox::Show("请输入正确的费率值！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				   return;
			   }

			   if (dataGridView->Rows[rowIndex]->Cells[3]->Value != nullptr)
			   {
				   Filed.LongMarginRatioByMoney = atof(Tools::String2string(dataGridView->Rows[rowIndex]->Cells[3]->Value->ToString()).c_str());
			   }
			   else
			   {
				   Filed.LongMarginRatioByMoney = InvalideValue;
			   }

			   if (dataGridView->Rows[rowIndex]->Cells[4]->Value != nullptr)
			   {
				   Filed.LongMarginRatioByVolume = atof(Tools::String2string(dataGridView->Rows[rowIndex]->Cells[4]->Value->ToString()).c_str());
			   }
			   else
			   {
				   Filed.LongMarginRatioByVolume = InvalideValue;
			   }

			   if (dataGridView->Rows[rowIndex]->Cells[5]->Value != nullptr)
			   {
				   Filed.ShortMarginRatioByMoney = atof(Tools::String2string(dataGridView->Rows[rowIndex]->Cells[5]->Value->ToString()).c_str());
			   }
			   else
			   {
				   Filed.ShortMarginRatioByMoney = InvalideValue;
			   }

			   if (dataGridView->Rows[rowIndex]->Cells[6]->Value != nullptr)
			   {
				   Filed.ShortMarginRatioByVolume = atof(Tools::String2string(dataGridView->Rows[rowIndex]->Cells[6]->Value->ToString()).c_str());
			   }
			   else
			   {
				   Filed.ShortMarginRatioByVolume = InvalideValue;
			   }

			   if (dataGridView->Rows[rowIndex]->Cells[7]->Value != nullptr)
			   {
				   Filed.OpenRatioByMoney = atof(Tools::String2string(dataGridView->Rows[rowIndex]->Cells[7]->Value->ToString()).c_str());
			   }
			   else
			   {
				   Filed.OpenRatioByMoney = InvalideValue;
			   }

			   if (dataGridView->Rows[rowIndex]->Cells[8]->Value != nullptr)
			   {
				   Filed.OpenRatioByVolume = atof(Tools::String2string(dataGridView->Rows[rowIndex]->Cells[8]->Value->ToString()).c_str());
			   }
			   else
			   {
				   Filed.OpenRatioByVolume = InvalideValue;
			   }

			   if (dataGridView->Rows[rowIndex]->Cells[9]->Value != nullptr)
			   {
				   Filed.CloseRatioByMoney = atof(Tools::String2string(dataGridView->Rows[rowIndex]->Cells[9]->Value->ToString()).c_str());
			   }
			   else
			   {
				   Filed.CloseRatioByMoney = InvalideValue;
			   }

			   if (dataGridView->Rows[rowIndex]->Cells[10]->Value != nullptr)
			   {

				   Filed.CloseRatioByVolume = atof(Tools::String2string(dataGridView->Rows[rowIndex]->Cells[10]->Value->ToString()).c_str());
			   }
			   else
			   {
				   Filed.CloseRatioByVolume = InvalideValue;
			   }

			   if (dataGridView->Rows[rowIndex]->Cells[11]->Value != nullptr)
			   {
				   Filed.CloseTodayRatioByMoney = atof(Tools::String2string(dataGridView->Rows[rowIndex]->Cells[11]->Value->ToString()).c_str());
			   }
			   else
			   {
				   Filed.CloseTodayRatioByMoney = InvalideValue;
			   }

			   if (dataGridView->Rows[rowIndex]->Cells[12]->Value != nullptr)
			   {
				   Filed.CloseTodayRatioByVolume = atof(Tools::String2string(dataGridView->Rows[rowIndex]->Cells[12]->Value->ToString()).c_str());
			   }
			   else
			   {
				   Filed.CloseTodayRatioByVolume = InvalideValue;
			   }




			   CommissionMarginRate OriginalFiled;
			   memset(&OriginalFiled,0,sizeof(OriginalFiled));
			   RateKey key(Filed.AccountID,Filed.InstrumentID,Filed.BrokerID);
			   if( m_pDataInfo->GetCommissionMarginRate(key,OriginalFiled) )
			   {
				   if ( Filed.LongMarginRatioByMoney == OriginalFiled.LongMarginRatioByMoney && Filed.LongMarginRatioByVolume ==OriginalFiled.LongMarginRatioByVolume
					   &&Filed.ShortMarginRatioByMoney==OriginalFiled.ShortMarginRatioByMoney && Filed.ShortMarginRatioByVolume==OriginalFiled.ShortMarginRatioByVolume
					   &&Filed.OpenRatioByMoney==OriginalFiled.OpenRatioByMoney && Filed.ShortMarginRatioByVolume==OriginalFiled.ShortMarginRatioByVolume
					   &&Filed.CloseRatioByMoney==OriginalFiled.CloseRatioByMoney && Filed.CloseRatioByVolume==OriginalFiled.CloseRatioByVolume
					   && Filed.CloseTodayRatioByVolume== OriginalFiled.CloseTodayRatioByVolume && Filed.CloseTodayRatioByVolume==OriginalFiled.CloseTodayRatioByVolume  )

					   return;

			   }


			   m_pDataInfo->SetModifiedRate(&Filed);
		   }
private: System::Void dataGridView_SHFE_CellEndEdit(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
		 {
			 DataGridView^ dataGridView = (DataGridView^)(sender);
			 int rowIndex  =e->RowIndex;
			 int columnIndex  =e->ColumnIndex;
			 SaveModifiedRate(dataGridView,rowIndex,columnIndex);
			


		 }
private: System::Void dataGridView_CZCE_CellEndEdit(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
		 {
			 DataGridView^ dataGridView = (DataGridView^)(sender);
			 int rowIndex  =e->RowIndex;
			 int columnIndex  =e->ColumnIndex;
			 SaveModifiedRate(dataGridView,rowIndex,columnIndex);
		 }
private: System::Void dataGridView_DCE_CellEndEdit(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
		 {
			 DataGridView^ dataGridView = (DataGridView^)(sender);
			 int rowIndex  =e->RowIndex;
			 int columnIndex  =e->ColumnIndex;
			 SaveModifiedRate(dataGridView,rowIndex,columnIndex);
		 }
private: System::Void dataGridView_CFFEX_CellEndEdit(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
		 {
			 DataGridView^ dataGridView = (DataGridView^)(sender);
			 int rowIndex  =e->RowIndex;
			 int columnIndex  =e->ColumnIndex;
			 SaveModifiedRate(dataGridView,rowIndex,columnIndex);
		 }
private: System::Void comboBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 comboBox1->Items->Clear();
			 comboBox1->Items->Add(Tools::string2String(SELECT_ALL));
			 ComboBox^ comboBox = (ComboBox^)(sender);
			 string strbrokerID = (Tools::String2string(comboBox->Text));
			 int nbrokerID  = -1;;
			 if (!(m_pDataInfo->GetBrokerIDByBrokerCode(strbrokerID,nbrokerID)) && strbrokerID != SELECT_ALL)
			 {
				  MessageBox::Show("错误的纪经公司ID", "提示",MessageBoxButtons::OK);
				  return;
			 }
			

			 std::map<int, TradeAccount> mapTradeAccount;
			 m_pDataInfo->GetTradeAccount(mapTradeAccount);
			 std::map<int, TradeAccount>::iterator it = mapTradeAccount.begin();
			 while ( it != mapTradeAccount.end() )
			 {
				 if ( strbrokerID !="" && it->second.nBrokerID == nbrokerID )
				 {
					 comboBox1->Items->Add(Tools::string2String(it->second.szTradeAccount));
				 }
				 if (strbrokerID =="")
				 {
					 comboBox1->Items->Add(Tools::string2String(it->second.szTradeAccount));
				 }

				 it++;
			 }
		 }
private: System::Void button_apply_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 std::map<RateKey,CommissionMarginRate>  lMapFiled;
			  m_pDataInfo->GetModifiedRate(lMapFiled);
			  if (lMapFiled.empty())
			  {
				  return;
			  }

			  std::vector<CommissionMarginRate> lVecFiled;

			  std::map<RateKey,CommissionMarginRate>::iterator lIter = lMapFiled.begin();
			  while(lIter != lMapFiled.end())
			  {
			  lVecFiled.push_back(lIter->second);
			  lIter++;
			  }
			  m_pDataInfo->GetLogonInfo(*m_pLogin);	
			  if ( !CTcpLayer::SendData(Cmd_Settlement_ModifyCommissionMarginRate_Req,&lVecFiled[0],sizeof(CommissionMarginRate)*lVecFiled.size(), 0, m_pLogin->mnUserID))
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }
			  labelprompting->Text=L"修改费率后需重算成交，请稍后";
			  button_confirm->Enabled=false;
			  button_apply->Enabled =false;
			  button_cancel->Enabled =false;

		 }
private: System::Void button_confirm_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 
			 if(m_pDataInfo->CheckRate())
			 {
				 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"是否确定确认费率？", "提示", MessageBoxButtons::YesNo);
				 if ( dr == System::Windows::Forms::DialogResult::Yes)
				 { 
					 std::map<RateKey,CommissionMarginRate>  lMapFiled;
					 m_pDataInfo->GetModifiedRate(lMapFiled);
					 if (!lMapFiled.empty())
					 {
						 std::vector<CommissionMarginRate> lVecFiled;

						 std::map<RateKey,CommissionMarginRate>::iterator lIter = lMapFiled.begin();
						 while(lIter != lMapFiled.end())
						 {
							 lVecFiled.push_back(lIter->second);
							 lIter++;
						 }
						 m_pDataInfo->GetLogonInfo(*m_pLogin);	
						 if ( !CTcpLayer::SendData(Cmd_Settlement_ModifyCommissionMarginRate_Req,&lVecFiled[0],sizeof(CommissionMarginRate)*lVecFiled.size(), 0, m_pLogin->mnUserID))
						 {
							 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
						 }
						 labelprompting->Text=L"修改费率后需重算成交，请稍后";
						 bConfirmOrApply= true;
						/* m_pDataInfo->ModifyCommissionMarginRate();			
						 m_pDataInfo->ClearModifiedRate();*/
						 button_confirm->Enabled=false;
						 button_apply->Enabled =false;
						 button_cancel->Enabled =false;
					 }

					
					 else
					 {
						 std::vector<CommissionMarginRate>  vecRate;
						 m_pDataInfo->GetCommissionMarginRate(vecRate );
						 if ( !CTcpLayer::SendData(Cmd_Settlement_ConfirmCommissionMarginRate_Req,&vecRate[0],sizeof(CommissionMarginRate)*vecRate.size(), 0, m_pLogin->mnUserID))
						 {
							 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
						 }
						 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
						 this->Close();
					 }
					 
				 }

			 }
			 else

				 MessageBox::Show("费率数据不全！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
		 }
private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (m_pDataInfo->HaveModifiedRate())
			 {
				 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"您已做数据修改，但是未保存，是否取消操作并关闭窗口？", "提示", MessageBoxButtons::YesNo);
				 if ( dr == System::Windows::Forms::DialogResult::Yes)
				 { 
					 this->DialogResult = System::Windows::Forms::DialogResult::No;
					 m_pDataInfo->ClearModifiedRate();
					 this->Close();
				 }

			 }
			 else
				 this->Close();

		 }
private: System::Void CommissionMarginConfirmForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
		 {
			 if (m_pDataInfo->HaveModifiedRate())
			 {
				 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"您已做数据修改，但是未保存，是否取消操作并关闭窗口？", "提示", MessageBoxButtons::YesNo);
				 if ( dr == System::Windows::Forms::DialogResult::Yes)
				 { 
					 this->DialogResult = System::Windows::Forms::DialogResult::No;
					 m_pDataInfo->ClearModifiedRate();
					 this->Close();
				 }
				 
			 }
			 else
				 this->Close();

			 IntPtr hWnd=this->Handle;
			 CTcpLayer::UnsubscribeAllPkg((int)hWnd);		 
		 }
private: System::Void button_LoadHistoryCommission_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 LoadHistoryCommission^ dlg = gcnew LoadHistoryCommission();
			 m_pDataInfo->SetQryRate();
			 if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
			 {

				 string strAccountID = Tools::String2string(comboBox1->Text);
				 string strBrokerID = Tools::String2string(comboBox2->Text);
				 if ( strAccountID =="" )
				 {
					 strAccountID = SELECT_ALL;
				 }
				 if ( strBrokerID =="" )
				 {
					 strBrokerID = SELECT_ALL ;
				 }

				 std::vector<CommissionMarginRate> vecRate;
				 vecRate.clear();
				 m_pDataInfo->GetCommissionMarginRate(vecRate,tabControl1->SelectedIndex,strAccountID,strBrokerID);
				 switch(tabControl1->SelectedIndex)
				 {
				 case 0:
					 LoadCommissionMarginRate(dataGridView_SHFE,vecRate );
					 break;
				 case 1:
					 LoadCommissionMarginRate(dataGridView_CZCE,vecRate );
					 break;
				 case 2:
					 LoadCommissionMarginRate(dataGridView_DCE,vecRate );
					 break;
				 case 3:
					 LoadCommissionMarginRate(dataGridView_CFFEX,vecRate );
					 break;
				 default:
					 LoadCommissionMarginRate(dataGridView_SHFE,vecRate );
					 break;
				 }							 

				
			 }
		 }
};
}

#pragma once


#include "TcpLayer.h"
#include "CommonDef.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "CDataInfo.h"
#include "Tools.h"
#include "CInstrumentIDTreeListDlg.h"
#include "CommonMacro.h"
#include "AddTradeForm.h"
#include "PromptForm.h"
#include "FileOpr.h"
#include <algorithm>
#include <functional>
#include <vector>
#include "SettleSort.h"
const int DEFAULT_ROW_NUM_PER_PAGE = 10;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;

static bool (__clrcall*InnerSort0)(const PlatformStru_TradeInfo, const PlatformStru_TradeInfo);
static bool (__clrcall*InnerSort1)(const PlatformStru_TradeInfo, const PlatformStru_TradeInfo);


namespace CltServerMgr {

	/// <summary>
	/// Summary for TradeConfirmForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class TradeConfirmForm : public System::Windows::Forms::Form
	{
	public:
		TradeConfirmForm(String^ tradeDate)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pLogin = new sLoginRsp();
			m_pDataInfo = CDataInfo::NewInstance();
			m_pTradeInfo = new PlatformStru_TradeInfo;
			m_pOriginalTradeInfo = new PlatformStru_TradeInfo;
			m_nRowNumPerPage = DEFAULT_ROW_NUM_PER_PAGE;
			m_nCurShowPage = 1;
			InvalideValue=util::GetDoubleInvalidValue();
			m_TradeDate=tradeDate;
		}
      
		//static bool __clrcall  InnerSort0(const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2) 
		//{			
		//	return m1.ExchangeID < m2.ExchangeID;				 
		//}
		//static  bool __clrcall InnerSort1(const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2) 
		//{			
		//	return m1.TradeID < m2.TradeID;				 
		//}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~TradeConfirmForm()
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
			if ( NULL != m_pTradeInfo )
			{
				delete m_pTradeInfo;
				m_pTradeInfo = NULL;
			}
			if ( NULL != m_pOriginalTradeInfo )
			{
				delete m_pOriginalTradeInfo;
				m_pOriginalTradeInfo = NULL;
			}
		}

	public: String^ 	m_TradeDate;
	private: CInstrumentIDTreeListDlg^ dlgInstrumentIDTreeList;
	private: sLoginRsp*     m_pLogin;
	private: CDataInfo*     m_pDataInfo;
	private: PlatformStru_TradeInfo* m_pTradeInfo; 
	private: PlatformStru_TradeInfo* m_pOriginalTradeInfo;
	private: int            m_nRowNumPerPage;
	private: double         InvalideValue;
	private: int            m_nCurShowPage;
    

	protected: 


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  comboBox_exchange;

	private: System::Windows::Forms::Label^  label2;
	private: AnywndComboBox::AnywndComboBoxControl^  comboBox_Ins;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::ComboBox^  comboBox_buySell;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::ComboBox^  comboBox_Direction;
	private: System::Windows::Forms::Button^  button_query;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::ComboBox^  comboBox_Hedge;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::TextBox^  textBox_TradeID;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::ComboBox^  comboBox_user;


	private: System::Windows::Forms::Button^  button_clear;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::Button^  button_add;
	private: System::Windows::Forms::Button^  button_Modify;
	private: System::Windows::Forms::Button^  button_delete;
	private: System::Windows::Forms::Button^  button_confirm;
	private: System::Windows::Forms::Button^  button_apply;
	private: System::Windows::Forms::Button^  button_cannel;
	private: System::Windows::Forms::Label^  label_count;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::ComboBox^  comboBox2;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::ComboBox^  comboBox3;
	private: System::Windows::Forms::Label^  label10;















	private: System::Windows::Forms::Button^  button_caculate;
	private: System::Windows::Forms::Label^  label12;

private: System::Windows::Forms::Label^  label13;
private: System::Windows::Forms::Button^  button_pre;
private: System::Windows::Forms::Button^  button_next;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column1;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column5;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column6;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column7;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column8;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column9;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column10;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column12;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column13;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column14;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_closeprofit;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_TradeCommission;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_tradedate;






























		 //private: System::Windows::Forms::Label^  label13;
//private: System::Windows::Forms::Button^  button_pre;
//private: System::Windows::Forms::Button^  button_next;
//private: System::Windows::Forms::Label^  label13;
//private: System::Windows::Forms::Button^  button_pre;
//private: System::Windows::Forms::Button^  button_next;
//private: System::Windows::Forms::Label^  label13;
//private: System::Windows::Forms::Button^  button1;
//private: System::Windows::Forms::Button^  button2;
//private: System::Windows::Forms::Label^  label13;
//private: System::Windows::Forms::Button^  button1;
//private: System::Windows::Forms::Label^  label13;





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
			this->comboBox_exchange = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboBox_Ins = (gcnew AnywndComboBox::AnywndComboBoxControl());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->comboBox_buySell = (gcnew System::Windows::Forms::ComboBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->comboBox_Direction = (gcnew System::Windows::Forms::ComboBox());
			this->button_query = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->comboBox_Hedge = (gcnew System::Windows::Forms::ComboBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->textBox_TradeID = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->comboBox_user = (gcnew System::Windows::Forms::ComboBox());
			this->button_clear = (gcnew System::Windows::Forms::Button());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->button_add = (gcnew System::Windows::Forms::Button());
			this->button_Modify = (gcnew System::Windows::Forms::Button());
			this->button_delete = (gcnew System::Windows::Forms::Button());
			this->button_confirm = (gcnew System::Windows::Forms::Button());
			this->button_apply = (gcnew System::Windows::Forms::Button());
			this->button_cannel = (gcnew System::Windows::Forms::Button());
			this->label_count = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->comboBox3 = (gcnew System::Windows::Forms::ComboBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->button_caculate = (gcnew System::Windows::Forms::Button());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->button_pre = (gcnew System::Windows::Forms::Button());
			this->button_next = (gcnew System::Windows::Forms::Button());
			this->Column1 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column5 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column6 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column7 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column8 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column9 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column10 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column12 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column13 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column14 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_closeprofit = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_TradeCommission = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_tradedate = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(38, 88);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(65, 12);
			this->label1->TabIndex = 3;
			this->label1->Text = L"交易所名称";
			// 
			// comboBox_exchange
			// 
			this->comboBox_exchange->FormattingEnabled = true;
			this->comboBox_exchange->Items->AddRange(gcnew cli::array< System::Object^  >(5) {L"全部", L"上期所", L"郑商所", L"大商所", L"中金所"});
			this->comboBox_exchange->Location = System::Drawing::Point(109, 84);
			this->comboBox_exchange->Name = L"comboBox_exchange";
			this->comboBox_exchange->Size = System::Drawing::Size(102, 20);
			this->comboBox_exchange->TabIndex = 4;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(250, 86);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(59, 12);
			this->label2->TabIndex = 5;
			this->label2->Text = L"品种\\合约";
			// 
			// comboBox_Ins
			// 
			this->comboBox_Ins->BackColor = System::Drawing::Color::White;
			this->comboBox_Ins->ChildControl = nullptr;
			this->comboBox_Ins->DropDownHeight = 1;
			this->comboBox_Ins->DropDownWidth = 1;
			this->comboBox_Ins->FormattingEnabled = true;
			this->comboBox_Ins->IntegralHeight = false;
			this->comboBox_Ins->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"全部"});
			this->comboBox_Ins->Location = System::Drawing::Point(315, 83);
			this->comboBox_Ins->MaxDropDownItems = 1;
			this->comboBox_Ins->Name = L"comboBox_Ins";
			this->comboBox_Ins->Size = System::Drawing::Size(105, 20);
			this->comboBox_Ins->TabIndex = 6;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(468, 92);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(35, 12);
			this->label3->TabIndex = 7;
			this->label3->Text = L"买\\卖";
			// 
			// comboBox_buySell
			// 
			this->comboBox_buySell->FormattingEnabled = true;
			this->comboBox_buySell->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"全部", L"买", L"卖"});
			this->comboBox_buySell->Location = System::Drawing::Point(530, 84);
			this->comboBox_buySell->Name = L"comboBox_buySell";
			this->comboBox_buySell->Size = System::Drawing::Size(83, 20);
			this->comboBox_buySell->TabIndex = 8;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(648, 89);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(35, 12);
			this->label4->TabIndex = 9;
			this->label4->Text = L"开\\平";
			// 
			// comboBox_Direction
			// 
			this->comboBox_Direction->FormattingEnabled = true;
			this->comboBox_Direction->Items->AddRange(gcnew cli::array< System::Object^  >(6) {L"全部", L"开仓", L"平仓", L"强平", L"平今", L"平昨"});
			this->comboBox_Direction->Location = System::Drawing::Point(699, 84);
			this->comboBox_Direction->Name = L"comboBox_Direction";
			this->comboBox_Direction->Size = System::Drawing::Size(76, 20);
			this->comboBox_Direction->TabIndex = 10;
			// 
			// button_query
			// 
			this->button_query->Location = System::Drawing::Point(816, 76);
			this->button_query->Name = L"button_query";
			this->button_query->Size = System::Drawing::Size(75, 23);
			this->button_query->TabIndex = 11;
			this->button_query->Text = L"查询";
			this->button_query->UseVisualStyleBackColor = true;
			this->button_query->Click += gcnew System::EventHandler(this, &TradeConfirmForm::button_query_Click);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(40, 123);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(59, 12);
			this->label5->TabIndex = 12;
			this->label5->Text = L"投机\\套保";
			// 
			// comboBox_Hedge
			// 
			this->comboBox_Hedge->FormattingEnabled = true;
			this->comboBox_Hedge->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"全部", L"投机", L"套保"});
			this->comboBox_Hedge->Location = System::Drawing::Point(109, 123);
			this->comboBox_Hedge->Name = L"comboBox_Hedge";
			this->comboBox_Hedge->Size = System::Drawing::Size(102, 20);
			this->comboBox_Hedge->TabIndex = 13;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(252, 129);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(53, 12);
			this->label6->TabIndex = 14;
			this->label6->Text = L"成交编号";
			// 
			// textBox_TradeID
			// 
			this->textBox_TradeID->Location = System::Drawing::Point(315, 120);
			this->textBox_TradeID->Name = L"textBox_TradeID";
			this->textBox_TradeID->Size = System::Drawing::Size(105, 21);
			this->textBox_TradeID->TabIndex = 15;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(468, 129);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(41, 12);
			this->label7->TabIndex = 16;
			this->label7->Text = L"交易员";
			// 
			// comboBox_user
			// 
			this->comboBox_user->FormattingEnabled = true;
			this->comboBox_user->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"全部"});
			this->comboBox_user->Location = System::Drawing::Point(530, 120);
			this->comboBox_user->Name = L"comboBox_user";
			this->comboBox_user->Size = System::Drawing::Size(76, 20);
			this->comboBox_user->TabIndex = 17;
			this->comboBox_user->SelectedIndexChanged += gcnew System::EventHandler(this, &TradeConfirmForm::comboBox1_SelectedIndexChanged);
			// 
			// button_clear
			// 
			this->button_clear->Location = System::Drawing::Point(816, 120);
			this->button_clear->Name = L"button_clear";
			this->button_clear->Size = System::Drawing::Size(75, 23);
			this->button_clear->TabIndex = 18;
			this->button_clear->Text = L"清空";
			this->button_clear->UseVisualStyleBackColor = true;
			this->button_clear->Click += gcnew System::EventHandler(this, &TradeConfirmForm::button_clear_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(15) {this->Column1, 
				this->Column2, this->Column3, this->Column5, this->Column6, this->Column7, this->Column8, this->Column9, this->Column10, this->Column12, 
				this->Column13, this->Column14, this->Column_closeprofit, this->Column_TradeCommission, this->Column_tradedate});
			this->dataGridView1->Location = System::Drawing::Point(42, 197);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(862, 425);
			this->dataGridView1->TabIndex = 19;
			this->dataGridView1->Sorted += gcnew System::EventHandler(this, &TradeConfirmForm::dataGridView1_Sorted);
			this->dataGridView1->ColumnHeaderMouseClick += gcnew System::Windows::Forms::DataGridViewCellMouseEventHandler(this, &TradeConfirmForm::dataGridView1_ColumnHeaderMouseClick);
			// 
			// button_add
			// 
			this->button_add->Location = System::Drawing::Point(42, 644);
			this->button_add->Name = L"button_add";
			this->button_add->Size = System::Drawing::Size(57, 23);
			this->button_add->TabIndex = 20;
			this->button_add->Text = L"新增";
			this->button_add->UseVisualStyleBackColor = true;
			this->button_add->Click += gcnew System::EventHandler(this, &TradeConfirmForm::button_add_Click);
			// 
			// button_Modify
			// 
			this->button_Modify->Location = System::Drawing::Point(109, 644);
			this->button_Modify->Name = L"button_Modify";
			this->button_Modify->Size = System::Drawing::Size(55, 23);
			this->button_Modify->TabIndex = 21;
			this->button_Modify->Text = L"修改";
			this->button_Modify->UseVisualStyleBackColor = true;
			this->button_Modify->Click += gcnew System::EventHandler(this, &TradeConfirmForm::button_Modify_Click);
			// 
			// button_delete
			// 
			this->button_delete->Location = System::Drawing::Point(175, 644);
			this->button_delete->Name = L"button_delete";
			this->button_delete->Size = System::Drawing::Size(55, 23);
			this->button_delete->TabIndex = 22;
			this->button_delete->Text = L"删除";
			this->button_delete->UseVisualStyleBackColor = true;
			this->button_delete->Click += gcnew System::EventHandler(this, &TradeConfirmForm::button_delete_Click);
			// 
			// button_confirm
			// 
			this->button_confirm->Location = System::Drawing::Point(346, 660);
			this->button_confirm->Name = L"button_confirm";
			this->button_confirm->Size = System::Drawing::Size(103, 23);
			this->button_confirm->TabIndex = 23;
			this->button_confirm->Text = L"确认提交";
			this->button_confirm->UseVisualStyleBackColor = true;
			this->button_confirm->Click += gcnew System::EventHandler(this, &TradeConfirmForm::button_confirm_Click);
			// 
			// button_apply
			// 
			this->button_apply->Location = System::Drawing::Point(733, 660);
			this->button_apply->Name = L"button_apply";
			this->button_apply->Size = System::Drawing::Size(75, 23);
			this->button_apply->TabIndex = 24;
			this->button_apply->Text = L"应用";
			this->button_apply->UseVisualStyleBackColor = true;
			this->button_apply->Visible = false;
			this->button_apply->Click += gcnew System::EventHandler(this, &TradeConfirmForm::button_apply_Click);
			// 
			// button_cannel
			// 
			this->button_cannel->Location = System::Drawing::Point(816, 660);
			this->button_cannel->Name = L"button_cannel";
			this->button_cannel->Size = System::Drawing::Size(75, 23);
			this->button_cannel->TabIndex = 25;
			this->button_cannel->Text = L"取消";
			this->button_cannel->UseVisualStyleBackColor = true;
			this->button_cannel->Visible = false;
			this->button_cannel->Click += gcnew System::EventHandler(this, &TradeConfirmForm::button_cannel_Click);
			// 
			// label_count
			// 
			this->label_count->AutoSize = true;
			this->label_count->Location = System::Drawing::Point(475, 168);
			this->label_count->Name = L"label_count";
			this->label_count->Size = System::Drawing::Size(41, 12);
			this->label_count->TabIndex = 26;
			this->label_count->Text = L"查询到";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(583, 168);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(53, 12);
			this->label8->TabIndex = 27;
			this->label8->Text = L"每页显示";
			// 
			// comboBox2
			// 
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Location = System::Drawing::Point(636, 165);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(42, 20);
			this->comboBox2->TabIndex = 28;
			this->comboBox2->SelectionChangeCommitted += gcnew System::EventHandler(this, &TradeConfirmForm::comboBox2_SelectionChangeCommitted);
			this->comboBox2->SelectedIndexChanged += gcnew System::EventHandler(this, &TradeConfirmForm::comboBox2_SelectedIndexChanged);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(722, 168);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(41, 12);
			this->label9->TabIndex = 29;
			this->label9->Text = L"当前第";
			// 
			// comboBox3
			// 
			this->comboBox3->FormattingEnabled = true;
			this->comboBox3->Location = System::Drawing::Point(769, 165);
			this->comboBox3->Name = L"comboBox3";
			this->comboBox3->Size = System::Drawing::Size(39, 20);
			this->comboBox3->TabIndex = 30;
			this->comboBox3->SelectionChangeCommitted += gcnew System::EventHandler(this, &TradeConfirmForm::comboBox3_SelectionChangeCommitted);
			this->comboBox3->SelectedIndexChanged += gcnew System::EventHandler(this, &TradeConfirmForm::comboBox3_SelectedIndexChanged);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(814, 168);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(47, 12);
			this->label10->TabIndex = 31;
			this->label10->Text = L"label10";
			// 
			// button_caculate
			// 
			this->button_caculate->Location = System::Drawing::Point(500, 660);
			this->button_caculate->Name = L"button_caculate";
			this->button_caculate->Size = System::Drawing::Size(75, 23);
			this->button_caculate->TabIndex = 33;
			this->button_caculate->Text = L"试运算";
			this->button_caculate->UseVisualStyleBackColor = true;
			this->button_caculate->Click += gcnew System::EventHandler(this, &TradeConfirmForm::button_caculate_Click);
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"SimSun", 10.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->label12->Location = System::Drawing::Point(235, 29);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(56, 14);
			this->label12->TabIndex = 34;
			this->label12->Text = L"label12";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(599, 665);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(0, 12);
			this->label13->TabIndex = 35;
			// 
			// button_pre
			// 
			this->button_pre->Location = System::Drawing::Point(684, 162);
			this->button_pre->Name = L"button_pre";
			this->button_pre->Size = System::Drawing::Size(37, 23);
			this->button_pre->TabIndex = 36;
			this->button_pre->Text = L"<<";
			this->button_pre->UseVisualStyleBackColor = true;
			this->button_pre->Click += gcnew System::EventHandler(this, &TradeConfirmForm::button_pre_Click);
			// 
			// button_next
			// 
			this->button_next->Location = System::Drawing::Point(867, 162);
			this->button_next->Name = L"button_next";
			this->button_next->Size = System::Drawing::Size(37, 23);
			this->button_next->TabIndex = 37;
			this->button_next->Text = L">>";
			this->button_next->UseVisualStyleBackColor = true;
			this->button_next->Click += gcnew System::EventHandler(this, &TradeConfirmForm::button_next_Click);
			// 
			// Column1
			// 
			this->Column1->HeaderText = L"交易所代码";
			this->Column1->Name = L"Column1";
			this->Column1->ReadOnly = true;
			this->Column1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column1->Width = 90;
			// 
			// Column2
			// 
			this->Column2->HeaderText = L"成交编号";
			this->Column2->Name = L"Column2";
			this->Column2->ReadOnly = true;
			this->Column2->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column2->Width = 78;
			// 
			// Column3
			// 
			this->Column3->HeaderText = L"买卖方向";
			this->Column3->Name = L"Column3";
			this->Column3->ReadOnly = true;
			this->Column3->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column3->Width = 78;
			// 
			// Column5
			// 
			this->Column5->HeaderText = L"交易员账号";
			this->Column5->Name = L"Column5";
			this->Column5->ReadOnly = true;
			this->Column5->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column5->Width = 90;
			// 
			// Column6
			// 
			this->Column6->HeaderText = L"合约代码";
			this->Column6->Name = L"Column6";
			this->Column6->ReadOnly = true;
			this->Column6->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column6->Width = 78;
			// 
			// Column7
			// 
			this->Column7->HeaderText = L"开平状态";
			this->Column7->Name = L"Column7";
			this->Column7->ReadOnly = true;
			this->Column7->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column7->Width = 78;
			// 
			// Column8
			// 
			this->Column8->HeaderText = L"投机\\套保";
			this->Column8->Name = L"Column8";
			this->Column8->ReadOnly = true;
			this->Column8->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column8->Width = 84;
			// 
			// Column9
			// 
			this->Column9->HeaderText = L"成交手数";
			this->Column9->Name = L"Column9";
			this->Column9->ReadOnly = true;
			this->Column9->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column9->Width = 78;
			// 
			// Column10
			// 
			this->Column10->HeaderText = L"成交价格";
			this->Column10->Name = L"Column10";
			this->Column10->ReadOnly = true;
			this->Column10->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column10->Width = 78;
			// 
			// Column12
			// 
			this->Column12->HeaderText = L"成交时间";
			this->Column12->Name = L"Column12";
			this->Column12->ReadOnly = true;
			this->Column12->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column12->Width = 78;
			// 
			// Column13
			// 
			this->Column13->HeaderText = L"成交类型";
			this->Column13->Name = L"Column13";
			this->Column13->ReadOnly = true;
			this->Column13->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column13->Width = 78;
			// 
			// Column14
			// 
			this->Column14->HeaderText = L"报单编号";
			this->Column14->Name = L"Column14";
			this->Column14->ReadOnly = true;
			this->Column14->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column14->Width = 78;
			// 
			// Column_closeprofit
			// 
			this->Column_closeprofit->HeaderText = L"平仓盈亏";
			this->Column_closeprofit->Name = L"Column_closeprofit";
			this->Column_closeprofit->ReadOnly = true;
			this->Column_closeprofit->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column_closeprofit->Width = 78;
			// 
			// Column_TradeCommission
			// 
			this->Column_TradeCommission->HeaderText = L"手续费";
			this->Column_TradeCommission->Name = L"Column_TradeCommission";
			this->Column_TradeCommission->ReadOnly = true;
			this->Column_TradeCommission->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column_TradeCommission->Width = 66;
			// 
			// Column_tradedate
			// 
			this->Column_tradedate->HeaderText = L"成交日期";
			this->Column_tradedate->Name = L"Column_tradedate";
			this->Column_tradedate->ReadOnly = true;
			this->Column_tradedate->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Programmatic;
			this->Column_tradedate->Width = 78;
			// 
			// TradeConfirmForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(939, 695);
			this->Controls->Add(this->button_next);
			this->Controls->Add(this->button_pre);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->button_caculate);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->comboBox3);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->comboBox2);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label_count);
			this->Controls->Add(this->button_cannel);
			this->Controls->Add(this->button_apply);
			this->Controls->Add(this->button_confirm);
			this->Controls->Add(this->button_delete);
			this->Controls->Add(this->button_Modify);
			this->Controls->Add(this->button_add);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->button_clear);
			this->Controls->Add(this->comboBox_user);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->textBox_TradeID);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->comboBox_Hedge);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->button_query);
			this->Controls->Add(this->comboBox_Direction);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->comboBox_buySell);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->comboBox_Ins);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboBox_exchange);
			this->Controls->Add(this->label1);
			this->Name = L"TradeConfirmForm";
			this->Text = L"成交确认";
			this->Load += gcnew System::EventHandler(this, &TradeConfirmForm::TradeConfirmForm_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &TradeConfirmForm::TradeConfirmForm_FormClosed);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		protected:virtual void WndProc( Message% m ) override
				  {
					if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival )
					  {
						  char *p = (char*)m.LParam.ToInt32();
						  Stru_UniPkgHead head;					
						  memset(&head, 0, sizeof(head));
						  memcpy(&head, p, sizeof(head));
						  /*OUTPUT_LOG("客户端成交确认收到消息： %d",  head.cmdid);*/

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

									  std::map<int, BrokerInfo> mapBrokerInfo;
									  int nCount = head.len / sizeof(BrokerInfo);
									  if(nCount == 0)
										  break;
									  for ( int i = 0; i < nCount; i++ )
									  {
										  BrokerInfo* pBrokerInfo = (BrokerInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(BrokerInfo));
										  mapBrokerInfo[pBrokerInfo->nBrokerID] = *pBrokerInfo;
										 
									  }
									  m_pDataInfo->SetBrokerInfos(mapBrokerInfo);

								  }
								  else
								  {
									  string str = (char*)(p+sizeof(Stru_UniPkgHead));

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
									  /*comboBox1->Items->Clear();
									  comboBox1->Items->Add(Tools::string2String(SELECT_ALL));*/

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
										_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
												CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
												str.c_str());
								  }

								  break;
							  }
						  case Cmd_Settlement_QueryTrade_Rsp:
							  {
								  if ( head.userdata1 == CF_ERROR_SUCCESS )
								  {

									  
									  std::map<TradeKey,PlatformStru_TradeInfo>  lMapFiled;
									  int nCount = head.len / sizeof(PlatformStru_TradeInfo);
									  if(nCount == 0)
										  break;
									  for ( int i = 0; i < nCount; i++ )
									  {
										  PlatformStru_TradeInfo* pField = (PlatformStru_TradeInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(PlatformStru_TradeInfo));

										  TradeKey key(*pField);
										  lMapFiled.insert(make_pair(key,*pField));
									  }
									  m_pDataInfo->ClearTrade();
									  m_pDataInfo->SetTrade(lMapFiled);

									  std::vector<PlatformStru_TradeInfo> vecTrade;
									  GetTradeVec(vecTrade);
									  m_pDataInfo->SaveSortedTrade(vecTrade);
									  UpdatePageBar(vecTrade);
									  std::vector<PlatformStru_TradeInfo> vecShowTrade;
									  GetShowData(vecTrade,vecShowTrade);
									  loadTradeView(vecShowTrade);
									  this->label12->Text = L"";
									  this->button_query->Enabled =true;
									  if (vecTrade.size() == 0)
									  {
										  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
											  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
											  "没有成交记录");
									  }

								  }
								  else
								  {
									  string str = (char*)(p+sizeof(Stru_UniPkgHead));
										_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
												CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
												str.c_str());
									  label12->Text=L"";
									  this->button_query->Enabled =true;
								  }

								  break;
							  }
						  case Cmd_Settlement_TradeCaculate_Rsp:
							  {
								  if ( head.userdata1 == CF_ERROR_SUCCESS )
								  {


									  std::map<TradeKey,PlatformStru_TradeInfo>  lMapFiled;
									  int nCount = head.len / sizeof(PlatformStru_TradeInfo);
									  if(nCount == 0)
										  break;
									  for ( int i = 0; i < nCount; i++ )
									  {
										  PlatformStru_TradeInfo* pField = (PlatformStru_TradeInfo*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(PlatformStru_TradeInfo));

										 TradeKey key(*pField);
										  lMapFiled.insert(make_pair(key,*pField));
									  }
									  m_pDataInfo->ClearTrade();
									  m_pDataInfo->SetTrade(lMapFiled);

									  std::vector<PlatformStru_TradeInfo> vecTrade;
									  GetTradeVec(vecTrade);
									  m_pDataInfo->SaveSortedTrade(vecTrade);
									  UpdatePageBar(vecTrade);
									  std::vector<PlatformStru_TradeInfo> vecShowTrade;
									  GetShowData(vecTrade,vecShowTrade);
									  loadTradeView(vecShowTrade);
									  this->label13->Text = L"";

								  }
								  else
								  {
									  string str = (char*)(p+sizeof(Stru_UniPkgHead));
										_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
												CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
												str.c_str());
									  //PromptForm^ dlg = gcnew PromptForm(strtemp);
									  //dlg->Show();
									  this->label13->Text = L"";
								  }

								  break;
							  }
						  case Cmd_Settlement_AddTrade_Rsp:
							  {
								  if ( head.userdata1 == CF_ERROR_SUCCESS )
								  {
									  PlatformStru_TradeInfo* pField = (PlatformStru_TradeInfo*)((char*)(p+sizeof(Stru_UniPkgHead)));
									  m_pDataInfo->AddTrade(*pField);	
									  /*int newRowIndex=dataGridView1->Rows->Add();
									  DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
									  UpdateRow(dr,*pField);*/	
									  std::vector<PlatformStru_TradeInfo> vecTrade;
									  GetTradeVec(vecTrade);
									  m_pDataInfo->SaveSortedTrade(vecTrade);
									  UpdatePageBar(vecTrade);
									  std::vector<PlatformStru_TradeInfo> vecShowTrade;
									  GetShowData(vecTrade,vecShowTrade);
									  loadTradeView(vecShowTrade);
								  }
								  else
								  {
									  string str = (char*)(p+sizeof(Stru_UniPkgHead));
										_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
												CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
												str.c_str());
								  }

								  break;
							  }
						  case Cmd_Settlement_ModifyTrade_Rsp:
							  {
								  if ( head.userdata1 == CF_ERROR_SUCCESS )
								  {
									  PlatformStru_TradeInfo* pField = (PlatformStru_TradeInfo*)((char*)(p+sizeof(Stru_UniPkgHead)));
									  m_pDataInfo->ModifyTrade(*m_pOriginalTradeInfo,*pField);	
									  DataGridViewRow^  dr = dataGridView1->CurrentRow;
									  if(dr != nullptr)
										  UpdateRow(dr,*pField);
									  std::vector<PlatformStru_TradeInfo> vecTrade;
									  GetTradeVec(vecTrade);
									  m_pDataInfo->SaveSortedTrade(vecTrade);
								  }
								  else
								  {
									  string str = (char*)(p+sizeof(Stru_UniPkgHead));
										_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
												CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
												str.c_str());
								  }

								  break;
							  }
						  case Cmd_Settlement_DeleteTrade_Rsp:
							  {
								  if ( head.userdata1 == CF_ERROR_SUCCESS )
								  {
									  PlatformStru_TradeInfo* pField = (PlatformStru_TradeInfo*)((char*)(p+sizeof(Stru_UniPkgHead)));
									  m_pDataInfo->DeleteTrade(*pField);	
									  /*DataGridViewRow^  dr = dataGridView1->CurrentRow;
									  if(dr != nullptr)
									      dataGridView1->Rows->RemoveAt(dr->Index);  */
									  std::vector<PlatformStru_TradeInfo> vecTrade;
									  GetTradeVec(vecTrade);
									  m_pDataInfo->SaveSortedTrade(vecTrade);
									  UpdatePageBar(vecTrade);
									  std::vector<PlatformStru_TradeInfo> vecShowTrade;
									  GetShowData(vecTrade,vecShowTrade);
									  loadTradeView(vecShowTrade);
								  }
								  else
								  {
									  string str = (char*)(p+sizeof(Stru_UniPkgHead));
										_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
												CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
												str.c_str());
								  }

								  break;
							  }
						  case Cmd_Settlement_ConfirmTrade_Rsp:
							  {
								  if ( head.userdata1 == CF_ERROR_SUCCESS )
								  {
									  this->DialogResult = System::Windows::Forms::DialogResult::Yes;
									  this->Close();
								  }
								  else
								  {
									  string str = (char*)(p+sizeof(Stru_UniPkgHead));
										_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
												CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
												str.c_str());
								  }

								  break;
							  }
						  case Cmd_QryUser_Rsp:
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
										  if ( pField->nUserType == USER_TYPE_TRADER )
										  {											 
											  string strTemp =pField->szAccount;
											  string strTempN =pField->szAlias;
											  strTemp += "+" + strTempN;
											  comboBox_user->Items->Add(Tools::string2String(strTemp.c_str()));
										  }

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

						  default:
							  break;
						  }

					  }

					  Form::WndProc ( m );
				  }
	private: System::Void TradeConfirmForm_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent(); 
				 int nHwnd = this->Handle.ToInt32();
				 m_pDataInfo->GetLogonInfo(*m_pLogin);


				 dlgInstrumentIDTreeList = gcnew CInstrumentIDTreeListDlg();
				 dlgInstrumentIDTreeList->ResetRelateCombo(comboBox_Ins);
				 comboBox_Ins->ChildControl = dlgInstrumentIDTreeList;

				 
				 PlatformStru_QryInstrument Field;
				 CTcpLayer::SubscribePkg(Cmd_TradeDataManager_QryInstrument_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_QueryTradeAccount_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_QryUser_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_QueryBrokerInfo_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_QueryTrade_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_AddTrade_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_ModifyTrade_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_DeleteTrade_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_ConfirmTrade_Rsp, nHwnd);
				 CTcpLayer::SubscribePkg(Cmd_Settlement_TradeCaculate_Rsp, nHwnd);
				
				 if ( !CTcpLayer::SendData(Cmd_QueryTradeAccount_Req, NULL, 0, 0, m_pLogin->mnUserID)
					 || (!CTcpLayer::SendData(Cmd_Settlement_QueryTrade_Req, NULL, 0, 0, m_pLogin->mnUserID))
					 || (!CTcpLayer::SendData(Cmd_TradeDataManager_QryInstrument_Req, &Field, sizeof(PlatformStru_QryInstrument), 0, m_pLogin->mnUserID))
					 || (!CTcpLayer::SendData(Cmd_QueryBrokerInfo_Req, NULL, 0, 0, m_pLogin->mnUserID))
					 || (!CTcpLayer::SendData(Cmd_QryUser_Req, NULL, 0, 0, m_pLogin->mnUserID)))
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }
				 m_pDataInfo->ClearTrade();
				 std::vector<PlatformStru_TradeInfo> vecTrade;
				 GetTradeVec(vecTrade);
				 UpdatePageBar(vecTrade);
				 label12->Text=L"正在查询成交数据，请稍后";
				 this->button_query->Enabled =false;

				 
			 }
private: System::Void TradeConfirmForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
		 {
			 /*if (m_pDataInfo->HaveModifiedTrade())
			 {
				 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"您已做数据修改，但是未保存，是否取消操作并关闭窗口？", "提示", MessageBoxButtons::YesNo);
				 if ( dr == System::Windows::Forms::DialogResult::Yes)
				 { 
					 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
					 this->Close();
				 }

			 }*/
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
		
		 }
private: System::Void GetTradeVec(std::vector<PlatformStru_TradeInfo>& vecTrade) 
		 {
			 string strUserID = Tools::String2string(comboBox_user->Text);
			 if ( strUserID =="" )
			 {
				 strUserID = SELECT_ALL;
			 }
		

			 unsigned pos = strUserID.find("+");        

			 std::string str = strUserID.substr ( 0,pos );
			 std::vector<PlatformStru_TradeInfo> vecTradeUser;
			 vecTradeUser.clear();
			 m_pDataInfo->GetTrade(vecTradeUser,str);

			 std::set<string> setFilterInstrumentID;
			 dlgInstrumentIDTreeList->GetSelectID(setFilterInstrumentID);
			 std::vector<PlatformStru_TradeInfo> vecTradeIns;
			 if (setFilterInstrumentID.size()!=0)
			 {
				 std::set<string>::iterator it =setFilterInstrumentID.begin();
				 for (; it!=setFilterInstrumentID.end();it++)
				 {
					 for (int i=0;i<(int)vecTradeUser.size();i++)
					 {
						 if( strncmp((*it).c_str(),vecTradeUser[i].InstrumentID,(*it).length()) == 0)
							 vecTradeIns.push_back(vecTradeUser[i]);
					 }
				 }
			 }
			 else
				 vecTradeIns = vecTradeUser;


			 //交易所
			 string strExchange = Tools::String2string(comboBox_exchange->Text);
			 if (strExchange != ""&& strExchange !=SELECT_ALL)
			 {
				 std::vector<PlatformStru_TradeInfo>::iterator it= vecTradeIns.begin();
				 while (it!=vecTradeIns.end())
				 {
					 string exchange =  m_pDataInfo->ExchangeName2ID(strExchange);
					 if (strcmp((*it).ExchangeID ,exchange.c_str()) != 0)
					 {
						 it=vecTradeIns.erase(it);
					 }
					 else
						 ++it;
				 }
			 }

			 //买卖
			 string strbuySell = Tools::String2string(comboBox_buySell->Text);
			 if (strbuySell != ""&& strbuySell !=SELECT_ALL)
			 {
				 std::vector<PlatformStru_TradeInfo>::iterator it= vecTradeIns.begin();
				 while (it!=vecTradeIns.end())
				 {
					 if (m_pDataInfo->TradeDirection2String((*it).Direction) !=strbuySell )
					 {
						 it=vecTradeIns.erase(it);
					 }
					 else
						 ++it;
				 }
			 }
			 //开平
			 string strDirection = Tools::String2string(comboBox_Direction->Text);
			 if (strDirection != ""&& strDirection !=SELECT_ALL)
			 {
				 std::vector<PlatformStru_TradeInfo>::iterator it= vecTradeIns.begin();
				 while(it!=vecTradeIns.end())
				 {
					 if (m_pDataInfo->OffsetFlag2String((*it).OffsetFlag) !=strDirection )
					 {
						 it=vecTradeIns.erase(it);
					 }
					 else
						 ++it;
				 }
			 }
			 //投机套保
			 string strHedge = Tools::String2string(comboBox_Hedge->Text);
			 if (strHedge != ""&& strHedge !=SELECT_ALL)
			 {
				 std::vector<PlatformStru_TradeInfo>::iterator it= vecTradeIns.begin();
				 while (it!=vecTradeIns.end())
				 {
					 if (m_pDataInfo->HedgeFlag2String((*it).HedgeFlag) !=strHedge )
					 {
						 it=vecTradeIns.erase(it);
					 }
					 else
						 ++it;
				 }
			 }
			 //成交编号
			 string strTradeID = Tools::String2string(textBox_TradeID->Text);
			 if (strTradeID != "")
			 {
				 std::vector<PlatformStru_TradeInfo>::iterator it= vecTradeIns.begin();
				 while (it!=vecTradeIns.end())
				 {
					 string strtemp((*it).TradeID);
					 CTools_Ansi::mytrimleft(strtemp);
					 if ( strtemp != strTradeID  )
					 {
						 it=vecTradeIns.erase(it);
					 }
					 else
						 ++it;
				 }
			 }
			 vecTrade = vecTradeIns;

		 }
 private: System::Void UpdateRow(DataGridViewRow^ dr,PlatformStru_TradeInfo& Field)
		{
			dr->Cells[0]->Value = Tools::string2String(Field.ExchangeID);
			dr->Cells[1]->Value = Tools::string2String(Field.TradeID)->Trim();
			dr->Cells[2]->Value = Tools::string2String(m_pDataInfo->TradeDirection2String(Field.Direction));
			dr->Cells[3]->Value = Tools::string2String(Field.InvestorID);
			dr->Cells[4]->Value = Tools::string2String(Field.InstrumentID);
			dr->Cells[5]->Value = Tools::string2String(m_pDataInfo->OffsetFlag2String(Field.OffsetFlag));
			dr->Cells[6]->Value = Tools::string2String(m_pDataInfo->HedgeFlag2String(Field.HedgeFlag));

			char strVolume[32];
			memset(strVolume,0,strlen(strVolume)+1);
			sprintf(strVolume, "%d",Field.Volume);
			dr->Cells[7]->Value = Tools::string2String(strVolume);
			char strPrice[128];
			memset(strPrice,0,sizeof(strPrice));
			sprintf(strPrice, "%.3f",Field.Price);
			dr->Cells[8]->Value = Tools::string2String(strPrice);
		
			dr->Cells[9]->Value = Tools::string2String(Field.TradeTime);
			dr->Cells[10]->Value = Tools::string2String(m_pDataInfo->TradeType2String(Field.TradeType));
			dr->Cells[11]->Value = Tools::string2String(Field.OrderSysID)->Trim();
			if (Field.CloseProfitByDate!= InvalideValue)
			{
			memset(strPrice,0,sizeof(strPrice));
			sprintf(strPrice, "%.2f",Field.CloseProfitByDate);
			dr->Cells[12]->Value = Tools::string2String(strPrice);
			}
			if (Field.TradeCommission!= InvalideValue)
			{
			memset(strPrice,0,sizeof(strPrice));
			sprintf(strPrice, "%.2f",Field.TradeCommission);
			dr->Cells[13]->Value = Tools::string2String(strPrice);
			}
			dr->Cells[14]->Value = Tools::string2String(Field.TradeDate);
		 }
private: System::Void loadTradeView(std::vector<PlatformStru_TradeInfo>& lvecField)
			{
				dataGridView1->Rows->Clear();
				std::vector<PlatformStru_TradeInfo>::iterator it;
				for(it = lvecField.begin(); it != lvecField.end(); it++)
				{

					int newRowIndex=dataGridView1->Rows->Add();
					DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
					UpdateRow(dr,*it);					

				}



			}
private: System::Void button_query_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			if(!CTcpLayer::SendData(Cmd_Settlement_QueryTrade_Req, NULL, 0, 0, m_pLogin->mnUserID))
			{
				MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			}
			 
			 label12->Text=L"正在查询成交数据，请稍后";
			 
		 }


private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button_confirm_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"是否确定确认成交？", "提示", MessageBoxButtons::YesNo);
			 if ( dr == System::Windows::Forms::DialogResult::Yes)
			 { 
				 int nHwnd = this->Handle.ToInt32();
				 //CTcpLayer::SetMainWndHandle(nHwnd);

				 m_pDataInfo->GetLogonInfo(*m_pLogin);

				 if ( !CTcpLayer::SendData(Cmd_Settlement_ConfirmTrade_Req, NULL, 0, 0, m_pLogin->mnUserID))
				 {
					 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }	
				 label12->Text=L"正在向服务器提交成交数据，请稍后";
			 }
		 }
private: System::Void button_LoadCTP_Click(System::Object^  sender, System::EventArgs^  e) 
		 {

			 int nHwnd = this->Handle.ToInt32();
			 //CTcpLayer::SetMainWndHandle(nHwnd);

			 m_pDataInfo->GetLogonInfo(*m_pLogin);

			 if ( !CTcpLayer::SendData(Cmd_Settlement_QueryTrade_Req, NULL, 0, 0, m_pLogin->mnUserID,SettlementLoadOption_TradingDataWithCTPSett))
			 {
				 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			 }
		 }
private: System::Void button_Loadintraday_Click(System::Object^  sender, System::EventArgs^  e) 
		 {

			 int nHwnd = this->Handle.ToInt32();
			 //CTcpLayer::SetMainWndHandle(nHwnd);

			 m_pDataInfo->GetLogonInfo(*m_pLogin);

			 if ( !CTcpLayer::SendData(Cmd_Settlement_QueryTrade_Req, NULL, 0, 0, m_pLogin->mnUserID,SettlementLoadOption_TradingDataWithTodayDB))
			 {
				 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			 }
		 }
private: System::Void button_add_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 AddTradeForm^ dlg = gcnew AddTradeForm(m_TradeDate);
			 dlg->SetOperType(Oper_Add);
			 if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
			 {
				 *m_pTradeInfo = *(dlg->GetTradeInfo());  			
				
				 CTcpLayer::SendData(Cmd_Settlement_AddTrade_Req, m_pTradeInfo, sizeof(PlatformStru_TradeInfo), 0,m_pLogin->mnUserID);
			 }
		 }
private: System::Void button_Modify_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(dataGridView1->SelectedRows != nullptr && dataGridView1->SelectedRows->Count == 1)
			 {
				 DataGridViewRow^ cur =   dataGridView1->CurrentRow;	
				
				 char  traderID[21]="";
				 sprintf(traderID,"%12s", Tools::String2string(cur->Cells[1]->Value->ToString()).c_str());
				 char  InvestorID[13]="";
				 sprintf(InvestorID,"%s", Tools::String2string(cur->Cells[3]->Value->ToString()).c_str());
				 char  InstrumentID[31]="";
				 sprintf(InstrumentID,"%s", Tools::String2string(cur->Cells[4]->Value->ToString()).c_str());
				 char  tradeDate[9]="";
				 sprintf(tradeDate,"%s", Tools::String2string(cur->Cells[14]->Value->ToString()).c_str());
				 char  OrderSysID[21]="";
				 sprintf(OrderSysID,"%s", Tools::String2string(cur->Cells[11]->Value->ToString()).c_str());
				 char Direction = m_pDataInfo->TradeDirection2ID(Tools::String2string(cur->Cells[2]->Value->ToString()));
                 char HedgeFlag = m_pDataInfo->HedgeFlag2ID(Tools::String2string(cur->Cells[6]->Value->ToString()));
				 TradeKey key(InvestorID,InstrumentID,traderID,tradeDate,Direction,HedgeFlag);
				 if(!m_pDataInfo->GetTradeInfo(key,*m_pOriginalTradeInfo))
					 return;

				 AddTradeForm^ dlg = gcnew AddTradeForm(m_TradeDate);
				 dlg->SetOperType(Oper_Modify);
				 dlg->SetTradeInfo(*m_pOriginalTradeInfo);
				 if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
				 {
					 *m_pTradeInfo = *(dlg->GetTradeInfo()); 
					 std::vector<PlatformStru_TradeInfo> lVecFiled;
					 lVecFiled.push_back(*m_pOriginalTradeInfo);
					 lVecFiled.push_back(*m_pTradeInfo);
					 CTcpLayer::SendData(Cmd_Settlement_ModifyTrade_Req, &lVecFiled[0], sizeof(PlatformStru_TradeInfo)*lVecFiled.size(), 0,m_pLogin->mnUserID);
				 }
			 }
			 else
				 MessageBox::Show("请选择内容");
		 }
private: System::Void button_delete_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(dataGridView1->SelectedRows != nullptr && dataGridView1->SelectedRows->Count == 1)
			 {
				 DataGridViewRow^ cur =   dataGridView1->CurrentRow;	
				 char  traderID[21]="";
				 sprintf(traderID,"%12s", Tools::String2string(cur->Cells[1]->Value->ToString()).c_str());
				 char  InvestorID[13]="";
				 sprintf(InvestorID,"%s", Tools::String2string(cur->Cells[3]->Value->ToString()).c_str());

				 char  InstrumentID[31]="";
				 sprintf(InstrumentID,"%s", Tools::String2string(cur->Cells[4]->Value->ToString()).c_str());
				 char  tradeDate[9]="";
				 sprintf(tradeDate,"%s", Tools::String2string(cur->Cells[14]->Value->ToString()).c_str());
				 char  OrderSysID[21]="";
				 sprintf(OrderSysID,"%s", Tools::String2string(cur->Cells[11]->Value->ToString()).c_str());
				 char Direction = m_pDataInfo->TradeDirection2ID(Tools::String2string(cur->Cells[2]->Value->ToString()));
                 char HedgeFlag = m_pDataInfo->HedgeFlag2ID(Tools::String2string(cur->Cells[6]->Value->ToString()));
				 TradeKey key(InvestorID,InstrumentID,traderID,tradeDate,Direction,HedgeFlag);
				 if(!m_pDataInfo->GetTradeInfo(key,*m_pTradeInfo))
					 return;

				 CTcpLayer::SendData(Cmd_Settlement_DeleteTrade_Req, m_pTradeInfo, sizeof(PlatformStru_TradeInfo), 0,m_pLogin->mnUserID);
			 }
			 else
				 MessageBox::Show("请选择内容");
		 }
private: System::Void button_apply_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
		 }

private: System::Void button_cannel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
		 }
private: System::Void button_clear_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 m_pDataInfo->ClearTrade();
			 std::vector<PlatformStru_TradeInfo> vecTrade;
			 GetTradeVec(vecTrade);
			 m_pDataInfo->SaveSortedTrade(vecTrade);
			 UpdatePageBar(vecTrade);

			 dataGridView1->Rows->Clear();
			 
		 }
private: System::Void button_caculate_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (dataGridView1->Rows->Count == 0)
			 {
				 return;
			 }
		
			 m_pDataInfo->GetLogonInfo(*m_pLogin);
			 if ( !CTcpLayer::SendData(Cmd_Settlement_TradeCaculate_Req, NULL, 0, 0, m_pLogin->mnUserID))
			 {
				 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			 }
			 this->label13->Text = L"正在运算，请稍后";
		 }

private: void GetShowData(std::vector<PlatformStru_TradeInfo>& vecAllTrade,std::vector<PlatformStru_TradeInfo>& vecShowTrade)
		 {

			 int nDistance = (m_nCurShowPage-1) * m_nRowNumPerPage;
			 std::vector<PlatformStru_TradeInfo>::iterator it_dis = vecAllTrade.begin();
			 if ( nDistance < static_cast<int>(vecAllTrade.size()))
			 {
				 std::advance(it_dis, nDistance);
				 for ( int i = 0; i < m_nRowNumPerPage && it_dis != vecAllTrade.end(); i++, it_dis++ )
				 {
					vecShowTrade.push_back(*it_dis);

				 }
			 }
			 
		 }

private:void  UpdatePageBar(std::vector<PlatformStru_TradeInfo>& vecTrade)
		 {

			 char strVolume[32];
			 memset(strVolume,0,strlen(strVolume)+1);
			 sprintf(strVolume, "查询到%d条记录",vecTrade.size());
			 this->label_count->Text = Tools::string2String(strVolume);

			 int nTotalPage = vecTrade.size() / m_nRowNumPerPage;
			 if ( vecTrade.size() % m_nRowNumPerPage != 0 )
			 {
				 nTotalPage += 1;
			 }



			 memset(strVolume,0,strlen(strVolume)+1);
			 sprintf(strVolume, "/%d页",nTotalPage);
			 this->label10->Text = Tools::string2String(strVolume);

			 
			 

			 comboBox2->Items->Clear();

			 for ( int i = 10; i <=  (static_cast<int>(vecTrade.size())+10) && i<= 50; i+=10 )
			 {
				 comboBox2->Items->Add( i.ToString() );
			 }
            this->comboBox2->Text = m_nRowNumPerPage.ToString();


			 comboBox3->Items->Clear();

			 int nCount = comboBox3->Items->Count;
			 if ( nCount > nTotalPage )
			 {
				 for ( int i = 0; i < nCount - nTotalPage; i++ )
				 {
					 comboBox3->Items->Add( (nCount-1-i).ToString() );
				 }
			 }
			 else if ( nCount < nTotalPage )
			 {
				 for ( int i = 0; i < nTotalPage - nCount; i++ )
				 {

					 comboBox3->Items->Add( (nCount+1+i).ToString() );
				 }
			 }
			 this->comboBox3->Text = m_nCurShowPage.ToString();
			 if (m_nCurShowPage > nTotalPage )
			 {
				 m_nCurShowPage =1;
				 comboBox3->Text = L"1";

			 }
			 if ( 0 == nTotalPage )
			 {
				 m_nCurShowPage =1;
				 comboBox3->Text = L"0";
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

private: System::Void comboBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {

		 }
private: System::Void comboBox3_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			
		 }
private: System::Void comboBox2_SelectionChangeCommitted(System::Object^  sender, System::EventArgs^  e) 
		 {
			 m_nRowNumPerPage = int::Parse(comboBox2->SelectedItem->ToString());
			 //m_nCurShowPage = atoi(Tools::String2string(comboBox3->Text).c_str());
			 std::vector<PlatformStru_TradeInfo> vecTrade;
			 m_pDataInfo->GetSortedTrade(vecTrade);
			 UpdatePageBar(vecTrade);
			 std::vector<PlatformStru_TradeInfo> vecShowTrade;
			 GetShowData(vecTrade,vecShowTrade);
			 loadTradeView(vecShowTrade);
		 }
private: System::Void comboBox3_SelectionChangeCommitted(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //m_nRowNumPerPage = atoi(Tools::String2string(comboBox2->Text).c_str());
			 m_nCurShowPage = int::Parse(comboBox3->SelectedItem->ToString());
			 CFileOpr::getObj().writelocallog("TradeConfirmForm.log","comboBox3_SelectionChangeCommitted m_nCurShowPage =%d",m_nCurShowPage);
			 std::vector<PlatformStru_TradeInfo> vecTrade;
			 m_pDataInfo->GetSortedTrade(vecTrade);
			 std::vector<PlatformStru_TradeInfo> vecShowTrade;
			 GetShowData(vecTrade,vecShowTrade);
			 loadTradeView(vecShowTrade);
             CFileOpr::getObj().writelocallog("TradeConfirmForm.log","comboBox3_SelectionChangeCommitted loadTradeView");
			 UpdatePageBar(vecTrade);
			 CFileOpr::getObj().writelocallog("TradeConfirmForm.log","comboBox3_SelectionChangeCommitted UpdatePageBar");
		 }
private: System::Void button_pre_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if ( m_nCurShowPage > 1 )
			 {
				 m_nCurShowPage -= 1;

				 std::vector<PlatformStru_TradeInfo> vecTrade;
				 m_pDataInfo->GetSortedTrade(vecTrade);
				 CFileOpr::getObj().writelocallog("TradeConfirmForm.log","button_pre_Click GetTradeVec m_nCurShowPage =%d",m_nCurShowPage);
				 std::vector<PlatformStru_TradeInfo> vecShowTrade;
				 GetShowData(vecTrade,vecShowTrade);
				 loadTradeView(vecShowTrade);
				 CFileOpr::getObj().writelocallog("TradeConfirmForm.log","button_pre_Click loadTradeView");
				 UpdatePageBar(vecTrade);
				  CFileOpr::getObj().writelocallog("TradeConfirmForm.log","button_pre_Click UpdatePageBar");
			 }
		 }

private: System::Void button_next_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 std::vector<PlatformStru_TradeInfo> vecTrade;
			 m_pDataInfo->GetSortedTrade(vecTrade);
			 CFileOpr::getObj().writelocallog("TradeConfirmForm.log","button_next_Click GetTradeVec");
			 if ( m_nCurShowPage < (int)vecTrade.size() )
			 {
				 m_nCurShowPage += 1;

				  CFileOpr::getObj().writelocallog("TradeConfirmForm.log","button_next_Click m_nCurShowPage =%d",m_nCurShowPage);
				 std::vector<PlatformStru_TradeInfo> vecShowTrade;
				 GetShowData(vecTrade,vecShowTrade);
				 CFileOpr::getObj().writelocallog("TradeConfirmForm.log","button_next_Click GetShowData");
				 loadTradeView(vecShowTrade);
				 CFileOpr::getObj().writelocallog("TradeConfirmForm.log","button_next_Click loadTradeView");
				 UpdatePageBar(vecTrade);
				 CFileOpr::getObj().writelocallog("TradeConfirmForm.log","button_next_Click UpdatePageBar");
			 }
		 }
		 //static bool InnerSort0(const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2) 
		 //{			
			// return m1.ExchangeID < m2.ExchangeID;				 
		 //}
		 //static bool InnerSort1(const PlatformStru_TradeInfo & m1, const PlatformStru_TradeInfo & m2) 
		 //{			
			// return m1.TradeID < m2.TradeID;				 
		 //}

private: System::Void dataGridView1_Sorted(System::Object^  sender, System::EventArgs^  e) 
		 {
			
			// DataGridView^ dataGridView = (DataGridView^)(sender);
			// DataGridViewColumn^ SColumn = dataGridView1->SortedColumn;

			//
			//// dataGridView1->FirstDisplayedCell = dataGridView1->CurrentCell;
			// std::vector<PlatformStru_TradeInfo> vecTrade;
			// GetTradeVec(vecTrade);
   //         // 排序
			// if (SColumn->HeaderCell->SortGlyphDirection != SortOrder::Ascending)
			// {
			//	 SColumn->HeaderCell->SortGlyphDirection = SortOrder::Ascending;

			//	 switch(SColumn->Index)
			//	 {
			//	 case 0:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_0());
			//		 break;
			//	 case 1:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_1());
			//		 break;
			//	 case 2:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_2());
			//		 break;
			//	 case 3:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_3());
			//		 break;
			//	 case 4:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_4());
			//		 break;
			//	 case 5:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_5());
			//		 break;
			//	 case 6:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_6());
			//		 break;
			//	 case 7:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_7());
			//		 break;
			//	 case 8:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_8());
			//		 break;
			//	 case 9:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_9());
			//		 break;
			//	 case 10:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_10());
			//		 break;
			//	 case 11:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_11());
			//		 break;
			//	 case 12:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_12());
			//		 break;
			//	 case 13:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_13());
			//		 break;
			//	 case 14:
			//		 sort(vecTrade.begin(),vecTrade.end(),AComparer_14());
			//		 break;


			//	 }
			// }
			// else
			// {
			//     SColumn->HeaderCell->SortGlyphDirection = SortOrder::Descending;

			//	 switch(SColumn->Index)
			//	 {
			//	 case 0:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_0());
			//		 break;
			//	 case 1:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_1());
			//		 break;
			//	 case 2:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_2());
			//		 break;
			//	 case 3:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_3());
			//		 break;
			//	 case 4:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_4());
			//		 break;
			//	 case 5:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_5());
			//		 break;
			//	 case 6:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_6());
			//		 break;
			//	 case 7:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_7());
			//		 break;
			//	 case 8:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_8());
			//		 break;
			//	 case 9:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_9());
			//		 break;
			//	 case 10:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_10());
			//		 break;
			//	 case 11:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_11());
			//		 break;
			//	 case 12:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_12());
			//		 break;
			//	 case 13:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_13());
			//		 break;
			//	 case 14:
			//		 sort(vecTrade.begin(),vecTrade.end(),Comparer_14());
			//		 break;


			//	 }
			// }
			// 
			//
			//
			//
			// 
			// UpdatePageBar(vecTrade);
			// std::vector<PlatformStru_TradeInfo> vecShowTrade;
			// GetShowData(vecTrade,vecShowTrade);
			// loadTradeView(vecShowTrade);
		
			
			 
		 }
private: System::Void dataGridView1_ColumnHeaderMouseClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellMouseEventArgs^  e) 
		 {
			 DataGridView^ dataGridView = (DataGridView^)(sender);
			 DataGridViewColumn^ SColumn = dataGridView1->Columns[e->ColumnIndex];


			 // dataGridView1->FirstDisplayedCell = dataGridView1->CurrentCell;
			 std::vector<PlatformStru_TradeInfo> vecTrade;
			 GetTradeVec(vecTrade);
			 // 排序

			 if (SColumn->HeaderCell->SortGlyphDirection == SortOrder::Ascending)
			 {
				 SColumn->HeaderCell->SortGlyphDirection = SortOrder::Descending;

				 switch(SColumn->Index)
				 {
				 case 0:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_0());
					 break;
				 case 1:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_1());
					 break;
				 case 2:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_2());
					 break;
				 case 3:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_3());
					 break;
				 case 4:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_4());
					 break;
				 case 5:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_5());
					 break;
				 case 6:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_6());
					 break;
				 case 7:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_7());
					 break;
				 case 8:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_8());
					 break;
				 case 9:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_9());
					 break;
				 case 10:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_10());
					 break;
				 case 11:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_11());
					 break;
				 case 12:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_12());
					 break;
				 case 13:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_13());
					 break;
				 case 14:
					 sort(vecTrade.begin(),vecTrade.end(),AComparer_14());
					 break;


				 }
			 }
			 else
			 {
				 SColumn->HeaderCell->SortGlyphDirection = SortOrder::Ascending;

				 switch(SColumn->Index)
				 {
				 case 0:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_0());
					 break;
				 case 1:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_1());
					 break;
				 case 2:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_2());
					 break;
				 case 3:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_3());
					 break;
				 case 4:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_4());
					 break;
				 case 5:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_5());
					 break;
				 case 6:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_6());
					 break;
				 case 7:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_7());
					 break;
				 case 8:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_8());
					 break;
				 case 9:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_9());
					 break;
				 case 10:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_10());
					 break;
				 case 11:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_11());
					 break;
				 case 12:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_12());
					 break;
				 case 13:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_13());
					 break;
				 case 14:
					 sort(vecTrade.begin(),vecTrade.end(),Comparer_14());
					 break;


				 }
			 }

             
			 m_pDataInfo->SaveSortedTrade(vecTrade);


			 UpdatePageBar(vecTrade);
			 std::vector<PlatformStru_TradeInfo> vecShowTrade;
			 GetShowData(vecTrade,vecShowTrade);
			 loadTradeView(vecShowTrade);
		 }
};
}

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Client {

	/// <summary>
	/// Summary for RiskEventRecord
	/// </summary>
	public ref class RiskEventRecord : public System::Windows::Forms::UserControl
	{
	public:
		RiskEventRecord(void)
		{
			m_nCurrentAccountID = -1;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_UserRiskEvt;
	public: 
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Account_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  RiskType_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Detail_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Indicator_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Processor_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Incharge_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  RiskEvtStatus_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Action_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Time_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Remarks_UserRiskEvt;
		
		 
		int m_nCurrentAccountID; //当前显示的账户
	public:
		void SetCurrentAccountID(int nCurrentAccountID)
		{
			m_nCurrentAccountID = nCurrentAccountID;
		}
		int GetCurrentAccountID()
		{
			return m_nCurrentAccountID;
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RiskEventRecord()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	protected: 
	public: System::Windows::Forms::DataGridView^  dataGridView1;









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
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->ID_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Account_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->RiskType_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Detail_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Indicator_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Processor_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Incharge_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->RiskEvtStatus_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Action_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Time_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Remarks_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(3, 4);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"账户风险记录";
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToResizeRows = false;
			this->dataGridView1->BackgroundColor = System::Drawing::SystemColors::Control;
			this->dataGridView1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(11) {this->ID_UserRiskEvt, 
				this->Account_UserRiskEvt, this->RiskType_UserRiskEvt, this->Detail_UserRiskEvt, this->Indicator_UserRiskEvt, this->Processor_UserRiskEvt, 
				this->Incharge_UserRiskEvt, this->RiskEvtStatus_UserRiskEvt, this->Action_UserRiskEvt, this->Time_UserRiskEvt, this->Remarks_UserRiskEvt});
			this->dataGridView1->Location = System::Drawing::Point(5, 35);
			this->dataGridView1->MultiSelect = false;
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(570, 336);
			this->dataGridView1->TabIndex = 1;
			// 
			// ID_UserRiskEvt
			// 
			this->ID_UserRiskEvt->HeaderText = L"风险事件ID";
			this->ID_UserRiskEvt->Name = L"ID_UserRiskEvt";
			this->ID_UserRiskEvt->ReadOnly = true;
			// 
			// Account_UserRiskEvt
			// 
			this->Account_UserRiskEvt->HeaderText = L"账户";
			this->Account_UserRiskEvt->Name = L"Account_UserRiskEvt";
			this->Account_UserRiskEvt->ReadOnly = true;
			// 
			// RiskType_UserRiskEvt
			// 
			this->RiskType_UserRiskEvt->HeaderText = L"风险指标类型";
			this->RiskType_UserRiskEvt->Name = L"RiskType_UserRiskEvt";
			this->RiskType_UserRiskEvt->ReadOnly = true;
			// 
			// Detail_UserRiskEvt
			// 
			this->Detail_UserRiskEvt->HeaderText = L"风险详情";
			this->Detail_UserRiskEvt->Name = L"Detail_UserRiskEvt";
			this->Detail_UserRiskEvt->ReadOnly = true;
			// 
			// Indicator_UserRiskEvt
			// 
			this->Indicator_UserRiskEvt->HeaderText = L"指标值";
			this->Indicator_UserRiskEvt->Name = L"Indicator_UserRiskEvt";
			this->Indicator_UserRiskEvt->ReadOnly = true;
			// 
			// Processor_UserRiskEvt
			// 
			this->Processor_UserRiskEvt->HeaderText = L"处理人";
			this->Processor_UserRiskEvt->Name = L"Processor_UserRiskEvt";
			this->Processor_UserRiskEvt->ReadOnly = true;
			// 
			// Incharge_UserRiskEvt
			// 
			this->Incharge_UserRiskEvt->HeaderText = L"受理人";
			this->Incharge_UserRiskEvt->Name = L"Incharge_UserRiskEvt";
			this->Incharge_UserRiskEvt->ReadOnly = true;
			// 
			// RiskEvtStatus_UserRiskEvt
			// 
			this->RiskEvtStatus_UserRiskEvt->HeaderText = L"风险事件状态";
			this->RiskEvtStatus_UserRiskEvt->Name = L"RiskEvtStatus_UserRiskEvt";
			this->RiskEvtStatus_UserRiskEvt->ReadOnly = true;
			// 
			// Action_UserRiskEvt
			// 
			this->Action_UserRiskEvt->HeaderText = L"动作";
			this->Action_UserRiskEvt->Name = L"Action_UserRiskEvt";
			this->Action_UserRiskEvt->ReadOnly = true;
			// 
			// Time_UserRiskEvt
			// 
			this->Time_UserRiskEvt->HeaderText = L"时间";
			this->Time_UserRiskEvt->Name = L"Time_UserRiskEvt";
			this->Time_UserRiskEvt->ReadOnly = true;
			// 
			// Remarks_UserRiskEvt
			// 
			this->Remarks_UserRiskEvt->HeaderText = L"备注";
			this->Remarks_UserRiskEvt->Name = L"Remarks_UserRiskEvt";
			this->Remarks_UserRiskEvt->ReadOnly = true;
			// 
			// RiskEventRecord
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::GrayText;
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->label1);
			this->Name = L"RiskEventRecord";
			this->Size = System::Drawing::Size(613, 416);
			this->SizeChanged += gcnew System::EventHandler(this, &RiskEventRecord::RiskEventRecord_SizeChanged);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void RiskEventRecord_SizeChanged(System::Object^  sender, System::EventArgs^  e) {
				 int wid=this->Size.Width;
				 int hei=this->Size.Height;
				 int pox=this->Location.X;
				 int poy=this->Location.Y;

				 dataGridView1->SetBounds(0, 20, wid, hei-20);
			 }
};
}

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;


namespace StrategyTester {

    struct sQueryCondition
    {
        string m_start_date;
        string m_end_date;
        bool   m_bSameDay;
    };

	/// <summary>
	/// Summary for HistoryQueryForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class HistoryQueryForm : public System::Windows::Forms::Form
	{
	public:
		HistoryQueryForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_sCondition = new sQueryCondition;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~HistoryQueryForm()
		{
			if (components)
			{
				delete components;
			}

            if(m_sCondition)
            {
                delete m_sCondition;
                m_sCondition = NULL;
            }
		}
    private: System::Windows::Forms::GroupBox^  groupBox1;
    protected: 
    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::DateTimePicker^  dateTimePicker2;
    private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
    private: System::Windows::Forms::Button^  button_Cancel;
    private: System::Windows::Forms::Button^  button_OK;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

        //////////////////////////////////////////////////////////////////////////
        sQueryCondition* m_sCondition;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->dateTimePicker2 = (gcnew System::Windows::Forms::DateTimePicker());
            this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->button_Cancel = (gcnew System::Windows::Forms::Button());
            this->button_OK = (gcnew System::Windows::Forms::Button());
            this->groupBox1->SuspendLayout();
            this->SuspendLayout();
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->label3);
            this->groupBox1->Controls->Add(this->dateTimePicker2);
            this->groupBox1->Controls->Add(this->dateTimePicker1);
            this->groupBox1->Controls->Add(this->label2);
            this->groupBox1->Controls->Add(this->label1);
            this->groupBox1->Location = System::Drawing::Point(12, 12);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(466, 67);
            this->groupBox1->TabIndex = 0;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"时间范围";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(211, 31);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(29, 12);
            this->label3->TabIndex = 2;
            this->label3->Text = L"----";
            // 
            // dateTimePicker2
            // 
            this->dateTimePicker2->CustomFormat = L"yyyyMMdd";
            this->dateTimePicker2->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
            this->dateTimePicker2->Location = System::Drawing::Point(317, 28);
            this->dateTimePicker2->Name = L"dateTimePicker2";
            this->dateTimePicker2->Size = System::Drawing::Size(114, 21);
            this->dateTimePicker2->TabIndex = 1;
            // 
            // dateTimePicker1
            // 
            this->dateTimePicker1->CustomFormat = L"yyyyMMdd";
            this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
            this->dateTimePicker1->Location = System::Drawing::Point(84, 27);
            this->dateTimePicker1->Name = L"dateTimePicker1";
            this->dateTimePicker1->Size = System::Drawing::Size(111, 21);
            this->dateTimePicker1->TabIndex = 1;
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(258, 32);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(53, 12);
            this->label2->TabIndex = 0;
            this->label2->Text = L"结束时间";
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(24, 32);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(53, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"开始时间";
            // 
            // button_Cancel
            // 
            this->button_Cancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
            this->button_Cancel->Location = System::Drawing::Point(390, 91);
            this->button_Cancel->Name = L"button_Cancel";
            this->button_Cancel->Size = System::Drawing::Size(75, 23);
            this->button_Cancel->TabIndex = 1;
            this->button_Cancel->Text = L"取消";
            this->button_Cancel->UseVisualStyleBackColor = true;
            // 
            // button_OK
            // 
            this->button_OK->DialogResult = System::Windows::Forms::DialogResult::OK;
            this->button_OK->Location = System::Drawing::Point(279, 91);
            this->button_OK->Name = L"button_OK";
            this->button_OK->Size = System::Drawing::Size(75, 23);
            this->button_OK->TabIndex = 1;
            this->button_OK->Text = L"确定";
            this->button_OK->UseVisualStyleBackColor = true;
            this->button_OK->Click += gcnew System::EventHandler(this, &HistoryQueryForm::button_OK_Click);
            // 
            // HistoryQueryForm
            // 
            this->AcceptButton = this->button_OK;
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->CancelButton = this->button_Cancel;
            this->ClientSize = System::Drawing::Size(493, 126);
            this->Controls->Add(this->button_Cancel);
            this->Controls->Add(this->button_OK);
            this->Controls->Add(this->groupBox1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"HistoryQueryForm";
            this->Text = L"历史查询";
            this->groupBox1->ResumeLayout(false);
            this->groupBox1->PerformLayout();
            this->ResumeLayout(false);

        }
#pragma endregion
    private: System::Void button_OK_Click(System::Object^  sender, System::EventArgs^  e) {
                 memset(m_sCondition,0,sizeof(sQueryCondition));

                 String^ start_date = dateTimePicker1->Text;
                 String^ end_date = dateTimePicker2->Text;

                 DateTime time1 = Convert::ToDateTime(dateTimePicker1->Value.Date.ToString("yyyy-MM-dd"));
                 DateTime time2 = Convert::ToDateTime(dateTimePicker2->Value.Date.ToString("yyyy-MM-dd")); 
                 if (DateTime::Compare(time1,time2)<0) //判断日期大小
                     m_sCondition->m_bSameDay = false;
                 else if (DateTime::Compare(time1,time2)==0)
                     m_sCondition->m_bSameDay = true;                     
                 else if (DateTime::Compare(time1,time2)>0)
                 {
                     MessageBox::Show("起止日期顺序错误！");
                 }

                 m_sCondition->m_start_date = (char*)(Marshal::StringToHGlobalAnsi(start_date)).ToPointer();
                 m_sCondition->m_end_date = (char*)(Marshal::StringToHGlobalAnsi(end_date)).ToPointer();

                 this->Close();
             }

         //////////////////////////////////////////////////////////////////////////
         public: sQueryCondition* GetQueryCondition()
                 {
                     return m_sCondition;
                 } 
};
}

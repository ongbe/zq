#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltRisk {

	/// <summary>
	/// Summary for FormRiskPlanSetup_Del
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormRiskPlanSetup_Del : public System::Windows::Forms::Form
	{
	public:
		FormRiskPlanSetup_Del(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormRiskPlanSetup_Del()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::Button^  button_OK;
    private: System::Windows::Forms::Button^  button_Cancel;
    protected: 

    protected: 


    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;

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
            this->button_OK = (gcnew System::Windows::Forms::Button());
            this->button_Cancel = (gcnew System::Windows::Forms::Button());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->SuspendLayout();
            // 
            // button_OK
            // 
            this->button_OK->Location = System::Drawing::Point(62, 101);
            this->button_OK->Name = L"button_OK";
            this->button_OK->Size = System::Drawing::Size(75, 23);
            this->button_OK->TabIndex = 1;
            this->button_OK->Text = L"确认";
            this->button_OK->UseVisualStyleBackColor = true;
            this->button_OK->Click += gcnew System::EventHandler(this, &FormRiskPlanSetup_Del::button_OK_Click);
            // 
            // button_Cancel
            // 
            this->button_Cancel->Location = System::Drawing::Point(168, 101);
            this->button_Cancel->Name = L"button_Cancel";
            this->button_Cancel->Size = System::Drawing::Size(75, 23);
            this->button_Cancel->TabIndex = 2;
            this->button_Cancel->Text = L"取消";
            this->button_Cancel->UseVisualStyleBackColor = true;
            this->button_Cancel->Click += gcnew System::EventHandler(this, &FormRiskPlanSetup_Del::button_Cancel_Click);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(32, 30);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(131, 12);
            this->label1->TabIndex = 0;
            this->label1->Text = L"确认要删除此风控方案\?";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(34, 59);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(233, 12);
            this->label2->TabIndex = 3;
            this->label2->Text = L"“是”点击确认按钮  “否”点击取消按钮";
            // 
            // FormRiskPlanSetup_Del
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(304, 146);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->button_Cancel);
            this->Controls->Add(this->button_OK);
            this->Controls->Add(this->label1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"FormRiskPlanSetup_Del";
            this->Text = L"确认提示";
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
private: System::Void button_OK_Click(System::Object^  sender, System::EventArgs^  e) {
             this->DialogResult = ::DialogResult::OK;
             Close();
         }
private: System::Void button_Cancel_Click(System::Object^  sender, System::EventArgs^  e) {
             Close();
         }
};
}

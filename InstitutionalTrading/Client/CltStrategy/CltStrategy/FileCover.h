#pragma once
#include "Tools.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltStrategy {

	/// <summary>
	/// Summary for FileCover
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FileCover : public System::Windows::Forms::Form
	{
	public:
		FileCover(string& strFileName)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_fileName = Tools::string2String(strFileName.c_str());
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FileCover()
		{
			if (components)
			{
				delete components;
			}
		}
	String^   m_fileName;
	private: System::Windows::Forms::TextBox^  textBox1;
	protected: 
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button_yes;
	private: System::Windows::Forms::Button^  button_no;

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
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button_yes = (gcnew System::Windows::Forms::Button());
			this->button_no = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(22, 52);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->textBox1->Size = System::Drawing::Size(491, 214);
			this->textBox1->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(20, 26);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(317, 12);
			this->label1->TabIndex = 1;
			this->label1->Text = L"文件夹中已存在以下文件，是否用导入文件替换已有文件？";
			// 
			// button_yes
			// 
			this->button_yes->Location = System::Drawing::Point(103, 300);
			this->button_yes->Name = L"button_yes";
			this->button_yes->Size = System::Drawing::Size(75, 23);
			this->button_yes->TabIndex = 2;
			this->button_yes->Text = L"是";
			this->button_yes->UseVisualStyleBackColor = true;
			this->button_yes->Click += gcnew System::EventHandler(this, &FileCover::button_yes_Click);
			// 
			// button_no
			// 
			this->button_no->Location = System::Drawing::Point(323, 300);
			this->button_no->Name = L"button_no";
			this->button_no->Size = System::Drawing::Size(75, 23);
			this->button_no->TabIndex = 3;
			this->button_no->Text = L"否";
			this->button_no->UseVisualStyleBackColor = true;
			this->button_no->Click += gcnew System::EventHandler(this, &FileCover::button_no_Click);
			// 
			// FileCover
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(564, 360);
			this->Controls->Add(this->button_no);
			this->Controls->Add(this->button_yes);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->textBox1);
			this->Name = L"FileCover";
			this->Text = L"确认文件替换";
			this->Load += gcnew System::EventHandler(this, &FileCover::FileCover_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void FileCover_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				  this->CenterToParent();
				  textBox1->Text = m_fileName;
			 }
	private: System::Void button_yes_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
				 this->Close();
			 }
private: System::Void button_no_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Close();
		 }
};
}

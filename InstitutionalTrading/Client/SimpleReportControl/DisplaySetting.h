#pragma once
#include <cliext/set>
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace SimpleReportControl {

	/// <summary>
	/// Summary for DisplaySetting
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class DisplaySetting : public System::Windows::Forms::Form
	{
	public:
		DisplaySetting(void)
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
		~DisplaySetting()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ListView^  listView1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	private: cliext::set<String^>^ mAllColumn;
	private: cliext::set<String^>^ mHideColumn;
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
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// listView1
			// 
			this->listView1->CheckBoxes = true;
			this->listView1->Location = System::Drawing::Point(-1, 0);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(245, 309);
			this->listView1->TabIndex = 0;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::List;
			this->listView1->SelectedIndexChanged += gcnew System::EventHandler(this, &DisplaySetting::listView1_SelectedIndexChanged);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(27, 327);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"确定";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &DisplaySetting::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(119, 327);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"取消";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &DisplaySetting::button2_Click);
			// 
			// DisplaySetting
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(244, 362);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->listView1);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"DisplaySetting";
			this->ShowInTaskbar = false;
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->Text = L"显示设置";
			this->Load += gcnew System::EventHandler(this, &DisplaySetting::DisplaySetting_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void listView1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->DialogResult = ::DialogResult::OK;
				 this->Close();
			 }
	private: System::Void DisplaySetting_Load(System::Object^  sender, System::EventArgs^  e) {
				 for each (String^ ls in mAllColumn)
				 {
					 ListViewItem ^ lItem = gcnew ListViewItem(ls);
					 lItem->Checked = true;
					 if(mHideColumn != nullptr)
					 {
						 if(mHideColumn->find(ls) != mHideColumn->end())
							 lItem->Checked = false;
						 else 
							 lItem->Checked = true;
					 }
					 listView1->Items->Add(lItem);
				 }
			 }
	public: System::Void Clear()
			{
				if(mAllColumn != nullptr)
					mAllColumn->clear();
				if(mHideColumn != nullptr)
					mHideColumn->clear();
			}
	public: System::Void AddColumn(String^ lColumn)
			{
				if(lColumn == nullptr)
					return;
				if(mAllColumn == nullptr)
					mAllColumn = gcnew cliext::set<String^>();
				mAllColumn->insert(lColumn);
			}
	public: System::Void AddHideColumn(String^ lColumn)
			{
				if(lColumn == nullptr)
					return;
				if(mHideColumn == nullptr)
					mHideColumn = gcnew cliext::set<String^>();
				mHideColumn->insert(lColumn);
			}
	public: System::Void GetUnSelectHideItem(cliext::set<String^>^ % nUnSelectHideColumn)
			{
				for each(ListViewItem ^ lItem in listView1->Items)
				{
					if(lItem->Checked== false)
						nUnSelectHideColumn->insert(lItem->Text);						
				}

			}
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->DialogResult = ::DialogResult::Cancel;
				 this->Close();
			 }
};
}

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

	/// <summary>
	/// Summary for BaseForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class BaseForm : public WeifenLuo::WinFormsUI::Docking::DockContent
	{
	public:
		BaseForm(void)
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
		~BaseForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: SimpleReportControl::SimpleReportControlControl^  simpleReportControlControl1;
	protected: 
	private: System::ComponentModel::IContainer^  components;

	protected: 





	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->simpleReportControlControl1 = (gcnew SimpleReportControl::SimpleReportControlControl());
			this->SuspendLayout();
			// 
			// simpleReportControlControl1
			// 
			this->simpleReportControlControl1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->simpleReportControlControl1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->simpleReportControlControl1->Location = System::Drawing::Point(0, 0);
			this->simpleReportControlControl1->Name = L"simpleReportControlControl1";
			this->simpleReportControlControl1->RealDataTable = nullptr;
			this->simpleReportControlControl1->Size = System::Drawing::Size(292, 266);
			this->simpleReportControlControl1->TabIndex = 1;
			// 
			// BaseForm
			// 
			this->AllowEndUserDocking = false;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 266);
			this->CloseButton = false;
			this->CloseButtonVisible = false;
			this->Controls->Add(this->simpleReportControlControl1);
			this->DockAreas = WeifenLuo::WinFormsUI::Docking::DockAreas::Document;
			this->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->Name = L"BaseForm";
			this->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->ShowHint = WeifenLuo::WinFormsUI::Docking::DockState::Document;
			this->TabText = L"BaseForm";
			this->Text = L"BaseForm";
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}

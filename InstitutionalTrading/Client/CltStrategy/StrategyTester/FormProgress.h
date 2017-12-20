#pragma once

#include "TestEntity.h"
#include "TestRecordForm.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace StrategyTester {

	/// <summary>
	/// Summary for FormProgress
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormProgress : public System::Windows::Forms::Form
	{
	public:
		FormProgress(CTestEntity* pTestEntity)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pTestEntity = pTestEntity;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormProgress()
		{
			if (components)
			{
				delete components;
			}
		}
	protected: CTestEntity*		m_pTestEntity;

	private: System::Windows::Forms::ProgressBar^  progressBar1;
    private: System::Windows::Forms::Timer^  timer1;
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
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(2, 2);
			this->progressBar1->Maximum = 32400;
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(420, 32);
			this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBar1->TabIndex = 0;
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &FormProgress::timer1_Tick);
			// 
			// FormProgress
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(424, 36);
			this->Controls->Add(this->progressBar1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"FormProgress";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"进度条窗口";
			this->Load += gcnew System::EventHandler(this, &FormProgress::FormProgress_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	public: void ResetRange(int nMin, int nMax)
		{
			progressBar1->Minimum = nMin;
			progressBar1->Maximum = nMax;
		}
	public: void ResetPos(int nPos)
		{
			progressBar1->Value = nPos;
		}
	private: System::Void FormProgress_Load(System::Object^  sender, System::EventArgs^  e) {
				 this->CenterToParent();
				 timer1->Interval = 500;
				 timer1->Start();
			 }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
			 if(m_pTestEntity->GetRunStatus()!=CTestEntity::conRunFinish) {
				 ResetPos(m_pTestEntity->GetCurrQuotPos());
			 }
			 else {
				 timer1->Stop();
				 ResetPos(32400-1);
				 //MessageBox::Show(this, "策略测试完成。");
                 TestRecordForm^ dlg = gcnew TestRecordForm(false);
                 if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
                 {

                 }
				 this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
				 this->Close();
			 }
         }
	};
}

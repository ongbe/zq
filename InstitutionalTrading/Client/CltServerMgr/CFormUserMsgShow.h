#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


#include "CommonDef.h"
#include "CommonStruct.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "CFormUserMsgShowData.h"


namespace CltServerMgr {

	/// <summary>
	/// Summary for CFormUserMsgShow
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CFormUserMsgShow : public System::Windows::Forms::Form
	{
	public:
		CFormUserMsgShow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			save = NULL;
			m_bHasExit = false;
			save = CFormUserMsgShowData::NewInstance();
		}

		void AppendUserMsg(UserMsgStru& msg, System::Windows::Forms::Form^ owner) {
			save->AppendUserMsg(msg);
			save->MoveToLast();
			ShowMsg(msg, save->GetMsgCount()>1 ? 1 : 4 );
			if(!this->Visible)
				this->Show(owner);
		};
		void ResetExit(bool bIsExit) {
			m_bHasExit = bIsExit;
		};

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CFormUserMsgShow()
		{
			if (components)
			{
				delete components;
			}
			CFormUserMsgShowData::ReleaseInstance(save);
		}
	
	private: CFormUserMsgShowData* save;
	private: bool m_bHasExit;

	private: System::Windows::Forms::Button^  buttonConfirm;
	protected: 
	private: System::Windows::Forms::Button^  buttonNextItem;
	private: System::Windows::Forms::Button^  buttonPrevItem;
	private: System::Windows::Forms::TextBox^  textBoxContent;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  labelTitle;
	private: System::Windows::Forms::Label^  label1;

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
			this->buttonConfirm = (gcnew System::Windows::Forms::Button());
			this->buttonNextItem = (gcnew System::Windows::Forms::Button());
			this->buttonPrevItem = (gcnew System::Windows::Forms::Button());
			this->textBoxContent = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->labelTitle = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// buttonConfirm
			// 
			this->buttonConfirm->Location = System::Drawing::Point(256, 209);
			this->buttonConfirm->Name = L"buttonConfirm";
			this->buttonConfirm->Size = System::Drawing::Size(66, 22);
			this->buttonConfirm->TabIndex = 13;
			this->buttonConfirm->Text = L"确定";
			this->buttonConfirm->UseVisualStyleBackColor = true;
			this->buttonConfirm->Click += gcnew System::EventHandler(this, &CFormUserMsgShow::buttonConfirm_Click);
			// 
			// buttonNextItem
			// 
			this->buttonNextItem->Enabled = false;
			this->buttonNextItem->Location = System::Drawing::Point(65, 209);
			this->buttonNextItem->Name = L"buttonNextItem";
			this->buttonNextItem->Size = System::Drawing::Size(53, 22);
			this->buttonNextItem->TabIndex = 12;
			this->buttonNextItem->Text = L"下一条";
			this->buttonNextItem->UseVisualStyleBackColor = true;
			this->buttonNextItem->Click += gcnew System::EventHandler(this, &CFormUserMsgShow::buttonNextItem_Click);
			// 
			// buttonPrevItem
			// 
			this->buttonPrevItem->Enabled = false;
			this->buttonPrevItem->Location = System::Drawing::Point(7, 209);
			this->buttonPrevItem->Name = L"buttonPrevItem";
			this->buttonPrevItem->Size = System::Drawing::Size(53, 22);
			this->buttonPrevItem->TabIndex = 11;
			this->buttonPrevItem->Text = L"上一条";
			this->buttonPrevItem->UseVisualStyleBackColor = true;
			this->buttonPrevItem->Click += gcnew System::EventHandler(this, &CFormUserMsgShow::buttonPrevItem_Click);
			// 
			// textBoxContent
			// 
			this->textBoxContent->Location = System::Drawing::Point(7, 45);
			this->textBoxContent->Multiline = true;
			this->textBoxContent->Name = L"textBoxContent";
			this->textBoxContent->ReadOnly = true;
			this->textBoxContent->Size = System::Drawing::Size(315, 158);
			this->textBoxContent->TabIndex = 10;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(7, 30);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(35, 12);
			this->label2->TabIndex = 9;
			this->label2->Text = L"内容:";
			// 
			// labelTitle
			// 
			this->labelTitle->AutoSize = true;
			this->labelTitle->Location = System::Drawing::Point(48, 9);
			this->labelTitle->Name = L"labelTitle";
			this->labelTitle->Size = System::Drawing::Size(35, 12);
			this->labelTitle->TabIndex = 8;
			this->labelTitle->Text = L"title";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(7, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 12);
			this->label1->TabIndex = 7;
			this->label1->Text = L"标题:";
			// 
			// CFormUserMsgShow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(329, 240);
			this->Controls->Add(this->buttonConfirm);
			this->Controls->Add(this->buttonNextItem);
			this->Controls->Add(this->buttonPrevItem);
			this->Controls->Add(this->textBoxContent);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->labelTitle);
			this->Controls->Add(this->label1);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"CFormUserMsgShow";
			this->Text = L"用户消息";
			this->Load += gcnew System::EventHandler(this, &CFormUserMsgShow::CFormUserMsgShow_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &CFormUserMsgShow::CFormUserMsgShow_FormClosed);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &CFormUserMsgShow::CFormUserMsgShow_FormClosing);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: void ShowMsg(UserMsgStru& msg, int nMode) {
				 this->textBoxContent->Text = gcnew String(msg.Msg);
				 this->labelTitle->Text = gcnew String(msg.Title);
				 switch(nMode) {
				 case 1:
					 this->buttonNextItem->Enabled = false;
					 this->buttonPrevItem->Enabled = true;
					 break;
				 case 2:
					 this->buttonNextItem->Enabled = true;
					 this->buttonPrevItem->Enabled = false;
					 break;
				 case 4:
					 this->buttonNextItem->Enabled = false;
					 this->buttonPrevItem->Enabled = false;
					 break;
				 case 0:
				 default:
					 this->buttonNextItem->Enabled = true;
					 this->buttonPrevItem->Enabled = true;
					 break;
				 }
			 }
	private: System::Void CFormUserMsgShow_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void CFormUserMsgShow_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 }
	private: System::Void CFormUserMsgShow_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
				 if(!m_bHasExit) {
					 e->Cancel = true;
					 this->Hide();
				 }
			 }
	private: System::Void buttonPrevItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(save->GetMsgCount()>0) {
					 UserMsgStru msg;
					 if(save->GetPrevUserMsg(msg)) {
						int nMode = save->nRowIndex==0 ? (save->GetMsgCount()==1 ? 4 : 2) : 0;
						ShowMsg(msg, nMode);
					}
				 }
			 }
	private: System::Void buttonNextItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 if(save->GetMsgCount()>0) {
					 UserMsgStru msg;
					 if(save->GetNextUserMsg(msg)) {
						int nMode = save->nRowIndex==save->GetMsgCount()-1 ? (save->GetMsgCount()==1 ? 4 : 1) : 0;
						ShowMsg(msg, nMode);
					}
				 }
			 }
	private: System::Void buttonConfirm_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->Hide();
				 save->ResetData();
			 }
};
}

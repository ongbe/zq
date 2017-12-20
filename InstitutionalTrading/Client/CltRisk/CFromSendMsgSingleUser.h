#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include <string>
#include "CommonDef.h"
#include "CommonStruct.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "Tools.h"
//#include "..\Misc\TcpLayer.h"
#include "TcpLayer.h"
#include "CCommonFunc.h"

using namespace std;

namespace CltRisk {

	/// <summary>
	/// Summary for CFromSendMsgSingleUser
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CFromSendMsgSingleUser : public System::Windows::Forms::Form
	{
	public:
		CFromSendMsgSingleUser(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			nUserID = 0;
			nTargetUserID = 0;
		}

		void ResetUser(int nUserID, int nTargetUserID, string& strTargetUser) {
			this->nUserID = nUserID;
			this->nTargetUserID = nTargetUserID;
			labelUserName->Text = Tools::string2String(strTargetUser.c_str());
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CFromSendMsgSingleUser()
		{
			if (components)
			{
				delete components;
			}
		}
	private: int nUserID;
	private: int nTargetUserID;

	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::Label^  labelUserName;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Button^  buttonSend;
	private: System::Windows::Forms::Button^  buttonCancel;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBoxTitle;
	private: System::Windows::Forms::TextBox^  textBoxMsg;


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
			this->labelUserName = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->textBoxMsg = (gcnew System::Windows::Forms::TextBox());
			this->buttonSend = (gcnew System::Windows::Forms::Button());
			this->buttonCancel = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBoxTitle = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"对象：";
			// 
			// labelUserName
			// 
			this->labelUserName->AutoSize = true;
			this->labelUserName->Location = System::Drawing::Point(59, 9);
			this->labelUserName->Name = L"labelUserName";
			this->labelUserName->Size = System::Drawing::Size(47, 12);
			this->labelUserName->TabIndex = 1;
			this->labelUserName->Text = L"交易员1";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->textBoxMsg);
			this->groupBox1->Location = System::Drawing::Point(6, 54);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(274, 181);
			this->groupBox1->TabIndex = 2;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"内容";
			// 
			// textBoxMsg
			// 
			this->textBoxMsg->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxMsg->Location = System::Drawing::Point(2, 16);
			this->textBoxMsg->MaxLength = 250;
			this->textBoxMsg->Multiline = true;
			this->textBoxMsg->Name = L"textBoxMsg";
			this->textBoxMsg->Size = System::Drawing::Size(269, 160);
			this->textBoxMsg->TabIndex = 2;
			// 
			// buttonSend
			// 
			this->buttonSend->Location = System::Drawing::Point(6, 241);
			this->buttonSend->Name = L"buttonSend";
			this->buttonSend->Size = System::Drawing::Size(55, 23);
			this->buttonSend->TabIndex = 3;
			this->buttonSend->Text = L"发送";
			this->buttonSend->UseVisualStyleBackColor = true;
			this->buttonSend->Click += gcnew System::EventHandler(this, &CFromSendMsgSingleUser::buttonSend_Click);
			// 
			// buttonCancel
			// 
			this->buttonCancel->Location = System::Drawing::Point(225, 241);
			this->buttonCancel->Name = L"buttonCancel";
			this->buttonCancel->Size = System::Drawing::Size(55, 23);
			this->buttonCancel->TabIndex = 4;
			this->buttonCancel->Text = L"取消";
			this->buttonCancel->UseVisualStyleBackColor = true;
			this->buttonCancel->Click += gcnew System::EventHandler(this, &CFromSendMsgSingleUser::buttonCancel_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 30);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(41, 12);
			this->label2->TabIndex = 5;
			this->label2->Text = L"标题：";
			// 
			// textBoxTitle
			// 
			this->textBoxTitle->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxTitle->Location = System::Drawing::Point(59, 25);
			this->textBoxTitle->MaxLength = 15;
			this->textBoxTitle->Name = L"textBoxTitle";
			this->textBoxTitle->Size = System::Drawing::Size(195, 21);
			this->textBoxTitle->TabIndex = 1;
			// 
			// CFromSendMsgSingleUser
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(285, 271);
			this->Controls->Add(this->textBoxTitle);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->buttonCancel);
			this->Controls->Add(this->buttonSend);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->labelUserName);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Name = L"CFromSendMsgSingleUser";
			this->Text = L"单人消息";
			this->Load += gcnew System::EventHandler(this, &CFromSendMsgSingleUser::CFromSendMsgSingleUser_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &CFromSendMsgSingleUser::CFromSendMsgSingleUser_FormClosed);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	protected:virtual void WndProc( Message% m ) override {

		if(m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival) {
			char *p = (char*)m.LParam.ToInt32();
			Stru_UniPkgHead head;
			
			memset(&head, 0, sizeof(head));
			memcpy(&head, p, sizeof(head));
			switch(head.cmdid) {
			case Cmd_UserMsg_Send_Rsp:
			{
				if(head.userdata1 == CF_ERROR_SUCCESS) {
					int i=0;
				}
				else {
					_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
							CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, p+sizeof(Stru_UniPkgHead));
				}
			}
				break;
			};
		}

		Form::WndProc(m);
	}

	bool SendMsg(int nUserID, int nTargetUserID, string& strTitle, string& strMsg)
	{
		UserSendMsgStru tMsg = {0};

		memset(&tMsg, 0, sizeof(tMsg));
		tMsg.OwnerUserID = nUserID;
		tMsg.ExpiredDate = GetLocalDate();
		tMsg.TargetUserCount = 1;
		tMsg.TargetUsers[0] = nTargetUserID;
		strncpy(tMsg.Title, strTitle.c_str(), sizeof(tMsg.Title)-1);
		strncpy(tMsg.Msg, strMsg.c_str(), sizeof(tMsg.Msg)-1);
		CTcpLayer::SendData(Cmd_UserMsg_Send_Req, (void*)&tMsg, sizeof(tMsg), 0, nUserID, 
				0, 0, nUserID);
		return true;
	}

	private: System::Void CFromSendMsgSingleUser_Load(System::Object^  sender, System::EventArgs^  e) {
				 IntPtr hWnd=this->Handle;
				 CTcpLayer::SubscribePkg(Cmd_UserMsg_Send_Rsp, (int)hWnd);
			 }
	private: System::Void CFromSendMsgSingleUser_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
				 IntPtr hWnd=this->Handle;
				 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
			 }
	private: System::Void buttonSend_Click(System::Object^  sender, System::EventArgs^  e) {
				 SendMsg(nUserID, nTargetUserID, 
						string(Tools::String2string(textBoxTitle->Text)), 
						string(Tools::String2string(textBoxMsg->Text)));
			 }
	private: System::Void buttonCancel_Click(System::Object^  sender, System::EventArgs^  e) {
				 Close();
			 }
};
}

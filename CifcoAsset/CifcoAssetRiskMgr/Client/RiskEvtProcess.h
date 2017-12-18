#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
#include "RiskManageStruct.h"
#include "CDataInfo.h"
#include "Tools.h"
namespace Client {

	/// <summary>
	/// Summary for RiskEvtProcess
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class RiskEvtProcess : public System::Windows::Forms::Form
	{
	public:
		RiskEvtProcess(void)
		{
			m_pdataInfo = CDataInfo::NewInstance();
			InitializeComponent();
			this->CenterToParent();
			radioButton1->Checked = true;
			m_nRadioButton = 0;
			//
			//TODO: Add the constructor code here
			//
		}
		void InitDialog()
		{
			radioButton1->Checked = true;
			m_nRadioButton = 0;
			this->textBox1->Text = L"";
			this->textBox2->Text = L"";
			this->textBox1->ReadOnly = true;

		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RiskEvtProcess()
		{
			if (components)
			{
				delete components;
			}
		}
	
	public: CDataInfo*  m_pdataInfo;
	private: System::Windows::Forms::RadioButton^  radioButton1;
	private: System::Windows::Forms::RadioButton^  radioButton2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox2;

	private: System::Windows::Forms::GroupBox^  groupBox1;
	protected: 
		int			m_nRadioButton;
		String^     m_strTextBox1_Mess1;		//通知
		String^		m_strTextBox1_Process2;     //解决 按钮


		String^     m_strTextBox2_Mess1;     //通知
		String^		m_strTextBox2_Process2;  //解决

	
		int			m_nTradeAccountID;
		int			m_nRiskEventID;
		int         m_nRiskEventSubID;
	public:
		void SetTextBox1(String^ str)
		{
			this->textBox1->Text = str;
			m_strTextBox1_Mess1  = str;
		}
		void SetRiskEvent(RiskEvent& riskEvent)
		{
			m_nTradeAccountID = riskEvent.nTradeAccountID;
			m_nRiskEventID	  = riskEvent.nRiskEventID;
			m_nRiskEventSubID = riskEvent.nRiskEventSubID;
		}
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
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Cursor = System::Windows::Forms::Cursors::Default;
			this->radioButton1->Location = System::Drawing::Point(16, 25);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(47, 16);
			this->radioButton1->TabIndex = 0;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"通知";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->CheckedChanged += gcnew System::EventHandler(this, &RiskEvtProcess::radioButton1_CheckedChanged);
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(16, 116);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(47, 16);
			this->radioButton2->TabIndex = 1;
			this->radioButton2->TabStop = true;
			this->radioButton2->Text = L"解决";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->CheckedChanged += gcnew System::EventHandler(this, &RiskEvtProcess::radioButton2_CheckedChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(162, 72);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(89, 12);
			this->label1->TabIndex = 2;
			this->label1->Text = L"已委派给交易员";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(258, 67);
			this->textBox1->Name = L"textBox1";
			this->textBox1->ReadOnly = true;
			this->textBox1->Size = System::Drawing::Size(119, 21);
			this->textBox1->TabIndex = 3;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(383, 70);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(29, 12);
			this->label2->TabIndex = 4;
			this->label2->Text = L"处理";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(140, 107);
			this->textBox2->MaxLength = 200;
			this->textBox2->Multiline = true;
			this->textBox2->Name = L"textBox2";
			this->textBox2->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox2->Size = System::Drawing::Size(291, 196);
			this->textBox2->TabIndex = 5;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->radioButton2);
			this->groupBox1->Controls->Add(this->radioButton1);
			this->groupBox1->Location = System::Drawing::Point(27, 47);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(95, 182);
			this->groupBox1->TabIndex = 6;
			this->groupBox1->TabStop = false;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(176, 318);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 7;
			this->button1->Text = L"确定";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &RiskEvtProcess::button1_Click);
			// 
			// button2
			// 
			this->button2->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->button2->Location = System::Drawing::Point(316, 318);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 8;
			this->button2->Text = L"取消";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &RiskEvtProcess::button2_Click);
			// 
			// RiskEvtProcess
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(443, 366);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label1);
			this->Location = System::Drawing::Point(293, 324);
			this->Name = L"RiskEvtProcess";
			this->Text = L"风险事件处理窗口";
			this->Load += gcnew System::EventHandler(this, &RiskEvtProcess::Back_Load);
			this->Click += gcnew System::EventHandler(this, &RiskEvtProcess::button2_Click);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
			 {//通知
				 m_nRadioButton = 0;
				 this->label1->Text = L"已委派给交易员";
				 this->label2->Text = L"处理";
				 this->textBox1->Text = m_strTextBox1_Mess1;
				 this->textBox1->ReadOnly = true;
			 }
	private: System::Void radioButton2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		    {//解决
				m_nRadioButton = 1;
				this->label1->Text = L"该风险事件已由";
				this->label2->Text = L"解决";
				this->textBox1->Text = L"";
				this->textBox1->ReadOnly = false;
			}

private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 String ^strTextBox1 = textBox1->Text;
			 if(m_nRadioButton == 1 && strTextBox1->IsNullOrEmpty(strTextBox1))
			 {					
				 MessageBox::Show("风险事件处理人不能为空！", "提示",MessageBoxButtons::OK);
				 return;
			 }	
			
			 RiskEventHandlingUserReq REHandle;
			 REHandle.nTradeAccountID	= this->m_nTradeAccountID;
			 REHandle.nRiskEventID		= this->m_nRiskEventID;
			 REHandle.nRiskEventSubID   = this->m_nRiskEventSubID;

			String^ str = this->textBox2->Text;
			string strstrContent = Tools::String2string(str);			
			strncpy_s(REHandle.szContent, sizeof(REHandle.szContent), strstrContent.c_str(), sizeof(REHandle.szContent)-1);
			if(m_nRadioButton == 0)
			{//通知情况下： 处理人（操作的风控员），受理人（账户的交易员）
				string strUserName;
				m_pdataInfo->GetUserName(strUserName);				
				strncpy_s(REHandle.szHandler, sizeof(REHandle.szHandler), strUserName.c_str(), sizeof(REHandle.szHandler)-1);
				string strTextBox1_Mess1 = Tools::String2string(m_strTextBox1_Mess1);
				strncpy_s(REHandle.szAccepter, sizeof(REHandle.szAccepter), strTextBox1_Mess1.c_str(), sizeof(REHandle.szAccepter)-1);
			}
			else if(m_nRadioButton == 1)
			{//处理情况下： 处理人（输入框内容）、受理人（空）
				
				String^ strProcess = this->textBox1->Text;
				string  str = Tools::String2string(strProcess);
				strncpy_s(REHandle.szHandler, sizeof(REHandle.szHandler), str.c_str(), sizeof(REHandle.szHandler)-1);			
				strncpy_s(REHandle.szAccepter, sizeof(REHandle.szAccepter), "", sizeof(REHandle.szAccepter)-1);
			}

			CTcpLayer::SendData(Cmd_RM_AddRiskEvtHandling_Req, (void*)&REHandle,sizeof(REHandle), 0);			
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
				
			
		 }
	private: System::Void Back_Load(System::Object^  sender, System::EventArgs^  e)
			 {
				 //订阅事件
				 IntPtr hWnd=this->Handle;
				 CTcpLayer::SubscribePkg(Cmd_RM_AddRiskEvtHandling_Rsp, (int)(hWnd.ToInt32()));
			 }
			 //重载WndProc，以处理WM_COMMAND消息
	protected:virtual void WndProc( Message% m ) override
			  {
				  if (  m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival )
				  {
					  char *p = (char*)m.LParam.ToInt32();
					  Stru_UniPkgHead head;					
					  memset(&head, 0, sizeof(head));
					  memcpy(&head, p, sizeof(head));

					  if ( head.cmdid == Cmd_RM_AddRiskEvtHandling_Rsp )
					  {
						  if ( head.userdata1 == OPERATOR_SUCCESS )
						  {	
							 Close();
							 return;
						  }
						  else
						  {
							  MessageBox::Show("发送风险事件处理命令失败", "提示",MessageBoxButtons::OK);
						  }
					  }
				  }
				  Form::WndProc ( m );
			  }

};
}

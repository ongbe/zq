#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Tools.h"
#include "TcpLayer.h"
#include "CommonDef.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "CDataInfo.h"
#include "popmsgwin.h"
#include "CommonUserOpr.h"

enum eButtonClick{ BTN_PLAY,BTN_PAUSE,BTN_STOP };

namespace CltServerMgr {

	/// <summary>
	/// Summary for SimulateQuotServerSetupForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class SimulateQuotServerSetupForm : public System::Windows::Forms::Form
	{
	public:
		SimulateQuotServerSetupForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pLogin = new sLoginRsp();
            m_pDataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SimulateQuotServerSetupForm()
		{
			if (components)
			{
				delete components;
			}
            if(m_pLogin)
            {
                delete m_pLogin;
                m_pLogin = NULL;
            }
		}
    private: System::Windows::Forms::GroupBox^  groupBox1;
    protected: 
    private: System::Windows::Forms::GroupBox^  groupBox2;
    private: System::Windows::Forms::DateTimePicker^  dateTimePicker_End;



    private: System::Windows::Forms::DateTimePicker^  dateTimePicker_Start;
    private: System::Windows::Forms::RadioButton^  radioButton_PalyNormal;
    private: System::Windows::Forms::RadioButton^  radioButton_PlayNodelay;
    private: System::Windows::Forms::Button^  button_Quit;










    private: System::Windows::Forms::RadioButton^  radioButton_SpecialDay;

    private: System::Windows::Forms::RadioButton^  radioButton_NearestDay;

    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::GroupBox^  groupBox3;
    private: System::Windows::Forms::Button^  button_Pause;
    private: System::Windows::Forms::Button^  button_Stop;


    private: System::Windows::Forms::Button^  button_Start;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

    private: sLoginRsp*     m_pLogin;
    private: System::Windows::Forms::Button^  button_Apply;
    private: CDataInfo*     m_pDataInfo;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
            this->radioButton_SpecialDay = (gcnew System::Windows::Forms::RadioButton());
            this->radioButton_NearestDay = (gcnew System::Windows::Forms::RadioButton());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->dateTimePicker_End = (gcnew System::Windows::Forms::DateTimePicker());
            this->dateTimePicker_Start = (gcnew System::Windows::Forms::DateTimePicker());
            this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
            this->radioButton_PlayNodelay = (gcnew System::Windows::Forms::RadioButton());
            this->radioButton_PalyNormal = (gcnew System::Windows::Forms::RadioButton());
            this->button_Quit = (gcnew System::Windows::Forms::Button());
            this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
            this->button_Pause = (gcnew System::Windows::Forms::Button());
            this->button_Stop = (gcnew System::Windows::Forms::Button());
            this->button_Start = (gcnew System::Windows::Forms::Button());
            this->button_Apply = (gcnew System::Windows::Forms::Button());
            this->groupBox1->SuspendLayout();
            this->groupBox2->SuspendLayout();
            this->groupBox3->SuspendLayout();
            this->SuspendLayout();
            // 
            // groupBox1
            // 
            this->groupBox1->Controls->Add(this->radioButton_SpecialDay);
            this->groupBox1->Controls->Add(this->radioButton_NearestDay);
            this->groupBox1->Controls->Add(this->label3);
            this->groupBox1->Controls->Add(this->dateTimePicker_End);
            this->groupBox1->Controls->Add(this->dateTimePicker_Start);
            this->groupBox1->Location = System::Drawing::Point(43, 27);
            this->groupBox1->Name = L"groupBox1";
            this->groupBox1->Size = System::Drawing::Size(461, 96);
            this->groupBox1->TabIndex = 2;
            this->groupBox1->TabStop = false;
            this->groupBox1->Text = L"模拟时间配置";
            // 
            // radioButton_SpecialDay
            // 
            this->radioButton_SpecialDay->AutoSize = true;
            this->radioButton_SpecialDay->Location = System::Drawing::Point(25, 58);
            this->radioButton_SpecialDay->Name = L"radioButton_SpecialDay";
            this->radioButton_SpecialDay->Size = System::Drawing::Size(83, 16);
            this->radioButton_SpecialDay->TabIndex = 2;
            this->radioButton_SpecialDay->Text = L"指定交易日";
            this->radioButton_SpecialDay->UseVisualStyleBackColor = true;
            // 
            // radioButton_NearestDay
            // 
            this->radioButton_NearestDay->AutoSize = true;
            this->radioButton_NearestDay->Checked = true;
            this->radioButton_NearestDay->Location = System::Drawing::Point(25, 32);
            this->radioButton_NearestDay->Name = L"radioButton_NearestDay";
            this->radioButton_NearestDay->Size = System::Drawing::Size(107, 16);
            this->radioButton_NearestDay->TabIndex = 1;
            this->radioButton_NearestDay->TabStop = true;
            this->radioButton_NearestDay->Text = L"最近一个交易日";
            this->radioButton_NearestDay->UseVisualStyleBackColor = true;
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(271, 58);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(17, 12);
            this->label3->TabIndex = 5;
            this->label3->Text = L"到";
            // 
            // dateTimePicker_End
            // 
            this->dateTimePicker_End->CustomFormat = L"yyyyMMdd";
            this->dateTimePicker_End->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
            this->dateTimePicker_End->Location = System::Drawing::Point(310, 54);
            this->dateTimePicker_End->Name = L"dateTimePicker_End";
            this->dateTimePicker_End->Size = System::Drawing::Size(116, 21);
            this->dateTimePicker_End->TabIndex = 4;
            // 
            // dateTimePicker_Start
            // 
            this->dateTimePicker_Start->CustomFormat = L"yyyyMMdd";
            this->dateTimePicker_Start->Format = System::Windows::Forms::DateTimePickerFormat::Custom;
            this->dateTimePicker_Start->Location = System::Drawing::Point(136, 54);
            this->dateTimePicker_Start->Name = L"dateTimePicker_Start";
            this->dateTimePicker_Start->Size = System::Drawing::Size(119, 21);
            this->dateTimePicker_Start->TabIndex = 3;
            // 
            // groupBox2
            // 
            this->groupBox2->Controls->Add(this->radioButton_PlayNodelay);
            this->groupBox2->Controls->Add(this->radioButton_PalyNormal);
            this->groupBox2->Location = System::Drawing::Point(43, 152);
            this->groupBox2->Name = L"groupBox2";
            this->groupBox2->Size = System::Drawing::Size(461, 70);
            this->groupBox2->TabIndex = 3;
            this->groupBox2->TabStop = false;
            this->groupBox2->Text = L"模拟回放配置";
            // 
            // radioButton_PlayNodelay
            // 
            this->radioButton_PlayNodelay->AutoSize = true;
            this->radioButton_PlayNodelay->Location = System::Drawing::Point(259, 30);
            this->radioButton_PlayNodelay->Name = L"radioButton_PlayNodelay";
            this->radioButton_PlayNodelay->Size = System::Drawing::Size(167, 16);
            this->radioButton_PlayNodelay->TabIndex = 6;
            this->radioButton_PlayNodelay->Text = L"按实盘行情无时间间隔回放";
            this->radioButton_PlayNodelay->UseVisualStyleBackColor = true;
            // 
            // radioButton_PalyNormal
            // 
            this->radioButton_PalyNormal->AutoSize = true;
            this->radioButton_PalyNormal->Checked = true;
            this->radioButton_PalyNormal->Location = System::Drawing::Point(25, 30);
            this->radioButton_PalyNormal->Name = L"radioButton_PalyNormal";
            this->radioButton_PalyNormal->Size = System::Drawing::Size(179, 16);
            this->radioButton_PalyNormal->TabIndex = 5;
            this->radioButton_PalyNormal->TabStop = true;
            this->radioButton_PalyNormal->Text = L"按实盘行情时间间隔正常回放";
            this->radioButton_PalyNormal->UseVisualStyleBackColor = true;
            // 
            // button_Quit
            // 
            this->button_Quit->Location = System::Drawing::Point(429, 376);
            this->button_Quit->Name = L"button_Quit";
            this->button_Quit->Size = System::Drawing::Size(75, 23);
            this->button_Quit->TabIndex = 11;
            this->button_Quit->Text = L"退出";
            this->button_Quit->UseVisualStyleBackColor = true;
            this->button_Quit->Click += gcnew System::EventHandler(this, &SimulateQuotServerSetupForm::button_Quit_Click);
            // 
            // groupBox3
            // 
            this->groupBox3->Controls->Add(this->button_Pause);
            this->groupBox3->Controls->Add(this->button_Stop);
            this->groupBox3->Controls->Add(this->button_Start);
            this->groupBox3->Location = System::Drawing::Point(43, 269);
            this->groupBox3->Name = L"groupBox3";
            this->groupBox3->Size = System::Drawing::Size(461, 80);
            this->groupBox3->TabIndex = 6;
            this->groupBox3->TabStop = false;
            this->groupBox3->Text = L"回放手动控制";
            // 
            // button_Pause
            // 
            this->button_Pause->Location = System::Drawing::Point(194, 38);
            this->button_Pause->Name = L"button_Pause";
            this->button_Pause->Size = System::Drawing::Size(75, 23);
            this->button_Pause->TabIndex = 9;
            this->button_Pause->Text = L"暂停";
            this->button_Pause->UseVisualStyleBackColor = true;
            this->button_Pause->Click += gcnew System::EventHandler(this, &SimulateQuotServerSetupForm::button_Pause_Click);
            // 
            // button_Stop
            // 
            this->button_Stop->Location = System::Drawing::Point(320, 38);
            this->button_Stop->Name = L"button_Stop";
            this->button_Stop->Size = System::Drawing::Size(75, 23);
            this->button_Stop->TabIndex = 10;
            this->button_Stop->Text = L"停止";
            this->button_Stop->UseVisualStyleBackColor = true;
            this->button_Stop->Click += gcnew System::EventHandler(this, &SimulateQuotServerSetupForm::button_Stop_Click);
            // 
            // button_Start
            // 
            this->button_Start->Location = System::Drawing::Point(68, 38);
            this->button_Start->Name = L"button_Start";
            this->button_Start->Size = System::Drawing::Size(75, 23);
            this->button_Start->TabIndex = 8;
            this->button_Start->Text = L"开始";
            this->button_Start->UseVisualStyleBackColor = true;
            this->button_Start->Click += gcnew System::EventHandler(this, &SimulateQuotServerSetupForm::button_Start_Click);
            // 
            // button_Apply
            // 
            this->button_Apply->Location = System::Drawing::Point(429, 238);
            this->button_Apply->Name = L"button_Apply";
            this->button_Apply->Size = System::Drawing::Size(75, 23);
            this->button_Apply->TabIndex = 7;
            this->button_Apply->Text = L"应用";
            this->button_Apply->UseVisualStyleBackColor = true;
            this->button_Apply->Click += gcnew System::EventHandler(this, &SimulateQuotServerSetupForm::button_Apply_Click);
            // 
            // SimulateQuotServerSetupForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(538, 406);
            this->ControlBox = false;
            this->Controls->Add(this->button_Apply);
            this->Controls->Add(this->groupBox3);
            this->Controls->Add(this->button_Quit);
            this->Controls->Add(this->groupBox2);
            this->Controls->Add(this->groupBox1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"SimulateQuotServerSetupForm";
            this->Text = L"模拟行情服务器设置";
            this->Load += gcnew System::EventHandler(this, &SimulateQuotServerSetupForm::SimulateQuotServerSetupForm_Load);
            this->groupBox1->ResumeLayout(false);
            this->groupBox1->PerformLayout();
            this->groupBox2->ResumeLayout(false);
            this->groupBox2->PerformLayout();
            this->groupBox3->ResumeLayout(false);
            this->ResumeLayout(false);

        }
#pragma endregion
protected:virtual void WndProc( Message% m ) override{
                  if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival )
                  {
                      char *p = (char*)m.LParam.ToInt32();
                      Stru_UniPkgHead head;					
                      memset(&head, 0, sizeof(head));
                      memcpy(&head, p, sizeof(head));


                      String^ strError = String::Empty;
                      bool bShowMsg= false;

                      switch ( head.cmdid )
                      {	 
                      case Cmd_QrySimulateDay_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS && head.len == sizeof(sSimulateDay))
                              {
                                  sSimulateDay *pSimulateDay = (sSimulateDay*)(p+sizeof(Stru_UniPkgHead));
                                  if(SimulateDayType_Nearest == pSimulateDay->mType)
                                      radioButton_NearestDay->Checked = true;
                                  else if(SimulateDayType_Special == pSimulateDay->mType)
                                  {
                                      radioButton_SpecialDay->Checked = true;

                                      dateTimePicker_Start->Value = DateTime::ParseExact(Tools::string2String(pSimulateDay->mStartDay),"yyyyMMdd",nullptr);
                                      dateTimePicker_End->Value = DateTime::ParseExact(Tools::string2String(pSimulateDay->mEndDay),"yyyyMMdd",nullptr);
                                  }
                              }
                              break;
                          } 
                      case Cmd_QrySimulatePlayMode_Rsp:
                          {
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  int nSimulatePlayMode = *(int*)((char*)(p+sizeof(Stru_UniPkgHead)));
                                  if(SimulatePlayMode_Normal == nSimulatePlayMode)
                                      radioButton_PalyNormal->Checked = true;
                                  else if(SimulatePlayMode_NoDelay == nSimulatePlayMode)
                                      radioButton_PlayNodelay->Checked = true;

                              }
                              break;
                          } 
                      case Cmd_SimulatePlay_Rsp:
                          {
                              bShowMsg= true;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  strError = "开始模拟操作成功";
                                  EnableCtrols(BTN_PLAY);
                              }
                              else
                              {
                                  strError = "开始模拟操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          } 
                      case Cmd_SimulatePause_Rsp:
                          {
                              bShowMsg= true;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  strError = "暂停模拟操作成功";
                                  EnableCtrols(BTN_PAUSE);
                              }
                              else
                              {
                                  strError = "暂停模拟操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          } 
                      case Cmd_SimulateStop_Rsp:
                          {
                              bShowMsg= true;
                              if ( head.userdata1 == CF_ERROR_SUCCESS )
                              {
                                  strError = "停止模拟操作成功";
                                  EnableCtrols(BTN_STOP);
                              }
                              else
                              {
                                  strError = "停止模拟操作失败，失败原因是："+Tools::string2String((char*)(p+sizeof(Stru_UniPkgHead)));
                              }
                              break;
                          } 
                      }

                      if(bShowMsg)
                          _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
                          CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
                          Tools::String2string(strError).c_str());
                  }
                  Form::WndProc ( m );
              }


private: System::Void SimulateQuotServerSetupForm_Load(System::Object^  sender, System::EventArgs^  e) {
             this->CenterToParent(); 

             int nHwnd = this->Handle.ToInt32();                 
             m_pDataInfo->GetLogonInfo(*m_pLogin);

             //订阅信息     
             CTcpLayer::SubscribePkg(Cmd_QrySimulateDay_Rsp, nHwnd);                
             CTcpLayer::SubscribePkg(Cmd_QrySimulatePlayMode_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_SimulatePlay_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_SimulatePause_Rsp, nHwnd);
             CTcpLayer::SubscribePkg(Cmd_SimulateStop_Rsp, nHwnd);

             //请求信息
             CTcpLayer::SendData(Cmd_QrySimulateDay_Req, NULL,0, 0, m_pLogin->mnUserID);
             CTcpLayer::SendData(Cmd_QrySimulatePlayMode_Req, NULL,0, 0, m_pLogin->mnUserID);  

             button_Pause->Enabled = false;
             button_Stop->Enabled = false;
         }
private: System::Void button_Apply_Click(System::Object^  sender, System::EventArgs^  e) {
             sSimulateDay oSimulateDay;
             memset(&oSimulateDay,0,sizeof(sSimulateDay));
             oSimulateDay.mType = (radioButton_NearestDay->Checked)?SimulateDayType_Nearest:SimulateDayType_Special;
             if(SimulateDayType_Special == oSimulateDay.mType)
             {
                 memcpy(oSimulateDay.mStartDay,Tools::String2string(dateTimePicker_Start->Text).c_str(),sizeof(oSimulateDay.mStartDay));
                 memcpy(oSimulateDay.mEndDay,Tools::String2string(dateTimePicker_End->Text).c_str(),sizeof(oSimulateDay.mEndDay));
             }

             int nPlayMode = (radioButton_PalyNormal->Checked)?0:1;

             int nLen = sizeof(sSimulateDay)+sizeof(int);
             char *pBuffer = new char[nLen];
             if(!pBuffer)
                 return;

             memset(pBuffer,0,nLen);
             memcpy(pBuffer,&oSimulateDay,sizeof(sSimulateDay));
             memcpy(pBuffer+sizeof(sSimulateDay),&nPlayMode,sizeof(int));
             
             CTcpLayer::SendData(Cmd_SimulateApply_Req,pBuffer,nLen,0,m_pLogin->mnUserID);

             delete pBuffer;
             pBuffer = NULL;                 
         }
private: System::Void button_Start_Click(System::Object^  sender, System::EventArgs^  e) {
             CTcpLayer::SendData(Cmd_SimulatePlay_Req,NULL,0,0,m_pLogin->mnUserID);
         }
private: System::Void button_Pause_Click(System::Object^  sender, System::EventArgs^  e) {
             CTcpLayer::SendData(Cmd_SimulatePause_Req,NULL,0,0,m_pLogin->mnUserID);
         }
private: System::Void button_Stop_Click(System::Object^  sender, System::EventArgs^  e) {
             CTcpLayer::SendData(Cmd_SimulateStop_Req,NULL,0,0,m_pLogin->mnUserID);
         }
private: System::Void button_Quit_Click(System::Object^  sender, System::EventArgs^  e) {
             int nHwnd = this->Handle.ToInt32();            
             //取消订阅信息     
             CTcpLayer::UnsubscribePkg(Cmd_QrySimulateDay_Rsp, nHwnd);                
             CTcpLayer::UnsubscribePkg(Cmd_QrySimulatePlayMode_Rsp, nHwnd);
             CTcpLayer::UnsubscribePkg(Cmd_SimulatePlay_Rsp, nHwnd);
             CTcpLayer::UnsubscribePkg(Cmd_SimulatePause_Rsp, nHwnd);
             CTcpLayer::UnsubscribePkg(Cmd_SimulateStop_Rsp, nHwnd);
             this->Close();
         }

         //////////////////////////////////////////////////////////////////////////
         private: void EnableCtrols(eButtonClick eBTN)
                  {
                      switch(eBTN)
                      {
                      case BTN_PLAY:
                        radioButton_NearestDay->Enabled = false;
                        radioButton_SpecialDay->Enabled = false;
                        radioButton_PalyNormal->Enabled = false;                        
                        radioButton_PlayNodelay->Enabled = false;      

                        dateTimePicker_Start->Enabled = false;
                        dateTimePicker_End->Enabled = false;

                        button_Apply->Enabled = false;
                        button_Start->Enabled = false;
                        button_Pause->Enabled = true;
                        button_Stop->Enabled = true;
                        break;
                      case BTN_PAUSE:
                          radioButton_NearestDay->Enabled = false;
                          radioButton_SpecialDay->Enabled = false;
                          radioButton_PalyNormal->Enabled = false;                        
                          radioButton_PlayNodelay->Enabled = false;

                          dateTimePicker_Start->Enabled = false;
                          dateTimePicker_End->Enabled = false;

                          button_Apply->Enabled = false;
                          button_Start->Enabled = true;
                          button_Pause->Enabled = false;
                          button_Stop->Enabled = true;
                          break;
                      case BTN_STOP:
                          radioButton_NearestDay->Enabled = true;
                          radioButton_SpecialDay->Enabled = true;
                          radioButton_PalyNormal->Enabled = true;                        
                          radioButton_PlayNodelay->Enabled = true;

                          dateTimePicker_Start->Enabled = true;
                          dateTimePicker_End->Enabled = true;

                          button_Apply->Enabled = true;
                          button_Start->Enabled = true;
                          button_Pause->Enabled = false;
                          button_Stop->Enabled = false;
                          break;
                      }
                  }

};
}

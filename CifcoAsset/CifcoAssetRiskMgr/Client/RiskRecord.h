#pragma once
#include <sstream>
#include <vector>
#include "CCommonFunc.h"
#include "CDataInfo.h"
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "Tools.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Client {

	/// <summary>
	/// Summary for RiskRecord
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class RiskRecord : public System::Windows::Forms::Form
	{
	public:
		RiskRecord(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pdataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RiskRecord()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	protected: 









    private: CDataInfo*                        m_pdataInfo;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_RiskEvtID;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_account;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_handler;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_Assignees;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_riskEvtStatus;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnInd;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnIndicator;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnRiskLevel;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnInstrument;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_action;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_time;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_note;

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
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column_RiskEvtID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_account = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_handler = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_Assignees = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_riskEvtStatus = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnInd = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnIndicator = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnRiskLevel = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnInstrument = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_action = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_time = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_note = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(12) {this->Column_RiskEvtID, 
				this->Column_account, this->Column_handler, this->Column_Assignees, this->Column_riskEvtStatus, this->ColumnInd, this->ColumnIndicator, 
				this->ColumnRiskLevel, this->ColumnInstrument, this->Column_action, this->Column_time, this->Column_note});
			this->dataGridView1->Location = System::Drawing::Point(12, 12);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->Size = System::Drawing::Size(879, 419);
			this->dataGridView1->TabIndex = 0;
			// 
			// Column_RiskEvtID
			// 
			this->Column_RiskEvtID->HeaderText = L"风险事件ID";
			this->Column_RiskEvtID->Name = L"Column_RiskEvtID";
			this->Column_RiskEvtID->ReadOnly = true;
			this->Column_RiskEvtID->Width = 72;
			// 
			// Column_account
			// 
			this->Column_account->HeaderText = L"账户号";
			this->Column_account->Name = L"Column_account";
			this->Column_account->ReadOnly = true;
			this->Column_account->Width = 61;
			// 
			// Column_handler
			// 
			this->Column_handler->HeaderText = L"处理人";
			this->Column_handler->Name = L"Column_handler";
			this->Column_handler->ReadOnly = true;
			this->Column_handler->Width = 61;
			// 
			// Column_Assignees
			// 
			this->Column_Assignees->HeaderText = L"受理人";
			this->Column_Assignees->Name = L"Column_Assignees";
			this->Column_Assignees->ReadOnly = true;
			this->Column_Assignees->Width = 61;
			// 
			// Column_riskEvtStatus
			// 
			this->Column_riskEvtStatus->HeaderText = L"风险事件状态";
			this->Column_riskEvtStatus->Name = L"Column_riskEvtStatus";
			this->Column_riskEvtStatus->ReadOnly = true;
			this->Column_riskEvtStatus->Width = 72;
			// 
			// ColumnInd
			// 
			this->ColumnInd->HeaderText = L"风险指标";
			this->ColumnInd->Name = L"ColumnInd";
			this->ColumnInd->ReadOnly = true;
			this->ColumnInd->Width = 61;
			// 
			// ColumnIndicator
			// 
			this->ColumnIndicator->HeaderText = L"风险指标值";
			this->ColumnIndicator->Name = L"ColumnIndicator";
			this->ColumnIndicator->ReadOnly = true;
			this->ColumnIndicator->Width = 72;
			// 
			// ColumnRiskLevel
			// 
			this->ColumnRiskLevel->HeaderText = L"风险等级";
			this->ColumnRiskLevel->Name = L"ColumnRiskLevel";
			this->ColumnRiskLevel->ReadOnly = true;
			this->ColumnRiskLevel->Width = 61;
			// 
			// ColumnInstrument
			// 
			this->ColumnInstrument->HeaderText = L"合约ID";
			this->ColumnInstrument->Name = L"ColumnInstrument";
			this->ColumnInstrument->ReadOnly = true;
			this->ColumnInstrument->Width = 61;
			// 
			// Column_action
			// 
			this->Column_action->HeaderText = L"动作";
			this->Column_action->Name = L"Column_action";
			this->Column_action->ReadOnly = true;
			this->Column_action->Width = 51;
			// 
			// Column_time
			// 
			this->Column_time->HeaderText = L"时间";
			this->Column_time->Name = L"Column_time";
			this->Column_time->ReadOnly = true;
			this->Column_time->Width = 51;
			// 
			// Column_note
			// 
			this->Column_note->HeaderText = L"备注";
			this->Column_note->Name = L"Column_note";
			this->Column_note->ReadOnly = true;
			this->Column_note->Width = 51;
			// 
			// RiskRecord
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(903, 443);
			this->Controls->Add(this->dataGridView1);
			this->Name = L"RiskRecord";
			this->Text = L"账户风险记录";
			this->Load += gcnew System::EventHandler(this, &RiskRecord::RiskRecord_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &RiskRecord::RiskRecord_FormClosed);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void RiskRecord_Load(System::Object^  sender, System::EventArgs^  e) 
			 {//订阅事件
				 IntPtr hWnd=this->Handle;
				 CTcpLayer::SubscribePkg(Cmd_RM_QryRiskEvtHandling_Rsp,(int)(hWnd.ToInt32()));
			 }
	public: System::Void SetRiskRecord(RiskEventHandlingQueryKey& riskEvtHandleKey)
			 {
				 /*vector<RiskEventHandling> vexRiskEventHandling;
				 vexRiskEventHandling.clear();

				 m_pdataInfo->GetRiskEvent(nAccountID, nRiskEvtId, vexRiskEventHandling);
				 vector<RiskEventHandling>::iterator it = vexRiskEventHandling.begin();
				 for( it; it != vexRiskEventHandling.end(); it++)
				 {
					 ProcessRiskEventHandling(&(*it));

				 }*/
				  CTcpLayer::SendData(Cmd_RM_QryRiskEvtHandling_Req,(void *)&riskEvtHandleKey, sizeof(riskEvtHandleKey), 0);
				 
			 }
		string GetMsgStatusTypeString(MsgStatusType nMsgStatusType)
		{//得到风险事件状态
			string str ="";
			switch(nMsgStatusType)
			{
			case MsgStatus_New:
				str = STR_MsgStatus_New;
				break;
			case MsgStatus_ChangeLevel:
				str = STR_MsgStatus_ChangeLevel;
				break;
			case MsgStatus_Delete:
				str = STR_MsgStatus_Delete;
				break;
			case MsgStatus_Appoint:
				str = STR_MsgStatus_Appoint;
				break;
			case MsgStatus_Finished:
				str = STR_MsgStatus_Finished;
				break;
			case MsgStatus_ForceClose:
				str = STR_MsgStatus_ForceClose;
				break;
			default:break;
			}
			return str;
		}
	string GetEventHandlingActionTypeString(EventHandlingActionType nActionType)
		{//得到风险事件状态
			string str ="";
			switch(nActionType)
			{
			case Action_ServerRecord:
				str = STR_Action_ServerRecord;
				break;
			case Action_ClientRecord:
				str = STR_Action_ClientRecord;
				break;
			case Action_ForceClose:
				str = STR_Action_ForceClose;
				break;
			default:break;
			}
			return str;
		}
	string GetRiskIndicatorIDString(RiskIndicatorType nRiskIndicatorID)
		{//得到风险指标的字符串
			string str ="";
			RiskIndicator	sRiskIndicator;
			if(m_pdataInfo && m_pdataInfo->GetRiskIndicator(nRiskIndicatorID, sRiskIndicator))
			{
				str = sRiskIndicator.szName;
			}
			return str;
		}
private: System::Void ProcessRiskEventHandling(RiskEventHandlingEx* pRiskEventHandling)
		{
			array<String^>^ newrow=gcnew array<String^>(12);
			std::stringstream   stream; 
			string str; 

			//风险事件ID
			stream.clear();    
			stream.str("");
			stream   <<  pRiskEventHandling->nRiskEventID;  
			str = stream.str();
			String^  Str  = Tools::string2String(str.c_str());
			newrow[0] = Str;

			//委托交易账户ID
			stream.clear();    
			stream.str("");
			TrustTradeAccount account;
		    memset(&account,0,sizeof(account));
			m_pdataInfo->GetTrustTradeAccount(pRiskEventHandling->nTradeAccountID,account);
			stream   <<  account.szAccount;
			str = stream.str();
			Str  = Tools::string2String(str.c_str());
			newrow[1] = Str;

			//处理人
			stream.clear();    
			stream.str("");
			str  = pRiskEventHandling->szHandler;
			Str  = Tools::string2String(str.c_str());
			newrow[2] = Str;

			//受理人
			stream.clear();    
			stream.str("");
			str  = pRiskEventHandling->szAccepter;
			Str  = Tools::string2String(str.c_str());
			newrow[3] = Str;

			//风险事件状态
			stream.clear();    
			stream.str("");
			str  = GetMsgStatusTypeString(pRiskEventHandling->nProcessStatus);
			Str  = Tools::string2String(str.c_str());
			newrow[4] = Str;

			//风险指标
			stream.clear();    
			stream.str("");
			/*RiskIndicator  riskInd;
			memset(&riskInd,0,sizeof(riskInd));*/
			
			Str  = Tools::string2String(GetRiskIndicatorIDString(pRiskEventHandling->nRiskIndicatorID).c_str());
			newrow[5] = Str;

			//风险指标值
			stream.clear();    
			stream.str("");
			char strlIndicatorValue[128];
			sprintf(strlIndicatorValue, "%f", pRiskEventHandling->dblIndicatorValue);
			Str  = Tools::string2String(strlIndicatorValue);
			newrow[6] = Str;

			//风险等级
			stream.clear();    
			stream.str("");
			char strlevel[12];
			sprintf(strlevel, "%d", pRiskEventHandling->nRiskLevelID);
			Str  = Tools::string2String(strlevel);
			newrow[7] = Str;

			//合约ID
			stream.clear();    
			stream.str("");
			str  = pRiskEventHandling->InstrumentID;
			Str  = Tools::string2String(str.c_str());
			newrow[8] = Str;
			
			//动作
			stream.clear();    
			stream.str("");
			str  = GetEventHandlingActionTypeString(pRiskEventHandling->nAction);
			Str  = Tools::string2String(str.c_str());
			newrow[9] = Str;

			//时间
			stream.clear();    
			stream.str("");
			tm *pTmp = localtime(&pRiskEventHandling->lHandlingTime);
			if( pTmp != NULL )
			{
				char pszTime[20];
				sprintf(pszTime, "%4d-%02d-%02d %02d:%02d:%02d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);  
				str = pszTime;
			}
			Str  = Tools::string2String(str.c_str());
			newrow[10] = Str;

			str  = pRiskEventHandling->szContent;
			Str  = Tools::string2String(str.c_str());
			newrow[11] = Str;	
			dataGridView1->Rows->Add(newrow);	
		}
private: System::Void RiskRecord_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
		 {
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::SubscribePkg( Cmd_RM_QryRiskEvtHandling_Rsp ,(int)(hWnd.ToInt32()));
		 }
//处理风险事件处理记录消息
bool ProcessQueryRiskEvtRecordPkg(const char* pPkg,std::map<int, RiskEventHandlingEx>& mapRiskEventHandling,int fieldcnt)
{
	for ( int i = 0; i < fieldcnt; i++ )
	{
		RiskEventHandlingEx* pField = (RiskEventHandlingEx*)((char*)(pPkg+sizeof(Stru_UniPkgHead)) + i*sizeof(RiskEventHandlingEx));

		mapRiskEventHandling.insert(make_pair(pField->nRiskEventHandlingID,*pField));

	}

	

	return true;
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

				  if ( head.cmdid == Cmd_RM_QryRiskEvtHandling_Rsp)//查询风险事件处理记录返回
				  {
					  if ( head.userdata1 == OPERATOR_SUCCESS )
					  {
						  int pkgLen=head.len;
						  int MessageCnt=pkgLen/sizeof(RiskEventHandlingEx);
						  std::map<int, RiskEventHandlingEx> mapRiskEventHandling;

						  bool brlt =ProcessQueryRiskEvtRecordPkg((const char*)m.LParam.ToInt32(),mapRiskEventHandling,MessageCnt);
						  if(brlt)
						  {
							  std::map<int, RiskEventHandlingEx>::iterator it ;
							  for(it = mapRiskEventHandling.begin(); it != mapRiskEventHandling.end(); it++)
			                  {
							  //loadDataGridView1(mapRiskEvent);
                              ProcessRiskEventHandling(&(it->second));
							  }

						  }
					  }
					  else
							{
								string str = (char*)(p+sizeof(Stru_UniPkgHead));
								String^ strtemp = Tools::string2String(str.c_str());
								MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);
							}
				  }
			  }

			  Form::WndProc ( m );
		  }


};
}

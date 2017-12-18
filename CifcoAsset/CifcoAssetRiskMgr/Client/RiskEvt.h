#pragma warning(disable : 4819)

#pragma once
#include <string>
using namespace std;
#include "TcpLayer.h"
#include "RiskEvt2.h"
#include "Tools.h"
#include "RiskManageCmd.h"
#include "RiskManageStruct.h"
#include "CommonPkg.h"
#include <map>
#include <vector>
#include <sstream>
#include "Tools.h"
#include "CDataInfo.h"
#include "CurRiskEventPanel.h"
#include "HisRiskEventPanel.h"
#include "RiskEventRecord.h"
#include "RiskEvtProcess.h"
#include "RiskEventMessage.h"
#include "CDlgAllAccountFundInfo.h"
namespace Client {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for RiskEvt
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class RiskEvt : public System::Windows::Forms::Form
	{
	public:
		RiskEvt(void)
		{
			m_pdataInfo = CDataInfo::NewInstance();

			InitializeComponent();
			InitializeComponent2();
/*
			//测试用例，保留这部分注释掉的代码
			RiskEvent RiskEvent2;
			memset(&RiskEvent2, 0, sizeof(RiskEvent2));
			//RiskEvent* pRiskEvent =  new RiskEvent;
			RiskEvent* pRiskEvent =  &RiskEvent2;

			pRiskEvent->nRiskEventID			= 11;
			pRiskEvent->nTradeAccountID			= 1;	
			pRiskEvent->nRiskIndicatorID		= RI_MaxPosition;
			pRiskEvent->lEventTime				= 811111111;			
			pRiskEvent->nRiskLevelID			= 111;
			pRiskEvent->nMsgSendStatus          = MsgStatus_New;
			pRiskEvent->nIsValid				= RiskEvent_Realtime;
			strcpy(pRiskEvent->InstrumentID, "abc");	
			ProcessRiskEvent(pRiskEvent, true);


			pRiskEvent->nRiskEventID			= 11;
			pRiskEvent->nTradeAccountID			= 2;		
			pRiskEvent->nRiskIndicatorID		= RI_MaxPosition;
			pRiskEvent->lEventTime				= 82342554;	
			pRiskEvent->nMsgSendStatus          = MsgStatus_Delete;
			pRiskEvent->nRiskLevelID			= 222;
			pRiskEvent->nIsValid				= RiskEvent_Realtime;
			strcpy(pRiskEvent->InstrumentID, "abc");
			ProcessRiskEvent(pRiskEvent, true);

			pRiskEvent->nRiskEventID			= 33;
			pRiskEvent->nTradeAccountID			= 2;			
			pRiskEvent->nRiskIndicatorID		= RI_MaxPosition;
			pRiskEvent->lEventTime				= 92342554;	
			pRiskEvent->nMsgSendStatus          = MsgStatus_New;
			pRiskEvent->nRiskLevelID			= 333;
			pRiskEvent->nIsValid				= RiskEvent_Realtime;
			strcpy(pRiskEvent->InstrumentID, "abc");	
			ProcessRiskEvent(pRiskEvent, true);
/*

			pRiskEvent->nRiskEventID			= 33;
			pRiskEvent->nTradeAccountID			= 2;			
			pRiskEvent->nRiskIndicatorID		= RI_MaxPosition;
			pRiskEvent->lEventTime				= 99342554;	
			pRiskEvent->nMsgSendStatus          = MsgStatus_Delete;
			pRiskEvent->nRiskLevelID			= 0;
			pRiskEvent->nIsValid				= RiskEvent_History;
			strcpy(pRiskEvent->InstrumentID, "abcd");	
			ProcessRiskEvent(pRiskEvent);

			
			pRiskEvent->nRiskEventID			= 55;
			pRiskEvent->nTradeAccountID			= 1;			
			pRiskEvent->nRiskIndicatorID		= RI_MaxPosition;
			pRiskEvent->lEventTime				= 1811111111;			
			pRiskEvent->nRiskLevelID			= 555;
			pRiskEvent->nMsgSendStatus          = MsgStatus_New;
			pRiskEvent->nIsValid				= RiskEvent_History;
			strcpy(pRiskEvent->InstrumentID, "abc");	
			ProcessRiskEvent(pRiskEvent);

			pRiskEvent->nRiskEventID			= 55;
			pRiskEvent->nTradeAccountID			= 1;			
			pRiskEvent->nRiskIndicatorID		= RI_MaxPosition;
			pRiskEvent->lEventTime				= 2811111111;			
			pRiskEvent->nRiskLevelID			= 555;
			pRiskEvent->nMsgSendStatus          = MsgStatus_Delete;
			pRiskEvent->nIsValid				= RiskEvent_History;
			strcpy(pRiskEvent->InstrumentID, "abc");	
			ProcessRiskEvent(pRiskEvent);

/*			pRiskEvent->nRiskEventID			= 66;
			pRiskEvent->nTradeAccountID			= 1;
			pRiskEvent->nRiskIndicatorID		= RI_FundLoss;
			pRiskEvent->lEventTime				= 1911111111;			
			pRiskEvent->nRiskLevelID			= 666;
			pRiskEvent->nMsgSendStatus          = MsgStatus_New;
			pRiskEvent->nIsValid				= RiskEvent_Realtime;
			strcpy(pRiskEvent->InstrumentID, "");	
			ProcessRiskEvent(pRiskEvent);

			pRiskEvent->nRiskEventID			= 77;
			pRiskEvent->nTradeAccountID			= 77;
			pRiskEvent->nRiskIndicatorID		= RI_GappedMarket;
			pRiskEvent->lEventTime				= 2011111111;			
			pRiskEvent->nRiskLevelID			= 777;
			pRiskEvent->nMsgSendStatus          = MsgStatus_New;
			pRiskEvent->nIsValid				= RiskEvent_Realtime;
			strcpy(pRiskEvent->InstrumentID, "");	
			ProcessRiskEvent(pRiskEvent);

			pRiskEvent->nTradeAccountID			= 77;
			pRiskEvent->nRiskEventID			= 77;
			pRiskEvent->lEventTime				= 2111111111;
			pRiskEvent->nRiskIndicatorID		= RI_GappedMarket;
			pRiskEvent->nRiskLevelID			= 777;
			pRiskEvent->nMsgSendStatus          = MsgStatus_Delete;
			pRiskEvent->nIsValid				= RiskEvent_Realtime;
		strcpy(pRiskEvent->InstrumentID, "");	
			ProcessRiskEvent(pRiskEvent);

			RiskEventHandling* pRiskEventHandling = new RiskEventHandling;
			pRiskEventHandling->nRiskEventHandlingID = 1;
			pRiskEventHandling->lHandlingTime        = 2111111111;
			pRiskEventHandling->nTradeAccountID      = 11;
			pRiskEventHandling->nRiskEventID         = 111;
			strcpy(pRiskEventHandling->szHandler, "caiguosen");	
			strcpy(pRiskEventHandling->szAccepter, "caicai");	
			pRiskEventHandling->nAction         = Action_ServerRecord;
			pRiskEventHandling->nProcessStatus         = MsgStatus_Handling;
			strcpy(pRiskEventHandling->szContent, "beizhu");	
			ProcessRiskEventHandling(pRiskEventHandling);

			pRiskEventHandling->nRiskEventHandlingID = 1;
			pRiskEventHandling->lHandlingTime        = 2111111111;
			pRiskEventHandling->nTradeAccountID      = 11;
			pRiskEventHandling->nRiskEventID         = 111;
			strcpy(pRiskEventHandling->szHandler, "caiguosen4");	
			strcpy(pRiskEventHandling->szAccepter, "caicai4");	
			pRiskEventHandling->nAction         = Action_ServerRecord;
			pRiskEventHandling->nProcessStatus         = MsgStatus_Handling;
			strcpy(pRiskEventHandling->szContent, "beizhu4");	
			ProcessRiskEventHandling(pRiskEventHandling);

			pRiskEventHandling->nRiskEventHandlingID = 2;
			pRiskEventHandling->lHandlingTime        = 2211111111;
			pRiskEventHandling->nTradeAccountID      = 22;
			pRiskEventHandling->nRiskEventID         = 222;
			strcpy(pRiskEventHandling->szHandler, "caiguosen2");	
			strcpy(pRiskEventHandling->szAccepter, "caicai2");	
			pRiskEventHandling->nAction         = Action_ClientRecord;
			pRiskEventHandling->nProcessStatus         = MsgStatus_Delete;
			strcpy(pRiskEventHandling->szContent, "beizhu2");	
			ProcessRiskEventHandling(pRiskEventHandling);

			pRiskEventHandling->nRiskEventHandlingID = 3;
			pRiskEventHandling->lHandlingTime        = 2311111111;
			pRiskEventHandling->nTradeAccountID      = 33;
			pRiskEventHandling->nRiskEventID         = 333;
			strcpy(pRiskEventHandling->szHandler, "caiguosen3");	
			strcpy(pRiskEventHandling->szAccepter, "caicai3");	
			pRiskEventHandling->nAction         = Action_ForceClose;
			pRiskEventHandling->nProcessStatus         = MsgStatus_New;
			strcpy(pRiskEventHandling->szContent, "beizhu3");	
			ProcessRiskEventHandling(pRiskEventHandling);*/

			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RiskEvt()
		{
			CDataInfo::DestroyInstance();
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	protected: 
	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	
	public:
	delegate void VibrateEventHandler(Object^ sender, RiskEventEventArgs^ e);
			 // 定义事件
	event VibrateEventHandler^ VibrateEvent1;


	 public: CDataInfo*  m_pdataInfo;

	public:
		CurRiskEventPanel^ CurRiskEventPanel1;
		HisRiskEventPanel^ HisRiskEventPanel1;
		RiskEventRecord^   RiskEventRecord1;
		RiskEvtProcess^    RiskEvtProcess1;
		CDlgAllAccountFundInfo^ DlgAllAccountFundInfo1;
		
	public: 

	











	private: System::Windows::Forms::ListView^  listView1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader1;
	private: System::Windows::Forms::ColumnHeader^  columnHeader2;














	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Account_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Processor_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Incharge_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  RiskEvtStatus_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Action_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Time_UserRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Remarks_UserRiskEvt;






















	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_CurRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  TradeAccountID_CurRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  TradeAccountName_CurRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  RiskType_CurRisk;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Detail_CurRisk;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Time_CurRiskEvt;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_HistRiskEvt;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  TradeAccountID_HistRiskEvt;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  TradeAccountName_HistRiskEvt;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  RiskType_HisCurRisk;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Detail_HisCurRisk;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  Time_HistRiskEvt;
private: System::Windows::Forms::DataGridViewTextBoxColumn^  TimeDispare_HistRiskEvt;
private: System::Windows::Forms::SplitContainer^  splitContainer3;











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
		void InitializeComponent2(void)
		{
			this->CurRiskEventPanel1 = (gcnew CurRiskEventPanel);			
			this->splitContainer2->Panel1->Controls->Add(this->CurRiskEventPanel1);
		
			this->DlgAllAccountFundInfo1 = (gcnew CDlgAllAccountFundInfo);
			DlgAllAccountFundInfo1->TopLevel=false;
			DlgAllAccountFundInfo1->Show();
			this->splitContainer2->Panel2->Controls->Add(this->DlgAllAccountFundInfo1);
		

			this->RiskEventRecord1 = (gcnew RiskEventRecord);
			this->splitContainer3->Panel1->Controls->Add(this->RiskEventRecord1);

			this->HisRiskEventPanel1 = (gcnew HisRiskEventPanel);			
			this->splitContainer3->Panel2->Controls->Add(this->HisRiskEventPanel1);

			for(int i=0; i<6; i++)
				CurRiskEventPanel1->CurRiskEvtList->Columns[i]->Tag= i;//为 每一列绑定一个序号

			for(int i=0; i<7; i++)
				HisRiskEventPanel1->HistRiskEvtList->Columns[i]->Tag= i;

				
			this->CurRiskEventPanel1->CurRiskEvent1 += gcnew CurRiskEventPanel::CurRiskEventEventHandler(this, &RiskEvt::RiskEvt_CurRiskEvent);
			this->HisRiskEventPanel1->HisRiskEvent1 += gcnew HisRiskEventPanel::HisRiskEventEventHandler(this, &RiskEvt::RiskEvt_HisRiskEvent);
		
			this->RiskEvtProcess1 = gcnew RiskEvtProcess;
			this->CurRiskEventPanel1->CurRiskEvent_DBClick += gcnew CurRiskEventPanel::CurRiskEvent_DBClciktEventHandler(this, &RiskEvt::RiskEvt_CurRiskEventDBClcik);
			this->HisRiskEventPanel1->HisRiskEvent_DBClick += gcnew HisRiskEventPanel::HisRiskEvent_DBClciktEventHandler(this, &RiskEvt::RiskEvt_HisRiskEventDBClcik);
		}
		
		void InitializeComponent(void)
		{
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->splitContainer3 = (gcnew System::Windows::Forms::SplitContainer());
			this->ID_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Account_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Processor_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Incharge_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->RiskEvtStatus_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Action_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Time_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Remarks_UserRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ID_CurRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->TradeAccountID_CurRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->TradeAccountName_CurRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->RiskType_CurRisk = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Detail_CurRisk = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Time_CurRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ID_HistRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->TradeAccountID_HistRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->TradeAccountName_HistRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->RiskType_HisCurRisk = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Detail_HisCurRisk = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Time_HistRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->TimeDispare_HistRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->splitContainer3->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->splitContainer2);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer3);
			this->splitContainer1->Size = System::Drawing::Size(292, 266);
			this->splitContainer1->SplitterDistance = 144;
			this->splitContainer1->TabIndex = 0;
			//this->splitContainer1->SplitterMoved += gcnew System::Windows::Forms::SplitterEventHandler(this, &RiskEvt::splitContainer1_SplitterMoved);
			// 
			// splitContainer2
			// 
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Name = L"splitContainer2";
			this->splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &RiskEvt::splitContainer2_Panel1_Paint);
			this->splitContainer2->Size = System::Drawing::Size(144, 266);
			this->splitContainer2->SplitterDistance = 168;
			this->splitContainer2->TabIndex = 0;
			// 
			// splitContainer3
			// 
			this->splitContainer3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer3->Location = System::Drawing::Point(0, 0);
			this->splitContainer3->Name = L"splitContainer3";
			this->splitContainer3->Orientation = System::Windows::Forms::Orientation::Horizontal;
			this->splitContainer3->Size = System::Drawing::Size(144, 266);
			this->splitContainer3->SplitterDistance = 168;
			this->splitContainer3->TabIndex = 0;

			// 
			this->splitContainer3->Panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &RiskEvt::splitContainer3_Panel1_Paint);
			this->splitContainer3->Size = System::Drawing::Size(144, 266);
			this->splitContainer3->SplitterDistance = 168;
			this->splitContainer3->TabIndex = 0;
			// ID_UserRiskEvt
			// 
			this->ID_UserRiskEvt->Name = L"ID_UserRiskEvt";
			// 
			// Account_UserRiskEvt
			// 
			this->Account_UserRiskEvt->Name = L"Account_UserRiskEvt";
			// 
			// Processor_UserRiskEvt
			// 
			this->Processor_UserRiskEvt->Name = L"Processor_UserRiskEvt";
			// 
			// Incharge_UserRiskEvt
			// 
			this->Incharge_UserRiskEvt->Name = L"Incharge_UserRiskEvt";
			// 
			// RiskEvtStatus_UserRiskEvt
			// 
			this->RiskEvtStatus_UserRiskEvt->Name = L"RiskEvtStatus_UserRiskEvt";
			// 
			// Action_UserRiskEvt
			// 
			this->Action_UserRiskEvt->Name = L"Action_UserRiskEvt";
			// 
			// Time_UserRiskEvt
			// 
			this->Time_UserRiskEvt->Name = L"Time_UserRiskEvt";
			// 
			// Remarks_UserRiskEvt
			// 
			this->Remarks_UserRiskEvt->Name = L"Remarks_UserRiskEvt";
			// 
			// ID_CurRiskEvt
			// 
			this->ID_CurRiskEvt->Name = L"ID_CurRiskEvt";
			// 
			// TradeAccountID_CurRiskEvt
			// 
			this->TradeAccountID_CurRiskEvt->Name = L"TradeAccountID_CurRiskEvt";
			// 
			// TradeAccountName_CurRiskEvt
			// 
			this->TradeAccountName_CurRiskEvt->Name = L"TradeAccountName_CurRiskEvt";
			// 
			// RiskType_CurRisk
			// 
			this->RiskType_CurRisk->Name = L"RiskType_CurRisk";
			// 
			// Detail_CurRisk
			// 
			this->Detail_CurRisk->Name = L"Detail_CurRisk";
			// 
			// Time_CurRiskEvt
			// 
			this->Time_CurRiskEvt->Name = L"Time_CurRiskEvt";
			// 
			// ID_HistRiskEvt
			// 
			this->ID_HistRiskEvt->Name = L"ID_HistRiskEvt";
			// 
			// TradeAccountID_HistRiskEvt
			// 
			this->TradeAccountID_HistRiskEvt->Name = L"TradeAccountID_HistRiskEvt";
			// 
			// TradeAccountName_HistRiskEvt
			// 
			this->TradeAccountName_HistRiskEvt->Name = L"TradeAccountName_HistRiskEvt";
			// 
			// RiskType_HisCurRisk
			// 
			this->RiskType_HisCurRisk->Name = L"RiskType_HisCurRisk";
			// 
			// Detail_HisCurRisk
			// 
			this->Detail_HisCurRisk->Name = L"Detail_HisCurRisk";
			// 
			// Time_HistRiskEvt
			// 
			this->Time_HistRiskEvt->Name = L"Time_HistRiskEvt";
			// 
			// TimeDispare_HistRiskEvt
			// 
			this->TimeDispare_HistRiskEvt->Name = L"TimeDispare_HistRiskEvt";
			// 
			// RiskEvt
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(292, 266);
			this->ControlBox = false;
			this->Controls->Add(this->splitContainer1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"RiskEvt";
			this->ShowIcon = false;
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Load += gcnew System::EventHandler(this, &RiskEvt::RiskEvt_Load);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->splitContainer2->ResumeLayout(false);
			this->splitContainer3->ResumeLayout(false);
			this->ResumeLayout(false);

		}
		string GetRiskIndicatorIDString(RiskIndicatorType nRiskIndicatorID)
		{//得到风险类型的字符串		
			
			string str ="";
			RiskIndicator	sRiskIndicator;
			if(m_pdataInfo && m_pdataInfo->GetRiskIndicator(nRiskIndicatorID, sRiskIndicator))
			{
				str = sRiskIndicator.szName;
			}
			return str;
		}
	/*	RiskIndicatorType GettRiskIndicatorID(string str)
		{
			if(str == STR_RI_MaxPosition)
				return RI_MaxPosition;
			else if(str == STR_RI_FundLoss)
				return RI_FundLoss;
			else if(str == STR_RI_GappedMarket)
				return RI_GappedMarket;
			else if(str == STR_RI_MarginRatio)
				return RI_MarginRatio;
			else if(str == STR_RI_ProductMaxPositon)
				return RI_ProductMaxPositon;
			return (RiskIndicatorType)0;
		}*/

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
		MsgStatusType GetMsgStatusTypeID(string str)
		{
			if(str == STR_MsgStatus_New)
				return MsgStatus_New;
			else if(str == STR_MsgStatus_ChangeLevel)
				return MsgStatus_ChangeLevel;
			else if(str == STR_MsgStatus_Delete)
				return MsgStatus_Delete;

			else if(str == STR_MsgStatus_Appoint)
				return MsgStatus_Appoint;
			else if(str == STR_MsgStatus_Finished)
				return MsgStatus_Finished;
			else if(str == STR_MsgStatus_ForceClose)
				return MsgStatus_ForceClose;
			return (MsgStatusType)-1;
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
		EventHandlingActionType GetEventHandlingActionTypeID(string str)
		{
			if(str == STR_Action_ServerRecord)
				return Action_ServerRecord;
			else if(str == STR_Action_ClientRecord)
				return Action_ClientRecord;
			else if(str == STR_Action_ForceClose)
				return Action_ForceClose;
			return (EventHandlingActionType)-1;
		}
		int  GetColumnIndex(DataGridView^ EvtList, int nTag)
		{
			int nColCount = EvtList->ColumnCount;
			for(int i =0; i< nColCount; i++)
			{
				int nID=int::Parse(EvtList->Columns[i]->Tag->ToString());
				if(nID == nTag)
					return i;
			}
			return -1;
		}
	
		bool GetExistRowIndex(RiskEvent* pRiskEvent, DataGridView^ EvtList, int& nRowIndex)
		{//表中已经有这个风险的事件
			int nRowCount = EvtList->Rows->Count-1;//多了一行空白行，不知道怎么回事
			int nColCount = EvtList->ColumnCount;
			for(int nRow = 0; nRow < nRowCount; nRow ++)
			{
				int nIndex = GetColumnIndex(EvtList, 0);//Cells[0]代表风险事件ID
				if(EvtList->Rows[nRow]->Cells[nIndex]->Value == nullptr)
					continue;

				String^ str = EvtList->Rows[nRow]->Cells[nIndex]->Value->ToString();
				string strTemp =  Tools::String2string(str);
				int nRiskID = atoi(strTemp.c_str());

				if(nRiskID == pRiskEvent->nRiskEventID)
				{
					nRowIndex = nRow;
					return true;
				}
			}
			return false;
		}
		void RiskEvent2Array(RiskEvent* pRiskEvent, array<String^>^ newrow)
		{
		//	array<String^>^ newrow=gcnew array<String^>(6);

			std::stringstream   stream; 
			string str;  

			//风险事件ID
			stream   <<  pRiskEvent->nRiskEventID;  
			str = stream.str();
			String^  Str  = Tools::string2String(str.c_str());
			newrow[0] = Str;
			

			//委托交易账户ID	
			stream.clear();    
			stream.str("");
			str ="";
			string strName ="";
			TrustTradeAccount fP;
			if(m_pdataInfo->GetTrustTradeAccount(pRiskEvent->nTradeAccountID, fP))
			{
				str = fP.szAccount;	
				strName = fP.szName;
			}
			Str  = Tools::string2String(str.c_str());
			newrow[1] = Str;

			//委托交易账户NAME			
			Str  = Tools::string2String(strName.c_str());
			newrow[2] = Str;
			
			//风险指标类型
			stream.clear();    
			stream.str("");
			str = GetRiskIndicatorIDString(pRiskEvent->nRiskIndicatorID);
			Str  = Tools::string2String(str.c_str());
			newrow[3] = Str;

			//风险详情

			char szBuffer[100];
			memset(szBuffer, 0, sizeof(szBuffer));
			sprintf(szBuffer, "%d级风险", pRiskEvent->nRiskLevelID);
			
			string strTemp;
			strTemp = szBuffer;
			if(pRiskEvent->nRiskIndicatorID == RI_MaxPosition)
			{
				string strConstract = pRiskEvent->InstrumentID;
				str = strConstract+ ": " + strTemp;
			}
			else if(pRiskEvent->nRiskIndicatorID == RI_ProductMaxPositon)
			{
				string strConstract = pRiskEvent->InstrumentID;
				str = strConstract+ ": " + strTemp;
			}
			else 
				str = strTemp;
			Str  = Tools::string2String(str.c_str());
			newrow[4] = Str;

			tm *pTmp = localtime(&pRiskEvent->lEventTime);
			char pszTime[20];
			sprintf(pszTime, "%4d-%02d-%02d %02d:%02d:%02d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);  
			str = pszTime;
			Str  = Tools::string2String(str.c_str());
			newrow[5] = Str;

			//EvtList->Rows->Add(newrow);
		}
		void BeepAndShake(RiskEvent* pRiskEvent)
		{
			ProductRiskWarningKey key;
			key.nRiskIndicatorID = pRiskEvent->nRiskIndicatorID;
			key.nRiskLevelID     = pRiskEvent->nRiskLevelID;
			key.nTradeAccountID	 = pRiskEvent->nTradeAccountID;
			InstrumentField instrumentField;
			if(pRiskEvent->nRiskIndicatorID == RI_MaxPosition)
			{
				if(!m_pdataInfo->GetInstrumentFieldByInstrumentID(pRiskEvent->InstrumentID, instrumentField))
					return;
				key.szProductID      = instrumentField.ProductID;
			}
			else if(pRiskEvent->nRiskIndicatorID == RI_ProductMaxPositon)
				key.szProductID      = pRiskEvent->InstrumentID;
			

			ProductRiskWarning rw;						
			if(pRiskEvent->nRiskIndicatorID == RI_MaxPosition || pRiskEvent->nRiskIndicatorID == RI_ProductMaxPositon)
			{
				if(m_pdataInfo->GetProductRiskWarning(key,  rw))
				{					
					if(rw.nResponseType == WarningType_Beep || rw.nResponseType == WarningType_Both)
					{//鸣笛					
						System::Console::Beep(800, 300);
					}
					if(rw.nResponseType == WarningType_Vibrate || rw.nResponseType == WarningType_Both)
					{
						RiskEventEventArgs^ AccountArgs = gcnew RiskEventEventArgs( -1);
						VibrateEvent1( this, AccountArgs );
					}
				}
			}
			else 
			{
				RiskWarning rw;
				RiskWarningKey key;
				key.nRiskIndModuleID = pRiskEvent->nRiskIndModuleID;
				key.nRiskIndicatorID = pRiskEvent->nRiskIndicatorID;
				key.nRiskLevelID	 = pRiskEvent->nRiskLevelID;	

				if(m_pdataInfo->GetRiskWarning(key,  rw))
				{					
					if(rw.nResponseType == WarningType_Beep || rw.nResponseType == WarningType_Both)
					{//鸣笛
						System::Console::Beep(800, 300);
					}
					if(rw.nResponseType == WarningType_Vibrate || rw.nResponseType == WarningType_Both)
					{
						RiskEventEventArgs^ AccountArgs = gcnew RiskEventEventArgs( -1);
						VibrateEvent1( this, AccountArgs );
					}
				}
			}

		}
		
		void ProcessRiskEvent_Realtime(RiskEvent* pRiskEvent, bool bTip)
		{
			int ncount = m_pdataInfo->m_mapCurRiskEvent.count(pRiskEvent->nRiskEventID);
			MAPInt2RiskEvent::iterator it = m_pdataInfo->m_mapCurRiskEvent.find(pRiskEvent->nRiskEventID);
			RiskEvent re;
			int nSubItem = -1;
			for(int nIndex = 0; nIndex < ncount; nIndex ++)
			{
				if(it->second.nRiskEventSubID > nSubItem)
				{//subId最大的为第后一个风险事件
					nSubItem = it->second.nRiskEventSubID;
					re = it->second;	//得到最后那一条风险事件
				}				
				it ++;				
			}	
			m_pdataInfo->m_mapCurRiskEvent.insert(make_pair(pRiskEvent->nRiskEventID, *pRiskEvent));
			if(0 == ncount)
			{//列表中没有相应的数据
				

				array<String^>^ newrow=gcnew array<String^>(6);
				RiskEvent2Array(pRiskEvent, newrow);
				CurRiskEventPanel1->CurRiskEvtList->Rows->Add(newrow);
				if(bTip)
					BeepAndShake(pRiskEvent);
				return;
			}
			else
			{
				if(pRiskEvent->nMsgSendStatus != MsgStatus_Delete)
				{
					int nRowIndex = 0;	
					if(GetExistRowIndex(pRiskEvent, CurRiskEventPanel1->CurRiskEvtList,  nRowIndex))
					{
					//	RiskEvent re = it->second;//包含开始时间

						array<String^>^ newrow=gcnew array<String^>(6);
						RiskEvent2Array(pRiskEvent, newrow);

						for(int i = 0; i< 6; i++)
						{//更改原有风险事件列表的内容
							int nIndex = GetColumnIndex(CurRiskEventPanel1->CurRiskEvtList, i);											
							CurRiskEventPanel1->CurRiskEvtList->Rows[nRowIndex]->Cells[nIndex]->Value = newrow[i];
						}
					}
					if(bTip)
						BeepAndShake(pRiskEvent);
				}
				else
				{//风险事件消失
					int nRowIndex = 0;							
					if(GetExistRowIndex(pRiskEvent, CurRiskEventPanel1->CurRiskEvtList,  nRowIndex))
					{
					//	RiskEvent re = it->second;//包含开始时间
						array<String^>^ newrow=gcnew array<String^>(7);										
						RiskEvent2Array(&re, newrow);
						tm *pTmp = localtime(&pRiskEvent->lEventTime);
						char pszTime[20];
						sprintf(pszTime, "%4d-%02d-%02d %02d:%02d:%02d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);  
						string str = pszTime;
						String^ Str  = Tools::string2String(str.c_str());
						newrow[6] = Str;
						HisRiskEventPanel1->HistRiskEvtList->Rows->Add(newrow);		

						
						CurRiskEventPanel1->CurRiskEvtList->Rows->RemoveAt(nRowIndex);	

						RiskEventHistory rHis;
						rHis.set(re, pRiskEvent->lEventTime);
						m_pdataInfo->m_mapHisEvent[pRiskEvent->nRiskEventID] = rHis;
					}
					else
					{//如果这种情况出现，那么风险事件的产生事件将没有
						array<String^>^ newrow=gcnew array<String^>(7);										
						RiskEvent2Array(pRiskEvent, newrow);
						tm *pTmp = localtime(&pRiskEvent->lEventTime);
						char pszTime[20];
						sprintf(pszTime, "%4d-%02d-%02d %02d:%02d:%02d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);  
						string str = pszTime;
						String^ Str  = Tools::string2String(str.c_str());
						newrow[6] = Str;
						HisRiskEventPanel1->HistRiskEvtList->Rows->Add(newrow);	

						RiskEventHistory rHis;
						rHis.set(*pRiskEvent, pRiskEvent->lEventTime);
						m_pdataInfo->m_mapHisEvent[pRiskEvent->nRiskEventID] = rHis;
					}

				}
			}
		}
		void ProcessRiskEvent_History(RiskEvent* pRiskEvent)
		{
			int ncount = m_pdataInfo->m_mapCurRiskEvent.count(pRiskEvent->nRiskEventID);
			MAPInt2RiskEvent::iterator it2 = m_pdataInfo->m_mapCurRiskEvent.find(pRiskEvent->nRiskEventID);
			RiskEvent re;//得到最后一个风险事件
			int nSubItem = -1;
			for(int nIndex = 0; nIndex < ncount; nIndex ++)
			{//主要是兼容服务端最后一个消失的风险事件没有消失之前的信息
				if(it2->second.nRiskEventSubID > nSubItem)
				{//subId最大的为第后一个风险事件
					nSubItem = it2->second.nRiskEventSubID;
					re = it2->second;	//得到最后那一条风险事件
				}				
				it2 ++;				
			}	

			m_pdataInfo->m_mapCurRiskEvent.insert(make_pair(pRiskEvent->nRiskEventID, *pRiskEvent));//右侧账户处理记录需要风险事件的信息	
			
			if(pRiskEvent->nMsgSendStatus == MsgStatus_Delete)
			{
				array<String^>^ newrow=gcnew array<String^>(7);										
				RiskEvent2Array(&re, newrow);//主要是兼容服务端最后一个消失的风险事件没有消失之前的信息
				String^ Str  = "";
				newrow[6] = Str;				
				tm *pTmp = localtime(&pRiskEvent->lEventTime);
				char pszTime[20];
				sprintf(pszTime, "%4d-%02d-%02d %02d:%02d:%02d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);  
				string str = pszTime;
				Str  = Tools::string2String(str.c_str());
				newrow[6] = Str;
				
				HisRiskEventPanel1->HistRiskEvtList->Rows->Add(newrow);	

				RiskEventHistory rHis;
				rHis.set(re, pRiskEvent->lEventTime);
				m_pdataInfo->m_mapHisEvent[pRiskEvent->nRiskEventID] = rHis;

			}
		/*	MAPInt2RiskEventHistory::iterator it = m_pdataInfo->m_mapHisEvent.find(pRiskEvent->nRiskEventID);
			if(it == m_pdataInfo->m_mapHisEvent.end())
			{
				array<String^>^ newrow=gcnew array<String^>(7);										
				RiskEvent2Array(&re, newrow);
				String^ Str  = "";
				newrow[6] = Str;
				if(pRiskEvent->nMsgSendStatus == MsgStatus_Delete)
				{
					tm *pTmp = localtime(&pRiskEvent->lEventTime);
					char pszTime[20];
					sprintf(pszTime, "%4d-%02d-%02d %02d:%02d:%02d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);  
					string str = pszTime;
					Str  = Tools::string2String(str.c_str());
					newrow[6] = Str;
				}
				HisRiskEventPanel1->HistRiskEvtList->Rows->Add(newrow);	

				RiskEventHistory rHis;
				rHis.set(*pRiskEvent, pRiskEvent->lEventTime);
				m_pdataInfo->m_mapHisEvent[pRiskEvent->nRiskEventID] = rHis;
			}
			else
			{//历史风险事件已经在当前风险事件表里面显示（需要删除，然后再历史风险事件表里面添加记录）

				int nRowIndex = 0;							
				if(GetExistRowIndex(pRiskEvent, HisRiskEventPanel1->HistRiskEvtList,  nRowIndex))
				{
					RiskEventHistory re = it->second;//包含开始时间
					array<String^>^ newrow=gcnew array<String^>(7);										
					RiskEvent2Array(&re.riskEvent, newrow);
					newrow[6] ="";
					if(pRiskEvent->nMsgSendStatus == MsgStatus_Delete)
					{
						tm *pTmp = localtime(&pRiskEvent->lEventTime);
						char pszTime[20];
						sprintf(pszTime, "%4d-%02d-%02d %02d:%02d:%02d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);  
						string str = pszTime;
						String^ Str  = Tools::string2String(str.c_str());
						newrow[6] = Str;
					}

					for(int i = 0; i< 7; i++)
					{//更改原有历史风险事件列表的内容
						int nIndex = GetColumnIndex(HisRiskEventPanel1->HistRiskEvtList, i);											
						HisRiskEventPanel1->HistRiskEvtList->Rows[nRowIndex]->Cells[nIndex]->Value = newrow[i];
					}

					RiskEventHistory rHis;
					rHis.set(re.riskEvent, pRiskEvent->lEventTime);
					m_pdataInfo->m_mapHisEvent[pRiskEvent->nRiskEventID] = rHis;				
																				
				}
				else
				{//如果这种情况出现，那么风险事件的产生事件将没有
					array<String^>^ newrow=gcnew array<String^>(7);										
					RiskEvent2Array(pRiskEvent, newrow);
					newrow[6] ="";
					if(pRiskEvent->nMsgSendStatus == MsgStatus_Delete)
					{
						tm *pTmp = localtime(&pRiskEvent->lEventTime);
						char pszTime[20];
						sprintf(pszTime, "%4d-%02d-%02d %02d:%02d:%02d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);  
						string str = pszTime;
						String^ Str  = Tools::string2String(str.c_str());
						newrow[6] = Str;
					}
					HisRiskEventPanel1->HistRiskEvtList->Rows->Add(newrow);		

					RiskEventHistory rHis;
					rHis.set(*pRiskEvent, pRiskEvent->lEventTime);
					m_pdataInfo->m_mapHisEvent[pRiskEvent->nRiskEventID] = rHis;	
				}			
			}*/
		}
		void ProcessRiskEvent(RiskEvent* pRiskEvent, bool bTip)
		{
			if(pRiskEvent->nIsValid == RiskEvent_Realtime)
			{
				ProcessRiskEvent_Realtime(pRiskEvent, bTip);
			}
			else
			{
				ProcessRiskEvent_History(pRiskEvent);
			}
		
			
	/*		if(pRiskEvent->nIsValid == RiskEvent_Realtime)
			{//1 实时风险事件
				SRiskEventKey sKey;
				sKey.nTradeAccountID	= pRiskEvent->nTradeAccountID;
				sKey.strInstrumentID	= pRiskEvent->InstrumentID;
				sKey.nRiskIndicatorID	= pRiskEvent->nRiskIndicatorID;
				MAPKey2RiskEvent::iterator it = m_pdataInfo->m_mapRiskEvtID2RiskEvent.find(sKey);
				if(it == m_pdataInfo->m_mapRiskEvtID2RiskEvent.end())
				{//列表中没有相应的数据
					m_pdataInfo->m_mapRiskEvtID2RiskEvent.insert(make_pair(sKey, *pRiskEvent));	// 增加

					array<String^>^ newrow=gcnew array<String^>(6);
					RiskEvent2Array(pRiskEvent, newrow);

				
					CurRiskEventPanel1->CurRiskEvtList->Rows->Add(newrow);
				
					return;
				}
				else
				{//有相应的数据
					int nRowIndex = 0;							
					if(GetExistRowIndex(pRiskEvent, CurRiskEventPanel1->CurRiskEvtList,  nRowIndex))
					{
						RiskEvent re = it->second;//包含开始时间

						array<String^>^ newrow=gcnew array<String^>(6);
						RiskEvent2Array(pRiskEvent, newrow);

						for(int i = 0; i< 6; i++)
						{//更改原有风险事件列表的内容
							int nIndex = GetColumnIndex(CurRiskEventPanel1->CurRiskEvtList, i);											
							CurRiskEventPanel1->CurRiskEvtList->Rows[nRowIndex]->Cells[nIndex]->Value = newrow[i];
						}

						array<String^>^ newrowLast=gcnew array<String^>(7);
						RiskEvent2Array(&re, newrowLast);
						tm *pTmp = localtime(&pRiskEvent->lEventTime);
						char pszTime[20];
						sprintf(pszTime, "%4d-%02d-%02d %02d:%02d:%02d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);  
						string str = pszTime;
						String^ Str  = Tools::string2String(str.c_str());
						newrowLast[6] = Str;
						HisRiskEventPanel1->HistRiskEvtList->Rows->Add(newrowLast);											
					}
					m_pdataInfo->m_mapRiskEvtID2RiskEvent[sKey] = *pRiskEvent;//修改
				}
			}
			else if(pRiskEvent->nIsValid == RiskEvent_History)
			{//0 是历史风险事件
				SRiskEventKey sKey;
				sKey.nTradeAccountID	= pRiskEvent->nTradeAccountID;
				sKey.strInstrumentID	= pRiskEvent->InstrumentID;
				sKey.nRiskIndicatorID	= pRiskEvent->nRiskIndicatorID;
				MAPKey2RiskEvent::iterator it = m_pdataInfo->m_mapRiskEvtID2RiskEvent.find(sKey);
				if(it == m_pdataInfo->m_mapRiskEvtID2RiskEvent.end())
				{
					array<String^>^ newrow=gcnew array<String^>(7);										
					RiskEvent2Array(pRiskEvent, newrow);
					tm *pTmp = localtime(&pRiskEvent->lEventTime);
					char pszTime[20];
					sprintf(pszTime, "%4d-%02d-%02d %02d:%02d:%02d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);  
					string str = pszTime;
					String^ Str  = Tools::string2String(str.c_str());
					newrow[6] = Str;
					HisRiskEventPanel1->HistRiskEvtList->Rows->Add(newrow);		
				}
				else
				{//历史风险事件已经在当前风险事件表里面显示（需要删除，然后再历史风险事件表里面添加记录）
					int nRowIndex = 0;							
					if(GetExistRowIndex(pRiskEvent, CurRiskEventPanel1->CurRiskEvtList,  nRowIndex))
					{
						RiskEvent re = it->second;//包含开始时间
						array<String^>^ newrow=gcnew array<String^>(7);										
						RiskEvent2Array(&re, newrow);
						tm *pTmp = localtime(&pRiskEvent->lEventTime);
						char pszTime[20];
						sprintf(pszTime, "%4d-%02d-%02d %02d:%02d:%02d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);  
						string str = pszTime;
						String^ Str  = Tools::string2String(str.c_str());
						newrow[6] = Str;
						HisRiskEventPanel1->HistRiskEvtList->Rows->Add(newrow);		

						CurRiskEventPanel1->CurRiskEvtList->Rows->RemoveAt(nRowIndex);											
					}
					else
					{//如果这种情况出现，那么风险事件的产生事件将没有
						array<String^>^ newrow=gcnew array<String^>(7);										
						RiskEvent2Array(pRiskEvent, newrow);
						tm *pTmp = localtime(&pRiskEvent->lEventTime);
						char pszTime[20];
						sprintf(pszTime, "%4d-%02d-%02d %02d:%02d:%02d", pTmp->tm_year + 1900, pTmp->tm_mon + 1, pTmp->tm_mday, pTmp->tm_hour, pTmp->tm_min, pTmp->tm_sec);  
						string str = pszTime;
						String^ Str  = Tools::string2String(str.c_str());
						newrow[6] = Str;
						HisRiskEventPanel1->HistRiskEvtList->Rows->Add(newrow);		
					}
					m_pdataInfo->m_mapRiskEvtID2RiskEvent.erase(sKey);
				}
			}*/
		}
      void ProcessRiskEventHandling(RiskEventHandling* pRiskEventHandling)
		{
			array<String^>^ newrow=gcnew array<String^>(11);
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
			str ="";			
			TrustTradeAccount fP;
			if(m_pdataInfo->GetTrustTradeAccount(pRiskEventHandling->nTradeAccountID, fP))
			{
				str = fP.szAccount;					
			}
			Str  = Tools::string2String(str.c_str());
			newrow[1] = Str;
//////////////////////////////////////////////////////////////////////////
			RiskEvent	riskEvent, riskEventLastOne;
			int ncount = m_pdataInfo->m_mapCurRiskEvent.count(pRiskEventHandling->nRiskEventID);
			MAPInt2RiskEvent::iterator it = m_pdataInfo->m_mapCurRiskEvent.find(pRiskEventHandling->nRiskEventID);
			MAPInt2RiskEvent::iterator itLast = it;
			for(int nIndex = 0; nIndex < ncount; nIndex ++)
			{
				if(pRiskEventHandling->nProcessStatus == MsgStatus_Delete)
				{
					if(it->second.nRiskEventSubID == pRiskEventHandling->nRiskEventSubID-1)
					{//主要是兼容服务端最后一个消失的风险事件没有消失之前的信息
						riskEvent = it->second;	
						break;
					}
				}
				else if(it->second.nRiskEventSubID == pRiskEventHandling->nRiskEventSubID)
				{
					riskEvent = it->second;	
					break;
				}				
				it ++;				  
			}
			for(int nIndex = 0; nIndex < ncount; nIndex ++)
			{
				if(itLast->second.nRiskEventSubID == pRiskEventHandling->nRiskEventSubID)
				{
					riskEventLastOne = itLast->second;	
					break;
				}				
				itLast ++;				  
			}
			//风险指标类型
			stream.clear();    
			stream.str("");
			str = GetRiskIndicatorIDString(riskEvent.nRiskIndicatorID);
			Str  = Tools::string2String(str.c_str());
			newrow[2] = Str;

			//风险详情
			stream.clear();    
			stream.str("");
			stream   <<  riskEvent.nRiskLevelID;
			string strTemp;
			strTemp = stream.str();
			if(riskEvent.nRiskIndicatorID == RI_MaxPosition)
			{
				string strConstract = riskEvent.InstrumentID;
				str = strConstract+ ": " + strTemp;
			}
			else if(riskEvent.nRiskIndicatorID == RI_ProductMaxPositon)
			{
				string strConstract = riskEvent.InstrumentID;
				str = strConstract+ ": " + strTemp;
			}
			else 
				str = strTemp;
			Str  = Tools::string2String(str.c_str());
			newrow[3] = Str;
			//指标值
			char szBuffer[100];
			memset(szBuffer, 0, sizeof(szBuffer));
			if(riskEvent.nRiskIndicatorID == RI_MaxPosition || riskEvent.nRiskIndicatorID == RI_ProductMaxPositon)
			{
				sprintf(szBuffer, "%.0f手", riskEventLastOne.dblIndicatorValue);
			}
			else if(riskEvent.nRiskIndicatorID == RI_FundNetValue)
			{
				sprintf(szBuffer, "%.6f", riskEventLastOne.dblIndicatorValue);
			}
			else
				sprintf(szBuffer, "%.2f%%", riskEventLastOne.dblIndicatorValue*100);

			str = szBuffer;
			Str  = Tools::string2String(str.c_str());
			newrow[4] = Str;
//////////////////////////////////////////////////////////////////////////
			//处理人
			stream.clear();    
			stream.str("");
			str  = pRiskEventHandling->szHandler;
			Str  = Tools::string2String(str.c_str());
			newrow[5] = Str;

			//受理人
			stream.clear();    
			stream.str("");
			str  = pRiskEventHandling->szAccepter;
			Str  = Tools::string2String(str.c_str());
			newrow[6] = Str;

			//风险事件状态
			stream.clear();    
			stream.str("");
			str  = GetMsgStatusTypeString(pRiskEventHandling->nProcessStatus);
			Str  = Tools::string2String(str.c_str());
			newrow[7] = Str;
			
			//动作
			stream.clear();    
			stream.str("");
			str  = GetEventHandlingActionTypeString(pRiskEventHandling->nAction);
			Str  = Tools::string2String(str.c_str());
			newrow[8] = Str;

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
			newrow[9] = Str;

			str  = pRiskEventHandling->szContent;
			Str  = Tools::string2String(str.c_str());
			newrow[10] = Str;	
			RiskEventRecord1->dataGridView1->Rows->Add(newrow);	
		}
#pragma endregion

			 //重载WndProc，以处理WM_COMMAND消息
	protected:virtual void WndProc( Message% m ) override
			{
				if (  m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival )
				{
					char *p = (char*)m.LParam.ToInt32();
					Stru_UniPkgHead head;					
					memset(&head, 0, sizeof(head));
					memcpy(&head, p, sizeof(head));

					if ( head.cmdid == Cmd_RM_SubscribeRiskEvent_Rsp )
					{
						if ( head.userdata1 == OPERATOR_SUCCESS )
						{								
							int nCount = head.len / sizeof(RiskEvent);
							for ( int i = 0; i < nCount; i++ )
							{
								RiskEvent* pRiskEvent = (RiskEvent*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(RiskEvent));
								ProcessRiskEvent(pRiskEvent, false);														
							}				
						}
						else
						{
						//	MessageBox::Show("用户名或者密码错误，登录失败！", "提示",MessageBoxButtons::OK);
						}
					}
					else if ( head.cmdid == Cmd_RM_RiskEvent_Push )
					{
						if ( head.userdata1 == OPERATOR_SUCCESS )
						{								
							int nCount = head.len / sizeof(RiskEvent);
							for ( int i = 0; i < nCount; i++ )
							{
								RiskEvent* pRiskEvent = (RiskEvent*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(RiskEvent));
								ProcessRiskEvent(pRiskEvent, true);														
							}				
						}
						else
						{
						//	MessageBox::Show("用户名或者密码错误，登录失败！", "提示",MessageBoxButtons::OK);
						}
					}
					else if (head.cmdid == Cmd_RM_SubscribeRiskEvtHandling_Rsp  )
					{
						if ( head.userdata1 == OPERATOR_SUCCESS )
						{								
							int nCount = head.len / sizeof(RiskEventHandling);
							for ( int i = 0; i < nCount; i++ )
							{
								RiskEventHandling* pRiskEventHandling = (RiskEventHandling*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(RiskEventHandling));
								m_pdataInfo->SetRiskEventHandling(pRiskEventHandling);	
								
								if(pRiskEventHandling->nRiskEventID == 6377735)
								{
									int i =0;
								}
								int nAccountID = RiskEventRecord1->GetCurrentAccountID();
								if(nAccountID == pRiskEventHandling->nTradeAccountID)
									ProcessRiskEventHandling(pRiskEventHandling);									
							}
						}
						else
						{

						}
					}
					else if ( Cmd_RM_RiskEvtHandling_Push == head.cmdid )
					{
						if ( head.userdata1 == OPERATOR_SUCCESS )
						{								
							int nCount = head.len / sizeof(RiskEventHandling);
							for ( int i = 0; i < nCount; i++ )
							{
								RiskEventHandling* pRiskEventHandling = (RiskEventHandling*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(RiskEventHandling));
								m_pdataInfo->SetRiskEventHandling(pRiskEventHandling);	

								int nAccountID = RiskEventRecord1->GetCurrentAccountID();
								if(nAccountID == pRiskEventHandling->nTradeAccountID)
									ProcessRiskEventHandling(pRiskEventHandling);									
							}
						}
						else
						{

						}
					}
				}
				Form::WndProc ( m );
			}
	private: System::Void RiskEvt_Load(System::Object^  sender, System::EventArgs^  e)
			{
				//订阅事件
				IntPtr hWnd=this->Handle;
				CTcpLayer::SubscribePkg(Cmd_RM_SubscribeRiskEvent_Rsp, (int)(hWnd.ToInt32()));
				CTcpLayer::SubscribePkg(Cmd_RM_RiskEvent_Push, (int)(hWnd.ToInt32()));
				CTcpLayer::SubscribePkg(Cmd_RM_SubscribeRiskEvtHandling_Rsp,(int)(hWnd.ToInt32()));
				CTcpLayer::SubscribePkg(Cmd_RM_RiskEvtHandling_Push,(int)(hWnd.ToInt32()));

				
				//订阅事件
				//IntPtr hWnd=this->Handle;
				//RiskEvt2::SubscribeEvt((unsigned int)(hWnd.ToInt32()));

				//demo: 向服务器端请求风控事件
				//RiskEvt2::ReqRiskEvt();
			}

//private: System::Void GetCheckedNode(System::Windows::Forms::TreeNodeCollection tnc)
//		 {
//			  for each(TreeNode^  node in tnc)
//			 {
//				 if(node->Checked)
//					 strAccount +=node->Text;
//			 }
//			  GetCheckedNode(node.Nodes); 
//		 }

private: System::Void treeView1_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
			// 
			// System::Windows::Forms::TreeNode^  node ;
			//// for(int i=0; i<this->treeView1->item)
			 //String^ strAccount;
			 //int i=strAccount->Length;
			// for each(TreeNode^  node in this->treeView1->Nodes)
			// {
			//	 if(node->Checked)
			//		 strAccount +=node->Text;
			// }

		/*	 GetCheckedNode(this->treeView1->Nodes);*/
			 
		 }
private: System::Void splitContainer2_Panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
		 {
			 int wid = splitContainer2->Panel1->Size.Width;
			 int hei = splitContainer2->Panel1->Size.Height;
			 int pox = splitContainer2->Panel1->Location.X;
			 int poy = splitContainer2->Panel1->Location.Y;

			if(CurRiskEventPanel1 != nullptr)
				CurRiskEventPanel1->SetBounds(pox, poy, wid, hei);

			int wid1 = splitContainer2->Panel2->Size.Width;
			int hei1 = splitContainer2->Panel2->Size.Height;
			int pox1 = splitContainer2->Panel2->Location.X;
			int poy1 = splitContainer2->Panel2->Location.Y;

			if(DlgAllAccountFundInfo1 != nullptr)
				DlgAllAccountFundInfo1->SetBounds(pox1, 0, wid1, hei1);
		 }
private: System::Void splitContainer3_Panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) 
		 {
			 int wid = splitContainer3->Panel1->Size.Width;
			 int hei = splitContainer3->Panel1->Size.Height;
			 int pox = splitContainer3->Panel1->Location.X;
			 int poy = splitContainer3->Panel1->Location.Y;

			 if(RiskEventRecord1 != nullptr)
				 RiskEventRecord1->SetBounds(pox, poy, wid, hei);

			 int wid1 = splitContainer2->Panel2->Size.Width;
			 int hei1 = splitContainer2->Panel2->Size.Height;
			 int pox1 = splitContainer2->Panel2->Location.X;
			 int poy1 = splitContainer2->Panel2->Location.Y;

			 if(HisRiskEventPanel1 != nullptr)
				 HisRiskEventPanel1->SetBounds(pox1, 0, wid1, hei1);
		 }
private: System::Void splitContainer1_SizeChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
		 }
private: System::Void splitContainer1_SplitterMoved(System::Object^  sender, System::Windows::Forms::SplitterEventArgs^  e) 
		 {
			 int wid1 = splitContainer1->Panel2->Size.Width;
			 int hei1 = splitContainer1->Panel2->Size.Height;
			 int pox1 = splitContainer1->Panel2->Location.X;
			 int poy1 = splitContainer1->Panel2->Location.Y;

			 if(CurRiskEventPanel1 != nullptr)
				 RiskEventRecord1->SetBounds(0, 0, wid1, hei1);
		 }
	
	private: System::Void RiskEvt_CurRiskEvent(Object^ sender, RiskEventEventArgs^ e)
			 {
				 HisRiskEventPanel1->HistRiskEvtList->ClearSelection();
				 int nID = e->nID;
				 if(nID == -1)
				 {
					
					 return;
				 }
				 int nAccountID = RiskEventRecord1->GetCurrentAccountID();
				 if(nID == nAccountID)
					 return;
				 RiskEventRecord1->SetCurrentAccountID(nID);

				 vector<RiskEventHandling> vexRiskEventHandling;
				 m_pdataInfo->GetRiskEvent(nID, -1, vexRiskEventHandling);

				 int nRowCount = RiskEventRecord1->dataGridView1->Rows->Count-1;//多了一行空白行，不知道怎么回事
				 for(int i = nRowCount -1; i >= 0; i-- )
					RiskEventRecord1->dataGridView1->Rows->RemoveAt(i);	

				 for(int i =0; i< (int)vexRiskEventHandling.size(); i++)
				 {				
					RiskEventHandling* pRiskEventHandling = &vexRiskEventHandling[i];
					this->ProcessRiskEventHandling(pRiskEventHandling);	
				 }

				  RiskEventRecord1->dataGridView1->ClearSelection(); 
				
			 }
	 private: System::Void RiskEvt_HisRiskEvent(Object^ sender, RiskEventEventArgs^ e)
			  {
				   CurRiskEventPanel1->CurRiskEvtList->ClearSelection();	
				  int nID = e->nID;
				  if(nID == -1)
				  {					
					  return;
				  }				  

				  int nAccountID = RiskEventRecord1->GetCurrentAccountID();
				  if(nID == nAccountID)
					  return;
				  RiskEventRecord1->SetCurrentAccountID(nID);

				  vector<RiskEventHandling> vexRiskEventHandling;
				  m_pdataInfo->GetRiskEvent(nID, -1, vexRiskEventHandling);

				  int nRowCount = RiskEventRecord1->dataGridView1->Rows->Count-1;//多了一行空白行，不知道怎么回事
				  for(int i = nRowCount -1; i >= 0; i-- )
					  RiskEventRecord1->dataGridView1->Rows->RemoveAt(i);	
					
				 
				  for(int i =0; i< (int)vexRiskEventHandling.size(); i++)
				  {				
					  RiskEventHandling* pRiskEventHandling = &vexRiskEventHandling[i];
					  this->ProcessRiskEventHandling(pRiskEventHandling);	
				  }
				  RiskEventRecord1->dataGridView1->ClearSelection();
			
			  }
	 private: System::Void RiskEvt_CurRiskEventDBClcik(Object^ sender, RiskEventEventArgs^ e)
		    {	
				int nID = e->nID;
				int ncount = m_pdataInfo->m_mapCurRiskEvent.count(nID);
				MAPInt2RiskEvent::iterator it = m_pdataInfo->m_mapCurRiskEvent.find(nID);
				RiskEvent riskEvent;
				if(0 == ncount)
					return;
				int nSubItem =-1;
				for(int nIndex = 0; nIndex < ncount; nIndex ++)
				{
					if(it->second.nRiskEventSubID > nSubItem)
					{
						nSubItem = it->second.nRiskEventSubID;
						riskEvent = it->second;					
					}				
					it ++;					
				}						
				TrustTradeAccount fP;
				if(!m_pdataInfo->GetTrustTradeAccount(riskEvent.nTradeAccountID, fP))
					return;

				TraderInfo	 sTraderInfo;
				if(!m_pdataInfo->GetTraderInfo(fP.nTraderID, sTraderInfo))
				{
					strcpy(sTraderInfo.szName, "");//没有设置交易员
				}

				RiskEvtProcess1->InitDialog();
				RiskEvtProcess1->SetRiskEvent(riskEvent);
				string strAccountName = sTraderInfo.szName;
				String^ strtemp = Tools::string2String(strAccountName.c_str());				
				RiskEvtProcess1->SetTextBox1(strtemp);
				RiskEvtProcess1->ShowDialog();
			}		
private: System::Void RiskEvt_HisRiskEventDBClcik(Object^ sender, RiskEventEventArgs^ e)
		 {	
			 int nID = e->nID;
			 MAPInt2RiskEventHistory::iterator it = m_pdataInfo->m_mapHisEvent.find(nID);
			 RiskEventHistory riskEvent;
			 if(it == m_pdataInfo->m_mapHisEvent.end())
				 return;
			  
			 riskEvent = it->second;				
			 TrustTradeAccount fP;
			 if(!m_pdataInfo->GetTrustTradeAccount(riskEvent.riskEvent.nTradeAccountID, fP))
				 return;
			 
			 TraderInfo	 sTraderInfo;
			 if(!m_pdataInfo->GetTraderInfo(fP.nTraderID, sTraderInfo))
			 {
					strcpy(sTraderInfo.szName, "");//没有设置交易员
			 }

			 RiskEvtProcess1->InitDialog();
			 RiskEvtProcess1->SetRiskEvent(riskEvent.riskEvent);
			 string strAccountName = sTraderInfo.szName;
			 String^ strtemp = Tools::string2String(strAccountName.c_str());				
			 RiskEvtProcess1->SetTextBox1(strtemp);
			 RiskEvtProcess1->ShowDialog();
		 }		

			  
};
}





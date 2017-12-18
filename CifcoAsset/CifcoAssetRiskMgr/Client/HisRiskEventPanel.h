#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
#include "Tools.h"
#include "RiskEventMessage.h"
using namespace std;
#include <string>
#include "CDataInfo.h"
namespace Client {

	/// <summary>
	/// Summary for HisRiskEventPanel
	/// </summary>
	public ref class HisRiskEventPanel : public System::Windows::Forms::UserControl
	{
	public:
		HisRiskEventPanel(void)
		{
			m_pdataInfo = CDataInfo::NewInstance();
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
		delegate void HisRiskEventEventHandler(Object^ sender, RiskEventEventArgs^ e);
		// 定义事件
		event HisRiskEventEventHandler^ HisRiskEvent1;


		delegate void HisRiskEvent_DBClciktEventHandler(Object^ sender, RiskEventEventArgs^ e);
		event HisRiskEvent_DBClciktEventHandler^ HisRiskEvent_DBClick;
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~HisRiskEventPanel()
		{
			CDataInfo::DestroyInstance();
			if (components)
			{
				delete components;
			}
		}
	public: CDataInfo*  m_pdataInfo;
	public: System::Windows::Forms::DataGridView^  HistRiskEvtList;
	protected: 

	protected: 
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_HistRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  TradeAccountID_HistRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  TradeAccountName_HistRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  RiskType_HisCurRisk;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Detail_HisCurRisk;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Time_HistRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  TimeDispare_HistRiskEvt;

	private: System::Windows::Forms::ContextMenuStrip^  contextMenuDataGrid;
	private: System::Windows::Forms::ToolStripMenuItem^  MenuItem_ProcessRiskEventRecord;
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
			this->HistRiskEvtList = (gcnew System::Windows::Forms::DataGridView());
			this->ID_HistRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->TradeAccountID_HistRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->TradeAccountName_HistRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->RiskType_HisCurRisk = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Detail_HisCurRisk = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Time_HistRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->TimeDispare_HistRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->HistRiskEvtList))->BeginInit();
			this->SuspendLayout();
			// 
			// HistRiskEvtList
			// 
			this->HistRiskEvtList->AllowUserToResizeRows = false;
			this->HistRiskEvtList->BackgroundColor = System::Drawing::SystemColors::Control;
			this->HistRiskEvtList->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->HistRiskEvtList->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->HistRiskEvtList->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(7) {this->ID_HistRiskEvt, 
				this->TradeAccountID_HistRiskEvt, this->TradeAccountName_HistRiskEvt, this->RiskType_HisCurRisk, this->Detail_HisCurRisk, this->Time_HistRiskEvt, 
				this->TimeDispare_HistRiskEvt});
			this->HistRiskEvtList->Location = System::Drawing::Point(3, 24);
			this->HistRiskEvtList->MultiSelect = false;
			this->HistRiskEvtList->Name = L"HistRiskEvtList";
			this->HistRiskEvtList->RowHeadersVisible = false;
			this->HistRiskEvtList->RowTemplate->Height = 23;
			this->HistRiskEvtList->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->HistRiskEvtList->Size = System::Drawing::Size(539, 307);
			this->HistRiskEvtList->TabIndex = 0;
			this->HistRiskEvtList->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &HisRiskEventPanel::HistRiskEvtList_MouseDoubleClick);
			this->HistRiskEvtList->CellPainting += gcnew System::Windows::Forms::DataGridViewCellPaintingEventHandler(this, &HisRiskEventPanel::HistRiskEvtList_CellPainting);
			this->HistRiskEvtList->SelectionChanged += gcnew System::EventHandler(this, &HisRiskEventPanel::HistRiskEvtList_SelectionChanged);
			// 
			// ID_HistRiskEvt
			// 
			this->ID_HistRiskEvt->HeaderText = L"风险事件ID";
			this->ID_HistRiskEvt->Name = L"ID_HistRiskEvt";
			this->ID_HistRiskEvt->ReadOnly = true;
			// 
			// TradeAccountID_HistRiskEvt
			// 
			this->TradeAccountID_HistRiskEvt->HeaderText = L"委托交易账户ID";
			this->TradeAccountID_HistRiskEvt->Name = L"TradeAccountID_HistRiskEvt";
			this->TradeAccountID_HistRiskEvt->ReadOnly = true;
			// 
			// TradeAccountName_HistRiskEvt
			// 
			this->TradeAccountName_HistRiskEvt->HeaderText = L"委托交易账户名称";
			this->TradeAccountName_HistRiskEvt->Name = L"TradeAccountName_HistRiskEvt";
			this->TradeAccountName_HistRiskEvt->ReadOnly = true;
			// 
			// RiskType_HisCurRisk
			// 
			this->RiskType_HisCurRisk->HeaderText = L"风险指标类型";
			this->RiskType_HisCurRisk->Name = L"RiskType_HisCurRisk";
			this->RiskType_HisCurRisk->ReadOnly = true;
			// 
			// Detail_HisCurRisk
			// 
			this->Detail_HisCurRisk->HeaderText = L"风险详情";
			this->Detail_HisCurRisk->Name = L"Detail_HisCurRisk";
			this->Detail_HisCurRisk->ReadOnly = true;
			// 
			// Time_HistRiskEvt
			// 
			this->Time_HistRiskEvt->HeaderText = L"发生时间";
			this->Time_HistRiskEvt->Name = L"Time_HistRiskEvt";
			this->Time_HistRiskEvt->ReadOnly = true;
			// 
			// TimeDispare_HistRiskEvt
			// 
			this->TimeDispare_HistRiskEvt->HeaderText = L"消失时间";
			this->TimeDispare_HistRiskEvt->Name = L"TimeDispare_HistRiskEvt";
			this->TimeDispare_HistRiskEvt->ReadOnly = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(3, 4);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 12);
			this->label1->TabIndex = 1;
			this->label1->Text = L"历史风险事件";
			// 
			// HisRiskEventPanel
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::GrayText;
			this->Controls->Add(this->label1);
			this->Controls->Add(this->HistRiskEvtList);
			this->Name = L"HisRiskEventPanel";
			this->Size = System::Drawing::Size(545, 348);
			this->SizeChanged += gcnew System::EventHandler(this, &HisRiskEventPanel::HisRiskEventPanel_SizeChanged);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->HistRiskEvtList))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

			this->components = (gcnew System::ComponentModel::Container());
			this->contextMenuDataGrid = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->MenuItem_ProcessRiskEventRecord = (gcnew System::Windows::Forms::ToolStripMenuItem());

			// contextMenuDataGrid
			// 
			this->contextMenuDataGrid->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->MenuItem_ProcessRiskEventRecord});
			this->contextMenuDataGrid->Name = L"contextMenuDataGrid";
			this->contextMenuDataGrid->Size = System::Drawing::Size(119, 54);

			// 
			// MenuItem_SetColumns 
			this->MenuItem_ProcessRiskEventRecord->Name = L"MenuItem_SetColumns";
			this->MenuItem_ProcessRiskEventRecord->Size = System::Drawing::Size(118, 22);
			this->MenuItem_ProcessRiskEventRecord->Text = L"风险事件处理记录";
			this->MenuItem_ProcessRiskEventRecord->Click += gcnew System::EventHandler(this, &HisRiskEventPanel::ProcessRiskEventRecord);

			this->HistRiskEvtList->ContextMenuStrip = this->contextMenuDataGrid;
			this->contextMenuDataGrid->SuspendLayout();

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
#pragma endregion
	private: System::Void HisRiskEventPanel_SizeChanged(System::Object^  sender, System::EventArgs^  e)
			 {
				 int wid=this->Size.Width;
				 int hei=this->Size.Height;
				 int pox=this->Location.X;
				 int poy=this->Location.Y;

				 HistRiskEvtList->SetBounds(pox, 20, wid, hei-20);
			 }
private: System::Void HistRiskEvtList_SelectionChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 int nSelects = HistRiskEvtList->SelectedRows->Count;
			 if(nSelects == 0)
				 return;
			 int nIndex = GetColumnIndex(HistRiskEvtList, 0);
			 if(nIndex == -1)
			 {
				 RiskEventEventArgs^ AccountArgs = gcnew RiskEventEventArgs( -1);
				 HisRiskEvent1( this, AccountArgs );
				 return;
			 }
			 DataGridViewRow^ cur =   HistRiskEvtList->CurrentRow;
			 if(cur->Cells[nIndex]->Value == nullptr)
			 {
				 RiskEventEventArgs^ AccountArgs = gcnew RiskEventEventArgs( -1);
				 HisRiskEvent1( this, AccountArgs );
				 return;
			 }
			 String^ str = cur->Cells[nIndex]->Value->ToString();

			 string strText = Tools::String2string(str);
			 int nID = atoi(strText.c_str());
			 MAPInt2RiskEventHistory::iterator it = m_pdataInfo->m_mapHisEvent.find(nID);
			 if(it != m_pdataInfo->m_mapHisEvent.end())
			 {
				 RiskEventHistory riskEvent = it->second;

				 int nAccountID = riskEvent.riskEvent.nTradeAccountID;
				 RiskEventEventArgs^ AccountArgs = gcnew RiskEventEventArgs( nAccountID);
				 HisRiskEvent1( this, AccountArgs );

			 }
			
		 }
private:System::Void HistRiskEvtList_ProcessRiskEvent()
		{
			int nSelects =HistRiskEvtList->SelectedRows->Count;
			if(nSelects == 0)
				return;

			int nIndex = GetColumnIndex(HistRiskEvtList, 0);
			if(nIndex == -1)
			{				
				return;
			}
			int nColCount = HistRiskEvtList->ColumnCount;

			DataGridViewRow^ cur =   HistRiskEvtList->CurrentRow;
			if(cur->Cells[nIndex]->Value == nullptr)
			{				
				return;
			}
			String^ str = cur->Cells[nIndex]->Value->ToString();

			string strText = Tools::String2string(str);
			int nID = atoi(strText.c_str());

			RiskEventEventArgs^ AccountArgs = gcnew RiskEventEventArgs( nID);
			HisRiskEvent_DBClick( this, AccountArgs );
		}
private: System::Void HistRiskEvtList_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 HistRiskEvtList_ProcessRiskEvent();
		 }
private: System::Void HistRiskEvtList_CellPainting(System::Object^  sender, System::Windows::Forms::DataGridViewCellPaintingEventArgs^  e) 
		 {
			 if (e->RowIndex < 0)  
				 return;    

			 int nIndex = GetColumnIndex(HistRiskEvtList, 0);//Cells[1]代表风险事件ID
			 DataGridViewRow^ His = HistRiskEvtList->Rows[e->RowIndex];  		
			 if(His->Cells[nIndex]->Value == nullptr)
			 {			 
				 return ;
			 }
			 String^ str = His->Cells[nIndex]->Value->ToString();
			 string strText = Tools::String2string(str);
			 int nID = atoi(strText.c_str());

			 RiskEventHistory	riskEvent;
			 MAPInt2RiskEventHistory::iterator it = m_pdataInfo->m_mapHisEvent.find(nID);
			 if(it != m_pdataInfo->m_mapHisEvent.end())
			 {
				 riskEvent = it->second;				 
			 }			
			 int nColumnIndex = GetColumnIndex(HistRiskEvtList, e->ColumnIndex);
			 try  
			 {  
				 if(e->ColumnIndex == 4)//风险详情列
				 { 	
					 ProductRiskWarningKey key;
					 key.nRiskIndicatorID = riskEvent.riskEvent.nRiskIndicatorID;
					 key.nRiskLevelID     = riskEvent.riskEvent.nRiskLevelID;
					 key.nTradeAccountID	 = riskEvent.riskEvent.nTradeAccountID;
					 InstrumentField instrumentField;
					 if(riskEvent.riskEvent.nRiskIndicatorID == RI_MaxPosition)
					 {
						 if(!m_pdataInfo->GetInstrumentFieldByInstrumentID(riskEvent.riskEvent.InstrumentID, instrumentField))
							 return;
						  key.szProductID      = instrumentField.ProductID;
					 }
					 else if(riskEvent.riskEvent.nRiskIndicatorID == RI_ProductMaxPositon)
						  key.szProductID      = riskEvent.riskEvent.InstrumentID;
					

					 ProductRiskWarning rw;						
					 if(riskEvent.riskEvent.nRiskIndicatorID == RI_MaxPosition || riskEvent.riskEvent.nRiskIndicatorID == RI_ProductMaxPositon)
					 {
						 if(m_pdataInfo->GetProductRiskWarning(key,  rw))
						 {
							 int R = rw.nColor & 0xff;
							 int G = (rw.nColor>>8)&0xff;
							 int B = (rw.nColor >>16) & 0xff;
							 e->CellStyle->ForeColor =  Color::FromArgb(R,G,B);
							 return;
						 }
					 }
					
					RiskWarning rw_Risk;
					RiskWarningKey key_Risk;
					key_Risk.nRiskIndModuleID = riskEvent.riskEvent.nRiskIndModuleID;
					key_Risk.nRiskIndicatorID = riskEvent.riskEvent.nRiskIndicatorID;
					key_Risk.nRiskLevelID	 = riskEvent.riskEvent.nRiskLevelID;							
					if(m_pdataInfo->GetRiskWarning(key_Risk,  rw_Risk))
					{
						int R = rw_Risk.nColor & 0xff;
						int G = (rw_Risk.nColor>>8)&0xff;
						int B = (rw_Risk.nColor >>16) & 0xff;
						// e->CellStyle->ForeColor =  Color::FromArgb(rw.nColor);
						e->CellStyle->ForeColor =  Color::FromArgb(R,G,B);					
					}
					
				 }  
			 }  
			 catch (Exception^ ex) 
			 {   
				 MessageBox::Show(ex->Message); 
			 } 
		 }
		
 private: System::Void ProcessRiskEventRecord(System::Object^  sender, System::EventArgs^  e) 
		  {
			 HistRiskEvtList_ProcessRiskEvent();
		  }
};
}

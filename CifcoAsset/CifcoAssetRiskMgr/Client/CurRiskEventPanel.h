#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
#include "Tools.h"
#include "CDataInfo.h"
#include "RiskEventMessage.h"
	/// <summary>
	/// Summary for CurRiskEventPanel
	/// </summary>
	public ref class CurRiskEventPanel : public System::Windows::Forms::UserControl
	{
	public:
		CurRiskEventPanel()
		{		
			m_pdataInfo = CDataInfo::NewInstance();
			InitializeComponent();
			bool bPermission = m_pdataInfo->HasPermission(FORCE_CLOSE_ORDER);
			this->MenuItem_FastForceClose->Enabled = bPermission;

			this->MenuItem_FastForceClose->Enabled = false;//快捷强平暂时不可用
		}
		delegate void CurRiskEventEventHandler(Object^ sender, RiskEventEventArgs^ e);
		// 定义事件
		event CurRiskEventEventHandler^ CurRiskEvent1;

		delegate void CurRiskEvent_DBClciktEventHandler(Object^ sender, RiskEventEventArgs^ e);
		event CurRiskEvent_DBClciktEventHandler^ CurRiskEvent_DBClick;
		
		
		delegate void CurRiskEvent_FastForceEventHandler(Object^ sender, RiskEventEventArgs^ e);
		event CurRiskEvent_FastForceEventHandler^ CurRiskEvent_FastForce;

		delegate void CurRiskEvent_ManualForceEventHandler(Object^ sender, RiskEventEventArgs^ e);
		event CurRiskEvent_ManualForceEventHandler^ CurRiskEvent_ManualForce;
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CurRiskEventPanel()
		{
			CDataInfo::DestroyInstance();
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label1;
	public: System::Windows::Forms::DataGridView^  CurRiskEvtList;
	protected: 

	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ID_CurRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  TradeAccountID_CurRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  TradeAccountName_CurRiskEvt;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  RiskType_CurRisk;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Detail_CurRisk;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Time_CurRiskEvt;

	public: CDataInfo*  m_pdataInfo;
	

	private: System::Windows::Forms::ContextMenuStrip^  contextMenuDataGrid;
	private: System::Windows::Forms::ToolStripMenuItem^  MenuItem_FastForceClose;
	private: System::Windows::Forms::ToolStripMenuItem^  ManualItem_ManualForceClose;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripMenuItem1;
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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->CurRiskEvtList = (gcnew System::Windows::Forms::DataGridView());
			this->ID_CurRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->TradeAccountID_CurRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->TradeAccountName_CurRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->RiskType_CurRisk = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Detail_CurRisk = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Time_CurRiskEvt = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CurRiskEvtList))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(3, 3);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(77, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"当前风险事件";
			// 
			// CurRiskEvtList
			// 
			this->CurRiskEvtList->AllowUserToResizeRows = false;
			this->CurRiskEvtList->BackgroundColor = System::Drawing::SystemColors::Control;
			this->CurRiskEvtList->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->CurRiskEvtList->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->CurRiskEvtList->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(6) {this->ID_CurRiskEvt, 
				this->TradeAccountID_CurRiskEvt, this->TradeAccountName_CurRiskEvt, this->RiskType_CurRisk, this->Detail_CurRisk, this->Time_CurRiskEvt});
			this->CurRiskEvtList->Location = System::Drawing::Point(10, 30);
			this->CurRiskEvtList->MultiSelect = false;
			this->CurRiskEvtList->Name = L"CurRiskEvtList";
			this->CurRiskEvtList->RowHeadersVisible = false;
			this->CurRiskEvtList->RowTemplate->Height = 23;
			this->CurRiskEvtList->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->CurRiskEvtList->Size = System::Drawing::Size(476, 289);
			this->CurRiskEvtList->TabIndex = 1;
			this->CurRiskEvtList->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &CurRiskEventPanel::CurRiskEvtList_MouseDoubleClick);
			this->CurRiskEvtList->CellPainting += gcnew System::Windows::Forms::DataGridViewCellPaintingEventHandler(this, &CurRiskEventPanel::CurRiskEvtList_CellPainting);
			this->CurRiskEvtList->SelectionChanged += gcnew System::EventHandler(this, &CurRiskEventPanel::CurRiskEvtList_SelectionChanged);
			// 
			// ID_CurRiskEvt
			// 
			this->ID_CurRiskEvt->HeaderText = L"风险事件ID";
			this->ID_CurRiskEvt->Name = L"ID_CurRiskEvt";
			this->ID_CurRiskEvt->ReadOnly = true;
			// 
			// TradeAccountID_CurRiskEvt
			// 
			this->TradeAccountID_CurRiskEvt->HeaderText = L"委托交易账户ID";
			this->TradeAccountID_CurRiskEvt->Name = L"TradeAccountID_CurRiskEvt";
			this->TradeAccountID_CurRiskEvt->ReadOnly = true;
			// 
			// TradeAccountName_CurRiskEvt
			// 
			this->TradeAccountName_CurRiskEvt->HeaderText = L"委托交易账户名称";
			this->TradeAccountName_CurRiskEvt->Name = L"TradeAccountName_CurRiskEvt";
			this->TradeAccountName_CurRiskEvt->ReadOnly = true;
			// 
			// RiskType_CurRisk
			// 
			this->RiskType_CurRisk->HeaderText = L"风险指标类型";
			this->RiskType_CurRisk->Name = L"RiskType_CurRisk";
			this->RiskType_CurRisk->ReadOnly = true;
			// 
			// Detail_CurRisk
			// 
			this->Detail_CurRisk->HeaderText = L"风险详情";
			this->Detail_CurRisk->Name = L"Detail_CurRisk";
			this->Detail_CurRisk->ReadOnly = true;
			// 
			// Time_CurRiskEvt
			// 
			this->Time_CurRiskEvt->HeaderText = L"发生时间";
			this->Time_CurRiskEvt->Name = L"Time_CurRiskEvt";
			this->Time_CurRiskEvt->ReadOnly = true;
			// 
			// CurRiskEventPanel
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::GrayText;
			this->Controls->Add(this->CurRiskEvtList);
			this->Controls->Add(this->label1);
			this->Name = L"CurRiskEventPanel";
			this->Size = System::Drawing::Size(505, 338);
			this->SizeChanged += gcnew System::EventHandler(this, &CurRiskEventPanel::CurRiskEventPanel_SizeChanged);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->CurRiskEvtList))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();
			
			this->components = (gcnew System::ComponentModel::Container());
			this->contextMenuDataGrid = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->MenuItem_FastForceClose = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->ManualItem_ManualForceClose = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->MenuItem_ProcessRiskEventRecord = (gcnew System::Windows::Forms::ToolStripMenuItem());

		
			// contextMenuDataGrid
			// 
			this->contextMenuDataGrid->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->MenuItem_FastForceClose, this->ManualItem_ManualForceClose, 
				this->toolStripMenuItem1, this->MenuItem_ProcessRiskEventRecord});
			this->contextMenuDataGrid->Name = L"contextMenuDataGrid";
			this->contextMenuDataGrid->Size = System::Drawing::Size(119, 54);
			// 
			// MenuItem_FastForceClose
			// 
			this->MenuItem_FastForceClose->Name = L"MenuItem_FastForceClose";
			this->MenuItem_FastForceClose->Size = System::Drawing::Size(118, 22);
			this->MenuItem_FastForceClose->Text = L"快捷强平";
			this->MenuItem_FastForceClose->Click += gcnew System::EventHandler(this, &CurRiskEventPanel::MenuItem_FastForceClose_Click);
			// 
			// tool	
			// ManualItem_ManualForceClose
			// 
			this->ManualItem_ManualForceClose->Name = L"ManualItem_ManualForceClose";
			this->ManualItem_ManualForceClose->Size = System::Drawing::Size(118, 22);
			this->ManualItem_ManualForceClose->Text = L"手动强平";
			this->ManualItem_ManualForceClose->Click += gcnew System::EventHandler(this, &CurRiskEventPanel::MenuItem_ManualForceClose_Click);
			//	StripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(115, 6);
			// 
			// MenuItem_SetColumns 
			this->MenuItem_ProcessRiskEventRecord->Name = L"MenuItem_SetColumns";
			this->MenuItem_ProcessRiskEventRecord->Size = System::Drawing::Size(118, 22);
			this->MenuItem_ProcessRiskEventRecord->Text = L"风险事件处理记录";
			this->MenuItem_ProcessRiskEventRecord->Click += gcnew System::EventHandler(this, &CurRiskEventPanel::ProcessRiskEventRecord);

			this->CurRiskEvtList->ContextMenuStrip = this->contextMenuDataGrid;
			this->contextMenuDataGrid->SuspendLayout();
		}
	public:
		bool GetSelItemRiskEvent(RiskEvent& riskEvent)
		{
			int nSelects =CurRiskEvtList->SelectedRows->Count;
			if(nSelects == 0)
				return false;

			int nIndex = GetColumnIndex(CurRiskEvtList, 0);//Cells[1]代表风险事件ID
			DataGridViewRow^ cur =   CurRiskEvtList->CurrentRow;			
			if(cur->Cells[nIndex]->Value == nullptr)
			{			 
				return false;
			}
			String^ str = cur->Cells[nIndex]->Value->ToString();
			string strText = Tools::String2string(str);
			int nID = atoi(strText.c_str());

			int ncount = m_pdataInfo->m_mapCurRiskEvent.count(nID);
			MAPInt2RiskEvent::iterator it = m_pdataInfo->m_mapCurRiskEvent.find(nID);
			int nSubItem =-1;
			for(int nIndex = 0; nIndex < ncount; nIndex ++)
			{
				if(it->second.nRiskEventSubID > nSubItem)
				{
					nSubItem = it->second.nRiskEventSubID;
					riskEvent = it->second;					
				}				
				it ++;
				if(nIndex == ncount -1)
					return true;
			}		
			return false;
		}
		
		bool TradeAccountHasRiskEvent(int nTradeAccountID)
		{
			int nRowCount = CurRiskEvtList->Rows->Count-1;//多了一行空白行，不知道怎么回事
			int nColCount = CurRiskEvtList->ColumnCount;
			for(int nRow = 0; nRow < nRowCount; nRow ++)
			{
				int nIndex = GetColumnIndex(CurRiskEvtList, 1);//Cells[1]代表交易账号
				if(CurRiskEvtList->Rows[nRow]->Cells[nIndex]->Value == nullptr)
					continue;

				String^ str = CurRiskEvtList->Rows[nRow]->Cells[nIndex]->Value->ToString();
				string strTemp =  Tools::String2string(str);
				int nAccount = atoi(strTemp.c_str());
				if(nTradeAccountID == nAccount)//交易账号相同
				{
					return true;
				}
			}
			return false;
		}
	/*	int GetSelItemTradeAccount()
		{
			int nSelects =CurRiskEvtList->SelectedRows->Count;
			if(nSelects == 0)
				return -1;
			
			int nIndex = GetColumnIndex(CurRiskEvtList, 1);//Cells[1]代表交易账号
			DataGridViewRow^ cur =   CurRiskEvtList->CurrentRow;			
			if(cur->Cells[nIndex]->Value == nullptr)
			{			 
			  return -2;
			 }
			String^ str = cur->Cells[nIndex]->Value->ToString();
			string strText = Tools::String2string(str);
			int nID = atoi(strText.c_str());
			return nID;
		}*/
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
	private: System::Void CurRiskEventPanel_SizeChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 int wid=this->Size.Width;
				 int hei=this->Size.Height;
				 int pox=this->Location.X;
				 int poy=this->Location.Y;

				 CurRiskEvtList->SetBounds(pox, 20, wid, hei-20);
			 }
private: System::Void CurRiskEvtList_SelectionChanged(System::Object^  sender, System::EventArgs^  e)
		 {		
			int nSelects =CurRiskEvtList->SelectedRows->Count;
			if(nSelects == 0)
				return;

			 int nIndex = GetColumnIndex(CurRiskEvtList, 0);//风险事件ID
			 if(nIndex == -1)
			 {
				 RiskEventEventArgs^ AccountArgs = gcnew RiskEventEventArgs( -1);
				 CurRiskEvent1( this, AccountArgs );
				 return;
			 }
			 int nColCount = CurRiskEvtList->ColumnCount;

			 DataGridViewRow^ cur =   CurRiskEvtList->CurrentRow;
			 if(cur->Cells[nIndex]->Value == nullptr)
			 {
				 RiskEventEventArgs^ AccountArgs = gcnew RiskEventEventArgs( -1);
				 CurRiskEvent1( this, AccountArgs );
				 return;
			 }
			 String^ str = cur->Cells[nIndex]->Value->ToString();
			
			string strText = Tools::String2string(str);
			int nID = atoi(strText.c_str());
			int ncount = m_pdataInfo->m_mapCurRiskEvent.count(nID);
			MAPInt2RiskEvent::iterator it = m_pdataInfo->m_mapCurRiskEvent.find(nID);
			if(0 != ncount)
			{
				RiskEvent riskEvent = it->second;				
				int nAccountID = riskEvent.nTradeAccountID;
				RiskEventEventArgs^ AccountArgs = gcnew RiskEventEventArgs( nAccountID);
				CurRiskEvent1( this, AccountArgs );				
			}
			
		
			
		 }
private:System::Void CurRiskEvtList_ProcessRiskEvent()
		{
			int nSelects =CurRiskEvtList->SelectedRows->Count;
			if(nSelects == 0)
				return;

			int nIndex = GetColumnIndex(CurRiskEvtList, 0);
			if(nIndex == -1)
			{				
				return;
			}
			int nColCount = CurRiskEvtList->ColumnCount;

			DataGridViewRow^ cur =   CurRiskEvtList->CurrentRow;
			if(cur->Cells[nIndex]->Value == nullptr)
			{				
				return;
			}
			String^ str = cur->Cells[nIndex]->Value->ToString();

			string strText = Tools::String2string(str);
			int nID = atoi(strText.c_str());

			RiskEventEventArgs^ AccountArgs = gcnew RiskEventEventArgs( nID);
			CurRiskEvent_DBClick( this, AccountArgs );
		}
private: System::Void CurRiskEvtList_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {
			 CurRiskEvtList_ProcessRiskEvent();
		 }
private: System::Void CurRiskEvtList_CellPainting(System::Object^  sender, System::Windows::Forms::DataGridViewCellPaintingEventArgs^  e)
		 {				
			  if (e->RowIndex < 0)  
				return;    
			
			  int nIndex = GetColumnIndex(CurRiskEvtList, 0);//Cells[1]代表风险事件ID
			  DataGridViewRow^ cur = CurRiskEvtList->Rows[e->RowIndex];  		
			  if(cur->Cells[nIndex]->Value == nullptr)
			  {			 
				  return ;
			  }
			  String^ str = cur->Cells[nIndex]->Value->ToString();
			  string strText = Tools::String2string(str);
			  int nID = atoi(strText.c_str());
			 
			  RiskEvent	riskEvent;
			  int ncount = m_pdataInfo->m_mapCurRiskEvent.count(nID);
			  MAPInt2RiskEvent::iterator it = m_pdataInfo->m_mapCurRiskEvent.find(nID);
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
			  int nColumnIndex = GetColumnIndex(CurRiskEvtList, e->ColumnIndex);
			  try  
			  {  
				  if(e->ColumnIndex == 4)//风险详情列
				  { 	
						ProductRiskWarningKey key;
						key.nRiskIndicatorID = riskEvent.nRiskIndicatorID;
						key.nRiskLevelID     = riskEvent.nRiskLevelID;
						key.nTradeAccountID	 = riskEvent.nTradeAccountID;
						InstrumentField instrumentField;
						if(riskEvent.nRiskIndicatorID == RI_MaxPosition)
						{
							if(!m_pdataInfo->GetInstrumentFieldByInstrumentID(riskEvent.InstrumentID, instrumentField))							
								return;
							key.szProductID      = instrumentField.ProductID;
						}
						else if(riskEvent.nRiskIndicatorID == RI_ProductMaxPositon)
						{
							key.szProductID      = riskEvent.InstrumentID;
						}						

						ProductRiskWarning rw;						
						if(riskEvent.nRiskIndicatorID == RI_MaxPosition || riskEvent.nRiskIndicatorID == RI_ProductMaxPositon)
						{
							if( m_pdataInfo->GetProductRiskWarning(key,  rw))
							{
								int R = rw.nColor & 0xff;
								int G = (rw.nColor>>8)&0xff;
								int B = (rw.nColor >>16) & 0xff;
								e->CellStyle->ForeColor =  Color::FromArgb(R,G,B);
								return;
							}
						}
						RiskWarning rw2;
						RiskWarningKey key2;
						key2.nRiskIndModuleID = riskEvent.nRiskIndModuleID;
						key2.nRiskIndicatorID = riskEvent.nRiskIndicatorID;
						key2.nRiskLevelID	 = riskEvent.nRiskLevelID;	
						
						if(m_pdataInfo->GetRiskWarning(key2,  rw2))
						{
							int R = rw2.nColor & 0xff;
							int G = (rw2.nColor>>8)&0xff;
							int B = (rw2.nColor >>16) & 0xff;
							e->CellStyle->ForeColor =  Color::FromArgb(R,G,B);
						}
						
				  }  
			  }  
			  catch (Exception^ ex) 
			  {   
				  MessageBox::Show(ex->Message); 
			  } 
	     }
 private: System::Void MenuItem_FastForceClose_Click(System::Object^  sender, System::EventArgs^  e) 
		  {
			  RiskEventEventArgs^ AccountArgs = gcnew RiskEventEventArgs( 0);
			  CurRiskEvent_FastForce( this, AccountArgs );
		  }
 private: System::Void MenuItem_ManualForceClose_Click(System::Object^  sender, System::EventArgs^  e) 
		  {
			  RiskEventEventArgs^ AccountArgs = gcnew RiskEventEventArgs( 0);
			  CurRiskEvent_ManualForce( this, AccountArgs );
		  }
 private: System::Void ProcessRiskEventRecord(System::Object^  sender, System::EventArgs^  e) 
		  {
			 CurRiskEvtList_ProcessRiskEvent();
		  }
};


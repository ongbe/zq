#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


#include <string>
#include "Tools.h"
#include "CommonDef.h"
#include "CommonStruct.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "CCommonFunc.h"
#include "defineGridCols.h"
#include "CFormUserMsgQueryData.h"
#include "TcpLayer.h"
#include "RiskData.h"

using namespace std;


namespace CltRisk {

	/// <summary>
	/// Summary for CFormUserMsgQuery
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CFormUserMsgQuery : public System::Windows::Forms::Form
	{
	public:
		CFormUserMsgQuery(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			save = NULL;
			m_bHasExit = false;
			m_bHasChanged_QueryParam = true;
			save = CFormUserMsgQueryData::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CFormUserMsgQuery()
		{
			if (components)
			{
				delete components;
			}
		}
	private: CFormUserMsgQueryData* save;
	private: bool m_bHasExit;
	private: bool m_bHasChanged_QueryParam;
	private: unsigned long dwTcpStartTime;
	private: unsigned long dwTcpEndTime;
	private: unsigned long dwShowStartTime;
	private: unsigned long dwShowEndTime;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::DateTimePicker^  dtpStart;
	private: System::Windows::Forms::Button^  buttonQuerySend;
	private: System::Windows::Forms::Button^  buttonQueryRecv;
	private: System::Windows::Forms::Button^  buttonClearSend;
	private: System::Windows::Forms::Button^  buttonClearRecv;
	private: System::Windows::Forms::DataGridView^  dataGridSend;
	private: System::Windows::Forms::DataGridView^  dataGridRecv;
	protected: 

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
			this->dtpStart = (gcnew System::Windows::Forms::DateTimePicker());
			this->buttonQuerySend = (gcnew System::Windows::Forms::Button());
			this->buttonQueryRecv = (gcnew System::Windows::Forms::Button());
			this->buttonClearSend = (gcnew System::Windows::Forms::Button());
			this->buttonClearRecv = (gcnew System::Windows::Forms::Button());
			this->dataGridSend = (gcnew System::Windows::Forms::DataGridView());
			this->dataGridRecv = (gcnew System::Windows::Forms::DataGridView());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridSend))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridRecv))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(10, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"日期：";
			// 
			// dtpStart
			// 
			this->dtpStart->CustomFormat = L"yyyy-MM-dd";
			this->dtpStart->Location = System::Drawing::Point(57, 5);
			this->dtpStart->Name = L"dtpStart";
			this->dtpStart->Size = System::Drawing::Size(110, 21);
			this->dtpStart->TabIndex = 18;
			// 
			// buttonQuerySend
			// 
			this->buttonQuerySend->Location = System::Drawing::Point(10, 32);
			this->buttonQuerySend->Name = L"buttonQuerySend";
			this->buttonQuerySend->Size = System::Drawing::Size(128, 23);
			this->buttonQuerySend->TabIndex = 19;
			this->buttonQuerySend->Text = L"查询已发送消息";
			this->buttonQuerySend->UseVisualStyleBackColor = true;
			this->buttonQuerySend->Click += gcnew System::EventHandler(this, &CFormUserMsgQuery::buttonQuerySend_Click);
			// 
			// buttonQueryRecv
			// 
			this->buttonQueryRecv->Location = System::Drawing::Point(519, 32);
			this->buttonQueryRecv->Name = L"buttonQueryRecv";
			this->buttonQueryRecv->Size = System::Drawing::Size(128, 23);
			this->buttonQueryRecv->TabIndex = 20;
			this->buttonQueryRecv->Text = L"查询已接收消息";
			this->buttonQueryRecv->UseVisualStyleBackColor = true;
			this->buttonQueryRecv->Click += gcnew System::EventHandler(this, &CFormUserMsgQuery::buttonQueryRecv_Click);
			// 
			// buttonClearSend
			// 
			this->buttonClearSend->Location = System::Drawing::Point(144, 32);
			this->buttonClearSend->Name = L"buttonClearSend";
			this->buttonClearSend->Size = System::Drawing::Size(45, 23);
			this->buttonClearSend->TabIndex = 21;
			this->buttonClearSend->Text = L"清空";
			this->buttonClearSend->UseVisualStyleBackColor = true;
			this->buttonClearSend->Click += gcnew System::EventHandler(this, &CFormUserMsgQuery::buttonClearSend_Click);
			// 
			// buttonClearRecv
			// 
			this->buttonClearRecv->Location = System::Drawing::Point(653, 32);
			this->buttonClearRecv->Name = L"buttonClearRecv";
			this->buttonClearRecv->Size = System::Drawing::Size(45, 23);
			this->buttonClearRecv->TabIndex = 22;
			this->buttonClearRecv->Text = L"清空";
			this->buttonClearRecv->UseVisualStyleBackColor = true;
			this->buttonClearRecv->Click += gcnew System::EventHandler(this, &CFormUserMsgQuery::buttonClearRecv_Click);
			// 
			// dataGridSend
			// 
			this->dataGridSend->AllowUserToAddRows = false;
			this->dataGridSend->AllowUserToDeleteRows = false;
			this->dataGridSend->AllowUserToResizeRows = false;
			this->dataGridSend->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridSend->EditMode = System::Windows::Forms::DataGridViewEditMode::EditProgrammatically;
			this->dataGridSend->Location = System::Drawing::Point(10, 59);
			this->dataGridSend->Margin = System::Windows::Forms::Padding(1);
			this->dataGridSend->MultiSelect = false;
			this->dataGridSend->Name = L"dataGridSend";
			this->dataGridSend->ReadOnly = true;
			this->dataGridSend->RowHeadersVisible = false;
			this->dataGridSend->RowTemplate->Height = 23;
			this->dataGridSend->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridSend->Size = System::Drawing::Size(499, 461);
			this->dataGridSend->TabIndex = 23;
			// 
			// dataGridRecv
			// 
			this->dataGridRecv->AllowUserToAddRows = false;
			this->dataGridRecv->AllowUserToDeleteRows = false;
			this->dataGridRecv->AllowUserToResizeRows = false;
			this->dataGridRecv->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridRecv->EditMode = System::Windows::Forms::DataGridViewEditMode::EditProgrammatically;
			this->dataGridRecv->Location = System::Drawing::Point(519, 59);
			this->dataGridRecv->Margin = System::Windows::Forms::Padding(1);
			this->dataGridRecv->MultiSelect = false;
			this->dataGridRecv->Name = L"dataGridRecv";
			this->dataGridRecv->ReadOnly = true;
			this->dataGridRecv->RowHeadersVisible = false;
			this->dataGridRecv->RowTemplate->Height = 23;
			this->dataGridRecv->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridRecv->Size = System::Drawing::Size(374, 461);
			this->dataGridRecv->TabIndex = 24;
			// 
			// CFormUserMsgQuery
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(901, 529);
			this->Controls->Add(this->dataGridRecv);
			this->Controls->Add(this->dataGridSend);
			this->Controls->Add(this->buttonClearRecv);
			this->Controls->Add(this->buttonClearSend);
			this->Controls->Add(this->buttonQueryRecv);
			this->Controls->Add(this->buttonQuerySend);
			this->Controls->Add(this->dtpStart);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Name = L"CFormUserMsgQuery";
			this->Text = L"消息通知查询";
			this->Load += gcnew System::EventHandler(this, &CFormUserMsgQuery::CFormUserMsgQuery_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &CFormUserMsgQuery::CFormUserMsgQuery_FormClosed);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &CFormUserMsgQuery::CFormUserMsgQuery_FormClosing);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridSend))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridRecv))->EndInit();
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
			case Cmd_UserMsg_Query_Send_Rsp:
			{
				if(head.userdata1 == CF_ERROR_SUCCESS) {
					UserMsgStru* pMsg = NULL;
					int nItemCount = head.len/sizeof(UserMsgStru);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pMsg = (UserMsgStru*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(UserMsgStru));

						save->queueLastSendMsg.push(*pMsg);
					}
					save->UnlockObject();

					if(save->queueLastSendMsg.size() == 0)
						return;
					
					ShowQueue_Send(save->queueLastSendMsg);
				}
				else {
					_GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
							CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, p+sizeof(Stru_UniPkgHead));
				}
			}
				break;
			case Cmd_UserMsg_Query_Recv_Rsp:
			{
				if(head.userdata1 == CF_ERROR_SUCCESS) {
					UserMsgStru* pMsg = NULL;
					int nItemCount = head.len/sizeof(UserMsgStru);

					save->LockObject();
					for(int i=0; i<nItemCount; i++) {
						pMsg = (UserMsgStru*)
								(p+sizeof(Stru_UniPkgHead)+i*sizeof(UserMsgStru));

						save->queueLastRecvMsg.push(*pMsg);
					}
					save->UnlockObject();

					if(save->queueLastRecvMsg.size() == 0)
						return;
					
					ShowQueue_Recv(save->queueLastRecvMsg);
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

	int GetRowIndexByRowID_Send(int nRowID) {
		for(int i=0; i<dataGridSend->Rows->Count; i++) {
			if(int::Parse(dataGridSend->Rows[i]->Tag->ToString()) == nRowID)
				return dataGridSend->Rows[i]->Index;
		}
		return -1;
	}

	int GetColIndexByColID_Send(int nColID) {
		for(int i=0; i<dataGridSend->Columns->Count; i++) {
			if(int::Parse(dataGridSend->Columns[i]->Tag->ToString()) == nColID)
				return dataGridSend->Columns[i]->Index;
		}
		return -1;
	}

	int GetRowIndexByRowID_Recv(int nRowID) {
		for(int i=0; i<dataGridRecv->Rows->Count; i++) {
			if(int::Parse(dataGridRecv->Rows[i]->Tag->ToString()) == nRowID)
				return dataGridRecv->Rows[i]->Index;
		}
		return -1;
	}

	int GetColIndexByColID_Recv(int nColID) {
		for(int i=0; i<dataGridRecv->Columns->Count; i++) {
			if(int::Parse(dataGridRecv->Columns[i]->Tag->ToString()) == nColID)
				return dataGridRecv->Columns[i]->Index;
		}
		return -1;
	}

	void InitGridHead_Send() {

		dataGridSend->Columns->Clear();

		int nColID = 0;
		System::Windows::Forms::DataGridViewTextBoxColumn^ gridColumn;

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_USM_MSGID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"消息ID";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_USM_MSGID));
		dataGridSend->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_USM_Title;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"标题";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_USM_Title));
		dataGridSend->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_USM_MSG;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"内容";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_USM_MSG));
		dataGridSend->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_USM_CreateTime;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"时间";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_USM_CreateTime));
		dataGridSend->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_USM_HasRecvUser;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"已接收人";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_HQA_Within));
		dataGridSend->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_USM_NotRecvUser;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"未接收人";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_USM_NotRecvUser));
		dataGridSend->Columns->Add(gridColumn);

	}

	void InitGridHead_Recv() {

		dataGridRecv->Columns->Clear();

		int nColID = 0;
		System::Windows::Forms::DataGridViewTextBoxColumn^ gridColumn;

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_URM_TARGETMSGID;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"消息ID";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_URM_TARGETMSGID));
		dataGridRecv->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_URM_Title;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"标题";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_URM_Title));
		dataGridRecv->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_URM_MSG;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"内容";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_URM_MSG));
		dataGridRecv->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_URM_RecvTime;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"时间";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_URM_RecvTime));
		dataGridRecv->Columns->Add(gridColumn);

		gridColumn = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
		nColID = conCol_URM_SendUser;
		gridColumn->Tag = nColID;
		gridColumn->HeaderText = L"发送人";
		gridColumn->Name = Tools::string2String(MACRO2STRING(conCol_URM_SendUser));
		dataGridRecv->Columns->Add(gridColumn);

	}

	void UpdateRow_Send(int nIndex, UserMsgStru& msg, string& strHasRecv, string& strNotRecv) {

		if(nIndex<0)
			return;
		int nCol = -1;
		std::string strText;
		DataGridViewRow^ dr = dataGridSend->Rows[nIndex];
		char strTemp[1024];

		if((nCol = GetColIndexByColID_Send(conCol_USM_MSGID))>=0) {
			sprintf(strTemp, "%d", msg.MsgID);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID_Send(conCol_USM_Title))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(msg.Title);
		}
		if((nCol = GetColIndexByColID_Send(conCol_USM_MSG))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(msg.Msg);
		}
		if((nCol = GetColIndexByColID_Send(conCol_USM_CreateTime))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(msg.CreateDateTime);
		}
		if((nCol = GetColIndexByColID_Send(conCol_USM_HasRecvUser))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(strHasRecv.c_str());
		}
		if((nCol = GetColIndexByColID_Send(conCol_USM_NotRecvUser))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(strNotRecv.c_str());
		}
	}

	void UpdateRow_Recv(int nIndex, UserMsgStru& msg, const string& strMe) {

		if(nIndex<0)
			return;
		int nCol = -1;
		std::string strText;
		DataGridViewRow^ dr = dataGridRecv->Rows[nIndex];
		char strTemp[1024];

		if((nCol = GetColIndexByColID_Recv(conCol_URM_TARGETMSGID))>=0) {
			sprintf(strTemp, "%d", msg.TargetMsgID);
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
		if((nCol = GetColIndexByColID_Recv(conCol_URM_Title))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(msg.Title);
		}
		if((nCol = GetColIndexByColID_Recv(conCol_URM_MSG))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(msg.Msg);
		}
		if((nCol = GetColIndexByColID_Recv(conCol_URM_RecvTime))>=0) {
			dr->Cells[nCol]->Value = Tools::string2String(msg.SendDateTime);
		}
		if((nCol = GetColIndexByColID_Recv(conCol_URM_SendUser))>=0) {
			sprintf(strTemp, "%s", strMe.c_str());
			dr->Cells[nCol]->Value = Tools::string2String(strTemp);
		}
	}

	void ShowQueue_Send(std::queue<UserMsgStru>& queueRet) {

		std::map<int, int>::iterator it;
		std::map<int, string>::iterator itUsrs;
		int nIndex = -1;
		int nKey = -1;
		string strHasRecv, strNotRecv;
		CRiskData* pDataInfo = CRiskData::GetInstance();
		string strUserName;
		char strTemp[1024];

		save->LockObject();
		
		while(!queueRet.empty()) {
			UserMsgStru field = queueRet.front();
			queueRet.pop();

			strHasRecv = "";
			strNotRecv = "";

			nKey = field.MsgID;
			memset(strTemp, 0, sizeof(strTemp));
			if(pDataInfo!=NULL) {
				if(pDataInfo->GetUserNameByID(field.TargetUserID, strUserName))
					sprintf_s(strTemp, sizeof(strTemp)-1, "%s, ", strUserName.c_str());
			}
			if(field.SendDateTime[1]!=0) {
				itUsrs = save->mapSendMsg_HasRecv.find(nKey);
				if(itUsrs != save->mapSendMsg_HasRecv.end()) 
					itUsrs->second += strTemp;
				else 
					save->mapSendMsg_HasRecv.insert(make_pair(nKey, strTemp));
			}
			else {
				itUsrs = save->mapSendMsg_NotRecv.find(nKey);
				if(itUsrs != save->mapSendMsg_NotRecv.end()) 
					itUsrs->second += strTemp;
				else 
					save->mapSendMsg_NotRecv.insert(make_pair(nKey, strTemp));
			}
			itUsrs = save->mapSendMsg_HasRecv.find(nKey);
			if(itUsrs != save->mapSendMsg_HasRecv.end())
				strHasRecv = itUsrs->second;
			itUsrs = save->mapSendMsg_NotRecv.find(nKey);
			if(itUsrs != save->mapSendMsg_NotRecv.end()) 
				strNotRecv = itUsrs->second;

			it = save->mapKey2Index_Send.find(nKey);
			if(it != save->mapKey2Index_Send.end()) {
				nIndex = GetRowIndexByRowID_Send(it->second);
			}
			else {
				DataGridViewRow^ newRow = gcnew DataGridViewRow;
				int nRowID = save->GetNextRowID();
				newRow->Tag = nRowID;
				nIndex = dataGridSend->Rows->Add(newRow);
				if(nIndex>=0) {
					save->mapKey2Index_Send.insert(std::make_pair(nKey, nRowID));
					save->mapIndex2Key_Send.insert(std::make_pair(nRowID, nKey));
				}
			}
			if(nIndex>=0) {
				UpdateRow_Send(nIndex, field, strHasRecv, strNotRecv);
			}
		}
		save->UnlockObject();
	}

	void ShowQueue_Recv(std::queue<UserMsgStru>& queueRet) {

		std::map<int, int>::iterator it;
		std::map<int, string>::iterator itUsrs;
		int nIndex = -1;
		int nKey = -1;
		string strMe;
		CRiskData* pDataInfo = CRiskData::GetInstance();
		string strUserName;
		char strTemp[1024];

		save->LockObject();
		
		while(!queueRet.empty()) {
			UserMsgStru field = queueRet.front();
			queueRet.pop();

			nKey = field.MsgID;

			it = save->mapKey2Index_Recv.find(nKey);
			if(it != save->mapKey2Index_Recv.end()) {
				nIndex = GetRowIndexByRowID_Recv(it->second);
			}
			else {
				DataGridViewRow^ newRow = gcnew DataGridViewRow;
				int nRowID = save->GetNextRowID();
				newRow->Tag = nRowID;
				nIndex = dataGridRecv->Rows->Add(newRow);
				if(nIndex>=0) {
					save->mapKey2Index_Recv.insert(std::make_pair(nKey, nRowID));
					save->mapIndex2Key_Recv.insert(std::make_pair(nRowID, nKey));
				}
			}
			if(nIndex>=0) {
				memset(strTemp, 0, sizeof(strTemp));
				if(pDataInfo!=NULL) {
					if(pDataInfo->GetUserNameByID(field.TargetUserID, strUserName)) {
						sprintf_s(strTemp, sizeof(strTemp)-1, "%s, ", strUserName.c_str());
						strMe = strTemp;
					}
				}
				UpdateRow_Recv(nIndex, field, strMe);
			}
		}
		save->UnlockObject();
	}

	private: System::Void CFormUserMsgQuery_Load(System::Object^  sender, System::EventArgs^  e) {
				 InitGridHead_Send();
				 InitGridHead_Recv();

				 IntPtr hWnd=this->Handle;
				 CTcpLayer::SubscribePkg(Cmd_UserMsg_Query_Send_Rsp, (int)hWnd);
				 CTcpLayer::SubscribePkg(Cmd_UserMsg_Query_Recv_Rsp, (int)hWnd);

			 }
	private: System::Void CFormUserMsgQuery_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
				 //if(!m_bHasExit) {
					// Hide();
					// e->Cancel = true;
				 //}
				 save->ResetQuery_Send();
				 save->ResetQuery_Recv();
				 dataGridSend->Rows->Clear();
				 dataGridRecv->Rows->Clear();
			 }
	private: System::Void CFormUserMsgQuery_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
				 IntPtr hWnd=this->Handle;
				 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
			 }
	private: System::Void buttonQuerySend_Click(System::Object^  sender, System::EventArgs^  e) {

				 if(!m_bHasChanged_QueryParam)
					 return;

				 bool bHasData = false;
				 dataGridSend->Rows->Clear();

				 save->ResetQuery_Send();

				 std::string strDate;

				 strDate = Tools::String2string(dtpStart->Value.Date.ToString("yyyyMMdd"));

				 dwTcpStartTime = GetTickCount();
				 dwTcpEndTime = 0;
				 dwShowStartTime = 0;
				 dwShowEndTime = 0;

				 sLoginRsp logInfo;

				 memset(&logInfo, 0, sizeof(logInfo));
				 CRiskData::GetInstance()->GetLogonInfo(logInfo);
				 save->NewQuery(logInfo.mnUserID, strDate, true);
			 }
	private: System::Void buttonClearSend_Click(System::Object^  sender, System::EventArgs^  e) {
				 //m_bHasChanged_QueryParam = true;
				 //if(!m_bHasChanged_QueryParam)
					// return;

				 dataGridSend->Rows->Clear();
				 save->ResetQuery_Send();
			 }
	private: System::Void buttonQueryRecv_Click(System::Object^  sender, System::EventArgs^  e) {

				 if(!m_bHasChanged_QueryParam)
					 return;

				 bool bHasData = false;
				 dataGridRecv->Rows->Clear();

				 save->ResetQuery_Recv();

				 std::string strDate;

				 strDate = Tools::String2string(dtpStart->Value.Date.ToString("yyyyMMdd"));

				 dwTcpStartTime = GetTickCount();
				 dwTcpEndTime = 0;
				 dwShowStartTime = 0;
				 dwShowEndTime = 0;

				 sLoginRsp logInfo;

				 memset(&logInfo, 0, sizeof(logInfo));
				 CRiskData::GetInstance()->GetLogonInfo(logInfo);
				 save->NewQuery(logInfo.mnUserID, strDate, false);
			 }
	private: System::Void buttonClearRecv_Click(System::Object^  sender, System::EventArgs^  e) {
				 //m_bHasChanged_QueryParam = true;
				 //if(!m_bHasChanged_QueryParam)
					// return;

				 dataGridRecv->Rows->Clear();
				 save->ResetQuery_Recv();
			 }
};
}

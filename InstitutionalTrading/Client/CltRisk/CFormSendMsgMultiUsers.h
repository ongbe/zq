#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include <string>
#include <vector>
#include <set>
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
	/// Summary for CFormSendMsgMultiUsers
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CFormSendMsgMultiUsers : public System::Windows::Forms::Form
	{
	public:
		CFormSendMsgMultiUsers(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			nUserID = 0;
			m_nHandCheckIndex = -1;
		}

		void ResetUser(int nUserID) {
			this->nUserID = nUserID;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CFormSendMsgMultiUsers()
		{
			if (components)
			{
				delete components;
			}
		}
	private: int nUserID;
	private: int m_nHandCheckIndex;

	private: System::Windows::Forms::Label^  label1;
	protected: 
	private: System::Windows::Forms::ComboBox^  comboBoxUserType;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::TreeView^  treeOrganAndAccount;
	private: System::Windows::Forms::DateTimePicker^  dtpExpiredDate;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBoxTitle;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::TextBox^  textBoxMsg;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Button^  buttonSend;

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
			this->comboBoxUserType = (gcnew System::Windows::Forms::ComboBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->treeOrganAndAccount = (gcnew System::Windows::Forms::TreeView());
			this->dtpExpiredDate = (gcnew System::Windows::Forms::DateTimePicker());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBoxTitle = (gcnew System::Windows::Forms::TextBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->textBoxMsg = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->buttonSend = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"用户类型";
			// 
			// comboBoxUserType
			// 
			this->comboBoxUserType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBoxUserType->FormattingEnabled = true;
			this->comboBoxUserType->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"管理员", L"风控员", L"交易员"});
			this->comboBoxUserType->Location = System::Drawing::Point(71, 6);
			this->comboBoxUserType->Name = L"comboBoxUserType";
			this->comboBoxUserType->Size = System::Drawing::Size(120, 20);
			this->comboBoxUserType->TabIndex = 1;
			this->comboBoxUserType->SelectedIndexChanged += gcnew System::EventHandler(this, &CFormSendMsgMultiUsers::comboBoxUserType_SelectedIndexChanged);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->treeOrganAndAccount);
			this->groupBox1->Location = System::Drawing::Point(12, 32);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(179, 312);
			this->groupBox1->TabIndex = 2;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"用户";
			// 
			// treeOrganAndAccount
			// 
			this->treeOrganAndAccount->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->treeOrganAndAccount->CheckBoxes = true;
			this->treeOrganAndAccount->FullRowSelect = true;
			this->treeOrganAndAccount->HideSelection = false;
			this->treeOrganAndAccount->Location = System::Drawing::Point(6, 20);
			this->treeOrganAndAccount->Name = L"treeOrganAndAccount";
			this->treeOrganAndAccount->Size = System::Drawing::Size(167, 286);
			this->treeOrganAndAccount->TabIndex = 1;
			this->treeOrganAndAccount->AfterCheck += gcnew System::Windows::Forms::TreeViewEventHandler(this, &CFormSendMsgMultiUsers::treeOrganAndAccount_AfterCheck);
			// 
			// dtpExpiredDate
			// 
			this->dtpExpiredDate->CustomFormat = L"yyyy-MM-dd";
			this->dtpExpiredDate->Location = System::Drawing::Point(264, 6);
			this->dtpExpiredDate->Name = L"dtpExpiredDate";
			this->dtpExpiredDate->Size = System::Drawing::Size(110, 21);
			this->dtpExpiredDate->TabIndex = 20;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(203, 10);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 19;
			this->label2->Text = L"有效日期";
			// 
			// textBoxTitle
			// 
			this->textBoxTitle->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxTitle->Location = System::Drawing::Point(264, 33);
			this->textBoxTitle->MaxLength = 15;
			this->textBoxTitle->Name = L"textBoxTitle";
			this->textBoxTitle->Size = System::Drawing::Size(195, 21);
			this->textBoxTitle->TabIndex = 21;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->textBoxMsg);
			this->groupBox2->Location = System::Drawing::Point(197, 62);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(274, 253);
			this->groupBox2->TabIndex = 22;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"内容";
			// 
			// textBoxMsg
			// 
			this->textBoxMsg->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxMsg->Location = System::Drawing::Point(2, 16);
			this->textBoxMsg->MaxLength = 250;
			this->textBoxMsg->Multiline = true;
			this->textBoxMsg->Name = L"textBoxMsg";
			this->textBoxMsg->Size = System::Drawing::Size(269, 231);
			this->textBoxMsg->TabIndex = 2;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(203, 38);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(41, 12);
			this->label3->TabIndex = 24;
			this->label3->Text = L"标题：";
			// 
			// buttonSend
			// 
			this->buttonSend->Location = System::Drawing::Point(413, 321);
			this->buttonSend->Name = L"buttonSend";
			this->buttonSend->Size = System::Drawing::Size(55, 23);
			this->buttonSend->TabIndex = 23;
			this->buttonSend->Text = L"发送";
			this->buttonSend->UseVisualStyleBackColor = true;
			this->buttonSend->Click += gcnew System::EventHandler(this, &CFormSendMsgMultiUsers::buttonSend_Click);
			// 
			// CFormSendMsgMultiUsers
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(483, 356);
			this->Controls->Add(this->textBoxTitle);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->buttonSend);
			this->Controls->Add(this->dtpExpiredDate);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->comboBoxUserType);
			this->Controls->Add(this->label1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"CFormSendMsgMultiUsers";
			this->Text = L"用户消息发送";
			this->Load += gcnew System::EventHandler(this, &CFormSendMsgMultiUsers::CFormSendMsgMultiUsers_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &CFormSendMsgMultiUsers::CFormSendMsgMultiUsers_FormClosed);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &CFormSendMsgMultiUsers::CFormSendMsgMultiUsers_FormClosing);
			this->groupBox1->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
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

	private: void LoadOrganAccount(TreeNode^ node, int nOrganID, int nUserType) 
	{

		vector<UserInfo> vecUserInfo;
		vector<UserInfo> ::iterator it;
		CRiskData::GetInstance()->GetUserInfosByOrgID(nOrganID,(UserType)nUserType,vecUserInfo,false);
		if(!vecUserInfo.empty())
		{
			char strTemp[1024];
			for(it = vecUserInfo.begin(); it != vecUserInfo.end(); it++) 
			{
				//if(nUserType!=-1 && it->nUserType!=nUserType)
				//	continue;
				memset(strTemp, 0, sizeof(strTemp));
				sprintf(strTemp, "%d", (*it).nUserID);
				String^ strText = Tools::string2String(strTemp);
				memset(strTemp, 0, sizeof(strTemp));
				sprintf(strTemp, "%s", (*it).szAccount);
				String^ strValue = Tools::string2String(strTemp);
				node->Nodes->Add(strText, strValue);
			}
		}

	}

	private: void LoadSubOrganAndAccount(TreeNode^ nodeMain, int nID, 
			map<int, map<int, Organization>>& mapMain, 
			map<int, Organization>& mapSub, int nUserType, set<int>& setOrgID) 
	{

		map<int, map<int, Organization>>::iterator it;
		map<int, Organization>::iterator itSub;

		TreeNode^ subNode;
		char strTemp[1024];
		memset(strTemp, 0, sizeof(strTemp));

		itSub = mapSub.begin();
		for(; itSub != mapSub.end(); itSub++) 
		{
			sprintf(strTemp, "0%d", itSub->first);
			String^ strText = Tools::string2String(strTemp);
			String^ strValue = Tools::string2String(itSub->second.szName);
			subNode = nodeMain->Nodes->Add(strText, strValue);

			it = mapMain.find(itSub->first);
			if(it != mapMain.end())
			LoadSubOrganAndAccount(subNode, itSub->first, mapMain, it->second, nUserType, setOrgID);
		}
		if(setOrgID.find(nID)!=setOrgID.end()) {
			LoadOrganAccount(nodeMain, nID, nUserType);
		}

	}

	private: void LoadOrganAndAccount(int nUserType) 
	{

		int nHwnd = this->Handle.ToInt32();

		switch(nUserType) {
		case USER_TYPE_MANAGE:			//管理员
		case USER_TYPE_RISK_CONTROL:	//风控员
		{
			map<int, UserInfo> mapUserInfo;
			char strTemp[1024];

			CRiskData::GetInstance()->GetAllUserInfo(mapUserInfo);
			treeOrganAndAccount->Nodes->Clear();
			for(map<int, UserInfo>::iterator it = mapUserInfo.begin(); 
					it != mapUserInfo.end(); it++) {
				if(it->second.nUserType != nUserType)
					continue;
				memset(strTemp, 0, sizeof(strTemp));
				sprintf(strTemp, "%d", it->second.nUserID);
				String^ strText = Tools::string2String(strTemp);
				String^ strValue = Tools::string2String(it->second.szAccount);
				treeOrganAndAccount->Nodes->Add(strText, strValue);
			}
		}
			break;
		case USER_TYPE_TRADER:			//交易员
		{
			sLoginRsp Login;
			CRiskData::GetInstance()->GetLogonInfo(Login);

			int nOrgID = CRiskData::GetInstance()->GetOrgIDByUserID(Login.mnUserID);
			if(nOrgID<0)
				return;

			//CRiskData::GetInstance()->GetOrgIDByUserID(m_pLogin->mnUserID,USER_ORG_RELATION_ATTACH,vecOrgID);
			//CRiskData::GetInstance()->GetOrgIDByUserID(Login.mnUserID,USER_ORG_RELATION_ALL,vecOrgID);
			vector<int> vecOrgID;
			set<int> setOrgID;
			CRiskData::GetInstance()->GetOrgIDByUserID(Login.mnUserID,USER_ORG_RELATION_MONITOR,vecOrgID);
			for(int i=0; i<(int)vecOrgID.size(); i++) {
				setOrgID.insert(vecOrgID[i]);
			}

			Organization assertOrg;
			memset(&assertOrg, 0, sizeof(assertOrg));
			if(!CRiskData::GetInstance()->GetOrganizationByID(nOrgID, assertOrg))
				return;

			TreeNode^ node;

			char strTemp[1024];
			memset(strTemp, 0, sizeof(strTemp));
			sprintf(strTemp, "0%d", assertOrg.nAssetMgmtOrgID);
			String^ strText = Tools::string2String(strTemp);
			String^ strValue = Tools::string2String(assertOrg.szName);
			treeOrganAndAccount->Nodes->Clear();
			node = treeOrganAndAccount->Nodes->Add(strText, strValue);

			map<int, map<int, Organization>> mapAssetMgmtOrgTree;
			CRiskData::GetInstance()->GetOrganization(mapAssetMgmtOrgTree);

			map<int, map<int, Organization>>::iterator it = mapAssetMgmtOrgTree.find(nOrgID);
			if(it == mapAssetMgmtOrgTree.end())
				return;

			map<int, Organization>& mapSub = it->second;
			if(mapSub.begin()==mapSub.end())
				return;

			LoadSubOrganAndAccount(node, assertOrg.nAssetMgmtOrgID, 
					mapAssetMgmtOrgTree, mapSub, nUserType, setOrgID);

			node->Expand();
		}
			break;
		};
	}

	bool GetSelectID(set<int>& setRetAccntID) {
		setRetAccntID.clear();
		TreeNode^ node;
		bool bHasCheckSub = false;
		for(int i=0; i<treeOrganAndAccount->Nodes->Count; i++) {
			node = treeOrganAndAccount->Nodes[i];

			
			GetNodeID(node, setRetAccntID);
		}
		return setRetAccntID.size()>0;
	}

	void GetNodeID(TreeNode^ node, std::set<int>& setRetAccntID) {

		std::set<int> setAccountID;

		std::string strKey = Tools::String2string(node->Name);
		std::string strID;
		BOOL bIsOrgan = false;
		if(strKey[0]== '0')
			bIsOrgan =true;

		int nID = 0;

		strID = strKey;
		//strID[0] = ' ';
		nID = atoi(strID.c_str());

		if(bIsOrgan) 
		{
			for(int i=0; i<node->Nodes->Count; i++) 
			{
				GetNodeID(node->Nodes[i], setRetAccntID);
			}
		}
		else 
		{
			if(node->Checked)
				setRetAccntID.insert(nID);
		}
	}

	void AutoCheckSubNode(TreeNode^ node, bool bCheck) {
		for(int i=0; i<node->Nodes->Count; i++) {
			node->Nodes[i]->Checked = bCheck;
			AutoCheckSubNode(node->Nodes[i], bCheck);
		}
	}
			 
	bool HasTreeChecked(TreeNode^ node, bool& bNoChecked) {
		std::string strKey = Tools::String2string(node->Name);
		BOOL bIsOrgan = (strKey[0]=='O');
		if(!bIsOrgan && node->Checked) {
			bNoChecked = false;
		}
		for(int i=0; i<node->Nodes->Count; i++) {
			HasTreeChecked(node->Nodes[i], bNoChecked);
		}
		return bNoChecked;
	}

	void GetTreeItemText(TreeNode^ node, std::string& strText, bool bNoChecked) {
		std::string strKey = Tools::String2string(node->Name);
		BOOL bIsOrgan = (strKey[0]=='O');
		if(!bIsOrgan) {
			//if(bNoChecked || (!bNoChecked && node->Checked)) {
			if(node->Checked) {
				String^ strKey = node->Text;
				strText += Tools::String2string(strKey);
				strText += ", ";
			}
		}
		for(int i=0; i<node->Nodes->Count; i++) {
			GetTreeItemText(node->Nodes[i], strText, bNoChecked);
		}
	}
	
	void UncheckTreeNode(TreeNode^ node) {
		node->Checked = false;
		for(int i=0; i<node->Nodes->Count; i++) {
			UncheckTreeNode(node->Nodes[i]);
		}
	}

	bool SendMsg(int nUserID, set<int> setAccntID, int nExpiredDate, string& strTitle, string& strMsg)
	{
		if(setAccntID.size()==0)
			return false;

		int nLen = sizeof(UserSendMsgStru)+(setAccntID.size()-1)*sizeof(int);
		char* pData = new char[nLen];
		UserSendMsgStru& tMsg = *(UserSendMsgStru*)pData;
		int i = 0;

		memset(pData, 0, nLen);
		tMsg.OwnerUserID = nUserID;
		tMsg.ExpiredDate = nExpiredDate;
		tMsg.TargetUserCount = setAccntID.size();
		strncpy(tMsg.Title, strTitle.c_str(), sizeof(tMsg.Title)-1);
		strncpy(tMsg.Msg, strMsg.c_str(), sizeof(tMsg.Msg)-1);
		for(set<int>::iterator it = setAccntID.begin(); it != setAccntID.end(); it++) {
			tMsg.TargetUsers[i] = *it;
			i++;
		}
		CTcpLayer::SendData(Cmd_UserMsg_Send_Req, (void*)pData, nLen, 0, nUserID, 
				0, 0, nUserID);
		return true;
	}

	private: System::Void CFormSendMsgMultiUsers_Load(System::Object^  sender, System::EventArgs^  e) {
				 IntPtr hWnd=this->Handle;
				 CTcpLayer::SubscribePkg(Cmd_UserMsg_Send_Rsp, (int)hWnd);
			 }
	private: System::Void CFormSendMsgMultiUsers_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
				 IntPtr hWnd=this->Handle;
				 CTcpLayer::UnsubscribeAllPkg((int)hWnd);
			 }
	private: System::Void CFormSendMsgMultiUsers_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			 }
	private: System::Void buttonSend_Click(System::Object^  sender, System::EventArgs^  e) {
				 set<int> setAccntID;
				 int nExpiredDate = atoi(Tools::String2string(dtpExpiredDate->Value.Date.ToString("yyyyMMdd")).c_str());
					//GetLocalDate();
				 GetSelectID(setAccntID);
				 SendMsg(nUserID, setAccntID, nExpiredDate, 
						string(Tools::String2string(textBoxTitle->Text)), 
						string(Tools::String2string(textBoxMsg->Text)));
			 }
	private: System::Void comboBoxUserType_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
				 LoadOrganAndAccount(comboBoxUserType->SelectedIndex);
			 }
	private: System::Void treeOrganAndAccount_AfterCheck(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
				 if(m_nHandCheckIndex==-1) {
					 m_nHandCheckIndex = e->Node->Index;
				 }
				 else {
					 return;
				 }
				 AutoCheckSubNode(e->Node, e->Node->Checked);
				 m_nHandCheckIndex = -1;
			 }
};
}

#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Tools.h"
#include "RiskData.h"
#include "CommonStruct.h"

namespace CltRisk {

	/// <summary>
	/// Summary for COrganAndAccountIDTreeListDlg
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class COrganAndAccountIDTreeListDlg : public AnywndComboBox::CAnywndComboBoxControlContainter
	{
	public:
		COrganAndAccountIDTreeListDlg(void)
		{
			m_nHandCheckIndex = -1;
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			LoadOrganAndAccount();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~COrganAndAccountIDTreeListDlg()
		{
			if (components)
			{
				delete components;
			}
		}
	private: int m_nHandCheckIndex;
	private: System::Windows::Forms::ComboBox^  comboRelate;

	private: System::Windows::Forms::TreeView^  treeOrganAndAccount;


	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

	public:
		void ResetRelateCombo(System::Windows::Forms::ComboBox^ comboRelate) {
			this->comboRelate = comboRelate;
		}
		void Clear() {
			comboRelate->Text = Tools::string2String("");
			if(treeOrganAndAccount->Nodes->Count>0)
				UncheckTreeNode(treeOrganAndAccount->Nodes[0]);
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

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->treeOrganAndAccount = (gcnew System::Windows::Forms::TreeView());
			this->SuspendLayout();
			// 
			// treeOrganAndAccount
			// 
			this->treeOrganAndAccount->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->treeOrganAndAccount->CheckBoxes = true;
			this->treeOrganAndAccount->FullRowSelect = true;
			this->treeOrganAndAccount->HideSelection = false;
			this->treeOrganAndAccount->Location = System::Drawing::Point(0, 0);
			this->treeOrganAndAccount->Name = L"treeOrganAndAccount";
			this->treeOrganAndAccount->Size = System::Drawing::Size(160, 238);
			this->treeOrganAndAccount->TabIndex = 0;
			this->treeOrganAndAccount->AfterCheck += gcnew System::Windows::Forms::TreeViewEventHandler(this, &COrganAndAccountIDTreeListDlg::treeOrganAndAccount_AfterCheck);
			// 
			// COrganAndAccountIDTreeListDlg
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->treeOrganAndAccount);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"COrganAndAccountIDTreeListDlg";
			this->Size = System::Drawing::Size(160, 238);
			this->Load += gcnew System::EventHandler(this, &COrganAndAccountIDTreeListDlg::COrganAndAccountIDTreeListDlg_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: void LoadOrganAccount(TreeNode^ node, int nOrganID) 
			 {

				 vector<UserInfo> vecUserInfo;
				 vector<UserInfo> ::iterator it;
				 CRiskData::GetInstance()->GetUserInfosByOrgID(nOrganID,USER_TYPE_TRADER,vecUserInfo,false);
				 if(!vecUserInfo.empty())
				 {
					 char strTemp[1024];
					 for(it = vecUserInfo.begin(); it != vecUserInfo.end(); it++) 
					 {
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

	private: void LoadSubOrganAndAccount(TreeNode^ nodeMain, 
				 map<int, map<int, Organization>>& mapMain, 
				 map<int, Organization>& mapSub) 
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

					 if(CRiskData::GetInstance()->IsMonitorOrg(itSub->first))
					 {
						 vector<UserInfo> vecUserInfo;
						 CRiskData::GetInstance()->GetUserInfosByOrgID(itSub->first,USER_TYPE_TRADER,vecUserInfo,false);
						 if(!vecUserInfo.empty())
						 {
							 TreeNode^ traderNode;
							 vector<UserInfo> ::iterator itUserInfo;
							 char strTemp[1024];
							 for(itUserInfo = vecUserInfo.begin(); itUserInfo != vecUserInfo.end(); itUserInfo++) 
							 {
								 memset(strTemp, 0, sizeof(strTemp));
								 sprintf(strTemp, "%d", (*itUserInfo).nUserID);
								 String^ strText = Tools::string2String(strTemp);
								 memset(strTemp, 0, sizeof(strTemp));
								 sprintf(strTemp, "%s", (*itUserInfo).szAccount);
								 String^ strValue = Tools::string2String(strTemp);
								 traderNode = subNode->Nodes->Add(strText, strValue);
								 //vecUserID.push_back(it->nUserID);
							 }
						 }						 
					 }
					 /*LoadOrganAccount(nodeMain, nID);*/
					 it = mapMain.find(itSub->first);
					 if(it != mapMain.end())
						 LoadSubOrganAndAccount(subNode,  mapMain,it->second);

				 }
			}

	private: void LoadOrganAndAccount() 
			 {

				 int nHwnd = this->Handle.ToInt32();

				 sLoginRsp Login;
				 CRiskData::GetInstance()->GetLogonInfo(Login);

				 vector<int> vecOrgID;
				 //vector<int> vecUserID;
				 int nOrgID;
				 CRiskData::GetInstance()->GetOrgIDByUserID(Login.mnUserID,USER_ORG_RELATION_ATTACH,vecOrgID);    
				 if(!vecOrgID.empty())
				 { 
					 nOrgID =vecOrgID[0];
				 }

				 /*int nOrgID = CRiskData::GetInstance()->GetOrgIDByUserID(Login.mnUserID);
				 if(nOrgID<0)
					 return;*/

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

				 if(CRiskData::GetInstance()->IsMonitorOrg(assertOrg.nAssetMgmtOrgID))
				 {
					 vector<UserInfo> vecUserInfo;
					 vector<UserInfo> ::iterator it;
					 CRiskData::GetInstance()->GetUserInfosByOrgID(assertOrg.nAssetMgmtOrgID,USER_TYPE_TRADER,vecUserInfo,false);
					 if(!vecUserInfo.empty())
					 {
						 char strTemp[1024];
						 for(it = vecUserInfo.begin(); it != vecUserInfo.end(); it++) 
						 {
							 memset(strTemp, 0, sizeof(strTemp));
							 sprintf(strTemp, "%d", (*it).nUserID);
							 String^ strText = Tools::string2String(strTemp);
							 memset(strTemp, 0, sizeof(strTemp));
							 sprintf(strTemp, "%s", (*it).szAccount);
							 String^ strValue = Tools::string2String(strTemp);
							 node->Nodes->Add(strText, strValue);
							 //vecUserID.push_back(it->nUserID);
						 }
					 }

				 }

				 map<int, map<int, Organization>> mapAssetMgmtOrgTree;
				 CRiskData::GetInstance()->GetOrganization(mapAssetMgmtOrgTree);

				 map<int, map<int, Organization>>::iterator it = mapAssetMgmtOrgTree.find(nOrgID);
				 if(it == mapAssetMgmtOrgTree.end())
					 return;

				 map<int, Organization>& mapSub = it->second;
				 if(mapSub.begin()==mapSub.end())
					 return;

				 LoadSubOrganAndAccount(node, 
					 mapAssetMgmtOrgTree, mapSub);

				 node->Expand();

			//	 vector<int> vecOrgID;
			//	 CRiskData::GetInstance()->GetOrgIDByUserID(Login.mnUserID,USER_ORG_RELATION_MONITOR,vecOrgID);
			//	 
			//	 treeOrganAndAccount->Nodes->Clear();

			//	 for ( int i= 0; i<vecOrgID.size(); i++)
			//	 {
			//		 Organization assertOrg;
			//		 memset(&assertOrg, 0, sizeof(assertOrg));
			//		 if(!CRiskData::GetInstance()->GetOrganizationByID(vecOrgID[i], assertOrg))
			//			 return;

			//		 TreeNode^ node;

			//		 char strTemp[1024];
			//		 memset(strTemp, 0, sizeof(strTemp));
			//		 sprintf(strTemp, "0%d", assertOrg.nAssetMgmtOrgID);
			//		 String^ strText = Tools::string2String(strTemp);
			//		 String^ strValue = Tools::string2String(assertOrg.szName);
			//		 
			//		 node = treeOrganAndAccount->Nodes->Add(strText, strValue);
			//		 
			//		 
			//		 vector<UserInfo> vecUserInfo;
			//		 vector<UserInfo> ::iterator it;
			//		 CRiskData::GetInstance()->GetUserInfosByOrgID(vecOrgID[i],USER_TYPE_TRADER,vecUserInfo,false);
			//		 if(!vecUserInfo.empty())
			//		 {
			//			 char strTemp[1024];
			//			 for(it = vecUserInfo.begin(); it != vecUserInfo.end(); it++) 
			//			 {
			//				 memset(strTemp, 0, sizeof(strTemp));
			//				 sprintf(strTemp, "%d", (*it).nUserID);
			//				 String^ strText = Tools::string2String(strTemp);
			//				 memset(strTemp, 0, sizeof(strTemp));
			//				 sprintf(strTemp, "%s", (*it).szAccount);
			//				 String^ strValue = Tools::string2String(strTemp);
			//				 node->Nodes->Add(strText, strValue);
			//			 }
			//		 }
			//		 					 
			//		 //node->Expand();
			//	 }
			//	 

			//	//map<int, map<int, Organization>> mapAssetMgmtOrgTree;
			//	//CRiskData::GetInstance()->GetOrganization(mapAssetMgmtOrgTree);

			//	//map<int, map<int, Organization>>::iterator it = mapAssetMgmtOrgTree.find(nOrgID);
			//	//if(it == mapAssetMgmtOrgTree.end())
			//	//	return;

			///*	map<int, Organization>& mapSub = it->second;
			//	if(mapSub.begin()==mapSub.end())
			//		return;

			//	LoadSubOrganAndAccount(node, assertOrg.nAssetMgmtOrgID, 
			//			mapAssetMgmtOrgTree, mapSub);*/

				
			 }

	private: System::Void COrganAndAccountIDTreeListDlg_Load(System::Object^  sender, System::EventArgs^  e) {
				 //Rectangle rect = this->DisplayRectangle;
				 treeOrganAndAccount->Location = Point(0, 0);
				 //treeOrganAndAccount->Width = rect.Width;
				 //treeOrganAndAccount->Height = rect.Height;
			 }

	private: void GetNodeID(TreeNode^ node, std::set<int>& setRetAccntID) {

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
	private: void UncheckTreeNode(TreeNode^ node) {
				 node->Checked = false;
				 for(int i=0; i<node->Nodes->Count; i++) {
					 UncheckTreeNode(node->Nodes[i]);
				 }
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
public: virtual Void ProcessData() override
		{
			 std::string strText;
			 bool bNoChecked = true;
			 if(treeOrganAndAccount->Nodes->Count>0) {
				 //bNoChecked = !treeOrganAndAccount->Nodes[0]->Checked;
				 HasTreeChecked(treeOrganAndAccount->Nodes[0], bNoChecked);
				 //if(bNoChecked || (!bNoChecked && treeOrganAndAccount->Nodes[0]->Checked)) {
					//String^ strKey = treeOrganAndAccount->Nodes[0]->Text;
					//strText += Tools::String2string(strKey);
					//strText += ", ";
				 //}
				 GetTreeItemText(treeOrganAndAccount->Nodes[0], strText, bNoChecked);
				 comboRelate->Text = Tools::string2String(strText.c_str());
			 }
			 else
				 comboRelate->Text = Tools::string2String("");
		 }
};
}

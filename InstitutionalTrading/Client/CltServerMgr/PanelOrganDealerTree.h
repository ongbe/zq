#pragma once

#include "Tools.h"
#include "CDataInfo.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

	/// <summary>
	/// Summary for PanelOrganDealerTree
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class PanelOrganDealerTree : public AnywndComboBox::CAnywndComboBoxControlContainter
	{
	public:
		PanelOrganDealerTree(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pdataInfo = CDataInfo::NewInstance();
			LoadOrganAndAccount();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PanelOrganDealerTree()
		{
			if (components)
			{
				delete components;
			}
		}

	private: int m_nHandCheckIndex;
	private: CDataInfo* m_pdataInfo;
	private: System::Windows::Forms::ComboBox^  comboRelate;
	private: System::Windows::Forms::TreeView^  treeOrganAndAccount;
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
		bool GetSelectID(std::set<std::string>& setRetAccntID) {
			setRetAccntID.clear();
			TreeNode^ node;
			bool bHasCheckSub = false;
			for(int i=0; i<treeOrganAndAccount->Nodes->Count; i++) {
				node = treeOrganAndAccount->Nodes[i];
				GetNodeID(node, setRetAccntID);
			}
			return setRetAccntID.size()>0;
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
				this->treeOrganAndAccount->TabIndex = 1;
				this->treeOrganAndAccount->AfterCheck += gcnew System::Windows::Forms::TreeViewEventHandler(this, &PanelOrganDealerTree::treeOrganAndAccount_AfterCheck);
				// 
				// PanelOrganDealerTree
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->Controls->Add(this->treeOrganAndAccount);
				this->Name = L"PanelOrganDealerTree";
				this->Size = System::Drawing::Size(158, 237);
				this->ResumeLayout(false);

			}
#pragma endregion
	private: void LoadOrganAccount(TreeNode^ node, int nOrganID) {
#if 1
				 std::set<int> setDealerID;
				 std::set<int>::iterator it;
				 if(!m_pdataInfo->GetDealerByOrganID(nOrganID, setDealerID))
					 return;

				 UserInfo sUserInfo = {0};
				 char strTemp[1024];
				 for(it = setDealerID.begin(); it != setDealerID.end(); it++) {
					 if(!m_pdataInfo->GetUserInfoByUserID(*it, sUserInfo))
						 continue;
					 memset(strTemp, 0, sizeof(strTemp));
					 //sprintf(strTemp, "A%d", (*it));
					 sprintf(strTemp, "A%s", sUserInfo.szAccount);
					 String^ strText = Tools::string2String(strTemp);
					 memset(strTemp, 0, sizeof(strTemp));
					 //sprintf(strTemp, "%s%s", (*it).szAccount, (*it).szName);
					 //sprintf_s(strTemp, sizeof(strTemp)-1, "%s%s", sUserInfo.szAccount, sUserInfo.szAlias);
					 sprintf_s(strTemp, sizeof(strTemp)-1, "%s", sUserInfo.szAccount);
					 String^ strValue = Tools::string2String(strTemp);
					 node->Nodes->Add(strText, strValue);
				 }
#endif
			 }

	private: void LoadSubOrganAndAccount(TreeNode^ nodeMain, int nID, 
				 std::map<int, std::map<int, Organization>>& mapMain, 
				 std::map<int, Organization>& mapSub) {
#if 1
					 std::map<int, std::map<int, Organization>>::iterator it;
					 std::map<int, Organization>::iterator itSub;

					 TreeNode^ subNode;
					 char strTemp[1024];
					 memset(strTemp, 0, sizeof(strTemp));

					 itSub = mapSub.begin();
					 for(; itSub != mapSub.end(); itSub++) {
						 sprintf(strTemp, "O%d", itSub->first);
						 String^ strText = Tools::string2String(strTemp);
						 String^ strValue = Tools::string2String(itSub->second.szName);
						 subNode = nodeMain->Nodes->Add(strText, strValue);

						 it = mapMain.find(itSub->first);
						 if(it != mapMain.end())
							 LoadSubOrganAndAccount(subNode, itSub->first, mapMain, it->second);
					 }
					 LoadOrganAccount(nodeMain, nID);
#endif
			 }

	private: void LoadOrganAndAccount() {
#if 1
				 //if(!m_pdataInfo->IsInit())
				 // return;

				 int nUserID = 1;
				 //sLoginRsp loginRsp;
				 //memset(&loginRsp, 0, sizeof(loginRsp));
				 //m_pdataInfo->GetLogonInfo(loginRsp);

				 //UserInfo userInfo;
				 //memset(&userInfo, 0, sizeof(userInfo));
				 //if(!m_pdataInfo->GetUserInfoByUserID(loginRsp.mnUserID, userInfo))
				 // return;

				 std::map<int, std::map<int, Organization>> mapOrgTree;
				 m_pdataInfo->GetOrganization(mapOrgTree);
				 std::map<int, std::map<int, Organization>>::iterator it;

				 // 通过用户ID获得组织机构ID
				 int organID = -1;
				 string strName;
				 //if((organID = m_pdataInfo->GetOrganIDByUserID(loginRsp.mnUserID)) == -1)
					// return;
				 // 由于目前管理员未与组织机构挂钩，所以直接使用顶层机构
				 {
					 it = mapOrgTree.begin();
					 if(it->second.begin() != it->second.end()) {
						 organID = it->second.begin()->first;
						 strName = it->second.begin()->second.szName;
					 }
				 }

				 // 显示用户所属机构为RootNode
				 TreeNode^ node;
				 char strTemp[1024];
				 memset(strTemp, 0, sizeof(strTemp));
				 sprintf(strTemp, "O%d", organID);
				 String^ strText = Tools::string2String(strTemp);
				 String^ strValue = Tools::string2String(strName.c_str());
				 treeOrganAndAccount->Nodes->Clear();
				 node = treeOrganAndAccount->Nodes->Add(strText, strValue);

				it = mapOrgTree.find(organID);
				 if(it == mapOrgTree.end())
					 return;

				 std::map<int, Organization>& mapSub = it->second;
				 LoadSubOrganAndAccount(node, organID, mapOrgTree, mapSub);

#if 0
				 {
					 MAPTrustTradeAccount mapFPID;
					 MAPTrustTradeAccount::iterator it;
					 m_pdataInfo->GetTrustTradeAccount(mapFPID);
					 int nCount = 0;
					 char strTemp[1024];
					 for(it = mapFPID.begin(); it != mapFPID.end()/* && nCount<300*/; it++, nCount++) {
						 //if(!(strcmp(it->second.szAccount, "020059")==0 || 
						 //		strcmp(it->second.szAccount, "021491")==0 || 
						 //		strcmp(it->second.szAccount, "020661")==0 || 
						 //		strcmp(it->second.szAccount, "15377052501")==0 || 
						 //		strcmp(it->second.szAccount, "13018051601")==0 || 
						 //		strcmp(it->second.szAccount, "100081")==0))
						 //	continue;

						 memset(strTemp, 0, sizeof(strTemp));
						 sprintf(strTemp, "A%d", it->second.nTradeAccountID);
						 String^ strText = Tools::string2String(strTemp);
						 memset(strTemp, 0, sizeof(strTemp));
						 sprintf(strTemp, "%s%s", it->second.szAccount, it->second.szName);
						 String^ strValue = Tools::string2String(strTemp);
						 node->Nodes->Add(strText, strValue);
					 }
				 }
#endif

#else
				 MAPAeestMgmtOrganization mapAssetMgmtOrgTree;
				 m_pdataInfo->GetAeestMgmtOrg(mapAssetMgmtOrgTree);
				 MAPAeestMgmtOrganization::iterator it = mapAssetMgmtOrgTree.begin();
				 if(it == mapAssetMgmtOrgTree.end())
					 return;

				 std::map<int, AssetMgmtOrganization>& mapSub = it->second;
				 if(mapSub.begin()==mapSub.end())
					 return;

				 std::map<int, AssetMgmtOrganization>::iterator itSub;
				 itSub = mapSub.begin();

				 TreeNode^ node;

				 char strTemp[1024];
				 memset(strTemp, 0, sizeof(strTemp));
				 sprintf(strTemp, "O%d", itSub->second.nAssetMgmtOrgID);
				 String^ strText = Tools::string2String(strTemp);
				 String^ strValue = Tools::string2String(itSub->second.szName);
				 treeOrganAndAccount->Nodes->Clear();
				 node = treeOrganAndAccount->Nodes->Add(strText, strValue);

				 it = mapAssetMgmtOrgTree.find(itSub->second.nAssetMgmtOrgID);
				 if(it == mapAssetMgmtOrgTree.end())
					 return;

				 LoadSubOrganAndAccount(node, itSub->second.nAssetMgmtOrgID, 
					 mapAssetMgmtOrgTree, it->second);
				 //LoadOrganAccount(node, itSub->second.nAssetMgmtOrgID);

#endif
				 node->Expand();
			 }

	private: System::Void COrganAndAccountIDTreeListDlg_Load(System::Object^  sender, System::EventArgs^  e) {
				 //Rectangle rect = this->DisplayRectangle;
				 treeOrganAndAccount->Location = Point(0, 0);
				 //treeOrganAndAccount->Width = rect.Width;
				 //treeOrganAndAccount->Height = rect.Height;
			 }

	private: void GetNodeID(TreeNode^ node, std::set<std::string>& setRetAccntID) {

				 std::set<int> setAccountID;

				 std::string strKey = Tools::String2string(node->Name);
				 BOOL bIsOrgan = (strKey[0]=='O');

				 if(bIsOrgan) {
					 //std::set<TrustTradeAccount> setFPID;
					 //std::set<TrustTradeAccount>::iterator it;
					 //if(m_pdataInfo->GetTrustTradeAccountByOrgID(
					 //		nID, false, setFPID)>0) {
					 //	for(it = setFPID.begin(); it != setFPID.end(); it++) {
					 //		setRetAccntID.insert((*it).nTradeAccountID);
					 //	}
					 //}
					 for(int i=0; i<node->Nodes->Count; i++) {
						 GetNodeID(node->Nodes[i], setRetAccntID);
					 }
				 }
				 else {
					 if(node->Checked) {
						 std::string strID(strKey);
						 strID[0] = ' ';
						 setRetAccntID.insert(strID.c_str()+1);
					 }
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
				 //else {
				//	 return;
				 //}
				 AutoCheckSubNode(e->Node, e->Node->Checked);
				 m_nHandCheckIndex = -1;
			 }
	};
}

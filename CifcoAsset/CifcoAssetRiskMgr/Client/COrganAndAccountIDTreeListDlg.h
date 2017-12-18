#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Tools.h"
#include "CDataInfo.h"

namespace Client {

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
			m_pdataInfo = CDataInfo::NewInstance();
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
	private: CDataInfo* m_pdataInfo;
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

				//std::string strKey = Tools::String2string(node->Name);
				//std::string strID;
				//BOOL bIsOrgan = (strKey[0]=='O');
				//int nID = 0;

				//strID = strKey;
				//strID[0] = ' ';
				//nID = atoi(strID.c_str());

				//if(bIsOrgan && node->Checked) {
				//	std::set<TrustTradeAccount> setFPID;
				//	std::set<TrustTradeAccount>::iterator it;
				//	if(m_pdataInfo->GetTrustTradeAccountByOrgID(
				//			nID, false, setFPID)>0) {
				//		for(it = setFPID.begin(); it != setFPID.end(); it++) {
				//			setRetAccntID.insert((*it).nTradeAccountID);
				//		}
				//	}
				//}
				//if(!bIsOrgan && node->Checked) {
				//	setRetAccntID.insert(nID);
				//}
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
	private: void LoadOrganAccount(TreeNode^ node, int nOrganID) {
#if 1
				std::set<TrustTradeAccount> setFPID;
				std::set<TrustTradeAccount>::iterator it;
				if(m_pdataInfo->GetTrustTradeAccountByOrgID(
						nOrganID, false, setFPID)<=0)
					return;

				char strTemp[1024];
				for(it = setFPID.begin(); it != setFPID.end(); it++) {
					memset(strTemp, 0, sizeof(strTemp));
					sprintf(strTemp, "A%d", (*it).nTradeAccountID);
					String^ strText = Tools::string2String(strTemp);
					memset(strTemp, 0, sizeof(strTemp));
					sprintf(strTemp, "%s%s", (*it).szAccount, (*it).szName);
					String^ strValue = Tools::string2String(strTemp);
					node->Nodes->Add(strText, strValue);
				}
#endif
			 }

	private: void LoadSubOrganAndAccount(TreeNode^ nodeMain, int nID, 
					MAPAeestMgmtOrganization& mapMain, 
					std::map<int, AssetMgmtOrganization>& mapSub) {
#if 1
				MAPAeestMgmtOrganization::iterator it;
				std::map<int, AssetMgmtOrganization>::iterator itSub;

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
				if(!m_pdataInfo->IsInit())
					return;

				LoginRsp loginRsp;
				memset(&loginRsp, 0, sizeof(loginRsp));
				m_pdataInfo->GetLoginRsp(loginRsp);

				AssetMgmtOrganization assertOrg;
				memset(&assertOrg, 0, sizeof(assertOrg));
				if(!m_pdataInfo->GetAssetMgmtOrg(
						loginRsp.nAssetMgmtOrgID, assertOrg))
					return;

				TreeNode^ node;
				
				char strTemp[1024];
				memset(strTemp, 0, sizeof(strTemp));
				sprintf(strTemp, "O%d", assertOrg.nAssetMgmtOrgID);
				String^ strText = Tools::string2String(strTemp);
				String^ strValue = Tools::string2String(assertOrg.szName);
				treeOrganAndAccount->Nodes->Clear();
				node = treeOrganAndAccount->Nodes->Add(strText, strValue);

				MAPAeestMgmtOrganization mapAssetMgmtOrgTree;
				m_pdataInfo->GetAeestMgmtOrg(mapAssetMgmtOrgTree);
				MAPAeestMgmtOrganization::iterator it = 
						mapAssetMgmtOrgTree.find(loginRsp.nAssetMgmtOrgID);
				if(it == mapAssetMgmtOrgTree.end())
					return;

				std::map<int, AssetMgmtOrganization>& mapSub = it->second;
				//if(mapSub.begin()==mapSub.end())
				//	return;

				LoadSubOrganAndAccount(node, assertOrg.nAssetMgmtOrgID, 
						mapAssetMgmtOrgTree, mapSub);

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

	private: void GetNodeID(TreeNode^ node, std::set<int>& setRetAccntID) {

				 std::set<int> setAccountID;
				 
				 std::string strKey = Tools::String2string(node->Name);
				 std::string strID;
				 BOOL bIsOrgan = (strKey[0]=='O');
				 int nID = 0;

				 strID = strKey;
				 strID[0] = ' ';
				 nID = atoi(strID.c_str());

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

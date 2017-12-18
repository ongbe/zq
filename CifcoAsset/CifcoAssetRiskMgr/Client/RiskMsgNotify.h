#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "Tools.h"
#include "CDataInfo.h"
#include "RiskMsgNotify2.h"
namespace Client {

	/// <summary>
	/// Summary for RiskMsgNotify
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class RiskMsgNotify : public System::Windows::Forms::Form
	{
	public:
		RiskMsgNotify(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            m_pdataInfo = CDataInfo::NewInstance();
            m_pRiskMsgNotify2 = CRiskMsgNotify2::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RiskMsgNotify()
		{
			if (components)
			{
				delete components;
			}

            if(m_pRiskMsgNotify2 != NULL)
                CRiskMsgNotify2::DestroyInstance(m_pRiskMsgNotify2);
		}
    private: System::Windows::Forms::TreeView^  organizationDepartment_treeView;
    private: System::Windows::Forms::Button^  allSelect_button;
    private: System::Windows::Forms::Button^  nullSelect_button;
    private: System::Windows::Forms::Button^  userOnline_button;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::DateTimePicker^  data_dateTimePicker;


    private: System::Windows::Forms::Label^  label2;
    private: System::Windows::Forms::Label^  label3;
    private: System::Windows::Forms::TextBox^  title_textBox;

    private: System::Windows::Forms::Button^  send_button;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
    private: System::Windows::Forms::TextBox^  content_textBox;
    private: CDataInfo* m_pdataInfo;
    private: CRiskMsgNotify2* m_pRiskMsgNotify2;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->organizationDepartment_treeView = (gcnew System::Windows::Forms::TreeView());
            this->allSelect_button = (gcnew System::Windows::Forms::Button());
            this->nullSelect_button = (gcnew System::Windows::Forms::Button());
            this->userOnline_button = (gcnew System::Windows::Forms::Button());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->data_dateTimePicker = (gcnew System::Windows::Forms::DateTimePicker());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->label3 = (gcnew System::Windows::Forms::Label());
            this->title_textBox = (gcnew System::Windows::Forms::TextBox());
            this->send_button = (gcnew System::Windows::Forms::Button());
            this->content_textBox = (gcnew System::Windows::Forms::TextBox());
            this->SuspendLayout();
            // 
            // organizationDepartment_treeView
            // 
            this->organizationDepartment_treeView->CheckBoxes = true;
            this->organizationDepartment_treeView->Location = System::Drawing::Point(12, 22);
            this->organizationDepartment_treeView->Name = L"organizationDepartment_treeView";
            this->organizationDepartment_treeView->Size = System::Drawing::Size(193, 379);
            this->organizationDepartment_treeView->TabIndex = 0;
            this->organizationDepartment_treeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &RiskMsgNotify::organizationDepartment_treeView_AfterSelect);
            // 
            // allSelect_button
            // 
            this->allSelect_button->Location = System::Drawing::Point(40, 420);
            this->allSelect_button->Name = L"allSelect_button";
            this->allSelect_button->Size = System::Drawing::Size(49, 23);
            this->allSelect_button->TabIndex = 1;
            this->allSelect_button->Text = L"全选";
            this->allSelect_button->UseVisualStyleBackColor = true;
            this->allSelect_button->Click += gcnew System::EventHandler(this, &RiskMsgNotify::allSelect_button_Click);
            // 
            // nullSelect_button
            // 
            this->nullSelect_button->Location = System::Drawing::Point(132, 420);
            this->nullSelect_button->Name = L"nullSelect_button";
            this->nullSelect_button->Size = System::Drawing::Size(49, 23);
            this->nullSelect_button->TabIndex = 1;
            this->nullSelect_button->Text = L"空选";
            this->nullSelect_button->UseVisualStyleBackColor = true;
            this->nullSelect_button->Click += gcnew System::EventHandler(this, &RiskMsgNotify::nullSelect_button_Click);
            // 
            // userOnline_button
            // 
            this->userOnline_button->Enabled = false;
            this->userOnline_button->Location = System::Drawing::Point(224, 420);
            this->userOnline_button->Name = L"userOnline_button";
            this->userOnline_button->Size = System::Drawing::Size(75, 23);
            this->userOnline_button->TabIndex = 1;
            this->userOnline_button->Text = L"在线用户";
            this->userOnline_button->UseVisualStyleBackColor = true;
            this->userOnline_button->Visible = false;
            this->userOnline_button->Click += gcnew System::EventHandler(this, &RiskMsgNotify::userOnline_button_Click);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(222, 25);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(53, 12);
            this->label1->TabIndex = 2;
            this->label1->Text = L"有效日期";
            // 
            // data_dateTimePicker
            // 
            this->data_dateTimePicker->Location = System::Drawing::Point(289, 21);
            this->data_dateTimePicker->MinDate = System::DateTime(2013, 1, 25, 0, 0, 0, 0);
            this->data_dateTimePicker->Name = L"data_dateTimePicker";
            this->data_dateTimePicker->Size = System::Drawing::Size(200, 21);
            this->data_dateTimePicker->TabIndex = 3;
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(222, 54);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(29, 12);
            this->label2->TabIndex = 2;
            this->label2->Text = L"标题";
            // 
            // label3
            // 
            this->label3->AutoSize = true;
            this->label3->Location = System::Drawing::Point(222, 83);
            this->label3->Name = L"label3";
            this->label3->Size = System::Drawing::Size(29, 12);
            this->label3->TabIndex = 2;
            this->label3->Text = L"正文";
            // 
            // title_textBox
            // 
            this->title_textBox->Location = System::Drawing::Point(289, 51);
            this->title_textBox->Name = L"title_textBox";
            this->title_textBox->Size = System::Drawing::Size(200, 21);
            this->title_textBox->TabIndex = 4;
            // 
            // send_button
            // 
            this->send_button->Location = System::Drawing::Point(414, 423);
            this->send_button->Name = L"send_button";
            this->send_button->Size = System::Drawing::Size(75, 23);
            this->send_button->TabIndex = 1;
            this->send_button->Text = L"发送";
            this->send_button->UseVisualStyleBackColor = true;
            this->send_button->Click += gcnew System::EventHandler(this, &RiskMsgNotify::send_button_Click);
            // 
            // content_textBox
            // 
            this->content_textBox->AllowDrop = true;
            this->content_textBox->Location = System::Drawing::Point(289, 83);
            this->content_textBox->Multiline = true;
            this->content_textBox->Name = L"content_textBox";
            this->content_textBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
            this->content_textBox->Size = System::Drawing::Size(329, 318);
            this->content_textBox->TabIndex = 6;
            // 
            // RiskMsgNotify
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(641, 458);
            this->Controls->Add(this->content_textBox);
            this->Controls->Add(this->title_textBox);
            this->Controls->Add(this->data_dateTimePicker);
            this->Controls->Add(this->label3);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->send_button);
            this->Controls->Add(this->userOnline_button);
            this->Controls->Add(this->nullSelect_button);
            this->Controls->Add(this->allSelect_button);
            this->Controls->Add(this->organizationDepartment_treeView);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->Name = L"RiskMsgNotify";
            this->Text = L"RiskMsgNotify";
            this->Load += gcnew System::EventHandler(this, &RiskMsgNotify::RiskMsgNotify_Load);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &RiskMsgNotify::RiskMsgNotify_FormClosed);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
protected:virtual void WndProc( Message% m ) override
            {
              if (  m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival )
              {
                  char *p = (char*)m.LParam.ToInt32();
                  Stru_UniPkgHead head;					
                  memset(&head, 0, sizeof(head));
                  memcpy(&head, p, sizeof(head));

                  if ( head.cmdid == Cmd_RM_AddMessage_Rsp )//消息通知响应
                  {
                      if ( head.userdata1 == OPERATOR_SUCCESS )
                      {		
                          MessageBox::Show("消息通知成功！", "提示",MessageBoxButtons::OK);
                      }
                      else
                      {
                          MessageBox::Show("服务器异常，消息通知失败！", "提示",MessageBoxButtons::OK);
                      }
                  }                  
              }
              Form::WndProc ( m );
            }

private: System::Void RiskMsgNotify_Load(System::Object^  sender, System::EventArgs^  e) {
             IntPtr hWnd=this->Handle;   
             CRiskMsgNotify2::SubscribeAddMessageRsp((int)hWnd);             

             LoadAssertMgmtOrgan();
             //添加点击复选框事件
             this->organizationDepartment_treeView->AfterCheck += gcnew  System::Windows::Forms::TreeViewEventHandler(this,&RiskMsgNotify::organizationDepartment_treeView_AfterSelect);
         }
private: void LoadAssertMgmtOrgan() {
         LoginRsp loginRsp;
         memset(&loginRsp, 0, sizeof(loginRsp));
         m_pdataInfo->GetLoginRsp(loginRsp);

         AssetMgmtOrganization assertOrg;
         memset(&assertOrg, 0, sizeof(assertOrg));
         if(!m_pdataInfo->GetAssetMgmtOrg(loginRsp.nAssetMgmtOrgID, assertOrg))
             return;

         TreeNode^ node;

         int^ nID = assertOrg.nAssetMgmtOrgID;
         String^ strText = nID->ToString();
         String^ strValue = Tools::string2String(assertOrg.szName);
         organizationDepartment_treeView->Nodes->Clear();
         node = organizationDepartment_treeView->Nodes->Add(strText, strValue);

         MAPAeestMgmtOrganization mapAssetMgmtOrgTree;
         m_pdataInfo->GetAeestMgmtOrg(mapAssetMgmtOrgTree);

         m_pRiskMsgNotify2->ClearRiskMgmtUserInfo();
         MAPOrgID2RiskMgmtUserInfo mapOrgID2RiskMgmtUserInfo;
         m_pdataInfo->GetmapOrgID2RiskMgmtUserInfo(mapOrgID2RiskMgmtUserInfo);
         GetRiskMgmtUserInfoByOrgID(node,mapAssetMgmtOrgTree,loginRsp.nAssetMgmtOrgID,mapOrgID2RiskMgmtUserInfo);
         node->Expand();
     }
private: void GetRiskMgmtUserInfoByOrgID(TreeNode^ nodeMain,MAPAeestMgmtOrganization& mapMain,int  nAssetMgmtOrg,MAPOrgID2RiskMgmtUserInfo& mapOrgID2RiskMgmtUserInfo)
 {
     std::map<int, map<int, AssetMgmtOrganization>>::iterator it = mapMain.find(nAssetMgmtOrg);
     if ( it == mapMain.end() )
         return ;

     TreeNode^ subNode;

     map<int, AssetMgmtOrganization>& mapAMO  = it->second; 
     for(map<int, AssetMgmtOrganization>::const_iterator iter = mapAMO.begin(); iter != mapAMO.end(); ++iter)
     {  
         it = mapMain.find(iter->first);
         if(it != mapMain.end())
         {       
             int^ nID = iter->first;
             String^ strText = nID->ToString();
             String^ strValue = Tools::string2String(iter->second.szName);
             subNode = nodeMain->Nodes->Add(strText, strValue);
             GetRiskMgmtUserInfoByOrgID(subNode,mapMain,Convert::ToInt32(nID),mapOrgID2RiskMgmtUserInfo);
         }
     }

     int ncount = mapOrgID2RiskMgmtUserInfo.count(nAssetMgmtOrg);
     MAPOrgID2RiskMgmtUserInfo::iterator it2 = mapOrgID2RiskMgmtUserInfo.find(nAssetMgmtOrg);
     for(int nIndex = 0; nIndex < ncount; nIndex ++)
     {	
         RiskMgmtUserInfo riskMgmtUserInfo = it2->second;

         int^ nID = riskMgmtUserInfo.nAssetMgmtOrgID;
         String^ strText = nID->ToString();
         String^ strValue = Tools::string2String(riskMgmtUserInfo.szAccount);
         nodeMain->Nodes->Add(strText, strValue);

         m_pRiskMsgNotify2->SetRiskMgmtUserInfo(riskMgmtUserInfo);

         it2++;
     }
     return ;
 }
private: System::Void allSelect_button_Click(System::Object^  sender, System::EventArgs^  e) {
             for(int i=0; i<organizationDepartment_treeView->Nodes->Count; i++) 
             {
                 TreeNode^ node = organizationDepartment_treeView->Nodes[i];               
                 node->Checked = true;

                 CheckAllChildNodes(node,node->Checked);
             }
         }
private: System::Void nullSelect_button_Click(System::Object^  sender, System::EventArgs^  e) {
             for(int i=0; i<organizationDepartment_treeView->Nodes->Count; i++) 
             {
                 TreeNode^ node = organizationDepartment_treeView->Nodes[i];               
                 node->Checked = false;

                 CheckAllChildNodes(node,node->Checked);
             }
         }
private: System::Void userOnline_button_Click(System::Object^  sender, System::EventArgs^  e) {
         }
private: void FindSelectCheck(TreeNode^ node)
         {
//              if (node->Checked)
//                  MessageBox::Show(node->Text);

             TargetAccount targetAccount;
             memset(&targetAccount,0,sizeof(TargetAccount));

             std::vector<RiskMgmtUserInfo> vecRiskMgmtUserInfo = m_pRiskMsgNotify2->GetRiskMgmtUserInfo();
             for (std::vector<RiskMgmtUserInfo>::iterator it = vecRiskMgmtUserInfo.begin();it!=vecRiskMgmtUserInfo.end();it++)
             {
                 if (node->Checked && node->Text == Tools::string2String(it->szAccount))
                 {
                     targetAccount.nRiskMgmtUserID = it->nRiskMgmtUserID;             
                     memcpy(targetAccount.szAccount,(void*)it->szAccount,sizeof(targetAccount.szAccount));

                     m_pRiskMsgNotify2->SetTargetAccount(targetAccount);
                 }
             }

             for each(TreeNode^ childNode in node->Nodes)
             {
                 //if (childNode->Checked)
                 {                         
                     FindSelectCheck(childNode);// 递归
                 }
             }
         }
private: System::Void send_button_Click(System::Object^  sender, System::EventArgs^  e) {
             MessageInfo msgInfo;
             memset(&msgInfo,0,sizeof(MessageInfo));

             m_pRiskMsgNotify2->ClearTargetAccount();


             char szData[128] = {0};
             struct tm *p;
             time_t timeData = time(NULL);
             p = localtime(&timeData);
             p->tm_year = p->tm_year + 1900;
             p->tm_mon = p->tm_mon + 1;

             if( this->data_dateTimePicker->Value.Year < p->tm_year ||
                 (this->data_dateTimePicker->Value.Year >= p->tm_year && this->data_dateTimePicker->Value.Month < p->tm_mon) ||
                 (this->data_dateTimePicker->Value.Year >= p->tm_year && this->data_dateTimePicker->Value.Month >= p->tm_mon && this->data_dateTimePicker->Value.Day < p->tm_mday)) 
             {
                 MessageBox::Show("有效日期选择无效！", "提示",MessageBoxButtons::OK);
                 return;
             }

             sprintf(szData,"%04d-%02d-%02d",this->data_dateTimePicker->Value.Year, this->data_dateTimePicker->Value.Month, this->data_dateTimePicker->Value.Day);

             strcpy(msgInfo.szTitle,(Tools::String2string(this->title_textBox->Text)).c_str());
             strcpy(msgInfo.szContent,(Tools::String2string(this->content_textBox->Text)).c_str());
             strcpy(msgInfo.szExpiredDate,szData);

             LoginRsp loginRsp;
             m_pdataInfo->GetLoginRsp(loginRsp); 
             msgInfo.nOwner = loginRsp.nAccountID;

             string strUserName;
             m_pdataInfo->GetUserName(strUserName);
             strcpy(msgInfo.szOwner,strUserName.c_str());
   
             //遍历树
             for each (TreeNode^ CNode in organizationDepartment_treeView->Nodes)
             {
                 FindSelectCheck(CNode);
             } 

             std::vector<TargetAccount> vecTargetAccount = m_pRiskMsgNotify2->GetTargetAccount();
             CRiskMsgNotify2::ReqAddMessage(msgInfo,vecTargetAccount);
         }
private: void CheckAllChildNodes(TreeNode^ treeNode, bool nodeChecked) 
{
    for each(TreeNode^ node in treeNode->Nodes) 
    { 
        node->Checked = nodeChecked; 
        if(node->Nodes->Count > 0) 
        { 
            this->CheckAllChildNodes(node, nodeChecked); 
        } 
    } 
} 
private: System::Void organizationDepartment_treeView_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
             if (e->Action != System::Windows::Forms::TreeViewAction::Unknown)
             {
                 CheckAllChildNodes(e->Node,e->Node->Checked);
                 
                 bool bol = true;
                 if (e->Node->Parent != nullptr)
                 {
                     for (int i=0;i<e->Node->Parent->Nodes->Count;i++)
                     {
                         if(!e->Node->Parent->Nodes[i]->Checked)
                             bol = false;
                     }

                     TreeNode^ tn = e->Node;
                     while(tn->Parent != nullptr)
                     {
                         tn = tn->Parent;
                         tn->Checked = bol;
                     }

                     tn->Checked = bol;
                 }
             }

         }
private: System::Void RiskMsgNotify_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
             IntPtr hWnd=this->Handle;   
             CRiskMsgNotify2::UnsubscribeAllEvt((int)hWnd);
         }
};
}

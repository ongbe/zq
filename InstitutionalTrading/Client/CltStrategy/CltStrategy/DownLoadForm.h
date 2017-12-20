#pragma once
#include "Stdafx.h"
#include "DataManageEx.h"
#include "Tools.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltStrategy {

	/// <summary>
	/// Summary for DownLoadForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class DownLoadForm : public System::Windows::Forms::Form
	{
	public:
		DownLoadForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pDataInfo = CDataManageEx::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~DownLoadForm()
		{
			if (components)
			{
				delete components;
			}
			CDataManageEx::DestroyInstance();
		}
	private: CDataManageEx*     m_pDataInfo;
	private: System::Windows::Forms::Button^  button_cancel;
	protected: 
	private: System::Windows::Forms::Button^  button_ok;

	private: System::Windows::Forms::TreeView^  treeView_I;
	private: System::Windows::Forms::TreeView^  treeView_s;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;

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
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->button_ok = (gcnew System::Windows::Forms::Button());
			this->treeView_I = (gcnew System::Windows::Forms::TreeView());
			this->treeView_s = (gcnew System::Windows::Forms::TreeView());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(557, 526);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(100, 23);
			this->button_cancel->TabIndex = 15;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &DownLoadForm::button_cancel_Click);
			// 
			// button_ok
			// 
			this->button_ok->Location = System::Drawing::Point(434, 526);
			this->button_ok->Name = L"button_ok";
			this->button_ok->Size = System::Drawing::Size(100, 23);
			this->button_ok->TabIndex = 14;
			this->button_ok->Text = L"确定";
			this->button_ok->UseVisualStyleBackColor = true;
			this->button_ok->Click += gcnew System::EventHandler(this, &DownLoadForm::button_ok_Click);
			// 
			// treeView_I
			// 
			this->treeView_I->CheckBoxes = true;
			this->treeView_I->Location = System::Drawing::Point(355, 46);
			this->treeView_I->Name = L"treeView_I";
			this->treeView_I->Size = System::Drawing::Size(302, 398);
			this->treeView_I->TabIndex = 12;
			this->treeView_I->AfterCheck += gcnew System::Windows::Forms::TreeViewEventHandler(this, &DownLoadForm::treeView_I_AfterCheck);
			this->treeView_I->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &DownLoadForm::treeView_I_AfterSelect);
			// 
			// treeView_s
			// 
			this->treeView_s->CheckBoxes = true;
			this->treeView_s->Location = System::Drawing::Point(29, 46);
			this->treeView_s->Name = L"treeView_s";
			this->treeView_s->Size = System::Drawing::Size(287, 398);
			this->treeView_s->TabIndex = 11;
			this->treeView_s->AfterCheck += gcnew System::Windows::Forms::TreeViewEventHandler(this, &DownLoadForm::treeView_s_AfterCheck);
			this->treeView_s->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &DownLoadForm::treeView_s_AfterSelect);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(353, 22);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(65, 12);
			this->label2->TabIndex = 10;
			this->label2->Text = L"选择指标：";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(27, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(65, 12);
			this->label1->TabIndex = 9;
			this->label1->Text = L"选择策略：";
			// 
			// DownLoadForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(694, 581);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->button_ok);
			this->Controls->Add(this->treeView_I);
			this->Controls->Add(this->treeView_s);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Name = L"DownLoadForm";
			this->Text = L"策略与指标下载";
			this->Load += gcnew System::EventHandler(this, &DownLoadForm::DownLoadForm_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &DownLoadForm::DownLoadForm_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	protected: virtual void WndProc( Message% m ) override{
				if(m.Msg == _GetEventMsgID()) 
				{
					switch(m.WParam.ToInt32())
					{
					case CBaseBusinessSpi::conDownloadStrategyFileRsp:
					case CBaseBusinessSpi::conDownloadIndicatorFileRsp:
						m_pDataInfo->OnDownloadFileNext();
						break;
					};
				}
				Form::WndProc ( m );
			}
	private: System::Void DownLoadForm_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent();
				 m_pDataInfo->SubscribeEvent(CBaseBusinessSpi::conDownloadStrategyFileRsp, 
					 (HWND)this->Handle.ToPointer());
				 m_pDataInfo->SubscribeEvent(CBaseBusinessSpi::conDownloadIndicatorFileRsp, 
					 (HWND)this->Handle.ToPointer());
				 //m_pDataInfo->indicatorDisplyMapClear();
				 std::map<std::string, CStrategyDataEx>   MapStrategy;
				 m_pDataInfo->GetAllStrategy_Download(MapStrategy);
				 TreeNode^ nodeS;
				 nodeS = treeView_s->Nodes->Add("策略");
				 std::map<std::string, CStrategyDataEx>::iterator its = MapStrategy.begin();
				 for ( its; its!=MapStrategy.end(); its++)
				 {
					 if (its->second.GetStrategyPrivate() == 1)
					 {
						 String^ strText = Tools::string2String((its->first).c_str());
						 TreeNode^ node = gcnew TreeNode();
						 node->Name = strText;
						 node->Text = strText;
						 node->BackColor = Color::Empty;//背景颜色

						 nodeS->Nodes->Add(node);
					 }

					 else //if (its->second == 2)
					 {
						 String^ strText = Tools::string2String((its->first).c_str());

						 TreeNode^ node = gcnew TreeNode();
						 node->Name = strText;
						 node->Text = strText;
						 node->BackColor = Color::Empty;//背景颜色

						 nodeS->Nodes->Add(node);
					 }

				 }
				 nodeS->Expand();




				 std::map<std::string, CIndicatorDataEx>   MapIndicator;
				 m_pDataInfo->GetAllIndicator_Download(MapIndicator);
				 TreeNode^ nodeI;
				 nodeI = treeView_I->Nodes->Add("指标");
				 std::map<std::string, CIndicatorDataEx>::iterator iti = MapIndicator.begin();
				 for ( iti; iti!=MapIndicator.end(); iti++)
				 {

					 if (iti->second.GetIndicatorPrivate() == 1)
					 {
						 String^ strText = Tools::string2String((iti->first).c_str());
						 TreeNode^ node = gcnew TreeNode();
						 node->Name = strText;
						 node->Text = strText;
						 node->BackColor = Color::Empty;//背景颜色

						 nodeI->Nodes->Add(node);
					 }

					 else //if (iti->second == 2)
					 {
						 String^ strText = Tools::string2String((iti->first).c_str());

						 TreeNode^ node = gcnew TreeNode();
						 node->Name = strText;
						 node->Text = strText;
						 node->BackColor = Color::Empty;//背景颜色

						 nodeI->Nodes->Add(node);
					 }
				 }
				 nodeI->Expand();
			 }
private: System::Void DownLoadForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 m_pDataInfo->UnsubscribeAllEvent((HWND)this->Handle.ToPointer());
		 }
private: System::Void button_ok_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			if(m_pDataInfo->GetLoginTraderStatus() != CDataManageEx::conLogon) 
			{
				MessageBox::Show("未登录到策略服务器。", "错误", MessageBoxButtons::OK);
				return;
			}
			 list<string> listStrategy;//策略名
			 TreeNode^ nodeS;
			 nodeS = treeView_s->Nodes[0];
			 for(int i=0; i<nodeS->Nodes->Count; i++) 
			 {				 
				 if(nodeS->Nodes[i]->Checked) 
				 {
					 listStrategy.push_back(Tools::String2string(nodeS->Nodes[i]->Text));
				 }
			 }


			 list<string> listIndicator;//指标名
			 TreeNode^ nodeI;
			 nodeI = treeView_I->Nodes[0];
			 for(int i=0; i<nodeI->Nodes->Count; i++) 
			 {				 
				 if(nodeI->Nodes[i]->Checked) 
				 {
					 listIndicator.push_back(Tools::String2string(nodeI->Nodes[i]->Text));
				 }
			 }

			 list<string>::iterator itIndi;
			 list<string>::iterator itStra;

			 for(itIndi = listIndicator.begin(); itIndi != listIndicator.end(); itIndi++) {
				 m_pDataInfo->GetTraderApi()->ReqDownloadIndicatorInfo(*itIndi, 0);
				 m_pDataInfo->GetTraderApi()->ReqDownloadIndicatorFile(*itIndi, 0, true);
				 m_pDataInfo->GetTraderApi()->ReqDownloadIndicatorFile(*itIndi, 0, false);
			 }

			 //Sleep(1000);

			 for(itStra = listStrategy.begin(); itStra != listStrategy.end(); itStra++) {
				 m_pDataInfo->GetTraderApi()->ReqDownloadStategyInfo(*itStra, 0);
				 m_pDataInfo->GetTraderApi()->ReqDownloadStategyFile(*itStra, 0, true);
				 m_pDataInfo->GetTraderApi()->ReqDownloadStategyFile(*itStra, 0, false);
			 }

		 }
private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Close();
		 }
		 //取消节点选中状态之后，取消所有父节点的选中状态
			 private: System::Void setParentNodeCheckedState(TreeNode^ currNode, bool state)
					  {
						  TreeNode^ parentNode = currNode->Parent;

						  parentNode->Checked = state;
						  if (currNode->Parent->Parent != nullptr)
						  {
							  setParentNodeCheckedState(currNode->Parent, state);
						  }
					  }






					  //选中节点之后，选中节点的所有子节点
			 private: System::Void setChildNodeCheckedState(TreeNode^ currNode, bool state)
					  {
						  TreeNodeCollection^ nodes = currNode->Nodes;
						  if (nodes->Count > 0)
							  for each (TreeNode^ tn in nodes)
							  {

								  tn->Checked = state;
								  setChildNodeCheckedState(tn, state);
							  }
					  }
private: System::Void treeView_s_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
		 {
			 //if (e->Action == TreeViewAction::ByMouse)
			 //{

				// if (e->Node->Checked)
				// {
				//	 //取消节点选中状态之后，取消所有子节点的选中状态
				//	 setChildNodeCheckedState(e->Node, true);

				// }
				// else
				// {
				//	 //节点选中状态之后，所有zi节点的选中状态
				//	 setChildNodeCheckedState(e->Node, false);
				//	 //如果节点存在父节点，取消父节点的选中状态
				//	 if (e->Node->Parent != nullptr)
				//	 {
				//		 setParentNodeCheckedState(e->Node, false);
				//	 }
				// }
			 //}
		 }
private: System::Void treeView_I_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
		 {
			 //if (e->Action == TreeViewAction::ByMouse)
			 //{

				// if (e->Node->Checked)
				// {
				//	 //取消节点选中状态之后，取消所有子节点的选中状态
				//	 setChildNodeCheckedState(e->Node, true);

				// }
				// else
				// {
				//	 //节点选中状态之后，所有zi节点的选中状态
				//	 setChildNodeCheckedState(e->Node, false);
				//	 //如果节点存在父节点，取消父节点的选中状态
				//	 if (e->Node->Parent != nullptr)
				//	 {
				//		 setParentNodeCheckedState(e->Node, false);
				//	 }
				// }
			 //}
		 }

		 private: System::Void setIndicatorDisplay(String^ name, bool state)
					 {
						 std::list<string> filed;
						 m_pDataInfo->GetIndicator(Tools::String2string(name),filed);
						 m_pDataInfo->SetIndicatorDiaplay(filed,state);
						 TreeNode^ nodeI;
						 nodeI = treeView_I->Nodes[0];
						 for(int i=0; i<nodeI->Nodes->Count; i++) 
						 {
							 if (m_pDataInfo->IsRelativeIndi(Tools::String2string(nodeI->Nodes[i]->Text)))
							 {
								 nodeI->Nodes[i]->BackColor = Color::LightBlue;//背景颜色
							 }
							 else
							 {
								 nodeI->Nodes[i]->BackColor = Color::Empty;//背景颜色
							 }
						 }

					 }
private: System::Void treeView_s_AfterCheck(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
		 {
			 if (e->Action == TreeViewAction::ByMouse)
			 {

				 if (e->Node->Checked)
				 {
					 //选中节点之后，选中节点的所有子节点，取消节点选中状态之后，取消所有子节点的选中状态
					 setChildNodeCheckedState(e->Node, true);
					 //选中策略后，相关的指标高亮
					 setIndicatorDisplay(e->Node->Text, true);

				 }
				 else
				 {
					 //取消节点选中状态之后，取消所有子节点的选中状态
					 setChildNodeCheckedState(e->Node, false);
					  setIndicatorDisplay(e->Node->Text, false);
					 //如果节点存在父节点，取消父节点的选中状态
					 if (e->Node->Parent != nullptr)
					 {
						 setParentNodeCheckedState(e->Node, false);
					 }
				 }
			 }
		 }
private: System::Void treeView_I_AfterCheck(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
		 {
			 if (e->Action == TreeViewAction::ByMouse)
			 {

				 if (e->Node->Checked)
				 {
					 //取消节点选中状态之后，取消所有子节点的选中状态
					 setChildNodeCheckedState(e->Node, true);

				 }
				 else
				 {
					 //节点选中状态之后，所有zi节点的选中状态
					 setChildNodeCheckedState(e->Node, false);
					 //如果节点存在父节点，取消父节点的选中状态
					 if (e->Node->Parent != nullptr)
					 {
						 setParentNodeCheckedState(e->Node, false);
					 }
				 }
			 }
		 }
};
}

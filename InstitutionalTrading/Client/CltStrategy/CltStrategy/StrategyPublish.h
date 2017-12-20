#pragma once
#include "Stdafx.h"
#include "DataManageEx.h"
#include "Tools.h"
#include "ToolsEx.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace EnvDTE80;


namespace CltStrategy {

	/// <summary>
	/// Summary for StrategyPublish
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class StrategyPublish : public System::Windows::Forms::Form
	{
	public:
		StrategyPublish(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pDataInfo = CDataManageEx::NewInstance();
			m_pListWaitIndicator = new list<string>();
			m_pListWaitStrategy = new list<string>();
			m_pMapIndis = new std::map<std::string, CIndicatorDataEx>();
			m_pMapStras = new std::map<std::string, CStrategyDataEx>();
			m_pWaitUpEnd = new UploadEnd;
			m_bHasWaitUpdate = false;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~StrategyPublish()
		{
			if (components)
			{
				delete components;
			}
			CDataManageEx::DestroyInstance();
			delete m_pListWaitIndicator;
			m_pListWaitIndicator = NULL;
			delete m_pListWaitStrategy;
			m_pListWaitStrategy = NULL;
			delete m_pMapIndis;
			m_pMapIndis = NULL;
			delete m_pMapStras;
			m_pMapStras = NULL;
			delete m_pWaitUpEnd;
			m_pWaitUpEnd = NULL;
		}

	private: EnvDTE80::DTE2^ _applicationObject;
	private: CDataManageEx*     m_pDataInfo;
	private: list<string>*		m_pListWaitIndicator;
	private: list<string>*		m_pListWaitStrategy;
	private: std::map<std::string, CIndicatorDataEx>*	m_pMapIndis;
	private: std::map<std::string, CStrategyDataEx>*	m_pMapStras;
	private: UploadEnd*			m_pWaitUpEnd;
	private: bool				m_bHasWaitUpdate;
	private: System::Windows::Forms::Label^  label1;


	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TreeView^  treeView_s;
	private: System::Windows::Forms::TreeView^  treeView_I;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Button^  button_ok;
	private: System::Windows::Forms::Button^  button_cancel;


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
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->treeView_s = (gcnew System::Windows::Forms::TreeView());
			this->treeView_I = (gcnew System::Windows::Forms::TreeView());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->button_ok = (gcnew System::Windows::Forms::Button());
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(30, 26);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(65, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"选择策略：";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(356, 26);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(65, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"选择指标：";
			// 
			// treeView_s
			// 
			this->treeView_s->CheckBoxes = true;
			this->treeView_s->Location = System::Drawing::Point(32, 50);
			this->treeView_s->Name = L"treeView_s";
			this->treeView_s->Size = System::Drawing::Size(287, 398);
			this->treeView_s->TabIndex = 4;
			this->treeView_s->AfterCheck += gcnew System::Windows::Forms::TreeViewEventHandler(this, &StrategyPublish::treeView_s_AfterCheck);
			this->treeView_s->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &StrategyPublish::treeView_s_AfterSelect);
			// 
			// treeView_I
			// 
			this->treeView_I->CheckBoxes = true;
			this->treeView_I->Location = System::Drawing::Point(358, 50);
			this->treeView_I->Name = L"treeView_I";
			this->treeView_I->Size = System::Drawing::Size(302, 398);
			this->treeView_I->TabIndex = 5;
			this->treeView_I->AfterCheck += gcnew System::Windows::Forms::TreeViewEventHandler(this, &StrategyPublish::treeView_I_AfterCheck);
			this->treeView_I->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &StrategyPublish::treeView_I_AfterSelect);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Font = (gcnew System::Drawing::Font(L"SimSun", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(134)));
			this->checkBox1->Location = System::Drawing::Point(553, 477);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(107, 20);
			this->checkBox1->TabIndex = 6;
			this->checkBox1->Text = L"仅本人使用";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// button_ok
			// 
			this->button_ok->Location = System::Drawing::Point(437, 530);
			this->button_ok->Name = L"button_ok";
			this->button_ok->Size = System::Drawing::Size(100, 23);
			this->button_ok->TabIndex = 7;
			this->button_ok->Text = L"确定";
			this->button_ok->UseVisualStyleBackColor = true;
			this->button_ok->Click += gcnew System::EventHandler(this, &StrategyPublish::button_ok_Click);
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(560, 530);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(100, 23);
			this->button_cancel->TabIndex = 8;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &StrategyPublish::button_cancel_Click);
			// 
			// StrategyPublish
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(696, 578);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->button_ok);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->treeView_I);
			this->Controls->Add(this->treeView_s);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Name = L"StrategyPublish";
			this->Text = L"发布";
			this->Load += gcnew System::EventHandler(this, &StrategyPublish::StrategyPublish_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &StrategyPublish::StrategyPublish_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	protected: virtual void WndProc( Message% m ) override{
				if(m.Msg == _GetEventMsgID()) 
				{
					switch(m.WParam.ToInt32())
					{
					case CBaseBusinessSpi::conUploadStartRsp:
						if(m.LParam.ToInt32()==0)
							ContinueForUpload();
						m_bHasWaitUpdate = false;
						break;
					};
				}
				Form::WndProc ( m );
			}
	public: EnvDTE80::DTE2^ getApplicationObject()
			{
				return _applicationObject;					
			}

	public: void setApplicationObject(EnvDTE80::DTE2^ app)
			{

				_applicationObject = app;
			}
	private: void WaitForUpload(list<string>& listIndicator, list<string>& listStrategy, 
				 map<std::string, CIndicatorDataEx>& mapIndis, 
				 map<std::string, CStrategyDataEx>& mapStras)
			 {
				*m_pListWaitIndicator = listIndicator;
				*m_pListWaitStrategy = listStrategy;
				*m_pMapIndis = mapIndis;
				*m_pMapStras = mapStras;
			 }
	private: void ContinueForUpload()
			 {
				list<string>& listIndicator = *m_pListWaitIndicator;
				list<string>& listStrategy = *m_pListWaitStrategy;

				std::map<std::string, CIndicatorDataEx>& mapIndis = *m_pMapIndis;
				list<string>::iterator itIndi;

				for(itIndi = listIndicator.begin(); itIndi != listIndicator.end(); itIndi++) {
				 std::map<std::string, CIndicatorDataEx>::iterator it = 
					 mapIndis.find(*itIndi);
				 if(it != mapIndis.end()) {
					 m_pDataInfo->GetTraderApi()->ReqUploadIndicatorBaseInfo(it->second, 0);
					 m_pDataInfo->GetTraderApi()->ReqUploadIndicatorFile(it->first, 
							it->second.GetDLLFile(), 0, true);
					 m_pDataInfo->GetTraderApi()->ReqUploadIndicatorFile(it->first, 
							it->second.GetZipFile(), 0, false);
				 }
				}

				std::map<std::string, CStrategyDataEx>& mapStras = *m_pMapStras;
				list<string>::iterator itStra;

				for(itStra = listStrategy.begin(); itStra != listStrategy.end(); itStra++) {
					std::map<std::string, CStrategyDataEx>::iterator it = 
							mapStras.find(*itStra);
					if(it != mapStras.end()) {
						m_pDataInfo->GetTraderApi()->ReqUploadStategyBaseInfo(it->second, 0);
						m_pDataInfo->GetTraderApi()->ReqUploadStategyFile(it->first, 
								it->second.GetDLLFile(), 0, true);
						m_pDataInfo->GetTraderApi()->ReqUploadStategyFile(it->first, 
								it->second.GetZipFile(), 0, false);
					}
				}

				m_pDataInfo->GetTraderApi()->ReqUploadEnd(*m_pWaitUpEnd, 0);
			 }

	private: System::Void StrategyPublish_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent();
				 
				 m_pDataInfo->SubscribeEvent(CBaseBusinessSpi::conUploadStartRsp, 
						(HWND)this->Handle.ToPointer());

				 m_pDataInfo->indicatorDisplyMapClear();
				 std::map<std::string, CStrategyDataEx>   MapStrategy;
				 m_pDataInfo->GetAllStrategy(MapStrategy);
				 TreeNode^ nodeS;
				 nodeS = treeView_s->Nodes->Add("策略");
				 std::map<std::string, CStrategyDataEx>::iterator its = MapStrategy.begin();
				 for ( its; its!=MapStrategy.end(); its++)
				 {
					 //if (its->second.GetStrategyPrivate() == 1)
					 {
						 String^ strText = Tools::string2String(its->second.GetStrategyName().c_str());
					
						 nodeS->Nodes->Add(strText, strText);
					 }


				 }
                 nodeS->Expand();



				 std::map<std::string, CIndicatorDataEx>   MapIndicator;
				 m_pDataInfo->GetAllIndicator(MapIndicator);
				 TreeNode^ nodeI;
				 nodeI = treeView_I->Nodes->Add("指标");
				 std::map<std::string, CIndicatorDataEx>::iterator iti = MapIndicator.begin();
				 for ( iti; iti!=MapIndicator.end(); iti++)
				 {
					 //if (iti->second.GetIndicatorPrivate() == 1)
					 {
						 String^ strText = Tools::string2String(iti->second.GetIndicatorName().c_str());
						 nodeI->Nodes->Add(strText, strText);
					 }
				 }
				 nodeI->Expand();

			 }

	private: System::Void StrategyPublish_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
				 m_pDataInfo->UnsubscribeAllEvent((HWND)this->Handle.ToPointer());
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
	private: System::Void treeView_s_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
			 {
				 //if (e->Action == TreeViewAction::ByMouse)
				 //{

					// if (e->Node->Checked)
					// {
					//	 //取消节点选中状态之后，取消所有zi节点的选中状态
					//	 setChildNodeCheckedState(e->Node, true);
					//	 //选中策略后，相关的指标高亮
					//	 setIndicatorDisplay(e->Node->Text, true);

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
private: System::Void button_ok_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			if(m_pDataInfo->GetLoginTraderStatus() != CDataManageEx::conLogon) 
			{
				MessageBox::Show("未登录到策略服务器。", "错误", MessageBoxButtons::OK);
				return;
			}
			 int bPrivate = checkBox1->Checked;
			 std::string strStrategys, strIndicators;

			 //上传的策略已启用则不能上传
			 std::map<std::string, CStrategyDataEx>   MapStrategy;
			 m_pDataInfo->GetAllStrategy_Download(MapStrategy);

			 list<string> listStrategy;//策略名
			 TreeNode^ nodeS;
			 nodeS = treeView_s->Nodes[0];
			 for(int i=0; i<nodeS->Nodes->Count; i++) 
			 {				 
				 if(nodeS->Nodes[i]->Checked) 
				 {
					 string strStrategy =Tools::String2string(nodeS->Nodes[i]->Text);
					 std::map<std::string, CStrategyDataEx>::iterator its = MapStrategy.find(strStrategy);
					 if (its !=MapStrategy.end() && its->second.GetServerUseStatus()==1 )
					 {
						 MessageBox::Show(Tools::string2String(its->first.c_str())+"策略处于启用状态无法覆盖，请先禁用该策略。", "错误", MessageBoxButtons::OK);
						 return; 
					 }
					 listStrategy.push_back(strStrategy);
					 strStrategys += Tools::String2string(nodeS->Nodes[i]->Text);
					 strStrategys += ";";
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
					 strIndicators += Tools::String2string(nodeI->Nodes[i]->Text);
					 strIndicators += ";";
				 }
			 }

			 std::map<std::string, CIndicatorDataEx> mapIndis;
			 list<string>::iterator itIndi;
			 bool bCheckExist = true;
			 std::string strProjectname;
			 std::string strDllFile;

			 m_pDataInfo->GetAllIndicator(mapIndis);
			 for(itIndi = listIndicator.begin(); itIndi != listIndicator.end(); itIndi++) {
				 std::map<std::string, CIndicatorDataEx>::iterator it = 
					 mapIndis.find(*itIndi);
				 if(it != mapIndis.end()) {

					 std::string strRetZipFile;
					 std::string strWinRar(_GetSupportFilesPath());
					 std::string strWorkPath(_GetProjectsPath());
					 strProjectname = it->second.GetIndicatorName();
					 strDllFile = _GetProjectsPath()+it->second.GetIndicatorName()
							+"\\bin\\Release\\"+it->second.GetIndicatorName()+".dll";
					 CToolsEx::Zip_Project(strWinRar, strWorkPath, strProjectname, strRetZipFile);
					 it->second.SetZipFile(strRetZipFile);
					 it->second.SetDLLFile(strDllFile);
					 it->second.SetIndicatorPrivate(bPrivate?1:2);
					 
					 if(!CToolsEx::IsExistFile(strDllFile)) {
						 bCheckExist = false;
						 break;
					 }

				 }
			 }
			 if(!bCheckExist) {
				 String^ strMsg = "指标[";
				 strMsg += gcnew String(strProjectname.c_str()); 
				 strMsg += gcnew String("]的dll文件[");
				 strMsg += gcnew String(strDllFile.c_str());
				 strMsg += gcnew String("]未找到，不能上传。");
				 MessageBox::Show(strMsg);
				 return;
			 }

			 std::map<std::string, CStrategyDataEx> mapStras;
			 list<string>::iterator itStra;

			 m_pDataInfo->GetAllStrategy(mapStras);
			 for(itStra = listStrategy.begin(); itStra != listStrategy.end(); itStra++) {
				 std::map<std::string, CStrategyDataEx>::iterator it = 
					 mapStras.find(*itStra);
				 if(it != mapStras.end()) {

					 std::string strRetZipFile;
					 std::string strWinRar(_GetSupportFilesPath());
					 std::string strWorkPath(_GetProjectsPath());
					 strProjectname = it->second.GetStrategyName();
					 strDllFile = _GetProjectsPath()+it->second.GetStrategyName()
							+"\\bin\\Release\\"+it->second.GetStrategyName()+".dll";
					 CToolsEx::Zip_Project(strWinRar, strWorkPath, strProjectname, strRetZipFile);
					 it->second.SetZipFile(strRetZipFile);
					 it->second.SetDLLFile(strDllFile);
					 it->second.SetStrategyPrivate(bPrivate?1:2);

					 if(!CToolsEx::IsExistFile(strDllFile)) {
						 bCheckExist = false;
						 break;
					 }

				 }
			 }
			 if(!bCheckExist) {
				 String^ strMsg = "策略[";
				 strMsg += gcnew String(strProjectname.c_str()); 
				 strMsg += gcnew String("]的dll文件[");
				 strMsg += gcnew String(strDllFile.c_str());
				 strMsg += gcnew String("]未找到，不能上传。");
				 MessageBox::Show(strMsg);
				 return;
			 }

			 UINT nReqID = _GetNextRequestID();
			 std::string strXml;
			 UploadStart upStart = {0};
			 //UploadEnd upEnd = {0};
			 memset(m_pWaitUpEnd, 0, sizeof(*m_pWaitUpEnd));

			 sprintf_s(m_pWaitUpEnd->uploadend, sizeof(m_pWaitUpEnd->uploadend)-1, "%u", nReqID);
			 strncpy(m_pWaitUpEnd->strTraderName, _GetCurrUserID().c_str(), sizeof(m_pWaitUpEnd->strTraderName)-1);
			 strXml += "<uploadid>";
			 strXml += m_pWaitUpEnd->uploadend;
			 strXml += "</uploadid>";
			 strXml += "<traderid>";
			 strXml += _GetCurrUserID();
			 strXml += "</traderid>";
			 strXml += "<stratregyname>";
			 strXml += strStrategys;
			 strXml += "</stratregyname>";
			 strXml += "<indexname>";
			 strXml += strIndicators;
			 strXml += "</indexname>";
			 strncpy(upStart.uploadstring, strXml.c_str(), sizeof(upStart.uploadstring)-1);
			 m_pDataInfo->GetTraderApi()->ReqUploadStart(upStart, 0);

			 m_bHasWaitUpdate = true;
			 WaitForUpload(listIndicator, listStrategy, mapIndis, mapStras);

			 //for(itIndi = listIndicator.begin(); itIndi != listIndicator.end(); itIndi++) {
				// std::map<std::string, CIndicatorDataEx>::iterator it = 
				//	 mapIndis.find(*itIndi);
				// if(it != mapIndis.end()) {

				//	 it->second.SetIndicatorPrivate(bPrivate?1:2);
				//	 m_pDataInfo->GetTraderApi()->ReqUploadIndicatorBaseInfo(it->second, 0);
				//	 m_pDataInfo->GetTraderApi()->ReqUploadIndicatorFile(it->first, 
				//			it->second.GetDLLFile(), 0, true);
				//	 m_pDataInfo->GetTraderApi()->ReqUploadIndicatorFile(it->first, 
				//			it->second.GetZipFile(), 0, false);
				// }
			 //}

			 //for(itStra = listStrategy.begin(); itStra != listStrategy.end(); itStra++) {
				// std::map<std::string, CStrategyDataEx>::iterator it = 
				//	 mapStras.find(*itStra);
				// if(it != mapStras.end()) {

				//	 it->second.SetStrategyPrivate(bPrivate?1:2);
				//	 m_pDataInfo->GetTraderApi()->ReqUploadStategyBaseInfo(it->second, 0);
				//	 m_pDataInfo->GetTraderApi()->ReqUploadStategyFile(it->first, 
				//			it->second.GetDLLFile(), 0, true);
				//	 m_pDataInfo->GetTraderApi()->ReqUploadStategyFile(it->first, 
				//			it->second.GetZipFile(), 0, false);
				// }
			 //}

			 //m_pDataInfo->GetTraderApi()->ReqUploadEnd(upEnd, 0);

		 }
private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Close();
		 }
private: System::Void treeView_s_AfterCheck(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
		 {
			 if (e->Action == TreeViewAction::ByMouse)
			 {

				 if (e->Node->Checked)
				 {
					 //节点选中状态之后，所有zi节点的选中状态
					 setChildNodeCheckedState(e->Node, true);
					 //选中策略后，相关的指标高亮
					 setIndicatorDisplay(e->Node->Text, true);

				 }
				 else
				 {
					 //取消节点选中状态之后，取消所有zi节点的选中状态
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

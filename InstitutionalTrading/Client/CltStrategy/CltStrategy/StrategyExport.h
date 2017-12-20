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


namespace CltStrategy {

	/// <summary>
	/// Summary for StrategyExport
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class StrategyExport : public System::Windows::Forms::Form
	{
	public:
		StrategyExport(void)
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
		~StrategyExport()
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
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Button^  button_selecte;

	private: System::Windows::Forms::TextBox^  textBox_dir;

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
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button_selecte = (gcnew System::Windows::Forms::Button());
			this->textBox_dir = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(537, 553);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(100, 23);
			this->button_cancel->TabIndex = 19;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &StrategyExport::button_cancel_Click);
			// 
			// button_ok
			// 
			this->button_ok->Location = System::Drawing::Point(414, 553);
			this->button_ok->Name = L"button_ok";
			this->button_ok->Size = System::Drawing::Size(100, 23);
			this->button_ok->TabIndex = 18;
			this->button_ok->Text = L"确定";
			this->button_ok->UseVisualStyleBackColor = true;
			this->button_ok->Click += gcnew System::EventHandler(this, &StrategyExport::button_ok_Click);
			// 
			// treeView_I
			// 
			this->treeView_I->CheckBoxes = true;
			this->treeView_I->Location = System::Drawing::Point(346, 34);
			this->treeView_I->Name = L"treeView_I";
			this->treeView_I->Size = System::Drawing::Size(302, 398);
			this->treeView_I->TabIndex = 17;
			this->treeView_I->AfterCheck += gcnew System::Windows::Forms::TreeViewEventHandler(this, &StrategyExport::treeView_I_AfterCheck);
			// 
			// treeView_s
			// 
			this->treeView_s->CheckBoxes = true;
			this->treeView_s->Location = System::Drawing::Point(20, 34);
			this->treeView_s->Name = L"treeView_s";
			this->treeView_s->Size = System::Drawing::Size(287, 398);
			this->treeView_s->TabIndex = 16;
			this->treeView_s->AfterCheck += gcnew System::Windows::Forms::TreeViewEventHandler(this, &StrategyExport::treeView_s_AfterCheck);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(353, 19);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(65, 12);
			this->label2->TabIndex = 21;
			this->label2->Text = L"选择指标：";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(27, 19);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(65, 12);
			this->label1->TabIndex = 20;
			this->label1->Text = L"选择策略：";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->button_selecte);
			this->groupBox1->Controls->Add(this->textBox_dir);
			this->groupBox1->Location = System::Drawing::Point(20, 460);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(628, 75);
			this->groupBox1->TabIndex = 22;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"导出路径：";
			// 
			// button_selecte
			// 
			this->button_selecte->Location = System::Drawing::Point(542, 32);
			this->button_selecte->Name = L"button_selecte";
			this->button_selecte->Size = System::Drawing::Size(75, 23);
			this->button_selecte->TabIndex = 1;
			this->button_selecte->Text = L"选择";
			this->button_selecte->UseVisualStyleBackColor = true;
			this->button_selecte->Click += gcnew System::EventHandler(this, &StrategyExport::button1_Click);
			// 
			// textBox_dir
			// 
			this->textBox_dir->Location = System::Drawing::Point(9, 32);
			this->textBox_dir->Name = L"textBox_dir";
			this->textBox_dir->Size = System::Drawing::Size(508, 21);
			this->textBox_dir->TabIndex = 0;
			// 
			// StrategyExport
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(684, 601);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->button_ok);
			this->Controls->Add(this->treeView_I);
			this->Controls->Add(this->treeView_s);
			this->Name = L"StrategyExport";
			this->Text = L"策略导出";
			this->Load += gcnew System::EventHandler(this, &StrategyExport::StrategyExport_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void StrategyExport_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent();
				 m_pDataInfo->indicatorDisplyMapClear();
				 std::map<std::string, CStrategyDataEx>   MapStrategy;
				 m_pDataInfo->GetAllStrategy(MapStrategy);
				 TreeNode^ nodeS;
				 nodeS = treeView_s->Nodes->Add("策略");
				 std::map<std::string, CStrategyDataEx>::iterator its = MapStrategy.begin();
				 for ( its; its!=MapStrategy.end(); its++)
				 {
					 if (its->second.GetStrategyPrivate() == 1)
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
					 if (iti->second.GetIndicatorPrivate() == 1)
					 {
						 String^ strText = Tools::string2String(iti->second.GetIndicatorName().c_str());
						 nodeI->Nodes->Add(strText, strText);
					 }
				 }
				 nodeI->Expand();
			 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 SaveFileDialog^ dlg = gcnew SaveFileDialog();
			 dlg->Title ="请选择要导出的策略Zip文件存放路径";
			 dlg->Filter = "zip压缩文件(*.zip)|*.zip";

			 if(dlg->ShowDialog() == System::Windows::Forms::DialogResult::OK) 
			 {
				textBox_dir->Text =dlg->FileName;
			 }

		 }
private: System::Void button_ok_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 std::set<std::string> setIndicator;	//策略名
			 std::set<std::string> setStrategy;		//指标名
			 TreeNode^ nodeS;
			 nodeS = treeView_s->Nodes[0];
			 for(int i=0; i<nodeS->Nodes->Count; i++) 
			 {				 
				 if(nodeS->Nodes[i]->Checked) 
				 {
					 setStrategy.insert(Tools::String2string(nodeS->Nodes[i]->Text));
				 }
			 }

			 TreeNode^ nodeI;
			 nodeI = treeView_I->Nodes[0];
			 for(int i=0; i<nodeI->Nodes->Count; i++) 
			 {				 
				 if(nodeI->Nodes[i]->Checked) 
				 {
					 setIndicator.insert(Tools::String2string(nodeI->Nodes[i]->Text));
				 }
			 }

			 std::string strExportFile(Tools::String2string(textBox_dir->Text));

			 m_pDataInfo->ExportCodes(setStrategy, setIndicator, strExportFile);
			 
			 MessageBox::Show("导出完成");

			 this->Close();
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
};
}

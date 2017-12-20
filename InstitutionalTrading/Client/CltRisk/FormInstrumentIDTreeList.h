#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "RiskData.h"
#include "Tools.h"

namespace CltRisk {

	/// <summary>
	/// Summary for FormInstrumentIDTreeList
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormInstrumentIDTreeList : public AnywndComboBox::CAnywndComboBoxControlContainter
	{
	public:
		FormInstrumentIDTreeList(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			LoadInstrumentTree();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormInstrumentIDTreeList()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ComboBox^  comboRelate;
	private: System::Windows::Forms::TreeView^  treeInstrument;
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
		};
		void Clear() {
			comboRelate->Text = Tools::string2String("");
			for(int i=0; i<treeInstrument->Nodes->Count; i++) {
				treeInstrument->Nodes[i]->Checked = false;
				UncheckTreeNode(treeInstrument->Nodes[i]);
			}
		}
		bool GetSelectID(set<string>& setInstrumentID) {
			setInstrumentID.clear();
			TreeNode^ node;
			bool bHasCheckSub = false;
			for(int i=0; i<treeInstrument->Nodes->Count; i++) {
				node = treeInstrument->Nodes[i];
				if(node->Checked) {
					const char* pStr = (const char*)(
							Runtime::InteropServices::Marshal::
							StringToHGlobalAnsi(node->Text)).ToPointer();
					setInstrumentID.insert(string(pStr));
				}
				else {
					bHasCheckSub = false;
					for(int k=0; k<node->Nodes->Count; k++) {
						if(node->Nodes[k]->Checked) {
							const char* pStr = (const char*)(
									Runtime::InteropServices::Marshal::
									StringToHGlobalAnsi(node->Nodes[k]->Text)).ToPointer();
							setInstrumentID.insert(string(pStr));
							bHasCheckSub = true;
						}
					}
				}
			}
			return setInstrumentID.size()>0;
		}
		bool GetTreeText(std::string& strInstrumentID) {

            TreeNode^ node_root = treeInstrument->Nodes[0];
            if (node_root->Checked == true)
            {
                for each (TreeNode^ node in node_root->Nodes)
                {
                    if (node->Checked == true)
                    {
                        const char* pStr = (const char*)(
                            Runtime::InteropServices::Marshal::
                            StringToHGlobalAnsi(node->Text)).ToPointer();
                        strInstrumentID += string(pStr);
                        strInstrumentID += ";";
                    }
                }
            }
            if (node_root->Checked == false)
            {
                for each (TreeNode^ node_1 in node_root->Nodes)
                {
                    if(node_1->Checked)
                    {
                        const char* pStr = (const char*)(
                            Runtime::InteropServices::Marshal::
                            StringToHGlobalAnsi(node_1->Text)).ToPointer();
                        strInstrumentID += string(pStr);
                        strInstrumentID += ";";
                    }
                }
            }

			return true;
		}

        void SetCheckTreeNode(String^ InstID) 
        {
            for(int i=0; i<treeInstrument->Nodes->Count; i++) 
                SetCheckTreeNode2(treeInstrument->Nodes[i],InstID);

            //如果子节点全部选中，父节点也自动选中
            TreeNode^ root=treeInstrument->Nodes[0];
            if(GetCheckTreeNode(root))
                root->Checked = true;
        }
        void SetCheckTreeNode2(TreeNode^ node,String^ InstID)
        {
            if(InstID->CompareTo(node->Text)==0)
                node->Checked = true;

            for(int i=0; i<node->Nodes->Count; i++) 
                SetCheckTreeNode2(node->Nodes[i],InstID);
        }
	public:
		void LoadInstrumentTree() {
			vector<SProduct> vecProducts;
			CRiskData::GetInstance()->GetInstrumentStyle(vecProducts);
			
			TreeNode^ node;
			node = treeInstrument->Nodes->Add("全部", "全部");

			TreeNode^ subNode;
			vector<SProduct>::iterator it;
			for(it = vecProducts.begin(); it != vecProducts.end(); it++) {
				String^ strText = Tools::string2String(it->ProductID);
				subNode=node->Nodes->Add(strText, strText);
			}
		}

    private: bool GetCheckTreeNode(TreeNode^ node)
             {                 
                 for(int i=0; i<node->Nodes->Count; i++) 
                 {
                     if(node->Nodes[i]->Checked == false) 
                         return false;

                     GetCheckTreeNode(node->Nodes[i]);
                 }

                 return true;
             }

	private: void UncheckTreeNode(TreeNode^ node) {
				 node->Checked = false;
				 for(int i=0; i<node->Nodes->Count; i++) {
					 UncheckTreeNode(node->Nodes[i]);
				 }
			 }

     private: void SetNodeCheckStatus(TreeNode^ tn, bool Checked)
     {
         if (tn == nullptr) return;

         // 设置子节点状态
         for each (TreeNode^ tnChild in tn->Nodes)
         {
             tnChild->Checked = Checked;

             SetNodeCheckStatus(tnChild, Checked);
         }
         // 设置父节点状态
         TreeNode^ tnParent = tn;
         int nNodeCount = 0;

         while (tnParent->Parent != nullptr)
         {
             tnParent = (TreeNode^)(tnParent->Parent);
             nNodeCount = 0;
             for each (TreeNode^ tnTemp in tnParent->Nodes)
             {
                 if (tnTemp->Checked == Checked)
                 {
                     nNodeCount++;
                 }
             }

             if (nNodeCount == tnParent->Nodes->Count)
             {
                 tnParent->Checked = Checked;
             }
             else
             {
                 tnParent->Checked = false;
             }
         }
     }



#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->treeInstrument = (gcnew System::Windows::Forms::TreeView());
            this->SuspendLayout();
            // 
            // treeInstrument
            // 
            this->treeInstrument->BorderStyle = System::Windows::Forms::BorderStyle::None;
            this->treeInstrument->CheckBoxes = true;
            this->treeInstrument->FullRowSelect = true;
            this->treeInstrument->HideSelection = false;
            this->treeInstrument->Location = System::Drawing::Point(0, 0);
            this->treeInstrument->Name = L"treeInstrument";
            this->treeInstrument->Size = System::Drawing::Size(160, 238);
            this->treeInstrument->TabIndex = 0;
            this->treeInstrument->AfterCheck += gcnew System::Windows::Forms::TreeViewEventHandler(this, &FormInstrumentIDTreeList::treeInstrument_AfterCheck);
            // 
            // FormInstrumentIDTreeList
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->Controls->Add(this->treeInstrument);
            this->Margin = System::Windows::Forms::Padding(0);
            this->Name = L"FormInstrumentIDTreeList";
            this->Size = System::Drawing::Size(160, 238);
            this->Load += gcnew System::EventHandler(this, &FormInstrumentIDTreeList::CInstrumentIDTreeListDlg_Load);
            this->ResumeLayout(false);

        }

#pragma endregion
private: System::Void CInstrumentIDTreeListDlg_Load(System::Object^  sender, System::EventArgs^  e) {
			 //Rectangle rect = this->DisplayRectangle;
			 treeInstrument->Location = Point(0, 0);
			 //treeInstrument->Width = rect.Width;
			 //treeInstrument->Height = rect.Height;

			 if(treeInstrument->Nodes->Count>0)
				treeInstrument->Nodes[0]->Expand();
		 }
public: virtual Void ProcessData() override
		{
			 std::string strText;
			 GetTreeText(strText);
			 comboRelate->Text = Tools::string2String(strText.c_str());
		 }
private: System::Void treeInstrument_AfterCheck(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
             if (e->Action != TreeViewAction::Unknown)
             {
                 SetNodeCheckStatus(e->Node, e->Node->Checked);
             }
         }
};
}

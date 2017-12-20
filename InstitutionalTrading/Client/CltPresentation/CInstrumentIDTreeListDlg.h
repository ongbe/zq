#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include "CDataInfo.h"
#include "Tools.h"

namespace CltPresentation {

	/// <summary>
	/// Summary for CInstrumentIDTreeListDlg
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CInstrumentIDTreeListDlg : public AnywndComboBox::CAnywndComboBoxControlContainter
	{
	public:
		CInstrumentIDTreeListDlg(void)
		{
			m_pdataInfo = CDataInfo::NewInstance();
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
		~CInstrumentIDTreeListDlg()
		{
			if (components)
			{
				delete components;
			}
		}
	private: CDataInfo* m_pdataInfo;
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
			TreeNode^ node;
			bool bHasCheckSub = false;
			for(int i=0; i<treeInstrument->Nodes->Count; i++) {
				node = treeInstrument->Nodes[i];
				if(node->Checked) {
					const char* pStr = (const char*)(
							Runtime::InteropServices::Marshal::
							StringToHGlobalAnsi(node->Text)).ToPointer();
					strInstrumentID += string(pStr);
					strInstrumentID += ", ";
				}
				else {
					bHasCheckSub = false;
					for(int k=0; k<node->Nodes->Count; k++) {
						if(node->Nodes[k]->Checked) {
							const char* pStr = (const char*)(
									Runtime::InteropServices::Marshal::
									StringToHGlobalAnsi(node->Nodes[k]->Text)).ToPointer();
							strInstrumentID += string(pStr);
							strInstrumentID += ", ";
						}
					}
				}
			}
			return true;
		}

	private:
		void LoadInstrumentTree() {
		

			set<string> setProducts;
			m_pdataInfo->GetAllProducts(setProducts);
			
			TreeNode^ node;
			set<string>::iterator it;
			for(it = setProducts.begin(); it != setProducts.end(); it++) {
				String^ strText = Tools::string2String((*it).c_str());
				node = treeInstrument->Nodes->Add(strText, strText);				
			}
#if 0
			node = treeInstrument->Nodes->Add("cu", "cu");
			node->Nodes->Add("cu1301", "cu1301");
			node->Nodes->Add("cu1302", "cu1302");
			node->Nodes->Add("cu1303", "cu1303");
			node->Nodes->Add("cu1301", "cu1304");
			node->Nodes->Add("cu1302", "cu1305");
			node->Nodes->Add("cu1303", "cu1306");
			node->Nodes->Add("cu1301", "cu1307");
			node->Nodes->Add("cu1302", "cu1308");
			node->Nodes->Add("cu1303", "cu1309");
			node->Nodes->Add("cu1301", "cu1310");
			node->Nodes->Add("cu1302", "cu1311");
			node->Nodes->Add("cu1303", "cu1312");
			node = treeInstrument->Nodes->Add("IF", "IF");
			node->Nodes->Add("IF1301", "IF1301");
			node->Nodes->Add("IF1302", "IF1302");
			node->Nodes->Add("IF1303", "IF1303");
			node->Nodes->Add("IF1303", "IF1306");
			node = treeInstrument->Nodes->Add("CF", "CF");
			node->Nodes->Add("IF1301", "CF301");
			node->Nodes->Add("IF1302", "CF303");
			node->Nodes->Add("IF1303", "CF305");
			node->Nodes->Add("IF1303", "CF307");
			node->Nodes->Add("IF1303", "CF309");
			node->Nodes->Add("IF1303", "CF311");
#endif
		}

	private: void UncheckTreeNode(TreeNode^ node) {
				 node->Checked = false;
				 for(int i=0; i<node->Nodes->Count; i++) {
					 UncheckTreeNode(node->Nodes[i]);
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
			// 
			// CInstrumentIDTreeListDlg
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->treeInstrument);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"CInstrumentIDTreeListDlg";
			this->Size = System::Drawing::Size(160, 238);
			this->Load += gcnew System::EventHandler(this, &CInstrumentIDTreeListDlg::CInstrumentIDTreeListDlg_Load);
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
};
}

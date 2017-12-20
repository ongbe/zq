#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#include <set>
#include <map>
#include "Tools.h"
#include "CDataInfo.h"
#include "CFinancialProductIDListDlgData.h"

namespace CltPresentation {

	/// <summary>
	/// Summary for CFinancialProductIDListDlg
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CFinancialProductIDListDlg : public AnywndComboBox::CAnywndComboBoxControlContainter
	{
	public:
		CFinancialProductIDListDlg(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			save = CFinancialProductIDListDlgData::NewInstance();
			m_pdataInfo = CDataInfo::NewInstance();
			LoadAllFinanProduct();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CFinancialProductIDListDlg()
		{
			if (components)
			{
				delete components;
			}
			CFinancialProductIDListDlgData::ReleaseInstance(save);
		}

	private: CDataInfo* m_pdataInfo;
	private: CFinancialProductIDListDlgData* save;
	private: System::Windows::Forms::ComboBox^  comboRelate;
	private: System::Windows::Forms::ListView^  listFinacialProduct;
	private: System::Windows::Forms::ColumnHeader^  col1;



	protected: 

	protected: 

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
			UncheckList();
		}
		//void ResetFinaProduct(std::map<int, FinancialProduct> mapFinaProduct, bool bDel) {
		//	SaveCheckedToData();
		//	if(bDel) {
		//		std::map<int, FinancialProduct>::iterator it;
		//		for(it = mapFinaProduct.begin(); 
		//				it != mapFinaProduct.end(); it++) {
		//			save->mapFinaProduct.erase(it->first);
		//		}
		//	}
		//	else {
		//		CFinancialProductIDListDlgData::CData data;
		//		std::map<int, FinancialProduct>::iterator itIns;
		//		for(itIns = mapFinaProduct.begin(); 
		//				itIns != mapFinaProduct.end(); itIns++) {
		//			data.nID = itIns->first;
		//			data.data = itIns->second;
		//			save->mapFinaProduct.insert(
		//					std::make_pair(itIns->first, data));
		//		}
		//	}
		//	
		//	std::map<int, CFinancialProductIDListDlgData::CData>::iterator it;
		//	listFinacialProduct->Items->Clear();
		//	for(it = save->mapFinaProduct.begin(); 
		//			it != save->mapFinaProduct.end(); it++) {
		//		String^ strItem = gcnew String(it->second.data.szName);
		//		listFinacialProduct->Items->Add(strItem, it->second.bSelect);
		//	}
		//}

		void ResetFinaProduct(std::map<int, FinancialProduct> mapFinaProduct, bool bDelAll) {
			SaveCheckedToData();
			if(bDelAll) {
				save->mapFinaProduct.clear();
			}
			std::set<int> setDel;
			std::map<int, CFinancialProductIDListDlgData::CData>::iterator it;
			for(it = save->mapFinaProduct.begin(); 
					it != save->mapFinaProduct.end(); it++) {
				if(mapFinaProduct.find(it->first)==mapFinaProduct.end())
					setDel.insert(it->first);
			}

			std::set<int>::iterator itDel;
			for(itDel = setDel.begin(); itDel != setDel.end(); itDel++) {
				save->mapFinaProduct.erase(*itDel);
			}
			
			CFinancialProductIDListDlgData::CData data;
			std::map<int, FinancialProduct>::iterator itIns;
			for(itIns = mapFinaProduct.begin(); 
					itIns != mapFinaProduct.end(); itIns++) {
				data.nID = itIns->first;
				data.data = itIns->second;
				save->mapFinaProduct.insert(
						std::make_pair(itIns->first, data));
			}
			
			listFinacialProduct->Items->Clear();
			for(it = save->mapFinaProduct.begin(); 
					it != save->mapFinaProduct.end(); it++) {
				ListViewItem^ item = gcnew ListViewItem;
				item->Text = gcnew String(it->second.data.szName);
				int^ nID = it->first;
				item->Name = nID->ToString();
				item->Checked = it->second.bSelect;
				listFinacialProduct->Items->Add(item);
			}
			Clear();
		}
		bool GetSelectID(std::set<int>& setProductID) {
			SaveCheckedToData();
			std::map<int, CFinancialProductIDListDlgData::CData>::iterator it
					= save->mapFinaProduct.begin();
			for(; it != save->mapFinaProduct.end(); it++) {
				if(it->second.bSelect)
					setProductID.insert(it->first);
			}
			if(setProductID.size()==0) {
				it = save->mapFinaProduct.begin();
				for(; it != save->mapFinaProduct.end(); it++) {
					setProductID.insert(it->first);
				}
			}
			return setProductID.size()>0;
		}
		void LoadAllFinanProduct() {
	

			MAPFinancialProduct mapFinanProduct;
			m_pdataInfo->GetFinancialProduct(mapFinanProduct);

		
			if(mapFinanProduct.size()>0) {
				CFinancialProductIDListDlgData::CData data;			
				MAPFinancialProduct::iterator itInfo;
				for(itInfo = mapFinanProduct.begin(); 
						itInfo != mapFinanProduct.end(); itInfo++) {
					//System::Diagnostics::Debug::Print("查询到的产品编号 %d\n", *itset);
						data.nID = itInfo->first;
						data.data = itInfo->second;
						save->mapFinaProduct.insert(
								std::make_pair(itInfo->first, data));
					
				}
			}

			std::map<int, CFinancialProductIDListDlgData::CData>::iterator it;

			listFinacialProduct->Items->Clear();
			for(it = save->mapFinaProduct.begin(); 
					it != save->mapFinaProduct.end(); it++) {
				ListViewItem^ item = gcnew ListViewItem;
				item->Text = gcnew String(it->second.data.szName);
				int^ nID = it->first;
				item->Name = nID->ToString();
				item->Checked = it->second.bSelect;
				listFinacialProduct->Items->Add(item);
			}
		}

	private: 
		void SaveCheckedToData() {
			int nID = 0;
			 for(int i=0; i<listFinacialProduct->Items->Count; i++) {
				std::string strKey = Tools::String2string(listFinacialProduct->Items[i]->Name);
				nID = atoi(strKey.c_str());
				std::map<int, CFinancialProductIDListDlgData::CData>::iterator it = 
						save->mapFinaProduct.find(nID);
				if(it != save->mapFinaProduct.end()) {
					it->second.bSelect = listFinacialProduct->Items[i]->Checked;
				}
			}
		}
	private: bool HasItemChecked() {
				 bool bNoChecked = true;
				 for(int i=0; i<listFinacialProduct->Items->Count; i++) {
					 if(listFinacialProduct->Items[i]->Checked) {
						 bNoChecked = false;
					 }
				 }
				 return bNoChecked;
			 }

	private: void GetItemText(std::string& strText, bool bNoChecked) {
				 for(int i=0; i<listFinacialProduct->Items->Count; i++) {
					 if(bNoChecked || (!bNoChecked && listFinacialProduct->Items[i]->Checked)) {
						String^ strKey = listFinacialProduct->Items[i]->Text;
						strText += Tools::String2string(strKey);
						strText += ", ";
					 }
				 }
			 }
	private: void UncheckList() {
				 for(int i=0; i<listFinacialProduct->Items->Count; i++) {
					 listFinacialProduct->Items[i]->Checked = false;
				 }
			 }

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->listFinacialProduct = (gcnew System::Windows::Forms::ListView());
			this->col1 = (gcnew System::Windows::Forms::ColumnHeader());
			this->SuspendLayout();
			// 
			// listFinacialProduct
			// 
			this->listFinacialProduct->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->listFinacialProduct->CheckBoxes = true;
			this->listFinacialProduct->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(1) {this->col1});
			this->listFinacialProduct->FullRowSelect = true;
			this->listFinacialProduct->HideSelection = false;
			this->listFinacialProduct->Location = System::Drawing::Point(0, -1);
			this->listFinacialProduct->Name = L"listFinacialProduct";
			this->listFinacialProduct->Size = System::Drawing::Size(160, 238);
			this->listFinacialProduct->TabIndex = 0;
			this->listFinacialProduct->UseCompatibleStateImageBehavior = false;
			this->listFinacialProduct->View = System::Windows::Forms::View::List;
			this->listFinacialProduct->Click += gcnew System::EventHandler(this, &CFinancialProductIDListDlg::listFinacialProduct_Click);
			// 
			// col1
			// 
			this->col1->Width = 150;
			// 
			// CFinancialProductIDListDlg
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->listFinacialProduct);
			this->Margin = System::Windows::Forms::Padding(0);
			this->Name = L"CFinancialProductIDListDlg";
			this->Size = System::Drawing::Size(160, 238);
			this->Load += gcnew System::EventHandler(this, &CFinancialProductIDListDlg::CFinancialProductIDListDlg_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void CFinancialProductIDListDlg_Load(System::Object^  sender, System::EventArgs^  e) {
			 //Rectangle rect = this->DisplayRectangle;
			 listFinacialProduct->Location = Point(0, 0);
			 //listFinacialProduct->Width = rect.Width;
			 //listFinacialProduct->Height = rect.Height;
		 }
private: System::Void listFinacialProduct_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
public: virtual Void ProcessData() override
		{
			std::string strText;
			bool bNoChecked = HasItemChecked();
			if(!bNoChecked)
			{
				GetItemText(strText, bNoChecked);
			}
			comboRelate->Text = Tools::string2String(strText.c_str());

		}
};
}

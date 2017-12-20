#pragma once

#include <cliext/map>
#include <cliext/vector>
#include "Tools.h"

#include "RiskData.h"


extern vector<string> g_vecInstSelected;

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltRisk {

	/// <summary>
	/// Summary for FormAutoSelectInstSetup
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class FormAutoSelectInstSetup : public System::Windows::Forms::Form
	{
	public:
        FormAutoSelectInstSetup()
        {
            InitializeComponent();
            //
            //TODO: Add the constructor code here
            //
        }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormAutoSelectInstSetup()
		{
			if (components)
			{
				delete components;
			}
		}

    private: System::Windows::Forms::ComboBox^  comboBox_Exchange;
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::DataGridView^  dataGridView_Exchange;
//     private: System::Windows::Forms::DataGridView^  dataGridView_AutoSelectedInst;
//     private: System::Windows::Forms::Button^  button_Add;
//     private: System::Windows::Forms::Button^  button_Del;
    private: System::Windows::Forms::Button^  button_OK;
    private: System::Windows::Forms::Button^  button_Cancel;
    private: System::Windows::Forms::DataGridViewCheckBoxColumn^  Column1;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;


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
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
//             System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle2 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            this->comboBox_Exchange = (gcnew System::Windows::Forms::ComboBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->dataGridView_Exchange = (gcnew System::Windows::Forms::DataGridView());
            this->Column1 = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
            this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
//             this->dataGridView_AutoSelectedInst = (gcnew System::Windows::Forms::DataGridView());
            this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
//             this->button_Add = (gcnew System::Windows::Forms::Button());
//             this->button_Del = (gcnew System::Windows::Forms::Button());
            this->button_OK = (gcnew System::Windows::Forms::Button());
            this->button_Cancel = (gcnew System::Windows::Forms::Button());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_Exchange))->BeginInit();
//             (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_AutoSelectedInst))->BeginInit();
            this->SuspendLayout();
            // 
            // comboBox_Exchange
            // 
            this->comboBox_Exchange->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
            this->comboBox_Exchange->FormattingEnabled = true;
            this->comboBox_Exchange->Location = System::Drawing::Point(85, 21);
            this->comboBox_Exchange->Name = L"comboBox_Exchange";
            this->comboBox_Exchange->Size = System::Drawing::Size(121, 20);
            this->comboBox_Exchange->TabIndex = 0;
            this->comboBox_Exchange->SelectedIndexChanged += gcnew System::EventHandler(this, &FormAutoSelectInstSetup::comboBox_Exchange_SelectedIndexChanged);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(34, 25);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(41, 12);
            this->label1->TabIndex = 1;
            this->label1->Text = L"交易所";
            // 
            // dataGridView_Exchange
            // 
            this->dataGridView_Exchange->AllowUserToAddRows = false;
            this->dataGridView_Exchange->AllowUserToResizeColumns = false;
            this->dataGridView_Exchange->AllowUserToResizeRows = false;
            this->dataGridView_Exchange->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
            dataGridViewCellStyle1->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
            dataGridViewCellStyle1->BackColor = System::Drawing::SystemColors::Control;
            dataGridViewCellStyle1->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            dataGridViewCellStyle1->ForeColor = System::Drawing::SystemColors::WindowText;
            dataGridViewCellStyle1->SelectionBackColor = System::Drawing::SystemColors::Highlight;
            dataGridViewCellStyle1->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
            dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
            this->dataGridView_Exchange->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle1;
            this->dataGridView_Exchange->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridView_Exchange->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {this->Column1, 
                this->Column2});
            this->dataGridView_Exchange->Location = System::Drawing::Point(33, 57);
            this->dataGridView_Exchange->Name = L"dataGridView_Exchange";
            this->dataGridView_Exchange->RowHeadersVisible = false;
            this->dataGridView_Exchange->RowTemplate->Height = 23;
            this->dataGridView_Exchange->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
            this->dataGridView_Exchange->Size = System::Drawing::Size(199, 324);
            this->dataGridView_Exchange->TabIndex = 2;
            // 
            // Column1
            // 
            this->Column1->HeaderText = L"";
            this->Column1->Name = L"Column1";
            this->Column1->Resizable = System::Windows::Forms::DataGridViewTriState::True;
            this->Column1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Automatic;
            // 
            // Column2
            // 
            this->Column2->HeaderText = L"合约代码";
            this->Column2->Name = L"Column2";
            this->Column2->Resizable = System::Windows::Forms::DataGridViewTriState::True;
            this->Column2->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // dataGridView_AutoSelectedInst
            // 
//             this->dataGridView_AutoSelectedInst->AllowUserToAddRows = false;
//             this->dataGridView_AutoSelectedInst->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::Fill;
//             dataGridViewCellStyle2->Alignment = System::Windows::Forms::DataGridViewContentAlignment::MiddleCenter;
//             dataGridViewCellStyle2->BackColor = System::Drawing::SystemColors::Control;
//             dataGridViewCellStyle2->Font = (gcnew System::Drawing::Font(L"SimSun", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
//                 static_cast<System::Byte>(134)));
//             dataGridViewCellStyle2->ForeColor = System::Drawing::SystemColors::WindowText;
//             dataGridViewCellStyle2->SelectionBackColor = System::Drawing::SystemColors::Highlight;
//             dataGridViewCellStyle2->SelectionForeColor = System::Drawing::SystemColors::HighlightText;
//             dataGridViewCellStyle2->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
//             this->dataGridView_AutoSelectedInst->ColumnHeadersDefaultCellStyle = dataGridViewCellStyle2;
//             this->dataGridView_AutoSelectedInst->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
//             this->dataGridView_AutoSelectedInst->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(1) {this->Column3});
//             this->dataGridView_AutoSelectedInst->Location = System::Drawing::Point(366, 57);
//             this->dataGridView_AutoSelectedInst->Name = L"dataGridView_AutoSelectedInst";
//             this->dataGridView_AutoSelectedInst->RowHeadersVisible = false;
//             this->dataGridView_AutoSelectedInst->RowTemplate->Height = 23;
//             this->dataGridView_AutoSelectedInst->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
//             this->dataGridView_AutoSelectedInst->Size = System::Drawing::Size(199, 324);
//             this->dataGridView_AutoSelectedInst->TabIndex = 3;
            // 
            // Column3
            // 
            this->Column3->HeaderText = L"自选合约代码";
            this->Column3->Name = L"Column3";
            this->Column3->ReadOnly = true;
            // 
            // button_Add
            // 
//             this->button_Add->Location = System::Drawing::Point(259, 167);
//             this->button_Add->Name = L"button_Add";
//             this->button_Add->Size = System::Drawing::Size(75, 23);
//             this->button_Add->TabIndex = 1;
//             this->button_Add->Text = L"添加";
//             this->button_Add->UseVisualStyleBackColor = true;
//             this->button_Add->Click += gcnew System::EventHandler(this, &FormAutoSelectInstSetup::button_Add_Click);
            // 
            // button_Del
            // 
//             this->button_Del->Location = System::Drawing::Point(259, 252);
//             this->button_Del->Name = L"button_Del";
//             this->button_Del->Size = System::Drawing::Size(75, 23);
//             this->button_Del->TabIndex = 2;
//             this->button_Del->Text = L"删除";
//             this->button_Del->UseVisualStyleBackColor = true;
//             this->button_Del->Click += gcnew System::EventHandler(this, &FormAutoSelectInstSetup::button_Del_Click);
            // 
            // button_OK
            // 
            this->button_OK->Location = System::Drawing::Point(44, 418);
            this->button_OK->Name = L"button_OK";
            this->button_OK->Size = System::Drawing::Size(75, 23);
            this->button_OK->TabIndex = 5;
            this->button_OK->Text = L"确定";
            this->button_OK->UseVisualStyleBackColor = true;
            this->button_OK->Click += gcnew System::EventHandler(this, &FormAutoSelectInstSetup::button_OK_Click);
            // 
            // button_Cancel
            // 
            this->button_Cancel->Location = System::Drawing::Point(150, 417);
            this->button_Cancel->Name = L"button_Cancel";
            this->button_Cancel->Size = System::Drawing::Size(75, 23);
            this->button_Cancel->TabIndex = 6;
            this->button_Cancel->Text = L"取消";
            this->button_Cancel->UseVisualStyleBackColor = true;
            this->button_Cancel->Click += gcnew System::EventHandler(this, &FormAutoSelectInstSetup::button_Cancel_Click);
            // 
            // FormAutoSelectInstSetup
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(257, 482);
            this->Controls->Add(this->button_Cancel);
            this->Controls->Add(this->button_OK);
//             this->Controls->Add(this->button_Del);
//             this->Controls->Add(this->button_Add);
//             this->Controls->Add(this->dataGridView_AutoSelectedInst);
            this->Controls->Add(this->dataGridView_Exchange);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->comboBox_Exchange);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"FormAutoSelectInstSetup";
            this->Text = L"自选合约设置";
            this->Load += gcnew System::EventHandler(this, &FormAutoSelectInstSetup::FormAutoSelectInstSetup_Load);
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_Exchange))->EndInit();
//             (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_AutoSelectedInst))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
public:
    String^ GetSelectExchange()
    {
        return comboBox_Exchange->Text;
    }
    bool GetSelectInst(vector<string>& vecInstSelected)
    {
        vecInstSelected = g_vecInstSelected;
        return ((int)(g_vecInstSelected.size())>0)?true:false;
    }

private:
        void SetData()
        {
            map<string,vector<string>> mapExchange2Inst;
            CRiskData::GetInstance()->GetExchange2Instrument(mapExchange2Inst);
            if(mapExchange2Inst.empty())
                return;

            for (map<string,vector<string>>::iterator it=mapExchange2Inst.begin();it!=mapExchange2Inst.end();it++)
            {
                string szExchange = it->first;
                comboBox_Exchange->Items->Add(Tools::string2String(szExchange.c_str()));
            }

            comboBox_Exchange->SelectedIndex = 0;
        }

//         bool TransferSLT2cli(map<string,vector<string>> mapSrc,cliext::map<String^,cliext::vector<String^>^>^ climapDest)
//         {
//             if(!mapSrc.empty())
//             {
//                 for (map<string,vector<string>>::iterator it=mapSrc.begin();it!=mapSrc.end();it++)
//                 {
//                     String^ szExchange = Tools::string2String(it->first.c_str());
//                     cliext::vector<String^>^ clivecInstrument = gcnew cliext::vector<String^>();
//                     for (vector<string>::iterator it2=it->second.begin();it2!=it->second.end();it2++)
//                         clivecInstrument->push_back(Tools::string2String((*it2).c_str()));
// 
//                     climapDest->insert(cliext::make_pair(szExchange, clivecInstrument));
//                 }
// 
//                 return true;
//             }
// 
//             return false;
//         }


        bool GetSelectedInstFromCfg(string szExchange, string szInst)
        {
            map<string,vector<string>> mapExchange2InstFromCfg = CRiskData::GetInstance()->GetConfigFile(XML_TYPE_INST);
            if(mapExchange2InstFromCfg.empty())
                return false;

            map<string,vector<string>>::iterator it = mapExchange2InstFromCfg.find(szExchange);
            if(it != mapExchange2InstFromCfg.end())
            {
                vector<string> vecInst = it->second;
                for (vector<string>::iterator it2=vecInst.begin();it2!=vecInst.end();it2++)
                {
                    string szInstSrc = *it2;
                    if(szInst == szInstSrc)
                        return true;
                }
            }

            return false;
        }

private: System::Void FormAutoSelectInstSetup_Load(System::Object^  sender, System::EventArgs^  e) {
             comboBox_Exchange->Items->Clear();
             dataGridView_Exchange->Rows->Clear();
//              dataGridView_AutoSelectedInst->Rows->Clear();
             
             SetData();

             this->CenterToParent();
         }
private: System::Void comboBox_Exchange_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {

             dataGridView_Exchange->Rows->Clear();

             map<string,vector<string>> mapExchange2Inst;
             CRiskData::GetInstance()->GetExchange2Instrument(mapExchange2Inst);
             map<string,vector<string>>::iterator it = mapExchange2Inst.find(Tools::String2string(comboBox_Exchange->Text));
             if(it != mapExchange2Inst.end())
             {
                 vector<string> vecInst = it->second;
                 for (vector<string>::iterator it2=vecInst.begin();it2!=vecInst.end();it2++)
                 {
                     string szInst = *it2;
                     String^ strSelected = GetSelectedInstFromCfg(Tools::String2string(comboBox_Exchange->Text),szInst)?"true":"false"; 
                     array<String^>^ newRow = {strSelected,Tools::string2String(szInst.c_str())};
                     int nIndex = dataGridView_Exchange->Rows->Add(newRow);
                 }
             }
         }
// private: System::Void button_Add_Click(System::Object^  sender, System::EventArgs^  e) {
//              for each (DataGridViewRow^ row in dataGridView_Exchange->SelectedRows) 
//              {
//                  String^ strName = row->Cells[0]->Value->ToString();  
//                  dataGridView_AutoSelectedInst->Rows->Add(strName);
//              } 
//          }
// private: System::Void button_Del_Click(System::Object^  sender, System::EventArgs^  e) {
//              for each (DataGridViewRow^ row in dataGridView_Exchange->SelectedRows) 
//              {
//                  dataGridView_AutoSelectedInst->Rows->RemoveAt(row->Index);
//              } 
//          }
private: System::Void button_OK_Click(System::Object^  sender, System::EventArgs^  e) {
             g_vecInstSelected.clear();
             int count = dataGridView_Exchange->Rows->Count;
             for (int i = 0; i < count; i++) 
             {
                 DataGridViewCheckBoxCell^ checkCell = (DataGridViewCheckBoxCell^)dataGridView_Exchange->Rows[i]->Cells[0]; 
                 Boolean flag = Convert::ToBoolean(checkCell->Value); 
                 if (flag == true)      
                 {                         
                     String^ strName = dataGridView_Exchange->Rows[i]->Cells[1]->Value->ToString();  
                     g_vecInstSelected.push_back(Tools::String2string(strName)); 
                 }                
                 else            
                     continue;   
             } 

             this->DialogResult = ::DialogResult::OK;
             this->Close();
         }
private: System::Void button_Cancel_Click(System::Object^  sender, System::EventArgs^  e) {             
             this->Close();
         }

};
}

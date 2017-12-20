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

            m_mapSelectedInstFromLocal = gcnew cliext::map<String^,cliext::vector<String^>^>();
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
    private: System::Windows::Forms::Button^  button_OK;
    private: System::Windows::Forms::Button^  button_Cancel;


    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column3;
    private: System::Windows::Forms::DataGridViewCheckBoxColumn^  Column1;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column2;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components; 


        cliext::map<String^,cliext::vector<String^>^> m_mapSelectedInstFromLocal;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            System::Windows::Forms::DataGridViewCellStyle^  dataGridViewCellStyle1 = (gcnew System::Windows::Forms::DataGridViewCellStyle());
            this->comboBox_Exchange = (gcnew System::Windows::Forms::ComboBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->dataGridView_Exchange = (gcnew System::Windows::Forms::DataGridView());
            this->Column3 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->button_OK = (gcnew System::Windows::Forms::Button());
            this->button_Cancel = (gcnew System::Windows::Forms::Button());
            this->Column1 = (gcnew System::Windows::Forms::DataGridViewCheckBoxColumn());
            this->Column2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_Exchange))->BeginInit();
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
            this->dataGridView_Exchange->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &FormAutoSelectInstSetup::dataGridView_Exchange_CellContentClick);
            // 
            // Column3
            // 
            this->Column3->HeaderText = L"自选合约代码";
            this->Column3->Name = L"Column3";
            this->Column3->ReadOnly = true;
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
            // Column1
            // 
            this->Column1->FalseValue = L"false";
            this->Column1->HeaderText = L"选择";
            this->Column1->Name = L"Column1";
            this->Column1->Resizable = System::Windows::Forms::DataGridViewTriState::True;
            this->Column1->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::Automatic;
            this->Column1->TrueValue = L"true";
            // 
            // Column2
            // 
            this->Column2->HeaderText = L"合约代码";
            this->Column2->Name = L"Column2";
            this->Column2->Resizable = System::Windows::Forms::DataGridViewTriState::True;
            this->Column2->SortMode = System::Windows::Forms::DataGridViewColumnSortMode::NotSortable;
            // 
            // FormAutoSelectInstSetup
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(257, 482);
            this->Controls->Add(this->button_Cancel);
            this->Controls->Add(this->button_OK);
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
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
public:
    String^ GetSelectExchange()
    {
        return comboBox_Exchange->Text;
    }
    bool GetSelectInst(std::map<string,std::vector<string>>& mapExchange2InstSelected)
    {
        return Tools::TransferSLT2cli(m_mapSelectedInstFromLocal,mapExchange2InstSelected);
    }

private:
        void SetData()
        {
            map<string,vector<string>> mapExchange2Inst;
            CRiskData::GetInstance()->GetExchange2Instrument(mapExchange2Inst);
            if(mapExchange2Inst.empty())
                return;

            map<string,vector<string>> mapAutoSelectedInst = CRiskData::GetInstance()->GetConfigFile(XML_TYPE_INST);
            Tools::TransferSLT2cli(mapAutoSelectedInst, m_mapSelectedInstFromLocal);

            for (map<string,vector<string>>::iterator it=mapExchange2Inst.begin();it!=mapExchange2Inst.end();it++)
            {
                string szExchange = it->first;
                comboBox_Exchange->Items->Add(Tools::string2String(szExchange.c_str()));
            }

            comboBox_Exchange->SelectedIndex = 0;
        }

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
        //当前内存选中保存
        bool GetSelectedInstFromLocal(String^ szExchange,String^ szInst)
        {            
            if(m_mapSelectedInstFromLocal.empty())
                return false;

            cliext::map<String^,cliext::vector<String^>^>::iterator it = m_mapSelectedInstFromLocal.find(szExchange);
            if (it!=m_mapSelectedInstFromLocal.end())
            {
                cliext::vector<String^>^ vecInst = it->second;
                for (cliext::vector<String^>::iterator it_inst=vecInst->begin();it_inst!=vecInst->end();it_inst++)
                {
                    String^ szInstSrc = *it_inst;
                    if(szInstSrc->CompareTo(szInst)==0)
                        return true;
                }
            }

            return false;
        }

private: System::Void FormAutoSelectInstSetup_Load(System::Object^  sender, System::EventArgs^  e) {
             comboBox_Exchange->Items->Clear();
             dataGridView_Exchange->Rows->Clear();
             m_mapSelectedInstFromLocal.clear();
             
             SetData();

             this->CenterToParent();
         }
private: System::Void comboBox_Exchange_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             dataGridView_Exchange->Rows->Clear();

             map<string,vector<string>> mapExchange2Inst;
             CRiskData::GetInstance()->GetExchange2Instrument(mapExchange2Inst);

             String^ strExchange = comboBox_Exchange->Text;
             map<string,vector<string>>::iterator it = mapExchange2Inst.find(Tools::String2string(strExchange));
             if(it != mapExchange2Inst.end())
             {
                 vector<string> vecInst = it->second;
                 for (vector<string>::iterator it_inst=vecInst.begin();it_inst!=vecInst.end();it_inst++)
                 {
                     string szInst = *it_inst;
                     String^ strSelected = GetSelectedInstFromLocal(strExchange,Tools::string2String(szInst.c_str()))?"true":"false"; 
                     array<String^>^ newRow = {strSelected,Tools::string2String(szInst.c_str())};
                     int nIndex = dataGridView_Exchange->Rows->Add(newRow);
                 }
             }
         }
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
private: System::Void dataGridView_Exchange_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
             if (e->ColumnIndex == 0 && e->RowIndex != -1)        
             {   
                 //获取控件的值                 
                 String^ strExchange = comboBox_Exchange->Text;
                 String^ strInst = dataGridView_Exchange->Rows[e->RowIndex]->Cells[1]->EditedFormattedValue->ToString();
                 bool bFlag = Convert::ToBoolean(dataGridView_Exchange->Rows[e->RowIndex]->Cells[0]->EditedFormattedValue); 
                 if(bFlag) 
                 {
                    cliext::map<String^,cliext::vector<String^>^>::iterator it = m_mapSelectedInstFromLocal.find(strExchange);
                    if(it!=m_mapSelectedInstFromLocal.end())
                    {
                        cliext::vector<String^>^% vecInst = it->second;
                        vecInst->push_back(strInst);
                    }
                    else
                    {
                        cliext::vector<String^>^ newVecInst = gcnew cliext::vector<String^>();
                        newVecInst->push_back(strInst);
                        m_mapSelectedInstFromLocal.insert(cliext::make_pair(strExchange,newVecInst));
                    }
                 }
                 else
                 {
                     cliext::map<String^,cliext::vector<String^>^>::iterator it = m_mapSelectedInstFromLocal.find(strExchange);
                     if(it!=m_mapSelectedInstFromLocal.end())
                     {
                         cliext::vector<String^>^% vecInst = it->second;
                         cliext::vector<String^>::iterator it_inst = vecInst->begin();
                         for(;it_inst != vecInst->end();++it_inst)
                         {
                             String^ szInstSrc = *it_inst;
                             if(szInstSrc->CompareTo(strInst)==0)
                             {
                                 it_inst = vecInst->erase(it_inst);        
                                 return;
                             }
                         }
                     }
                 }
             }
         }
};
}

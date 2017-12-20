#pragma once
#include "DataManageEx.h"
#include "Tools.h"
#include "..\CommonClasses\ConvEnum.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltStrategy {

	/// <summary>
	/// Summary for StrategeIndiConfig
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class StrategeIndiConfig : public System::Windows::Forms::Form
	{
	public:
		StrategeIndiConfig(int count,CStrategyDataEx*  pStrategyData)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_lstControls = gcnew List<Control^>();
			m_nInsCount = count;
			m_pDataInfo = CDataManageEx::NewInstance();
			m_StrategyIndicator = new StrategyIndicator;
			bFlag =true;//触发事件
			m_StrategyData = pStrategyData;
			m_bOprationType=true;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~StrategeIndiConfig()
		{
			if (components)
			{
				delete components;
			}
			CDataManageEx::DestroyInstance();
			if ( NULL != m_StrategyIndicator )
			{
				delete m_StrategyIndicator;
				m_StrategyIndicator = NULL;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
	private: bool             m_bOprationType;//新增参数true,修改参数false
	private: CStrategyDataEx*  m_StrategyData;
	private: bool bFlag;
	private: int m_nInsCount;
	private: CDataManageEx*     m_pDataInfo;
	private: StrategyIndicator*  m_StrategyIndicator;
	protected: List<Control^>^	m_lstControls;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  comboBox_indi;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox_name;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::ComboBox^  comboBox_ins;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label_describe;

	private: System::Windows::Forms::Button^  button_ok;
	private: System::Windows::Forms::Button^  button_cancle;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Panel^  panel1;







			 System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->comboBox_indi = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox_name = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->comboBox_ins = (gcnew System::Windows::Forms::ComboBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label_describe = (gcnew System::Windows::Forms::Label());
			this->button_ok = (gcnew System::Windows::Forms::Button());
			this->button_cancle = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(35, 28);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"指标：";
			// 
			// comboBox_indi
			// 
			this->comboBox_indi->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox_indi->FormattingEnabled = true;
			this->comboBox_indi->Location = System::Drawing::Point(82, 24);
			this->comboBox_indi->Name = L"comboBox_indi";
			this->comboBox_indi->Size = System::Drawing::Size(121, 20);
			this->comboBox_indi->TabIndex = 1;
			this->comboBox_indi->SelectedIndexChanged += gcnew System::EventHandler(this, &StrategeIndiConfig::comboBox1_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(267, 28);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(41, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"名称：";
			// 
			// textBox_name
			// 
			this->textBox_name->Location = System::Drawing::Point(314, 23);
			this->textBox_name->Name = L"textBox_name";
			this->textBox_name->Size = System::Drawing::Size(100, 21);
			this->textBox_name->TabIndex = 3;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(35, 59);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(41, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"合约：";
			// 
			// comboBox_ins
			// 
			this->comboBox_ins->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->comboBox_ins->FormattingEnabled = true;
			this->comboBox_ins->Location = System::Drawing::Point(82, 56);
			this->comboBox_ins->Name = L"comboBox_ins";
			this->comboBox_ins->Size = System::Drawing::Size(121, 20);
			this->comboBox_ins->TabIndex = 5;
			this->comboBox_ins->SelectedIndexChanged += gcnew System::EventHandler(this, &StrategeIndiConfig::comboBox2_SelectedIndexChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(35, 90);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(131, 12);
			this->label4->TabIndex = 6;
			this->label4->Text = L"指标父类： CIndicator";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(273, 90);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(65, 12);
			this->label5->TabIndex = 7;
			this->label5->Text = L"是否输出：";
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(345, 90);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(30, 16);
			this->checkBox1->TabIndex = 8;
			this->checkBox1->Text = L" ";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label_describe);
			this->groupBox1->Location = System::Drawing::Point(37, 126);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(382, 104);
			this->groupBox1->TabIndex = 9;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"指标描述";
			// 
			// label_describe
			// 
			this->label_describe->AutoSize = true;
			this->label_describe->Location = System::Drawing::Point(28, 26);
			this->label_describe->Name = L"label_describe";
			this->label_describe->Size = System::Drawing::Size(0, 12);
			this->label_describe->TabIndex = 0;
			// 
			// button_ok
			// 
			this->button_ok->Location = System::Drawing::Point(237, 450);
			this->button_ok->Name = L"button_ok";
			this->button_ok->Size = System::Drawing::Size(75, 23);
			this->button_ok->TabIndex = 11;
			this->button_ok->Text = L"确定";
			this->button_ok->UseVisualStyleBackColor = true;
			this->button_ok->Click += gcnew System::EventHandler(this, &StrategeIndiConfig::button_ok_Click);
			// 
			// button_cancle
			// 
			this->button_cancle->Location = System::Drawing::Point(339, 450);
			this->button_cancle->Name = L"button_cancle";
			this->button_cancle->Size = System::Drawing::Size(75, 23);
			this->button_cancle->TabIndex = 12;
			this->button_cancle->Text = L"取消";
			this->button_cancle->UseVisualStyleBackColor = true;
			this->button_cancle->Click += gcnew System::EventHandler(this, &StrategeIndiConfig::button_cancle_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(17, 27);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(41, 12);
			this->label6->TabIndex = 0;
			this->label6->Text = L"label6";
			this->label6->Visible = false;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(92, 26);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(41, 12);
			this->label7->TabIndex = 1;
			this->label7->Text = L"label7";
			this->label7->Visible = false;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(200, 26);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(41, 12);
			this->label9->TabIndex = 3;
			this->label9->Text = L"label9";
			this->label9->Visible = false;
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(248, 21);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 20);
			this->comboBox1->TabIndex = 4;
			this->comboBox1->Visible = false;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(19, 52);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(47, 12);
			this->label10->TabIndex = 5;
			this->label10->Text = L"label10";
			this->label10->Visible = false;
			// 
			// groupBox2
			// 
			this->groupBox2->BackColor = System::Drawing::SystemColors::Control;
			this->groupBox2->Controls->Add(this->panel1);
			this->groupBox2->Controls->Add(this->label10);
			this->groupBox2->Controls->Add(this->comboBox1);
			this->groupBox2->Controls->Add(this->label9);
			this->groupBox2->Controls->Add(this->label7);
			this->groupBox2->Controls->Add(this->label6);
			this->groupBox2->Location = System::Drawing::Point(37, 262);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(382, 156);
			this->groupBox2->TabIndex = 10;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"参数设置";
			// 
			// panel1
			// 
			this->panel1->AutoScroll = true;
			this->panel1->BackColor = System::Drawing::SystemColors::Control;
			this->panel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel1->Location = System::Drawing::Point(3, 17);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(376, 136);
			this->panel1->TabIndex = 6;
			// 
			// StrategeIndiConfig
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(465, 494);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->button_cancle);
			this->Controls->Add(this->button_ok);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->comboBox_ins);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->textBox_name);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboBox_indi);
			this->Controls->Add(this->label1);
			this->Name = L"StrategeIndiConfig";
			this->Text = L"指标配置";
			this->Load += gcnew System::EventHandler(this, &StrategeIndiConfig::StrategeIndiConfig_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public:StrategyIndicator* GetStrategyIndicator()
		   {

			   return m_StrategyIndicator;
		   }

	public: void SetStrategyIndiConfig(const StrategyIndicator& Para)
			{
				*m_StrategyIndicator = Para;
				m_bOprationType=false;
				this->textBox_name->Enabled = false;
			}

		   /*新建策略-》指标配置-》参数设置 中新增一行参数*/
	private: System::Void GenerateNewParaLine(IndicatorPara& filed,int Cnt)
			 {
				 char strtemp[32];
				
				 //参数名称
				 Label^ lbname = gcnew Label();
				 //Point^ p =      gcnew Point(17, 27);
				 lbname->AutoSize = true;
				 lbname->Location =   System::Drawing::Point(17, 27+30*Cnt);
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","label_name",Cnt );
				 lbname->Name = Tools::string2String(strtemp);
				 lbname->Size = System::Drawing::Size(41, 12);
				 lbname->TabIndex = Cnt+4*Cnt;
				 lbname->Text = Tools::string2String(filed.szName);
				 this->panel1->Controls->Add(lbname);
				 m_lstControls->Add(lbname);
				 lbname->BringToFront();

				 //参数类型
				 Label^ lbtype = gcnew Label();
				 lbtype->AutoSize = true;
				 lbtype->Location = System::Drawing::Point(92, 27+30*Cnt);
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","label_type",Cnt );
				 lbtype->Name = Tools::string2String(strtemp);
				 lbtype->Size = System::Drawing::Size(41, 12);
				 lbtype->TabIndex = Cnt+4*Cnt;
				  memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%s","类型：",IndiPara2String(filed.eType) );
				 lbtype->Text = Tools::string2String(strtemp);
				 m_lstControls->Add(lbtype);
				 this->panel1->Controls->Add(lbtype);

				 //参数值LABEL
				 Label^ lbvalue = gcnew Label();
				 lbvalue->AutoSize = true;
				 lbvalue->Location = System::Drawing::Point(200, 27+30*Cnt);
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","label_value",Cnt );
				 lbvalue->Name = Tools::string2String(strtemp);
				 lbvalue->Size = System::Drawing::Size(41, 12);
				 lbvalue->TabIndex = Cnt+4*Cnt;
				 lbvalue->Text =  L"值：";
				 this->panel1->Controls->Add(lbvalue);
				 m_lstControls->Add(lbvalue);

				 //参数值comobox
				 ComboBox^  comboBox_value = gcnew ComboBox();
				 comboBox_value->FormattingEnabled = true;
				 comboBox_value->Location = System::Drawing::Point(248, 21+30*Cnt);
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","comboBox_value",Cnt );
				 comboBox_value->Name =  Tools::string2String(strtemp);
				 comboBox_value->Size = System::Drawing::Size(121, 20);
				 comboBox_value->TabIndex = Cnt+4*Cnt;
				 this->panel1->Controls->Add(comboBox_value);
				 m_lstControls->Add(comboBox_value);

				 multimap<string,string> mapFiled;
				 m_StrategyData->GetParaType2NameMap(mapFiled);
				 int nNameCnt = mapFiled.count(IndiPara2String(filed.eType));
				 multimap<string,string>::iterator it = mapFiled.find(IndiPara2String(filed.eType));
				 ArrayList^ indiAL = gcnew ArrayList;
				 for ( int i = 0; i< nNameCnt; i++)
				 {					 
					 indiAL->Add( Tools::string2String(it->second.c_str() ));
					 it++;
				 }
				 comboBox_value->Items->AddRange(indiAL->ToArray());

			 }
	private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 std::map<std::string, IndicatorPara>    mapFiled;
				 string name = Tools::String2string(comboBox_indi->Text);
				 m_pDataInfo->GetIndiPara(name,mapFiled);
				 CIndicatorDataEx* filed =  new CIndicatorDataEx();
				 if (m_pDataInfo->GetIndicatorInfobyName(name,*filed) && filed->GetIndicatorDescribe() != "")
				 {
					 label_describe->Text = Tools::string2String(filed->GetIndicatorDescribe().c_str());
				 }

				 for each(Control^ item in m_lstControls)
				 {
					this->panel1->Controls->Remove(item);
				 }
				 m_lstControls->Clear();

                 filed->GetIndiPara(mapFiled);
				 std::map<std::string, IndicatorPara>::iterator it = mapFiled.begin();
				 for ( int i=0; it != mapFiled.end(); it++,i++)
				 {
					 IndicatorPara indiPara;
					 memset(&indiPara,0,sizeof(IndicatorPara));
					 
					 indiPara = it->second;

					 GenerateNewParaLine(indiPara,i);
				 }

				 
			 }
private: System::Void StrategeIndiConfig_Load(System::Object^  sender, System::EventArgs^  e) 
		 {
			 bFlag = false;
			 this->CenterToParent();
			 //初始化comboBox_indi
			 ArrayList^ indiAL = gcnew ArrayList;
			 std::map<std::string, CIndicatorDataEx> filed;
			 m_pDataInfo->GetAllIndicator(filed);
			 if (filed.size() > 0)
			 {
				 std::map<std::string, CIndicatorDataEx>::iterator it=filed.begin();
				 while(it!= filed.end())
				 {
					 indiAL->Add( Tools::string2String(it->first.c_str() ));
					 it++;
				 }
				 comboBox_indi->Items->AddRange(indiAL->ToArray());

				 it=filed.begin();
				 comboBox_indi->Text = Tools::string2String( it->first.c_str());
			 }


			 //初始化comboBox_ins
			 ArrayList^ myAL = gcnew ArrayList;
			 char strtemp[32];
			 for (int i=0; i< m_nInsCount;i++)
			 {				 
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","合约",i+1 );
				 myAL->Add( Tools::string2String(strtemp ));
			 }
			 comboBox_ins->Items->AddRange(myAL->ToArray());
			 memset(strtemp,0,sizeof(strtemp));
			 sprintf( strtemp, "%s%d","合约",m_StrategyIndicator->nInstrument );
			 comboBox_ins->Text = Tools::string2String(strtemp);


			 if (m_bOprationType == false)
			 {
				 textBox_name->Text = Tools::string2String(m_StrategyIndicator->szTitle);
				 comboBox_indi->Text = Tools::string2String(m_StrategyIndicator->szName);
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","合约",m_StrategyIndicator->nInstrument );
				 comboBox_ins->Text = Tools::string2String(strtemp);
				 checkBox1->Checked = m_StrategyIndicator->bOutput;

				 std::map<std::string, IndicatorPara>    mapFiled;
				 string name = Tools::String2string(comboBox_indi->Text);
				 m_pDataInfo->GetIndiPara(name,mapFiled);
				 CIndicatorDataEx* filed =  new CIndicatorDataEx();
				 if (m_pDataInfo->GetIndicatorInfobyName(name,*filed) && filed->GetIndicatorDescribe() != "")
				 {
					 label_describe->Text = Tools::string2String(filed->GetIndicatorDescribe().c_str());
				 }


				 filed->GetIndiPara(mapFiled);
				 std::map<std::string, IndicatorPara>::iterator it = mapFiled.begin();
				 
				 for ( int i=0; it != mapFiled.end(); it++,i++)
				 {
					 IndicatorPara indiPara;
					 memset(&indiPara,0,sizeof(IndicatorPara));
					 indiPara = it->second;
					 GenerateNewParaLine(indiPara,i);

					 memset(strtemp,0,sizeof(strtemp));
					 sprintf( strtemp, "%s%d","comboBox_value",i );
					 ComboBox^  comboBox_Ins  = (ComboBox^)panel1->Controls->Find( Tools::string2String(strtemp),true )[0];

					 array<System::String ^>^sArray =gcnew array<System::String ^>{};
					 sArray = Tools::string2String(m_StrategyIndicator->szPara)->Split(',');
					 comboBox_Ins->Text = sArray[i];
				 }
			 }

			 bFlag = true;
		 }
private: System::Void comboBox2_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		 }

private: System::Void button_ok_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if( String::IsNullOrEmpty(textBox_name->Text))  
			 {
				 MessageBox::Show("请输入指标名称!");    
				 return;
			 }
			 strncpy(m_StrategyIndicator->szName , Tools::String2string(comboBox_indi->Text).c_str(),sizeof(m_StrategyIndicator->szTitle)-1);
			 strncpy(m_StrategyIndicator->szTitle , Tools::String2string(textBox_name->Text).c_str(),sizeof(m_StrategyIndicator->szName)-1);
			 m_StrategyIndicator->nInstrument = comboBox_ins->SelectedIndex+1;
			 m_StrategyIndicator->bOutput = checkBox1->Checked;

			 std::map<std::string, IndicatorPara>    mapFiled;
			 string name = Tools::String2string(comboBox_indi->Text);
			 m_pDataInfo->GetIndiPara(name,mapFiled);
			  char strtemp[32];
			  std::map<std::string, IndicatorPara>::iterator  it = mapFiled.begin();
			  string strVa="";
			 for (int i=0; it != mapFiled.end() ; it++,i++)
			 {
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","comboBox_value",i );
				 String^ value = ((ComboBox^)panel1->Controls->Find( Tools::string2String(strtemp),true )[0])->Text;
				// sprintf( strtemp, "%s=%s,", it->first.c_str(),Tools::String2string(value).c_str() );
				 sprintf( strtemp, "%s,", Tools::String2string(value).c_str() );
				 strVa += strtemp;
				 
			 }
			 strVa = strVa.replace(strVa.size()-1,1,"");
			 strncpy(m_StrategyIndicator->szPara , strVa.c_str(),sizeof(m_StrategyIndicator->szPara)-1);

			 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
			 this->Close();
		 }
private: System::Void button_cancle_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Close();
		 }
};
}

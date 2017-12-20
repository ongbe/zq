#pragma once
#include "DataManageEx.h"
#include "Tools.h"
#include "stdio.h"
#include "..\CommonClasses\ConvEnum.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltStrategy {

	/// <summary>
	/// Summary for StrategyPlanCfg
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class StrategyPlanCfg : public System::Windows::Forms::Form
	{
	public:
		StrategyPlanCfg(SStrategyInstance* pStrategyInstance,bool bAdd)//新增true,修改false
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pDataInfo = CDataManageEx::NewInstance();
			m_StrategyInstance =pStrategyInstance;
			m_bAdd =bAdd;
			m_strategy = new CStrategyDataEx();
			bFirstChange =false;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~StrategyPlanCfg()
		{
			if (components)
			{
				delete components;
			}
			CDataManageEx::DestroyInstance();
			
		}
	bool bFirstChange;
	CStrategyDataEx*  m_strategy;
	private: bool m_bAdd;
	private: CDataManageEx*     m_pDataInfo;
	private: SStrategyInstance*  m_StrategyInstance;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::CheckBox^  checkBox_timer;
	protected: 

	private: System::Windows::Forms::TextBox^  textBox_describe;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  textBox_version;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  textBox_name;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  comboBox_timer;
	private: System::Windows::Forms::TextBox^  textBox_timer;


	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Panel^  panel_para;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  lable5;
	private: System::Windows::Forms::Panel^  panel_instrment;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::GroupBox^  groupBox4;
	private: System::Windows::Forms::CheckBox^  checkBox_StopCancel;

	private: System::Windows::Forms::CheckBox^  checkBox_offline;
	private: System::Windows::Forms::CheckBox^  checkBox_loadStart;
	private: System::Windows::Forms::Button^  button_ok;
	private: System::Windows::Forms::Button^  button_close;











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
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->comboBox_timer = (gcnew System::Windows::Forms::ComboBox());
			this->textBox_timer = (gcnew System::Windows::Forms::TextBox());
			this->checkBox_timer = (gcnew System::Windows::Forms::CheckBox());
			this->textBox_describe = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox_version = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->textBox_name = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->panel_para = (gcnew System::Windows::Forms::Panel());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->lable5 = (gcnew System::Windows::Forms::Label());
			this->panel_instrment = (gcnew System::Windows::Forms::Panel());
			this->groupBox4 = (gcnew System::Windows::Forms::GroupBox());
			this->checkBox_StopCancel = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_offline = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox_loadStart = (gcnew System::Windows::Forms::CheckBox());
			this->button_ok = (gcnew System::Windows::Forms::Button());
			this->button_close = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->groupBox3->SuspendLayout();
			this->groupBox4->SuspendLayout();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->comboBox_timer);
			this->groupBox1->Controls->Add(this->textBox_timer);
			this->groupBox1->Controls->Add(this->checkBox_timer);
			this->groupBox1->Controls->Add(this->textBox_describe);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->textBox_version);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->textBox_name);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(21, 12);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(658, 155);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"策略信息";
			// 
			// comboBox_timer
			// 
			this->comboBox_timer->Enabled = false;
			this->comboBox_timer->FormattingEnabled = true;
			this->comboBox_timer->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"秒", L"分", L"时"});
			this->comboBox_timer->Location = System::Drawing::Point(234, 124);
			this->comboBox_timer->Name = L"comboBox_timer";
			this->comboBox_timer->Size = System::Drawing::Size(62, 20);
			this->comboBox_timer->TabIndex = 8;
			// 
			// textBox_timer
			// 
			this->textBox_timer->Enabled = false;
			this->textBox_timer->Location = System::Drawing::Point(165, 124);
			this->textBox_timer->Name = L"textBox_timer";
			this->textBox_timer->Size = System::Drawing::Size(53, 21);
			this->textBox_timer->TabIndex = 7;
			// 
			// checkBox_timer
			// 
			this->checkBox_timer->AutoSize = true;
			this->checkBox_timer->Location = System::Drawing::Point(38, 130);
			this->checkBox_timer->Name = L"checkBox_timer";
			this->checkBox_timer->Size = System::Drawing::Size(120, 16);
			this->checkBox_timer->TabIndex = 6;
			this->checkBox_timer->Text = L"启用定时器，间隔";
			this->checkBox_timer->UseVisualStyleBackColor = true;
			this->checkBox_timer->CheckedChanged += gcnew System::EventHandler(this, &StrategyPlanCfg::checkBox_timer_CheckedChanged);
			// 
			// textBox_describe
			// 
			this->textBox_describe->Enabled = false;
			this->textBox_describe->Location = System::Drawing::Point(95, 57);
			this->textBox_describe->Multiline = true;
			this->textBox_describe->Name = L"textBox_describe";
			this->textBox_describe->Size = System::Drawing::Size(446, 52);
			this->textBox_describe->TabIndex = 5;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(36, 66);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(53, 12);
			this->label3->TabIndex = 4;
			this->label3->Text = L"策略描述";
			// 
			// textBox_version
			// 
			this->textBox_version->Enabled = false;
			this->textBox_version->Location = System::Drawing::Point(441, 20);
			this->textBox_version->Name = L"textBox_version";
			this->textBox_version->Size = System::Drawing::Size(100, 21);
			this->textBox_version->TabIndex = 3;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(370, 29);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 12);
			this->label2->TabIndex = 2;
			this->label2->Text = L"策略版本";
			// 
			// textBox_name
			// 
			this->textBox_name->Enabled = false;
			this->textBox_name->Location = System::Drawing::Point(95, 21);
			this->textBox_name->Name = L"textBox_name";
			this->textBox_name->Size = System::Drawing::Size(146, 21);
			this->textBox_name->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(36, 30);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"策略名称";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->panel_para);
			this->groupBox2->Location = System::Drawing::Point(21, 326);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(658, 217);
			this->groupBox2->TabIndex = 1;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"自定义参数";
			// 
			// panel_para
			// 
			this->panel_para->AutoScroll = true;
			this->panel_para->Location = System::Drawing::Point(7, 21);
			this->panel_para->Name = L"panel_para";
			this->panel_para->Size = System::Drawing::Size(645, 185);
			this->panel_para->TabIndex = 0;
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->label5);
			this->groupBox3->Controls->Add(this->label4);
			this->groupBox3->Controls->Add(this->lable5);
			this->groupBox3->Controls->Add(this->panel_instrment);
			this->groupBox3->Location = System::Drawing::Point(21, 173);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(652, 147);
			this->groupBox3->TabIndex = 2;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"合约信息";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(489, 20);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(29, 12);
			this->label5->TabIndex = 3;
			this->label5->Text = L"合约";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(338, 20);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(29, 12);
			this->label4->TabIndex = 2;
			this->label4->Text = L"品种";
			// 
			// lable5
			// 
			this->lable5->AutoSize = true;
			this->lable5->Location = System::Drawing::Point(185, 21);
			this->lable5->Name = L"lable5";
			this->lable5->Size = System::Drawing::Size(41, 12);
			this->lable5->TabIndex = 1;
			this->lable5->Text = L"交易所";
			// 
			// panel_instrment
			// 
			this->panel_instrment->AutoScroll = true;
			this->panel_instrment->Location = System::Drawing::Point(7, 47);
			this->panel_instrment->Name = L"panel_instrment";
			this->panel_instrment->Size = System::Drawing::Size(639, 94);
			this->panel_instrment->TabIndex = 0;
			// 
			// groupBox4
			// 
			this->groupBox4->Controls->Add(this->checkBox_StopCancel);
			this->groupBox4->Controls->Add(this->checkBox_offline);
			this->groupBox4->Controls->Add(this->checkBox_loadStart);
			this->groupBox4->Location = System::Drawing::Point(21, 550);
			this->groupBox4->Name = L"groupBox4";
			this->groupBox4->Size = System::Drawing::Size(658, 68);
			this->groupBox4->TabIndex = 3;
			this->groupBox4->TabStop = false;
			this->groupBox4->Text = L"策略控制";
			// 
			// checkBox_StopCancel
			// 
			this->checkBox_StopCancel->AutoSize = true;
			this->checkBox_StopCancel->Location = System::Drawing::Point(405, 30);
			this->checkBox_StopCancel->Name = L"checkBox_StopCancel";
			this->checkBox_StopCancel->Size = System::Drawing::Size(132, 16);
			this->checkBox_StopCancel->TabIndex = 2;
			this->checkBox_StopCancel->Text = L"策略停止前执行撤单";
			this->checkBox_StopCancel->UseVisualStyleBackColor = true;
			this->checkBox_StopCancel->Visible = false;
			// 
			// checkBox_offline
			// 
			this->checkBox_offline->AutoSize = true;
			this->checkBox_offline->Location = System::Drawing::Point(203, 30);
			this->checkBox_offline->Name = L"checkBox_offline";
			this->checkBox_offline->Size = System::Drawing::Size(144, 16);
			this->checkBox_offline->TabIndex = 1;
			this->checkBox_offline->Text = L"交易员离线后暂停运行";
			this->checkBox_offline->UseVisualStyleBackColor = true;
			// 
			// checkBox_loadStart
			// 
			this->checkBox_loadStart->AutoSize = true;
			this->checkBox_loadStart->Checked = true;
			this->checkBox_loadStart->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBox_loadStart->Location = System::Drawing::Point(38, 30);
			this->checkBox_loadStart->Name = L"checkBox_loadStart";
			this->checkBox_loadStart->Size = System::Drawing::Size(108, 16);
			this->checkBox_loadStart->TabIndex = 0;
			this->checkBox_loadStart->Text = L"加载后立即运行";
			this->checkBox_loadStart->UseVisualStyleBackColor = true;
			// 
			// button_ok
			// 
			this->button_ok->Location = System::Drawing::Point(147, 651);
			this->button_ok->Name = L"button_ok";
			this->button_ok->Size = System::Drawing::Size(75, 23);
			this->button_ok->TabIndex = 4;
			this->button_ok->Text = L"确定";
			this->button_ok->UseVisualStyleBackColor = true;
			this->button_ok->Click += gcnew System::EventHandler(this, &StrategyPlanCfg::button_ok_Click);
			// 
			// button_close
			// 
			this->button_close->Location = System::Drawing::Point(383, 651);
			this->button_close->Name = L"button_close";
			this->button_close->Size = System::Drawing::Size(75, 23);
			this->button_close->TabIndex = 5;
			this->button_close->Text = L"关闭";
			this->button_close->UseVisualStyleBackColor = true;
			this->button_close->Click += gcnew System::EventHandler(this, &StrategyPlanCfg::button_close_Click);
			// 
			// StrategyPlanCfg
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(710, 707);
			this->Controls->Add(this->button_close);
			this->Controls->Add(this->button_ok);
			this->Controls->Add(this->groupBox4);
			this->Controls->Add(this->groupBox3);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Name = L"StrategyPlanCfg";
			this->Text = L"策略实例配置";
			this->Load += gcnew System::EventHandler(this, &StrategyPlanCfg::StrategyPlanCfg_Load);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			this->groupBox4->ResumeLayout(false);
			this->groupBox4->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	public:SStrategyInstance* GetStrategyInstance()
		   {

			   return m_StrategyInstance;
		   }
private: System::Void GenerateInstrumentList(int  Count)
		 {
			 for ( int i=0; i<Count; i++)
			 {
				  char strtemp[32];
				 //合约
				 Label^ lbvalue = gcnew Label();
				 lbvalue->AutoSize = true;
				 lbvalue->Location = System::Drawing::Point(31, 23+30*i);
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","label_Ins",i+1 );
				 lbvalue->Name = Tools::string2String(strtemp);
				 lbvalue->Size = System::Drawing::Size(41, 12);
				 lbvalue->TabIndex = 4*i;
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","合约",i+1 );
				 lbvalue->Text =  Tools::string2String(strtemp );
				 this->panel_instrment->Controls->Add(lbvalue);

				 //交易所comobox
				 ComboBox^  comboBox_exchange = gcnew ComboBox();
				 comboBox_exchange->FormattingEnabled = true;
				 comboBox_exchange->Location = System::Drawing::Point(142, 15+30*i);
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","comboBox_exchange",i );
				 comboBox_exchange->Name =  Tools::string2String(strtemp);
				 comboBox_exchange->Size = System::Drawing::Size(121, 20);
				 comboBox_exchange->TabIndex = 1+4*i;
				 comboBox_exchange->Items->AddRange(gcnew cli::array< System::Object^  >(4) {L"中金所", L"大商所", L"郑商所",L"上期所"});
				 comboBox_exchange->SelectedIndexChanged += gcnew System::EventHandler(this, &StrategyPlanCfg::comboBox_exchange_SelectedIndexChanged);
				 this->panel_instrment->Controls->Add(comboBox_exchange);

				 //品种comobox
				 ComboBox^  comboBox_product = gcnew ComboBox();
				 comboBox_product->FormattingEnabled = true;
				 comboBox_product->Location = System::Drawing::Point(295, 15+30*i);
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","comboBox_product",i );
				 comboBox_product->Name =  Tools::string2String(strtemp);
				 comboBox_product->Size = System::Drawing::Size(121, 20);
				 comboBox_product->TabIndex = 2+4*i;
				 comboBox_product->SelectedIndexChanged += gcnew System::EventHandler(this, &StrategyPlanCfg::comboBox_product_SelectedIndexChanged);
				 this->panel_instrment->Controls->Add(comboBox_product);

				 //合约comobox
				 ComboBox^  comboBox_Ins = gcnew ComboBox();
				 comboBox_Ins->FormattingEnabled = true;
				 comboBox_Ins->Location = System::Drawing::Point(457, 15+30*i);
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","comboBox_Ins",i );
				 comboBox_Ins->Name =  Tools::string2String(strtemp);
				 comboBox_Ins->Size = System::Drawing::Size(121, 20);
				 comboBox_Ins->TabIndex = 3+4*i;
				 this->panel_instrment->Controls->Add(comboBox_Ins);
			 }

		 }
private: System::Void GenerateStrategyParaList(CStrategyDataEx&  strategyData)
		 {
			 char strtemp[32];
			 std::map<std::string,StrategyPara> mapPara;
			 strategyData.GetStrategyPara(mapPara);
			 std::map<std::string,StrategyPara>::iterator it = mapPara.begin();
			 for ( int i= 0; it!= mapPara.end(); it++,i++)
			 {
				 
				 //参数名
				 Label^ lbName = gcnew Label();
				 lbName->AutoSize = true;
				 lbName->Location = System::Drawing::Point(29, 23+30*i);
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","label_paraName",i+1 );
				 lbName->Name = Tools::string2String(strtemp);
				 lbName->Size = System::Drawing::Size(41, 12);
				 lbName->TabIndex = 4*i;
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s(%s)",it->second.szTitle ,it->second.szName);
				 lbName->Text =  Tools::string2String(strtemp );
				 this->panel_para->Controls->Add(lbName);

				 //参数类型
				 Label^ lbType = gcnew Label();
				 lbType->AutoSize = true;
				 lbType->Location = System::Drawing::Point(225, 23+30*i);
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","label_paraType",i+1 );
				 lbType->Name = Tools::string2String(strtemp);
				 lbType->Size = System::Drawing::Size(41, 12);
				 lbType->TabIndex = 4*i+1;
				 lbType->Text =  Tools::string2String(StrategyType2String(it->second.eType));
				 this->panel_para->Controls->Add(lbType);

                 //默认值
				 TextBox^  textBoxDefault= gcnew TextBox();
				 textBoxDefault->Location = System::Drawing::Point(355, 15+30*i);
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","textBoxDefault",i+1 );
				 textBoxDefault->Name = Tools::string2String(strtemp);
				 textBoxDefault->Size = System::Drawing::Size(100, 21);
				 textBoxDefault->TabIndex =4*i+ 2;
				 textBoxDefault->Text =  Tools::string2String(it->second.szDefault);
				 this->panel_para->Controls->Add(textBoxDefault);
			 }

		 }
	private: System::Void StrategyPlanCfg_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent();

				 textBox_name->Text = Tools::string2String(m_StrategyInstance->strategyName);

				 m_pDataInfo->GetDownloadStrategy( m_StrategyInstance->strategyName,*m_strategy);
				 textBox_version->Text = Tools::string2String(m_strategy->GetStrategyVersion().c_str());		 
				 textBox_describe->Text = Tools::string2String(m_strategy->GetDescribe().c_str());
				 //上传人
				 strncpy(m_StrategyInstance->strTraderName, m_strategy->GetOwner().c_str(), sizeof(m_StrategyInstance->strTraderName)-1);

				 int insCount = m_strategy->GetInstrumentCount();
				 GenerateInstrumentList(insCount);
				 GenerateStrategyParaList(*m_strategy);

				 //定时器默认不勾选
				 checkBox_timer->Checked =false;
				 textBox_timer->Enabled =false ;
				 comboBox_timer->Enabled =false;

				 if ( m_bAdd)//新增
				 {

				 }
				 else//修改
				 {
					 bFirstChange =true;
					 //定时器
                     if (m_StrategyInstance->bStartTimer)
                     {
						 checkBox_timer->Checked =true;
						 textBox_timer->Enabled =true ;
						 comboBox_timer->Enabled =true;
						 int nSpan = m_StrategyInstance->nTimerSpan;
						 
						 if ( nSpan % 3600 == 0 )
						 {
							 int hour =(m_StrategyInstance->nTimerSpan)/3600;
							 textBox_timer->Text = Convert::ToString(hour);
							 comboBox_timer->Text = "时";
						 }
						 else if (nSpan % 60 == 0  )
						 {
							 textBox_timer->Text = Convert::ToString((m_StrategyInstance->nTimerSpan)/60);
							 comboBox_timer->Text = "分";
						 }
						 else
						 {
							 textBox_timer->Text = Convert::ToString(m_StrategyInstance->nTimerSpan);
							 comboBox_timer->Text = "秒";
						 }
						 
                     }

					 //合约
					 char strtemp[32];
					 array<System::String ^>^sArray =gcnew array<System::String ^>{};
					 sArray = Tools::string2String(m_StrategyInstance->Instruments)->Split(',');
					 for ( int i = 0; i < sArray->Length; i++ )
					 {
						 memset(strtemp,0,sizeof(strtemp));
						 sprintf( strtemp, "%s%d","comboBox_Ins",i );
						 ComboBox^  comboBox_Ins  = (ComboBox^)panel_instrment->Controls->Find( Tools::string2String(strtemp),true )[0];
						 comboBox_Ins->Text = sArray[i];
						 string productID ="" ; 
						 memset(strtemp,0,sizeof(strtemp));
						 sprintf( strtemp, "%s%d","comboBox_product",i );
						 ComboBox^ comboBoxP = ((ComboBox^)panel_instrment->Controls->Find( Tools::string2String(strtemp),true )[0]);
						 if (m_pDataInfo->GetProductID(Tools::String2string(sArray[i]),productID))
						 {
							 comboBoxP->Text = Tools::string2String(productID.c_str());
							 std::map<std::string, PlatformStru_InstrumentInfo> mapIns;
							 if (m_pDataInfo->GetInstrument(productID,mapIns))
							 {
								 comboBox_Ins->Items->Clear();
								 std::map<std::string, PlatformStru_InstrumentInfo>::iterator it =mapIns.begin();
								 while ( it != mapIns.end() )
								 {
									 comboBox_Ins->Items->Add(Tools::string2String(it->second.InstrumentID));
									 it++;
								 }
							 }
						 }
						 string ExchangeID ="" ; 
						 memset(strtemp,0,sizeof(strtemp));
						 sprintf( strtemp, "%s%d","comboBox_exchange",i );
						 ComboBox^ comboBoxE = ((ComboBox^)panel_instrment->Controls->Find( Tools::string2String(strtemp),true )[0]);
						 if (m_pDataInfo->GetExchangeID(Tools::String2string(sArray[i]),ExchangeID))
						 {
							 comboBoxE->Text = Tools::string2String(ExchangeID2String(ExchangeID).c_str());
							
							 std::set<std::string> setRetProductID;
							 if (m_pDataInfo->GetProductID(ExchangeID,  setRetProductID))
							 {
								 comboBoxP->Items->Clear();
								 std::set<std::string>::iterator it = setRetProductID.begin();
								 while (it!=setRetProductID.end())
								 {
									 comboBoxP->Items->Add(Tools::string2String((*it).c_str()));
									 it++;
								 }
							 }
						 }

					 }
					 //参数

					 CStrategyDataEx strategy;

					 strategy.ImportFromXml(std::string(m_StrategyInstance->strPara));
					 std::map<std::string,StrategyPara>& mapPara = strategy.GetStrategyParams();
					 for ( int i =0; i< mapPara.size(); i++)
					 {
						 memset(strtemp,0,sizeof(strtemp));
					     sprintf( strtemp, "%s%d","label_paraName",i+1 );
						 Label^  Label_para  = (Label^)panel_para->Controls->Find( Tools::string2String(strtemp),true )[0];						 
						 int nfind = Label_para->Text->IndexOf("(");
						 String^ paraTitle= Label_para->Text->Remove(nfind);
						 std::map<std::string,StrategyPara>::iterator it = mapPara.find(Tools::String2string(paraTitle));
						 if (it!=mapPara.end())
						 {
							 memset(strtemp,0,sizeof(strtemp));
							 sprintf( strtemp, "%s%d","textBoxDefault",i+1 );
							 TextBox^  textBoxDefault  = (TextBox^)panel_para->Controls->Find( Tools::string2String(strtemp),true )[0];		
							 textBoxDefault->Text = Tools::string2String(it->second.szDefault);
						 }
					 }
					 
					 //策略控制
					 checkBox_loadStart->Checked = (bool)m_StrategyInstance->nRunAfterLoad;
					 checkBox_offline->Checked = m_StrategyInstance->nRunAfterOffline == 0;
					 checkBox_StopCancel->Checked = (bool)m_StrategyInstance->nOrderActionBeforeStop;

					 

				 }

			 }

private: System::Void comboBox_exchange_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (bFirstChange)
			 {
				 bFirstChange =false;
				 return;
			 }
			 
			 ComboBox^ comboBoxE = (ComboBox^)(sender);
			 String^ comboBoxName = comboBoxE->Name; 
			 String^ product = "comboBox_product" + comboBoxName->Substring(17, 1);
			  String^ instrument = "comboBox_Ins"  + comboBoxName->Substring(17, 1);
			 ComboBox^ comboBoxP = ((ComboBox^)panel_instrment->Controls->Find( product,true )[0]);
			  ComboBox^ comboBoxI = ((ComboBox^)panel_instrment->Controls->Find( instrument,true )[0]);

			  String^ selectedExchange = (String^)(comboBoxE->SelectedItem);
			  string exchang =ExchangeName2ID(Tools::String2string(selectedExchange));
			  std::set<std::string> setRetProductID;
			  if (m_pDataInfo->GetProductID(exchang,  setRetProductID))
			  {
				  comboBoxP->Items->Clear();
				  comboBoxP->Text ="";
				  comboBoxI->Items->Clear();
				  comboBoxI->Text ="";
				  std::set<std::string>::iterator it = setRetProductID.begin();
				  while (it!=setRetProductID.end())
				  {
					  comboBoxP->Items->Add(Tools::string2String((*it).c_str()));
					  it++;
				  }
			  }
			  
		 }

private: System::Void comboBox_product_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (bFirstChange)
			 {
				 bFirstChange =false;
				 return;
			 }
			 ComboBox^ comboBoxP = (ComboBox^)(sender);
			 String^ comboBoxName = comboBoxP->Name; 
			 String^ instrument = "comboBox_Ins" + comboBoxName->Substring(16, 1);
			 ComboBox^ comboBoxI = ((ComboBox^)panel_instrment->Controls->Find( instrument,true )[0]);

			 String^ selectedpProduct = (String^)(comboBoxP->SelectedItem);
			 std::map<std::string, PlatformStru_InstrumentInfo> mapIns;
			 if (m_pDataInfo->GetInstrument(Tools::String2string(selectedpProduct),mapIns))
			 {
				 comboBoxI->Items->Clear();
				 comboBoxI->Text ="";
				 std::map<std::string, PlatformStru_InstrumentInfo>::iterator it =mapIns.begin();
				 while ( it != mapIns.end() )
				 {
					 comboBoxI->Items->Add(Tools::string2String(it->second.InstrumentID));
					 it++;
				 }
			 }
		 }

private: System::Void button_ok_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //合约
			 char strtemp[32];
			 int insCount = m_strategy->GetInstrumentCount();
			 string strVa="";
			 for (int i=0; i< insCount; i++)
			 {
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","comboBox_Ins",i );
				 String^ value = ((ComboBox^)panel_instrment->Controls->Find( Tools::string2String(strtemp),true )[0])->Text;
				 if (String::IsNullOrEmpty(value))
				 {
					 MessageBox::Show("请选择合约!");  
					 return;
				 }
				 sprintf( strtemp, "%s,", Tools::String2string(value).c_str() );
				 strVa += strtemp;

			 }
			 strVa = strVa.replace(strVa.size()-1,1,"");
			 strncpy(m_StrategyInstance->Instruments , strVa.c_str(),sizeof(m_StrategyInstance->Instruments)-1);

			 //策略控制参数
			 if (checkBox_loadStart->Checked)
			 {
				 m_StrategyInstance->nRunAfterLoad =1;
			 }
			 else
			 {
				 m_StrategyInstance->nRunAfterLoad =0;
			 }

			 if (checkBox_offline->Checked)
			 {
				 m_StrategyInstance->nRunAfterOffline =0;
			 }
			 else
			 {
				 m_StrategyInstance->nRunAfterOffline =1;
			 }

			 if (checkBox_StopCancel->Checked)
			 {
				 m_StrategyInstance->nOrderActionBeforeStop =1;
			 }
			 else
			 {
				 m_StrategyInstance->nOrderActionBeforeStop =0;
			 }

			 //自定义参数

			 std::map<std::string,StrategyPara>& mapPara = m_strategy->GetStrategyParams();
			 std::map<std::string,StrategyPara>::iterator it = mapPara.begin();
			 for ( int i= 0; it!= mapPara.end(); it++,i++)
			 {
				 memset(strtemp,0,sizeof(strtemp));
				 sprintf( strtemp, "%s%d","textBoxDefault",i+1 );
				 String^ value = ((TextBox^)panel_para->Controls->Find( Tools::string2String(strtemp),true )[0])->Text;
				 //sprintf( strtemp, "%s,", Tools::String2string(value).c_str() );
				 if (String::IsNullOrEmpty(value))
				 {
					 MessageBox::Show("请输入默认值!");  
					 return;
				 }
				 strncpy(it->second.szDefault,Tools::String2string(value).c_str(), sizeof(it->second.szDefault)-1);
			 }
			 string para;
			  m_strategy->MakeXml(para);
			  strncpy(m_StrategyInstance->strPara,para.c_str(), sizeof(m_StrategyInstance->strPara)-1);

			  //定时器
			  m_StrategyInstance->bStartTimer = checkBox_timer->Checked;
			  if(m_StrategyInstance->bStartTimer) {
				  int interval = Convert::ToInt32(textBox_timer->Text);
				  m_StrategyInstance->nTimerSpan = (int)(interval *pow(60.0,comboBox_timer->SelectedIndex));
			  }
			  else 
				  m_StrategyInstance->nTimerSpan = 0;
				

			  this->DialogResult = System::Windows::Forms::DialogResult::Yes;
			  this->Close();

		 }
private: System::Void button_close_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			  this->DialogResult = System::Windows::Forms::DialogResult::No;
			 this->Close();
		 }
private: System::Void checkBox_timer_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if (checkBox_timer->Checked)
			 {
				 textBox_timer->Enabled =true;
				 comboBox_timer->Enabled =true;
			 }
			 else
			 {
				 textBox_timer->Enabled =false ;
				 comboBox_timer->Enabled =false;
			 }
		 }
};
}

#pragma once
#include "CDataInfo.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Xml;
#include <cliext/vector>
#include "Tools.h"

namespace Client {

	public ref class ScreenConfig
	{
	public:
		Int32 mnType;
		Int32 mnOrgID;
		String^ msOrgName;
		Int32 mnProID;
		String^ msProName;	
	public:
		ScreenConfig()
		{
			mnType = 0; //无效的类型
		}
	};

	public ref class ComboOrgItem
	{
	public:		
		Int32 mnOrgID;
		String^ msOrgName;
	public: virtual String^ ToString() override
		{
			return msOrgName;
		}
		
	};

	public ref class ComboOrgProItem
	{
	public:		
		Int32 mnOrgID;
		String^ msOrgName;
		Int32 mnProId;
		String^ msProName;
	public: virtual String^ ToString() override
			{
				return msOrgName + "++" + msProName;
			}

	};
	/// <summary>
	/// Summary for RiskCombinationScreenSelect
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class RiskCombinationScreenSelect : public System::Windows::Forms::Form
	{
	public:
		RiskCombinationScreenSelect(void)
		{
			m_pdataInfo = CDataInfo::NewInstance();
			InitializeComponent();
			mXmlDoc = gcnew XmlDocument();
			mnSelectScreenIndex = -1;
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~RiskCombinationScreenSelect()
		{
			if (components)
			{
				delete components;
			}
		}
	private: CDataInfo* m_pdataInfo;
	private: cliext::vector<ScreenConfig^> mScreensConfig;
	private: System::Xml::XmlDocument ^ mXmlDoc;
	private: Int32 mnSelectScreenIndex;
	private: System::Windows::Forms::Panel^  panel1;
	protected: 
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::SplitContainer^  splitContainer3;
	private: System::Windows::Forms::SplitContainer^  splitContainer2;

	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::ComboBox^  comboBox2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::CheckBox^  checkBox3;
	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::CheckBox^  checkBox4;
	private: System::Windows::Forms::CheckBox^  SelectCheckBox;
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
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->splitContainer3 = (gcnew System::Windows::Forms::SplitContainer());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox4 = (gcnew System::Windows::Forms::CheckBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->panel1->SuspendLayout();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->splitContainer3->Panel1->SuspendLayout();
			this->splitContainer3->Panel2->SuspendLayout();
			this->splitContainer3->SuspendLayout();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::SystemColors::ControlDark;
			this->panel1->Controls->Add(this->splitContainer1);
			this->panel1->Location = System::Drawing::Point(12, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(461, 288);
			this->panel1->TabIndex = 0;
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->splitContainer3);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer2);
			this->splitContainer1->Size = System::Drawing::Size(461, 288);
			this->splitContainer1->SplitterDistance = 225;
			this->splitContainer1->TabIndex = 0;
			// 
			// splitContainer3
			// 
			this->splitContainer3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer3->Location = System::Drawing::Point(0, 0);
			this->splitContainer3->Name = L"splitContainer3";
			this->splitContainer3->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer3.Panel1
			// 
			this->splitContainer3->Panel1->Controls->Add(this->checkBox1);
			// 
			// splitContainer3.Panel2
			// 
			this->splitContainer3->Panel2->Controls->Add(this->checkBox3);
			this->splitContainer3->Size = System::Drawing::Size(225, 288);
			this->splitContainer3->SplitterDistance = 134;
			this->splitContainer3->TabIndex = 0;
			// 
			// checkBox1
			// 
			this->checkBox1->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox1->AutoSize = true;
			this->checkBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->checkBox1->Location = System::Drawing::Point(0, 0);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(225, 134);
			this->checkBox1->TabIndex = 0;
			this->checkBox1->Text = L"无";
			this->checkBox1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &RiskCombinationScreenSelect::checkBox1_CheckedChanged);
			// 
			// checkBox3
			// 
			this->checkBox3->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox3->AutoSize = true;
			this->checkBox3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->checkBox3->Location = System::Drawing::Point(0, 0);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(225, 150);
			this->checkBox3->TabIndex = 0;
			this->checkBox3->Text = L"无";
			this->checkBox3->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->checkBox3->UseVisualStyleBackColor = true;
			this->checkBox3->CheckedChanged += gcnew System::EventHandler(this, &RiskCombinationScreenSelect::checkBox3_CheckedChanged);
			// 
			// splitContainer2
			// 
			this->splitContainer2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer2->Location = System::Drawing::Point(0, 0);
			this->splitContainer2->Name = L"splitContainer2";
			this->splitContainer2->Orientation = System::Windows::Forms::Orientation::Horizontal;
			// 
			// splitContainer2.Panel1
			// 
			this->splitContainer2->Panel1->Controls->Add(this->checkBox2);
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->checkBox4);
			this->splitContainer2->Size = System::Drawing::Size(232, 288);
			this->splitContainer2->SplitterDistance = 136;
			this->splitContainer2->TabIndex = 0;
			// 
			// checkBox2
			// 
			this->checkBox2->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox2->AutoSize = true;
			this->checkBox2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->checkBox2->Location = System::Drawing::Point(0, 0);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(232, 136);
			this->checkBox2->TabIndex = 0;
			this->checkBox2->Text = L"无";
			this->checkBox2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->checkBox2->UseVisualStyleBackColor = true;
			this->checkBox2->CheckedChanged += gcnew System::EventHandler(this, &RiskCombinationScreenSelect::checkBox2_CheckedChanged);
			// 
			// checkBox4
			// 
			this->checkBox4->Appearance = System::Windows::Forms::Appearance::Button;
			this->checkBox4->AutoSize = true;
			this->checkBox4->Dock = System::Windows::Forms::DockStyle::Fill;
			this->checkBox4->Location = System::Drawing::Point(0, 0);
			this->checkBox4->Name = L"checkBox4";
			this->checkBox4->Size = System::Drawing::Size(232, 148);
			this->checkBox4->TabIndex = 0;
			this->checkBox4->Text = L"无";
			this->checkBox4->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->checkBox4->UseVisualStyleBackColor = true;
			this->checkBox4->CheckedChanged += gcnew System::EventHandler(this, &RiskCombinationScreenSelect::checkBox4_CheckedChanged);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(406, 370);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(93, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"确定";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &RiskCombinationScreenSelect::button1_Click);
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Location = System::Drawing::Point(504, 51);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 20);
			this->comboBox1->TabIndex = 2;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &RiskCombinationScreenSelect::OnOrgChange);
			// 
			// comboBox2
			// 
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Location = System::Drawing::Point(506, 103);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(121, 20);
			this->comboBox2->TabIndex = 2;
			this->comboBox2->SelectedIndexChanged += gcnew System::EventHandler(this, &RiskCombinationScreenSelect::OnProductChange);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(504, 33);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(101, 12);
			this->label1->TabIndex = 3;
			this->label1->Text = L"选择要显示的组织";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(502, 88);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(125, 12);
			this->label2->TabIndex = 3;
			this->label2->Text = L"选择要显示的理财产品";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(512, 370);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(93, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"取消";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &RiskCombinationScreenSelect::button2_Click);
			// 
			// RiskCombinationScreenSelect
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(662, 405);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->comboBox2);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->panel1);
			this->Name = L"RiskCombinationScreenSelect";
			this->Text = L"组合屏显示选择";
			this->Load += gcnew System::EventHandler(this, &RiskCombinationScreenSelect::RiskCombinationScreenSelect_Load);
			this->panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->splitContainer3->Panel1->ResumeLayout(false);
			this->splitContainer3->Panel1->PerformLayout();
			this->splitContainer3->Panel2->ResumeLayout(false);
			this->splitContainer3->Panel2->PerformLayout();
			this->splitContainer3->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel1->PerformLayout();
			this->splitContainer2->Panel2->ResumeLayout(false);
			this->splitContainer2->Panel2->PerformLayout();
			this->splitContainer2->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void RiskCombinationScreenSelect_Load(System::Object^  sender, System::EventArgs^  e) {
			 SelectCheckBox = nullptr;
			 EnableSetting(false);
			 //获取理财产品和组织机构 填充comboBox
			 if(!m_pdataInfo->IsInit())
				 return;

			 LoginRsp loginRsp;
			 memset(&loginRsp, 0, sizeof(loginRsp));
			 m_pdataInfo->GetLoginRsp(loginRsp);

			 MAPFinancialProduct mmapFinanProduct;
			 m_pdataInfo->GetFinancialProduct(mmapFinanProduct);

			 AssetMgmtOrganization assertOrg;
			 memset(&assertOrg, 0, sizeof(assertOrg));
			 if(!m_pdataInfo->GetAssetMgmtOrg(
				 loginRsp.nAssetMgmtOrgID, assertOrg))
				 return;

			 MAPAeestMgmtOrganization mapAssetMgmtOrgTree;
			 m_pdataInfo->GetAeestMgmtOrg(mapAssetMgmtOrgTree);          
			 MAPAeestMgmtOrganization::iterator it = 
				 mapAssetMgmtOrgTree.find(loginRsp.nAssetMgmtOrgID);
			 if(it == mapAssetMgmtOrgTree.end())
				 return;

			 std::map<int, AssetMgmtOrganization>& mapSub = it->second;		
			 mapSub.insert(mapSub.begin(),std::make_pair(loginRsp.nAssetMgmtOrgID,assertOrg));
			 std::map<int, AssetMgmtOrganization>::iterator itSub = mapSub.begin();
			 while(itSub != mapSub.end())
			 {			
				 ComboOrgItem^ lItem = gcnew ComboOrgItem();
				 lItem->mnOrgID = itSub->second.nAssetMgmtOrgID;
				 lItem->msOrgName = Tools::string2String(itSub->second.szName);

				 //处理理财产品
				 std::set<int> mapProductID;
				 m_pdataInfo->GetFinancialProduct( lItem->mnOrgID, true, mapProductID);
				 std::set<int>::iterator itIns;
				 MAPFinancialProduct::iterator itInfo;				
				 for(itIns = mapProductID.begin(); 
					 itIns != mapProductID.end(); itIns++) 
				 {
					 itInfo = mmapFinanProduct.find(*itIns);
					 if(itInfo != mmapFinanProduct.end())
					 {
						 ComboOrgProItem ^ lProItem = gcnew ComboOrgProItem();
						 lProItem->mnOrgID =  lItem->mnOrgID;
						 lProItem->msOrgName = lItem->msOrgName;
						 lProItem->mnProId =  *itIns;
						 lProItem->msProName =  Tools::string2String(itInfo->second.szName);					

						 comboBox2->Items->Add(lProItem);
					 }

				 }

				
				 comboBox1->Items->Add(lItem);
				 itSub++;
			 }
			 //获取配置文件，填充checkBox
			 LoadConfig();
			 UpdateScreenConfigUI();
			 

		 }
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 //1
			 this->checkBox2->Checked = false;
			 this->checkBox3->Checked = false;
			 this->checkBox4->Checked = false;
			 EnableSetting(true);
			 SelectCheckBox = this->checkBox1;
			 mnSelectScreenIndex = 0;
		 }
private: System::Void checkBox2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 //2
			 this->checkBox1->Checked = false;
			 this->checkBox3->Checked = false;
			 this->checkBox4->Checked = false;
			 EnableSetting(true);
			 SelectCheckBox = this->checkBox2;
			 mnSelectScreenIndex = 1;
		 }
private: System::Void checkBox3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 //3
			 this->checkBox2->Checked = false;
			 this->checkBox1->Checked = false;
			 this->checkBox4->Checked = false;
			 EnableSetting(true);
			 SelectCheckBox = this->checkBox3;
			 mnSelectScreenIndex = 2;
		 }
private: System::Void checkBox4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 //4
			 this->checkBox2->Checked = false;
			 this->checkBox3->Checked = false;
			 this->checkBox1->Checked = false;
			 EnableSetting(true);
			 SelectCheckBox = this->checkBox4;
			 mnSelectScreenIndex = 3;
		 }
private: System::Void EnableSetting(bool nbVal)
		 {
			 this->label1->Enabled = nbVal;
			 this->label2->Enabled = nbVal;
			 this->comboBox1->Enabled = nbVal;
			 this->comboBox2->Enabled = nbVal;
			
		 }
private:System::Void LoadConfig()
		{
			/*
			<Screens>
			<Screen Type="1" OrgId="2" OrgName="北京" PrdID="" ProName="">      
			</Screen>
			<Screen Type="2" OrgId="2" OrgName="北京" ProID="33" ProName="理财1">      
			</Screen>
			<Screen Type="2" OrgId="2" OrgName="北京" ProID="33" ProName="理财1">      
			</Screen>
			<Screen Type="2" OrgId="2" OrgName="北京" ProID="33" ProName="理财1">      
			</Screen>
			</Screens>
			*/
			//添加4个默认的
			mScreensConfig.clear();			
			for(int i = 0 ;i < 4 ;i++)
			{
				ScreenConfig ^ lConfig = gcnew ScreenConfig();
				mScreensConfig.push_back(lConfig);
			}
			
			try
			{
				mXmlDoc->Load("Screens.xml");
			}
			catch(Exception ^ e)
			{
				return;
			}
			

			int i = 0;
			XmlNodeList^ lScreenNodeList = mXmlDoc->SelectNodes("/Screens/Screen");
			if(lScreenNodeList != nullptr)
			{
				 for each (XmlNode^ lScreenNode in lScreenNodeList)
				 {
                      ScreenConfig ^ lConfig = mScreensConfig[i++];
					  if(lConfig == nullptr)
						  break;
					  lConfig->mnType =  Convert::ToInt32(lScreenNode->Attributes["Type"]->Value);
					  lConfig->mnOrgID =  Convert::ToInt32(lScreenNode->Attributes["OrgId"]->Value);
					  lConfig->msOrgName =  lScreenNode->Attributes["OrgName"]->Value;
					  lConfig->mnProID =  Convert::ToInt32(lScreenNode->Attributes["ProId"]->Value);
					  lConfig->msProName =  lScreenNode->Attributes["ProName"]->Value;					
				 }
			}

		}
private:System::Void SaveConfig()
		{
			 System::Xml::XmlDocument ^ lXmlDoc = gcnew System::Xml::XmlDocument();
			 lXmlDoc->CreateXmlDeclaration("1.0", "utf-8", "yes");
			 XmlNode^ rootNode = lXmlDoc->CreateElement("Screens");
			 for each (ScreenConfig^ empl in mScreensConfig)
			 {
				 XmlNode^ lScreenNode = lXmlDoc->CreateElement("Screen");
				 XmlAttribute^ lattType = lXmlDoc->CreateAttribute("Type");
				 lattType->Value = empl->mnType.ToString();
				 lScreenNode->Attributes->Append(lattType);

				 XmlAttribute^ lorgId = lXmlDoc->CreateAttribute("OrgId");
				 lorgId->Value = empl->mnOrgID.ToString();
				 lScreenNode->Attributes->Append(lorgId);

				 XmlAttribute^ lorgName = lXmlDoc->CreateAttribute("OrgName");
				 lorgName->Value = empl->msOrgName;
				 lScreenNode->Attributes->Append(lorgName);

				 XmlAttribute^ lproId = lXmlDoc->CreateAttribute("ProId");
				 lproId->Value = empl->mnProID.ToString();
				 lScreenNode->Attributes->Append(lproId);

				 XmlAttribute^ lproName = lXmlDoc->CreateAttribute("ProName");
				 lproName->Value = empl->msProName;
				 lScreenNode->Attributes->Append(lproName);

				 rootNode->AppendChild(lScreenNode);
				 lXmlDoc->AppendChild(rootNode);
				
			 }
			 lXmlDoc->Save("Screens.xml");

		}
private:System::Void UpdateScreenConfigUI()
		{
			int i = 1;
			for each (ScreenConfig^ empl in mScreensConfig)
			{
				System::Windows::Forms::CheckBox^ lCheckBox = nullptr;
				if(i == 1)
					lCheckBox = checkBox1;
				if(i == 2)
					lCheckBox = checkBox2;				
				if(i == 3)
					lCheckBox = checkBox3;
				if(i == 4)
					lCheckBox = checkBox4;	
				if(lCheckBox == nullptr)
					break;
				if(empl->mnType == 0)
				{
					lCheckBox->Text = "无";
				}
				if(empl->mnType == 1)
				{
					lCheckBox->Text = "组织机构：" + empl->msOrgName;
				}
				if(empl->mnType == 2)
				{
					lCheckBox->Text = "理财产品：" + empl->msOrgName + "++" + empl->msProName;
				}
				i++;
			}

		}

private: System::Void OnOrgChange(System::Object^  sender, System::EventArgs^  e) 
		 {
			if(mnSelectScreenIndex == -1)
				return;
			ScreenConfig ^ lConfig = mScreensConfig.at(mnSelectScreenIndex);
			if(lConfig == nullptr)
				return;
			lConfig->mnType = 1;
			lConfig->mnOrgID = ((ComboOrgItem^) comboBox1->SelectedItem)->mnOrgID;
			lConfig->msOrgName = ((ComboOrgItem^) comboBox1->SelectedItem)->msOrgName;
			SaveConfig();
			UpdateScreenConfigUI();
		 }
private: System::Void OnProductChange(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(mnSelectScreenIndex == -1)
				 return;
			 ScreenConfig ^ lConfig = mScreensConfig.at(mnSelectScreenIndex);
			 if(lConfig == nullptr)
				 return;
			 lConfig->mnType = 2;
			 lConfig->mnOrgID = ((ComboOrgProItem^) comboBox2->SelectedItem)->mnOrgID;
			 lConfig->msOrgName = ((ComboOrgProItem^) comboBox2->SelectedItem)->msOrgName;
			 lConfig->mnProID = ((ComboOrgProItem^) comboBox2->SelectedItem)->mnProId;
			 lConfig->msProName = ((ComboOrgProItem^) comboBox2->SelectedItem)->msProName;
			 SaveConfig();
			 UpdateScreenConfigUI();
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 SaveConfig();
			 this->DialogResult = ::DialogResult::Cancel;  
			 this->Close();  			 
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->DialogResult = ::DialogResult::OK;  
			 this->Close();  
		 }
public: ScreenConfig ^ GetScreenConfigAt(Int32 nIndex)
		{
			return mScreensConfig[nIndex];
		}
};
}

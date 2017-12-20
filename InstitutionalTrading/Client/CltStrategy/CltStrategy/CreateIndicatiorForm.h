#pragma once
#include "Stdafx.h"
#include "AddIndicatorParaForm.h"
#include "DataManageEx.h"
#include "ToolsEx.h"
#include "..\CommonClasses\ConvEnum.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace EnvDTE;
using namespace EnvDTE80;
using namespace VSLangProj;
using namespace System::IO;
using namespace System::Text;
using namespace System::Reflection;
using namespace System::Diagnostics;
using namespace System::Collections::Generic;
//using namespace com::cifco::zqstp::strategy::codegenerate;

namespace CltStrategy {

	/// <summary>
	/// Summary for CreateIndicatiorForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class CreateIndicatiorForm : public System::Windows::Forms::Form
	{
	public:
		CreateIndicatiorForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pDataInfo = CDataManageEx::NewInstance();
			m_IndicatorData = new CIndicatorDataEx();
			
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CreateIndicatiorForm()
		{
			if (components)
			{
				delete components;
			}
			CDataManage::DestroyInstance();
			if ( NULL != m_IndicatorData )
			{
				delete m_IndicatorData;
				m_IndicatorData = NULL;
			}
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private: EnvDTE80::DTE2^ _applicationObject;
		private: CDataManageEx*     m_pDataInfo;
		System::ComponentModel::Container ^components;
		private: CIndicatorDataEx*     m_IndicatorData;
	private: System::Windows::Forms::Label^  label_IndicatorName;
	private: System::Windows::Forms::Label^  label_IndicatorType;



	private: System::Windows::Forms::TextBox^  textBox1;


	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_name;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_type;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_default;
	private: System::Windows::Forms::Button^  button_add;
	private: System::Windows::Forms::ComboBox^  comboBox1_type;
	private: System::Windows::Forms::ComboBox^  comboBox_file;
	private: System::Windows::Forms::Button^  button_delete;
	private: System::Windows::Forms::Button^  button_confirm;
	private: System::Windows::Forms::Button^  button_cancel;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBox_describe;
	private: System::Windows::Forms::Label^  label_file;




#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label_IndicatorName = (gcnew System::Windows::Forms::Label());
			this->label_IndicatorType = (gcnew System::Windows::Forms::Label());
			this->label_file = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button_delete = (gcnew System::Windows::Forms::Button());
			this->button_add = (gcnew System::Windows::Forms::Button());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column_name = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_type = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_default = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->comboBox1_type = (gcnew System::Windows::Forms::ComboBox());
			this->comboBox_file = (gcnew System::Windows::Forms::ComboBox());
			this->button_confirm = (gcnew System::Windows::Forms::Button());
			this->button_cancel = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox_describe = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// label_IndicatorName
			// 
			this->label_IndicatorName->AutoSize = true;
			this->label_IndicatorName->Location = System::Drawing::Point(46, 24);
			this->label_IndicatorName->Name = L"label_IndicatorName";
			this->label_IndicatorName->Size = System::Drawing::Size(65, 12);
			this->label_IndicatorName->TabIndex = 1;
			this->label_IndicatorName->Text = L"指标名称：";
			// 
			// label_IndicatorType
			// 
			this->label_IndicatorType->AutoSize = true;
			this->label_IndicatorType->Location = System::Drawing::Point(286, 24);
			this->label_IndicatorType->Name = L"label_IndicatorType";
			this->label_IndicatorType->Size = System::Drawing::Size(65, 12);
			this->label_IndicatorType->TabIndex = 2;
			this->label_IndicatorType->Text = L"指标类型：";
			// 
			// label_file
			// 
			this->label_file->AutoSize = true;
			this->label_file->Location = System::Drawing::Point(541, 24);
			this->label_file->Name = L"label_file";
			this->label_file->Size = System::Drawing::Size(53, 12);
			this->label_file->TabIndex = 3;
			this->label_file->Text = L"文件夹：";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(126, 18);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 21);
			this->textBox1->TabIndex = 4;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->button_delete);
			this->groupBox1->Controls->Add(this->button_add);
			this->groupBox1->Controls->Add(this->dataGridView1);
			this->groupBox1->Location = System::Drawing::Point(28, 127);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(692, 278);
			this->groupBox1->TabIndex = 7;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"参数";
			// 
			// button_delete
			// 
			this->button_delete->Location = System::Drawing::Point(572, 146);
			this->button_delete->Name = L"button_delete";
			this->button_delete->Size = System::Drawing::Size(75, 23);
			this->button_delete->TabIndex = 2;
			this->button_delete->Text = L"删除";
			this->button_delete->UseVisualStyleBackColor = true;
			this->button_delete->Click += gcnew System::EventHandler(this, &CreateIndicatiorForm::button_delete_Click);
			// 
			// button_add
			// 
			this->button_add->Location = System::Drawing::Point(572, 91);
			this->button_add->Name = L"button_add";
			this->button_add->Size = System::Drawing::Size(75, 23);
			this->button_add->TabIndex = 1;
			this->button_add->Text = L"新增";
			this->button_add->UseVisualStyleBackColor = true;
			this->button_add->Click += gcnew System::EventHandler(this, &CreateIndicatiorForm::button_add_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->AllowUserToOrderColumns = true;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->Column_name, 
				this->Column_type, this->Column_default});
			this->dataGridView1->Location = System::Drawing::Point(20, 20);
			this->dataGridView1->MultiSelect = false;
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(513, 231);
			this->dataGridView1->TabIndex = 0;
			this->dataGridView1->CellDoubleClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &CreateIndicatiorForm::dataGridView1_CellDoubleClick);
			// 
			// Column_name
			// 
			this->Column_name->HeaderText = L"名称";
			this->Column_name->Name = L"Column_name";
			this->Column_name->ReadOnly = true;
			// 
			// Column_type
			// 
			this->Column_type->HeaderText = L"类型";
			this->Column_type->Name = L"Column_type";
			this->Column_type->ReadOnly = true;
			// 
			// Column_default
			// 
			this->Column_default->HeaderText = L"默认值";
			this->Column_default->Name = L"Column_default";
			this->Column_default->ReadOnly = true;
			// 
			// comboBox1_type
			// 
			this->comboBox1_type->FormattingEnabled = true;
			this->comboBox1_type->Items->AddRange(gcnew cli::array< System::Object^  >(1) {L"快照行情指标"});
			this->comboBox1_type->Location = System::Drawing::Point(357, 19);
			this->comboBox1_type->Name = L"comboBox1_type";
			this->comboBox1_type->Size = System::Drawing::Size(133, 20);
			this->comboBox1_type->TabIndex = 8;
			// 
			// comboBox_file
			// 
			this->comboBox_file->FormattingEnabled = true;
			this->comboBox_file->Items->AddRange(gcnew cli::array< System::Object^  >(3) {L"价格指标", L"交易量指标", L"指标样例"});
			this->comboBox_file->Location = System::Drawing::Point(600, 19);
			this->comboBox_file->Name = L"comboBox_file";
			this->comboBox_file->Size = System::Drawing::Size(121, 20);
			this->comboBox_file->TabIndex = 9;
			// 
			// button_confirm
			// 
			this->button_confirm->Location = System::Drawing::Point(432, 449);
			this->button_confirm->Name = L"button_confirm";
			this->button_confirm->Size = System::Drawing::Size(112, 23);
			this->button_confirm->TabIndex = 10;
			this->button_confirm->Text = L"确定";
			this->button_confirm->UseVisualStyleBackColor = true;
			this->button_confirm->Click += gcnew System::EventHandler(this, &CreateIndicatiorForm::button_confirm_Click);
			// 
			// button_cancel
			// 
			this->button_cancel->Location = System::Drawing::Point(611, 449);
			this->button_cancel->Name = L"button_cancel";
			this->button_cancel->Size = System::Drawing::Size(100, 23);
			this->button_cancel->TabIndex = 11;
			this->button_cancel->Text = L"取消";
			this->button_cancel->UseVisualStyleBackColor = true;
			this->button_cancel->Click += gcnew System::EventHandler(this, &CreateIndicatiorForm::button_cancel_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(46, 58);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(65, 12);
			this->label1->TabIndex = 12;
			this->label1->Text = L"指标描述：";
			// 
			// textBox_describe
			// 
			this->textBox_describe->Location = System::Drawing::Point(126, 58);
			this->textBox_describe->Multiline = true;
			this->textBox_describe->Name = L"textBox_describe";
			this->textBox_describe->Size = System::Drawing::Size(594, 51);
			this->textBox_describe->TabIndex = 13;
			// 
			// CreateIndicatiorForm
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(751, 510);
			this->Controls->Add(this->textBox_describe);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button_cancel);
			this->Controls->Add(this->button_confirm);
			this->Controls->Add(this->comboBox_file);
			this->Controls->Add(this->comboBox1_type);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label_IndicatorName);
			this->Controls->Add(this->label_IndicatorType);
			this->Controls->Add(this->label_file);
			this->Name = L"CreateIndicatiorForm";
			this->Text = L"创建指标";
			this->Load += gcnew System::EventHandler(this, &CreateIndicatiorForm::CreateIndicatiorForm_Load);
			this->groupBox1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public: EnvDTE80::DTE2^ getApplicationObject()
			{
				return _applicationObject;					
			}

	public: void setApplicationObject(EnvDTE80::DTE2^ app)
			{

				_applicationObject = app;
			}
	private: System::Void CreateIndicatiorForm_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				this->CenterToParent();
				//m_pDataInfo->ClearIndiPara();
			 }
	private: System::Void button_add_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 int newRowIndex=dataGridView1->Rows->Count+1;
				 AddIndicatorParaForm^ dlg = gcnew AddIndicatorParaForm(m_IndicatorData,newRowIndex);
				 
				 if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
				 {
					IndicatorPara field;
					memset(&field,0,sizeof(field));

					 field=*dlg->GetParaInfo();	
					 int newRowIndex=dataGridView1->Rows->Add();
					 DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
					 UpdateRow(dr,field);
					 m_IndicatorData->AddIndcatorPara(field);
					 

					
				 }
			 }
private: System::Void UpdateRow(DataGridViewRow^ dr,IndicatorPara& data)
		 {
			 dr->Cells[0]->Value = Tools::string2String(data.szName);
			 dr->Cells[1]->Value = Tools::string2String(IndiPara2String(data.eType));
			 dr->Cells[2]->Value = Tools::string2String(data.szDefault);

		 }
private: System::Void button_delete_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 DataGridViewRow^  dr = dataGridView1->CurrentRow;
			 if(dr != nullptr)
			 { 
				 string szname = Tools::String2string(dr->Cells[0]->Value->ToString());
			     m_IndicatorData->DeleteIndcatorPara(szname);
				 dataGridView1->Rows->RemoveAt(dr->Index);

			 }
			
		 }

private: System::Void dataGridView1_CellDoubleClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
		 {
			 if(dataGridView1->SelectedRows != nullptr && dataGridView1->SelectedRows->Count == 1)
			 {
				 DataGridViewRow^  dr = dataGridView1->CurrentRow;

				 IndicatorPara field;
				 memset(&field,0,sizeof(field));
				 strncpy(field.szName,Tools::String2string(dr->Cells[0]->Value->ToString()).c_str(),sizeof(field.szName)-1);
				 field.eType = IndiPara2Enum(Tools::String2string(dr->Cells[1]->Value->ToString()));
				 strncpy(field.szDefault,Tools::String2string(dr->Cells[2]->Value->ToString()).c_str(),sizeof(field.szDefault)-1);
				 AddIndicatorParaForm^ dlg = gcnew AddIndicatorParaForm(m_IndicatorData,1);
				 dlg->SetParaInfo(field);
				 //string strName(field.szName);
				 //m_IndicatorData->DeleteIndcatorPara(strName);

				 if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
				 {
					 IndicatorPara* pFieid;
					 pFieid=dlg->GetParaInfo();	
					 UpdateRow(dr,*pFieid);

					 m_IndicatorData->AddIndcatorPara(*pFieid);


				 }
			 }
			 else
				 MessageBox::Show("请选择内容");
			 
		 }

		  private: bool InputCheck()
					  {
						  if( String::IsNullOrEmpty(textBox1->Text))  
						  {
							  MessageBox::Show("请输入指标名称!");    
							  return false;
						  } 
						  if( String::IsNullOrEmpty(comboBox1_type->Text))  
						  {
							  MessageBox::Show("请选择指标类型!");    
							  return false;
						  } 
						  if( String::IsNullOrEmpty(comboBox_file->Text))  
						  {
							  MessageBox::Show("请选择文件夹!");    
							  return false;
						  } 
						  if (!m_pDataInfo->CheckIndicatorName(Tools::String2string(textBox1->Text).c_str()))
						  {
							  MessageBox::Show("指标名称重复!");
							  return false;
						  }
						  return true;
					  }


				   Project^ GetProjectByName(EnvDTE90::Solution3^ soln, String^ projName) {
					   Project^ proj = nullptr;
					   for(int i = 0; i<soln->Projects->Count; i++) {
						   proj = soln->Projects->Item(i+1);
						   if(proj->default == projName) {
							   return proj;
						   }
						   else {
							   for(int i = 0; i<proj->ProjectItems->Count; i++) {
								   ProjectItem^ proj2 = proj->ProjectItems->Item(i+1);
								   if(proj2->default == projName) {
									   return (Project^)proj2->Object;
								   }
								   //else {
									  // try {
										 //  for(int i = 0; i<proj2->ProjectItems->Count; i++) {
											//   ProjectItem^ proj3 = proj2->ProjectItems->Item(i+1);
											//   if(proj3->default == projName) {
											//	   return (Project^)proj3->Object;
											//   }
										 //  }
									  // }
									  // catch(Exception e) {
									  // }
								   //}
							   }
						   }
					   }
					   return nullptr;
				   }

private: System::Void button_confirm_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			
			 if(InputCheck()== false)
				 return;

			 //IndicatorInfo indicator;
			 //memset(&indicator,0,sizeof(IndicatorInfo));
			 //strncpy(m_IndicatorData->szName , Tools::String2string(textBox1->Text).c_str(),sizeof(m_IndicatorData->szName));
			 m_IndicatorData->SetIndicatorName(Tools::String2string(textBox1->Text));
			 m_IndicatorData->SetIndicatorType(IndiType2Enum(Tools::String2string(comboBox1_type->Text).c_str()));
			 m_IndicatorData->SetIndicatorFile(Tools::String2string(comboBox_file->Text));
			 m_IndicatorData->SetIndicatorDescribe(Tools::String2string(textBox_describe->Text));

			

			 int protectcount = _applicationObject->Solution->Projects->Count; 
			 
			 string folderFullName;

			 String^ p1 = (gcnew String(_GetProjectsPath().c_str())) + textBox1->Text;

			 String^ p3=textBox1->Text+".cs";
			 String^ strCodeFile = Path::Combine( p1, p3 );
			 String^ strParamsFile = Path::Combine( p1, "params.xml" );
			 Project^ proj = nullptr;
			 
			 EnvDTE90::Solution3^ soln = (EnvDTE90::Solution3^)_applicationObject->Solution;
			 proj = GetProjectByName(soln, comboBox_file->Text);
			 SolutionFolder^ SF = (SolutionFolder^)proj->Object;

			 String^ classLibProjTemplatePath = soln->GetProjectTemplate("ClassLibrary.zip", "CSharp");
			 String^ domainProjName = textBox1->Text;
			 Project^ IndiProj = nullptr;

			 // 需要处理proj==null的情况
			 IndiProj = SF->AddFromTemplate(classLibProjTemplatePath, 
					p1, domainProjName);//, false);

			 String^ strCodeTempl = gcnew String((_GetSupportFilesPath()+"CIndicatorTemplate.txt").c_str());
			 m_IndicatorData->MakeCodeToFile(strCodeTempl, strCodeFile);
			 m_IndicatorData->MakeXmlToFile(strParamsFile);

			bool bFound = false;

			 for(int i = 0; i<proj->ProjectItems->Count; i++) {
				 ProjectItem^ proj2 = proj->ProjectItems->Item(i+1);
				 String^ strName = proj2->default;
				 if(strName == domainProjName) {
					 IndiProj = (Project^)proj2->Object;
					 bFound = true;
					 break;
				 }
			 }

			 try {
				 IndiProj->ProjectItems->AddFromFile(strCodeFile);
				 IndiProj->ProjectItems->AddFromFile(strParamsFile);
				// IndiProj->ProjectItems->Item("Class1.cs");// 需要删除多余的Class1.cs
				 VSProject^ vsproj = (VSProject^)IndiProj->Object;
				 vsproj->References->Add(gcnew String((_GetSupportFilesPath()+"CommonBizEntity.dll").c_str()));
				 // 引用一个项目。但由于指标的复杂性，可能只有dll，这样需要判断是私有还是共享指标，
				 // 决定用哪个方式引用指标。
				 //vsproj->References->AddProject();
				 IndiProj->Save(IndiProj->FileName);
				 IndiProj->ProjectItems->Item("Class1.cs")->Delete();
			     System::Boolean open = IndiProj->ProjectItems->Item(domainProjName+ ".cs")->IsOpen[EnvDTE::Constants::vsViewKindTextView];
				 Document^ doc = IndiProj->ProjectItems->Item(domainProjName+ ".cs")->Open(EnvDTE::Constants::vsViewKindTextView)->Document;
				 open = IndiProj->ProjectItems->Item(domainProjName+ ".cs")->IsOpen[EnvDTE::Constants::vsViewKindTextView]; 
				 doc->Activate();
				/* ItemOperations^ ItemOp = DTE::ItemOperations;
				 ItemOp->OpenFile(IndiProj->FileName, Constants.vsViewKindTextView);*/



				 //SolutionBuild2^ sb = (SolutionBuild2^)soln->SolutionBuild; 
				 //sb->BuildProject("Release", IndiProj->FullName, true);

				 //soln->SaveAs(soln->FileName);

				 //std::string strRetZipFile;
				 //std::string strWinRar(_GetSupportFilesPath());
				 //std::string strWorkPath(_GetProjectsPath());
				 //std::string strProjectname = Tools::String2string(domainProjName);
				 //CToolsEx::Zip_Project(strWinRar, strWorkPath, strProjectname, strRetZipFile);
				 //m_IndicatorData->SetZipFile(strRetZipFile);
				 //m_IndicatorData->SetDLLFile(_GetProjectsPath()+m_IndicatorData->GetIndicatorName()
					//	+"\\bin\\Release\\"+m_IndicatorData->GetIndicatorName()+".dll");

				 //m_pDataInfo->GetTraderApi()->ReqUploadIndicatorBaseInfo(
					//	*m_IndicatorData, 0);
				 //m_pDataInfo->GetTraderApi()->ReqUploadIndicatorFile(
					//	m_IndicatorData->GetIndicatorName(), 
					//	m_IndicatorData->GetDLLFile(), 
					//	0, true);
				 //m_pDataInfo->GetTraderApi()->ReqUploadIndicatorFile(
					//	m_IndicatorData->GetIndicatorName(), 
					//	m_IndicatorData->GetZipFile(), 0, false);
			 }
			 catch(Exception^ ex) {
				 Debug::WriteLine("{0}", ex->ToString());
				//Console.WriteLine("{0}{1}",v1,v2);
			 }

			 m_IndicatorData->SetIndicatorPrivate(1);
			  m_pDataInfo->AddIndcator(*m_IndicatorData);
			 this->DialogResult = System::Windows::Forms::DialogResult::Yes;
			 this->Close();
		 }
private: System::Void button_cancel_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			  this->Close();
		 }

};
}

#pragma once

#include "Stdafx.h"
#include "DataManageEx.h"
#include "Tools.h"
#include "StrategyPlanCfg.h"
#include "StrategyLoadOption.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;



namespace CltStrategy {

	/// <summary>
	/// Summary for StrategyPlan
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class StrategyPlan : public System::Windows::Forms::Form
	{
	public:
		StrategyPlan(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pDataInfo = CDataManageEx::NewInstance();
			m_pNewStrategyInstance = new SStrategyInstance;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~StrategyPlan()
		{
			if (components)
			{
				delete components;
			}
			CDataManageEx::DestroyInstance();
			delete m_pNewStrategyInstance;
		}

	private: SStrategyInstance* m_pNewStrategyInstance;
	private: CDataManageEx*     m_pDataInfo;
	private: System::Windows::Forms::Button^  button_add;
	private: System::Windows::Forms::Button^  button_delete;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::DataGridView^  dataGridView1;






	private: System::Windows::Forms::Button^  button_close;



	private: System::Windows::Forms::Button^  button_modify;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_name;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_Instrument;
	private: System::Windows::Forms::DataGridViewButtonColumn^  Column_status;




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
			this->button_add = (gcnew System::Windows::Forms::Button());
			this->button_delete = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column_name = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_Instrument = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_status = (gcnew System::Windows::Forms::DataGridViewButtonColumn());
			this->button_close = (gcnew System::Windows::Forms::Button());
			this->button_modify = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// button_add
			// 
			this->button_add->Location = System::Drawing::Point(692, 63);
			this->button_add->Name = L"button_add";
			this->button_add->Size = System::Drawing::Size(88, 23);
			this->button_add->TabIndex = 0;
			this->button_add->Text = L"新增实例";
			this->button_add->UseVisualStyleBackColor = true;
			this->button_add->Click += gcnew System::EventHandler(this, &StrategyPlan::button_add_Click);
			// 
			// button_delete
			// 
			this->button_delete->Location = System::Drawing::Point(692, 180);
			this->button_delete->Name = L"button_delete";
			this->button_delete->Size = System::Drawing::Size(88, 23);
			this->button_delete->TabIndex = 1;
			this->button_delete->Text = L"删除实例";
			this->button_delete->UseVisualStyleBackColor = true;
			this->button_delete->Click += gcnew System::EventHandler(this, &StrategyPlan::button_delete_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->dataGridView1);
			this->groupBox1->Location = System::Drawing::Point(21, 33);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(581, 200);
			this->groupBox1->TabIndex = 2;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"本人当前策略方案";
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->AutoSizeRowsMode = System::Windows::Forms::DataGridViewAutoSizeRowsMode::DisplayedCells;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(3) {this->Column_name, 
				this->Column_Instrument, this->Column_status});
			this->dataGridView1->Location = System::Drawing::Point(6, 20);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			dataGridViewCellStyle1->WrapMode = System::Windows::Forms::DataGridViewTriState::True;
			this->dataGridView1->RowsDefaultCellStyle = dataGridViewCellStyle1;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(562, 174);
			this->dataGridView1->TabIndex = 0;
			this->dataGridView1->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &StrategyPlan::dataGridView1_CellContentClick);
			// 
			// Column_name
			// 
			this->Column_name->HeaderText = L"策略名";
			this->Column_name->Name = L"Column_name";
			this->Column_name->ReadOnly = true;
			// 
			// Column_Instrument
			// 
			this->Column_Instrument->HeaderText = L"管理合约";
			this->Column_Instrument->Name = L"Column_Instrument";
			this->Column_Instrument->ReadOnly = true;
			// 
			// Column_status
			// 
			this->Column_status->HeaderText = L"状态";
			this->Column_status->Name = L"Column_status";
			this->Column_status->ReadOnly = true;
			this->Column_status->Resizable = System::Windows::Forms::DataGridViewTriState::True;
			// 
			// button_close
			// 
			this->button_close->Location = System::Drawing::Point(348, 281);
			this->button_close->Name = L"button_close";
			this->button_close->Size = System::Drawing::Size(75, 23);
			this->button_close->TabIndex = 5;
			this->button_close->Text = L"关闭";
			this->button_close->UseVisualStyleBackColor = true;
			this->button_close->Click += gcnew System::EventHandler(this, &StrategyPlan::button_close_Click);
			// 
			// button_modify
			// 
			this->button_modify->Location = System::Drawing::Point(692, 121);
			this->button_modify->Name = L"button_modify";
			this->button_modify->Size = System::Drawing::Size(88, 23);
			this->button_modify->TabIndex = 6;
			this->button_modify->Text = L"修改实例";
			this->button_modify->UseVisualStyleBackColor = true;
			this->button_modify->Click += gcnew System::EventHandler(this, &StrategyPlan::button_modify_Click);
			// 
			// StrategyPlan
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(844, 328);
			this->Controls->Add(this->button_modify);
			this->Controls->Add(this->button_close);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->button_delete);
			this->Controls->Add(this->button_add);
			this->Name = L"StrategyPlan";
			this->Text = L"策略实例管理";
			this->Load += gcnew System::EventHandler(this, &StrategyPlan::StrategyPlan_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &StrategyPlan::StrategyPlan_FormClosed);
			this->groupBox1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	protected: virtual void WndProc( Message% m ) override{
				if(m.Msg == _GetEventMsgID()) 
				{
					switch(m.WParam.ToInt32())
					{
					case CBaseBusinessSpi::conDownloadStrategyInstanceListRsp:
						{
							// 这里添加显示服务器策略运行方案的代码
							std::map<std::string, SStrategyInstance> mapfield;
							mapfield = m_pDataInfo->GetStrategyInstances();
							LoadStrategyList(mapfield);
						}
						break;
					case CBaseBusinessSpi::conStrategyInstanceAddRsp:
						{
							// 这里添加显示服务器策略运行方案的代码
							m_pDataInfo->GetTraderApi()->ReqDownloadStrategyInstanceList(0);
							 //int newRowIndex=dataGridView1->Rows->Add();
							 //DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
							 //UpdateRow(dr, m_pNewStrategyInstance);
						}
						break;
					case CBaseBusinessSpi::conStrategyInstanceDeleteRsp:
						{
							// 这里添加显示服务器策略运行方案的代码
							m_pDataInfo->GetTraderApi()->ReqDownloadStrategyInstanceList(0);
							 //int newRowIndex=dataGridView1->Rows->Add();
							 //DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
							 //UpdateRow(dr, m_pNewStrategyInstance);
						}
						break;
					case CBaseBusinessSpi::conStrategyInstanceResetRsp:
						{
							// 这里添加显示服务器策略运行方案的代码
							m_pDataInfo->GetTraderApi()->ReqDownloadStrategyInstanceList(0);
							 //int newRowIndex=dataGridView1->Rows->Add();
							 //DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
							 //UpdateRow(dr, m_pNewStrategyInstance);
						}
						break;
					case CBaseBusinessSpi::conStrategyInstanceRunStopRsp:
						{
							// 这里添加显示服务器策略运行方案的代码
							m_pDataInfo->GetTraderApi()->ReqDownloadStrategyInstanceList(0);
							 //int newRowIndex=dataGridView1->Rows->Add();
							 //DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
							 //UpdateRow(dr, m_pNewStrategyInstance);
						}
						break;
					};
				}
				Form::WndProc ( m );
			}
	private: System::Void StrategyPlan_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 m_pDataInfo->SubscribeEvent(CBaseBusinessSpi::conDownloadStrategyInstanceListRsp, 
					 (HWND)this->Handle.ToPointer());
				 m_pDataInfo->SubscribeEvent(CBaseBusinessSpi::conStrategyInstanceAddRsp, 
					 (HWND)this->Handle.ToPointer());
				 m_pDataInfo->SubscribeEvent(CBaseBusinessSpi::conStrategyInstanceDeleteRsp, 
					 (HWND)this->Handle.ToPointer());
				 m_pDataInfo->SubscribeEvent(CBaseBusinessSpi::conStrategyInstanceResetRsp, 
					 (HWND)this->Handle.ToPointer());
				 m_pDataInfo->SubscribeEvent(CBaseBusinessSpi::conStrategyInstanceRunStopRsp, 
					 (HWND)this->Handle.ToPointer());
				 m_pDataInfo->GetTraderApi()->ReqDownloadStrategyInstanceList(0);


			 }
	private: System::Void StrategyPlan_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
				 m_pDataInfo->UnsubscribeAllEvent((HWND)this->Handle.ToPointer());
			 }

			 //加载当前登录交易员所加载使用的策略列表
private: System::Void  LoadStrategyList(std::map<std::string, SStrategyInstance>& lMapFiled)
		 {
			 dataGridView1->Rows->Clear();
			 dataGridView1->Refresh();
			 std::map<std::string, CStrategyDataEx>   MapStrategy;
			 m_pDataInfo->GetAllStrategy_Download(MapStrategy);

			 std::map<std::string, SStrategyInstance>::iterator it = lMapFiled.begin();
			 for ( it;it != lMapFiled.end(); it++ )
			 {
				 std::map<std::string, CStrategyDataEx>::iterator its = MapStrategy.find(it->first);
				 if (its!= MapStrategy.end() && its->second.GetServerUseStatus()==1)
				 {

					 int newRowIndex=dataGridView1->Rows->Add();
					 DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
					 dr->Cells[0]->Value = Tools::string2String(it->second.strategyName);
					 String^ instrumet = Tools::string2String(it->second.Instruments);
					 instrumet = instrumet->Replace(",", "\r\n");
					 dr->Cells[1]->Value = instrumet;
					 //array<System::String ^>^sArray =gcnew array<System::String ^>{};
					 //sArray = instrumet->Split(',');
					 //String ^ MultiLineIns;
					 //for each(String^ s in sArray)
					 //{
					 // MultiLineIns += s +"\r\n";

					 //}
					 //dr->Cells[1]->Value =MultiLineIns;
					 if (it->second.nStart == 1 )
					 {
						 dr->Cells[2]->Value = "停止";
					 }
					 else //if (it->second.nStart == 0 )
					 {
						 dr->Cells[2]->Value = "运行";
					 }

				 }
			 }


		 }

private: System::Void button_add_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(m_pDataInfo->GetLoginTraderStatus() != CDataManageEx::conLogon) 
			 {
				 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"未登录到策略服务器。", "错误", MessageBoxButtons::OK);
				 return;
			 }
			 StrategyLoadOption^ dlg = gcnew StrategyLoadOption();

			 if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
			 {
				 SStrategyInstance field;
				 memset(&field,0,sizeof(field));

				 field=*(dlg->GetStrategyInstance());	

				 std::map<std::string, SStrategyInstance> mapfield;
				 mapfield = m_pDataInfo->GetStrategyInstances();
				 std::map<std::string, SStrategyInstance>::iterator it = mapfield.find(field.strategyName);
				 if (it!= mapfield.end())
				 {
					 MessageBox::Show("同名策略实例已存在，创建失败。");
					 return;
				 }



				 //向服务器提交新增策略实例，成功后UpdateRow

				 m_pDataInfo->GetTraderApi()->ReqAddStrategyInstance(
						std::string(field.strategyName), field, 0);
				 *m_pNewStrategyInstance = field;

				 /*int newRowIndex=dataGridView1->Rows->Add();
				 DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
				 UpdateRow(dr,field);*/
				 
			 }
		 }
private: System::Void button_delete_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(m_pDataInfo->GetLoginTraderStatus() != CDataManageEx::conLogon) 
			 {
				 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"未登录到策略服务器。", "错误", MessageBoxButtons::OK);
				 return;
			 }
			 if(dataGridView1->CurrentRow == nullptr) {
				 return;
			 }

             //策略名
			 string strategyName = Tools::String2string(dataGridView1->CurrentRow->Cells[0]->Value->ToString());
			 //向服务器请求删除实例
			 m_pDataInfo->GetTraderApi()->ReqDeleteStrategyInstance(strategyName, 0);
		 }
//private: System::Void button_apply_Click(System::Object^  sender, System::EventArgs^  e)
//		 {
//		 }
//private: System::Void button_confirm_Click(System::Object^  sender, System::EventArgs^  e) 
//		 {
//		 }
private: System::Void button_close_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Close();
		 }
private: System::Void button_modify_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 if(m_pDataInfo->GetLoginTraderStatus() != CDataManageEx::conLogon) 
			 {
				 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"未登录到策略服务器。", "错误", MessageBoxButtons::OK);
				 return;
			 }
			 if(dataGridView1->CurrentRow == nullptr) 
			 {
				 return;
			 }
			 if(dataGridView1->CurrentRow->Cells[2]->Value->ToString() == "停止") 
			 {
				  System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"请先暂停该实例再进行修改。", "错误", MessageBoxButtons::OK);
				 return;
			 }
			 SStrategyInstance field;
			 memset(&field,0,sizeof(field));
			 //策略名
			 string strategyName = Tools::String2string(dataGridView1->CurrentRow->Cells[0]->Value->ToString());
			 m_pDataInfo->GetStrategyInstance(strategyName,field);
			 StrategyPlanCfg^ dlg = gcnew StrategyPlanCfg(&field,false);//修改一个策略方案

			 if (System::Windows::Forms::DialogResult::Yes == dlg->ShowDialog(this))
			 {
				*m_pNewStrategyInstance=*dlg->GetStrategyInstance();	
				 //向服务器请求重置/编辑实例
				m_pDataInfo->GetTraderApi()->ReqResetStrategyInstance(strategyName, *m_pNewStrategyInstance, 0);
			 }

		 }
private: System::Void dataGridView1_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
		 {
			 if(m_pDataInfo->GetLoginTraderStatus() != CDataManageEx::conLogon) 
			 {
				 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"未登录到策略服务器。", "错误", MessageBoxButtons::OK);
				 return;
			 }
             //策略名
			 string strategyName = Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[0]->Value->ToString());
			 if ( e->ColumnIndex == 2 )
			 {

				 DataGridViewButtonCell^ clickedButtonCell = (DataGridViewButtonCell^)dataGridView1->Rows[e->RowIndex]->Cells[e->ColumnIndex];
				 if ( clickedButtonCell->Value->ToString() == "停止")
				 {
					
					 dataGridView1->Rows[e->RowIndex]->Cells[e->ColumnIndex]->Value =  L"运行";
					 //向服务器请求运行实例
					 m_pDataInfo->GetTraderApi()->ReqStrategyInstance_RunStop(strategyName, false, 0);
				 }
				 else if ( clickedButtonCell->Value->ToString() == "运行")
				 {
					 dataGridView1->Rows[e->RowIndex]->Cells[e->ColumnIndex]->Value =  L"停止";
					 //向服务器请求暂停实例
					 m_pDataInfo->GetTraderApi()->ReqStrategyInstance_RunStop(strategyName, true, 0);
				 }




			 }
		 }
};
}

#pragma once
#include "Stdafx.h"
#include "DataManageEx.h"
#include "Tools.h"
#include "DataGridViewDisableButtonColumn.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltStrategy {

	/// <summary>
	/// Summary for StrategyManagement
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class StrategyManagement : public System::Windows::Forms::Form
	{
	public:
		StrategyManagement(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pDataInfo = CDataManageEx::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~StrategyManagement()
		{
			if (components)
			{
				delete components;
			}
			CDataManageEx::DestroyInstance();
		}
	private: CDataManageEx*     m_pDataInfo;

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::DataGridView^  dataGridView1;







	private: System::Windows::Forms::Button^  button_close;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_NAME;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_chnName;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_version;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_describe;
	private: System::Windows::Forms::DataGridViewButtonColumn^  Column_StratStop;
	private: CltStrategy::DataGridViewDisableButtonColumn^  Column_Controldelete;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_private;














	protected: 

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
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column_NAME = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_chnName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_version = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_describe = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_StratStop = (gcnew System::Windows::Forms::DataGridViewButtonColumn());
			this->Column_Controldelete = (gcnew CltStrategy::DataGridViewDisableButtonColumn());
			this->Column_private = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->button_close = (gcnew System::Windows::Forms::Button());
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->dataGridView1);
			this->groupBox1->Location = System::Drawing::Point(26, 12);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(658, 254);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"本人上传策略列表";
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(7) {this->Column_NAME, 
				this->Column_chnName, this->Column_version, this->Column_describe, this->Column_StratStop, this->Column_Controldelete, this->Column_private});
			this->dataGridView1->Location = System::Drawing::Point(7, 21);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->Size = System::Drawing::Size(643, 213);
			this->dataGridView1->TabIndex = 0;
			this->dataGridView1->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &StrategyManagement::dataGridView1_CellContentClick);
			// 
			// Column_NAME
			// 
			this->Column_NAME->HeaderText = L"策略英文名";
			this->Column_NAME->Name = L"Column_NAME";
			this->Column_NAME->ReadOnly = true;
			// 
			// Column_chnName
			// 
			this->Column_chnName->HeaderText = L"策略中文名称";
			this->Column_chnName->Name = L"Column_chnName";
			this->Column_chnName->ReadOnly = true;
			// 
			// Column_version
			// 
			this->Column_version->HeaderText = L"策略版本";
			this->Column_version->Name = L"Column_version";
			this->Column_version->ReadOnly = true;
			// 
			// Column_describe
			// 
			this->Column_describe->HeaderText = L"策略描述";
			this->Column_describe->Name = L"Column_describe";
			this->Column_describe->ReadOnly = true;
			// 
			// Column_StratStop
			// 
			this->Column_StratStop->HeaderText = L"禁用/启用";
			this->Column_StratStop->Name = L"Column_StratStop";
			this->Column_StratStop->ReadOnly = true;
			// 
			// Column_Controldelete
			// 
			this->Column_Controldelete->HeaderText = L"控制";
			this->Column_Controldelete->Name = L"Column_Controldelete";
			this->Column_Controldelete->ReadOnly = true;
			this->Column_Controldelete->Text = L"删除";
			// 
			// Column_private
			// 
			this->Column_private->HeaderText = L"共享/本人";
			this->Column_private->Name = L"Column_private";
			this->Column_private->ReadOnly = true;
			// 
			// button_close
			// 
			this->button_close->Location = System::Drawing::Point(583, 307);
			this->button_close->Name = L"button_close";
			this->button_close->Size = System::Drawing::Size(75, 23);
			this->button_close->TabIndex = 2;
			this->button_close->Text = L"关闭";
			this->button_close->UseVisualStyleBackColor = true;
			this->button_close->Click += gcnew System::EventHandler(this, &StrategyManagement::button_close_Click);
			// 
			// StrategyManagement
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(711, 356);
			this->Controls->Add(this->button_close);
			this->Controls->Add(this->groupBox1);
			this->Name = L"StrategyManagement";
			this->Text = L"策略管理";
			this->Load += gcnew System::EventHandler(this, &StrategyManagement::StrategyManagement_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &StrategyManagement::StrategyManagement_FormClosed);
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
					case CBaseBusinessSpi::conDownloadStrategyListRsp:
						{
							 std::map<std::string, CStrategyDataEx> lMapFiled;
							 m_pDataInfo->GetAllStrategy_Download(lMapFiled,true);
							 LoadStrategyList(lMapFiled);
						}
						break;
					case CBaseBusinessSpi::conResetStrategyUseStatusRsp:
						{
							 m_pDataInfo->GetTraderApi()->ReqDownloadStategyList(0);
						}
						break;
					case CBaseBusinessSpi::conDeleteStrategyRsp:
						{
							// 必要是处理服务器返回的删除策略是否成功的标志。
							 m_pDataInfo->GetTraderApi()->ReqDownloadStategyList(0);
						}
						break;
					};
				}
				Form::WndProc ( m );
			}
	private: System::Void StrategyManagement_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 m_pDataInfo->SubscribeEvent(CBaseBusinessSpi::conDownloadStrategyListRsp, 
					 (HWND)this->Handle.ToPointer());
				 m_pDataInfo->SubscribeEvent(CBaseBusinessSpi::conResetStrategyUseStatusRsp, 
					 (HWND)this->Handle.ToPointer());
				 m_pDataInfo->SubscribeEvent(CBaseBusinessSpi::conDeleteStrategyRsp, 
					 (HWND)this->Handle.ToPointer());
				 //m_pDataInfo->GetTraderApi()->ReqDownloadIndicatorList(0);
				 m_pDataInfo->GetTraderApi()->ReqDownloadStategyList(0);

				 this->CenterToParent();

			 }
private: System::Void StrategyManagement_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
			 m_pDataInfo->UnsubscribeAllEvent((HWND)this->Handle.ToPointer());
		 }
			 //加载自己上传的策略列表
private: System::Void  LoadStrategyList(std::map<std::string, CStrategyDataEx>& lMapFiled)
		 {
			 dataGridView1->Rows->Clear();
			 dataGridView1->Refresh();
			 std::map<std::string, CStrategyDataEx>::iterator it = lMapFiled.begin();
			 for ( it;it != lMapFiled.end(); it++ )
			 {
				 int newRowIndex=dataGridView1->Rows->Add();
				 DataGridViewRow^ dr = dataGridView1->Rows[newRowIndex];
				 dr->Cells[0]->Value = Tools::string2String(it->second.GetStrategyName().c_str());
				 dr->Cells[1]->Value = Tools::string2String(it->second.GetStrategyNickName().c_str());
				 dr->Cells[2]->Value = Tools::string2String(it->second.GetStrategyVersion().c_str());
				 dr->Cells[3]->Value = Tools::string2String(it->second.GetDescribe().c_str());
				 
                 int  nStatus = it->second.GetServerUseStatus();		

				 if (nStatus == 1)
				 {
					 dr->Cells[4]->Value =  L"禁用";
				 }
				 else //if (nStatus == 0)
				 {
					 dr->Cells[4]->Value =  L"启用";
				 }

				 dr->Cells[5]->Value =  L"删除";

				 if (it->second.GetStrategyPrivate() == 1)
				 {
					  dr->Cells[6]->Value =  L"私有";
				 }
				 else //if (it->second.GetStrategyPrivate() == 2)
				 {
					 dr->Cells[6]->Value =  L"共享";
				 }

			 }

		 }

private: System::Void button_close_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 this->Close();
		 }
private: System::Void dataGridView1_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
		 {
			 if(m_pDataInfo->GetLoginTraderStatus() != CDataManageEx::conLogon) {
				 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"未登录到策略服务器。", "错误", MessageBoxButtons::OK);
				 return;
			 }
             //策略名
			 string strategyName = Tools::String2string(dataGridView1->Rows[e->RowIndex]->Cells[0]->Value->ToString());

			 if ( e->ColumnIndex == 4 )
			 {

				 DataGridViewButtonCell^ clickedButtonCell = (DataGridViewButtonCell^)dataGridView1->Rows[e->RowIndex]->Cells[e->ColumnIndex];
				 if ( clickedButtonCell->Value->ToString() == "启用")
				 {
				     dataGridView1->Rows[e->RowIndex]->Cells[e->ColumnIndex]->Value =  L"禁用";
					 //向服务器请求禁用/启动
					 m_pDataInfo->GetTraderApi()->ReqResetStrategyUseStatus(strategyName, 1, 0);
				 }
				 else if ( clickedButtonCell->Value->ToString() == "禁用")
				 {
					 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"禁用此策略，则其他加载用户均无法使用，是否确认此操作？", "提示", MessageBoxButtons::YesNo);
					 if ( dr == System::Windows::Forms::DialogResult::Yes)
					 { 
						 dataGridView1->Rows[e->RowIndex]->Cells[e->ColumnIndex]->Value =  L"启用";
						 //向服务器请求禁用/启动
						 m_pDataInfo->GetTraderApi()->ReqResetStrategyUseStatus(strategyName, 2, 0);
					 }
				 }
			 }
			 else if ( e->ColumnIndex == 5 )
			 {
				 DataGridViewButtonCell^ clickedButtonCell = (DataGridViewButtonCell^)dataGridView1->Rows[e->RowIndex]->Cells[4];
				 if ( clickedButtonCell->Value->ToString() == "禁用")
				 {
					 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"删除失败，请先禁用策略，再进行删除操作。", "错误", MessageBoxButtons::OK);
					 if ( dr == System::Windows::Forms::DialogResult::Yes)
					 { 
					 }
				 }
				 else {
					 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"删除此策略，则其他加载用户均无法使用，是否确认此操作？", "提示", MessageBoxButtons::YesNo);
					 if ( dr == System::Windows::Forms::DialogResult::Yes)
					 { 
						 //向服务器请求删除此策略
						 m_pDataInfo->GetTraderApi()->ReqDeleteStrategy(strategyName, 0);
					 }
					 //成功后 
					 DataGridViewDisableButtonCell^ cell = (DataGridViewDisableButtonCell^)dataGridView1->Rows[e->RowIndex]->Cells[5];
					 cell->Enabled = false;
				 }
			 }
		 }
};
}

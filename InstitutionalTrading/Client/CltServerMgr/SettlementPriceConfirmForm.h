#pragma once
#include "TcpLayer.h"
#include "CommonDef.h"
#include "CommonPkg.h"
#include "CommonErrorCode.h"
#include "CDataInfo.h"
#include "Tools.h"
#include "CommonStruct.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace CltServerMgr {

	/// <summary>
	/// Summary for SettlementPriceConfirmForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class SettlementPriceConfirmForm : public System::Windows::Forms::Form
	{
	public:
		SettlementPriceConfirmForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			m_pLogin = new sLoginRsp();
			m_pDataInfo = CDataInfo::NewInstance();
			InvalideValue=util::GetDoubleInvalidValue();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SettlementPriceConfirmForm()
		{
			if (components)
			{
				delete components;
			}
			CDataInfo::DestroyInstance();
			if(m_pLogin)
			{
				delete m_pLogin;
				m_pLogin = nullptr;
			}
		}
	private: sLoginRsp*     m_pLogin;
	private: CDataInfo*     m_pDataInfo;
	private: double         InvalideValue;
	private: System::Windows::Forms::TabControl^  tabControl;
	private: System::Windows::Forms::TabPage^  tabPage_shangqi;
	private: System::Windows::Forms::TabPage^  tabPage_zhengshang;
	private: System::Windows::Forms::TabPage^  tabPage_dashang;
	private: System::Windows::Forms::TabPage^  tabPage_zhongjin;
	private: System::Windows::Forms::DataGridView^  dataGridView1;



	private: System::Windows::Forms::TreeView^  treeView1;
	private: System::Windows::Forms::Button^  button_confirm;
	private: System::Windows::Forms::DataGridView^  dataGridView2;


	private: System::Windows::Forms::TreeView^  treeView2;
	private: System::Windows::Forms::TreeView^  treeView3;
	private: System::Windows::Forms::DataGridView^  dataGridView3;


	private: System::Windows::Forms::DataGridView^  dataGridView4;
	private: System::Windows::Forms::TreeView^  treeView4;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_Ins;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_Price;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_Inszs;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_pricezs;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_Insds;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_priceds;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_Inszj;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_pricezj;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::ComponentModel::IContainer^  components;




	protected: 

	protected: 

	protected: 



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->tabControl = (gcnew System::Windows::Forms::TabControl());
			this->tabPage_shangqi = (gcnew System::Windows::Forms::TabPage());
			this->treeView1 = (gcnew System::Windows::Forms::TreeView());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->Column_Ins = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_Price = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->tabPage_zhengshang = (gcnew System::Windows::Forms::TabPage());
			this->dataGridView2 = (gcnew System::Windows::Forms::DataGridView());
			this->Column_Inszs = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_pricezs = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->treeView2 = (gcnew System::Windows::Forms::TreeView());
			this->tabPage_dashang = (gcnew System::Windows::Forms::TabPage());
			this->treeView3 = (gcnew System::Windows::Forms::TreeView());
			this->dataGridView3 = (gcnew System::Windows::Forms::DataGridView());
			this->Column_Insds = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_priceds = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->tabPage_zhongjin = (gcnew System::Windows::Forms::TabPage());
			this->dataGridView4 = (gcnew System::Windows::Forms::DataGridView());
			this->Column_Inszj = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_pricezj = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->treeView4 = (gcnew System::Windows::Forms::TreeView());
			this->button_confirm = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->tabControl->SuspendLayout();
			this->tabPage_shangqi->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			this->tabPage_zhengshang->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->BeginInit();
			this->tabPage_dashang->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView3))->BeginInit();
			this->tabPage_zhongjin->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView4))->BeginInit();
			this->SuspendLayout();
			// 
			// tabControl
			// 
			this->tabControl->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tabControl->Controls->Add(this->tabPage_shangqi);
			this->tabControl->Controls->Add(this->tabPage_zhengshang);
			this->tabControl->Controls->Add(this->tabPage_dashang);
			this->tabControl->Controls->Add(this->tabPage_zhongjin);
			this->tabControl->Location = System::Drawing::Point(12, 12);
			this->tabControl->Name = L"tabControl";
			this->tabControl->SelectedIndex = 0;
			this->tabControl->Size = System::Drawing::Size(569, 441);
			this->tabControl->TabIndex = 0;
			this->tabControl->SelectedIndexChanged += gcnew System::EventHandler(this, &SettlementPriceConfirmForm::tabControl_SelectedIndexChanged);
			// 
			// tabPage_shangqi
			// 
			this->tabPage_shangqi->Controls->Add(this->treeView1);
			this->tabPage_shangqi->Controls->Add(this->dataGridView1);
			this->tabPage_shangqi->Location = System::Drawing::Point(4, 21);
			this->tabPage_shangqi->Name = L"tabPage_shangqi";
			this->tabPage_shangqi->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_shangqi->Size = System::Drawing::Size(561, 416);
			this->tabPage_shangqi->TabIndex = 0;
			this->tabPage_shangqi->Text = L"上期";
			this->tabPage_shangqi->UseVisualStyleBackColor = true;
			// 
			// treeView1
			// 
			this->treeView1->Location = System::Drawing::Point(6, 6);
			this->treeView1->Name = L"treeView1";
			this->treeView1->Size = System::Drawing::Size(172, 390);
			this->treeView1->TabIndex = 1;
			this->treeView1->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &SettlementPriceConfirmForm::treeView1_AfterSelect);
			this->treeView1->NodeMouseClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &SettlementPriceConfirmForm::treeView1_NodeMouseClick);
			this->treeView1->Click += gcnew System::EventHandler(this, &SettlementPriceConfirmForm::treeView1_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {this->Column_Ins, 
				this->Column_Price});
			this->dataGridView1->ImeMode = System::Windows::Forms::ImeMode::Disable;
			this->dataGridView1->Location = System::Drawing::Point(201, 6);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->Size = System::Drawing::Size(333, 404);
			this->dataGridView1->TabIndex = 0;
			this->dataGridView1->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &SettlementPriceConfirmForm::dataGridView1_CellEndEdit);
			// 
			// Column_Ins
			// 
			this->Column_Ins->HeaderText = L"合约名称";
			this->Column_Ins->Name = L"Column_Ins";
			this->Column_Ins->ReadOnly = true;
			this->Column_Ins->Width = 78;
			// 
			// Column_Price
			// 
			this->Column_Price->HeaderText = L"结算价";
			this->Column_Price->Name = L"Column_Price";
			this->Column_Price->Width = 66;
			// 
			// tabPage_zhengshang
			// 
			this->tabPage_zhengshang->Controls->Add(this->dataGridView2);
			this->tabPage_zhengshang->Controls->Add(this->treeView2);
			this->tabPage_zhengshang->Location = System::Drawing::Point(4, 21);
			this->tabPage_zhengshang->Name = L"tabPage_zhengshang";
			this->tabPage_zhengshang->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_zhengshang->Size = System::Drawing::Size(561, 416);
			this->tabPage_zhengshang->TabIndex = 1;
			this->tabPage_zhengshang->Text = L"郑商";
			this->tabPage_zhengshang->UseVisualStyleBackColor = true;
			// 
			// dataGridView2
			// 
			this->dataGridView2->AllowUserToAddRows = false;
			this->dataGridView2->AllowUserToDeleteRows = false;
			this->dataGridView2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView2->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView2->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView2->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {this->Column_Inszs, 
				this->Column_pricezs});
			this->dataGridView2->ImeMode = System::Windows::Forms::ImeMode::Disable;
			this->dataGridView2->Location = System::Drawing::Point(157, 6);
			this->dataGridView2->Name = L"dataGridView2";
			this->dataGridView2->RowHeadersVisible = false;
			this->dataGridView2->RowTemplate->Height = 23;
			this->dataGridView2->Size = System::Drawing::Size(348, 393);
			this->dataGridView2->TabIndex = 1;
			this->dataGridView2->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &SettlementPriceConfirmForm::dataGridView2_CellEndEdit);
			// 
			// Column_Inszs
			// 
			this->Column_Inszs->HeaderText = L"合约名称";
			this->Column_Inszs->Name = L"Column_Inszs";
			this->Column_Inszs->ReadOnly = true;
			this->Column_Inszs->Width = 78;
			// 
			// Column_pricezs
			// 
			this->Column_pricezs->HeaderText = L"结算价";
			this->Column_pricezs->Name = L"Column_pricezs";
			this->Column_pricezs->Width = 66;
			// 
			// treeView2
			// 
			this->treeView2->Location = System::Drawing::Point(6, 6);
			this->treeView2->Name = L"treeView2";
			this->treeView2->Size = System::Drawing::Size(136, 393);
			this->treeView2->TabIndex = 0;
			this->treeView2->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &SettlementPriceConfirmForm::treeView2_AfterSelect);
			this->treeView2->NodeMouseClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &SettlementPriceConfirmForm::treeView2_NodeMouseClick);
			// 
			// tabPage_dashang
			// 
			this->tabPage_dashang->Controls->Add(this->treeView3);
			this->tabPage_dashang->Controls->Add(this->dataGridView3);
			this->tabPage_dashang->Location = System::Drawing::Point(4, 21);
			this->tabPage_dashang->Name = L"tabPage_dashang";
			this->tabPage_dashang->Size = System::Drawing::Size(561, 416);
			this->tabPage_dashang->TabIndex = 2;
			this->tabPage_dashang->Text = L"大商";
			this->tabPage_dashang->UseVisualStyleBackColor = true;
			// 
			// treeView3
			// 
			this->treeView3->Location = System::Drawing::Point(3, 3);
			this->treeView3->Name = L"treeView3";
			this->treeView3->Size = System::Drawing::Size(170, 383);
			this->treeView3->TabIndex = 1;
			this->treeView3->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &SettlementPriceConfirmForm::treeView3_AfterSelect);
			this->treeView3->NodeMouseClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &SettlementPriceConfirmForm::treeView3_NodeMouseClick);
			// 
			// dataGridView3
			// 
			this->dataGridView3->AllowUserToAddRows = false;
			this->dataGridView3->AllowUserToDeleteRows = false;
			this->dataGridView3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView3->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView3->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView3->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {this->Column_Insds, 
				this->Column_priceds});
			this->dataGridView3->ImeMode = System::Windows::Forms::ImeMode::Disable;
			this->dataGridView3->Location = System::Drawing::Point(179, 3);
			this->dataGridView3->Name = L"dataGridView3";
			this->dataGridView3->RowHeadersVisible = false;
			this->dataGridView3->RowTemplate->Height = 23;
			this->dataGridView3->Size = System::Drawing::Size(368, 383);
			this->dataGridView3->TabIndex = 0;
			this->dataGridView3->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &SettlementPriceConfirmForm::dataGridView3_CellEndEdit);
			// 
			// Column_Insds
			// 
			this->Column_Insds->HeaderText = L"合约名称";
			this->Column_Insds->Name = L"Column_Insds";
			this->Column_Insds->ReadOnly = true;
			this->Column_Insds->Width = 78;
			// 
			// Column_priceds
			// 
			this->Column_priceds->HeaderText = L"结算价";
			this->Column_priceds->Name = L"Column_priceds";
			this->Column_priceds->Width = 66;
			// 
			// tabPage_zhongjin
			// 
			this->tabPage_zhongjin->Controls->Add(this->dataGridView4);
			this->tabPage_zhongjin->Controls->Add(this->treeView4);
			this->tabPage_zhongjin->Location = System::Drawing::Point(4, 21);
			this->tabPage_zhongjin->Name = L"tabPage_zhongjin";
			this->tabPage_zhongjin->Size = System::Drawing::Size(561, 416);
			this->tabPage_zhongjin->TabIndex = 3;
			this->tabPage_zhongjin->Text = L"中金";
			this->tabPage_zhongjin->UseVisualStyleBackColor = true;
			// 
			// dataGridView4
			// 
			this->dataGridView4->AllowUserToAddRows = false;
			this->dataGridView4->AllowUserToDeleteRows = false;
			this->dataGridView4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView4->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView4->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView4->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(2) {this->Column_Inszj, 
				this->Column_pricezj});
			this->dataGridView4->ImeMode = System::Windows::Forms::ImeMode::Disable;
			this->dataGridView4->Location = System::Drawing::Point(185, 4);
			this->dataGridView4->Name = L"dataGridView4";
			this->dataGridView4->RowHeadersVisible = false;
			this->dataGridView4->RowTemplate->Height = 23;
			this->dataGridView4->Size = System::Drawing::Size(373, 396);
			this->dataGridView4->TabIndex = 1;
			this->dataGridView4->CellLeave += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &SettlementPriceConfirmForm::dataGridView4_CellLeave);
			this->dataGridView4->CellEndEdit += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &SettlementPriceConfirmForm::dataGridView4_CellEndEdit);
			// 
			// Column_Inszj
			// 
			this->Column_Inszj->HeaderText = L"合约名称";
			this->Column_Inszj->Name = L"Column_Inszj";
			this->Column_Inszj->ReadOnly = true;
			this->Column_Inszj->Width = 78;
			// 
			// Column_pricezj
			// 
			this->Column_pricezj->HeaderText = L"结算价";
			this->Column_pricezj->Name = L"Column_pricezj";
			this->Column_pricezj->Width = 66;
			// 
			// treeView4
			// 
			this->treeView4->Location = System::Drawing::Point(3, 3);
			this->treeView4->Name = L"treeView4";
			this->treeView4->Size = System::Drawing::Size(166, 397);
			this->treeView4->TabIndex = 0;
			this->treeView4->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &SettlementPriceConfirmForm::treeView4_AfterSelect);
			this->treeView4->NodeMouseClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &SettlementPriceConfirmForm::treeView4_NodeMouseClick);
			// 
			// button_confirm
			// 
			this->button_confirm->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->button_confirm->Location = System::Drawing::Point(243, 475);
			this->button_confirm->Name = L"button_confirm";
			this->button_confirm->Size = System::Drawing::Size(75, 23);
			this->button_confirm->TabIndex = 1;
			this->button_confirm->Text = L"确认结算价";
			this->button_confirm->UseVisualStyleBackColor = true;
			this->button_confirm->Click += gcnew System::EventHandler(this, &SettlementPriceConfirmForm::button_confirm_Click);
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &SettlementPriceConfirmForm::timer1_Tick);
			// 
			// SettlementPriceConfirmForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(604, 510);
			this->Controls->Add(this->button_confirm);
			this->Controls->Add(this->tabControl);
			this->Name = L"SettlementPriceConfirmForm";
			this->Text = L"结算价确认";
			this->Load += gcnew System::EventHandler(this, &SettlementPriceConfirmForm::SettlementPriceConfirmForm_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &SettlementPriceConfirmForm::SettlementPriceConfirmForm_FormClosed);
			this->tabControl->ResumeLayout(false);
			this->tabPage_shangqi->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			this->tabPage_zhengshang->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->EndInit();
			this->tabPage_dashang->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView3))->EndInit();
			this->tabPage_zhongjin->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView4))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void SettlementPriceConfirmForm_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->CenterToParent(); 
				 int nHwnd = this->Handle.ToInt32();
				 //CTcpLayer::SetMainWndHandle(nHwnd);

				 m_pDataInfo->GetLogonInfo(*m_pLogin);
				 /*CTcpLayer::SubscribePkg(Cmd_Settlement_QuerySettlementPrice_Rsp, nHwnd);*/
				 CTcpLayer::SubscribePkg(Cmd_Settlement_ModifySettlementPrice_Rsp, nHwnd); 
				 CTcpLayer::SubscribePkg(Cmd_Settlement_ConfirmSettlementPrice_Rsp, nHwnd);
				 /* if ( !CTcpLayer::SendData(Cmd_Settlement_QuerySettlementPrice_Req, NULL, 0, 0, m_pLogin->mnUserID))
				 {
				 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
				 }*/
				 dataGridView1->Columns[0]->ReadOnly = true;
				 dataGridView2->Columns[0]->ReadOnly = true;
				 dataGridView3->Columns[0]->ReadOnly = true;
				 dataGridView4->Columns[0]->ReadOnly = true;

				 std::set<string> setProducts;
				 int ntab=this->tabControl->SelectedIndex;
				 m_pDataInfo->GetAllProducts(setProducts,ntab);
				 TreeNode^ node;
				 set<string>::iterator it;
				 for(it = setProducts.begin(); it != setProducts.end(); it++)
				 {
					 String^ strText = Tools::string2String((*it).c_str());
					 node = treeView1->Nodes->Add(strText, strText);
				 }
    //            //结算价不全时确认按钮灰显
				//if(!m_pDataInfo->CheckSettlePrice())
				//   button_confirm->Enabled=false;
				timer1->Interval = 1000;

			 }

	protected:virtual void WndProc( Message% m ) override
			  {
				  if (  m.Msg == WM_USER_Win && m.WParam.ToInt32()==WndUserCmd_YourPkgArrival )
				  {
					  char *p = (char*)m.LParam.ToInt32();
					  Stru_UniPkgHead head;					
					  memset(&head, 0, sizeof(head));
					  memcpy(&head, p, sizeof(head));

					  switch ( head.cmdid )
					  {
						  //case Cmd_Settlement_QuerySettlementPrice_Rsp:
						  // {
						  //  if ( head.userdata1 == CF_ERROR_SUCCESS )
						  //  {
						  //	  std::map<std::string,SettlementPriceField>  lMapFiled;
						  //	  int nCount = head.len / sizeof(SettlementPriceField);
						  //	  if(nCount == 0)
						  //		  break;
						  //	  for ( int i = 0; i < nCount; i++ )
						  //	  {
						  //		  SettlementPriceField* pField = (SettlementPriceField*)((char*)(p+sizeof(Stru_UniPkgHead)) + i*sizeof(SettlementPriceField));
						  //		  lMapFiled[pField->InstrumentID] = *pField;
						  //	  }

						  //	  m_pDataInfo->SetOriginalSettlementPrice(lMapFiled);
						  //	  std::set<string> setProducts;
						  //	  int ntab=this->tabControl->SelectedIndex;
						  //	  m_pDataInfo->GetAllProducts(setProducts,ntab);
						  //	  TreeNode^ node;
						  //	  set<string>::iterator it;
						  //	  for(it = setProducts.begin(); it != setProducts.end(); it++)
						  //	  {
						  //		  String^ strText = Tools::string2String((*it).c_str());
						  //		  node = treeView1->Nodes->Add(strText, strText);
						  //	  }
						  //  }
						  //  else
						  //  {
						  //	  string str = (char*)(p+sizeof(Stru_UniPkgHead));
						  //	  String^ strtemp = Tools::string2String(str.c_str());
						  //	  m_pDataInfo->PopMsgShow(str.c_str());
						  //  }

						  //  break;
						  // }
					  case Cmd_Settlement_ModifySettlementPrice_Rsp:
						  {
							  if ( head.userdata1 == CF_ERROR_SUCCESS )
							  {

								  SettlementPriceField Field = *(SettlementPriceField*)(p+sizeof(Stru_UniPkgHead));
								  m_pDataInfo->ModifySettlementPrice(Field);

								  //if(m_pDataInfo->CheckSettlePrice())
									 // button_confirm->Enabled=true;
								  //else
									 // button_confirm->Enabled=false;

							  }
							  else
							  {
								  string str = (char*)(p+sizeof(Stru_UniPkgHead));
								  String^ strtemp = Tools::string2String(str.c_str());
								  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
									  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
									  str.c_str());
							  }
							 
							  break;
						  }
					  case Cmd_Settlement_ConfirmSettlementPrice_Rsp:
						  {
							  if ( head.userdata1 == CF_ERROR_SUCCESS )
							  {

								  this->DialogResult = System::Windows::Forms::DialogResult::Yes;
								  this->Close();

							  }
							  else
							  {
								  string str = (char*)(p+sizeof(Stru_UniPkgHead));
								  String^ strtemp = Tools::string2String(str.c_str());
								  _GetPopMsg()->AppendStr(CPopMsgWin::ForceShow, CPopMsgWin::NoDelOldContent, 
									  CPopMsgWin::AddCRLF, CPopMsgWin::InsertTime, 
									  str.c_str());
							  }

							  break;
						  }

					  default:
						  break;
					  }

				  }

				  Form::WndProc ( m );
			  }
	private: System::Void treeView1_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
			 {
				 String^ strText = e->Node->Text;

				 std::vector<SettlementPriceField> lvecField;
				 m_pDataInfo->GetSettlementPrice(lvecField,Tools::String2string(strText).c_str());

				 LoadSettlementPrice( dataGridView1,lvecField );

			 }
	private: System::Void treeView1_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
			 }
	private: System::Void tabControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
			 {
				 std::set<string> setProducts;
				 int ntab=this->tabControl->SelectedIndex;
				 m_pDataInfo->GetAllProducts(setProducts,ntab);
				 TreeNode^ node;
				 set<string>::iterator it;

				 switch(ntab)
				 {
				 case 0:
					 treeView1->Nodes->Clear();
					 for(it = setProducts.begin(); it != setProducts.end(); it++)
					 {
						 String^ strText = Tools::string2String((*it).c_str());
						 node = treeView1->Nodes->Add(strText, strText);
					 }
					 break;
				 case 1:
					 treeView2->Nodes->Clear();
					 for(it = setProducts.begin(); it != setProducts.end(); it++)
					 {
						 String^ strText = Tools::string2String((*it).c_str());
						 node = treeView2->Nodes->Add(strText, strText);
					 }
					 break;
				 case 2:
					 treeView3->Nodes->Clear();
					 for(it = setProducts.begin(); it != setProducts.end(); it++)
					 {
						 String^ strText = Tools::string2String((*it).c_str());
						 node = treeView3->Nodes->Add(strText, strText);
					 }
					 break;
				 case 3:
					 treeView4->Nodes->Clear();
					 for(it = setProducts.begin(); it != setProducts.end(); it++)
					 {
						 String^ strText = Tools::string2String((*it).c_str());
						 node = treeView4->Nodes->Add(strText, strText);
					 }
					 break;
				 }

			 }
	protected: System::Void LoadSettlementPrice(DataGridView^ dataGridView,std::vector<SettlementPriceField>& lvecField )
			   {
				   dataGridView->Rows->Clear();
				   std::vector<SettlementPriceField>::iterator it;
				   for(it = lvecField.begin(); it != lvecField.end(); it++)
				   {

					   int newRowIndex=dataGridView->Rows->Add();
					   DataGridViewRow^ dr = dataGridView->Rows[newRowIndex];
					   dr->Cells[0]->Value = Tools::string2String((*it).InstrumentID);
					   if ((*it).SettlementPrice != InvalideValue)
					   {
						   char strPrice[128];					  
						   sprintf(strPrice, "%.3f",( *it).SettlementPrice);
						   dr->Cells[1]->Value = Tools::string2String(strPrice);
					   }


				   }
			   }
	protected: System::Void treeView1_NodeMouseClick(System::Object^  sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^  e) 
			   {

				   /* TreeNode^ selectNode = this->treeView1->selectedNode;*/
				   /*  String^ strText = this->treeView1->selectedNode->get_Text();*/
				   //String^ strText = e->node->text;
				   String^ strText = e->Node->Text;

				   std::vector<SettlementPriceField> lvecField;
				   m_pDataInfo->GetSettlementPrice(lvecField,Tools::String2string(strText).c_str());

				   LoadSettlementPrice( dataGridView1,lvecField );

			   }
	private: System::Void treeView2_NodeMouseClick(System::Object^  sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^  e) 
			 {
				 String^ strText = e->Node->Text;

				 std::vector<SettlementPriceField> lvecField;
				 m_pDataInfo->GetSettlementPrice(lvecField,Tools::String2string(strText).c_str());

				 LoadSettlementPrice( dataGridView2,lvecField );
			 }
	private: System::Void treeView3_NodeMouseClick(System::Object^  sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^  e)
			 {
				 String^ strText = e->Node->Text;

				 std::vector<SettlementPriceField> lvecField;
				 m_pDataInfo->GetSettlementPrice(lvecField,Tools::String2string(strText).c_str());

				 LoadSettlementPrice( dataGridView3,lvecField );
			 }
	private: System::Void treeView4_NodeMouseClick(System::Object^  sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^  e) 
			 {
				 String^ strText = e->Node->Text;

				 std::vector<SettlementPriceField> lvecField;
				 m_pDataInfo->GetSettlementPrice(lvecField,Tools::String2string(strText).c_str());

				 LoadSettlementPrice( dataGridView4,lvecField );
			 }
	private: System::Void GetListData(DataGridView^ dataGridView,std::map<std::string,SettlementPriceField>& lMapFiled)
			 {
				 for ( int i=0;i<dataGridView1->RowCount;i++)
				 { 
					 SettlementPriceField Filed;
					 memset(&Filed,0,sizeof(Filed));
					 String^ strIns = dataGridView1->Rows[i]->Cells[0]->Value->ToString();
					 strncpy(Filed.InstrumentID,Tools::String2string(strIns).c_str(),sizeof(Filed.InstrumentID)-1);
					 String^ strPrice = dataGridView1->Rows[i]->Cells[1]->Value->ToString();
					 Filed.SettlementPrice = atof(Tools::String2string(strPrice).c_str());
					 lMapFiled.insert(std::make_pair(Filed.InstrumentID, Filed));

				 }
			 }
	private: System::Void button_confirm_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 
				 //检查结算价数据全不全,待测试！！！
				 string Ins="";
				 if(m_pDataInfo->CheckSettlePrice() && m_pDataInfo->CompareSettlePrice(Ins))
				 {
					 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"是否确定确认结算价？", "提示", MessageBoxButtons::YesNo);
					 if ( dr == System::Windows::Forms::DialogResult::Yes)
					 { 

						 int nHwnd = this->Handle.ToInt32();
						 m_pDataInfo->GetLogonInfo(*m_pLogin);

						 if ( !CTcpLayer::SendData(Cmd_Settlement_ConfirmSettlementPrice_Req, NULL, 0, 0, m_pLogin->mnUserID))
						 {
							 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
						 }	
					 }

				 }
				 else

					 //MessageBox::Show("结算价数据不全！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 timer1->Start();

			 }
	private: System::Void close_button_Click(System::Object^  sender, System::EventArgs^  e) 
			 {
				 this->DialogResult = System::Windows::Forms::DialogResult::No;
				 this->Close();
			 }

	private: System::Void dataGridView2_CellEndEdit(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
			 {
				 SettlementPriceField Filed;
				 memset(&Filed,0,sizeof(Filed));
				 String^ strIns = dataGridView2->Rows[e->RowIndex]->Cells[0]->Value->ToString();
				 strncpy(Filed.InstrumentID,Tools::String2string(strIns).c_str(),sizeof(Filed.InstrumentID)-1);
				 if (dataGridView2->Rows[e->RowIndex]->Cells[1]->FormattedValue == "")
				 {
					 MessageBox::Show("请输入结算价！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 return;
				 }

				 String^ strPrice = dataGridView2->Rows[e->RowIndex]->Cells[1]->Value->ToString();
				 Filed.SettlementPrice = atof(Tools::String2string(strPrice).c_str());
				 double OriginalPrice = m_pDataInfo->GetSettlementPrice(Filed.InstrumentID);
				 if ( OriginalPrice == Filed.SettlementPrice )
					 return;

				 if ( Filed.SettlementPrice < 0 )
				 {
					 MessageBox::Show("请输入正确的结算价！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 return;
				 }

				 strncpy(Filed.ExchangeID,"CZCE",sizeof(Filed.ExchangeID)-1);
				 if (treeView2->SelectedNode != nullptr)
				 {
					 String^ strProduct = treeView2->SelectedNode->Name->ToString();
					 strncpy(Filed.ProductID,Tools::String2string(strProduct).c_str(),sizeof(Filed.ProductID)-1);
				 }

				 m_pDataInfo->GetLogonInfo(*m_pLogin);
				 if( OriginalPrice != InvalideValue )
				 {
					 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"是否确定修改结算价？", "提示", MessageBoxButtons::YesNo);
					 if ( dr == System::Windows::Forms::DialogResult::Yes)
					 { 
						 if ( !CTcpLayer::SendData(Cmd_Settlement_ModifySettlementPrice_Req, &Filed, sizeof(Filed), 0, m_pLogin->mnUserID))
						 {
							 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
						 }
					 }
				 }
				 else
				 {
					 if ( !CTcpLayer::SendData(Cmd_Settlement_ModifySettlementPrice_Req, &Filed, sizeof(Filed), 0, m_pLogin->mnUserID))
					 {
						 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 }
				 }



				m_pDataInfo->ModifyLocalPrice(Filed);
			 }

	private: System::Void dataGridView1_CellEndEdit(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
			 { 
				 SettlementPriceField Filed;
				 memset(&Filed,0,sizeof(Filed));
				 String^ strIns = dataGridView1->Rows[e->RowIndex]->Cells[0]->Value->ToString();
				 strncpy(Filed.InstrumentID,Tools::String2string(strIns).c_str(),sizeof(Filed.InstrumentID)-1);
				 if (dataGridView1->Rows[e->RowIndex]->Cells[1]->FormattedValue == "")
				 {
					 MessageBox::Show("请输入结算价！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 return;
				 }
				 String^ strPrice = dataGridView1->Rows[e->RowIndex]->Cells[1]->Value->ToString();
				 Filed.SettlementPrice = atof(Tools::String2string(strPrice).c_str());
				 double OriginalPrice = m_pDataInfo->GetSettlementPrice(Filed.InstrumentID);
				 if ( OriginalPrice == Filed.SettlementPrice )
					 return;

				 if ( Filed.SettlementPrice < 0 )
				 {
					 MessageBox::Show("请输入正确的结算价！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 return;
				 }

				 strncpy(Filed.ExchangeID,"SHFE",sizeof(Filed.ExchangeID)-1);
				 if (treeView1->SelectedNode != nullptr)
				 {
					 String^ strProduct = treeView1->SelectedNode->Name->ToString();
					 strncpy(Filed.ProductID,Tools::String2string(strProduct).c_str(),sizeof(Filed.ProductID)-1);
				 }


				 m_pDataInfo->GetLogonInfo(*m_pLogin);	
				 if( OriginalPrice != InvalideValue )
				 {
					 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"是否确定修改结算价？", "提示", MessageBoxButtons::YesNo);
					 if ( dr == System::Windows::Forms::DialogResult::Yes)
					 { 
						 if ( !CTcpLayer::SendData(Cmd_Settlement_ModifySettlementPrice_Req, &Filed, sizeof(Filed), 0, m_pLogin->mnUserID))
						 {
							 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
						 }
					 }
				 }
				 else
				 {
					 if ( !CTcpLayer::SendData(Cmd_Settlement_ModifySettlementPrice_Req, &Filed, sizeof(Filed), 0, m_pLogin->mnUserID))
					 {
						 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 }
				 }

				m_pDataInfo->ModifyLocalPrice(Filed);
			 }
	private: System::Void dataGridView3_CellEndEdit(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e)
			 {
				 SettlementPriceField Filed;
				 memset(&Filed,0,sizeof(Filed));
				 String^ strIns = dataGridView3->Rows[e->RowIndex]->Cells[0]->Value->ToString();
				 strncpy(Filed.InstrumentID,Tools::String2string(strIns).c_str(),sizeof(Filed.InstrumentID)-1);
				 if (dataGridView3->Rows[e->RowIndex]->Cells[1]->FormattedValue == "")
				 {
					 MessageBox::Show("请输入结算价！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 return;
				 }
				 String^ strPrice = dataGridView3->Rows[e->RowIndex]->Cells[1]->Value->ToString();
				 Filed.SettlementPrice = atof(Tools::String2string(strPrice).c_str());
				 double OriginalPrice = m_pDataInfo->GetSettlementPrice(Filed.InstrumentID);
				 if ( OriginalPrice == Filed.SettlementPrice )
					 return;

				 if ( Filed.SettlementPrice < 0 )
				 {
					 MessageBox::Show("请输入正确的结算价！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 return;
				 }

				 strncpy(Filed.ExchangeID,"DCE",sizeof(Filed.ExchangeID)-1);
				 if (treeView3->SelectedNode != nullptr)
				 {
					 String^ strProduct = treeView3->SelectedNode->Name->ToString();
					 strncpy(Filed.ProductID,Tools::String2string(strProduct).c_str(),sizeof(Filed.ProductID)-1);
				 }

				 m_pDataInfo->GetLogonInfo(*m_pLogin);	
				 if( OriginalPrice != InvalideValue )
				 {
					 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"是否确定修改结算价？", "提示", MessageBoxButtons::YesNo);
					 if ( dr == System::Windows::Forms::DialogResult::Yes)
					 { 
						 if ( !CTcpLayer::SendData(Cmd_Settlement_ModifySettlementPrice_Req, &Filed, sizeof(Filed), 0, m_pLogin->mnUserID))
						 {
							 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
						 }
					 }
				 }
				 else
				 {
					 if ( !CTcpLayer::SendData(Cmd_Settlement_ModifySettlementPrice_Req, &Filed, sizeof(Filed), 0, m_pLogin->mnUserID))
					 {
						 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 }
				 }

				m_pDataInfo->ModifyLocalPrice(Filed);
			 }
	private: System::Void dataGridView4_CellEndEdit(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) 
			 {
				 SettlementPriceField Filed;
				 memset(&Filed,0,sizeof(Filed));
				 String^ strIns = dataGridView4->Rows[e->RowIndex]->Cells[0]->Value->ToString();
				 strncpy(Filed.InstrumentID,Tools::String2string(strIns).c_str(),sizeof(Filed.InstrumentID)-1);
				 if (dataGridView4->Rows[e->RowIndex]->Cells[1]->FormattedValue == "")
				 {
					 MessageBox::Show("请输入结算价！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 return;
				 }
				 String^ strPrice = dataGridView4->Rows[e->RowIndex]->Cells[1]->Value->ToString();
				 Filed.SettlementPrice = atof(Tools::String2string(strPrice).c_str());
				 double OriginalPrice = m_pDataInfo->GetSettlementPrice(Filed.InstrumentID);
				 if ( OriginalPrice == Filed.SettlementPrice )
					 return;
				 if ( Filed.SettlementPrice < 0 )
				 {
					 MessageBox::Show("请输入正确的结算价！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 return;
				 }

				 strncpy(Filed.ExchangeID,"CFFEX",sizeof(Filed.ExchangeID)-1);
				 if (treeView4->SelectedNode != nullptr)
				 {
					 String^ strProduct = treeView4->SelectedNode->Name->ToString();
					 strncpy(Filed.ProductID,Tools::String2string(strProduct).c_str(),sizeof(Filed.ProductID)-1);
				 }


				 m_pDataInfo->GetLogonInfo(*m_pLogin);	
				 if( OriginalPrice != InvalideValue )
				 {
					 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"是否确定修改结算价？", "提示", MessageBoxButtons::YesNo);
					 if ( dr == System::Windows::Forms::DialogResult::Yes)
					 { 
						 if ( !CTcpLayer::SendData(Cmd_Settlement_ModifySettlementPrice_Req, &Filed, sizeof(Filed), 0, m_pLogin->mnUserID))
						 {
							 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
						 }
						 
					 }
				 }
				 else
				 {
					 if ( !CTcpLayer::SendData(Cmd_Settlement_ModifySettlementPrice_Req, &Filed, sizeof(Filed), 0, m_pLogin->mnUserID))
					 {
						 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 }
					 
				 }

				 m_pDataInfo->ModifyLocalPrice(Filed);
			 }

	private: System::Void SettlementPriceConfirmForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
			 {
				 IntPtr hWnd=this->Handle;


			 }
	private: System::Void treeView4_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
			 {
				 String^ strText = e->Node->Text;

				 std::vector<SettlementPriceField> lvecField;
				 m_pDataInfo->GetSettlementPrice(lvecField,Tools::String2string(strText).c_str());

				 LoadSettlementPrice( dataGridView4,lvecField );
			 }
private: System::Void treeView2_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
		 {
			 String^ strText = e->Node->Text;

			 std::vector<SettlementPriceField> lvecField;
			 m_pDataInfo->GetSettlementPrice(lvecField,Tools::String2string(strText).c_str());

			 LoadSettlementPrice( dataGridView2,lvecField );
		 }
private: System::Void treeView3_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
		 {
			 String^ strText = e->Node->Text;

			 std::vector<SettlementPriceField> lvecField;
			 m_pDataInfo->GetSettlementPrice(lvecField,Tools::String2string(strText).c_str());

			 LoadSettlementPrice( dataGridView3,lvecField );
		 }
private: System::Void dataGridView4_CellLeave(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
		 }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 timer1->Stop();
			 //检查结算价数据全不全,待测试！！！
			 string Ins="";
			 if(m_pDataInfo->CheckSettlePrice() && m_pDataInfo->CompareSettlePrice(Ins))
			 {
				 System::Windows::Forms::DialogResult dr = MessageBox::Show(this,"是否确定确认结算价？", "提示", MessageBoxButtons::YesNo);
				 if ( dr == System::Windows::Forms::DialogResult::Yes)
				 { 

					 int nHwnd = this->Handle.ToInt32();
					 m_pDataInfo->GetLogonInfo(*m_pLogin);

					 if ( !CTcpLayer::SendData(Cmd_Settlement_ConfirmSettlementPrice_Req, NULL, 0, 0, m_pLogin->mnUserID))
					 {
						 MessageBox::Show("网络连接异常，请求失败！", "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
					 }	
				 }

			 }
			 else
			 {
				 char strMsg[256];
				 memset(strMsg,0,sizeof(strMsg));
				 if ( !m_pDataInfo->CheckSettlePrice())
				 {
					sprintf(strMsg,  "结算价数据不全！");
				 }
				 if (!m_pDataInfo->CompareSettlePrice(Ins))
				 {
					 sprintf(strMsg,  "%s 合约的结算价提交服务器失败，请再次输入",Ins);
				 }
				 MessageBox::Show(Tools::string2String(strMsg), "提示", MessageBoxButtons::OK,  MessageBoxIcon::Error);
			 }

				
		 }
};
}

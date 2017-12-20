#pragma once
#include "windows.h"
#include "CommonDef.h"
#include "CommonPkg.h"
#include "TestTcpBusiness.h"
#include "TraderManager.h"
#include "CommandManager.h"
#include "Tools.h"
#include "TestCmdDef.h"
#include "orderForm.h"
#include "TraderList.h"
namespace TesterAdmin {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			mTraderManager = new CTraderManager();
			mCommandManager = new CCommandManager();
			mpsCommand = new sCommand();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
			if(mTraderManager)
				delete mTraderManager;
			if(mCommandManager)
				delete mCommandManager;
			if(mpsCommand)
				delete mpsCommand;
		}

	protected: 






	private: System::Windows::Forms::ImageList^  imageList1;




	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  文件ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  设置ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  退出ToolStripMenuItem;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton1;
	private: System::Windows::Forms::ToolStripMenuItem^  交易员ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  查看所有交易员ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton2;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton3;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton4;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton5;
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::ListView^  listView1;
	private: System::Windows::Forms::ColumnHeader^  选择;
	private: System::Windows::Forms::ColumnHeader^  交易员;
	private: System::Windows::Forms::ColumnHeader^  IP;
	private: System::Windows::Forms::ColumnHeader^  Socket;
	private: System::Windows::Forms::ColumnHeader^  备注;

	private: System::Windows::Forms::SplitContainer^  splitContainer2;
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label1;
	private: System::ComponentModel::IContainer^  components;

	private: CTraderManager*  mTraderManager;
	private: sCommand*  mpsCommand;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton6;
	private: CCommandManager*  mCommandManager;


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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->imageList1 = (gcnew System::Windows::Forms::ImageList(this->components));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->文件ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->设置ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->退出ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->交易员ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->查看所有交易员ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton3 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton4 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton5 = (gcnew System::Windows::Forms::ToolStripButton());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->listView1 = (gcnew System::Windows::Forms::ListView());
			this->选择 = (gcnew System::Windows::Forms::ColumnHeader());
			this->交易员 = (gcnew System::Windows::Forms::ColumnHeader());
			this->IP = (gcnew System::Windows::Forms::ColumnHeader());
			this->Socket = (gcnew System::Windows::Forms::ColumnHeader());
			this->备注 = (gcnew System::Windows::Forms::ColumnHeader());
			this->splitContainer2 = (gcnew System::Windows::Forms::SplitContainer());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->toolStripButton6 = (gcnew System::Windows::Forms::ToolStripButton());
			this->menuStrip1->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->splitContainer2->Panel1->SuspendLayout();
			this->splitContainer2->Panel2->SuspendLayout();
			this->splitContainer2->SuspendLayout();
			this->SuspendLayout();
			// 
			// imageList1
			// 
			this->imageList1->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"imageList1.ImageStream")));
			this->imageList1->TransparentColor = System::Drawing::Color::Transparent;
			this->imageList1->Images->SetKeyName(0, L"32f7bdcc87ff7e41573532ddfd2ff46a.jpg");
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->文件ToolStripMenuItem, 
				this->交易员ToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(749, 24);
			this->menuStrip1->TabIndex = 4;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// 文件ToolStripMenuItem
			// 
			this->文件ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->设置ToolStripMenuItem, 
				this->退出ToolStripMenuItem});
			this->文件ToolStripMenuItem->Name = L"文件ToolStripMenuItem";
			this->文件ToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->文件ToolStripMenuItem->Text = L"文件";
			// 
			// 设置ToolStripMenuItem
			// 
			this->设置ToolStripMenuItem->Name = L"设置ToolStripMenuItem";
			this->设置ToolStripMenuItem->Size = System::Drawing::Size(94, 22);
			this->设置ToolStripMenuItem->Text = L"设置";
			// 
			// 退出ToolStripMenuItem
			// 
			this->退出ToolStripMenuItem->Name = L"退出ToolStripMenuItem";
			this->退出ToolStripMenuItem->Size = System::Drawing::Size(94, 22);
			this->退出ToolStripMenuItem->Text = L"退出";
			this->退出ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::退出ToolStripMenuItem_Click);
			// 
			// 交易员ToolStripMenuItem
			// 
			this->交易员ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->查看所有交易员ToolStripMenuItem});
			this->交易员ToolStripMenuItem->Name = L"交易员ToolStripMenuItem";
			this->交易员ToolStripMenuItem->Size = System::Drawing::Size(53, 20);
			this->交易员ToolStripMenuItem->Text = L"交易员";
			// 
			// 查看所有交易员ToolStripMenuItem
			// 
			this->查看所有交易员ToolStripMenuItem->Name = L"查看所有交易员ToolStripMenuItem";
			this->查看所有交易员ToolStripMenuItem->Size = System::Drawing::Size(154, 22);
			this->查看所有交易员ToolStripMenuItem->Text = L"查看所有交易员";
			this->查看所有交易员ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::查看所有交易员ToolStripMenuItem_Click);
			// 
			// toolStrip1
			// 
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->toolStripButton6, 
				this->toolStripButton1, this->toolStripButton2, this->toolStripButton3, this->toolStripButton4, this->toolStripButton5});
			this->toolStrip1->Location = System::Drawing::Point(0, 24);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->Size = System::Drawing::Size(749, 25);
			this->toolStrip1->TabIndex = 5;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// toolStripButton1
			// 
			this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton1.Image")));
			this->toolStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton1->Name = L"toolStripButton1";
			this->toolStripButton1->Size = System::Drawing::Size(109, 22);
			this->toolStripButton1->Text = L"自动分配交易员";
			this->toolStripButton1->Click += gcnew System::EventHandler(this, &Form1::toolStripButton1_Click);
			// 
			// toolStripButton2
			// 
			this->toolStripButton2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton2.Image")));
			this->toolStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton2->Name = L"toolStripButton2";
			this->toolStripButton2->Size = System::Drawing::Size(85, 22);
			this->toolStripButton2->Text = L"重新初始化";
			this->toolStripButton2->Click += gcnew System::EventHandler(this, &Form1::toolStripButton2_Click);
			// 
			// toolStripButton3
			// 
			this->toolStripButton3->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton3.Image")));
			this->toolStripButton3->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton3->Name = L"toolStripButton3";
			this->toolStripButton3->Size = System::Drawing::Size(49, 22);
			this->toolStripButton3->Text = L"启动";
			this->toolStripButton3->Click += gcnew System::EventHandler(this, &Form1::toolStripButton3_Click);
			// 
			// toolStripButton4
			// 
			this->toolStripButton4->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton4.Image")));
			this->toolStripButton4->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton4->Name = L"toolStripButton4";
			this->toolStripButton4->Size = System::Drawing::Size(49, 22);
			this->toolStripButton4->Text = L"关闭";
			this->toolStripButton4->Click += gcnew System::EventHandler(this, &Form1::toolStripButton4_Click);
			// 
			// toolStripButton5
			// 
			this->toolStripButton5->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton5.Image")));
			this->toolStripButton5->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton5->Name = L"toolStripButton5";
			this->toolStripButton5->Size = System::Drawing::Size(49, 22);
			this->toolStripButton5->Text = L"下单";
			this->toolStripButton5->Click += gcnew System::EventHandler(this, &Form1::toolStripButton5_Click);
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 49);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->listView1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->splitContainer2);
			this->splitContainer1->Size = System::Drawing::Size(749, 404);
			this->splitContainer1->SplitterDistance = 311;
			this->splitContainer1->TabIndex = 6;
			// 
			// listView1
			// 
			this->listView1->CheckBoxes = true;
			this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(5) {this->选择, this->交易员, this->IP, 
				this->Socket, this->备注});
			this->listView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->listView1->Location = System::Drawing::Point(0, 0);
			this->listView1->Name = L"listView1";
			this->listView1->Size = System::Drawing::Size(311, 404);
			this->listView1->TabIndex = 0;
			this->listView1->UseCompatibleStateImageBehavior = false;
			this->listView1->View = System::Windows::Forms::View::Details;
			// 
			// 选择
			// 
			this->选择->Text = L"选择";
			// 
			// 交易员
			// 
			this->交易员->Text = L"交易员";
			// 
			// IP
			// 
			this->IP->Text = L"IP";
			// 
			// Socket
			// 
			this->Socket->Text = L"Socket";
			// 
			// 备注
			// 
			this->备注->Text = L"编号";
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
			this->splitContainer2->Panel1->Controls->Add(this->richTextBox1);
			// 
			// splitContainer2.Panel2
			// 
			this->splitContainer2->Panel2->Controls->Add(this->button1);
			this->splitContainer2->Panel2->Controls->Add(this->label1);
			this->splitContainer2->Size = System::Drawing::Size(434, 404);
			this->splitContainer2->SplitterDistance = 249;
			this->splitContainer2->TabIndex = 0;
			// 
			// richTextBox1
			// 
			this->richTextBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->richTextBox1->Location = System::Drawing::Point(0, 0);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(434, 249);
			this->richTextBox1->TabIndex = 0;
			this->richTextBox1->Text = L"";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(327, 85);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(133, 31);
			this->button1->TabIndex = 1;
			this->button1->Text = L"停止";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(15, 14);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(53, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"当前任务";
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 1000;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::TimerTick);
			// 
			// toolStripButton6
			// 
			this->toolStripButton6->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton6.Image")));
			this->toolStripButton6->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton6->Name = L"toolStripButton6";
			this->toolStripButton6->Size = System::Drawing::Size(49, 22);
			this->toolStripButton6->Text = L"全选";
			this->toolStripButton6->Click += gcnew System::EventHandler(this, &Form1::toolStripButton6_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(749, 453);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->menuStrip1);
			this->IsMdiContainer = true;
			this->Name = L"Form1";
			this->Text = L"测试总控";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->ResumeLayout(false);
			this->splitContainer2->Panel1->ResumeLayout(false);
			this->splitContainer2->Panel2->ResumeLayout(false);
			this->splitContainer2->Panel2->PerformLayout();
			this->splitContainer2->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void 设置ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
private: System::Void listView1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 //订阅链接和断开
			 int nWnd = this->Handle.ToInt32();	
			 CTestTcpBusiness::getObj().SubscribePkg(CMDID_SvrTcpDisconn, nWnd);
			 CTestTcpBusiness::getObj().SubscribePkg(CMDID_SvrTcpCon, nWnd);

			 //装载所有的交易员
			 mTraderManager->Load();

		 }


		 protected:
	virtual void WndProc( Message% m ) override{
	if (  m.Msg == WM_TCP_MESSAGE  )
	   {
		  /* char *p = (char*)m.LParam.ToInt32();
		   Stru_UniPkgHead head;					
		   memset(&head, 0, sizeof(head));
		   memcpy(&head, p, sizeof(head));*/

		   switch(m.WParam.ToInt32()) {
			case CMDID_SvrTcpDisconn:
				{
					//删除socket对应的item
					//回收分配的交易员					
					for(int i = listView1->Items->Count-1; i >= 0;i--)
					{		
						ListViewItem^ selection = listView1->Items[i];
						std::string lsUser = Tools::String2string(selection->SubItems[1]->Text);
						Int32 nShocket = Convert::ToInt32(selection->SubItems[3]->Text);
						Int32 nIdex = Convert::ToInt32(selection->SubItems[4]->Text);
						mTraderManager->SetTraderStatus(lsUser,false);
						if(selection->SubItems[3]->Text == m.LParam.ToString())
							listView1->Items->RemoveAt(i);
					}
					
				}
				break;	
			case CMDID_SvrTcpCon:
				{
					std::string lsIp;
					CTestTcpBusiness::getObj().GetIPAddress(m.LParam.ToInt32(),lsIp);
					//添加socket对应的交易员
					for(int i = 0; i < mTraderManager->mnMaxNode;i++)
					{
						ListViewItem ^ lItem = gcnew ListViewItem();
						lItem->Checked = false;						
						lItem->SubItems->Add("未分配");
						lItem->SubItems->Add(Tools::string2String(lsIp.c_str()));
						lItem->SubItems->Add(m.LParam.ToString());
						lItem->SubItems->Add(i.ToString());
						listView1->Items->Add(lItem);
					}
				
				}
				break;	
		   };
	   }
	   Form::WndProc ( m );
	}
private: System::Void toolStripButton1_Click(System::Object^  sender, System::EventArgs^  e) {
			 //自动分配交易员
			 for(int i = 0; i < listView1->Items->Count;i++)
			 {		
				 ListViewItem^ selection = listView1->Items[i];
				 if(selection->SubItems[1]->Text == "未分配")
				 {
					 TraderInfo lTraderInfo;
					 mTraderManager->FetchOneAviableTrader(lTraderInfo);
					 if(!lTraderInfo.msName.empty())
					 {
						 selection->SubItems[1]->Text = Tools::string2String(lTraderInfo.msName.c_str());
						 mTraderManager->SetTraderStatus(lTraderInfo.msName,true);

					 }
				 }
			 }
		 }
private: System::Void toolStripButton2_Click(System::Object^  sender, System::EventArgs^  e) {
			 //重新初始化
			 CTestTcpBusiness::getObj().BroadcastPkgData(CMDID_REINIT,NULL,0,0);
		 }
private: System::Void toolStripButton3_Click(System::Object^  sender, System::EventArgs^  e) {
			 //启动
			 for each(ListViewItem ^ lItem in listView1->Items)
			 {
					if(lItem->Checked != false && lItem->SubItems[1]->Text != "未分配" )
					{
						std::string lsUser = Tools::String2string(lItem->SubItems[1]->Text);
						Int32 nShocket = Convert::ToInt32(lItem->SubItems[3]->Text);
						Int32 nIndex = Convert::ToInt32(lItem->SubItems[4]->Text);
						std::string lsPass ;
						mTraderManager->GetTraderPassword(lsUser,lsPass);
						sTrader lTrader;
						strcpy(lTrader.msName,lsUser.c_str());
						strcpy(lTrader.msPass,lsPass.c_str());
						CTestTcpBusiness::getObj().SendPkgData(nShocket,CMDID_LOGIN,(void*)&lTrader,sizeof(lTrader),0,0,nShocket,nIndex);
					}
										
			 }			
			
		 }
private: System::Void toolStripButton4_Click(System::Object^  sender, System::EventArgs^  e) {
			 //关闭
			 for each(ListViewItem ^ lItem in listView1->Items)
			 {
				 if(lItem->Checked != false && lItem->SubItems[1]->Text != "未分配" )
					{
						std::string lsUser = Tools::String2string(lItem->SubItems[1]->Text);
						Int32 nShocket = Convert::ToInt32(lItem->SubItems[3]->Text);
						Int32 nIndex = Convert::ToInt32(lItem->SubItems[4]->Text);
						
						CTestTcpBusiness::getObj().SendPkgData(nShocket,CMDID_LOGOFF,(void*)lsUser.c_str(),lsUser.length(),0,0,nShocket,nIndex);
					}

			 }			
		 }
private: System::Void toolStripButton5_Click(System::Object^  sender, System::EventArgs^  e) {
			 //下单
			 OrderForm ^ ldlg = gcnew OrderForm();
			 ldlg->SetCommand(mpsCommand);
			 std::set<std::string> lsetProduct ;
			 mTraderManager->GetProductSet(lsetProduct);
			 std::set<std::string>::iterator lIter = lsetProduct.begin();
			 for(;lIter != lsetProduct.end();lIter++)
			 {
				 ldlg->AddProduct(Tools::string2String(lIter->c_str()));
			 }
			 if(::DialogResult::OK == ldlg->ShowDialog())
			 {
				 if(mCommandManager->IsRuning())
				 {
					 MessageBox::Show("上一个任务还没有执行结束","提示");
				     return;
				 }

				 mpsCommand->Clear();
				 //添加选择的用户
				 for each(ListViewItem ^ lItem in listView1->Items)
				 {
					 if(lItem->Checked != false && lItem->SubItems[1]->Text != "未分配" )
					 {
						 sUserAndSocket ltempUserAndSocket;
						 ltempUserAndSocket.msTraderName = Tools::String2string(lItem->SubItems[1]->Text);
						 ltempUserAndSocket.mnSocket = Convert::ToInt32(lItem->SubItems[3]->Text);
						 ltempUserAndSocket.mnIndex = Convert::ToInt32(lItem->SubItems[4]->Text);
						 mpsCommand->AddUserAndSocket(ltempUserAndSocket);						
					 }
				 }	
				 if(mpsCommand->msDestionTrader.size() == 0)
				 {					 
					 MessageBox::Show("没有选择用户，任务无法进行","提示");
					 return;
				 }
				 mCommandManager->SetCurrentCommand(*mpsCommand);
				 mCommandManager->Start();
				 richTextBox1->Text = "";
			 }
		 }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
			 //停止
			 if(!mCommandManager->IsRuning())
			 {
				 MessageBox::Show("当前没有正在运行的任务，无需停止","提示");
				 return;
			 }

			// mCommandManager->SetCurrentCommand(mpsCommand);
			 mCommandManager->End();
		 }
private: System::Void 查看所有交易员ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 TraderList ^ ldlg = gcnew TraderList();
			 std::map<std::string,TraderInfo> lTraderMap ;
			 mTraderManager->GetAllTraderInfo(lTraderMap);
			 std::map<std::string,TraderInfo>::iterator lIter = lTraderMap.begin();
			 for(;lIter != lTraderMap.end();lIter++)
			 {
				 ldlg->AddTrader(Tools::string2String(lIter->first.c_str()),lIter->second.mnAllocNum);
			 }
			 if(::DialogResult::OK == ldlg->ShowDialog())
			 {
				 //sCommand ls = 
			 }
		 }
private: System::Void 退出ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
private: System::Void TimerTick(System::Object^  sender, System::EventArgs^  e) {
			 //
			 std::vector<sOrderAction> lvecAction;
			 if(mCommandManager->FetchCustomOrderAction(lvecAction))		
			 {
				 std::string nDesc;
				 mCommandManager->GetDesc(nDesc);
				 label1->Text = Tools::string2String(nDesc.c_str());
				 button1->Enabled = true;
				// std::vector<sOrderAction> lvecAction;
				// if(mCommandManager->FetchCustomOrderAction(lvecAction))
				 {
					 for(int i = 0; i < lvecAction.size(); i++)
					 {
						 char lsTempDesc[1024] = {0};
						 //char ls1[10] =  lvecAction[i].mnOrder == OrderType_OrderInput ? "下单":"撤单";
						 //char ls2[10] =  lvecAction[i].mnOpenClose == OpenCloseType_Open ?"开":"平";
						 sprintf(lsTempDesc, "产生一个具体任务： 用户 %s,Socket:%d,Index:%d, %s,%s,Product:%s\n",
							 lvecAction[i].mUserName,
							 lvecAction[i].mnSocket,
							 lvecAction[i].mnIndex,
							 lvecAction[i].mnOrder == OrderType_OrderInput ? "下单":"撤单",
							 lvecAction[i].mnOpenClose == OpenCloseType_Open ?"开":"平",
							 lvecAction[i].mProduct);
						 richTextBox1->AppendText(Tools::string2String(lsTempDesc));
						 CTestTcpBusiness::getObj().SendPkgData(lvecAction[i].mnSocket,CMDID_ORDER,(void*)&lvecAction[i],sizeof(sOrderAction),0);

					 }
				 }
			 }
			 else
			 {
				// label1->Text = "";
				 button1->Enabled = false;
			 }
		 }
private: System::Void toolStripButton6_Click(System::Object^  sender, System::EventArgs^  e) {
			 if( toolStripButton6->Text == "全选")
			 {
				 for each(ListViewItem ^ lItem in listView1->Items)
				 {
					 lItem->Checked = true;					 	
				 }	
				 toolStripButton6->Text = "取消全选";			
			 }
			 else
			 {
				 for each(ListViewItem ^ lItem in listView1->Items)
				 {
					 lItem->Checked = false;					 	
				 }	
				 toolStripButton6->Text = "全选";		
			 }
			
		 }
};
}


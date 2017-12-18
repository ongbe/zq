#pragma once
#include "QueryMeaasge2.h"
#include <string>
#include <map>
#include "TcpLayer.h"
#include "Tools.h"
#include "RiskManageCmd.h"
#include "UserApiStruct.h"
#include "CDataInfo.h"


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Client {

	/// <summary>
	/// Summary for QueryMeaasge
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class QueryMeaasge : public System::Windows::Forms::Form
	{
	public:
		QueryMeaasge(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			//列表添加空行
			dataGridView1_Load();

			//日期控件只显示以前日期
			dataSet();
			m_QueryMeaasge2 = new QueryMeaasge2();
			m_pdataInfo = CDataInfo::NewInstance();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~QueryMeaasge()
		{
			if (components)
			{
				delete components;
			}
		}



	private: QueryMeaasge2*                      m_QueryMeaasge2;
	private: CDataInfo*                          m_pdataInfo;



				 


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::DateTimePicker^  dateTimePicker1;
	private: System::Windows::Forms::Button^  buttonQuerySend;

	private: System::Windows::Forms::Button^  buttonClear;
	private: System::Windows::Forms::DataGridView^  dataGridView1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnMessageID;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnTitle;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnContent;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_time;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnReceiver;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_Notreceived;
	private: System::Windows::Forms::DataGridView^  dataGridView2;
	private: System::Windows::Forms::Button^  buttonQureyRev;
	private: System::Windows::Forms::Button^  buttonClear2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  Columnid;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnTitle2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnContent2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnTime2;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  ColumnSender;
























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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->dateTimePicker1 = (gcnew System::Windows::Forms::DateTimePicker());
			this->buttonQuerySend = (gcnew System::Windows::Forms::Button());
			this->buttonClear = (gcnew System::Windows::Forms::Button());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->ColumnMessageID = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnTitle = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnContent = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_time = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnReceiver = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->Column_Notreceived = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->dataGridView2 = (gcnew System::Windows::Forms::DataGridView());
			this->Columnid = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnTitle2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnContent2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnTime2 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->ColumnSender = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->buttonQureyRev = (gcnew System::Windows::Forms::Button());
			this->buttonClear2 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(38, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(41, 12);
			this->label1->TabIndex = 0;
			this->label1->Text = L"日期：";
			// 
			// dateTimePicker1
			// 
			this->dateTimePicker1->AllowDrop = true;
			this->dateTimePicker1->Location = System::Drawing::Point(110, 13);
			this->dateTimePicker1->Name = L"dateTimePicker1";
			this->dateTimePicker1->Size = System::Drawing::Size(121, 21);
			this->dateTimePicker1->TabIndex = 2;
			this->dateTimePicker1->Value = System::DateTime(2012, 12, 28, 0, 0, 0, 0);
			// 
			// buttonQuerySend
			// 
			this->buttonQuerySend->Location = System::Drawing::Point(27, 52);
			this->buttonQuerySend->Name = L"buttonQuerySend";
			this->buttonQuerySend->Size = System::Drawing::Size(116, 23);
			this->buttonQuerySend->TabIndex = 8;
			this->buttonQuerySend->Text = L"查询我发送的消息";
			this->buttonQuerySend->UseVisualStyleBackColor = true;
			this->buttonQuerySend->Click += gcnew System::EventHandler(this, &QueryMeaasge::buttonQuery_Click);
			// 
			// buttonClear
			// 
			this->buttonClear->Location = System::Drawing::Point(202, 52);
			this->buttonClear->Name = L"buttonClear";
			this->buttonClear->Size = System::Drawing::Size(75, 23);
			this->buttonClear->TabIndex = 9;
			this->buttonClear->Text = L"清空";
			this->buttonClear->UseVisualStyleBackColor = true;
			this->buttonClear->Click += gcnew System::EventHandler(this, &QueryMeaasge::buttonClear_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView1->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView1->AutoSizeRowsMode = System::Windows::Forms::DataGridViewAutoSizeRowsMode::AllCells;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(6) {this->ColumnMessageID, 
				this->ColumnTitle, this->ColumnContent, this->Column_time, this->ColumnReceiver, this->Column_Notreceived});
			this->dataGridView1->Location = System::Drawing::Point(25, 95);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->RowTemplate->Height = 23;
			this->dataGridView1->SelectionMode = System::Windows::Forms::DataGridViewSelectionMode::FullRowSelect;
			this->dataGridView1->Size = System::Drawing::Size(515, 368);
			this->dataGridView1->TabIndex = 10;
			this->dataGridView1->CellContentClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &QueryMeaasge::dataGridView1_CellContentClick);
			// 
			// ColumnMessageID
			// 
			this->ColumnMessageID->HeaderText = L"消息ID";
			this->ColumnMessageID->Name = L"ColumnMessageID";
			this->ColumnMessageID->ReadOnly = true;
			this->ColumnMessageID->Width = 66;
			// 
			// ColumnTitle
			// 
			this->ColumnTitle->HeaderText = L"标题";
			this->ColumnTitle->Name = L"ColumnTitle";
			this->ColumnTitle->ReadOnly = true;
			this->ColumnTitle->Width = 54;
			// 
			// ColumnContent
			// 
			this->ColumnContent->HeaderText = L"内容";
			this->ColumnContent->Name = L"ColumnContent";
			this->ColumnContent->ReadOnly = true;
			this->ColumnContent->Width = 54;
			// 
			// Column_time
			// 
			this->Column_time->HeaderText = L"时间";
			this->Column_time->Name = L"Column_time";
			this->Column_time->ReadOnly = true;
			this->Column_time->Width = 54;
			// 
			// ColumnReceiver
			// 
			this->ColumnReceiver->HeaderText = L"已接收人";
			this->ColumnReceiver->Name = L"ColumnReceiver";
			this->ColumnReceiver->ReadOnly = true;
			this->ColumnReceiver->Width = 78;
			// 
			// Column_Notreceived
			// 
			this->Column_Notreceived->HeaderText = L"未接收人";
			this->Column_Notreceived->Name = L"Column_Notreceived";
			this->Column_Notreceived->ReadOnly = true;
			this->Column_Notreceived->Width = 78;
			// 
			// dataGridView2
			// 
			this->dataGridView2->AllowUserToAddRows = false;
			this->dataGridView2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->dataGridView2->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
			this->dataGridView2->AutoSizeRowsMode = System::Windows::Forms::DataGridViewAutoSizeRowsMode::AllCells;
			this->dataGridView2->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView2->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(5) {this->Columnid, 
				this->ColumnTitle2, this->ColumnContent2, this->ColumnTime2, this->ColumnSender});
			this->dataGridView2->Location = System::Drawing::Point(580, 95);
			this->dataGridView2->Name = L"dataGridView2";
			this->dataGridView2->ReadOnly = true;
			this->dataGridView2->RowHeadersVisible = false;
			this->dataGridView2->RowTemplate->Height = 23;
			this->dataGridView2->Size = System::Drawing::Size(504, 368);
			this->dataGridView2->TabIndex = 11;
			// 
			// Columnid
			// 
			this->Columnid->HeaderText = L"消息ID";
			this->Columnid->Name = L"Columnid";
			this->Columnid->ReadOnly = true;
			this->Columnid->Width = 66;
			// 
			// ColumnTitle2
			// 
			this->ColumnTitle2->HeaderText = L"标题";
			this->ColumnTitle2->Name = L"ColumnTitle2";
			this->ColumnTitle2->ReadOnly = true;
			this->ColumnTitle2->Width = 54;
			// 
			// ColumnContent2
			// 
			this->ColumnContent2->HeaderText = L"内容";
			this->ColumnContent2->Name = L"ColumnContent2";
			this->ColumnContent2->ReadOnly = true;
			this->ColumnContent2->Width = 54;
			// 
			// ColumnTime2
			// 
			this->ColumnTime2->HeaderText = L"时间";
			this->ColumnTime2->Name = L"ColumnTime2";
			this->ColumnTime2->ReadOnly = true;
			this->ColumnTime2->Width = 54;
			// 
			// ColumnSender
			// 
			this->ColumnSender->HeaderText = L"发送人";
			this->ColumnSender->Name = L"ColumnSender";
			this->ColumnSender->ReadOnly = true;
			this->ColumnSender->Width = 66;
			// 
			// buttonQureyRev
			// 
			this->buttonQureyRev->Location = System::Drawing::Point(599, 51);
			this->buttonQureyRev->Name = L"buttonQureyRev";
			this->buttonQureyRev->Size = System::Drawing::Size(103, 23);
			this->buttonQureyRev->TabIndex = 12;
			this->buttonQureyRev->Text = L"查询已接收消息";
			this->buttonQureyRev->UseVisualStyleBackColor = true;
			this->buttonQureyRev->Click += gcnew System::EventHandler(this, &QueryMeaasge::buttonQureyRev_Click);
			// 
			// buttonClear2
			// 
			this->buttonClear2->Location = System::Drawing::Point(763, 51);
			this->buttonClear2->Name = L"buttonClear2";
			this->buttonClear2->Size = System::Drawing::Size(75, 23);
			this->buttonClear2->TabIndex = 13;
			this->buttonClear2->Text = L"清空";
			this->buttonClear2->UseVisualStyleBackColor = true;
			this->buttonClear2->Click += gcnew System::EventHandler(this, &QueryMeaasge::buttonClear2_Click);
			// 
			// QueryMeaasge
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1112, 493);
			this->Controls->Add(this->buttonClear2);
			this->Controls->Add(this->buttonQureyRev);
			this->Controls->Add(this->dataGridView2);
			this->Controls->Add(this->dataGridView1);
			this->Controls->Add(this->buttonClear);
			this->Controls->Add(this->buttonQuerySend);
			this->Controls->Add(this->dateTimePicker1);
			this->Controls->Add(this->label1);
			this->Name = L"QueryMeaasge";
			this->Text = L"消息通知查询";
			this->Load += gcnew System::EventHandler(this, &QueryMeaasge::QueryMeaasge_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &QueryMeaasge::QueryMeaasge_FormClosed);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void QueryMeaasge_Load(System::Object^  sender, System::EventArgs^  e) 
			 {
				 //订阅查询返回
				IntPtr hWnd=this->Handle;
				m_QueryMeaasge2->SubscribeEvt((unsigned int)(hWnd.ToInt32()));
			 }

	 private: System::Void dataGridView1_Load()
			  {
				  //列表添加空白行
				 /*for(int i=0; i<14; i++)
				 {
					 this->dataGridView1->Rows->Add();
				 }*/

			  }
	private: System::Void dataSet()
			 {
				 this->dateTimePicker1->MaxDate = System::DateTime::Now;
				 this->dateTimePicker1->Value = System::DateTime::Now;
				 this->dateTimePicker1->Format = DateTimePickerFormat::Custom;
				 this->dateTimePicker1->CustomFormat = "yyyy-MM-dd";

			 }
			  			

private: System::Void buttonClear_Click(System::Object^  sender, System::EventArgs^  e) 
		 {

			/* for(int   i=0;i<dataGridView1->Rows->Count;i++)   
			 {   
				 dataGridView1->Rows->RemoveAt(i);  

			 }*/
			  dataGridView1->Rows->Clear();
			  m_QueryMeaasge2->m_mapMessageID2RowID.clear();
		 }
private: System::Void buttonQuery_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			
			 m_QueryMeaasge2->ReqQueryMessage(Tools::String2string(this->dateTimePicker1->Text->ToString()),QUERY_SEND);
			  dataGridView1->Rows->Clear();
			  m_QueryMeaasge2->m_mapMessageID2RowID.clear();
		 }
private: System::Void updateRowInfo( int RowIndex, MessageInfo&  messageInfo )
		 {
			 DataGridViewRow^ dr = dataGridView1->Rows[RowIndex];
			 dr->Cells[0]->Value = messageInfo.nMessageID;//消息ID
			 dr->Cells[3]->Value = Tools::string2String(messageInfo.szCreateDate);//时间
			 //string strSender = "";
			 //m_pdataInfo->GetUserName(strSender);
			 //dr->Cells[2]->Value = Tools::string2String(strSender.c_str());//发送人

			 dr->Cells[1]->Value = Tools::string2String(messageInfo.szTitle);//标题
			 dr->Cells[2]->Value = Tools::string2String(messageInfo.szContent);//内容
		 }
private: System::Void updateRowStatus( int RowIndex, MsgSendStatus&  messageStatus )
		 { 
			 DataGridViewRow^ dr = dataGridView1->Rows[RowIndex];
			 
			 if( messageStatus.nSendStatus == 1 )
			 {
				 std::string strReceived = "";
				 
				 if(dr->Cells[4]->Value != nullptr)
				 {
					 strReceived = Tools::String2string(dr->Cells[3]->Value->ToString());
					 strReceived.append(",");
				 }

				 strReceived.append(messageStatus.szAccount);
				 dr->Cells[4]->Value = Tools::string2String(strReceived.c_str());//已接收人
			 }
			 else if( messageStatus.nSendStatus == 0 )
			 {
				 std::string strNotreceived = "";
				  if(dr->Cells[5]->Value != nullptr)
				 {
				     strNotreceived = Tools::String2string(dr->Cells[5]->Value->ToString());
				     strNotreceived.append(",");
				 }
				 strNotreceived.append(messageStatus.szAccount);
				 dr->Cells[5]->Value = Tools::string2String(strNotreceived.c_str());//未接收人
			 }
		 
		 }


//重载WndProc，以处理WM_COMMAND消息
protected:virtual void WndProc( Message% m ) override
		  {
			  if (  m.Msg == WM_COMMAND_Win && m.WParam.ToInt32()==WndCmd_YourPkgArrival )
			  {

				  char *p = (char*)m.LParam.ToInt32();
				  Stru_UniPkgHead head;					
				  memset(&head, 0, sizeof(head));
				  memcpy(&head, p, sizeof(head));

				  if ( head.cmdid == Cmd_RM_QrySendMessage_Rsp)//查询我发送的消息返回
				  {
					  if ( head.userdata1 == OPERATOR_SUCCESS )
					  {
						  int pkgLen=head.len;
						  int MessageCnt=pkgLen/sizeof(MessageInfo);
						  std::map<int, MessageInfo> mapMessageInfo;

						  bool brlt = m_QueryMeaasge2->ProcessMessageInfoEvtPkg((const char*)m.LParam.ToInt32(),mapMessageInfo,MessageCnt);
						  if(brlt)
						  {
							  std::map<int, MessageInfo>::iterator it = mapMessageInfo.begin();
							  for(it;it!=mapMessageInfo.end();it++)
							  {
								  std::map <int,int>::iterator it2 = m_QueryMeaasge2->m_mapMessageID2RowID.find(it->second.nMessageID);
								  if(it2 == m_QueryMeaasge2->m_mapMessageID2RowID.end())
								  {
									  int RowIndex=dataGridView1->Rows->Add();
								      m_QueryMeaasge2->m_mapMessageID2RowID[it->second.nMessageID] = RowIndex;
									  updateRowInfo( RowIndex,it->second );
								  }
								  else
								  {
									  updateRowInfo( it2->second,it->second );
								  }

	
								 
							  }
						  }

					  }
					  else
							{
								string str = (char*)(p+sizeof(Stru_UniPkgHead));
								String^ strtemp = Tools::string2String(str.c_str());
								MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);
							}
				  }
				   if ( head.cmdid == Cmd_RM_QryMsgSendStatus_Rsp)//查询消息发送状态返回
				  {
					  if ( head.userdata1 == OPERATOR_SUCCESS )
					  {
						  int pkgLen=head.len;
						  int MessageCnt=pkgLen/sizeof(MsgSendStatus);
						  std::multimap<int, MsgSendStatus> mapMsgSendStatus;

						  bool brlt = m_QueryMeaasge2->ProcessMessageStatusEvtPkg((const char*)m.LParam.ToInt32(),mapMsgSendStatus,MessageCnt);
						  if(brlt)
						  {
							  std::multimap<int, MsgSendStatus>::iterator it = mapMsgSendStatus.begin();
							  for(it;it!=mapMsgSendStatus.end();it++)
							  {

								  std::map <int,int>::iterator it2 = m_QueryMeaasge2->m_mapMessageID2RowID.find(it->second.nMessageID);
								  if(it2 == m_QueryMeaasge2->m_mapMessageID2RowID.end())
								  {
									  int RowIndex=dataGridView1->Rows->Add();
								      m_QueryMeaasge2->m_mapMessageID2RowID[it->second.nMessageID] = RowIndex;
									  updateRowStatus(RowIndex,it->second );
								  }
								  else
								  {
									  updateRowStatus(it2->second,it->second );
								  }
							  }
						  }

					  }
					  else
							{
								string str = (char*)(p+sizeof(Stru_UniPkgHead));
								String^ strtemp = Tools::string2String(str.c_str());
								MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);
							}
				  }
				   if ( head.cmdid == Cmd_RM_QryRecvMessage_Rsp)//查询我接收的消息返回
				  {
					  if ( head.userdata1 == OPERATOR_SUCCESS )
					  {
						  int pkgLen=head.len;
						  int MessageCnt=pkgLen/sizeof(MessageInfo);
						  std::map<int, MessageInfo> mapMessageInfo;

						  bool brlt = m_QueryMeaasge2->ProcessMessageInfoEvtPkg((const char*)m.LParam.ToInt32(),mapMessageInfo,MessageCnt);
						  if(brlt)
						  {
							  std::map<int, MessageInfo>::iterator it = mapMessageInfo.begin();
							  for(it;it!=mapMessageInfo.end();it++)
							  {

								  int RowIndex=dataGridView2->Rows->Add();
								  DataGridViewRow^ dr = dataGridView2->Rows[RowIndex];
								  dr->Cells[0]->Value = it->second.nMessageID;//消息ID
								  dr->Cells[3]->Value = Tools::string2String(it->second.szCreateDate);//时间
								  if( it->second.nOwner == 0 )
								  {
								  dr->Cells[4]->Value = Tools::string2String("系统");//发送人是系统
								  }
								  else
								  {
									
									  dr->Cells[4]->Value = Tools::string2String(it->second.szOwner);//发送人是其他风控员
								  }

								  dr->Cells[1]->Value = Tools::string2String(it->second.szTitle);//标题
								  dr->Cells[2]->Value = Tools::string2String(it->second.szContent);//内容

									 

	
								 
							  }
						  }

					  }
					  else
							{
								string str = (char*)(p+sizeof(Stru_UniPkgHead));
								String^ strtemp = Tools::string2String(str.c_str());
								MessageBox::Show(strtemp, "提示",MessageBoxButtons::OK);
							}
				  }
			  }

			  Form::WndProc ( m );
		  }
private: System::Void QueryMeaasge_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) 
		 {
			 IntPtr hWnd=this->Handle;
			 CTcpLayer::SubscribePkg( Cmd_RM_QryMsgSendStatus_Rsp ,(int)(hWnd.ToInt32()));
			 CTcpLayer::SubscribePkg( Cmd_RM_QryRecvMessage_Rsp ,(int)(hWnd.ToInt32()));
			 CTcpLayer::SubscribePkg( Cmd_RM_QryMsgSendStatus_Rsp ,(int)(hWnd.ToInt32()));
		 }
private: System::Void dataGridView1_CellContentClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
		 }
private: System::Void buttonQureyRev_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 m_QueryMeaasge2->ReqQueryMessage(Tools::String2string(this->dateTimePicker1->Text->ToString()),QUERY_RECV);
			  dataGridView2->Rows->Clear();

		 }
private: System::Void buttonClear2_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 dataGridView2->Rows->Clear();
		 }
};
}

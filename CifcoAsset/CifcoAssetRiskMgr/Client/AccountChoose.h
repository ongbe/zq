#pragma once

#include <map>
#include <string>
#include "AccountChangedEventArgs.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


//public ref class AccountChangedEventArgs : EventArgs // 事件参数类
//		{
//		public:
//			AccountChangedEventArgs( String^ nodeId, String^ nodeName )
//			{
//				this->nodeId = nodeId;
//				this->nodeName = nodeName;
//			}
//			String^ nodeId;
//			String^ nodeName;
//		};

namespace Client {

	/// <summary>
	/// Summary for AccountChoose
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class AccountChoose : public System::Windows::Forms::Form
	{
	public:
		AccountChoose(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AccountChoose()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TreeView^  treeView1;
	protected: 
	private: System::Windows::Forms::Button^  button_add;
	private: System::Windows::Forms::Button^  button_clear;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		
		// 声明委托
	public:
		delegate void AccountChangedEventHandler(Object^ sender, AccountChangedEventArgs^ e);
		// 定义事件
	    event AccountChangedEventHandler^ AccountChanged;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::TreeNode^  treeNode1 = (gcnew System::Windows::Forms::TreeNode(L"北京"));
			System::Windows::Forms::TreeNode^  treeNode2 = (gcnew System::Windows::Forms::TreeNode(L"北中国", gcnew cli::array< System::Windows::Forms::TreeNode^  >(1) {treeNode1}));
			System::Windows::Forms::TreeNode^  treeNode3 = (gcnew System::Windows::Forms::TreeNode(L"深圳"));
			System::Windows::Forms::TreeNode^  treeNode4 = (gcnew System::Windows::Forms::TreeNode(L"南中国", gcnew cli::array< System::Windows::Forms::TreeNode^  >(1) {treeNode3}));
			System::Windows::Forms::TreeNode^  treeNode5 = (gcnew System::Windows::Forms::TreeNode(L"总部", gcnew cli::array< System::Windows::Forms::TreeNode^  >(2) {treeNode2, 
				treeNode4}));
			this->treeView1 = (gcnew System::Windows::Forms::TreeView());
			this->button_add = (gcnew System::Windows::Forms::Button());
			this->button_clear = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// treeView1
			// 
			this->treeView1->CheckBoxes = true;
			this->treeView1->Location = System::Drawing::Point(12, 12);
			this->treeView1->Name = L"treeView1";
			treeNode1->Name = L"NodeBeijing";
			treeNode1->Tag = L"2";
			treeNode1->Text = L"北京";
			treeNode2->Name = L"NodeNorthChina";
			treeNode2->Tag = L"1";
			treeNode2->Text = L"北中国";
			treeNode3->Name = L"NodeShenzhen";
			treeNode3->Tag = L"4";
			treeNode3->Text = L"深圳";
			treeNode4->Name = L"NodeSouthChina";
			treeNode4->Tag = L"3";
			treeNode4->Text = L"南中国";
			treeNode5->Name = L"NodeHeadquarters";
			treeNode5->Tag = L"0";
			treeNode5->Text = L"总部";
			this->treeView1->Nodes->AddRange(gcnew cli::array< System::Windows::Forms::TreeNode^  >(1) {treeNode5});
			this->treeView1->Size = System::Drawing::Size(332, 340);
			this->treeView1->TabIndex = 0;
			this->treeView1->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &AccountChoose::treeView1_AfterSelect);
			// 
			// button_add
			// 
			this->button_add->Location = System::Drawing::Point(35, 377);
			this->button_add->Name = L"button_add";
			this->button_add->Size = System::Drawing::Size(75, 23);
			this->button_add->TabIndex = 1;
			this->button_add->Text = L"添加";
			this->button_add->UseVisualStyleBackColor = true;
			this->button_add->Click += gcnew System::EventHandler(this, &AccountChoose::button_add_Click);
			// 
			// button_clear
			// 
			this->button_clear->Location = System::Drawing::Point(240, 376);
			this->button_clear->Name = L"button_clear";
			this->button_clear->Size = System::Drawing::Size(75, 23);
			this->button_clear->TabIndex = 2;
			this->button_clear->Text = L"清空";
			this->button_clear->UseVisualStyleBackColor = true;
			// 
			// AccountChoose
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(369, 423);
			this->Controls->Add(this->button_clear);
			this->Controls->Add(this->button_add);
			this->Controls->Add(this->treeView1);
			this->Name = L"AccountChoose";
			this->Text = L"选择账户";
			this->Load += gcnew System::EventHandler(this, &AccountChoose::AccountChoose_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void treeView1_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) {
			 }
private: System::Void AccountChoose_Load(System::Object^  sender, System::EventArgs^  e) 
		 {
			//添加点击复选框事件
			this->treeView1->AfterCheck += gcnew  System::Windows::Forms::TreeViewEventHandler(this,&AccountChoose::treeView1_AfterCheck);
		 }
private: System::Void CheckAllChildNodes( TreeNode^ treeNode, bool nodeChecked )
		 {
			 IEnumerator^ myEnum = treeNode->Nodes->GetEnumerator();
			 while ( myEnum->MoveNext() )
			 {
				 TreeNode^ node = safe_cast<TreeNode^>(myEnum->Current);
				 node->Checked = nodeChecked;
				 if ( node->Nodes->Count > 0 )
				 {

					 // If the current node has child nodes, call the CheckAllChildsNodes method recursively.
					 this->CheckAllChildNodes( node, nodeChecked );
				 }
			 }
		 }
//private: System::Void TraversNodes( System::Windows::Forms::TreeNode^ parent)
//		 {
//             System::Windows::Forms::TreeNode^  node = parent;
//			 if(node != nullptr)
//			 {
//				 if(node::Tag != nullptr && node::Tag->ToString() == tag)
//			 }
//		 }
private: System::Void GetCheckedNode(System::Windows::Forms::TreeNodeCollection^ tnc/*,array<array<String ^>^>^ Message,int count*/)
		 {
			 String^ strAccount=L"";

			 for each(TreeNode^  node in tnc)
			 {
				 if(node->Checked)
					 //strAccount +=node->Text;
				 {
					 /*Message[count] = gcnew array<String^>{node->Text->ToString(), node->Tag->ToString()};
					 count++;*/
					 AccountChangedEventArgs^ AccountArgs = gcnew AccountChangedEventArgs( node->Tag->ToString(),node->Text->ToString() );
					 AccountChanged( this, AccountArgs );
				 }

				 GetCheckedNode(node->Nodes/*,Message,count*/);
			 }
			 /*if(strAccount->Length !=0)
			  this->textBoxAccount->AppendText(strAccount);*/
			   
		 }
private: System::Void treeView1_AfterCheck(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e) 
		 {
			 /* string nodeName = treeview1.SelectedNodes.Text;
			 ListView1.ListItems(i).Checked=True ;*/
			 //System::Windows::Forms::TreeNode^  node = e->Node;
			 //node->
			 if (e->Action != System::Windows::Forms::TreeViewAction::Unknown)
			 {
				 // System::Windows::Forms::TreeNode^  node = e->Node;

				 if ( e->Node->Nodes->Count > 0 )
				 {
					 /* Calls the CheckAllChildNodes method, passing in the current
					 Checked value of the TreeNode whose checked state changed. */
					 this->CheckAllChildNodes( e->Node, e->Node->Checked );
				 }
				 
				 //String^ temp;
				 //for each(TreeNode^  node in this->treeView1->Nodes)
				 //{
					// if(node->Checked)
					//	 strAccount +=node->Text;
					// //strAccount
				 //}
				 	 
			 }
		 }
private: System::Void button_add_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 array<array<String ^>^>^ NodeMessage = gcnew array<array<String^>^>(5);
			 //std::map<String^, String^> NodeMessage;
             int i = 0;
			 GetCheckedNode(this->treeView1->Nodes);
			 this->Close();
			
		 }
};
}

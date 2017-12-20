#pragma once
#include "windows.h"
#include "ComboBoxControlContainer.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace AnywndComboBox {


	/// <summary>
	/// Summary for AnywndComboBoxControl
	/// </summary>
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	public ref class AnywndComboBoxControl : public System::Windows::Forms::ComboBox
	{
	public:
		AnywndComboBoxControl(void)
		{
			mrChildContorl = nullptr;
			mTreeViewHost = nullptr;
			mDropDown = nullptr;
			mbDropDownShowed = false;
			InitializeComponent();
		}
		

	public: 


	private:
		Boolean mbDropDownShowed;
		ToolStripControlHost^ mTreeViewHost;
		ToolStripDropDown^ mDropDown;
		CAnywndComboBoxControlContainter ^ mrChildContorl;
	public:
		property CAnywndComboBoxControlContainter ^ ChildControl
		{
			CAnywndComboBoxControlContainter ^ get() {return mrChildContorl;}
			void set(CAnywndComboBoxControlContainter^ nr) 
			{ 			
				if(nr == nullptr)
					return;
				mrChildContorl = nr;
				mTreeViewHost = gcnew ToolStripControlHost(mrChildContorl);
				// create drop down and add it
				mDropDown = gcnew ToolStripDropDown();
				mDropDown->Items->Add(mTreeViewHost);
                
				mDropDown->Closed += gcnew System::Windows::Forms::ToolStripDropDownClosedEventHandler(this, &AnywndComboBoxControl::OnDropDownLeavedClose); 
				//mDropDown->VisibleChanged +=gcnew System::EventHandler(this, &AnywndComboBoxControl::OnDropDownVisibleChanged); 
				/*::COMBOBOXINFO cb ;
				cb.cbSize = sizeof(::COMBOBOXINFO);
				if(::GetComboBoxInfo((HWND)this->Handle.ToInt32(),&cb))
					::DestroyWindow(cb.hwndList);*/
			}
		};

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>		
		~AnywndComboBoxControl()
		{
			if (components)
			{
				delete components;
			}
		}


	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// AnywndComboBoxControl
			// 
			this->BackColor = System::Drawing::Color::White;
			this->DropDownHeight = 1;
			this->DropDownWidth = 1;
			this->IntegralHeight = false;
			this->MaxDropDownItems = 1;
			this->Size = System::Drawing::Size(163, 20);
		    this->ResumeLayout(false);

		}
#pragma endregion
	
	   protected:virtual System::Void OnDrawItem(System::Windows::Forms::DrawItemEventArgs^e) override
	   {
              return;
	   }

	   public:  System::Void OnDropDownLeavedClose(
					 Object^ sender, 
					 ToolStripDropDownClosedEventArgs^ e
					 )
	   {
			  //OutputDebugString(L"closed");
			  if(mrChildContorl)
			  {
		 		 OutputDebugString(L"Process data \n");
		 		 mrChildContorl->ProcessData();

                 this->OnSelectedIndexChanged(nullptr);
			  }
	  }
	

	   private: void MyShowDropDown() 
	   {	  
			if (mDropDown != nullptr ) 
			{		
				
					OutputDebugString(L"Show dropdown\n");				
					mrChildContorl->Show();						
					//::SetParent((HWND)mDropDown->Handle.ToInt32(),(HWND)this->Handle.ToInt32());			
					mDropDown->Show(this, 0 , this->Height);					
					mDropDown->BringToFront();				
							
			}
	   }	 
		
	   
	   protected: virtual System::Void WndProc(System::Windows::Forms::Message% m) override
	   {
			 if (m.Msg == ( WM_USER + WM_COMMAND + 0x1C00)) {
				 if (HIWORD((int)m.WParam) == CBN_DROPDOWN) { 		
					
					 MyShowDropDown();				 
					 return;
				 }

				 //if (HIWORD((int)m.WParam) == CBN_CLOSEUP) { 
					// mbDropDownShowed = false;
					// OutputDebugString(L"Hide dropdown\n");					
					//// mDropDown->Hide();
					// if(mrChildContorl)
					// {
					//	 OutputDebugString(L"Process data \n");
					//	 mrChildContorl->ProcessData();	
					// }
					// 				
				 //}
				
			 }
			
			 ComboBox::WndProc(m);
	   }



};
}

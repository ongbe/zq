#pragma once


namespace TestReport {

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
		}
	private: SimpleReportControl::SimpleReportControlControl^  simpleReportControlControl1;
	protected: 
	private: System::ComponentModel::IContainer^  components;
	private: DataTable ^ mDataTable;
	private:array<String^>^ mrCloumeHeaderString;
	private:array<System::Type^>^ mrColumnType;
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
			this->simpleReportControlControl1 = (gcnew SimpleReportControl::SimpleReportControlControl());
			this->SuspendLayout();
			// 
			// simpleReportControlControl1
			// 
			this->simpleReportControlControl1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->simpleReportControlControl1->ConfigGuid = L"{53D95967-4A96-4fdb-B7ED-87678E051023}";
			this->simpleReportControlControl1->Location = System::Drawing::Point(59, 38);
			this->simpleReportControlControl1->Name = L"simpleReportControlControl1";
			this->simpleReportControlControl1->RealDataTable = nullptr;
			this->simpleReportControlControl1->SimpleMode = true;
			this->simpleReportControlControl1->Size = System::Drawing::Size(579, 346);
			this->simpleReportControlControl1->StatisticDataTable = nullptr;
			this->simpleReportControlControl1->TabIndex = 0;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(696, 411);
			this->Controls->Add(this->simpleReportControlControl1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {

				 mDataTable = gcnew DataTable();


				 mrCloumeHeaderString = gcnew array<String^> {"日期","合约代码","NUMBER"};
				 mrColumnType = gcnew array<Type^> (mrCloumeHeaderString->Length);
				 mrColumnType[0] = Type::GetType("System.String");
				 mrColumnType[1] = 	 Type::GetType("System.String");
				 mrColumnType[2] = 	 Type::GetType("System.Double");
				 mDataTable->Rows->Clear();
				 mDataTable->Columns->Clear();
				 for(Int32 i = 0; i < mrCloumeHeaderString->Length;i++)
				 {
					 mDataTable->Columns->Add(mrCloumeHeaderString[i],mrColumnType[i]);
				 }

				
				 for(Int32 i = 0 ; i < 10000;i++)
				 {
				 int j = 0; 
				 DataRow ^ nrDataRow = mDataTable->NewRow();	
				 nrDataRow[mrCloumeHeaderString[j++]] =  "20130829";						
				 nrDataRow[mrCloumeHeaderString[j++]] =  "IF1382";		
				 double lx = 2.27493 * i;
				 nrDataRow[mrCloumeHeaderString[j++]] =  lx;// Convert::ToDouble(lx).ToString("N2") ;

				// nrDataRow[mrCloumeHeaderString[j++]] =  i.ToString();

				 mDataTable->Rows->Add(nrDataRow);
				 }
				 simpleReportControlControl1->RealDataTable = mDataTable;

				 for(Int32 i = 10000 ; i < 20000;i++)
				 {
					 int j = 0; 
					 DataRow ^ nrDataRow = mDataTable->NewRow();	
					 nrDataRow[mrCloumeHeaderString[j++]] =  "20130829";						
					 nrDataRow[mrCloumeHeaderString[j++]] =  "IF1382";	
					 double lx = 2.2324 * i;
					  nrDataRow[mrCloumeHeaderString[j++]] = lx;//   Convert::ToDouble(lx).ToString("N2")  ;

					 mDataTable->Rows->Add(nrDataRow);
				 }
				  simpleReportControlControl1->Refresh2();
			 }
	};
}


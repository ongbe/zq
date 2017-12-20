#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace Microsoft::Office::Core;
using namespace Microsoft::Office::Interop;
using namespace Microsoft::Office::Interop::Excel;


namespace CltRisk
{
	 public ref class CDataOpr
	 {
	 public: static bool ExportToExcel(System::Data::DataTable^ nrTable,String^ nsFilePath)   //导出excel
		 {
			 if(nrTable == nullptr)
				 return false;

			 Excel::Application^ lrExcel = gcnew  Excel::ApplicationClass();
			 if (lrExcel == nullptr)
			 {
				 MessageBox::Show("Excel无法启动", "错误", MessageBoxButtons::OK, MessageBoxIcon::Error);
				 return false;
			 }

			 Excel::Workbook ^ workbookData = nullptr;
			 Excel::Worksheet ^ worksheetData = nullptr;
			 Range^ range;
			 workbookData = lrExcel->Workbooks->Add(System::Reflection::Missing::Value);		
             worksheetData = (Excel::Worksheet^)workbookData->Worksheets[1];
			 Int32 lnLastRow = 0;
			 if(nrTable != nullptr)
			 {
				 
				 lnLastRow = nrTable->Rows->Count + 2;
				 //写入标题
				 for (int i = 0; i < nrTable->Columns->Count; i++)
				 {					
					 worksheetData->Cells[1, i + 1] = nrTable->Columns[i]->ColumnName;
					 range = (Range^)worksheetData->Cells[1, i + 1];
					 range->Interior->ColorIndex = 15;
					 range->Font->Bold = true;
					 range->NumberFormatLocal = "@";//文本格式 
					 // range.EntireColumn.AutoFit();//自动调整列宽 
					 // range.WrapText = true; //文本自动换行   
					 range->ColumnWidth = 15;				

				 }
				 //写入数值

				 for (int r = 0; r < nrTable->Rows->Count; r++)
				 {
					 for (int i = 0; i < nrTable->Columns->Count; i++)		
					 {					
						 worksheetData->Cells[r + 2, i + 1] = "'"+nrTable->Rows[r][i]->ToString(); 
					 }
					
					 System::Windows::Forms::Application::DoEvents();
				 }           

				 worksheetData->Columns->EntireColumn->AutoFit();
				 
			 }
			 

			 try
			 {				
					
				 workbookData->Saved = true;
				 workbookData->SaveAs(nsFilePath, System::Reflection::Missing::Value, System::Reflection::Missing::Value, 
					                             System::Reflection::Missing::Value,false, false,
						                         Excel::XlSaveAsAccessMode::xlNoChange,
						                         System::Reflection::Missing::Value, System::Reflection::Missing::Value, System::Reflection::Missing::Value,
						                         System::Reflection::Missing::Value, System::Reflection::Missing::Value);
				

			 }
			 catch (System::Exception ^ )
			 {				
				 lrExcel->Quit();				
				 return false;
			 }
			
			 lrExcel->Quit();			
			 return true;
		 }
	 };

}
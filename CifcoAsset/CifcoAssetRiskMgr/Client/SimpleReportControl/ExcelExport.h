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


namespace SimpleReportControl
{
	 public class CFileDBUtil
	 {
	 public: static bool ExportToExcel(System::Data::DataTable^ nrTable,System::Windows::Forms::FlowLayoutPanel^ nrChartContainer,String^ nsFilePath)   //导出excel
		 {
			 if(nrChartContainer == nullptr && nrTable == nullptr)
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
			 if(nrChartContainer != nullptr)
			 {
				 lnLastRow ++; //图片和数据隔一行
				 System::Collections::IEnumerator ^ liEnum = nrChartContainer->Controls->GetEnumerator();
				 while(liEnum && liEnum->MoveNext())
				 {
					  Control ^ lTemp = (Control^)liEnum->Current;
					  System::Windows::Forms::DataVisualization::Charting::Chart ^ lChart = dynamic_cast<System::Windows::Forms::DataVisualization::Charting::Chart ^>(lTemp);
					  if(lChart)
					  {
						  String^ lsPath = System::IO::Path::GetTempFileName();				
						  lChart->SaveImage(lsPath,System::Windows::Forms::DataVisualization::Charting::ChartImageFormat::Png);
						  
						  Excel::Range^ insertRange = worksheetData->Range["A"+lnLastRow.ToString(),"B"+lnLastRow.ToString()];//(Excel::Range^)worksheetData->Cells[lnLastRow+1, 1];
						  insertRange->Select();
						  //Excel::Pictures^ pics = (Excel::Pictures^)worksheetData->Pictures(System::Reflection::Missing::Value);
						  //pics->Insert(lsPath, System::Reflection::Missing::Value);     
						  worksheetData->Shapes->AddPicture(lsPath,Microsoft::Office::Core::MsoTriState::msoFalse, 
							  Microsoft::Office::Core::MsoTriState::msoTrue, Convert::ToSingle(insertRange->Left),
							  Convert::ToSingle(insertRange->Top), 600, 300); 

						  lnLastRow += 23;

					  }

				 }				 
				 
				 //worksheetData->ro
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
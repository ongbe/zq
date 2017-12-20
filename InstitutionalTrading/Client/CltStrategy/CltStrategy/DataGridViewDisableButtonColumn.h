using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Windows::Forms::VisualStyles;

namespace CltStrategy {

public  ref class DataGridViewDisableButtonCell : DataGridViewButtonCell
{
private: bool enabledValue;
public :
	    bool Enabled;

		bool getEnabled()
		{
			return enabledValue;
		}
		void setEnabled(bool value)
		{
			enabledValue = value;
		}


	// Override the Clone method so that the Enabled property is copied.
public:   Object^ Clone2()
	{
		DataGridViewDisableButtonCell^ cell =
			(DataGridViewDisableButtonCell^)this->Clone();
		cell->Enabled = this->Enabled;
		return cell;
	}

	// By default, enable the button cell.
public: DataGridViewDisableButtonCell()
	{
		this->enabledValue = true;
	}

protected:  void Paint(Graphics^ graphics,
				System::Drawing::Rectangle^ clipBounds, System::Drawing::Rectangle^ cellBounds, int rowIndex,
		DataGridViewElementStates^ elementState, Object^ value,
		Object^ formattedValue, String^ errorText,
		DataGridViewCellStyle^ cellStyle,
		DataGridViewAdvancedBorderStyle^ advancedBorderStyle,
		DataGridViewPaintParts^ paintParts)
	{
		// The button cell is disabled, so paint the border,  
		// background, and disabled button for the cell.
		if (!this->enabledValue)
		{
			// Draw the cell background, if specified.
			if (paintParts == DataGridViewPaintParts::Background )
			{
				SolidBrush^ cellBackground =
					gcnew SolidBrush(cellStyle->BackColor);
				graphics->FillRectangle(cellBackground, *cellBounds);
				//cellBackground->Dispose();
			}

			// Draw the cell borders, if specified.
			if (paintParts  ==
				DataGridViewPaintParts::Border)
			{
				PaintBorder(graphics, *clipBounds, *cellBounds, cellStyle,
					advancedBorderStyle);
			}

			// Calculate the area in which to draw the button.
			System::Drawing::Rectangle^ buttonArea = cellBounds;
			System::Drawing::Rectangle^ buttonAdjustment =
				this->BorderWidths(advancedBorderStyle);
			buttonArea->X += buttonAdjustment->X;
			buttonArea->Y += buttonAdjustment->Y;
			buttonArea->Height -= buttonAdjustment->Height;
			buttonArea->Width -= buttonAdjustment->Width;

			// Draw the disabled button.                
			ButtonRenderer::DrawButton(graphics, *buttonArea,
				PushButtonState::Disabled);

			// Draw the disabled button text. 
			if (this->FormattedValue !=nullptr) 
			{
				TextRenderer::DrawText(graphics,
					(String^)this->FormattedValue,
					this->DataGridView->Font,
					*buttonArea, SystemColors::GrayText);
			}
		}
		else
		{
			// The button cell is enabled, so let the base class 
			// handle the painting.
			this->Paint(graphics, clipBounds, cellBounds, rowIndex,
				elementState, value, formattedValue, errorText,
				cellStyle, advancedBorderStyle, paintParts);
		}
	}
};
public  ref class DataGridViewDisableButtonColumn : DataGridViewButtonColumn
{
public:
	DataGridViewDisableButtonColumn()
	{
		this->CellTemplate = gcnew DataGridViewDisableButtonCell();
	}
};
}
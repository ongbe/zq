using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;




namespace SimpleGradientProgressBar
{
	/// <summary>
	/// 颜色渐变进度条
	/// </summary>
	public partial class SimpleGradientProgessBar : UserControl
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>	
		private int mnMini = 0;                              //最小值
        private int mnMaxi = 100;                            //最大值
        private int mnCurrVal = 0;                           //当前值
        private int mnPercent = 0;                           //百分比

		private Color mBarForeColour = Color.LightSeaGreen;  //前色
		private Color mBarBackColour = Color.DimGray;        //后色
        private System.Drawing.Rectangle mRectDraw;
	
		private LinearGradientMode mGradMode = LinearGradientMode.ForwardDiagonal; //Default Gradient Mode

		private bool mDispLabl = true;                        //是否显示百分比的label
	

        internal readonly Font DataFont = new Font(FontFamily.GenericSansSerif, 9, FontStyle.Regular);
        internal readonly StringFormat CenterFormat = new StringFormat();

        public SimpleGradientProgessBar()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
            CenterFormat.Alignment = StringAlignment.Center;
            CenterFormat.LineAlignment = StringAlignment.Center;

			//Prevent Flickering, This Produces Smooth Progression
			SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint | ControlStyles.DoubleBuffer, true);

		}

		/// <summary>
		/// Displays Current Progress
		/// </summary>
		/// <param name="percentage"></param>
        //public void SetProgComplete(int percentage) 
        //{ 
        //    int Width = (int)Math.Floor((double)(this.ClientRectangle.Width * percentage) / 100);  //Calculate New Width
        //    int Height = (int)this.ClientRectangle.Height; //Calculate New Height
    
        //    if (Width > 0 && Height > 0)  //If Height And Width Valid
        //    { 
        //        this.Invalidate(new Rectangle(this.ClientRectangle.X, this.ClientRectangle.Y, Width, Height)); //Paint New Rectangle
        
        //        label1.Text = mnPercent.ToString() + "%";  //Display Current Progress
        //    }     
        //} 

	
		protected override void OnResize(EventArgs e) 
		{ 
			// Repaint Control
			this.Invalidate(); 
		} 
	
		protected override void OnPaint(PaintEventArgs e) 
		{
            if (mRectDraw.Width != 0 && mRectDraw.Height != 0)  //If Valid Rectangle           
			{ 
				//创建渐变画刷
                LinearGradientBrush GradBack = new LinearGradientBrush(mRectDraw, mBarBackColour, mBarForeColour, mGradMode);

                e.Graphics.FillRectangle(GradBack, mRectDraw);

            }
            if (ShowLabel)
            {
                e.Graphics.DrawString(mnPercent.ToString() + "%", DataFont, Brushes.White, this.ClientRectangle, CenterFormat);

            }
		} 

		/// <summary>
		/// GradientStyle Property
		/// </summary>
		[Description("Sets The Gradient Style"), Category("Appearance")] 
		public LinearGradientMode GradientStyle 
		{ 
			get 
			{
				return mGradMode; //Get Value
			} 
			set
			{
				mGradMode = value; //Set New Value
			} 
		} 

		/// <summary>
		/// Minimum Property
		/// </summary>
		[Description("Sets The Minimum Value"), Category("Behavior")] 
		public int Minimum 
		{ 
			get 
			{
				return mnMini; //Get Value
			} 
    
			set 
			{ 
				//Is Value Less Than 0 ?
				if ((value < 0)) 
				{
                    mnMini = 0; 
				} 
        
				//Is Value Greater Than Max ?
				if ((value > mnMaxi)) 
				{
                    mnMini = value; 
				} 
        
				//Is Value Still In Range ?
                if ((mnCurrVal < mnMini)) 
				{
                    mnCurrVal = mnMini; 
				} 
        
				//Repaint 
               
				this.Invalidate(); 
			} 
		} 

		/// <summary>
		/// Maximum Property
		/// </summary>
		[Description("Sets The Maximum Value"), Category("Behavior")] 
		public int Maximum 
		{ 
			get 
			{
                return mnMaxi; //Get Value
			} 
    
			set 
			{ 
				//Is Value Less Than Min ?
				if ((value < mnMini)) 
				{ 
					mnMini = value; //Store New Min Value
				}

                mnMaxi = value; //Continue
        
				//Is Value Still In Range ?
                if ((mnCurrVal > mnMaxi)) 
				{
                    mnCurrVal = mnMaxi; 
				} 
        
				//Repaint 
				this.Invalidate(); 
			} 
		} 

	

		/// <summary>
		/// Sets Current Value & Updates Display
		/// </summary>
		public int Value 
		{ 
			get 
			{
				return mnCurrVal; //Get Value
			} 
    
			set 
			{
               
        
				//Keep In Specified Range
				if ((value < mnMini)) 
				{
                    mnCurrVal = mnMini; 
				} 
				else if ((value > mnMaxi)) 
				{
                    mnCurrVal = mnMaxi; 
				} 
				else 
				{
                    mnCurrVal = value; 
				}         
				
        
				//New Value To Calculate New Rectangle 
                mnPercent = (mnCurrVal - mnMini) * 100 / (mnMaxi - mnMini);
                mRectDraw.X = mRectDraw.Y = 0;
                mRectDraw.Height = this.Height;
                mRectDraw.Width = (this.Width * (mnCurrVal - mnMini) ) / (mnMaxi - mnMini );
                //label1.Text = mnPercent.ToString() + "%";  //Display Current Progress
				//Repaint Intersection 
				this.Invalidate(); 
			} 
		} 

		/// <summary>
		/// ProgressBar's Fore Colour
		/// </summary>
		[Description("Sets The Bar Fore Colour"), Category("Appearance")] 
		public Color ProgressBarForeColour 
		{ 
			get 
			{
				return mBarForeColour; //Get Value
			} 
    
			set 
			{ 
				mBarForeColour = value; //Set Value
				
				//Repaint. 
				this.Invalidate(); 
			} 
		} 

		/// <summary>
		/// ProgressBar's Back Colour
		/// </summary>
		[Description("Sets The Bar Back Colour"), Category("Appearance")] 
		public Color ProgressBarBackColour 
		{ 
			get 
			{
				return mBarBackColour; //Get Value
			} 
    
			set 
			{ 
				mBarBackColour = value; //Set Value

				//Repaint. 
				this.Invalidate(); 
			} 
		} 

		

		

		/// <summary>
		/// Shows Or Hides Label
		/// </summary>
		[Description("Shows Or Hides The Label"), Category("Behavior")] 
		public bool ShowLabel 
		{ 
			get 
			{
				return mDispLabl; //Get Value
			} 
    
			set 
			{ 
				mDispLabl = value; //Set Value				
			} 
		} 

	



	}
}


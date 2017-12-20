using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;

namespace SimpleFlagControl
{
    public partial class SimpleFlagControl : UserControl
    {
        private Single mfMinValue = -100;
        private Single mfMaxValue = 100;
        private Single mfCurrentVal;
        private Color mForeColour = Color.LightSeaGreen;  //前色
        private Color mBackColour = Color.DimGray;        //后色
        private System.Drawing.Rectangle mRectDraw;

        /// <summary>
        /// Minimum Property
        /// </summary>
        [Description("Sets The Minimum Value"), Category("Behavior")]
        public Single Minimum
        {
            get
            {
                return mfMinValue; //Get Value
            }

            set
            {
                //Is Value Less Than 0 ?
                //if ((value < 0))
                //{
                //    mfMinValue = value;
                //}             

                //Repaint 
                this.Invalidate();
            }
        }

        /// <summary>
        /// Maximum Property
        /// </summary>
        [Description("Sets The Maximum Value"), Category("Behavior")]
        public Single Maximum
        {
            get
            {
                return mfMaxValue; //Get Value
            }

            set
            {
               
                mfMaxValue = value; //Continue              
                                //Repaint 
                this.Invalidate();
            }
        }



        /// <summary>
        /// Sets Current Value & Updates Display
        /// </summary>
        public Single Value
        {
            get
            {
                return mfCurrentVal; //Get Value
            }

            set
            {


                //Keep In Specified Range
                if ((value < mfMinValue))
                {
                    mfCurrentVal = mfMinValue;
                }
                else if ((value > mfMaxValue))
                {
                    mfCurrentVal = mfMaxValue;
                }
                else
                {
                    mfCurrentVal = value;
                }


                //New Value To Calculate New Rectangle 

                mRectDraw.X = (Int32)Math.Floor(this.Width * (mfCurrentVal + 100) / (mfMaxValue - mfMinValue)) - 5;
                mRectDraw.Y = 0;
                mRectDraw.Height = this.Height;
                mRectDraw.Width = 10;
                //label1.Text = mnPercent.ToString() + "%";  //Display Current Progress
                //Repaint Intersection 
                this.Invalidate();
            }
        }

        /// <summary>
        /// ProgressBar's Fore Colour
        /// </summary>
        [Description("Sets The Bar Fore Colour"), Category("Appearance")]
        public Color ForeColour
        {
            get
            {
                return mForeColour; //Get Value
            }

            set
            {
                mForeColour = value; //Set Value

                //Repaint. 
                this.Invalidate();
            }
        }

        /// <summary>
        /// ProgressBar's Back Colour
        /// </summary>
        [Description("Sets The Bar Back Colour"), Category("Appearance")]
        public Color BackColour
        {
            get
            {
                return mBackColour; //Get Value
            }

            set
            {
                mBackColour = value; //Set Value

                //Repaint. 
                this.Invalidate();
            }
        } 
        public SimpleFlagControl()
        {
            InitializeComponent();
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            if (mRectDraw.Width != 0 && mRectDraw.Height != 0)  //If Valid Rectangle           
            { 
                LinearGradientBrush GradBack;
                //创建渐变画刷
                if (mfCurrentVal < 0)
                    GradBack = new LinearGradientBrush(mRectDraw, mForeColour, mBackColour, LinearGradientMode.Horizontal);
                else
                    GradBack = new LinearGradientBrush(mRectDraw, mBackColour, mForeColour, LinearGradientMode.Horizontal);

                e.Graphics.FillRectangle(GradBack, mRectDraw);                
            }
        } 
    };
}

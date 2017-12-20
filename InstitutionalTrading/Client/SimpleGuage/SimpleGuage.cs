
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;
using System.Diagnostics;



namespace SimpleGuage
{
    public partial class SimpleGuage : UserControl
    {
#region enum, var, delegate, event
        

     
        //private Bitmap gaugeBitmap;
     

        private Single mfValue;              //总表示的值
        private Single mfBigValue;           //大环的值
        private Single mfSmallValue;         //小环的值 
        private Single mfMinValue = 0;       //总表示的最小值
        private Single mfMaxValue = 100;     //总表示的最大值
     
        private Single mnBigMinValue = 0;    //大环表示的最小值
        private Single mnBigMaxValue = 10;   //大环表示的最大值      
        private Int32 mnBigArcStart = 105;   //大环开始的角度
        private Int32 mnBigArcCount = 300;     //大环跨越的角度

 
        private Single mnSmallMinValue = 0;  //小环表示的最小值
        private Single mnSmallMaxValue = 90; //小环表示的最大值      
        private Int32 mnSmallArcStart = 135; //小环开始的角度
        private Int32 mnSmallArcCount = 270; //小环跨越的角度      

     
        private Int32 mnBigNeedleRadius = 120;   //大环半径
        private Int32 mnSmallNeedleRadius = 80;  //小环半径

        private Color m_NeedleColor2 = Color.DimGray;
        private Int32 m_NeedleWidth = 2;

        public class ValueInRangeChangedEventArgs : EventArgs
        {
            public Int32 valueInRange;

            public ValueInRangeChangedEventArgs(Int32 valueInRange)
            {
                this.valueInRange = valueInRange;
            }
        }
    
#endregion


        public SimpleGuage()
        {
            InitializeComponent();

            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
        }

#region properties
        [System.ComponentModel.Browsable(true),
        System.ComponentModel.Category("SimleGuage"),
        System.ComponentModel.Description("The value.")]
        public Single Value
        {
            get
            {
                return mfValue;
            }
            set
            {
                if (mfValue != value)
                {
                    mfValue = value;
                    if(mfValue == mfMaxValue)
                    {
                        mfBigValue = mnBigMaxValue;
                        mfSmallValue = mnSmallMaxValue;
                    }                    

                    //m_value = Math.Min(Math.Max(value, m_MinValue), m_MaxValue);
                    mfSmallValue = ((Int32)mfValue / 10) * 10;
                    mfBigValue = mfValue % 10;
                }
                this.Invalidate(true);

                 
            }
        }

      


        [System.ComponentModel.Browsable(true),
        System.ComponentModel.Category("SimleGuage"),
        System.ComponentModel.Description("The big circle start angle of the base arc.")]
        public Int32 BigArcStart
        {
            get
            {
                return mnBigArcStart;
            }
            set
            {
                if (mnBigArcStart != value)
                {
                    mnBigArcStart = value;                
                }
            }
        }

        [System.ComponentModel.Browsable(true),
        System.ComponentModel.Category("SimleGuage"),
        System.ComponentModel.Description("The big circle count angle of the base arc.")]
        public Int32 BigArcCount
        {
            get
            {
                return mnBigArcCount;
            }
            set
            {
                if (mnBigArcCount != value)
                {
                    mnBigArcCount = value;                  
                }
            }
        }


        [System.ComponentModel.Browsable(true),
               System.ComponentModel.Category("SimleGuage"),
               System.ComponentModel.Description("The Small circle start angle of the base arc.")]
        public Int32 SmallArcStart
        {
            get
            {
                return mnSmallArcStart;
            }
            set
            {
                if (mnSmallArcStart != value)
                {
                    mnSmallArcStart = value;
                }
            }
        }

        [System.ComponentModel.Browsable(true),
        System.ComponentModel.Category("SimleGuage"),
        System.ComponentModel.Description("The Small circle count angle of the base arc.")]
        public Int32 SmallArcCount
        {
            get
            {
                return mnSmallArcCount;
            }
            set
            {
                if (mnSmallArcCount != value)
                {
                    mnSmallArcCount = value;
                }
            }
        }         
       
       

        [System.ComponentModel.Browsable(true),
        System.ComponentModel.Category("SimleGuage"),
        System.ComponentModel.Description("The  big circle radius of the needle.")]
        public Int32 BigNeedleRadius
        {
            get
            {
                return mnBigNeedleRadius;
            }
            set
            {
                if (mnBigNeedleRadius != value)
                {
                    mnBigNeedleRadius = value;                   
                }
            }
        }
        [System.ComponentModel.Browsable(true),
             System.ComponentModel.Category("SimleGuage"),
             System.ComponentModel.Description("The Small circle radius of the needle.")]
        public Int32 SmallNeedleRadius
        {
            get
            {
                return mnSmallNeedleRadius;
            }
            set
            {
                if (mnSmallNeedleRadius != value)
                {
                    mnSmallNeedleRadius = value;
                }
            }
        }

   
        #endregion


#region base member overrides

        void DrawBigNeedle(PaintEventArgs pe)
        {
            Point Center = new Point(this.Size.Width / 2, this.Size.Height / 2);

            //pe.Graphics.DrawImageUnscaled(gaugeBitmap, 0, 0);
            pe.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
            pe.Graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;

            //画指针
            Single brushAngle = (Int32)(mnBigArcStart + (mfBigValue - mnBigMinValue) * mnBigArcCount / (mnBigMaxValue - mnBigMinValue)) % 360;
            Double needleAngle = brushAngle * Math.PI / 180;

            Point startPoint=new Point((Int32)(Center.X - mnBigNeedleRadius / 8 * Math.Cos(needleAngle)),
                                       (Int32)(Center.Y - mnBigNeedleRadius / 8 * Math.Sin(needleAngle)));
            Point endPoint = new Point((Int32)(Center.X + mnBigNeedleRadius  * Math.Cos(needleAngle)),
                                     (Int32)(Center.Y + mnBigNeedleRadius  * Math.Sin(needleAngle)));

            pe.Graphics.FillEllipse(new SolidBrush(m_NeedleColor2), Center.X - m_NeedleWidth * 3, Center.Y - m_NeedleWidth * 3, m_NeedleWidth * 6, m_NeedleWidth * 6);

          
            pe.Graphics.DrawLine(new Pen(Color.Red, m_NeedleWidth), Center.X, Center.Y, endPoint.X, endPoint.Y);
            pe.Graphics.DrawLine(new Pen(Color.Red, m_NeedleWidth), Center.X, Center.Y, startPoint.X, startPoint.Y);
       
             
        }
        void DrawSmallNeedle(PaintEventArgs pe)
        {
            Point Center = new Point(this.Size.Width / 2, this.Size.Height / 2);

            //pe.Graphics.DrawImageUnscaled(gaugeBitmap, 0, 0);
            pe.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
            pe.Graphics.PixelOffsetMode = PixelOffsetMode.HighQuality;

            //画指针
            Single brushAngle = (Int32)(mnSmallArcStart + (mfSmallValue - mnSmallMinValue) * mnSmallArcCount / (mnSmallMaxValue - mnSmallMinValue)) % 360;
            Double needleAngle = brushAngle * Math.PI / 180;


            PointF[] points = new PointF[3];
            Brush brush1 = Brushes.White;
            Brush brush2 = Brushes.White;
            Brush brush3 = Brushes.White;
            Brush brush4 = Brushes.White;

            Brush brushBucket = Brushes.White;
            Int32 subcol = (Int32)(((brushAngle + 225) % 180) * 100 / 180);
            Int32 subcol2 = (Int32)(((brushAngle + 135) % 180) * 100 / 180);

            // pe.Graphics.FillEllipse(new SolidBrush(m_NeedleColor2), Center.X - m_NeedleWidth * 3, Center.Y - m_NeedleWidth * 3, m_NeedleWidth * 6, m_NeedleWidth * 6);
            brush1 = new SolidBrush(Color.FromArgb(80 + subcol, 80 + subcol, 80 + subcol));
            brush2 = new SolidBrush(Color.FromArgb(180 - subcol, 180 - subcol, 180 - subcol));
            brush3 = new SolidBrush(Color.FromArgb(80 + subcol2, 80 + subcol2, 80 + subcol2));
            brush4 = new SolidBrush(Color.FromArgb(180 - subcol2, 180 - subcol2, 180 - subcol2));
            //  pe.Graphics.DrawEllipse(Pens.Gray, Center.X - m_NeedleWidth * 3, Center.Y - m_NeedleWidth * 3, m_NeedleWidth * 6, m_NeedleWidth * 6);


            if (Math.Floor((Single)(((brushAngle + 225) % 360) / 180.0)) == 0)
            {
                brushBucket = brush1;
                brush1 = brush2;
                brush2 = brushBucket;
            }

            if (Math.Floor((Single)(((brushAngle + 135) % 360) / 180.0)) == 0)
            {
                brush4 = brush3;
            }

            points[0].X = (Single)(Center.X + mnSmallNeedleRadius * Math.Cos(needleAngle));
            points[0].Y = (Single)(Center.Y + mnSmallNeedleRadius * Math.Sin(needleAngle));
            points[1].X = (Single)(Center.X - mnSmallNeedleRadius / 20 * Math.Cos(needleAngle));
            points[1].Y = (Single)(Center.Y - mnSmallNeedleRadius / 20 * Math.Sin(needleAngle));
            points[2].X = (Single)(Center.X - mnSmallNeedleRadius / 5 * Math.Cos(needleAngle) + m_NeedleWidth * 2 * Math.Cos(needleAngle + Math.PI / 2));
            points[2].Y = (Single)(Center.Y - mnSmallNeedleRadius / 5 * Math.Sin(needleAngle) + m_NeedleWidth * 2 * Math.Sin(needleAngle + Math.PI / 2));
            pe.Graphics.FillPolygon(brush1, points);

            points[2].X = (Single)(Center.X - mnSmallNeedleRadius / 5 * Math.Cos(needleAngle) + m_NeedleWidth * 2 * Math.Cos(needleAngle - Math.PI / 2));
            points[2].Y = (Single)(Center.Y - mnSmallNeedleRadius / 5 * Math.Sin(needleAngle) + m_NeedleWidth * 2 * Math.Sin(needleAngle - Math.PI / 2));
            pe.Graphics.FillPolygon(brush2, points);

            points[0].X = (Single)(Center.X - (mnSmallNeedleRadius / 20 - 1) * Math.Cos(needleAngle));
            points[0].Y = (Single)(Center.Y - (mnSmallNeedleRadius / 20 - 1) * Math.Sin(needleAngle));
            points[1].X = (Single)(Center.X - mnSmallNeedleRadius / 5 * Math.Cos(needleAngle) + m_NeedleWidth * 2 * Math.Cos(needleAngle + Math.PI / 2));
            points[1].Y = (Single)(Center.Y - mnSmallNeedleRadius / 5 * Math.Sin(needleAngle) + m_NeedleWidth * 2 * Math.Sin(needleAngle + Math.PI / 2));
            points[2].X = (Single)(Center.X - mnSmallNeedleRadius / 5 * Math.Cos(needleAngle) + m_NeedleWidth * 2 * Math.Cos(needleAngle - Math.PI / 2));
            points[2].Y = (Single)(Center.Y - mnSmallNeedleRadius / 5 * Math.Sin(needleAngle) + m_NeedleWidth * 2 * Math.Sin(needleAngle - Math.PI / 2));
            pe.Graphics.FillPolygon(brush4, points);

            points[0].X = (Single)(Center.X - mnSmallNeedleRadius / 20 * Math.Cos(needleAngle));
            points[0].Y = (Single)(Center.Y - mnSmallNeedleRadius / 20 * Math.Sin(needleAngle));
            points[1].X = (Single)(Center.X + mnSmallNeedleRadius * Math.Cos(needleAngle));
            points[1].Y = (Single)(Center.Y + mnSmallNeedleRadius * Math.Sin(needleAngle));

            pe.Graphics.DrawLine(new Pen(m_NeedleColor2), Center.X, Center.Y, points[0].X, points[0].Y);
            pe.Graphics.DrawLine(new Pen(m_NeedleColor2), Center.X, Center.Y, points[1].X, points[1].Y);         
             
        }
        protected override void OnPaint(PaintEventArgs pe)
        {
            if ((Width < 10) || (Height < 10))
            {
                return;
            } 
            
            DrawBigNeedle(pe);
            DrawSmallNeedle(pe);
           
           
           
        }
 
#endregion

        private void OnLoad(object sender, EventArgs e)
        {
            //if (this.BackgroundImage != null)
             //   this.Size = this.BackgroundImage.Size;
            Int32 lnMax = this.Size.Width > this.Size.Height ? this.Size.Width : this.Size.Height;
            this.Size = new System.Drawing.Size(lnMax, lnMax);
        }

    }
}

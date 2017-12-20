
namespace CFMap.WindowsForms
{
   using System.Drawing;
   using System.Drawing.Imaging;
   using System.Drawing.Drawing2D;
   using System.Collections.Generic;
   using CFMap.CoreType;

   using CFMap.WinForm.Control.Properties;
   using System;
   using System.Runtime.Serialization;

    public enum RiskLevel
    {
        RiskLevel_none = 0,
        RiskLevel_normal,
        RiskLevel_lower,
        RiskLevel_low,
        RiskLevel_high,
        RiskLevel_higher,
        RiskLevel_highest
    };   


   public class CMarkerRisk : IMapMarker
   {
      public float? Bearing;
      Bitmap Bitmap;
      static Image BitmapBack;
      Single mfAng = 360.0f;

     
      //public event EventHandler<EventArgs> OnFrameChanged;   
     // static Bitmap sBitmapRisk;
      //static Bitmap shadow_small;
      //static Bitmap pushpin_shadow;

    
  

       //风险show 设置相关变量
      RiskLevel mLevel;
      String msOrgName;
      Int32 mnOrgLevel;
      //Color mcClorr;

       //风险Draw 相关变量
      Graphics mg = null;
      internal readonly Font DataFont = new Font(FontFamily.GenericSansSerif, 11, FontStyle.Bold);
      internal readonly StringFormat CenterFormat = new StringFormat();
      internal readonly Size DrawSize = new Size(20, 20);

    
      public CMarkerRisk(PointLatLng p,  RiskLevel l, String nsOrgName, Int32 nOrgLevel)
          : base(p)
      {
          this.mLevel = l;
          this.msOrgName = nsOrgName;
          this.mnOrgLevel = nOrgLevel;

          if (this.mLevel != RiskLevel.RiskLevel_none)
          {
              LoadBitmap();
          }
      }

      public CMarkerRisk(Int32 nID,PointLatLng p,Int32 nMinZoomLevel,Int32 nMaxZoomLevel, RiskLevel l,string nsOrgName,Int32 nOrgLevel)
         : base(nID,p,nMinZoomLevel,nMaxZoomLevel)
      {        
         this.mLevel = l;
         this.msOrgName = nsOrgName;
         this.mnOrgLevel = nOrgLevel;

          CenterFormat.Alignment = StringAlignment.Center;
          CenterFormat.LineAlignment = StringAlignment.Center;
         if (this.mLevel != RiskLevel.RiskLevel_none)
         {
            LoadBitmap();
         }
      }


      void LoadBitmap()
      {
          switch (mLevel)
          {
              case RiskLevel.RiskLevel_normal:
                  {
                     
                      Bitmap = GetIcon("green");
                      Size = new System.Drawing.Size(DrawSize.Width, DrawSize.Height);

                  }
                  break;
              case RiskLevel.RiskLevel_lower:
              case RiskLevel.RiskLevel_low:
              case RiskLevel.RiskLevel_high:
              case RiskLevel.RiskLevel_higher:
              case RiskLevel.RiskLevel_highest:
                  {
                      Bitmap = GetIcon("red");
                      Size = new System.Drawing.Size(DrawSize.Width, DrawSize.Height);
                      if (BitmapBack == null)
                          BitmapBack = Image.FromFile("gif\\wb2.png");
                      System.Timers.Timer t = new System.Timers.Timer(50);//实例化Timer类，设置间隔时间为10000毫秒；
                      t.Elapsed += new System.Timers.ElapsedEventHandler(AnimateTimeOut);//到达时间的时候执行事件；
                      t.AutoReset = true;//设置是执行一次（false）还是一直执行(true)；
                      t.Enabled = true;//是否执行System.Timers.Timer.Elapsed事件；
                      //if (sWarningBackGif == null)
                      //{
                      //    sWarningBackGif = Image.FromFile("gif\\pink.gif");
                      //    if (sWarningBackGif == null)
                      //        break;
                      //   // lock (sWarningBackGif)
                      //    {
                      //        Boolean mCanAnimate = ImageAnimator.CanAnimate(sWarningBackGif);
                      //        if (mCanAnimate)
                      //        {

                      //            Guid[] guid = sWarningBackGif.FrameDimensionsList;
                      //            sWarningBackGifframeDimension = new FrameDimension(guid[0]);
                      //            mnFrameCount = sWarningBackGif.GetFrameCount(sWarningBackGifframeDimension);
                      //            // ImageAnimator.Animate(sWarningBackGif, new EventHandler(FrameChanged));

                      //            System.Timers.Timer t = new System.Timers.Timer(50);//实例化Timer类，设置间隔时间为10000毫秒；
                      //            t.Elapsed += new System.Timers.ElapsedEventHandler(AnimateTimeOut);//到达时间的时候执行事件；
                      //            t.AutoReset = true;//设置是执行一次（false）还是一直执行(true)；
                      //            t.Enabled = true;//是否执行System.Timers.Timer.Elapsed事件；
                      //        }
                      //    }
                      //}
                  }
                  break;

          }        
      }

        public void AnimateTimeOut(object source, System.Timers.ElapsedEventArgs e)
        {
            // MessageBox.Show("OK!");
           
           // mnCurrentFrame = mnCurrentFrame + 1 >= mnFrameCount ? 0 : mnCurrentFrame + 1;
           //// lock (sWarningBackGif)
            
           // {
           //     sWarningBackGif.SelectActiveFrame(sWarningBackGifframeDimension, mnCurrentFrame);

           //    // sWarningBackGif.MakeTransparent(sWarningBackGif.GetPixel(1, 1));
           //     if (mg != null && sWarningBackGif != null && mLevel >= RiskLevel.RiskLevel_normal)
           //     {

           //         mg.DrawImage(sWarningBackGif, LocalPosition.X - 10, LocalPosition.Y - 10, Size.Width + 20, Size.Height + 20);

           //     }  
                
           // }
            
            base.MapOverlay.Control.Invalidate();
        }




      static readonly Dictionary<string, Bitmap> iconCache = new Dictionary<string, Bitmap>();

      internal static Bitmap GetIcon(string name)
      {
         Bitmap ret;
         if(!iconCache.TryGetValue(name, out ret))
         {
            ret = Resources.ResourceManager.GetObject(name, Resources.Culture) as Bitmap;
            iconCache.Add(name, ret);
         }
         return ret;
      }

      static readonly Point[] Arrow = new Point[] { new Point(-7, 7), new Point(0, -22), new Point(7, 7), new Point(0, 2) };

      public override void OnRender(Graphics g)
      {

          mg = g;

         if (mg != null && BitmapBack != null && mLevel > RiskLevel.RiskLevel_normal)
         {
             //BitmapBack.RotateFlip()
             // mg.TranslateTransform(LocalPosition.X, LocalPosition.Y);
             mfAng -= 10;
             if (mfAng > 360f)
                 mfAng = 0.0f;

             GraphicsState transState = g.Save();

             g.TranslateTransform(LocalPosition.X + Size.Width / 2, LocalPosition.Y + Size.Height / 2);
             g.RotateTransform(mfAng);
             g.DrawImage(BitmapBack, -((Size.Width + 20) / 2), -((Size.Height + 20) / 2), Size.Width + 20, Size.Height + 20);

             g.Restore(transState);


         }  
         if(Bitmap != null)
         {
            g.DrawImage(Bitmap, LocalPosition.X, LocalPosition.Y, Size.Width, Size.Height);
         }
         if (msOrgName.Length != 0)
         {
            // g.DrawString(msOrgName, LocalPosition.X, LocalPosition.Y, Size.Width, Size.Height);
             g.DrawString(msOrgName, DataFont, Brushes.Blue, new RectangleF(LocalPosition.X - 50 + DrawSize.Width / 2, LocalPosition.Y + DrawSize.Height, 100, 15), CenterFormat);

         }
        
       

      }

      public override void Dispose()
      {
         if(Bitmap != null)
         {
            if(!iconCache.ContainsValue(Bitmap))
            {
               Bitmap.Dispose();
               Bitmap = null;
            }
         }
         if (BitmapBack != null)
         {

             BitmapBack.Dispose();
             BitmapBack = null;
            
         }
         //if (sWarningBackGif != null)
         //{
         //    lock (sWarningBackGif)   
         //      {
         //          ImageAnimator.StopAnimate(sWarningBackGif, new EventHandler(FrameChanged));   
         //      }   
         // }
      
         base.Dispose();
      }
   
   }

 
     

}

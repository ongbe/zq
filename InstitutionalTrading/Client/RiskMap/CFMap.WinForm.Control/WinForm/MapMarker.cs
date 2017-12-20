
namespace CFMap.WindowsForms
{
   using System;
   using System.Drawing;
   using System.Runtime.Serialization;
   using System.Windows.Forms;

    using CFMap.CoreType;
    using CFMap.CoreType.Internals;

   /// <summary>
   /// GMap.NET marker
   /// </summary>  
   public abstract class IMapMarker :  IDisposable
   {


       CFMapOverlay mMapOverlay;
       public CFMapOverlay MapOverlay
      {
         get
         {
             return mMapOverlay;
         }
         internal set
         {
             mMapOverlay = value;
         }
      }

       Int32 mnMinShowZoomLevel;
       public Int32 MinShowZoomLevel
       {
           get
           {
               return mnMinShowZoomLevel;
           }
           internal set
           {
               mnMinShowZoomLevel = value;
           }
       }
       Int32 mnMaxShowZoomLevel;
       public Int32 MaxShowZoomLevel
       {
           get
           {
               return mnMaxShowZoomLevel;
           }
           internal set
           {
               mnMaxShowZoomLevel = value;
           }
       }

       Int32 mnMakerID;
       public Int32 MakerID
       {
           get
           {
               return mnMakerID;
           }
           internal set
           {
               mnMakerID = value;
           }
       }

      private PointLatLng position;
      public PointLatLng Position
      {
         get
         {
            return position;
         }
         set
         {
            if(position != value)
            {
               position = value;

               if(IsVisible)
               {
                   if (MapOverlay != null && MapOverlay.Control != null)
                  {
                      MapOverlay.Control.UpdateMarkerLocalPosition(this);
                  }
               }
            }
         }
      }

      public object Tag;

      Point offset;
      public Point Offset
      {
         get
         {
            return offset;
         }
         set
         {
            offset = value;
         }
      }

      Rectangle area;

      /// <summary>
      /// marker position in local coordinates, internal only, do not set it manualy
      /// </summary>
      public Point LocalPosition
      {
         get
         {
            return area.Location;
         }
         set
         {
            if(area.Location != value)
            {
               area.Location = value;
               {
                   if (MapOverlay != null && MapOverlay.Control != null)
                  {
                      if (!MapOverlay.Control.HoldInvalidation)
                     {
                         MapOverlay.Control.Core.Refresh.Set();
                     }
                  }
               }
            }
         }
      }

    

      public Size Size
      {
         get
         {
            return area.Size;
         }
         set
         {
            area.Size = value;
         }
      }

      public Rectangle LocalArea
      {
         get
         {
            return area;
         }
      }

      internal Rectangle LocalAreaInControlSpace
      {
         get
         {
            Rectangle r = area;
            if (MapOverlay != null && MapOverlay.Control != null)
            {
                r.Offset((int)MapOverlay.Control.Core.renderOffset.X, (int)MapOverlay.Control.Core.renderOffset.Y);
            }
            return r;
         }
      }

  
      

      private bool visible = true;

      /// <summary>
      /// is marker visible
      /// </summary>
      public bool IsVisible
      {
         get
         {
            return visible;
         }
         set
         {
            if(value != visible)
            {
               visible = value;

               if (MapOverlay != null && MapOverlay.Control != null)
               {
                  if(visible)
                  {
                      MapOverlay.Control.UpdateMarkerLocalPosition(this);
                  }

                  {
                      if (!MapOverlay.Control.HoldInvalidation)
                     {
                         MapOverlay.Control.Core.Refresh.Set();
                     }
                  }
               }
            }
         }
      }

      /// <summary>
      /// if true, marker will be rendered even if it's outside current view
      /// </summary>
      public bool DisableRegionCheck = false;

      /// <summary>
      /// can maker receive input
      /// </summary>
      public bool IsHitTestVisible = true;

      private bool isMouseOver = false;

      /// <summary>
      /// is mouse over marker
      /// </summary>
      public bool IsMouseOver
      {
         get
         {
            return isMouseOver;
         }
         internal set
         {
            isMouseOver = value;              
         }
      }
      public IMapMarker(PointLatLng pos)
      {
          this.Position = pos;       
      }
      public IMapMarker(Int32 nID,PointLatLng pos,Int32 nMinZoomLevel,Int32 nMaxZoomLevel)
      {
         this.MakerID = nID;
         this.Position = pos;
         this.MinShowZoomLevel = nMinZoomLevel;
         this.MaxShowZoomLevel = nMaxZoomLevel;
      }

      public virtual void OnRender(Graphics g)
      {
         //
      }





      #region IDisposable Members

      bool disposed = false;

      public virtual void Dispose()
      {
         if(!disposed)
         {
            disposed = true;

            Tag = null;           
         }
      }

      #endregion
   }

   public delegate void MarkerClick(IMapMarker item, MouseEventArgs e);
   public delegate void MarkerEnter(IMapMarker item);
   public delegate void MarkerLeave(IMapMarker item);

   /// <summary>
   /// modeof tooltip
   /// </summary>
   public enum MarkerTooltipMode
   {
      OnMouseOver,
      Never,
      Always,
   }
}

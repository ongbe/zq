
namespace CFMap.WindowsForms
{
   using System;
   using System.Drawing;
   using System.Runtime.Serialization;
   using System.Windows.Forms;
    using CFMap.ObjectModel;
    using CFMap.CoreType;
    using CFMap.CoreType.Internals;
   /// <summary>
   /// GMap.NET overlay
   /// </summary>

   public class CFMapOverlay :  IDisposable
   {
      bool isVisibile = true;

      /// <summary>
      /// is overlay visible
      /// </summary>
      public bool IsVisibile
      {
         get
         {
            return isVisibile;
         }
         set
         {
            if(value != isVisibile)
            {
               isVisibile = value;

               if(Control != null)
               {
                  if(isVisibile)
                  {
                     Control.HoldInvalidation = true;
                     ForceUpdate();
                     Control.Refresh();
                  }
                  else
                  {
                     if(!Control.HoldInvalidation)
                     {
                        Control.Invalidate();
                     }
                  }
               }
            }
         }
      }

      /// <summary>
      /// overlay Id
      /// </summary>
      public string Id;

      /// <summary>
      /// list of markers, should be thread safe
      /// </summary>
      public readonly ObservableCollectionThreadSafe<IMapMarker> Markers = new ObservableCollectionThreadSafe<IMapMarker>();

     
      CFMapControl control;
      public CFMapControl Control
      {
         get
         {
            return control;
         }
         internal set
         {
            control = value;
         }
      }

      public CFMapOverlay()
      {
         CreateEvents();
      }

      public CFMapOverlay(string id)
      {
         Id = id;
         CreateEvents();
      }

      void CreateEvents()
      {
         Markers.CollectionChanged += new NotifyCollectionChangedEventHandler(Markers_CollectionChanged);
      }

      void ClearEvents()
      {
         Markers.CollectionChanged -= new NotifyCollectionChangedEventHandler(Markers_CollectionChanged);  
      }

      public void Clear()
      {
         Markers.Clear();
 
      }

    
      void Markers_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
      {
         if(e.NewItems != null)
         {
            foreach(IMapMarker obj in e.NewItems)
            {
               if(obj != null)
               {
                   obj.MapOverlay = this;
                  if(Control != null)
                  {
                     Control.UpdateMarkerLocalPosition(obj);
                  }
               }
            }
         }

         if(Control != null)
         {
            if(e.Action == NotifyCollectionChangedAction.Remove || e.Action == NotifyCollectionChangedAction.Reset)
            {
               if(Control.IsMouseOverMarker)
               {
                  Control.IsMouseOverMarker = false;
                  Control.RestoreCursorOnLeave();
               }
            }

            if(!Control.HoldInvalidation)
            {
               Control.Invalidate();
            }
         }
      }

      /// <summary>
      /// updates local positions of objects
      /// </summary>
      internal void ForceUpdate()
      {
         if(Control != null)
         {
             foreach (IMapMarker obj in Markers)
            {
               if(obj.IsVisible)
               {
                  Control.UpdateMarkerLocalPosition(obj);
               }
            }
           
         }
      }

      /// <summary>
      /// renders objects/routes/polygons
      /// </summary>
      /// <param name="g"></param>
      public virtual void OnRender(Graphics g,Int32 nZoomLevel)
      {
         if(Control != null)
         {
                      
            if(Control.MarkersEnabled)
            {
               // markers
                foreach (IMapMarker m in Markers)
               {
                  //if(m.IsVisible && (m.DisableRegionCheck || Control.Core.currentRegion.Contains(m.LocalPosition.X, m.LocalPosition.Y)))
                  if(m.IsVisible || m.DisableRegionCheck)
                  {
                      if (nZoomLevel <= m.MaxShowZoomLevel && nZoomLevel >= m.MinShowZoomLevel)
                         m.OnRender(g);
                  }
               }
             
            }
         }
      }


       //判断地图缩放变化是否引起了maker的显示变化
      public virtual bool IsChangedShowMarker(Int32 nOldZoomLevel, Int32 nNewZoomLevel)
      {
          foreach (IMapMarker m in Markers)
          {
              if (nNewZoomLevel <= m.MaxShowZoomLevel && nNewZoomLevel >= m.MinShowZoomLevel)
              {
                  m.IsVisible = true;
                  //查看nOldZoomLevel是否也属于新的这些maker里面
                  if (nOldZoomLevel <= m.MaxShowZoomLevel && nOldZoomLevel >= m.MinShowZoomLevel)
                      return false;
              }
              else
                  m.IsVisible = false;
            
          }
          return true;

      }

      public virtual IMapMarker CheckWhickMarkCloseToCenter(Int32 nZoolLevel,PointLatLng npCenter)
      {
          IMapMarker lnColser = null;
          double lnMin = 10000000000000.0;
          foreach (IMapMarker m in Markers)
          {
              if (nZoolLevel <= m.MaxShowZoomLevel && nZoolLevel >= m.MinShowZoomLevel)
              {
                 
                  double ldbTemp = npCenter.LengthTwoPoint(m.Position);
                  if (lnMin > ldbTemp)
                  {
                      lnColser = m;
                      lnMin = ldbTemp;
                  }
              }

          }
          return lnColser;

      }



      #region IDisposable Members

      bool disposed = false;

      public void Dispose()
      {
         if(!disposed)
         {
            disposed = true;

            ClearEvents();

            foreach(var m in Markers)
            {
               m.Dispose();
            }
         
            Clear();
         }
      }

      #endregion
   }
}
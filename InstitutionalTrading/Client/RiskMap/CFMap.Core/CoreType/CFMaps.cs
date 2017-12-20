
namespace CFMap.CoreType
{
   using System;
   using System.Collections.Generic;
   using System.Diagnostics;
   using System.Globalization;
   using System.IO;
   using System.Net;
   using System.Text;
   using System.Threading;
   using System.Xml;
   using System.Xml.Serialization;
  
    using CFMap.CoreType.Internals;
    using CFMap.Interface;
    using CFMap.CacheProviders;



   /// <summary>
   /// maps manager
   /// </summary>
   public class CFMaps : Singleton<CFMaps>
   {
      /// <summary>
      /// tile access mode
      /// </summary>
      public AccessMode Mode = AccessMode.ServerAndCache;

      /// <summary>
      /// is map ussing cache for routing
      /// </summary>
      public bool UseRouteCache = true;

      /// <summary>
      /// is map using cache for geocoder
      /// </summary>
      public bool UseGeocoderCache = true;

      /// <summary>
      /// is map using cache for directions
      /// </summary>
      public bool UseDirectionsCache = true;

      /// <summary>
      /// is map using cache for placemarks
      /// </summary>
      public bool UsePlacemarkCache = true;

      /// <summary>
      /// is map ussing cache for other url
      /// </summary>
      public bool UseUrlCache = true;

      /// <summary>
      /// is map using memory cache for tiles
      /// </summary>
      public bool UseMemoryCache = true;

      /// <summary>
      /// primary cache provider, by default: ultra fast SQLite!
      /// </summary>
      public PureImageCache PrimaryCache
      {
         get
         {
            return Cache.Instance.ImageCache;
         }
         set
         {
            Cache.Instance.ImageCache = value;
         }
      }

      /// <summary>
      /// secondary cache provider, by default: none,
      /// use it if you have server in your local network
      /// </summary>
      public PureImageCache SecondaryCache
      {
         get
         {
            return Cache.Instance.ImageCacheSecond;
         }
         set
         {
            Cache.Instance.ImageCacheSecond = value;
         }
      }

      /// <summary>
      /// MemoryCache provider
      /// </summary>
      public readonly MemoryCache MemoryCache = new MemoryCache();

      /// <summary>
      /// load tiles in random sequence
      /// </summary>
      public bool ShuffleTilesOnLoad = true;

      /// <summary>
      /// tile queue to cache
      /// </summary>
      readonly Queue<CacheQueueItem> tileCacheQueue = new Queue<CacheQueueItem>();

      bool? isRunningOnMono;

      /// <summary>
      /// return true if running on mono
      /// </summary>
      /// <returns></returns>
      public bool IsRunningOnMono
      {
         get
         {
            if(!isRunningOnMono.HasValue)
            {
               try
               {
                  isRunningOnMono = (Type.GetType("Mono.Runtime") != null);
                  return isRunningOnMono.Value;
               }
               catch
               {
               }
            }
            else
            {
               return isRunningOnMono.Value;
            }
            return false;
         }
      }

      /// <summary>
      /// cache worker
      /// </summary>
      Thread CacheEngine;

      internal readonly AutoResetEvent WaitForCache = new AutoResetEvent(false);

      public CFMaps()
      {
         #region singleton check
         if(Instance != null)
         {
            throw (new Exception("You have tried to create a new singleton class where you should have instanced it. Replace your \"new class()\" with \"class.Instance\""));
         }
         #endregion

         ServicePointManager.DefaultConnectionLimit = 5;
      }


      /// <summary>
      /// triggers dynamic sqlite loading, 
      /// call this before you use sqlite for other reasons than caching maps
      /// </summary>
      public void SQLitePing()
      {

         SQLitePureImageCache.Ping();

      }


      #region -- Stuff --


    

      /// <summary>
      /// optimizes map database, *.gmdb
      /// </summary>
      /// <param name="file">database file name or null to optimize current user db</param>
      /// <returns></returns>
      public bool OptimizeMapDb(string file)
      {
         if(PrimaryCache is CFMap.CacheProviders.SQLitePureImageCache)
         {
            if(string.IsNullOrEmpty(file))
            {
               StringBuilder db = new StringBuilder((PrimaryCache as SQLitePureImageCache).GtileCache);
               db.AppendFormat(CultureInfo.InvariantCulture, "{0}{1}Data.gmdb", IMapProvider.LanguageStr, Path.DirectorySeparatorChar);

               return SQLitePureImageCache.VacuumDb(db.ToString());
            }
            else
            {
               return SQLitePureImageCache.VacuumDb(file);
            }
         }

         return false;
      }



      /// <summary>
      /// enqueueens tile to cache
      /// </summary>
      /// <param name="task"></param>
      void EnqueueCacheTask(CacheQueueItem task)
      {
         lock(tileCacheQueue)
         {
            if(!tileCacheQueue.Contains(task))
            {
               Debug.WriteLine("EnqueueCacheTask: " + task);

               tileCacheQueue.Enqueue(task);

               if(CacheEngine != null && CacheEngine.IsAlive)
               {
                  WaitForCache.Set();
               }

               else if(CacheEngine == null || CacheEngine.ThreadState == System.Threading.ThreadState.Stopped || CacheEngine.ThreadState == System.Threading.ThreadState.Unstarted)
               {
                  CacheEngine = null;
                  CacheEngine = new Thread(new ThreadStart(CacheEngineLoop));
                  CacheEngine.Name = "CacheEngine";
                  CacheEngine.IsBackground = false;
                  CacheEngine.Priority = ThreadPriority.Lowest;

                  abortCacheLoop = false;
                  CacheEngine.Start();
               }
            }
         }
      }

      volatile bool abortCacheLoop = false;
      internal volatile bool noMapInstances = false;

      public TileCacheComplete OnTileCacheComplete;
      public TileCacheStart OnTileCacheStart;
      public TileCacheProgress OnTileCacheProgress;

      /// <summary>
      /// immediately stops background tile caching, call it if you want fast exit the process
      /// </summary>
      public void CancelTileCaching()
      {
         Debug.WriteLine("CancelTileCaching...");

         abortCacheLoop = true;
         lock(tileCacheQueue)
         {
            tileCacheQueue.Clear();
            WaitForCache.Set();
         }
      }

      int readingCache = 0;

      /// <summary>
      /// delays writing tiles to cache while performing reads
      /// </summary>
      public volatile bool CacheOnIdleRead = true;

      /// <summary>
      /// disables delay between saving tiles into database/cache
      /// </summary>
      public volatile bool BoostCacheEngine = false;

      /// <summary>
      /// live for cache ;}
      /// </summary>
      /// <param name="sender"></param>
      /// <param name="e"></param>
      void CacheEngineLoop()
      {
         Debug.WriteLine("CacheEngine: start");
         int left = 0;

         if(OnTileCacheStart != null)
         {
            OnTileCacheStart();
         }

         bool startEvent = false;

         while(!abortCacheLoop)
         {
            try
            {
               CacheQueueItem? task = null;

               lock(tileCacheQueue)
               {
                  left = tileCacheQueue.Count;
                  if(left > 0)
                  {
                     task = tileCacheQueue.Dequeue();
                  }
               }

               if(task.HasValue)
               {
                  if(startEvent)
                  {
                     startEvent = false;

                     if(OnTileCacheStart != null)
                     {
                        OnTileCacheStart();
                     }
                  }

                  if(OnTileCacheProgress != null)
                  {
                     OnTileCacheProgress(left);
                  }

                  #region -- save --
                  // check if stream wasn't disposed somehow
                  if(task.Value.Img != null)
                  {
                     Debug.WriteLine("CacheEngine[" + left + "]: storing tile " + task.Value + ", " + task.Value.Img.Length / 1024 + "kB...");

                     if((task.Value.CacheType & CacheUsage.First) == CacheUsage.First && PrimaryCache != null)
                     {
                        if(CacheOnIdleRead)
                        {
                           while(Interlocked.Decrement(ref readingCache) > 0)
                           {
                              Thread.Sleep(1000);
                           }
                        }
                        PrimaryCache.PutImageToCache(task.Value.Img,task.Value.Tile.MapId, task.Value.Tile.Pos, task.Value.Tile.Zoom);
                     }

                     //if((task.Value.CacheType & CacheUsage.Second) == CacheUsage.Second && SecondaryCache != null)
                     //{
                     //   if(CacheOnIdleRead)
                     //   {
                     //      while(Interlocked.Decrement(ref readingCache) > 0)
                     //      {
                     //         Thread.Sleep(1000);
                     //      }
                     //   }
                     //   SecondaryCache.PutImageToCache(task.Value.Img,  task.Value.Tile.Pos, task.Value.Tile.Zoom);
                     //}

                     task.Value.Clear();

                     if(!BoostCacheEngine)
                     {
                        Thread.Sleep(333);
                     }
                  }
                  else
                  {
                     Debug.WriteLine("CacheEngineLoop: skip, tile disposed to early -> " + task.Value);
                  }
                  task = null;
                  #endregion
               }
               else
               {
                  if(!startEvent)
                  {
                     startEvent = true;

                     if(OnTileCacheComplete != null)
                     {
                        OnTileCacheComplete();
                     }
                  }

                  if(abortCacheLoop || noMapInstances || !WaitForCache.WaitOne(33333, false) || noMapInstances)
                  {
                     break;
                  }
               }
            }

            catch(AbandonedMutexException)
            {
               break;
            }
            catch(Exception ex)
            {
               Debug.WriteLine("CacheEngineLoop: " + ex.ToString());
            }
         }
         Debug.WriteLine("CacheEngine: stop");

         if(!startEvent)
         {
            if(OnTileCacheComplete != null)
            {
               OnTileCacheComplete();
            }
         }
      }

      class StringWriterExt : StringWriter
      {
         public StringWriterExt(IFormatProvider info)
            : base(info)
         {

         }

         public override Encoding Encoding
         {
            get
            {
               return Encoding.UTF8;
            }
         }
      }

     

      #endregion

      /// <summary>
      /// gets image from tile server
      /// </summary>
      /// <param name="provider"></param>
      /// <param name="pos"></param>
      /// <param name="zoom"></param>
      /// <returns></returns>
      public PureImage GetImageFrom(IMapProvider provider, GPoint pos, int zoom, out Exception result)
      {
         PureImage ret = null;
         result = null;

         try
         {
            var rtile = new RawTile(provider.MapId,pos, zoom);

            // let't check memmory first
            if(UseMemoryCache)
            {
               var m = MemoryCache.GetTileFromMemoryCache(rtile);
               if(m != null)
               {
                  if(IMapProvider.TileImageProxy != null)
                  {
                      ret = IMapProvider.TileImageProxy.FromArray(m);
                     if(ret == null)
                     {
#if DEBUG
                         Debug.WriteLine("Image disposed in MemoryCache o.O, should never happen ;} " + new RawTile(provider.MapId, pos, zoom));
                        if(Debugger.IsAttached)
                        {
                           Debugger.Break();
                        }
#endif
                        m = null;
                     }
                  }
               }
            }

            if(ret == null)
            {
               if(Mode != AccessMode.ServerOnly)
               {
                  if(PrimaryCache != null)
                  {
                     // hold writer for 5s
                     if(CacheOnIdleRead)
                     {
                        Interlocked.Exchange(ref readingCache, 5);
                     }

                     ret = PrimaryCache.GetImageFromCache(provider.MapId, pos, zoom);
                     if(ret != null)
                     {
                        if(UseMemoryCache)
                        {
                           MemoryCache.AddTileToMemoryCache(rtile, ret.Data.GetBuffer());
                        }
                        return ret;
                     }
                  }
                
               }

               if(Mode != AccessMode.CacheOnly)
               {
                  ret = provider.GetTileImage(pos, zoom);
                  {
                     // Enqueue Cache
                     if(ret != null)
                     {
                        if(UseMemoryCache)
                        {
                           MemoryCache.AddTileToMemoryCache(rtile, ret.Data.GetBuffer());
                        }

                        if(Mode != AccessMode.ServerOnly)
                        {
                           EnqueueCacheTask(new CacheQueueItem(rtile, ret.Data.GetBuffer(), CacheUsage.Both));
                        }
                     }
                  }
               }
               else
               {
                  result = noDataException;
               }
            }
         }
         catch(Exception ex)
         {
            result = ex;
            ret = null;
            Debug.WriteLine("GetImageFrom: " + ex.ToString());
         }

         return ret;
      }

      readonly Exception noDataException = new Exception("No data in local tile cache...");
   }
}

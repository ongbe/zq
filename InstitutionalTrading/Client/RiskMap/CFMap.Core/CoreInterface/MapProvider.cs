
namespace CFMap.Interface
{
   using System;
   using System.Collections.Generic;
   using System.Diagnostics;
   using System.IO;
   using System.Net;
   using System.Security.Cryptography;
    using CFMap.CoreType;
    using CFMap.CoreType.Internals;
    using CFMap.MapProviders;
   using System.Text;
    using CFMap.Projections;

    public class CCFMapProviders
    {     

        public static readonly EmptyProvider EmptyProvider = EmptyProvider.Instance;
        public static readonly OpenStreetMapProvider OpenStreetMap = OpenStreetMapProvider.Instance;
        public static readonly GoogleSatelliteMapProvider GoogleSatelliteMap = GoogleSatelliteMapProvider.Instance;
        public static readonly GoogleChinaSatelliteMapProvider GoogleChinaSatelliteMap = GoogleChinaSatelliteMapProvider.Instance;
        public static readonly BingMapProvider BingMap = BingMapProvider.Instance;
        public static readonly BingSatelliteMapProvider BingSatelliteMap = BingSatelliteMapProvider.Instance;
    }
     
   /// <summary>
   /// base class for each map provider
   /// </summary>
   public abstract class IMapProvider
   {
      /// <summary>
      /// unique provider id
      /// </summary>
      public abstract Guid Id
      {
         get;
      }

      public abstract int MapId
      {
          get;
      }
      /// <summary>
      /// provider name
      /// </summary>
      public abstract string Name
      {
         get;
      }

      /// <summary>
      /// provider projection
      /// </summary>
      public abstract PureProjection Projection
      {
         get;
      }

      /// <summary>
      /// provider overlays
      /// </summary>
      public abstract IMapProvider[] Overlays
      {
         get;
      }

      /// <summary>
      /// gets tile image using implmented provider
      /// </summary>
      /// <param name="pos"></param>
      /// <param name="zoom"></param>
      /// <returns></returns>
      public abstract PureImage GetTileImage(GPoint pos, int zoom);

     
      static readonly SHA1CryptoServiceProvider HashProvider = new SHA1CryptoServiceProvider();

     

      static IMapProvider()
      {
         WebProxy = EmptyWebProxy.Instance;
      }

      bool isInitialized = false;

      /// <summary>
      /// was provider initialized
      /// </summary>
      public bool IsInitialized
      {
         get
         {
            return isInitialized;
         }
         internal set
         {
            isInitialized = value;
         }
      }

      /// <summary>
      /// called before first use
      /// </summary>
      public virtual void OnInitialized()
      {
         // nice place to detect current provider version
      }

   
      /// <summary>
      /// area of map
      /// </summary>
      public RectLatLng? Area;

      /// <summary>
      /// minimum level of zoom
      /// </summary>
      public int MinZoom;

      /// <summary>
      /// maximum level of zoom
      /// </summary>
      public int? MaxZoom = 17;

      /// <summary>
      /// proxy for net access
      /// </summary>
      public static IWebProxy WebProxy;

      /// <summary>
      /// NetworkCredential for tile http access
      /// </summary>
      public static ICredentials Credential;

      /// <summary>
      /// Gets or sets the value of the User-agent HTTP header.
      /// It's pseudo-randomized to avoid blockages...
      /// </summary>                  
      public static string UserAgent = string.Format("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:{0}.0) Gecko/{2}{3:00}{4:00} Firefox/{0}.0.{1}", Stuff.random.Next(3, 14), Stuff.random.Next(1, 10), Stuff.random.Next(DateTime.Today.Year - 4, DateTime.Today.Year), Stuff.random.Next(12), Stuff.random.Next(30));

      /// <summary>
      /// timeout for provider connections
      /// </summary>
      public static int TimeoutMs = 5 * 1000;

      /// <summary>
      /// Gets or sets the value of the Referer HTTP header.
      /// </summary>
      public string RefererUrl = string.Empty;

      public string Copyright = string.Empty;

      /// <summary>
      /// true if tile origin at BottomLeft, WMS-C
      /// </summary>
      public bool InvertedAxisY = false;

      static string languageStr = "en";
      public static string LanguageStr
      {
         get
         {
            return languageStr;
         }
      }
      static LanguageType language = LanguageType.English;

      /// <summary>
      /// map language
      /// </summary>
      public static LanguageType Language
      {
         get
         {
            return language;
         }
         set
         {
            language = value;
            languageStr = Stuff.EnumToString(Language);
         }
      }

      /// <summary>
      /// internal proxy for image managment
      /// </summary>
      public static PureImageProxy TileImageProxy;

      static readonly string requestAccept = "*/*";
      static readonly string responseContentType = "image";

      protected virtual bool CheckTileImageHttpResponse(HttpWebResponse response)
      {
         //Debug.WriteLine(response.StatusCode + "/" + response.StatusDescription + "/" + response.ContentType + " -> " + response.ResponseUri);
         return response.ContentType.Contains(responseContentType);
      }

      public PureImage GetTileImageUsingHttp(string url)
      {
         PureImage ret = null;

         HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
         if(WebProxy != null)
         {
            request.Proxy = WebProxy;
         }

         if(Credential != null)
         {
            request.PreAuthenticate = true;
            request.Credentials = Credential;
         }

         request.UserAgent = UserAgent;
         request.Timeout = TimeoutMs;
         request.ReadWriteTimeout = TimeoutMs * 6;
         request.Accept = requestAccept;
         request.Referer = RefererUrl;

         using(HttpWebResponse response = request.GetResponse() as HttpWebResponse)
         {
            if(CheckTileImageHttpResponse(response))
            {
               MemoryStream responseStream = Stuff.CopyStream(response.GetResponseStream(), false);
               {
                  Debug.WriteLine("Response[" + responseStream.Length + " bytes]: " + url);

                  if(responseStream.Length > 0)
                  {
                     ret = TileImageProxy.FromStream(responseStream);
                  }

                  if(ret != null)
                  {
                     ret.Data = responseStream;
                     ret.Data.Position = 0;
                  }
                  else
                  {
                     responseStream.Dispose();
                  }
               }
               responseStream = null;
            }
            else
            {
               Debug.WriteLine("CheckTileImageHttpResponse[false]: " + url);
            }

            response.Close();
         }
         return ret;
      }

      public string GetContentUsingHttp(string url)
      {
         string ret = string.Empty;

         HttpWebRequest request = (HttpWebRequest)WebRequest.Create(url);
         if(WebProxy != null)
         {
            request.Proxy = WebProxy;
         }

         if(Credential != null)
         {
            request.PreAuthenticate = true;
            request.Credentials = Credential;
         }

         request.UserAgent = UserAgent;
         request.Timeout = TimeoutMs;
         request.ReadWriteTimeout = TimeoutMs * 6;
         request.Accept = requestAccept;
         request.Referer = RefererUrl;

         using(HttpWebResponse response = request.GetResponse() as HttpWebResponse)
         {
            //if(response.StatusCode == HttpStatusCode.OK)
            {
               using(Stream responseStream = response.GetResponseStream())
               {
                  using(StreamReader read = new StreamReader(responseStream, Encoding.UTF8))
                  {
                     ret = read.ReadToEnd();
                  }
               }
            }

            response.Close();
         }

         return ret;
      }

      public static int GetServerNum(GPoint pos, int max)
      {
         return (int)(pos.X + 2 * pos.Y) % max;
      }

   
      public override bool Equals(object obj)
      {
         if(obj is IMapProvider)
         {
            return Id.Equals((obj as IMapProvider).Id);
         }
         return false;
      }

      public override int GetHashCode()
      {
          return this.Id.GetHashCode() ;
      }

      public override string ToString()
      {
         return Name;
      }
   }

   ///实现一个空的 MapProvider 和 WebProxy
   ///
   /// <summary>
   /// represents empty provider
   /// </summary>
   public class EmptyProvider : IMapProvider
   {
       public static readonly EmptyProvider Instance;

       EmptyProvider()
       {
           MaxZoom = null;
       }

       static EmptyProvider()
       {
           Instance = new EmptyProvider();
       }

       #region IMapProvider Members

       public override Guid Id
       {
           get
           {
               return Guid.Empty;
           }
       }
       public override int MapId
       {
           get
           {
               return 0;
           }
       }

       readonly string name = "None";
       public override string Name
       {
           get
           {
               return name;
           }
       }

    
       public override PureProjection Projection
       {
           get
           {
               return MercatorProjection.Instance;
           }
       }

       public override IMapProvider[] Overlays
       {
           get
           {
               return null;
           }
       }

       public override PureImage GetTileImage(GPoint pos, int zoom)
       {
           return null;
       }

       #endregion
   }

   public sealed class EmptyWebProxy : IWebProxy
   {
       public static readonly EmptyWebProxy Instance = new EmptyWebProxy();

       private ICredentials m_credentials;
       public ICredentials Credentials
       {
           get
           {
               return this.m_credentials;
           }
           set
           {
               this.m_credentials = value;
           }
       }

       public Uri GetProxy(Uri uri)
       {
           return uri;
       }

       public bool IsBypassed(Uri uri)
       {
           return true;
       }
   }
}

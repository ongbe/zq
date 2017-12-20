//Open Street Map 提供者
namespace CFMap.MapProviders
{
    using System;
    using System.Collections.Generic;
    using System.Diagnostics;
    using System.Globalization;
    using System.Xml;
    using CFMap.CoreType;
    using CFMap.CoreType.Internals;
    using CFMap.Interface;
    using CFMap.Projections;

   public abstract class OpenStreetMapProviderBase : IMapProvider
   {
      public OpenStreetMapProviderBase()
      {
         MaxZoom = null;
         RefererUrl = "http://www.openstreetmap.org/";
         Copyright = string.Format("© OpenStreetMap - Map data ©{0} OpenStreetMap", DateTime.Today.Year);
      }

      public readonly string ServerLetters = "abc";

      #region IMapProvider Members

      public override Guid Id
      {
         get
         {
            throw new NotImplementedException();
         }
      }
      public override int MapId
      {
          get
          {
              return 101;
          }
      }

      public override string Name
      {
         get
         {
            throw new NotImplementedException();
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
            throw new NotImplementedException();
         }
      }

      public override PureImage GetTileImage(GPoint pos, int zoom)
      {
         throw new NotImplementedException();
      }

      #endregion



    
   }

   /// <summary>
   /// OpenStreetMap provider - http://www.openstreetmap.org/
   /// </summary>
   public class OpenStreetMapProvider : OpenStreetMapProviderBase
   {
      public static readonly OpenStreetMapProvider Instance;

      OpenStreetMapProvider()
      {
      }

      static OpenStreetMapProvider()
      {
         Instance = new OpenStreetMapProvider();
      }

      #region IMapProvider Members

      readonly Guid id = new Guid("0521335C-92EC-47A8-98A5-6FD333DDA9C0");
      public override Guid Id
      {
         get
         {
            return id;
         }
      }

      readonly string name = "OpenStreetMap";
      public override string Name
      {
         get
         {
            return name;
         }
      }

      IMapProvider[] overlays;
      public override IMapProvider[] Overlays
      {
          get
          {
              if (overlays == null)
              {
                  overlays = new IMapProvider[] { this };
              }
              return overlays;
          }
      }

      public override PureImage GetTileImage(GPoint pos, int zoom)
      {
         string url = MakeTileImageUrl(pos, zoom, string.Empty);

         return GetTileImageUsingHttp(url);
      }

      #endregion

      string MakeTileImageUrl(GPoint pos, int zoom, string language)
      {
         char letter = ServerLetters[GetServerNum(pos, 3)];
         return string.Format(UrlFormat, letter, zoom, pos.X, pos.Y);
      }

      static readonly string UrlFormat = "http://{0}.tile.openstreetmap.org/{1}/{2}/{3}.png";
   }
}

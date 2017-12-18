namespace CFMap.CoreType.Internals
{
   using System.IO;
   using System;

   /// <summary>
   /// struct for raw tile
   /// </summary>
   internal struct RawTile
   {

      public int MapId;
      public GPoint Pos;
      public int Zoom;
     

      public RawTile( int nMapId,GPoint Pos, int Zoom)
      {
         this.MapId = nMapId;
         this.Pos = Pos;
         this.Zoom = Zoom;
      }

      public override string ToString()
      {
          return MapId + " at zoom " + Zoom + ", pos: " + Pos;
      }
   }
}

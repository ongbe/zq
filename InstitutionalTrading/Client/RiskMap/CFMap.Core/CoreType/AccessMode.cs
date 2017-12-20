
namespace CFMap.CoreType
{
   /// <summary>
   /// tile access mode
   /// </summary>
   public enum AccessMode
   {
      /// <summary>
      /// access only server
      /// </summary>
      ServerOnly,

      /// <summary>
      /// access first server and caches localy
      /// </summary>
      ServerAndCache,

      /// <summary>
      /// access first cache localy and Server
      /// </summary>
      CacheAndServer,

      /// <summary>
      /// access only cache
      /// </summary>
      CacheOnly,
   }
}

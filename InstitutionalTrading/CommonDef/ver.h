#pragma once
#define  STR(s) #s

#define  VER_MAJOR 1
#define  VER_MINI  0
#define  VER_Y	   1
#define  VER_DAY   08191

#define  CLIENT_VERSION(y,d) VER_MAJOR,VER_MINI,y,d
#define  CLIENT_VERSTRING(major,mini,y,d) STR(major.##mini.##y.##d)

#define  SERVER_VERSION  VER_MAJOR,VER_MINI,VER_Y,VER_DAY
#define  SERVER_VERSTRING(major,mini,y,d) STR(major.##mini.##y.##d)

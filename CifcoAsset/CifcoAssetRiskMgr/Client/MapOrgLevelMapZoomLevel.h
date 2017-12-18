
#pragma once
#include "MapOrgnization.h"
namespace RiskMap
{
	 //转换类，组织架构最大10级，从 1到10。 地图缩放级别也是10级，从4到13。
	 //我们需要帮助他们建立一个对应关系
	
     ref class COrgLevelToMapZoomLevel
	 {
	 public:
		
		static Int32 MaxZoomLevel = 14;
		static Int32 MinZoomLevel = 4;
	 private:
		static array<int, 2>^ gArrayDeep1 = {{4,13}};
		static array<int, 2>^ gArrayDeep2 = {{4,4},{5,13}};
		static array<int, 2>^ gArrayDeep3 = {{4,4},{5,8},{9,13}};
		static array<int, 2>^ gArrayDeep4 = {{4,4},{5,8},{9,10},{11,13}};
		static array<int, 2>^ gArrayDeep5 = {{4,4},{5,5},{6,8},{9,9},{10,13}};
		static array<int, 2>^ gArrayDeep6 = {{4,4},{5,5},{6,8},{9,9},{10,10},{11,13}};
		static array<int, 2>^ gArrayDeep7 = {{4,4},{5,5},{6,8},{9,9},{10,10},{11,11},{12,13}};
		static array<int, 2>^ gArrayDeep8 = {{4,4},{5,5},{6,8},{9,9},{10,10},{11,11},{12,12},{13,13}};
		static array<int, 2>^ gArrayDeep9 = {{4,4},{5,5},{6,7},{8,8},{9,9},{10,10},{11,11},{12,12},{13,13}};
		static array<int, 2>^ gArrayDeep10 = {{4,4},{5,5},{6,6},{7,7},{8,8},{9,9},{10,10},{11,11},{12,12},{13,13}};
	 public: 
		 static bool TranslateOrgLevelToZoomLevel(Int32 nMaxSacleLevel,Int32 nScaleLevel,Int32 * nMinZoomLevel,Int32 * nMaxZoomLevel)
		 {
			 if(nMaxSacleLevel > COrgnization::MaxDeep || nMaxSacleLevel < COrgnization::MinDeep)
				 return false;

			 if(nScaleLevel > nMaxSacleLevel || nScaleLevel < COrgnization::MinDeep)
				 return false;

			 if(nMaxZoomLevel == NULL || nMaxZoomLevel == NULL)
				 return false;

			 array<int, 2>^ larrTemp = nullptr;

			 switch(nMaxSacleLevel)
			 {
			 case 1:
				 larrTemp = gArrayDeep1;break;
			 case 2:
				 larrTemp = gArrayDeep2;break;
			 case 3:
				 larrTemp = gArrayDeep3;break;
			 case 4:
				 larrTemp = gArrayDeep4;break;
			 case 5:
				 larrTemp = gArrayDeep5;break;
			 case 6:
				 larrTemp = gArrayDeep6;break;
			 case 7:
				 larrTemp = gArrayDeep7;break;
			 case 8:
				 larrTemp = gArrayDeep8;break;
			 case 9:
				 larrTemp = gArrayDeep9;break;
			 case 10:
				 larrTemp = gArrayDeep10;break;	
			 default:break;
			 }

			 if(larrTemp == nullptr)
				 return false;
			 
			 *nMinZoomLevel = larrTemp[nScaleLevel-1,0];
			 *nMaxZoomLevel = larrTemp[nScaleLevel-1,1];
			 return true;
		 }

		 static Int32 IsChangedShowOrg(Int32 nMaxSacleLevel,Int32 nOldZoom,Int32 nNewZoom)
		 {
			 if(nMaxSacleLevel > COrgnization::MaxDeep || nMaxSacleLevel < COrgnization::MinDeep)
				 return 0;

			 if(nOldZoom > MaxZoomLevel || nOldZoom < MinZoomLevel)
				 return 0;

			 if(nNewZoom > MaxZoomLevel || nNewZoom < MinZoomLevel)
				 return 0;

			 array<int, 2>^ larrTemp = nullptr;

			 switch(nMaxSacleLevel)
			 {
			 case 1:
				 larrTemp = gArrayDeep1;break;
			 case 2:
				 larrTemp = gArrayDeep2;break;
			 case 3:
				 larrTemp = gArrayDeep3;break;
			 case 4:
				 larrTemp = gArrayDeep4;break;
			 case 5:
				 larrTemp = gArrayDeep5;break;
			 case 6:
				 larrTemp = gArrayDeep6;break;
			 case 7:
				 larrTemp = gArrayDeep7;break;
			 case 8:
				 larrTemp = gArrayDeep8;break;
			 case 9:
				 larrTemp = gArrayDeep9;break;
			 case 10:
				 larrTemp = gArrayDeep10;break;	
			 default:break;
			 }

			 if(larrTemp == nullptr)
				 return 0;

			 for(Int32 i = 0 ; i < larrTemp->Length/2 ;i++)
			 {
				 // check old zoom 
                  if(larrTemp[i,0] <= nOldZoom && nOldZoom <= larrTemp[i,1])
				  {
					  //check new room also in this level
					  if(larrTemp[i,0] <= nNewZoom && nNewZoom <= larrTemp[i,1])
					  {
						  return 0;
					  }
					  else
						  return nNewZoom > nOldZoom ? i+2 : i;
					  
				  }
			 }			
			 return 0;
		 }
	 };

}
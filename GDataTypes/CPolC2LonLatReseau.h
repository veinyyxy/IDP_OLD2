///////////////////////////////////////////////////////
//CPolC2LonLatReseau.h by chenxiang 
//created by chenxiang 20080417
//for极坐标转经纬度
//极坐标转经度差纬度差,极坐标转经度纬度,已知经纬度求地球距离
///////////////////////////////////////////////////////

#pragma once

#ifndef		CPOLC2LONLATRESEAU_H
#define		CPOLC2LONLATRESEAU_H

#include	"GDef.h"
class DLLEXP_GSYS CPolC2LonLatReseau
{
public:
	static bool PolC2DLonDLat(gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& DLon,gInt32& DLat,gInt32 H=0);//极坐标转经度差,纬度差
	//参数说明:极坐标角度(方位角,单位0.01度),极坐标半径(单位米),雷达中心经度,纬度,经度差,纬度差(单位都是0.00001度),雷达高度H(单位米)
	static bool PolCLonLat(gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H=0);//极坐标转经度纬度
	//参数说明:极坐标角度(方位角,单位0.01度),极坐标半径(单位米),雷达中心经度,纬度,经度差,纬度差(单位都是0.00001度),雷达高度H(单位米)
	//add by chenxaing 20080506~~~~~~~~~~~~~~~~~~~~~~~
	static bool SameLon2Distance(gInt32 DLat,gInt32 &DistanceY);								//同经度上不同纬度在地球上的实际距离
	//参数说明:纬度差(0.00001度),纵向距离引用(米)
	static bool SameLat2Distance(gInt32 DLon,gInt32 Lat,gInt32 &DistanceX);						//同纬度不同经度在地球上的实际距离
	//参数说明:经度差(0.00001度),纬度(0.00001度)(!注意这里不是 纬度差),横向距离引用(米)	
	static bool EqualDistPolC2DLonDLat(gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& DLon,gInt32& DLat,gInt32 H=0);//等距极坐标转经纬度差
	static bool EqualDistPolCLonLat(gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H=0);//等距极坐标转经纬度
	//end of add~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//add by chenxiang 20080518~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static bool QuickPolC2DLonDLat( gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& DLon,gInt32& DLat,gInt32 H );//高效的极坐标转经度差纬度差适用于等距离投影
	static bool QuickPolC2LonLat( gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H ); //高效的极坐标转经度纬度适用于等距离投影
	//add by chenxiang 20080606~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static bool QuickPolC2LonLat( gInt32 Elevation,gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H ); //地心投影 极坐标转经纬度
	//参数说明:仰角(0.01度),极坐标角度(方位角,单位0.01度),极坐标半径 (单位米) ,雷达中心经度,纬度,目标点经度,纬度 (单位都是0.00001度) ,雷达高度H (单位米)
	//end of add~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//add by chenxaing 20080506~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	static gInt32 L1ToL2(gInt32 L1);//用于等距投影用的长度转换函数 小变大 转换后的L2再代入极坐标转经纬度函数出来的就是等距投影后的经纬度
//add by chenxiang 20080606~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static gInt32 LBToLC(gInt32 LB,gInt32 Elevation,gInt32 H);//用于地心投影的长度转换,将cos后距离转成弧长
//end of add~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};

#endif

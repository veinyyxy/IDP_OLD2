///////////////////////////////////////////////////////
//CPolC2LonLatReseau.cpp by chenxiang 
//created by chenxiang 20080417
//for极坐标转经纬度
//极坐标转经度差纬度差,极坐标转经度纬度,已知经纬度求地球距离
///////////////////////////////////////////////////////

#include "CPolC2LonLatReseau.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif//_USE_MATH_DEFINES
#include	<math.h>

#define REarth	6371300													//单位米
#define e5		100000													//经纬度的单位换算系数

bool CPolC2LonLatReseau::PolC2DLonDLat( gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& DLon,gInt32& DLat,gInt32 H ) //极坐标转经度差,纬度差
//参数说明:极坐标角度(方位角,单位0.01度),极坐标半径(单位米),雷达中心经度,纬度,经度差,纬度差(单位都是0.00001度) ,雷达高度H(单位米) 默认参数默认为0
{
	//极坐标转经纬网格,适用于南北纬85度以内因为高纬度经线密集容易产生误差
	if( Lon0 < -180 * e5 || Lon0 > 180 * e5 )							//雷达中心经度限制 0度东\西经180度
	{
		return false;
	}
	if( Lat0 < -85 * e5 || Lat0 > 85 * e5 )								//雷达中心纬度限制 0度到南\北纬85度
	{
		return false;
	}
	if( A < 0 || A >= 360 * 100 )										//极坐标角度范围限制 0度到360度
	{
		return false;
	}
	A = 45000 - A;														//add by chenxiang for 更改极坐标角度和方位角方向和起始位置不统一而做的角度转变 20080424
	if( L < 0 || L >= 700000 )											//极坐标半径范围限制 0m到700公里
	{
		return false;
	}
	gInt32 K = 0;														//用来判断极坐标角度sin或cos的正负//四舍五入用的系数 防止负数四舍五入错误
	gInt32 sinA = gInt32( sin ( gDouble( A ) / 100 * M_PI / 180 ) * 1000000000 ) ;									//乘倍是为了零值比较用
	( ( sinA > 1 ) || ( sinA < -1 ) ) ? K = sinA / gInt32( fabs( gDouble( sinA ) ) ) : K = 0;						//?前面那个是sinA的零值比较

	DLat = gInt32( e5*atan( L * sin( gDouble( A ) / 100 * M_PI / 180 ) / ( REarth + H ) ) * 180 / M_PI + K * 0.5 );	//计算纬度差
	gInt32 cosA = gInt32( cos( gDouble( A ) / 100 * M_PI / 180 ) * 1000000000 );									//乘1000000000是为了变为整型后零值比较用
	( ( cosA > 1 ) || ( cosA < -1 ) ) ? K = cosA / gInt32( fabs( gDouble( cosA ) ) ) : K = 0;						//?前面那个是cosA的零值比较

	DLon = gInt32( e5 * ( 180 / M_PI ) * 2 * asin( 2 * sin( atan( gDouble( L ) / ( REarth + H ) ) /2 ) * cos( gDouble( A ) / 100 * M_PI / 180 ) 
		/ ( cos( gDouble( Lat0 ) * M_PI / 180 / e5 ) + cos( ( gDouble( Lat0 ) + DLat ) * M_PI / 180 / e5 ) ) ) + K * 0.5 ) ;//计算经度差
	if( ( Lat0 + DLat ) < -89 * e5 || ( Lat0 + DLat ) > 89 * e5 )		//如果目标纬度超出范围则返回false
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool CPolC2LonLatReseau::PolCLonLat( gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H ) //极坐标转经度纬度
//参数说明:极坐标角度(方位角,单位0.01度),极坐标半径 (单位米) ,雷达中心经度,纬度,目标点经度,纬度 (单位都是0.00001度) ,雷达高度H (单位米) 默认参数默认为0
{
	gInt32 DLon = 0,DLat = 0;//初始化经度差和纬度差
	if ( !PolC2DLonDLat( A,L,Lon0,Lat0,DLon,DLat,H ) )					//检查参数的合法性和 获得经度差纬度差
	{
		return false;
	}
	Lon1 = ( Lon0+DLon ) ;
	if( Lon1 > 180 * e5 )												//大于180度
	{
		Lon1 -= 360 * e5;												//东经越界 转成西经
	}
	if( Lon1 < -180 * e5 )												//小于-180度
	{
		Lon1 += 360 * e5;												//西经越界 转成东经
	}
	Lat1 = Lat0 + DLat;
	return true;
}
//add by chenxaing 20080506~~~~~~~~~~~~~~~~~~~~~~~
bool CPolC2LonLatReseau::SameLon2Distance( gInt32 DLat,gInt32 &DistanceY )//同经度上不同纬度在地球上的实际距离
{//参数说明:纬度差(0.00001度),纵向距离引用(米)
	if( DLat < -85 * e5 || DLat > 85 * e5 )								//雷达中心纬度差限制 0度到南\北纬85度
	{
		return false;
	}
	DistanceY = gInt32 ( REarth * DLat * M_PI / 180 / e5 +0.5);
	return true;
}
bool CPolC2LonLatReseau::SameLat2Distance( gInt32 DLon,gInt32 Lat,gInt32 &DistanceX )//同纬度不同经度在地球上的实际距离
{//参数说明:经度差(0.00001度),纬度(0.00001度)(!注意这里不是 纬度差),横向距离引用(米)
	if( DLon < -180 * e5 || DLon > 180 * e5 )							//雷达中心经度差限制 0度到东西经180度
	{
		return false;
	}
	DistanceX = gInt32 ( REarth * cos( gDouble( Lat * M_PI / 180 /e5 ) ) * DLon * M_PI / 180 / e5 +0.5);
	return true;
}
gInt32 CPolC2LonLatReseau::L1ToL2( gInt32 L1 )//用于等距投影用的长度转换函数 小变大 转换后的L2再代入极坐标转经纬度函数出来的就是等距投影后的经纬度
{
	gInt32 L2=gInt32( REarth * tan( gDouble( L1 ) / REarth  ) );
	return L2;															//转换后的距离
}
bool CPolC2LonLatReseau::EqualDistPolC2DLonDLat(gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& DLon,gInt32& DLat,gInt32 H)//等距投影极坐标转经纬度差
{//参数说明:极坐标角度(方位角,单位0.01度),极坐标半径(单位米),雷达中心经度,纬度,经度差,纬度差(单位都是0.00001度) ,雷达高度H(单位米) 默认参数默认为0
	return PolC2DLonDLat( A, L1ToL2(L), Lon0, Lat0, DLon, DLat, H );
}
bool CPolC2LonLatReseau::EqualDistPolCLonLat(gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H)//等距投影极坐标转经纬度
{//参数说明:极坐标角度(方位角,单位0.01度) ,极坐标半径 (单位米) ,雷达中心经度,纬度,目标点经度,纬度 (单位都是0.00001度) ,雷达高度H (单位米) 默认参数默认为0
	return PolCLonLat( A, L1ToL2(L), Lon0, Lat0, Lon1, Lat1, H );
}
// end of add~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//add by chenxiang 20080518~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//为提高效率而修改的极坐标转经纬度函数 //适用于等距离投影 球面投影
bool CPolC2LonLatReseau::QuickPolC2DLonDLat( gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& DLon,gInt32& DLat,gInt32 H ) //极坐标转经度纬度
//参数说明:极坐标角度(方位角,单位0.01度),极坐标半径 (单位米) ,雷达中心经度,纬度,目标点经度,纬度 (单位都是0.00001度) ,雷达高度H (单位米)
{
	//极坐标转经纬网格,适用于南北纬85度以内因为高纬度经线密集容易产生误差
	if( Lon0 < -180 * e5 || Lon0 > 180 * e5 || Lat0 < -85 * e5 || Lat0 > 85 * e5)
	{																	//雷达中心经度限制 0度东\西经180度//雷达中心纬度限制 0度到南\北纬85度
		return false;
	}
	if( A < 0 || A >= 360 * 100 )										//极坐标角度范围限制 0度到360度
	{
		return false;
	}
	gInt32 A2 = 45000 - A;												//更改极坐标角度和方位角方向和起始位置不统一而做的角度转变
	if( L < 0 || L >= 700000 )											//极坐标半径范围限制 0m到700公里
	{
		return false;
	}
	gDouble Angle2Radian = M_PI / 180;									//角度转弧度 要乘以这个系数
	gInt8 K = 1;														//四舍五入系数
	( A >= 0 && A <= 18000 ) ? K=1 : K=-1;//sin>0
	DLat = gInt32( e5*( L * sin( gDouble( A2 ) / 100 * Angle2Radian ) / ( REarth + H ) ) / Angle2Radian + K * 0.5 );//计算纬度差
	( ( A >= 0 && A <= 9000 ) || ( A >= 27000 && A < 36000 ) ) ? K=1 : K=-1;//cos>0
	DLon = gInt32(e5 / Angle2Radian * 2 * asin( 2 * sin( gDouble( L ) / ( REarth + H ) / 2 ) * cos( gDouble( A2 ) / 100 * Angle2Radian )
		/ ( cos( gDouble( Lat0 ) * Angle2Radian / e5 ) + cos( ( gDouble( Lat0 ) + DLat ) * Angle2Radian / e5 ) ) ) + K * 0.5);//计算经度差

	if( ( Lat0 + DLat ) < -89 * e5 || ( Lat0 + DLat ) > 89 * e5 )		//如果目标纬度超出范围则返回false
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool CPolC2LonLatReseau::QuickPolC2LonLat( gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H ) //高效的极坐标转经度纬度适用于等距离投影
//参数说明:极坐标角度(方位角,单位0.01度),极坐标半径 (单位米) ,雷达中心经度,纬度,目标点经度,纬度 (单位都是0.00001度) ,雷达高度H (单位米) 
{
	gInt32 DLon = 0,DLat = 0;											//初始化经度差和纬度差
	if ( !QuickPolC2DLonDLat( A,L,Lon0,Lat0,DLon,DLat,H ) )				//检查参数的合法性和 获得经度差纬度差
	{
		return false;
	}
	Lon1 = ( Lon0+DLon ) ;
	if( Lon1 > 180 * e5 )												//大于180度
	{
		Lon1 -= 360 * e5;												//东经越界 转成西经
	}
	if( Lon1 < -180 * e5 )												//小于-180度
	{
		Lon1 += 360 * e5;												//西经越界 转成东经
	}
	Lat1 = Lat0 + DLat;
	return true;
}
// end of add~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//add by chenxiang 20080606~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gInt32 CPolC2LonLatReseau::LBToLC(gInt32 LB,gInt32 Elevation,gInt32 H)////用于地心投影的长度转换,将cos后距离转成弧长
{//参数说明 :转换前的距离(cos扫描距离)(米),仰角(0.01度),雷达高度(米)
	//gInt32 LC = gInt32( REarth * tan( gDouble( L1 ) / REarth  ) );
	gFloat E =Elevation * gFloat(M_PI / 18000.0f);
	gInt32 LC = gInt32( (REarth+H) * atan( LB * cos( E ) / ( LB * sin( E ) + ( REarth + H ) * cos( E ) ) ) +0.5);
	return LC;															//转换后的距离
}
bool CPolC2LonLatReseau::QuickPolC2LonLat( gInt32 Elevation,gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H ) //地心投影 极坐标转经纬度
{//参数说明:仰角(0.01度),极坐标角度(方位角,单位0.01度),极坐标半径 (单位米) ,雷达中心经度,纬度,目标点经度,纬度 (单位都是0.00001度) ,雷达高度H (单位米)
	if (Elevation < 0 || Elevation >= 9000)
	{
		return false;
	}
	return QuickPolC2LonLat( A, LBToLC( L,Elevation,H), Lon0, Lat0, Lon1, Lat1, H );
}
//end of add~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

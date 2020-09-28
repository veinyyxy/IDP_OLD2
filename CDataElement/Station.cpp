#include "Station.h"


//////////////////////////////////// CStation 
/************************************************************************/
/* 函数功能： 构造函数  						                        */
/* 参数说明:  NULL														*/
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 ？ 月 ？ 日										*/
/************************************************************************/
CStation::CStation()
{

}

CStation::CStation(const CStation& station)
{
	c0                = station.c0;
	by_status         = station.by_status;
	ch_area_city      = station.ch_area_city;
	ch_area_county    = station.ch_area_county;
	ch_area_province  = station.ch_area_province;
	ch_business_type  = station.ch_business_type;
	ch_end_month      = station.ch_end_month;
	ch_end_year       = station.ch_end_year;
	ch_name           = station .ch_name;
	ch_obs_type       = station.ch_obs_type;
	ch_start_month    = station.ch_start_month;
	ch_start_year     = station .ch_start_year;
	v01000            = station.v01000;
	v02001            = station.v02001;
	v05001            = station.v05001;
	v06001            = station.v06001;
	v07001            = station.v07001;
	v07033            = station.v07033;
	rowid             = station.rowid;
}
/************************************************************************/
/* 函数功能： 重载运算符  						                        */
/* 参数说明:  NULL														*/
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 ？ 月 ？ 日										*/
/************************************************************************/
CStation& CStation::operator =(const CStation& station)
{
	if(&station != this )
	{
		c0                = station.c0;
		by_status         = station.by_status;
		ch_area_city      = station.ch_area_city;
		ch_area_county    = station.ch_area_county;
		ch_area_province  = station.ch_area_province;
		ch_business_type  = station.ch_business_type;
		ch_end_month      = station.ch_end_month;
		ch_end_year       = station.ch_end_year;
		ch_name           = station .ch_name;
		ch_obs_type       = station.ch_obs_type;
		ch_start_month    = station.ch_start_month;
		ch_start_year     = station .ch_start_year;
		v01000            = station.v01000;
		v02001            = station.v02001;
		v05001            = station.v05001;
		v06001            = station.v06001;
		v07001            = station.v07001;
		v07033            = station.v07033;
		rowid             = station.rowid;
	}

	return *this;		
};
//////////////////////////////////// end CStation 
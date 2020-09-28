#include "ProductInfo.h"

proAttr::proAttr():
	//site_code(255, 'R')
	//,staName(255, 'R')
	NumberOfRadars(0)
	,pro_code(0)
	//,pro_name(255, 'R')
	,scan_date(0)
	,scan_time(0)
	,generation_date(0)
	,generation_time(0)
	,altitude(0)
	,longitude(0)
	,longitude2(0)
	,latitude(0)
	,latitude2(0)
	,operation_mode(0)
	,vcp_mode(0)
	,elevation(0.0)
	,max_value(0.0)
	,width(0.0)
	,HeightOfElevation(0)
	,numberoflayers(0)
	,param1(50)
	,param2(0)
	,height_evp_vwp(0)
	,Uh_evp_vwp(0)
	,Du_evp_vwp(0)
	,ResolutionOfRow(0)
	,ResolutionOfCol(0)
	,max_value2(0.0)
	,max_value3(0.0)
{
	memset(site_code, sizeof site_code, 0);
	memset(staName, sizeof staName, 0);
	memset(pro_name, sizeof pro_name, 0);
	}

proAttr::~proAttr()
{

}

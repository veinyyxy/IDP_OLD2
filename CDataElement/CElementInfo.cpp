/*================================================================================== 
项目名： 人机交互平台
作 者： 杨潇
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：CElementInfo.h CElementInfo.cpp
类 名： CElementInfo
父 类： NULL
子 类： 无 
功能说明： CIPAS系统数据需求规格说明书 3.2.13.1 NCEP再分析资料
调用说明： 

-------------------------------------版本更新-----------------------------------------
V 1.2 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：杨潇
完成日期：2011年05月09日
=====================================================================================*/
#include "CElementInfo.h"
//////////////////CElementInfo//////////////////////

/************************************************************************/
/* 函数功能： 默认构造函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  杨潇													    */
/* 日    期:  2011 年 05 月 09 日										*/
/************************************************************************/
CElementInfo::CElementInfo()
{
	m_offset = 0;
	m_rule = 0;
}

/************************************************************************/
/* 函数功能： 构造函数	      					                        */
/* 参数说明:  NULL														*/
/* 作    者:  杨潇													    */
/* 日    期:  2011 年 05 月 09 日										*/
/************************************************************************/
CElementInfo::CElementInfo(QString time,QString type,QString elefile,QString en_name,QString ch_name,QString units,float offset,float rule,QString level,QString hr,QString vr,QString ranges,QString defaults,QString full_elefile)
{
	m_time = time;
	m_en_name = en_name;
	m_type = type;
	m_elefile = elefile;
	m_ch_name = ch_name;
	m_level = level;
	m_units = units;
	m_offset = offset;
	m_vertical_resolution = vr;
	m_horizontal_resolution = hr;
	m_ranges = ranges;
	m_default = defaults;
	m_rule = rule;
	m_full_elefile = full_elefile;
}

/************************************************************************/
/* 函数功能： 构造函数	      					                        */
/* 参数说明:  NULL														*/
/* 作    者:  杨潇													    */
/* 日    期:  2011 年 05 月 09 日										*/
/************************************************************************/
CElementInfo::CElementInfo(const CElementInfo& ts)
{
	m_units = ts.m_units;
	m_offset = ts.m_offset;
	m_vertical_resolution = ts.m_vertical_resolution;
	m_horizontal_resolution = ts.m_horizontal_resolution;
	m_ranges = ts.m_ranges;
	m_default = ts.m_default;
	m_rule = ts.m_rule;
	m_type = ts.m_type;
	m_elefile = ts.m_elefile;
	m_ch_name = ts.m_ch_name;
	m_level = ts.m_level;
	m_time = ts.m_time;
	m_en_name = ts.m_en_name;
	m_full_elefile = ts.m_full_elefile;
}

/************************************************************************/
/* 函数功能： 重载运算符构造函数	      			                    */
/* 参数说明:  NULL														*/
/* 作    者:  杨潇													    */
/* 日    期:  2011 年 05 月 09 日										*/
/************************************************************************/
CElementInfo& CElementInfo::operator = (const CElementInfo &ts)
{
	m_units = ts.m_units;
	m_offset = ts.m_offset;
	m_vertical_resolution = ts.m_vertical_resolution;
	m_horizontal_resolution = ts.m_horizontal_resolution;
	m_ranges = ts.m_ranges;
	m_default = ts.m_default;
	m_rule = ts.m_rule;
	m_type = ts.m_type;
	m_elefile = ts.m_elefile;
	m_ch_name = ts.m_ch_name;
	m_level = ts.m_level;
	m_time = ts.m_time;
	m_en_name = ts.m_en_name;
    m_full_elefile = ts.m_full_elefile;

	return *this;
}

/************************************************************************/
/* 函数功能： 获得要素中文名     	      			                    */
/* 参数说明:  NULL														*/
/* 返 回 值： m_ch_name   要素中文名                                    */
/* 作    者:  杨潇													    */
/* 日    期:  2011 年 05 月 09 日										*/
/************************************************************************/
QString CElementInfo::GetChineseName()
{
	return m_ch_name;
}
 //////////////////////////CElementFactory////////////////////////

CElementFactory* CElementFactory::pFactory = NULL;

/************************************************************************/
/* 函数功能： 创建CElementFactory  	      			                    */
/* 参数说明:  NULL														*/
/* 返 回 值： pFactory                                                  */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 05 月 09 日										*/
/************************************************************************/
CElementFactory* CElementFactory::GetInstance()
{
	if(!pFactory)
	{
		pFactory = new CElementFactory();
	}

	return pFactory;
}

/************************************************************************/
/* 函数功能： 默认构造函数						                        */
/* 参数说明:  NULL														*/
/* 作    者:  杨潇													    */
/* 日    期:  2011 年 05 月 09 日										*/
/************************************************************************/
CElementFactory::CElementFactory()
{
	InitAllElements();
}

/************************************************************************/
/* 函数功能： 初始化             	      			                    */
/* 参数说明:  NULL														*/
/* 返 回 值： 空                                                        */
/* 作    者:  杨潇													    */
/* 日    期:  2011 年 05 月 09 日										*/
/************************************************************************/
void CElementFactory::InitAllElements()
{
    //插入顺序为  时次 种类 要素文件 要素英文名 要素中文名 单位 偏移量 尺度因子 层次 水平分辨率 垂直分辨率 范围 缺省值 完整要素文件名
	////////////////////////////////逐月/////////////////////////////////////
	m_elements.clear();
 	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "csulftop"),new CElementInfo(QObject::tr("逐月"),"other_gaus","csulftop","Clear Sky Upward Longwave Flux",QObject::tr("晴空向上长波通量"),"W//m^2",0.00,1.00,QObject::tr("名义上的大气层顶"),"192×94","1","0-358.125,-88.542-88.542","-9.96921E+36","csulftop.mon.mean.nc"));
 	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "csusftop"),new CElementInfo(QObject::tr("逐月"),"other_gaus","csusftop","Clear Sky Upward Shortwave Flux",QObject::tr("晴空向上短波通量"),"W//m^2",0.00,1.00,QObject::tr("名义上的大气层顶"),"192×94","1","0-358.125,-88.542-88.542","-9.96921E+36","csusftop.mon.mean.nc"));
 	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "dswrftop"),new CElementInfo(QObject::tr("逐月"),"other_gaus","dswrftop","Downward Solar Radiation Flux",QObject::tr("向下太阳辐射通量"),"W//m^2",0.00,1.00,QObject::tr("名义上的大气层顶"),"192×94","1","0-358.125,-88.542-88.542","-9.96921E+36","dswrftop.mon.mean.nc"));
 	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "ulwrftop"),new CElementInfo(QObject::tr("逐月"),"other_gaus","ulwrftop","Upward Longwave Radiation Flux",QObject::tr("向上长波辐射通量"),"W//m^2",0.00,1.00,QObject::tr("名义上的大气层顶"),"192×94","1","0-358.125,-88.542-88.542","-9.96921E+36","ulwrftop.mon.mean.nc"));
 	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "uswrftop"),new CElementInfo(QObject::tr("逐月"),"other_gaus","uswrftop","Upward Solar Radiation Flux",QObject::tr("向上太阳辐射通量"),"W//m^2",0.00,1.00,QObject::tr("名义上的大气层顶"),"192×94","1","0-358.125,-88.542-88.542","-9.96921E+36","dswrftop.mon.mean.nc"));
 	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "pv"),new CElementInfo(QObject::tr("逐月"),"other_gaus","pv","Upward Solar Radiation Flux","位涡","pvu (=10^(-6)*m^2*s^(-1)*kg^(-1)*K)",0.00,1.00,"345 degK","192×94","1","0-358.125,-88.542-88.542","-9.96921E+36","pv.mon.mean.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "air"),new CElementInfo(QObject::tr("逐月"),"pressure","air","Air temperature",QObject::tr("气温"),"degC",127.65,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144×73","17","0-357.5, -90-90","-9.96921E+36","air.mon.mean.nc"));
 	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "hgt"),new CElementInfo(QObject::tr("逐月"),"pressure","hgt","geopotential height",QObject::tr("位势高度"),"m",0.00,1.00,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144×73","17","0-357.5, -90-90","-9.96921E+36","hgt.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "pottmp"),new CElementInfo(QObject::tr("逐月"),"pressure","pottmp","Potential Temperature",QObject::tr("位温"),"degK",0.00,1.00,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144×73","17","0-357.5, -90-90","-9.96921E+36","pottmp.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "uwnd"),new CElementInfo(QObject::tr("逐月"),"pressure","uwnd","u wind",QObject::tr("U风场"),"m//s",202.65,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144×73","17","0-357.5, -90-90","-9.96921E+36","uwnd.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "vwnd"),new CElementInfo(QObject::tr("逐月"),"pressure","vwnd","v wind",QObject::tr("V风场"),"m//s",202.65,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144×73","17","0-357.5, -90-90","-9.96921E+36","vwnd.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "wspd"),new CElementInfo(QObject::tr("逐月"),"pressure","wspd","wind speed",QObject::tr("风速"),"m/s",202.65,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144×73","17","0-357.5, -90-90","-9.96921E+36","wspd.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "omega"),new CElementInfo(QObject::tr("逐月"),"pressure","omega","Omega",QObject::tr("垂直速度"),"Pascal//s",29.765,0.001,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144×73","前12层","0-357.5, -90-90","-9.96921E+36","omega.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "rhum"),new CElementInfo(QObject::tr("逐月"),"pressure","rhum","Relative Humidity",QObject::tr("相对湿度"),"%",302.65,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144×73","前8层","0-357.5, -90-90","-9.96921E+36","rhum.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "shum"),new CElementInfo(QObject::tr("逐月"),"pressure","shum","Specific Humidity",QObject::tr("比湿"),"grams//kg",32.665,0.001,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144×73","前8层","0-357.5, -90-90","-9.96921E+36","shum.mon.mean.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "chi"),new CElementInfo(QObject::tr("逐月"),"sigma","chi","Velocity Potential",QObject::tr("速度势"),"m*m//s",0.00,1.00,"sigma＝0.995, 0.8458, 0.2582, 0.2101, 0.1682","192×94","5","0-358.125,-88.542f -88.542","-9.96921E+36","chi.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "psi"),new CElementInfo(QObject::tr("逐月"),"sigma","psi","Streamfunction",QObject::tr("流函数"),"m*m//s",0.00,1.00,"sigma＝0.995, 0.8458, 0.2582, 0.2101, 0.1682","192×94","5","0-358.125,-88.542f -88.542","-9.96921E+36","psi.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "div"),new CElementInfo(QObject::tr("逐月"),"sigma","div","Divergence",QObject::tr("散度"),"1.//s",0.00,1.00,"sigma＝0.995, 0.2101","192×94","2","0-358.125,-88.542f -88.542","-9.96921E+36","div.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "vor"),new CElementInfo(QObject::tr("逐月"),"sigma","vor","Vorticity",QObject::tr("涡度"),"1.//s",0.00,1.00,"sigma＝0.995, 0.2101","192×94","2","0-358.125,-88.542f -88.542","-9.96921E+36","vor.mon.mean.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "chi"),new CElementInfo(QObject::tr("逐月"),"spectral","chi","Velocity Potential",QObject::tr("速度势"),"m*m//s",0.00,1.00,"sigma＝0.995, 0.8458, 0.2582, 0.2101, 0.1682","192×94","5","0-358.125,-88.542f -88.542","-9.96921E+36","chi.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "psi"),new CElementInfo(QObject::tr("逐月"),"spectral","psi","Streamfunction",QObject::tr("流函数"),"m*m//s",0.00,1.00,"sigma＝0.995, 0.8458, 0.2582, 0.2101, 0.1682","192×94","5","0-358.125,-88.542f -88.542","-9.96921E+36","psi.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "div"),new CElementInfo(QObject::tr("逐月"),"spectral","div","Divergence",QObject::tr("散度"),"1.//s",0.00,1.00,"sigma＝0.995, 0.2101","192×94","2","0-358.125,-88.542f -88.542","-9.96921E+36","div.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "vor"),new CElementInfo(QObject::tr("逐月"),"spectral","vor","Vorticity",QObject::tr("涡度"),"1.//s",0.00,1.00,"sigma＝0.995, 0.2101","192×94","2","0-358.125,-88.542f -88.542","-9.96921E+36","vor.mon.mean.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "air"),new CElementInfo(QObject::tr("逐月"),"surface","air","Air Temperature",QObject::tr("表面气温"),"degC",0.00,1.00,QObject::tr("表面"),"144×73","1","0-357.5, -90-90","-9.96921E+36","air.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "pres"),new CElementInfo(QObject::tr("逐月"),"surface","pres","Surface Pressure",QObject::tr("表面气压"),"hPa",0.00,1.00,QObject::tr("表面"),"144×73","1","0-357.5, -90-90","-9.96921E+36","pres.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "pr_wtr"),new CElementInfo(QObject::tr("逐月"),"surface","pr_wtr","Precipitable Water Content",QObject::tr("可降水"),"kg//m^2",0.00,1.00,QObject::tr("表面"),"144×73","1","0-357.5, -90-90","-9.96921E+36","pr_wtr.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "rhum"),new CElementInfo(QObject::tr("逐月"),"surface","rhum","Relative Humidity",QObject::tr("相对湿度"),"%",0.00,1.00,QObject::tr("表面"),"144×73","1","0-357.5, -90-90","-9.96921E+36","rhum.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "slp"),new CElementInfo(QObject::tr("逐月"),"surface","slp","Sea Level Pressure",QObject::tr("海平面气压"),"hPa",0.00,1.00,QObject::tr("海平面"),"144×73","1","0-357.5, -90-90","-9.96921E+36","slp.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "thickness"),new CElementInfo(QObject::tr("逐月"),"surface","thickness","Thickness of 1000-500mb",QObject::tr("1000-500mb大气厚度"),"m",0.00,1.00,"1000-500mb","144×73","1","0-357.5, -90-90","-9.96921E+36","thickness.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "thickness_1000500"),new CElementInfo(QObject::tr("逐月"),"surface","thickness_1000500","Thickness of 1000-500mb",QObject::tr("1000-500mb大气厚度"),"m",0.00,1.00,"1000-500mb","144×73","1","0-357.5, -90-90","-9.96921E+36","thickness_1000500.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "thickness_500200"),new CElementInfo(QObject::tr("逐月"),"surface","thickness_500200","Thickness of 500-200mb",QObject::tr("500-200mb大气厚度"),"m",0.00,1.00,"500-200mb","144×73","1","0-357.5, -90-90","-9.96921E+36","thickness_500200.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "thickness_850500"),new CElementInfo(QObject::tr("逐月"),"surface","thickness_850500","Thickness of 850-500mb",QObject::tr("850-500mb大气厚度"),"m",0.00,1.00,"850-500mb","144×73","1","0-357.5, -90-90","-9.96921E+36","thickness_850500.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "uwnd"),new CElementInfo(QObject::tr("逐月"),"surface","uwnd","surface zonal wind",QObject::tr("表面纬向风"),"m//s",0.00,1.00,QObject::tr("表面"),"144×73","1","0-357.5, -90-90","-9.96921E+36","uwnd.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "vwnd"),new CElementInfo(QObject::tr("逐月"),"surface","vwnd","surface meridional wind",QObject::tr("表面经向风"),"m//s",0.00,1.00,QObject::tr("表面"),"144×73","1","0-357.5, -90-90","-9.96921E+36","vwnd.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "wspd"),new CElementInfo(QObject::tr("逐月"),"surface","wspd","surface wind speed",QObject::tr("表面风速"),"m//s",202.65,0.01,QObject::tr("表面"),"144×73","1","0-357.5, -90-90","-9.96921E+36","wspd.mon.mean.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "air2m"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","air2m","Air Temperature",QObject::tr("2m气温"),"degK",0.00,1.00,"2m","192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","air2m.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "shum2m"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","shum2m","Specific Humidity",QObject::tr("2m比湿"),"grams//kg",0.00,1.00,"2m","192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","shum2m.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "uwnd10m"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","uwnd10m","U-Wind at 10 m",QObject::tr("10m U风场"),"m//s",0.00,1.00,"10m","192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","uwnd10m.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "vwnd10m"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","vwnd10m","V wind at 10 m",QObject::tr("10m V风场"),"m//s",0.00,1.00,"10m","192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","vwnd10m.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "cfnlf"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","cfnlf","Cloud Forcing Net Longwave Flux",QObject::tr("云强迫净长波通量"),"W//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","cfnlf.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "csdlf"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","csdlf","Clear Sky Downward Longwave Flux",QObject::tr("晴空向下长波通量"),"W//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","csdlf.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "csdsf"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","csdsf","Clear Sky Downward Solar Flux",QObject::tr("晴空向下太阳通量"),"W//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","csdsf.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "csusf"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","csusf","Clear Sky Upward Solar Flux",QObject::tr("晴空向上太阳通量"),"W//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","csusf.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "nbdsf"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","nbdsf","Near IR Beam Downward Solar Flux",QObject::tr("近红外光向下太阳通量"),"W//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","nbdsf.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "nlwrs"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","nlwrs","Net Longwave Radiation Flux",QObject::tr("净长波辐射通量"),"W//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","nlwrs.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "nswrs"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","nswrs","Net Shortwave Radiation Flux",QObject::tr("净短波辐射通量"),"W//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","nswrs.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "ulwrf"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","ulwrf","Upward Longwave Radiation Flux",QObject::tr("向上长波辐射通量"),"W//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","ulwrf.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "pevpr"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","pevpr","Potential Evaporation Rate",QObject::tr("潜蒸发率"),"W//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","pevpr.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "gflux"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","gflux","Ground Heat Flux",QObject::tr("地热通量"),"W//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","gflux.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "lhtfl"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","lhtfl","Latent Heat Net Flux",QObject::tr("潜热净通量"),"W//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","lhtfl.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "shtfl"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","shtfl","Sensible Heat Net Flux",QObject::tr("感热净通量"),"W//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","shtfl.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "ugwd"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","ugwd","Zonal Gravity Wave Stress",QObject::tr("纬向重力波应力"),"N//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","ugwd.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "vgwd"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","vgwd","Meridional Gravity Wave Stress",QObject::tr("经向重力波应力"),"N//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","vgwd.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "uflx"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","uflx","Momentum Flux, U-Component",QObject::tr("角动量通量纬向分量"),"N//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","uflx.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "vflx"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","vflx","Momentum Flux, V-Component",QObject::tr("角动量通量经向分量"),"N//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","vflx.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "skt"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","skt","Skin Temperature",QObject::tr("表面温度"),"degC",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","skt.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "prate"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","prate","Precipitation Rate",QObject::tr("降水率"),"Kg//m^2//s",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","prate.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "runof"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","runof","Water Runoff",QObject::tr("水径流"),"kg//m^2",0.00,1.00,QObject::tr("表面"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","runof.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "soilw"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","soilw","Volumetric Soil Moisture",QObject::tr("土壤体湿度"),"fraction",0.00,1.00,QObject::tr("地下0-10cm"),"192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","soilw.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "soilw200"),new CElementInfo(QObject::tr("逐月"),"surface_gauss","soilw200","Volumetric Soil Moisture Between 10-200 cm BGL",QObject::tr("地面以下10-200cm土壤体湿度"),"fraction",0.00,1.00,"地下10-200cm","192×94","1","0-358.125,-88.542f -88.542","-9.96921E+36","soilw200.mon.mean.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "air"),new CElementInfo(QObject::tr("逐月"),"tropopause","air","Air Temperature at Tropopause",QObject::tr("对流层顶气温"),"degC",225.46,0.01,QObject::tr("对流层顶"),"144×73","1","0.-357.5,  -90-90","-9.96921E+36","air.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐月"), "pres"),new CElementInfo(QObject::tr("逐月"),"tropopause","pres","Pressure at Tropopause",QObject::tr("对流层顶气压"),"hPa",225.45,0.01,QObject::tr("对流层顶"),"144×73","1","0.-357.5,  -90-90","-9.96921E+36","pres.mon.mean.nc"));

	///////////////////////////////////////////逐日/////////////////////////////////////////////////////////
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "csulf"),new CElementInfo(QObject::tr("逐日"),"other_gauss","csulf","Clear Sky Upward Longwave Flux",QObject::tr("晴空向上长波辐射通量"),"W//m^2",3176.50,0.10,QObject::tr("名义上的大气层顶"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","csulf.ntat.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "csusf"),new CElementInfo(QObject::tr("逐日"),"other_gauss","csusf","Clear Sky Upward Solar Flux",QObject::tr("晴空向上太阳辐射通量"),"W//m^2",3176.50,0.10,QObject::tr("名义上的大气层顶"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","csusf.ntat.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "dswrf"),new CElementInfo(QObject::tr("逐日"),"other_gauss","dswrf","Downward Solar Radiation Flux",QObject::tr("向下太阳辐射通量"),"W//m^2",3176.50,0.10,QObject::tr("名义上的大气层顶"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","dswrf.ntat.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "ulwrf"),new CElementInfo(QObject::tr("逐日"),"other_gauss","ulwrf","Upward Longwave Radiation Flux",QObject::tr("向上长波辐射通量"),"W//m^2",3176.50,0.10,QObject::tr("名义上的大气层顶"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","ulwrf.ntat.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "uswrf"),new CElementInfo(QObject::tr("逐日"),"other_gauss","uswrf","Upward Solar Radiation Flux",QObject::tr("向上太阳辐射通量"),"W//m^2",3176.50,0.10,QObject::tr("名义上的大气层顶"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","uswrf.ntat.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "pres"),new CElementInfo(QObject::tr("逐日"),"other_gauss","pres","High Cloud Bottom Pressure",QObject::tr("高云云底气压"),"Pascals",327650.00,10.00,QObject::tr("高云云底"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","pres.hcb.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "pres"),new CElementInfo(QObject::tr("逐日"),"other_gauss","pres","Pressure",QObject::tr("高云云顶气压"),"Pascals",327650.00,10.00,QObject::tr("高云云顶"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","pres.hct.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "pres"),new CElementInfo(QObject::tr("逐日"),"other_gauss","pres","Pressure",QObject::tr("低云云底气压"),"Pascals",327650.00,10.00,QObject::tr("低云云底"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","pres.lcb.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "pres"),new CElementInfo(QObject::tr("逐日"),"other_gauss","pres","Pressure",QObject::tr("低云云顶气压"),"Pascals",327650.00,10.00,QObject::tr("低云云顶"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","pres.lct.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "pres"),new CElementInfo(QObject::tr("逐日"),"other_gauss","pres","Pressure",QObject::tr("中云云底气压"),"Pascals",327650.00,10.00,QObject::tr("中云云底"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","pres.mcb.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "pres"),new CElementInfo(QObject::tr("逐日"),"other_gauss","pres","Pressure",QObject::tr("中云云顶气压"),"Pascals",327650.00,10.00,QObject::tr("中云云顶"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","pres.mct.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "tcdc"),new CElementInfo(QObject::tr("逐日"),"other_gauss","tcdc","Total cloud cover",QObject::tr("总云量"),"%",3276.50,0.10,QObject::tr("整层大气视为一层"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","tcdc.eatm.gauss.YYYY.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "air"),new CElementInfo(QObject::tr("逐日"),"pressure","air","Air temperature",QObject::tr("气温"),"degK",477.66,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa ","144×73","17层","0-357.5, -90-90","32766","air.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "hgt"),new CElementInfo(QObject::tr("逐日"),"pressure","hgt","Geopotential height",QObject::tr("位势高度"),"m",32066.00 ,1.00,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa ","144×73","17层","0-357.5, -90-90","32766","hgt.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "uwnd"),new CElementInfo(QObject::tr("逐日"),"pressure","uwnd","U-wind",QObject::tr("U风场"),"m//s",202.66,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa ","144×73","17层","0-357.5, -90-90","32766","uwnd.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "vwnd"),new CElementInfo(QObject::tr("逐日"),"pressure","vwnd","V-wind",QObject::tr("U风场"),"m//s",202.66,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa ","144×73","17层","0-357.5, -90-90","32766","vwnd.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "omega"),new CElementInfo(QObject::tr("逐日"),"pressure","omega","Omega",QObject::tr("垂直速度"),"Pascal//s",29.765,0.001,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa ","144×73","前12层","0-357.5, -90-90","32766","omega.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "rhum"),new CElementInfo(QObject::tr("逐日"),"pressure","rhum","Relative humidity",QObject::tr("相对湿度"),"%",302.66 ,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa ","144×73","前8层","0-357.5, -90-90","32766","rhum.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "shum"),new CElementInfo(QObject::tr("逐日"),"pressure","shum","Specific humidity",QObject::tr("比湿"),"kg/kg",0.032666,1.E-06,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa ","144×73","前8层","0-357.5, -90-90","32766","shum.YYYY.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "air"),new CElementInfo(QObject::tr("逐日"),"pressure","air","Air temperature at sigma level 995",QObject::tr("sigma=0.995层气温"),"degK",512.81,0.01,"sigma=0.995","144×73",QObject::tr("1层"),"0-357.5, -90-90","32766","air.sig995.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "pottmp"),new CElementInfo(QObject::tr("逐日"),"pressure","pottmp","potential temperature at sigma level 995",QObject::tr("sigma=0.995层位温"),"degK",3426.50 ,0.10,"sigma=0.995","144×73",QObject::tr("1层"),"0-357.5, -90-90","32766","pottmp.sig995.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "rhum"),new CElementInfo(QObject::tr("逐日"),"pressure","rhum","relative humidity at sigma level 995",QObject::tr("sigma=0.995层相对湿度"),"%",302.65,0.01,"sigma=0.995","144×73",QObject::tr("1层"),"0-357.5, -90-90","32766","rhum.sig995.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "uwnd"),new CElementInfo(QObject::tr("逐日"),"pressure","uwnd","u-wind at sigma level 995",QObject::tr("sigma=0.995层U风场"),"m//s",225.45,0.01,"sigma=0.995","144×73",QObject::tr("1层"),"0-357.5, -90-90","32766","uwnd.sig995.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "vwnd"),new CElementInfo(QObject::tr("逐日"),"pressure","vwnd","u-wind at sigma level 995",QObject::tr("sigma=0.995层V风场"),"m//s",225.45,0.01,"sigma=0.995","144×73",QObject::tr("1层"),"0-357.5, -90-90","32766","vwnd.sig995.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "omega"),new CElementInfo(QObject::tr("逐日"),"pressure","omega","omega at surface",QObject::tr("表面垂直速度"),"Pascal//s",29.765,0.001,"sigma=0.995","144×73",QObject::tr("1层"),"0-357.5, -90-90","32766","omega.sig995.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "lftx"),new CElementInfo(QObject::tr("逐日"),"pressure","lftx","Surface Lifted Index",QObject::tr("表面抬升指数"),"degK",309.65,0.01,"sigma=0.995","144×73",QObject::tr("1层"),"0-357.5, -90-90","32766","lftx.sfc.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "lftx4"),new CElementInfo(QObject::tr("逐日"),"pressure","lftx4","Best (4-layer) Lifted Index",QObject::tr("最佳抬升指数"),"degK",302.65,0.01,"sigma=0.995","144×73",QObject::tr("1层"),"0-357.5, -90-90","32766","lftx4.sfc.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "pres"),new CElementInfo(QObject::tr("逐日"),"pressure","pres","Pressure at surface",QObject::tr("表面气压"),"Pascals",367650.00,10.00,"sigma=0.995","144×73",QObject::tr("1层"),"0-357.5, -90-90","32766","pres.sfc.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "pr_wtr"),new CElementInfo(QObject::tr("逐日"),"pressure","pr_wtr","Precipitable Water for entire atmosphere",QObject::tr("整层大气可降水量"),"kg//m^2",277.65,0.01,"sigma=0.995","144×73",QObject::tr("1层"),"0-357.5, -90-90","32766","pr_wtr.eatm.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "slp"),new CElementInfo(QObject::tr("逐日"),"pressure","slp","Sea Level Pressure",QObject::tr("海表面气压"),"Pascals",119765.00,1.00,"sigma=0.995","144×73",QObject::tr("1层"),"0-357.5, -90-90","32766","slp.YYYY.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "air"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","air","Air temperature at 2 m",QObject::tr("2米气温"),"degK",477.65,0.01,"2m","192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","air.2m.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "tmax"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","tmax","Maximum Temperature at 2 m",QObject::tr("2米最高气温"),"degK",477.65,0.01,"2m","192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","tmax.2m.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "tmin"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","tmin","Minimum Temperature at 2 m",QObject::tr("2米最低气温"),"degK",477.65,0.01,"2m","192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","tmin.2m.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "shum"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","shum","Specific Humidity at 2 m",QObject::tr("2米比湿"),"kg/kg",0.32765,1.E-05,"2m","192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","shum.2m.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "soilw"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","soilw","Volumetric Soil Moisture between 0-10 cm Below Ground Level",QObject::tr("地下0-10cm之间单位体积土壤湿度"),"fraction",3.2765,0.0001,"地下0-10cm","192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","soilw.0-10cm.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "soilw"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","soilw","Volumetric Soil Moisture between 10-200 cm Below Ground Level",QObject::tr("地下10-200cm之间单位体积土壤体湿度"),"fraction",3.2765,0.0001,"地下10-200cm","192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","soilw.10-200cm.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "tmp"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","tmp","Temperature between 0-10 cm below ground level",QObject::tr("地下0-10cm之间温度"),"degK",477.65,0.01,"地下0-10cm","192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","tmp.0-10cm.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "tmp"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","tmp","Temperature between 10-200 cm below ground level",QObject::tr("地下10-200cm之间温度"),"degK",477.65,0.01,"地下10-200cm","192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","tmp.10-200cm.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "tmp"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","tmp","Temperature at 300 cm below ground level",QObject::tr("地下300cm温度"),"degK",477.65,0.01,"地下300cm","192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","tmp.300cm.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "uwnd"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","uwnd","u-wind at 10 m",QObject::tr("10米u风场"),"m//s",225.45,0.01,"10m","192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","uwnd.10m.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "vwnd"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","vwnd","v-wind at 10 m",QObject::tr("10米v风场"),"m//s",225.45,0.01,"10m","192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","vwnd.10m.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "cprat"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","cprat","Convective Precipitation Rate at surface",QObject::tr("表面对流降水率"),"Kg//m^2//s",0.0031765,1.E-07,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","cprat.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "prate"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","prate","Precipitation Rate at surface",QObject::tr("表面降水率"),"Kg//m^2//s",0.0032765,1.E-07,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","prate.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "icec"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","icec","Ice Concentration at surface","表面冰密集度","",3276.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","icec.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "cfnlf"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","cfnlf","Cloud Forcing Net Longwave Flux at Surface",QObject::tr("表面云强迫净长波通量"),"W//m^2",3176.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","cfnlf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "csdlf"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","csdlf","Clear Sky Downward Longwave Flux at surface",QObject::tr("表面晴空向下长波通量"),"W//m^2",3176.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","csdlf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "dlwrf"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","dlwrf","Downward Longwave Radiation Flux at surface",QObject::tr("表面向下长波辐射通量"),"W//m^2",3176.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","dlwrf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "ulwrf"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","ulwrf","Upward Longwave Radiation Flux at surface",QObject::tr("表面向上长波辐射通量"),"W//m^2",3246.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","ulwrf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "cfnsf"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","cfnsf","Cloud Forcing Net Solar Flux at Surface",QObject::tr("表面云强迫净太阳辐射通量"),"W//m^2",2376.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","cfnsf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "csdsf"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","csdsf","Clear Sky Downward Solar Flux at surface",QObject::tr("表面晴空向下太阳辐射通量"),"W//m^2",3156.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","csdsf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "csusf"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","csusf","Clear Sky Upward Solar Flux at surface",QObject::tr("表面晴空向上太阳辐射通量"),"W//m^2",3176.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","csusf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "dswrf"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","dswrf","Downward Solar Radiation Flux at surface",QObject::tr("表面向下太阳辐射通量"),"W//m^2",3156.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","dswrf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "uswrf"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","uswrf","Upward Solar Radiation Flux at surface",QObject::tr("表面向上太阳辐射通量"),"W//m^2",3176.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","uswrf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "gflux"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","gflux","Ground Heat Flux at surface",QObject::tr("表面地热通量"),"W//m^2",2876.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","gflux.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "lhtfl"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","lhtfl","Latent Heat Net Flux at surface",QObject::tr("表面潜热净通量"),"W//m^2",1276.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","lhtfl.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "shtfl"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","shtfl","Sensible Heat Net Flux at surface",QObject::tr("表面感热净通量"),"W//m^2",30766.00,1.00,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","shtfl.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "nbdsf"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","nbdsf","Near IR Beam Downward Solar Flux at surface",QObject::tr("表面近红外光向下太阳辐射通量"),"W//m^2",3126.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","nbdsf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "nddsf"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","nddsf","Near IR Diffuse Downward Solar Flux at surface",QObject::tr("表面近红外散射向下太阳辐射通量"),"W//m^2",3156.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","nddsf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "nlwrs"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","nlwrs","Net Longwave Radiation Flux at Surface",QObject::tr("表面净长波辐射通量"),"W//m^2",3176.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","nlwrs.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "nswrs"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","nswrs","Net Shortwave Radiation Flux at Surface",QObject::tr("表面净短波辐射通量"),"W//m^2",1876.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","nswrs.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "pevpr"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","pevpr","Potential Evaporation Rate at surface",QObject::tr("表面潜蒸发率"),"W//m^2",2476.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","pevpr.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "pres"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","pres","Surface Pressure",QObject::tr("表面气压"),"Pascals",367650.00,10.00,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","pres.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "runof"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","runof","Water Runoff at surface",QObject::tr("表面水径流"),"kg//m^2",307.66,0.01,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","runof.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "sfcr"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","sfcr","Surface Roughness",QObject::tr("表面粗糙度"),"m",0.032065,1.E-06,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","sfcr.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "skt"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","skt","SST/Land Skin Temperature",QObject::tr("海表/陆表面温度"),"degK",477.65,0.01,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","skt.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "uflx"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","uflx","Momentum Flux, u-component at surface",QObject::tr("表面动量u分量"),"N//m^2",28.765,0.001,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","uflx.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "vflx"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","vflx","Momentum Flux, v-component at surface",QObject::tr("表面动量v分量"),"N//m^2",28.765,0.001,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","vflx.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "ugwd"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","ugwd","Zonal Gravity Wave Stress at surface",QObject::tr("表面纬向重力波应力"),"N//m^2",0.765,0.001,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","ugwd.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "vgwd"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","vgwd","Meridional Gravity Wave Stress at surface",QObject::tr("表面经向重力波应力"),"N//m^2",0.765,0.001,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","vgwd.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "vbdsf"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","vbdsf","Visible Beam Downward Solar Flux at surface",QObject::tr("表面可见光向下太阳辐射通量"),"W//m^2",3126.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","vbdsf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "vddsf"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","vddsf","Visible Diffuse Downward Solar Flux at surface",QObject::tr("表面可见散射向下太阳辐射通量"),"W//m^2",3176.50,0.10,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","vddsf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "weasd"),new CElementInfo(QObject::tr("逐日"),"surface_gauss","weasd","Water Equiv. of Accum. Snow Depth at surface",QObject::tr("表面积雪深度的水当量"),"kg//m^2",3276.50,1.00,QObject::tr("表面"),"192×94",QObject::tr("1层"),"0-358.125,-88.542-88.542","32766","weasd.sfc.gauss.YYYY.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "air"),new CElementInfo(QObject::tr("逐日"),"tropopause","air","Air Temperature at Tropopause",QObject::tr("对流层顶气温"),"degK",467.65,0.01,QObject::tr("对流层顶"),"144×73",QObject::tr("1层"),"0-357.5, -90-90","32766","air.tropp.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("逐日"), "pres"),new CElementInfo(QObject::tr("逐日"),"tropopause","pres","Pressure at Tropopause",QObject::tr("对流层顶气压"),"Pascals",327660.00,10.00,QObject::tr("对流层顶"),"144×73",QObject::tr("1层"),"0-357.5, -90-90","32766","pres.tropp.YYYY.nc"));
}

 /************************************************************************/
 /* 函数功能： 获得要素信息      	      			                     */
 /* 参数说明:  time  时次 name  英文短名								 */
 /* 返 回 值： m_elements   要素成员                                     */
 /* 作    者:  杨潇													     */
 /* 日    期:  2011 年 05 月 09 日										 */
 /************************************************************************/
CElementInfo* CElementFactory::GetElementInfo(QString time, QString name)
{
	QPair<QString,QString> key(time , name);
	return m_elements.value(key);
}

CElementFactory::~CElementFactory()
{
	QHash<QPair<QString, QString>, CElementInfo*>::const_iterator moditDrawType;
	//循环m_htDate得到符合该节点名称的数据结构体初始化对话框
	for( moditDrawType=m_elements.constBegin(); moditDrawType!=m_elements.constEnd(); ++moditDrawType)
	{
		delete moditDrawType.value();
	}
}

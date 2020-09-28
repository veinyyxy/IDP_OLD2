/*================================================================================== 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ����
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����CElementInfo.h CElementInfo.cpp
�� ���� CElementInfo
�� �ࣺ NULL
�� �ࣺ �� 
����˵���� CIPASϵͳ����������˵���� 3.2.13.1 NCEP�ٷ�������
����˵���� 

-------------------------------------�汾����-----------------------------------------
V 1.2 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ������
������ڣ�2011��05��09��
=====================================================================================*/
#include "CElementInfo.h"
//////////////////CElementInfo//////////////////////

/************************************************************************/
/* �������ܣ� Ĭ�Ϲ��캯��						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� 05 �� 09 ��										*/
/************************************************************************/
CElementInfo::CElementInfo()
{
	m_offset = 0;
	m_rule = 0;
}

/************************************************************************/
/* �������ܣ� ���캯��	      					                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� 05 �� 09 ��										*/
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
/* �������ܣ� ���캯��	      					                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� 05 �� 09 ��										*/
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
/* �������ܣ� ������������캯��	      			                    */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� 05 �� 09 ��										*/
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
/* �������ܣ� ���Ҫ��������     	      			                    */
/* ����˵��:  NULL														*/
/* �� �� ֵ�� m_ch_name   Ҫ��������                                    */
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� 05 �� 09 ��										*/
/************************************************************************/
QString CElementInfo::GetChineseName()
{
	return m_ch_name;
}
 //////////////////////////CElementFactory////////////////////////

CElementFactory* CElementFactory::pFactory = NULL;

/************************************************************************/
/* �������ܣ� ����CElementFactory  	      			                    */
/* ����˵��:  NULL														*/
/* �� �� ֵ�� pFactory                                                  */
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� 05 �� 09 ��										*/
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
/* �������ܣ� Ĭ�Ϲ��캯��						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� 05 �� 09 ��										*/
/************************************************************************/
CElementFactory::CElementFactory()
{
	InitAllElements();
}

/************************************************************************/
/* �������ܣ� ��ʼ��             	      			                    */
/* ����˵��:  NULL														*/
/* �� �� ֵ�� ��                                                        */
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� 05 �� 09 ��										*/
/************************************************************************/
void CElementFactory::InitAllElements()
{
    //����˳��Ϊ  ʱ�� ���� Ҫ���ļ� Ҫ��Ӣ���� Ҫ�������� ��λ ƫ���� �߶����� ��� ˮƽ�ֱ��� ��ֱ�ֱ��� ��Χ ȱʡֵ ����Ҫ���ļ���
	////////////////////////////////����/////////////////////////////////////
	m_elements.clear();
 	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "csulftop"),new CElementInfo(QObject::tr("����"),"other_gaus","csulftop","Clear Sky Upward Longwave Flux",QObject::tr("������ϳ���ͨ��"),"W//m^2",0.00,1.00,QObject::tr("�����ϵĴ����㶥"),"192��94","1","0-358.125,-88.542-88.542","-9.96921E+36","csulftop.mon.mean.nc"));
 	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "csusftop"),new CElementInfo(QObject::tr("����"),"other_gaus","csusftop","Clear Sky Upward Shortwave Flux",QObject::tr("������϶̲�ͨ��"),"W//m^2",0.00,1.00,QObject::tr("�����ϵĴ����㶥"),"192��94","1","0-358.125,-88.542-88.542","-9.96921E+36","csusftop.mon.mean.nc"));
 	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "dswrftop"),new CElementInfo(QObject::tr("����"),"other_gaus","dswrftop","Downward Solar Radiation Flux",QObject::tr("����̫������ͨ��"),"W//m^2",0.00,1.00,QObject::tr("�����ϵĴ����㶥"),"192��94","1","0-358.125,-88.542-88.542","-9.96921E+36","dswrftop.mon.mean.nc"));
 	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "ulwrftop"),new CElementInfo(QObject::tr("����"),"other_gaus","ulwrftop","Upward Longwave Radiation Flux",QObject::tr("���ϳ�������ͨ��"),"W//m^2",0.00,1.00,QObject::tr("�����ϵĴ����㶥"),"192��94","1","0-358.125,-88.542-88.542","-9.96921E+36","ulwrftop.mon.mean.nc"));
 	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "uswrftop"),new CElementInfo(QObject::tr("����"),"other_gaus","uswrftop","Upward Solar Radiation Flux",QObject::tr("����̫������ͨ��"),"W//m^2",0.00,1.00,QObject::tr("�����ϵĴ����㶥"),"192��94","1","0-358.125,-88.542-88.542","-9.96921E+36","dswrftop.mon.mean.nc"));
 	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pv"),new CElementInfo(QObject::tr("����"),"other_gaus","pv","Upward Solar Radiation Flux","λ��","pvu (=10^(-6)*m^2*s^(-1)*kg^(-1)*K)",0.00,1.00,"345 degK","192��94","1","0-358.125,-88.542-88.542","-9.96921E+36","pv.mon.mean.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "air"),new CElementInfo(QObject::tr("����"),"pressure","air","Air temperature",QObject::tr("����"),"degC",127.65,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144��73","17","0-357.5, -90-90","-9.96921E+36","air.mon.mean.nc"));
 	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "hgt"),new CElementInfo(QObject::tr("����"),"pressure","hgt","geopotential height",QObject::tr("λ�Ƹ߶�"),"m",0.00,1.00,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144��73","17","0-357.5, -90-90","-9.96921E+36","hgt.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pottmp"),new CElementInfo(QObject::tr("����"),"pressure","pottmp","Potential Temperature",QObject::tr("λ��"),"degK",0.00,1.00,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144��73","17","0-357.5, -90-90","-9.96921E+36","pottmp.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "uwnd"),new CElementInfo(QObject::tr("����"),"pressure","uwnd","u wind",QObject::tr("U�糡"),"m//s",202.65,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144��73","17","0-357.5, -90-90","-9.96921E+36","uwnd.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "vwnd"),new CElementInfo(QObject::tr("����"),"pressure","vwnd","v wind",QObject::tr("V�糡"),"m//s",202.65,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144��73","17","0-357.5, -90-90","-9.96921E+36","vwnd.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "wspd"),new CElementInfo(QObject::tr("����"),"pressure","wspd","wind speed",QObject::tr("����"),"m/s",202.65,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144��73","17","0-357.5, -90-90","-9.96921E+36","wspd.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "omega"),new CElementInfo(QObject::tr("����"),"pressure","omega","Omega",QObject::tr("��ֱ�ٶ�"),"Pascal//s",29.765,0.001,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144��73","ǰ12��","0-357.5, -90-90","-9.96921E+36","omega.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "rhum"),new CElementInfo(QObject::tr("����"),"pressure","rhum","Relative Humidity",QObject::tr("���ʪ��"),"%",302.65,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144��73","ǰ8��","0-357.5, -90-90","-9.96921E+36","rhum.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "shum"),new CElementInfo(QObject::tr("����"),"pressure","shum","Specific Humidity",QObject::tr("��ʪ"),"grams//kg",32.665,0.001,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa","144��73","ǰ8��","0-357.5, -90-90","-9.96921E+36","shum.mon.mean.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "chi"),new CElementInfo(QObject::tr("����"),"sigma","chi","Velocity Potential",QObject::tr("�ٶ���"),"m*m//s",0.00,1.00,"sigma��0.995, 0.8458, 0.2582, 0.2101, 0.1682","192��94","5","0-358.125,-88.542f -88.542","-9.96921E+36","chi.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "psi"),new CElementInfo(QObject::tr("����"),"sigma","psi","Streamfunction",QObject::tr("������"),"m*m//s",0.00,1.00,"sigma��0.995, 0.8458, 0.2582, 0.2101, 0.1682","192��94","5","0-358.125,-88.542f -88.542","-9.96921E+36","psi.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "div"),new CElementInfo(QObject::tr("����"),"sigma","div","Divergence",QObject::tr("ɢ��"),"1.//s",0.00,1.00,"sigma��0.995, 0.2101","192��94","2","0-358.125,-88.542f -88.542","-9.96921E+36","div.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "vor"),new CElementInfo(QObject::tr("����"),"sigma","vor","Vorticity",QObject::tr("�ж�"),"1.//s",0.00,1.00,"sigma��0.995, 0.2101","192��94","2","0-358.125,-88.542f -88.542","-9.96921E+36","vor.mon.mean.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "chi"),new CElementInfo(QObject::tr("����"),"spectral","chi","Velocity Potential",QObject::tr("�ٶ���"),"m*m//s",0.00,1.00,"sigma��0.995, 0.8458, 0.2582, 0.2101, 0.1682","192��94","5","0-358.125,-88.542f -88.542","-9.96921E+36","chi.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "psi"),new CElementInfo(QObject::tr("����"),"spectral","psi","Streamfunction",QObject::tr("������"),"m*m//s",0.00,1.00,"sigma��0.995, 0.8458, 0.2582, 0.2101, 0.1682","192��94","5","0-358.125,-88.542f -88.542","-9.96921E+36","psi.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "div"),new CElementInfo(QObject::tr("����"),"spectral","div","Divergence",QObject::tr("ɢ��"),"1.//s",0.00,1.00,"sigma��0.995, 0.2101","192��94","2","0-358.125,-88.542f -88.542","-9.96921E+36","div.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "vor"),new CElementInfo(QObject::tr("����"),"spectral","vor","Vorticity",QObject::tr("�ж�"),"1.//s",0.00,1.00,"sigma��0.995, 0.2101","192��94","2","0-358.125,-88.542f -88.542","-9.96921E+36","vor.mon.mean.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "air"),new CElementInfo(QObject::tr("����"),"surface","air","Air Temperature",QObject::tr("��������"),"degC",0.00,1.00,QObject::tr("����"),"144��73","1","0-357.5, -90-90","-9.96921E+36","air.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pres"),new CElementInfo(QObject::tr("����"),"surface","pres","Surface Pressure",QObject::tr("������ѹ"),"hPa",0.00,1.00,QObject::tr("����"),"144��73","1","0-357.5, -90-90","-9.96921E+36","pres.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pr_wtr"),new CElementInfo(QObject::tr("����"),"surface","pr_wtr","Precipitable Water Content",QObject::tr("�ɽ�ˮ"),"kg//m^2",0.00,1.00,QObject::tr("����"),"144��73","1","0-357.5, -90-90","-9.96921E+36","pr_wtr.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "rhum"),new CElementInfo(QObject::tr("����"),"surface","rhum","Relative Humidity",QObject::tr("���ʪ��"),"%",0.00,1.00,QObject::tr("����"),"144��73","1","0-357.5, -90-90","-9.96921E+36","rhum.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "slp"),new CElementInfo(QObject::tr("����"),"surface","slp","Sea Level Pressure",QObject::tr("��ƽ����ѹ"),"hPa",0.00,1.00,QObject::tr("��ƽ��"),"144��73","1","0-357.5, -90-90","-9.96921E+36","slp.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "thickness"),new CElementInfo(QObject::tr("����"),"surface","thickness","Thickness of 1000-500mb",QObject::tr("1000-500mb�������"),"m",0.00,1.00,"1000-500mb","144��73","1","0-357.5, -90-90","-9.96921E+36","thickness.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "thickness_1000500"),new CElementInfo(QObject::tr("����"),"surface","thickness_1000500","Thickness of 1000-500mb",QObject::tr("1000-500mb�������"),"m",0.00,1.00,"1000-500mb","144��73","1","0-357.5, -90-90","-9.96921E+36","thickness_1000500.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "thickness_500200"),new CElementInfo(QObject::tr("����"),"surface","thickness_500200","Thickness of 500-200mb",QObject::tr("500-200mb�������"),"m",0.00,1.00,"500-200mb","144��73","1","0-357.5, -90-90","-9.96921E+36","thickness_500200.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "thickness_850500"),new CElementInfo(QObject::tr("����"),"surface","thickness_850500","Thickness of 850-500mb",QObject::tr("850-500mb�������"),"m",0.00,1.00,"850-500mb","144��73","1","0-357.5, -90-90","-9.96921E+36","thickness_850500.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "uwnd"),new CElementInfo(QObject::tr("����"),"surface","uwnd","surface zonal wind",QObject::tr("����γ���"),"m//s",0.00,1.00,QObject::tr("����"),"144��73","1","0-357.5, -90-90","-9.96921E+36","uwnd.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "vwnd"),new CElementInfo(QObject::tr("����"),"surface","vwnd","surface meridional wind",QObject::tr("���澭���"),"m//s",0.00,1.00,QObject::tr("����"),"144��73","1","0-357.5, -90-90","-9.96921E+36","vwnd.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "wspd"),new CElementInfo(QObject::tr("����"),"surface","wspd","surface wind speed",QObject::tr("�������"),"m//s",202.65,0.01,QObject::tr("����"),"144��73","1","0-357.5, -90-90","-9.96921E+36","wspd.mon.mean.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "air2m"),new CElementInfo(QObject::tr("����"),"surface_gauss","air2m","Air Temperature",QObject::tr("2m����"),"degK",0.00,1.00,"2m","192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","air2m.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "shum2m"),new CElementInfo(QObject::tr("����"),"surface_gauss","shum2m","Specific Humidity",QObject::tr("2m��ʪ"),"grams//kg",0.00,1.00,"2m","192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","shum2m.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "uwnd10m"),new CElementInfo(QObject::tr("����"),"surface_gauss","uwnd10m","U-Wind at 10 m",QObject::tr("10m U�糡"),"m//s",0.00,1.00,"10m","192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","uwnd10m.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "vwnd10m"),new CElementInfo(QObject::tr("����"),"surface_gauss","vwnd10m","V wind at 10 m",QObject::tr("10m V�糡"),"m//s",0.00,1.00,"10m","192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","vwnd10m.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "cfnlf"),new CElementInfo(QObject::tr("����"),"surface_gauss","cfnlf","Cloud Forcing Net Longwave Flux",QObject::tr("��ǿ�Ⱦ�����ͨ��"),"W//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","cfnlf.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "csdlf"),new CElementInfo(QObject::tr("����"),"surface_gauss","csdlf","Clear Sky Downward Longwave Flux",QObject::tr("������³���ͨ��"),"W//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","csdlf.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "csdsf"),new CElementInfo(QObject::tr("����"),"surface_gauss","csdsf","Clear Sky Downward Solar Flux",QObject::tr("�������̫��ͨ��"),"W//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","csdsf.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "csusf"),new CElementInfo(QObject::tr("����"),"surface_gauss","csusf","Clear Sky Upward Solar Flux",QObject::tr("�������̫��ͨ��"),"W//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","csusf.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "nbdsf"),new CElementInfo(QObject::tr("����"),"surface_gauss","nbdsf","Near IR Beam Downward Solar Flux",QObject::tr("�����������̫��ͨ��"),"W//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","nbdsf.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "nlwrs"),new CElementInfo(QObject::tr("����"),"surface_gauss","nlwrs","Net Longwave Radiation Flux",QObject::tr("����������ͨ��"),"W//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","nlwrs.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "nswrs"),new CElementInfo(QObject::tr("����"),"surface_gauss","nswrs","Net Shortwave Radiation Flux",QObject::tr("���̲�����ͨ��"),"W//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","nswrs.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "ulwrf"),new CElementInfo(QObject::tr("����"),"surface_gauss","ulwrf","Upward Longwave Radiation Flux",QObject::tr("���ϳ�������ͨ��"),"W//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","ulwrf.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pevpr"),new CElementInfo(QObject::tr("����"),"surface_gauss","pevpr","Potential Evaporation Rate",QObject::tr("Ǳ������"),"W//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","pevpr.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "gflux"),new CElementInfo(QObject::tr("����"),"surface_gauss","gflux","Ground Heat Flux",QObject::tr("����ͨ��"),"W//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","gflux.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "lhtfl"),new CElementInfo(QObject::tr("����"),"surface_gauss","lhtfl","Latent Heat Net Flux",QObject::tr("Ǳ�Ⱦ�ͨ��"),"W//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","lhtfl.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "shtfl"),new CElementInfo(QObject::tr("����"),"surface_gauss","shtfl","Sensible Heat Net Flux",QObject::tr("���Ⱦ�ͨ��"),"W//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","shtfl.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "ugwd"),new CElementInfo(QObject::tr("����"),"surface_gauss","ugwd","Zonal Gravity Wave Stress",QObject::tr("γ��������Ӧ��"),"N//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","ugwd.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "vgwd"),new CElementInfo(QObject::tr("����"),"surface_gauss","vgwd","Meridional Gravity Wave Stress",QObject::tr("����������Ӧ��"),"N//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","vgwd.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "uflx"),new CElementInfo(QObject::tr("����"),"surface_gauss","uflx","Momentum Flux, U-Component",QObject::tr("�Ƕ���ͨ��γ�����"),"N//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","uflx.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "vflx"),new CElementInfo(QObject::tr("����"),"surface_gauss","vflx","Momentum Flux, V-Component",QObject::tr("�Ƕ���ͨ���������"),"N//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","vflx.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "skt"),new CElementInfo(QObject::tr("����"),"surface_gauss","skt","Skin Temperature",QObject::tr("�����¶�"),"degC",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","skt.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "prate"),new CElementInfo(QObject::tr("����"),"surface_gauss","prate","Precipitation Rate",QObject::tr("��ˮ��"),"Kg//m^2//s",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","prate.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "runof"),new CElementInfo(QObject::tr("����"),"surface_gauss","runof","Water Runoff",QObject::tr("ˮ����"),"kg//m^2",0.00,1.00,QObject::tr("����"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","runof.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "soilw"),new CElementInfo(QObject::tr("����"),"surface_gauss","soilw","Volumetric Soil Moisture",QObject::tr("������ʪ��"),"fraction",0.00,1.00,QObject::tr("����0-10cm"),"192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","soilw.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "soilw200"),new CElementInfo(QObject::tr("����"),"surface_gauss","soilw200","Volumetric Soil Moisture Between 10-200 cm BGL",QObject::tr("��������10-200cm������ʪ��"),"fraction",0.00,1.00,"����10-200cm","192��94","1","0-358.125,-88.542f -88.542","-9.96921E+36","soilw200.mon.mean.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "air"),new CElementInfo(QObject::tr("����"),"tropopause","air","Air Temperature at Tropopause",QObject::tr("�����㶥����"),"degC",225.46,0.01,QObject::tr("�����㶥"),"144��73","1","0.-357.5,  -90-90","-9.96921E+36","air.mon.mean.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pres"),new CElementInfo(QObject::tr("����"),"tropopause","pres","Pressure at Tropopause",QObject::tr("�����㶥��ѹ"),"hPa",225.45,0.01,QObject::tr("�����㶥"),"144��73","1","0.-357.5,  -90-90","-9.96921E+36","pres.mon.mean.nc"));

	///////////////////////////////////////////����/////////////////////////////////////////////////////////
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "csulf"),new CElementInfo(QObject::tr("����"),"other_gauss","csulf","Clear Sky Upward Longwave Flux",QObject::tr("������ϳ�������ͨ��"),"W//m^2",3176.50,0.10,QObject::tr("�����ϵĴ����㶥"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","csulf.ntat.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "csusf"),new CElementInfo(QObject::tr("����"),"other_gauss","csusf","Clear Sky Upward Solar Flux",QObject::tr("�������̫������ͨ��"),"W//m^2",3176.50,0.10,QObject::tr("�����ϵĴ����㶥"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","csusf.ntat.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "dswrf"),new CElementInfo(QObject::tr("����"),"other_gauss","dswrf","Downward Solar Radiation Flux",QObject::tr("����̫������ͨ��"),"W//m^2",3176.50,0.10,QObject::tr("�����ϵĴ����㶥"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","dswrf.ntat.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "ulwrf"),new CElementInfo(QObject::tr("����"),"other_gauss","ulwrf","Upward Longwave Radiation Flux",QObject::tr("���ϳ�������ͨ��"),"W//m^2",3176.50,0.10,QObject::tr("�����ϵĴ����㶥"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","ulwrf.ntat.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "uswrf"),new CElementInfo(QObject::tr("����"),"other_gauss","uswrf","Upward Solar Radiation Flux",QObject::tr("����̫������ͨ��"),"W//m^2",3176.50,0.10,QObject::tr("�����ϵĴ����㶥"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","uswrf.ntat.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pres"),new CElementInfo(QObject::tr("����"),"other_gauss","pres","High Cloud Bottom Pressure",QObject::tr("�����Ƶ���ѹ"),"Pascals",327650.00,10.00,QObject::tr("�����Ƶ�"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","pres.hcb.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pres"),new CElementInfo(QObject::tr("����"),"other_gauss","pres","Pressure",QObject::tr("�����ƶ���ѹ"),"Pascals",327650.00,10.00,QObject::tr("�����ƶ�"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","pres.hct.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pres"),new CElementInfo(QObject::tr("����"),"other_gauss","pres","Pressure",QObject::tr("�����Ƶ���ѹ"),"Pascals",327650.00,10.00,QObject::tr("�����Ƶ�"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","pres.lcb.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pres"),new CElementInfo(QObject::tr("����"),"other_gauss","pres","Pressure",QObject::tr("�����ƶ���ѹ"),"Pascals",327650.00,10.00,QObject::tr("�����ƶ�"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","pres.lct.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pres"),new CElementInfo(QObject::tr("����"),"other_gauss","pres","Pressure",QObject::tr("�����Ƶ���ѹ"),"Pascals",327650.00,10.00,QObject::tr("�����Ƶ�"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","pres.mcb.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pres"),new CElementInfo(QObject::tr("����"),"other_gauss","pres","Pressure",QObject::tr("�����ƶ���ѹ"),"Pascals",327650.00,10.00,QObject::tr("�����ƶ�"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","pres.mct.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "tcdc"),new CElementInfo(QObject::tr("����"),"other_gauss","tcdc","Total cloud cover",QObject::tr("������"),"%",3276.50,0.10,QObject::tr("���������Ϊһ��"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","tcdc.eatm.gauss.YYYY.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "air"),new CElementInfo(QObject::tr("����"),"pressure","air","Air temperature",QObject::tr("����"),"degK",477.66,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa ","144��73","17��","0-357.5, -90-90","32766","air.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "hgt"),new CElementInfo(QObject::tr("����"),"pressure","hgt","Geopotential height",QObject::tr("λ�Ƹ߶�"),"m",32066.00 ,1.00,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa ","144��73","17��","0-357.5, -90-90","32766","hgt.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "uwnd"),new CElementInfo(QObject::tr("����"),"pressure","uwnd","U-wind",QObject::tr("U�糡"),"m//s",202.66,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa ","144��73","17��","0-357.5, -90-90","32766","uwnd.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "vwnd"),new CElementInfo(QObject::tr("����"),"pressure","vwnd","V-wind",QObject::tr("U�糡"),"m//s",202.66,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa ","144��73","17��","0-357.5, -90-90","32766","vwnd.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "omega"),new CElementInfo(QObject::tr("����"),"pressure","omega","Omega",QObject::tr("��ֱ�ٶ�"),"Pascal//s",29.765,0.001,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa ","144��73","ǰ12��","0-357.5, -90-90","32766","omega.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "rhum"),new CElementInfo(QObject::tr("����"),"pressure","rhum","Relative humidity",QObject::tr("���ʪ��"),"%",302.66 ,0.01,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa ","144��73","ǰ8��","0-357.5, -90-90","32766","rhum.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "shum"),new CElementInfo(QObject::tr("����"),"pressure","shum","Specific humidity",QObject::tr("��ʪ"),"kg/kg",0.032666,1.E-06,"1000hPa, 925hPa, 850hPa, 700hPa, 600hPa, 500hPa, 400hPa, 300hPa, 250hPa, 200hPa, 150hPa, 100hPa, 70hPa, 50hPa,30hPa, 20hPa, 10 hPa ","144��73","ǰ8��","0-357.5, -90-90","32766","shum.YYYY.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "air"),new CElementInfo(QObject::tr("����"),"pressure","air","Air temperature at sigma level 995",QObject::tr("sigma=0.995������"),"degK",512.81,0.01,"sigma=0.995","144��73",QObject::tr("1��"),"0-357.5, -90-90","32766","air.sig995.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pottmp"),new CElementInfo(QObject::tr("����"),"pressure","pottmp","potential temperature at sigma level 995",QObject::tr("sigma=0.995��λ��"),"degK",3426.50 ,0.10,"sigma=0.995","144��73",QObject::tr("1��"),"0-357.5, -90-90","32766","pottmp.sig995.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "rhum"),new CElementInfo(QObject::tr("����"),"pressure","rhum","relative humidity at sigma level 995",QObject::tr("sigma=0.995�����ʪ��"),"%",302.65,0.01,"sigma=0.995","144��73",QObject::tr("1��"),"0-357.5, -90-90","32766","rhum.sig995.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "uwnd"),new CElementInfo(QObject::tr("����"),"pressure","uwnd","u-wind at sigma level 995",QObject::tr("sigma=0.995��U�糡"),"m//s",225.45,0.01,"sigma=0.995","144��73",QObject::tr("1��"),"0-357.5, -90-90","32766","uwnd.sig995.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "vwnd"),new CElementInfo(QObject::tr("����"),"pressure","vwnd","u-wind at sigma level 995",QObject::tr("sigma=0.995��V�糡"),"m//s",225.45,0.01,"sigma=0.995","144��73",QObject::tr("1��"),"0-357.5, -90-90","32766","vwnd.sig995.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "omega"),new CElementInfo(QObject::tr("����"),"pressure","omega","omega at surface",QObject::tr("���洹ֱ�ٶ�"),"Pascal//s",29.765,0.001,"sigma=0.995","144��73",QObject::tr("1��"),"0-357.5, -90-90","32766","omega.sig995.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "lftx"),new CElementInfo(QObject::tr("����"),"pressure","lftx","Surface Lifted Index",QObject::tr("����̧��ָ��"),"degK",309.65,0.01,"sigma=0.995","144��73",QObject::tr("1��"),"0-357.5, -90-90","32766","lftx.sfc.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "lftx4"),new CElementInfo(QObject::tr("����"),"pressure","lftx4","Best (4-layer) Lifted Index",QObject::tr("���̧��ָ��"),"degK",302.65,0.01,"sigma=0.995","144��73",QObject::tr("1��"),"0-357.5, -90-90","32766","lftx4.sfc.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pres"),new CElementInfo(QObject::tr("����"),"pressure","pres","Pressure at surface",QObject::tr("������ѹ"),"Pascals",367650.00,10.00,"sigma=0.995","144��73",QObject::tr("1��"),"0-357.5, -90-90","32766","pres.sfc.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pr_wtr"),new CElementInfo(QObject::tr("����"),"pressure","pr_wtr","Precipitable Water for entire atmosphere",QObject::tr("��������ɽ�ˮ��"),"kg//m^2",277.65,0.01,"sigma=0.995","144��73",QObject::tr("1��"),"0-357.5, -90-90","32766","pr_wtr.eatm.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "slp"),new CElementInfo(QObject::tr("����"),"pressure","slp","Sea Level Pressure",QObject::tr("��������ѹ"),"Pascals",119765.00,1.00,"sigma=0.995","144��73",QObject::tr("1��"),"0-357.5, -90-90","32766","slp.YYYY.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "air"),new CElementInfo(QObject::tr("����"),"surface_gauss","air","Air temperature at 2 m",QObject::tr("2������"),"degK",477.65,0.01,"2m","192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","air.2m.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "tmax"),new CElementInfo(QObject::tr("����"),"surface_gauss","tmax","Maximum Temperature at 2 m",QObject::tr("2���������"),"degK",477.65,0.01,"2m","192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","tmax.2m.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "tmin"),new CElementInfo(QObject::tr("����"),"surface_gauss","tmin","Minimum Temperature at 2 m",QObject::tr("2���������"),"degK",477.65,0.01,"2m","192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","tmin.2m.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "shum"),new CElementInfo(QObject::tr("����"),"surface_gauss","shum","Specific Humidity at 2 m",QObject::tr("2�ױ�ʪ"),"kg/kg",0.32765,1.E-05,"2m","192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","shum.2m.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "soilw"),new CElementInfo(QObject::tr("����"),"surface_gauss","soilw","Volumetric Soil Moisture between 0-10 cm Below Ground Level",QObject::tr("����0-10cm֮�䵥λ�������ʪ��"),"fraction",3.2765,0.0001,"����0-10cm","192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","soilw.0-10cm.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "soilw"),new CElementInfo(QObject::tr("����"),"surface_gauss","soilw","Volumetric Soil Moisture between 10-200 cm Below Ground Level",QObject::tr("����10-200cm֮�䵥λ���������ʪ��"),"fraction",3.2765,0.0001,"����10-200cm","192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","soilw.10-200cm.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "tmp"),new CElementInfo(QObject::tr("����"),"surface_gauss","tmp","Temperature between 0-10 cm below ground level",QObject::tr("����0-10cm֮���¶�"),"degK",477.65,0.01,"����0-10cm","192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","tmp.0-10cm.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "tmp"),new CElementInfo(QObject::tr("����"),"surface_gauss","tmp","Temperature between 10-200 cm below ground level",QObject::tr("����10-200cm֮���¶�"),"degK",477.65,0.01,"����10-200cm","192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","tmp.10-200cm.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "tmp"),new CElementInfo(QObject::tr("����"),"surface_gauss","tmp","Temperature at 300 cm below ground level",QObject::tr("����300cm�¶�"),"degK",477.65,0.01,"����300cm","192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","tmp.300cm.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "uwnd"),new CElementInfo(QObject::tr("����"),"surface_gauss","uwnd","u-wind at 10 m",QObject::tr("10��u�糡"),"m//s",225.45,0.01,"10m","192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","uwnd.10m.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "vwnd"),new CElementInfo(QObject::tr("����"),"surface_gauss","vwnd","v-wind at 10 m",QObject::tr("10��v�糡"),"m//s",225.45,0.01,"10m","192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","vwnd.10m.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "cprat"),new CElementInfo(QObject::tr("����"),"surface_gauss","cprat","Convective Precipitation Rate at surface",QObject::tr("���������ˮ��"),"Kg//m^2//s",0.0031765,1.E-07,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","cprat.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "prate"),new CElementInfo(QObject::tr("����"),"surface_gauss","prate","Precipitation Rate at surface",QObject::tr("���潵ˮ��"),"Kg//m^2//s",0.0032765,1.E-07,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","prate.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "icec"),new CElementInfo(QObject::tr("����"),"surface_gauss","icec","Ice Concentration at surface","������ܼ���","",3276.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","icec.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "cfnlf"),new CElementInfo(QObject::tr("����"),"surface_gauss","cfnlf","Cloud Forcing Net Longwave Flux at Surface",QObject::tr("������ǿ�Ⱦ�����ͨ��"),"W//m^2",3176.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","cfnlf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "csdlf"),new CElementInfo(QObject::tr("����"),"surface_gauss","csdlf","Clear Sky Downward Longwave Flux at surface",QObject::tr("����������³���ͨ��"),"W//m^2",3176.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","csdlf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "dlwrf"),new CElementInfo(QObject::tr("����"),"surface_gauss","dlwrf","Downward Longwave Radiation Flux at surface",QObject::tr("�������³�������ͨ��"),"W//m^2",3176.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","dlwrf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "ulwrf"),new CElementInfo(QObject::tr("����"),"surface_gauss","ulwrf","Upward Longwave Radiation Flux at surface",QObject::tr("�������ϳ�������ͨ��"),"W//m^2",3246.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","ulwrf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "cfnsf"),new CElementInfo(QObject::tr("����"),"surface_gauss","cfnsf","Cloud Forcing Net Solar Flux at Surface",QObject::tr("������ǿ�Ⱦ�̫������ͨ��"),"W//m^2",2376.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","cfnsf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "csdsf"),new CElementInfo(QObject::tr("����"),"surface_gauss","csdsf","Clear Sky Downward Solar Flux at surface",QObject::tr("�����������̫������ͨ��"),"W//m^2",3156.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","csdsf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "csusf"),new CElementInfo(QObject::tr("����"),"surface_gauss","csusf","Clear Sky Upward Solar Flux at surface",QObject::tr("�����������̫������ͨ��"),"W//m^2",3176.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","csusf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "dswrf"),new CElementInfo(QObject::tr("����"),"surface_gauss","dswrf","Downward Solar Radiation Flux at surface",QObject::tr("��������̫������ͨ��"),"W//m^2",3156.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","dswrf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "uswrf"),new CElementInfo(QObject::tr("����"),"surface_gauss","uswrf","Upward Solar Radiation Flux at surface",QObject::tr("��������̫������ͨ��"),"W//m^2",3176.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","uswrf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "gflux"),new CElementInfo(QObject::tr("����"),"surface_gauss","gflux","Ground Heat Flux at surface",QObject::tr("�������ͨ��"),"W//m^2",2876.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","gflux.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "lhtfl"),new CElementInfo(QObject::tr("����"),"surface_gauss","lhtfl","Latent Heat Net Flux at surface",QObject::tr("����Ǳ�Ⱦ�ͨ��"),"W//m^2",1276.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","lhtfl.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "shtfl"),new CElementInfo(QObject::tr("����"),"surface_gauss","shtfl","Sensible Heat Net Flux at surface",QObject::tr("������Ⱦ�ͨ��"),"W//m^2",30766.00,1.00,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","shtfl.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "nbdsf"),new CElementInfo(QObject::tr("����"),"surface_gauss","nbdsf","Near IR Beam Downward Solar Flux at surface",QObject::tr("��������������̫������ͨ��"),"W//m^2",3126.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","nbdsf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "nddsf"),new CElementInfo(QObject::tr("����"),"surface_gauss","nddsf","Near IR Diffuse Downward Solar Flux at surface",QObject::tr("���������ɢ������̫������ͨ��"),"W//m^2",3156.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","nddsf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "nlwrs"),new CElementInfo(QObject::tr("����"),"surface_gauss","nlwrs","Net Longwave Radiation Flux at Surface",QObject::tr("���澻��������ͨ��"),"W//m^2",3176.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","nlwrs.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "nswrs"),new CElementInfo(QObject::tr("����"),"surface_gauss","nswrs","Net Shortwave Radiation Flux at Surface",QObject::tr("���澻�̲�����ͨ��"),"W//m^2",1876.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","nswrs.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pevpr"),new CElementInfo(QObject::tr("����"),"surface_gauss","pevpr","Potential Evaporation Rate at surface",QObject::tr("����Ǳ������"),"W//m^2",2476.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","pevpr.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pres"),new CElementInfo(QObject::tr("����"),"surface_gauss","pres","Surface Pressure",QObject::tr("������ѹ"),"Pascals",367650.00,10.00,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","pres.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "runof"),new CElementInfo(QObject::tr("����"),"surface_gauss","runof","Water Runoff at surface",QObject::tr("����ˮ����"),"kg//m^2",307.66,0.01,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","runof.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "sfcr"),new CElementInfo(QObject::tr("����"),"surface_gauss","sfcr","Surface Roughness",QObject::tr("����ֲڶ�"),"m",0.032065,1.E-06,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","sfcr.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "skt"),new CElementInfo(QObject::tr("����"),"surface_gauss","skt","SST/Land Skin Temperature",QObject::tr("����/½�����¶�"),"degK",477.65,0.01,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","skt.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "uflx"),new CElementInfo(QObject::tr("����"),"surface_gauss","uflx","Momentum Flux, u-component at surface",QObject::tr("���涯��u����"),"N//m^2",28.765,0.001,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","uflx.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "vflx"),new CElementInfo(QObject::tr("����"),"surface_gauss","vflx","Momentum Flux, v-component at surface",QObject::tr("���涯��v����"),"N//m^2",28.765,0.001,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","vflx.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "ugwd"),new CElementInfo(QObject::tr("����"),"surface_gauss","ugwd","Zonal Gravity Wave Stress at surface",QObject::tr("����γ��������Ӧ��"),"N//m^2",0.765,0.001,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","ugwd.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "vgwd"),new CElementInfo(QObject::tr("����"),"surface_gauss","vgwd","Meridional Gravity Wave Stress at surface",QObject::tr("���澭��������Ӧ��"),"N//m^2",0.765,0.001,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","vgwd.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "vbdsf"),new CElementInfo(QObject::tr("����"),"surface_gauss","vbdsf","Visible Beam Downward Solar Flux at surface",QObject::tr("����ɼ�������̫������ͨ��"),"W//m^2",3126.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","vbdsf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "vddsf"),new CElementInfo(QObject::tr("����"),"surface_gauss","vddsf","Visible Diffuse Downward Solar Flux at surface",QObject::tr("����ɼ�ɢ������̫������ͨ��"),"W//m^2",3176.50,0.10,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","vddsf.sfc.gauss.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "weasd"),new CElementInfo(QObject::tr("����"),"surface_gauss","weasd","Water Equiv. of Accum. Snow Depth at surface",QObject::tr("�����ѩ��ȵ�ˮ����"),"kg//m^2",3276.50,1.00,QObject::tr("����"),"192��94",QObject::tr("1��"),"0-358.125,-88.542-88.542","32766","weasd.sfc.gauss.YYYY.nc"));

	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "air"),new CElementInfo(QObject::tr("����"),"tropopause","air","Air Temperature at Tropopause",QObject::tr("�����㶥����"),"degK",467.65,0.01,QObject::tr("�����㶥"),"144��73",QObject::tr("1��"),"0-357.5, -90-90","32766","air.tropp.YYYY.nc"));
	m_elements.insert(QPair<QString, QString>(QObject::tr("����"), "pres"),new CElementInfo(QObject::tr("����"),"tropopause","pres","Pressure at Tropopause",QObject::tr("�����㶥��ѹ"),"Pascals",327660.00,10.00,QObject::tr("�����㶥"),"144��73",QObject::tr("1��"),"0-357.5, -90-90","32766","pres.tropp.YYYY.nc"));
}

 /************************************************************************/
 /* �������ܣ� ���Ҫ����Ϣ      	      			                     */
 /* ����˵��:  time  ʱ�� name  Ӣ�Ķ���								 */
 /* �� �� ֵ�� m_elements   Ҫ�س�Ա                                     */
 /* ��    ��:  ����													     */
 /* ��    ��:  2011 �� 05 �� 09 ��										 */
 /************************************************************************/
CElementInfo* CElementFactory::GetElementInfo(QString time, QString name)
{
	QPair<QString,QString> key(time , name);
	return m_elements.value(key);
}

CElementFactory::~CElementFactory()
{
	QHash<QPair<QString, QString>, CElementInfo*>::const_iterator moditDrawType;
	//ѭ��m_htDate�õ����ϸýڵ����Ƶ����ݽṹ���ʼ���Ի���
	for( moditDrawType=m_elements.constBegin(); moditDrawType!=m_elements.constEnd(); ++moditDrawType)
	{
		delete moditDrawType.value();
	}
}

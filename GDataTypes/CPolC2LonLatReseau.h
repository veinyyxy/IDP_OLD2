///////////////////////////////////////////////////////
//CPolC2LonLatReseau.h by chenxiang 
//created by chenxiang 20080417
//for������ת��γ��
//������ת���Ȳ�γ�Ȳ�,������ת����γ��,��֪��γ����������
///////////////////////////////////////////////////////

#pragma once

#ifndef		CPOLC2LONLATRESEAU_H
#define		CPOLC2LONLATRESEAU_H

#include	"GDef.h"
class DLLEXP_GSYS CPolC2LonLatReseau
{
public:
	static bool PolC2DLonDLat(gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& DLon,gInt32& DLat,gInt32 H=0);//������ת���Ȳ�,γ�Ȳ�
	//����˵��:������Ƕ�(��λ��,��λ0.01��),������뾶(��λ��),�״����ľ���,γ��,���Ȳ�,γ�Ȳ�(��λ����0.00001��),�״�߶�H(��λ��)
	static bool PolCLonLat(gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H=0);//������ת����γ��
	//����˵��:������Ƕ�(��λ��,��λ0.01��),������뾶(��λ��),�״����ľ���,γ��,���Ȳ�,γ�Ȳ�(��λ����0.00001��),�״�߶�H(��λ��)
	//add by chenxaing 20080506~~~~~~~~~~~~~~~~~~~~~~~
	static bool SameLon2Distance(gInt32 DLat,gInt32 &DistanceY);								//ͬ�����ϲ�ͬγ���ڵ����ϵ�ʵ�ʾ���
	//����˵��:γ�Ȳ�(0.00001��),�����������(��)
	static bool SameLat2Distance(gInt32 DLon,gInt32 Lat,gInt32 &DistanceX);						//ͬγ�Ȳ�ͬ�����ڵ����ϵ�ʵ�ʾ���
	//����˵��:���Ȳ�(0.00001��),γ��(0.00001��)(!ע�����ﲻ�� γ�Ȳ�),�����������(��)	
	static bool EqualDistPolC2DLonDLat(gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& DLon,gInt32& DLat,gInt32 H=0);//�Ⱦ༫����ת��γ�Ȳ�
	static bool EqualDistPolCLonLat(gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H=0);//�Ⱦ༫����ת��γ��
	//end of add~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//add by chenxiang 20080518~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static bool QuickPolC2DLonDLat( gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& DLon,gInt32& DLat,gInt32 H );//��Ч�ļ�����ת���Ȳ�γ�Ȳ������ڵȾ���ͶӰ
	static bool QuickPolC2LonLat( gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H ); //��Ч�ļ�����ת����γ�������ڵȾ���ͶӰ
	//add by chenxiang 20080606~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static bool QuickPolC2LonLat( gInt32 Elevation,gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H ); //����ͶӰ ������ת��γ��
	//����˵��:����(0.01��),������Ƕ�(��λ��,��λ0.01��),������뾶 (��λ��) ,�״����ľ���,γ��,Ŀ��㾭��,γ�� (��λ����0.00001��) ,�״�߶�H (��λ��)
	//end of add~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//add by chenxaing 20080506~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
private:
	static gInt32 L1ToL2(gInt32 L1);//���ڵȾ�ͶӰ�õĳ���ת������ С��� ת�����L2�ٴ��뼫����ת��γ�Ⱥ��������ľ��ǵȾ�ͶӰ��ľ�γ��
//add by chenxiang 20080606~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	static gInt32 LBToLC(gInt32 LB,gInt32 Elevation,gInt32 H);//���ڵ���ͶӰ�ĳ���ת��,��cos�����ת�ɻ���
//end of add~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
};

#endif

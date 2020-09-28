///////////////////////////////////////////////////////
//CPolC2LonLatReseau.cpp by chenxiang 
//created by chenxiang 20080417
//for������ת��γ��
//������ת���Ȳ�γ�Ȳ�,������ת����γ��,��֪��γ����������
///////////////////////////////////////////////////////

#include "CPolC2LonLatReseau.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif//_USE_MATH_DEFINES
#include	<math.h>

#define REarth	6371300													//��λ��
#define e5		100000													//��γ�ȵĵ�λ����ϵ��

bool CPolC2LonLatReseau::PolC2DLonDLat( gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& DLon,gInt32& DLat,gInt32 H ) //������ת���Ȳ�,γ�Ȳ�
//����˵��:������Ƕ�(��λ��,��λ0.01��),������뾶(��λ��),�״����ľ���,γ��,���Ȳ�,γ�Ȳ�(��λ����0.00001��) ,�״�߶�H(��λ��) Ĭ�ϲ���Ĭ��Ϊ0
{
	//������ת��γ����,�������ϱ�γ85��������Ϊ��γ�Ⱦ����ܼ����ײ������
	if( Lon0 < -180 * e5 || Lon0 > 180 * e5 )							//�״����ľ������� 0�ȶ�\����180��
	{
		return false;
	}
	if( Lat0 < -85 * e5 || Lat0 > 85 * e5 )								//�״�����γ������ 0�ȵ���\��γ85��
	{
		return false;
	}
	if( A < 0 || A >= 360 * 100 )										//������Ƕȷ�Χ���� 0�ȵ�360��
	{
		return false;
	}
	A = 45000 - A;														//add by chenxiang for ���ļ�����ǶȺͷ�λ�Ƿ������ʼλ�ò�ͳһ�����ĽǶ�ת�� 20080424
	if( L < 0 || L >= 700000 )											//������뾶��Χ���� 0m��700����
	{
		return false;
	}
	gInt32 K = 0;														//�����жϼ�����Ƕ�sin��cos������//���������õ�ϵ�� ��ֹ���������������
	gInt32 sinA = gInt32( sin ( gDouble( A ) / 100 * M_PI / 180 ) * 1000000000 ) ;									//�˱���Ϊ����ֵ�Ƚ���
	( ( sinA > 1 ) || ( sinA < -1 ) ) ? K = sinA / gInt32( fabs( gDouble( sinA ) ) ) : K = 0;						//?ǰ���Ǹ���sinA����ֵ�Ƚ�

	DLat = gInt32( e5*atan( L * sin( gDouble( A ) / 100 * M_PI / 180 ) / ( REarth + H ) ) * 180 / M_PI + K * 0.5 );	//����γ�Ȳ�
	gInt32 cosA = gInt32( cos( gDouble( A ) / 100 * M_PI / 180 ) * 1000000000 );									//��1000000000��Ϊ�˱�Ϊ���ͺ���ֵ�Ƚ���
	( ( cosA > 1 ) || ( cosA < -1 ) ) ? K = cosA / gInt32( fabs( gDouble( cosA ) ) ) : K = 0;						//?ǰ���Ǹ���cosA����ֵ�Ƚ�

	DLon = gInt32( e5 * ( 180 / M_PI ) * 2 * asin( 2 * sin( atan( gDouble( L ) / ( REarth + H ) ) /2 ) * cos( gDouble( A ) / 100 * M_PI / 180 ) 
		/ ( cos( gDouble( Lat0 ) * M_PI / 180 / e5 ) + cos( ( gDouble( Lat0 ) + DLat ) * M_PI / 180 / e5 ) ) ) + K * 0.5 ) ;//���㾭�Ȳ�
	if( ( Lat0 + DLat ) < -89 * e5 || ( Lat0 + DLat ) > 89 * e5 )		//���Ŀ��γ�ȳ�����Χ�򷵻�false
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool CPolC2LonLatReseau::PolCLonLat( gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H ) //������ת����γ��
//����˵��:������Ƕ�(��λ��,��λ0.01��),������뾶 (��λ��) ,�״����ľ���,γ��,Ŀ��㾭��,γ�� (��λ����0.00001��) ,�״�߶�H (��λ��) Ĭ�ϲ���Ĭ��Ϊ0
{
	gInt32 DLon = 0,DLat = 0;//��ʼ�����Ȳ��γ�Ȳ�
	if ( !PolC2DLonDLat( A,L,Lon0,Lat0,DLon,DLat,H ) )					//�������ĺϷ��Ժ� ��þ��Ȳ�γ�Ȳ�
	{
		return false;
	}
	Lon1 = ( Lon0+DLon ) ;
	if( Lon1 > 180 * e5 )												//����180��
	{
		Lon1 -= 360 * e5;												//����Խ�� ת������
	}
	if( Lon1 < -180 * e5 )												//С��-180��
	{
		Lon1 += 360 * e5;												//����Խ�� ת�ɶ���
	}
	Lat1 = Lat0 + DLat;
	return true;
}
//add by chenxaing 20080506~~~~~~~~~~~~~~~~~~~~~~~
bool CPolC2LonLatReseau::SameLon2Distance( gInt32 DLat,gInt32 &DistanceY )//ͬ�����ϲ�ͬγ���ڵ����ϵ�ʵ�ʾ���
{//����˵��:γ�Ȳ�(0.00001��),�����������(��)
	if( DLat < -85 * e5 || DLat > 85 * e5 )								//�״�����γ�Ȳ����� 0�ȵ���\��γ85��
	{
		return false;
	}
	DistanceY = gInt32 ( REarth * DLat * M_PI / 180 / e5 +0.5);
	return true;
}
bool CPolC2LonLatReseau::SameLat2Distance( gInt32 DLon,gInt32 Lat,gInt32 &DistanceX )//ͬγ�Ȳ�ͬ�����ڵ����ϵ�ʵ�ʾ���
{//����˵��:���Ȳ�(0.00001��),γ��(0.00001��)(!ע�����ﲻ�� γ�Ȳ�),�����������(��)
	if( DLon < -180 * e5 || DLon > 180 * e5 )							//�״����ľ��Ȳ����� 0�ȵ�������180��
	{
		return false;
	}
	DistanceX = gInt32 ( REarth * cos( gDouble( Lat * M_PI / 180 /e5 ) ) * DLon * M_PI / 180 / e5 +0.5);
	return true;
}
gInt32 CPolC2LonLatReseau::L1ToL2( gInt32 L1 )//���ڵȾ�ͶӰ�õĳ���ת������ С��� ת�����L2�ٴ��뼫����ת��γ�Ⱥ��������ľ��ǵȾ�ͶӰ��ľ�γ��
{
	gInt32 L2=gInt32( REarth * tan( gDouble( L1 ) / REarth  ) );
	return L2;															//ת����ľ���
}
bool CPolC2LonLatReseau::EqualDistPolC2DLonDLat(gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& DLon,gInt32& DLat,gInt32 H)//�Ⱦ�ͶӰ������ת��γ�Ȳ�
{//����˵��:������Ƕ�(��λ��,��λ0.01��),������뾶(��λ��),�״����ľ���,γ��,���Ȳ�,γ�Ȳ�(��λ����0.00001��) ,�״�߶�H(��λ��) Ĭ�ϲ���Ĭ��Ϊ0
	return PolC2DLonDLat( A, L1ToL2(L), Lon0, Lat0, DLon, DLat, H );
}
bool CPolC2LonLatReseau::EqualDistPolCLonLat(gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H)//�Ⱦ�ͶӰ������ת��γ��
{//����˵��:������Ƕ�(��λ��,��λ0.01��) ,������뾶 (��λ��) ,�״����ľ���,γ��,Ŀ��㾭��,γ�� (��λ����0.00001��) ,�״�߶�H (��λ��) Ĭ�ϲ���Ĭ��Ϊ0
	return PolCLonLat( A, L1ToL2(L), Lon0, Lat0, Lon1, Lat1, H );
}
// end of add~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//add by chenxiang 20080518~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Ϊ���Ч�ʶ��޸ĵļ�����ת��γ�Ⱥ��� //�����ڵȾ���ͶӰ ����ͶӰ
bool CPolC2LonLatReseau::QuickPolC2DLonDLat( gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& DLon,gInt32& DLat,gInt32 H ) //������ת����γ��
//����˵��:������Ƕ�(��λ��,��λ0.01��),������뾶 (��λ��) ,�״����ľ���,γ��,Ŀ��㾭��,γ�� (��λ����0.00001��) ,�״�߶�H (��λ��)
{
	//������ת��γ����,�������ϱ�γ85��������Ϊ��γ�Ⱦ����ܼ����ײ������
	if( Lon0 < -180 * e5 || Lon0 > 180 * e5 || Lat0 < -85 * e5 || Lat0 > 85 * e5)
	{																	//�״����ľ������� 0�ȶ�\����180��//�״�����γ������ 0�ȵ���\��γ85��
		return false;
	}
	if( A < 0 || A >= 360 * 100 )										//������Ƕȷ�Χ���� 0�ȵ�360��
	{
		return false;
	}
	gInt32 A2 = 45000 - A;												//���ļ�����ǶȺͷ�λ�Ƿ������ʼλ�ò�ͳһ�����ĽǶ�ת��
	if( L < 0 || L >= 700000 )											//������뾶��Χ���� 0m��700����
	{
		return false;
	}
	gDouble Angle2Radian = M_PI / 180;									//�Ƕ�ת���� Ҫ�������ϵ��
	gInt8 K = 1;														//��������ϵ��
	( A >= 0 && A <= 18000 ) ? K=1 : K=-1;//sin>0
	DLat = gInt32( e5*( L * sin( gDouble( A2 ) / 100 * Angle2Radian ) / ( REarth + H ) ) / Angle2Radian + K * 0.5 );//����γ�Ȳ�
	( ( A >= 0 && A <= 9000 ) || ( A >= 27000 && A < 36000 ) ) ? K=1 : K=-1;//cos>0
	DLon = gInt32(e5 / Angle2Radian * 2 * asin( 2 * sin( gDouble( L ) / ( REarth + H ) / 2 ) * cos( gDouble( A2 ) / 100 * Angle2Radian )
		/ ( cos( gDouble( Lat0 ) * Angle2Radian / e5 ) + cos( ( gDouble( Lat0 ) + DLat ) * Angle2Radian / e5 ) ) ) + K * 0.5);//���㾭�Ȳ�

	if( ( Lat0 + DLat ) < -89 * e5 || ( Lat0 + DLat ) > 89 * e5 )		//���Ŀ��γ�ȳ�����Χ�򷵻�false
	{
		return false;
	}
	else
	{
		return true;
	}
}
bool CPolC2LonLatReseau::QuickPolC2LonLat( gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H ) //��Ч�ļ�����ת����γ�������ڵȾ���ͶӰ
//����˵��:������Ƕ�(��λ��,��λ0.01��),������뾶 (��λ��) ,�״����ľ���,γ��,Ŀ��㾭��,γ�� (��λ����0.00001��) ,�״�߶�H (��λ��) 
{
	gInt32 DLon = 0,DLat = 0;											//��ʼ�����Ȳ��γ�Ȳ�
	if ( !QuickPolC2DLonDLat( A,L,Lon0,Lat0,DLon,DLat,H ) )				//�������ĺϷ��Ժ� ��þ��Ȳ�γ�Ȳ�
	{
		return false;
	}
	Lon1 = ( Lon0+DLon ) ;
	if( Lon1 > 180 * e5 )												//����180��
	{
		Lon1 -= 360 * e5;												//����Խ�� ת������
	}
	if( Lon1 < -180 * e5 )												//С��-180��
	{
		Lon1 += 360 * e5;												//����Խ�� ת�ɶ���
	}
	Lat1 = Lat0 + DLat;
	return true;
}
// end of add~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//add by chenxiang 20080606~~~~~~~~~~~~~~~~~~~~~~~~~~~~
gInt32 CPolC2LonLatReseau::LBToLC(gInt32 LB,gInt32 Elevation,gInt32 H)////���ڵ���ͶӰ�ĳ���ת��,��cos�����ת�ɻ���
{//����˵�� :ת��ǰ�ľ���(cosɨ�����)(��),����(0.01��),�״�߶�(��)
	//gInt32 LC = gInt32( REarth * tan( gDouble( L1 ) / REarth  ) );
	gFloat E =Elevation * gFloat(M_PI / 18000.0f);
	gInt32 LC = gInt32( (REarth+H) * atan( LB * cos( E ) / ( LB * sin( E ) + ( REarth + H ) * cos( E ) ) ) +0.5);
	return LC;															//ת����ľ���
}
bool CPolC2LonLatReseau::QuickPolC2LonLat( gInt32 Elevation,gInt32 A,gInt32 L,gInt32 Lon0,gInt32 Lat0,gInt32& Lon1,gInt32& Lat1,gInt32 H ) //����ͶӰ ������ת��γ��
{//����˵��:����(0.01��),������Ƕ�(��λ��,��λ0.01��),������뾶 (��λ��) ,�״����ľ���,γ��,Ŀ��㾭��,γ�� (��λ����0.00001��) ,�״�߶�H (��λ��)
	if (Elevation < 0 || Elevation >= 9000)
	{
		return false;
	}
	return QuickPolC2LonLat( A, LBToLC( L,Elevation,H), Lon0, Lat0, Lon1, Lat1, H );
}
//end of add~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

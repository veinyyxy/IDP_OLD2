#pragma once

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <QList>
#include <QPoint>
#include "goGeometry.h"
#include "../GDataTypes/cDrawProduct.h"
#include "SingleDataProvider.h"
#include "CoordinateTransform.h"
//体扫的PPI数据结构
const int nazim=512;
const int ngate=1000;
#define SITE_NAME_LEN       255						//站点名称长度(字符)
#define SITE_TYPE_LEN		255						//雷达数据格式类型名长度(字符)
#define DATA_TYPE_LEN	    255

#define VALUE_INVALID		-9900 
#define VALUE_RANFOLD		-9901 
#define VALUE_CLUTTER		-9902
#define VALUE_CLEARAIR		-9903
#define VALUE_SEACLUTTER	-9904
#define VALUE_ROSEINVALID   -32767

#define CODE_INVALID	0
#define CODE_RANFOLD	1
#define CODE_NO_OBSERV	2
#define CODE_CSRED		3 
#define MAX_HGT_LEVELS		40
#define KM_TO_DEG          0.01

;

#ifndef Max
#define Max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef Min
#define Min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

struct PPISTRUCT
{
	char	radar_name[SITE_NAME_LEN];//baishan
	char   data_type[SITE_TYPE_LEN];//sa sb cc
	char   data_name[DATA_TYPE_LEN];//ref
	//     short int	vcpnum;
	short int 	year;//nian
	short int	month;//yue
	short int	day;//ri
	short int	hour;//xiaoshi 
	short int	minute;//fenzhong
	short int	second;//miao
	int	    radlat;		//*1000//lei da zhan dian wei du
	int	    radlon;		//*1000// jing du
	short int	radhgt;		// meters hai ba gao du
	short int	elev_angle;	//*10 //ceng yang jiao
	short int	fstgatdis;	//*10 // di yi ge dian ju li
	short int	nyq_vel;	//*10 // zui da sudu 
	short int	num_beam;   // jing xiang ge shu
	short int	num_gate;  //ku shu
	short int	gateWidth;//[nazim];  //m //ku kuan
	short int	elev[nazim];	//*10 //jing xiang yang jiao
	short int	azim[nazim];	//*10 // fang wei jiao
	short int	field[nazim][ngate];  //*10 shujuqu
} ;//PPISTRUCT;
using namespace goto_gis;

typedef enum
{
	NOTVALID = 0,
	LOWVALID = 1,
	UPVALID =2,
	DOUBLEVALID =3
}WHATELEVALID;
class InterpolationAlgorithm
{
public:
	InterpolationAlgorithm();
	~InterpolationAlgorithm();
public:
	inline void setcDrawProduct(cDrawProduct * p ){m_produceDraw = p;DataPrepar();};
	void PolarToDescartes(short nCuts, struct PPISTRUCT *strct_Data,
		short x_Num, float x_Dlt, short y_Num, float y_Dlt, short z_Num, float z_Dlt, float z_Start,
		unsigned char ***outData,
		float radLon, float radLat, float radHgt);
	void DrawImge(QVector<QImage>* imageList,QHash<int ,QVector4D>colorHash);
	void Cutting(osg::Vec3& pp1, osg::Vec3& pp2, osg::Vec3& pp3, osg::Vec3& pp4, osg::Node* pClipNode,QHash<int ,QVector4D>colorHashcolorHash,goto_gis::CoordinateTransform* proj_earth);
private:
	void DataPrepar();
	void coordTransform(osg::Vec3& pp1_in, osg::Vec3& pp2_in,osg::Vec3& pp1_out, osg::Vec3& pp2_out );//将剖面线经纬度转化为雷达覆盖范围的仰角方位角 距离库数
	double getAngle(osg::Vec3 pSrc, osg::Vec3 p1, osg::Vec3 p2);//获得剖面线端点
	double GetAzimuth(osg::Vec3 p);//获得方位角
	int GetElevationNumMap(double ele);//ele 输入角度（不接受弧度）
	double Getdistans(osg::Vec3 p1, osg::Vec3 p2);
	void InitGeometry();

private:
	//PPISTRUCT *struct_Data ;
	osg::Vec3 topleft ;
	osg::Vec3 bottomright;
	osg::Vec3 circlePoint;//圆心平面坐标
	cDrawProduct *m_produceDraw;
	int m_eleCnt;   
	int radialNumPerLayer;
	int numOfGates    ; 
	gInt16	LengthOfGate;
	//float m_elevation[20];
	QMap<int,float> elevationMap;
	int m_xNum;
	int m_yNum;
	int m_zNum;
	int layerHight;
	float hhight;
	unsigned short iHeights[MAX_HGT_LEVELS];
	//unsigned char ***outData;
	gInt16 *m_outData;
	gInt16 ***m_poutData;

	osg::Geometry* pGeom;
	osg::Vec3Array* pVertexArray;
	osg::Vec2Array* texcoords;
	osg::Vec4Array* pColorArray;
	osg::StateSet* pStateSet;
	osg::Texture2D* pTexture2D;
};











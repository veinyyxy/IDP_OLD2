#pragma once

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <QList>
#include <QPoint>
#include "CoordinateTransform.h"
#include "../GDataTypes/SingleProductBase.h"
#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDataFiles.h"
#include "../GDataTypes/cDrawProduct.h"
#include "RadReadColor_XML.h"
#include "RadProjTransform.h"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkVolume.h"
#include <vtkLookupTable.h>
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
class InterpolationAlgorithm
{
public:
	InterpolationAlgorithm();
	~InterpolationAlgorithm();
public:
	inline void setcDrawProduct(cDrawProduct * p ){m_produceDraw = p;};
	vtkImageData * GetVolumeData(QHash<int ,QVector4D>colorHash);
private:	
	void PolarToDescartes(short nCuts, struct PPISTRUCT *strct_Data,
				short x_Num, float x_Dlt, short y_Num, float y_Dlt, short z_Num, float z_Dlt, float z_Start,
				unsigned char ***outData,
				float radLon, float radLat, float radHgt);

private:
	PPISTRUCT *struct_Data ;
	cDrawProduct *m_produceDraw;   
	int m_xNum;
	int m_yNum;
	int m_zNum;
	unsigned short iHeights[MAX_HGT_LEVELS];
	gInt16 *m_outData;
};











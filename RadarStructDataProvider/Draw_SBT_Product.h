/////////////////////////////////////////////////////
// class for drawing  vwp procuct
//create by youruibang
////////////////////////////////////////////////////
#pragma once
#include "RadarStructProductionDraw.h"
#include "radarstructdataprovider_global.h"
//#include "QDataArray.h"
//add by ljg 20080826 for new interface for draw.
//class CProductStructDraw_SBT : public CProductDraw
#define PI 3.1415927
using namespace goto_gis;
class  CProductStructDraw_SBT :
	public RadarStructProductionDraw
{
public:
	CProductStructDraw_SBT();
	virtual ~CProductStructDraw_SBT();
public:
	STRUCT_DATA_TYPE GetTypeOfProduct();//判断产品绘图类型
	/*virtual int GetCountOfLayer(CProductRead* lpProduct = NULL);//判断产品绘图数量，共几张图*/
	virtual bool DumpAll(Layer* layer,CProductRead* lpProduct, 
		/*CProductDrawParam* lpParam,*/P_COLORTABLE_VECTOR pColorTable,QString filename,int requestLayer/* = 0*/);//画产品图片，指定某张图片
	//virtual bool DrawLayer(wxBitmap* lpBitmap,CProductRead* lpProduct, CProductDrawParam* lpParam,int requestLayer/* = 0*/,int productnum,double ratio);
	//int	 xWorld2DC(double x, double y);       // 经度转化为屏幕坐标  产品叠加时根据要叠加到的产品选择投影算法
	//int	 yWorld2DC(double y,double x);        // 纬度转化为屏幕坐标
	//int	 xWorld2DC2(double x, double y);      // 经度转化为屏幕坐标，   注：只是用CR的投影方法来投影
	//int	 yWorld2DC2(double y,double x);       // 纬度转化为屏幕坐标
	void CreatAttributeList(Layer* layer,CProductRead * pProductData,QString fileName);
	void GetStationNameAndPName(QString &fileName,QString &stationName,
		QString &productName);
	QStringList FileNameAnalyes( QString fName );
	double		Get_Length(double dx, double dy);
	QImage DrawThumbs(CProductRead*,P_COLORTABLE_VECTOR,float);
	float GetRadius(CProductRead*);
	void GetRect(QRectF &rect );
private:
	void DrawCR(Layer *layer,CProductRead* lpProduct, 
		/*CProductDrawParam* lpParam,*/int requestLayer/* = 0*/);
	//void DrawCR(wxBitmap* lpBitmap,CProductRead* lpProduct, CProductDrawParam* lpParam,int requestLayer = 0);//add by ljg 200907224 for SBT中增加CR数据。
	double m_CenterLon,m_CenterLat,m_CenterAltitude;
	double m_ProductNumber;
	int    m_BitmapWidth,m_BitmapHeight;
	int    m_ProductNO;
	double m_Elevation;
	double m_Resolution;
	double m_nPixelPerLat;
	double lon;
	double lat;
	double iCenterLon;
	double iCenterLat;
	double piclat;
	double piclon;
	float maxdis;
};

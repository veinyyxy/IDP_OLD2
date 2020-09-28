/////////////////////////////////////////////////////
// class for drawing  vwp procuct
//create by youruibang
////////////////////////////////////////////////////
#pragma once
#include "RadarStructProductionDraw.h"
#include "radarstructdataprovider_global.h"

using namespace goto_gis;

//#include "QDataArray.h"
//#include "GPM.h"
//#include "ProductDraw.h"//add by ljg 20080826 for new interface for draw.
//#include <wx/bitmap.h>
class  Draw_VWP_Product :public RadarStructProductionDraw
{
public:
	Draw_VWP_Product(void);
	~Draw_VWP_Product(void);
	//void Get_Site_ID(QString &strTime, CProductRead *lpProduct); //վ����Q
	//void Get_Scan_Date(QString &szDate, int iProductDate); //��ɨ����
	void Get_Scan_Time(int &hour, int &minute, int iProductTime); //��ɨʱ��
	bool MilliSecond2Time( gInt32 MilliSecond,gInt32& T_Hour,gInt32& T_Minute,gInt32& T_Second,gInt32& T_MilliSecond );
	void Get_Time_Abscissa(QStringList &strTime, int hour, int minute); //������ʱ��

	void Draw_VWP_Background(Layer* layer,QStringList strTime,int iAbscissa[11]/*wxBitmap &bmp,wxArrayString strTime,int iAbscissa[11]*/);//��vwp���� 
	void Draw_VWP_WindFeather(Layer* layer,void *p,/*wxBitmap &bmp,*/ int NumbersOfStruct,P_COLORTABLE_VECTOR pColorTable/*const wxColour colorRibbon[],*//*float cn[][2],int memberofcn,*/,int iAbscissa);//������
	virtual int GetCountOfLayer(CProductRead* lpProduct = NULL);//�жϲ�Ʒ��ͼ������������ͼ
	virtual bool DumpAll(Layer* layer,CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,QString filename,int requestLayer = 0);//����ƷͼƬ��ָ��ĳ��ͼƬ
	bool DrawVwpVer0(Layer* layer,/*wxBitmap* lpBitmap,*/P_COLORTABLE_VECTOR pColorTable,CProductRead* lpProduct,/* CProductDrawParam* lpParam,*/int requestLayer = 0);//add by ljg 20090727 for ��VWP��
	void CreatAttributeList(Layer* layer, CProductRead * pProductData,QString fileName);
	QImage DrawThumbs(CProductRead*,P_COLORTABLE_VECTOR,float);
	STRUCT_DATA_TYPE GetTypeOfProduct();//�жϲ�Ʒ��ͼ����
	//float GetRadius(CProductRead*){return 0.0;};
	void GetStationNameAndPName(QString &fileName,QString &stationName,QString &productName);
	QStringList FileNameAnalyes( QString fName );
	osg::Drawable* DrawText(const std::string& text,osg::Vec3 positionOffset,float size);
	
public:
	QString*        m_arrayNum;
	QString*        m_arrayCHName;
	QString*        m_arrayENGName;
	float Max_V;  //��¼�ٶ����ֵm/s
	//CProductDrawType GetTypeOfProduct();//�жϲ�Ʒ��ͼ����
	
};

//add by ljg 20080826 for new interface for draw.
//class CProductStructDraw_VWP : public CProductDraw
//{
//public:
//	CProductStructDraw_VWP();
//	virtual ~CProductStructDraw_VWP();
//public:
//	QString*        m_arrayNum;
//	QString*        m_arrayCHName;
//	QString*        m_arrayENGName;
//	CProductDrawType GetTypeOfProduct();//�жϲ�Ʒ��ͼ����
//	virtual int GetCountOfLayer(CProductRead* lpProduct = NULL);//�жϲ�Ʒ��ͼ������������ͼ
//	virtual bool DumpAll(QRenderable* container,P_COLORTABLE_VECTOR pColorTable,/*wxBitmap* lpBitmap,*/CProductRead* lpProduct,/* CProductDrawParam* lpParam,*/int requestLayer = 0);//����ƷͼƬ��ָ��ĳ��ͼƬ
//	bool DrawVwpVer0(QRenderable* container,/*wxBitmap* lpBitmap,*/P_COLORTABLE_VECTOR pColorTable,CProductRead* lpProduct,/* CProductDrawParam* lpParam,*/int requestLayer = 0);//add by ljg 20090727 for ��VWP��
//	//bool DrawVwpVer1(P_COLORTABLE_VECTOR pColorTable/*wxBitmap* lpBitmap*/,CProductRead* lpProduct,/* CProductDrawParam* lpParam,*/int requestLayer = 0);//add by ljg 20090727 for ��VWP��
//};

#pragma once

#include "RadarStructProductionDraw.h"
#include "radarstructdataprovider_global.h"


#define PI 3.1415927
using namespace goto_gis;

class CProductStructDraw_LRM : public RadarStructProductionDraw
{
public:
	CProductStructDraw_LRM();
	virtual ~CProductStructDraw_LRM();

public:
	STRUCT_DATA_TYPE GetTypeOfProduct();//判断产品绘图类型
	virtual QImage DrawThumbs(CProductRead*,P_COLORTABLE_VECTOR,float);
	virtual bool      DumpAll(Layer* layer ,CProductRead* lpProduct, /* CProductDrawParam* lpParam,*/ P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer = 0);//画产品图片，指定某张图片
	osg::Drawable* DrawText(const std::string& text,osg::Vec3 positionOffset,float size);
private:	
	int				GetOrdinate(int i, int ii);
	int				GetAbscissa(int i, int ii, int jj);
	void			GetInitValue(CProductRead* lpProduct);
	void			GetLessPoint(osg::Vec2 pnt[4]);
	void			GetMorePoint(osg::Vec2 pnt[4]);
	void			GetPointAbscissa(osg::Vec2 pnt[4]);
	void			GetPointOrdinate(osg::Vec2 pnt[4]);

private:
	int				m_iScaleOfData;		//缩放值
	int				m_iNumberOfLayers;	//层数
	int				m_iNumberOfBlocks;	//块数
	int				m_iNumberOfRows;	//行数
	int				m_iNumberOfCols;	//列数	
	int				m_iHeightOfLayer;	//每层的间距
	float			m_fLeft;			//每行左移的宽度
	float			m_fWidth;			//每小格的宽度
	float			m_fHeight;			//每行的高度
	bool			m_bLessOrMore;
};

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
	STRUCT_DATA_TYPE GetTypeOfProduct();//�жϲ�Ʒ��ͼ����
	virtual QImage DrawThumbs(CProductRead*,P_COLORTABLE_VECTOR,float);
	virtual bool      DumpAll(Layer* layer ,CProductRead* lpProduct, /* CProductDrawParam* lpParam,*/ P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer = 0);//����ƷͼƬ��ָ��ĳ��ͼƬ
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
	int				m_iScaleOfData;		//����ֵ
	int				m_iNumberOfLayers;	//����
	int				m_iNumberOfBlocks;	//����
	int				m_iNumberOfRows;	//����
	int				m_iNumberOfCols;	//����	
	int				m_iHeightOfLayer;	//ÿ��ļ��
	float			m_fLeft;			//ÿ�����ƵĿ��
	float			m_fWidth;			//ÿС��Ŀ��
	float			m_fHeight;			//ÿ�еĸ߶�
	bool			m_bLessOrMore;
};

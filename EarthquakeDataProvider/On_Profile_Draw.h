#pragma once
#include "earthquakeproductiondraw.h"
#include <QStringList>
#include <QVector>
#include <QtGui/QImage>
#include "helper.h"
#include "fmdata.h"
#include <QtGui/QColor>
#include "ContourTracer.h"

using namespace goto_gis ;
using namespace FMDATA;

typedef std::vector<QColor> ColorVector;

inline double Min_(double a, double b)
{
	return (a<b) ? a : b;
}

inline bool isPracticallyZero(double a, double b = 0)
{
	if (!b)
		return (fabs (a) <=  DBL_MIN);	

	return (fabs (a - b) <= Min_(fabs(a), fabs(b))*DBL_EPSILON);	
}


class On_Profile_Draw :public EQProductionDraw
{
public:
	On_Profile_Draw(void);
	~On_Profile_Draw(void);
	virtual EARTHQUAKE_DATA_TYPE GetTypeOfProduct();//�жϲ�Ʒ��ͼ����
	virtual void LoadFile(const QString& filename);
	virtual bool DumpAll(Layer* layer,P_E_COLORTABLE_VECTOR pColorTable = 0);//����ƷͼƬ��ָ��ĳ��ͼƬ
	virtual void LonLatToGL(Layer* layer, osg::Vec3& in, osg::Vec3& out);
	//add by zhanglei below
	/** @brief ��������*/
	osg::Geode* createProfile(Layer* layer,const osg::Vec3& origin,const osg::Vec3& last);

	void SaveProfileToQImage();
	/** @brief ����ɫ��*/
	bool openDynamicColorMap(ColorVector& cv, QString fname);
	/** @brief ����Ƭ���ݱ���ΪͼƬ*/
	//void SilceToQImage(string slicetype,float fvalue);
	QColor ValueToPixel(float fmin,float fmax,float fvalue);
	
	bool readXYZfile(const QString& filename);

	bool readAscFile(const QString& filename);

	osg::Vec3 LonLat2GL(Layer* layer, osg::Vec3& in,float zshift=0.0f);

	/** @brief ����ĳֵ�ĵ�ֵ�ߣ����ص�ֵ��*/
	void SearchContour(const ProfileData& data,CONTOURLEVELS levels);
	
	osg::Geode* createContourDrawable(Layer* layer);

private:
//	VGrids vgrids;
	ProfileData profiledata;
	ColorVector  m_cv;
	QString m_fileName;
	ContourTracer m_contourtracer;
	vector<osg::Vec3Array*> m_ContourlinestripArray;
};


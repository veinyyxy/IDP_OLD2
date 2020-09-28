#pragma once
#include <math.h>
#include <osg/Array>

#include "DataProvider.h"
#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDef.h"
#include "../GDataTypes/cDrawProduct.h"
#include "radarsingledataprovider.h"

class LrmAlg
{
public:
	LrmAlg(void);
	~LrmAlg(void);

	void SetRectPoint(float x1, float y1, float x2, float y2, float fx1, float fy1, float fx2, float fy2);
	void SetLayerList(const QStringList& strList);
	void SetImageList(QVector<QImage>* pList);
	void Initialize(cDrawProduct* pData, P_COLORTABLE_VECTOR pColorTable);			//初始化对象
	void Execute();//执行算法
	double Get_Length(double dx, double dy);
	void Swap(float &m_small, float &m_big);
	void transcoor(int &tsx,int &tsy);//将按照雷达坐标为十字坐标原点计算出来的坐标转换为以框住的四边形右上点为原点Y轴向下为正的十字坐标系.....                这话写出来自己都乱
	void ResultToImage(QImage& image);
	void DrawLrm( osg::Vec4Array& m_p4DArray,int m_lay,int m_x_normal,int m_y_normal,int m_x_long,int m_y_long,P_COLORTABLE_VECTOR m_ColorTable ,QList<int> m_highlay);
private:
	cDrawProduct *m_produceDraw;
	QStringList m_HighOfLay;
	osg::Vec3Array *m_Point;
	osg::Vec4Array* m_Output_Poitn;
	int       m_eleCnt;  //仰角层数
	int       m_azimuthNum;  //方位角
	int       m_count_gate; //距离库数
	int       m_len_gate;  //距离库长度
	int		  m_radarHeight;  //雷达高度，08.01
	int		  num_of_req_H;//请求的横剖面数量
	int		  x_long,y_long;//xy方向的坐标数
	int       x_normal,y_normal;//xy的基准坐标，就是左下点
	int       x_coor,y_coor;//xy的每个点坐标
	QList<int>LayerHight;
	P_COLORTABLE_VECTOR m_color;
	QVector<QImage>* m_LayerPixmap;
};


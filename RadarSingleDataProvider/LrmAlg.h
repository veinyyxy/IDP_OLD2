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
	void Initialize(cDrawProduct* pData, P_COLORTABLE_VECTOR pColorTable);			//��ʼ������
	void Execute();//ִ���㷨
	double Get_Length(double dx, double dy);
	void Swap(float &m_small, float &m_big);
	void transcoor(int &tsx,int &tsy);//�������״�����Ϊʮ������ԭ��������������ת��Ϊ�Կ�ס���ı������ϵ�Ϊԭ��Y������Ϊ����ʮ������ϵ.....                �⻰д�����Լ�����
	void ResultToImage(QImage& image);
	void DrawLrm( osg::Vec4Array& m_p4DArray,int m_lay,int m_x_normal,int m_y_normal,int m_x_long,int m_y_long,P_COLORTABLE_VECTOR m_ColorTable ,QList<int> m_highlay);
private:
	cDrawProduct *m_produceDraw;
	QStringList m_HighOfLay;
	osg::Vec3Array *m_Point;
	osg::Vec4Array* m_Output_Poitn;
	int       m_eleCnt;  //���ǲ���
	int       m_azimuthNum;  //��λ��
	int       m_count_gate; //�������
	int       m_len_gate;  //����ⳤ��
	int		  m_radarHeight;  //�״�߶ȣ�08.01
	int		  num_of_req_H;//����ĺ���������
	int		  x_long,y_long;//xy�����������
	int       x_normal,y_normal;//xy�Ļ�׼���꣬�������µ�
	int       x_coor,y_coor;//xy��ÿ��������
	QList<int>LayerHight;
	P_COLORTABLE_VECTOR m_color;
	QVector<QImage>* m_LayerPixmap;
};


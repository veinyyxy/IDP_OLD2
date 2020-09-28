#pragma once
#include <list>
using namespace std;

const float Epsilon = 0.000001f;     //����������еĸ������Ƚϵľ���
const float Excursion = 0.0001f;     //����ֵ

struct POS3D
{
	double x,y,z;
};

//��ֵ����Ϣ
struct IsoPoint
{
	int _column;         //��������е����ֵҪ��_rows _columnsС1
	int _row;            //����������ǰ������ߣ�ע�����ߣ������

	float _x;
	float _y;

	bool _isHorizon;     //��ֵ���Ƿ���X���ϣ�ˮƽ���ϣ�   true--X   false--Y
};

struct IsoPointListInfo     //��ֵ�ߵ���Ϣ
{
	vector<IsoPoint> _aIsoPointList;
	float _value;     //ָʾ��ǰ��ֵ�ߵ�ֵ
};

//ÿ�����ϵ���Ϣ�����޵�ֵ�㣬��ʱ���ںδ���_rate��
struct EdgeIsoInfo         
{
	float _rate;
	bool _isIsoPoint;   //�ڴ˱����Ƿ��е�ֵ��
};

struct GridInfo
{
	int _rows;
	int _columns;

	float _xMin;
	float _xMax;
	float _yMin;
	float _yMax;
	float _zMin;
	float _zMax;
};

//��������ݽṹ
typedef float GridPoint ;
/*
struct GridPoint
{
	float _x;
	float _y;
	float _z;
};
*/



typedef vector<float> CONTOURLEVELS;
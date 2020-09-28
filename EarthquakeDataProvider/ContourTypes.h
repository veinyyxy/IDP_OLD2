#pragma once
#include <list>
using namespace std;

const float Epsilon = 0.000001f;     //在这个程序中的浮点数比较的精度
const float Excursion = 0.0001f;     //修正值

struct POS3D
{
	double x,y,z;
};

//等值点信息
struct IsoPoint
{
	int _column;         //这里的行列的最大值要比_rows _columns小1
	int _row;            //这里的行列是按行列线（注意是线）来算的

	float _x;
	float _y;

	bool _isHorizon;     //等值点是否在X轴上（水平线上）   true--X   false--Y
};

struct IsoPointListInfo     //等值线的信息
{
	vector<IsoPoint> _aIsoPointList;
	float _value;     //指示当前等值线的值
};

//每条边上的信息（有无等值点，有时点在何处（_rate）
struct EdgeIsoInfo         
{
	float _rate;
	bool _isIsoPoint;   //在此边上是否有等值点
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

//网格点数据结构
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
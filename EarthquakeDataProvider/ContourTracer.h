#pragma once
#include <list>
#include <vector>
#include <osg/Vec3f>
#include <osg/Array>

#include "ContourTypes.h"
#include "fmdata.h"

using namespace std;
using namespace FMDATA;
using namespace osg;

class ContourTracer
{
public:
	ContourTracer() {};
	~ContourTracer() {};
	//一系列得到等值线操作的启动器
	void BeginSeriesAction();

	void SetGrid(const ProfileData& data);	
	
	void SetContourLevels(CONTOURLEVELS  levels) {_contourlevels = levels;}

	vector<osg::Vec3Array*> ToOSGVecArray();

	//list<IsoPointListInfo>* GetContourLines() {return &_allIsoPointList;}
protected:

	void SetGridInfo(int _rows,int _columns,float _xMin,float _xMax,float _yMin,float _yMax,float _zMin,float _zMax);
	 //分配边信息内存
	void AllocateMemory(); 
	
	void CalcCoord(int row, int column, bool isHorizon);

	/** @brief 扫描并计算纵、横边上等值点的情况*/
	void InterpolateTracingValue();

	void TracingNonClosedContour();
	//追踪一条封闭的等值线
	void TracingOneClosedContour();
	//追踪一条等值线
	//这个函数要完成一条等值线追踪的所有操作
	void TracingOneNonClosedContour();
	//追踪封闭等值线
	void TrackingClosedContour();

	void TracingNextPoint();
	//自下向上追踪函数
	void TracingFromBottom2Top();
	void TracingFromLeft2Right();
	void TracingFromRight2Left();
	void TracingFromTop2Bottom();

	void HandlingAfterNextPointFound();

	void HandlingAfterNextPointFound(int row, int column, bool isHorizon);
private:
	vector<IsoPointListInfo> _allIsoPointList;
	IsoPointListInfo _isoPointListInfo;

	CONTOURLEVELS _contourlevels;

	GridInfo _GridInfo;
	vector<GridPoint> gridPoints;

	vector<vector<EdgeIsoInfo> > _xSide;
	vector<vector<EdgeIsoInfo> > _ySide;

	float _deltX;
	float _deltY;

	string _preString;      //先前的文件名，来决定是否再次读取数据

	float _curFollowingValue;       //当前正在追踪的等值线值
	IsoPoint _preIsoPoint;
	IsoPoint _curIsoPoint;
	IsoPoint _nextIsoPoint;

};
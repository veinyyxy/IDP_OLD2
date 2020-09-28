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
	//һϵ�еõ���ֵ�߲�����������
	void BeginSeriesAction();

	void SetGrid(const ProfileData& data);	
	
	void SetContourLevels(CONTOURLEVELS  levels) {_contourlevels = levels;}

	vector<osg::Vec3Array*> ToOSGVecArray();

	//list<IsoPointListInfo>* GetContourLines() {return &_allIsoPointList;}
protected:

	void SetGridInfo(int _rows,int _columns,float _xMin,float _xMax,float _yMin,float _yMax,float _zMin,float _zMax);
	 //�������Ϣ�ڴ�
	void AllocateMemory(); 
	
	void CalcCoord(int row, int column, bool isHorizon);

	/** @brief ɨ�貢�����ݡ�����ϵ�ֵ������*/
	void InterpolateTracingValue();

	void TracingNonClosedContour();
	//׷��һ����յĵ�ֵ��
	void TracingOneClosedContour();
	//׷��һ����ֵ��
	//�������Ҫ���һ����ֵ��׷�ٵ����в���
	void TracingOneNonClosedContour();
	//׷�ٷ�յ�ֵ��
	void TrackingClosedContour();

	void TracingNextPoint();
	//��������׷�ٺ���
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

	string _preString;      //��ǰ���ļ������������Ƿ��ٴζ�ȡ����

	float _curFollowingValue;       //��ǰ����׷�ٵĵ�ֵ��ֵ
	IsoPoint _preIsoPoint;
	IsoPoint _curIsoPoint;
	IsoPoint _nextIsoPoint;

};
#include "ContourTracer.h"
#include <algorithm>

void ContourTracer::SetGrid(const ProfileData& data)
{
	_GridInfo._rows = data.n_v;
	_GridInfo._columns = data.n_u;
	_GridInfo._xMax = data.umax;
	_GridInfo._xMin = data.umin;
	_GridInfo._yMax = data.vmax;
	_GridInfo._yMin = data.vmin;
	_GridInfo._zMax = data.fmax;
	_GridInfo._zMin = data.fmin;

	int ncols = data.n_u;
	int nrows = data.n_v;

	for (int i=0;i<data.n_v;i++)
	{
		for (int j=0;j<data.n_u;j++)
		{
			gridPoints.push_back(data.m_data.at((nrows-i-1)*ncols+j));
		}
	}
	//gridPoints = data.m_data;
}

void ContourTracer::SetGridInfo(int _rows,int _columns,float _xMin,float _xMax,float _yMin,float _yMax,float _zMin,float _zMax)
{
	_GridInfo._rows = _rows;
	_GridInfo._columns = _columns;
	_GridInfo._xMax = _xMax;
	_GridInfo._xMin = _xMin;
	_GridInfo._yMax = _yMax;
	_GridInfo._yMin = _yMin;
	_GridInfo._zMax = _zMax;
	_GridInfo._zMin = _zMin;
}

void ContourTracer::AllocateMemory()
{
	for (int i=0;i<_GridInfo._rows;i++)
	{
		vector<EdgeIsoInfo> edgelist;
		for (int j=0;j<_GridInfo._columns-1;j++)
		{
			 EdgeIsoInfo info;
			edgelist.push_back(info);
		}
		_xSide.push_back(edgelist);
	}

	for (int i=0;i<_GridInfo._rows-1;i++)
	{
		vector<EdgeIsoInfo> edgelist;
		for (int j=0;j<_GridInfo._columns;j++)
		{
			EdgeIsoInfo info;
			edgelist.push_back(info);
		}
		_ySide.push_back(edgelist);
	}
}

void ContourTracer::BeginSeriesAction()
{
	
	//if(_preString!=_accessContour.FileName)
	//	_accessContour.GetGridData();

//	if (_isSetDefaultContourLevel)
	//	SetDefaultContourLevel();
	//else
	//	SetContourLevel();      //�û��޸�

	_deltX = (_GridInfo._xMax - _GridInfo._xMin) / (_GridInfo._columns - 1);
	_deltY = (_GridInfo._yMax - _GridInfo._yMin) / (_GridInfo._rows - 1);

	AllocateMemory();       //�������Ϣ�ڴ�
	sort(_contourlevels.begin(),_contourlevels.end());

	for (int i = 0; i < _contourlevels.size(); i++)
	{
		_curFollowingValue = _contourlevels.at(i);

		InterpolateTracingValue(); //ɨ�貢�����ݡ�����ϵ�ֵ������

		TracingNonClosedContour();  //׷�ٿ���ֵ��

		TrackingClosedContour();    //׷�ٷ�յ�ֵ��
	}

}

void ContourTracer::InterpolateTracingValue()
{
	if ( _xSide.empty() || _ySide.empty())
		return;

	int i = 0;
	int j = 0;
	float h0, h1;       //��¼һ�����ϵ�����ֵ
	float flag;        //�м�ֵ

	int rows = _GridInfo._rows;               //�����������
	int columns = _GridInfo._columns;         //��������������������������Ϊ�˼��ٳ������ת
	//����Ч��

	int edgeRows= _GridInfo._rows;                //������������Ǹ��ݺᣬ�ݱ�������
	int edgeColumns = _GridInfo._columns - 1;     //�����ȼ�����Ǻ����

	for (i = 0; i < edgeRows; i++)
		for (j = 0; j < edgeColumns; j++)
		{
			h0 = gridPoints.at(i * columns + j);
			h1 = gridPoints.at(i * columns + j + 1);

			if (fabs(h0 - h1) < Epsilon)
			{
				_xSide.at(i).at(j)._rate = -2.0f;
				_xSide.at(i).at(j)._isIsoPoint = false;
			}
			else
			{
				flag = (_curFollowingValue - h0) * (_curFollowingValue - h1);

				if (flag > 0)
				{
					_xSide.at(i).at(j)._rate = -2.0f;
					_xSide.at(i).at(j)._isIsoPoint = false;
				}
				else if (flag < 0)
				{
					_xSide.at(i).at(j)._rate = (_curFollowingValue - h0) / (h1 - h0);
					_xSide.at(i).at(j)._isIsoPoint = true;
				}
				else
				{
					if ((_curFollowingValue - h0) < Epsilon && (_curFollowingValue - h0) > -Epsilon)
						h0 += Excursion;
					else
						h1 += Excursion;

					_xSide.at(i).at(j)._rate = (_curFollowingValue - h0) / (h1 - h0);

					if (_xSide.at(i).at(j)._rate < 0 || _xSide.at(i).at(j)._rate > 1)
						_xSide.at(i).at(j)._isIsoPoint = false;
					else 
						_xSide.at(i).at(j)._isIsoPoint = true;
				}
			}
		}

		edgeRows = _GridInfo._rows - 1;
		edgeColumns = _GridInfo._columns;               //�ټ��������

		for (i=0; i<edgeRows; i++)
			for (j = 0; j < edgeColumns; j++)
			{
				h0 = gridPoints.at(i * columns + j);
				h1 = gridPoints.at((i + 1) * columns + j);

				if (fabs(h0 - h1) < Epsilon)
				{
					_ySide.at(i).at(j)._rate = -2.0f;
					_ySide.at(i).at(j)._isIsoPoint = false;
				}
				else
				{
					flag = (_curFollowingValue - h0) * (_curFollowingValue - h1);

					if (flag > 0)
					{
						_ySide.at(i).at(j)._rate = -2.0f;
						_ySide.at(i).at(j)._isIsoPoint = false;
					}
					else if (flag < 0)
					{
						_ySide.at(i).at(j)._rate = (_curFollowingValue - h0) / (h1 - h0);
						_ySide.at(i).at(j)._isIsoPoint = true;
					}
					else
					{
						if ((_curFollowingValue - h0) < Epsilon && (_curFollowingValue - h0) > -Epsilon)
							h0 += Excursion;
						else
							h1 += Excursion;

						_ySide.at(i).at(j)._rate = (_curFollowingValue - h0) / (h1 - h0);

						if (_ySide.at(i).at(j)._rate < 0 || _ySide.at(i).at(j)._rate > 1)
							_ySide.at(i).at(j)._isIsoPoint = false;
						else
							_ySide.at(i).at(j)._isIsoPoint = true;
					}
				}
			}
}

void ContourTracer::TracingNonClosedContour()
{
	 /*
             * ������ڴ沼�ֿ�����Ϊ��     ^   |-------------|
             * ����ֱ�������ϵ���           |   |      |      |
             * ��ˮƽ�������ҵ���           |   |-------------|
             *                              |   |      |      |
             *                              |   |-------------|
             *                              |-------------------->
             *������׷��ʱ�ķ���ͬ����Ҫ�ֳɲ�ͬ�����
             */
     int edgeRows = _GridInfo._rows - 1;
     int edgeColumns = _GridInfo._columns - 1;
            
      //Լ���׳�i��ʾ�У�j��ʾ��
      //׷�ٵױ߿�
       for (int j = 0; j < edgeColumns; j++)       
        {
             if (_xSide.at(0).at(j)._isIsoPoint)
                {
                    _preIsoPoint._row = -1;
                    _preIsoPoint._column = j;
                    _preIsoPoint._isHorizon = true;
                    _curIsoPoint._row = 0;
                    _curIsoPoint._column = j;
                    _curIsoPoint._isHorizon = true;

                    TracingOneNonClosedContour();
                }
        }

            //׷����߿�
        for (int i = 0; i < edgeRows; i++)
          {
                if (_ySide.at(i).at(0)._isIsoPoint)
                {
                    _preIsoPoint._row = i;
                    _preIsoPoint._column = -1;
                    _preIsoPoint._isHorizon = false;
                    _curIsoPoint._row = i;
                    _curIsoPoint._column = 0;
                    _curIsoPoint._isHorizon = false;

                    TracingOneNonClosedContour();
               }
         }

            //׷���ϱ߿�
            for (int j = 0; j < edgeColumns; j++)
            {
                if (_xSide.at(edgeRows).at(j)._isIsoPoint)
                {
                    _preIsoPoint._row = edgeRows;
                    _preIsoPoint._column = j;
                    _preIsoPoint._isHorizon = true;
                    _curIsoPoint._row = edgeRows;
                    _curIsoPoint._column = j;
                    _curIsoPoint._isHorizon = true;

                    TracingOneNonClosedContour();
                }
            }

            //׷���ұ߿�
            for (int i = 0; i < edgeRows; i++)
            {
                if (_ySide.at(i).at(edgeColumns)._isIsoPoint)
                {
                    _preIsoPoint._row = i;
                    _preIsoPoint._column = edgeColumns;
                    _preIsoPoint._isHorizon = false;
                    _curIsoPoint._row = i;
                    _curIsoPoint._column = edgeColumns;
                    _curIsoPoint._isHorizon = false;

                    TracingOneNonClosedContour();
                }
            }
}

void ContourTracer::CalcCoord(int row, int column, bool isHorizon)
{
	IsoPoint isoPoint;
	isoPoint._column = column;
	isoPoint._row = row;
	isoPoint._isHorizon = isHorizon;

	if (isHorizon)
	{
		isoPoint._x = _GridInfo._xMin + (column + _xSide.at(row).at(column)._rate) * _deltX;
		isoPoint._y = _GridInfo._yMin + row * _deltY;
	}
	else
	{
		isoPoint._x = _GridInfo._xMin + column * _deltX;
		isoPoint._y = _GridInfo._yMin + (row + _ySide.at(row).at(column)._rate) * _deltY;
	}

	float value = _curFollowingValue;
	_isoPointListInfo._aIsoPointList.push_back(isoPoint);
}
	//׷��һ����յĵ�ֵ��
void ContourTracer::TracingOneClosedContour()
{
	int rows = _GridInfo._rows;       //������������������
	int columns = _GridInfo._columns;

	int startI = _curIsoPoint._row;
	int startJ = _curIsoPoint._column;

	_isoPointListInfo._aIsoPointList.clear();
	_isoPointListInfo._value = _curFollowingValue;

	CalcCoord(_curIsoPoint._row, _curIsoPoint._column, false);

	TracingNextPoint();

	_preIsoPoint = _curIsoPoint;
	_curIsoPoint = _nextIsoPoint;

	bool isClosed = false;

	while (!isClosed)
	{
		TracingNextPoint();

		_preIsoPoint = _curIsoPoint;
		_curIsoPoint = _nextIsoPoint;

		isClosed = (_curIsoPoint._row == startI) && (_curIsoPoint._column == startJ) && (false == _curIsoPoint._isHorizon);
	}

	_allIsoPointList.push_back(_isoPointListInfo);
}
	//׷��һ����ֵ��
	//�������Ҫ���һ����ֵ��׷�ٵ����в���
void ContourTracer::TracingOneNonClosedContour()
{
	int rows = _GridInfo._rows;       //������������������
	int columns = _GridInfo._columns;

	_isoPointListInfo._aIsoPointList.clear();
	_isoPointListInfo._value = _curFollowingValue;

	CalcCoord(_curIsoPoint._row, _curIsoPoint._column, _curIsoPoint._isHorizon);

	if (_curIsoPoint._isHorizon)
		_xSide.at(_curIsoPoint._row).at(_curIsoPoint._column)._isIsoPoint = false;
	else
		_ySide.at(_curIsoPoint._row).at(_curIsoPoint._column)._isIsoPoint = false;

	TracingNextPoint();

	_preIsoPoint = _curIsoPoint;
	_curIsoPoint = _nextIsoPoint;

	bool isFinish = (_curIsoPoint._row == 0 && _curIsoPoint._isHorizon) || (_curIsoPoint._column == 0 && !_curIsoPoint._isHorizon)
		|| (_curIsoPoint._row == rows - 1) || (_curIsoPoint._column == columns - 1);

	while (!isFinish)
	{
		TracingNextPoint();

		_preIsoPoint = _curIsoPoint;
		_curIsoPoint = _nextIsoPoint;

		isFinish = (_curIsoPoint._row == 0 && _curIsoPoint._isHorizon) || (_curIsoPoint._column == 0 && !_curIsoPoint._isHorizon)
			|| (_curIsoPoint._row == rows - 1) || (_curIsoPoint._column == columns - 1);
	}

	_allIsoPointList.push_back(_isoPointListInfo);
}

	//׷�ٷ�յ�ֵ��
void ContourTracer::TrackingClosedContour()
{
	int rows = _GridInfo._rows - 1;
	int columns = _GridInfo._columns - 1;

	int i = 0;
	int j = 0;
	for(j = 1 ; j < columns; j++)
		for (i = 0; i < rows; i++)
		{
			if (_ySide.at(i).at(j)._isIsoPoint)
			{
				_preIsoPoint._row = i;
				_preIsoPoint._column = 0;
				_preIsoPoint._isHorizon = false;
				_curIsoPoint._row = i;
				_curIsoPoint._column = j;
				_curIsoPoint._isHorizon = false;

				TracingOneClosedContour();
			}
		}
}

void ContourTracer::TracingNextPoint()
{
	if (_curIsoPoint._row > _preIsoPoint._row)
	{
		TracingFromBottom2Top();
		return;
	}
	else if (_curIsoPoint._column > _preIsoPoint._column)
	{
		TracingFromLeft2Right();
		return;
	}
	else if (_curIsoPoint._isHorizon)
	{
		TracingFromTop2Bottom();
		return;
	}
	else
	{
		TracingFromRight2Left();
		return;
	}
}

void ContourTracer::HandlingAfterNextPointFound(int row, int column, bool isHorizon)
{
	_nextIsoPoint._row = row;
	_nextIsoPoint._column = column;
	_nextIsoPoint._isHorizon = isHorizon;

	CalcCoord(row, column, isHorizon);

	if (isHorizon)
		_xSide.at(row).at(column)._isIsoPoint = false;
	else
		_ySide.at(row).at(column)._isIsoPoint = false;
}
	//��������׷�ٺ���
void ContourTracer::TracingFromBottom2Top()
{
	int row = _curIsoPoint._row;
	int column = _curIsoPoint._column;

	if (_ySide.at(row).at(column)._isIsoPoint)
	{
		if (_xSide.at(row + 1).at(column)._isIsoPoint)
		{
			if (_ySide.at(row).at(column)._rate < _ySide.at(row).at(column + 1)._rate)
			{
				HandlingAfterNextPointFound(row, column, false);
				return;
			}
			else if (_ySide.at(row).at(column)._rate > _ySide.at(row).at(column + 1)._rate)
			{
				HandlingAfterNextPointFound(row, column + 1, false);
				return;
			}
			else
			{
				if (_ySide.at(row+1).at(column)._rate < 0.5f)
				{
					HandlingAfterNextPointFound(row, column, false);
					return;
				}
				else
				{
					HandlingAfterNextPointFound(row, column + 1, false);
					return;
				}
			}
		}
		else
		{
			HandlingAfterNextPointFound(row, column, false);
			return;
		}
	}
	else if (_xSide.at(row+1).at(column)._isIsoPoint)
	{
		HandlingAfterNextPointFound(row + 1, column, true);
		return;
	}
	else
		HandlingAfterNextPointFound(row, column + 1, false);
}

void ContourTracer::TracingFromLeft2Right()
{
	int row = _curIsoPoint._row;
	int column = _curIsoPoint._column;

	if (_xSide.at(row+1).at(column)._isIsoPoint)
	{
		if (_ySide.at(row).at(column+1)._isIsoPoint)
		{
			if (_ySide.at(row+1).at(column)._rate < _ySide.at(row).at(column)._rate)
			{
				HandlingAfterNextPointFound(row + 1, column, false);
				return;
			}
			else if (_ySide.at(row+1).at(column)._rate > _ySide.at(row).at(column)._rate)
			{
				HandlingAfterNextPointFound(row, column, false);
				return;
			}
			else
			{
				if (_ySide.at(row).at(column+1)._rate < 0.5f)
				{
					HandlingAfterNextPointFound(row + 1, column, true);
					return;
				}
				else
				{
					HandlingAfterNextPointFound(row, column, true);
					return;
				}
			}
		}
		else
		{
			HandlingAfterNextPointFound(row + 1, column, true);
			return;
		}
	}
	else if (_ySide.at(row).at(column+1)._isIsoPoint)
	{
		HandlingAfterNextPointFound(row, column + 1, false);
		return;
	}
	else
	{
		HandlingAfterNextPointFound(row, column, true);
		return;
	}
}

void ContourTracer::TracingFromRight2Left()
{
	int row = _curIsoPoint._row;
	int column = _curIsoPoint._column;

	if (_xSide.at(row).at(column-1)._isIsoPoint)
	{
		if (_ySide.at(row).at(column-1)._isIsoPoint)
		{
			if (_xSide.at(row).at(column-1)._rate < _xSide.at(row+1).at(column-1)._rate)
			{
				HandlingAfterNextPointFound(row + 1, column - 1, true);
				return;
			}
			else if (_xSide.at(row).at(column-1)._rate > _xSide.at(row+1).at(column-1)._rate)
			{
				HandlingAfterNextPointFound(row, column - 1, true);
				return;
			}
			else
			{
				if (_ySide.at(row).at(column-1)._rate < 0.5f)
				{
					HandlingAfterNextPointFound(row, column - 1, true);
					return;
				}
				else
				{
					HandlingAfterNextPointFound(row + 1, column - 1, true);
					return;
				}
			}
		}
		else
		{
			HandlingAfterNextPointFound(row, column - 1, true);
			return;
		}
	}
	else if (_ySide.at(row).at(column-1)._isIsoPoint)
	{
		HandlingAfterNextPointFound(row, column - 1, false);
		return;
	}
	else
	{
		HandlingAfterNextPointFound(row + 1, column - 1, true);
		return;
	}
}

void ContourTracer::TracingFromTop2Bottom()
{
	int row = _curIsoPoint._row;
	int column = _curIsoPoint._column;

	if (_ySide.at(row-1).at(column+1)._isIsoPoint)
	{
		if (_xSide.at(row-1).at(column)._isIsoPoint)
		{
			if (_ySide.at(row-1).at(column)._rate < _ySide.at(row-1).at(column+1)._rate)      //����Ҫע�������ϵ��µĲ�ͬ�Ĵ���
			{
				HandlingAfterNextPointFound(row - 1, column + 1, false);
				return;
			}
			else if (_ySide.at(row-1).at(column)._rate > _ySide.at(row-1).at(column+1)._rate)
			{
				HandlingAfterNextPointFound(row - 1, column, false);
				return;
			}
			else
			{
				if (_xSide.at(row-1).at(column)._rate < 0.5f)
				{
					HandlingAfterNextPointFound(row - 1, column, false);
					return;
				}
				else
				{
					HandlingAfterNextPointFound(row - 1, column + 1, false);
					return;
				}
			}

		}
		else
		{
			HandlingAfterNextPointFound(row - 1, column + 1, false);
			return;
		}
	}
	else if (_xSide.at(row-1).at(column)._isIsoPoint)
	{
		HandlingAfterNextPointFound(row - 1, column, true);
		return;
	}
	else
	{
		HandlingAfterNextPointFound(row - 1, column, false);
		return;
	}
}


void ContourTracer::HandlingAfterNextPointFound()
{

}

vector<osg::Vec3Array*> ContourTracer::ToOSGVecArray()
{
	vector<osg::Vec3Array*> linestripArray;
	for (int i=0;i<_allIsoPointList.size();i++)
	{
		vector<IsoPoint> line = _allIsoPointList.at(i)._aIsoPointList;
		osg::Vec3Array* array = new osg::Vec3Array;
		for (int j=0;j<line.size();j++)
		{
			array->push_back(osg::Vec3(line.at(j)._x,line.at(j)._y,0));
		}
		linestripArray.push_back(array);
	}
	return linestripArray;
}
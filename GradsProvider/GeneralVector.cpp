#include "GeneralVector.h"

extern QVector<lineData*> ContourData;						// 等值线线段数组

GeneralVector::GeneralVector(void)
{
	m_cBitColor = QColor(255,255,255);
}

GeneralVector::~GeneralVector(void)
{
}

void GeneralVector::InitCommand()
{
	gcmn.sig = 0;
	gacmd("set csmooth on",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set cterp on",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set grid off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set mpdraw off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set frame off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set clab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set grads off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set xlab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set ylab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set arrlab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set lon 0 360",&gcmn,0);   // added by Liu Xinsheng 2011-05-02
	gcmn.sig = 0;
	gacmd("set lat -89 89",&gcmn,0);     // added by WangJL 2011-05-02
	gcmn.sig = 0;
	gacmd("set cstyle 1",&gcmn,0);
}
void GeneralVector::ScaleDataLine()
{
	unsigned int nCount = ContourData.size();
	unsigned int i;
	double x,y,lon,lat;

	for(i = 0 ;i< nCount; i++)
	{
		lineData *lineData1 = ContourData.at(i);
		for(int j = 0 ; j< lineData1->arrayData.count();j = j+3)
		{
			x = lineData1->arrayData[j];
			y = lineData1->arrayData[j+1];
			gxxy2w (x, y, &lon, &lat);
			lineData1->arrayData[j] = lon;
			lineData1->arrayData[j+1] = lat;
		}
	}
}
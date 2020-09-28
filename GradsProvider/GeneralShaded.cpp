#include "GeneralShaded.h"
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/geom/Polygon.h>
#include "goPolygon.h"

extern QVector<XFILLPOINT*> ShadeData;						// 填图多边形数组
extern QVector<lineData*> ContourData;						// 等值线线段数组
extern QVector<LineTextValue*> ContourTextValueArray;		// 数字位置数组
extern QVector<double>  vector_ContorValue;					// 等值线属性值
extern QVector<XFILLPOINT*> ShadeRecData;					// 填图四边形数组

GeneralShaded::GeneralShaded(void)
{
}

GeneralShaded::~GeneralShaded(void)
{
	
}

int GeneralShaded::Draw(const std::string& strCmd)
{
	InitCommand();							// 用命令行去除绘制色斑图过程中产生的：图形边框、坐标格网、边框左侧及下侧外围的短线

	//if(m_listInterval->empty())	// 绘制默认色斑图
	//{
		gcmn.sig = 0;
		gacmd("set gxout shaded",&gcmn,0);     

		gcmn.sig = 0;
		if(gacmd((char*)(strCmd.c_str()),&gcmn,0))
		{
			if(gcmn.str_errorinfo != NULL)
				return 0;
				////CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
			else
				return 0;
				////CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),tr("发生未知绘图错误！"));

			return 1;
		}
		if (gcmn.error == true)
		{
			////CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
			return 1;
		}
		ScaleDataShade();

		m_ZlevelIndex.clear();
		double dIndex = 1.00001f;
		double dStep = 1.00001f;
		for(int i = 0; i < gcmn.shdcnt; i++)
		{
			m_ZlevelIndex.insert(gcmn.shdlvs[i] ,dIndex+dStep*i);
		}

		//for (int i = 0;i < gcmn.shdcnt;i++)
		//{
		//	qDebug()<<"shaded:"<<gcmn.shdlvs[i];
		//}

		//gcmn.sig = 0;
		//gacmd("set gxout contour",&gcmn,0);     // 绘制色斑图标值

		//gcmn.sig = 0;
		//gacmd("set lon 0 360",&gcmn,0);

		//gcmn.sig = 0;
		//gacmd("set clab on",&gcmn,0);

		//sprintf(m_Command,"set clevs");
		//for(int i = 1; i < gcmn.shdcnt; i++)
		//{
		//	QString tempVal = QString("%1").arg((float)gcmn.shdlvs[i],0,'f',m_nPrecision);
		//	QByteArray tempByte = tempVal.toLocal8Bit();
		//	char *temp = tempByte.data();
		//	sprintf(m_Temp," %s",temp);
		//	strcat(m_Command,m_Temp);
		//}
		//gcmn.sig = 0;
		//gacmd(m_Command,&gcmn,0);

		//gcmn.sig = 0;
		//gacmd("set cstyle 1",&gcmn,0);

		//gcmn.sig = 0;
		//gacmd("display va",&gcmn,0);			// 绘制色斑图标值

		//for (int i = 0;i < gcmn.cntrcnt;i++)
		//{
		//	qDebug()<<"contour:"<<gcmn.cntrlevs[i];
		//}

		//ReadColorList(m_nReadColorList);
		//SetColorDrawIndex();
	//}
	//else 	// 绘制指定色斑图
	//{
	//	gcmn.sig = 0;
	//	gacmd("set gxout shaded",&gcmn,0);

	//	char tempColorCommand[256];
	//	sprintf(tempColorCommand,"set ccols");
	//	sprintf(m_Command,"set clevs");
	//	for(int i = 1; i < m_listInterval->size(); i++)
	//	{
	//		QString tempVal = QString("%1").arg(m_listInterval->at(i)->fValue,0,'f',m_nPrecision);
	//		QByteArray tempByte = tempVal.toLocal8Bit();
	//		char *temp = tempByte.data();
	//		sprintf(m_Temp," %s",temp);
	//		strcat(m_Command,m_Temp);

	//		sprintf(m_Temp," %d",i);
	//		strcat(tempColorCommand,m_Temp);
	//	}
	//	gcmn.sig = 0;
	//	gacmd(m_Command,&gcmn,0);

	//	sprintf(m_Temp," %d",m_listInterval->size());
	//	strcat(tempColorCommand,m_Temp);

	//	gcmn.sig = 0;
	//	gacmd(tempColorCommand,&gcmn,0);

	//	gcmn.sig = 0;
	//	if(gacmd("display va",&gcmn,0))
	//	{
	//		//if(gcmn.str_errorinfo != NULL)
	//		//	//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
	//		//else
	//		//	//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),tr("发生未知绘图错误！"));

	//		return 1;
	//	}
	//	if (gcmn.error == true)
	//	{
	//		////CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
	//		return 1;
	//	}

	//	gcmn.sig = 0;
	//	gacmd("set gxout contour",&gcmn,0);     // 绘制色斑图标值

	//	gcmn.sig = 0;
	//	gacmd("set lon 0 360",&gcmn,0);

	//	gcmn.sig = 0;
	//	gacmd("set clab on",&gcmn,0);

	//	sprintf(m_Command,"set clevs");
	//	for(int i = 1; i < m_listInterval->size(); i++)
	//	{
	//		QString tempVal = QString("%1").arg(m_listInterval->at(i)->fValue,0,'f',m_nPrecision);
	//		QByteArray tempByte = tempVal.toLocal8Bit();
	//		char *temp = tempByte.data();
	//		sprintf(m_Temp," %s",temp);
	//		strcat(m_Command,m_Temp);
	//	}
	//	gcmn.sig = 0;
	//	gacmd(m_Command,&gcmn,0);

	//	gcmn.sig = 0;
	//	gacmd("set cstyle 1",&gcmn,0);

	//	gcmn.sig = 0;
	//	gacmd("display va",&gcmn,0);			// 绘制色斑图标值

	//	
	//}

	//gcmn.sig = 0;
	//gacmd("reinit",&gcmn,0); //初始化GRADS

	return 0;
}

void GeneralShaded::ScaleDataShade()
{
	int nCount = ShadeData.size();
	int i,j;
	XFILLPOINT *pfillpoint = NULL;
	double lon,lat;

	for(i = 0 ;i< nCount; i ++)
	{
		pfillpoint = ShadeData[i];

		for(j = 0; j < pfillpoint->n; j++)
		{
			gxxy2w (pfillpoint->pPoint[j].x, pfillpoint->pPoint[j].y, &lon, &lat);
			pfillpoint->pPoint[j].x = lon;
			pfillpoint->pPoint[j].y = lat;
		}
	}
}

int GeneralShaded::Dump(goto_gis::Layer* pLayer, int iDirect)
{
	if(!pLayer) return 0;
	float fZ = pLayer->LayerZ();
	goto_gis::goGeometry* pGoGeomtry = 0;
	//geos::geom::Geometry* pGeosGeome= 0;
	const geos::geom::GeometryFactory * pGeometryFactory = geos::geom::GeometryFactory::getDefaultInstance();
	const geos::geom::CoordinateSequenceFactory* pCoordinateSequenceFactory = geos::geom::CoordinateArraySequenceFactory::instance();

	for(int i = 0; i <  ShadeData.count(); i++)
	{
		XFILLPOINT * pFillPoint = ShadeData.at(i);

		int iPointCount = pFillPoint->n;

		int iPPC = pFillPoint->n;
		geos::geom::CoordinateSequence* pCoordArray = pCoordinateSequenceFactory->create((size_t)0, 3);

		pGoGeomtry = new goto_gis::goPolygon;
		pGoGeomtry->UserLayerColor(false);
		pGoGeomtry->UserLayerZValue(false);

		for(int j = 0; j < iPointCount; j++)
		{	
			geos::geom::Coordinate coordinate((pFillPoint->pPoint + j)->x, (pFillPoint->pPoint + j)->y);
			int iZDirect = 0;
			if(iDirect == 0)
				iZDirect = abs(m_ZlevelIndex.value(pFillPoint->fShadeValue))*100 + fZ;
			else
				iZDirect = -(abs(m_ZlevelIndex.value(pFillPoint->fShadeValue))*100 + fZ);

			coordinate.z = iZDirect;
			pCoordArray->add(coordinate);
		}
		pGoGeomtry->GetColorArray()->push_back(osg::Vec4(pFillPoint->ColorData[0],pFillPoint->ColorData[1],pFillPoint->ColorData[2],1));

		geos::geom::LinearRing* pLinearRing = pGeometryFactory->createLinearRing(pCoordArray);
		geos::geom::Polygon* pPolygon = pGeometryFactory->createPolygon(pLinearRing, 0);

		const geos::geom::CoordinateSequence* pCS = pPolygon->getCoordinates();

		pGoGeomtry->SetGeosGeomtry(pPolygon);
		pGoGeomtry->SetCoordinateSequence((geos::geom::CoordinateSequence*)pCS);
		pLayer->GetMgoGeometry()->insert(goto_gis::Layer::MgoGeometryPair(i, pGoGeomtry));
	}


	/*******************clear shaded data*****************/
	QVector<XFILLPOINT*>::Iterator iteratorXFP;
	for (iteratorXFP = ShadeData.begin(); iteratorXFP != ShadeData.end();++iteratorXFP)
	{
		delete[] (*iteratorXFP)->pPoint;
		delete *iteratorXFP;
	}
	ShadeData.clear();

	QVector<XFILLPOINT*>::Iterator iteratorXFPRec;
	for (iteratorXFPRec = ShadeRecData.begin(); iteratorXFPRec != ShadeRecData.end();++iteratorXFPRec)
	{
		delete[] (*iteratorXFPRec)->pPoint;
		delete *iteratorXFPRec;
	}
	ShadeRecData.clear();
	/******************clear line data***********/
	QVector<lineData*>::iterator iteratorLine;
	for (iteratorLine = ContourData.begin(); iteratorLine != ContourData.end();++iteratorLine)
	{
		(*iteratorLine)->arrayData.clear();
		delete *iteratorLine;
	}
	ContourData.clear();
	/******************clear text data***********/
	QVector<LineTextValue*>::Iterator iteratorCTVA;
	for (iteratorCTVA = ContourTextValueArray.begin(); iteratorCTVA != ContourTextValueArray.end();++iteratorCTVA)
	{
		delete *iteratorCTVA;
	}
	ContourTextValueArray.clear();
	/******************clear line attr data***********/
	vector_ContorValue.clear();

	gcmn.sig = 0;
	gacmd("clear",&gcmn,0);					// 清空绘图区内容
	gcmn.sig = 0;
	gacmd("reset",&gcmn,0);					// 重置Grads系统环境
	return 0;
}

void GeneralShaded::InitCommand()
{
	gcmn.sig = 0;
	gacmd("set csmooth on",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set cterp off",&gcmn,0);
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

	for (int i = 16;i < 64;i++)
	{
		sprintf(m_Command,"set rgb");

		sprintf(m_Temp," %d",i);
		strcat(m_Command,m_Temp);

		sprintf(m_Temp," %d",i*4);
		strcat(m_Command,m_Temp);

		sprintf(m_Temp," %d",i*4);
		strcat(m_Command,m_Temp);

		sprintf(m_Temp," %d",i*4);
		strcat(m_Command,m_Temp);

		gcmn.sig = 0;
		gacmd(m_Command,&gcmn,0);
	}

}
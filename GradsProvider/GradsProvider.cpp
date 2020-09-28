#include "GradsProvider.h"

#include "qcoreapplication.h"
#include "QtXml/QXmlStreamReader"
#include "qfile.h"
#include <QtGui/QIcon>

#include "Layer.h"
//#include "Polygon.h"
//#include "SymbolContourLine.h"

//#include "CPointSymbol.h"
//#include "../GIS_Wrapper/ReadEleColor.h"

#include "gxchpl.h"
#include "gxmeta.h"
//#include "PointSizeAttribute.h"
//#include "Proj4Transform.h"
//#include "RadProjTransform.h"

//#if _DEBUG
//#include "vld.h"
//#endif
#include "GradsDataInterfaceImp.h"

extern QVector<XFILLPOINT*> ShadeData;			// 填图多边形数组
extern QVector<XFILLPOINT*> ShadeRecData;			// 填图多边形数组
extern QVector<lineData*> ContourData;						// 等值线线段数组
extern QVector<LineTextValue*> ContourTextValueArray;		// 数字位置数组
extern QVector<WindBarbData*> WindBarbDataArray;			// 风标位置数组
extern QVector<double>  vector_ContorValue;   // 等值线属性值

//extern short *hbuff, *hpnt, *hend;

const float LVALUE = -70;
const float RVALUE = -70.00004;

/*************************   GradsProvider class member function   ************************/
DataProvider* g_pDataProvider = 0;

extern "C"
{
	DataProvider Q_DECL_EXPORT * DataProviderPluginMain()
	{
		g_pDataProvider = new GradsProvider;
		return g_pDataProvider;
	}
}

extern "C"
{
	bool Q_DECL_EXPORT MineData(std::string& stFileName)
	{
		QString strFileName;
		strFileName = QString::fromStdString(stFileName);
		QFileInfo fileInfo;
		fileInfo.setFile(strFileName);

		QString strSuffix("grads");
		QString strSuffix1 = fileInfo.suffix();
		if(strSuffix1 == strSuffix)
			return true;
		else
			return false;
	}
}

GradsProvider::GradsProvider()
{
	m_pDataInterface = new GradsDataInterfaceImp(this);

	m_ExtendInterface.insert(INDEX_INTERFACE_PAIR(0, m_pDataInterface));

	//m_pTempElement=NULL;
	//int rc,i,j,
	int land,port,cmdflg,hstflg,gflag,xwideflg,killflg,ratioflg;
	int metabuff,size=0,g2size=0;
	int txtcs=-2;
	int ipcflg = 0; /* for IPC friendly interaction via pipes */
	char *icmd,*arg,*rc1;
	double aspratio;

	/*--- common block sets before gainit ---*/
	gcmn.batflg = 0;
	land = 0;
	port = 0;
	cmdflg = 0;
	metabuff = 0;
	hstflg = 1;
	gflag = 0;
	xwideflg = 0;
	icmd = NULL;
	arg = NULL;
	rc1 = NULL;
	killflg = 0;
	ratioflg = 0;
	aspratio = -999.9;
	m_isRain = false;

	if (port) {
		gcmn.xsiz = 8.5;
		gcmn.ysiz = 11.0;
	} else {
		gcmn.xsiz = 11.0;
		gcmn.ysiz = 8.5;
	}
	if (aspratio>-990) { /* user has specified aspect ratio */
		if (aspratio>0.2 && aspratio < 5.0) {   /* range is limited here. */
			if (aspratio < 1.0) {
				gcmn.xsiz = 11.0*aspratio;
				gcmn.ysiz = 11.0;
			} else {
				gcmn.ysiz = 11.0/aspratio;
				gcmn.xsiz = 11.0;
			}
		}
		else {
			printf("Warning: Aspect ratio must be between 0.2 and 5.0 -- defaulting to landscape mode\n");
		}
	}

	/* Functions in gxX:
	gxwdln: use X server for wide lines
	if(xwideflg) gxwdln();             */

	gainit();
	gcmn.pfi1 = NULL;              /* No data sets open      */
	gcmn.pfid = NULL;
	gcmn.fnum = 0;
	gcmn.dfnum = 0;
	gcmn.undef = -9.99e8;         /* default undef value */
	gcmn.fseq = 10;
	gcmn.pdf1 = NULL;
	gcmn.grflg = 0;
	gcmn.devbck = 0;
	gcmn.sdfwname = NULL;
	gcmn.sdfwtype = 1;
	gcmn.sdfwpad = 0;
	gcmn.sdfchunk = 0;
	gcmn.sdfzip = 0;
	gcmn.sdfprec = 8;
	gcmn.ncwid = -999;
	gcmn.xchunk = 0;
	gcmn.ychunk = 0;
	gcmn.zchunk = 0;
	gcmn.tchunk = 0;
	gcmn.echunk = 0;
	gcmn.attr = NULL;
	gcmn.ffile = NULL;
	gcmn.sfile = NULL;
	gcmn.fwname = NULL;
	gcmn.gtifname = NULL;    /* for GeoTIFF output */
	gcmn.tifname = NULL;     /* for KML output */
	gcmn.kmlname = NULL;     /* for KML output */
	gcmn.kmlflg = 1;         /* default KML output is an image file */
	gcmn.shpfname = NULL;    /* for shapefile output */
	gcmn.shptype = 2;        /* default shape type is line */
	//gcmn.fwenflg = BYTEORDER;
	gcmn.fwenflg = 0;
	gcmn.fwsqflg = 0;        /* default is stream */
	gcmn.fwexflg = 0;        /* default is not exact -- old bad way */
	gcmn.gtifflg = 1;        /* default geotiff output format is float */
	if (size) gcmn.hbufsz = size;
	if (g2size) gcmn.g2bufsz = g2size;
	gcmn.cachesf = 1.0;      /* global scale factor for netcdf4/hdf5 cache */
	gcmn.fillpoly = -1;      /* default is to not fill shapefile polygons */
	gcmn.marktype = 3;       /* default is to draw points as closed circe */
	gcmn.marksize = 0.05;    /* default mark size */

	m_dArrayValue = NULL;

	//m_pTextColorAttri = new ColorAttribute;
	//m_pTextColorAttri->Color(QVector4D(0.0,0.0,0.0,1));//默认设置为黑色

	//m_PTempText = new QBitmapText;
	//
	//QVector3D posOrigin(25, 0, 0), gltempPos ;
	//QVector3D posEnd(0,0,0), glposEnd;
	//ScreenToGL(posOrigin,gltempPos);
	//ScreenToGL(posEnd,glposEnd);
	//m_fdistance = gltempPos.x()- glposEnd.x(); //初始化地面填图中各要素的间距

	////Initialize();
	//m_stPoint = NULL;

	//m_bLonLatChanged = false;
	//m_bhashOnce = true;

	gcmn.str_errorinfo = NULL;
	gcmn.isProfile = false;
	gcmn.isContour = false;
	gcmn.isLineBar = false;
	gcmn.isShaded = false;
	gcmn.isVBS = false;
	gcmn.nCmax = NULL;
	gcmn.nCmin = NULL;
	gcmn.nCint = NULL;

	gcmn.error = false;

	m_pProduct = NULL;

	//m_pGPloyLineBar = NULL;
	//m_pTProfile = NULL;
	//m_pSProfile = NULL;

	//m_pContourShadedBMT = NULL;

	//m_gplistInterval.clear();

	//m_pPixmapPoint = NULL;
	//m_Pixmap = QIcon(":/images/ShiXinYuan.png").pixmap(32,32);
	//m_nSymbolSize = 3;
	//m_fSymbolAngle = 0.0f;
	//m_bShowValue = true;
	//m_fMaxValue = 65535.0;
	//m_fMinValue = -65535.0;

	m_nPrecision = 3;

	//m_Font = QFont(QString(tr("宋体")),12);
	//m_FontColor = QColor(0,0,255);
	//m_FontShow = 1;
	//m_pTextureFont = NULL;

	ClearVector();
}
GradsProvider::~GradsProvider()
{
	Initialize();
	
	//if (m_pPixmapPoint != NULL)
	//{
	//	delete m_pPixmapPoint;
	//	m_pPixmapPoint = NULL;
	//}
	//if (m_pTextColorAttri != NULL)
	//{
	//	delete m_pTextColorAttri;
	//	m_pTextColorAttri = NULL;
	//}
	if(m_dArrayValue != NULL)
	{
		delete[] m_dArrayValue;
		m_dArrayValue = NULL;
		//gcmn.pfi1->pArrayValuebuf = NULL;
	}
	//if (m_PTempText != NULL)
	//{
	//	delete m_PTempText;
	//	m_PTempText = NULL;
	//}

	if(m_pProduct != NULL)
	{
		delete m_pProduct;
		m_pProduct = NULL;
	}

	//if(m_pGPloyLineBar != NULL)
	//{
	//	delete m_pGPloyLineBar;
	//	m_pGPloyLineBar = NULL;
	//}
	//if(m_pTProfile != NULL)
	//{
	//	delete m_pTProfile;
	//	m_pTProfile = NULL;
	//}
	//if(m_pSProfile != NULL)
	//{
	//	delete m_pSProfile;
	//	m_pSProfile = NULL;
	//}

	for (int i=0;i<8;i++)
	{
		if (gcmn.mpdset[i])
		{
			free(gcmn.mpdset[i]);
			gcmn.mpdset[i] = NULL;
		}
	}

	for(int i=0;i<10;i++)
	{
		if (fch[i]) /* Pointers to font data once it is read in */
		{
			free(fch[i]);
			fch[i] = NULL;
		}
		if (foff[i]) /* Pointers to character offsets */
		{
			free(foff[i]);
			foff[i] = NULL;
		}
		if (flen[i]) /* Pointers to character lengths */
		{
			free(flen[i]);
			flen[i] = NULL;
		}
	}
	//if (hbuff)
	//{
	//	free(hbuff);
	//	hbuff = NULL;
	//}
	for (int i=0;i<250;i++)
	{
		//free(bufs[i]);
		bufs[i] = NULL;
		//free(bufs2[i]);
		bufs2[i] = NULL;
	}
	
	gcmn.isProfile = false;
	gcmn.isContour = false;
	gcmn.isLineBar = false;
	gcmn.isShaded = false;
	gcmn.isVBS = false;
}

//bool GradsProvider::DumpAll()
//{
//	//VectorLayer *cLayer = (VectorLayer*)DataProvider::ParentLayer();
//	//bool bRes = false;
//
//	//if (m_graphicType  == 1 || m_graphicType == 2 || m_graphicType == 3 || m_graphicType == 4 || m_graphicType == 5 || m_graphicType == 23)
//	//{
//	//	if (m_pProduct != NULL)
//	//	{
//	//		m_pProduct->SetContourDump(cLayer->Get2DHashLineList());
//	//		m_pProduct->SetShadedDump(cLayer->Get2DFigureData(), cLayer->Get2DFDnoTessel(), cLayer->Get2DHashLineList(), parentLayer->GetPolygonType());
//	//		m_pProduct->SetBarbDump(parentLayer->CoordTrans(),m_pSymbolPlugin);
//	//		m_pProduct->SetGirdDump(m_nLonNCCount,m_nLatNCCount,m_fLonNCBegin,m_fLatNCBegin,m_fLonNCDelta,m_fLatNCDelta,m_fMissingV,m_dArrayValue);
//
//	//		if(m_pProduct->Dump(container) == 0) bRes = true;
//	//		return bRes;
//	//	}
//	//}
//	//else if (m_graphicType == 6 || m_graphicType == 11 || m_graphicType == 16 || m_graphicType == 17 || m_graphicType == 18 || cLayer->GraphicsType() == 21)
//	//{
//	//	return DumpMicapsElement(container,m_MicapsHashData,m_DrawElement,cLayer->GraphicsType(),cLayer->GetAttributeList());
//	//}
//
//	return bRes;
//}

#if 0
int GradsProvider::GenerateCommand4ContourLine()
{
	//ClearVector();
	Initialize();

	//VectorLayer *vLayer = dynamic_cast<VectorLayer*>(DataProvider::ParentLayer());
	//if(NULL == vLayer) return 1;

	//vLayer->GraphicsType(1);
	//GraphicType(1);

	if (m_pProduct != NULL) delete m_pProduct;
	m_pProduct = new GeneralContour();
	if (m_pProduct != NULL)
	{
		gcmn.isContour = true;
		//m_pProduct->SetContourDraw(&m_gplistInterval);
		m_pProduct->Draw();
		return m_pProduct->Dump(/*vLayer->GetAttributeList()*/);
	}
	else
		return 1;
}
#endif

#if 0
int GradsProvider::GenerateCommand4ContourShaded()
{
	//ClearVector();
	Initialize();

	//VectorLayer *vLayer = dynamic_cast<VectorLayer*>(DataProvider::ParentLayer());
	//if(NULL == vLayer) return 1;

	//vLayer->GraphicsType(2);
	//GraphicType(2);

	//m_pGShaded  = new GeneralShaded();
	//if (m_pGShaded != NULL)
	//{
	//	gcmn.isShaded = true;
	//	m_pGShaded->SetColorCardIndex(ColorCardIndex);
	//	int drawResult = m_pGShaded->Draw(vLayer->GetAttributeList(), &m_gplistInterval);
	//	if (drawResult == 0)
	//		vLayer->SetColorIndex(*m_pGShaded->GetColorIndex());
	//	return drawResult;
	//}
	//else
	//	return 1;

	if (m_pProduct != NULL) delete m_pProduct;
	m_pProduct = new GeneralShaded();
	if (m_pProduct != NULL)
	{
		gcmn.isShaded = true;
		//m_pProduct->SetShadedDraw(&m_gplistInterval,ColorCardIndex,m_nPrecision,m_isRain);
		int drawResult = m_pProduct->Dump(/*vLayer->GetAttributeList()*/);
		//if (drawResult == 0)
		//	vLayer->SetColorIndex(*m_pProduct->GetColorIndex());
		return drawResult;
	}
	else
		return 1;
}
#endif

#if 0
int GradsProvider::GenerateCommand4UV_All(const char* strMode)
{
	ClearVector();

	if(strcmp(strMode,"vector") && strcmp(strMode,"barb") && strcmp(strMode,"stream"))
	{
		//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),tr("绘制风命令错误!"));
		//CDiskFileLog::WriteLog(tr("错误"), tr("GradsProvider::GenerateCommand4UV_All 绘制风命令错误！").toLocal8Bit());
		return 1;
	}

	VectorLayer *vLayer = dynamic_cast<VectorLayer*>(DataProvider::ParentLayer());
	if(NULL == vLayer) return 1;
	if (m_pProduct != NULL) delete m_pProduct;
	if (0 == strcmp(strMode,"vector"))
	{
		vLayer->GraphicsType(3);
		GraphicType(3);
		m_pProduct = new GeneralVector();
	}
	else if (0 == strcmp(strMode,"barb"))
	{
		vLayer->GraphicsType(4);
		GraphicType(4);
		m_pProduct = new GeneralBarb();
	}
	else if (0 == strcmp(strMode,"stream"))
	{
		vLayer->GraphicsType(5);
		GraphicType(5);
		m_pProduct = new GeneralStream();
	}
	if (m_pProduct != NULL)
	{
		gcmn.isVBS = true;
		return m_pProduct->Draw(vLayer->GetAttributeList());
	}
	else
		return 1;
}

int GradsProvider::GenerateCommand4Plot_Polyline_Bar(QLeaf* container, QList<QColor>* plistcolor,int nDim, float fLat, float fLon, const char *strMode, bool bXY2YX)
{
	ClearVector();

	m_pGPloyLineBar = new GeneralPolyLineBar();
	if (m_pGPloyLineBar != NULL)
	{
		gcmn.isLineBar = true;
		return m_pGPloyLineBar->Draw(container, plistcolor, nDim, vector_gradsInfo[0].nTime, vector_gradsInfo[0].nLevel, fLat, fLon, strMode, bXY2YX, m_nElementNum);
	}
	return 1;
}

int GradsProvider::GenerateCommand4SpaceProfile( double lon1, double lat1, double lon2, double lat2, /*VECTOR_UNIT* container*/QLeaf* container, const char* strMode, bool isXY2YX, QList<QColor>* plistcolor)
{
	ClearVector();

	m_pSProfile = new GeneralProfile();
	if(m_pSProfile != NULL)
	{
		gcmn.isProfile = true;
		return m_pSProfile->DrawSpace(vector_gradsInfo[0].nLevel, lon1, lat1, lon2, lat2, container, strMode, isXY2YX, m_nElementNum, plistcolor);
	}
	return 1;
}

int GradsProvider::GenerateCommand4LevelProfile( double lon1, double lat1, double lon2, double lat2, /*VECTOR_UNIT* container*/QLeaf* container, const char* strMode ,bool isXY2YX, QList<QColor>* plistcolor)
{
	ClearVector();

	m_pSProfile = new GeneralProfile();
	if(m_pSProfile != NULL)
	{
		gcmn.isProfile = true;
		return m_pSProfile->DrawLevelTime(vector_gradsInfo[0].nLevel,vector_gradsInfo[0].nTime, lon1, lat1, lon2, lat2, container, strMode, isXY2YX, m_nElementNum, plistcolor);
	}
	return 1;
}

int GradsProvider::GenerateCommand4TimeProfile( double lon1, double lat1, double lon2, double lat2, /*VECTOR_UNIT* container*/QLeaf* container, const char* strMode ,bool isXY2YX, QList<QColor>* plistcolor)
{
	ClearVector();

	m_pTProfile = new GeneralProfile();
	if(m_pTProfile != NULL)
	{
		gcmn.isProfile = true;
		return m_pTProfile->DrawTime(vector_gradsInfo[0].nTime, lon1, lat1, lon2, lat2, container, strMode, isXY2YX, m_nElementNum, plistcolor);
	}
	return 1;
}

int GradsProvider::GenerateCommand4SpaceTime( double lon1, double lat1, double lon2, double lat2, /*VECTOR_UNIT* container*/QLeaf* container, const char* strMode ,bool isXY2YX, QList<QColor>* plistcolor)
{
	ClearVector();

	m_pTProfile = new GeneralProfile();
	if(m_pTProfile != NULL)
	{
		gcmn.isProfile = true;
		return m_pTProfile->DrawTS(vector_gradsInfo[0].nTime, lon1, lat1, lon2, lat2, container, strMode, isXY2YX, m_nElementNum, plistcolor);
	}
	return 1;
}

int GradsProvider::GenerateCommand4Gird()
{
	ClearVector();

	VectorLayer *vLayer = dynamic_cast<VectorLayer*>(DataProvider::ParentLayer());
	if(NULL == vLayer) return 1;

	vLayer->GraphicsType(23);
	GraphicType(23);

	m_pProduct = new GeneralGird();
	m_pProduct->SetGirdDraw(vLayer);
	if (m_pProduct != NULL)
	{
		return m_pProduct->Draw(vLayer->GetAttributeList());
	}
	else
		return 1;
}
#endif

void GradsProvider::ClearVector()
{
	QVector<lineData*>::iterator iteratorLine;
	for (iteratorLine = ContourData.begin(); iteratorLine != ContourData.end();++iteratorLine)
	{
		delete *iteratorLine;
	}
	ContourData.clear();

	QVector<XFILLPOINT*>::Iterator iteratorXFP;
	for (iteratorXFP = ShadeData.begin(); iteratorXFP != ShadeData.end();++iteratorXFP)
	{
		delete[] (*iteratorXFP)->pPoint;
		delete *iteratorXFP;
	}
	ShadeData.clear();

	QVector<XFILLPOINT*>::Iterator iteratorRecXFP;
	for (iteratorRecXFP = ShadeRecData.begin(); iteratorRecXFP != ShadeRecData.end();++iteratorRecXFP)
	{
		delete *iteratorRecXFP;
	}
	ShadeRecData.clear();

	QVector<LineTextValue*>::Iterator iteratorCTVA;
	for (iteratorCTVA = ContourTextValueArray.begin(); iteratorCTVA != ContourTextValueArray.end();++iteratorCTVA)
	{
		delete *iteratorCTVA;
	}
	ContourTextValueArray.clear();

	QVector<WindBarbData*>::Iterator iteratorWBD;
	for (iteratorWBD = WindBarbDataArray.begin(); iteratorWBD != WindBarbDataArray.end();++iteratorWBD)
	{
		delete *iteratorWBD;
	}
	WindBarbDataArray.clear();

	vector_ContorValue.clear();

	gcmn.sig = 0;
	gacmd("clear",&gcmn,0);					// 清空绘图区内容
	gcmn.sig = 0;
	gacmd("reset",&gcmn,0);					// Grads系统环境
}

void GradsProvider::Initialize()
{
	ClearVector();

	//vector_gradsInfo.clear();

	//memset(gcmn.shdcls,0,sizeof(int)*256);
	//memset(gcmn.shdlvs,0,sizeof(double)*256);

	gcmn.sig = 0;
	gacmd("reinit",&gcmn,0);					// 初始化系统

	//if (!m_PTempText)
	//{
	//	delete m_PTempText;
	//	m_PTempText = NULL;
	//}
	
	//m_stPoint = NULL;
	//m_vectorElem.clear();
	//m_pStationSymbolPointArray.clear();
	//m_pStationTextureFontArray.clear();
	/////////////////////////////////////////地面填图
	//m_pWindSymbolPointArray.clear();			//存储风图标指针
	//m_pCloudSymbolPointArray.clear();			//存储云图标指针
	//m_pWeatherSymbolPointArray.clear();			//存储天气图标指针
	//m_stationpointArray.clear();
	//m_MicapsHashData.clear();

	//m_bLonLatChanged = false;
	//m_bhashOnce = true;
	if (gcmn.str_errorinfo)
	{
		delete[] gcmn.str_errorinfo;
		gcmn.str_errorinfo = NULL;
	}

	gcmn.nCmax = NULL;
	gcmn.nCmin = NULL;
	gcmn.nCint = NULL;

	gcmn.error = false;
}

//int GradsProvider::NetCDF2Grads(CElement * pElem1, CElement * pElem2, unsigned int nOrder)
//{
//	if((pElem1 == NULL && pElem2 == NULL) || nOrder == 0) return -1;
//
//	const char strMon[12][5]     =  {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};  // CTL文件月标记
//	const char strTimeuint[5][3] =  {"yr","mo","dy","hr","mn"};                                                 // CTL文件 时间增量步长单位: 分钟、小时、天、月、年
//
//	FILE *fp = NULL;
//
//	char strCTL[100] ="";
//	char strDAT[100] ="";
//	char strTimelabel[100] = "";
//	char strCMD[128] = "";
//
//	int nx, ny, nz, nz1, nz2, nt, nt1, nt2, nvars;
//	int i,j,k,l;
//	float fValue;
//	float *pData = NULL,*pData2 = NULL;
//	float f_lonstart,f_lonstep,f_latstart,f_latstep;
//
//	nz2 = 0;
//	nt2 = 0;
//	//*******************************  Write *.dat    the data file******************************************
//
//	gcmn.sig = 0;
//	gacmd("q file",&gcmn,0);
//
//	strcpy_s(strDAT,(pElem1->SelectedPeriod()).m_begin.toString("yyyyMMddhhmmss").toLatin1());
//	strcpy_s(strCTL,strDAT);
//	//m_gradsFileName = strCTL;          // 将生成的临时文件的文件名存储到成员变量m_gradsFileName中
//	strcat_s(strDAT,".dat");
//	strcat_s(strCTL,".ctl");
//
//	//remove(strCTL);
//	//remove(strDAT);
//
//	fopen_s(&fp, strDAT, "wb");
//	if (NULL == fp)
//	{
//		printf ("cannot open write file");
//		////CDiskFileLog::WriteLog("警告", tr("GradsProvider::NetCDF2Grads 无法写文件！").toLocal8Bit());
//		return -1;
//	}
//
//	if(pElem1 != NULL)
//	{
//		nx     = pElem1->SelectedLon().m_count;
//		ny     = pElem1->SelectedLat().m_count;
//		nz1   = pElem1->Level().m_selected.size();
//		if (nz1 == 0)
//		{
//			nz1 = 1;
//		}
//		nt1		= pElem1->SelectedPeriod().m_count;
//		if (nt1 == 0)
//		{
//			nt1 = 1;
//		}
//		pData = pElem1->Data().GetDataArray(0);
//		//long *nELem1 = pElem1->Data().pSize;
//
//		f_lonstart = pElem1->SelectedLon().m_begin <= pElem1->SelectedLon().m_end ? pElem1->SelectedLon().m_begin : pElem1->SelectedLon().m_end;
//		f_lonstep  = pElem1->SelectedLon().m_delt >= 0 ? pElem1->SelectedLon().m_delt : -1 * pElem1->SelectedLon().m_delt;
//		f_latstart = pElem1->SelectedLat().m_begin <= pElem1->SelectedLat().m_end ? pElem1->SelectedLat().m_begin : pElem1->SelectedLat().m_end;
//		f_latstep  = pElem1->SelectedLat().m_delt >= 0 ? pElem1->SelectedLat().m_delt : -1 * pElem1->SelectedLat().m_delt;
//		nvars = 1;
//
//		nz = nz1;
//		nt = nt1;
//	}
//	if(pElem2 != NULL)
//	{
//		nz2    = pElem2->Level().m_selected.size();
//		if (nz2 == 0)
//		{
//			nz2 = 1;
//		}
//		nt2    = pElem2->SelectedPeriod().m_count;
//		if (nt2 == 0)
//		{
//			nt2 = 1;
//		}
//		pData2 = pElem2->Data().GetDataArray(0);
//
//		nz = nz1 > nz2 ? nz1 : nz2;
//		nt = nt1 > nt2 ? nt1 : nt2;
//
//		nvars = 2;
//	}
//
//	for(int ne = 0; ne < 1; ne++)
//	{
//		for(i = 0; i < nt; i++)
//		{
//			for(j = 0; j < nz; j++)          // write pElem1
//			{
//				for(k = ny - 1; k >=0; k--)   // 南北顺序相反，要颠倒过来
//				{
//					for(l= 0; l < nx; l++)
//					{
//						fValue = (float)pData[i * nz1 * ny * nx + j * ny * nx + k * nx + l];
//						fwrite(&fValue, sizeof(float), 1, fp);
//						////qDebug()<<fValue;
//					}
//				}
//			}
//
//			for(j = 0; j < nz2; j++)          // write pElem2
//			{
//				for(k = ny - 1; k >=0; k--)
//				{
//					for(l= 0; l < nx; l++)
//					{
//						fValue = pData2[i * nz2 * ny * nx + j * ny * nx + k * nx + l];
//						fwrite(&fValue, sizeof(float), 1, fp);
//					}
//				}
//			}
//		} // endfor nt
//	} // endfor ne
//
//	fclose(fp);
//
//	//*******************************  Write *.ctl file ****************************************
//
//	fopen_s(&fp, strCTL, "wb");
//	if (NULL == fp)
//	{
//		printf ("cannot open write file");
//		////CDiskFileLog::WriteLog("警告", tr("GradsProvider::NetCDF2Grads 无法写文件！").toLocal8Bit());
//		return -1;
//	}
//
//	fprintf_s(fp,"dset ^%s\n",strDAT);
//	fprintf_s(fp,"title \"netcdf to grads binary\"\n");
//	fprintf_s(fp,"undef %lf\n", pElem1->MissingValue());
//	fprintf_s(fp,"xdef %d linear %f %f\n",nx, f_lonstart,f_lonstep);   // from east to west
//	fprintf_s(fp,"ydef %d linear %f %f\n",ny, f_latstart,f_latstep);     // from south to north
//	if (nz == 1)
//	{
//		fprintf_s(fp,"zdef %d linear 1 1\n",nz);
//	}
//	else
//	{
//		fprintf_s(fp,"zdef %d levels\n",nz);
//
//		if(nz1 >= nz2)
//		{
//			for(i = 0; i < nz1 - 1; i++)
//				fprintf_s(fp,"%d ",pElem1->Level().m_selected[i]);
//
//			if(nz1 > 1)
//				fprintf_s(fp,"%d\n",pElem1->Level().m_selected[nz1 - 1]);
//			else
//				fprintf_s(fp,"\n");
//		}
//		else
//		{
//			for(i = 0; i < nz2 - 1; i++)
//				fprintf_s(fp,"%d ",pElem2->Level().m_selected[i]);
//
//			if(nz2 > 1)
//				fprintf_s(fp,"%d\n",pElem2->Level().m_selected[nz2 - 1]);
//			else
//				fprintf_s(fp,"\n");
//		}
//	}
//
//	QDate startDate(pElem1->SelectedPeriod().m_begin.date());
//	sprintf(strTimelabel, "linear %dZ%d%s%d %d%s",0, startDate.day() ,strMon[startDate.month()-1], startDate.year(),pElem1->SelectedPeriod().GetTimeScale().GetValue(), strTimeuint[pElem1->SelectedPeriod().GetTimeScale().GetUnit() - 1] );// hhZddmmmyyyy  vvkk  小时Z日期月份年  时间增量步长（需要修改此处）pElem1->GetTimeScale().m_value
//
//	fprintf_s(fp,"tdef %d %s\n",nt, strTimelabel);
//
//	if(nz2 > 0) nvars = 2;
//	fprintf_s(fp,"vars %d\n",nvars);
//	if(nvars == 1)
//	{
//		if(nz1 == 1) nz1 =0;
//		fprintf_s(fp,"%s %d  99 %s\n","va", nz1, pElem1->name().toLatin1().data());
//	}
//	else
//	{
//		if(nz1 == 1) nz1 =0;
//		fprintf_s(fp,"%s %d  99 %s\n","va", nz1, pElem1->name().toLatin1().data());
//		if(nz2 == 1) nz2 =0;
//		fprintf_s(fp,"%s %d  99 %s\n","vb", nz2, pElem2->name().toLatin1().data());
//	}
//
//	fprintf_s(fp,"endvars\n");
//
//	fclose(fp);
//
//	//*******************************  open the grads binary file output by above code **********************
//	gagx(&gcmn);
//	strcpy_s(strCMD, "open ");
//	strcat_s(strCMD, strCTL);
//	gcmn.sig = 0;
//	if (gacmd(strCMD,&gcmn,0) == 1)
//	{
//		printf("open error");
//		////CDiskFileLog::WriteLog("错误", tr("GradsProvider::NetCDF2Grads 打开文件错误！").toLocal8Bit());
//		return 1;
//	}
//	gcmn.sig = 0;
//	gacmd("q file",&gcmn,0);
//
//	//DumpInformation();
//
//	//*******************************  Draw figures according to  nOrder **********************
//
//	return 0;
//}

//int GradsProvider::NetCDF2Array(CElement * pElem1, CElement * pElem2, unsigned int nOrder, QString strCalculate)
int GradsProvider::CElement2GradsData(
	size_t iLonNCCount, 
	size_t iLatNCCount,  
	size_t iTimeNCCount, 
	size_t iLevelNCCount, 
	float fLonBegin, 
	float fLonEnd,
	float fLonDelta,
	float fLatBegin, 
	float fLatEnd,
	float fLatDelta,
	float fMissingValue,
	float fLevelBegin,
	float fLevelEnd,
	float fLevelDelta, 
	std::vector<float>& fLevelVector, 
	std::string& strTimeBegin,
	std::string& strTimeEnd,
	int iTimeDelta,
	double* dData, 
	int nModel, 
	int useNineP)
{
	const char strMon[12][5]     =  {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};  // CTL文件月标记
	const char strTimeuint[5][3] =  {"yr","mo","dy","hr","mn"};                                                 // CTL文件 时间增量步长单位: 分钟、小时、天、月、年

	char strDAT[100] ="";
	char strTimelabel[100] = "";
	char strTemp[20] = "";
	char *pstrCTL[200];

	int nElementIndex;
	int nx, ny, nz, nt;
	float f_missingdata;

	bool isCalculate = false;
	bool isNormalMulti = false;

	for(int i = 0; i < 200; i++)
		pstrCTL[i] = m_strCTL[i];

	/*m_nLonNCCount	= pListCElement->at(0)->SelectedLon().m_count		>0 ? pListCElement->at(0)->SelectedLon().m_count	:1;
	m_nLatNCCount	= pListCElement->at(0)->SelectedLat().m_count		>0 ? pListCElement->at(0)->SelectedLat().m_count	:1;
	m_nTimeNCCount	= pListCElement->at(0)->SelectedPeriod().m_count	>0 ? pListCElement->at(0)->SelectedPeriod().m_count	:1;
	m_nLevelNCCount	= pListCElement->at(0)->Level().m_selected.size()	>0 ? pListCElement->at(0)->Level().m_selected.size():1;*/

	m_nLonNCCount = iLonNCCount;
	m_nLatNCCount	= iLatNCCount;
	m_nTimeNCCount = iTimeNCCount;
	m_nLevelNCCount = iLevelNCCount;

	m_fLonNCBegin	= fLonBegin	<= fLonEnd ? fLonBegin	: fLonEnd;
	m_fLonNCDelta	= fLonDelta	>= 0	? fLonDelta : -1 * fLonDelta;
	m_fLatNCBegin	= fLatBegin	<= fLatEnd ? fLatBegin : fLatEnd;
	m_fLatNCDelta	= fLatDelta	>= 0	? fLatDelta : -1 * fLatDelta;

//	m_nLonNCCount = m_nLonNCCount-1;
	m_fMissingV = fMissingValue;

	bool  NorthToSouth = true;

# if 0
	for (nElementIndex = 0;nElementIndex < pListCElement->count();nElementIndex++)
	{
		if (nElementIndex == 0)
		{
			if(pListCElement->at(nElementIndex)->SelectedLat().m_begin <= pListCElement->at(0)->SelectedLat().m_end)
			{
				NorthToSouth = false;
			}
		}
		else
		{
			if(pListCElement->at(nElementIndex)->SelectedLat().m_begin <= pListCElement->at(0)->SelectedLat().m_end)
			{
				if (NorthToSouth != false)
				{
					return 1;
				}
			}
		}
	}
#endif

	//m_isRain = pListCElement->at(0)->GetRCheck();

	nx	= m_nLonNCCount;
	ny	= m_nLatNCCount;
	nz	= m_nLevelNCCount;
	nt	= m_nTimeNCCount;
	f_missingdata = m_fMissingV;

	/*if(nModel == 1)
	{
			if (!Array4NormalElement(pListCElement,nx,ny,nz,nt,f_missingdata,NorthToSouth))
			{
				return 1;
			}
	}*/

	//if (useNineP)
	//{
	//	m_NinePoint.Execute(m_pTempElement,&m_dArrayValue,m_pTempElement->MissingValue());
	//}

	//*******************************  Write *.ctl file ****************************************

	QString timestr = "Time";//(m_pTempElement->SelectedPeriod()).m_begin.toString("yyyyMMddhhmmss");
	strcpy(strDAT,timestr.toLatin1());
	strcat(strDAT,".dat");
	int j = 0;
	char *chrTemp = "abcdefghijklmnopqrstuvwxyz";
	memset(m_strCTL,0,40000);
	sprintf(m_strCTL[j++],"dset ^%s\n",strDAT);
	strcpy(m_strCTL[j++],"title \"netcdf to Array\"\n");
	sprintf(m_strCTL[j++],"undef %lf\n", m_fMissingV);
	sprintf(m_strCTL[j++],"xdef %d linear %f %f\n", m_nLonNCCount, m_fLonNCBegin, m_fLonNCDelta);   // from east to west
	sprintf(m_strCTL[j++],"ydef %d linear %f %f\n", m_nLatNCCount, m_fLatNCBegin, m_fLatNCDelta);   // from south to north
	if (m_nLevelNCCount == 1)
	{
		sprintf(m_strCTL[j++],"zdef %d linear 1 1\n",m_nLevelNCCount);
	}
	else
	{
		sprintf(m_strCTL[j++],"zdef %d levels\n",m_nLevelNCCount);

		std::vector<float>::iterator it;
		//QHash<int, float>::const_iterator it;
		for(it = fLevelVector.begin(); it != fLevelVector.end(); it++)
		{
			sprintf(strTemp,"%.3lf ", *it);
			strcat(m_strCTL[j],strTemp);
		}
		//it = m_pTempElement->Level().m_selected.constEnd()-1;
		sprintf(strTemp,"\n");
		strcat(m_strCTL[j++],strTemp);
	}
	if (strTimeBegin.size() != 0)
	{
		QDate startDate = QDate::fromString(QString::fromStdString(strTimeBegin));
		if (startDate.year() > 2050 || startDate.year()< 0001)
			sprintf(strTimelabel, "linear 0Z1JAN1948 1mo");// hhZddmmmyyyy  vvkk  小时Z日期月份年  时间增量步长（需要修改此处）pElem1->GetTimeScale().m_value
		else
			sprintf(strTimelabel, "linear %dZ%d%s%d %d%s",0, startDate.day() ,strMon[startDate.month()-1], startDate.year(), iTimeNCCount, strTimeuint[iTimeDelta - 1] );// hhZddmmmyyyy  vvkk  小时Z日期月份年  时间增量步长（需要修改此处）pElem1->GetTimeScale().m_value
	}
	else
	{
		sprintf(strTimelabel, "linear 0Z1JAN1948 1mo");
	}
	sprintf(m_strCTL[j++],"tdef %d %s\n",m_nTimeNCCount, strTimelabel);
	sprintf(m_strCTL[j++],"vars %d\n",1);
	if (isCalculate)
	{
		sprintf(m_strCTL[j++],"va %d 99 isCalculate\n", m_nLevelNCCount);
	}
	else
	{
		sprintf(m_strCTL[j++],"v%c %d 99 %s\n", 'A', m_nLevelNCCount, "FUCK");
	}
	strcpy(m_strCTL[j++],"endvars\n");
	strcpy(m_strCTL[j],"");
	//for (int x = 0;x < 100;x++)
	//{
	//	//qDebug()<<m_strCTL[x];
	//}

	//*******************************  open the grads binary file output by above code **********************
	gagx(&gcmn);
	gcmn.sig = 0;
	if (ccsopen(pstrCTL, &gcmn)== 1)
	{
		////CDiskFileLog::WriteLog(tr("警告"), tr("GradsProvider::NetCDF2Array 文件载入错误！").toLocal8Bit());//写日志--renxue
		////CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),tr("NETCDF2Array：文件载入错误!"));
		return 1;
	}
	gcmn.pfi1->pArrayValuebuf = dData;

	//printf("**********************");
	//gcmn.sig = 0;
	//gacmd("q file",&gcmn,0);

	//DumpInformation();

	return (0);
}

#if 0
int GradsProvider::Station2Array( CElement * pElem, unsigned int nOrder)
{
	//Initialize();
	if(pElem == NULL  || nOrder == 0)  return  1;

	const char strMon[12][5]     =  {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};  // CTL文件月标记
	const char strTimeuint[5][3] =  {"yr","mo","dy","hr","mn"};                                                 // CTL文件 时间增量步长单位: 分钟、小时、天、月、年

	char strDAT[100] ="";
	char strTimelabel[100] = "";
	char strTemp[20] = "";
	char *pstrCTL[100];

	int nx, ny, nz, nt, nvars;
	int i, j;
	float f_lonstart,f_lonstep,f_latstart,f_latstep;

	for(i = 0; i < 100; i++)
		pstrCTL[i] = m_strCTL[i];

	if (!(pElem->GetCInterval().isEmpty())) 
	{
	//	m_gplistInterval = pElem->GetCInterval();
	//}
	//else{
	//	eleDes *ed = new eleDes;
	//	ed->_Name = pElem->name();
	//	ed->_Type = ysz;
	//	//m_gplistInterval = ReadEleColor::GetColorList(*ed);
	//	if (ed) delete ed;
	}
	m_isRain = pElem->GetRCheck();

	//*******************************  Write *.dat    the data file******************************************

	strcpy(strDAT,(pElem->SelectedPeriod()).m_begin.toString("yyyyMMddhhmmss").toLatin1());
	strcat(strDAT,".dat");

	nx = pElem->SelectedLon().m_count;
	ny = pElem->SelectedLat().m_count;
	nz = pElem->Level().m_selected.size();
	if (nz == 0)
	{
		nz = 1;
	}
	nt = pElem->SelectedPeriod().m_count;
	if (nt == 0)
	{
		nt = 1;
	}

	f_lonstart = pElem->SelectedLon().m_begin <= pElem->SelectedLon().m_end ? pElem->SelectedLon().m_begin : pElem->SelectedLon().m_end;
	f_lonstep  = pElem->SelectedLon().m_delt >= 0 ? pElem->SelectedLon().m_delt : -1 * pElem->SelectedLon().m_delt;
	f_latstart = pElem->SelectedLat().m_begin <= pElem->SelectedLat().m_end ? pElem->SelectedLat().m_begin : pElem->SelectedLat().m_end;
	f_latstep  = pElem->SelectedLat().m_delt >= 0 ? pElem->SelectedLat().m_delt : -1 * pElem->SelectedLat().m_delt;
	nvars = 1;

	//*******************************  Write *.ctl file ****************************************

	j = 0;
	memset(m_strCTL,0,10000);
	sprintf(m_strCTL[j++],"dset ^%s\n",strDAT);
	strcpy(m_strCTL[j++],"title \"Station to Array\"\n");
	sprintf(m_strCTL[j++],"undef %lf\n", pElem->MissingValue());
	sprintf(m_strCTL[j++],"xdef %d linear %f %f\n",nx, f_lonstart,f_lonstep);   // from east to west
	sprintf(m_strCTL[j++],"ydef %d linear %f %f\n",ny, f_latstart,f_latstep);     // from south to north
	if (nz == 1)
	{
		sprintf(m_strCTL[j++],"zdef %d linear 1 1\n",nz);
	}
	else
	{
		sprintf(m_strCTL[j++],"zdef %d levels\n",nz);

		for(i = 0; i < nz-1; i++)
		{
			sprintf(strTemp,"%d ",(int)pElem->Level().m_selected.value(i));
			//sprintf(strTemp,"%f ",pElem->Level().m_selected.value(i));
			strcat(m_strCTL[j],strTemp);
		}
		sprintf(strTemp,"%d\n",(int)pElem->Level().m_selected.value(i));
		//sprintf(strTemp,"%f\n",pElem->Level().m_selected.value(i));
		strcat(m_strCTL[j++],strTemp);
	}

	QDate startDate(pElem->SelectedPeriod().m_begin.date());
	if (startDate.year() > 2050 || startDate.year()< 0001)
		sprintf(strTimelabel, "linear 0Z1JAN1948 1mo");
	else
		sprintf(strTimelabel, "linear %dZ%d%s%d %d%s",0, startDate.day() ,strMon[startDate.month()-1], startDate.year(),pElem->SelectedPeriod().GetTimeScale().GetValue(), strTimeuint[pElem->SelectedPeriod().GetTimeScale().GetUnit() - 1] );// hhZddmmmyyyy  vvkk  小时Z日期月份年  时间增量步长（需要修改此处）pElem1->GetTimeScale().m_value

	sprintf(m_strCTL[j++],"tdef %d %s\n",nt, strTimelabel);

	sprintf(m_strCTL[j++],"vars %d\n",nvars);
	if(nvars == 1)
	{
		if(nz == 1) nz =0;
		sprintf(m_strCTL[j++],"%s %d  99 %s\n","va", nz, pElem->name().toLatin1().data());
	}
	//else
	//{
	//	if(nz == 1) nz = 0;
	//    sprintf(m_strCTL[j++],"%s %d  99 %s\n","va", nz, pElem->name().toLatin1().data());
	//}
	strcpy(m_strCTL[j++],"endvars\n");
	strcpy(m_strCTL[j],"");

	//*******************************  open the grads binary file output by above code **********************
	gagx(&gcmn);
	gcmn.sig = 0;
	if (ccsopen(pstrCTL, &gcmn)== 1)
	{
		printf("open error");
		//CDiskFileLog::WriteLog("警告", tr("GradsProvider::NetCDF2Array 文件打开失败！").toLocal8Bit());//写日志--renxue
		return 1;
	}
	gcmn.pfi1->pArrayValuebuf = m_dArrayValue;

	gcmn.sig = 0;
	gacmd("q file",&gcmn,0);

	//DumpInformation();

	return (0);
}

int GradsProvider::ReloadData()
{
	char *pstrCTL[100];
	int i = 0;
	for(i = 0; i <100; i++) pstrCTL[i] = m_strCTL[i];
	if(!strcmp(pstrCTL[0],"") || NULL == m_dArrayValue )  return 1;

	gagx(&gcmn);
	gcmn.sig = 0;
	if (ccsopen(pstrCTL, &gcmn)== 1)
	{
		printf("open error");
		//CDiskFileLog::WriteLog("警告", tr("GradsProvider::NetCDF2Array 文件打开失败！").toLocal8Bit());//写日志--renxue
		return 1;
	}
	gcmn.pfi1->pArrayValuebuf = m_dArrayValue;

	//DumpInformation();

	return (0);
}

int GradsProvider::OpenMicaps3and4( const QString& filename )
{
	if (filename.isEmpty())
	{
		return 1;
	}
	//strcpy(m_file, "NULL");
	//strcpy(m_fileName, "NULL");
	//strcpy(m_filelast, "NULL");
	m_file.clear();
	m_file = filename;
	//strcpy(m_file,filename.toLocal8Bit().data());       //中文乱码
	//GetFileLastName(m_file, m_filelast);
	//GetFileName(m_file, m_fileName);

	char	strTitle[10];			// diamond
	int		iMicapsType;			// Micaps数据类型
	FILE* fp = NULL;

	/*if (!strcmp(m_filelast.toLocal8Bit().data(),"000"))
	{*/
	fp = fopen(m_file.toLocal8Bit().data(), "r");
	if (fp == NULL)
	{
		printf ("cannot open file");
		return(-1);
	}

	fscanf(fp,"%s %d", strTitle, &iMicapsType);
	if(iMicapsType < 3 || iMicapsType > 4)
	{
		printf ("Micaps type is not 3 or 4.0\n");
		fclose(fp);
		fp = NULL;
		return(1);
	}

	fclose(fp);
	fp = NULL;

	strcpy(m_Command,"miopen ");
	strcat(m_Command,m_file.toLocal8Bit().data());

	gcmn.sig = 0;
	if( gacmd(m_Command,&gcmn,0) == 1 )
	{
		printf("Open error\n");
		//CDiskFileLog::WriteLog("警告", tr("GradsProvider::NetCDF2Array 文件打开失败！").toLocal8Bit());//写日志--renxue
		return 1;
	}

	return 0;
}

bool GradsProvider::DumpGroundFillElement(QRenderable* container,QHash<QString, CElement*> Elementhash,QString strElement,const int nModel)
{

	//m_PTempText = new QBitmapText;
	//初始化图标指针数组
	//InitSymbolPointArray("All");
	//初始化数据
	CData tempData[10];
	InitAllCDataArray(Elementhash,tempData,tr("地面填图"));
	//--------------------------------------------------------------------------------
	InitSymbolPointArray("Wind");
	InitSymbolPointArray("Cloud");

	DrawMicapsGroundMap *m_pDrawGroundMap = new DrawMicapsGroundMap(container,m_pSymbolPlugin,false);
	QVector3DArray *poriginalArray = m_pDrawGroundMap->OriginalArray();

	for (int i = 0;i<tempData[0].DataBlockDimensionSize(0,0);i++)
	{
		float flon,flat;
		flon = tempData[0].GetDataArray(0)[i];
		flat = tempData[1].GetDataArray(0)[i];
		if (!tempData[2].GetDataArray(0)) return false;  // WUENPING
		if (flon>= m_MDAttribute.f_lonstart && flon<= m_MDAttribute.f_lonend  &&
			flat>= m_MDAttribute.f_latstart && flat<= m_MDAttribute.f_latend)
		{
			QVector3D glPoint(flon,flat,0);
			poriginalArray->push_back(glPoint);
			if (i<m_lodList.count())
			{
				m_pDrawGroundMap->AddLodToList(m_lodList.at(i));
				m_pDrawGroundMap->SetAllDataArray(glPoint,tempData[5].GetDataArray(0)[i]
				,tempData[6].GetDataArray(0)[i],tempData[7].GetDataArray(0)[i],tempData[8].GetDataArray(0)[i],tempData[9].GetDataArray(0)[i]);

				if(m_MDAttribute.b_dWSymbol)
					DrawWindSymbol(i,tempData[0].GetDataArray(0)[i],tempData[1].GetDataArray(0)[i],tempData[3].GetDataArray(0)[i],tempData[4].GetDataArray(0)[i]);//风标
				if(m_MDAttribute.b_dTCloud)	
					DrawCloudSymbol(i,tempData[0].GetDataArray(0)[i],tempData[1].GetDataArray(0)[i],tempData[2].GetDataArray(0)[i]);//总云量

				Point* point = new Point;		
				point->SetRenderLODindex(m_lodList.at(i));
				point->getVectexArray()->push_back(glPoint);	
				point->getColorArray()->push_back(QVector4D(0.0f,0.0f,0.0f,1.0f));
				point->OriginalArray()->push_back(glPoint);
				point->SetColorArray();
				m_stationpointArray.push_back(point);
			}			
			
		}
	}
	m_pDrawGroundMap->SetBshowData(m_MDAttribute.b_dFlogP,m_MDAttribute.b_d3HourPre,m_MDAttribute.b_dSeaPre,m_MDAttribute.b_dTemperature,m_MDAttribute.b_dCurWeather);
	m_pDrawGroundMap->SetBakData();
	container->InsertRenderable(m_pDrawGroundMap);
	
	for (int i=0;i<m_stationpointArray.count();i++)
	{
		m_stationpointArray.at(i)->SetBakData();
		container->InsertRenderable(m_stationpointArray.at(i));
	}
	
	for (int mm = 0;mm<m_pWindSymbolPointArray.count();mm++)
	{
		m_pWindSymbolPointArray.at(mm)->SetBakData();
		container->InsertRenderable(m_pWindSymbolPointArray.at(mm));
	}
	for (int mm = 0;mm<m_pCloudSymbolPointArray.count();mm++)
	{
		m_pCloudSymbolPointArray.at(mm)->SetBakData();
		//m_pCloudSymbolPointArray.at(mm)->DisplayList(true);
		container->InsertRenderable(m_pCloudSymbolPointArray.at(mm));
	}

	return true;
}

bool GradsProvider::DumpAirFillElement(QRenderable* container,QHash<QString, CElement*> Elementhash,QString strElement,const int nModel)
{
	if (!m_PTempText)
		{
				m_PTempText = new QBitmapText;
		}
			
			CData tempData[7];
			InitAllCDataArray(Elementhash,tempData,tr("高空填图"));

			m_pAirWindSymbolPointMap.clear();

			DrawMicapsGroundMap *m_pDrawGroundMap = new DrawMicapsGroundMap(container,m_pSymbolPlugin,true);
			QVector3DArray *poriginalArray = m_pDrawGroundMap->OriginalArray();

			for (int i=0;i<tempData[0].DataBlockDimensionSize(0,0);i++)
			{
				float flon,flat,fwindSp,fwindDr;
				if (!tempData[0].GetDataArray(0)) return false;
				flon = tempData[0].GetDataArray(0)[i];
				if (!tempData[1].GetDataArray(0)) return false;
				flat = tempData[1].GetDataArray(0)[i];
				if (!tempData[2].GetDataArray(0)) return false;  // WUENPING

				fwindDr = tempData[2].GetDataArray(0)[i];
				fwindSp = tempData[3].GetDataArray(0)[i];
				if (flon>= m_MDAttribute.f_lonstart && flon<= m_MDAttribute.f_lonend  &&
					flat>= m_MDAttribute.f_latstart && flat<= m_MDAttribute.f_latend)
				{
					QVector3D glPoint(flon,flat,0);
					poriginalArray->push_back(glPoint);
					float fTmFgGap,fHeight,fTemperature;
					fTmFgGap = tempData[4].GetDataArray(0)[i];
					fHeight = tempData[5].GetDataArray(0)[i];
					fTemperature = tempData[6].GetDataArray(0)[i];
					m_pDrawGroundMap->SetAllAirDataArray(glPoint,fTmFgGap,fHeight,fTemperature);

					if(m_MDAttribute.b_gWSymbol)
					{
						int nText = (int)fwindSp;
						if (nText != 9999 && nText != -999 && nText != 99999)
						{
							SymbolPoint * pSymbolpoint = NULL;
							QMap <float , SymbolPoint*>::Iterator it;
							it = m_pAirWindSymbolPointMap.find(fwindSp);
							if(it != m_pAirWindSymbolPointMap.end())
							{
								pSymbolpoint = it.value();
								pSymbolpoint->SetPosition(glPoint);
								pSymbolpoint->SetAngle(fwindDr);
							}
							else
							{
								SymbolPoint * pSymbolpoint = new SymbolPoint;
								m_pSymbol = m_pSymbolPlugin->GetSymbol(-1,fwindSp);
								if (m_pSymbol)
								{
									pSymbolpoint->SetPointStyle(m_pSymbol);
									pSymbolpoint->SetPosition(glPoint);
									pSymbolpoint->SetAngle(fwindDr);
									m_pAirWindSymbolPointMap.insert(fwindSp,pSymbolpoint);
								}
							}
						}
					}
					/*	
					StationValues sv;
					for(int it =0;it<m_ElementsName.size();it++)
					{
					CElement *pTempEle = Elementhash.value(m_ElementsName.at(it));
					if(NULL != pTempEle)
					sv.SetStationValue(m_ElementsName.at(it),pTempEle->Data().GetDataArray(0)[i]);
					}
					m_PTempText->StationValueArray()->push_back(sv);*/
				}
			}
			m_pDrawGroundMap->SetBshowAirData(m_MDAttribute.b_gTempFlogPGap,m_MDAttribute.b_gHight,m_MDAttribute.b_gTemperature);
			m_pDrawGroundMap->SetBakData();
			container->InsertRenderable(m_pDrawGroundMap);

			QMap <float , SymbolPoint*>::Iterator it;
			for (it = m_pAirWindSymbolPointMap.begin();it != m_pAirWindSymbolPointMap.end();it++)
			{
				it.value()->SetBakData();
				container->InsertRenderable(it.value());
			}

	  return true;
}

bool GradsProvider::DumpWindElement(QRenderable* container,QHash<QString, CElement*> Elementhash,QString strElement,const int nModel)
{
	//{
	//	m_stPoint = new Point;
	//	InitSymbolPointArray("Wind");
	//	CData tempData[4];
	//	InitAllCDataArray(Elementhash,tempData,strElement);
	//	for (int i=0;i<tempData[0].DataBlockDimensionSize(0,0);i++)
	//	{
	//		float flon,flat,fwindSp,fwindDr;
	//		flon = tempData[0].GetDataArray(0)[i];
	//		flat = tempData[1].GetDataArray(0)[i];
	//		fwindDr = tempData[2].GetDataArray(0)[i];
	//		fwindSp = tempData[3].GetDataArray(0)[i];
	//		if (flon>= m_MDAttribute.f_lonstart && flon<= m_MDAttribute.f_lonend  &&
	//			flat>= m_MDAttribute.f_latstart && flat<= m_MDAttribute.f_latend)
	//		{
	//			DrawWindSymbol(flon,flat,fwindDr,fwindSp);//风标
	//			if(m_MDAttribute.b_StationP)
	//				DrawStationPoint(flon,flat);
	//		}
	//	}
	//	for (int mm = 0;mm<m_pWindSymbolPointArray.count();mm++)
	//	{
	//		m_pWindSymbolPointArray.at(mm)->SetBakData();
	//		container->InsertRenderable(m_pWindSymbolPointArray.at(mm));
	//	}
	//	m_stPoint->SetColorArray();
	//	m_stPoint->SetBakData();
	//	container->InsertRenderable(m_stPoint);

	//}
	//else if(strElement == "总云量")
	//{
	//	m_stPoint = new Point;
	//	InitSymbolPointArray("Cloud");
	//	CData tempData[3];
	//	InitAllCDataArray(Elementhash,tempData,"总云量");
	//	for (int i=0;i<tempData[0].DataBlockDimensionSize(0,0);i++)
	//	{
	//		float flon,flat,fcloud;
	//		flon = tempData[0].GetDataArray(0)[i];
	//		flat = tempData[1].GetDataArray(0)[i];
	//		fcloud = tempData[2].GetDataArray(0)[i];
	//		if (flon>= m_MDAttribute.f_lonstart && flon<= m_MDAttribute.f_lonend  &&
	//			flat>= m_MDAttribute.f_latstart && flat<= m_MDAttribute.f_latend)
	//		{
	//			DrawCloudSymbol(flon,flat,fcloud);//总云量
	//			if(m_MDAttribute.b_StationP)
	//				DrawStationPoint(flon,flat);
	//		}
	//	}
	//	for (int mm = 0;mm<m_pCloudSymbolPointArray.count();mm++)
	//	{
	//		m_pCloudSymbolPointArray.at(mm)->SetBakData();
	//		//m_pCloudSymbolPointArray.at(mm)->DisplayList(true);
	//		container->InsertRenderable(m_pCloudSymbolPointArray.at(mm));
	//	}
	//	m_stPoint->SetColorArray();
	//	m_stPoint->SetBakData();
	//	container->InsertRenderable(m_stPoint);
	//}
	//else if(strElement == "现在天气" || strElement == "天气现象1" || strElement == "天气现象2")
	//{
	//	m_stPoint = new Point;
	//	InitSymbolPointArray("Weather");
	//	CData tempData[3];
	//	InitAllCDataArray(Elementhash,tempData,strElement);
	//	for (int i=0;i<tempData[0].DataBlockDimensionSize(0,0);i++)
	//	{
	//		float flon,flat,fwether;
	//		flon = tempData[0].GetDataArray(0)[i];
	//		flat = tempData[1].GetDataArray(0)[i];
	//		fwether = tempData[2].GetDataArray(0)[i];
	//		if (flon>= m_MDAttribute.f_lonstart && flon<= m_MDAttribute.f_lonend  &&
	//			flat>= m_MDAttribute.f_latstart && flat<= m_MDAttribute.f_latend)
	//		{
	//			DrawWeatherSymbol(flon,flat,fwether,0);//现在天气
	//			if(m_MDAttribute.b_StationP)
	//				DrawStationPoint(flon,flat);
	//		}
	//	}
	//	for (int mm = 0;mm<m_pWeatherSymbolPointArray.count();mm++)
	//	{
	//		m_pWeatherSymbolPointArray.at(mm)->SetBakData();
	//		//m_pCloudSymbolPointArray.at(mm)->DisplayList(true);
	//		container->InsertRenderable(m_pWeatherSymbolPointArray.at(mm));
	//	}
	//	m_stPoint->SetColorArray();
	//	m_stPoint->SetBakData();
	//	container->InsertRenderable(m_stPoint);
	//}
	return true;
}

bool GradsProvider::DumpInterPolation(QRenderable* container,QHash<QString, CElement*> Elementhash,QString strElement,const int nModel)
{
	if (strElement == tr("风标"))
	{
		return false;
	}

	CElement *pElem =Elementhash.value(strElement);
	if (NULL == pElem)
	{
		return false;
	}

	CData pDataArray[3];
	InitAllCDataArray(Elementhash,pDataArray,strElement);

	//CressmanParam *structParam = &m_cressmanParam;
	//structParam->fUndef = pElem->MissingValue();
	//CressmanInterpolation(structParam, pDataArray);
	//SetElementInfo(pElem, structParam);
	if ( nModel == 8 ||  nModel == 7 )
	{
		m_Cressman.SetMissingValue(pElem->MissingValue());
		m_Cressman.Execute(pDataArray,&m_dArrayValue);
		m_Cressman.SetElementInfo(pElem);

		if (true)
		{
			m_NinePoint.Execute(pElem,&m_dArrayValue,pElem->MissingValue());
		}
	}
	else if ( nModel == 25 || nModel == 26 )
	{
		//m_IDWInterpolation.set
		m_IDWInterpolation.SetMissingValue(pElem->MissingValue());
		m_IDWInterpolation.Execute(pDataArray,&m_dArrayValue);
		m_IDWInterpolation.SetElementInfo(pElem);

		if (true)
		{
			m_NinePoint.Execute(pElem,&m_dArrayValue,pElem->MissingValue());
		}
	}

	//NetCDF2Array(pElem, NULL, nModel);
	if( !Station2Array(pElem, nModel))
	{
		if( nModel == 7 || nModel == 25)
		{
			if(GenerateCommand4ContourLine() == 1) return false;
		}
		if( nModel == 8 || nModel == 26)
		{
			int colorKey = -1;
			//if (strElement.contains(QString::fromLocal8Bit("降水")))
			//{
			//	colorKey = 26;
			//}
			//else if (strElement.contains(QString::fromLocal8Bit("温度")))
			//{
			//	colorKey = 25;
			//}
			if(GenerateCommand4ContourShaded(colorKey) == 1) return false;
		}
		if(!DumpAll(container)) return false;

		return true;
	}
	else
		return false;
}

bool GradsProvider::DumpVectorBarbStream(QRenderable* container,QHash<QString, CElement*> Elementhash,QString strElement,const int nModel)
{

	QList<CElement*> productElementList;
	if (Elementhash.value("WIND UA") != NULL)
	{
		productElementList.append(Elementhash.value("WIND UA"));
	}
	if (Elementhash.value("WIND VA") != NULL)
	{
		productElementList.append(Elementhash.value("WIND VA"));
	}
	if( !NetCDF2Array(&productElementList, nModel))
	{
		if(nModel == 18)
		{
			if(GenerateCommand4UV_All("vector") == 1) return false;
			if(!DumpAll(container)) return false;
		}
		else if (nModel == 19)
		{
			if(GenerateCommand4UV_All("barb") == 1) return false;
			if(!DumpAll(container)) return false;
		}
		else
		{
			if(GenerateCommand4UV_All("stream") == 1) return false;
			if(!DumpAll(container)) return false;
		}
		return true;
	}
	else
	{
		////CDiskFileLog::WriteLog(tr("警告"), tr("GradsProvider::DumpMicapsElement NetCDF2Array 错误！").toLocal8Bit());//写日志--renxue
		////CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),tr("NetCDF2Array error"));
		return false;
	}
}

bool GradsProvider::DumpMode14And15(QRenderable* container,QHash<QString, CElement*> Elementhash,QString strElement,const int nModel)
{
	CElement* pElemCon = Elementhash.value(tr("描述信息"));   // MICAPS 4类数据
	if(pElemCon != NULL)
	{
		QStringList strList = pElemCon->GetMicapsDeclaration();
		double fBeg, fEnd,fStep,fTemp;
		fStep = strList.at(17).toDouble();
		fBeg  = strList.at(18).toDouble();
		fEnd  = strList.at(19).toDouble();
		fTemp = fBeg;
		while(fTemp <= fEnd)
		{
			CInterval *interval = new CInterval;
			interval->fValue = (float)fTemp;
			m_gplistInterval.append(interval);
			fTemp = fTemp + fStep;
		}
	}
	QList<CElement*> productElementList;
	if (Elementhash.value(tr("格点数据")) != NULL)
	{
		productElementList.append(Elementhash.value(tr("格点数据")));
	}
	if( !NetCDF2Array(&productElementList, nModel))
	{
		if(nModel == 15)
		{
			if(GenerateCommand4ContourShaded() == 1) return false;
		}
		else
		{
			if (GenerateCommand4ContourLine() == 1) return false;
		}

		if(!DumpAll(container)) return false;

		return true;
	}
	else
	{
		//CDiskFileLog::WriteLog(tr("警告"), tr("GradsProvider::DumpMicapsElement NetCDF2Array 错误！").toLocal8Bit());//写日志--renxue
		//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),tr("NetCDF2Array error"));
		return false;
	}
}

bool GradsProvider::DumpTyphoonElement(QRenderable* container,QHash<QString, CElement*> Elementhash,QString strElement,const int nModel,P_LAYER_ATTRIBUTE pLayerAttribute)
{
	//if (strElement != tr("台风路径") && strElement != tr("台风预报"))
	//{
	//	return false;
	//}

	if (Elementhash.size())
	{
		m_TyphoonElementhash = Elementhash;

		QString strTemp = CIPASApplication::ConfigPath();
		QString iniPath = strTemp + "typhoonPath_Attribute.ini";
		QSettings *configIni;
		configIni = new QSettings(iniPath, QSettings::IniFormat);  
		m_MDAttribute.b_aPath = configIni->value("/tPathAttribute/b_aPath").toBool();
		m_MDAttribute.b_dPoint8 = configIni->value("/tPathAttribute/b_dPoint8").toBool();
		m_MDAttribute.b_fSelectW = configIni->value("/tPathAttribute/b_fSelectW").toInt();
		//m_MDAttribute.b_cPoint = configIni->value("/tPathAttribute/b_cPoint").toBool();

		m_MDAttribute.c_aa = QColor(configIni->value("/aPath/aaR").toInt(),configIni->value("/aPath/aaG").toInt(),configIni->value("/aPath/aaB").toInt());
		m_MDAttribute.c_ab = QColor(configIni->value("/aPath/abR").toInt(),configIni->value("/aPath/abG").toInt(),configIni->value("/aPath/abB").toInt());
		m_MDAttribute.c_ac = QColor(configIni->value("/aPath/acR").toInt(),configIni->value("/aPath/acG").toInt(),configIni->value("/aPath/acB").toInt());
		m_MDAttribute.c_ad = QColor(configIni->value("/aPath/adR").toInt(),configIni->value("/aPath/adG").toInt(),configIni->value("/aPath/adB").toInt());
		m_MDAttribute.c_ae = QColor(configIni->value("/aPath/aeR").toInt(),configIni->value("/aPath/aeG").toInt(),configIni->value("/aPath/aeB").toInt());
		m_MDAttribute.c_af = QColor(configIni->value("/aPath/afR").toInt(),configIni->value("/aPath/afG").toInt(),configIni->value("/aPath/afB").toInt());
		m_MDAttribute.c_bColor = QColor(configIni->value("/aPath/dR").toInt(),configIni->value("/aPath/dG").toInt(),configIni->value("/aPath/dB").toInt());
		Point * pPoint;
		QBitmapText * pText;
		Line * pLine;

		CData tempData;
		CElement *tempEle;
		QStringList tempStrList;
		float *pDataArray;
		int nPointNum,i;

		tempEle = m_TyphoonElementhash.value(tr("台风路径"));
		if (!tempEle) 
		{
			tempEle = m_TyphoonElementhash.value(tr("台风预报"));	
		}
		else if (!tempEle)
		{
			return 2;
		}
		tempData = tempEle->Data();
		pDataArray = tempData.GetDataArray(0);

		tempEle = Elementhash.value(tr("文件描述"));
		tempStrList = tempEle->GetMicapsDeclaration();
		nPointNum = tempStrList.at(tempStrList.size()-1).toInt();

		float fCurrentLon = 0,fCurrentLat = 0,fLastLon = 0,fLastLat = 0;
		QString sText;

		//pText  = new QBitmapText();
		pPoint = new Point();
		pPoint->setPointSize(7.0);
		QVector3DArray *pPointArray =pPoint->getVectexArray();
		QVector4DArray *pPointColor = pPoint->getColorArray();

		const int n = 20;
		const GLfloat Pi = 3.1415926536f;
		int j;

		for (i = 0;i < nPointNum;i++)
		{
			//if (pDataArray[i*10] /*== 0*/)
			{
				const GLfloat R7 = pDataArray[i*10 + 6];
				const GLfloat R10 = pDataArray[i*10 + 7];
				fCurrentLon = pDataArray[i*10 + 2];
				fCurrentLat = pDataArray[i*10 + 1];
				int typ_Speed = 0;
				typ_Speed = pDataArray[i*10 + 4];
				QVector4D colorVector = QVector4D(0.0, 0.0, 0.0,1.0f);

				//给每个台风观测点按照风力绘制颜色 add 20130428
				if (m_MDAttribute.b_aPath)
				{
					if (typ_Speed>=10.8 && typ_Speed <17.1)//热带低压
					{
						colorVector = QVector4D(m_MDAttribute.c_aa.red()/255.0,m_MDAttribute.c_aa.green()/255.0,m_MDAttribute.c_aa.blue()/255.0,1.0f);
					}
					else if (typ_Speed >=17.1 && typ_Speed <24.4)//热带风暴
					{
						colorVector = QVector4D(m_MDAttribute.c_ab.red()/255.0,m_MDAttribute.c_ab.green()/255.0,m_MDAttribute.c_ab.blue()/255.0,1.0f);
					}
					else if (typ_Speed >=24.4 && typ_Speed < 32.6)//强热带风暴
					{
						colorVector = QVector4D(m_MDAttribute.c_ac.red()/255.0,m_MDAttribute.c_ac.green()/255.0,m_MDAttribute.c_ac.blue()/255.0,1.0f);
					}
					else if (typ_Speed >=32.7 && typ_Speed <41.4)//台风
					{
						colorVector = QVector4D(m_MDAttribute.c_ad.red()/255.0,m_MDAttribute.c_ad.green()/255.0,m_MDAttribute.c_ad.blue()/255.0,1.0f);
					}
					else if (typ_Speed >=41.4 && typ_Speed <50.9)//强台风
					{
						colorVector = QVector4D(m_MDAttribute.c_ae.red()/255.0,m_MDAttribute.c_ae.green()/255.0,m_MDAttribute.c_ae.blue()/255.0,1.0f);
					}
					else if (typ_Speed >=51)//超强台风
					{
						colorVector = QVector4D(m_MDAttribute.c_af.red()/255.0,m_MDAttribute.c_af.green()/255.0,m_MDAttribute.c_af.blue()/255.0,1.0f);
					}
				}
				else
				{
					colorVector = QVector4D(m_MDAttribute.c_bColor.red()/255.0,m_MDAttribute.c_bColor.green()/255.0,m_MDAttribute.c_bColor.blue()/255.0,1.0f);
				}
				pPointArray->push_back(QVector3D(fCurrentLon,fCurrentLat,0.0f));
				//pPointColor->push_back(QVector4D(0.0f, 0.2f, 0.2f,1.0f));
				pPointColor->push_back(colorVector);
				GISPolygon* pPolygon = new GISPolygon;
				pPolygon->SetPolygonMode(GL_LINES);
				QVector3DArray* pArray =pPolygon->getVectexArray();
				QVector4DArray* pColor = pPolygon->getColorArray();
				getCircleCood(QVector3D(fCurrentLon,fCurrentLat,0.0f),R7,pArray,pColor,QVector4D(0.16f, 0.16f, 0.94f,1.0f));
				getCircleCood(QVector3D(fCurrentLon,fCurrentLat,0.0f),R10,pArray,pColor,QVector4D(0.92f, 0.17f, 0.2f,1.0f));
				if (m_MDAttribute.b_dPoint8)
				{
					int ist = pDataArray[i*10];
					int ss = QString("%1").arg(ist).right(2).toInt();
					if (ss == 8)//每日8时特殊标记
					{
						container->InsertRenderable(Mark_08(QVector3D(fCurrentLon,fCurrentLat,0.0f),colorVector,ist));
					}
				}

				//for(j=0; j<n; ++j)
				//{
				//	pArray->push_back(QVector3D(fCurrentLon+R*cos(2*Pi/n*j),fCurrentLat+R*sin(2*Pi/n*j),0.0f));
				//	pColor->push_back(QVector4D(0.0f, 0.2f, 0.2f,1.0f));
				//}
				pPolygon->SetBakData();
				pPolygon->SetColorArray();
				container->InsertRenderable(pPolygon);

				if (i != 0)
				{
					pLine = new Line();
					QVector4DArray* pColor = pLine->getColorArray();
					QVector3DArray* pArray = pLine->getVectexArray();

					//pColor->push_back(QVector4D(0.0f, 0.2f, 0.2f,1.0f));
					pColor->push_back(colorVector);
					pArray->push_back(QVector3D(fLastLon,fLastLat,0.0f));

					//pColor->push_back(QVector4D(0.0f, 0.2f, 0.2f,1.0f));
					pColor->push_back(colorVector);
					pArray->push_back(QVector3D(fCurrentLon,fCurrentLat,0.0f));

					pLine->SetColorArray();
					pLine->SetBakData();
					//pLine->SetLineStyle(5,255,-1);
					LineStippleAttribute *lineStipple = new LineStippleAttribute;
					LineWidthAttribute *lineWidth = new LineWidthAttribute;

					QAttributeSet attribSet;
					lineStipple->Factor(255);
					lineStipple->LinePattern(-1);
					lineWidth->Width(m_MDAttribute.b_fSelectW);
// 					if (!m_MDAttribute.b_cPoint)
// 					{
// 						PointSizeAttribute *pointsize = new PointSizeAttribute;
// 						pointsize->Size(1);
// 						attribSet.insert(QE_ATTRI_POINT_SIZE,pointsize);
// 					}
					attribSet.insert(QE_ATTRI_LINE_STIPPLE, lineStipple);
					attribSet.insert(QE_ATTRI_LINE_WIDTH, lineWidth);

					container->AttribSet(attribSet);
					container->InsertRenderable(pLine);
				}
				
				fLastLon = fCurrentLon;
				fLastLat = fCurrentLat;
			}
		}

		pPoint->SetColorArray();
		pPoint->SetBakData();
		container->InsertRenderable(pPoint);

		QStringList strLev;
		strLev.append("inValid");

		QStringList strLevs;
		strLevs.append(tr("否"));
		strLevs.append(tr("是"));

		QStringList strLStyle;
		strLStyle.append(tr("实线"));
		strLStyle.append(tr("点线"));
		strLStyle.append(tr("短划线"));

		QStringList n2;
		n2.append("");

		ILayerAttribute *cLevSub = 0;


		cLevSub = new ILayerAttribute("aPath","a",tr("是否显示分级路径"),m_MDAttribute.b_aPath,QVariant::StringList,0,1,strLevs);
		pLayerAttribute->insert("aPath",cLevSub);

		if (m_MDAttribute.b_aPath)
		{
			cLevSub = new ILayerAttribute("aa","aa",tr("热带低压(10.8~17.1)"),QColor(m_MDAttribute.c_aa.red(),m_MDAttribute.c_aa.green(),m_MDAttribute.c_aa.blue(),255),QVariant::Color,0,15,n2);
			pLayerAttribute->insert("aa",cLevSub);

			cLevSub = new ILayerAttribute("ab","ab",tr("热带风暴(17.1~24.4)"),QColor(m_MDAttribute.c_ab.red(),m_MDAttribute.c_ab.green(),m_MDAttribute.c_ab.blue(),255),QVariant::Color,0,15,n2);
			pLayerAttribute->insert("ab",cLevSub);

			cLevSub = new ILayerAttribute("ac","ac",tr("热带风暴(24.4~32.6)"),QColor(m_MDAttribute.c_ac.red(),m_MDAttribute.c_ac.green(),m_MDAttribute.c_ac.blue(),255),QVariant::Color,0,15,n2);
			pLayerAttribute->insert("ac",cLevSub);

			cLevSub = new ILayerAttribute("ad","ad",tr("台风(32.6~41.4)"),QColor(m_MDAttribute.c_ad.red(),m_MDAttribute.c_ad.green(),m_MDAttribute.c_ad.blue(),255),QVariant::Color,0,15,n2);
			pLayerAttribute->insert("ad",cLevSub);

			cLevSub = new ILayerAttribute("ae","ae",tr("强台风(41.4~50.9)"),QColor(m_MDAttribute.c_ae.red(),m_MDAttribute.c_ae.green(),m_MDAttribute.c_ae.blue(),255),QVariant::Color,0,15,n2);
			pLayerAttribute->insert("ae",cLevSub);

			cLevSub = new ILayerAttribute("af","af",tr("超强台风(>50.9)"),QColor(m_MDAttribute.c_af.red(),m_MDAttribute.c_af.green(),m_MDAttribute.c_af.blue(),255),QVariant::Color,0,15,n2);
			pLayerAttribute->insert("af",cLevSub);
		}
		else
		{
			cLevSub = new ILayerAttribute("bColor","d",tr("颜色"),QColor(m_MDAttribute.c_bColor.red(),m_MDAttribute.c_bColor.green(),m_MDAttribute.c_bColor.blue(),255),QVariant::Color,0,15,n2);
			pLayerAttribute->insert("bColor",cLevSub);
		}
		
// 		cLevSub = new ILayerAttribute("cPoint","b",tr("显示点"),1,QVariant::StringList,0,1,strLevs);
// 		pLayerAttribute->insert("cPoint",cLevSub);

		cLevSub = new ILayerAttribute("dPoint8","c",tr("显示08时标记"),m_MDAttribute.b_dPoint8,QVariant::StringList,0,1,strLevs);
		pLayerAttribute->insert("dPoint8",cLevSub);

// 		cLevSub = new ILayerAttribute("eStyle","e",tr("线型"),0,QVariant::StringList,0,2,strLStyle);
// 		pLayerAttribute->insert("eStyle",cLevSub);

		cLevSub = new ILayerAttribute("fSelectW","f",tr("线宽"),m_MDAttribute.b_fSelectW,QVariant::Int,1,10,n2);
		pLayerAttribute->insert("fSelectW",cLevSub);

		return true;
	}
	else if (m_TyphoonElementhash.size())
	{
		//////////////////////////////////////////////m_MDAttribute.b_dPoint8 = iLA->Value().toBool();

		QString strTemp = CIPASApplication::ConfigPath();
		QString iniPath = strTemp + "typhoonPath_Attribute.ini";
		QSettings *configIni;
		configIni = new QSettings(iniPath, QSettings::IniFormat);  

		configIni->setValue("/tPathAttribute/b_aPath",m_MDAttribute.b_aPath);
		configIni->setValue("/tPathAttribute/b_dPoint8",m_MDAttribute.b_dPoint8);
		configIni->setValue("/tPathAttribute/b_fSelectW",m_MDAttribute.b_fSelectW);
		configIni->setValue("/tPathAttribute/b_cPoint",m_MDAttribute.b_cPoint);

		configIni->setValue("/aPath/aaR",m_MDAttribute.c_aa.red());
		configIni->setValue("/aPath/aaG",m_MDAttribute.c_aa.green());
		configIni->setValue("/aPath/aaB",m_MDAttribute.c_aa.blue());

		configIni->setValue("/aPath/abR",m_MDAttribute.c_ab.red());
		configIni->setValue("/aPath/abG",m_MDAttribute.c_ab.green());
		configIni->setValue("/aPath/abB",m_MDAttribute.c_ab.blue());

		configIni->setValue("/aPath/acR",m_MDAttribute.c_ac.red());
		configIni->setValue("/aPath/acG",m_MDAttribute.c_ac.green());
		configIni->setValue("/aPath/acB",m_MDAttribute.c_ac.blue());

		configIni->setValue("/aPath/adR",m_MDAttribute.c_ad.red());
		configIni->setValue("/aPath/adG",m_MDAttribute.c_ad.green());
		configIni->setValue("/aPath/adB",m_MDAttribute.c_ad.blue());

		configIni->setValue("/aPath/aeR",m_MDAttribute.c_ae.red());
		configIni->setValue("/aPath/aeG",m_MDAttribute.c_ae.green());
		configIni->setValue("/aPath/aeB",m_MDAttribute.c_ae.blue());

		configIni->setValue("/aPath/afR",m_MDAttribute.c_af.red());
		configIni->setValue("/aPath/afG",m_MDAttribute.c_af.green());
		configIni->setValue("/aPath/afB",m_MDAttribute.c_af.blue());

		configIni->setValue("/aPath/dR",m_MDAttribute.c_bColor.red());
		configIni->setValue("/aPath/dG",m_MDAttribute.c_bColor.green());
		configIni->setValue("/aPath/dB",m_MDAttribute.c_bColor.blue());

		Point * pPoint;
		QBitmapText * pText;
		Line * pLine;

		CData tempData;
		CElement *tempEle;
		QStringList tempStrList;
		float *pDataArray;
		int nPointNum,i;

		tempEle = m_TyphoonElementhash.value(tr("台风路径"));
		if (!tempEle) 
		{
			tempEle = m_TyphoonElementhash.value(tr("台风预报"));	
		}
		else if (!tempEle)
		{
			return 2;
		}
		tempData = tempEle->Data();
		pDataArray = tempData.GetDataArray(0);

		tempEle = m_TyphoonElementhash.value(tr("文件描述"));
		tempStrList = tempEle->GetMicapsDeclaration();
		nPointNum = tempStrList.at(tempStrList.size()-1).toInt();

		float fCurrentLon = 0,fCurrentLat = 0,fLastLon = 0,fLastLat = 0;
		QString sText;

		//pText  = new QBitmapText();
		pPoint = new Point();
//		if (m_MDAttribute.b_cPoint)
//		{
			pPoint->setPointSize(5.0);
//		}
// 		else
// 		{
// 			pPoint->setPointSize(0.01);
// 		}
		
		QVector3DArray *pPointArray =pPoint->getVectexArray();
		QVector4DArray *pPointColor = pPoint->getColorArray();

		const int n = 20;
		const GLfloat Pi = 3.1415926536f;
		int j;



		for (i = 0;i < nPointNum;i++)
		{
			//if (pDataArray[i*10] /*== 0*/)
			{
				const GLfloat R7 = pDataArray[i*10 + 6];
				const GLfloat R10 = pDataArray[i*10 + 7];
				fCurrentLon = pDataArray[i*10 + 2];
				fCurrentLat = pDataArray[i*10 + 1];
				int typ_Speed = 0;
				typ_Speed = pDataArray[i*10 + 4];
				QVector4D colorVector = QVector4D(0.0f, 0.2f, 0.2f,1.0f);

				if (m_MDAttribute.b_aPath)
				{
					//给每个台风观测点按照风力绘制颜色 add 20130428
					if (typ_Speed>=10.8 && typ_Speed <17.1)//热带低压
					{
						colorVector = QVector4D(m_MDAttribute.c_aa.red()/255.0,m_MDAttribute.c_aa.green()/255.0,m_MDAttribute.c_aa.blue()/255.0,1.0f);
					}																										  
					else if (typ_Speed >=17.1 && typ_Speed <24.4)//热带风暴													  
					{																										  
						colorVector = QVector4D(m_MDAttribute.c_ab.red()/255.0,m_MDAttribute.c_ab.green()/255.0,m_MDAttribute.c_ab.blue()/255.0,1.0f);
					}																										  
					else if (typ_Speed >=24.4 && typ_Speed < 32.6)//强热带风暴												 
					{																										 
						colorVector = QVector4D(m_MDAttribute.c_ac.red()/255.0,m_MDAttribute.c_ac.green()/255.0,m_MDAttribute.c_ac.blue()/255.0,1.0f);
					}																										
					else if (typ_Speed >=32.7 && typ_Speed <41.4)//台风														
					{																										
						colorVector = QVector4D(m_MDAttribute.c_ad.red()/255.0,m_MDAttribute.c_ad.green()/255.0,m_MDAttribute.c_ad.blue()/255.0,1.0f);
					}																										
					else if (typ_Speed >=41.4 && typ_Speed <50.9)//强台风													
					{																										
						colorVector = QVector4D(m_MDAttribute.c_ae.red()/255.0,m_MDAttribute.c_ae.green()/255.0,m_MDAttribute.c_ae.blue()/255.0,1.0f);
					}																										 
					else if (typ_Speed >=51)//超强台风																		 
					{																										 
						colorVector = QVector4D(m_MDAttribute.c_af.red()/255.0,m_MDAttribute.c_af.green()/255.0,m_MDAttribute.c_af.blue()/255.0,1.0f);
					}
				}
				else
				{
					colorVector = QVector4D(m_MDAttribute.c_bColor.red()/255.0,m_MDAttribute.c_bColor.green()/255.0,m_MDAttribute.c_bColor.blue()/255.0,1.0f);
				}

				pPointArray->push_back(QVector3D(fCurrentLon,fCurrentLat,0.0f));
				//pPointColor->push_back(QVector4D(0.0f, 0.2f, 0.2f,1.0f));
				pPointColor->push_back(colorVector);
				GISPolygon* pPolygon = new GISPolygon;
				pPolygon->SetPolygonMode(GL_LINES);
				QVector3DArray* pArray =pPolygon->getVectexArray();
				QVector4DArray* pColor = pPolygon->getColorArray();
				getCircleCood(QVector3D(fCurrentLon,fCurrentLat,0.0f),R7,pArray,pColor,QVector4D(0.16f, 0.16f, 0.94f,1.0f));
				getCircleCood(QVector3D(fCurrentLon,fCurrentLat,0.0f),R10,pArray,pColor,QVector4D(0.92f, 0.17f, 0.2f,1.0f));
				if (m_MDAttribute.b_dPoint8)
				{
					int ist = pDataArray[i*10];
					int ss = QString("%1").arg(ist).right(2).toInt();
					if (ss == 8)//每日8时特殊标记
					{
						container->InsertRenderable(Mark_08(QVector3D(fCurrentLon,fCurrentLat,0.0f),colorVector,ist));
					}
				}
				//for(j=0; j<n; ++j)
				//{
				//	pArray->push_back(QVector3D(fCurrentLon+R*cos(2*Pi/n*j),fCurrentLat+R*sin(2*Pi/n*j),0.0f));
				//	pColor->push_back(QVector4D(0.0f, 0.2f, 0.2f,1.0f));
				//}
				pPolygon->SetBakData();
				pPolygon->SetColorArray();
				container->InsertRenderable(pPolygon);

				// 			if (i == 0)
				// 			{
				// 				sText.clear();
				// 				sText.append(QString::number(pDataArray[i*10+3]));
				// 				sText.append(".");
				// 				sText.append(QString::number(pDataArray[i*10+4]));
				// 				//sText.append(".");
				// 				//sText.append(QString::number(pDataArray[i*13+2]));
				// 				//sText.append(".");
				// 				//sText.append(QString::number(pDataArray[i*13+3]));
				// 
				// 				pText->Pointion()->push_back(QVector3D(fCurrentLon+0.02f,fCurrentLat+0.02f,0.1f));
				// 				pText->StringArray()->append(sText);
				// 			}
				// 
				if (i != 0)
				{

					//m_MDAttribute.b_eStyle;
					//m_MDAttribute.b_fSelectW;

					pLine = new Line();
					QVector4DArray* pColor = pLine->getColorArray();
					QVector3DArray* pArray = pLine->getVectexArray();

					//pColor->push_back(QVector4D(0.0f, 0.2f, 0.2f,1.0f));
					pColor->push_back(colorVector);
					pArray->push_back(QVector3D(fLastLon,fLastLat,0.0f));

					//pColor->push_back(QVector4D(0.0f, 0.2f, 0.2f,1.0f));
					pColor->push_back(colorVector);
					pArray->push_back(QVector3D(fCurrentLon,fCurrentLat,0.0f));
					//pLine->SetLineStyle(m_MDAttribute.b_fSelectW,255,-1);
					pLine->SetColorArray();
					pLine->SetBakData();

					
 					LineStippleAttribute *lineStipple = new LineStippleAttribute;
 					LineWidthAttribute *lineWidth = new LineWidthAttribute;
					
 					QAttributeSet attribSet;
 					lineStipple->Factor(255);
 					lineStipple->LinePattern(-1);
 					lineWidth->Width(m_MDAttribute.b_fSelectW);
					
// 					if (!m_MDAttribute.b_cPoint)
// 					{
// 						PointSizeAttribute *pointsize = new PointSizeAttribute;
// 						pointsize->Size(1);
// 						attribSet.insert(QE_ATTRI_POINT_SIZE,pointsize);
// 					}
 					attribSet.insert(QE_ATTRI_LINE_STIPPLE, lineStipple);
 					attribSet.insert(QE_ATTRI_LINE_WIDTH, lineWidth);
					
					container->AttribSet(attribSet);
					container->InsertRenderable(pLine);
				}

				fLastLon = fCurrentLon;
				fLastLat = fCurrentLat;
			}
		}

		pPoint->SetColorArray();
		pPoint->SetBakData();
		container->InsertRenderable(pPoint);

		// 	ColorAttribute * TextColorAttri = new ColorAttribute;
		// 	TextColorAttri->Color(QVector4D(1.0f,1.0f,1.0f,0.1f));//设置为白色
		// 	pText->SetColor(TextColorAttri);
		// 
		// 	pText->DisplayList(true);
		// 	pText->SetBakData();
		// 	container->InsertRenderable(pText);
		//ParentLayer()->setWidth(m_MDAttribute.b_fSelectW);
		return true;
	}

}

//绘制格点数据,refactor by zhanglei
bool GradsProvider::DumpGridElement(QRenderable* container,QHash<QString, CElement*> Elementhash,QString strElement,const int nModel)
{
	//m_PTempText = new QBitmapText;
	m_pTextureFont = new QEText;
	QVector3DArray *pOrignalArray = m_pTextureFont->OriginalArray();
	QVector3DArray *pPositionArray = m_pTextureFont->GetPositionArray();
	m_FontColor.setAlphaF(m_FontShow);
	m_pTextureFont->SetColor(m_FontColor);
	m_pTextureFont->SetFont(m_Font);
	m_pTextureFont->SetTextMode(QEText::STRING_LIST_MODE);

	m_stPoint = new Point;
	CData tempData;
	CElement *tempEle;
	tempEle = Elementhash.value(tr("格点数据"));
	if (NULL == tempEle)
	{
		return false;
	}
	//if (!m_bLonLatChanged)
	//{
	//	m_MDAttribute.f_lonstart = tempEle->SelectedLon().m_begin;
	//	m_MDAttribute.f_lonend = tempEle->SelectedLon().m_end;
	//	if (tempEle->SelectedLat().m_begin > tempEle->SelectedLat().m_end)
	//	{
	//		m_MDAttribute.f_latstart = tempEle->SelectedLat().m_end;//考虑micaps 起始纬度为90 终止纬度为 0
	//		m_MDAttribute.f_latend = tempEle->SelectedLat().m_begin;
	//	}
	//	else
	//	{
	//		m_MDAttribute.f_latstart = tempEle->SelectedLat().m_begin;//考虑micaps 起始纬度为90 终止纬度为 0
	//		m_MDAttribute.f_latend = tempEle->SelectedLat().m_end;
	//	}
	//}
	tempData = tempEle->Data();
	QPointF lonlat;

	float *pArray = tempData.GetDataArray(0);
	long isize,jsize;
	isize = tempData.DataBlockDimensionSize(0,0);
	jsize = tempData.DataBlockDimensionSize(0,1);
	for (int j = 0; j < jsize ; j++)//
	{
		for (int i =  0; i < isize; i++)//
		{
			if(tempData.GetDataArray(0)[j * isize + i] == -999) continue;
			xy2lonlat(i,j,&lonlat,tempEle);

			//if (lonlat.x()>= m_MDAttribute.f_lonstart && lonlat.x()<= m_MDAttribute.f_lonend &&
			//	lonlat.y()>= m_MDAttribute.f_latstart && lonlat.y()<= m_MDAttribute.f_latend)
			//{
				//m_PTempText->Pointion()->push_back(QVector3D(lonlat.x(), lonlat.y(), 0));
				//m_PTempText->StringArray()->append(QString("%1").arg(*pArray));
				QString strTemp;
				strTemp.append(QString("%1").arg(*pArray));
				double curValue = *pArray;
				if (curValue <= m_fMaxValue && curValue >= m_fMinValue)
				{
					if (m_bShowValue)
					{
						m_pTextureFont->StringList().append(strTemp);
						pPositionArray->push_back(QVector3D(lonlat.x(), lonlat.y(), 0));
						pOrignalArray->push_back(QVector3D(lonlat.x(), lonlat.y(), 0));
					}

					if(m_MDAttribute.b_StationP)
						DrawStationPoint(lonlat.x(),lonlat.y());
				}
			//}
			pArray++;
		}
	}
	//m_PTempText->SetColor(m_pTextColorAttri);
	//m_PTempText->DisplayList(true);
	//m_PTempText->SetBakData();
	//container->InsertRenderable(m_PTempText);
	m_pTextureFont->SetBakData();
	m_pTextureFont->DisplayList(true);
	container->InsertRenderable(m_pTextureFont);

	m_stPoint->SetColorArray();
	m_stPoint->SetBakData();
	container->InsertRenderable(m_stPoint);

	return true;
	//GenerateMicapsAttribute(strElement);
}

//refactor by zhanglei
int GradsProvider::InitLevelSymbol()//绘制分级符号
{
	//分级符号。。。初始化
	m_pStationSymbolPointArray.clear();
	int nsymbolcount = m_StationClassifyInfoList.size();
	for(int i = 0;i<nsymbolcount;i++)
	{
		SymbolPoint * pSymbolpointC = NULL;
		pSymbolpointC = new SymbolPoint;
		QPixmap *pPixmapPoint = new QPixmap(m_StationClassifyInfoList.at(i).qPixmap);
		//pPixmapPoint->save("c:/aaa.bmp");
		m_pSymbol = m_pSymbolPlugin->GetSymbol(0);
		if(m_pSymbol != NULL)
		{
			//m_pSymbol->SetTextureFromPixmap(pPixmapPoint, ParentLayer()->GetGLView());
			m_pSymbol->SetNormalColor(QVector4D(1.0, 1.0, 1.0, 1.0f));
			m_pSymbol->setSymbolSize(m_StationClassifyInfoList.at(i).nSymbolSize);
			pSymbolpointC->SetPointStyle(m_pSymbol);
		}
		m_pStationSymbolPointArray.push_back(pSymbolpointC);
		//-------------------------
	}

	return nsymbolcount;
}

int GradsProvider::InitLevelMark()
{
	//分级标值。。。初始化
	m_pStationTextureFontArray.clear();
	int nValueCount = m_StationClassifyValueList.count();
	for(int iv = 0;iv<nValueCount;iv++)
	{
		QEText *pTextureFont = new QEText;
		QColor tempcolor = m_StationClassifyValueList.at(iv).valueColor;
		tempcolor.setAlphaF(1.0f);
		pTextureFont->SetColor(tempcolor);
		pTextureFont->SetFont( m_StationClassifyValueList.at(iv).qValueFont);
		pTextureFont->SetTextMode(QEText::STRING_LIST_MODE);
		m_pStationTextureFontArray.push_back(pTextureFont);
		//-------------------------
	}

	return nValueCount;
}

bool GradsProvider::DumpLevelSymbol()//绘制分级符号
{
	return false;
}

bool GradsProvider::DumpLevelMark(QVector3DArray *pOrignalArray,QVector3DArray *pPositionArray,int nValueCount,float flon,float flat,float ftext) //绘制分级标值
{
	return false;
}

void GradsProvider::ShowValue(float flon,float flat,float ftext,int nValueCount,StationValues& sv)
{
	if(m_bShowValue && m_pTextureFont)//------------------显示值
	{
		static int nCount = 0;
		int lodlevel = 0;

		if (nCount>=m_lodList.count())
		{
			nCount = 0;
		}
		if (nCount<m_lodList.count())
		{
			lodlevel = m_lodList.at(nCount);
		}
		
		QVector3DArray *pOrignalArray = m_pTextureFont->OriginalArray();
		QVector3DArray *pPositionArray = m_pTextureFont->GetPositionArray();
		
		//	DumpLevelMark(pOrignalArray,pPositionArray,nValueCount,flon,flat,ftext);
		int nText = (int)ftext;				
		if (nText != 9999 && nText != -999 && nText != 99999 && nText != 999999) //如果不是无效值
		{
			if (m_fMinValue == 0.0 && m_fMaxValue == 0.0)
			{
				bool bClassify = false;
				for(int isv = 0;isv<nValueCount;isv++)
				{
					if (ftext<=m_StationClassifyValueList.at(isv).fMaxValue && ftext >= m_StationClassifyValueList.at(isv).fMinValue)
					{
						if (m_StationClassifyValueList.at(isv).bShowValue)
						{
							//m_pStationTextureFontArray.at(isv)->AddLodToList(lodlevel);
							m_pStationTextureFontArray.at(isv)->StationValueArray()->push_back(sv);
							m_pStationTextureFontArray.at(isv)->StringList().append(QString::number(ftext));
							m_pStationTextureFontArray.at(isv)->GetPositionArray()->push_back(QVector3D(flon, flat, 0));
							QVector3DArray *pTempOrignalArray = m_pStationTextureFontArray.at(isv)->OriginalArray();
							pTempOrignalArray->push_back(QVector3D(flon, flat, 0));
							pTempOrignalArray = NULL;
							bClassify = true;                                               //是分级数据显示
							break;
						}else
						{
							bClassify = true;                                               //是分级数据不显示
							break;
						}
					}
				}
				if (!bClassify)
				{	
					//m_pTextureFont->AddLodToList(lodlevel);
					m_pTextureFont->StationValueArray()->push_back(sv);
					//m_pTextureFont->StringList().append(QString::number(ftext));
				//	m_pTextureFont->StringList().append(QString("%1").arg(ftext,0,'f',m_nPrecision));
					pPositionArray->push_back(QVector3D(flon, flat, 0));
					pOrignalArray->push_back(QVector3D(flon, flat, 0));					
				}
			}
			else if (m_fMaxValue >= m_fMinValue && ftext >= m_fMinValue && ftext <= m_fMaxValue)
			{
					bool bClassify = false;
					for(int isv = 0;isv<nValueCount;isv++)
					{
						if (ftext<=m_StationClassifyValueList.at(isv).fMaxValue && ftext >= m_StationClassifyValueList.at(isv).fMinValue)
						{
							if (m_StationClassifyValueList.at(isv).bShowValue)
							{
								//m_pStationTextureFontArray.at(isv)->AddLodToList(lodlevel);
								m_pStationTextureFontArray.at(isv)->StationValueArray()->push_back(sv);
								m_pStationTextureFontArray.at(isv)->StringList().append(QString::number(ftext));
								m_pStationTextureFontArray.at(isv)->GetPositionArray()->push_back(QVector3D(flon, flat, 0));
								QVector3DArray *pTempOrignalArray = m_pStationTextureFontArray.at(isv)->OriginalArray();
								pTempOrignalArray->push_back(QVector3D(flon, flat, 0));
								pTempOrignalArray = NULL;
								bClassify = true;                                               //是分级数据显示
								break;
							}else
							{
								bClassify = true;                                               //是分级数据不显示
								break;
							}
						}
					}
					if (!bClassify)
					{
						//m_pTextureFont->AddLodToList(lodlevel);
						m_pTextureFont->StationValueArray()->push_back(sv);							

						//m_pTextureFont->StringList().append(QString("%1").arg(ftext,0,'f',m_nPrecision));
						pPositionArray->push_back(QVector3D(flon, flat, 0));
						pOrignalArray->push_back(QVector3D(flon, flat, 0));
					}
			}
		}
		else
		{
			//m_pTextureFont->AddLodToList(lodlevel);
			m_pTextureFont->StationValueArray()->push_back(sv);				 
			pPositionArray->push_back(QVector3D(flon, flat, 0));
			pOrignalArray->push_back(QVector3D(flon, flat, 0));
		}
        nCount++;		
	}
}

void GradsProvider::ShowValue(QRenderable* container,float flon,float flat,float ftext,int nValueCount,StationValues& sv,int lodlevel)
{
	QEText* pTextureFont = new QEText;
	QVector3DArray *pOrignalArray = pTextureFont->OriginalArray();
	QVector3DArray *pPositionArray = pTextureFont->GetPositionArray();
	m_FontColor.setAlphaF(m_FontShow);
	pTextureFont->SetColor(m_FontColor);
	pTextureFont->SetFont(m_Font);
	pTextureFont->SetTextMode(QEText::STRING_MODE);
	//pTextureFont->SetRenderLODindex(lodlevel);
	
	int nText = (int)ftext;				
	if (nText != 9999 && nText != -999 && nText != 99999 && nText != 999999)
	{
		pTextureFont->StationValueArray()->push_back(sv);
		//m_pTextureFont->StringList().append(QString::number(ftext));
		//pTextureFont->StringList().append(QString("%1").arg(ftext,0,'f',m_nPrecision));
		pTextureFont->SetString(QString("%1").arg(ftext,0,'f',m_nPrecision));
		pPositionArray->push_back(QVector3D(flon, flat, 0));
		pOrignalArray->push_back(QVector3D(flon, flat, 0));
		container->InsertRenderable(pTextureFont);	
	}
}

void GradsProvider::SortHashElement(QHash<QString, CElement*>& Elementhash,StationValues& sv,int i,int nlevel,int ncount)
{
	//1,判断是否需要排序
	bool needsort = false;
	QHash<QString ,CElement*>::iterator iter = Elementhash.begin();
	for (;iter != Elementhash.end();++iter) 
	{
		CElement* pTempElement = iter.value();
		if (pTempElement && pTempElement->ID() != "" && pTempElement->ID().toInt() !=0)
		{
			needsort = true;
		}
	}

	if (needsort)
	{
		int num = m_ElementsName.size();
		for (int j=0;j<num;j++)
		{
			QHash<QString ,CElement*>::iterator iter = Elementhash.begin();
			for (;iter != Elementhash.end();++iter) 
			{
				CElement* pTempElement = iter.value();
				if (pTempElement)
				{
					if (pTempElement->ID()!="" && pTempElement->ID().toInt()==j && pTempElement->DataBlocksCount()>0)
					{						
						if (iter.key() == m_ElementsName.at(0))
						{
							sv.SetStationValue(iter.key(),iter.value()->Data().GetDataArray(0)[i]);
						}else
							sv.SetStationValue(iter.key(),iter.value()->Data().GetDataArray(0)[nlevel*ncount+i]);
						continue;	
					}						
				}
				else 
				{
					continue;	
				}					
			}
		}
	}
	else
	{
		for(int it =0;it<m_ElementsName.size();it++)
		{
			CElement *pTempEle = Elementhash.value(m_ElementsName.at(it));
			if(NULL != pTempEle && pTempEle->DataBlocksCount()>0)
			{
				if (it == 0 )
				{
					sv.SetStationValue(m_ElementsName.at(it),pTempEle->Data().GetDataArray(0)[i]);
				}else
					sv.SetStationValue(m_ElementsName.at(it),pTempEle->Data().GetDataArray(0)[nlevel*ncount+i]);
			}
			else
				continue;					
		}
	}	
}

void GradsProvider::initSymbol()
{
	m_pPixmapPoint = new QPixmap(m_Pixmap);
	if (m_pPixmapPoint != NULL)
	{
		m_pSymbol = m_pSymbolPlugin->GetSymbol(0);
		if(m_pSymbol != NULL)
		{
			//m_pSymbol->SetTextureFromPixmap(m_pPixmapPoint, ParentLayer()->GetGLView());
			m_pSymbol->SetNormalColor(QVector4D(1.0, 1.0, 1.0, 1.0f));
			m_pSymbol->setSymbolSize(m_nSymbolSize);
		}
		//pSymbolpoint->SetPointStyle(m_pSymbol);
	}

		/*
	SymbolPoint * pSymbolpoint = NULL;
	pSymbolpoint = new SymbolPoint;
	pSymbolpoint->StationValueArray()->clear();

	m_pPixmapPoint = new QPixmap(m_Pixmap);
	if (m_pPixmapPoint != NULL)
	{
		m_pSymbol = m_pSymbolPlugin->GetSymbol(0);
		if(m_pSymbol != NULL)
		{
			m_pSymbol->SetTextureFromPixmap(m_pPixmapPoint, ParentLayer()->GetGLView());
			m_pSymbol->SetNormalColor(QVector4D(1.0, 1.0, 1.0, 1.0f));
			m_pSymbol->setSymbolSize(m_nSymbolSize);
		}
		pSymbolpoint->SetPointStyle(m_pSymbol);
	}
	*/
}

void GradsProvider::DrawSymbolPoint(SymbolPoint* pSymbolpoint,int nsymbolcount,float flon,float flat,float ftext,StationValues& sv)
{
	if(m_MDAttribute.b_StationP && pSymbolpoint)//------------------绘图标
	{
		bool bsymbol = false;
		static int nCount = 0;
		int lodlevel = 0;

		if (nCount>=m_lodList.count())
		{
			nCount = 0;
		}
		if (nCount<m_lodList.count())
		{
			lodlevel = m_lodList.at(nCount);
		}
		for(int is = 0;is<nsymbolcount;is++)
		{
			if (ftext<=m_StationClassifyInfoList.at(is).fMaxValue && ftext >= m_StationClassifyInfoList.at(is).fMinValue)
			{			
				//m_pStationSymbolPointArray.at(is)->AddLodToList(lodlevel);
				m_pStationSymbolPointArray.at(is)->SetPosition(QVector3D(flon,flat,0));
				m_pStationSymbolPointArray.at(is)->SetAngle(m_fSymbolAngle);
				m_pStationSymbolPointArray.at(is)->StationValueArray()->push_back(sv);
				bsymbol = true;
				break;
			}
		}
		if (!bsymbol) //不在分级显示范围内的，放入pSymbolpoint;如果没有分级，默认放入pSymbolPoint
		{				
			//pSymbolpoint->AddLodToList(lodlevel);
			pSymbolpoint->SetPosition(QVector3D(flon,flat,0));
			pSymbolpoint->SetAngle(m_fSymbolAngle);
		//	pSymbolpoint->StationValueArray()->push_back(sv);
		}
		nCount++;
	}
}

void GradsProvider::DrawSymbolPoint(QRenderable* container,QVector3D& pos,StationValues& sv,int lodlevel)
{
	SymbolPoint * pSymbolpoint =  new SymbolPoint;	
	pSymbolpoint->StationValueArray()->clear();
	pSymbolpoint->SetPointStyle(m_pSymbol);
	pSymbolpoint->SetPosition(pos);
	pSymbolpoint->SetAngle(m_fSymbolAngle);
//	m_pStationSymbolPointArray.push_back(pSymbolpoint);
	pSymbolpoint->StationValueArray()->push_back(sv);
	//pSymbolpoint->SetRenderLODindex(lodlevel);

	pSymbolpoint->SetBakData();
	ParentLayer()->GetQListSymbolPoint()->append(pSymbolpoint);
	m_pStationSymbolPointArray.push_back(pSymbolpoint);
	//container->InsertRenderable(pSymbolpoint);
}

void GradsProvider::ClearStationSymbolPoint()
{
	if (!m_pStationSymbolPointArray.isEmpty())
	{
		for (int i=0;i<m_pStationSymbolPointArray.count();i++)
		{
			SymbolPoint* pTemp =  m_pStationSymbolPointArray.at(i);
			delete pTemp;
			pTemp = NULL;
		}
		m_pStationSymbolPointArray.clear();
	}
}

bool GradsProvider::DumpSymbolPoint(QRenderable* container,QHash<QString, CElement*> Elementhash,QString strElement,const int nModel)
{
	return false;
}

bool GradsProvider::DumpOtherElement(QRenderable* container,QHash<QString, CElement*> Elementhash,QString strElement,const int nModel)
{
	m_pTextureFont = new QEText;
	QVector3DArray *pOrignalArray = m_pTextureFont->OriginalArray();
	QVector3DArray *pPositionArray = m_pTextureFont->GetPositionArray();
	m_FontColor.setAlphaF(m_FontShow);
	m_pTextureFont->SetColor(m_FontColor);
	m_pTextureFont->SetFont(m_Font);
	m_pTextureFont->SetTextMode(QEText::STRING_LIST_MODE);	
	
	int nsymbolcount = InitLevelSymbol();
	int nValueCount = InitLevelMark();

	SymbolPoint * pSymbolpoint = NULL;
	pSymbolpoint = new SymbolPoint;
	pSymbolpoint->StationValueArray()->clear();

	m_pPixmapPoint = new QPixmap(m_Pixmap);
	if (m_pPixmapPoint != NULL)
	{
		m_pSymbol = m_pSymbolPlugin->GetSymbol(0);
		if(m_pSymbol != NULL)
		{
			//m_pSymbol->SetTextureFromPixmap(m_pPixmapPoint, ParentLayer()->GetGLView());
			m_pSymbol->SetNormalColor(QVector4D(1.0, 1.0, 1.0, 1.0f));
			m_pSymbol->setSymbolSize(m_nSymbolSize);
		}
		pSymbolpoint->SetPointStyle(m_pSymbol);
	}

	CData tempData[3];
	InitAllCDataArray(Elementhash,tempData,strElement);
	int ncount = tempData[0].DataBlockDimensionSize(0,0);
	//根据层设置选取数据
	int nlevel = 0;
	if (NULL == Elementhash.value(strElement))
	{
		return false;
	}
	if (Elementhash.value(strElement)->Level().m_valus.size()>1)
	{
		if(Elementhash.value(strElement)->Level().m_selected.size()==1)
			nlevel = Elementhash.value(strElement)->Level().m_selected.keys().at(0);
	}
	if(m_bhashOnce)
	{
		if(ncount > 2)
		{
			if (!tempData[2].GetDataArray(0)) return false;  // WUENPING
			float ftemp = tempData[2].GetDataArray(0)[0];
			int nText = (int)ftemp;
			if (nText != 9999 && nText != -999 && nText != 99999  && nText != 999999)
			{	
				m_fMaxValue = ftemp;
				m_fMinValue = ftemp;
			}else
			{
				ftemp = tempData[2].GetDataArray(0)[1];
				int nText = (int)ftemp;
				if (nText != 9999 && nText != -999 && nText != 99999 && nText != 999999)
				{	
					m_fMaxValue = ftemp;
					m_fMinValue = ftemp;
				}
			}

		}
	}
	
	QBitmapText *pTempText = new QBitmapText;	
	QVector<StationValues> stationvaluearry;
	QStringList strTextArray;
	for (int i=0;i<ncount;i++)
	{
		float flon,flat,ftext;
		flon = tempData[0].GetDataArray(0)[i];
		flat = tempData[1].GetDataArray(0)[i];
		if (!tempData[2].GetDataArray(0)) return false;  // WUENPING
		ftext = tempData[2].GetDataArray(0)[nlevel*ncount+i];
		if(m_bhashOnce)
		{
			int nText = (int)ftext;
			if (nText != 9999 && nText != -999 && nText != 99999 && nText != 999999)
			{	
				if (ftext > m_fMaxValue)
				{
					m_fMaxValue = ftext;
				}else
					if (ftext < m_fMinValue)
					{
						m_fMinValue = ftext;
					}
			}
		}

		if (flon>= m_MDAttribute.f_lonstart && flon<= m_MDAttribute.f_lonend  &&
			flat>= m_MDAttribute.f_latstart && flat<= m_MDAttribute.f_latend)
		{
			StationValues sv;
			
			if (!m_ElementsName.contains("站点经度"))
			{
				sv.SetStationValue(QString("站点经度"),flon);
			}
			if (!m_ElementsName.contains("站点纬度"))
			{
				sv.SetStationValue(QString("站点纬度"),flat);
			}

			SortHashElement(Elementhash,sv,i,nlevel,ncount);
			
			ShowValue(flon,flat,ftext,0,sv);
			
			DrawSymbolPoint(pSymbolpoint,nsymbolcount,flon,flat,ftext,sv);

			strTextArray.append(QString::number(ftext));
			stationvaluearry.append(sv);

			pTempText->StationValueArray()->push_back(sv);
			pTempText->OriginalArray()->push_back(QVector3D(flon,flat,ftext));
			pTempText->Pointion()->push_back(QVector3D(flon,flat,ftext));
		}
	}
	m_pTextureFont->SetStringList(strTextArray);
	ParentLayer()->SetStation(pTempText);	//将站点保存到plyer中
	//ParentLayer()->SetStationValueArrays(stationvaluearry);
	//---------------------------------------------------------------插入层start
	
	if (m_bShowValue)
	{
		m_pTextureFont->SetBakData();
		m_pTextureFont->DisplayList(true);
		container->InsertRenderable(m_pTextureFont);
		//ParentLayer()->GetQETextArray()->push_back(m_pTextureFont);
		pOrignalArray = NULL;
	}
	else
	{
		delete m_pTextureFont;
		m_pTextureFont = NULL;
		pOrignalArray = NULL;
	}
	
	
	for(int iv = 0;iv<nValueCount;iv++)
	{
		m_pStationTextureFontArray.at(iv)->SetBakData();
		m_pStationTextureFontArray.at(iv)->DisplayList(true);
		container->InsertRenderable(m_pStationTextureFontArray.at(iv));
	}
	
	
	for(int i = 0;i<nsymbolcount;i++)
	{
		m_pStationSymbolPointArray.at(i)->SetBakData();
		ParentLayer()->GetQListSymbolPoint()->append(m_pStationSymbolPointArray.at(i));
		container->InsertRenderable(m_pStationSymbolPointArray.at(i));
	}	
	if (m_pPixmapPoint != NULL)
	{
		pSymbolpoint->SetBakData();
		ParentLayer()->GetQListSymbolPoint()->append(pSymbolpoint);
		container->InsertRenderable(pSymbolpoint);
	}
	return true;
}

/************************************************************************/
/* 函数功能： 初始化图标纹理					                        */
/* 参数说明:  						*/
/* 返 回 值:   NULL														*/
/* 作    者:  赵高攀														*/
/* 日    期:  2011 年 5 月 10 日										*/
/************************************************************************/
void GradsProvider::InitSymbolPointArray(QString tempStr)
{
	if (tempStr =="Wind" || tempStr== "All" )
	{
		m_pWindSymbolPointArray.clear();
		for (int m = 1;m<11;m++)
		{
			SymbolPoint * pSymbolpoint = new SymbolPoint;
			m_pSymbol = m_pSymbolPlugin->GetSymbol(m+100);
			if (m_pSymbol)
			{
				pSymbolpoint->SetPointStyle(m_pSymbol);
				//pSymbolpoint->CoordTrans((Proj4Transform*)parentLayer->CoordTrans());
			}
			m_pWindSymbolPointArray.push_back(pSymbolpoint);
		}
	}
	if (tempStr =="Cloud" || tempStr== "All")
	{
		m_pCloudSymbolPointArray.clear();
		for (int m = 1;m<12;m++)
		{
			SymbolPoint * pSymbolpoint = new SymbolPoint;
			m_pSymbol = m_pSymbolPlugin->GetSymbol(m+700);
			if (m_pSymbol)
			{
				pSymbolpoint->SetPointStyle(m_pSymbol);
				// pSymbolpoint->CoordTrans((Proj4Transform*)parentLayer->CoordTrans());
			}
			m_pCloudSymbolPointArray.push_back(pSymbolpoint);
		}
	}
	if (tempStr =="Weather" || tempStr== "All")
	{
		m_pWeatherSymbolPointArray.clear();
		for (int m = 1;m<27;m++)//根据人机图标编号文件取天气符号
		{
			int nindex = 0;
			switch(m)
			{
			case 1:
			case 2:
			case 3:
				nindex = m + 500;
				break;
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				nindex = m-3 + 200;
				break;
			case 9:
				nindex = 207;
				break;
			case 10:
				nindex = 507;
				break;
			case 11:
				nindex = 504;
				break;
			case 12:
				nindex = 505;
				break;
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
				nindex = m-12+300;
				break;
			case 20:
				nindex = 212;
				break;
			case 21:
				nindex = 213;
				break;
			case 22:
				nindex = 509;
				break;
			case 23:
			case 24:
			case 25:
			case 26:
				nindex = m-22+300;
				break;
			}
			SymbolPoint * pSymbolpoint = new SymbolPoint;
			m_pSymbol = m_pSymbolPlugin->GetSymbol(nindex);
			if (m_pSymbol)
			{
				pSymbolpoint->SetPointStyle(m_pSymbol);
			}
			m_pWeatherSymbolPointArray.push_back(pSymbolpoint);
		}
	}
}

/************************************************************************/
/* 函数功能： 初始化地面填图绘图数据					                        */
/* 参数说明:  						*/
/* 返 回 值:   NULL														*/
/* 作    者:  赵高攀														*/
/* 日    期:  2011 年 5 月 10 日										*/
/************************************************************************/
void GradsProvider::InitAllCDataArray(QHash<QString, CElement*> Elementhash,CData* pTempDataArray,QString strTemp)
{
	if (strTemp == tr("地面填图"))
	{
		if (Elementhash.value(tr("站点经度"))!= NULL)
		{
			pTempDataArray[0] = Elementhash.value(tr("站点经度"))->Data();
		}
		if (Elementhash.value(tr("站点纬度"))!= NULL)
		{
			pTempDataArray[1] = Elementhash.value(tr("站点纬度"))->Data();
		}
		if (Elementhash.value(tr("总云量"))!= NULL)
		{
			pTempDataArray[2] = Elementhash.value(tr("总云量"))->Data();
		}
		if (Elementhash.value(tr("风向"))!= NULL)
		{
			pTempDataArray[3] = Elementhash.value(tr("风向"))->Data();
		}
		if (Elementhash.value(tr("风速"))!= NULL)
		{
			pTempDataArray[4] = Elementhash.value(tr("风速"))->Data();
		}
		if (Elementhash.value(tr("露点"))!= NULL)
		{
			pTempDataArray[5] = Elementhash.value(tr("露点"))->Data();
		}
		if (Elementhash.value(tr("3小时变压"))!= NULL)
		{
			pTempDataArray[6] = Elementhash.value(tr("3小时变压"))->Data();
		}
		if (Elementhash.value(tr("海平面气压"))!= NULL)
		{
			pTempDataArray[7] = Elementhash.value(tr("海平面气压"))->Data();
		}
		if (Elementhash.value(tr("温度"))!= NULL)
		{
			pTempDataArray[8] = Elementhash.value(tr("温度"))->Data();
		}
		if (Elementhash.value(tr("现在天气"))!= NULL)
		{
			pTempDataArray[9] = Elementhash.value(tr("现在天气"))->Data();
		}
	}
	else if (strTemp == tr("风标"))
	{
		if (Elementhash.value(tr("站点经度"))!= NULL)
		{
			pTempDataArray[0] = Elementhash.value(tr("站点经度"))->Data();
		}
		if (Elementhash.value(tr("站点纬度"))!= NULL)
		{
			pTempDataArray[1] = Elementhash.value(tr("站点纬度"))->Data();
		}
		if (Elementhash.value(tr("风向"))!= NULL)
		{
			pTempDataArray[2] = Elementhash.value(tr("风向"))->Data();
		}
		if (Elementhash.value(tr("风速"))!= NULL)
		{
			pTempDataArray[3] = Elementhash.value(tr("风速"))->Data();
		}
	}
	else if (strTemp == tr("风标1"))
	{
		if (Elementhash.value(tr("站点经度"))!= NULL)
		{
			pTempDataArray[0] = Elementhash.value(tr("站点经度"))->Data();
		}
		if (Elementhash.value(tr("站点纬度"))!= NULL)
		{
			pTempDataArray[1] = Elementhash.value(tr("站点纬度"))->Data();
		}
		if (Elementhash.value(tr("风向1"))!= NULL)
		{
			pTempDataArray[2] = Elementhash.value(tr("风向1"))->Data();
		}
		if (Elementhash.value(tr("风速1"))!= NULL)
		{
			pTempDataArray[3] = Elementhash.value(tr("风速1"))->Data();
		}
	}
	else if (strTemp == tr("风标2"))
	{
		if (Elementhash.value(tr("站点经度"))!= NULL)
		{
			pTempDataArray[0] = Elementhash.value(tr("站点经度"))->Data();
		}
		if (Elementhash.value(tr("站点纬度"))!= NULL)
		{
			pTempDataArray[1] = Elementhash.value(tr("站点纬度"))->Data();
		}
		if (Elementhash.value(tr("风向2"))!= NULL)
		{
			pTempDataArray[2] = Elementhash.value(tr("风向2"))->Data();
		}
		if (Elementhash.value(tr("风速2"))!= NULL)
		{
			pTempDataArray[3] = Elementhash.value(tr("风速2"))->Data();
		}
	}
	else if (strTemp == tr("高空填图"))
	{
		if (Elementhash.value(tr("站点经度"))!= NULL)
		{
			pTempDataArray[0] = Elementhash.value(tr("站点经度"))->Data();
		}
		if (Elementhash.value(tr("站点纬度"))!= NULL)
		{
			pTempDataArray[1] = Elementhash.value(tr("站点纬度"))->Data();
		}
		if (Elementhash.value(tr("风向"))!= NULL)
		{
			pTempDataArray[2] = Elementhash.value(tr("风向"))->Data();
		}
		if (Elementhash.value(tr("风速"))!= NULL)
		{
			pTempDataArray[3] = Elementhash.value(tr("风速"))->Data();
		}
		if (Elementhash.value(tr("温度露点差"))!= NULL)
		{
			pTempDataArray[4] = Elementhash.value(tr("温度露点差"))->Data();
		}
		if (Elementhash.value(tr("高度"))!= NULL)
		{
			pTempDataArray[5] = Elementhash.value(tr("高度"))->Data();
		}
		if (Elementhash.value(tr("温度"))!= NULL)
		{
			pTempDataArray[6] = Elementhash.value(tr("温度"))->Data();
		}
	}
	else
	{
		if (Elementhash.value(tr("站点经度"))!= NULL)
		{
			pTempDataArray[0] = Elementhash.value(tr("站点经度"))->Data();
		}
		if (Elementhash.value(tr("站点纬度"))!= NULL)
		{
			pTempDataArray[1] = Elementhash.value(tr("站点纬度"))->Data();
		}
		if (Elementhash.value(strTemp)!= NULL)
		{
			pTempDataArray[2] = Elementhash.value(strTemp)->Data();
		}
	}
	//-----------------------------------------------------------获取其他元素名称列表
	QHash<QString,CElement *>::const_iterator modiDate;
	m_ElementsName<<tr("站点编号");
	for( modiDate=Elementhash.constBegin(); modiDate!=Elementhash.constEnd(); ++modiDate)
	{
		//if (modiDate.value()->NeedDisplay())
		{
			m_ElementsName<<modiDate.key();
		}
	}
}

void GradsProvider::SetStationLOD(Layer* lyer,QHash<QString, CElement*> Elementhash)
{
	if (lyer)
	{
		m_lodList.clear();

		CData data;
		CElement *pElement = Elementhash.value(tr("格点数据"));
		if(pElement == NULL)
		{
			pElement = Elementhash.value(tr("站点编号"));
		}
		if (pElement == NULL)
		{
			return;
		}
		data = pElement->Data();
		int ncount = data.DataBlockDimensionSize(0,0);

		if (ncount<1)
		{
			return;
		}
		//ReadData from config file
		QString filename;// = lyer->GetStationFileName();
		QList<StationLODInfo> infolist;
		LAYER_DISPLAY_INFO disinfo;
		int levelconunt;

		lodconfig.ReadSTMap();
		
		if (!lodconfig.ReadLodConfigFile(m_lodgroup) || m_lodgroup.loditems.isEmpty())
		{
			return;
		}
        //////////////////////////////////////////////////////////////////////////

		float disnear,disfar;	
		QList<int> itemidlist; //判断站点级别	
		int id,grpid,counttemp;
		int lodmin,lodmax,range1,range2,lodrange1,lodrange2;
		
		QList<float> fLevelList = m_lodgroup.levelist;
		QList<int> stepcountList = m_lodgroup.StepList;
		lodmin = m_lodgroup.loditems.constBegin().value().lodmin;
		lodmax = m_lodgroup.loditems.constBegin().value().lodmax;

		for (int i=0;i<ncount;i++)
		{
			id = data.GetDataArray(0)[i];
			grpid = lodconfig.GetSTTypeByID(id); //id有可能无效，如10009
			if (grpid!=-1 && !itemidlist.contains(grpid))
			{
				itemidlist.append(grpid);
				LODItem item = m_lodgroup.loditems.value(grpid);
				lodmin = item.lodmin<lodmin?item.lodmin:lodmin;
				lodmax = item.lodmax>lodmax?item.lodmax:lodmax;
			}			
		}
		if (lodmin<0 || lodmax>fLevelList.size()-1)
		{
			return;
		}
		disnear = fLevelList.at(lodmin);
		disfar = fLevelList.at(lodmax);
		
		range1 = GetRangeMin(stepcountList,lodmin);
		range2 = GetRangeMin(stepcountList,lodmax);

		qSort(itemidlist);

		//////////////////////////////////////////////////////////////////////////

		if (lodconfig.ReadFile(filename,infolist,disinfo,ncount))
		{
			m_staionlodinfo = infolist;
			m_disinfo = disinfo;
			//lyer->SetStaionLODInfo(m_staionlodinfo);
			//lyer->SetLayerDisPlayInfo(disinfo);
			//lyer->ApplyLayerDisPlayInfo();

			for (int i=0;i<m_staionlodinfo.count();i++)
			{
				m_lodList.push_back(m_staionlodinfo.at(i).lodLevel);
			}
		}
		else //如果读取配置文件失败，自动创建一个配置文件
		{			
			if (ncount>0)
			{				
				int currentCount2;
				int grpidtemp,lodmin,lodmax;
				//int lodrange1,lodrange2;
				LODItem item;

				for (int i=0;i<ncount;i++)
				{
					StationLODInfo info;
					info.staionID = data.GetDataArray(0)[i];
					info.index = i;
					grpidtemp = lodconfig.GetSTTypeByID(info.staionID); //根据站点编号获取组编号					
										
					if (grpidtemp!=-1)
					{
						int indexgrp = itemidlist.indexOf(grpidtemp);
						item = m_lodgroup.loditems.value(grpidtemp);
						lodmin = item.lodmin;
						lodmax = item.lodmax;
						lodrange1 = GetRangeMin(stepcountList,lodmin);
						lodrange2 = GetRangeMin(stepcountList,lodmax);
						info.lodLevel = GetRand(lodrange1,lodrange2);

						m_staionlodinfo.push_back(info);
						m_lodList.push_back(info.lodLevel);
					}					
				}
				//lyer->SetStaionLODInfo(m_staionlodinfo);

				LAYER_DISPLAY_INFO displayinfo;
				displayinfo.disnear = disnear;
				displayinfo.disfar 	= disfar;
				displayinfo.levelcount = range2-range1;
				//lyer->SetLayerDisPlayInfo(displayinfo);
				//lyer->ApplyLayerDisPlayInfo();

				lodconfig.WriteFile(lyer);
			}	
		}	
	}
}

/************************************************************************/
/* 函数功能： 填充云图标纹理点数组等					                        */
/* 参数说明:
/* 返 回 值:   NULL														*/
/* 作    者:  赵高攀														*/
/* 日    期:  2011 年 05 月 10日										*/
/************************************************************************/
void GradsProvider::DrawCloudSymbol(int i,float flon,float flat,float fCloud)
{
	QVector3D glPoint(flon,flat,0);
	int nCloud = (int)fCloud;
	if (nCloud < m_pCloudSymbolPointArray.count() && nCloud>-1)
	{
		if (i<m_lodList.count())
		{
			//m_pCloudSymbolPointArray.at(nCloud)->AddLodToList(m_lodList.at(i));
		}		
		m_pCloudSymbolPointArray.at(nCloud)->SetPosition(glPoint);
		m_pCloudSymbolPointArray.at(nCloud)->SetAngle(0.0f);
	}
}

/************************************************************************/
/* 函数功能： 绘制站点					                        */
/* 参数说明:
/* 返 回 值:   NULL														*/
/* 作    者:  赵高攀														*/
/* 日    期:  2011 年 05 月 10日										*/
/************************************************************************/
void GradsProvider::DrawStationPoint(float flon,float flat)
{
	QVector3DArray *pArray =m_stPoint->getVectexArray();
	QVector4DArray *pColor = m_stPoint->getColorArray();
	pArray->push_back(QVector3D(flon,flat,0));
	pColor->push_back(QVector4D(0.0f, 0.2f, 0.2f,1.0f));
}

/************************************************************************/
/* 函数功能： 填充风图标纹理点数组等					                        */
/* 参数说明:
/* 返 回 值:   NULL														*/
/* 作    者:  赵高攀														*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
void GradsProvider::DrawWindSymbol(int i,float flon,float flat,float fWindDirec,float fWindSpeed)
{
	QVector3D glPoint(flon,flat,0);
	int nspd = (int)fWindSpeed/2;
	if (nspd<m_pWindSymbolPointArray.count() && nspd >=0)
	{
		if (i<m_lodList.count())
		{
			//m_pWindSymbolPointArray.at(nspd)->AddLodToList(m_lodList.at(i));
		}		
		m_pWindSymbolPointArray.at(nspd)->SetPosition(glPoint);
		m_pWindSymbolPointArray.at(nspd)->SetAngle(fWindDirec);
	}
}

/************************************************************************/
/* 函数功能： 填充天气图标纹理点数组等					                */
/* 参数说明:  int nDraw 控制值绘制位置0绘制在坐标点，1按地面填图标准绘制
/* 返 回 值:   NULL														*/
/* 作    者:  赵高攀													*/
/* 日    期:  2011 年 05月 10 日										*/
/************************************************************************/
void GradsProvider::DrawWeatherSymbol(float flon,float flat,float fWeather,int nDraw)
{
	if (nDraw==0)
	{
	}else
	{
		flon=flon-0.09;
	}
	QVector3D glPoint(flon,flat,0);
	int nw = (int)fWeather;
	if (nw<m_pWeatherSymbolPointArray.count() && nw >-1)
	{
		m_pWeatherSymbolPointArray.at(nw)->SetPosition(glPoint);
		m_pWeatherSymbolPointArray.at(nw)->SetAngle(0.0f);
	}
}

/************************************************************************/
/* 函数功能： 填充图标贴值点数组					                    */
/* 参数说明: int nDraw 控制值绘制位置
/* 返 回 值:   NULL														*/
/* 作    者:  赵高攀													*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
void GradsProvider::DrawBitmapText(float flon,float flat,float fText,int nDraw)
{
	if (nDraw != 0 )
	{
		if ( nDraw== 1)
		{
			flon -= 0.09;
			flat -= 0.09;
		}else if ( nDraw== 2)
		{
			flon += 0.09;
			flat -= 0.09;
		}else if ( nDraw== 3)
		{
			flon += 0.09;
			flat += 0.09;
		}else if (nDraw == 4)
		{
			flon -= 0.09;
			flat += 0.09;
		}
	}
	int nText = (int)fText;
	if (nText != 9999 && nText != -999 && nText != 99999)
	{
		QString sText = QString::number(fText);
		m_PTempText->Pointion()->push_back(QVector3D(flon,flat,0));
		m_PTempText->StringArray()->append(sText);
	}
}

/************************************************************************/
/* 函数功能： 坐标点转换为经纬点					                        */
/* 参数说明:
/* 返 回 值:   NULL														*/
/* 作    者:  赵高攀														*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
bool GradsProvider::xy2lonlat(int x, int y, QPointF* lonlat ,CElement *tempEle)
{
	//if(x < 0 || x >= tempEle->Data().DataBlockDimensionSize(0,1) || y < 0 || y >= tempEle->Data().DataBlockDimensionSize(0,0)) return false;          // out of range

	double lon,lat;                                     // lon--x, lat--y
	lon = tempEle->SelectedLon().m_begin + x * tempEle->SelectedLon().m_delt;
	lat = tempEle->SelectedLat().m_begin + y * tempEle->SelectedLat().m_delt;
	lonlat->setX(lon);
	lonlat->setY(lat);
	return true;
}

/************************************************************************/
/* 函数功能： 屏幕坐标点转换为Gl点					                        */
/* 参数说明:
/* 返 回 值:   NULL														*/
/* 作    者:  赵高攀														*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
void GradsProvider::ScreenToGL( const QVector3D& screenPoint, QVector3D& glPoint )
{
	GLint    viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ = 0.0f;
	GLdouble posX, posY, posZ;

	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	winX = screenPoint.x();
	winY = viewport[3] - screenPoint.y();
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	glPoint.setX(posX);
	glPoint.setY(posY);
	glPoint.setZ(0.0);
}

bool GradsProvider::Dump4PolylineBar( QLeaf* container, QColor linecolor )
{
	if(m_pGPloyLineBar != NULL)
	{
		return m_pGPloyLineBar->Dump(container,linecolor);
	}
	return false;
}

bool GradsProvider::Dump4Profile( QLeaf* container )
{
	if(m_pTProfile != NULL)
	{
		return m_pTProfile->Dump(container);
	}
	if(m_pSProfile != NULL)
	{
		return m_pSProfile->Dump(container);
	}
	return false;
}

/************************************************************************/
/* 函数功能： 建立micaps绘图的相关属性				                        */
/* 参数说明:
/* 返 回 值:   NULL														*/
/* 作    者:  赵高攀														*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
void GradsProvider::GenerateMicapsAttribute(QString &strIndex)
{
	//ClearVector();

	VectorLayer *vLayer = dynamic_cast<VectorLayer*>(DataProvider::ParentLayer());
	if(NULL == vLayer) return;

	QMap<QString,ILayerAttribute*> *p_LayerAttribute;		// 产品属性集合
	p_LayerAttribute = vLayer->GetAttributeList();
	p_LayerAttribute->clear();
	QStringList n;

	QStringList strLatLonRange;
	strLatLonRange.append("inValid");
	ILayerAttribute * cLatLonRange = new ILayerAttribute("cLatLonRange","a",tr("显示地理范围"),"",QVariant::Invalid, 0, 0 ,strLatLonRange);
	ILayerAttribute *cLonStart = new ILayerAttribute("aa","aa",tr("起始经度"),m_MDAttribute.f_lonstart,QVariant::Double,0.0,360.0,n);
	cLatLonRange->InsertSubItem(cLonStart);
	ILayerAttribute *cLonEnd = new ILayerAttribute("ab","ab",tr("终止经度"),m_MDAttribute.f_lonend,QVariant::Double,0.0,360.0,n);
	cLatLonRange->InsertSubItem(cLonEnd);
	ILayerAttribute *cLatStart = new ILayerAttribute("ac","ac",tr("起始纬度"),m_MDAttribute.f_latstart,QVariant::Double,-89.9,89.9,n);
	cLatLonRange->InsertSubItem(cLatStart);
	ILayerAttribute *cLatEnd = new ILayerAttribute("ad","ad",tr("终止纬度"),m_MDAttribute.f_latend,QVariant::Double,-89.9,89.9,n);
	cLatLonRange->InsertSubItem(cLatEnd);
	p_LayerAttribute->insert("a",cLatLonRange);

	if (strIndex == tr("地面填图"))
	{
		vLayer->GraphicsType(16);
		GraphicType(16);
		QStringList strTCloud;
		strTCloud.append(tr("否"));
		strTCloud.append(tr("是"));
		ILayerAttribute *TCloud = new ILayerAttribute("bb","bb",tr("总云量"),1,QVariant::StringList,0,1,strTCloud);
		p_LayerAttribute->insert("bb",TCloud);
		QStringList strClab;
		strClab.append(tr("否"));
		strClab.append(tr("是"));
		ILayerAttribute * Wsymbol= new ILayerAttribute("bw","bw",tr("风标"),1,QVariant::StringList,0,1,strClab);
		p_LayerAttribute->insert("bw",Wsymbol);
		ILayerAttribute *Temp = new ILayerAttribute("bd","bd",tr("温度"),1,QVariant::StringList,0,1,strClab);
		p_LayerAttribute->insert("bd",Temp);
		ILayerAttribute * Flogpoint= new ILayerAttribute("be","be",tr("露点"),1,QVariant::StringList,0,1,strClab);
		p_LayerAttribute->insert("be",Flogpoint);
		ILayerAttribute * hourp= new ILayerAttribute("bf","bf",tr("3小时变压"),1,QVariant::StringList,0,1,strClab);
		p_LayerAttribute->insert("bf",hourp);
		ILayerAttribute *Seap = new ILayerAttribute("ag","ag",tr("海平面气压"),1,QVariant::StringList,0,1,strClab);
		p_LayerAttribute->insert("bg",Seap);
		ILayerAttribute *Curw = new ILayerAttribute("bh","bh",tr("现在天气"),1,QVariant::StringList,0,1,strClab);
		p_LayerAttribute->insert("bh",Curw);

		ILayerAttribute *Hattribute = new ILayerAttribute("c","c",tr("属性"),0,QVariant::StringList,0,1,strClab);
		p_LayerAttribute->insert("c",Hattribute);
	}
	else if (strIndex == tr("高空填图"))
	{
		vLayer->GraphicsType(17);
		GraphicType(17);
		QStringList strClab;
		strClab.append(tr("否"));
		strClab.append(tr("是"));
		ILayerAttribute *Wsymbol = new ILayerAttribute("b","b",tr("风标"),1,QVariant::StringList,0,1,strClab);
		p_LayerAttribute->insert("b",Wsymbol);
		ILayerAttribute * Hight= new ILayerAttribute("h","h",tr("高度"),1,QVariant::StringList,0,1,strClab);
		p_LayerAttribute->insert("c",Hight);
		ILayerAttribute *Temp = new ILayerAttribute("d","d",tr("温度"),1,QVariant::StringList,0,1,strClab);
		p_LayerAttribute->insert("d",Temp);
		ILayerAttribute * TeFg= new ILayerAttribute("e","e",tr("温度露点差"),1,QVariant::StringList,0,1,strClab);
		p_LayerAttribute->insert("e",TeFg);
	}
	else if (strIndex == tr("格点数据"))
	{
		vLayer->GraphicsType(11);
		GraphicType(11);
		QStringList strRange;
		strRange.append(tr("否"));
		strRange.append(tr("是"));
		ILayerAttribute * cRange = new ILayerAttribute("b","b",tr("显示符号"),1,QVariant::StringList, 0, 1 ,strRange);
		p_LayerAttribute->insert("b",cRange);
		QStringList strClabi;
		strClabi.append(tr("否"));
		strClabi.append(tr("是"));
		ILayerAttribute *Hattribute = new ILayerAttribute("c","c",tr("属性"),0,QVariant::StringList,0,1,strClabi);
		Hattribute->Pixmap(m_nPixmapIndex,m_Pixmap,m_nSymbolSize,m_fSymbolAngle);
		Hattribute->SetBshowValue(m_bShowValue,m_fMaxValue,m_fMinValue);
		Hattribute->SetFontAndColor(m_Font,m_FontColor,m_FontShow);		
		p_LayerAttribute->insert("c",Hattribute);
	}
	else
	{
		vLayer->GraphicsType(6);
		GraphicType(6);
		QStringList strRange;
		strRange.append(tr("否"));
		strRange.append(tr("是"));
		ILayerAttribute * cRange = new ILayerAttribute("b","b",tr("显示符号"),1,QVariant::StringList, 0, 1 ,strRange);
		p_LayerAttribute->insert("b",cRange);
		QStringList strClabi;
		strClabi.append(tr("否"));
		strClabi.append(tr("是"));
		ILayerAttribute *Hattribute = new ILayerAttribute("c","c",tr("属性"),0,QVariant::StringList,0,1,strClabi);
		Hattribute->Pixmap(m_nPixmapIndex,m_Pixmap,m_nSymbolSize,m_fSymbolAngle);
		Hattribute->SetBshowValue(m_bShowValue,m_fMaxValue,m_fMinValue);
		Hattribute->SetFontAndColor(m_Font,m_FontColor,m_FontShow);
		p_LayerAttribute->insert("c",Hattribute);
	}
}
/************************************************************************/
/* 函数功能： 更新Micaps绘图属性				                        */
/* 参数说明:
/* 返 回 值:   NULL														*/
/* 作    者:  赵高攀													*/
/* 日    期:  2011 年 05 月 10 日										*/
/************************************************************************/
int GradsProvider::UpdateMicapsAttribute(P_LAYER_ATTRIBUTE layerAttribe,ILayerAttribute*iChangedLARR,int ntype)
{
	ILayerAttribute* iLA = NULL;
	QMap<QString,ILayerAttribute*> *pSubLayerAttribute = NULL;

	if (ntype == 21)
	{
		iLA = layerAttribe->value(QString("aPath"));
		if (iLA) m_MDAttribute.b_aPath = iLA->Value().toBool();

		iLA = layerAttribe->value(QString("aa"));
		if (iLA) m_MDAttribute.c_aa = iLA->Value().value<QColor>();
		iLA = layerAttribe->value(QString("ab"));
		if (iLA) m_MDAttribute.c_ab = iLA->Value().value<QColor>();
		iLA = layerAttribe->value(QString("ac"));
		if (iLA) m_MDAttribute.c_ac = iLA->Value().value<QColor>();
		iLA = layerAttribe->value(QString("ad"));
		if (iLA) m_MDAttribute.c_ad = iLA->Value().value<QColor>();
		iLA = layerAttribe->value(QString("ae"));
		if (iLA) m_MDAttribute.c_ae = iLA->Value().value<QColor>();
		iLA = layerAttribe->value(QString("af"));
		if (iLA) m_MDAttribute.c_af = iLA->Value().value<QColor>();

		iLA = layerAttribe->value(QString("bColor"));
		if (iLA) m_MDAttribute.c_bColor = iLA->Value().value<QColor>();

		iLA = layerAttribe->value(QString("cPoint"));
		if (iLA) m_MDAttribute.b_cPoint = iLA->Value().toBool();

		iLA = layerAttribe->value(QString("dPoint8"));
		if (iLA) m_MDAttribute.b_dPoint8 = iLA->Value().toBool();

		iLA = layerAttribe->value(QString("dPoint8"));
		if (iLA) m_MDAttribute.b_dPoint8 = iLA->Value().toBool();

		iLA = layerAttribe->value(QString("eStyle"));
		if (iLA) m_MDAttribute.b_eStyle = iLA->Value().toInt();

		iLA = layerAttribe->value(QString("fSelectW"));
		if (iLA) m_MDAttribute.b_fSelectW = iLA->Value().toInt();

		if (iChangedLARR->Name() == "a")//更改属性列表
		{
			QStringList strLev;
			strLev.append("inValid");

			QStringList strLevs;
			strLevs.append(tr("否"));
			strLevs.append(tr("是"));

			QStringList strLStyle;
			strLStyle.append(tr("实线"));
			strLStyle.append(tr("点线"));
			strLStyle.append(tr("短划线"));

			QStringList n2;
			n2.append("");

			ILayerAttribute *cLevSub = 0;

			if (iChangedLARR->Value().toBool())//使用分级颜色列表
			{
				/******************************重新生成属性列表（删除）*****************************/
				QMap<QString,ILayerAttribute*> ::const_iterator iterAttr;
				iterAttr = layerAttribe->begin();
				for (;iterAttr != layerAttribe->end();iterAttr++)
				{
					iLA = iterAttr.value();
					if(!iLA) return 2;
					delete iterAttr.value();									// 删除二级属性
				}
				layerAttribe->clear();							// 清空三级属性Map
				/*************************************************************************/

				cLevSub = new ILayerAttribute("aPath","a",tr("是否显示分级路径"),1,QVariant::StringList,0,1,strLevs);
				layerAttribe->insert("aPath",cLevSub);

				cLevSub = new ILayerAttribute("aa","aa",tr("热带低压(10.8~17.1)"),QColor(0.16f*255, 0.95f*255, 0.2f*255,1.0f*255),QVariant::Color,0,15,n2);
				layerAttribe->insert("aa",cLevSub);
				cLevSub = new ILayerAttribute("ab","ab",tr("热带风暴(17.1~24.4)"),QColor(0.16f*255, 0.16f*255, 0.94f*255,1.0f*255),QVariant::Color,0,15,n2);
				layerAttribe->insert("ab",cLevSub);
				cLevSub = new ILayerAttribute("ac","ac",tr("热带风暴(24.4~32.6)"),QColor(0.92f*255, 0.92f*255, 0.2f*255,1.0f*255),QVariant::Color,0,15,n2);
				layerAttribe->insert("ac",cLevSub);
				cLevSub = new ILayerAttribute("ad","ad",tr("台风(32.6~41.4)"),QColor(0.92f*255, 0.64f*255, 0.2f*255,1.0f*255),QVariant::Color,0,15,n2);
				layerAttribe->insert("ad",cLevSub);
				cLevSub = new ILayerAttribute("ae","ae",tr("强台风(41.4~50.9)"),QColor(0.92f*255, 0.5f*255, 0.92f*255,1.0f*255),QVariant::Color,0,15,n2);
				layerAttribe->insert("ae",cLevSub);
				cLevSub = new ILayerAttribute("af","af",tr("超强台风(>50.9)"),QColor(0.92f*255, 0.17f*255, 0.2f*255,1.0f*255),QVariant::Color,0,15,n2);
				layerAttribe->insert("af",cLevSub);

// 				cLevSub = new ILayerAttribute("cPoint","b",tr("显示点"),(int)m_MDAttribute.b_cPoint,QVariant::StringList,0,1,strLevs);
// 				layerAttribe->insert("cPoint",cLevSub);

				cLevSub = new ILayerAttribute("dPoint8","c",tr("显示08时标记"),(int)m_MDAttribute.b_dPoint8,QVariant::StringList,0,1,strLevs);
				layerAttribe->insert("dPoint8",cLevSub);

// 				cLevSub = new ILayerAttribute("eStyle","e",tr("线型"),m_MDAttribute.b_eStyle,QVariant::StringList,0,2,strLStyle);
// 				layerAttribe->insert("eStyle",cLevSub);

				cLevSub = new ILayerAttribute("fSelectW","f",tr("线宽"),m_MDAttribute.b_fSelectW,QVariant::Int,1,10,n2);
				layerAttribe->insert("fSelectW",cLevSub);
			}
			else//使用单色
			{
				/******************************重新生成属性列表（删除）*****************************/
				QMap<QString,ILayerAttribute*> ::const_iterator iterAttr;
				iterAttr = layerAttribe->begin();
				for (;iterAttr != layerAttribe->end();iterAttr++)
				{
					iLA = iterAttr.value();
					if(!iLA) return 2;
					delete iterAttr.value();									// 删除二级属性
				}
				layerAttribe->clear();							// 清空三级属性Map
				/*************************************************************************/

				cLevSub = new ILayerAttribute("aPath","a",tr("是否显示分级路径"),0,QVariant::StringList,0,1,strLevs);
				layerAttribe->insert("aPath",cLevSub);

				cLevSub = new ILayerAttribute("bColor","d",tr("颜色"),QColor(0,127,0),QVariant::Color,0,15,n2);
				layerAttribe->insert("bColor",cLevSub);

// 				cLevSub = new ILayerAttribute("cPoint","b",tr("显示点"),(int)m_MDAttribute.b_cPoint,QVariant::StringList,0,1,strLevs);
// 				layerAttribe->insert("cPoint",cLevSub);

				cLevSub = new ILayerAttribute("dPoint8","c",tr("显示08时标记"),(int)m_MDAttribute.b_dPoint8,QVariant::StringList,0,1,strLevs);
				layerAttribe->insert("dPoint8",cLevSub);

// 				cLevSub = new ILayerAttribute("eStyle","e",tr("线型"),m_MDAttribute.b_eStyle,QVariant::StringList,0,2,strLStyle);
// 				layerAttribe->insert("eStyle",cLevSub);

				cLevSub = new ILayerAttribute("fSelectW","f",tr("线宽"),m_MDAttribute.b_fSelectW,QVariant::Int,1,10,n2);
				layerAttribe->insert("fSelectW",cLevSub);
			}
		}
		else//不修改属性列表
		{

		}
		
		return 1;
	}

	iLA = layerAttribe->value(QString("a"));
	pSubLayerAttribute = iLA->GetSubItems();

	iLA = pSubLayerAttribute->value(QString("aa"));
	m_MDAttribute.f_lonstart = iLA->Value().toFloat();
	iLA = pSubLayerAttribute->value(QString("ab"));
	m_MDAttribute.f_lonend = iLA->Value().toFloat();
	iLA = pSubLayerAttribute->value(QString("ac"));
	m_MDAttribute.f_latstart = iLA->Value().toFloat();
	iLA = pSubLayerAttribute->value(QString("ad"));
	m_MDAttribute.f_latend = iLA->Value().toFloat();
	m_bLonLatChanged = true;                                                 // 使MICAPS4类数据格点图范围属性更新有效 added by Liu Xinsheng 2011-07-04
	if (ntype == 6)
	{
		iLA = layerAttribe->value(QString("b"));
		m_MDAttribute.b_StationP = iLA->Value().toBool();

		if(iChangedLARR->LableName() == QString(tr("属性")))
		{
			//注销下面的代码，因为会引起系统崩溃,add by zhanglei
// 			if (m_pPixmapPoint != NULL) 
// 			{
// 				delete m_pPixmapPoint;
// 			}
		}

		iLA = layerAttribe->value(QString("c"));
		m_Pixmap = iLA->Pixmap(m_nPixmapIndex,m_nSymbolSize,m_fSymbolAngle);
		iLA->GetBshowValue(m_bShowValue,m_fMaxValue,m_fMinValue);
		iLA->GetFontAndColor(m_Font,m_FontColor,m_FontShow);

	}
	else if (ntype == 11)
	{
		iLA = layerAttribe->value(QString("b"));
		m_MDAttribute.b_StationP = iLA->Value().toBool();

		if(iChangedLARR->LableName() == QString(tr("属性")))
		{

		}
		iLA = layerAttribe->value(QString("c"));
		m_Pixmap = iLA->Pixmap(m_nPixmapIndex,m_nSymbolSize,m_fSymbolAngle);
		iLA->GetBshowValue(m_bShowValue,m_fMaxValue,m_fMinValue);
		iLA->GetFontAndColor(m_Font,m_FontColor,m_FontShow);

	}
	else if (ntype == 16)
	{
		iLA = layerAttribe->value(QString("bb"));
		if (iLA)
		   m_MDAttribute.b_dTCloud = iLA->Value().toBool();

		iLA = layerAttribe->value(QString("bw"));
		if (iLA)
		   m_MDAttribute.b_dWSymbol = iLA->Value().toBool();

		iLA = layerAttribe->value(QString("bd"));
		if (iLA)
		   m_MDAttribute.b_dTemperature = iLA->Value().toBool();

		iLA = layerAttribe->value(QString("be"));
		if (iLA)	
		  m_MDAttribute.b_dFlogP = iLA->Value().toBool();

		iLA = layerAttribe->value(QString("bf"));
		if (iLA)
		   m_MDAttribute.b_d3HourPre = iLA->Value().toBool();

		iLA = layerAttribe->value(QString("bg"));
		if (iLA)		
		 m_MDAttribute.b_dSeaPre = iLA->Value().toBool();

		iLA = layerAttribe->value(QString("bh"));
		if (iLA)	
			m_MDAttribute.b_dCurWeather = iLA->Value().toBool();		
		
	}
	else if (ntype == 17)
	{
		iLA = layerAttribe->value(QString("b"));
		m_MDAttribute.b_gWSymbol = iLA->Value().toBool();
		iLA = layerAttribe->value(QString("c"));
		m_MDAttribute.b_gHight = iLA->Value().toBool();
		iLA = layerAttribe->value(QString("d"));
		m_MDAttribute.b_gTemperature = iLA->Value().toBool();
		iLA = layerAttribe->value(QString("e"));
		m_MDAttribute.b_gTempFlogPGap = iLA->Value().toBool();
	}
	
	return 1;
}

QString GradsProvider::GetTreeName(int n)
{
	if (n<m_strTreeName.size())
	{
		return m_strTreeName.at(n);
	}
	else
		if (m_strTreeName.size()>0)
		{
			return m_strTreeName.at(0);
		}
		return QString("");
}

void GradsProvider::SetTreeName( QString str)
{
	m_strTreeName << str;
}
//站点分级符号数据结构
void GradsProvider::GradsSymbolClear()
{
	m_StationClassifyInfoList.clear();
}
//站点分级符号数据结构
void GradsProvider::GradsSymbolSettings(StationClassifyInfo &StationClaInfo)
{
	m_StationClassifyInfoList.append(StationClaInfo);
}
//站点分级符号数据结构
void GradsProvider::GradsSymbolGettings(QList<StationClassifyInfo> &StationClassifyInfoList)
{
	StationClassifyInfoList = m_StationClassifyInfoList;
}
//站点分级标值数据结构
void GradsProvider::GradsValueClear()
{
	m_StationClassifyValueList.clear();
}
//站点分级标值数据结构
void GradsProvider::GradsValueSettings(StationClassifyValue &StationClaValue)
{
	m_StationClassifyValueList.append(StationClaValue);
}
//站点分级标值数据结构
void GradsProvider::GradsValueGettings(QList<StationClassifyValue> &StationClassifyValueList)
{
	StationClassifyValueList = m_StationClassifyValueList;
}

bool GradsProvider::Array4CalculateElement(QString strCalculate,QList<CElement*> *pListCElement,int nx,int ny,int nz,int nt,float f_missingdata,bool NorthToSouth)
{
	int nCalCount = strCalculate.count(QRegExp("[\+\*-/]"));
	int nNumCount = strCalculate.count(QRegExp("[\+\*-/][0-9]+\.?[0-9]*"));
	int nEleCount = pListCElement->count();

	long nSingleEleArrSize = m_nLonNCCount * m_nLatNCCount * m_nTimeNCCount * m_nLevelNCCount;
	long nMultiEleArrSize = m_nLonNCCount * m_nLatNCCount * m_nTimeNCCount * m_nLevelNCCount * m_nElementNum;

	bool isCalculate = false;
	if (nEleCount + nNumCount == nCalCount + 1 && nCalCount != 0)
	{
		//qDebug()<<"OK";
		isCalculate = true;
	}

	m_nElementNum = pListCElement->count();
	float f_Accuracy = 0.0001;

	int nElementIndex;
	bool isSingleEle = true;
	bool isDoubleEle = false;
	bool bOk = false;
	bool bUseAdd = false;
	bool bUseSubtract = false;
	bool bUseMultiply = false;
	bool bUseDivide = false;
	QString strNumber;
	float fCalculate;

	float *pData = NULL;

	if (strCalculate.count(QRegExp("^[a-zA-Z]+[\+\*/-][a-zA-Z]+$")) == 1)
	{
		isDoubleEle = true;
		isSingleEle = false;
		strCalculate.remove(QRegExp("[a-zA-Z]"));
		if (strCalculate.contains("+"))
		{
			bUseAdd = true;
		}
		else if (strCalculate.contains("-"))
		{
			bUseSubtract = true;
		}
		else if (strCalculate.contains("*"))
		{
			bUseMultiply = true;
		}
		else if (strCalculate.contains("/"))
		{
			bUseDivide = true;
		}
	}
	else if (strCalculate.count(QRegExp("^[a-zA-Z]+[\+\*/-][0-9]+\.?[0-9]*$")) == 1)
	{
		isSingleEle = true;
		isDoubleEle = false;
		strCalculate.remove(QRegExp("[a-zA-Z]"));
		if (strCalculate.contains("+"))
		{
			strNumber = strCalculate.remove("+");
			fCalculate = strNumber.toFloat(&bOk);
			if (bOk)
			{
				bUseAdd = true;
			}
		}
		else if (strCalculate.contains("-"))
		{
			strNumber = strCalculate.remove("-");
			fCalculate = strNumber.toFloat(&bOk);
			if (bOk)
			{
				bUseSubtract = true;
			}
		}
		else if (strCalculate.contains("*"))
		{
			strNumber = strCalculate.remove("*");
			fCalculate = strNumber.toFloat(&bOk);
			if (bOk)
			{
				bUseMultiply = true;
			}
		}
		else if (strCalculate.contains("/"))
		{
			strNumber = strCalculate.remove("/");
			fCalculate = strNumber.toFloat(&bOk);
			if (bOk)
			{
				bUseDivide = true;
			}
		}
	}

	if (pListCElement->count() > 1)
	{
		for (nElementIndex = 1;nElementIndex < pListCElement->count();nElementIndex++)
		{
			int nextEleLon	= pListCElement->at(nElementIndex)->SelectedLon().m_count		>0 ? pListCElement->at(nElementIndex)->SelectedLon().m_count	:1;
			int nextEleLat	= pListCElement->at(nElementIndex)->SelectedLat().m_count		>0 ? pListCElement->at(nElementIndex)->SelectedLat().m_count	:1;
			int nextEleTim	= pListCElement->at(nElementIndex)->SelectedPeriod().m_count	>0 ? pListCElement->at(nElementIndex)->SelectedPeriod().m_count	:1;
			int nextEleLev	= pListCElement->at(nElementIndex)->Level().m_selected.size()	>0 ? pListCElement->at(nElementIndex)->Level().m_selected.size():1;

			if (!(qAbs(nextEleLon*nextEleLat*nextEleTim*nextEleLev - nSingleEleArrSize) < f_Accuracy))
			{
				return false;
			}
		}
	}

	m_dArrayValue = (double *)malloc(sizeof(double) * nSingleEleArrSize);
	pData = pListCElement->at(0)->Data().GetDataArray(0);
	//pData = pListCElement->at(0)->Data().GetDataArray(0);
	//******************************** Write data ********************************
	int i,j,k,l,n = 0;
	for(int ne = 0; ne < 1; ne++)
	{
		for(i = 0; i < nt; i++)
		{
			for(j = 0; j < nz; j++)          // Output pElem1 to m_dArrayValue
			{
				if(NorthToSouth)
				{
					for(k = ny - 1; k >=0; k--)   // 由北到南：南北顺序相反，要颠倒过来
					{
						for(l= 0; l < nx; l++)
						{
							if (isSingleEle)
							{
								

								if (bUseAdd)			// +
								{
									if (qAbs(pData[i * nz * ny * nx + j * ny * nx + k * nx + l] - f_missingdata) < f_Accuracy)
									{
										m_dArrayValue[n] = m_fMissingV;
										++n;
									}
									else
									{
										m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l] + fCalculate;
										++n;
									}
								}
								else if (bUseSubtract)	// -
								{
									if (qAbs(pData[i * nz * ny * nx + j * ny * nx + k * nx + l] - f_missingdata) < f_Accuracy)
									{
										m_dArrayValue[n] = m_fMissingV;
										++n;
									}
									else
									{
										m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l] - fCalculate;
										++n;
									}
								}
								else if (bUseMultiply)	// *
								{
									if (qAbs(pData[i * nz * ny * nx + j * ny * nx + k * nx + l] - f_missingdata) < f_Accuracy)
									{
										m_dArrayValue[n] = m_fMissingV;
										++n;
									}
									else
									{
										m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l] * fCalculate;
										++n;
									}
								}
								else if (bUseDivide)	// /
								{
									if (qAbs(pData[i * nz * ny * nx + j * ny * nx + k * nx + l] - f_missingdata) < f_Accuracy)
									{
										m_dArrayValue[n] = m_fMissingV;
										++n;
									}
									else
									{
										m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l] / fCalculate;
										++n;
									}
								}
								else	//normal display!!!!
								{
									if (qAbs(pData[i * nz * ny * nx + j * ny * nx + k * nx + l] - f_missingdata) < f_Accuracy)
									{
										m_dArrayValue[n] = m_fMissingV;
										++n;
									}
									else
									{
										m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l];
										++n;
									}
								}
							}
							else if (isDoubleEle)
							{
								float *fElement;
								fElement = (float *)malloc(sizeof(float) * pListCElement->count());

								for (nElementIndex = 0;nElementIndex < pListCElement->count();nElementIndex++)
								{
									if(pListCElement->at(nElementIndex) != NULL)
									{
										pData = pListCElement->at(nElementIndex)->Data().GetDataArray(0);
										float tempMissDat= pListCElement->at(0)->MissingValue();
										if (qAbs(pData[i * nz * ny * nx + j * ny * nx + k * nx + l] - tempMissDat) < f_Accuracy)
										{
											m_dArrayValue[n] = m_fMissingV;
											++n;
										}
										else
										{
											fElement[nElementIndex] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l];
										}
									}
								}// endfor ListElement

								if (bUseAdd)			// +
								{
									if (!(qAbs(m_dArrayValue[n] - m_fMissingV) < f_Accuracy))
									{
										m_dArrayValue[n] = fElement[0] + fElement[1];
										++n;
									}
								}
								else if (bUseSubtract)	// -
								{
									if (!(qAbs(m_dArrayValue[n] - m_fMissingV) < f_Accuracy))
									{
										m_dArrayValue[n] = fElement[0] - fElement[1];
										++n;
									}
								}
								else if (bUseMultiply)	// *
								{
									if (!(qAbs(m_dArrayValue[n] - m_fMissingV) < f_Accuracy))
									{
										m_dArrayValue[n] = fElement[0] * fElement[1];
										++n;
									}
								}
								else if (bUseDivide)	// /
								{
									if (!(qAbs(m_dArrayValue[n] - m_fMissingV) < f_Accuracy))
									{
										m_dArrayValue[n] = fElement[0] / fElement[1];
										++n;
									}
								}
								else	//something wrong!!!!
								{
									//qDebug()<<"something wrong!!!! GradsProvider::NetCDF2Array";
								}
							}
						}// endfor nx
					}// endfor ny
				}
				else
				{
					for(k = 0; k < ny; k++)   //由南到北
					{
						for(l= 0; l < nx; l++)
						{
							if (isSingleEle)
							{
								

								if (bUseAdd)			// +
								{
									if (qAbs(pData[i * nz * ny * nx + j * ny * nx + k * nx + l] - f_missingdata) < f_Accuracy)
									{
										m_dArrayValue[n] = m_fMissingV;
										++n;
									}
									else
									{
										m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l] + fCalculate;
										++n;
									}
								}
								else if (bUseSubtract)	// -
								{
									if (qAbs(pData[i * nz * ny * nx + j * ny * nx + k * nx + l] - f_missingdata) < f_Accuracy)
									{
										m_dArrayValue[n] = m_fMissingV;
										++n;
									}
									else
									{
										m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l] - fCalculate;
										++n;
									}
								}
								else if (bUseMultiply)	// *
								{
									if (qAbs(pData[i * nz * ny * nx + j * ny * nx + k * nx + l] - f_missingdata) < f_Accuracy)
									{
										m_dArrayValue[n] = m_fMissingV;
										++n;
									}
									else
									{
										m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l] * fCalculate;
										++n;
									}
								}
								else if (bUseDivide)	// /
								{
									if (qAbs(pData[i * nz * ny * nx + j * ny * nx + k * nx + l] - f_missingdata) < f_Accuracy)
									{
										m_dArrayValue[n] = m_fMissingV;
										++n;
									}
									else
									{
										m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l] / fCalculate;
										++n;
									}
								}
								else	//normal display!!!!
								{
									if (qAbs(pData[i * nz * ny * nx + j * ny * nx + k * nx + l] - f_missingdata) < f_Accuracy)
									{
										m_dArrayValue[n] = m_fMissingV;
										++n;
									}
									else
									{
										m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l];
										++n;
									}
								}
							}
							else if (isDoubleEle)
							{
								float *fElement;
								fElement = (float *)malloc(sizeof(float) * pListCElement->count());

								for (nElementIndex = 0;nElementIndex < pListCElement->count();nElementIndex++)
								{
									if(pListCElement->at(nElementIndex) != NULL)
									{
										pData = pListCElement->at(nElementIndex)->Data().GetDataArray(0);
										float tempMissDat= pListCElement->at(0)->MissingValue();
										if (qAbs(pData[i * nz * ny * nx + j * ny * nx + k * nx + l] - tempMissDat) < f_Accuracy)
										{
											m_dArrayValue[n] = m_fMissingV;
											++n;
										}
										else
										{
											fElement[nElementIndex] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l];
										}
									}
								}// endfor ListElement

								if (bUseAdd)			// +
								{
									if (!(qAbs(m_dArrayValue[n] - m_fMissingV) < f_Accuracy))
									{
										m_dArrayValue[n] = fElement[0] + fElement[1];
										++n;
									}
								}
								else if (bUseSubtract)	// -
								{
									if (!(qAbs(m_dArrayValue[n] - m_fMissingV) < f_Accuracy))
									{
										m_dArrayValue[n] = fElement[0] - fElement[1];
										++n;
									}
								}
								else if (bUseMultiply)	// *
								{
									if (!(qAbs(m_dArrayValue[n] - m_fMissingV) < f_Accuracy))
									{
										m_dArrayValue[n] = fElement[0] * fElement[1];
										++n;
									}
								}
								else if (bUseDivide)	// /
								{
									if (!(qAbs(m_dArrayValue[n] - m_fMissingV) < f_Accuracy))
									{
										m_dArrayValue[n] = fElement[0] / fElement[1];
										++n;
									}
								}
								else	//something wrong!!!!
								{
									//qDebug()<<"something wrong!!!! GradsProvider::NetCDF2Array";
								}
							}
						}// endfor nx
					}// endfor ny
				}// else
			} // endfor nz
		} // endfor nt
	} // endfor ne

	m_nLonNCCount	= pListCElement->at(0)->SelectedLon().m_count		>0 ? pListCElement->at(0)->SelectedLon().m_count	:1;
	m_nLatNCCount	= pListCElement->at(0)->SelectedLat().m_count		>0 ? pListCElement->at(0)->SelectedLat().m_count	:1;
	m_nTimeNCCount	= pListCElement->at(0)->SelectedPeriod().m_count	>0 ? pListCElement->at(0)->SelectedPeriod().m_count	:1;
	m_nLevelNCCount	= pListCElement->at(0)->Level().m_selected.size()	>0 ? pListCElement->at(0)->Level().m_selected.size():1;

	m_fLonNCBegin	= pListCElement->at(0)->SelectedLon().m_begin	<= pListCElement->at(0)->SelectedLon().m_end? pListCElement->at(0)->SelectedLon().m_begin	: pListCElement->at(0)->SelectedLon().m_end;
	m_fLonNCDelta	= pListCElement->at(0)->SelectedLon().m_delt	>= 0										? pListCElement->at(0)->SelectedLon().m_delt	: -1 * pListCElement->at(0)->SelectedLon().m_delt;
	m_fLatNCBegin	= pListCElement->at(0)->SelectedLat().m_begin	<= pListCElement->at(0)->SelectedLat().m_end? pListCElement->at(0)->SelectedLat().m_begin	: pListCElement->at(0)->SelectedLat().m_end;
	m_fLatNCDelta	= pListCElement->at(0)->SelectedLat().m_delt	>= 0										? pListCElement->at(0)->SelectedLat().m_delt	: -1 * pListCElement->at(0)->SelectedLat().m_delt;

	m_fMissingV = pListCElement->at(0)->MissingValue();

	if (m_pTempElement)
	{
		delete m_pTempElement;
		m_pTempElement = NULL;
	}

	m_pTempElement = new CElement(*pListCElement->at(0));

	m_pTempElement->SelectedLon().m_count = m_nLonNCCount;
	m_pTempElement->SelectedLat().m_count = m_nLatNCCount;
	m_pTempElement->SelectedPeriod().m_count = m_nTimeNCCount;
	m_pTempElement->Level().m_count = m_nLevelNCCount;

	m_pTempElement->SelectedLon().m_begin = m_fLonNCBegin;
	m_pTempElement->SelectedLon().m_delt = m_fLonNCDelta;
	m_pTempElement->SelectedLat().m_begin = m_fLatNCBegin;
	m_pTempElement->SelectedLat().m_delt = m_fLatNCDelta;

	m_pTempElement->MissingValue(m_fMissingV);

	return true;
}

bool GradsProvider::Array4MultiElement( QList<CElement*> *pListCElement,int nx,int ny,int nz,int nt,float f_missingdata,bool NorthToSouth )
{
	struct element
	{
		QVector<float> vfLon;
		QVector<float> vfLat;
		QVector<float> vfLevel;
		QVector<QDateTime> vQTime;
	};

	QVector<element> vMultiElement;

	float fLonDelt = 0;
	float fLatDelt = 0;
	int nTimeValueDelt = 0;
	int nTimeUnitDelt = 0;

	float *pData = NULL;
	int i, j, k, l;
	long n;
	int nElementIndex;

	int *nMarkFlag = (int*)malloc(sizeof(int)*4);
	memset(nMarkFlag,0,sizeof(int)*4);
	for (int nElementIndex = 0;nElementIndex < pListCElement->count();nElementIndex++)
	{
		nx	= pListCElement->at(nElementIndex)->SelectedLon().m_count		>0 ? pListCElement->at(nElementIndex)->SelectedLon().m_count:1;
		ny	= pListCElement->at(nElementIndex)->SelectedLat().m_count		>0 ? pListCElement->at(nElementIndex)->SelectedLat().m_count:1;
		nz	= pListCElement->at(nElementIndex)->Level().m_selected.size()	>0 ? pListCElement->at(nElementIndex)->Level().m_selected.size():1;
		nt	= pListCElement->at(nElementIndex)->SelectedPeriod().m_count	>0 ? pListCElement->at(nElementIndex)->SelectedPeriod().m_count:1;

		QVector<float> currVectorLon;
		if (nx == 1)						/*没有经度维*/
		{
			currVectorLon.push_back(-9999);
		}
		else if (nx > 1)					/*有经度维*/
		{
			if (nElementIndex == 0)				/*经度维数据验证*/
			{
				nMarkFlag[0] = 1;
				fLonDelt = pListCElement->at(nElementIndex)->SelectedLon().m_delt;
			}
			else
			{
				if (nMarkFlag[0] != 1)
					return 1;
				if (!qAbs(fLonDelt - pListCElement->at(nElementIndex)->SelectedLon().m_delt) < 0.001)
					return 1;
			}

			for (int i = 0;i < nx;i++)			/*经度维数据保存*/
			{
				float currLon = pListCElement->at(nElementIndex)->SelectedLon().m_begin + pListCElement->at(nElementIndex)->SelectedLon().m_delt*i;
				currVectorLon.push_back(currLon);
			}
		}

		QVector<float> currVectorLat;
		if (ny == 1)						/*没有纬度维*/
		{
			currVectorLat.push_back(-9999);
		}
		else if (ny > 1)					/*有纬度维*/
		{
			if (nElementIndex == 0)				/*纬度维数据验证*/
			{
				nMarkFlag[1] = 1;
				fLatDelt = pListCElement->at(nElementIndex)->SelectedLat().m_delt;
			}
			else
			{
				if (nMarkFlag[1] != 1)
					return 1;
				if (!qAbs(fLatDelt - pListCElement->at(nElementIndex)->SelectedLat().m_delt) < 0.001)
					return 1;
			}

			for (int i = 0;i < ny;i++)			/*纬度维数据保存*/
			{
				float currLat = pListCElement->at(nElementIndex)->SelectedLat().m_begin + pListCElement->at(nElementIndex)->SelectedLat().m_delt*i;
				currVectorLat.push_back(currLat);
			}
		}

		QVector<float> currVectorLevel;
		if (nz == 1)						/*没有高度维*/
		{
			currVectorLevel.push_back(-9999);
		}
		else if (nz > 1)					/*有高度维*/
		{
			if (nElementIndex == 0)				/*高度维数据验证*/
				nMarkFlag[2] = 1;
			else
				if (nMarkFlag[2] != 1)
					return 1;

			for (int i = 0;i < nz;i++)			/*高度维数据保存*/
			{
				float currLevel = pListCElement->at(nElementIndex)->Level().m_selected.value(i);
				currVectorLevel.push_back(currLevel);
			}
		}

		QVector<QDateTime> currVectorTime;
		if (nt == 1)						/*没有时间维*/
		{
			currVectorTime.push_back(QDateTime(QDate (0,0,0),QTime(0,0)));
		}
		else if (nt > 1)					/*有时间维*/
		{
			if (nElementIndex == 0)				/*时间维数据验证*/
			{
				nMarkFlag[3] = 1;
				nTimeValueDelt = pListCElement->at(nElementIndex)->SelectedPeriod().GetTimeScale().GetValue();
				nTimeUnitDelt = pListCElement->at(nElementIndex)->SelectedPeriod().GetTimeScale().GetUnit();
			}
			else
			{
				if (nMarkFlag[3] != 1)
					return 1;
				if (nTimeValueDelt != pListCElement->at(nElementIndex)->SelectedPeriod().GetTimeScale().GetValue() ||
					nTimeUnitDelt != pListCElement->at(nElementIndex)->SelectedPeriod().GetTimeScale().GetUnit())
					return 1;
			}

			for (int i = 0;i < nt;i++)			/*时间维数据保存*/
			{
				QDateTime currBeginTime = pListCElement->at(nElementIndex)->SelectedPeriod().m_begin;
				int nScaleValue = pListCElement->at(nElementIndex)->SelectedPeriod().GetTimeScale().GetValue();
				int nScaleUnit = pListCElement->at(nElementIndex)->SelectedPeriod().GetTimeScale().GetUnit();

				enum Enum_TimeUnit
				{
					Year = 1,
					Month = 2,
					Day = 3,
					Hour = 4,
					Munite = 5,
					Second = 6,
					Season = 7, // 季度
					TenDays = 8, // 旬
					FiveDays = 9, // 侯
					FiveDaysOfChina = 10, // 中国侯
					Week = 11,
					Decade = 12,// 年代
					Century = 13 //世纪
				};

				QDateTime currTime;
				if (nScaleUnit == Year)
				{
					currTime = currBeginTime.addYears(nScaleValue*i);
				}
				else if (nScaleUnit == Month)
				{
					currTime = currBeginTime.addMonths(nScaleValue*i);
				}
				else if (nScaleUnit == Day)
				{
					currTime = currBeginTime.addDays(nScaleValue*i);
				}
				else if (nScaleUnit == Hour)
				{
					currTime = currBeginTime.addSecs(nScaleValue*i*(int)3600);
				}
				else if (nScaleUnit == Munite)
				{
					currTime = currBeginTime.addSecs(nScaleValue*i*(int)60);
				}
				else if (nScaleUnit == Second)
				{
					currTime = currBeginTime.addSecs(nScaleValue*i);
				}
				else if (nScaleUnit == Season)
				{
					currTime = currBeginTime.addMonths(nScaleValue*i*(int)3);
				}
				else if (nScaleUnit == TenDays)
				{
					currTime = currBeginTime.addDays(nScaleValue*i*(int)10);
				}
				else if (nScaleUnit == FiveDaysOfChina)
				{
					currTime = currBeginTime.addDays(nScaleValue*i*(int)5);
				}
				else if (nScaleUnit == Week)
				{
					currTime = currBeginTime.addDays(nScaleValue*i*(int)7);
				}
				else if (nScaleUnit == Decade)
				{
					currTime = currBeginTime.addYears(nScaleValue*i*(int)10);
				}
				else if (nScaleUnit == Century)
				{
					currTime = currBeginTime.addYears(nScaleValue*i*(int)100);
				}
				else
				{
					//qDebug()<<"GradsProvider.cpp NetCDF2Array something wrong!!!!";
					return 1;
				}

				currVectorTime.push_back(currTime);
			}
		}

		element struckElement;
		struckElement.vfLon = currVectorLon;
		struckElement.vfLat = currVectorLat;
		struckElement.vfLevel = currVectorLevel;
		struckElement.vQTime = currVectorTime;

		vMultiElement.push_back(struckElement);
	}

	QVector<float> vFinalLon;
	QVector<float> vFinalLat;
	QVector<float> vFinalLevel;
	QVector<QDateTime> vFinalTime;

	if(nx == 1)
	{
		vFinalLon.push_back(vMultiElement.at(0).vfLon.at(0));
	}
	else if (nx>1)
	{
		float fMinLon = 180;
		float fMaxLon = -180;
		for (int i = 0;i < vMultiElement.count();i++)
		{
			QVector<float> tempVLon = vMultiElement.at(i).vfLon;
			qSort(tempVLon.begin(), tempVLon.end());

			if (fMinLon > tempVLon.at(0))
			{
				fMinLon = tempVLon.at(0);
			}
			if (fMaxLon < tempVLon.at(tempVLon.count()-1))
			{
				fMaxLon = tempVLon.at(tempVLon.count()-1);
			}
		}

		float fCurrLon = fMinLon;
		for (;fCurrLon <= fMaxLon; fCurrLon+=qAbs(fLonDelt))
		{
			vFinalLon.push_back(fCurrLon);
		}
		nx = vFinalLon.count();
	}

	if (ny == 1)
	{
		vFinalLat.push_back(vMultiElement.at(0).vfLat.at(0));
	}
	else if (ny>1)
	{
		float fMinLat = 180;
		float fMaxLat = -180;
		for (int i = 0;i < vMultiElement.count();i++)
		{
			QVector<float> tempVLat = vMultiElement.at(i).vfLat;
			qSort(tempVLat.begin(), tempVLat.end());

			if (fMinLat > tempVLat.at(0))
			{
				fMinLat = tempVLat.at(0);
			}
			if (fMaxLat < tempVLat.at(tempVLat.count()-1))
			{
				fMaxLat = tempVLat.at(tempVLat.count()-1);
			}
		}

		float fCurrLat = fMinLat;
		for (;fCurrLat <= fMaxLat; fCurrLat+=qAbs(fLatDelt))
		{
			vFinalLat.push_back(fCurrLat);
		}
		ny = vFinalLat.count();
	}

	if (nz == 1)
	{
		vFinalLevel.push_back(vMultiElement.at(0).vfLevel.at(0));
	}
	else if (nz>1)
	{
		for (int i = 0;i < vMultiElement.count();i++)
		{
			QVector<float> tempVLevel = vMultiElement.at(i).vfLevel;
			qSort(tempVLevel.begin(), tempVLevel.end());

			for (int j = 0;j < tempVLevel.count();j++)
			{
				if (!vFinalLevel.contains(tempVLevel.at(j)))
				{
					vFinalLevel.push_back(tempVLevel.at(j));
				}
			}
		}
		qSort(vFinalLevel.begin(), vFinalLevel.end());
		nz = vFinalLevel.count();
	}

	if (nt == 1)
	{
		vFinalTime.push_back(vMultiElement.at(0).vQTime.at(0));
	}
	else if (nt>1)
	{
		for (int i = 0;i < vMultiElement.count();i++)
		{
			QVector<QDateTime> tempQTime = vMultiElement.at(i).vQTime;
			qSort(tempQTime.begin(),tempQTime.end());

			for (int j = 0;j < tempQTime.count();j++)
			{
			//	if (!vFinalTime.contains(tempQTime.at(j)))
				{
					vFinalTime.push_back(tempQTime.at(j));
				}
			}
		}
		qSort(vFinalTime.begin(),vFinalTime.end());
		nt = vFinalTime.count();
	}

	//*******************************  Write *.dat    the data file******************************************
	long m_dArrayValueSize = nt*nz*nx*ny*pListCElement->count();
	m_dArrayValue = (double *)malloc(sizeof(double) * m_dArrayValueSize);
	////qDebug()<<"m_dArrayValueSize="<<m_dArrayValueSize;

	//FILE *fp = NULL;
	//char* strtestDAT = "test.dat";
	//fopen_s(&fp, strtestDAT, "wb");

	n = 0;
	for(int ne = 0; ne < 1; ne++)
	{
		for(i = 0; i < nt; i++)
		{
			for (nElementIndex = 0;nElementIndex < pListCElement->count();nElementIndex++)
			{
				pData = pListCElement->at(nElementIndex)->Data().GetDataArray(0);

				int nSpecified_I = -9999;
				if (vMultiElement.at(nElementIndex).vQTime.contains(vFinalTime.at(i)))
				{
					nSpecified_I = vMultiElement.at(nElementIndex).vQTime.indexOf(vFinalTime.at(i));
				}

				for(j = 0; j < nz; j++)          // Output pElem1 to m_dArrayValue
				{
					int nSpecified_J = -9999;
					if (vMultiElement.at(nElementIndex).vfLevel.contains(vFinalLevel.at(j)))
					{
						nSpecified_J = vMultiElement.at(nElementIndex).vfLevel.indexOf(vFinalLevel.at(j));
					}

					for(k = 0; k < ny; k++)   //由南到北
					{
						int nSpecified_K = -9999;
						if (vMultiElement.at(nElementIndex).vfLat.contains(vFinalLat.at(k)))
						{
							nSpecified_K = vMultiElement.at(nElementIndex).vfLat.indexOf(vFinalLat.at(k));
						}

						for(l= 0; l < nx; l++)
						{
							int nSpecified_L = -9999;
							if (vMultiElement.at(nElementIndex).vfLon.contains(vFinalLon.at(l)))
							{
								nSpecified_L = vMultiElement.at(nElementIndex).vfLon.indexOf(vFinalLon.at(l));
							}

							if (nSpecified_I != -9999 && nSpecified_J != -9999 && nSpecified_K != -9999 && nSpecified_L != -9999)
							{
								int nLocalx	= pListCElement->at(nElementIndex)->SelectedLon().m_count		>0 ? pListCElement->at(nElementIndex)->SelectedLon().m_count:1;
								int nLocaly	= pListCElement->at(nElementIndex)->SelectedLat().m_count		>0 ? pListCElement->at(nElementIndex)->SelectedLat().m_count:1;
								int nLocalz	= pListCElement->at(nElementIndex)->Level().m_selected.size()	>0 ? pListCElement->at(nElementIndex)->Level().m_selected.size():1;
								int nLocalt	= pListCElement->at(nElementIndex)->SelectedPeriod().m_count	>0 ? pListCElement->at(nElementIndex)->SelectedPeriod().m_count:1;

								float currValue = pData[nSpecified_I * nLocalz * nLocaly * nLocalx + nSpecified_J * nLocaly * nLocalx + nSpecified_K * nLocalx + nSpecified_L];
								if (!qAbs(currValue - pListCElement->at(nElementIndex)->MissingValue()) < 0.001)
								{
									m_dArrayValue[n] = currValue;
								}
								else
								{
									m_dArrayValue[n] = 32767.0f;
								}
							}
							else
							{
								m_dArrayValue[n] = 32767.0f;
							}
							n++;
						}// endfor nx
					}// endfor ny
				} // endfor nz
			}// endfor ListElement
		} // endfor nt
	} // endfor ne
	//fclose(fp);

	////qDebug()<<"n="<<n;
	//for (n = 0;n < m_dArrayValueSize;n++)
	//{
	//	//qDebug()<<"m_dArrayValue"<<n<<m_dArrayValue[n];
	//}

	if (m_pTempElement)
	{
		delete m_pTempElement;
		m_pTempElement = NULL;
	}
	m_pTempElement = new CElement(*pListCElement->at(0));

	if (nx>1)
	{
		m_pTempElement->SelectedLon().m_begin = vFinalLon.at(0);
		m_pTempElement->SelectedLon().m_end = vFinalLon.at(vFinalLon.count()-1);
		m_pTempElement->SelectedLon().m_count = vFinalLon.count();
	}
	if (ny>1)
	{
		m_pTempElement->SelectedLat().m_begin = vFinalLat.at(0);
		m_pTempElement->SelectedLat().m_end = vFinalLat.at(vFinalLat.count()-1);
		m_pTempElement->SelectedLat().m_count = vFinalLat.count();
	}
	if (nz>1)
	{
		QList<float> l;
		for (int i = 0;i < vFinalLevel.count();i++)
		{
			l.push_back(vFinalLevel.at(i));
		}
		m_pTempElement->ResetLevel(l);
	}
	if (nt>1)
	{
		m_pTempElement->SelectedPeriod().m_begin = vFinalTime.at(0);
		m_pTempElement->SelectedPeriod().m_end = vFinalTime.at(vFinalTime.count()-1);
		m_pTempElement->SelectedPeriod().m_count = vFinalTime.count();
	}

	m_nLonNCCount	= m_pTempElement->SelectedLon().m_count		>0 ? m_pTempElement->SelectedLon().m_count	:1;
	m_nLatNCCount	= m_pTempElement->SelectedLat().m_count		>0 ? m_pTempElement->SelectedLat().m_count	:1;
	m_nTimeNCCount	= m_pTempElement->SelectedPeriod().m_count	>0 ? m_pTempElement->SelectedPeriod().m_count	:1;
	m_nLevelNCCount	= m_pTempElement->Level().m_selected.size()	>0 ? m_pTempElement->Level().m_selected.size():1;

	m_fLonNCBegin	= m_pTempElement->SelectedLon().m_begin	<= m_pTempElement->SelectedLon().m_end? m_pTempElement->SelectedLon().m_begin	: m_pTempElement->SelectedLon().m_end;
	m_fLonNCDelta	= m_pTempElement->SelectedLon().m_delt	>= 0										? m_pTempElement->SelectedLon().m_delt	: -1 * m_pTempElement->SelectedLon().m_delt;
	m_fLatNCBegin	= m_pTempElement->SelectedLat().m_begin	<= m_pTempElement->SelectedLat().m_end? m_pTempElement->SelectedLat().m_begin	: m_pTempElement->SelectedLat().m_end;
	m_fLatNCDelta	= m_pTempElement->SelectedLat().m_delt	>= 0										? m_pTempElement->SelectedLat().m_delt	: -1 * m_pTempElement->SelectedLat().m_delt;

	m_fMissingV = 32767.0f;

	m_pTempElement->SelectedLon().m_count = m_nLonNCCount;
	m_pTempElement->SelectedLat().m_count = m_nLatNCCount;
	m_pTempElement->SelectedPeriod().m_count = m_nTimeNCCount;
	m_pTempElement->Level().m_count = m_nLevelNCCount;

	m_pTempElement->SelectedLon().m_begin = m_fLonNCBegin;
	m_pTempElement->SelectedLon().m_delt = m_fLonNCDelta;
	m_pTempElement->SelectedLat().m_begin = m_fLatNCBegin;
	m_pTempElement->SelectedLat().m_delt = m_fLatNCDelta;

	m_pTempElement->MissingValue(m_fMissingV);

	//	/////////////////////////////////yangdong/////////////////////////////////////////
	//
	//	QVector<QDateTime> vDTbegin;
	//	QVector<QDateTime> vDTend;
	//	QVector<float> vLever;
	//	QVector<float> vXb;
	//	QVector<float> vXe;
	//	QVector<float> vX;
	//	QVector<float> vYb;
	//	QVector<float> vYe;
	//	QVector<float> vY;
	//	QHash<int, float> hashLever;
	//	const QDateTime *dtb;
	//	const QDateTime *dte;
	//	float xb;
	//	float xe;
	//	float yb;
	//	float ye;
	//	int yearBegin;
	//	int monthBegin;
	//	int yearEnd;
	//	int monthEnd;
	//
	//	int nElementIndex;
	//	// 		float lb;
	//	// 		float le;
	//
	//	for (nElementIndex = 0;nElementIndex < pListCElement->count();nElementIndex++)
	//	{
	//		nx	= pListCElement->at(nElementIndex)->SelectedLon().m_count		>0 ? pListCElement->at(nElementIndex)->SelectedLon().m_count:1;
	//		ny	= pListCElement->at(nElementIndex)->SelectedLat().m_count		>0 ? pListCElement->at(nElementIndex)->SelectedLat().m_count:1;
	//		nz	= pListCElement->at(nElementIndex)->Level().m_selected.size()	>0 ? pListCElement->at(nElementIndex)->Level().m_selected.size():1;
	//		nt	= pListCElement->at(nElementIndex)->SelectedPeriod().m_count	>0 ? pListCElement->at(nElementIndex)->SelectedPeriod().m_count:1;
	//		if (nt > 1)//选择的是时间
	//		{
	//			vDTbegin.push_back(pListCElement->at(nElementIndex)->SelectedPeriod().m_begin);
	//			vDTend.push_back(pListCElement->at(nElementIndex)->SelectedPeriod().m_end);
	//		}
	//		/*else */if (nz > 1)//选择的是高度
	//		{
	//			QHash<int, float> h = pListCElement->at(nElementIndex)->Level().m_selected;
	//			QHash<int, float>::iterator it;
	//			for (it=h.begin(); it!=h.end(); it++)
	//			{
	//				hashLever.insert(it.key(), it.value());
	//			}
	//		}
	//		/*else */if (nx > 1) //选择的是经度
	//		{
	//			vXb.push_back(pListCElement->at(nElementIndex)->SelectedLon().m_begin);
	//			vXe.push_back(pListCElement->at(nElementIndex)->SelectedLon().m_end);
	//		}
	//		/*else */if (ny > 1)//纬度
	//		{
	//			vYb.push_back(pListCElement->at(nElementIndex)->SelectedLat().m_begin);
	//			vYe.push_back(pListCElement->at(nElementIndex)->SelectedLat().m_end);
	//		}
	//	}
	//	if (ny>1)
	//	{
	//		qSort(vYb.begin(), vYb.end());
	//		qSort(vYe.begin(), vYe.end());
	//		yb = vYb.at(vYe.count()-1);
	//		ye = *vYe.begin();
	//		for (float y=yb; y>=ye; y-=abs(pListCElement->at(0)->ActualLat().m_delt))
	//		{
	//			vY.push_back(y);
	//		}
	//		ny = abs(yb-ye) / abs(pListCElement->at(0)->ActualLat().m_delt) + 1;
	//	}
	//	if (nx>1)
	//	{
	//		qSort(vXb.begin(), vXb.end());
	//		qSort(vXe.begin(), vXe.end());
	//		xb = *vXb.begin();
	//		xe = vXe.at(vXe.count()-1);
	//		for (float x=xb; x<=xe; x+=abs(pListCElement->at(0)->ActualLon().m_delt))
	//		{
	//			vX.push_back(x);
	//		}
	//		nx = abs(xb-xe) / abs(pListCElement->at(0)->ActualLon().m_delt) + 1;
	//		// 			//pListCElement->at(0)->ActualLon().m_delt;
	//		//
	//		// 			//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,"错误","经度尺度不一致");
	//		// 			//CDiskFileLog::WriteLog("警告", QString::fromLocal8Bit("GradsProvider::NetCDF2Array 经度尺度不一致！") );//写日志--renxue
	//		// 			return 1;
	//	}
	//	if (nz>1)
	//	{
	//		QHash<int, float>::iterator it;
	//		for (it=hashLever.begin(); it!=hashLever.end(); it++)
	//		{
	//			vLever.push_back(it.value());
	//		}
	//		qSort(vLever.begin(), vLever.end(), qGreater<float>());
	//		// 			lb = vLever.at(0);
	//		// 			le = vLever.at(vLever.count()-1);
	//		nz = hashLever.count();
	//
	//		// 			//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,"错误","高度尺度不一致!");
	//		// 			//CDiskFileLog::WriteLog("警告", QString::fromLocal8Bit("GradsProvider::NetCDF2Array 高度尺度不一致！") );//写日志--renxue
	//		// 			return 1;
	//	}
	//	if (nt>1)
	//	{
	//		qSort(vDTbegin.begin(), vDTbegin.end());
	//		qSort(vDTend.begin(), vDTend.end());
	//
	//		dtb = vDTbegin.begin();
	//		dte = &vDTend.at(vDTend.count()-1);//vector中最后一个值
	//
	//		//尺度是月--计算nt
	//		yearBegin = dtb->toString(QString("yyyy")).toInt();
	//		monthBegin = dtb->toString(QString("MM")).toInt();;
	//
	//		yearEnd = dte->toString(QString("yyyy")).toInt();
	//		monthEnd = dte->toString(QString("MM")).toInt();
	//
	//		nt = (yearEnd-yearBegin)*12+(monthEnd-monthBegin)+1;
	//	}
	//
	//	if (!(pListCElement->at(0)->GetCInterval().isEmpty()))
	//	{
	//		m_gplistInterval = pListCElement->at(0)->GetCInterval();
	//	}
	//
	//	//***************exception validation ******************//
	//	if(m_fLonNCDelta <= 0)
	//	{
	//		//CCIPASMessageBox::CIPASWaring(NULL, //CCIPASMessageBox::CIPAS_OK,"ERROR",tr("f_lonstep <= 0: 经度间隔小于0!"));
	//		//CDiskFileLog::WriteLog("警告", tr("GradsProvider::NetCDF2Array 经度间隔小于0！").toLocal8Bit());//写日志--renxue
	//		return false;
	//	}
	//	if(m_fLatNCDelta <= 0)
	//	{
	//		//CCIPASMessageBox::CIPASWaring(NULL, //CCIPASMessageBox::CIPAS_OK,"ERROR",tr("f_latstep <= 0: 纬度间隔小于0!"));
	//		//CDiskFileLog::WriteLog("警告", tr("GradsProvider::NetCDF2Array 纬度间隔小于0！").toLocal8Bit());//写日志--renxue
	//		return false;
	//	}
	//
	//	//*******************************  Write *.dat    the data file******************************************
	//	//m_dArrayValue = (double *)malloc(sizeof(double) * nMultiEleArrSize);
	//
	//	m_dArrayValue = (double *)malloc(sizeof(double) * nt*nz*nx*ny*pListCElement->count());
	//	////qDebug()<<"nMultiEleArrSize="<<nMultiEleArrSize;
	//
	//	float *pData = NULL;
	//	int n,i,j,k,l;
	//	int count = 0;
	//	n = 0;
	//	QHash<int, int> ele_ne;
	//	int hh = pListCElement->count();
	//	int *nIndex = new int[hh];
	//	for (nElementIndex = 0;nElementIndex < pListCElement->count();nElementIndex++)
	//	{
	//		nIndex[nElementIndex]=0;//pdata对应的索引
	//	}
	//
	//	FILE *fp = NULL;
	//	char* strtestDAT = "test.dat";
	//	fopen_s(&fp, strtestDAT, "wb");
	//
	//	for(int ne = 0; ne < 1; ne++)
	//	{
	//		for(i = 0; i < nt; i++)
	//		{
	//			for (nElementIndex = 0;nElementIndex < pListCElement->count();nElementIndex++)
	//			{
	//				ele_ne.insert(nElementIndex, nIndex[nElementIndex]);
	//
	//				for(j = 0; j < nz; j++)          // Output pElem1 to m_dArrayValue
	//				{
	//					if(NorthToSouth)
	//					{
	//						for(k = ny - 1; k >=0; k--)   // 由北到南：南北顺序相反，要颠倒过来
	//						{
	//							for(l= 0; l < nx; l++)
	//							{
	//								CElement *ele = pListCElement->at(nElementIndex);
	//
	//								if(ele!=NULL && nt>1)
	//								{
	//									int yearb = ele->SelectedPeriod().m_begin.toString(QString("yyyy")).toInt();
	//									int monthb = ele->SelectedPeriod().m_begin.toString(QString("MM")).toInt();
	//									int selBeginTime = (yearb - yearBegin) * 12 + monthb - monthBegin;  //起始时间点
	//
	//									int yeare = ele->SelectedPeriod().m_end.toString(QString("yyyy")).toInt();
	//									int monthe = ele->SelectedPeriod().m_end.toString(QString("MM")).toInt();
	//									int selEndTime = (yeare - yearBegin)*12 + monthe-monthBegin;//结束时间点
	//
	//									if ((selBeginTime <= i) && (selEndTime>=i))
	//									{
	//										pData = ele->Data().GetDataArray(0);
	//
	//										if (selBeginTime == 0)
	//										{
	//											if (pData[i * nz * ny * nx + j * ny * nx + k * nx + l] != pListCElement->at(nElementIndex)->MissingValue())
	//											{
	//												m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l];
	//											}
	//											else
	//											{
	//												m_dArrayValue[n] = m_fMissingV;
	//											}
	//										}
	//										else
	//										{
	//											if (pData[(i-selBeginTime) * nz * ny * nx + j * ny * nx + k * nx + l] != pListCElement->at(nElementIndex)->MissingValue())
	//											{
	//												m_dArrayValue[n] = pData[(i-selBeginTime) * nz * ny * nx + j * ny * nx + k * nx + l];
	//											}
	//											else
	//											{
	//												m_dArrayValue[n] = m_fMissingV;
	//											}
	//										}
	//									}
	//									else
	//									{
	//										m_dArrayValue[n] = m_fMissingV;
	//									}
	//									////qDebug()<<m_dArrayValue[n];
	//									count++;
	//									++n;
	//								}
	//								else if (ele!=NULL && nz>1)//高度
	//								{
	//									QHash<int, float> hh = ele->Level().m_selected;
	//									int tt = hh.count();
	//									QVector<float> v;
	//									QHash<int, float>::iterator it;
	//									for (it=hh.begin(); it!=hh.end(); it++)
	//									{
	//										v.push_back(it.value());
	//									}
	//									qSort(v.begin(), v.end());
	//									float this_ele_b = v.at(0);
	//									float this_ele_e = v.at(v.count()-1);
	//
	//									if (vLever.indexOf(this_ele_b)<=j && vLever.indexOf(this_ele_e)>=j)//正常赋值
	//									{
	//										pData = ele->Data().GetDataArray(0);
	//										QHash<int, int>::iterator it;
	//										for (it=ele_ne.begin(); it!=ele_ne.end(); it++)
	//										{
	//											if (nElementIndex == it.key())
	//											{
	//												if (pData[i * nz * ny * nx + j * ny * nx + k * nx + l-it.value()] != pListCElement->at(nElementIndex)->MissingValue())
	//												{
	//													m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l-it.value()];
	//												}
	//												else
	//												{
	//													m_dArrayValue[n] = m_fMissingV;
	//												}
	//											}
	//										}
	//									}
	//									else
	//									{
	//										m_dArrayValue[n] = m_fMissingV;
	//										ele_ne.insert(nElementIndex, ++nIndex[nElementIndex]);
	//									}
	//									count++;
	//									++n;
	//								}
	//								else if(ele!=NULL && nx>1)
	//								{
	//									QVector<float> vl;
	//									for (float x=ele->SelectedLon().m_begin; x<=ele->SelectedLon().m_end; x+=ele->SelectedLon().m_delt)
	//									{
	//										vl.push_back(x);
	//									}
	//									qSort(vl.begin(), vl.end());
	//									float currentValueb = vl.at(0);
	//									float currentValuee = vl.at(vl.count()-1);
	//									if (vX.indexOf(currentValueb)<=l && vX.indexOf(currentValuee)>=l)
	//									{
	//										pData = ele->Data().GetDataArray(0);
	//
	//										if (vX.indexOf(currentValueb)==0)
	//										{
	//											if (pData[i * nz * ny * nx + j * ny * nx + k * nx + l] != pListCElement->at(nElementIndex)->MissingValue())
	//											{
	//												m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l];
	//											}
	//											else
	//											{
	//												m_dArrayValue[n] = m_fMissingV;
	//											}
	//										}
	//										else
	//										{
	//											if (pData[i * nz * ny * nx + j * ny * nx + k * nx + l-vX.indexOf(currentValueb)] != pListCElement->at(nElementIndex)->MissingValue())
	//											{
	//												m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l-vX.indexOf(currentValueb)];
	//											}
	//											else
	//											{
	//												m_dArrayValue[n] = m_fMissingV;
	//											}
	//										}
	//									}
	//									else
	//									{
	//										m_dArrayValue[n] = m_fMissingV;
	//									}
	//									////qDebug()<<m_dArrayValue[n];
	//									count++;
	//									++n;
	//								}
	//								else if (ele!=NULL && ny>1)
	//								{
	//									QVector<float> vl;
	//									for (float y=ele->SelectedLat().m_end; y<=ele->SelectedLat().m_begin; y+=abs(ele->SelectedLat().m_delt))
	//									{
	//										vl.push_back(y);
	//									}
	//									qSort(vl.begin(), vl.end());
	//									float currentValueb = vl.at(vl.count()-1);//0,90
	//									float currentValuee = vl.at(0);//100,-90
	//
	//									if (vY.indexOf(currentValueb)<=k && vY.indexOf(currentValuee)>=k)
	//									{
	//										pData = ele->Data().GetDataArray(0);
	//
	//										if (vY.indexOf(currentValueb) == 0 && vY.indexOf(currentValuee) < (ny-1))//把小头
	//										{
	//											if (pData[i * nz * ny * nx + j * ny * nx + k * nx + l] != pListCElement->at(nElementIndex)->MissingValue())
	//											{
	//												m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l];
	//											}
	//											else
	//											{
	//												m_dArrayValue[n] = m_fMissingV;
	//											}
	//										}
	//										else//把中间&大头
	//										{
	//											if (pData[i * nz * ny * nx + j * ny * nx + (k-vY.indexOf(currentValueb)) * nx + l] != pListCElement->at(nElementIndex)->MissingValue())
	//											{
	//												m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + (k-vY.indexOf(currentValueb)) * nx + l];
	//											}
	//											else
	//											{
	//												m_dArrayValue[n] = m_fMissingV;
	//											}
	//										}
	//									}
	//									else
	//									{
	//										m_dArrayValue[n] = m_fMissingV;
	//									}
	//									//float fValue = m_dArrayValue[n];
	//									//fwrite(&fValue, sizeof(float), 1, fp);
	//									////qDebug()<<m_dArrayValue[n];
	//									count++;
	//									++n;
	//								}
	//							}// endfor nx
	//						}// endfor ny
	//					}
	//					else
	//					{
	//						for(k = 0; k < ny; k++)   //由南到北
	//						{
	//							for(l= 0; l < nx; l++)
	//							{
	//								CElement *ele = pListCElement->at(nElementIndex);
	//
	//								if(ele!=NULL && nt>1)
	//								{
	//									int yearb = ele->SelectedPeriod().m_begin.toString(QString("yyyy")).toInt();
	//									int monthb = ele->SelectedPeriod().m_begin.toString(QString("MM")).toInt();
	//									int selBeginTime = (yearb - yearBegin) * 12 + monthb - monthBegin;  //起始时间点
	//
	//									int yeare = ele->SelectedPeriod().m_end.toString(QString("yyyy")).toInt();
	//									int monthe = ele->SelectedPeriod().m_end.toString(QString("MM")).toInt();
	//									int selEndTime = (yeare - yearBegin)*12 + monthe-monthBegin;//结束时间点
	//
	//									if ((selBeginTime <= i) && (selEndTime>=i))
	//									{
	//										pData = ele->Data().GetDataArray(0);
	//
	//										if (selBeginTime == 0)
	//										{
	//											if (pData[i * nz * ny * nx + j * ny * nx + k * nx + l] != pListCElement->at(nElementIndex)->MissingValue())
	//											{
	//												m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l];
	//											}
	//											else
	//											{
	//												m_dArrayValue[n] = m_fMissingV;
	//											}
	//										}
	//										else
	//										{
	//											if (pData[(i-selBeginTime) * nz * ny * nx + j * ny * nx + k * nx + l] != pListCElement->at(nElementIndex)->MissingValue())
	//											{
	//												m_dArrayValue[n] = pData[(i-selBeginTime) * nz * ny * nx + j * ny * nx + k * nx + l];
	//											}
	//											else
	//											{
	//												m_dArrayValue[n] = m_fMissingV;
	//											}
	//										}
	//									}
	//									else
	//									{
	//										m_dArrayValue[n] = m_fMissingV;
	//									}
	//									////qDebug()<<m_dArrayValue[n];
	//									count++;
	//									++n;
	//								}
	//								else if (ele!=NULL && nz>1)//高度
	//								{
	//									QHash<int, float> hh = ele->Level().m_selected;
	//									int tt = hh.count();
	//									QVector<float> v;
	//									QHash<int, float>::iterator it;
	//									for (it=hh.begin(); it!=hh.end(); it++)
	//									{
	//										v.push_back(it.value());
	//									}
	//									qSort(v.begin(), v.end());
	//									float this_ele_b = v.at(v.count()-1);
	//									float this_ele_e = v.at(0);
	//
	//									if (vLever.indexOf(this_ele_b)<=j && vLever.indexOf(this_ele_e)>=j)//正常赋值
	//									{
	//										pData = ele->Data().GetDataArray(0);
	//										QHash<int, int>::iterator it;
	//										for (it=ele_ne.begin(); it!=ele_ne.end(); it++)
	//										{
	//											if (nElementIndex == it.key())
	//											{
	//												if (pData[i * nz * ny * nx + (j-it.value()) * ny * nx + k * nx + l] != pListCElement->at(nElementIndex)->MissingValue())
	//												{
	//													m_dArrayValue[n] = pData[i * nz * ny * nx + (j-it.value()) * ny * nx + k * nx + l];
	//												}
	//												else
	//												{
	//													m_dArrayValue[n] = m_fMissingV;
	//												}
	//											}
	//										}
	//									}
	//									else
	//									{
	//										m_dArrayValue[n] = m_fMissingV;
	//										ele_ne.insert(nElementIndex, ++nIndex[nElementIndex]);
	//									}
	//									////qDebug()<<m_dArrayValue[n];
	//									//float fValue = m_dArrayValue[n];
	//									//fwrite(&fValue, sizeof(float), 1, fp);
	//									++n;
	//								}
	//								else if(ele!=NULL && nx>1)
	//								{
	//									QVector<float> vl;
	//									for (float x=ele->SelectedLon().m_begin; x<=ele->SelectedLon().m_end; x+=ele->SelectedLon().m_delt)
	//									{
	//										vl.push_back(x);
	//									}
	//									qSort(vl.begin(), vl.end());
	//									float currentValueb = vl.at(0);
	//									float currentValuee = vl.at(vl.count()-1);
	//									if (vX.indexOf(currentValueb)<=l && vX.indexOf(currentValuee)>=l)
	//									{
	//										pData = ele->Data().GetDataArray(0);
	//
	//										if (vX.indexOf(currentValueb)==0)
	//										{
	//											if (pData[i * nz * ny * nx + j * ny * nx + k * nx + l] != pListCElement->at(nElementIndex)->MissingValue())
	//											{
	//												m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l];
	//											}
	//											else
	//											{
	//												m_dArrayValue[n] = m_fMissingV;
	//											}
	//										}
	//										else
	//										{
	//											if (pData[i * nz * ny * nx + j * ny * nx + k * nx + l-vX.indexOf(currentValueb)] != pListCElement->at(nElementIndex)->MissingValue())
	//											{
	//												m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l-vX.indexOf(currentValueb)];
	//											}
	//											else
	//											{
	//												m_dArrayValue[n] = m_fMissingV;
	//											}
	//										}
	//									}
	//									else
	//									{
	//										m_dArrayValue[n] = m_fMissingV;
	//									}
	//									////qDebug()<<m_dArrayValue[n];
	//									count++;
	//									++n;
	//								}
	//								else if (ele!=NULL && ny>1)
	//								{
	//									QVector<float> vl;
	//									for (float y=ele->SelectedLat().m_end; y<=ele->SelectedLat().m_begin; y+=abs(ele->SelectedLat().m_delt))
	//									{
	//										vl.push_back(y);
	//									}
	//									qSort(vl.begin(), vl.end());
	//									float currentValueb = vl.at(vl.count()-1);
	//									float currentValuee = vl.at(0);
	//									if (vY.indexOf(currentValueb)<=k && vY.indexOf(currentValuee)>=k)
	//									{
	//										pData = ele->Data().GetDataArray(0);
	//										QHash<int, int>::iterator it;
	//										for (it=ele_ne.begin(); it!=ele_ne.end(); it++)
	//										{
	//											if (nElementIndex == it.key())
	//											{
	//												if (pData[i * nz * ny * nx + j * ny * nx + (k-it.value()) * nx + l] != pListCElement->at(nElementIndex)->MissingValue())
	//												{
	//													m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + (k-it.value()) * nx + l];
	//												}
	//												else
	//												{
	//													m_dArrayValue[n] = m_fMissingV;
	//												}
	//											}
	//										}
	//									}
	//									else
	//									{
	//										m_dArrayValue[n] = m_fMissingV;
	//										ele_ne.insert(nElementIndex, ++nIndex[nElementIndex]);
	//									}
	//									//float fValue = m_dArrayValue[n];
	//									//fwrite(&fValue, sizeof(float), 1, fp);
	//									////qDebug()<<m_dArrayValue[n];
	//									count++;
	//									++n;
	//								}
	//							}// endfor nx
	//						}// endfor ny
	//					}// else NorthToSouth
	//				} // endfor nz
	//			}// endfor ListElement
	//		} // endfor nt
	//	} // endfor ne
	//	//fclose(fp);
	//
	//	////qDebug()<<"count="<<count;
	//
	//	if (m_pTempElement)
	//	{
	//		delete m_pTempElement;
	//		m_pTempElement = NULL;
	//	}
	//	m_pTempElement = new CElement(*pListCElement->at(0));
	//
	//	if (nt>1)
	//	{
	//		m_pTempElement->SelectedPeriod().m_begin = *dtb;
	//		m_pTempElement->SelectedPeriod().m_end = *dte;
	//		m_pTempElement->SelectedPeriod().m_count = nt;
	//	}
	//	if (nz>1)
	//	{
	//		QList<float> l;
	//		QHash<int, float>::iterator it;
	//		for (it=hashLever.begin(); it!=hashLever.end(); it++)
	//		{
	//			l.push_back(it.value());
	//		}
	//		m_pTempElement->ResetLevel(l);
	//	}
	//	if (nx>1)
	//	{
	//		m_pTempElement->SelectedLon().m_begin = xb;
	//		m_pTempElement->SelectedLon().m_end = xe;
	//		m_pTempElement->SelectedLon().m_count = vX.size();
	//	}
	//	if (ny>1)
	//	{
	//		m_pTempElement->SelectedLat().m_begin = yb;
	//		m_pTempElement->SelectedLat().m_end = ye;
	//		m_pTempElement->SelectedLat().m_count = vY.size();
	//	}
	//
	//	m_nLonNCCount	= m_pTempElement->SelectedLon().m_count		>0 ? m_pTempElement->SelectedLon().m_count	:1;
	//	m_nLatNCCount	= m_pTempElement->SelectedLat().m_count		>0 ? m_pTempElement->SelectedLat().m_count	:1;
	//	m_nTimeNCCount	= m_pTempElement->SelectedPeriod().m_count	>0 ? m_pTempElement->SelectedPeriod().m_count	:1;
	//	m_nLevelNCCount	= m_pTempElement->Level().m_selected.size()	>0 ? m_pTempElement->Level().m_selected.size():1;
	//
	//	m_fLonNCBegin	= m_pTempElement->SelectedLon().m_begin	<= m_pTempElement->SelectedLon().m_end? m_pTempElement->SelectedLon().m_begin	: m_pTempElement->SelectedLon().m_end;
	//	m_fLonNCDelta	= m_pTempElement->SelectedLon().m_delt	>= 0										? m_pTempElement->SelectedLon().m_delt	: -1 * m_pTempElement->SelectedLon().m_delt;
	//	m_fLatNCBegin	= m_pTempElement->SelectedLat().m_begin	<= m_pTempElement->SelectedLat().m_end? m_pTempElement->SelectedLat().m_begin	: m_pTempElement->SelectedLat().m_end;
	//	m_fLatNCDelta	= m_pTempElement->SelectedLat().m_delt	>= 0										? m_pTempElement->SelectedLat().m_delt	: -1 * m_pTempElement->SelectedLat().m_delt;
	//
	//	m_fMissingV = m_pTempElement->MissingValue();

	return true;
}

void GradsProvider::ClearPolyLineBar()
{
	if (m_pGPloyLineBar != NULL)
	{
		m_pGPloyLineBar->Clear();
	}
}

#endif

#if 0
bool GradsProvider::Array4NormalElement( QList<CElement*> *pListCElement,int nx,int ny,int nz,int nt,float f_missingdata,bool NorthToSouth )
{
	m_nElementNum = pListCElement->count();
	//f_missingdata = 32766.0f;
	long nSingleEleArrSize = m_nLonNCCount * m_nLatNCCount * m_nTimeNCCount * m_nLevelNCCount;
	long nMultiEleArrSize = m_nLonNCCount * m_nLatNCCount * m_nTimeNCCount * m_nLevelNCCount * m_nElementNum;
	float f_Accuracy = 0.0001;
	int nElementIndex;
	float *pData = NULL;

	if (pListCElement->count() > 1)
	{
		for (nElementIndex = 1;nElementIndex < pListCElement->count();nElementIndex++)
		{
			int nextEleLon	= pListCElement->at(nElementIndex)->SelectedLon().m_count		>0 ? pListCElement->at(nElementIndex)->SelectedLon().m_count	:1;
			int nextEleLat	= pListCElement->at(nElementIndex)->SelectedLat().m_count		>0 ? pListCElement->at(nElementIndex)->SelectedLat().m_count	:1;
			int nextEleTim	= pListCElement->at(nElementIndex)->SelectedPeriod().m_count	>0 ? pListCElement->at(nElementIndex)->SelectedPeriod().m_count	:1;
			int nextEleLev	= pListCElement->at(nElementIndex)->Level().m_selected.size()	>0 ? pListCElement->at(nElementIndex)->Level().m_selected.size():1;

			if (!(qAbs(nextEleLon*nextEleLat*nextEleTim*nextEleLev - nSingleEleArrSize) < f_Accuracy))
			{
				return false;
			}
		}
	}

	m_dArrayValue = (double *)malloc(sizeof(double) * nMultiEleArrSize);

	//******************************** Write data ********************************
	int i,j,k,l,n = 0;
	for(int ne = 0; ne < 1; ne++)
	{
		for(i = 0; i < nt; i++)
		{
			for (nElementIndex = 0;nElementIndex < pListCElement->count();nElementIndex++)
			{
				pData = pListCElement->at(nElementIndex)->Data().GetDataArray(0);
				//pData = pListCElement->at(nElementIndex)->Data().GetDataArray(0);
				for(j = 0; j < nz; j++)          // Output pElem1 to m_dArrayValue
				{
					if(NorthToSouth)
					{
						for(k = ny - 1; k >=0; k--)   // 由北到南：南北顺序相反，要颠倒过来
						{
							for(l= 0; l < nx; l++)
							{
								//normal display!!!!
								
								m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l];
								++n;
							}// endfor nx
						}// endfor ny
					}
					else
					{
						for(k = 0; k < ny; k++)   //由南到北
						{
							for(l= 0; l < nx; l++)
							{
								//normal display!!!!
								
								m_dArrayValue[n] = pData[i * nz * ny * nx + j * ny * nx + k * nx + l];
								++n;
							}// endfor nx
						}// endfor ny
					}// else
				} // endfor nz
			} // endfor element
		} // endfor nt
	} // endfor ne

	////qDebug()<<"n="<<n;
	//for (n = 0;n < nMultiEleArrSize;n++)
	//{
	//	//qDebug()<<"m_dArrayValue"<<n<<m_dArrayValue[n];
	//}

	m_nLonNCCount	= pListCElement->at(0)->SelectedLon().m_count		>0 ? pListCElement->at(0)->SelectedLon().m_count	:1;
	m_nLatNCCount	= pListCElement->at(0)->SelectedLat().m_count		>0 ? pListCElement->at(0)->SelectedLat().m_count	:1;
	m_nTimeNCCount	= pListCElement->at(0)->SelectedPeriod().m_count	>0 ? pListCElement->at(0)->SelectedPeriod().m_count	:1;
	m_nLevelNCCount	= pListCElement->at(0)->Level().m_selected.size()	>0 ? pListCElement->at(0)->Level().m_selected.size():1;

	m_fLonNCBegin	= pListCElement->at(0)->SelectedLon().m_begin	<= pListCElement->at(0)->SelectedLon().m_end? pListCElement->at(0)->SelectedLon().m_begin	: pListCElement->at(0)->SelectedLon().m_end;
	m_fLonNCDelta	= pListCElement->at(0)->SelectedLon().m_delt	>= 0										? pListCElement->at(0)->SelectedLon().m_delt	: -1 * pListCElement->at(0)->SelectedLon().m_delt;
	m_fLatNCBegin	= pListCElement->at(0)->SelectedLat().m_begin	<= pListCElement->at(0)->SelectedLat().m_end? pListCElement->at(0)->SelectedLat().m_begin	: pListCElement->at(0)->SelectedLat().m_end;
	m_fLatNCDelta	= pListCElement->at(0)->SelectedLat().m_delt	>= 0										? pListCElement->at(0)->SelectedLat().m_delt	: -1 * pListCElement->at(0)->SelectedLat().m_delt;

	m_fMissingV = pListCElement->at(0)->MissingValue();

	if (m_pTempElement)
	{
		delete m_pTempElement;
		m_pTempElement = NULL;
	}

	m_pTempElement =  new CElement(*pListCElement->at(0));

	m_pTempElement->SelectedLon().m_count = m_nLonNCCount;
	m_pTempElement->SelectedLat().m_count = m_nLatNCCount;
	m_pTempElement->SelectedPeriod().m_count = m_nTimeNCCount;
	m_pTempElement->Level().m_count = m_nLevelNCCount;

	m_pTempElement->SelectedLon().m_begin = m_fLonNCBegin;
	m_pTempElement->SelectedLon().m_delt = m_fLonNCDelta;
	m_pTempElement->SelectedLat().m_begin = m_fLatNCBegin;
	m_pTempElement->SelectedLat().m_delt = m_fLatNCDelta;

	m_pTempElement->MissingValue(m_fMissingV);

	return true;
}
#endif

bool GradsProvider::LoadFile( const std::string& filename )
{
	return false;
}

int GradsProvider::MakeProduct( MeteorologyProducts* pMP, int iDirect, const std::string& strCmd )
{
	ClearVector();
	//Initialize();

	if (pMP != NULL)
	{
		gcmn.isContour = true;
		pMP->Draw(strCmd);
		return pMP->Dump(m_pParentLayer, iDirect);
	}
	else
		return 1;
}

int GradsProvider::SetCtlFile( const std::string& ctlFile, const std::string& dataFile )
{
	memset(m_strCTL,0,40000);
	std::ifstream fin(ctlFile, std::ios::in);

	int i = 0;
	char* firstChar  = m_strCTL[0];
	while(fin.getline(firstChar + i * 200, 200))
	{
		i++;
	}
	
	
	std::ifstream fin_binary;
	fin_binary.open(dataFile, std::ios::in|std::ios::binary);

	fin_binary.seekg(0, std::ios::end);
	streamoff pt = fin_binary.tellg();
	ULONGLONG floatSize = pt/sizeof(float);

	double* dData = new double[floatSize];

	fin_binary.seekg(0, std::ios::beg);
	
	char* tempBuffer = new char[sizeof(float)];
	i = 0;
	while(!fin_binary.eof())
	{
		fin_binary.read(tempBuffer, sizeof(float));
		float fTemp = *(float*)tempBuffer;

		*(dData + i) = fTemp;

		i++;
	}

	
	//*******************************  open the grads binary file output by above code **********************
	gagx(&gcmn);

	gcmn.sig = 0;
	char *pstrCTL[200] = {0};

	for(int i = 0; i < 200; i++)
		pstrCTL[i] = m_strCTL[i];

	if (ccsopen(pstrCTL, &gcmn)== 1)
	{
		////CDiskFileLog::WriteLog(tr("警告"), tr("GradsProvider::NetCDF2Array 文件载入错误！").toLocal8Bit());//写日志--renxue
		////CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),tr("NETCDF2Array：文件载入错误!"));
		return 1;
	}

	gcmn.pfi1->pArrayValuebuf = dData;
	return 0;
}

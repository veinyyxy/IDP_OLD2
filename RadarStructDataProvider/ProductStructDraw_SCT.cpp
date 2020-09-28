#include <math.h>
#include <QtCore/QCoreApplication>
#include "QtXml/QXmlStreamReader"
#include <QtCore/QFile>
#include <QDir>
#include <QDate>

#include "Struct_278_SCT.h"
#include "RadProjTransform.h"
#include "goQuadrilateral.h"
#include "goPolygon.h"

#include "ProductStructDraw_SCT.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class CProductStructDraw_SCT 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//add by ljg 20080825 for functions recharge.
CProductStructDraw_SCT::CProductStructDraw_SCT()
{
	m_fR_km=460.0;
	m_fKmPerPixel=1.0;
	//m_nSizeOfStormTarg=5;
	m_nSizeOfStormTargHistory=3;
	m_nSizeOfStormTargCurrent=5;
	m_nSizeOfStormTargForeCast=5;
	//m_nSizeOfStormTargTrackLine=1;
	m_fLenghUnit=1.0;//1km
	//BGR...
	m_nColorHistory.set(1.0,1.0,1.0,1.0);//鍏?
	m_nColorCurrent.set(1.0,1.0,1.0,1.0);//绾?
	m_nColorForeCast.set(1.0,1.0,1.0,1.0);//缁?
	m_nColorTrackLine.set(1.0,1.0,1.0,1.0);//鐧藉叞
};

CProductStructDraw_SCT::~CProductStructDraw_SCT()
{
};

STRUCT_DATA_TYPE CProductStructDraw_SCT::GetTypeOfProduct()
{
	STRUCT_DATA_TYPE n = STRUCT_SCT;
	return n;
};

bool CProductStructDraw_SCT::DumpAll(Layer* layer ,CProductRead* lpProduct, /* CProductDrawParam* lpParam,*/ P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer/* = 0*/)
{
	bool bRet=false;
	try
	{
		Draw(lpProduct,layer);
		bRet=true;
	}
	catch(...)
	{
		bRet=false;
	}
	return bRet;
};


int CProductStructDraw_SCT::GetCountOfStormCells(ProductBase* pProduct ,int nRequestLayer/* = -1*/)
{
	int nRet=0;
	PDBlock* blok;
	CProductRead* pThisProduct=(CProductRead*) pProduct;
	if(pThisProduct!=NULL)
	{
		blok = pThisProduct->GetHeadOfProduct();
		if(blok!=NULL)
		{
			if(blok->ProductCode==278)
			{
				int n=blok->NumberOfLayers;
				int i=nRequestLayer;
				if( i == -1 )
				{
					i = n-1;
				}
				if( i < n )
				{
					STORM_CTHEAD* tHdata = (STORM_CTHEAD*)pThisProduct->GetDataOfBlockStruct(i,0);//j=0
					nRet=tHdata->NumberOfStormCells;
				}
			}
		}
	}
	return nRet;
};

void CProductStructDraw_SCT::DrawLayer(const void* pBlockHead,const void* pBlockData,/*gInt16 nScale,*/int nCurLayer,int nCountOfLayer ,Layer* layer)
{
	bool bRet=false;
	//wxBitmap bitmp;
	//wxMemoryDC memDC;

	////int nDPixel=m_fR_km/m_fKmPerPixel*2+1+4*m_nSizeOfStormTargCurrent;//鐩村緞+1+4*m_nSizeOfStormTarg
	//int nDPixel=m_fR_km/m_fKmPerPixel*2/*+1+4*m_nSizeOfStormTargCurrent*/;//鐩村緞+1+4*m_nSizeOfStormTarg

	//bitmp.Create( nDPixel,nDPixel,-1);
	//bmp.Create( nDPixel,nDPixel,-1);

	//memDC.SelectObject( bitmp );
	//memDC.Clear();
	//memDC.SetBackgroundMode(wxTRANSPARENT);
	//memDC.SetBrush( *wxBLACK_BRUSH );
	//memDC.SetBackground(*wxBLACK);
	//memDC.DrawRectangle(0,0,bitmp.GetWidth(),bitmp.GetHeight());
	//memDC.SetBrush( *wxBLUE_BRUSH );
		
	try
	{
		DrawLayer(pBlockHead,pBlockData,nCurLayer,nCountOfLayer,layer,true);
		bRet=true;
	}
	catch(...)
	{
		bRet=false;
	}
	//if(bRet==true)
	//{
	//	wxImage temp = bitmp.ConvertToImage();
	//	temp.SetMaskColour(0,0,0);
	//	bmp = wxBitmap(temp);
	//}
	//wxString str1=wxString::Format(wxT("bitmap%dof%d.png"),nCurLayer,nCountOfLayer);
	//wxString str2=wxString::Format(wxT("bmp%dof%d.png"),nCurLayer,nCountOfLayer);
	//bitmp.SaveFile(str1,wxBITMAP_TYPE_PNG);
	//bmp.SaveFile(str2,wxBITMAP_TYPE_PNG);
};



void CProductStructDraw_SCT::DrawLayer(const void* pBlockHead,const void* pBlockData,/*gInt16 nScale,*/int nCurLayer,int nCountOfLayer ,Layer* layer, bool bFlagCheck/* = true*/)
{
	bool bRet=false;
	float nScale=1.0/m_fLenghUnit;

	//AS by ljg 20080814 for verify data.
	static STORM_CTHEAD* tLastHdata = NULL;
	static STORM_TRACK* tLastTdata = NULL;
	//AE by ljg 20080814 for verify data.
	STORM_CTHEAD* tHdata = (STORM_CTHEAD*)pBlockHead;//j=0
	STORM_TRACK* tTdata = (STORM_TRACK*)pBlockData;//j=1
	//MS by ljg 20080820 for same with document .
	for (int k = 0; k < tHdata->NumberOfStormCells ; k++)//k
	{
		if(nCurLayer!=nCountOfLayer-1)
		{
			DrawHisPoint(tTdata[k].sct.ASCur,tTdata[k].sct.RSCur/nScale,layer);
		}
		else
		{
			DrawCurPoint(tTdata[k].sct.ASCur,tTdata[k].sct.RSCur/nScale,layer);
			gFloat a0,r0,a1,r1;
			a0=tTdata[k].sct.ASCur;
			r0=tTdata[k].sct.RSCur/nScale;//0.01km

			for(int ii=0;ii<4;ii++)
			{
				a1=tTdata[k].scf.ASF[ii];
				r1=tTdata[k].scf.RSF[ii];//km
				if(!bFlagCheck || r1!=0)
				{
					DrawCastPoint(a1,r1,layer);
					DrawLine(a0,r0,a1,r1,layer);
					a0=a1;
					r0=r1;
				}
			}
		}
		if(!tTdata[k].sct.NewStorm)
		{
			//AS by ljg 20080814 for verify data.
			//DrawLine(tTdata[k].sct.ASPre,tTdata[k].sct.RSPre/nScale,tTdata[k].sct.ASCur,tTdata[k].sct.RSCur/nScale,pDC);
			if(!bFlagCheck || VerifyPosExist(tLastHdata,tLastTdata,tTdata[k].sct.ASPre,tTdata[k].sct.RSPre))
			{
				DrawLine(tTdata[k].sct.ASPre,tTdata[k].sct.RSPre/nScale,tTdata[k].sct.ASCur,tTdata[k].sct.RSCur/nScale,layer);
			}
			//AE by ljg 20080814 for verify data.
		};			
	}
	//ME by ljg 20080820 for same with document .
	tLastHdata=tHdata;
	tLastTdata=tTdata;
};



void CProductStructDraw_SCT::DrawSCT(ProductBase* pProduct_278_SCT, Layer* layer)
{
	bool bRet=false;
	//wxBitmap bitmp;
	//wxMemoryDC memDC;
	int nDPixel;
	PDBlock* blok;
	CProductRead* pThisProduct=(CProductRead*) pProduct_278_SCT;
	//*(GenerationData*)this = *(GenerationData*)pProduct_278_SCT;
	//ProductBase* pThisProduct=this;

	blok = pThisProduct->GetHeadOfProduct();
	if(blok!=NULL)
	{
		//MS by ljg 20080901 for part of data cutted off.
		int wight=blok->ProductDependent[1];
		int realwight=blok->ProductDependent[3];
		realwight = realwight > wight ? realwight : wight;
		CaculateLayout(realwight);
		wight*=1000;
		//int wight=blok->ProductDependent[1]*1000.0;
		//int realwight=blok->ProductDependent[3]*1000.0;
		//if(wight<=0)
		//{
		//	wight=460000;
		//}
		//if(realwight<=0)
		//{
		//	realwight=wight;
		//}
		//CaculateLayout(realwight/1000.0);
	}
	//nDPixel=m_fR_km/m_fKmPerPixel*2+1;//鐩村緞+1
	//nDPixel=m_fR_km/m_fKmPerPixel*2+1+4*m_nSizeOfStormTargCurrent;//鐩村緞+1+4*m_nSizeOfStormTarg
	nDPixel=m_fR_km/m_fKmPerPixel*2/*+1+4*m_nSizeOfStormTargCurrent*/;//鐩村緞+1+4*m_nSizeOfStormTarg
		//ME by ljg 20080901 for part of data cutted off.

	//bitmp.Create( nDPixel,nDPixel,-1);
	//bmp.Create( nDPixel,nDPixel,-1);

	//memDC.SelectObject( bitmp );
	//memDC.Clear();
	//memDC.SetBackgroundMode(wxTRANSPARENT);
	//memDC.SetBrush( *wxBLACK_BRUSH );
	//memDC.SetBackground(*wxBLACK);
	//memDC.DrawRectangle(0,0,bitmp.GetWidth(),bitmp.GetHeight());
		
	try
	{
		int i = 0;
		for ( i = 0; i < blok->NumberOfLayers ; i++)//i
		{
			DILayer* _layer = pThisProduct->GetHeadOfLayer(i);		
			if(2== _layer->NumberOfBlocks )
			{
				STORM_CTHEAD* tHdata = (STORM_CTHEAD*)pThisProduct->GetDataOfBlockStruct(i,0);//j=0
				STORM_TRACK* tTdata = (STORM_TRACK*)pThisProduct->GetDataOfBlockStruct(i,1);//j=1
				DrawLayer(tHdata,tTdata,i,blok->NumberOfLayers,layer,true);
				bRet=true;
			}
			else
			{
				bRet=false;
				break;
			}
		}
		if(i==blok->NumberOfLayers)
		{
			bRet=true;
		}
	}
	catch(...)
	{
		bRet=false;
	}
	//memDC.SelectObject(wxNullBitmap);
	//if(bRet==true)
	//{
	//	wxImage temp = bitmp.ConvertToImage();
	////	temp.SetMaskColour(0,0,0);
	//	bmp = wxBitmap(temp);
	//	//bmp.SaveFile(wxT("SCT.png"), wxBITMAP_TYPE_PNG);
	//}
};

void CProductStructDraw_SCT::Draw(ProductBase* pProduct, Layer* layer)
{
	DrawSCT(pProduct,layer);
};

void CProductStructDraw_SCT::DrawHisPoint(gFloat fA,gFloat fR,Layer* layer)
{
	//int		nR=m_nSizeOfStormTargHistory;
	//wxPoint pt=TransARtoXy(fA,fR);

	//wxBrush br=pDC->GetBrush();
	//wxPen pen=pDC->GetPen();

	//pDC->SetPen(wxPen(m_nColorHistory));
	//pDC->SetBrush(wxBrush(m_nColorHistory));
	//pDC->DrawCircle(pt,nR);
	//pDC->SetPen(pen);
	//pDC->SetBrush(br);
};


void CProductStructDraw_SCT::DrawCurPoint(gFloat fA,gFloat fR,Layer* layer)
{
	int nR=m_nSizeOfStormTargCurrent;
	osg::Vec2 pt=TransARtoXy(fA,fR);
	//wxBrush br=pDC->GetBrush();

	//wxPen pen=pDC->GetPen();

	//pDC->SetPen(wxPen(m_nColorCurrent));
	//pDC->SetBrush(wxColor(0,0,0));
	//pDC->DrawCircle(pt,nR);

	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();

	goQuadrilateral *pLine = new goQuadrilateral;
	pLine->setGlenum(osg::PrimitiveSet::LINES);
	osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
	osg::ref_ptr <osg::Vec4Array> temp_color_parray = new osg::Vec4Array;//颜色数组

	temp_parray->push_back(osg::Vec3(pt.x()-nR,pt.y()-nR ,0.51));
	temp_parray->push_back(osg::Vec3(pt.x()+nR,pt.y()+nR ,0.51));
	temp_parray->push_back(osg::Vec3(pt.x()-nR,pt.y()+nR ,0.51));
	temp_parray->push_back(osg::Vec3(pt.x()+nR,pt.y()-nR ,0.51));

	temp_color_parray->push_back(osg::Vec4 (0,0,0,1));

	pLine->SetCoordinateTransform(layer->CoordTrans());
	pLine->SetOsgArray(temp_parray);
	pLine->SetOsgColorArray(*temp_color_parray);
	pLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pLine));


	//pDC->DrawLine(pt.x-nR,pt.y-nR,	pt.x+nR,pt.y+nR);
	//pDC->DrawLine(pt.x-nR,pt.y+nR,	pt.x+nR,pt.y-nR);//(X)
	//pDC->SetBrush(br);
	//pDC->SetPen(pen);
};
void CProductStructDraw_SCT::DrawCastPoint(gFloat fA,gFloat fR,Layer* layer)
{
	osg::Vec2 pt=TransARtoXy(fA,fR);
	int nR=m_nSizeOfStormTargForeCast;
	//wxPen pen=pDC->GetPen();

	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();

	goQuadrilateral *pLine = new goQuadrilateral;
	pLine->setGlenum(osg::PrimitiveSet::LINES);
	osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
	osg::ref_ptr <osg::Vec4Array> temp_color_parray = new osg::Vec4Array;//颜色数组

	temp_parray->push_back(osg::Vec3(pt.x()-nR,	pt.y() ,0.51));
	temp_parray->push_back(osg::Vec3(pt.x()+nR,	pt.y(),0.51));
	temp_parray->push_back(osg::Vec3(pt.x(),	pt.y()+nR,0.51));
	temp_parray->push_back(osg::Vec3(pt.x(),	pt.y()-nR	 ,0.51));

	pLine->SetCoordinateTransform(layer->CoordTrans());
	pLine->SetOsgArray(temp_parray);
	pLine->SetOsgColorArray(*temp_color_parray);
	pLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pLine));
	temp_color_parray->push_back(m_nColorForeCast);
	//pDC->SetPen(wxPen(m_nColorForeCast));
	//pDC->DrawLine(pt.x-nR,	pt.y,	pt.x+nR,	pt.y	);//-
	//pDC->DrawLine(	pt.x,	pt.y+nR,	pt.x,	pt.y-nR	);//| -->(+)

	//pDC->SetPen(pen);
};
void CProductStructDraw_SCT::DrawLine(gFloat fA1,gFloat fR1,gFloat fA2,gFloat fR2,Layer* layer)
{
	osg::Vec2 pt1=TransARtoXy(fA1,fR1);
	osg::Vec2 pt2=TransARtoXy(fA2,fR2);
	//wxPen pen=pDC->GetPen();

	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();

	goQuadrilateral *pLine = new goQuadrilateral;
	pLine->setGlenum(osg::PrimitiveSet::LINES);
	osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
	osg::ref_ptr <osg::Vec4Array> temp_color_parray = new osg::Vec4Array;//颜色数组

	temp_parray->push_back(osg::Vec3(pt1.x(),	pt1.y() ,0.51));
	temp_parray->push_back(osg::Vec3(pt2.x(),	pt2.y(),0.51));
	temp_color_parray->push_back(m_nColorTrackLine);

	pLine->SetCoordinateTransform(layer->CoordTrans());
	pLine->SetOsgArray(temp_parray);
	pLine->SetOsgColorArray(*temp_color_parray);
	pLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pLine));

	//pDC->SetPen(wxPen(m_nColorTrackLine));
	//pDC->DrawLine(pt1,pt2);
	//pDC->SetPen(pen);
};

osg::Vec2 CProductStructDraw_SCT::TransARtoXy(gFloat fA,gFloat fR)
{
	osg::Vec2 pt;
	int x0,y0,x1,y1;
	int nR;
	gFloat fAb;

	fAb=fA/180.0*M_PI;
	nR=m_fR_km/m_fKmPerPixel;//鍗婂緞
	x0=y0=nR;

	
	//ljg 20080812
	x1=fR*sin(fAb)/m_fKmPerPixel;
	y1=fR*cos(fAb)/m_fKmPerPixel;
	pt.set(x0+x1,y0-y1);
	return pt;
};
bool CProductStructDraw_SCT::VerifyPosExist(const void* pBlockHead,const void* pBlockData,gFloat fA,gFloat fR)
{
	bool bRet=false;
	STORM_CTHEAD* tHdata = (STORM_CTHEAD*)pBlockHead;//j=0
	STORM_TRACK* tTdata = (STORM_TRACK*)pBlockData;//j=1
	if(tHdata!=NULL && tTdata!=NULL)
	{
		int n=tHdata->NumberOfStormCells;
		for(int i=0;i<n;i++)
		{
			if(fA==tTdata[i].scf.ASCur && fR==tTdata[i].scf.RSCur)
			{
				bRet=true;
				break;
			}
		};
	}
	return bRet;
};

void CProductStructDraw_SCT::CaculateLayout(gFloat fRMax)
{
	m_fR_km=fRMax;//230;
	osg::Vec2 sz(460,460);
	//if(m_pParam!=NULL)
	//{
	//	sz=m_pParam->m_size;
	//	//double k=m_pParam->GetRatio();
	//	//if(k>0&&k!=m_pParam->m_InitRatio)
	//	//{
	//	//	m_fKmPerPixel=k/1000.0;
	//	//	return;
	//	//}
	//}

	if(m_fR_km<=0)
	{
		m_fR_km=460;//here need modify.
		m_fLenghUnit=1;
	}
	else if(m_fR_km>1000.0)
	{
		m_fKmPerPixel=m_fR_km*2/1000.0;//鏄剧ず绐楀彛澶у皬涓?000x1000
	}
	else if(m_fR_km>100)
	{
		m_fKmPerPixel=1;//鏄剧ず绐楀彛澶у皬涓?*pds.m_fR_km x 2*pds.m_fR_km銆?
	}						
	else if(m_fR_km>10)
	{
		m_fKmPerPixel=0.1;//鏄剧ず绐楀彛澶у皬涓?0*pds.m_fR_km x 20*pds.m_fR_km銆?
	}
	else
	{			
		m_fKmPerPixel=0.01;//鏄剧ず绐楀彛澶у皬涓?00*pds.m_fR_km x 200*pds.m_fR_km銆?
	}
	int n=sz.x()<sz.y()?sz.x():sz.y();
	//m_fKmPerPixel=(m_fR_km*2+1+4*m_nSizeOfStormTarg)/n;
	m_fKmPerPixel=(m_fR_km*2/*+1+4*m_nSizeOfStormTarg*/)/n;
};

QImage CProductStructDraw_SCT::DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize)
{
	QImage image;
	return image;
}

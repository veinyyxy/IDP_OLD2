#include <math.h>
#include <QtCore/QCoreApplication>
#include "QtXml/QXmlStreamReader"
#include <QtCore/QFile>
#include <QDir>
#include <QDate>

#include <osgText/Text>

#include "RadProjTransform.h"
#include "goQuadrilateral.h"

#include "Struct_516_MSBT.h"
#include "ProductDraw_MSBT.h"

CProductStructDraw_MSBT::CProductStructDraw_MSBT()
{
};

CProductStructDraw_MSBT::~CProductStructDraw_MSBT()
{
};

//判断产品绘图类型
STRUCT_DATA_TYPE CProductStructDraw_MSBT::GetTypeOfProduct()
{
	STRUCT_DATA_TYPE n = STRUCT_MSBT;
	return n;
};
//判断产品绘图类型

//画产品图片，指定某张图片
bool CProductStructDraw_MSBT:: DumpAll(Layer* layer ,CProductRead* lpProduct, /* CProductDrawParam* lpParam,*/ P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer)
{
	//lpParam->SetRatio(500); //MMSBT产品的分辨率特殊

	tagBoundFileHeader * pHead = (tagBoundFileHeader*)(lpProduct->GetDataOfBlockStruct(0,0));
	
	//wxImage	   imageLayer;
	//wxBitmap   bitmap;
	//wxMemoryDC memDC;
	//
	//bitmap.Create(pHead->Width,pHead->Height,-1);
	//memDC.SelectObject(bitmap);
	//memDC.Clear();
	//memDC.SetBackgroundMode(wxTRANSPARENT);
	//memDC.SetBackground(*wxBLACK);
	//memDC.SetBrush( *wxBLACK_BRUSH );
	//memDC.DrawRectangle(0,0,bitmap.GetWidth(),bitmap.GetHeight());
	
	bool bRet=false;
	PDBlock* blok = lpProduct->GetHeadOfProduct();

	try
	{
		for (int i = requestLayer ; i < blok->NumberOfLayers ; i++)
		{
			tagBoundFileHeader * pHead = (tagBoundFileHeader*)(lpProduct->GetDataOfBlockStruct(i,0));
			OutBound * pDataBound = (OutBound*)lpProduct->GetDataOfBlockStruct(i,1);
			//memDC.SetPen(wxPen(lpParam->m_colorRibbon[i]));
			//memDC.SetBrush( wxBrush(lpParam->m_colorRibbon[i]));

			for(int j = 0; j< pHead->cntBound; j++)
			{
				int iCenX = pDataBound[j].head.cenX;
				int iCenY = pDataBound[j].head.cenY;					
				//DrawCircle(wxPoint(iCenX,iCenY),2); //画轮廓中心			

				for(int k = 0; k< pDataBound[j].head.len; k++)
				{
					int X = pDataBound[j].X[k];
					int Y = pDataBound[j].Y[k];
					if((X>=0) && (Y>=0))
					{
						DrawPoint(layer,osg::Vec3(X,Y,0.51),osg::Vec4(1.0,1.0,1.0,255));					
					}
				}			
			}

			if(i==0)
			{
				//imageLayer=bitmap.ConvertToImage();
			}
			else
			{
				//wxImage imageCur = bitmap.ConvertToImage();
				//imageCur.SetMaskColour(0,0,0);
				//imageLayer.Paste(imageCur,0,0);
			}			
		}
		//memDC.SelectObject(wxNullBitmap);
		//*lpBitmap = wxBitmap(imageLayer);
	}
	catch(...)
	{
		bRet=false;
	}
	return bRet;
};

void CProductStructDraw_MSBT::DrawPoint( Layer* layer , osg::Vec3 pointArray , osg::Vec4 color)
{
	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();

	goQuadrilateral *pPoint = new goQuadrilateral;
	osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
	osg::ref_ptr <osg::Vec4Array> temp_color_parray = new osg::Vec4Array;//颜色数组
	temp_color_parray->push_back(osg::Vec4(1.0, 1.0,1.0,255));
	temp_parray->push_back(pointArray);

	//pPoint->SetBTransform(false);
	pPoint->setGlenum(osg::PrimitiveSet::POINTS);
	pPoint->SetCoordinateTransform(layer->CoordTrans());
	pPoint->SetOsgArray(temp_parray);
	pPoint->SetOsgColorArray(*temp_color_parray);
	pPoint->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pPoint));
}

QImage CProductStructDraw_MSBT::DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize)
{
	QImage image;
	return image;
}






#include <math.h>
#include <QtCore/QCoreApplication>
#include "QtXml/QXmlStreamReader"
#include <QtCore/QFile>
#include <QDir>
#include <QDate>

#include "Struct_278_SCT.h"
#include "RadProjTransform.h"
#include "goQuadrilateral.h"

#include "Struct_288_SWP.h "
#include "ProductDraw_SWP.h"

CProductStructDraw_SWP::CProductStructDraw_SWP()
{
};

CProductStructDraw_SWP::~CProductStructDraw_SWP()
{
};

//判断产品绘图类型
STRUCT_DATA_TYPE CProductStructDraw_SWP::GetTypeOfProduct()
{
	STRUCT_DATA_TYPE n = STRUCT_SWP;
	return n;
};


bool CProductStructDraw_SWP::DumpAll( Layer* layer ,CProductRead* lpProduct, /* CProductDrawParam* lpParam,*/ P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer /*= 0*/ )
{
	bool bRet=false;
	//wxBitmap bitmp;
	//wxMemoryDC memDC;
	int nDPixel;
	PDBlock* blok;
	CProductRead* pThisProduct=(CProductRead*) lpProduct;
	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();

	blok = pThisProduct->GetHeadOfProduct();
	if(blok!=NULL)
	{			
		try
		{
			if( blok->NumberOfLayers == 1 )
			{
				DILayer* _layer=pThisProduct->GetHeadOfLayer(0);
				if( _layer->NumberOfBlocks == 2)
				{
					StructHead* lpsh1 = pThisProduct->GetHeadOfBlockStruct(0,0);		
					StructHead* lpsh2 = pThisProduct->GetHeadOfBlockStruct(0,1);		
					if(lpsh1!= NULL && lpsh2!= NULL)
					{
						SWPHead* tHdata = (SWPHead*)pThisProduct->GetDataOfBlockStruct(0,0);//j=0
						SWPStruct* tTdata = (SWPStruct*)pThisProduct->GetDataOfBlockStruct(0,1);//j=1
						int n=lpsh2->NumbersOfData;
						int k=blok->ScaleOfData;
						if(k<0)
						{
							k=1;
						}
						if(lpsh2->LengthOfDataType==sizeof(SWPStruct))
						{
							double dk=1,dkw,dkh;
							int nw,nh;
							dkw =tHdata->Width;
							dkh = tHdata->Height;
							dk = dkw < dkh ? dkw : dkh;
							if(dk<0)
							{
								dk = 1.0;
							}
							//lpParam->SetRatio(dk*1000);
							dkw = dk * tHdata->Width;
							dkh = dk * tHdata->Height;
							//bitmp.Create( dkw,dkh,-1);
							//lpBitmap->Create( dkw,dkh,-1);
							//bitmp.Create( tHdata->Width,tHdata->Height,-1);
							//lpBitmap->Create( tHdata->Width,tHdata->Height,-1);

							//memDC.SelectObject( bitmp );
							//memDC.Clear();
							//memDC.SetBackgroundMode(wxTRANSPARENT);
							//memDC.SetBrush( *wxBLACK_BRUSH );
							//memDC.SetBackground(*wxBLACK);

							goQuadrilateral *pPolygon = new goQuadrilateral;

							pPolygon->SetBTransform(false);
							pPolygon->setGlenum(osg::PrimitiveSet::LINE_LOOP);
							osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
							osg::ref_ptr <osg::Vec4Array> temp_color_parray = new osg::Vec4Array;//颜色数组

							temp_color_parray->push_back(osg::Vec4(0.0,0.0,0.0,1));

							temp_parray->push_back(osg::Vec3(0,0 ,0.51));
							temp_parray->push_back(osg::Vec3(0 ,dkh ,0.51));

							temp_parray->push_back(osg::Vec3(dkw ,dkh ,0.51));
							temp_parray->push_back(osg::Vec3(dkw ,0 ,0.51));


							pPolygon->SetCoordinateTransform(layer->CoordTrans());
							pPolygon->SetOsgArray(temp_parray);
							pPolygon->SetOsgColorArray(*temp_color_parray);
							pPolygon->SetParentLayer(layer);
							pMG->insert(Layer::MgoGeometryPair(pMG->size(),pPolygon));

							//memDC.DrawRectangle(0,0,bitmp.GetWidth(),bitmp.GetHeight());

							//wxDC* pDC=&memDC;

							for(int i=0;i<n;i++)
							{
								double value=double(tTdata[i].Probability)/k;
								//wxPoint pt=wxPoint(tTdata[i].x * dk,tTdata[i].y *dk);
								//wxString str=wxString::Format(wxT("%g"),value);
								//wxColour color=lpParam->GetColor(value);
								//pDC->SetTextForeground(color);
								//pDC->DrawText(str,pt);
								osg::Geode* pGeo = layer->GetLayerNode()->asGeode();
								if(pGeo)
									pGeo->addDrawable(DrawText(QString::number(value).toStdString(), osg::Vec3(tTdata[i].x * dk,tTdata[i].y *dk,0.51),1000));
							}
							//memDC.SelectObject(wxNullBitmap);
						}
						//DrawLayer(tHdata,tTdata,i,blok->NumberOfLayers,&memDC,true);
						bRet=true;
					}
				}
			}
		}
		catch(...)
		{
			bRet=false;
		}
	}

	if(bRet==true)
	{

	}
	return bRet;
}

osg::Drawable* CProductStructDraw_SWP::DrawText(const std::string& text,osg::Vec3 positionOffset,float size)
{
	osgText::Text* t = new osgText::Text();
	setlocale( LC_ALL, "chs" );
	int requiredSize = mbstowcs(NULL, text.c_str(), 0); 
	wchar_t* wtext = new wchar_t[requiredSize+1]; 
	mbstowcs(wtext, text.c_str(), requiredSize+1); 
	t->setText(wtext); 
	delete [] wtext; 
	wtext = NULL; 

	t->setPosition( positionOffset );
	t->setAutoRotateToScreen( false );
	t->setCharacterSizeMode( osgText::Text::OBJECT_COORDS_WITH_MAXIMUM_SCREEN_SIZE_CAPPED_BY_FONT_HEIGHT );
	t->setCharacterSize(16);//100000  
	t->setAxisAlignment( osgText::Text::XY_PLANE );
	t->setAlignment( osgText::Text::LEFT_CENTER );
	osgText::Font* font = osgText::readFontFile("Fonts/simhei.ttf");
	t->setFont(font);

	t->setBackdropColor( osg::Vec4( 0.3 ,  0.3 ,  0.3 , 1 ) );
	t->setBackdropType( osgText::Text::OUTLINE );

	return t;
}

QImage CProductStructDraw_SWP::DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize)
{
	QImage image;
	return image;
}

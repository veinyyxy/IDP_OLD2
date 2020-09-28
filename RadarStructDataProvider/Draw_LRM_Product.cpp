#include <math.h>
#include <QtCore/QCoreApplication>
#include "QtXml/QXmlStreamReader"
#include <QtCore/QFile>
#include <QDir>
#include <QDate>

#include "Struct_278_SCT.h"
#include "RadProjTransform.h"
#include "goQuadrilateral.h"

#include "Draw_LRM_Product.h"


CProductStructDraw_LRM::CProductStructDraw_LRM()
{
	m_iScaleOfData = 0;		//缩放值
	m_iNumberOfLayers = 0;	//层数
	m_iNumberOfBlocks = 0;	//块数
	m_iNumberOfRows = 0;	//行数
	m_iNumberOfCols = 0;	//列数	
	m_fLeft = 0.0;			//每行左移的宽度
	m_fWidth = 0.0;			//每小格的宽度
	m_fHeight = 0.0;		//每行的高度
	m_iHeightOfLayer = 0;	//每层的间距
	m_bLessOrMore = false;
};

CProductStructDraw_LRM::~CProductStructDraw_LRM()
{

};

//判断产品绘图类型
STRUCT_DATA_TYPE CProductStructDraw_LRM::GetTypeOfProduct()
{
	STRUCT_DATA_TYPE n= STRUCT_LRM;
	return n;
};

//画产品图片，指定某张图片
bool CProductStructDraw_LRM:: DumpAll(Layer* layer ,CProductRead* lpProduct, /* CProductDrawParam* lpParam,*/ P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer)//画产品图片，指定某张图片
{
	GetInitValue(lpProduct);
	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();

	//wxBitmap bitmap;
	//wxMemoryDC  memDC;

	//bitmap.Create(900, 600, -1);
	//memDC.SelectObject(bitmap);
	//memDC.Clear();
	//memDC.SetPen(*wxRED);
	//memDC.SetBrush(*wxBLACK_BRUSH);
	//memDC.SetBackground(*wxBLACK);
	//memDC.SetBackgroundMode(wxTRANSPARENT);
	//memDC.SetTextForeground(wxColour(8, 0, 4));	

	bool bRet = false;

	try
	{
		for(int i=0; i<m_iNumberOfLayers; i++)
		{
			for (int j = 0; j<m_iNumberOfBlocks; j++)
			{
				//start 画每层的图像i
				for (int ii = 0; ii<m_iNumberOfRows; ii++)
				{
					osg::Vec2 DrawPoint[4];

					DrawPoint[0].set(DrawPoint[0].x(),GetOrdinate(i, ii));
					GetPointOrdinate(DrawPoint);

					gInt16  *arr = (gInt16 *)lpProduct->GetDataOfBlockGrid(i, j, ii);

					for (int jj=0; jj<m_iNumberOfCols; jj++)
					{
						DrawPoint[0].set(GetAbscissa(i, ii, jj),DrawPoint[0].y());
						GetPointAbscissa(DrawPoint);					

						//if(arr[jj]/m_iScaleOfData > 0)
						//{
						//	float ftemp =((float)(arr[jj])/(float)(m_iScaleOfData));

						//	for (int h=0; h<lpParam->m_nCount; h++)
						//	{
						//		float a = lpParam->m_cn[h][0];
						//		float b = lpParam->m_cn[h][1];

						//		if ((ftemp >= a) && (ftemp <= b))
						//		{
								//	memDC.SetPen(wxPen(lpParam->m_colorRibbon[h]));
								//	memDC.SetBrush(wxBrush(lpParam->m_colorRibbon[h]));
								//	memDC.DrawPolygon(4, DrawPoint, 0, 0, wxWINDING_RULE);

							goQuadrilateral *pPolygon = new goQuadrilateral;
							pPolygon->SetBTransform(false);
							pPolygon->setGlenum(osg::PrimitiveSet::LINE_LOOP);
							osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
							osg::ref_ptr <osg::Vec4Array> temp_color_parray = new osg::Vec4Array;//颜色数组

							temp_color_parray->push_back(osg::Vec4(1.0, 1.0,1.0,255));
							temp_color_parray->push_back(osg::Vec4(1.0, 1.0,1.0,255));
							temp_color_parray->push_back(osg::Vec4(1.0, 1.0,1.0,255));
							temp_color_parray->push_back(osg::Vec4(1.0, 1.0,1.0,255));

							temp_parray->push_back(osg::Vec3(DrawPoint[0].x(),DrawPoint[0].y(),0.51));
							temp_parray->push_back(osg::Vec3(DrawPoint[1].x(),DrawPoint[1].y(),0.51));
							temp_parray->push_back(osg::Vec3(DrawPoint[2].x(),DrawPoint[2].y(),0.51));
							temp_parray->push_back(osg::Vec3(DrawPoint[3].x(),DrawPoint[3].y(),0.51));

							pPolygon->SetCoordinateTransform(layer->CoordTrans());
							pPolygon->SetOsgArray(temp_parray);
							pPolygon->SetOsgColorArray(*temp_color_parray);
							pPolygon->SetParentLayer(layer);
							pMG->insert(Layer::MgoGeometryPair(pMG->size(),pPolygon));

								//	break;
								//}
							//}
						//}						
					}

				} //end of row	
				//end 画每层的图像i

				//start 画边框e汉字
				//memDC.SetBrush(*wxTRANSPARENT_BRUSH);
				//memDC.SetPen(wxPen(wxColour(8, 0, 4)));	

				osg::Vec2 BGPoint[4];
				BGPoint[0].set(GetAbscissa(i, 0, 0),BGPoint[0].y());
				BGPoint[0].set(BGPoint[0].x() , GetOrdinate(i, 0));

				if (m_iNumberOfLayers < 9)
				{
					GetLessPoint(BGPoint);			
				}
				else if (m_iNumberOfLayers > 8)
				{							
					GetMorePoint(BGPoint);					
				}				
				
				BGPoint[0].set(BGPoint[0].x() -= 2,BGPoint[0].y()); //将图像左侧超出部分圈进来
				BGPoint[3].set(BGPoint[3].x() -= 2,BGPoint[3].y());				
								
				//memDC.DrawPolygon(4, BGPoint, 0, 0, wxWINDING_RULE);

				goQuadrilateral *pPolygon = new goQuadrilateral;
				pPolygon->SetBTransform(false);
				pPolygon->setGlenum(osg::PrimitiveSet::LINE_LOOP);
				osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
				osg::ref_ptr <osg::Vec4Array> temp_color_parray = new osg::Vec4Array;//颜色数组

				temp_color_parray->push_back(osg::Vec4(1.0, 1.0,1.0,255));
				temp_color_parray->push_back(osg::Vec4(1.0, 1.0,1.0,255));
				temp_color_parray->push_back(osg::Vec4(1.0, 1.0,1.0,255));
				temp_color_parray->push_back(osg::Vec4(1.0, 1.0,1.0,255));

				temp_parray->push_back(osg::Vec3(BGPoint[0].x(),BGPoint[0].y(),0.51));
				temp_parray->push_back(osg::Vec3(BGPoint[1].x(),BGPoint[1].y(),0.51));
				temp_parray->push_back(osg::Vec3(BGPoint[2].x(),BGPoint[2].y(),0.51));
				temp_parray->push_back(osg::Vec3(BGPoint[3].x(),BGPoint[3].y(),0.51));

				pPolygon->SetCoordinateTransform(layer->CoordTrans());
				pPolygon->SetOsgArray(temp_parray);
				pPolygon->SetOsgColorArray(*temp_color_parray);
				pPolygon->SetParentLayer(layer);
				pMG->insert(Layer::MgoGeometryPair(pMG->size(),pPolygon));


				DILayer *temp_layer = lpProduct->GetHeadOfLayer(i);
				QString Alt = "";
				Alt = QString::number(i+1);
				Alt = ("Layer:") + Alt;

				osg::Geode* pGeo = layer->GetLayerNode()->asGeode();
				if(pGeo)
					pGeo->addDrawable(DrawText(Alt.toStdString(), osg::Vec3(BGPoint[0].x()-130, BGPoint[0].y()-5,0.51),1000));
				//memDC.DrawText(Alt, wxPoint(BGPoint[0].x-130, BGPoint[0].y-5));
				Alt = ("Height (m):")+ QString::number(temp_layer->HeightOfElevation);
				//memDC.DrawText(Alt, wxPoint(BGPoint[0].x-130, BGPoint[0].y+10));
				if(pGeo)
					pGeo->addDrawable(DrawText(Alt.toStdString(), osg::Vec3(BGPoint[0].x()-130, BGPoint[0].y()+10,0.51),1000));
				//end 画边框与汉字

			} //end of block			
		} //end of layer

		bRet = true;	
	} //end of try

	catch(...)
	{
		bRet = false;
	}
	//memDC.SelectObject(wxNullBitmap);
	if(bRet == true)
	{
		//wxImage temp = bitmap.ConvertToImage();
		//temp.SetMaskColour(0,0,0);
		//*lpBitmap = wxBitmap(temp);
	}

	return bRet;
};
//画产品图片，指定某张图片

void CProductStructDraw_LRM::GetInitValue(CProductRead* lpProduct)
{
	PDBlock *blok = lpProduct->GetHeadOfProduct();
	if(blok == NULL)
	{
		return;
	}	
	m_iScaleOfData = blok->ScaleOfData;		//缩放值
	m_iNumberOfLayers = blok->NumberOfLayers;	//层数

	if (m_iNumberOfLayers > 8)
	{
		m_bLessOrMore = true;
	}

	DILayer *layer = lpProduct->GetHeadOfLayer(0);
	if(layer == NULL)
	{
		return;
	}
	m_iNumberOfBlocks = layer->NumberOfBlocks;

	GridHead *pgridhead = lpProduct->GetHeadOfBlockGrid(0, 0);
	if (pgridhead == NULL)
	{
		return;
	}
	m_iNumberOfRows = pgridhead->NumberOfRows;  //行数
	m_iNumberOfCols = pgridhead->NumberOfCols;  //列数

	//start 计算画产品时，每格的宽和高、每行左移的值和每层的间隔。
	m_fLeft	  = 0.0;
	m_fWidth  = 0.0;
	m_fHeight = 0.0;
	m_iHeightOfLayer = 0;	

	if ((m_iNumberOfLayers > 0) && (m_iNumberOfLayers < 9))
	{	
		m_fLeft = 100.0/m_iNumberOfRows;
		m_fWidth = 300.0/m_iNumberOfCols;	
		m_fHeight = 100.0/m_iNumberOfRows;		
	}
	else if ((m_iNumberOfLayers > 8) && (m_iNumberOfLayers < 16))
	{	
		m_fLeft = 60.0/m_iNumberOfRows;
		m_fWidth = 180.0/(float)m_iNumberOfCols;
		m_fHeight = 50.0/(float)m_iNumberOfRows;	
	}

	if ((m_iNumberOfLayers > 0) && (m_iNumberOfLayers < 5)) 
	{
		m_iHeightOfLayer = 600/m_iNumberOfLayers;
	}
	else if ((m_iNumberOfLayers > 4) && (m_iNumberOfLayers < 16))
	{
		if ((m_iNumberOfLayers%2) == 0)
		{
			m_iHeightOfLayer = 600/m_iNumberOfLayers*2;
		} 
		else
		{
			m_iHeightOfLayer = 600/(m_iNumberOfLayers+1)*2;
		}		
	}
	//end 计算画产品时，每格的宽和高，以及每层的间隔。
}

int CProductStructDraw_LRM::GetOrdinate(int i, int ii) //获得纵坐标
{
	int iOrdinate = 0;
	
	if (m_iNumberOfLayers < 5)
	{
		iOrdinate = m_iHeightOfLayer/2 + m_iHeightOfLayer*(m_iNumberOfLayers-i-1) + m_fHeight*ii - 35;	
	}
	else if (m_iNumberOfLayers > 4)
	{
		int iLeft  = 0;
		int iRight = 0;
		int iQuot = (m_iNumberOfLayers+1)/2;

		if (m_iNumberOfLayers%2 == 0)
		{
			iLeft = m_iNumberOfLayers - iQuot - i - 1;
			iRight = m_iNumberOfLayers - i - 1;
		}
		else
		{
			iLeft = m_iNumberOfLayers - iQuot - i;
			iRight = m_iNumberOfLayers - i;
		}

		if (!m_bLessOrMore)
		{
			if (i < iQuot)
			{
				iOrdinate = m_iHeightOfLayer/2 + m_iHeightOfLayer*iLeft + m_fHeight*ii  - 35;				
			}
			else if (i > (iQuot-1))
			{
				iOrdinate = m_iHeightOfLayer/2 + m_iHeightOfLayer*iRight + m_fHeight*ii - 35;			
			}
		}	
		else
		{
			if (i < iQuot)
			{							
				iOrdinate =  m_iHeightOfLayer/2 + m_iHeightOfLayer*iLeft +  m_fHeight*ii - 10;				
			}
			else if (i > (iQuot-1))
			{
				iOrdinate = m_iHeightOfLayer/2 + m_iHeightOfLayer*iRight + m_fHeight*ii - 10;				
			}
		}
	}

	return iOrdinate;
};

int CProductStructDraw_LRM::GetAbscissa(int i, int ii, int jj) //获得横坐标
{	
	int iAbscissa = 0;
	int iQuot = (m_iNumberOfLayers+1)/2;

	if (m_iNumberOfLayers < 5)
	{
		iAbscissa = 350 - ii*m_fLeft + jj*m_fWidth; 						
	}
	else if ((m_iNumberOfLayers > 4) && (m_iNumberOfLayers < 9))
	{		
		if (i < iQuot)
		{
			iAbscissa = 150 - ii*m_fLeft + jj*m_fWidth; 			
		}
		else if (i > (iQuot-1))
		{
			iAbscissa = 600 - ii*m_fLeft + jj*m_fWidth; 		
		}
	}
	else if ((m_iNumberOfLayers > 8) && (m_iNumberOfLayers < 16))
	{
		if (i < iQuot)
		{
			iAbscissa = 250 - ii*m_fLeft + jj*m_fWidth; 				
		}
		else if (i > (iQuot-1))
		{
			iAbscissa = 600 - ii*m_fLeft + jj*m_fWidth; 							
		}
	}

	return iAbscissa;
};

void CProductStructDraw_LRM::GetLessPoint(osg::Vec2 pnt[4])
{
	pnt[1].set(pnt[0].x() + 300,pnt[0].y());
	pnt[2].set(pnt[0].x() + 200,pnt[0].y() + 100);
	pnt[3].set(pnt[0].x() - 100 , pnt[2].y());

	/*	pnt[1].x = pnt[0].x + 300;
	pnt[2].x = pnt[0].x + 200;
	pnt[3].x = pnt[0].x - 100 ;

	pnt[1].y = pnt[0].y;
	pnt[2].y = pnt[0].y + 100;
	pnt[3].y = pnt[2].y;*/		
}

void CProductStructDraw_LRM::GetMorePoint(osg::Vec2 pnt[4])
{
	pnt[1].set(pnt[0].x() + 180,pnt[0].y());
	pnt[2].set(pnt[0].x() + 120,pnt[0].y() + 50);
	pnt[3].set(pnt[0].x() - 60 , pnt[2].y());

	/*	pnt[1].x = pnt[0].x + 180;
	pnt[2].x = pnt[0].x + 120;
	pnt[3].x = pnt[0].x - 60 ;

	pnt[1].y = pnt[0].y;
	pnt[2].y = pnt[0].y + 50;
	pnt[3].y = pnt[2].y;	*/	
}

void CProductStructDraw_LRM::GetPointAbscissa(osg::Vec2 pnt[4])
{
	pnt[1].set(pnt[0].x()+ m_fWidth ,pnt[1].y());
	pnt[3].set(pnt[0].x() - m_fLeft,pnt[3].y());
	pnt[2].set(pnt[3].x() + m_fWidth , pnt[2].y());


	/*	pnt[1].x = pnt[0].x + m_fWidth;
	pnt[3].x = pnt[0].x - m_fLeft;
	pnt[2].x = pnt[3].x + m_fWidth;*/	
}

void CProductStructDraw_LRM::GetPointOrdinate(osg::Vec2 pnt[4])
{
	pnt[1].set(pnt[1].x() ,pnt[0].y());
	pnt[2].set(pnt[2].x(), pnt[0].y() + m_fHeight);
	pnt[3].set(pnt[3].x() , pnt[2].y());

	//pnt[1].y = pnt[0].y;
	//pnt[2].y = pnt[0].y + m_fHeight;
	//pnt[3].y = pnt[2].y;
}

QImage CProductStructDraw_LRM::DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize)
{
	QImage image;
	return image;
}

osg::Drawable* CProductStructDraw_LRM::DrawText( const std::string& text,osg::Vec3 positionOffset,float size )
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

#include "RadProjTransform.h"
#include "goLineString.h"
#include "goQuadrilateral.h"

#include <osgText/Text>
#include <osg/Point>

#include <math.h>
#include <vector>

#include <QtCore/QCoreApplication>
#include "QtXml/QXmlStreamReader"
#include <QtCore/QFile>
#include <QDir>
#include <QDate>

#include "Struct_285_EVP_269_VWP_268_VAD.h"
#include "ProductStructDraw_EVP.h"

using namespace std;

CProductStructDraw_EVP::CProductStructDraw_EVP(void)
{
	
}


CProductStructDraw_EVP::~CProductStructDraw_EVP(void)
{
}

//判断产品绘图类型
STRUCT_DATA_TYPE CProductStructDraw_EVP::GetTypeOfProduct()
{
	STRUCT_DATA_TYPE n = STRUCT_EVP;
	return n;
};

bool CProductStructDraw_EVP::DumpAll( Layer* layer,CProductRead* lpProduct, P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer /*= 0*/ )
{
	bool bRet=false;
	PDBlock* blok = lpProduct->GetHeadOfProduct();
	try
	{
		for (int i = requestLayer ; i < blok->NumberOfLayers ; i++)
		{
			DILayer* _layer = lpProduct->GetHeadOfLayer(i);
			//wxBitmap bitmp;
			//wxImage imageLayer;
			for (int j = 0;j < _layer->NumberOfBlocks ; j++)
			{
				DLIBlock* dblock = lpProduct->GetHeadOfBlock(i,j);
				if (dblock->TypeOfDataPackets == BLOCKTYPE_STRUCT)
				{				
					StructHead * structHead=lpProduct->GetHeadOfBlockStruct(i,j);
					gInt16	NumbersOfStruct=structHead->NumbersOfData;		//结构个数
					gInt16	LengthOfEachStruct=structHead->LengthOfDataType;	//每个结构体字节数
					float	Scale=(float)blok->ScaleOfData;						//数据放大倍数
					void * p = lpProduct->GetDataOfBlockStruct(i, j);
					if (blok->ProductCode == 285)//画VAD产品
					{
						//struct_product_evp evp;

						for(int i=0; i<NumbersOfStruct; i++)
						{
							if((((EVP_ELEMENTS *)p)[i].div>=-200 && ((EVP_ELEMENTS *)p)[i].div<=200)
								&& (((EVP_ELEMENTS *)p)[i].shear>=-200 && ((EVP_ELEMENTS *)p)[i].shear<=200)
								&& (((EVP_ELEMENTS *)p)[i].stretch>=-200 && ((EVP_ELEMENTS *)p)[i].stretch<=200))
							{
								Register_Point(((EVP_ELEMENTS *)p)[i]);
							}
						}

						//bitmp.Create(800,600,-1);
						Draw_EVP_Background(layer);
						//bitmp.SaveFile(wxT("bitmap_.png"), wxBITMAP_TYPE_PNG);
					}
					//if(j==0)
					//{
					//	imageLayer=bitmp.ConvertToImage();
					//}
					//else
					//{
					//	wxImage imageCur = bitmp.ConvertToImage();
					//	imageCur.SetMaskColour(0,0,0);
					//	imageLayer.Paste(imageCur,0,0);
					//}
				}
			}
			//*lpBitmap=wxBitmap(imageLayer);
			break;
		}
		bRet=true;
	}	
	catch(...)
	{
		bRet=false;
	}	
	return bRet;
}

void CProductStructDraw_EVP::Draw_EVP_Background( Layer* layer )
{
	osg::Vec3 origin_of_coordinate(540, 50,0.51); //坐标原点

	////wxMemoryDC memDC;
	//memDC.SelectObject( bitmap_ );
	//memDC.Clear();
	//memDC.SetBackgroundMode(wxTRANSPARENT);

	//memDC.SetPen(wxPen(wxColour(8,0,4)));
	//memDC.SetBrush(wxBrush(wxColour(8,0,4)));

	//memDC.DrawLine(origin_of_coordinate, osg::Vec2(origin_of_coordinate.x()+700, origin_of_coordinate.y()));
	//memDC.DrawLine(origin_of_coordinate, osg::Vec2(origin_of_coordinate.x(), origin_of_coordinate.y()-480));
	DrawLine(layer, origin_of_coordinate, osg::Vec3(origin_of_coordinate.x()+700, origin_of_coordinate.y(),0.51));
	DrawLine(layer,origin_of_coordinate, osg::Vec3(origin_of_coordinate.x(), origin_of_coordinate.y()+480 ,0.51));

	//begin for draw the background lines
	osg::Vec3 tmp_point_s = origin_of_coordinate;
	osg::Vec3 tmp_point_e(origin_of_coordinate.x()+120, origin_of_coordinate.y(),0.51);

	for(int i=0; i<8; i++)
	{
		tmp_point_s.set(tmp_point_s.x(), tmp_point_s.y()+60,0.51);
		tmp_point_e.set(tmp_point_e.x(), tmp_point_e.y()+60,0.51);
		DrawLine(layer,tmp_point_s, tmp_point_e);
	}

	for(int i=1; i<5; i++)
	{
		//memDC.DrawLine(osg::Vec2(origin_of_coordinate.x+30*i, origin_of_coordinate.y), wxPoint(origin_of_coordinate.x+30*i, origin_of_coordinate.y-480));
		DrawLine(layer, osg::Vec3(origin_of_coordinate.x()+30*i, origin_of_coordinate.y(),0.51) , osg::Vec3(origin_of_coordinate.x()+30*i, origin_of_coordinate.y()+480 ,0.51));
	}
	DrawLine(layer,osg::Vec3(origin_of_coordinate.x()+145, origin_of_coordinate.y(),0.51), osg::Vec3(origin_of_coordinate.x()+145, origin_of_coordinate.y()+480,0.51));

	tmp_point_s.set(origin_of_coordinate.x() +145,tmp_point_s.y(),0.51);
	tmp_point_s.set(tmp_point_s.x() ,origin_of_coordinate.y(), 0.51);
	tmp_point_e.set(tmp_point_s.x()+120,tmp_point_e.y(),0.51);
	tmp_point_e.set(tmp_point_e.x(), tmp_point_s.y(),0.51);

	for(int i=0; i<8; i++)
	{
		tmp_point_s.set(tmp_point_s.x(), tmp_point_s.y()+60,0.51);
		tmp_point_e.set(tmp_point_e.x(), tmp_point_e.y()+60,0.51);
		DrawLine(layer,tmp_point_s, tmp_point_e);
	}

	for(int i=1; i<5; i++)
	{
		DrawLine(layer,osg::Vec3(origin_of_coordinate.x()+145+30*i, origin_of_coordinate.y(),0.51), osg::Vec3(origin_of_coordinate.x()+145+30*i, origin_of_coordinate.y()+480,0.51));
	}
	DrawLine(layer,osg::Vec3(origin_of_coordinate.x()+290, origin_of_coordinate.y(),0.51), osg::Vec3(origin_of_coordinate.x()+290, origin_of_coordinate.y()+480,0.51));

	tmp_point_s.set(origin_of_coordinate.x() +290,tmp_point_s.y(),0.51);
	tmp_point_s.set(tmp_point_s.x(), origin_of_coordinate.y(),0.51);
	tmp_point_e.set(tmp_point_s.x()+120,tmp_point_e.y(),0.51);
	tmp_point_e.set(tmp_point_e.x() ,tmp_point_s.y(), 0.51);

	for(int i=0; i<8; i++)
	{
		tmp_point_s.set(tmp_point_s.x(), tmp_point_s.y()+60,0.51);
		tmp_point_e.set(tmp_point_e.x() ,tmp_point_e.y()+60,0.51);
		DrawLine(layer,tmp_point_s, tmp_point_e);
	}

	for(int i=1; i<5; i++)
	{
		DrawLine(layer,osg::Vec3(origin_of_coordinate.x()+290+30*i, origin_of_coordinate.y(),0.51), osg::Vec3(origin_of_coordinate.x()+290+30*i, origin_of_coordinate.y()+480,0.51));
	}
	DrawLine(layer, osg::Vec3(origin_of_coordinate.x()+435, origin_of_coordinate.y(),0.51), osg::Vec3(origin_of_coordinate.x()+435, origin_of_coordinate.y()+480,0.51));

	tmp_point_s.set(origin_of_coordinate.x() +435,origin_of_coordinate.y(),0.51);
	//tmp_point_s.y = origin_of_coordinate.y();
	tmp_point_e.set(tmp_point_s.x()+120, tmp_point_s.y(),0.51);
	//tmp_point_e.y = tmp_point_s.y;

	for(int i=0; i<8; i++)
	{
		tmp_point_s.set(tmp_point_s.x(), tmp_point_s.y()+60,0.51);
		tmp_point_e.set(tmp_point_e.x(), tmp_point_e.y()+60,0.51);
		DrawLine(layer,tmp_point_s, tmp_point_e);
	}

	for(int i=1; i<5; i++)
	{
		DrawLine(layer, osg::Vec3(origin_of_coordinate.x()+435+30*i, origin_of_coordinate.y(),0.51), osg::Vec3(origin_of_coordinate.x()+435+30*i, origin_of_coordinate.y()+480,0.51));
	}
	DrawLine(layer,osg::Vec3(origin_of_coordinate.x()+580, origin_of_coordinate.y(),0.51), osg::Vec3(origin_of_coordinate.x()+580, origin_of_coordinate.y()+480,0.51));

	tmp_point_s.set(origin_of_coordinate.x() +580,origin_of_coordinate.y(),0.51);
	//tmp_point_s.y = origin_of_coordinate.y;
	tmp_point_e.set(tmp_point_s.x()+120,tmp_point_s.y(),0.51);
	//tmp_point_e.y = tmp_point_s.y;

	for(int i=0; i<8; i++)
	{
		tmp_point_s.set(tmp_point_s.x(), tmp_point_s.y()+60,0.51);
		tmp_point_e.set(tmp_point_e.x(), tmp_point_e.y()+60,0.51);
		DrawLine(layer,tmp_point_s, tmp_point_e);
	}

	for(int i=1; i<5; i++)
	{
		DrawLine(layer,osg::Vec3(origin_of_coordinate.x()+580+30*i, origin_of_coordinate.y(),0.51), osg::Vec3(origin_of_coordinate.x()+580+30*i, origin_of_coordinate.y()+480,0.51));
	}


	//end for draw the background lines

	//begin for add the vertical values
	int start_of_elevation = 50;  //海拔起始值  单位:10m/s
	int end_of_elevation = 15500;	//海拔终止值   单位:10m/s
	int dif = 2000;

	//memDC.SetFont ( wxFont ( 8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD ) );
	//memDC.SetTextForeground (wxColour(8,0,4));
	QString coordinate_value = "";
	coordinate_value = QString::number(start_of_elevation)+"m";
	//coordinate_value += _("m");
	DrawText(layer, coordinate_value, osg::Vec3(origin_of_coordinate.x()-45, origin_of_coordinate.y(),0.51));
	start_of_elevation = 1000;
	coordinate_value = QString::number(start_of_elevation/1000)+"km";
	//coordinate_value += _("m");
	DrawText(layer,coordinate_value, osg::Vec3(origin_of_coordinate.x()-45, origin_of_coordinate.y()+60,0.51));

	for(int i=2; i<8; i++)
	{
		start_of_elevation += dif;
		coordinate_value = QString::number(start_of_elevation/1000)+"km";
		//coordinate_value += _("m");
		DrawText(layer, coordinate_value, osg::Vec3(origin_of_coordinate.x()-45, origin_of_coordinate.y()+0+60*i,0.51));
	}
	QString szTop = ("15.5km");
	//szTop += _("m");
	DrawText(layer, szTop, osg::Vec3(origin_of_coordinate.x()-45, origin_of_coordinate.y()+480,0.51));

	//end for add the vertical values

	//begin for add the horizontal values
	for(int i=0; i<5; i++)
	{
		coordinate_value = QString::number(90*i);
		DrawText(layer, coordinate_value, osg::Vec3(origin_of_coordinate.x()+28*i-5, origin_of_coordinate.y()-10,0.51));
	}
	DrawText(layer,("风向 (°)"), osg::Vec3(origin_of_coordinate.x()+10*2, origin_of_coordinate.y()-30,0.51));

	for(int i=0; i<5; i++)
	{
		coordinate_value = QString::number(32*i);
		DrawText(layer,coordinate_value, osg::Vec3(origin_of_coordinate.x()+145+28*i-5, origin_of_coordinate.y()-10,0.51));

	}
	DrawText(layer, ("风速(m/s)"), osg::Vec3(origin_of_coordinate.x()+145+10*2, origin_of_coordinate.y()-30,0.51));

	DrawText(layer, ("0"), osg::Vec3(origin_of_coordinate.x()+290+56, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("-200"), osg::Vec3(origin_of_coordinate.x()+290-10, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("-100"), osg::Vec3(origin_of_coordinate.x()+290+28-10, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("100"), osg::Vec3(origin_of_coordinate.x()+290+84-10, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("200"), osg::Vec3(origin_of_coordinate.x()+290+112-10, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("散度(10-6/s)"), osg::Vec3(origin_of_coordinate.x()+290+10*2, origin_of_coordinate.y()-30,0.51));

	DrawText(layer, ("0"), osg::Vec3(origin_of_coordinate.x()+435+56, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("-200"), osg::Vec3(origin_of_coordinate.x()+435-10, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("-100"), osg::Vec3(origin_of_coordinate.x()+435+28-10, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("100"), osg::Vec3(origin_of_coordinate.x()+435+84-10, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("200"), osg::Vec3(origin_of_coordinate.x()+435+112-10, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("拉伸变形(10-6/s)"), osg::Vec3(origin_of_coordinate.x()+435, origin_of_coordinate.y()-30,0.51));

	DrawText(layer, ("0"), osg::Vec3(origin_of_coordinate.x()+580+56, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("-200"), osg::Vec3(origin_of_coordinate.x()+580-10, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("-100"), osg::Vec3(origin_of_coordinate.x()+580+28-10, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("100"), osg::Vec3(origin_of_coordinate.x()+580+84-10, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("200"), osg::Vec3(origin_of_coordinate.x()+580+112-10, origin_of_coordinate.y()-10,0.51));
	DrawText(layer, ("切变形变(10-6/s)"), osg::Vec3(origin_of_coordinate.x()+580, origin_of_coordinate.y()-30,0.51));

	//end for add the horizontal values

	vector<osg::Vec2> Du_vec;
	vector<osg::Vec2> Uh_vec;
	vector<osg::Vec2> Div_vec;
	vector<osg::Vec2> Stretch_vec;
	vector<osg::Vec2> Shear_vec;

	for(list<EVP_ELEMENTS>::iterator itr=point_register_list.begin(); itr!=point_register_list.end(); itr++)
	{
		int hight =(*itr).hight;	//高度5-1550
		hight = hight*10;
		int y = 0;
		int m = 0;
		int n = 0;
		if(hight<50)
		{
			hight = 50;		//<50的按50处理
		}
		if(hight>15500)
		{
			hight = 15500;	//>15500的按15500处理
		}
		if(hight<1000)
		{
			y = origin_of_coordinate.y() + hight*60/(1000-50);
		}
		if((hight<=15500) && (hight>=13000))
		{
			y = origin_of_coordinate.y() + 60*7 + hight*60/(15500-13000);
		}
		else
		{
			hight -=1000;
			m = hight/2000;
			n = hight%2000;
			y = origin_of_coordinate.y() + 60 + m*60 + n*60/2000; //高度坐示值
		}


		int Du =(*itr).Du;	//风向0-3600
		if(Du>3600)
		{
			Du = 3600;
		}
		m = Du/900;
		n = Du%900;
		int Du_x = origin_of_coordinate.x()+30*m+30*n/900;	//风向的x坐标值
		Du_vec.push_back(osg::Vec2(Du_x, y));

		int Uh = (*itr).Uh;	//风速0-1280
		Uh = Uh/10;
		if(Uh>1280)
		{
			Uh = 1280;
		}
		m = Uh/320;
		n = Uh%320;
		int Uh_x = origin_of_coordinate.x()+145+30*m+30*n/320;//风速的x坐标值
		Uh_vec.push_back(osg::Vec2(Uh_x, y));

		double Div = (*itr).div;	//散度 放大倍数100
		int Div_x = 0;
		m = (int)(Div)/100;
		n = (int)(Div)%100;
		Div_x = origin_of_coordinate.x()+290+30*2+30*m+n*30/100;//散度的x坐标值			
		Div_vec.push_back(osg::Vec2(Div_x, y));


		double Stretch = (*itr).stretch;	//拉伸形变 放大倍数1000000
		int Stretch_x = 0;
		m = (int)(Stretch)/100;
		n = (int)(Stretch)%100;
		Stretch_x = origin_of_coordinate.x()+435+30*2+30*m+n*30/100;//散度的x坐标值			
		Stretch_vec.push_back(osg::Vec2(Stretch_x, y));

		double Shear = (*itr).shear;	//切变形变 放大倍数1000000
		int Shear_x = 0;
		m = (int)(Shear)/100;
		n = (int)(Shear)%100;
		Shear_x = origin_of_coordinate.x()+580+30*2+30*m+n*30/100;//散度的x坐标值			
		Shear_vec.push_back(osg::Vec2(Shear_x, y));

	}

	int num = Du_vec.size();
	if (num<3)
	{
		//memDC.SelectObject(wxNullBitmap);
		return;
	}
	osg::Vec3Array* pPointArray0 = new osg::Vec3Array;
	for(int i=0; i<num; i++)
	{
		pPointArray0->push_back(osg::Vec3(Du_vec[i].x(),Du_vec[i].y(),0.51));
	}
	//memDC.SetPen(wxPen(*wxBLUE));
	//memDC.DrawSpline(num, m_point);
	DrawPoints(layer, pPointArray0, osg::Vec4(0.0,0.0,1.0,1));

	//m_pPointArray->clear();
	osg::Vec3Array* pPointArray1 = new osg::Vec3Array;
	for(int i=0; i<num; i++)
	{
		pPointArray1->push_back(osg::Vec3(Uh_vec[i].x(),Uh_vec[i].y(),0.51));
	}
	//memDC.SetPen(wxPen(*wxRED));
	//memDC.DrawSpline(num, m_point);
	DrawPoints(layer,pPointArray1,osg::Vec4(1.0,0.0,0.0,1));

	osg::Vec3Array* pPointArray2 = new osg::Vec3Array;
	for(int i=0; i<num; i++)
	{
		pPointArray2->push_back(osg::Vec3(Div_vec[i].x(),Div_vec[i].y(),0.51));
	}
	//memDC.SetPen(wxPen(*wxGREEN));
	//memDC.DrawSpline(num, m_point);
	DrawPoints(layer,pPointArray2,osg::Vec4(0.0,1.0,0.0,1));

	osg::Vec3Array* pPointArray3 = new osg::Vec3Array;
	for(int i=0; i<num; i++)
	{
		pPointArray3->push_back(osg::Vec3(Stretch_vec[i].x(),Stretch_vec[i].y(),0.51));
	}
	//memDC.SetPen(wxPen(wxColour(100,100,100)));
	//memDC.DrawSpline(num, m_point);
	DrawPoints(layer,pPointArray3,osg::Vec4(1.0,1.0,1.0,1));

	osg::Vec3Array* pPointArray4 = new osg::Vec3Array;
	for(int i=0; i<num; i++)
	{
		pPointArray4->push_back(osg::Vec3(Shear_vec[i].x(),Shear_vec[i].y(),0.51));
	}
	//memDC.SetPen(wxPen(*wxBLUE));
	//memDC.DrawSpline(num, m_point);
	DrawPoints(layer,pPointArray4,osg::Vec4(0.0,0.0,1.0,1));
	//memDC.SelectObject(wxNullBitmap);
}

void CProductStructDraw_EVP::Register_Point( EVP_ELEMENTS &p )
{
	if(point_register_list.empty())
	{
		point_register_list.push_back(p);
	}
	else
	{
		bool bIn = false;
		for(list<EVP_ELEMENTS>::iterator itr = point_register_list.begin(); itr!=point_register_list.end(); itr++)
		{

			if((*itr).hight>p.hight)
			{
				point_register_list.insert(itr,1,p);
				bIn = true;
				break;
			}
		}
		if(!bIn)
		{
			point_register_list.push_back(p);
		}
	}
}

void CProductStructDraw_EVP::DrawLine(Layer* layer , osg::Vec3 strPostion , osg::Vec3 destPostion)
{
	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();
	goQuadrilateral *pLine = new goQuadrilateral;
	osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
	osg::ref_ptr <osg::Vec4Array> temp_color_parray = new osg::Vec4Array;//颜色数组

	temp_color_parray->push_back(osg::Vec4(1.0, 1.0,1.0,255));
	temp_color_parray->push_back(osg::Vec4(1.0, 1.0,1.0,255));

	temp_parray->push_back(strPostion);
	temp_parray->push_back(destPostion);

	pLine->SetBTransform(false);
	pLine->setGlenum(osg::PrimitiveSet::LINE_LOOP);
	pLine->SetCoordinateTransform(layer->CoordTrans());
	pLine->SetOsgArray(temp_parray);
	pLine->SetOsgColorArray(*temp_color_parray);
	pLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pLine));
}

void CProductStructDraw_EVP::DrawPoints( Layer* layer , osg::Vec3Array* pointArray , osg::Vec4 color)
{
	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();

	goQuadrilateral *pPoint = new goQuadrilateral;
	//osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
	osg::ref_ptr <osg::Vec4Array> temp_color_parray = new osg::Vec4Array;//颜色数组
	temp_color_parray->push_back(osg::Vec4(1.0, 1.0,1.0,255));

	pPoint->SetBTransform(false);
	pPoint->setGlenum(osg::PrimitiveSet::POINTS);
	pPoint->SetCoordinateTransform(layer->CoordTrans());
	pPoint->SetOsgArray(pointArray);
	pPoint->SetOsgColorArray(*temp_color_parray);
	pPoint->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pPoint));

}

void CProductStructDraw_EVP::DrawText(Layer* layer ,  QString _text, osg::Vec3 postion )
{
	string text = _text.toStdString();
	osgText::Text* t = new osgText::Text();
	setlocale( LC_ALL, "chs" );
	int requiredSize = mbstowcs(NULL, text.c_str(), 0); 
	wchar_t* wtext = new wchar_t[requiredSize+1]; 
	mbstowcs(wtext, text.c_str(), requiredSize+1); 
	t->setText(wtext); 
	delete [] wtext; 
	wtext = NULL; 

	t->setPosition( postion );
	t->setAutoRotateToScreen( false );
	t->setCharacterSizeMode( osgText::Text::OBJECT_COORDS_WITH_MAXIMUM_SCREEN_SIZE_CAPPED_BY_FONT_HEIGHT );
	t->setCharacterSize(16);//100000  
	t->setAxisAlignment( osgText::Text::XY_PLANE );
	t->setAlignment( osgText::Text::LEFT_CENTER );
	osgText::Font* font = osgText::readFontFile("Fonts/simhei.ttf");
	t->setFont(font);

	t->setBackdropColor( osg::Vec4( 0.3 ,  0.3 ,  0.3 , 1 ) );
	t->setBackdropType( osgText::Text::OUTLINE );
	osg::Geode* pGeo = layer->GetLayerNode()->asGeode();
	if(pGeo)
		pGeo->addDrawable(t);
}

QImage CProductStructDraw_EVP::DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize)
{
	QImage image;
	return image;
}

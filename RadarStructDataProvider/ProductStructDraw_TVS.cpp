#include "RadProjTransform.h"
#include "goLineString.h"
#include "goQuadrilateral.h"
#include "goPolygon.h"

#include <osgText/Text>

#include <math.h>
#include <QtCore/QCoreApplication>
#include "QtXml/QXmlStreamReader"
#include <QtCore/QFile>
#include <QDir>
#include <QDate>

#include "Struct_293_TVS.h"
#include "ProductStructDraw_TVS.h"


CProductStructDraw_TVS::CProductStructDraw_TVS(void)
{
}

CProductStructDraw_TVS::~CProductStructDraw_TVS(void)
{
}

//�жϲ�Ʒ��ͼ����
STRUCT_DATA_TYPE CProductStructDraw_TVS::GetTypeOfProduct()
{
	STRUCT_DATA_TYPE n = STRUCT_TVS;
	return n;
};
//�жϲ�Ʒ��ͼ����

//����ƷͼƬ��ָ��ĳ��ͼƬ
bool CProductStructDraw_TVS::DumpAll(Layer* layer,CProductRead* lpProduct, P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer)
{     
	int aaa = 1000; //1000�����ص㣬һ�����ص����0.5���һ��500����뾶250����
	int bb = 1000;
	float resolution = 0.5; //500m

	//wxImage	    imageLayer;
	//wxBitmap    bitmap;
	//wxMemoryDC  memDC;
	//bitmap.Create(aaa, bb, -1);
	//memDC.SelectObject(bitmap);
	//memDC.Clear();
	//memDC.SetBackgroundMode(wxTRANSPARENT);
	//memDC.SetBackground(*wxBLACK);
	//memDC.SetBrush( *wxBLACK_BRUSH );
	//memDC.DrawRectangle(0, 0, bitmap.GetWidth(), bitmap.GetHeight());

	bool bRet=false;

	try
	{
		int iCenterX = aaa/2;
        int iCenterY = bb/2;
		osg::Vec2 centerPoint = osg::Vec2(iCenterX,iCenterY);
		int iRadiux = (float)1/(float)resolution;  //�뾶 1km��2������

		//memDC.SetPen(wxPen(wxColour(8, 0, 4),1));//��ɫ����ɫ
		//memDC.DrawRectangle(wxPoint(0, 0), wxSize(aaa, bb));  //��������


		Layer::MgoGeometry* pMG = layer->GetMgoGeometry();

		goQuadrilateral *pPolygon = new goQuadrilateral;
		pPolygon->SetBTransform(false);
		pPolygon->setGlenum(osg::PrimitiveSet::LINE_LOOP);

		osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//��������
		osg::ref_ptr <osg::Vec4Array> temp_color_parray = new osg::Vec4Array;//��ɫ����

		temp_color_parray->push_back(osg::Vec4(8, 0, 4,1));
		temp_parray->push_back(osg::Vec3(0,0,0.51));
		temp_parray->push_back(osg::Vec3(0,bb,0.51));
		temp_parray->push_back(osg::Vec3(aaa,bb,0.51));
		temp_parray->push_back(osg::Vec3(aaa,0,0.51));

		pPolygon->SetCoordinateTransform(layer->CoordTrans());
		pPolygon->SetOsgArray(temp_parray);
		pPolygon->SetOsgColorArray(*temp_color_parray);
		pPolygon->SetParentLayer(layer);
		pMG->insert(Layer::MgoGeometryPair(pMG->size(),pPolygon));

		//memDC.SetPen(wxPen(wxColour(0, 255, 0))); //�״�վλ��
		//memDC.DrawCircle(centerPoint, iRadiux);

		TVSprohead * pheadstruct = (TVSprohead*)(lpProduct->GetDataOfBlockStruct(0, 0));
		int thresholdnum = pheadstruct->DV_thresholdnum;
		int max3dnum_onethreshold = pheadstruct->max3dnum_onethreshold;

		TVShead *  ptvsheadstruct=(TVShead*)(lpProduct->GetDataOfBlockStruct(0,1));
        TVSstruct * ptvsstruct = (TVSstruct * )(lpProduct->GetDataOfBlockStruct(0,2)); 

		for(int i = 0; i < thresholdnum; i++)
		{
			unsigned int TotalNum = ptvsheadstruct[i].ETVSnum + ptvsheadstruct[i].TVSnum;
			TVSstruct * p = ptvsstruct  + max3dnum_onethreshold * i;
			for(int j = 0; j < TotalNum; j++)
			{
				float x =  p[j].x;  //���״�վ�ĺ�����룬��λkm����-��+��
				float y =  p[j].y;  //���״�վ��������룬��λkm����-��+��
				int iAddX = (float)x/(float)resolution; //һ�����ر�ʾ0.5����ж��Ǿ��״�վ����ڼ�����
                int iAddY = (float)y/(float)resolution; //һ�����ر�ʾ0.5����ж��Ǿ��״�վ����ڼ�����
				int Draw_X = iCenterX + iAddX;
                int Draw_Y = iCenterY + iAddY;
				
				if(p[j].type == 1) //TVS
				{
					//memDC.SetPen(wxPen(lpParam->m_colorRibbon[i],1)); //��Բ(2������)
					//memDC.DrawCircle(wxPoint(Draw_X, Draw_Y), iRadiux);

					//memDC.DrawText(wxT("TVS"), wxPoint(Draw_X, Draw_Y)); //��������1
				}
				else if(p[j].type == 2) //ETVS
				{
					//memDC.SetPen(wxPen(lpParam->m_colorRibbon[i],1)); //��Բ(2������)
					//memDC.DrawCircle(wxPoint(Draw_X, Draw_Y), iRadiux);

					//memDC.DrawText(wxT("ETVS"), wxPoint(Draw_X, Draw_Y)); //��������2
				}				
			}
		}

		//imageLayer=bitmap.ConvertToImage();

		//memDC.SelectObject(wxNullBitmap);
		//*lpBitmap = wxBitmap(imageLayer);
	}	
	catch(...)
	{
		bRet=false;
	}

	return bRet;
};


void CProductStructDraw_TVS::DrawText(Layer* layer ,  QString _text, osg::Vec3 postion )
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

QImage CProductStructDraw_TVS::DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize)
{
	QImage image;
	return image;
}

//�жϲ�Ʒ��ͼ������������ͼ
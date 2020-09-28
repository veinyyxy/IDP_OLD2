#include "Draw_VWP_Product.h"
#include "Struct_285_EVP_269_VWP_268_VAD.h"

#include <osgText/Text>

#include <math.h>
#include <QtCore/QCoreApplication>
#include "QtXml/QXmlStreamReader"
#include <QtCore/QFile>
#include <QDir>
#include <QDate>
#include <qtcore/QVector>
#include <qstring>

#include "goQuadrilateral.h"
#include "RadProjTransform.h"


//#if (defined(WIN32) || defined(WIN64))
//#include "../BDB-RAVEN/db_ui.hpp"
//#else
//#include "../BDB-RAVEN/db_ui.hpp"
//#endif

#include "Draw_VWP_Product.h"
//#include "../Arithmetics/Arith_285_EVP_269_VWP_268_VAD/Struct_285_EVP_269_VWP_268_VAD.h"
#include "Draw_VWP_Product.h"
//#include "CJulMsGMT.h"
//#include "GDataFiles.h"
//#include "DirSeparator.h"
//#include "xmlSettings.h"
//#include "CommonMethod.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class Draw_VWP_Product 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//add by ljg 20080826 for functions recharge.
Draw_VWP_Product::Draw_VWP_Product()
{
	Max_V  = -100.0;
};

Draw_VWP_Product::~Draw_VWP_Product()
{

};
STRUCT_DATA_TYPE Draw_VWP_Product::GetTypeOfProduct()
{
	STRUCT_DATA_TYPE n = STRUCT_VWP;
	return n ;
}


osg::Drawable* Draw_VWP_Product::DrawText(const std::string& text,osg::Vec3 positionOffset,float size)
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
	//t->setAxisAlignment( osgText::Text::XZ_PLANE );
	//1115247.1
	//unsigned int  colour = TextColor();
	////int ir, ig, ib, ia;
	//osg::Vec4 colorVec4;
	//unsigned char ia = colour >> 24 & 0xFF;
	//unsigned char ir = colour >> 16 & 0xFF;
	//unsigned char ig = colour >> 8 & 0xFF;
	//unsigned char ib = colour & 0xFF;
	//colorVec4.set(ir,ig,ib,ia);
	//t->setColor(colorVec4);
	osgText::Font* font = osgText::readFontFile("Fonts/simhei.ttf");
	t->setFont(font);

	t->setBackdropColor( osg::Vec4( 0.3 ,  0.3 ,  0.3 , 1 ) );
	t->setBackdropType( osgText::Text::OUTLINE );

	return t;
}
//�жϲ�Ʒ��ͼ����
//CProductDrawType Draw_VWP_Product::GetTypeOfProduct()
//{
//	CProductDrawType n = enProductStruct;
//	return n;
//};
//�жϲ�Ʒ��ͼ����

//����ƷͼƬ��ָ��ĳ��ͼƬ

bool Draw_VWP_Product::DumpAll(Layer* layer,CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,QString filename,int requestLayer/* = 0*/)
{
	bool bRet=false;

	try
	{
		bRet=DrawVwpVer0(layer,pColorTable,/*lpBitmap,*/lpProduct,/*lpParam,*/requestLayer);	
		CreatAttributeList(layer, lpProduct,filename);
	}	
	catch(...)
	{
		bRet=false;
	}
	return bRet;
};

//����ƷͼƬ��ָ��ĳ��ͼƬ
bool Draw_VWP_Product::DrawVwpVer0(/*wxBitmap* lpBitmap,*/Layer* layer,P_COLORTABLE_VECTOR pColorTable,CProductRead* lpProduct,/* CProductDrawParam* lpParam,*/int requestLayer /*= 0*/)
{
	bool bRet=false;
	try
	{
		PDBlock* blok = lpProduct->GetHeadOfProduct();	
		gInt16 iProductCode = blok->ProductCode;	//��Ʒ��
		gInt16 iNumOfLayers = blok->NumberOfLayers;	//����
		gInt16 iProductDate = blok->ProductDate;	//��ɨ����
		gInt32 iProductTime = blok->ProductTime;	//��ɨʱ��

		for (int i=requestLayer; i<iNumOfLayers; i++)
		{
			/*wxBitmap bitmp;
			wxImage imageLayer;*/
			
			for (int j=0; j<lpProduct->GetHeadOfLayer(i)->NumberOfBlocks; j++)
			{
				if (lpProduct->GetHeadOfBlock(i, j)->TypeOfDataPackets == BLOCKTYPE_STRUCT)
				{						     
					if(iProductCode == 269)//��vwp ��Ʒ
					{
						int iAbscissa[11] = {130,191,252,313,374,435,496,557,618,679,740}; //11��������											

						int hour = 0;
						int minute = 0;
						QStringList strTime;
						Get_Scan_Time(hour, minute, iProductTime);	
						Get_Time_Abscissa(strTime, hour, minute);	
						Draw_VWP_Background(/*bitmp,*/layer,strTime, iAbscissa); //������

						void * p =lpProduct->GetDataOfBlockStruct(0, 0);
						gInt16	NumbersOfStruct = lpProduct->GetHeadOfBlockStruct(i,j)->NumbersOfData; //�ṹ���� 
						Draw_VWP_WindFeather(layer,p,/* bitmp, */NumbersOfStruct, pColorTable, /*lpParam->m_cn,lpParam->m_nCount, */iAbscissa[10]); //����ǰ��ɨ�ķ���

						//if (g_RequestMessageType == Product_Request_Routine) //������ճ���Ʒ�������ݿ⣬������Ҫ�����ɨ������
						//{	
						//	QString desc;
						//	QString szDate = wxT("");
						//	QString strSiteID = wxT("");
						//	QString szProductName = wxT("vwp");
						//	Draw_vwp.Get_Site_ID(strSiteID, lpProduct);
						//	Draw_vwp.Get_Scan_Date(szDate, iProductDate);													
						//	//mk_desc_c(&desc, "product", "routine",  "sys", (const char*)(strSiteID.mb_str()),
						//	//	(const char*)(szProductName.mb_str()), (const char*)(szDate.mb_str()), "", "", "", "", NULL);

						///*	FINAL_FIND_RESULT rslt;
						//	rslt.clear();
						//	{
						//		ACE_GUARD_RETURN(ACE_Thread_Mutex, dblock, g_DBLock, false);
						//		db_upgrade(0);
						//		db_query(desc, rslt);
						//	}	*/											

						//	for(FINAL_FIND_RESULT::iterator itr=rslt.begin(); itr!=rslt.end(); itr++)
						//	{
						//		list<ACE_CString>  Result_list;
						//		Result_list = itr->second;							
						//	
						//		for(list<ACE_CString>::iterator itr_list=Result_list.begin();itr_list!=Result_list.end();itr_list++)
						//		{								
						//			wxString FilePath =  wxString((char*)(*itr_list).c_str(), wxConvLibc);

						//			if (wxFile::Exists(FilePath))
						//			{
						//				wxString temp = FilePath.AfterLast(mcDirSeparator);
						//				temp = temp.AfterFirst('_');
						//				temp = temp.AfterFirst('_');
						//				temp = temp.AfterFirst('_');
						//				wxString szHourMinute = temp.Left(4);
						//				
						//				for (int i=0; i<strTime.Count(); i++)
						//				{
						//					if (strTime.Item(i).IsSameAs(szHourMinute))
						//					{
						//						GenerationData * gData = new GenerationData();
						//						GDataFiles * gDataFile  = (GDataFiles*)gData;
						//						int tt = (int)FilePath.Length();
						//						char * m_pFileName = NULL;
						//						m_pFileName = new char[tt*2];
						//						strcpy(m_pFileName, FilePath.mb_str());
						//						int ret = gDataFile->ReadFrom(m_pFileName);
						//						JUDGE_AND_FREE_ARRAY(m_pFileName, NULL);
						//						cDrawProduct *produceDraw = (cDrawProduct*)gDataFile;
						//						void *p = produceDraw->ReadStructData();

						Draw_VWP_WindFeather(layer,p, NumbersOfStruct,pColorTable, iAbscissa[10-i]);


												//JUDGE_AND_FREE(gData, NULL)
											//}
										//}
									//}
									//itr_list++;

								//}
							}	
						}
					}

					/*if(j == 0)
					{
						imageLayer = bitmp.ConvertToImage();
					}
					else
					{
						wxImage imageCur = bitmp.ConvertToImage();
						imageCur.SetMaskColour(0,0,0);
						imageLayer.Paste(imageCur,0,0);
					}*/
				}
			}

			//*lpBitmap = wxBitmap(imageLayer);
			//lpBitmap->SaveFile(wxT("12345.png"), wxBITMAP_TYPE_PNG);
//			break;
//		}
	//	bRet=true;
//	}	
	catch(...)
	{
		bRet=false;
	}
	return bRet;
};//add by ljg 20090727 for ��VWP��
//bool Draw_VWP_Product::DrawVwpVer1(/*wxBitmap* lpBitmap,*/P_COLORTABLE_VECTOR pColorTable,CProductRead* lpProduct,/* CProductDrawParam* lpParam,*/int requestLayer /*= 0*/)
//{
//	bool bRet=false;
//
//	try
//	{
//		PDBlock* blok = lpProduct->GetHeadOfProduct();	
//		gInt16 iProductCode = blok->ProductCode;	//��Ʒ��
//		gInt16 iNumOfLayers = blok->NumberOfLayers;	//����
//		gInt16 iProductDate = blok->ProductDate;	//��ɨ����
//		gInt32 iProductTime = blok->ProductTime;	//��ɨʱ��
//
//		int iAbscissa[11] = {130,191,252,313,374,435,496,557,618,679,740}; //11��������											
//
//		//wxBitmap bitmp;
//		//wxImage imageLayer;			
//
//		int hour = 0;
//		int minute = 0;
//		QString* strTime;
//		Draw_VWP_Product Draw_vwp;
//		Draw_vwp.Get_Scan_Time(hour, minute, iProductTime);	
//		//Draw_vwp.//Get_Time_Abscissa(strTime, hour, minute);	
//		Draw_vwp.Draw_VWP_Background(/*bitmp,*/ strTime, iAbscissa); //������
//
//		//imageLayer = bitmp.ConvertToImage();
//
//		for (int i=0; i<iNumOfLayers; i++)
//		{
//			DILayer* lpLayer=lpProduct->GetHeadOfLayer(i);
//
//			//int k=0;
//			int nhour = 0;
//			int nminute = 0;
//			QString strTimei;
//
//			Draw_vwp.Get_Scan_Time(nhour, nminute, lpLayer->LayerTime);	
//			strTimei = QString(tr("%02d%02d")).arg(nhour).arg(nminute);
//
//			//k=strTime.Index(strTimei);
//			/*if(k<0)
//			{
//				k=i;
//			}*/
//
//			for (int j=0; j<lpLayer->NumberOfBlocks; j++)
//			{
//				if (lpProduct->GetHeadOfBlock(i, j)->TypeOfDataPackets == BLOCKTYPE_STRUCT)
//				{						
//					void * p =lpProduct->GetDataOfBlockStruct(i, j);
//					gInt16	NumbersOfStruct = lpProduct->GetHeadOfBlockStruct(i,j)->NumbersOfData; //�ṹ����
//					Draw_vwp.Draw_VWP_WindFeather(p, /*bitmp,*/ NumbersOfStruct, 
//						pColorTable, /*lpParam->m_cn,
//						lpParam->m_nCount,*/ iAbscissa[10-/*k*/i]); //����ǰ��ɨ�ķ���
//
//					/*wxImage imageCur = bitmp.ConvertToImage();
//					imageCur.SetMaskColour(0,0,0);
//					imageLayer.Paste(imageCur,0,0);*/
//				}
//			}
//		}
//		/**lpBitmap = wxBitmap(imageLayer);*/
//		bRet=true;
//	}	
//	catch(...)
//	{
//		bRet=false;
//	}
//	return bRet;
//};//add by ljg 20090727 for ��VWP��

//�жϲ�Ʒ��ͼ������������ͼ

int Draw_VWP_Product::GetCountOfLayer(CProductRead* lpProduct)
{
	int nCount=0;

	//nCount = 1;
	PDBlock* blok = lpProduct->GetHeadOfProduct();
	if(blok!=NULL )
	{
		nCount=blok->NumberOfLayers;
	}
	return nCount;
};
//�жϲ�Ʒ��ͼ������������ͼ

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//class Draw_VWP_Product 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Draw_VWP_Product::Draw_VWP_Product(void)
//{
//}
//
//Draw_VWP_Product::~Draw_VWP_Product(void)
//{
//
//}

//void Draw_VWP_Product::Get_Site_ID(QString &strTime, CProductRead *lpProduct) //վ����
//{
//	RadarInfor * pRadarInfo = lpProduct->GetRadarInfoP(); 
//	QString strSiteID(pRadarInfo->SiteCode,/* wxConvLibc,*/ sizeof(pRadarInfo->SiteCode)); //�״�վ��
//	strTime=((cDrawProduct*)lpProduct)->TransToRadarCode5(strSiteID);	//modify by ljg 20090115 for bug2096
//}
//
//void Draw_VWP_Product::Get_Scan_Date(QString &szDate, int iProductDate) //��ɨ����
//{
//	gInt32 year = 0;
//	gInt32 mouth = 0;
//	gInt32 day = 0;	
////	CJulMsGMT::Jul2Greg((gInt16)iProductDate, year, mouth, day); //��ɨ��ʼʱ��
//
//	QString szTemp = wxT("");
//	szTemp.Printf(wxT("%d"),year);
//	szDate = szTemp;
//	szTemp.Printf(wxT("%d"),mouth);
//
//	if (mouth < 10)  
//	{		
//		szTemp = wxT("0") + szTemp;
//	}
//
//	szDate = szDate + szTemp;
//	szTemp.Printf(wxT("%d"),day);
//
//	if (mouth < 10)  
//	{		
//		szTemp = wxT("0") + szTemp;
//	}
//
//	szDate = szDate + szTemp; //��ɨ����
//}
//
//void Draw_VWP_Product::Get_Scan_Time(int &hour, int &minute, int iProductTime) //��ɨʱ��
//{
//	gInt32 tmpTime = iProductTime;
//	gInt32 tmpHour = 0;
//	gInt32 tmpMin = 0;
//	gInt32 second = 0;
//	gInt32 msecond = 0;
//	CJulMsGMT::MilliSecond2Time(tmpTime, tmpHour, tmpMin, second, msecond); 
//	hour = tmpHour;
//	minute = tmpMin;
//}

void Draw_VWP_Product::Get_Time_Abscissa(QStringList &strTime, int hour, int minute)
{
	int i = 0;
	int iMinute[11]; //��¼����ֵ
	int iHourChange = 0; //Сʱ�����仯��ʱ��

	while (i < 11)
	{
		iMinute[i] = minute;
		minute = minute - 6;

		if (minute < 0)
		{
			minute = minute + 60;

			if (0 == iMinute[0]) //��ֹ����ʼ����Ϊ0ʱ����
			{
				iHourChange = 1;
			} 
			else
			{
				if (i != 10)
				{
					iHourChange = i + 1;
				}									
			}																
		}

		i++;							
	}
	
	QString szHour = "";
	szHour = QString::number(hour);
	long lHour = 0;
	lHour = szHour.toLong();

	if ((lHour >= 0) && (lHour < 10)) 
	{
		szHour = "0" + szHour;
	}						

	for (int i=0; i<iHourChange; i++)
	{
		QString szMinute = "";
		szMinute = QString::number(iMinute[i]);

		if (iMinute[i] < 10)
		{
			strTime.append(szHour + "0" + szMinute);
		} 
		else
		{
			strTime.append(szHour + szMinute);
		}								
	}

	hour = hour - 1;
	szHour = QString::number(hour);

	if ((hour>=0)&&(hour<10))
	{						
		szHour = "0" + szHour; 
	}
	else if (hour == -1)
	{
		szHour = "23";
	}

	for (int i=iHourChange; i<11; i++)
	{
		QString szMinute = "";
		szMinute = QString::number(iMinute[i]);

		if (iMinute[i] < 10)
		{
			strTime.append(szHour + "0" + szMinute);
		} 
		else
		{
			strTime.append(szHour + szMinute);
		}								
	}
}

void Draw_VWP_Product::Draw_VWP_Background(Layer* layer,QStringList strTime,int iAbscissa[11])//��vwp����
{
	/*wxMemoryDC memDC;
	bitmap.Create(801, 601, -1);
	memDC.SelectObject(bitmap);
	memDC.SetBackgroundMode(wxTRANSPARENT);
	memDC.SetBackground(*wxWHITE_BRUSH);
	memDC.SetPen(wxPen(wxColour(8, 0, 4)));
	memDC.SetBrush(wxBrush(wxColour(8, 0, 4)));
	memDC.SetTextForeground(wxColour(8, 0, 4));
	memDC.SetTextBackground(*wxWHITE);
	memDC.Clear();*/

	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();
	osg::Geode* pGeo = layer->GetLayerNode()->asGeode();
	if(pGeo == 0) return;

	osg::ref_ptr <osg::Vec3Array> temp_array = new osg::Vec3Array  ;//��������
	osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//��ɫ����

	goQuadrilateral * pLine = new goQuadrilateral[46];
	goQuadrilateral * pTempLine = pLine;

	temp_array->push_back(osg::Vec3(69, 0, 0.51));
	temp_array->push_back(osg::Vec3(69, 46+31*16, 0.51));
	temp_color_array->push_back(osg::Vec4(1.0, 1.0,1.0,255));

	pTempLine->SetBTransform(false);
	pTempLine->setGlenum(osg::PrimitiveSet::LINES);
	pTempLine->SetCoordinateTransform(layer->CoordTrans());
	pTempLine->SetOsgArray(temp_array);
	pTempLine->SetOsgColorArray(*temp_color_array);
	pTempLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));
	pTempLine = ++pLine;


	osg::ref_ptr <osg::Vec3Array> temp_array0= new osg::Vec3Array  ;//��������
	osg::ref_ptr <osg::Vec4Array> temp_color_array0 = new osg::Vec4Array;//��ɫ����

	temp_array0->push_back(osg::Vec3(1, 30, 0.51));
	temp_array0->push_back(osg::Vec3(801, 30, 0.51));
	temp_color_array0->push_back(osg::Vec4(1.0, 1.0,1.0,255));

	pTempLine->SetBTransform(false);
	pTempLine->setGlenum(osg::PrimitiveSet::LINES);
	pTempLine->SetCoordinateTransform(layer->CoordTrans());
	pTempLine->SetOsgArray(temp_array0);
	pTempLine->SetOsgColorArray(*temp_color_array0);
	pTempLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));
	//memDC.DrawLine(69, 40, 69, 590);
	//memDC.DrawLine(1, 30, 801, 30);

	pTempLine = ++pLine;

	float altitude = 0.0;
	//QEText* pText = new QEText[33];
	for(int i=0; i<=30; i++)
	{
		osg::ref_ptr <osg::Vec3Array> temp_array1= new osg::Vec3Array  ;//��������
		osg::ref_ptr <osg::Vec4Array> temp_color_array1 = new osg::Vec4Array;//��ɫ����

		temp_array1->push_back(osg::Vec3(69, 46+i*16, 0.51));
		temp_array1->push_back(osg::Vec3(801, 46+i*16, 0.51));
		temp_color_array1->push_back(osg::Vec4(1.0, 1.0,1.0,255));

		pTempLine->SetBTransform(false);
		pTempLine->setGlenum(osg::PrimitiveSet::LINES);
		pTempLine->SetCoordinateTransform(layer->CoordTrans());
		pTempLine->SetOsgArray(temp_array1);
		pTempLine->SetOsgColorArray(*temp_color_array1);
		pTempLine->SetParentLayer(layer);
		pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

		pTempLine = ++pLine;

		if(i < 20)
		{
			altitude += 0.3;
		}
		if((i >= 20) && (i < 25))
		{
			altitude += 0.6;
		}
		if(i>=25)
		{
			altitude += 1.0;
		}
		
		//QEText* pTempText = pText++;
		//pTempText->SetColor(QColor(65,226,218,255));
		//pTempText->SetFont(QFont(("����"), 9));

		QString tempStr;//��Ʒ��λ
		tempStr = QString((""));
		tempStr.sprintf(("%2.1f"), altitude);
		//pTempText->SetString(tempStr);	
		//pTempText->GetPositionArray()->push_back(QVector3D(35, 538-i*16,0.51));
		pGeo->addDrawable(DrawText(tempStr.toStdString(), osg::Vec3(35, 46+i*16,0.51),1000));	
		//container->InsertRenderable(pTempText);

		/*wxString Alt=_T("");
		Alt.Printf(wxT("%2.1f"), altitude);
		memDC.DrawText(Alt, wxPoint(35, 538-i*16));*/
	}
	QString minute,hour,time;
	
	for(int i=1; i<12; i++)
	{
		osg::ref_ptr <osg::Vec3Array> temp_array2= new osg::Vec3Array  ;//��������
		osg::ref_ptr <osg::Vec4Array> temp_color_array2 = new osg::Vec4Array;//��ɫ����

		temp_array2->push_back(osg::Vec3(69+i*61, 35, 0.51));
		temp_array2->push_back(osg::Vec3(69+i*61, 30, 0.51));
		temp_color_array2->push_back(osg::Vec4(1.0, 1.0,1.0,255));
		pTempLine->SetBTransform(false);
		pTempLine->setGlenum(osg::PrimitiveSet::LINES);
		pTempLine->SetCoordinateTransform(layer->CoordTrans());
		pTempLine->SetOsgArray(temp_array2);
		pTempLine->SetOsgColorArray(*temp_color_array2);
		pTempLine->SetParentLayer(layer);
		pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

		minute = strTime[11-i].right(2);  //add by dqm for �ı�ʱ����ʾ��ʽ
		hour   = strTime[11-i].left(2);
		time = hour + ":"+ minute;
		//memDC.DrawText(time, wxPoint(iAbscissa[i-1]-15, 568));
		pGeo->addDrawable(DrawText(time.toStdString(), osg::Vec3(iAbscissa[i-1]-20, 20 ,0.51),1000));	
		pTempLine = ++pLine;
	}

	//QEText* pTempText = pText++;
	//pTempText->SetColor(QColor(65,226,218,255));
	//pTempText->SetFont(QFont(("����"), 12));

	QString tempStr;//��Ʒ��λ
	tempStr = QString(("ʱ��"));
	//tempStr.sprintf(("%2.1f"), altitude);
	//pTempText->SetString(tempStr);	
	//pTempText->GetPositionArray()->push_back(QVector3D(30, 570,0.51));
	//container->InsertRenderable(pTempText);
	pGeo->addDrawable(DrawText(tempStr.toStdString(), osg::Vec3(30, 15,0.51),1000));	

	//pTempText = pText++;
	//pTempText->SetColor(QColor(65,226,218,255));
	//pTempText->SetFont(QFont(("����"), 12));

//	QString tempStr;//��Ʒ��λ
	tempStr = QString(("�߶� KM"));
	//tempStr.sprintf(("%2.1f"), altitude);
	//pTempText->SetString(tempStr);	
	//pTempText->GetPositionArray()->push_back(QVector3D(30, 20,0.51));
	//container->InsertRenderable(pTempText);
	pGeo->addDrawable(DrawText(tempStr.toStdString(), osg::Vec3(30, 46+32*16,0.51),1000));	
//	wxFont font(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	//memDC.SetFont(font);
	//memDC.DrawText( _("TIME"), wxPoint(30, 570));
	//memDC.DrawText(_("ALT KM"), wxPoint(30, 20));
	//memDC.SelectObject(wxNullBitmap);
}

void Draw_VWP_Product::Draw_VWP_WindFeather(Layer* layer,void *p,/*wxBitmap &bmp,*/ int NumbersOfStruct,P_COLORTABLE_VECTOR pColorTable/*const wxColour colorRibbon[],*//*float cn[][2],int memberofcn,*/,int iAbscissa)//������
{
	/*wxMemoryDC memDC;
	memDC.SelectObject(bmp);*/
	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();
	osg::Geode* pGeo = layer->GetLayerNode()->asGeode();
	osg::ref_ptr <osg::Vec3Array> temp_array = new osg::Vec3Array  ;//��������
	osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//��ɫ����

	int iOrdinate = 30;
	goQuadrilateral * pLine = new goQuadrilateral[47*50];
	goQuadrilateral * pTempLine = pLine;

	for(int i=0; i<NumbersOfStruct; i++)
	{
		VWP_ELEMENTS Vwp=((VWP_ELEMENTS *)p)[i];

		float ResidualLevel = (float)Vwp.Uh/100; //����
		float Direction = (float)Vwp.Du/10; //����
		float RMS = (float)Vwp.RMS/100; //���پ�������	

		int Width        = 8;	//ÿ����־�ĳ���
		int BetweenEach  = 3;	//ÿ����־֮��ľ���
		int LengthOfBarb = 25;	//����ĳ���

		//Ѱ������ٶ�
		if (ResidualLevel > Max_V)
			Max_V = ResidualLevel;

		double dx, dy;
		dx = sin ( Direction* M_PI / 180.0 ) * BetweenEach;
		dy = 0 + cos ( Direction * M_PI / 180.0 ) * BetweenEach;

		QPoint CurrentRealPoint, CurrentRealPeak, CurrentRealHeel;
		QPoint CurrentPoint, CurrentPeak, CurrentHeel, LastPoint;
		QPoint StartPoint;

		StartPoint.setX(iAbscissa);
		StartPoint.setY(iOrdinate+16);

		int jhg = iOrdinate;

		if((RMS<0) && (Direction<0) && (ResidualLevel<0))//����Ϊ��ʱ�����ND��
		{
			//QEText* pText = new QEText[1];
			//QEText* pTempText = pText;
			//pTempText->SetColor(QColor(65,226,218,255));
			//pTempText->SetFont(QFont(("����"), 10));

			QString tempStr;//��Ʒ��λ
			tempStr = QString(("ND"));
			//tempStr.sprintf(("%2.1f"), altitude);
			//pTempText->SetString(tempStr);	
			//pTempText->GetPositionArray()->push_back(QVector3D(StartPoint.x()-8,StartPoint.y()-8,0.51));
			//container->InsertRenderable(pTempText);

			pGeo->addDrawable(DrawText(tempStr.toStdString(), osg::Vec3(StartPoint.x(),StartPoint.y(),0.51),1000));	

		//	pTempLine = pLine++;
		/*	wxFont font(10,wxFONTFAMILY_SWISS,wxNORMAL,wxNORMAL);
			memDC.SetFont(font);
			memDC.SetBackgroundMode(wxTRANSPARENT);
			memDC.SetTextBackground(*wxWHITE);
			memDC.SetTextForeground(wxColour(8,0,4));
			memDC.DrawText(wxT("ND"),StartPoint.x-8,StartPoint.y-8);*/
		}
		else//���ݲ�Ϊ��ʱ��������
		{
			/*Line * pLine2 = new Line[46];
			pTempLine2 = pLine2;*/
			//�������ɫ
			//for(int i=0; i<10/*memberofcn*/; i++)
			//{
			//	//float a = cn[i][0];
			//	//float b = cn[i][1];

			//	//if((RMS >=a) && (RMS<=b))
			//	//{
			//	//	//pdhi.m_ColorHail = QVector4D(pColorTable->at(i)->nRed /255,pColorTable->at(i)->nGreen /255,pColorTable->at(i)->nBlue /255,1);
			//	//	//memDC.SetPen(wxPen(colorRibbon[i]));
			//	//}
			//}
			//if (RMS > 16)
			//{
			//	//memDC.SetPen(wxPen(wxColor(128, 0, 64)));
			//}

			CurrentRealPoint.setX(StartPoint.x()+ sin(Direction*M_PI/180.0)*LengthOfBarb);
			CurrentRealPoint.setY(StartPoint.y()  + cos(Direction*M_PI/180.0)*LengthOfBarb);
			CurrentPoint.setX((int)(CurrentRealPoint.x()));
			CurrentPoint.setY((int)(CurrentRealPoint.y()));

			CurrentRealHeel.setX(CurrentRealPoint.x() + dx);
			CurrentRealHeel.setY(CurrentRealPoint.y() + dy);
			CurrentHeel.setX((int)(CurrentRealHeel.x()));
			CurrentHeel.setY((int)(CurrentRealHeel.y()));

			if(ResidualLevel >= 20)
			{
				CurrentRealPeak.setX(CurrentRealPoint.x() + dx + sin((Direction+90)*M_PI/180.0)*Width);
				CurrentRealPeak.setY(CurrentRealPoint.y() + dy + cos((Direction+90)*M_PI/180.0)*Width);
				CurrentPeak.setX(( int ) ( CurrentRealPeak.x()) );
				CurrentPeak.setY(( int ) ( CurrentRealPeak.y()) );
			}
			else
			{
				CurrentRealPeak.setX(CurrentRealPoint.x() + sin((Direction + 90)*M_PI/180.0)*Width);
				CurrentRealPeak.setY(CurrentRealPoint.y() + cos((Direction + 90)*M_PI/180.0)*Width);
				CurrentPeak.setX((int)(CurrentRealPeak.x()));
				CurrentPeak.setY((int)(CurrentRealPeak.y()));
			}

			int iTriangle = 0; //�Ƿ񻭹�����
			int iLinear = 0;   //�Ƿ񻭹�����
			
			//QVector3DArray* pVectex1 = pTempLine->getVectexArray();
			//ColorAttribute* colorAttribute = new ColorAttribute;

			while (ResidualLevel >= 2)
			{
				if (ResidualLevel >= 20)//��������
				{
					//colorAttribute->Color(QVector4D(1.0, 1.0,1.0,255));

					osg::ref_ptr <osg::Vec3Array> temp_array0= new osg::Vec3Array  ;//��������
					osg::ref_ptr <osg::Vec4Array> temp_color_array0 = new osg::Vec4Array;//��ɫ����

					temp_array0->push_back(osg::Vec3(CurrentHeel.x(), CurrentHeel.y()+16, 0.51));
					temp_array0->push_back(osg::Vec3(CurrentPeak.x(), CurrentPeak.y()+16, 0.51));
					temp_color_array0->push_back(osg::Vec4(1.0, 1.0,1.0,255));
					pTempLine->SetBTransform(false);
					pTempLine->setGlenum(osg::PrimitiveSet::LINES);
					pTempLine->SetCoordinateTransform(layer->CoordTrans());
					pTempLine->SetOsgArray(temp_array0);
					pTempLine->SetOsgColorArray(*temp_color_array0);
					pTempLine->SetParentLayer(layer);
					pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

					pTempLine = ++pLine;
		

					//pTempLine->AttribSet()->insert(QE_ATTRI_COLOR, colorAttribute);
					//pVectex1->push_back(QVector3D(CurrentHeel.x(), CurrentHeel.y(), 0.51));
					//pVectex1->push_back(QVector3D(CurrentPeak.x(), CurrentPeak.y(), 0.51));
					//container->InsertRenderable(pTempLine);

					osg::ref_ptr <osg::Vec3Array> temp_array1= new osg::Vec3Array  ;//��������
					osg::ref_ptr <osg::Vec4Array> temp_color_array1 = new osg::Vec4Array;//��ɫ����

					temp_array1->push_back(osg::Vec3(CurrentPeak.x(), CurrentPeak.y(), 0.51));
					temp_array1->push_back(osg::Vec3(CurrentPoint.x(), CurrentPoint.y(), 0.51));
					temp_color_array1->push_back(osg::Vec4(1.0, 1.0,1.0,255));
					pTempLine->SetBTransform(false);
					pTempLine->setGlenum(osg::PrimitiveSet::LINES);
					pTempLine->SetCoordinateTransform(layer->CoordTrans());
					pTempLine->SetOsgArray(temp_array1);
					pTempLine->SetOsgColorArray(*temp_color_array1);
					pTempLine->SetParentLayer(layer);
					pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

					pTempLine = ++pLine;

					/*colorAttribute->Color(QVector4D(1.0, 1.0,1.0,255));
					pTempLine->AttribSet()->insert(QE_ATTRI_COLOR, colorAttribute);
					pVectex1->push_back(QVector3D(CurrentPeak.x(), CurrentPeak.y(), 0.51));
					pVectex1->push_back(QVector3D(CurrentPoint.x(), CurrentPoint.y(), 0.51));
					container->InsertRenderable(pTempLine);
					pTempLine = pLine++;*/


					//colorAttribute->Color(QVector4D(1.0, 1.0,1.0,255));
					//pTempLine->AttribSet()->insert(QE_ATTRI_COLOR, colorAttribute);
					//pVectex1->push_back(QVector3D(CurrentPoint.x(), CurrentPoint.y(), 0.51));
					//pVectex1->push_back(QVector3D(CurrentHeel.x(), CurrentHeel.y(), 0.51));
					//container->InsertRenderable(pTempLine);

					osg::ref_ptr <osg::Vec3Array> temp_array2= new osg::Vec3Array  ;//��������
					osg::ref_ptr <osg::Vec4Array> temp_color_array2 = new osg::Vec4Array;//��ɫ����

					temp_array2->push_back(osg::Vec3(CurrentPoint.x(), CurrentPoint.y(), 0.51));
					temp_array2->push_back(osg::Vec3(CurrentHeel.x(), CurrentHeel.y(), 0.51));
					temp_color_array2->push_back(osg::Vec4(1.0, 1.0,1.0,255));
					pTempLine->SetBTransform(false);
					pTempLine->setGlenum(osg::PrimitiveSet::LINES);
					pTempLine->SetCoordinateTransform(layer->CoordTrans());
					pTempLine->SetOsgArray(temp_array2);
					pTempLine->SetOsgColorArray(*temp_color_array2);
					pTempLine->SetParentLayer(layer);
					pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

					pTempLine = ++pLine;
					//memDC.DrawLine (CurrentHeel, CurrentPeak);
					//memDC.DrawLine (CurrentPeak, CurrentPoint);
					//memDC.DrawLine (CurrentPoint, CurrentHeel);
					ResidualLevel -= 20;
					iTriangle++;
				}
				else if (ResidualLevel >= 4)//������
				{
					/*colorAttribute->Color(QVector4D(1.0, 1.0,1.0,255));
					pTempLine->AttribSet()->insert(QE_ATTRI_COLOR, colorAttribute);
					pVectex1->push_back(QVector3D(CurrentPeak.x(), CurrentPeak.y(), 0.51));
					pVectex1->push_back(QVector3D(CurrentPoint.x(), CurrentPoint.y(), 0.51));
					container->InsertRenderable(pTempLine);*/

					osg::ref_ptr <osg::Vec3Array> temp_array2= new osg::Vec3Array  ;//��������
					osg::ref_ptr <osg::Vec4Array> temp_color_array2 = new osg::Vec4Array;//��ɫ����

					temp_array2->push_back(osg::Vec3(CurrentPeak.x(), CurrentPeak.y(), 0.51));
					temp_array2->push_back(osg::Vec3(CurrentPoint.x(), CurrentPoint.y(), 0.51));
					temp_color_array2->push_back(osg::Vec4(1.0, 1.0,1.0,255));
					pTempLine->SetBTransform(false);
					pTempLine->setGlenum(osg::PrimitiveSet::LINES);
					pTempLine->SetCoordinateTransform(layer->CoordTrans());
					pTempLine->SetOsgArray(temp_array2);
					pTempLine->SetOsgColorArray(*temp_color_array2);
					pTempLine->SetParentLayer(layer);
					pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

					pTempLine = ++pLine;
					//memDC.DrawLine(CurrentPeak, CurrentPoint);
					ResidualLevel -= 4;
					iLinear++;
				}
				else if (ResidualLevel >= 2)//���̺�
				{
					//colorAttribute->Color(QVector4D(1.0, 1.0,1.0,255));
					//pTempLine->AttribSet()->insert(QE_ATTRI_COLOR, colorAttribute);
					//pVectex1->push_back(QVector3D((CurrentPeak.x()+CurrentPoint.x())/2, (CurrentPeak.y()+CurrentPoint.y() )/2, 0.51));
					//pVectex1->push_back(QVector3D(CurrentPoint.x(), CurrentPoint.y(), 0.51));
					//container->InsertRenderable(pTempLine);

					osg::ref_ptr <osg::Vec3Array> temp_array2= new osg::Vec3Array  ;//��������
					osg::ref_ptr <osg::Vec4Array> temp_color_array2 = new osg::Vec4Array;//��ɫ����

					temp_array2->push_back(osg::Vec3((CurrentPeak.x()+CurrentPoint.x())/2, (CurrentPeak.y()+CurrentPoint.y() )/2, 0.51));
					temp_array2->push_back(osg::Vec3(CurrentPoint.x(), CurrentPoint.y(), 0.51));
					temp_color_array2->push_back(osg::Vec4(1.0, 1.0,1.0,255));
					pTempLine->SetBTransform(false);
					pTempLine->setGlenum(osg::PrimitiveSet::LINES);
					pTempLine->SetCoordinateTransform(layer->CoordTrans());
					pTempLine->SetOsgArray(temp_array2);
					pTempLine->SetOsgColorArray(*temp_color_array2);
					pTempLine->SetParentLayer(layer);
					pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

					pTempLine = ++pLine;
					//memDC.DrawLine((CurrentPeak.x+CurrentPoint.x)/2, (CurrentPeak.y+CurrentPoint.y )/2, CurrentPoint.x, CurrentPoint.y );
					ResidualLevel -= 2;
				}

				LastPoint = CurrentPoint;

				CurrentRealPoint.setX(CurrentRealPoint.x()-dx);
				CurrentRealPoint.setY(CurrentRealPoint.y()-dy) ;
				CurrentPoint.setX((int)(CurrentRealPoint.x()));
				CurrentPoint.setY((int)(CurrentRealPoint.y()));

				if(ResidualLevel >= 20)
				{
					CurrentRealPeak.setX(CurrentRealPeak.x()-dx);
					CurrentRealPeak.setY(CurrentRealPeak.y()-dy);
					CurrentPeak.setX((int)(CurrentRealPeak.x()));
					CurrentPeak.setY((int)(CurrentRealPeak.y()));

					CurrentRealHeel.setX(CurrentRealHeel.x()-dx);
					CurrentRealHeel.setY(CurrentRealHeel.y()-dy);
					CurrentHeel.setX((int)(CurrentRealHeel.x()));
					CurrentHeel.setY((int)(CurrentRealHeel.y()));
				}
				else if((ResidualLevel>=4) && (iTriangle!=0) && (iLinear==0))//���������������δ������
				{
					CurrentRealPeak.setX(CurrentRealPeak.x()- 2*dx);
					CurrentRealPeak.setY(CurrentRealPeak.y()- 2*dy);
					CurrentPeak.setX((int)(CurrentRealPeak.x()));
					CurrentPeak.setY((int)(CurrentRealPeak.y()));
				}
				else if((ResidualLevel>=4) && (iTriangle==0))//���δ����������
				{
					CurrentRealPeak.setX(CurrentRealPeak.x()-dx);
					CurrentRealPeak.setY(CurrentRealPeak.y()-dy);
					CurrentPeak.setX((int)(CurrentRealPeak.x()));
					CurrentPeak.setY((int)(CurrentRealPeak.y()));
				}
				else if( ResidualLevel>=2 )
				{
					CurrentRealPeak.setX(CurrentRealPeak.x()-dx);
					CurrentRealPeak.setY(CurrentRealPeak.y()-dy);
					CurrentPeak.setX((int)(CurrentRealPeak.x()));
					CurrentPeak.setY((int)(CurrentRealPeak.y()));
				}
				//Line * pLine1 = new Line[46];

				osg::ref_ptr <osg::Vec3Array> temp_array2= new osg::Vec3Array  ;//��������
				osg::ref_ptr <osg::Vec4Array> temp_color_array2 = new osg::Vec4Array;//��ɫ����

				temp_array2->push_back(osg::Vec3(LastPoint.x(), LastPoint.y(), 0.51));
				temp_array2->push_back(osg::Vec3(CurrentPoint.x(), CurrentPoint.y(), 0.51));
				temp_color_array2->push_back(osg::Vec4(1.0, 1.0,1.0,255));
				pTempLine->SetBTransform(false);
				pTempLine->setGlenum(osg::PrimitiveSet::LINES);
				pTempLine->SetCoordinateTransform(layer->CoordTrans());
				pTempLine->SetOsgArray(temp_array2);
				pTempLine->SetOsgColorArray(*temp_color_array2);
				pTempLine->SetParentLayer(layer);
				pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));
				pTempLine = ++pLine;
			} //end while
			osg::ref_ptr <osg::Vec3Array> temp_array2= new osg::Vec3Array  ;//��������
			osg::ref_ptr <osg::Vec4Array> temp_color_array2 = new osg::Vec4Array;//��ɫ����

			temp_array2->push_back(osg::Vec3(CurrentPoint.x(), CurrentPoint.y(), 0.51));
			temp_array2->push_back(osg::Vec3(StartPoint.x(), StartPoint.y(), 0.51));
			temp_color_array2->push_back(osg::Vec4(1.0, 1.0,1.0,255));
			pTempLine->SetBTransform(false);
			pTempLine->setGlenum(osg::PrimitiveSet::LINES);
			pTempLine->SetCoordinateTransform(layer->CoordTrans());
			pTempLine->SetOsgArray(temp_array2);
			pTempLine->SetOsgColorArray(*temp_color_array2);
			pTempLine->SetParentLayer(layer);
			pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));
			pTempLine = ++pLine;

		} //end if
		iOrdinate = iOrdinate + 16;//����������
	//	bmp.SaveFile(wxT("1.png"),wxBITMAP_TYPE_PNG);
	} //end for
	//memDC.SelectObject(wxNullBitmap);
}

void Draw_VWP_Product::CreatAttributeList( Layer* layer ,CProductRead * pProductData,QString fileName )
{
	proAttr *pAttr = new proAttr;
	cDrawProduct*pDrawProduct = (cDrawProduct*)pProductData;
	if (pDrawProduct)
	{
		pDrawProduct->ReadInfo(pAttr,0);
	}

	QString StrUnit;//��Ʒ��λ
	StrUnit = QString(tr("dBZ"));
	//JudgeProdctUnit(fileName,StrUnit);
	// fileName = QString(tr("VAD"));
	QString tempStr;


	QString StrStationName,strProName;
	GetStationNameAndPName(fileName,StrStationName,strProName);

	layer->InsertLayerAttribute(strProName.toStdString(),std::string("��Ʒ���ƣ�"),"String");
	layer->InsertLayerAttribute(StrStationName.toStdString(),std::string("�״�վ�㣺"),"String");

	QDate tDate(1969,12,31);
	tDate = tDate.addDays(static_cast<int>(pAttr->scan_date));
	tempStr = tDate.toString("yyyy-MM-dd");
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("��ɨ��ʼ���ڣ�"),"String");

	QTime tTime(0,0,0,0);
	tTime = tTime.addMSecs(pAttr->scan_time);
	tempStr = tTime.toString("hh:mm:ss");
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("��ɨ��ʼʱ�䣺"),"String");

	QDate tDateG(1969,12,31);
	tDateG = tDateG.addDays(pAttr->generation_date);
	tempStr = tDateG.toString("yyyy-MM-dd");
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("��Ʒ�������ڣ�"),"String");

	QTime tTimeG(0,0,0,0);
	tTimeG = tTimeG.addMSecs(pAttr->generation_time);
	tempStr = tTimeG.toString("hh:mm:ss");
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("��Ʒ����ʱ�䣺"),"String");

	tempStr = QString::number((float)pAttr->altitude*0.1,'f',2);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("���θ߶ȣ�"),"String");

	QString strdu = QString::number(pAttr->longitude/10000);
	int ntemp = (pAttr->longitude%10000);
	int fen = (int)ntemp*60*0.0001;
	int ns =(  ((double)(ntemp*0.0001*60)-(double)fen)*60     );
	QString strfen = QString::number(fen);
	QString strS = QString::number(ns);
	tempStr = QString("%1��%2��%3�� E").arg(strdu).arg(strfen).arg(strS);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("���ȣ�"),"String");

	strdu = QString::number(pAttr->latitude/10000);
	int temp = (int)(pAttr->latitude%10000);
	fen = temp*0.0001*60;
	strfen = QString::number(fen);
	ns = (int)(  ((double)(temp*0.0001*60)-(double)fen)*60     );
	strS = QString::number(ns);
	tempStr = QString("%1��%2��%3�� N").arg(strdu).arg(strfen).arg(strS);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("γ�ȣ�"),"String");

	//tempStr = QString(tr("ά��"));
	//if (pAttr->operation_mode == 1)
	//{
	//	tempStr = QString(tr("���"));
	//}		
	//if (pAttr->operation_mode == 2)
	//{
	//	tempStr = QString(tr("��ˮ"));
	//}
	//propertylist.push_back(QString(tr("����ģʽ��%1")).arg(tempStr));

	/*double nwidth = GetStructProductRadius(0)/1000.0;
	tempStr = QString::number(nwidth,'f',3);
	propertylist.push_back(QString(tr("��Ч��Χ��%1Km")).arg(tempStr));*/

	//VWP_ELEMENTS * pHead = (VWP_ELEMENTS*)(pProductData->GetDataOfBlockStruct(0,0)); //����ͷ��Ϣ
	//tempStr = QString::number(pHead->r);
 //   propertylist.push_back(QString(tr("��Ч��Χ��%1Km")).arg(tempStr));

	if (pAttr->vcp_mode == 0)
	{
		tempStr =QString(tr("VCP 11"));
	}
	else if (pAttr->vcp_mode == 1)
	{
		tempStr =QString(tr("VCP 21"));
	}
	else if (pAttr->vcp_mode == 2)
	{
		tempStr =QString(tr("VCP 31"));
	}
	else if (pAttr->vcp_mode == 3)
	{
		tempStr =QString(tr("VCP 32"));
	}
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("��ɨģʽ��"),"String");

	tempStr = QString::number(0);
    tempStr = QString(tr("%1deg %2Km")).arg(tempStr).arg(tempStr);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("���ģ�"),"String");

	VWP_ELEMENTS * pHead = (VWP_ELEMENTS*)(pProductData->GetDataOfBlockStruct(0,0)); //����ͷ��Ϣ
	tempStr = QString::number(pHead->RMS);

	layer->InsertLayerAttribute(QString("%1km").arg(tempStr).toStdString(),std::string("���پ������"),"String");
	//tagBoundFileHeader * pHead = (tagBoundFileHeader*)(pProductData->GetDataOfBlockStruct(0,0)); //����ͷ��Ϣ
	//tempStr = QString::number(pHead->ikuchang);
	//propertylist.push_back(QString(tr("����Ȧ��%1m")).arg(tempStr));

	//m_fileDeclaration.push_back(StrUnit);
	delete pAttr;
	pAttr = NULL;
}

void Draw_VWP_Product::GetStationNameAndPName(QString &fileName,QString &stationName,
	QString &productName)
{
	QStringList stationInfo = FileNameAnalyes(fileName);
	CRadReadStationFile pRead ;
	QString appConfigPth = QCoreApplication::applicationDirPath().replace("/bin","") + "/config/";
	pRead.ReadFile(appConfigPth+"CINRAD_classB.dat");
	RADSTATION_HASH stationHash = pRead.RadStaion();
	if (stationInfo.size()<2)
	{
		return;
	}
	QString id = stationInfo.at(0);
	P_CINRAD_STATIONINFO info = stationHash.value(id);
	if (info == 0)
	{
		stationName = "";
	}
	else
	{
		stationName = info->cName;
	}
	//productName = GetProductNameByShortName(stationInfo.at(1));
	QMap<QString,QString> ProductNameMap;
	ProductNameMap.insert(QString(tr("VWP")),QString(tr("VAD������(VWP)")));
	productName = ProductNameMap.value(stationInfo.at(1));
}
QStringList Draw_VWP_Product::FileNameAnalyes( QString fName )
{
	QStringList attribute = QDir(fName).dirName().split("_");
	QString str_time = attribute[3].left(attribute[3].indexOf("."));
	attribute[2].append(str_time);
	return attribute;
}
QImage Draw_VWP_Product::DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize)
{
	void * p = lpProduct->GetDataOfBlockStruct(0, 0);
	int maxdis = 650;
	float rule = maxdis/95.0;
	int image_size = maxdis/rule;
	QImage image(image_size,image_size, QImage::Format_RGB32);
	return image;
}

void Draw_VWP_Product::Get_Scan_Time( int &hour, int &minute, int iProductTime )
{
	gInt32 tmpTime = iProductTime;
	gInt32 tmpHour = 0;
	gInt32 tmpMin = 0;
	gInt32 second = 0;
	gInt32 msecond = 0;
	MilliSecond2Time(tmpTime, tmpHour, tmpMin, second, msecond); 
	hour = tmpHour;
	minute = tmpMin;
	
}

bool Draw_VWP_Product::MilliSecond2Time( gInt32 MilliSecond,gInt32& T_Hour,gInt32& T_Minute,gInt32& T_Second,gInt32& T_MilliSecond ) 
{
	// ����תʱ��
	if( MilliSecond < 0 || MilliSecond >= ( 24 * 3600 * 1000 ) ) 
	{
		return false;						// ����ʱ�䷶Χ
	}
	T_Hour = gInt32( MilliSecond / 3600 / 1000 );
	MilliSecond -= T_Hour * 3600 * 1000;
	T_Minute = gInt32( MilliSecond / 60 / 1000 );
	MilliSecond -= T_Minute * 60 * 1000;
	T_Second = gInt32( MilliSecond / 1000 );
	T_MilliSecond = gInt32( MilliSecond - T_Second * 1000 );	
	return true;
}
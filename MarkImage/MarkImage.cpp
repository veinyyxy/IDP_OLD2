// MarkImage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "GeosOnTheOsg.h"
#include "DataInterface.h"
#include <osgViewer/Viewer>
#include <osg/ArgumentParser>
#include "Proj4Transform.h"
#include <osg/Camera>
#include <osgDB/WriteFile>
#include <QtCore/QSize>
#include <QtGui/QPixmap>

void ScreenToWorld(const osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out)
{
	osg::Matrix VPW = pCamera->getViewMatrix() *
		pCamera->getProjectionMatrix() *
		pCamera->getViewport()->computeWindowMatrix();
	osg::Matrix inverseVPW;
	inverseVPW.invert(VPW);
	v3Out = v3In  * inverseVPW;
	v3Out.set(v3Out.x(), 0, v3Out.z());
}

class PostDrawCallback : public osg::Camera::DrawCallback
{
public:
	goto_gis::CoordinateTransform* pCT;
	osg::Vec3* m_plt;
	//osg::Vec3* m_plb;
	//osg::Vec3* m_prt;
	osg::Vec3* m_prb;
	osgViewer::Viewer* m_pViewer;
	osg::Vec4* m_BackColor;
	std::string* m_outPath;

	virtual void operator()(const osg::Camera &camera) const
	{
		static bool bClip = true;

		if(bClip)
		{
			int x, y, width, height;
			x = camera.getViewport()->x();
			y = camera.getViewport()->y();
			width = camera.getViewport()->width();
			height = camera.getViewport()->height();

			osg::ref_ptr<osg::Image>image = new osg::Image;
			image->allocateImage( width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE);
			image->readPixels( 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE);
			//osgDB::writeImageFile( *(image.get()), "D:\\cc.png" );

			QImage newPixmap(width, height, QImage::Format_ARGB32);
			newPixmap.fill(QColor(0, 0, 0, 0));

			for(int i = 0; i < width; i++)
			{
				for(int j = 0; j < height; j++)
				{
					osg::Vec4 color = image->getColor(osg::Vec2(abs(width - i), j));

					//osg::Vec4 mywhite(1, 1, 1, 1);
					
					
					if(color != *m_BackColor)
					{
						QColor imageColor(255.0*color.r(), 255.0*color.g()
							, 255.0*color.b(), 255.0*color.a());
						newPixmap.setPixel(i, j, imageColor.rgba());
					}
				}
			}
			newPixmap.save(QString::fromStdString(*m_outPath));
			
			osg::Vec3 leftTop(x, height, 0), /*leftBottom(x, y, 0), rightTop(x, y, 0), */rightBottom(width, y, 0);
			osg::Vec3 leftTopLonLat, /*leftBottomLonLat, rightTopLonLat, */rightBottomLonLat;
			ScreenToWorld(&camera, leftTop, leftTopLonLat);
			//ScreenToWorld(&camera, leftBottom, leftBottomLonLat);
			//ScreenToWorld(&camera, rightTop, rightTopLonLat);
			ScreenToWorld(&camera, rightBottom, rightBottomLonLat);

			goto_gis::Proj4Transform proj4;
			proj4.setSourceCRS(pCT->getDesCRS());
			proj4.setDestCRS(pCT->getSourceCRS());

			*m_plt = proj4.Transform(leftTopLonLat);
			//*m_plb = proj4.Transform(leftBottomLonLat);
			//*m_prt = proj4.Transform(rightTopLonLat);
			*m_prb = proj4.Transform(rightBottomLonLat);

			bClip = false;

			m_pViewer->setDone(true);
		}
		
	}
};

class ImageHandler:public osgGA::GUIEventHandler
{
public:
	goto_gis::CoordinateTransform* pCT;
	ImageHandler(){m_Draw = true;};
	~ImageHandler(){};
	bool m_Draw;
	bool handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa )
	{
		osgViewer::Viewer* viewer =
			dynamic_cast<osgViewer::Viewer*>(&aa);
		if (!viewer)
			return false;
		switch(ea.getEventType())
		{
		case(osgGA::GUIEventAdapter::KEYDOWN):
			{
				if(ea.getKey() == 'c')
				{
					
					//////////////////////////////////////////////////////////////////////////
					
					
					//////////////////////////////////////////////////////////////////////////
				}
			}
		default:
			return false;
		}
	}
};



int _tmain(int argc, _TCHAR* argv[])
{
#if 0
	QImage pixmap("D:\\cc.png");
	int iWidth = pixmap.width();
	int iHeight = pixmap.height();

	QImage newPixmap(iWidth, iHeight, QImage::Format_ARGB32);
	newPixmap.fill(QColor(0, 0, 0, 0));

	for(int i = 0; i < iWidth; i++)
	{
		for(int j = 0; j < iHeight; j++)
		{
			QRgb rgb = pixmap.pixel(i, j);

			QColor mywhite(255, 255, 255, 255);
			QColor color = QColor::fromRgba(rgb);
			if(color != mywhite)
				newPixmap.setPixel(i, j, color.rgba());
		}
	}
	
	newPixmap.save("D:\\bbb.png");
#else
	osg::ArgumentParser args(&argc, argv);

	if(args.argc() < 8)
	{
		printf("-src \r\n -dst  \r\n-ctl \r\n-grd \r\n-back \r\n-bc \r\n-size \r\n-out ");
		return 0;
	}
	std::string strSrc, strDst, strCtl, strGrd;
	osg::ArgumentParser::Parameter parameterSrc(strSrc);
	osg::ArgumentParser::Parameter parameterDst(strDst);
	osg::ArgumentParser::Parameter parameterCtl(strCtl);
	osg::ArgumentParser::Parameter parameterGrd(strGrd);

	args.read("-src", parameterSrc);
	args.read("-dst", parameterDst);
	args.read("-ctl", parameterCtl);
	args.read("-grd", parameterGrd);
	
	int iBack = 0;
	int iRed = 0, iGreen = 0, iBlue = 0, iAhpha = 0;
	osg::ArgumentParser::Parameter parameterBack(iBack);
	osg::ArgumentParser::Parameter parameterRed(iRed);
	osg::ArgumentParser::Parameter parameterGreen(iGreen);
	osg::ArgumentParser::Parameter parameterBlue(iBlue);
	osg::ArgumentParser::Parameter parameterAhpha(iAhpha);
	args.read("-back", parameterBack);
	args.read("-bc", parameterRed, parameterGreen, parameterBlue, parameterAhpha);

	int iX = 0, iY = 0, iWidth = 0, iHeight = 0;
	osg::ArgumentParser::Parameter parameterX(iX);
	osg::ArgumentParser::Parameter parameterY(iY);
	osg::ArgumentParser::Parameter parameterWidth(iWidth);
	osg::ArgumentParser::Parameter parameterHeight(iHeight);
	args.read("-size",  parameterX, parameterY, parameterWidth, parameterHeight);

	std::string strOutDir;
	osg::ArgumentParser::Parameter parameterout(strOutDir);
	args.read("-out", parameterout);

	//////////////////////////////////////////////////////////////////////////
	goto_gis::GeosOnTheOsg* pGeosOnTheosg = goto_gis::GeosOnTheOsg::Instantiate();
	pGeosOnTheosg->LoadProviderPlugins("PrividerConfig.txt");
	pGeosOnTheosg->LoadGisExtendPlugins("GisExtendPlugins");
	goto_gis::VectorLayer* pLayer = new goto_gis::VectorLayer;
	pLayer->LayerZ(0);
	pGeosOnTheosg->OpenDataAsLayer("display.grads",pLayer);
	DataInterface* pDI = pDI = (DataInterface*)(pLayer->GetDataProvider()->GetExtendInterface(0));

	std::vector<float> levelv;
	
	pDI->DataInput(strCtl, strGrd);
	pDI->Draw(1, iBack, "display a");

	goto_gis::Proj4Transform proj4Transform;
	//"+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs"
	proj4Transform.setSourceCRS(strSrc);
	//"+proj=eqc +lat_ts=0 +lat_0=-3 +lon_0=180 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs"
	proj4Transform.setDestCRS(strDst);
	pLayer->CoordTrans(&proj4Transform);
	pLayer->CreateLayerData();

	osgViewer::Viewer viewer;
	viewer.setLightingMode(osgViewer::View::NO_LIGHT);

	osg::Node* pNode = pLayer->GetLayerNode();
	osg::StateSet* stateset = pNode->getOrCreateStateSet();
	//Alpha混合开启
	stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
	//取消深度测试
	stateset->setMode( GL_DEPTH_TEST, osg::StateAttribute::OFF  );
	stateset->setMode( GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED );

	viewer.setSceneData(pNode);
	osg::Camera* pCamera = viewer.getCamera();
	
	osg::Vec4 backColor((float)iRed / 255.0, (float)iGreen / 255.0
		,  (float)iBlue/ 255.0,  (float)iAhpha/ 255.0);
	pCamera->setClearColor(backColor);

	viewer.setUpViewInWindow(iX, iY, iWidth, iHeight);

	osg::Vec3 lt, lb, rt, rb;
	//viewer.addEventHandler(new ImageHandler);
	PostDrawCallback* pPDC = new PostDrawCallback;
	pPDC->pCT = pLayer->CoordTrans();
	pPDC->m_pViewer = &viewer;
	pPDC->m_plt = &lt;
	//pPDC->m_plb = &lb;
	//pPDC->m_prt = &rt;
	pPDC->m_prb = &rb;

	pPDC->m_BackColor = &backColor;
	pPDC->m_outPath = &strOutDir;
	pCamera->setPostDrawCallback(pPDC);

	viewer.run();

#endif

	std::string strProjFile = ".prf";
	strProjFile = strOutDir + strProjFile;
	FILE* textFile = fopen(strProjFile.c_str(), "w+");
	char buffer[1024] = {0};
	sprintf(buffer, "%.3f, %.3f\r\n%.3f,%.3f\r\n", lt.x(), lt.y(), rb.x(), rb.y());
	//std::string strText =
	size_t slen = strlen(buffer);
	fwrite(buffer, 1, slen, textFile);
	fclose(textFile);
	return 0;
}


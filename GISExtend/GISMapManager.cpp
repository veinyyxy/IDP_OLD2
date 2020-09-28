#include <QtCore/QString>
#include <QtGui/QFont>
#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QUuid>
#include <QtCore/QDir>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtCore/QCoreApplication>
#include <QtGui/QMessageBox>

#include "Layer.h"
#include "Map.h"
#include "DataProvider.h"
#include "VectorLayer.h"
#include "GraticuleCoordinates.h"
#include "Proj4Transform.h"
#include "GeosOnTheOsg.h"

using namespace goto_gis;

#include "GISMapManager.h"



GISMapManager::GISMapManager(Map* pMap) : m_pMap(pMap)
{
	m_pProj4 = new Proj4Transform();
	m_pProj4->setSourceCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
	m_pProj4->setSourceProjName("无投影");
	////proj4->setDestCRS("+proj=eqc +lat_ts=0 +lat_0=-3 +lon_0=180 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs");
	m_pProj4->setDestCRS("+proj=lcc +lat_1=20 +lat_2=60 +lat_0=0 +lon_0=103 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs");
	m_pProj4->setDesProjName("雷达中心兰勃特投影");
	m_pMap->SetCoordinateTransform(m_pProj4);

	QString iniPath = QCoreApplication::applicationDirPath()+tr("/Config/Config.ini");
	QSettings *configIni = new QSettings(iniPath, QSettings::IniFormat); 
	m_LonStart = configIni->value("/FishNet/LonStart").toFloat();
	m_LatStart = configIni->value("/FishNet/LatStart").toFloat();
	m_LonEnd = configIni->value("/FishNet/LonEnd").toFloat();
	m_LatEnd = configIni->value("/FishNet/LatEnd").toFloat();
	m_LonCellSize = configIni->value("/FishNet/LonCellSize").toFloat();
	m_LatCellSize = configIni->value("/FishNet/LatCellSize").toFloat();

	delete configIni;
	configIni = 0;
}

GISMapManager::~GISMapManager(void)
{
}
#if 0
bool GISMapManager::AddWorkStationLayer()
{
	typedef QObject* (*ClsFactory)(QString  Name,DBManager::EnumDBType dbType);
	ClsFactory avg = (ClsFactory) QLibrary::resolve("DBManager.dll", "CreateInstance");
	SqliteManager* pSqliteManager =(SqliteManager*)avg("MDMS_DBManager",DBManager::EnumDBType::Sqlite);
	QList<QList<QString>>  lists = pSqliteManager->QueryRadarListDB(tr("r_work_station"));
	
	VectorLayer* workStationLayer = new VectorLayer();
	workStationLayer->SetParentMap(m_pMap);
	workStationLayer->CoordTrans(m_pMap->GetCoordinateTransform());
	for (int i = 0 ; i< lists.count() ; i++)
	{
	
		QString name = lists.at(i).at(0);
		double x = lists.at(i).at(1).toDouble();
		double y = lists.at(i).at(2).toDouble();
		double z = 0.5;
		GeomField geomFiled;
		geomFiled.setName("NAME");
		geomFiled.setComment(name.toStdString());
		geomFiled.setX(x);
		geomFiled.setY(y);
		geomFiled.setZ(z);
		workStationLayer->GetFieldMap()->insert(FIELD_MAPPair(i,geomFiled));
	}
	if (m_pMap)
	{
		m_pMap->AddLayer("炮点图层",workStationLayer);
	}
	workStationLayer->FontSize(9000);
	workStationLayer->ShowLabelText();
	return true;
}
#endif
bool GISMapManager::ReadMapScheme(const QString filename, Map* pMap)
{
	GeosOnTheOsg* pGeosOsg = GeosOnTheOsg::Instantiate();

	QString schemeName, layerName, projectName, shpName, schemeDir, projsrc, projdst, proname, areaName;
	QString showFiedName;
	QList<QString> BasisLayer;
	qreal layerZ, isToRender, notToRender;
	int hasControler, isCheck;
	UINT layerColor = 0;
	int layerLineFator = 0/*, polygonType*/;
	float  layerLineWidth = 0.0;
	short layerLinePatter = 0;
	qreal mapScale = 0, qrX = 0, qrY = 0;
	QMatrix4x4 transMove;
	QColor bgColor, textColor;
	QFont tf;
	QFile file(filename);
	if(file.open(QFile::ReadOnly))
	{
		int i = 0, j = 0;
		
		unsigned int nbgColor;
		QDomDocument XmlReader;
		XmlReader.setContent(&file);

		QDomNodeList nodeList = XmlReader.elementsByTagName(tr("map_project_configure"));

		while (nodeList.count() > i)
		{
			QDomNode domNode = nodeList.at(i);
			QDomElement map_project_configure_element =  domNode.toElement();

			schemeName = map_project_configure_element.attribute("style_name");
			projectName = map_project_configure_element.attribute("proj_name");
			areaName = map_project_configure_element.attribute("are_name");
			mapScale = map_project_configure_element.attribute("scale").toFloat();
			nbgColor = map_project_configure_element.attribute("background_color").toUInt();
			bgColor.setRgba(nbgColor);
			qrX = map_project_configure_element.attribute("Xtrans").toFloat();
			qrY = map_project_configure_element.attribute("Ytrans").toFloat();
			i++;
		}
		pMap->BackgroundColor(nbgColor);
		i = 0;
		nodeList = XmlReader.elementsByTagName(tr("Info"));

		for(;nodeList.count() > i; i++)
		{
			QDomNode domNode = nodeList.at(i);
			QDomElement Info_element =  domNode.toElement();

			layerName = Info_element.attribute("layer_name");
			shpName = Info_element.attribute("shpname");
			layerZ = Info_element.attribute("layerz").toFloat();
			hasControler = Info_element.attribute("hascontroler").toInt();
			isToRender = Info_element.attribute("isrender").toDouble();
			notToRender = Info_element.attribute("notrender").toDouble();
			isCheck = Info_element.attribute("checked").toInt();
			unsigned int  layerColor = Info_element.attribute("layer_color").toUInt();
			layerLineFator = Info_element.attribute("layer_line_factor").toInt();
			layerLinePatter = Info_element.attribute("layer_line_patter").toShort();
			layerLineWidth = Info_element.attribute("layer_line_width").toInt();
			showFiedName = Info_element.attribute("showFiedName");

			tf.setFamily(Info_element.attribute("layer_text_font_famliy"));
			int fontSize = Info_element.attribute("layer_text_font_point_size").toInt();
			tf.setPointSize(fontSize);
			tf.setBold(Info_element.attribute("layer_text_font_bold").toInt());
			tf.setItalic(Info_element.attribute("layer_text_font_italic").toInt());
			tf.setUnderline(Info_element.attribute("layer_text_font_under_line").toInt());
			tf.setStrikeOut(Info_element.attribute("layer_text_font_strike_out").toInt());
			tf.setKerning(Info_element.attribute("layer_text_font_kerning").toInt());

			unsigned int urgba = Info_element.attribute("layer_text_color").toUInt();
			if(urgba)
				textColor = QColor::fromRgba(urgba);
			else
				textColor.setAlpha(255);
			
			Layer* pNewLayer = 0;
			QString fileSuffix = QFileInfo(shpName).suffix();
			if("shp" == fileSuffix)
			{

				/*typedef DataProvider* (*pDataProviderPluginMain)();
				DataProvider* pDataProvider = 0;
				QLibrary libQGISVectorProvider("GISDBEngine.dll");
				pDataProviderPluginMain GetProvider = (pDataProviderPluginMain)(libQGISVectorProvider.resolve("DataProviderPluginMain"));
				pDataProvider = GetProvider();

				pNewLayer = new VectorLayer(pDataProvider,m_pProj4);*/
				pNewLayer = NewVectorLayer(shpName, layerZ);

				if(!pNewLayer) continue;
				pNewLayer->CoordTrans(m_pProj4);
				pNewLayer->FontSize(fontSize);
				pNewLayer->GeometrySize(layerLineWidth);
				pNewLayer->ShowFiedName(showFiedName.toStdString());
				pNewLayer->TextColor(urgba);
				if(isCheck > 0)
					pNewLayer->Visible(true);
				else
					pNewLayer->Visible(false);

				pNewLayer->SetParentMap(pMap);
				pNewLayer->SetLayerName(layerName.toStdString());
				pGeosOsg->OpenDataAsLayer(shpName.toStdString(),pNewLayer);
			}
			else if("tif" == fileSuffix)
			{
				pNewLayer = NewRasterLayer(shpName, layerZ);
			}

			if (pNewLayer == 0)
			{
				QMessageBox::critical(0,tr("警告"),QCoreApplication::applicationDirPath() + shpName+tr("图层装载失败，请检查配置！"));
				exit(0);
			}
			
			if (layerColor)
			{
				QColor color = QColor::fromRgba(layerColor);
				osg::Vec4 vect4(color.redF(),color.greenF(),color.blueF(),color.alphaF());
				pNewLayer->SetLineWidth(layerLineWidth);
				pNewLayer->SetLineStripple(layerLineFator, layerLinePatter);
				pNewLayer->SetLayerColor(vect4);
			}
			if(pNewLayer == 0)
			{
				continue;
			}
			pMap->AddLayer(layerName.toStdString(),pNewLayer);
		}
		
		nodeList = XmlReader.elementsByTagName(tr("proj_info"));
		i = 0;
		while (nodeList.count() > i)
		{
			QDomNode domNode = nodeList.at(i);
			QDomElement proj_info_element =  domNode.toElement();
			projsrc = proj_info_element.attribute("src");
			projdst = proj_info_element.attribute("dst");
			proname = proj_info_element.attribute("name");
			i++;
		}

		nodeList = XmlReader.elementsByTagName(tr("BasisMap"));
		i = 0;
		
		while (nodeList.count() > i)
		{
			QDomNode domNode = nodeList.at(i);
			QDomElement proj_info_element =  domNode.toElement();
			BasisLayer = proj_info_element.attribute("BasisLayer").split(",",QString::SkipEmptyParts);
			i++;
		}
		pMap->BasisLayerList(BasisLayer);
		if (pMap->GetCoordinateTransform() == NULL)
		{
			m_pProj4 = new Proj4Transform();
			pMap->SetCoordinateTransform(m_pProj4);
		}
		pMap->GetCoordinateTransform()->setSourceCRS(projsrc.toStdString());
		pMap->GetCoordinateTransform()->setSourceProjName("无投影");
		pMap->GetCoordinateTransform()->setDestCRS(projdst.toStdString());
		pMap->GetCoordinateTransform()->setDesProjName("雷达中心兰勃特投影");
	//	pMap->CreateMapData();


	}
	file.close();
	//AddWorkStationLayer();
	return true;
}

void GISMapManager::WriteMapScheme( const QString schName, const QString filename )
{
#if 0
	QDomDocument XmlWriter;
	qreal mapScale = 0, qrX = 0, qrY = 0;
	QTreeWidget * pTreeWidget = this->getMainTree();
	QTreeWidgetItem* pTreeItem = pTreeWidget->invisibleRootItem();
	QString strItem = pTreeItem->child(0)->text(0);
	//////////////////////////////////////////////////////////////////////////
	mapScale = GetCurrentMapView()->GetCIPScene()->GetObserver("MainObserver")->Camera()->GetCameraScale();
	qrX = GetCurrentMapView()->GetCIPScene()->GetSceneTransform()->Tranlate().column(3).x();
	qrY = GetCurrentMapView()->GetCIPScene()->GetSceneTransform()->Tranlate().column(3).y();

	QDomElement map_project_configure_element = XmlWriter.createElement(QObject::tr("map_project_configure"));
	if(m_pSchemeComBox)
		map_project_configure_element.setAttribute("style_name", m_pSchemeComBox->currentText());
	else
		map_project_configure_element.setAttribute("style_name", tr("无"));

	map_project_configure_element.setAttribute("proj_name", strItem);
	map_project_configure_element.setAttribute("are_name", "");
	map_project_configure_element.setAttribute("background_color", QString::number(m_pCurrentMapView->GetCIPScene()->BackgroundColor().rgba()));
	map_project_configure_element.setAttribute("scale", QString("%1").arg(mapScale));
	map_project_configure_element.setAttribute("Xtrans", QString("%1").arg(qrX));
	map_project_configure_element.setAttribute("Ytrans", QString("%1").arg(qrY));
	//////////////////////////////////////////////////////////////////////////
	QMap<QString, QString> nameList;
	QMap<QString, bool> IsCheckSet;
	GetAllChild2(pTreeItem, nameList, IsCheckSet);
	//QMap<QString, LAYER_INFO> * LayerList = m_pCurrentMapView->GetGisLayerInfo();
	QMap<QString, QString>::Iterator it;
	QString strGuid  = "";
	for(it = nameList.begin(); it != nameList.end(); it++)
	{
		QString strKey = it.key();
		QString strValue = it.value();
		Layer* pL = gismap->GetLayer(strValue);
		if(!pL) continue;
		if(it.key() == tr("网格地图")) continue;

		QDomElement InfoElement = XmlWriter.createElement(QString("Info"));
		QVector4D v4C;
		QColor lc;

		InfoElement.setAttribute("layer_name", it.key());
		InfoElement.setAttribute("shpname", pL->FileName());
		InfoElement.setAttribute("layerz", QString::number(pL->LayerZ()));
		v4C = pL->getColor();
		lc.setRgbF(v4C.x(), v4C.y(), v4C.z(), v4C.w());
		InfoElement.setAttribute("layer_color", QString::number(lc.rgba()));
		InfoElement.setAttribute("layer_line_factor", QString::number(pL->getGLintStipple()));
		InfoElement.setAttribute("layer_line_patter", QString::number(pL->getShortStipple()));
		InfoElement.setAttribute("layer_line_width", QString::number(pL->getWidth()));
		QFont tf = pL->GetTextFont();
		InfoElement.setAttribute("layer_text_font_famliy", tf.family());
		InfoElement.setAttribute("layer_text_font_point_size", tf.pointSize());
		InfoElement.setAttribute("layer_text_font_bold", tf.bold());
		InfoElement.setAttribute("layer_text_font_italic", tf.italic());
		InfoElement.setAttribute("layer_text_font_under_line", tf.underline());
		InfoElement.setAttribute("layer_text_font_strike_out", tf.strikeOut());
		InfoElement.setAttribute("layer_text_font_kerning", tf.kerning());
		InfoElement.setAttribute("layer_text_color", pL->GetTextColor().rgba());
		InfoElement.setAttribute("plolygontype", QString::number(pL->GetPolygonType()));
		if(IsCheckSet[it.value()])
			InfoElement.setAttribute("checked", "1");
		else
			InfoElement.setAttribute("checked", "0");

		CLayerDisplayControl* pLayerDispalyContrl = dynamic_cast<CLayerDisplayControl*>(pL->GetNodeCallBack());
		if(pLayerDispalyContrl)
		{

			InfoElement.setAttribute("hascontroler", tr("1"));
			double dV = pLayerDispalyContrl->GetToRender();
			QString strNum = QString::number(dV);
			InfoElement.setAttribute("isrender", strNum);
			dV = pLayerDispalyContrl->GetNotRender();
			strNum = QString::number(dV);
			InfoElement.setAttribute("notrender", strNum);
		}
		else
		{
			InfoElement.setAttribute("hascontroler", tr("0"));
			InfoElement.setAttribute("isrender", tr("0"));
			InfoElement.setAttribute("notrender", tr("0"));
		}

		map_project_configure_element.appendChild(InfoElement);
	}

	//////////////////////////////////////////////////////////////////////////
	QDomElement proj_info_element;
	proj_info_element =  XmlWriter.createElement("proj_info");

	proj_info_element.setAttribute("src", QString(GetCurrentMapView()->GetMap()->GetCoordinateTransform()->getSourceCRS()));
	proj_info_element.setAttribute("dst", QString(GetCurrentMapView()->GetMap()->GetCoordinateTransform()->getDesCRS()));
	proj_info_element.setAttribute("name", QString(m_pCurrentMapView->GetLocalTC()));
	map_project_configure_element.appendChild(proj_info_element);

	XmlWriter.appendChild(map_project_configure_element);
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
		return ;
	QTextStream out(&file);
	out.setCodec("UTF-8");
	XmlWriter.save(out,4, QDomNode::EncodingFromTextStream);
	file.close();
#endif
}

Layer* GISMapManager::NewVectorLayer(const QString& fileName, qreal qrZ)
{
	QString backupFilePathName = fileName;
	QString backupFileName;
	QFileInfo fileInfo(backupFilePathName);

	if(!fileInfo.exists())
	{
		QString searchPath = QCoreApplication::applicationDirPath() + "/GEO/shp/";
		backupFileName = fileInfo.fileName();
		QDir seachDir(searchPath);

		if(seachDir.exists(backupFileName))
		{
			backupFilePathName = searchPath + backupFileName;
		}
		else
			return 0;
	}
	//QString guid = QUuid::createUuid().toString();
	//QSettings setting2("ProjectionType.ini", QSettings::IniFormat);
	//QString srcproj = setting2.value("/GeographicCoordinate_CONFIG/WGS84").toString();

	Layer* pLayer = new VectorLayer;
	
	if(!pLayer) return 0;

	pLayer->LayerZ(qrZ);

	return pLayer;
}

void GISMapManager::LoadBasicMap( const QString filename )
{
	ReadMapScheme(filename, m_pMap);
}

/** 
  * @brief    绘制经纬网格     
  * @author  陈志学   
  * @param [in]  lonstart 经度起始,latstart纬度起始 ,lonend 经度截止,latend纬度截止， lonCellSize 经度分辨率,LatCellSize纬度分辨率
  * @param [out] parameter name   parameter description
  * @return   return data
  * @note     detailed description 修改人  修改时间  修改内容
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
void GISMapManager::AddFishNet( float lonstart,float latstart,float lonend,float latend,float lonCellSize,float LatCellSize ,qreal qrz )
{
	Layer *newLayer = new QGraticuleCoordinates(0,m_pMap->GetCoordinateTransform());
	dynamic_cast<QGraticuleCoordinates*>(newLayer)->SetGrid( lonstart, latstart, lonend, latend, lonCellSize, LatCellSize);
	newLayer->LayerZ(qrz);
	newLayer->UseLayerZ(false);
	newLayer->CreateLayer("FishNet");
	newLayer->SetParentMap(m_pMap);
	newLayer->CoordTrans(m_pMap->GetCoordinateTransform());

	osg::StateSet* pSateSet = newLayer->GetLayerNode()->getOrCreateStateSet();
	pSateSet->setRenderBinDetails(2, "FishNet");
	pSateSet->setMode(GL_BLEND,osg::StateAttribute::ON);
	pSateSet->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON);
	m_pMap->AddLayer("地理网格",newLayer);

	//Layer *newLayer1 = new QGraticuleCoordinates(0,m_pProj4);
	//dynamic_cast<QGraticuleCoordinates*>(newLayer1)->SetGrid( lonstart, latstart, lonend, latend, lonCellSize, LatCellSize);
	//newLayer1->LayerZ(qrz+0.1);
	//newLayer1->CreateLayer("Frame1");
	//newLayer1->SetParentMap(m_pMap);
	//newLayer1->CoordTrans(m_pMap->GetCoordinateTransform());

	//osg::StateSet* pSateSet1 = newLayer1->GetLayerNode()->getOrCreateStateSet();
	//pSateSet1->setRenderBinDetails(2, "Frame1");
	//pSateSet1->setMode(GL_BLEND,osg::StateAttribute::ON);
	//pSateSet1->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON);
	//m_pMap->AddLayer("网格边框1",newLayer1);

	//Layer *newLayer2 = new QGraticuleCoordinates(0,m_pProj4);
	//dynamic_cast<QGraticuleCoordinates*>(newLayer2)->SetGrid( lonstart, latstart, lonend, latend, lonCellSize, LatCellSize);
	//newLayer2->LayerZ(qrz+0.2);
	//newLayer2->CreateLayer("Frame2");
	//newLayer2->SetParentMap(m_pMap);
	//newLayer2->CoordTrans(m_pMap->GetCoordinateTransform());
	//osg::StateSet* pSateSet2 = newLayer1->GetLayerNode()->getOrCreateStateSet();
	//pSateSet2->setRenderBinDetails(2, "Frame2");
	//pSateSet2->setMode(GL_BLEND,osg::StateAttribute::ON);
	//pSateSet2->setMode( GL_DEPTH_TEST, osg::StateAttribute::ON);
	//m_pMap->AddLayer("网格边框2",newLayer2);
	
}

Layer* GISMapManager::NewRasterLayer(const QString& fileName, qreal qrz )
{
	if(!m_pMap) return 0;

	goto_gis::DataProvider* pDataRovider;
	typedef DataProvider* (*DataProviderPluginMain)();

	goto_gis::CoordinateTransform* ct = m_pMap->GetCoordinateTransform();

	QLibrary myLib("TIFFProvider.dll");
	DataProviderPluginMain GetTiffProvider = (DataProviderPluginMain) myLib.resolve("DataProviderPluginMain");

	pDataRovider = GetTiffProvider();

	goto_gis::Layer* pLayer = new goto_gis::VectorLayer(pDataRovider, ct);
	pLayer->LayerZ(qrz);
	pDataRovider->ParentLayer(pLayer);
	pDataRovider->LoadFile(fileName.toStdString());

	return pLayer;
	//pMap->AddLayer(fileName.toStdString(), pLayer);
}

bool GISMapManager::AddLayer( Map* map,const QString& filename )
{
	Layer* pNewLayer = 0;
	//Layer* copyLayer = map->GetLayer("中国地图");
	float layer_text_font_point_size = 20;
	float layerLineWidth = 1;
	unsigned int layerColor = 4278255360;
	std::string showName = "NAME";
	qreal layerz = 0.5;

	QString fileSuffix = QFileInfo(filename).suffix();
	if("shp" == fileSuffix)
	{

		//typedef DataProvider* (*pDataProviderPluginMain)();
		//DataProvider* pDataProvider = 0;
		//QLibrary libQGISVectorProvider("GISDBEngine.dll");
		//pDataProviderPluginMain GetProvider = (pDataProviderPluginMain)(libQGISVectorProvider.resolve("DataProviderPluginMain"));
		//pDataProvider = GetProvider();
		GeosOnTheOsg* pGeosOsg = GeosOnTheOsg::Instantiate();

		pNewLayer = NewVectorLayer(filename, layerz);
		//pNewLayer = new VectorLayer(pDataProvider,map->GetCoordinateTransform());
		pNewLayer->CoordTrans(map->GetCoordinateTransform());
		pNewLayer->FontSize(layer_text_font_point_size);
		pNewLayer->GeometrySize(layerLineWidth);
		pNewLayer->ShowFiedName(showName);
		pNewLayer->TextColor(layerColor);
		pNewLayer->SetParentMap(map);

		pGeosOsg->OpenDataAsLayer(filename.toStdString(),pNewLayer);
	}
	else if("tif" == fileSuffix)
	{
		pNewLayer = NewRasterLayer(filename, layerz);
	}

	if (pNewLayer == 0)
	{
		QMessageBox::critical(0,tr("警告"),QCoreApplication::applicationDirPath() + filename+tr("图层装载失败，请检查配置！"));
		exit(0);
	}

	if (layerColor)
	{
		QColor color = QColor::fromRgb(layerColor);
		osg::Vec4 vect4(color.redF(),color.greenF(),color.blueF(),color.alphaF());
		pNewLayer->InsertLayerAttribute(vect4,"颜色","Color","图形颜色",QVariant::Color);
		pNewLayer->InsertLayerAttribute(layerLineWidth,"线宽","WidthInt","线宽",QVariant::Double);
		pNewLayer->InsertLayerAttribute(QStringList()<<"GL_LINE_STIPPLE"<<"LINESTIPPLE","线型","StrippleInt","线型",QVariant::StringList);
		pNewLayer->InsertLayerAttribute(layerLineWidth,"点大小","PointInt","点大小",QVariant::Double);
		pNewLayer->SetLayerColor(vect4);
	}
	pNewLayer->CreateLayerData();
	map->AddLayer(filename.toStdString(),pNewLayer);
	return true;
}



#include <QtOpenGL/QGLWidget>
#include <QtCore/QThread>
#include <OsgViewer/Viewer>
#include <OsgGA/TrackballManipulator>
#include<osgEarthUtil/SkyNode>

#include "AdapterWidget.h"
#include "ViewManager.h"
#include "OsgEarthBridge.h"
#include "DB2Dialog.h"
#include "ExtendMap.h"
#include "VolumeDraw.h"
#include "DataProvider.h"

int DB2Dialog::gRowCount = 1;
DB2Dialog::DB2Dialog(Map* srcMap , int mix , int max ,QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	m_intValidator = new QIntValidator();
	m_intValidator->setRange(mix,max);
	m_pMap = srcMap;
	InitDialog();
	InitSlots();
	ui.pushBtnCal_0->setUserData(0, new IsoUserData(m_pMap));
}

DB2Dialog::~DB2Dialog()
{

}

void DB2Dialog::InitDialog()
{
	
	ui.lineEdit_0->setFixedWidth(255);
	ui.lineEdit_0->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
	ui.lineEdit_0->setValidator(m_intValidator);

	ui.gridLayout->setContentsMargins(3,3,3,3);
	ui.gridLayout_2->setContentsMargins(0,0,0,0);

	ui.gridLayout->setSizeConstraint(QLayout::SetFixedSize);
	ui.gridLayout_2->setSizeConstraint(QLayout::SetFixedSize);
}

void DB2Dialog::InitSlots()
{
		connect(ui.pushBtnCal_0, SIGNAL(clicked()),this,  SLOT(Slots_PushButtonClicked()));
		connect(ui.pushBtnAdd_0, SIGNAL(clicked()),this,  SLOT(Slots_PushButtonClicked()));
		connect(ui.pushBtnDel_0, SIGNAL(clicked()),this,  SLOT(Slots_PushButtonClicked()));
}


/** 
  * @brief    图层计算槽   
  * @author   易家祥  
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   DB2Dialog  图层计算槽
  * @note     易家祥
  * @date     2013年3月                               
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
void DB2Dialog::Slots_PushButtonClicked()
{
	QPushButton* btn = dynamic_cast<QPushButton*>(sender());
	int rowCount = gRowCount;//ui.gridLayout->rowCount();//得到行号
	if (btn->text() == QString::fromLocal8Bit("计算"))//计算
	{
		QString row = btn->objectName().split("_")[1];//得到选择的行号
		QList<QLineEdit*> list = this->findChildren<QLineEdit*>();
		//QObjectList list = children();
		//for (int i = 0; i < list.size()-1; ++i) 
		foreach(QLineEdit* linedit , list)
		{
			//QLineEdit* linedit = dynamic_cast<QLineEdit*>(list.at(i));
			QString lineditObjName = tr("lineEdit_")+row;
			if ( linedit && linedit->objectName() == lineditObjName )
			{
				//int isoValue = linedit->text().toInt();
				QString strCommand = linedit->text();
				int iC1, iC2, iISO;
				QStringList comList = strCommand.split('-');

				if(comList.size() == 3)
				{
					iC1 = comList.at(0).toInt();
					iC2 = comList.at(1).toInt();
					iISO = comList.at(2).toInt();
				}
				else
				{
					iISO = strCommand.toInt();
				}
				IsoUserData* pIsoUserData = dynamic_cast<IsoUserData*>(btn->userData(0));
				if(pIsoUserData)
					pIsoUserData->ComputerIsoSurface(iISO);

				break;
			}
		}
	}
	else if (btn == ui.pushBtnAdd_0)//加号
	{
		if (gRowCount > 4)
		{
			QMessageBox::information(this,tr("提示"),tr("最多5个！"));
			return;
		}
		QLineEdit* lineEdit = new QLineEdit(this);
		lineEdit->setValidator(m_intValidator);
		lineEdit->setFixedWidth(255);
		lineEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
		lineEdit->setObjectName("lineEdit_"+QString::number(rowCount));
		//lineEdit->setText("lineEdit_"+QString::number(rowCount));
		ui.gridLayout->addWidget(lineEdit, rowCount, 0, 1, 1);

		QLabel* label = new QLabel(this);
		label->setObjectName("label_"+QString::number(rowCount));
		label->setText(tr("DB2"));
		ui.gridLayout->addWidget(label, rowCount, 1, 1, 1);

		QPushButton* pushBtnCal = new QPushButton(this);
		pushBtnCal->setUserData(0, new IsoUserData(m_pMap));
		pushBtnCal->setObjectName("pushBtnCal_"+QString::number(rowCount));
		pushBtnCal->setText(tr("计算"));
		ui.gridLayout->addWidget(pushBtnCal, rowCount, 2, 1, 1);
		connect(pushBtnCal, SIGNAL(clicked()),this,  SLOT(Slots_PushButtonClicked()));
		gRowCount++;
	}
	else if (btn == ui.pushBtnDel_0)//减号
	{
		if (rowCount > 1)
		{
			QWidget* w1 = ui.gridLayout->itemAtPosition(rowCount-1,0)->widget();
			QWidget* w2 = ui.gridLayout->itemAtPosition(rowCount-1,1)->widget();
			QWidget* w3 = ui.gridLayout->itemAtPosition(rowCount-1,2)->widget();

			IsoUserData* pIsoUserData = dynamic_cast<IsoUserData*>(w3->userData(0));

			if(pIsoUserData)
				pIsoUserData->DeleteIsoSuerface();

			ui.gridLayout->removeWidget(ui.gridLayout->itemAtPosition(rowCount-1,0)->widget());
			ui.gridLayout->removeWidget(ui.gridLayout->itemAtPosition(rowCount-1,1)->widget());
			ui.gridLayout->removeWidget(ui.gridLayout->itemAtPosition(rowCount-1,2)->widget());

			delete ui.gridLayout->itemAtPosition(rowCount-1,0);
			delete ui.gridLayout->itemAtPosition(rowCount-1,1);
			delete ui.gridLayout->itemAtPosition(rowCount-1,2);
			
			delete w1;
			delete w2;
			delete w3;

			gRowCount--;
			ui.gridLayout->setSizeConstraint(QLayout::SetFixedSize);
			ui.gridLayout_2->setSizeConstraint(QLayout::SetFixedSize);
		}
	}
}

void IsoUserData::ComputerIsoSurface( int iValue )
{
	if(m_computed) return;
	
	IsoSurface* pIS = (IsoSurface*)m_pMap->GetExtendInterface(std::string("IsoSurface"));
	pIS->ComputerIso(iValue);
#if 0
	goto_gis::Map* map1 = view->Map2D();

	goto_gis::Layer* pCircleLayer = map1->GetLayer("距离圈");

	if (pCircleLayer != 0)
	{
		boost::any anyValue;
		QString qstrValue;
		std::string strValue;
		double dR = -1, dH = 0;
		if(pCircleLayer->GetLayerAtrributeFromName("有效范围：", anyValue))
		{
			strValue = boost::any_cast<std::string>(anyValue);
			qstrValue = QString::fromStdString(strValue).remove("Km");
			dR = qstrValue.toDouble()  * 1000;
		}
		else
		{
			return;
		}

		if(pCircleLayer->GetLayerAtrributeFromName("海拔高度：", anyValue))
		{
			strValue = boost::any_cast<std::string>(anyValue);
			qstrValue = QString::fromStdString(strValue);
			
			dH = qstrValue.toDouble();
		}
		else
		{
			return;
		}

		goto_gis::Layer* pRadarLayer = map1->GetLayer("第1仰角层");
		if(!pRadarLayer) return;
		goto_gis::DataProvider* pDP = pRadarLayer->GetDataProvider();
		if(!pDP) return;

		VolumeImage* pVI = (VolumeImage*)(pDP->GetExtendInterface(3));
		if(!pVI) return;

		QVector<QImage> QtImages;
		pVI->GetImages(&QtImages);

		osg::ImageList imageList;

		for(size_t i = 0; i < QtImages.count(); i++)
		{
			osg::Image* pImage;
			QtImages.at(i).save(QString("radar%1.png").arg(i));

			/*QImage img = QGLWidget::convertToGLFormat(QtImages.at(i)); 

			unsigned char* data = img.bits(); 
			pImage = new osg::Image(); 
			pImage->setImage(img.width(), 
			img.height(), 
			1, 
			4, 
			GL_RGBA, 
			GL_UNSIGNED_BYTE, 
			data, 
			osg::Image::NO_DELETE, 
			1); */
			
			//osgDB::writeImageFile(*pImage, QString("radar%1.png").arg(i).toStdString());
		}
		
		for(size_t i = 0; i < QtImages.count(); i++)
		{
			osg::Image* pOsgImage = osgDB::readImageFile(QString("radar%1.png").arg(i).toStdString());
			imageList.push_back(pOsgImage);
		}
		pOEB->ComputerVolume(imageList, dR*2, dR*2, dH);
	}
#endif
	m_iValue = iValue;

	m_computed = true;
}

IsoUserData::IsoUserData(Map* srcMap)
{
	m_pMap = srcMap;
	m_computed = false;
}

void IsoUserData::DeleteIsoSuerface()
{
	if(!m_computed) return;

	//ViewerQT* view = SysMainWindow::GetMainWindow()->FromWidgetToQGLW();//GetViewManger();
	IsoSurface* pIS = (IsoSurface*)m_pMap->GetExtendInterface(std::string("IsoSurface"));
	pIS->DelIsoSuerface(m_iValue);
}

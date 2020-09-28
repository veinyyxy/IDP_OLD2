#include "stdafx.h"
#include "DisplaySettingsDialog.h"
#include "Map.h"
#include "MainControlFrame.h"
#include "ControllerFrameContext.h"
//#include "sysmainwindow.h"

MapConfigModel::MapConfigModel(void)
{
}


MapConfigModel::~MapConfigModel(void)
{
}

//////////////////////////////////////////////////////////////////////////

MapPropertyControl::MapPropertyControl() 
{

}

MapPropertyControl::~MapPropertyControl(void)
{
}

void MapPropertyControl::upDataMapProperty(MapConfigModel* pMCM )
{
	float fLeft, fBottom, fRight, fTop;
	QVector4D v4Color = pMCM->GetBackgroundColor();
	pMCM->GetLookedRegion(fLeft, fBottom, fRight, fTop);
	QVector4D v3MapCenter = pMCM->GetMapCenter();

	MainControlFrame* pMCF = MainControlFrame::GetMainControlFrameInstance();
	ControllerFrameContext* pCFC = pMCF->GetCurrentContext();
	goto_gis::Map* pMap = pCFC->GetCurrentMap();

	pMap->SetBackgroundColor(osg::Vec4(v4Color.x(), v4Color.y(), v4Color.z(), v4Color.w()));
	pMap->SetMapCenter(osg::Vec3(v3MapCenter.x(), v3MapCenter.y(), v3MapCenter.z()));
	pMap->SetLookedRegion(fLeft, fBottom, fRight, fTop);

	pMap->UpdateMap();

}

void MapPropertyControl::upDataToModel( MapConfigModel* pMCM )
{
	float fLeft, fBottom, fRight, fTop;
	osg::Vec4 v4Color;
	osg::Vec3 v3MapCenter;

	MainControlFrame* pMCF = MainControlFrame::GetMainControlFrameInstance();
	ControllerFrameContext* pCFC = pMCF->GetCurrentContext();
	goto_gis::Map* pMap = pCFC->GetCurrentMap();

	v4Color = pMap->GetBackgroundColor();
	v3MapCenter = pMap->GetMapCenter();
	pMap->GetLookedRegion(fLeft, fBottom, fRight, fTop);


	pMCM->SetBackgroundColor(QVector4D(v4Color.r(), v4Color.g(), v4Color.b(), v4Color.a()));
	pMCM->SetLookedRegion(fLeft, fBottom, fRight, fTop);
	pMCM->SetMapCenter(QVector3D(v3MapCenter.x(), v3MapCenter.y(), v3MapCenter.z()));
}

//////////////////////////////////////////////////////////////////////////
DisplaySettingsDialog::DisplaySettingsDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.listWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this, SLOT(Slot_ChangePage(QListWidgetItem*,QListWidgetItem*)));
	connect(ui.buttonBox, SIGNAL(accepted(void)), SLOT(Slot_OK()));
	connect(ui.bgPushButton , SIGNAL(clicked()),this,SLOT(Slot_PushButtonClicked()));
}

DisplaySettingsDialog::~DisplaySettingsDialog()
{

}


void DisplaySettingsDialog::Slot_ChangePage(QListWidgetItem *current, QListWidgetItem *previous)
{
	if (!current)
		current = previous;

	ui.stackedWidget->setCurrentIndex(ui.listWidget->row(current));
}

void DisplaySettingsDialog::StackedWidgetCurrentIndex( int index /*= 0*/ )
{
	ui.stackedWidget->setCurrentIndex(index);
}

void DisplaySettingsDialog::Slot_OK()
{
	MainControlFrame* pMCF = MainControlFrame::GetMainControlFrameInstance();
	ControllerFrameContext* pCFC = pMCF->GetCurrentContext();
	goto_gis::Map* pMap = pCFC->GetCurrentMap();

	if(!pMap) return;

	float fLeft, fBottom, fRight, fTop;
	QVector4D v4Color(1, 1, 1, 1);
	QColor color = ui.frame_2->palette().background().color();
	if (color.isValid())
	{
		v4Color.setX(color.redF());
		v4Color.setY(color.greenF());
		v4Color.setZ(color.blueF());
		v4Color.setW(color.alphaF());
	}
	QVector3D v3MapCenter;
	MapConfigModel mapconfigModel;
	MapPropertyControl mapPropertyControl;
	//QPalette::ColorRole fuckColor = ui.frame_2->();
	
	QString strScope = ui.lineEdit->text();
	QStringList strlist = strScope.split(',');
	if(strlist.size() != 4) return;
	fLeft	= strlist.at(0).toFloat();
	fTop	= strlist.at(1).toFloat();
	fRight	= strlist.at(2).toFloat();
	fBottom	= strlist.at(3).toFloat();
	QString strCenter = ui.lineEdit_2->text();
	strlist = strCenter.split(',');
	
	if(strlist.size() != 2) return;
	float fX = strlist.at(0).toFloat();
	float fY = strlist.at(1).toFloat();

	v3MapCenter.setX(fX);
	v3MapCenter.setY(fY);

	mapconfigModel.SetBackgroundColor(v4Color);
	mapconfigModel.SetLookedRegion(fLeft,fBottom,fRight,fTop);
	mapconfigModel.SetMapCenter(v3MapCenter);

	mapPropertyControl.upDataMapProperty(&mapconfigModel);
	this->close();
}

void DisplaySettingsDialog::Slot_PushButtonClicked()
{
	QPushButton* btn = dynamic_cast<QPushButton*>(sender());
	if (btn == ui.bgPushButton)//Ñ¡ÔñµØÍ¼±³¾°ÑÕÉ«
	{
		QColor color = QColorDialog::getColor(Qt::white,this);
		if (color.isValid())
		{
			//v4Color.setX(color.redF());
			//v4Color.setY(color.greenF());
			//v4Color.setZ(color.blueF());
			//v4Color.setW(color.alphaF());

			QPalette palette=ui.frame_2->palette();
			palette.setColor(ui.frame_2->backgroundRole(),color);
			ui.frame_2->setPalette(palette);
			ui.frame_2->setAutoFillBackground(true);
		}
	}
}

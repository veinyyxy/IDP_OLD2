#include "DisplaySettingsDialog.h"
#include "MapConfigModel.h"
#include "MapPropertyControl.h"
#include "Map.h"
#include "sysmainwindow.h"

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
	goto_gis::Map* pMap = SysMainWindow::GetMainWindow()->GetCurrentMap();
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
	MapPropertyControl mapPropertyControl(pMap);
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

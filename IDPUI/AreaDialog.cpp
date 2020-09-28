#include <QDebug>
#include "radarstructdataprovider.h"
#include "radarsingledataprovider.h"
#include "radarMosaicDataProvider.h"

#include "DataProvider.h"
#include "GeosOnTheOsg.h"

#include "AreaStatistic.h"
#include "DrawGeometryActions.h"
#include "DefaultManipulator.h"
#include "AdapterWidget.h"
#include "ManualDrawRect.h"
#include "DrawRect.h"

#include "AreaDialog.h"



class RadarRectHandler : public RectResult
{
public:
	RadarRectHandler() : m_pDataProvier(0){};
	virtual void FinishDraw( const osg::Vec3Array* pVec3Array )
	{
		_pVec3Array->push_back(pVec3Array->at(2));
		_pVec3Array->push_back(pVec3Array->at(3));
	}
	osg::Vec3Array* _pVec3Array;
	DataProvider* m_pDataProvier;
};

AreaDialog::AreaDialog(Map* srcMap , QWidget *parent): QDialog(parent),m_pMap(srcMap),m_bPie(true),m_vMin(0),m_vMax(0)
{
	ui.setupUi(this);
	
	ui.radioBtnNoAll->setVisible(false);
	InitDialog();
	InitSlots();
}

AreaDialog::~AreaDialog()
{

}
void AreaDialog::InitDialog()
{
	this->setFixedSize(180,230);
	ui.gridLayout->setContentsMargins(5,5,5,5);
	ui.gridLayout_2->setContentsMargins(0,0,0,0);
	ui.gridLayout_3->setContentsMargins(0,0,0,0);
	ui.gridLayout_4->setContentsMargins(0,0,0,0);
	ui.gridLayout_5->setContentsMargins(0,0,0,0);
	ui.gridLayout_6->setContentsMargins(0,0,0,0);
	ui.gridLayout_7->setContentsMargins(0,0,0,0);
	ui.gridLayout_8->setContentsMargins(0,0,0,0);
	//
	this->setContentsMargins(0,0,0,0);
	ui.groupBox->setContentsMargins(0,0,0,0);
	ui.groupBox_2->setContentsMargins(0,0,0,0);
	ui.groupBox_3->setContentsMargins(0,0,0,0);
	ui.gridLayout->setSizeConstraint(QLayout::SetFixedSize);

	m_pVec3Array = new osg::Vec3Array;

}

void AreaDialog::InitSlots()
{
	connect(ui.pushBtnOK, SIGNAL(clicked()),this,  SLOT(Slots_PushButtonClicked()));
	connect(ui.pushBtnExit, SIGNAL(clicked()),this,  SLOT(Slots_PushButtonClicked()));
	connect(ui.radioBtnNoAll,SIGNAL(clicked()),this,SLOT(Slots_RadioButtonClicked()));
}

void AreaDialog::Slots_PushButtonClicked()
{
	QPushButton* btn = dynamic_cast<QPushButton*>(sender());
	if (btn == ui.pushBtnExit)
	{
		this->close();
	}
	else if (btn == ui.pushBtnOK)
	{
		DrawArea();
	}
}


//////////////////////////////////////////////////////////////////////////
void AreaDialog::DrawArea()
{

	m_bPie = ui.radioBtnB->isChecked();
	m_vMax = ui.cmbMax->currentText().toFloat();
	m_vMin =  ui.cmbMin->currentText().toFloat();
	if (ui.radioBtnB->isChecked())
	{
		m_bPie = true;
	}
	else if (ui.radioBtnZ->isChecked())
	{
		m_bPie = false;
	}
	DataProvider* dp = m_pMap->CurrentLayer()->GetDataProvider();
	RadarSingleDataProvider* rsdp = dynamic_cast<RadarSingleDataProvider*>(dp);
	RadarMosaicDataProvider* rmdp = dynamic_cast<RadarMosaicDataProvider*>(dp);
	RadarStructDataProvider* rssdp = dynamic_cast<RadarStructDataProvider*>(dp);
	if (ui.radioBtnAll->isChecked())
	{
		if (rsdp)
		{
			//rsdp->SetRequsetLayer(0 );//默认统计第0层，之后动态识别当前层（陈志学修改）
			rsdp->AreaAdd(true,m_bPie,m_vMin,m_vMax,0);
		}

		else if (rmdp)
		{
			rmdp->AreaAdd(true,m_bPie,m_vMin,m_vMax,0);
		}

		else if (rssdp)
		{
			rssdp->AreaAdd(true,m_bPie,m_vMin,m_vMax,0);
		}
	}
	else
	{
		if(m_pVec3Array->size() <=0)
		{
			QMessageBox::information(0,QObject::tr("提示"),QObject::tr("请按住<Ctrl>键,选择区域！"));
			return;
		}
		//DataProvider* dp = m_pMap->CurrentLayer()->GetDataProvider();
		//if (dp)
		//{
		//	dp->AreaAdd(false,m_bPie,m_vMin,m_vMax,m_pVec3Array);
		//}
		if (rsdp)
		{
			//rsdp->SetRequsetLayer(0 );//默认统计第0层，之后动态识别当前层（陈志学修改）
			rsdp->AreaAdd(false,m_bPie,m_vMin,m_vMax,m_pVec3Array);
		}

		else if (rmdp)
		{
			rmdp->AreaAdd(false,m_bPie,m_vMin,m_vMax,m_pVec3Array);
		}

		else if (rssdp)
		{
			rssdp->AreaAdd(false,m_bPie,m_vMin,m_vMax,m_pVec3Array);
		}
	}
}

void AreaDialog::Slots_RadioButtonClicked()
{
	QRadioButton* btn = dynamic_cast<QRadioButton*>(sender());

	if (btn == ui.radioBtnNoAll)
	{
		DrawRectAction* pRectAction = new DrawRectAction(0);
		pRectAction->MyAction();
		RadarRectHandler* pRadarHandler = new RadarRectHandler;
		pRadarHandler->m_pDataProvier = m_pMap->CurrentLayer()->GetDataProvider();
		pRadarHandler->_pVec3Array = m_pVec3Array;
		goto_gis::Layer* pLayer = m_pMap->GetLayer("第1仰角层");
		pRadarHandler->m_pDataProvier = pLayer->GetDataProvider();
		pRectAction->GetDrawRect()->SetResultHandle(pRadarHandler);
		delete pRectAction;
	}
	else if (btn == ui.radioBtnAll)
	{

	}
	
}

//////////////////////////////////////////////////////////////////////////
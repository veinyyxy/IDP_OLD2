#include <windows.h>
#include <QDebug>
#include "ColorTableActions.h"

FlashAction::FlashAction(const QString &text, QObject* parent):QAction(text,parent)
{
	m_pFlashThread = NULL;
}

FlashAction::~FlashAction(void)
{
}

void FlashAction::MyDo()
{
	if (m_pFlashThread != NULL)
	{
		delete m_pFlashThread;
		m_pFlashThread = NULL;
	}
	QWidget* cw = (QWidget*)this->property("PW").toLongLong();
	int index = this->property("INDEX").toInt();
	qDebug()<<index;
	if (cw)
	{
		Map* map =(Map*)cw->property("Map").toLongLong();
		if (map)
		{
			Layer* layer = map->CurrentLayer();
			if (layer)
			{
				m_pFlashThread = new FlashThread(layer);
				startFlicker();
			}

		}
	}
}

void FlashAction::InitMyAction()
{
		m_pFlashThread = NULL;
}

void FlashAction::startFlicker()
{
	m_pFlashThread->setFlags(true);
	m_pFlashThread->startFlicker();
}

void FlashAction::stopFlicker()
{
	m_pFlashThread->setFlags(false);
	m_pFlashThread->stopFlicker();
}



FlashThread::FlashThread(Layer* layer )
{
	m_FlickerSpeed= 0.08 * 1000000;
	m_pLayer = layer;
	m_dfColorArray = dynamic_cast<osg::Vec4Array*>(m_pLayer->GetMgoGeometry()->at(0)->GetOsgGeometry()->getColorArray());
	m_chColorArray = new osg::Vec4Array();
	m_chColorArray->push_back(osg::Vec4(1.0,1.0,1.0,1.0));
}

FlashThread::~ FlashThread()
{
	
}

void FlashThread::run()
{
	do 
	{
		int m_iNextCount = 0;
		LARGE_INTEGER litmp;
		LONGLONG Qpart1,Qpart2,Useingtime;
		double dfMinus,dfFreq,dfTime;

		//�ı���ɫ
		ColorChange(m_dfColorArray);

		//���CPU��ʱ����ʱ��Ƶ��
		QueryPerformanceFrequency(&litmp);//ȡ�ø߾������м�������Ƶ��f,��λ��ÿ����ٴΣ�n/s����
		dfFreq = (double)litmp.QuadPart;
		QueryPerformanceCounter(&litmp);//ȡ�ø߾������м���������ֵ
		Qpart1 = litmp.QuadPart; //��ʼ��ʱ
		do 
		{
			QueryPerformanceCounter(&litmp);//ȡ�ø߾������м���������ֵ
			Qpart2 = litmp.QuadPart; //��ֹ��ʱ

			dfMinus = (double)(Qpart2 - Qpart1);//���������ֵ
			dfTime = dfMinus / dfFreq;//��ö�Ӧʱ�䣬��λΪ��,���Գ�1000000��ȷ��΢�뼶��us��
			dfTime *= 1000000;
		} while(dfTime < m_FlickerSpeed);
		//�ı���ɫ
		ColorChange(m_chColorArray);
	} while (m_bStop);
}

void FlashThread::stopFlicker()
{
	m_bStop = false;
}

void FlashThread::startFlicker()
{
	this->start();
}

void FlashThread::setFlags( bool bStop )
{
	m_bStop = bStop;
}

void FlashThread::ColorChange(osg::Vec4Array* colorArray)
{
	if (m_pLayer)
	{
		m_pLayer->GetMgoGeometry()->at(0)->GetOsgGeometry()->setColorArray(colorArray);
		m_pLayer->UpdateLayer();
	}
}
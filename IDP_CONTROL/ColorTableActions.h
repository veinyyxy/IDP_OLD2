#pragma once
#include <QThread>
#include <QAction>
#include <QMutex>

#include <osg/Vec4>
#include "Map.h"
#include "Layer.h"
#include "goQuadrilateral.h"


using namespace goto_gis;

class FlashThread : public QThread
{
public:
	FlashThread(Layer* layer);
	~ FlashThread();
	void stopFlicker();
	void startFlicker();

	void setFlags(bool bStop);
protected:
	void run();
	void ColorChange(osg::Vec4Array* colorArray);
private:
	double m_FlickerSpeed;
	bool m_bStop;
	QMutex mutex;
	Layer* m_pLayer;
	osg::Vec4Array* m_dfColorArray;
	osg::Vec4Array* m_chColorArray;
};

class FlashAction : public QAction
{
public: 
	FlashAction(const QString &text, QObject* parent = 0);
	virtual ~FlashAction(void);

	void MyDo();
	void InitMyAction();
	void stopFlicker();
	void startFlicker();

private:
	FlashThread* m_pFlashThread;
};


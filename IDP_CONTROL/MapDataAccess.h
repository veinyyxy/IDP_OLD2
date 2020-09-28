#ifndef MAPDATAACCESS_H
#define MAPDATAACCESS_H

#include "idp_control_global.h"
#include <QObject>

class DefaultManipulator;

class IDP_CONTROL_EXPORT MapDataAccess : public QObject
{
public:
	MapDataAccess(QObject *parent = 0);
	~MapDataAccess();

	static MapDataAccess *Instance();//�õ�������ʵ��
	void GoMapCenter(QWidget* viewerWidget , QString layerName =QString::fromLocal8Bit("�й���ͼ"));

	void MapALLMap(QWidget* viewerWidget);
	void MapMZoomIn(QWidget* viewerWidget);
	void MapMZoomOut(QWidget* viewerWidget);
	void MapZoomIn(QWidget* viewerWidget);
	void MapZoomOut(QWidget* viewerWidget);

	void MapTop(QWidget* viewerWidget);
	void MapBottom(QWidget* viewerWidget);
	void MapLeft(QWidget* viewerWidget);
	void MapRight(QWidget* viewerWidget);
private:
	DefaultManipulator* GetDM(QWidget* viewerWidget);
};

#endif // MAPDATAACCESS_H

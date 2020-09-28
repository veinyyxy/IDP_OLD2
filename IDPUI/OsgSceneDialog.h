#pragma once
#include <QtGui/QDialog>

namespace osgViewer
{
	class Viewer;
}

class OsgSceneDialog :
	public QDialog
{
public:
	OsgSceneDialog(void);
	virtual ~OsgSceneDialog(void);
protected:
	osgViewer::Viewer* m_pViewer;
};


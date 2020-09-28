#ifndef COMMONTOOLBAR_H
#define COMMONTOOLBAR_H

#include <QtGui>
#include "ui_CommonToolBar.h"
#include "idpui_global.h"
#include <osgDB/WriteFile>
#include <osgGA/KeySwitchMatrixManipulator>

class MainWindow;
class CCipasComposer;
class DefaultManipulator;

class SnapImageDrawCallback : public osg::Camera::DrawCallback 
{ 
public:
	SnapImageDrawCallback() 
	{ 
		_snapImageOnNextFrame = false; 
	}

	void setFileName(const std::string& filename) { _filename = filename; } 
	const std::string& getFileName() const { return _filename; }
	void setSnapImageOnNextFrame(bool flag) { _snapImageOnNextFrame = flag; } 
	bool getSnapImageOnNextFrame() const { return _snapImageOnNextFrame; }
	virtual void operator () (const ::osg::Camera& camera) const 
	{
		if (!_snapImageOnNextFrame) return;
		int x,y,width,height;
		x = camera.getViewport()->x();
		y = camera.getViewport()->y();
		width = camera.getViewport()->width();
		height = camera.getViewport()->height();
		::osg::ref_ptr< ::osg::Image> image = new ::osg::Image();
		image->readPixels(x,y,width,height,GL_RGB,GL_UNSIGNED_BYTE);
		osgDB::writeImageFile(*image,_filename);
		_snapImageOnNextFrame = false;
	}
protected:
	::std::string _filename; 
	mutable bool _snapImageOnNextFrame;

};

class IDPUI_EXPORT CommonToolBar : public QToolBar
{
	Q_OBJECT

public:
	CommonToolBar(QWidget *parent = 0);
	~CommonToolBar();
	void CreateImage();
private:
	void InitSlots();
	DefaultManipulator* GetDM( QWidget* viewerWidget );
public Q_SLOTS:
		virtual void setVisible(bool visible);
		void Slots_ActionTriggered( QAction* action );
		void MapFull();
		void MapZoonIn();
		void MapZoonOut();
		void eMapMove(QPoint beginPoint , QPoint endPoint);
	
private:
	Ui::CommonToolBar ui;
	QActionGroup m_CommonActionGroup;
	MainWindow* _mw;
	QAction* m_pActionL;
	QAction* m_pActionM;
	CCipasComposer* m_pCipasComposer;
};

#endif // COMMONTOOLBAR_H

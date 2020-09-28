#pragma once
#include "idpui_global.h"
#include <QtGui/QAction>

class AnimaticControler;
#define ADD_MYACTION_CONTSTRUCTED_DEFINITION(class_name) \
	class_name( QObject * parent ); \
	class_name( const QString & text, QObject * parent );\
	class_name( const QIcon & icon, const QString & text, QObject * parent );

#define ADD_MYACTION_CONTSTRUCTED_DECLARATION(class_name)\
	class_name::class_name( QObject * parent = 0 ) \
: MyActionInterface(parent){InitMyAction();}\
class_name::class_name( const QString & text, QObject * parent  = 0) \
	: MyActionInterface(text, parent) {InitMyAction();}\
class_name::class_name( const QIcon & icon, const QString & text, QObject * parent  = 0)\
	: MyActionInterface(icon, text, parent){InitMyAction();}

class Controller;
class DisplayViewFactory;

class IDPUI_EXPORT MyActionInterface : public QAction
{
public:
	MyActionInterface( QObject * parent );
	MyActionInterface( const QString & text, QObject * parent );
	MyActionInterface( const QIcon & icon, const QString & text, QObject * parent );
	virtual void MyAction() = 0;
	virtual void InitMyAction() = 0;
	virtual void SetController(Controller* pController) = 0;
	virtual void SetDisplayViewFactory(DisplayViewFactory* pDVE);
protected:
	DisplayViewFactory* m_pDisplayViewFactory;
};


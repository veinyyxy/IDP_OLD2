#ifndef COMPOSERTOOLBAR_H
#define COMPOSERTOOLBAR_H

#include <QObject>
#include <QtGui>

#include "tmui_global.h"

class TMUI_EXPORT ComposerToolBar : public QObject
{
	Q_OBJECT

public:
	ComposerToolBar(QWidget *parent = 0);
	~ComposerToolBar();
public Q_SLOTS:
		
private:
	
};

#endif // COMPOSERTOOLBAR_H

#ifndef LEGENDWIDGET_H
#define LEGENDWIDGET_H

#include <QtGui>
#include "idpui_global.h"

class IDPUI_EXPORT LegendWidget : public QTabWidget
{
	Q_OBJECT

public:
	LegendWidget(int flag , QWidget *parent = 0);
	~LegendWidget();
	void AddMenuItem();
protected:
	void closeEvent( QCloseEvent *event );
	bool winEvent(MSG *m, long *result);
	virtual void mouseReleaseEvent( QMouseEvent* event );
public Q_SLOTS:
	virtual void setVisible(bool visible);
Q_SIGNALS:
	void SetCurrentWidget(QWidget* currentWidget);
};

#endif // LEGENDWIDGET_H

#ifndef RADARGRAPHICSVIEW_H
#define RADARGRAPHICSVIEW_H

#include <QtGui>

class RadarGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	RadarGraphicsView(QWidget *parent = 0);
	RadarGraphicsView(QGraphicsScene *scene, QWidget *parent = 0);
	~RadarGraphicsView();

	public Q_SLOTS:
		virtual void setVisible(bool visible);
	
};

#endif // RADARGRAPHICSVIEW_H

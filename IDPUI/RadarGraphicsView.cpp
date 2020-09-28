#include "RadarGraphicsView.h"

RadarGraphicsView::RadarGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{
	QGraphicsScene* scene = new QGraphicsScene(this);
	setScene(scene);
}

RadarGraphicsView::RadarGraphicsView( QGraphicsScene *scene, QWidget *parent /*= 0*/ ): QGraphicsView(scene, parent)
{

}

RadarGraphicsView::~RadarGraphicsView()
{

}

void RadarGraphicsView::setVisible( bool visible )
{
	//if (visible)
	{
		QAction* action = (QAction*)this->property("Action").toLongLong();
		if (action)
		{
			action->setCheckable(visible);
			action->setChecked(visible);
		}
	}
	QGraphicsView::setVisible(visible);
}

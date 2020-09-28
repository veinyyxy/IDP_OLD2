#include <QtGui\QtGui>

#include "RoseGraph.h"


//! [0] //! [1]
RoseGraph::RoseGraph(QWidget *parent)
//! [0] //! [2]
    : QWidget(parent)
//! [2] //! [3]
{
//! [3] //! [4]
    QTimer *timer = new QTimer(this);
//! [4] //! [5]
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
//! [5] //! [6]
    timer->start(1000);
//! [6]

    setWindowTitle(tr("Í³¼ÆÍ¼"));
    resize(200, 200);
//! [7]
}
//! [1] //! [7]

//! [8] //! [9]
void RoseGraph::paintEvent(QPaintEvent *)
//! [8] //! [10]
{
    QColor hourColor(127, 0, 127);
    QColor minuteColor(0, 127, 127, 191);

    int side = qMin(width(), height());
    QTime time = QTime::currentTime();
//! [10]

//! [11]
    QPainter painter(this);
//! [11] //! [12]
    painter.setRenderHint(QPainter::Antialiasing);
//! [12] //! [13]
    painter.translate(width() / 2, height() / 2);

    painter.scale(side / 240.0, side / 240.0);
    
	painter.setPen(hourColor);

	  for (int i = 0; i < 12; ++i) {
		  painter.drawLine(85, 0, 90, 0);
		  painter.rotate(30.0);
	  }
	
	painter.drawEllipse(QPointF(0,0),90,90);

	painter.drawText(QRectF(QPointF(-3,-110),QPointF(3,-90)),QString("0"));
	painter.drawText(QRectF(QPointF(90,-15),QPointF(110,15)),QString("90"));
	painter.drawText(QRectF(QPointF(-10,90),QPointF(10,100)),QString("180"));
	painter.drawText(QRectF(QPointF(-110,-15),QPointF(-90,15)),QString("270"));

//    painter.setPen(Qt::NoPen);
//! [22] //! [23]
 //   painter.setBrush(minuteColor);

//! [24]
	for (int i=0;i<36;i++)
	{
		RoseLeaf leaf;
		leaf.SetSize(m_sizearray.at(i));
		painter.save();
		painter.rotate(10*i);
		painter.drawConvexPolygon(leaf.getRoseLeaf(),3);
		painter.restore();
	}
	
//! [23] //! [24]

//! [25]
    painter.setPen(minuteColor);
//! [25] //! [26]

//! [27]
    for (int j = 0; j < 36; ++j) {
        if ((j % 3) != 0)
            painter.drawLine(87, 0, 90, 0);
        painter.rotate(10.0);
    }
//! [27]
}

void RoseGraph::SetSizeArray( QVector<int> sizearray )
{
	m_sizearray = sizearray;
}

//! [26]

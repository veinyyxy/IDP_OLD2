#ifndef ROSEGRAPH_H
#define ROSEGRAPH_H

#include <QtGui/QWidget>
#include "GraphicsControler_global.h"

class RoseLeaf{
public:
	RoseLeaf()
	{
		Rose[0] =  QPointF(0,0);
		Rose[1] =  QPointF(0,-10);
		Rose[2] =  QPointF(10*sinf(M_PI/18), -10*cosf(M_PI/18));
		m_size = 1;
	};
	~RoseLeaf(){};
	void SetSize(int size) {m_size = size;m_radius = m_size*10;}
	
	void updateRoseLeaf()
	{
		Rose[1] =  QPointF(0,-m_radius);
		Rose[2] =  QPointF(m_radius*sinf(M_PI/18), -m_radius*cosf(M_PI/18));
	}
	const QPointF* getRoseLeaf() {updateRoseLeaf(); return Rose;}
private:	
	QPointF Rose[3];
	int m_size;
	int m_radius;
};

//! [0]
class GRAPHICSCONTROLER_EXPORT RoseGraph : public QWidget
{
public:
    RoseGraph(QWidget *parent = 0);
	void SetSizeArray(QVector<int> sizearray);
protected:
    void paintEvent(QPaintEvent *event);
private:
	QVector<int> m_sizearray;
};
//! [0]

#endif

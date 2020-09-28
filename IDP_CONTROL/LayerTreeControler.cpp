#include "Map.h"
#include "RadarVectorLayer.h"
#include "RadarCameraControler.h"
#include <QtCore/QString>
#include "LayerTreeControler.h"


LayerTreeControler::LayerTreeControler(void) : m_pMap(0), m_pViewer(0)
{
}

LayerTreeControler::LayerTreeControler( Map* pM, osgViewer::Viewer* pV ) : m_pMap(pM), m_pViewer(pV)
{

}

LayerTreeControler::~LayerTreeControler(void)
{
}

void LayerTreeControler::AlignmentRadar(const QString& layerName)
{
	Layer* pLayer = m_pMap->GetLayer(layerName.toStdString());
	RadarCameraControler radarControler;
	radarControler.SetViewer(m_pViewer);
}

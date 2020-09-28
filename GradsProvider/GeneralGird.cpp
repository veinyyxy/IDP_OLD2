#include "GeneralGird.h"
#include "VectorLayer.h"

GeneralGird::GeneralGird(void)
{
	
}

GeneralGird::~GeneralGird(void)
{
}

int GeneralGird::Dump()
{
#if 0
	m_PTempText = new QBitmapText;
	m_stPoint = new Point;
 	QVector<StationValues>* stationvaluearry = m_PTempText->StationValueArray();
	StationValues TempStationValue;

	int i,j;
	for (j = 0; j < m_nLatNCCount; j++)//列
	{
		for (i = 0; i < m_nLonNCCount; i++)//行
		{
			if (m_dArrayValue[j * m_nLonNCCount + i] == m_fMissingV) continue;

			float curLon = m_fLonNCBegin + i*m_fLonNCDelta;
			float curLat = m_fLatNCBegin + j*m_fLatNCDelta;

			if (curLon >= m_fLonStart
				&& curLon <= m_fLonEnd
				&& curLat >= m_fLatStart
				&& curLat <= m_fLatEnd)
			{
				m_PTempText->Pointion()->push_back(QVector3D(m_fLonNCBegin + i*m_fLonNCDelta,m_fLatNCBegin + j*m_fLatNCDelta, 0));
				m_PTempText->StringArray()->append(QString("%1").arg(m_dArrayValue[j * m_nLonNCCount + i]));

				m_stPoint->getVectexArray()->push_back(QVector3D(m_fLonNCBegin + i*m_fLonNCDelta,m_fLatNCBegin + j*m_fLatNCDelta,0));
				m_stPoint->getColorArray()->push_back(QVector4D(0.0f, 0.2f, 0.2f,1.0f));
			}
		}
	}

	ColorAttribute * pTextColorAttri = new ColorAttribute();
	pTextColorAttri->Color(QVector4D(0.0,0.0,0.0,1));//默认设置为黑色

	m_PTempText->SetColor(pTextColorAttri);
	m_PTempText->DisplayList(true);
	m_PTempText->SetBakData();
//	container->InsertRenderable(m_PTempText);

	m_stPoint->SetColorArray();
	m_stPoint->SetBakData(); 
//	container->InsertRenderable(m_stPoint);	

	if (m_layer)
	{
		m_layer->GetLayerData()->InsertRenderable(m_PTempText);
		m_layer->SetStation(m_PTempText);
		m_layer->GetLayerData()->InsertRenderable(m_stPoint);
	}
#endif
	return 0;
}

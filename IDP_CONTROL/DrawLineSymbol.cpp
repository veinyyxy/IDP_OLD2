/*============================================================ 
�ļ�����DrawLineSymbol.h DrawLineSymbol.cpp 
�� ���� CDrawLineSymbol
�� �ࣺ ManualDraw
�� �ࣺ �� 
����˵������ʸ�������ߵ��㷨
����˵����
1����ȡ�û���PING�����������㣬�����²��㷨�����ؼ���õ��ĵ㼯��
2������Ļ��ת����OPENGL�����꣬��������Ӧ�Ļ�ͼ�ܵ���

----------------------------�汾����--------------------------
V 1.0 
ԭ���� ����ѩ
������ڣ�2011��4��2��

V 1.1 
�� �ߣ�������
������ڣ�2011��4��25��
����˵��������undo����

V 1.2 
�� �ߣ��
������ڣ�2011��5��28��
����˵���������ͷ����

============================================================*/


#include "DrawLineSymbol.h"

CDrawLineSymbol::CDrawLineSymbol( int nType ) : m_nType(nType)
{
	m_pDisplayLine = CreateLine();
	m_bFinished = false;
	m_pArray = NULL;
	m_pSymbolLine = NULL;
	m_bFirst = true;
	m_pSpLine =  new CCardinalSpline;
	m_InputData = new osg::Vec3Array;
	m_OutputData = new osg::Vec3Array;
	//m_pSymboldata = new SymbolData;	
	m_nType = nType;
}


CDrawLineSymbol::~CDrawLineSymbol(void)
{
	if(NULL != m_pSpLine)
	{
		delete m_pSpLine;
		m_pSpLine = NULL;
	}
}

osg::Geometry* CDrawLineSymbol::CreateLine()
{
	osg::ref_ptr<osg::Vec3Array> pVertexs = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> pColors = new osg::Vec4Array;

	osg::Geometry* pGeom = new osg::Geometry;
	pGeom->setVertexArray(pVertexs);
	pGeom->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINE_STRIP, 0, 0));

	pGeom->setColorArray(pColors);
	pGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	return pGeom;
}

bool CDrawLineSymbol::handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea )
{
	float fZ = 0;

	if(m_pLayer)
		fZ = m_pLayer->LayerZ();

	osg::Vec3 locPos;
	locPos.set(pos.x(), pos.y(), fZ);

	switch( ea.getEventType() )
	{
	case osgGA::GUIEventAdapter::MOVE:
		//osg::Vec3  glPoint, scPoint(event->x(), event->y(), 0.0f);

		if(m_bFinished)
		{
			if(m_InputData->size()<3) return;
			//ȡ��ƽ����ĵ�����
			m_InputData->push_back(locPos);
			m_pSpLine->GetSmoothPoints(*m_InputData, *m_OutputData);
			osg::Vec4Array *pColor = dynamic_cast<osg::Vec4Array*>(m_pDisplayLine->getColorArray());
			osg::Vec3Array *pArray = dynamic_cast<osg::Vec3Array*>(m_pDisplayLine->getVertexArray());
			//����ϴεĵ�����
			pArray->clear();
			pColor->clear();
			int j=0;
			osg::Vec3 topLefPoint, topRightPoint, bottomLeftPoint, bottomRightPoint;
			
			if(pArray->size() <2)  return ;
			if(m_bFirst)
			{
				osg::Group* pGroup = dynamic_cast<osg::Group*>(m_pLayer->GetLayerNode());
				osg::ref_ptr<osg::Geode> pGeode = new osg::Geode;

				pGeode->addDrawable(m_pDisplayLine);

				pGroup->addChild(pGeode);
			}
			m_InputData->pop_back();
			m_OutputData->clear();
		}
		break;
	case osgGA::GUIEventAdapter::PUSH:
		if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
		{
			if(!m_bFinished)
			{
				m_InputData->clear();
				m_bFinished = true;
				m_pArray = NULL;
			}		
			m_InputData->push_back(locPos);

			if(m_InputData->size()<3)
			{
				m_OutputData->clear();
				for(int i=0; i<m_InputData->size(); i++)
					m_OutputData->push_back(osg::Vec3((*m_InputData)[i].x(), (*m_InputData)[i].y(), (*m_InputData)[i].z()));
			}
			else
			{
				m_pSpLine->GetSmoothPoints(*m_InputData, *m_OutputData);//ȡ��ƽ����ĵ�����
			}

			osg::Vec4Array *pColor = dynamic_cast<osg::Vec4Array*>(m_pDisplayLine->getColorArray());
			m_pArray = dynamic_cast<osg::Vec3Array*>(m_pDisplayLine->getVertexArray());
			m_pArray->clear();	//����ϴεĵ�����		
			pColor->clear();

			pColor->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
			

			if(m_bFirst)
			{
				osg::Group* pGroup = dynamic_cast<osg::Group*>(m_pLayer->GetLayerNode());
				osg::ref_ptr<osg::Geode> pGeode = new osg::Geode;

				pGeode->addDrawable(m_pDisplayLine);

				pGroup->addChild(pGeode);

				m_bFirst = false;
			}
		}
		else if(ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
		{
			if(m_bFinished)				//�����Ҽ�����ʼ��˫��
			{
				m_bFinished = false;
				if(m_pArray->size() <2)  return;          //ֻ��һ���㣬�˳�

				osg::ref_ptr<osg::Geometry> m_pSymbolLine = new osg::Geometry;
#if 0
				QVector3DArray * ArrayTemp = m_pSymbolLine->GetArray(); 
				QVector3DArray * pOriginalArray = m_pSymbolLine->OriginalArray();

				for(int i = 0 ; i < m_pArray->size(); i++)
				{
					ArrayTemp->push_back (m_pArray->at(i));
					QVector3D temp = m_pArray->at(i);
					pOriginalArray->push_back(TransGLtoWGS84(temp));
				}

				m_pArray->Clear();
				m_pSymbolLine->SetSymbolLineStyle(m_Symbol);

				SymbolLayer *sLayer = dynamic_cast<SymbolLayer*>(m_ParentLayer);
				if (sLayer == NULL) return;
				//SymbolData *m_pSymboldata = sLayer->getSymbolData();
				m_pSymboldata = sLayer->getSymbolData();
				m_pSymboldata->getSymbolLine()->push_back(m_pSymbolLine);

				if(!m_bFirst)
				{
					m_ParentLayer->GetLayerData()->RemoveRenderable(m_pDisplayLine);
					m_bFirst = true;
				}
#endif
			}
		}
		break;
	default:
		return false;
	}

	return false;
}


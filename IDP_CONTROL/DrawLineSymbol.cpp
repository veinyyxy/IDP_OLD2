/*============================================================ 
文件名：DrawLineSymbol.h DrawLineSymbol.cpp 
类 名： CDrawLineSymbol
父 类： ManualDraw
子 类： 无 
功能说明：画矢量符号线的算法
调用说明：
1、获取用户的PING命令鼠标坐标点，调用下层算法，返回计算得到的点集；
2、将屏幕点转换成OPENGL的坐标，并传给相应的绘图管道；

----------------------------版本更新--------------------------
V 1.0 
原作者 ：任雪
完成日期：2011年4月2日

V 1.1 
作 者：杨晓宇
完成日期：2011年4月25日
更新说明：加入undo机制

V 1.2 
作 者：杨东
完成日期：2011年5月28日
更新说明：分离箭头符号

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
			//取得平滑后的点数组
			m_InputData->push_back(locPos);
			m_pSpLine->GetSmoothPoints(*m_InputData, *m_OutputData);
			osg::Vec4Array *pColor = dynamic_cast<osg::Vec4Array*>(m_pDisplayLine->getColorArray());
			osg::Vec3Array *pArray = dynamic_cast<osg::Vec3Array*>(m_pDisplayLine->getVertexArray());
			//清除上次的点数组
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
				m_pSpLine->GetSmoothPoints(*m_InputData, *m_OutputData);//取得平滑后的点数组
			}

			osg::Vec4Array *pColor = dynamic_cast<osg::Vec4Array*>(m_pDisplayLine->getColorArray());
			m_pArray = dynamic_cast<osg::Vec3Array*>(m_pDisplayLine->getVertexArray());
			m_pArray->clear();	//清除上次的点数组		
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
			if(m_bFinished)				//按下右键，开始画双线
			{
				m_bFinished = false;
				if(m_pArray->size() <2)  return;          //只有一个点，退出

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


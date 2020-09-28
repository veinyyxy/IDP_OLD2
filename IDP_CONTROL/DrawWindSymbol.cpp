#include "DrawWindSymbol.h"


DrawWindSymbol::DrawWindSymbol(int iWind, int iWindWidth) : m_bStart(false), m_Wind(iWindWidth), m_iWindSpeed(2), m_iWindID(0)
{
	m_WindSpeedVector.push_back(2);
	m_WindSpeedVector.push_back(4);
	m_WindSpeedVector.push_back(6);
	m_WindSpeedVector.push_back(8);
	m_WindSpeedVector.push_back(10);
	m_WindSpeedVector.push_back(12);
	m_WindSpeedVector.push_back(16);
	m_WindSpeedVector.push_back(20);
	m_WindSpeedVector.push_back(24);
	m_WindSpeedVector.push_back(26);
	m_WindSpeedVector.push_back(32);
	//m_WindSpeed.push_back();

	m_pWindVertexs = new osg::Vec3Array;
	m_WindColors = new osg::Vec4Array;

	m_pLayer = 0;
	//m_DrawNode = new osg::Geode;
	m_pVertexs = new osg::Vec3Array;
	//m_Colors = new osg::Vec4Array;
	m_pCircle = new osg::Geometry;
	m_pWindLine = new osg::Geometry;
	m_WindDirect = CreateText("", 20, osg::Vec3(0, 0, 0), osg::Vec4(1, 1, 1, 1));
	m_SpeedWindText = CreateText("", 25, osg::Vec3(0, 0, 0), osg::Vec4(1, 1, 1, 1));

	CreateCircle(3);
	//m_pLine = new osg::Geometry;
	m_pCircleNode1 = new osg::AutoTransform;
	//m_pCircle2 = new osg::AutoTransform;
	m_pWindNode = new osg::AutoTransform;
	m_pWindTransformNode = new osg::MatrixTransform;
	m_pWindNode->addChild(m_pWindTransformNode);
	m_pWindNode->setAutoScaleToScreen(true);
	osg::ref_ptr<osg::Geode> pWindLineGeode = new osg::Geode;
	pWindLineGeode->addDrawable(m_pWindLine);
	m_pWindTransformNode->addChild(pWindLineGeode);

	osg::ref_ptr<osg::Geode> pCircleGeode = new osg::Geode;
	pCircleGeode->addDrawable(m_pCircle);
	m_pCircleNode1->addChild(pCircleGeode);
	m_pCircleNode1->setAutoScaleToScreen(true);

	//m_DrawArrays = new osg::DrawArrays(osg::PrimitiveSet::LINES,0,0);
	//osg::Geode* pGeode = m_DrawNode->asGeode();
	//pGeode->addDrawable(m_pLine);

	//m_Colors->push_back(m_color);
	
	//m_pLine->setVertexArray(m_pVertexs);
	//m_pLine->setColorArray(m_Colors);

	//m_pLine->addPrimitiveSet(m_DrawArrays);
	//m_pLine->setColorBinding(osg::Geometry::BIND_OVERALL);
}


DrawWindSymbol::~DrawWindSymbol(void)
{
}

bool DrawWindSymbol::handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea )
{
	float fZ = 0;

	if(m_pLayer)
		fZ = m_pLayer->LayerZ();

	osg::Vec3 locPos;
	locPos.set(pos.x(), fZ, pos.y());

	switch( ea.getEventType() )
	{
	case osgGA::GUIEventAdapter::MOVE:
		if(m_bStart)
		{
			m_pVertexs->pop_back();
			m_pVertexs->push_back(locPos);
		}
		break;
	case osgGA::GUIEventAdapter::PUSH:
		{
			if(ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{
				if(m_bStart)
				{
					//m_pVertexs->push_back(locPos);
					m_pVertexs->push_back(locPos);

					if(m_pVertexs->size() >=2)
					{
						//m_bStart = false;
						//CreateLineGeometry();

						m_pVertexs->pop_back();
					}
				}
				else
				{
					m_pVertexs->clear();
					m_pVertexs->push_back(locPos);
					m_pVertexs->push_back(locPos);
					m_bStart = true;
				}

			}
		}
		break;

	case osgGA::GUIEventAdapter::RELEASE:
		if(ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
		{
			CreateLineGeometry();
			m_bStart = false;
		}
		break;
	case osgGA::GUIEventAdapter::SCROLL:
		{
			int iMark = ea.getButtonMask();
			osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();
			if(sm == osgGA::GUIEventAdapter::SCROLL_UP)
			{
				m_iWindID++;
				if(m_iWindID == m_WindSpeedVector.size())
				{
					m_iWindID = 0;
				}
			}
			else
			{
				m_iWindID--;
				if(m_iWindID < 0)
				{
					m_iWindID = m_WindSpeedVector.size()-1;
				}
			}
			
			m_iWindSpeed = m_WindSpeedVector[m_iWindID];
			std::string SpeedText;
			char buf[32] = {0};
			sprintf(buf, " б╛%d-╝╢╖чб┐", m_iWindID + 1);
			SpeedText = buf;

			SetText(m_SpeedWindText, SpeedText);//m_SpeedWindText->setText(SpeedText);
			//m_SpeedWindText->setColor(osg::Vec4(1, 1, 1, 1));
		}
		break;
	default:
		return false;
	}
	if(m_bStart)
	{
		//(*m_Colors)[0] = m_color;

		if(m_pVertexs->size() > 1)
		{
			std::string strAngle;
			double dAngle = CalculateAngle(m_pVertexs->at(0), m_pVertexs->at(1));
			SetTextOnLine(dAngle, strAngle);

			SetText(m_WindDirect, strAngle);//m_WindDirect->setText(strAngle);
			m_WindDirect->setPosition(m_pVertexs->at(1));

			dAngle = dAngle * (3.141592/180);
			osg::Matrix rotWindMatrix;
			rotWindMatrix.makeRotate(-dAngle - 3.1425726, osg::Vec3(0, -1, 0));
			m_pWindTransformNode->setMatrix(rotWindMatrix);
			m_pCircleNode1->setPosition(m_pVertexs->at(0));
			m_pWindNode->setPosition(m_pVertexs->at(0));
			m_SpeedWindText->setPosition(m_pVertexs->at(0));

			CreateWind(m_iWindSpeed, m_pWindLine);
		}
		//m_DrawArrays->set(osg::PrimitiveSet::LINES, 0, m_pVertexs->size());
		//m_pLine->dirtyDisplayList();
		//m_pLine->dirtyBound();
		return true;
	}
	return true;
}

void DrawWindSymbol::CreateLineGeometry()
{
	m_WindDirect->setText("");
	m_SpeedWindText->setText("");
	
	osg::ref_ptr<osg::AutoTransform> pWindNode = (osg::AutoTransform*)(m_pWindNode->clone(osg::CopyOp::DEEP_COPY_ALL));
	osg::ref_ptr<osg::AutoTransform> pCircleNode = (osg::AutoTransform*)(m_pCircleNode1->clone(osg::CopyOp::DEEP_COPY_ALL));

	//osg::BoundingBox bb = m_pWindLine->computeBound();
	//osg::ref_ptr<osg::Geometry> pBoundBox = new osg::Geometry;

	//CreateBoundBox(bb, pBoundBox);
	//osg::ref_ptr<osg::Geode> pGeode = new osg::Geode;
	//pGeode->setName("Wind_Line");
	//pGeode->addDrawable(pBoundBox);

	//pBoundBox->setName("Wind_Line");
	//pBoundBox->setUserData(pCircleNode);
	osg::Group* pGroup = dynamic_cast<osg::Group*>(m_pLayer->GetLayerNode());
	if(!pGroup) return;

	//pWindNode->getChild(0)->asGroup()->addChild(pGeode);
	pGroup->addChild(pWindNode);
	//pGroup->addChild(pCircleNode);
	
}

void DrawWindSymbol::SetColor(osg::Vec4& v4c)
{
	m_color = v4c;
	//(*m_Colors)[0] = m_color;
}

void DrawWindSymbol::SetDrawLayer( goto_gis::Layer* pLayer )
{
	if(!pLayer) return;

	osg::Group* pGroup = dynamic_cast<osg::Group*>(pLayer->GetLayerNode());
	if(!pGroup) return;

	//pGroup->addChild(m_DrawNode);
	pGroup->addChild(m_pWindNode);
	pGroup->addChild(m_pCircleNode1);

	osg::ref_ptr<osg::Geode> pGeode = new osg::Geode;
	pGeode->addDrawable(m_WindDirect);
	pGeode->addDrawable(m_SpeedWindText);

	pGroup->addChild(pGeode);
	m_pLayer = pLayer;
}

void DrawWindSymbol::ToDrawCircle( osg::Vec3& pos, float fr, float fz, osg::Vec3Array& out )
{
	float X, Y;
	X = pos.x();
	Y = pos.y();

	for (int p=1; p<=360; p++)
	{
		osg::Vec3 circle;
		float angle = p-1;
		circle.set(X+fr * sin(M_PI / 180 * angle), fz,Y+fr * cos(M_PI / 180 * angle));
		out.push_back(circle);
	}
}

void DrawWindSymbol::CreateCircle( float fr)
{
	osg::ref_ptr<osg::Vec3Array> pVertexs = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> pColors = new osg::Vec4Array;

	ToDrawCircle(osg::Vec3(0, 0, 0), fr, 0, *pVertexs);
	pColors->push_back(osg::Vec4(1, 0, 0, 1));


	m_pCircle->setVertexArray(pVertexs);
	m_pCircle->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINE_LOOP, 0, pVertexs->size()));

	m_pCircle->setColorArray(pColors);
	m_pCircle->setColorBinding(osg::Geometry::BIND_OVERALL);
}

double DrawWindSymbol::CalculateAngle(const osg::Vec3& startPoint, const osg::Vec3& endPoint)
{
	double x1,x2,y1,y2,t,c2,angle=0.0;
	x1 = startPoint.x();
	y1 = startPoint.z();
	x2 = endPoint.x();
	y2 = endPoint.z();
	if (x2 < x1 && y2 == y1)
	{
		return 270.0;
	}
	else if (x2 > x1 && y2 == y1)
	{
		return 90.0;
	}
	else if (x2 == x1 && y2 < y1)
	{
		return 360.0;
	}
	else if (x2 == x1 && y2 > y1)
	{
		return 180.0;
	} 
	c2 = (y2-y1)*(y2-y1)+(x2-x1)*(x2-x1);
	if(abs(c2 - 0) < 0.000000001) return 0;

	t = abs(y2-y1)/sqrt(c2);
	angle = asin(t);
	double tempangle;
	tempangle = angle*180/3.1415926;
	if (x2 > x1 && y2 < y1)
	{
		return 90.0-tempangle;
	}
	else if (x2 > x1 && y2 > y1)
	{
		return 90.0+tempangle;
	}
	else if (x2 < x1 && y2 > y1)
	{
		return 270.0-tempangle;
	}
	else if (x2 < x1 && y2 < y1)
	{
		return 270.0+tempangle;
	}
	//m_fAngle = (GLfloat)tempangle;
	return tempangle;
}

void DrawWindSymbol::SetTextOnLine(double dAngle, std::string& strFinalText)
{
	char buf[32];

	std::string strText;
	std::string strFormatText;

	float  CurrentZero = 0 ; 
	float  k = 0 ; 
	osg::Vec3 Origin1,Origin2, glPoint1, glPoint2,glPoint;

	if(glPoint2.x() == glPoint1.x())
	{
		CurrentZero = 0;
	}else
	{
		k =( glPoint1.y() - glPoint2.y() )/( glPoint1.x() - glPoint2.x());
		CurrentZero = atan(k);
		CurrentZero = CurrentZero * 180 / 3.1415926;
		if(k < 0)
			CurrentZero =(90 - abs(CurrentZero));
		else
			CurrentZero = 90 - abs(CurrentZero);
	}

	int temp = 0 ;
	if(k < 0)
	{
		if(dAngle > (360 - CurrentZero))
		{
			temp = int(CurrentZero) -(360 - int(dAngle));
		}else
		{
			temp = int(CurrentZero) + int(dAngle);
		}
	}else
	{
		if(dAngle <= CurrentZero)
		{
			temp = 360 - CurrentZero+ dAngle;
		}else
		{
			temp = int(dAngle)  - int(CurrentZero);
		}
	}


	int tempAngle = temp;
	if (tempAngle == 0)
	{
		strText = "N/0бу";
	} 
	else if (tempAngle == 180)
	{
		strText = "S/180бу";
	}	 
	else if (tempAngle == 90)
	{
		strText = "E/90бу";
	}
	else if (tempAngle == 270)
	{
		strText = "W/270бу";
	}
	else if (tempAngle == 45)
	{
		strText = "NE/45бу";
	}
	else if (tempAngle == 135)
	{
		strText = "SE/135бу";
	}
	else if (tempAngle == 225)
	{
		strText = "SW/225бу";
	}
	else if (tempAngle == 315)
	{
		strText = "NW/315бу";
	}
	else if (tempAngle > 0 && tempAngle < 45)
	{
		strFormatText = "NNE/%dбу";
		//sprintf(buf, "NNE/%1бу", tempAngle);
		//strText.append(QString().arg(tempAngle));
	}
	else if (tempAngle > 45 && tempAngle < 90)
	{
		strFormatText = "ENE/%dбу";
		//strText.append(QString("ENE/%1бу").arg(tempAngle));
	}
	else if (tempAngle > 90 && tempAngle < 135)
	{
		strFormatText = "ESE/%dбу";
		//strText.append(QString("ESE/%1бу").arg(tempAngle));
	} 
	else if (tempAngle > 135 && tempAngle < 180)
	{
		strFormatText = "SSE/%dбу";
		//strText.append(QString("SSE/%1бу").arg(tempAngle));
	}
	else if (tempAngle > 180 && tempAngle < 225)
	{
		strFormatText = "SSW/%dбу";
		//strText.append(QString("SSW/%1бу").arg(tempAngle));
	}
	else if (tempAngle > 225 && tempAngle < 270)
	{
		strFormatText = "WSW/%dбу";
		//strText.append(QString("WSW/%1бу").arg(tempAngle));
	}
	else if (tempAngle > 270 && tempAngle < 315)
	{
		strFormatText = "WNW/%dбу";
		//strText.append(QString("WNW/%1бу").arg(tempAngle));
	} 
	else if (tempAngle > 315 && tempAngle < 360)
	{
		strFormatText = "NWN/%dбу";
		//strText.append(QString("NWN/%1бу").arg(tempAngle));
	}

	sprintf(buf, strFormatText.data(), tempAngle);

	strFormatText = buf;

	strFinalText = strText + strFormatText;
}

osgText::Text* DrawWindSymbol::CreateText(const std::string name, float fSize, const osg::Vec3 pos, const osg::Vec4 Color)
{
	osgText::Text* pTop = new osgText::Text;
	osgText::Font* font = osgText::readFontFile("Fonts/simsun.ttc");
	pTop->setFont(font);
	SetText(pTop, name);
	pTop->setAutoRotateToScreen(true);
	pTop->setCharacterSizeMode(osgText::TextBase::SCREEN_COORDS);
	pTop->setPosition(pos);
	pTop->setColor(Color);
	pTop->setCharacterSize(fSize);
	return pTop;
}

void DrawWindSymbol::SetText( osgText::Text* pT, const std::string& strText )
{
	std::string wstrName = strText;
	setlocale( LC_ALL, "chs" );
	int requiredSize = mbstowcs(NULL, wstrName.c_str(), 0); 
	wchar_t* wtext = new wchar_t[requiredSize+1]; 
	mbstowcs(wtext, wstrName.c_str(), requiredSize+1);
	pT->setText(wtext);
	delete [] wtext; 
	wtext = NULL;
}

void DrawWindSymbol::CreateWind( float fSpeed, osg::Geometry* pWind )
{
	m_pWindVertexs->clear();

	pWind->removePrimitiveSet(0, pWind->getNumPrimitiveSets());
	pWind->setVertexArray(m_pWindVertexs);
	//m_pWindLine->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));

	if(m_WindColors->size() >= 1)
		(*m_WindColors)[0] = m_color;
	else
		m_WindColors->push_back(m_color);

	pWind->setColorArray(m_WindColors);
	pWind->setColorBinding(osg::Geometry::BIND_OVERALL);

	std::vector<osg::ref_ptr<osg::Vec3Array>> outputLine;
	m_Wind.WindAlgorithm(fSpeed, outputLine);

	for(int i = 0; i < outputLine.size(); i++)
	{

		unsigned iStartC = 0;
		osg::Vec3Array* pVec3Array = outputLine[i];
		unsigned iS = pVec3Array->size();
		iStartC = m_pWindVertexs->size();
		for(unsigned iC = 0; iC < iS; iC++)
		{
			m_pWindVertexs->push_back(pVec3Array->at(iC));
		}

		if(i == 0)
			pWind->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINE_STRIP, iStartC, iS));
		else
		{
			pWind->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINE_STRIP, iStartC, iS));
		}
	}
}

void DrawWindSymbol::CreateBoundBox(osg::BoundingBox& bb, osg::Geometry* pGeom)
{
	osg::ref_ptr<osg::Vec3Array> pVertexs = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> pColors = new osg::Vec4Array;

	pColors->push_back(osg::Vec4(1, 1, 1, 0.3));


	pVertexs->push_back(bb.corner(0));
	pVertexs->push_back(bb.corner(2));
	pVertexs->push_back(bb.corner(3));
	pVertexs->push_back(bb.corner(1));

	/*pVertexs->push_back(bb.corner(4));
	pVertexs->push_back(bb.corner(5));
	pVertexs->push_back(bb.corner(6));
	pVertexs->push_back(bb.corner(7));*/

	pGeom->setVertexArray(pVertexs);
	pGeom->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::QUADS, 0, pVertexs->size()));

	pGeom->setColorArray(pColors);
	pGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
}
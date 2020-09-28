#pragma once
#include "GraphicsControler_global.h"

class DrawGeometryData;

class GRAPHICSCONTROLER_EXPORT DrawGeometryManipulator : public osgGA::CameraManipulator
{
public:
	DrawGeometryManipulator(DrawGeometryData*);
	~DrawGeometryManipulator();
	virtual void setByMatrix( const osg::Matrixd& matrix );
	virtual void setByInverseMatrix( const osg::Matrixd& matrix );
	virtual osg::Matrixd getMatrix() const;
	virtual osg::Matrixd getInverseMatrix() const;
	virtual bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );
	void ScreenToGL(osg::Camera* pC, const osg::Vec3& screenPoint, osg::Vec3& glPoint );
	void ScreenToWorld( osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out, float fZ);
	void WorldToScreen( osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out );

	osg::Node* CreateCanvas(float fr, osg::Vec3 center, const osg::Vec4& v4C = osg::Vec4(1, 1, 1, 1));
	inline void SetCanvasColor(const osg::Vec4& v4c);
	inline void SetCanvasZ(float fZ);
	inline DrawGeometryData* GetDrawGeometryData(){return m_Data.get();}
	inline void SetDrawGeometryData(DrawGeometryData* pDrawGeometry){m_Data = pDrawGeometry;}
	inline void AddCanvasNode(osg::Node* pN){m_nodePath.push_back(pN);}
	inline void ClearCanvasNode(){m_nodePath.clear();}
private:
	osg::ref_ptr<DrawGeometryData> m_Data;
	osg::Matrixd m_Matrix;
	osg::Matrixd m_InverseMatrix;
	bool m_bStart;
	osg::ref_ptr<osg::Geode> m_pCanvasGeode;
	osg::ref_ptr<osg::Geometry> m_pCanvas;
	osg::NodePath m_nodePath;
};
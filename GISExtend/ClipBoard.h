#pragma once
#include <osg/Geometry>
#include <osg/AutoTransform>

class ClipBoard : public osg::Group
{
public:
	class ClipBoardMoveCallback : public osg::Referenced
	{
	public:
		virtual void MoveCompleted(osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Node* pClipNode, osg::Image** ppImage) = 0;
	};
public:
	ClipBoard(const osgEarth::SpatialReference* pSRS);
	virtual ~ClipBoard(void);
	void GetPlaneParm(double* dA, double* dB, double* dC, double* dD);
	void SetFourPointAndInitClipBoard(const osg::Vec3& p1, const osg::Vec3& p2, const osg::Vec3& p3, const osg::Vec3& p4);
	void SetPlaneColor(const osg::Vec4 v4Color);
	void SetBorderlineColor(const osg::Vec4 v4Color);
	void SetPlaneVisible(bool bV);
	void SetBoardlineVisible(bool bV);
	void MoveClipBoard(int iPosID, double dXDist, double dYDist);
	osg::ClipPlane* GetClipPlane();
	inline void SetClipMoveCallback(ClipBoard::ClipBoardMoveCallback* pCallBack){m_pClipBoardMove = pCallBack;};
	void SetPlaneImage(osg::Image* pImage);
private:
	void GetPalneABCD(const osg::Vec3& p1, const osg::Vec3& p2, const osg::Vec3& p3, const osg::Vec3& p4, double* dA, double* dB, double* dC, double* dD);
	osg::Geometry* MakeLine(const osg::Vec3& p1, const osg::Vec3& p2);
	osgText::Text* MakeHgtTxt(const osg::Vec3& p1, const std::string& strHgtTxt);
	void ComputePlane();
	int ComputeBorderline();
	void ReComputeLine();
	void CreateLine();
	void ComputePointOnLine(osg::Vec3& p1, osg::Vec3 p2, double dX, double* outDY);
	float ComputeKOfLine(osg::Vec3& p1, osg::Vec3 p2);
	void ComputeKPointLine(osg::Vec3& p, float fK, double dX, double* outY);
	osg::Vec3 m_v3P1, m_v3P2, m_v3P3, m_v3P4;
	osg::ref_ptr<osg::Switch> m_pClipBoradSwitch;
	osg::ref_ptr<osg::Geode> m_PlaneGeode;
	osg::ref_ptr<osg::Geode> m_BorderlineGeode;
	osg::ref_ptr<osg::Geode> m_ControlGeode0;
	osg::ref_ptr<osg::AutoTransform> m_AutoTransNode1;
	osg::ref_ptr<osg::Geode> m_ControlGeode1;
	osg::ref_ptr<osg::AutoTransform> m_AutoTransNode2;
	osg::ref_ptr<osg::Geode> m_ControlGeode2;
	osg::ref_ptr<osg::AutoTransform> m_AutoTransNode3;
	osg::ref_ptr<osg::Geode> m_ControlGeode3;
	osg::ref_ptr<osg::Geometry> m_PlaneGeometry;
	osg::ref_ptr<osg::Geometry> m_BorderlineGeometry;
	std::vector< osg::ref_ptr<osgText::Text> > m_TextVector;
	std::vector<osg::Geometry*> m_LineGeo;
	osg::ref_ptr<osg::Vec2Array> m_Vec2PlaneTextrue;
	osg::ref_ptr<osg::Vec3Array> m_Vec3PlaneVertexArray;
	osg::ref_ptr<osg::Vec4Array> m_Vec4PlaneColorArray;
	
	osg::ref_ptr<osg::Vec3Array> m_Vec3BorderlineVertexArray;
	osg::ref_ptr<osg::Vec4Array> m_Vec4BorderlineColorArray;
	
	osg::ref_ptr<osg::DrawArrays> m_PlanePrimitiveset;
	osg::ref_ptr<osg::DrawArrays> m_BorderlinePrimitiveset;
	
	osg::ref_ptr<osg::ClipPlane> m_pClipPlane;
	//osgSim::DOFTransform* pDofTransform;

	/*osg::ref_ptr<osg::Sphere> m_Contrl_Sphere1;
	osg::ref_ptr<osg::Sphere> m_Contrl_Sphere2;
	osg::ref_ptr<osg::Sphere> m_Contrl_Sphere3;

	osg::ref_ptr<osg::ShapeDrawable> m_ShapeDrawable1;
	osg::ref_ptr<osg::ShapeDrawable> m_ShapeDrawable2;
	osg::ref_ptr<osg::ShapeDrawable> m_ShapeDrawable3;*/

	osg::Sphere* m_Contrl_Sphere1;
	osg::Sphere* m_Contrl_Sphere2;
	osg::Sphere* m_Contrl_Sphere3;

	osg::ShapeDrawable* m_ShapeDrawable1;
	osg::ShapeDrawable* m_ShapeDrawable2;
	osg::ShapeDrawable* m_ShapeDrawable3;

	const osgEarth::SpatialReference* m_pSpatialRef;

	osg::ref_ptr<ClipBoardMoveCallback> m_pClipBoardMove;
	osg::Texture2D* m_Texture2D;
};


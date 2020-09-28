#pragma once
/** @file		goMark.h
* @brief		
* @author		杨晓宇
* @date			2013.4
* @version		1.0
  @note			
*/

#include <osg/LineWidth>
#include <osg/LineStipple>
#include <osg/Point>
#include <osgText/Text>
#include <osgText/TextBase>

#include "goConfig.h"
#include "CoordinateTransform.h"
#include "goGeometry.h"

GOTO_NAMESPACE_START
class Layer;

/** @class  goMark
    @brief  字库标注类
    @author 杨晓宇
    @note   成员功能参见接口类。
*/
class GEOSONTHEOSG_EXPORT goMark : public goGeometry
{
public:
	goMark(void);
	goMark(std::string markLabel, osg::Vec3 markPostion,Layer* pL);
	virtual ~goMark(void);

	virtual void CreateOsgFromGeos(osg::Node* pNode);
	virtual void SetCoordinateTransform(CoordinateTransform* pCT);
	virtual geom::GeometryTypeId GetGeometryType();
	virtual void TransformGeometry();

	inline void MarkPostion(osg::Vec3& pos){m_markPostion = pos;}
	inline osg::Vec3 MarkPostion(){return m_markPostion;}

	inline float MarkSize(){return m_markSize;}
	inline void MarkSize(float markSize){m_markSize = markSize;}
	inline void SetFont(std::string& strFontPath){m_strFontPath = strFontPath;}
	inline void SetTextDirection(float fa){m_fTextDirection = fa;}
	inline void UsedLayerSet(bool bs){m_bUsedSet = bs;}
	inline void SetTextColor(float r, float g, float b, float a){m_markColor.set(r, g,b,a);}
	inline void SetAlignment(osgText::Text::AlignmentType al){m_alignment = al;};
	inline osgText::Text* GetMarkText(){return m_markText;}
	inline void SetText(const std::string& strText){m_markLabel = strText;}
	inline void SetText(const std::wstring& wstrText){m_markText->setText(wstrText.data());}

	inline std::string GetLabel() { return m_markLabel;}
	inline osg::Vec3 GetPostion() { return m_markPostion;}

	virtual void operator=(const goMark& other);
private:
	std::string m_strFontPath;
	osgText::Text* m_markText;
	std::string m_markLabel;
	osg::Vec4 m_markColor;
	osg::Vec3 m_markPostion;
	float m_markSize;
	float m_fTextDirection;
	osgText::Text::AlignmentType m_alignment;
	bool m_bUsedSet;

};

GOTO_NAMESPACE_END
#pragma once
#include "cipasscalebarstyle.h"

class CCipasSingleBoxScaleBarStyle :public CCipasScaleBarStyle
{
public:
	CCipasSingleBoxScaleBarStyle(const CCipasComposerScaleBar* bar);
	~CCipasSingleBoxScaleBarStyle(void);
	 QString name() const;

    /*! draw method
     @param p painter object
     @param xOffset x offset
     */
    void draw( QPainter* p, double xOffset = 0 ) const;

  private:
    CCipasSingleBoxScaleBarStyle(); //forbidden
};


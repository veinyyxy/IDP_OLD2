#pragma once

#include <QIcon>
#include <QRectF>

#include "tmui_global.h"

class CCipasComposerScaleBar;
class QPainter;

class TMUI_EXPORT CCipasScaleBarStyle
{
public:
	CCipasScaleBarStyle(const CCipasComposerScaleBar* bar);
    virtual ~CCipasScaleBarStyle();

    /**Draws the style
     @param p painter object
     @param xOffset offset to account for centered labeling*/
    virtual void draw( QPainter* p, double xOffset = 0 ) const = 0; //to do by every subclass
    virtual void drawLabels( QPainter* p ) const; //default implementation provided
    virtual QRectF calculateBoxSize() const; //default implementation provided
    virtual QString name() const = 0; //return name of the style
    //virtual QIcon styleIcon() const = 0;

  private:
    CCipasScaleBarStyle(); //default constructor forbidden

  protected:
    const CCipasComposerScaleBar* mScaleBar;
};


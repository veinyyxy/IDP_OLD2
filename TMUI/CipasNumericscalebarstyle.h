#ifndef CIPASNUMERICSCALEBARSTYLE_H
#define CIPASNUMERICSCALEBARSTYLE_H

#include "tmui_global.h"
#include "CipasScaleBarstyle.h"


class TMUI_EXPORT CipasNumericScaleBarStyle: public CCipasScaleBarStyle
{
  public:
    CipasNumericScaleBarStyle( CCipasComposerScaleBar* bar );
    ~CipasNumericScaleBarStyle();

    QString name() const;

    void draw( QPainter* p, double xOffset = 0 ) const;

    //calculation of box size is different compared to segment based scale bars
    QRectF calculateBoxSize() const;

  private:
    CipasNumericScaleBarStyle(); //forbidden
    /**Returns the text for the scale bar or an empty string in case of error*/
    QString scaleText() const;
};

#endif

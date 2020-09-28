#ifndef CIPASDOUBLEBOXSCALEBARSTYLE_H
#define CIPASDOUBLEBOXSCALEBARSTYLE_H

#include "tmui_global.h"
#include "CipasScaleBarStyle.h"


class TMUI_EXPORT CipasDoubleBoxScaleBarStyle: public CCipasScaleBarStyle
{
  public:
    CipasDoubleBoxScaleBarStyle( const CCipasComposerScaleBar* bar );
    ~CipasDoubleBoxScaleBarStyle();

    QString name() const;

    void draw( QPainter* p, double xOffset = 0 ) const;

  private:
    CipasDoubleBoxScaleBarStyle(); //forbidden
};

#endif

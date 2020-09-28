#ifndef CIPASTICKSSCALEBARSTYLE_H
#define CIPASTICKSSCALEBARSTYLE_H

#include "tmui_global.h"
#include "CipasScaleBarstyle.h"


class TMUI_EXPORT CipasTicksScaleBarStyle: public CCipasScaleBarStyle
{
  public:
    enum TickPosition
    {
      TicksUp,
      TicksDown,
      TicksMiddle
    };

    CipasTicksScaleBarStyle( const CCipasComposerScaleBar* bar );
    ~CipasTicksScaleBarStyle();

    QString name() const;

    /*! draw method
     @param p painter object
     @param xOffset offset
     */
    void draw( QPainter* p, double xOffset = 0 ) const;

    void setTickPosition( TickPosition p ) {mTickPosition = p;}

  private:
    CipasTicksScaleBarStyle(); //forbidden

    TickPosition mTickPosition;
};

#endif

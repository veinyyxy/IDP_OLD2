#ifndef CIPASCOLORBUTTON_H
#define CIPASCOLORBUTTON_H

#include <QToolButton>
#include <QPushButton>

#include "tmui_global.h"

class TMUI_EXPORT CipasColorButton: public QToolButton
{
  public:
    CipasColorButton( QWidget *parent = 0 );
    ~CipasColorButton();

    void setColor( const QColor &color );
    QColor color() const { return mColor; }

  protected:
    void paintEvent( QPaintEvent *e );

  private:
    QColor mColor;
};


class TMUI_EXPORT CipasColorButtonV2 : public QPushButton
{
  public:
    CipasColorButtonV2( QWidget* parent = 0 );
    CipasColorButtonV2( QString text, QWidget* parent = 0 );

    void setColor( const QColor &color );
    QColor color() const { return mColor; }

  private:
    QColor mColor;
};


#endif

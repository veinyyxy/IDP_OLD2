#include <QPainter>

#include "cipascolorbutton.h"

CipasColorButton::CipasColorButton( QWidget *parent )  : QToolButton( parent )
{
  setToolButtonStyle( Qt::ToolButtonTextOnly ); // decrease default button height
}

CipasColorButton::~CipasColorButton()
{}

/*!
  Paints button in response to a paint event.
*/
void CipasColorButton::paintEvent( QPaintEvent *e )
{
  QToolButton::paintEvent( e );
  if (
#ifdef Q_WS_MAC
    // Mac shows color only a when a window is active
    isActiveWindow() &&
#endif
    isEnabled() )
  {
    QPainter p( this );
    int margin = 2;  // Leave some space for highlighting
    QRect r = rect().adjusted( margin, margin, -margin, -margin );
    p.fillRect( r, mColor );
  }
}

void CipasColorButton::setColor( const QColor &color )
{
  mColor = color;
  update();
}


//////////////////

CipasColorButtonV2::CipasColorButtonV2( QWidget* parent )
    : QPushButton( parent )
{
}

CipasColorButtonV2::CipasColorButtonV2( QString text, QWidget* parent )
    : QPushButton( text, parent )
{
}

void CipasColorButtonV2::setColor( const QColor &color )
{
  mColor = color;

  QPixmap pixmap( iconSize() );
  pixmap.fill( QColor( 0, 0, 0, 0 ) );

  QRect rect( 1, 1, iconSize().width() - 2, iconSize().height() - 2 );

  // draw a slightly rounded rectangle
  QPainter p;
  p.begin( &pixmap );
  p.setPen( Qt::NoPen );
  p.setRenderHint( QPainter::Antialiasing );
  p.setBrush( color );
  p.drawRoundedRect( rect, 4, 4 );
  p.end();

  // set this pixmap as icon
  setIcon( QIcon( pixmap ) );
}

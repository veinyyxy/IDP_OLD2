#include <QStandardItem>
#include "Cipascomposerlegenditemdialog.h"

CipasComposerLegendItemDialog::CipasComposerLegendItemDialog(const QStandardItem* item , QWidget* parent ): QDialog( parent )
{
  setupUi( this );

  if ( item )
  {
    mItemTextLineEdit->setText( item->text() );
  }
}


CipasComposerLegendItemDialog::~CipasComposerLegendItemDialog()
{

}

QString CipasComposerLegendItemDialog::itemText() const
{
  return mItemTextLineEdit->text();
}

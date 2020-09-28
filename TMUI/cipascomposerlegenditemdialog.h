#ifndef CIPASCOMPOSERLEGENDITEMDIALOG_H
#define CIPASCOMPOSERLEGENDITEMDIALOG_H

#include "ui_Cipascomposerlegenditemdialogbase.h"

class QStandardItem;

/** \ingroup MapComposer
 * A dialog to enter properties of composer legend items (currently only item text)
 * */
class CipasComposerLegendItemDialog: private Ui::CipasComposerLegendItemDialogBase, public QDialog
{
  public:
    CipasComposerLegendItemDialog(const QStandardItem* item , QWidget* parent = 0 );
    ~CipasComposerLegendItemDialog();

    /**Returns the item text inserted by user*/
    QString itemText() const;

  //private:
  //  CipasComposerLegendItemDialog(); //forbidden
};

#endif //CIPASCOMPOSERLEGENDITEMDIALOG_H

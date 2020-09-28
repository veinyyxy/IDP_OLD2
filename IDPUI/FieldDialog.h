#ifndef FIELDDIALOG_H
#define FIELDDIALOG_H

#include <QDialog>
#include "ui_FieldDialog.h"
//#include "goConfig.h"
#include "Layer.h"
#include "Field.h"

using namespace goto_gis;

#include "idpui_global.h"


class IDPUI_EXPORT  FieldDialog : public QDialog
{
	Q_OBJECT

public:
	FieldDialog(Layer* layer ,QWidget *parent = 0);
	~FieldDialog();

public:
	void InitDialog(Layer* layer);
private:
	Ui::FieldDialog ui;
};

#endif // FIELDDIALOG_H

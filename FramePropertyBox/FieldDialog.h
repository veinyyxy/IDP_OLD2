#ifndef FIELDDIALOG_H
#define FIELDDIALOG_H

#include <QDialog>
#include "./GeneratedFiles/ui_FieldDialog.h"
//#include "goConfig.h"
#include "Layer.h"
#include "Field.h"

using namespace goto_gis;

class  FieldDialog : public QDialog
{
	Q_OBJECT

public:
	FieldDialog(goto_gis::Layer* layer ,QWidget *parent = 0);
	~FieldDialog();

public:
	void InitDialog(goto_gis::Layer* layer);
private:
	Ui::FieldDialog ui;
};

#endif // FIELDDIALOG_H

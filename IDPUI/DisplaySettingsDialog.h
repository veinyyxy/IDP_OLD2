#ifndef DISPLAYSETTINGSDIALOG_H
#define DISPLAYSETTINGSDIALOG_H

#include <QtGui>
#include "ui_DisplaySettingsDialog.h"

class DisplaySettingsDialog : public QDialog
{
	Q_OBJECT

public:
	DisplaySettingsDialog(QWidget *parent = 0);
	~DisplaySettingsDialog();
	void StackedWidgetCurrentIndex(int index = 0);
protected Q_SLOTS:
	void Slot_ChangePage(QListWidgetItem *current, QListWidgetItem *previous);
	void Slot_OK();
	void Slot_PushButtonClicked();
private:
	Ui::DisplaySettingsDialog ui;
};

#endif // DISPLAYSETTINGSDIALOG_H

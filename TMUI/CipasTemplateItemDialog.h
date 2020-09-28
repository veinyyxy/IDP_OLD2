#pragma once
#include <QtGui/QDialog>

class QTreeWidgetItem;
class QLineEdit;

class CCipasTemplateItemDialog :
	public QDialog
{
public:
	CCipasTemplateItemDialog(QTreeWidgetItem *item, QWidget* parent = 0 );
	~CCipasTemplateItemDialog(void);
	QString itemText() const;
private:
	void setUI(QDialog *dialog);

	QLineEdit *mLineEdit;
};


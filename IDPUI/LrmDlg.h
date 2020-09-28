#pragma once
#include <QDialog>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QList>
#include "idpui_global.h"

class IDPUI_EXPORT LrmDlg :public QDialog
{
	Q_OBJECT
public:
	LrmDlg(QWidget* parent = 0);
	~LrmDlg(void);
	void SetLayHigh(QStringList* pStrList){LayHigh = pStrList;}
public slots:
	void LrmConfig(int);
	void OkClicked();
signals:
	void OkDraw();
private:
	QGroupBox *m_MainGrp;
	QGridLayout* m_TopGLayout;
	QGridLayout* m_BottonGLayout;
	QVBoxLayout *m_MainGrpHLay;
	QList<QWidget*> m_pWidgetList;
	QStringList* LayHigh;
	QVBoxLayout *m_MainHLay;
	QHBoxLayout *m_buttonLay;
};


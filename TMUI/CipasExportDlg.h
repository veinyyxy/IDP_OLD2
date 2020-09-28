#pragma once
#include <QDialog>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>

class CCipasComposition;

class  CipasExportDlg : 	public QDialog
{
	Q_OBJECT
public:
	CipasExportDlg(CCipasComposition* c,QString inputs,QWidget* parent = 0);
	~CipasExportDlg(void);
	QString SaveFileName()const;
	int SaveDpi()const;
private:
	void SetupUI(QDialog *dialog);
	void connectSlots();
	void InitDlg();
public slots:	
	void on_pushButton_dlg_clicked();
	void on_okpushButton_clicked();
	void on_spinBox_valueChanged ( int );
	void on_lineEdit_sp_textChanged(const QString & text);
private:
	CCipasComposition *mComposition;
	QString mTemplateName;

	QGridLayout *gridLayout_3;
	QGroupBox *groupBox_option;
	QFormLayout *formLayout;
	QGridLayout *gridLayout_op;
	QLabel *label_w;
	QLabel *label_hp;
	QLineEdit *lineEdit_h;
	QSpinBox *spinBox;
	QLabel *label_h;
	QLineEdit *lineEdit_w;
	QLabel *label_wp;
	QLabel *label_r;
	QLabel *label_dpi;
	QGroupBox *groupBox_dlg;
	QFormLayout *formLayout_2;
	QGridLayout *gridLayout_sp;
	QLabel *label_sp;
	QLineEdit *lineEdit_sp;
	QPushButton *pushButton_dlg;
	QDialogButtonBox *buttonBox;
};


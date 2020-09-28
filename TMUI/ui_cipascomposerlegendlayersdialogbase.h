/********************************************************************************
** Form generated from reading UI file 'qgscomposerlegendlayersdialogbase.ui'
**
** Created: Wed Aug 31 17:44:46 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIPASCOMPOSERLEGENDLAYERSDIALOGBASE_H
#define UI_CIPASCOMPOSERLEGENDLAYERSDIALOGBASE_H

#include <QtCore/QVariant>
#include <QtGui>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QListWidget>

QT_BEGIN_NAMESPACE

class Ui_CipasComposerLegendLayersDialogBase
{
public:
    QGridLayout *gridLayout;
    QListWidget *listMapLayers;
    QDialogButtonBox *buttonBox;
	QLabel *labelName;
	QLineEdit *lineEditTitle;
	QPushButton *pushButton;
	QListWidget *listWidget;
	QPushButton *pushButtonAdd;
	QPushButton *pushButtonDel;
	QLineEdit *lineEditPix;

    void setupUi(QDialog *CipasComposerLegendLayersDialogBase)
    {
        if (CipasComposerLegendLayersDialogBase->objectName().isEmpty())
            CipasComposerLegendLayersDialogBase->setObjectName(QString::fromUtf8("CipasComposerLegendLayersDialogBase"));
        CipasComposerLegendLayersDialogBase->resize(252, 194);
        gridLayout = new QGridLayout(CipasComposerLegendLayersDialogBase);
		gridLayout->setContentsMargins(0,0,0,0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        //listMapLayers = new QListWidget(CipasComposerLegendLayersDialogBase);
        //listMapLayers->setObjectName(QString::fromUtf8("listMapLayers"));

        //gridLayout->addWidget(listMapLayers, 0, 0, 1, 1);

		labelName = new QLabel(QString::fromLocal8Bit("标题"),CipasComposerLegendLayersDialogBase);
		labelName->setObjectName(QString::fromUtf8("label"));

		gridLayout->addWidget(labelName, 1, 0, 1, 1);

		lineEditTitle = new QLineEdit(CipasComposerLegendLayersDialogBase);
		lineEditTitle->setObjectName(QString::fromUtf8("lineEdit"));

		gridLayout->addWidget(lineEditTitle, 1, 1, 1, 1);

		pushButton = new QPushButton(QString::fromLocal8Bit("浏览图片"),CipasComposerLegendLayersDialogBase);
		pushButton->setObjectName(QString::fromUtf8("pushButton"));

		gridLayout->addWidget(pushButton, 1, 3, 1, 1);

		buttonBox = new QDialogButtonBox(CipasComposerLegendLayersDialogBase);
		buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
		buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

		gridLayout->addWidget(buttonBox, 2, 0, 1, 6);

		listWidget = new QListWidget(CipasComposerLegendLayersDialogBase);

		listWidget->setObjectName(QString::fromUtf8("listWidget"));
		listWidget->setViewMode(QListView::ListMode);

		gridLayout->addWidget(listWidget, 0, 0, 1, 6);

		pushButtonAdd = new QPushButton("+",CipasComposerLegendLayersDialogBase);
		pushButtonAdd->setObjectName(QString::fromUtf8("pushButtonAdd"));
		pushButtonAdd->setMaximumWidth(23);
		gridLayout->addWidget(pushButtonAdd, 1, 4, 1, 1);

		pushButtonDel = new QPushButton("-",CipasComposerLegendLayersDialogBase);
		pushButtonDel->setObjectName(QString::fromUtf8("pushButtonDel"));
		pushButtonDel->setMaximumWidth(23);
		gridLayout->addWidget(pushButtonDel, 1, 5, 1, 1);

		lineEditPix = new QLineEdit(CipasComposerLegendLayersDialogBase);
		lineEditPix->setObjectName(QString::fromUtf8("lineEditPix"));

		gridLayout->addWidget(lineEditPix, 1, 2, 1, 1);

        retranslateUi(CipasComposerLegendLayersDialogBase);
        QObject::connect(buttonBox, SIGNAL(accepted()), CipasComposerLegendLayersDialogBase, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CipasComposerLegendLayersDialogBase, SLOT(reject()));

        QMetaObject::connectSlotsByName(CipasComposerLegendLayersDialogBase);
    } // setupUi

    void retranslateUi(QDialog *CipasComposerLegendLayersDialogBase)
    {
        CipasComposerLegendLayersDialogBase->setWindowTitle(("增加图层到图例"));
    } // retranslateUi

};

namespace Ui {
    class CipasComposerLegendLayersDialogBase: public Ui_CipasComposerLegendLayersDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIPASCOMPOSERLEGENDLAYERSDIALOGBASE_H

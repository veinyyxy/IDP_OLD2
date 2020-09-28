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
#include <QtGui/QAction>
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

    void setupUi(QDialog *CipasComposerLegendLayersDialogBase)
    {
        if (CipasComposerLegendLayersDialogBase->objectName().isEmpty())
            CipasComposerLegendLayersDialogBase->setObjectName(QString::fromUtf8("CipasComposerLegendLayersDialogBase"));
        CipasComposerLegendLayersDialogBase->resize(252, 194);
        gridLayout = new QGridLayout(CipasComposerLegendLayersDialogBase);
		gridLayout->setContentsMargins(0,0,0,0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listMapLayers = new QListWidget(CipasComposerLegendLayersDialogBase);
        listMapLayers->setObjectName(QString::fromUtf8("listMapLayers"));

        gridLayout->addWidget(listMapLayers, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(CipasComposerLegendLayersDialogBase);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(CipasComposerLegendLayersDialogBase);
        QObject::connect(buttonBox, SIGNAL(accepted()), CipasComposerLegendLayersDialogBase, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CipasComposerLegendLayersDialogBase, SLOT(reject()));

        QMetaObject::connectSlotsByName(CipasComposerLegendLayersDialogBase);
    } // setupUi

    void retranslateUi(QDialog *CipasComposerLegendLayersDialogBase)
    {
        CipasComposerLegendLayersDialogBase->setWindowTitle(QApplication::translate("CipasComposerLegendLayersDialogBase", "Add layer to legend", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class CipasComposerLegendLayersDialogBase: public Ui_CipasComposerLegendLayersDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIPASCOMPOSERLEGENDLAYERSDIALOGBASE_H

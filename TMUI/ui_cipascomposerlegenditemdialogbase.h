/********************************************************************************
** Form generated from reading UI file 'qgscomposerlegenditemdialogbase.ui'
**
** Created: Wed Aug 31 17:44:46 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIPASCOMPOSERLEGENDITEMDIALOGBASE_H
#define UI_CIPASCOMPOSERLEGENDITEMDIALOGBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_CipasComposerLegendItemDialogBase
{
public:
    QGridLayout *gridLayout;
    QLabel *mItemTextLabel;
    QLineEdit *mItemTextLineEdit;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CipasComposerLegendItemDialogBase)
    {
        if (CipasComposerLegendItemDialogBase->objectName().isEmpty())
            CipasComposerLegendItemDialogBase->setObjectName(QString::fromUtf8("CipasComposerLegendItemDialogBase"));
        CipasComposerLegendItemDialogBase->resize(360, 91);
        gridLayout = new QGridLayout(CipasComposerLegendItemDialogBase);
		gridLayout->setContentsMargins(0,0,0,0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mItemTextLabel = new QLabel(CipasComposerLegendItemDialogBase);
        mItemTextLabel->setObjectName(QString::fromUtf8("mItemTextLabel"));

        gridLayout->addWidget(mItemTextLabel, 0, 0, 1, 1);

        mItemTextLineEdit = new QLineEdit(CipasComposerLegendItemDialogBase);
        mItemTextLineEdit->setObjectName(QString::fromUtf8("mItemTextLineEdit"));

        gridLayout->addWidget(mItemTextLineEdit, 0, 1, 1, 1);

        buttonBox = new QDialogButtonBox(CipasComposerLegendItemDialogBase);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 2);

#ifndef QT_NO_SHORTCUT
        mItemTextLabel->setBuddy(mItemTextLineEdit);
#endif // QT_NO_SHORTCUT

        retranslateUi(CipasComposerLegendItemDialogBase);
        QObject::connect(buttonBox, SIGNAL(accepted()), CipasComposerLegendItemDialogBase, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CipasComposerLegendItemDialogBase, SLOT(reject()));

        QMetaObject::connectSlotsByName(CipasComposerLegendItemDialogBase);
    } // setupUi

    void retranslateUi(QDialog *CipasComposerLegendItemDialogBase)
    {
        CipasComposerLegendItemDialogBase->setWindowTitle(("编辑项目内容"));
        mItemTextLabel->setText(("项目文本"));
    } // retranslateUi

};

namespace Ui {
    class CipasComposerLegendItemDialogBase: public Ui_CipasComposerLegendItemDialogBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIPASCOMPOSERLEGENDITEMDIALOGBASE_H

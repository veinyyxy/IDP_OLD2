/********************************************************************************
** Form generated from reading UI file 'qgscomposerarrowwidgetbase.ui'
**
** Created: Wed Aug 31 17:44:46 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIPASCOMPOSERARROWWIDGETBASE_H
#define UI_CIPASCOMPOSERARROWWIDGETBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolBox>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CipasComposerArrowWidgetBase
{
public:
    QGridLayout *gridLayout_3;
    QToolBox *toolBox;
    QWidget *page;
    QGridLayout *gridLayout_2;
    QPushButton *mArrowColorButton;
    QDoubleSpinBox *mOutlineWidthSpinBox;
    QDoubleSpinBox *mArrowHeadWidthSpinBox;
    QGroupBox *mArrowMarkersGroupBox;
    QGridLayout *gridLayout_1;
    QRadioButton *mDefaultMarkerRadioButton;
    QRadioButton *mNoMarkerRadioButton;
   // QRadioButton *mSvgMarkerRadioButton;
    //QLabel *mStartMarkerLabel;
    //QLineEdit *mStartMarkerLineEdit;
    //QToolButton *mStartMarkerToolButton;
   // QLabel *mEndMarkerLabel;
    //QLineEdit *mEndMarkerLineEdit;
    //QToolButton *mEndMarkerToolButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *CipasComposerArrowWidgetBase)
    {
        if (CipasComposerArrowWidgetBase->objectName().isEmpty())
            CipasComposerArrowWidgetBase->setObjectName(QString::fromUtf8("CipasComposerArrowWidgetBase"));
        CipasComposerArrowWidgetBase->resize(196, 407);
        gridLayout_3 = new QGridLayout(CipasComposerArrowWidgetBase);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
		gridLayout_3->setContentsMargins(0,0,0,0);
        toolBox = new QToolBox(CipasComposerArrowWidgetBase);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 178, 363));
        gridLayout_2 = new QGridLayout(page);
		gridLayout_2->setContentsMargins(0,0,0,0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        mArrowColorButton = new QPushButton(page);
        mArrowColorButton->setObjectName(QString::fromUtf8("mArrowColorButton"));

        gridLayout_2->addWidget(mArrowColorButton, 0, 0, 1, 1);

        mOutlineWidthSpinBox = new QDoubleSpinBox(page);
		mOutlineWidthSpinBox->setMinimum(1.0);
        mOutlineWidthSpinBox->setObjectName(QString::fromUtf8("mOutlineWidthSpinBox"));

        gridLayout_2->addWidget(mOutlineWidthSpinBox, 1, 0, 1, 1);

        mArrowHeadWidthSpinBox = new QDoubleSpinBox(page);
        mArrowHeadWidthSpinBox->setObjectName(QString::fromUtf8("mArrowHeadWidthSpinBox"));

        gridLayout_2->addWidget(mArrowHeadWidthSpinBox, 2, 0, 1, 1);

        mArrowMarkersGroupBox = new QGroupBox(page);
        mArrowMarkersGroupBox->setObjectName(QString::fromUtf8("mArrowMarkersGroupBox"));
        gridLayout_1 = new QGridLayout(mArrowMarkersGroupBox);
		gridLayout_1->setContentsMargins(0,0,0,0);
        gridLayout_1->setObjectName(QString::fromUtf8("gridLayout_1"));
        mDefaultMarkerRadioButton = new QRadioButton(mArrowMarkersGroupBox);
        mDefaultMarkerRadioButton->setObjectName(QString::fromUtf8("mDefaultMarkerRadioButton"));

        gridLayout_1->addWidget(mDefaultMarkerRadioButton, 0, 0, 1, 2);

        mNoMarkerRadioButton = new QRadioButton(mArrowMarkersGroupBox);
        mNoMarkerRadioButton->setObjectName(QString::fromUtf8("mNoMarkerRadioButton"));
		
        gridLayout_1->addWidget(mNoMarkerRadioButton, 1, 0, 1, 1);
		/*
        mSvgMarkerRadioButton = new QRadioButton(mArrowMarkersGroupBox);
        mSvgMarkerRadioButton->setObjectName(QString::fromUtf8("mSvgMarkerRadioButton"));

        gridLayout_1->addWidget(mSvgMarkerRadioButton, 2, 0, 1, 2);

        mStartMarkerLabel = new QLabel(mArrowMarkersGroupBox);
        mStartMarkerLabel->setObjectName(QString::fromUtf8("mStartMarkerLabel"));

        gridLayout_1->addWidget(mStartMarkerLabel, 3, 0, 1, 1);

        mStartMarkerLineEdit = new QLineEdit(mArrowMarkersGroupBox);
        mStartMarkerLineEdit->setObjectName(QString::fromUtf8("mStartMarkerLineEdit"));

        gridLayout_1->addWidget(mStartMarkerLineEdit, 4, 0, 1, 1);

        mStartMarkerToolButton = new QToolButton(mArrowMarkersGroupBox);
        mStartMarkerToolButton->setObjectName(QString::fromUtf8("mStartMarkerToolButton"));

        gridLayout_1->addWidget(mStartMarkerToolButton, 4, 1, 1, 1);

        mEndMarkerLabel = new QLabel(mArrowMarkersGroupBox);
        mEndMarkerLabel->setObjectName(QString::fromUtf8("mEndMarkerLabel"));

        gridLayout_1->addWidget(mEndMarkerLabel, 5, 0, 1, 1);

        mEndMarkerLineEdit = new QLineEdit(mArrowMarkersGroupBox);
        mEndMarkerLineEdit->setObjectName(QString::fromUtf8("mEndMarkerLineEdit"));

        gridLayout_1->addWidget(mEndMarkerLineEdit, 6, 0, 1, 1);

        mEndMarkerToolButton = new QToolButton(mArrowMarkersGroupBox);
        mEndMarkerToolButton->setObjectName(QString::fromUtf8("mEndMarkerToolButton"));

        gridLayout_1->addWidget(mEndMarkerToolButton, 6, 1, 1, 1);

		*/
        gridLayout_2->addWidget(mArrowMarkersGroupBox, 3, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 62, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 4, 0, 1, 1);

        toolBox->addItem(page, QString::fromUtf8("Arrow"));

        gridLayout_3->addWidget(toolBox, 0, 0, 1, 1);


        retranslateUi(CipasComposerArrowWidgetBase);

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CipasComposerArrowWidgetBase);
    } // setupUi

    void retranslateUi(QWidget *CipasComposerArrowWidgetBase)
    {
        CipasComposerArrowWidgetBase->setWindowTitle(QApplication::translate("CipasComposerArrowWidgetBase", "Form", 0, QApplication::UnicodeUTF8));
        mArrowColorButton->setText(QString::fromLocal8Bit("箭头颜色..."));
        mOutlineWidthSpinBox->setPrefix(QString::fromLocal8Bit("线宽度"));
        mArrowHeadWidthSpinBox->setPrefix(QString::fromLocal8Bit("箭头宽度"));
        mArrowMarkersGroupBox->setTitle(QString::fromLocal8Bit("设置箭头"));
        mDefaultMarkerRadioButton->setText(QString::fromLocal8Bit("箭头"));
        mNoMarkerRadioButton->setText(QString::fromLocal8Bit("无箭头"));
       // mSvgMarkerRadioButton->setText(QApplication::translate("CipasComposerArrowWidgetBase", "SVG markers", 0, QApplication::UnicodeUTF8));
       // mStartMarkerLabel->setText(QApplication::translate("CipasComposerArrowWidgetBase", "Start marker", 0, QApplication::UnicodeUTF8));
       // mStartMarkerToolButton->setText(QApplication::translate("CipasComposerArrowWidgetBase", "...", 0, QApplication::UnicodeUTF8));
        //mEndMarkerLabel->setText(QApplication::translate("CipasComposerArrowWidgetBase", "End marker", 0, QApplication::UnicodeUTF8));
       // mEndMarkerToolButton->setText(QApplication::translate("CipasComposerArrowWidgetBase", "...", 0, QApplication::UnicodeUTF8));
        toolBox->setItemText(toolBox->indexOf(page),QString::fromLocal8Bit("箭头选项"));
    } // retranslateUi

};

namespace Ui {
    class CipasComposerArrowWidgetBase: public Ui_CipasComposerArrowWidgetBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CIPASCOMPOSERARROWWIDGETBASE_H

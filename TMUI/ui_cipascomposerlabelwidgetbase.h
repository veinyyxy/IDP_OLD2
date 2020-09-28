/********************************************************************************
** Form generated from reading UI file 'qgscomposerlabelwidgetbase.ui'
**
** Created: Wed Aug 31 17:44:46 2011
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CIPASCOMPOSERLABELWIDGETBASE_H
#define UI_CIPASCOMPOSERLABELWIDGETBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBox>
#include <QtGui/QWidget>
#include <QtGui/QCheckBox>

QT_BEGIN_NAMESPACE

class Ui_CipasComposerLabelWidgetBase
{
public:
    QGridLayout *gridLayout_2;
    QToolBox *toolBox;
    QWidget *page;
	QLabel *labelDescription;
    QGridLayout *gridLayout;
    QTextEdit *mTextEdit;
    QPushButton *mFontButton;
    QPushButton *mFontColorButton;
    QLabel *mMarginTextLabel;
	QCheckBox *advanceLabeCheckBox;
    QDoubleSpinBox *mMarginDoubleSpinBox;
    QGroupBox *buttonGroup1;
    QHBoxLayout *horizontalLayout;
    QRadioButton *mLeftRadioButton;
    QRadioButton *mCenterRadioButton;
    QRadioButton *mRightRadioButton;
    QGroupBox *buttonGroup2;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *mTopRadioButton;
    QRadioButton *mMiddleRadioButton;
    QRadioButton *mBottomRadioButton;
    QLineEdit *mLabelIdLineEdit;
    QLabel *mIdLabel;

    void setupUi(QWidget *CipasComposerLabelWidgetBase)
    {
        if (CipasComposerLabelWidgetBase->objectName().isEmpty())
            CipasComposerLabelWidgetBase->setObjectName(QString::fromUtf8("CipasComposerLabelWidgetBase"));
        CipasComposerLabelWidgetBase->resize(547, 425);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CipasComposerLabelWidgetBase->sizePolicy().hasHeightForWidth());
        CipasComposerLabelWidgetBase->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(CipasComposerLabelWidgetBase);
		gridLayout_2->setContentsMargins(0,0,0,0);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        toolBox = new QToolBox(CipasComposerLabelWidgetBase);
        toolBox->setObjectName(QString::fromUtf8("toolBox"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        page->setGeometry(QRect(0, 0, 513, 402));
        gridLayout = new QGridLayout(page);
		gridLayout->setContentsMargins(0,0,0,0);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));

		labelDescription=new QLabel(page);
		labelDescription->setObjectName(QString::fromUtf8("labelDescription"));
		labelDescription->setText(QObject::tr("标签内容"));
		gridLayout->addWidget(labelDescription, 0, 0, 1, 1);

		advanceLabeCheckBox=new QCheckBox(page);
		advanceLabeCheckBox->setObjectName(QString::fromUtf8("advanceLabeCheckBox"));
		advanceLabeCheckBox->setText(QObject::tr("高级"));
		gridLayout->addWidget(advanceLabeCheckBox, 0, 1, 1, 1);

        mTextEdit = new QTextEdit(page);
        mTextEdit->setObjectName(QString::fromUtf8("mTextEdit"));
        mTextEdit->setLineWrapMode(QTextEdit::NoWrap);

        gridLayout->addWidget(mTextEdit, 1, 0, 1, 2);

        mFontButton = new QPushButton(page);
        mFontButton->setObjectName(QString::fromUtf8("mFontButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(mFontButton->sizePolicy().hasHeightForWidth());
        mFontButton->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(mFontButton, 2, 0, 1, 2);

        mFontColorButton = new QPushButton(page);
        mFontColorButton->setObjectName(QString::fromUtf8("mFontColorButton"));

        gridLayout->addWidget(mFontColorButton, 3, 0, 1, 2);

        mMarginTextLabel = new QLabel(page);
        mMarginTextLabel->setObjectName(QString::fromUtf8("mMarginTextLabel"));


        gridLayout->addWidget(mMarginTextLabel, 7, 0, 1, 2);

        mMarginDoubleSpinBox = new QDoubleSpinBox(page);
        mMarginDoubleSpinBox->setObjectName(QString::fromUtf8("mMarginDoubleSpinBox"));

        gridLayout->addWidget(mMarginDoubleSpinBox, 8, 0, 1, 2);

        buttonGroup1 = new QGroupBox(page);
        buttonGroup1->setObjectName(QString::fromUtf8("buttonGroup1"));
        horizontalLayout = new QHBoxLayout(buttonGroup1);
		horizontalLayout->setContentsMargins(0,0,0,0);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        mLeftRadioButton = new QRadioButton(buttonGroup1);
        mLeftRadioButton->setObjectName(QString::fromUtf8("mLeftRadioButton"));

        horizontalLayout->addWidget(mLeftRadioButton);

        mCenterRadioButton = new QRadioButton(buttonGroup1);
        mCenterRadioButton->setObjectName(QString::fromUtf8("mCenterRadioButton"));

        horizontalLayout->addWidget(mCenterRadioButton);

        mRightRadioButton = new QRadioButton(buttonGroup1);
        mRightRadioButton->setObjectName(QString::fromUtf8("mRightRadioButton"));

        horizontalLayout->addWidget(mRightRadioButton);


        gridLayout->addWidget(buttonGroup1, 4, 0, 1, 2);

        buttonGroup2 = new QGroupBox(page);
        buttonGroup2->setObjectName(QString::fromUtf8("buttonGroup2"));
        horizontalLayout_2 = new QHBoxLayout(buttonGroup2);
		horizontalLayout_2->setContentsMargins(0,0,0,0);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        mTopRadioButton = new QRadioButton(buttonGroup2);
        mTopRadioButton->setObjectName(QString::fromUtf8("mTopRadioButton"));

        horizontalLayout_2->addWidget(mTopRadioButton);

        mMiddleRadioButton = new QRadioButton(buttonGroup2);
        mMiddleRadioButton->setObjectName(QString::fromUtf8("mMiddleRadioButton"));

        horizontalLayout_2->addWidget(mMiddleRadioButton);

        mBottomRadioButton = new QRadioButton(buttonGroup2);
        mBottomRadioButton->setObjectName(QString::fromUtf8("mBottomRadioButton"));

        horizontalLayout_2->addWidget(mBottomRadioButton);


        gridLayout->addWidget(buttonGroup2, 5, 0, 1, 2);

        mLabelIdLineEdit = new QLineEdit(page);
        mLabelIdLineEdit->setObjectName(QString::fromUtf8("mLabelIdLineEdit"));

        gridLayout->addWidget(mLabelIdLineEdit, 10, 0, 1, 1);

        mIdLabel = new QLabel(page);
        mIdLabel->setObjectName(QString::fromUtf8("mIdLabel"));

        gridLayout->addWidget(mIdLabel, 9, 0, 1, 2);

        toolBox->addItem(page, QString::fromUtf8("Label"));

        gridLayout_2->addWidget(toolBox, 0, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        mMarginTextLabel->setBuddy(mMarginDoubleSpinBox);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(mTextEdit, mFontButton);
        QWidget::setTabOrder(mFontButton, mFontColorButton);
        QWidget::setTabOrder(mFontColorButton, mMarginDoubleSpinBox);

        retranslateUi(CipasComposerLabelWidgetBase);

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(CipasComposerLabelWidgetBase);
    } // setupUi

    void retranslateUi(QWidget *CipasComposerLabelWidgetBase)
    {
        CipasComposerLabelWidgetBase->setWindowTitle(QApplication::translate("CipasComposerLabelWidgetBase", "Label Options", 0, QApplication::UnicodeUTF8));
        mFontButton->setText(QString::fromLocal8Bit("字体"));
        mFontColorButton->setText(QString::fromLocal8Bit("字体颜色..."));
        mMarginTextLabel->setText(QString::fromLocal8Bit("边距 (毫米)"));
        buttonGroup1->setTitle(QString::fromLocal8Bit("水平对齐"));
        mLeftRadioButton->setText(QString::fromLocal8Bit("左"));
        mCenterRadioButton->setText(QString::fromLocal8Bit("中"));
        mRightRadioButton->setText(QString::fromLocal8Bit("右"));
        buttonGroup2->setTitle(QString::fromLocal8Bit("垂直对齐"));
        mTopRadioButton->setText(QString::fromLocal8Bit("上"));
        mMiddleRadioButton->setText(QString::fromLocal8Bit("中"));
        mBottomRadioButton->setText(QString::fromLocal8Bit("下"));
        mIdLabel->setText(QString::fromLocal8Bit("标签标识"));
        toolBox->setItemText(toolBox->indexOf(page), QString::fromLocal8Bit("标签"));
    } // retranslateUi

};

namespace Ui {
    class CipasComposerLabelWidgetBase: public Ui_CipasComposerLabelWidgetBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGSCOMPOSERLABELWIDGETBASE_H

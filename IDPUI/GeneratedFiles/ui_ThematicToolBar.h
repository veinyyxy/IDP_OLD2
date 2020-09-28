/********************************************************************************
** Form generated from reading UI file 'ThematicToolBar.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_THEMATICTOOLBAR_H
#define UI_THEMATICTOOLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QToolBar>

QT_BEGIN_NAMESPACE

class Ui_ThematicToolBar
{
public:
    QAction *mActionPrint;
    QAction *mActionZoomAll;
    QAction *mActionZoomIn;
    QAction *mActionZoomOut;
    QAction *mActionMapZoomAll;
    QAction *mActionMapZoomIn;
    QAction *mActionMapZoomOut;
    QAction *mActionAddGLW;
    QAction *mActionAddNewLabel;
    QAction *mActionAddNewLegend;
    QAction *mActionSelectMoveItem;
    QAction *mActionExportAsImage;
    QAction *mActionExport2Clipboard;
    QAction *mActionExportAsPDF;
    QAction *mActionExportAsEPS;
    QAction *mActionExportAsSVG;
    QAction *mActionAddNewScalebar;
    QAction *mActionRefreshView;
    QAction *mActionAddImage;
    QAction *mActionMoveItemContent;
    QAction *mActionGroupItems;
    QAction *mActionUngroupItems;
    QAction *mActionRaiseItems;
    QAction *mActionLowerItems;
    QAction *mActionMoveItemsToTop;
    QAction *mActionMoveItemsToBottom;
    QAction *mActionLoadFromTemplate;
    QAction *mActionSaveAsTemplate;
    QAction *mActionAlignLeft;
    QAction *mActionAlignHCenter;
    QAction *mActionUndo;
    QAction *mActionRedo;

    void setupUi(QToolBar *ThematicToolBar)
    {
        if (ThematicToolBar->objectName().isEmpty())
            ThematicToolBar->setObjectName(QString::fromUtf8("ThematicToolBar"));
        ThematicToolBar->resize(743, 83);
        ThematicToolBar->setIconSize(QSize(25, 25));
        ThematicToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        mActionPrint = new QAction(ThematicToolBar);
        mActionPrint->setObjectName(QString::fromUtf8("mActionPrint"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/thematicMap/mActionFilePrint.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionPrint->setIcon(icon);
        mActionZoomAll = new QAction(ThematicToolBar);
        mActionZoomAll->setObjectName(QString::fromUtf8("mActionZoomAll"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/thematicMap/mActionPageZoomFullExtent.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionZoomAll->setIcon(icon1);
        mActionZoomIn = new QAction(ThematicToolBar);
        mActionZoomIn->setObjectName(QString::fromUtf8("mActionZoomIn"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/thematicMap/mActionPageZoomIn.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionZoomIn->setIcon(icon2);
        mActionZoomOut = new QAction(ThematicToolBar);
        mActionZoomOut->setObjectName(QString::fromUtf8("mActionZoomOut"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/thematicMap/mActionPageZoomOut.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionZoomOut->setIcon(icon3);
        mActionMapZoomAll = new QAction(ThematicToolBar);
        mActionMapZoomAll->setObjectName(QString::fromUtf8("mActionMapZoomAll"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/thematicMap/mActionZoomFullExtent.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionMapZoomAll->setIcon(icon4);
        mActionMapZoomIn = new QAction(ThematicToolBar);
        mActionMapZoomIn->setObjectName(QString::fromUtf8("mActionMapZoomIn"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/thematicMap/mActionZoomIn.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionMapZoomIn->setIcon(icon5);
        mActionMapZoomOut = new QAction(ThematicToolBar);
        mActionMapZoomOut->setObjectName(QString::fromUtf8("mActionMapZoomOut"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/thematicMap/mActionZoomOut.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionMapZoomOut->setIcon(icon6);
        mActionAddGLW = new QAction(ThematicToolBar);
        mActionAddGLW->setObjectName(QString::fromUtf8("mActionAddGLW"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/thematicMap/mActionAddNonDbLayer.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionAddGLW->setIcon(icon7);
        mActionAddNewLabel = new QAction(ThematicToolBar);
        mActionAddNewLabel->setObjectName(QString::fromUtf8("mActionAddNewLabel"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/thematicMap/mActionLabel.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionAddNewLabel->setIcon(icon8);
        mActionAddNewLegend = new QAction(ThematicToolBar);
        mActionAddNewLegend->setObjectName(QString::fromUtf8("mActionAddNewLegend"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/thematicMap/mActionAddLegend.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionAddNewLegend->setIcon(icon9);
        mActionSelectMoveItem = new QAction(ThematicToolBar);
        mActionSelectMoveItem->setObjectName(QString::fromUtf8("mActionSelectMoveItem"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/images/thematicMap/mActionSelectMoveItem.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionSelectMoveItem->setIcon(icon10);
        mActionExportAsImage = new QAction(ThematicToolBar);
        mActionExportAsImage->setObjectName(QString::fromUtf8("mActionExportAsImage"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/images/thematicMap/savepict.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionExportAsImage->setIcon(icon11);
        mActionExport2Clipboard = new QAction(ThematicToolBar);
        mActionExport2Clipboard->setObjectName(QString::fromUtf8("mActionExport2Clipboard"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/images/thematicMap/copy.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionExport2Clipboard->setIcon(icon12);
        mActionExportAsPDF = new QAction(ThematicToolBar);
        mActionExportAsPDF->setObjectName(QString::fromUtf8("mActionExportAsPDF"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/images/thematicMap/mActionSaveAsPDF.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionExportAsPDF->setIcon(icon13);
        mActionExportAsEPS = new QAction(ThematicToolBar);
        mActionExportAsEPS->setObjectName(QString::fromUtf8("mActionExportAsEPS"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/images/thematicMap/mActionExportAsEPS.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionExportAsEPS->setIcon(icon14);
        mActionExportAsSVG = new QAction(ThematicToolBar);
        mActionExportAsSVG->setObjectName(QString::fromUtf8("mActionExportAsSVG"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/images/thematicMap/mActionSaveAsSVG.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionExportAsSVG->setIcon(icon15);
        mActionAddNewScalebar = new QAction(ThematicToolBar);
        mActionAddNewScalebar->setObjectName(QString::fromUtf8("mActionAddNewScalebar"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/images/thematicMap/mActionScaleBar.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionAddNewScalebar->setIcon(icon16);
        mActionRefreshView = new QAction(ThematicToolBar);
        mActionRefreshView->setObjectName(QString::fromUtf8("mActionRefreshView"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/images/thematicMap/mActionRefreshView.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionRefreshView->setIcon(icon17);
        mActionAddImage = new QAction(ThematicToolBar);
        mActionAddImage->setObjectName(QString::fromUtf8("mActionAddImage"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/images/thematicMap/mActionAddMap.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionAddImage->setIcon(icon18);
        mActionMoveItemContent = new QAction(ThematicToolBar);
        mActionMoveItemContent->setObjectName(QString::fromUtf8("mActionMoveItemContent"));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/images/thematicMap/mActionPan.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionMoveItemContent->setIcon(icon19);
        mActionGroupItems = new QAction(ThematicToolBar);
        mActionGroupItems->setObjectName(QString::fromUtf8("mActionGroupItems"));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/images/thematicMap/mActionGroupItems.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionGroupItems->setIcon(icon20);
        mActionUngroupItems = new QAction(ThematicToolBar);
        mActionUngroupItems->setObjectName(QString::fromUtf8("mActionUngroupItems"));
        QIcon icon21;
        icon21.addFile(QString::fromUtf8(":/images/thematicMap/mActionUngroupItems.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionUngroupItems->setIcon(icon21);
        mActionRaiseItems = new QAction(ThematicToolBar);
        mActionRaiseItems->setObjectName(QString::fromUtf8("mActionRaiseItems"));
        QIcon icon22;
        icon22.addFile(QString::fromUtf8(":/images/thematicMap/mActionRaiseItems.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionRaiseItems->setIcon(icon22);
        mActionLowerItems = new QAction(ThematicToolBar);
        mActionLowerItems->setObjectName(QString::fromUtf8("mActionLowerItems"));
        QIcon icon23;
        icon23.addFile(QString::fromUtf8(":/images/thematicMap/mActionLowerItems.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionLowerItems->setIcon(icon23);
        mActionMoveItemsToTop = new QAction(ThematicToolBar);
        mActionMoveItemsToTop->setObjectName(QString::fromUtf8("mActionMoveItemsToTop"));
        QIcon icon24;
        icon24.addFile(QString::fromUtf8(":/images/thematicMap/mActionMoveItemsToTop.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionMoveItemsToTop->setIcon(icon24);
        mActionMoveItemsToBottom = new QAction(ThematicToolBar);
        mActionMoveItemsToBottom->setObjectName(QString::fromUtf8("mActionMoveItemsToBottom"));
        QIcon icon25;
        icon25.addFile(QString::fromUtf8(":/images/thematicMap/mActionMoveItemsToBottom.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionMoveItemsToBottom->setIcon(icon25);
        mActionLoadFromTemplate = new QAction(ThematicToolBar);
        mActionLoadFromTemplate->setObjectName(QString::fromUtf8("mActionLoadFromTemplate"));
        mActionSaveAsTemplate = new QAction(ThematicToolBar);
        mActionSaveAsTemplate->setObjectName(QString::fromUtf8("mActionSaveAsTemplate"));
        mActionAlignLeft = new QAction(ThematicToolBar);
        mActionAlignLeft->setObjectName(QString::fromUtf8("mActionAlignLeft"));
        QIcon icon26;
        icon26.addFile(QString::fromUtf8(":/images/thematicMap/mActionAlignLeft.png"), QSize(), QIcon::Normal, QIcon::Off);
        mActionAlignLeft->setIcon(icon26);
        mActionAlignHCenter = new QAction(ThematicToolBar);
        mActionAlignHCenter->setObjectName(QString::fromUtf8("mActionAlignHCenter"));
        QIcon icon27;
        icon27.addFile(QString::fromUtf8(":/images/thematicMap/mActionAlignHCenter.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionAlignHCenter->setIcon(icon27);
        mActionUndo = new QAction(ThematicToolBar);
        mActionUndo->setObjectName(QString::fromUtf8("mActionUndo"));
        QIcon icon28;
        icon28.addFile(QString::fromUtf8(":/images/thematicMap/mActionUndo.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionUndo->setIcon(icon28);
        mActionRedo = new QAction(ThematicToolBar);
        mActionRedo->setObjectName(QString::fromUtf8("mActionRedo"));
        QIcon icon29;
        icon29.addFile(QString::fromUtf8(":/images/thematicMap/mActionRedo.png"), QSize(), QIcon::Normal, QIcon::On);
        mActionRedo->setIcon(icon29);

        ThematicToolBar->addSeparator();
        ThematicToolBar->addAction(mActionLoadFromTemplate);
        ThematicToolBar->addAction(mActionSaveAsTemplate);
        ThematicToolBar->addAction(mActionExportAsImage);
        ThematicToolBar->addAction(mActionExportAsPDF);
        ThematicToolBar->addAction(mActionExportAsEPS);
        ThematicToolBar->addAction(mActionExportAsSVG);
        ThematicToolBar->addAction(mActionExport2Clipboard);
        ThematicToolBar->addAction(mActionPrint);
        ThematicToolBar->addSeparator();
        ThematicToolBar->addAction(mActionMapZoomAll);
        ThematicToolBar->addAction(mActionMapZoomIn);
        ThematicToolBar->addAction(mActionMapZoomOut);
        ThematicToolBar->addAction(mActionMoveItemContent);
        ThematicToolBar->addAction(mActionRefreshView);
        ThematicToolBar->addSeparator();
        ThematicToolBar->addAction(mActionUndo);
        ThematicToolBar->addAction(mActionRedo);
        ThematicToolBar->addAction(mActionAddGLW);
        ThematicToolBar->addAction(mActionAddNewLabel);
        ThematicToolBar->addAction(mActionAddNewLegend);
        ThematicToolBar->addAction(mActionAddNewScalebar);
        ThematicToolBar->addAction(mActionAddImage);
        ThematicToolBar->addSeparator();
        ThematicToolBar->addAction(mActionZoomAll);
        ThematicToolBar->addAction(mActionZoomIn);
        ThematicToolBar->addAction(mActionZoomOut);
        ThematicToolBar->addAction(mActionSelectMoveItem);
        ThematicToolBar->addSeparator();
        ThematicToolBar->addAction(mActionGroupItems);
        ThematicToolBar->addAction(mActionUngroupItems);
        ThematicToolBar->addAction(mActionRaiseItems);
        ThematicToolBar->addAction(mActionLowerItems);
        ThematicToolBar->addAction(mActionMoveItemsToTop);
        ThematicToolBar->addAction(mActionMoveItemsToBottom);
        ThematicToolBar->addAction(mActionAlignLeft);
        ThematicToolBar->addAction(mActionAlignHCenter);

        retranslateUi(ThematicToolBar);

        QMetaObject::connectSlotsByName(ThematicToolBar);
    } // setupUi

    void retranslateUi(QToolBar *ThematicToolBar)
    {
        ThematicToolBar->setWindowTitle(QApplication::translate("ThematicToolBar", "ThematicToolBar", 0, QApplication::UnicodeUTF8));
        mActionPrint->setText(QApplication::translate("ThematicToolBar", "\346\211\223\345\215\260\342\200\246", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionPrint->setToolTip(QApplication::translate("ThematicToolBar", "\346\211\223\345\215\260", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionZoomAll->setText(QApplication::translate("ThematicToolBar", "\347\272\270\345\274\240\345\205\250\345\233\276", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionZoomAll->setToolTip(QApplication::translate("ThematicToolBar", "\347\272\270\345\274\240\345\205\250\345\233\276", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionZoomIn->setText(QApplication::translate("ThematicToolBar", "\347\272\270\345\274\240\346\224\276\345\244\247", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionZoomIn->setToolTip(QApplication::translate("ThematicToolBar", "\347\272\270\345\274\240\346\224\276\345\244\247", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionZoomOut->setText(QApplication::translate("ThematicToolBar", "\347\272\270\345\274\240\347\274\251\345\260\217", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionZoomOut->setToolTip(QApplication::translate("ThematicToolBar", "\347\272\270\345\274\240\347\274\251\345\260\217", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionMapZoomAll->setText(QApplication::translate("ThematicToolBar", "\345\234\260\345\233\276\345\205\250\345\233\276", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionMapZoomAll->setToolTip(QApplication::translate("ThematicToolBar", "\345\234\260\345\233\276\345\205\250\345\233\276", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionMapZoomIn->setText(QApplication::translate("ThematicToolBar", "\345\234\260\345\233\276\346\224\276\345\244\247", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionMapZoomIn->setToolTip(QApplication::translate("ThematicToolBar", "\345\234\260\345\233\276\346\224\276\345\244\247", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionMapZoomOut->setText(QApplication::translate("ThematicToolBar", "\345\234\260\345\233\276\347\274\251\345\260\217", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionMapZoomOut->setToolTip(QApplication::translate("ThematicToolBar", "\345\234\260\345\233\276\347\274\251\345\260\217", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionAddGLW->setText(QApplication::translate("ThematicToolBar", "\346\267\273\345\212\240\345\234\260\345\233\276", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionAddGLW->setToolTip(QApplication::translate("ThematicToolBar", "\346\267\273\345\212\240\345\234\260\345\233\276", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionAddNewLabel->setText(QApplication::translate("ThematicToolBar", "\346\267\273\345\212\240\346\240\207\347\255\276", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionAddNewLabel->setToolTip(QApplication::translate("ThematicToolBar", "\346\267\273\345\212\240\346\240\207\347\255\276", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionAddNewLegend->setText(QApplication::translate("ThematicToolBar", "\346\267\273\345\212\240\345\233\276\344\276\213", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionAddNewLegend->setToolTip(QApplication::translate("ThematicToolBar", "\346\267\273\345\212\240\345\233\276\344\276\213", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionSelectMoveItem->setText(QApplication::translate("ThematicToolBar", "\351\200\211\346\213\251/\347\247\273\345\212\250 \351\241\271", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionSelectMoveItem->setToolTip(QApplication::translate("ThematicToolBar", "\351\200\211\346\213\251/\347\247\273\345\212\250 \351\241\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionExportAsImage->setText(QApplication::translate("ThematicToolBar", "\345\257\274\345\207\272\345\233\276\347\211\207", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionExportAsImage->setToolTip(QApplication::translate("ThematicToolBar", "\345\257\274\345\207\272\345\233\276\347\211\207", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionExport2Clipboard->setText(QApplication::translate("ThematicToolBar", "\345\244\215\345\210\266\345\210\260\345\211\252\345\210\207\346\235\277", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionExport2Clipboard->setToolTip(QApplication::translate("ThematicToolBar", "\345\244\215\345\210\266\345\210\260\345\211\252\345\210\207\346\235\277", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionExportAsPDF->setText(QApplication::translate("ThematicToolBar", "\345\257\274\345\207\272PDF", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionExportAsPDF->setToolTip(QApplication::translate("ThematicToolBar", "\345\257\274\345\207\272PDF", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionExportAsEPS->setText(QApplication::translate("ThematicToolBar", "\345\257\274\345\207\272EPS", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionExportAsEPS->setToolTip(QApplication::translate("ThematicToolBar", "\345\257\274\345\207\272EPS", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionExportAsSVG->setText(QApplication::translate("ThematicToolBar", "\345\257\274\345\207\272SVG", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionExportAsSVG->setToolTip(QApplication::translate("ThematicToolBar", "\345\257\274\345\207\272SVG", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionAddNewScalebar->setText(QApplication::translate("ThematicToolBar", "\346\267\273\345\212\240\346\257\224\344\276\213\345\260\272", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionAddNewScalebar->setToolTip(QApplication::translate("ThematicToolBar", "\346\267\273\345\212\240\346\257\224\344\276\213\345\260\272", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionRefreshView->setText(QApplication::translate("ThematicToolBar", "\345\210\267\346\226\260", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionRefreshView->setToolTip(QApplication::translate("ThematicToolBar", "\345\210\267\346\226\260\350\247\206\345\233\276", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionAddImage->setText(QApplication::translate("ThematicToolBar", "\345\242\236\345\212\240\345\233\276\347\211\207", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionAddImage->setToolTip(QApplication::translate("ThematicToolBar", "\345\242\236\345\212\240\345\233\276\347\211\207", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionMoveItemContent->setText(QApplication::translate("ThematicToolBar", "\347\247\273\345\212\250\345\234\260\345\233\276\345\206\205\345\256\271", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionMoveItemContent->setToolTip(QApplication::translate("ThematicToolBar", "\347\247\273\345\212\250\345\234\260\345\233\276\345\206\205\345\256\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionGroupItems->setText(QApplication::translate("ThematicToolBar", "\347\273\204\345\220\210", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionGroupItems->setToolTip(QApplication::translate("ThematicToolBar", "\347\273\204\345\220\210\351\241\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionUngroupItems->setText(QApplication::translate("ThematicToolBar", "\345\210\206\347\273\204", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionUngroupItems->setToolTip(QApplication::translate("ThematicToolBar", "\345\210\206\347\273\204\351\241\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionRaiseItems->setText(QApplication::translate("ThematicToolBar", "\345\211\215\347\247\273", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionRaiseItems->setToolTip(QApplication::translate("ThematicToolBar", "\345\220\221\345\211\215\347\247\273\345\212\250\351\200\211\344\270\255\347\232\204\351\241\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionLowerItems->setText(QApplication::translate("ThematicToolBar", "\345\220\216\347\247\273", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionLowerItems->setToolTip(QApplication::translate("ThematicToolBar", "\345\220\221\345\220\216\347\247\273\345\212\250\351\200\211\344\270\255\347\232\204\351\241\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionMoveItemsToTop->setText(QApplication::translate("ThematicToolBar", "\347\247\273\350\207\263\346\234\200\345\211\215", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionMoveItemsToTop->setToolTip(QApplication::translate("ThematicToolBar", "\347\247\273\345\212\250\351\200\211\344\270\255\347\232\204\351\241\271\345\210\260\346\234\200\345\211\215", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionMoveItemsToBottom->setText(QApplication::translate("ThematicToolBar", "\347\247\273\350\207\263\346\234\200\345\220\216", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionMoveItemsToBottom->setToolTip(QApplication::translate("ThematicToolBar", "\347\247\273\345\212\250\351\200\211\344\270\255\347\232\204\351\241\271\345\210\260\346\234\200\345\220\216", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionLoadFromTemplate->setText(QApplication::translate("ThematicToolBar", "\345\212\240\350\275\275\346\250\241\346\235\277", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionLoadFromTemplate->setToolTip(QApplication::translate("ThematicToolBar", "\345\212\240\350\275\275\346\250\241\346\235\277", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionSaveAsTemplate->setText(QApplication::translate("ThematicToolBar", "\344\277\235\345\255\230\346\250\241\346\235\277", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionSaveAsTemplate->setToolTip(QApplication::translate("ThematicToolBar", "\344\277\235\345\255\230\346\250\241\346\235\277", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionAlignLeft->setText(QApplication::translate("ThematicToolBar", "\345\267\246\351\275\220", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionAlignLeft->setToolTip(QApplication::translate("ThematicToolBar", "\345\220\221\345\267\246\345\257\271\351\275\220\351\200\211\344\270\255\351\241\271", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionAlignHCenter->setText(QApplication::translate("ThematicToolBar", "\345\261\205\344\270\255\345\257\271\351\275\220", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionAlignHCenter->setToolTip(QApplication::translate("ThematicToolBar", "\345\261\205\344\270\255\345\257\271\351\275\220", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionUndo->setText(QApplication::translate("ThematicToolBar", "\346\222\244\351\224\200", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionUndo->setToolTip(QApplication::translate("ThematicToolBar", "\346\222\244\351\224\200", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        mActionRedo->setText(QApplication::translate("ThematicToolBar", "\351\207\215\345\201\232", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mActionRedo->setToolTip(QApplication::translate("ThematicToolBar", "\351\207\215\345\201\232", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class ThematicToolBar: public Ui_ThematicToolBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_THEMATICTOOLBAR_H

/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionLeftDockW;
    QAction *actionArrt;
    QAction *actionData;
    QAction *actionViewR;
    QAction *actionOpen;
    QAction *actionCommonToolBar;
    QAction *actionDisplaySettings;
    QAction *action_5;
    QAction *action_6;
    QAction *m_pActionGrid;
    QAction *m_pActionM;
    QAction *m_pActionL;
    QAction *m_pActionLonLat;
    QAction *m_ActionProjCenter;
    QAction *m_pActionLookAt;
    QAction *action_22;
    QAction *action_23;
    QAction *actionCascad;
    QAction *actionTile;
    QAction *actionOutMID;
    QAction *actionInMID;
    QAction *actionCloseAll;
    QAction *actionClose;
    QAction *actionHelp;
    QAction *actionAbout;
    QAction *action_40;
    QAction *action_41;
    QAction *action_44;
    QAction *action_47;
    QAction *action_48;
    QAction *action_VTK;
    QAction *action_51;
    QAction *action_fenxi;
    QAction *actionArea;
    QAction *actionFullScreen;
    QAction *action_13;
    QAction *actionConfig;
    QAction *actionMapSettings;
    QAction *action_20;
    QAction *action_21;
    QAction *actionZuhefanshelv;
    QAction *action_25;
    QAction *action_30;
    QAction *action_33;
    QAction *action_39;
    QAction *action_43;
    QAction *action_45;
    QAction *action_46;
    QAction *action_49;
    QAction *actionCAPPI;
    QAction *action_53;
    QAction *action_54;
    QAction *action_55;
    QAction *action_56;
    QAction *action_57;
    QAction *action_58;
    QAction *actionThematic;
    QAction *action_report;
    QAction *action;
    QAction *action_RadarControl;
    QAction *actionWin4;
    QAction *action3D;
    QAction *action2D;
    QAction *actionAnimatic;
    QAction *actionRadarData;
    QWidget *centralWidget;
    QStatusBar *statusBar;
    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *menu_2;
    QMenu *menu_3;
    QMenu *menu_6;
    QMenu *menu_4;
    QMenu *menu_5;
    QMenu *menu_7;
    QMenu *menu_13;
    QMenu *menu_14;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(718, 446);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindowClass->setWindowIcon(icon);
        MainWindowClass->setIconSize(QSize(40, 40));
        MainWindowClass->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        actionLeftDockW = new QAction(MainWindowClass);
        actionLeftDockW->setObjectName(QString::fromUtf8("actionLeftDockW"));
        actionLeftDockW->setCheckable(true);
        actionLeftDockW->setChecked(true);
        actionArrt = new QAction(MainWindowClass);
        actionArrt->setObjectName(QString::fromUtf8("actionArrt"));
        actionArrt->setCheckable(true);
        actionArrt->setChecked(true);
        actionData = new QAction(MainWindowClass);
        actionData->setObjectName(QString::fromUtf8("actionData"));
        actionData->setCheckable(true);
        actionData->setChecked(true);
        actionViewR = new QAction(MainWindowClass);
        actionViewR->setObjectName(QString::fromUtf8("actionViewR"));
        actionViewR->setCheckable(false);
        actionViewR->setChecked(false);
        actionOpen = new QAction(MainWindowClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/defaultToolBar/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
        actionCommonToolBar = new QAction(MainWindowClass);
        actionCommonToolBar->setObjectName(QString::fromUtf8("actionCommonToolBar"));
        actionCommonToolBar->setCheckable(true);
        actionCommonToolBar->setChecked(true);
        actionDisplaySettings = new QAction(MainWindowClass);
        actionDisplaySettings->setObjectName(QString::fromUtf8("actionDisplaySettings"));
        action_5 = new QAction(MainWindowClass);
        action_5->setObjectName(QString::fromUtf8("action_5"));
        action_5->setEnabled(false);
        action_6 = new QAction(MainWindowClass);
        action_6->setObjectName(QString::fromUtf8("action_6"));
        m_pActionGrid = new QAction(MainWindowClass);
        m_pActionGrid->setObjectName(QString::fromUtf8("m_pActionGrid"));
        m_pActionM = new QAction(MainWindowClass);
        m_pActionM->setObjectName(QString::fromUtf8("m_pActionM"));
        m_pActionL = new QAction(MainWindowClass);
        m_pActionL->setObjectName(QString::fromUtf8("m_pActionL"));
        m_pActionLonLat = new QAction(MainWindowClass);
        m_pActionLonLat->setObjectName(QString::fromUtf8("m_pActionLonLat"));
        m_ActionProjCenter = new QAction(MainWindowClass);
        m_ActionProjCenter->setObjectName(QString::fromUtf8("m_ActionProjCenter"));
        m_pActionLookAt = new QAction(MainWindowClass);
        m_pActionLookAt->setObjectName(QString::fromUtf8("m_pActionLookAt"));
        action_22 = new QAction(MainWindowClass);
        action_22->setObjectName(QString::fromUtf8("action_22"));
        action_23 = new QAction(MainWindowClass);
        action_23->setObjectName(QString::fromUtf8("action_23"));
        actionCascad = new QAction(MainWindowClass);
        actionCascad->setObjectName(QString::fromUtf8("actionCascad"));
        actionTile = new QAction(MainWindowClass);
        actionTile->setObjectName(QString::fromUtf8("actionTile"));
        actionOutMID = new QAction(MainWindowClass);
        actionOutMID->setObjectName(QString::fromUtf8("actionOutMID"));
        actionInMID = new QAction(MainWindowClass);
        actionInMID->setObjectName(QString::fromUtf8("actionInMID"));
        actionCloseAll = new QAction(MainWindowClass);
        actionCloseAll->setObjectName(QString::fromUtf8("actionCloseAll"));
        actionClose = new QAction(MainWindowClass);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionHelp = new QAction(MainWindowClass);
        actionHelp->setObjectName(QString::fromUtf8("actionHelp"));
        actionAbout = new QAction(MainWindowClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        action_40 = new QAction(MainWindowClass);
        action_40->setObjectName(QString::fromUtf8("action_40"));
        action_41 = new QAction(MainWindowClass);
        action_41->setObjectName(QString::fromUtf8("action_41"));
        action_44 = new QAction(MainWindowClass);
        action_44->setObjectName(QString::fromUtf8("action_44"));
        action_47 = new QAction(MainWindowClass);
        action_47->setObjectName(QString::fromUtf8("action_47"));
        action_48 = new QAction(MainWindowClass);
        action_48->setObjectName(QString::fromUtf8("action_48"));
        action_VTK = new QAction(MainWindowClass);
        action_VTK->setObjectName(QString::fromUtf8("action_VTK"));
        action_51 = new QAction(MainWindowClass);
        action_51->setObjectName(QString::fromUtf8("action_51"));
        action_fenxi = new QAction(MainWindowClass);
        action_fenxi->setObjectName(QString::fromUtf8("action_fenxi"));
        actionArea = new QAction(MainWindowClass);
        actionArea->setObjectName(QString::fromUtf8("actionArea"));
        actionFullScreen = new QAction(MainWindowClass);
        actionFullScreen->setObjectName(QString::fromUtf8("actionFullScreen"));
        action_13 = new QAction(MainWindowClass);
        action_13->setObjectName(QString::fromUtf8("action_13"));
        actionConfig = new QAction(MainWindowClass);
        actionConfig->setObjectName(QString::fromUtf8("actionConfig"));
        actionMapSettings = new QAction(MainWindowClass);
        actionMapSettings->setObjectName(QString::fromUtf8("actionMapSettings"));
        action_20 = new QAction(MainWindowClass);
        action_20->setObjectName(QString::fromUtf8("action_20"));
        action_21 = new QAction(MainWindowClass);
        action_21->setObjectName(QString::fromUtf8("action_21"));
        actionZuhefanshelv = new QAction(MainWindowClass);
        actionZuhefanshelv->setObjectName(QString::fromUtf8("actionZuhefanshelv"));
        action_25 = new QAction(MainWindowClass);
        action_25->setObjectName(QString::fromUtf8("action_25"));
        action_30 = new QAction(MainWindowClass);
        action_30->setObjectName(QString::fromUtf8("action_30"));
        action_33 = new QAction(MainWindowClass);
        action_33->setObjectName(QString::fromUtf8("action_33"));
        action_39 = new QAction(MainWindowClass);
        action_39->setObjectName(QString::fromUtf8("action_39"));
        action_43 = new QAction(MainWindowClass);
        action_43->setObjectName(QString::fromUtf8("action_43"));
        action_45 = new QAction(MainWindowClass);
        action_45->setObjectName(QString::fromUtf8("action_45"));
        action_46 = new QAction(MainWindowClass);
        action_46->setObjectName(QString::fromUtf8("action_46"));
        action_49 = new QAction(MainWindowClass);
        action_49->setObjectName(QString::fromUtf8("action_49"));
        actionCAPPI = new QAction(MainWindowClass);
        actionCAPPI->setObjectName(QString::fromUtf8("actionCAPPI"));
        action_53 = new QAction(MainWindowClass);
        action_53->setObjectName(QString::fromUtf8("action_53"));
        action_54 = new QAction(MainWindowClass);
        action_54->setObjectName(QString::fromUtf8("action_54"));
        action_55 = new QAction(MainWindowClass);
        action_55->setObjectName(QString::fromUtf8("action_55"));
        action_56 = new QAction(MainWindowClass);
        action_56->setObjectName(QString::fromUtf8("action_56"));
        action_57 = new QAction(MainWindowClass);
        action_57->setObjectName(QString::fromUtf8("action_57"));
        action_58 = new QAction(MainWindowClass);
        action_58->setObjectName(QString::fromUtf8("action_58"));
        actionThematic = new QAction(MainWindowClass);
        actionThematic->setObjectName(QString::fromUtf8("actionThematic"));
        action_report = new QAction(MainWindowClass);
        action_report->setObjectName(QString::fromUtf8("action_report"));
        action_report->setCheckable(false);
        action_report->setEnabled(false);
        action = new QAction(MainWindowClass);
        action->setObjectName(QString::fromUtf8("action"));
        action_RadarControl = new QAction(MainWindowClass);
        action_RadarControl->setObjectName(QString::fromUtf8("action_RadarControl"));
        actionWin4 = new QAction(MainWindowClass);
        actionWin4->setObjectName(QString::fromUtf8("actionWin4"));
        action3D = new QAction(MainWindowClass);
        action3D->setObjectName(QString::fromUtf8("action3D"));
        action2D = new QAction(MainWindowClass);
        action2D->setObjectName(QString::fromUtf8("action2D"));
        actionAnimatic = new QAction(MainWindowClass);
        actionAnimatic->setObjectName(QString::fromUtf8("actionAnimatic"));
        actionRadarData = new QAction(MainWindowClass);
        actionRadarData->setObjectName(QString::fromUtf8("actionRadarData"));
        actionRadarData->setCheckable(true);
        actionRadarData->setChecked(true);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindowClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindowClass->setStatusBar(statusBar);
        menubar = new QMenuBar(MainWindowClass);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 718, 23));
        fileMenu = new QMenu(menubar);
        fileMenu->setObjectName(QString::fromUtf8("fileMenu"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        menu_6 = new QMenu(menu_3);
        menu_6->setObjectName(QString::fromUtf8("menu_6"));
        menu_6->setEnabled(false);
        menu_4 = new QMenu(menubar);
        menu_4->setObjectName(QString::fromUtf8("menu_4"));
        menu_5 = new QMenu(menubar);
        menu_5->setObjectName(QString::fromUtf8("menu_5"));
        menu_7 = new QMenu(menubar);
        menu_7->setObjectName(QString::fromUtf8("menu_7"));
        menu_13 = new QMenu(menubar);
        menu_13->setObjectName(QString::fromUtf8("menu_13"));
        menu_14 = new QMenu(menubar);
        menu_14->setObjectName(QString::fromUtf8("menu_14"));
        MainWindowClass->setMenuBar(menubar);

        menubar->addAction(fileMenu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menubar->addAction(menu_4->menuAction());
        menubar->addAction(menu_7->menuAction());
        menubar->addAction(menu_13->menuAction());
        menubar->addAction(menu_14->menuAction());
        menubar->addAction(menu_5->menuAction());
        menu_2->addAction(actionLeftDockW);
        menu_2->addAction(actionArrt);
        menu_2->addAction(actionCommonToolBar);
        menu_2->addAction(action_5);
        menu_3->addAction(actionMapSettings);
        menu_3->addSeparator();
        menu_3->addAction(m_pActionGrid);
        menu_3->addSeparator();
        menu_3->addAction(m_pActionM);
        menu_3->addAction(m_pActionL);
        menu_3->addAction(m_pActionLonLat);
        menu_3->addAction(m_ActionProjCenter);
        menu_3->addSeparator();
        menu_3->addAction(actionDisplaySettings);
        menu_3->addSeparator();
        menu_3->addAction(menu_6->menuAction());
        menu_6->addAction(action_22);
        menu_6->addAction(action_23);
        menu_4->addAction(actionCascad);
        menu_4->addAction(actionTile);
        menu_4->addAction(actionFullScreen);
        menu_4->addSeparator();
        menu_4->addAction(actionOutMID);
        menu_4->addAction(actionInMID);
        menu_4->addAction(actionClose);
        menu_4->addSeparator();
        menu_4->addAction(actionCloseAll);
        menu_5->addAction(actionHelp);
        menu_5->addAction(actionAbout);
        menu_7->addAction(actionRadarData);
        menu_7->addSeparator();
        menu_7->addAction(actionViewR);
        menu_7->addAction(action_RadarControl);
        menu_7->addAction(actionWin4);
        menu_7->addAction(actionArea);
        menu_7->addAction(actionAnimatic);
        menu_7->addSeparator();
        menu_7->addAction(action2D);
        menu_7->addAction(action3D);
        menu_13->addAction(actionThematic);
        menu_13->addAction(action_report);
        menu_14->addAction(actionConfig);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "\346\260\224\344\276\257\344\270\232\345\212\241\346\225\260\346\215\256\344\272\244\344\272\222\345\244\204\347\220\206\347\263\273\347\273\237", 0, QApplication::UnicodeUTF8));
        actionLeftDockW->setText(QApplication::translate("MainWindowClass", "\345\233\276\345\261\202\346\216\247\345\210\266", 0, QApplication::UnicodeUTF8));
        actionArrt->setText(QApplication::translate("MainWindowClass", "\345\233\276\345\261\202\345\261\236\346\200\247", 0, QApplication::UnicodeUTF8));
        actionData->setText(QApplication::translate("MainWindowClass", "\346\225\260\346\215\256\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        actionViewR->setText(QApplication::translate("MainWindowClass", "\351\233\267\350\276\276\345\217\263\344\276\247\345\261\236\346\200\247\350\247\206\345\217\243", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainWindowClass", "\346\211\223\345\274\200", 0, QApplication::UnicodeUTF8));
        actionCommonToolBar->setText(QApplication::translate("MainWindowClass", "\351\200\232\347\224\250\345\267\245\345\205\267\346\235\241", 0, QApplication::UnicodeUTF8));
        actionDisplaySettings->setText(QApplication::translate("MainWindowClass", "\346\230\276\347\244\272\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        action_5->setText(QApplication::translate("MainWindowClass", "\345\212\250\347\224\273\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        action_6->setText(QApplication::translate("MainWindowClass", "\346\227\266\351\227\264\350\275\264 ", 0, QApplication::UnicodeUTF8));
        m_pActionGrid->setText(QApplication::translate("MainWindowClass", "\346\267\273\345\212\240\347\273\217\347\272\254\347\275\221", 0, QApplication::UnicodeUTF8));
        m_pActionM->setText(QApplication::translate("MainWindowClass", "\345\242\250\345\215\241\346\211\230\346\212\225\345\275\261", 0, QApplication::UnicodeUTF8));
        m_pActionL->setText(QApplication::translate("MainWindowClass", "\345\205\260\345\213\203\347\211\271\346\212\225\345\275\261", 0, QApplication::UnicodeUTF8));
        m_pActionLonLat->setText(QApplication::translate("MainWindowClass", "\347\255\211\347\273\217\347\272\254\346\212\225\345\275\261", 0, QApplication::UnicodeUTF8));
        m_ActionProjCenter->setText(QApplication::translate("MainWindowClass", "\346\212\225\345\275\261\344\270\255\345\277\203\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        m_pActionLookAt->setText(QApplication::translate("MainWindowClass", "\350\247\202\345\257\237\344\270\255\345\277\203\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        action_22->setText(QApplication::translate("MainWindowClass", "\344\270\255\345\233\275 ", 0, QApplication::UnicodeUTF8));
        action_23->setText(QApplication::translate("MainWindowClass", "\350\207\252\345\256\232\344\271\211\346\230\276\347\244\272\350\214\203\345\233\264 ", 0, QApplication::UnicodeUTF8));
        actionCascad->setText(QApplication::translate("MainWindowClass", "\347\252\227\345\217\243\345\261\202\345\217\240 ", 0, QApplication::UnicodeUTF8));
        actionTile->setText(QApplication::translate("MainWindowClass", "\347\252\227\345\217\243\345\271\263\351\223\272", 0, QApplication::UnicodeUTF8));
        actionOutMID->setText(QApplication::translate("MainWindowClass", "\347\252\227\345\217\243\346\265\256\345\212\250", 0, QApplication::UnicodeUTF8));
        actionInMID->setText(QApplication::translate("MainWindowClass", "\347\252\227\345\217\243\351\224\201\345\256\232", 0, QApplication::UnicodeUTF8));
        actionCloseAll->setText(QApplication::translate("MainWindowClass", "\345\205\263\351\227\255\346\211\200\346\234\211", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionCloseAll->setToolTip(QApplication::translate("MainWindowClass", "\345\205\263\351\227\255\346\211\200\346\234\211", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionClose->setText(QApplication::translate("MainWindowClass", "\347\252\227\345\217\243\345\205\263\351\227\255", 0, QApplication::UnicodeUTF8));
        actionHelp->setText(QApplication::translate("MainWindowClass", "\347\263\273\347\273\237\345\270\256\345\212\251", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindowClass", "\345\205\263\344\272\216\346\234\254\347\263\273\347\273\237 ", 0, QApplication::UnicodeUTF8));
        action_40->setText(QApplication::translate("MainWindowClass", "\344\272\247\345\223\201\345\261\236\346\200\247 ", 0, QApplication::UnicodeUTF8));
        action_41->setText(QApplication::translate("MainWindowClass", "\346\230\276\347\244\272\350\267\235\347\246\273\345\234\210", 0, QApplication::UnicodeUTF8));
        action_44->setText(QApplication::translate("MainWindowClass", "\351\274\240\346\240\207\345\220\214\346\255\245", 0, QApplication::UnicodeUTF8));
        action_47->setText(QApplication::translate("MainWindowClass", "\351\233\267\350\276\276\347\220\203\351\235\242\344\270\211\347\273\264", 0, QApplication::UnicodeUTF8));
        action_48->setText(QApplication::translate("MainWindowClass", "\351\233\267\350\276\276\346\225\260\346\215\256\344\275\223\344\270\211\347\273\264", 0, QApplication::UnicodeUTF8));
        action_VTK->setText(QApplication::translate("MainWindowClass", "\344\270\211\347\273\264\345\211\226\345\210\207\345\210\206\346\236\220", 0, QApplication::UnicodeUTF8));
        action_51->setText(QApplication::translate("MainWindowClass", "\345\271\263\351\235\242\345\211\226\345\210\207\345\210\206\346\236\220", 0, QApplication::UnicodeUTF8));
        action_fenxi->setText(QApplication::translate("MainWindowClass", "\347\255\211\345\200\274\351\235\242\345\210\206\346\236\220", 0, QApplication::UnicodeUTF8));
        actionArea->setText(QApplication::translate("MainWindowClass", "\351\235\242\347\247\257\347\273\237\350\256\241", 0, QApplication::UnicodeUTF8));
        actionFullScreen->setText(QApplication::translate("MainWindowClass", "\347\252\227\345\217\243\345\205\250\345\261\217", 0, QApplication::UnicodeUTF8));
        action_13->setText(QApplication::translate("MainWindowClass", "\350\207\252\345\256\232\344\271\211", 0, QApplication::UnicodeUTF8));
        actionConfig->setText(QApplication::translate("MainWindowClass", "\347\263\273\347\273\237\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        actionMapSettings->setText(QApplication::translate("MainWindowClass", "\345\234\260\345\233\276\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        action_20->setText(QApplication::translate("MainWindowClass", "\345\237\272\346\234\254\345\217\215\345\260\204\347\216\207", 0, QApplication::UnicodeUTF8));
        action_21->setText(QApplication::translate("MainWindowClass", "\345\237\272\346\234\254\351\200\237\345\272\246", 0, QApplication::UnicodeUTF8));
        actionZuhefanshelv->setText(QApplication::translate("MainWindowClass", "\347\273\204\345\220\210\345\217\215\345\260\204\347\216\207 ", 0, QApplication::UnicodeUTF8));
        action_25->setText(QApplication::translate("MainWindowClass", "\345\233\236\346\263\242\351\241\266\351\253\230", 0, QApplication::UnicodeUTF8));
        action_30->setText(QApplication::translate("MainWindowClass", "\351\243\216\345\273\223\347\272\277", 0, QApplication::UnicodeUTF8));
        action_33->setText(QApplication::translate("MainWindowClass", "\345\236\202\347\233\264\347\264\257\350\256\241\346\266\262\346\200\201\346\260\264\345\220\253\351\207\217", 0, QApplication::UnicodeUTF8));
        action_39->setText(QApplication::translate("MainWindowClass", "\351\243\216\346\232\264\350\277\275\350\270\252", 0, QApplication::UnicodeUTF8));
        action_43->setText(QApplication::translate("MainWindowClass", "\345\206\260\351\233\271\346\214\207\346\225\260", 0, QApplication::UnicodeUTF8));
        action_45->setText(QApplication::translate("MainWindowClass", "\344\270\255\345\260\272\345\272\246\346\260\224\346\227\213", 0, QApplication::UnicodeUTF8));
        action_46->setText(QApplication::translate("MainWindowClass", "\344\270\200\345\260\217\346\227\266\347\264\257\350\256\241\351\231\215\346\260\264", 0, QApplication::UnicodeUTF8));
        action_49->setText(QApplication::translate("MainWindowClass", "\344\270\211\345\260\217\346\227\266\347\264\257\350\256\241\351\231\215\346\260\264", 0, QApplication::UnicodeUTF8));
        actionCAPPI->setText(QApplication::translate("MainWindowClass", "CAPPI\345\217\215\345\260\204\347\216\207", 0, QApplication::UnicodeUTF8));
        action_53->setText(QApplication::translate("MainWindowClass", "\344\270\200\345\260\217\346\227\266\351\231\215\346\260\264\344\274\260\346\265\213", 0, QApplication::UnicodeUTF8));
        action_54->setText(QApplication::translate("MainWindowClass", "\347\273\204\345\220\210\345\217\215\345\260\204\347\216\207", 0, QApplication::UnicodeUTF8));
        action_55->setText(QApplication::translate("MainWindowClass", "\351\235\242\347\273\230\345\210\266", 0, QApplication::UnicodeUTF8));
        action_56->setText(QApplication::translate("MainWindowClass", "\344\275\223\347\273\230\345\210\266", 0, QApplication::UnicodeUTF8));
        action_57->setText(QApplication::translate("MainWindowClass", "\351\200\217\346\230\216\345\272\246\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        action_58->setText(QApplication::translate("MainWindowClass", "\351\242\234\350\211\262\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        actionThematic->setText(QApplication::translate("MainWindowClass", "\344\270\223\351\242\230\345\233\276\345\210\266\344\275\234", 0, QApplication::UnicodeUTF8));
        action_report->setText(QApplication::translate("MainWindowClass", "\344\270\223\351\242\230\346\212\245\345\221\212\345\210\266\344\275\234", 0, QApplication::UnicodeUTF8));
        action->setText(QApplication::translate("MainWindowClass", "\345\215\225\347\253\231\344\272\247\345\223\201\345\210\227\350\241\250", 0, QApplication::UnicodeUTF8));
        action_RadarControl->setText(QApplication::translate("MainWindowClass", "\351\233\267\350\276\276\346\216\247\345\210\266\350\247\206\345\217\243", 0, QApplication::UnicodeUTF8));
        actionWin4->setText(QApplication::translate("MainWindowClass", "\344\275\216\344\273\260\350\247\222\345\233\233\350\201\224\345\261\217", 0, QApplication::UnicodeUTF8));
        action3D->setText(QApplication::translate("MainWindowClass", "\351\233\267\350\276\276\344\270\211\347\273\264\346\230\276\347\244\272", 0, QApplication::UnicodeUTF8));
        action2D->setText(QApplication::translate("MainWindowClass", "\351\233\267\350\276\276\344\272\214\347\273\264\346\230\276\347\244\272", 0, QApplication::UnicodeUTF8));
        actionAnimatic->setText(QApplication::translate("MainWindowClass", "\351\233\267\350\276\276\345\212\250\347\224\273", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAnimatic->setToolTip(QApplication::translate("MainWindowClass", "\351\233\267\350\276\276\345\212\250\347\224\273", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionRadarData->setText(QApplication::translate("MainWindowClass", "\351\233\267\350\276\276\345\237\272\346\225\260\346\215\256\347\252\227\345\217\243", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRadarData->setToolTip(QApplication::translate("MainWindowClass", "\351\233\267\350\276\276\345\237\272\346\225\260\346\215\256\347\252\227\345\217\243", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fileMenu->setTitle(QApplication::translate("MainWindowClass", "\346\226\207\344\273\266[&F]", 0, QApplication::UnicodeUTF8));
        menu_2->setTitle(QApplication::translate("MainWindowClass", "\350\247\206\345\233\276[&V]", 0, QApplication::UnicodeUTF8));
        menu_3->setTitle(QApplication::translate("MainWindowClass", "\345\234\260\345\233\276[&M]", 0, QApplication::UnicodeUTF8));
        menu_6->setTitle(QApplication::translate("MainWindowClass", "\346\230\276\347\244\272\350\214\203\345\233\264\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
        menu_4->setTitle(QApplication::translate("MainWindowClass", "\347\252\227\345\217\243[&W]", 0, QApplication::UnicodeUTF8));
        menu_5->setTitle(QApplication::translate("MainWindowClass", "\345\270\256\345\212\251[&H]", 0, QApplication::UnicodeUTF8));
        menu_7->setTitle(QApplication::translate("MainWindowClass", "\351\233\267\350\276\276\346\225\260\346\215\256[&R]", 0, QApplication::UnicodeUTF8));
        menu_13->setTitle(QApplication::translate("MainWindowClass", "\344\270\223\351\242\230\345\210\266\344\275\234", 0, QApplication::UnicodeUTF8));
        menu_14->setTitle(QApplication::translate("MainWindowClass", "\347\263\273\347\273\237\350\256\276\347\275\256", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

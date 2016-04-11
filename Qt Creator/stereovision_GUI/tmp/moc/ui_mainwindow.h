/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *lblOriginalLeft;
    QLabel *lblOriginalRight;
    QPlainTextEdit *textBoxOutput;
    QPushButton *btnPauseOrResume;
    QPushButton *btnShowDisparityMap;
    QPushButton *btnShowStereoParamSetup;
    QPushButton *btnShow3DReconstruction;
    QPushButton *btnShowInputImages;
    QPushButton *btnShowTrackingObjectView;
    QPushButton *btnShowDiffImage;
    QPushButton *btnShowWarningLines;
    QComboBox *methodSelector;
    QLCDNumber *lcdNumber;
    QPushButton *toggleBtnShowHist;
    QPushButton *toggleBtnShowXYZ;
    QPushButton *toggleBtnShowDispDepth;
    QComboBox *inputSelector;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1292, 709);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        lblOriginalLeft = new QLabel(centralWidget);
        lblOriginalLeft->setObjectName(QStringLiteral("lblOriginalLeft"));
        lblOriginalLeft->setGeometry(QRect(4, 4, 640, 480));
        lblOriginalLeft->setFocusPolicy(Qt::StrongFocus);
        lblOriginalLeft->setAutoFillBackground(true);
        lblOriginalRight = new QLabel(centralWidget);
        lblOriginalRight->setObjectName(QStringLiteral("lblOriginalRight"));
        lblOriginalRight->setGeometry(QRect(648, 4, 640, 480));
        lblOriginalRight->setMouseTracking(false);
        lblOriginalRight->setAutoFillBackground(true);
        textBoxOutput = new QPlainTextEdit(centralWidget);
        textBoxOutput->setObjectName(QStringLiteral("textBoxOutput"));
        textBoxOutput->setEnabled(true);
        textBoxOutput->setGeometry(QRect(614, 488, 671, 197));
        textBoxOutput->setFocusPolicy(Qt::StrongFocus);
        textBoxOutput->setReadOnly(true);
        btnPauseOrResume = new QPushButton(centralWidget);
        btnPauseOrResume->setObjectName(QStringLiteral("btnPauseOrResume"));
        btnPauseOrResume->setGeometry(QRect(10, 500, 145, 41));
        btnShowDisparityMap = new QPushButton(centralWidget);
        btnShowDisparityMap->setObjectName(QStringLiteral("btnShowDisparityMap"));
        btnShowDisparityMap->setGeometry(QRect(180, 540, 181, 41));
        btnShowStereoParamSetup = new QPushButton(centralWidget);
        btnShowStereoParamSetup->setObjectName(QStringLiteral("btnShowStereoParamSetup"));
        btnShowStereoParamSetup->setGeometry(QRect(10, 550, 145, 41));
        btnShow3DReconstruction = new QPushButton(centralWidget);
        btnShow3DReconstruction->setObjectName(QStringLiteral("btnShow3DReconstruction"));
        btnShow3DReconstruction->setGeometry(QRect(180, 580, 181, 41));
        btnShowInputImages = new QPushButton(centralWidget);
        btnShowInputImages->setObjectName(QStringLiteral("btnShowInputImages"));
        btnShowInputImages->setGeometry(QRect(180, 500, 181, 41));
        btnShowTrackingObjectView = new QPushButton(centralWidget);
        btnShowTrackingObjectView->setObjectName(QStringLiteral("btnShowTrackingObjectView"));
        btnShowTrackingObjectView->setGeometry(QRect(390, 500, 211, 41));
        btnShowDiffImage = new QPushButton(centralWidget);
        btnShowDiffImage->setObjectName(QStringLiteral("btnShowDiffImage"));
        btnShowDiffImage->setGeometry(QRect(390, 540, 211, 41));
        btnShowWarningLines = new QPushButton(centralWidget);
        btnShowWarningLines->setObjectName(QStringLiteral("btnShowWarningLines"));
        btnShowWarningLines->setGeometry(QRect(390, 580, 211, 41));
        methodSelector = new QComboBox(centralWidget);
        methodSelector->setObjectName(QStringLiteral("methodSelector"));
        methodSelector->setGeometry(QRect(10, 630, 141, 27));
        methodSelector->setFocusPolicy(Qt::StrongFocus);
        lcdNumber = new QLCDNumber(centralWidget);
        lcdNumber->setObjectName(QStringLiteral("lcdNumber"));
        lcdNumber->setGeometry(QRect(530, 630, 64, 23));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 0, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(127, 127, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(63, 63, 255, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(0, 0, 127, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(0, 0, 170, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush6(QColor(255, 255, 255, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
        QBrush brush7(QColor(255, 255, 220, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        lcdNumber->setPalette(palette);
        toggleBtnShowHist = new QPushButton(centralWidget);
        toggleBtnShowHist->setObjectName(QStringLiteral("toggleBtnShowHist"));
        toggleBtnShowHist->setGeometry(QRect(540, 10, 99, 27));
        toggleBtnShowXYZ = new QPushButton(centralWidget);
        toggleBtnShowXYZ->setObjectName(QStringLiteral("toggleBtnShowXYZ"));
        toggleBtnShowXYZ->setGeometry(QRect(540, 10, 99, 27));
        toggleBtnShowDispDepth = new QPushButton(centralWidget);
        toggleBtnShowDispDepth->setObjectName(QStringLiteral("toggleBtnShowDispDepth"));
        toggleBtnShowDispDepth->setGeometry(QRect(368, 10, 171, 27));
        inputSelector = new QComboBox(centralWidget);
        inputSelector->setObjectName(QStringLiteral("inputSelector"));
        inputSelector->setGeometry(QRect(10, 600, 141, 27));
        inputSelector->setFocusPolicy(Qt::StrongFocus);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1292, 25));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Stereo Vision Main", 0));
        lblOriginalLeft->setText(QString());
        lblOriginalRight->setText(QString());
        btnPauseOrResume->setText(QApplication::translate("MainWindow", "Pause", 0));
        btnShowDisparityMap->setText(QApplication::translate("MainWindow", "Show Disparity Map", 0));
        btnShowStereoParamSetup->setText(QApplication::translate("MainWindow", "Stereo Param Setup", 0));
        btnShow3DReconstruction->setText(QApplication::translate("MainWindow", "Show 3D Reconstruction", 0));
        btnShowInputImages->setText(QApplication::translate("MainWindow", "Show Left/Right", 0));
        btnShowTrackingObjectView->setText(QApplication::translate("MainWindow", "Show Tracking Object View", 0));
        btnShowDiffImage->setText(QApplication::translate("MainWindow", "Show DiffImage", 0));
        btnShowWarningLines->setText(QApplication::translate("MainWindow", "Show Warning Edges", 0));
        methodSelector->clear();
        methodSelector->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "StereoBM", 0)
         << QApplication::translate("MainWindow", "StereoSGBM", 0)
         << QApplication::translate("MainWindow", "StereoBM_GPU", 0)
        );
        toggleBtnShowHist->setText(QApplication::translate("MainWindow", "Hist", 0));
        toggleBtnShowXYZ->setText(QApplication::translate("MainWindow", "ShowXYZ", 0));
        toggleBtnShowDispDepth->setText(QApplication::translate("MainWindow", "Show Disparity/Depth", 0));
        inputSelector->clear();
        inputSelector->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Input 1", 0)
         << QApplication::translate("MainWindow", "Input 2", 0)
         << QApplication::translate("MainWindow", "Input 3", 0)
         << QApplication::translate("MainWindow", "Input 4", 0)
         << QApplication::translate("MainWindow", "Input 5", 0)
         << QApplication::translate("MainWindow", "Input 6", 0)
        );
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

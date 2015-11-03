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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *lblOriginalLeft;
    QLabel *lblOriginalRight;
    QPlainTextEdit *txtOutputBox;
    QPushButton *btnPauseOrResume;
    QPushButton *btnShowDisparityMap;
    QPushButton *btnShowStereoParamSetup;
    QPushButton *btnShow3DReconstruction;
    QPushButton *btnShowInputImages;
    QPushButton *btnShowTrackingObjectView;
    QPushButton *btnShowDiffImage;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1292, 691);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        lblOriginalLeft = new QLabel(centralWidget);
        lblOriginalLeft->setObjectName(QStringLiteral("lblOriginalLeft"));
        lblOriginalLeft->setGeometry(QRect(4, 4, 640, 480));
        lblOriginalLeft->setAutoFillBackground(true);
        lblOriginalRight = new QLabel(centralWidget);
        lblOriginalRight->setObjectName(QStringLiteral("lblOriginalRight"));
        lblOriginalRight->setGeometry(QRect(648, 4, 640, 480));
        lblOriginalRight->setMouseTracking(false);
        lblOriginalRight->setAutoFillBackground(true);
        txtOutputBox = new QPlainTextEdit(centralWidget);
        txtOutputBox->setObjectName(QStringLiteral("txtOutputBox"));
        txtOutputBox->setGeometry(QRect(614, 488, 671, 197));
        txtOutputBox->setReadOnly(true);
        btnPauseOrResume = new QPushButton(centralWidget);
        btnPauseOrResume->setObjectName(QStringLiteral("btnPauseOrResume"));
        btnPauseOrResume->setGeometry(QRect(10, 500, 145, 51));
        btnShowDisparityMap = new QPushButton(centralWidget);
        btnShowDisparityMap->setObjectName(QStringLiteral("btnShowDisparityMap"));
        btnShowDisparityMap->setGeometry(QRect(180, 540, 181, 41));
        btnShowStereoParamSetup = new QPushButton(centralWidget);
        btnShowStereoParamSetup->setObjectName(QStringLiteral("btnShowStereoParamSetup"));
        btnShowStereoParamSetup->setGeometry(QRect(10, 560, 145, 41));
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
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1292, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
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
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

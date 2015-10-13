/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QLabel *lblOriginal;
    QLabel *lblProcessed;
    QPushButton *btnPauseOrResume;
    QPlainTextEdit *txtXYRadius;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->resize(1292, 691);
        lblOriginal = new QLabel(Dialog);
        lblOriginal->setObjectName(QStringLiteral("lblOriginal"));
        lblOriginal->setGeometry(QRect(4, 4, 640, 480));
        lblOriginal->setAutoFillBackground(true);
        lblProcessed = new QLabel(Dialog);
        lblProcessed->setObjectName(QStringLiteral("lblProcessed"));
        lblProcessed->setGeometry(QRect(648, 4, 640, 480));
        lblProcessed->setAutoFillBackground(true);
        btnPauseOrResume = new QPushButton(Dialog);
        btnPauseOrResume->setObjectName(QStringLiteral("btnPauseOrResume"));
        btnPauseOrResume->setGeometry(QRect(72, 540, 145, 53));
        txtXYRadius = new QPlainTextEdit(Dialog);
        txtXYRadius->setObjectName(QStringLiteral("txtXYRadius"));
        txtXYRadius->setGeometry(QRect(308, 488, 977, 197));
        QFont font;
        font.setPointSize(10);
        txtXYRadius->setFont(font);
        txtXYRadius->setReadOnly(true);

        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0));
        lblOriginal->setText(QString());
        lblProcessed->setText(QString());
        btnPauseOrResume->setText(QApplication::translate("Dialog", "Pause", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H

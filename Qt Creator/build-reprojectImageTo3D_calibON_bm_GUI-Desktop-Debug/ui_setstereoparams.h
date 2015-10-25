/********************************************************************************
** Form generated from reading UI file 'setstereoparams.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETSTEREOPARAMS_H
#define UI_SETSTEREOPARAMS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_SetStereoParams
{
public:
    QDialogButtonBox *buttonBox;
    QSlider *horizontalSlider;
    QSpinBox *spinBox;
    QSlider *horizontalSlider_3;
    QSpinBox *spinBox_2;

    void setupUi(QDialog *SetStereoParams)
    {
        if (SetStereoParams->objectName().isEmpty())
            SetStereoParams->setObjectName(QStringLiteral("SetStereoParams"));
        SetStereoParams->resize(400, 586);
        buttonBox = new QDialogButtonBox(SetStereoParams);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 530, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        horizontalSlider = new QSlider(SetStereoParams);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(100, 100, 160, 29));
        horizontalSlider->setOrientation(Qt::Horizontal);
        spinBox = new QSpinBox(SetStereoParams);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(40, 100, 48, 27));
        horizontalSlider_3 = new QSlider(SetStereoParams);
        horizontalSlider_3->setObjectName(QStringLiteral("horizontalSlider_3"));
        horizontalSlider_3->setGeometry(QRect(100, 140, 160, 29));
        horizontalSlider_3->setOrientation(Qt::Horizontal);
        spinBox_2 = new QSpinBox(SetStereoParams);
        spinBox_2->setObjectName(QStringLiteral("spinBox_2"));
        spinBox_2->setGeometry(QRect(40, 140, 48, 27));

        retranslateUi(SetStereoParams);
        QObject::connect(buttonBox, SIGNAL(accepted()), SetStereoParams, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SetStereoParams, SLOT(reject()));

        QMetaObject::connectSlotsByName(SetStereoParams);
    } // setupUi

    void retranslateUi(QDialog *SetStereoParams)
    {
        SetStereoParams->setWindowTitle(QApplication::translate("SetStereoParams", "Set Stereo Param Setup", 0));
    } // retranslateUi

};

namespace Ui {
    class SetStereoParams: public Ui_SetStereoParams {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETSTEREOPARAMS_H

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
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_SetStereoParams
{
public:
    QDialogButtonBox *buttonBox;
    QSlider *preFilterSize_slider;
    QSpinBox *preFilterSize_spinBox;
    QSlider *preFilterCap_slider;
    QSpinBox *preFilterCap_spinBox;
    QLabel *preFilterSize_label;
    QLabel *preFilterCap_label;
    QLabel *SADWindowSize_label;
    QSlider *SADWindowSize_slider;
    QSpinBox *SADWindowSize_spinBox;
    QLabel *minDisparity_label;
    QSlider *minDisparity_slider;
    QSpinBox *minDisparity_spinBox;
    QLabel *numberOfDisparities_label;
    QSlider *numberOfDisparities_slider;
    QSpinBox *numberOfDisparities_spinBox;
    QLabel *textureThreshold_label;
    QSlider *textureThreshold_slider;
    QSpinBox *textureThreshold_spinBox;
    QLabel *uniquenessRatio_label;
    QSlider *uniquenessRatio_slider;
    QSpinBox *uniquenessRatio_spinBox;
    QLabel *speckleWindowSize_label;
    QSlider *speckleWindowSize_slider;
    QSpinBox *speckleWindowSize_spinBox;
    QLabel *speckleRange_label;
    QSlider *speckleRange_slider;
    QSpinBox *speckleRange_spinBox;
    QLabel *disp12MaxDiff_label;
    QSlider *disp12MaxDiff_slider;
    QSpinBox *disp12MaxDiff_spinBox;

    void setupUi(QDialog *SetStereoParams)
    {
        if (SetStereoParams->objectName().isEmpty())
            SetStereoParams->setObjectName(QStringLiteral("SetStereoParams"));
        SetStereoParams->resize(459, 473);
        buttonBox = new QDialogButtonBox(SetStereoParams);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(230, 420, 191, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        preFilterSize_slider = new QSlider(SetStereoParams);
        preFilterSize_slider->setObjectName(QStringLiteral("preFilterSize_slider"));
        preFilterSize_slider->setGeometry(QRect(169, 10, 201, 29));
        preFilterSize_slider->setMinimum(5);
        preFilterSize_slider->setMaximum(255);
        preFilterSize_slider->setSingleStep(2);
        preFilterSize_slider->setOrientation(Qt::Horizontal);
        preFilterSize_spinBox = new QSpinBox(SetStereoParams);
        preFilterSize_spinBox->setObjectName(QStringLiteral("preFilterSize_spinBox"));
        preFilterSize_spinBox->setGeometry(QRect(380, 10, 71, 27));
        preFilterSize_spinBox->setMinimum(5);
        preFilterSize_spinBox->setMaximum(255);
        preFilterSize_spinBox->setSingleStep(2);
        preFilterCap_slider = new QSlider(SetStereoParams);
        preFilterCap_slider->setObjectName(QStringLiteral("preFilterCap_slider"));
        preFilterCap_slider->setGeometry(QRect(169, 50, 201, 29));
        preFilterCap_slider->setMinimum(1);
        preFilterCap_slider->setMaximum(63);
        preFilterCap_slider->setSingleStep(1);
        preFilterCap_slider->setOrientation(Qt::Horizontal);
        preFilterCap_spinBox = new QSpinBox(SetStereoParams);
        preFilterCap_spinBox->setObjectName(QStringLiteral("preFilterCap_spinBox"));
        preFilterCap_spinBox->setGeometry(QRect(380, 50, 71, 27));
        preFilterCap_spinBox->setMinimum(1);
        preFilterCap_spinBox->setMaximum(63);
        preFilterSize_label = new QLabel(SetStereoParams);
        preFilterSize_label->setObjectName(QStringLiteral("preFilterSize_label"));
        preFilterSize_label->setGeometry(QRect(6, 10, 91, 31));
        preFilterCap_label = new QLabel(SetStereoParams);
        preFilterCap_label->setObjectName(QStringLiteral("preFilterCap_label"));
        preFilterCap_label->setGeometry(QRect(6, 50, 91, 31));
        SADWindowSize_label = new QLabel(SetStereoParams);
        SADWindowSize_label->setObjectName(QStringLiteral("SADWindowSize_label"));
        SADWindowSize_label->setGeometry(QRect(6, 90, 121, 31));
        SADWindowSize_slider = new QSlider(SetStereoParams);
        SADWindowSize_slider->setObjectName(QStringLiteral("SADWindowSize_slider"));
        SADWindowSize_slider->setGeometry(QRect(169, 90, 201, 29));
        SADWindowSize_slider->setMinimum(5);
        SADWindowSize_slider->setMaximum(255);
        SADWindowSize_slider->setSingleStep(2);
        SADWindowSize_slider->setOrientation(Qt::Horizontal);
        SADWindowSize_spinBox = new QSpinBox(SetStereoParams);
        SADWindowSize_spinBox->setObjectName(QStringLiteral("SADWindowSize_spinBox"));
        SADWindowSize_spinBox->setGeometry(QRect(380, 90, 71, 27));
        SADWindowSize_spinBox->setMinimum(5);
        SADWindowSize_spinBox->setMaximum(255);
        SADWindowSize_spinBox->setSingleStep(2);
        minDisparity_label = new QLabel(SetStereoParams);
        minDisparity_label->setObjectName(QStringLiteral("minDisparity_label"));
        minDisparity_label->setGeometry(QRect(6, 130, 91, 31));
        minDisparity_slider = new QSlider(SetStereoParams);
        minDisparity_slider->setObjectName(QStringLiteral("minDisparity_slider"));
        minDisparity_slider->setGeometry(QRect(169, 130, 201, 29));
        minDisparity_slider->setMinimum(-100);
        minDisparity_slider->setMaximum(100);
        minDisparity_slider->setOrientation(Qt::Horizontal);
        minDisparity_spinBox = new QSpinBox(SetStereoParams);
        minDisparity_spinBox->setObjectName(QStringLiteral("minDisparity_spinBox"));
        minDisparity_spinBox->setGeometry(QRect(380, 130, 71, 27));
        minDisparity_spinBox->setMinimum(-100);
        minDisparity_spinBox->setMaximum(100);
        numberOfDisparities_label = new QLabel(SetStereoParams);
        numberOfDisparities_label->setObjectName(QStringLiteral("numberOfDisparities_label"));
        numberOfDisparities_label->setGeometry(QRect(10, 170, 151, 31));
        numberOfDisparities_slider = new QSlider(SetStereoParams);
        numberOfDisparities_slider->setObjectName(QStringLiteral("numberOfDisparities_slider"));
        numberOfDisparities_slider->setGeometry(QRect(169, 170, 201, 29));
        numberOfDisparities_slider->setAcceptDrops(false);
        numberOfDisparities_slider->setMinimum(16);
        numberOfDisparities_slider->setMaximum(256);
        numberOfDisparities_slider->setSingleStep(16);
        numberOfDisparities_slider->setPageStep(16);
        numberOfDisparities_slider->setTracking(true);
        numberOfDisparities_slider->setOrientation(Qt::Horizontal);
        numberOfDisparities_slider->setInvertedAppearance(false);
        numberOfDisparities_slider->setTickPosition(QSlider::NoTicks);
        numberOfDisparities_slider->setTickInterval(0);
        numberOfDisparities_spinBox = new QSpinBox(SetStereoParams);
        numberOfDisparities_spinBox->setObjectName(QStringLiteral("numberOfDisparities_spinBox"));
        numberOfDisparities_spinBox->setGeometry(QRect(380, 170, 71, 27));
        numberOfDisparities_spinBox->setMinimum(16);
        numberOfDisparities_spinBox->setMaximum(256);
        numberOfDisparities_spinBox->setSingleStep(16);
        textureThreshold_label = new QLabel(SetStereoParams);
        textureThreshold_label->setObjectName(QStringLiteral("textureThreshold_label"));
        textureThreshold_label->setGeometry(QRect(6, 210, 131, 31));
        textureThreshold_slider = new QSlider(SetStereoParams);
        textureThreshold_slider->setObjectName(QStringLiteral("textureThreshold_slider"));
        textureThreshold_slider->setGeometry(QRect(169, 210, 201, 29));
        textureThreshold_slider->setMaximum(32000);
        textureThreshold_slider->setOrientation(Qt::Horizontal);
        textureThreshold_spinBox = new QSpinBox(SetStereoParams);
        textureThreshold_spinBox->setObjectName(QStringLiteral("textureThreshold_spinBox"));
        textureThreshold_spinBox->setGeometry(QRect(380, 210, 71, 27));
        textureThreshold_spinBox->setMaximum(32000);
        uniquenessRatio_label = new QLabel(SetStereoParams);
        uniquenessRatio_label->setObjectName(QStringLiteral("uniquenessRatio_label"));
        uniquenessRatio_label->setGeometry(QRect(6, 250, 121, 31));
        uniquenessRatio_slider = new QSlider(SetStereoParams);
        uniquenessRatio_slider->setObjectName(QStringLiteral("uniquenessRatio_slider"));
        uniquenessRatio_slider->setGeometry(QRect(169, 250, 201, 29));
        uniquenessRatio_slider->setMaximum(100);
        uniquenessRatio_slider->setOrientation(Qt::Horizontal);
        uniquenessRatio_spinBox = new QSpinBox(SetStereoParams);
        uniquenessRatio_spinBox->setObjectName(QStringLiteral("uniquenessRatio_spinBox"));
        uniquenessRatio_spinBox->setGeometry(QRect(380, 250, 71, 27));
        uniquenessRatio_spinBox->setMaximum(255);
        speckleWindowSize_label = new QLabel(SetStereoParams);
        speckleWindowSize_label->setObjectName(QStringLiteral("speckleWindowSize_label"));
        speckleWindowSize_label->setGeometry(QRect(6, 290, 141, 31));
        speckleWindowSize_slider = new QSlider(SetStereoParams);
        speckleWindowSize_slider->setObjectName(QStringLiteral("speckleWindowSize_slider"));
        speckleWindowSize_slider->setGeometry(QRect(169, 290, 201, 29));
        speckleWindowSize_slider->setMaximum(100);
        speckleWindowSize_slider->setOrientation(Qt::Horizontal);
        speckleWindowSize_spinBox = new QSpinBox(SetStereoParams);
        speckleWindowSize_spinBox->setObjectName(QStringLiteral("speckleWindowSize_spinBox"));
        speckleWindowSize_spinBox->setGeometry(QRect(380, 290, 71, 27));
        speckleWindowSize_spinBox->setMaximum(100);
        speckleRange_label = new QLabel(SetStereoParams);
        speckleRange_label->setObjectName(QStringLiteral("speckleRange_label"));
        speckleRange_label->setGeometry(QRect(6, 330, 101, 31));
        speckleRange_slider = new QSlider(SetStereoParams);
        speckleRange_slider->setObjectName(QStringLiteral("speckleRange_slider"));
        speckleRange_slider->setGeometry(QRect(169, 330, 201, 29));
        speckleRange_slider->setMaximum(100);
        speckleRange_slider->setOrientation(Qt::Horizontal);
        speckleRange_spinBox = new QSpinBox(SetStereoParams);
        speckleRange_spinBox->setObjectName(QStringLiteral("speckleRange_spinBox"));
        speckleRange_spinBox->setGeometry(QRect(380, 330, 71, 27));
        speckleRange_spinBox->setMaximum(100);
        disp12MaxDiff_label = new QLabel(SetStereoParams);
        disp12MaxDiff_label->setObjectName(QStringLiteral("disp12MaxDiff_label"));
        disp12MaxDiff_label->setGeometry(QRect(6, 370, 111, 31));
        disp12MaxDiff_slider = new QSlider(SetStereoParams);
        disp12MaxDiff_slider->setObjectName(QStringLiteral("disp12MaxDiff_slider"));
        disp12MaxDiff_slider->setGeometry(QRect(169, 370, 201, 29));
        disp12MaxDiff_slider->setMaximum(1);
        disp12MaxDiff_slider->setOrientation(Qt::Horizontal);
        disp12MaxDiff_spinBox = new QSpinBox(SetStereoParams);
        disp12MaxDiff_spinBox->setObjectName(QStringLiteral("disp12MaxDiff_spinBox"));
        disp12MaxDiff_spinBox->setGeometry(QRect(380, 370, 71, 27));
        disp12MaxDiff_spinBox->setMaximum(1);

        retranslateUi(SetStereoParams);
        QObject::connect(buttonBox, SIGNAL(accepted()), SetStereoParams, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), SetStereoParams, SLOT(reject()));

        QMetaObject::connectSlotsByName(SetStereoParams);
    } // setupUi

    void retranslateUi(QDialog *SetStereoParams)
    {
        SetStereoParams->setWindowTitle(QApplication::translate("SetStereoParams", "Stereo Param Setup", 0));
        preFilterSize_label->setText(QApplication::translate("SetStereoParams", "preFilterSize", 0));
        preFilterCap_label->setText(QApplication::translate("SetStereoParams", "preFilterCap", 0));
        SADWindowSize_label->setText(QApplication::translate("SetStereoParams", "SADWindowSize", 0));
        minDisparity_label->setText(QApplication::translate("SetStereoParams", "minDisparity", 0));
        numberOfDisparities_label->setText(QApplication::translate("SetStereoParams", "numberOfDisparities", 0));
        textureThreshold_label->setText(QApplication::translate("SetStereoParams", "textureThreshold", 0));
        uniquenessRatio_label->setText(QApplication::translate("SetStereoParams", "uniquenessRatio", 0));
        speckleWindowSize_label->setText(QApplication::translate("SetStereoParams", "speckleWindowSize", 0));
        speckleRange_label->setText(QApplication::translate("SetStereoParams", "speckleRange", 0));
        disp12MaxDiff_label->setText(QApplication::translate("SetStereoParams", "disp12MaxDiff", 0));
    } // retranslateUi

};

namespace Ui {
    class SetStereoParams: public Ui_SetStereoParams {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETSTEREOPARAMS_H

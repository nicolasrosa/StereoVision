/*
 * setstereoparams.cpp
 *
 *  Created on: Oct 1, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
#include <QHBoxLayout>
#include <QSlider>
#include <QSpinBox>
#include "ui_setstereoparams.h"

#include <iostream>

#include "inc/StereoSetParamsWindow.h"
#include "inc/StereoProcessor.h"

//using namespace cv;
using namespace std;

SetStereoParams::SetStereoParams(QWidget *parent, StereoProcessor *stereo) : QDialog(parent), ui(new Ui::SetStereoParams){
    ui->setupUi(this);

    this->stereo = stereo;

    connect(ui->preFilterSize_slider, SIGNAL(valueChanged(int)),ui->preFilterSize_spinBox,SLOT(setValue(int)));
    connect(ui->preFilterCap_slider, SIGNAL(valueChanged(int)),ui->preFilterCap_spinBox,SLOT(setValue(int)));
    connect(ui->SADWindowSize_slider, SIGNAL(valueChanged(int)),ui->SADWindowSize_spinBox,SLOT(setValue(int)));
    connect(ui->minDisparity_slider, SIGNAL(valueChanged(int)),ui->minDisparity_spinBox,SLOT(setValue(int)));
    connect(ui->numberOfDisparities_slider, SIGNAL(valueChanged(int)),ui->numberOfDisparities_spinBox,SLOT(setValue(int)));
    connect(ui->textureThreshold_slider, SIGNAL(valueChanged(int)),ui->textureThreshold_spinBox,SLOT(setValue(int)));
    connect(ui->uniquenessRatio_slider, SIGNAL(valueChanged(int)),ui->uniquenessRatio_spinBox,SLOT(setValue(int)));
    connect(ui->speckleWindowSize_slider, SIGNAL(valueChanged(int)),ui->speckleWindowSize_spinBox,SLOT(setValue(int)));
    connect(ui->speckleRange_slider, SIGNAL(valueChanged(int)),ui->speckleRange_spinBox,SLOT(setValue(int)));
    connect(ui->disp12MaxDiff_slider, SIGNAL(valueChanged(int)),ui->disp12MaxDiff_spinBox,SLOT(setValue(int)));
    connect(ui->preFilterSize_spinBox, SIGNAL(valueChanged(int)),ui->preFilterSize_slider,SLOT(setValue(int)));
    connect(ui->preFilterCap_spinBox, SIGNAL(valueChanged(int)),ui->preFilterCap_slider,SLOT(setValue(int)));
    connect(ui->SADWindowSize_spinBox, SIGNAL(valueChanged(int)),ui->SADWindowSize_slider,SLOT(setValue(int)));
    connect(ui->minDisparity_spinBox, SIGNAL(valueChanged(int)),ui->minDisparity_slider,SLOT(setValue(int)));
    connect(ui->numberOfDisparities_spinBox, SIGNAL(valueChanged(int)),ui->numberOfDisparities_slider,SLOT(setValue(int)));
    connect(ui->textureThreshold_spinBox, SIGNAL(valueChanged(int)),ui->textureThreshold_slider,SLOT(setValue(int)));
    connect(ui->uniquenessRatio_spinBox, SIGNAL(valueChanged(int)),ui->uniquenessRatio_slider,SLOT(setValue(int)));
    connect(ui->speckleWindowSize_spinBox, SIGNAL(valueChanged(int)),ui->speckleWindowSize_slider,SLOT(setValue(int)));
    connect(ui->speckleRange_spinBox, SIGNAL(valueChanged(int)),ui->speckleRange_slider,SLOT(setValue(int)));
    connect(ui->disp12MaxDiff_spinBox, SIGNAL(valueChanged(int)),ui->disp12MaxDiff_slider,SLOT(setValue(int)));
}

void SetStereoParams::loadStereoParamsUi(int preFilterSize,int preFilterCap,int SADWindowSize,int minDisparity,int numberOfDisparities,int textureThreshold,int uniquenessRatio, int speckleWindowSize, int speckleRange,int disp12MaxDiff){
    this->ui->preFilterSize_slider->setValue(preFilterSize);
    this->ui->preFilterSize_spinBox->setValue(preFilterSize);

    this->ui->preFilterCap_slider->setValue(preFilterCap);
    this->ui->preFilterCap_spinBox->setValue(preFilterCap);

    this->ui->SADWindowSize_slider->setValue(SADWindowSize);
    this->ui->SADWindowSize_spinBox->setValue(SADWindowSize);

    this->ui->minDisparity_slider->setValue(minDisparity);
    this->ui->minDisparity_spinBox->setValue(minDisparity);

    this->ui->numberOfDisparities_slider->setValue(numberOfDisparities);
    this->ui->numberOfDisparities_spinBox->setValue(numberOfDisparities);

    this->ui->textureThreshold_slider->setValue(textureThreshold);
    this->ui->textureThreshold_spinBox->setValue(textureThreshold);

    this->ui->uniquenessRatio_slider->setValue(uniquenessRatio);
    this->ui->uniquenessRatio_spinBox->setValue(uniquenessRatio);

    this->ui->speckleWindowSize_slider->setValue(speckleWindowSize);
    this->ui->speckleWindowSize_spinBox->setValue(speckleWindowSize);

    this->ui->speckleRange_slider->setValue(speckleRange);
    this->ui->speckleRange_spinBox->setValue(speckleRange);

    this->ui->disp12MaxDiff_slider->setValue(disp12MaxDiff);
    this->ui->disp12MaxDiff_spinBox->setValue(disp12MaxDiff);
}

SetStereoParams::~SetStereoParams(){
    delete ui;
}

/* Sliders */
void SetStereoParams::on_preFilterSize_slider_valueChanged(int value){
    //cout << "Bar1: " << value << endl;
    updateValues();
}

void SetStereoParams::on_preFilterCap_slider_valueChanged(int value){
    //cout << "Bar2: " << value << endl;
    updateValues();
}

void SetStereoParams::on_SADWindowSize_slider_valueChanged(int value){
    //cout << "Bar3: " << value << endl;
    updateValues();
}

void SetStereoParams::on_minDisparity_slider_valueChanged(int value){
    //cout << "Bar4: " << value << endl;
    updateValues();
}

void SetStereoParams::on_numberOfDisparities_slider_valueChanged(int value){
    //cout << "Bar5: " << value << endl;
    updateValues();
}

void SetStereoParams::on_textureThreshold_slider_valueChanged(int value){
    //cout << "Bar6: " << value << endl;
    updateValues();
}

void SetStereoParams::on_uniquenessRatio_slider_valueChanged(int value){
    //cout << "Bar7: " << value << endl;
    updateValues();
}

void SetStereoParams::on_speckleWindowSize_slider_valueChanged(int value){
    //cout << "Bar8: " << value << endl;
    updateValues();
}

void SetStereoParams::on_speckleRange_slider_valueChanged(int value){
    //cout << "Bar9: " << value << endl;
    updateValues();
}

void SetStereoParams::on_disp12MaxDiff_slider_valueChanged(int value){
    //cout << "Bar10: " << value << endl;
    updateValues();
}

/* SpinBoxes */
void SetStereoParams::on_preFilterSize_spinBox_valueChanged(int value){
    //cout << "Spin1: " << value << endl;
    updateValues();
}

void SetStereoParams::on_preFilterCap_spinBox_valueChanged(int value){
    //cout << "Spin2: " << value << endl;
    updateValues();
}

void SetStereoParams::on_SADWindowSize_spinBox_valueChanged(int value){
    //cout << "Spin3: " << value << endl;
    updateValues();
}

void SetStereoParams::on_minDisparity_spinBox_valueChanged(int value){
    //cout << "Spin4: " << value << endl;
    updateValues();
}

void SetStereoParams::on_numberOfDisparities_spinBox_valueChanged(int value){
    //cout << "Spin5: " << value << endl;
    updateValues();
}

void SetStereoParams::on_textureThreshold_spinBox_valueChanged(int value){
    //cout << "Spin6: " << value << endl;
    updateValues();
}

void SetStereoParams::on_uniquenessRatio_spinBox_valueChanged(int value){
    //cout << "Spin7: " << value << endl;
    updateValues();
}

void SetStereoParams::on_speckleWindowSize_spinBox_valueChanged(int value){
    //cout << "Spin8: " << value << endl;
    updateValues();
}

void SetStereoParams::on_speckleRange_spinBox_valueChanged(int value){
    //cout << "Spin9: " << value << endl;
    updateValues();
}

void SetStereoParams::on_disp12MaxDiff_spinBox_valueChanged(int value){
    //cout << "Spin10: " << value << endl;
    updateValues();
}

void SetStereoParams::on_buttonBox_accepted(){

}

void SetStereoParams::on_buttonBox_rejected(){

}

void SetStereoParams::updateValues() {
    //std::cout << "Update Values!\n";
    stereo->setValues(ui->preFilterSize_slider->value(),
                      ui->preFilterCap_slider->value(),
                      ui->SADWindowSize_slider->value(),
                      ui->minDisparity_slider->value(),
                      ui->numberOfDisparities_slider->value(),
                      ui->textureThreshold_slider->value(),
                      ui->uniquenessRatio_slider->value(),
                      ui->speckleWindowSize_slider->value(),
                      ui->speckleRange_slider->value(),
                      ui->disp12MaxDiff_slider->value());

    if(this->stereo->flags.methodBM)
        this->stereo->setStereoBM_Params();

    if(this->stereo->flags.methodSGBM)
        this->stereo->setStereoSGBM_Params();
}


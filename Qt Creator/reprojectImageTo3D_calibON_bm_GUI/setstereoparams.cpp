#include "setstereoparams.h"
#include "ui_setstereoparams.h"
#include "iostream"

#include <QHBoxLayout>
#include <QSlider>
#include <QSpinBox>

//using namespace cv;
using namespace std;

SetStereoParams::SetStereoParams(QWidget *parent) : QDialog(parent), ui(new Ui::SetStereoParams){
    ui->setupUi(this);

    //    QWidget *window = new QWidget;
    //    window->setWindowTitle("Enter Your Age");

    //    QSpinBox *spinBox = new QSpinBox;
    //    QSlider *slider = new QSlider(Qt::Horizontal);
    //    spinBox->setRange(0, 130);
    //    slider->setRange(0, 130);

    //    QObject::connect(spinBox, SIGNAL(valueChanged(int)),slider, SLOT(setValue(int)));
    //    QObject::connect(slider, SIGNAL(valueChanged(int)),spinBox, SLOT(setValue(int)));
    //    spinBox->setValue(35);

    //    QHBoxLayout *layout = new QHBoxLayout;

    //    layout->addWidget(spinBox);
    //    layout->addWidget(slider);
    //    window->setLayout(layout);

    //    window->show();

}

SetStereoParams::~SetStereoParams()
{
    delete ui;
}

/* Sliders */
void SetStereoParams::on_preFilterSize_slider_valueChanged(int value)
{
    cout << "Bar1: " << value << endl;
    connect(ui->preFilterSize_slider, SIGNAL(valueChanged(int)),ui->preFilterSize_spinBox,SLOT(setValue(int)));
}

void SetStereoParams::on_preFilterCap_slider_valueChanged(int value)
{
    cout << "Bar2: " << value << endl;
    connect(ui->preFilterCap_slider, SIGNAL(valueChanged(int)),ui->preFilterCap_spinBox,SLOT(setValue(int)));
}

void SetStereoParams::on_SADWindowSize_slider_valueChanged(int value)
{
    cout << "Bar3: " << value << endl;
    connect(ui->SADWindowSize_slider, SIGNAL(valueChanged(int)),ui->SADWindowSize_spinBox,SLOT(setValue(int)));
}

void SetStereoParams::on_minDisparity_slider_valueChanged(int value)
{
    cout << "Bar4: " << value << endl;
    connect(ui->minDisparity_slider, SIGNAL(valueChanged(int)),ui->minDisparity_spinBox,SLOT(setValue(int)));
}

void SetStereoParams::on_numberOfDisparities_slider_valueChanged(int value)
{
    cout << "Bar5: " << value << endl;
    connect(ui->numberOfDisparities_slider, SIGNAL(valueChanged(int)),ui->numberOfDisparities_spinBox,SLOT(setValue(int)));
}

void SetStereoParams::on_textureThreshold_slider_valueChanged(int value)
{
    cout << "Bar6: " << value << endl;
    connect(ui->textureThreshold_slider, SIGNAL(valueChanged(int)),ui->textureThreshold_spinBox,SLOT(setValue(int)));
}

void SetStereoParams::on_uniquenessRatio_slider_valueChanged(int value)
{
    cout << "Bar7: " << value << endl;
    connect(ui->uniquenessRatio_slider, SIGNAL(valueChanged(int)),ui->uniquenessRatio_spinBox,SLOT(setValue(int)));
}

void SetStereoParams::on_speckleWindowSize_slider_valueChanged(int value)
{
    cout << "Bar8: " << value << endl;
    connect(ui->speckleWindowSize_slider, SIGNAL(valueChanged(int)),ui->speckleWindowSize_spinBox,SLOT(setValue(int)));
}

void SetStereoParams::on_speckleRange_slider_valueChanged(int value)
{
    cout << "Bar9: " << value << endl;
    connect(ui->speckleRange_slider, SIGNAL(valueChanged(int)),ui->speckleRange_spinBox,SLOT(setValue(int)));
}

void SetStereoParams::on_disp12MaxDiff_slider_valueChanged(int value)
{
    cout << "Bar10: " << value << endl;
    connect(ui->disp12MaxDiff_slider, SIGNAL(valueChanged(int)),ui->disp12MaxDiff_spinBox,SLOT(setValue(int)));
}

/* SpinBoxes */
void SetStereoParams::on_preFilterSize_spinBox_valueChanged(int value)
{
    cout << "Spin1: " << value << endl;
    connect(ui->preFilterSize_spinBox, SIGNAL(valueChanged(int)),ui->preFilterSize_slider,SLOT(setValue(int)));
}

void SetStereoParams::on_preFilterCap_spinBox_valueChanged(int value)
{
    cout << "Spin2: " << value << endl;
    connect(ui->preFilterCap_spinBox, SIGNAL(valueChanged(int)),ui->preFilterCap_slider,SLOT(setValue(int)));
}

void SetStereoParams::on_SADWindowSize_spinBox_valueChanged(int value)
{
    cout << "Spin3: " << value << endl;
    connect(ui->SADWindowSize_spinBox, SIGNAL(valueChanged(int)),ui->SADWindowSize_slider,SLOT(setValue(int)));
}

void SetStereoParams::on_minDisparity_spinBox_valueChanged(int value)
{
    cout << "Spin4: " << value << endl;
    connect(ui->minDisparity_spinBox, SIGNAL(valueChanged(int)),ui->minDisparity_slider,SLOT(setValue(int)));
}

void SetStereoParams::on_numberOfDisparities_spinBox_valueChanged(int value)
{
    cout << "Spin5: " << value << endl;
    connect(ui->numberOfDisparities_spinBox, SIGNAL(valueChanged(int)),ui->numberOfDisparities_slider,SLOT(setValue(int)));
}

void SetStereoParams::on_textureThreshold_spinBox_valueChanged(int value)
{
    cout << "Spin6: " << value << endl;
    connect(ui->textureThreshold_spinBox, SIGNAL(valueChanged(int)),ui->textureThreshold_slider,SLOT(setValue(int)));
}

void SetStereoParams::on_uniquenessRatio_spinBox_valueChanged(int value)
{
    cout << "Spin7: " << value << endl;
    connect(ui->uniquenessRatio_spinBox, SIGNAL(valueChanged(int)),ui->uniquenessRatio_slider,SLOT(setValue(int)));
}

void SetStereoParams::on_speckleWindowSize_spinBox_valueChanged(int value)
{
    cout << "Spin8: " << value << endl;
    connect(ui->speckleWindowSize_spinBox, SIGNAL(valueChanged(int)),ui->speckleWindowSize_slider,SLOT(setValue(int)));
}

void SetStereoParams::on_speckleRange_spinBox_valueChanged(int value)
{
    cout << "Spin9: " << value << endl;
    connect(ui->speckleRange_spinBox, SIGNAL(valueChanged(int)),ui->speckleRange_slider,SLOT(setValue(int)));
}

void SetStereoParams::on_disp12MaxDiff_spinBox_valueChanged(int value)
{
    cout << "Spin10: " << value << endl;
    connect(ui->disp12MaxDiff_spinBox, SIGNAL(valueChanged(int)),ui->disp12MaxDiff_slider,SLOT(setValue(int)));
}

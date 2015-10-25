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

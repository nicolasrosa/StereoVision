#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
  //  connect(ui->pushButton,SIGNAL(clicked()),ui->widget,SLOT(click()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

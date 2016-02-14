/*
 * main.cpp
 *
 *  Created on: June, 2015
 *      Author: nicolasrosa
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    MainWindow mainwindow;
    mainwindow.show();

    return app.exec();
}

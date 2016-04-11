/*
 * main.cpp
 *
 *  Created on: June, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    MainWindow mainwindow;

    mainwindow.show();

    return app.exec();
}

//FIXME: Arrumar a Matrix K, os valores das ultimas colunas estao errados.
//FIXME: Arrumar a funcao StereoProcessor::calculateQMatrix().
//FIXME: Arrumar o Constructor da classe StereoDisparityMap para Alocacao de Memoria das variaveis: disp_16S,disp_8U,disp_BGR
//FIXME: Arrumar a inicializacao e separar as variaveis 'Stereocfg' para os metodos BM e SGBM
//FIXME: Arrumar os erros que acontecem quando clica-se nos botoes 'Track' and 'Diff' para o input 4
//FIXME: Arrumar a Feature de mostrar distância nos métodos SGBM e BM_GPU

/*
 * main.cpp
 *
 *  Created on: June, 2015
 *      Author: nicolasrosa
 */

/* Libraries */
#include "inc/main.h"

//#define ENABLE_SPLASH

void init_splash(int time);

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    MainWindow mainwindow;

#ifdef ENABLE_SPLASH
    /* Initializing Splash Animation */
    int splash_time = 2500;
    init_splash(splash_time);

    /* Initializing App */
    QTimer::singleShot(splash_time,&mainwindow,SLOT(show()));
#endif

#ifndef ENABLE_SPLASH
    mainwindow.show();
#endif

    return app.exec();
}


void init_splash(int time){
    int i;
    int numberIcons = 7;
    QSplashScreen *splash[numberIcons];

//    QPixmap pix = ...;
//    QPainter painter( &pix );
//    painter.setFont( QFont("Arial") );
//    painter.drawText( QPoint(100, 100), "Hello" );

    /* Instatianting QSplashScreen Objects */
    for(i=0;i<numberIcons;i++){
        splash[i] = new QSplashScreen;
    }

    /* Loading Icons */
    splash[0]->setPixmap(QPixmap(":/icons/icon/drone_black.png"));
    splash[1]->setPixmap(QPixmap(":/icons/icon/drone_blue.png"));
    splash[2]->setPixmap(QPixmap(":/icons/icon/drone_cyan.png"));
    splash[3]->setPixmap(QPixmap(":/icons/icon/drone_green.png"));
    splash[4]->setPixmap(QPixmap(":/icons/icon/drone_orange.png"));
    splash[5]->setPixmap(QPixmap(":/icons/icon/drone_purple.png"));
    splash[6]->setPixmap(QPixmap(":/icons/icon/drone_white.png"));

    /* Displaying Splash */
    for(int i=0;i<numberIcons;i++){
        QTimer::singleShot(i*time/numberIcons,splash[i],SLOT(show()));
        QTimer::singleShot((i+1)*time/numberIcons,splash[i],SLOT(close()));
    }
}

//FIXME: Arrumar a Matrix K, os valores das ultimas colunas estao errados.
//FIXME: Arrumar a funcao StereoProcessor::calculateQMatrix().
//FIXME: Arrumar o Constructor da classe StereoDisparityMap para Alocacao de Memoria das variaveis: disp_16S,disp_8U,disp_BGR
//FIXME: Arrumar a inicializacao e separar as variaveis 'Stereocfg' para os metodos BM e SGBM
//FIXME: Arrumar os erros que acontecem quando clica-se nos botoes 'Track' and 'Diff' para o input 4
//FIXME: Arrumar a Feature de mostrar distância nos métodos SGBM e BM_GPU.

//TODO: Give Credit to Icon's Author: "Icon made by Freepik from www.flaticon.com"
//TODO: Enable Slash when everything is finished.

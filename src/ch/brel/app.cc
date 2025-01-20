
#include <QCoreApplication>
#include <QDebug>
#include "app.h"

App::App(QObject *parent) : QObject(parent) { }

void App::exitMain()
{
    qDebug() << "App::exitMain()";
    QCoreApplication::exit(1);
}

void App::exit()
{
    qDebug() << "App::exit()";
    QCoreApplication::exit(0);
}

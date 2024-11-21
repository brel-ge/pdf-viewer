
#include <QCoreApplication>
#include <QDebug>

#include "app.h"

App::App(QObject *parent) : QObject(parent) {}

void App::exit() {
  qDebug() << "App::exit()";
  QCoreApplication::exit();
}

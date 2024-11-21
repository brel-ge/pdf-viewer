
#pragma once

#include <QObject>
#include <QString>
#include <qqmlregistration.h>

/**
  This class is the main application class.
  store the application state and to provide access to the backend connector.
  It also provides access to the application information and the log text.
  The class is a singleton and is registered as a QML type.
*/

class App : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON

public:
  App(QObject *parent = nullptr);

public slots:
  void exit();
};


#include "theme.h"
#include <QDebug>
#include <QFontDatabase>

using namespace Qt::Literals::StringLiterals;

Theme::Theme(QObject *parent) : QObject(parent) {

  QFontDatabase::addApplicationFont(
      "/usr/share/fonts/truetype/Roboto-Regular.ttf");
  QFontDatabase::addApplicationFont(
      "/usr/share/fonts/truetype/Roboto-Bold.ttf");

  m_boldFont = QFont("Roboto", 16, QFont::Bold);
  m_regularFont = QFont("Roboto", 16);
}

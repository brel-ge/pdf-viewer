
#include "theme.h"
#include <QDebug>
#include <QFontDatabase>

Theme::Theme(QObject *parent) : QObject(parent)
{

    QFontDatabase::addApplicationFont("/usr/share/fonts/truetype/Roboto-Regular.ttf");
    QFontDatabase::addApplicationFont("/usr/share/fonts/truetype/Roboto-Bold.ttf");

    const int defaultFontSize = 16;
    m_boldFont = QFont("Roboto", defaultFontSize, QFont::Bold);
    m_regularFont = QFont("Roboto", defaultFontSize, QFont::Normal);
}

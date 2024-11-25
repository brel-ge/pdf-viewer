
#pragma once

#include <QFont>
#include <QObject>
#include <QString>
#include <QUrl>
#include <qqmlregistration.h>

/**
 * This class stores the theme of the application.
 * It provides access to the colors and fonts used in the application.
 */
class Theme : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QString backgroundColor MEMBER m_backgroundColor CONSTANT)
    Q_PROPERTY(QString buttonColor MEMBER m_buttonColor CONSTANT)
    Q_PROPERTY(QString buttonDownColor MEMBER m_buttonDownColor CONSTANT)
    Q_PROPERTY(QString foregroundColor MEMBER m_foregroundColor CONSTANT)
    Q_PROPERTY(QString greenColor MEMBER m_greenColor CONSTANT)
    Q_PROPERTY(QFont regularFont MEMBER m_regularFont CONSTANT)
    Q_PROPERTY(QFont boldFont MEMBER m_boldFont CONSTANT)

public:
    explicit Theme(QObject *parent = nullptr);

private:
    QFont m_regularFont;
    QFont m_boldFont;
    QString m_backgroundColor = "#161616";
    QString m_buttonColor = "#252525";
    QString m_buttonDownColor = "#343434";
    QString m_foregroundColor = "#FFFFFF";
    QString m_greenColor = "#6EE152";
};

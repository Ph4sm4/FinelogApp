#ifndef STYLESHEETMANIPULATOR_H
#define STYLESHEETMANIPULATOR_H

#include <qglobal.h>
#include <QJsonArray>
#include <QString>
#include <QJsonObject>

class StylesheetManipulator
{
public:
    static QString updateStylesheetProperty(const QString &styleSheet, const QString &selector, const QString &property, const QString &newValue);

private:
    static QJsonArray styleSheetToJson(const QString &styleSheet);
    static QJsonArray styleSheetPropertiesToJson(const QString &properties);
    static QJsonArray updateValue(const QString &selector, const QString &propertyName, const QString &newValue, const QJsonArray &jsonStyleSheet);
    static QString jsonToStyleSheet(const QJsonArray &jsonStyleSheet);
};

#endif // STYLESHEETMANIPULATOR_H

#include "stylesheetmanipulator.h"

QString StylesheetManipulator::updateStylesheetProperty(const QString &styleSheet, const QString &selector, const QString &property, const QString &newValue)
{
    return jsonToStyleSheet(updateValue(selector, property, newValue, styleSheetToJson(styleSheet)));
}

QJsonArray StylesheetManipulator::styleSheetToJson(const QString &styleSheet)
{
    QJsonArray jsonStyleSheet;

    if (styleSheet.isEmpty())
        return jsonStyleSheet;

    foreach (const QString &style, styleSheet.trimmed().split("}")) {
        const QString &trimmedStyle(style.trimmed());

        if (!trimmedStyle.isEmpty()) {
            const QStringList &list(trimmedStyle.split("{"));

            jsonStyleSheet.append(QJsonObject {
                {"selector", list.first().trimmed()},
                {"properties", styleSheetPropertiesToJson(list.last())}
            });
        }
    }

    return jsonStyleSheet;
}

QJsonArray StylesheetManipulator::styleSheetPropertiesToJson(const QString &properties)
{
    QJsonArray jsonProperties;

    if (properties.isEmpty())
        return jsonProperties;

    foreach (const QString &property, properties.trimmed().split(";")) {
        const QString &trimmedProperty(property.trimmed());

        if (!trimmedProperty.isEmpty()) {
            const QStringList &list(trimmedProperty.split(":"));

            jsonProperties.append(QJsonObject{
                {"name", list.first().trimmed()},
                {"value", list.last().trimmed()}
            });
        }
    }

    return jsonProperties;
}

QJsonArray StylesheetManipulator::updateValue(const QString &selector, const QString &propertyName, const QString &newValue, const QJsonArray &jsonStyleSheet)
{
    QJsonArray a;

    foreach (const QJsonValue &value, jsonStyleSheet) {
        const QJsonObject &currentStyle(value.toObject());
        const QString &currentSelector(currentStyle["selector"].toString());
        bool selectorFound = currentSelector == selector;
        QJsonArray properties;

        foreach (const QJsonValue &value, currentStyle["properties"].toArray()) {
            QJsonObject property(value.toObject());

            if (selectorFound && (property["name"].toString() == propertyName))
                property["value"] = newValue;

            properties.append(property);
        }

        a.append(QJsonObject{
            {"selector", currentSelector},
            {"properties", properties}
        });
    }

    return a;
}

QString StylesheetManipulator::jsonToStyleSheet(const QJsonArray &jsonStyleSheet)
{
    QString styleSheet;

    foreach (const QJsonValue &value, jsonStyleSheet) {
        const QJsonObject &currentStyle(value.toObject());

        styleSheet.append(currentStyle["selector"].toString() + " {");

        foreach (const QJsonValue &value, currentStyle["properties"].toArray()) {
            QJsonObject property(value.toObject());

            styleSheet.append(" " + property["name"].toString() + ": " + property["value"].toString() + ";");
        }

        styleSheet.append(" } ");
    }

    return styleSheet;
}

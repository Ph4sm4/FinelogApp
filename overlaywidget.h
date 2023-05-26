#ifndef OVERLAYWIDGET_H
#define OVERLAYWIDGET_H

#include <QObject>
#include <QWidget>
#include "settingspanel.h"

class OverlayWidget : public QWidget
{
public:
    explicit OverlayWidget(QWidget* parent = nullptr);

    void setPanel(SettingsPanel* panel) { settingsPanel = panel; }
    void closeSettingsPanel();

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        qDebug() << "Widget clicked!";
        closeSettingsPanel();

        // Let the base class handle the event as well
        QWidget::mousePressEvent(event);
    }

private:
    SettingsPanel* settingsPanel = nullptr;
};

#endif // OVERLAYWIDGET_H

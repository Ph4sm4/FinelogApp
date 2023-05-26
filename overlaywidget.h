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

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        qDebug() << "Widget clicked!";
        if(settingsPanel) {
            settingsPanel->performAnimation(200, QPoint(width(), 0), settingsPanel->parent());
            this->hide();
        }
        else {
            qWarning() << "SETTINGS PANEL NOT SET FOR THE OVERLAY, REMEMBER TO SET IT!";
        }

        // Let the base class handle the event as well
        QWidget::mousePressEvent(event);
    }

private:
    SettingsPanel* settingsPanel = nullptr;
};

#endif // OVERLAYWIDGET_H

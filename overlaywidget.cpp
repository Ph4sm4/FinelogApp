#include "overlaywidget.h"


OverlayWidget::OverlayWidget(QWidget *parent) : QWidget(parent) {}

void OverlayWidget::closeSettingsPanel()
{
    if(settingsPanel) {
        settingsPanel->performAnimation(200, QPoint(width(), 0), settingsPanel->parent());
        this->hide();
    }
    else {
        qWarning() << "SETTINGS PANEL NOT SET FOR THE OVERLAY, REMEMBER TO SET IT!";
    }
}

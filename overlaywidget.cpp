#include "overlaywidget.h"

OverlayWidget::OverlayWidget(QWidget *parent)
    : QWidget(parent)
{}

void OverlayWidget::exitFromView()
{
    this->hide();
}

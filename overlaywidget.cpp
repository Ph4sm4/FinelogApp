#include "overlaywidget.h"
#include "ui_overlaywidget.h"

OverlayWidget::OverlayWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OverlayWidget)
{
    ui->setupUi(this);

    setFixedSize(parent->width(), parent->height());
    move(0, 0);
}

void OverlayWidget::exitFromView()
{
    this->hide();
}

OverlayWidget::~OverlayWidget()
{
    delete ui;
}

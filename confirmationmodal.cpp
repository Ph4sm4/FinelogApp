#include "confirmationmodal.h"
#include "ui_confirmationmodal.h"

ConfirmationModal::ConfirmationModal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfirmationModal)
{
    ui->setupUi(this);

    int w = parent->width();
    int h = parent->height();

    setGeometry((w - 300) / 2, (h - 400) / 2, 300, 250);
    raise();
}

ConfirmationModal::~ConfirmationModal()
{
    delete ui;
}

void ConfirmationModal::exitFromView()
{
    this->hide();
}

void ConfirmationModal::on_cancelButton_clicked()
{
    this->hide();
    emit cancelAction();
}

void ConfirmationModal::on_confirmButton_clicked()
{
    emit acceptAction(userId);
}

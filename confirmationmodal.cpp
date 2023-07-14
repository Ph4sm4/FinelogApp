#include "confirmationmodal.h"
#include "ui_confirmationmodal.h"

ConfirmationModal::ConfirmationModal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfirmationModal)
{
    ui->setupUi(this);

    int w = parent->width();
    int h = parent->height();

    setGeometry(0, 0, 300, 400);
    raise();
}

ConfirmationModal::~ConfirmationModal()
{
    delete ui;
}

void ConfirmationModal::on_cancelButton_clicked()
{
    emit cancelAction();
}

void ConfirmationModal::on_confirmButton_clicked() {}

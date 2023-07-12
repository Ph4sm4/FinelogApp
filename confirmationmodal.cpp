#include "confirmationmodal.h"
#include "ui_confirmationmodal.h"

ConfirmationModal::ConfirmationModal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfirmationModal)
{
    ui->setupUi(this);
}

ConfirmationModal::~ConfirmationModal()
{
    delete ui;
}

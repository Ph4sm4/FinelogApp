#include "confirmationmodal.h"
#include "ui_confirmationmodal.h"

ConfirmationModal::ConfirmationModal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfirmationModal)
{
    ui->setupUi(this);

    if (!parent) {
        qWarning() << "NO PARENT FOR THE CONFIRMATION MODAL, COULD NOT INITIALIZE GEOMETRY";
        return;
    }

    int w = parent->width();
    int h = parent->height();
    this->setGeometry(w / 2 - this->width() / 2, h / 2 - this->height() / 2, 2 * w / 3, 400);
    // Add the panel to the window
    this->show();
}

ConfirmationModal::~ConfirmationModal()
{
    delete ui;
}

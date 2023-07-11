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

    this->setGeometry(0, 0, 2 * parent->width() / 3, 400);
    // Set the initial position of the panel outside the visible area
    this->move(parent->width(), 0);
    // Add the panel to the window
    this->show();
}

ConfirmationModal::~ConfirmationModal()
{
    delete ui;
}

#include "userdeleteitem.h"
#include "ui_userdeleteitem.h"

UserDeleteItem::UserDeleteItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserDeleteItem)
{
    ui->setupUi(this);
}

UserDeleteItem::~UserDeleteItem()
{
    delete ui;
}

void UserDeleteItem::setFullName(const QString &name)
{
    fullName = name;
    ui->fullNameLabel->setText(fullName);
}

void UserDeleteItem::setEmail(const QString &e)
{
    email = e;
    ui->emailLabel->setText(email);
}

void UserDeleteItem::on_deleteButton_clicked()
{
    emit deleteClicked(userId);
}

#include "userpanel.h"
#include "ui_userpanel.h"
#include "listitem.h"
#include <QDate>
#include <QTime>

UserPanel::UserPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserPanel)
{
    ui->setupUi(this);

    //ListItem* x = new ListItem(QDate(2020, 4, 24), QTime(15, 13, 23), "Test project", "Test car name");
   // ui->protocol_list->addWidget(x);
}

UserPanel::~UserPanel()
{
    delete ui;
}

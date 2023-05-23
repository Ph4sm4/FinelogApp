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

    ListItem* x = new ListItem();
    ui->protocol_list->insertWidget(0, x);
}

UserPanel::~UserPanel()
{
    delete ui;
}

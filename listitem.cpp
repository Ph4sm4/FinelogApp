#include "listitem.h"
#include "ui_listitem.h"
#include <QDate>
#include <QTime>

ListItem::ListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListItem)
{
    ui->setupUi(this);
}

ListItem::~ListItem()
{
    delete ui;
}

ListItem::ListItem(QDate date, QTime time, QString project, QString car_name)
{
    ui->dateLabel->setText(date.toString());
    ui->hourLabel->setText(time.toString());
    ui->car_nameLabel->setText(car_name);
    ui->projectLabel->setText(project);
}

#include "selectdeleteitem.h"
#include "ui_selectdeleteitem.h"

SelectDeleteItem::SelectDeleteItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectDeleteItem)
{
    ui->setupUi(this);
}

SelectDeleteItem::~SelectDeleteItem()
{
    delete ui;
}

#include "selectdeleteitem.h"
#include <QJsonArray>
#include <QJsonValue>
#include <QString>
#include "ui_selectdeleteitem.h"

SelectDeleteItem::SelectDeleteItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SelectDeleteItem)
{
    ui->setupUi(this);
}

SelectDeleteItem::SelectDeleteItem(QJsonArray *val, int index)
    : itemIndex(index)
    , controlledValue(val)
{
    ui->fieldEdit->setText(controlledValue->at(index).toString());
}

SelectDeleteItem::~SelectDeleteItem()
{
    delete ui;
}

void SelectDeleteItem::on_passwordEdit_textChanged(const QString &arg1)
{
    controlledValue->replace(itemIndex, arg1);
}

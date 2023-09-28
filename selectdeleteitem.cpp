#include "selectdeleteitem.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include "ui_selectdeleteitem.h"

SelectDeleteItem::SelectDeleteItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SelectDeleteItem)
{
    ui->setupUi(this);
}

SelectDeleteItem::SelectDeleteItem(QJsonObject *val, const QString &key, int index)
    : ui(new Ui::SelectDeleteItem)
    , itemIndex(index)
    , controlledValue(val)
    , selectKey(key)
{
    ui->setupUi(this);

    ui->fieldEdit->setText(val->value(selectKey).toArray().at(index).toString());
}

SelectDeleteItem::~SelectDeleteItem()
{
    delete ui;
}

void SelectDeleteItem::on_fieldEdit_textChanged(const QString &arg1)
{
    QJsonArray jsonArray = (*controlledValue)[selectKey].toArray();
    jsonArray[itemIndex] = arg1;
    (*controlledValue)[selectKey] = jsonArray;
}

void SelectDeleteItem::on_deleteButton_clicked()
{
    QJsonArray jsonArray = (*controlledValue)[selectKey].toArray();
    jsonArray.replace(itemIndex, QJsonValue::Undefined);
    (*controlledValue)[selectKey] = jsonArray;

    this->hide();
    this->deleteLater();
    delete this;
}

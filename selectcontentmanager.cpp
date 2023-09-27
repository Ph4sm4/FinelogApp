#include "selectcontentmanager.h"
#include <QJsonArray>
#include "selectdeleteitem.h"
#include "ui_selectcontentmanager.h"

SelectContentManager::SelectContentManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectContentManager)
{
    ui->setupUi(this);
}

SelectContentManager::~SelectContentManager()
{
    delete ui;
}

void SelectContentManager::setTitle(const QString &newTitle)
{
    ui->title->setText(newTitle);
}

void SelectContentManager::setItems(QJsonArray *newFields)
{
    fieldsArray = newFields;

    QWidget *w = ui->fieldSelectWidgets;
    if (!w) {
        qCritical() << "SCROLL AREA widget DOES NOT EXIST";
        return;
    }
    QVBoxLayout *existingLayout = qobject_cast<QVBoxLayout *>(w->layout());
    if (!existingLayout) {
        qCritical() << "SCROLL AREA LAYOUT DOES NOT EXIST";
        return;
    }

    // clearing out any remaining widgets
    while (QLayoutItem *item = existingLayout->takeAt(0)) {
        if (QWidget *widget = item->widget())
            widget->deleteLater();

        delete item;
    }

    for (int i = 0; i < fieldsArray->size(); i++) {
        SelectDeleteItem *fieldItem = new SelectDeleteItem(fieldsArray, i);

        existingLayout->addWidget(fieldItem);
    }
}

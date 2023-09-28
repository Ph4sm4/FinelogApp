#include "selectcontentmanager.h"
#include <QGraphicsDropShadowEffect>
#include <QJsonArray>
#include <QJsonObject>
#include <QScroller>
#include "selectdeleteitem.h"
#include "ui_selectcontentmanager.h"

SelectContentManager::SelectContentManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectContentManager)
{
    ui->setupUi(this);

    QWidget *w = ui->fieldSelectWidgets;
    if (!w) {
        qCritical() << "SCROLL AREA widget DOES NOT EXIST";
        return;
    }
    existingLayout = qobject_cast<QVBoxLayout *>(w->layout());
    if (!existingLayout) {
        qCritical() << "SCROLL AREA LAYOUT DOES NOT EXIST";
        return;
    }

    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QScroller::grabGesture(ui->scrollArea->viewport(),
                           QScroller::TouchGesture); // Enable touch scrolling

    // Configure the scrolling behavior
    QScrollerProperties scrollerProperties = QScroller::scroller(ui->scrollArea->viewport())
                                                 ->scrollerProperties();
    scrollerProperties.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.6);
    scrollerProperties.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.0);
    scrollerProperties.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.6);
    scrollerProperties.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.4);
    scrollerProperties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy,
                                       QScrollerProperties::OvershootAlwaysOff);
    QScroller::scroller(ui->scrollArea->viewport())->setScrollerProperties(scrollerProperties);

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(0, 0, 0, 80));
    shadowEffect->setOffset(0, 0);
    ui->scrollArea->setGraphicsEffect(shadowEffect);
}

SelectContentManager::~SelectContentManager()
{
    delete ui;
}

void SelectContentManager::setTitle(const QString &newTitle)
{
    ui->title->setText(newTitle);
    key = newTitle;
}

void SelectContentManager::setItems(QJsonObject *newFields)
{
    dataObject = newFields;

    // clearing out any remaining widgets
    while (QLayoutItem *item = existingLayout->takeAt(0)) {
        if (QWidget *widget = item->widget())
            widget->deleteLater();

        delete item;
    }

    for (int i = 0; i < dataObject->value(key).toArray().size(); i++) {
        SelectDeleteItem *fieldItem = new SelectDeleteItem(dataObject, key, i);

        existingLayout->addWidget(fieldItem);
    }
    existingLayout->addStretch();
}

void SelectContentManager::on_addField_clicked()
{
    QJsonArray jsonArray = (*dataObject)[key].toArray();
    jsonArray.push_back("");
    (*dataObject)[key] = jsonArray;

    // clearing out any remaining widgets
    while (QLayoutItem *item = existingLayout->takeAt(0)) {
        if (QWidget *widget = item->widget())
            widget->deleteLater();

        delete item;
    }

    for (int i = 0; i < dataObject->value(key).toArray().size(); i++) {
        SelectDeleteItem *fieldItem = new SelectDeleteItem(dataObject, key, i);

        existingLayout->addWidget(fieldItem);
    }
    existingLayout->addStretch();
}

#include "listitem.h"
#include "ui_listitem.h"
#include <QDate>
#include <QTime>
#include <QGraphicsDropShadowEffect>

ListItem::ListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListItem)
{
    ui->setupUi(this);

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(0, 0, 0, 80));
    shadowEffect->setOffset(0, 0);
    ui->frame->setGraphicsEffect(shadowEffect);
}

ListItem::~ListItem()
{
    delete ui;
}

void ListItem::setDate(const QDate &dateOfUpload)
{
    protocolDate = dateOfUpload;
    ui->dateLabel->setText(protocolDate.toString());
}

void ListItem::setTime(const QTime &timeOfUpload)
{
    protocolTime = timeOfUpload;
    ui->timeLabel->setText(protocolTime.toString());
}

void ListItem::setProjectName(const QString &newName)
{
    projectName = newName;
    ui->projectLabel->setText(projectName);
}

void ListItem::setCarName(const QString &newCarName)
{
    carName = newCarName;
    ui->carNameLabel->setText(carName);
}

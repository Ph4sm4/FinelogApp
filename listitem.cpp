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

bool ListItem::event(QEvent* event) {
    if (event->type() == QEvent::TouchBegin || event->type() == QEvent::TouchEnd || event->type() == QEvent::TouchCancel) {
        return true; // Consume touch events to prevent them from being propagated further
    } else if (event->type() == QEvent::Gesture) {
        return gestureEvent(static_cast<QGestureEvent*>(event)); // Handle gesture events
    }
    return QWidget::event(event); // Call the base class implementation for other events
}

bool ListItem::gestureEvent(QGestureEvent* event) {
    if (QGesture* gesture = event->gesture(Qt::TapGesture)) {
        if (QTapGesture* tapGesture = static_cast<QTapGesture*>(gesture)) {
            if (tapGesture->state() == Qt::GestureFinished) {
                emit clicked(this->contentName); // Emit the clicked signal when the tap gesture is finished
                return true;
            }
        }
    }
    return false;
}

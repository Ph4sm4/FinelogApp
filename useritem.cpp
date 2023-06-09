#include "useritem.h"
#include <QGraphicsDropShadowEffect>
#include "ui_useritem.h"

UserItem::UserItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserItem)
{
    ui->setupUi(this);

    user = new FinelogUser();

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(0, 0, 0, 80));
    shadowEffect->setOffset(0, 0);
    ui->frame->setGraphicsEffect(shadowEffect);
}

UserItem::~UserItem()
{
    delete ui;
}

void UserItem::setNewUpload(bool upload)
{
    newUpload = upload;
    if (newUpload) {
        ui->newUpload->setText("NEW UPLOAD");
    } else {
        ui->newUpload->setText("");
    }
}

void UserItem::setFullName(const QString &name, const QString &surname)
{
    QString fullName = name + " " + surname;
    user->setName(name);
    user->setSurname(surname);
    ui->fullNameLabel->setText(fullName);
}

void UserItem::setEmail(const QString &newEmail)
{
    user->setEmail(newEmail);
    ui->emailLabel->setText(newEmail);
}

void UserItem::setNumberOfUploads(const int &uploadNumber)
{
    numberOfUploads = uploadNumber;
    ui->numberOfUploads->setText("Uploads: " + QString::number(numberOfUploads));
}

bool UserItem::event(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin || event->type() == QEvent::TouchEnd
        || event->type() == QEvent::TouchCancel) {
        return true; // Consume touch events to prevent them from being propagated further
    } else if (event->type() == QEvent::Gesture) {
        return gestureEvent(static_cast<QGestureEvent *>(event)); // Handle gesture events
    }
    return QWidget::event(event); // Call the base class implementation for other events
}

bool UserItem::gestureEvent(QGestureEvent *event)
{
    if (QGesture *gesture = event->gesture(Qt::TapGesture)) {
        if (QTapGesture *tapGesture = static_cast<QTapGesture *>(gesture)) {
            if (tapGesture->state() == Qt::GestureFinished) {
                emit clicked(user); // Emit the clicked signal when the tap gesture is finished
                return true;
            }
        }
    }
    return false;
}

void UserItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked(user);
    }
    QWidget::mousePressEvent(event); // Call the base class implementation
}

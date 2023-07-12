#include "successbox.h"
#include <QPropertyAnimation>
#include <QTimer>
#include "ui_successbox.h"

SuccessBox::SuccessBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SuccessBox)
{
    ui->setupUi(this);

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [this, parent]() -> void {
        QPropertyAnimation *animationBack = new QPropertyAnimation(this, "pos", parent);
        animationBack->setDuration(300);
        animationBack->setEasingCurve(QEasingCurve::InOutQuad);
        animationBack->setEndValue(QPoint(-140, 40));
        animationBack->start();

        connect(animationBack, &QPropertyAnimation::finished, this, [this]() -> void { hide(); });
    });

    setFixedSize(140, 40);
    ui->message->setStyleSheet(
        "QLabel { padding: 5px 10px; background: rgb(80, 200, 120); color: white; "
        "border: none; border-radius: 6px; font-size: 18px; font-weight: bold}");
    raise();
    show();
    move(-140, 40);
}

SuccessBox::~SuccessBox()
{
    delete ui;
}

void SuccessBox::comeIntoView()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos", parent());
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->setEndValue(QPoint(0, 40));
    animation->start();
    // CODE FOR ANIMATING THE SLIDE OF THE SUCCESS BOX IN AND OUT
    //
    //

    connect(animation, &QPropertyAnimation::finished, this, [this]() -> void {
        // after 3 seconds we would like to animate back
        timer->stop();
        timer->start(3000);
    });
}

void SuccessBox::setMessage(const QString &message)
{
    ui->message->setText(message);
}

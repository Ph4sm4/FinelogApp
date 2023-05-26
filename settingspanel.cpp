#include "settingspanel.h"
#include "ui_settingspanel.h"
#include <QPropertyAnimation>
#include "overlaywidget.h"
#include "fineloguser.h"
#include "inputmanager.h"

SettingsPanel::SettingsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsPanel)
{
    ui->setupUi(this);
    if(!parent) {
        qWarning() << "NO PARENT FOR THE SETTINGS PANEL, COULD NOT INITIALIZE GEOMETRY";
        return;
    }

    this->setGeometry(0, 0, 2 * parent->width() / 3, parent->height());
    // Set the initial position of the panel outside the visible area
    this->move(parent->width(), 0);
    // Add the panel to the main window
    this->show();
}

void SettingsPanel::performAnimation(const int& duration, const QPoint& endPoint, QObject* parent)
{
    // Create a property animation for the panel's position
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos", parent);
    animation->setDuration(duration);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    // Set the animation's target value (the final position of the panel)
    animation->setEndValue(endPoint);
    animation->start();
}

SettingsPanel::~SettingsPanel()
{
    delete ui;
}

void SettingsPanel::on_logoutButton_clicked()
{
    this->hide();
    if(overlay) {
        overlay->hide();
    }
    else {
        qWarning() << "OVERLAY FOR THIS SETTINGS PANEL HAS NOT BEEN SET";
    }

    emit logOutButtonClicked();
}


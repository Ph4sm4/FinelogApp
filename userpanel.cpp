#include "userpanel.h"
#include "ui_userpanel.h"
#include "listitem.h"
#include <QDate>
#include <QTime>
#include <QJsonDocument>
#include <QScroller>
#include <QScrollerProperties>

UserPanel::UserPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserPanel)
{
    ui->setupUi(this);

    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // Show vertical scroll bar as needed
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Disable horizontal scroll bar

    QWidget *central = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(central);
    ui->scrollArea->setWidget(central);
    ui->scrollArea->setWidgetResizable(true);

    ListItem* x1 = new ListItem();
    ListItem* x2 = new ListItem();
    ListItem* x3 = new ListItem();
    ListItem* x4 = new ListItem();
    ListItem* x5 = new ListItem();
    ListItem* x6 = new ListItem();
    ListItem* x7 = new ListItem();
    ListItem* x8 = new ListItem();
    ListItem* x9 = new ListItem();
    ListItem* x10 = new ListItem();

    layout->addWidget(x1);
    layout->addWidget(x2);
    layout->addWidget(x3);
    layout->addWidget(x4);
    layout->addWidget(x5);
    layout->addWidget(x6);
    layout->addWidget(x7);
    layout->addWidget(x8);
    layout->addWidget(x9);
    layout->addWidget(x10);

    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::TouchGesture); // Enable touch scrolling

    // Configure the scrolling behavior
    QScrollerProperties scrollerProperties = QScroller::scroller(
                                                 ui->scrollArea->viewport())->scrollerProperties();
    scrollerProperties.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.6);
    scrollerProperties.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.0);
    scrollerProperties.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.6);
    scrollerProperties.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.4);
    QScroller::scroller(ui->scrollArea->viewport())->setScrollerProperties(scrollerProperties);
}

UserPanel::~UserPanel()
{
    delete ui;
}



void UserPanel::on_logoutButton_clicked()
{
    delete currentUser;
    emit logOutButtonClicked();

}


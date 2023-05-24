#include "userpanel.h"
#include "ui_userpanel.h"
#include "listitem.h"
#include <QDate>
#include <QTime>
#include <QJsonDocument>

UserPanel::UserPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserPanel)
{
    ui->setupUi(this);

    QWidget* contentWidget = new QWidget;
    ui->scroll_list->setWidget(contentWidget);
    ui->scroll_list->setWidgetResizable(true); // Allow the content widget to resize within the scroll area
    ui->scroll_list->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // Show vertical scroll bar as needed
    ui->scroll_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Disable horizontal scroll bar

    QVBoxLayout* layout = new QVBoxLayout(contentWidget);
    ListItem* x = new ListItem();
    layout->insertWidget(0, x);
    layout->insertWidget(1, x);
    layout->insertWidget(2, x);
    layout->insertWidget(3, x);
    layout->insertWidget(4, x);
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


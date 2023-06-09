#include "adminpanel.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include "fineloguser.h"
#include "ui_adminpanel.h"
#include "useritem.h"

AdminPanel::AdminPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminPanel)
{
    ui->setupUi(this);
}

AdminPanel::~AdminPanel()
{
    delete ui;
}

void AdminPanel::initializeDashboard()
{
    QString usersPath = "Users";
    QString queryParams = "orderBy=\"isAdmin\"&equalTo=false";
    QJsonObject usersObject = dbHandler.performAuthenticatedGET(usersPath,
                                                                adminUser->getIdToken(),
                                                                queryParams);

    ui->usersNumberLabel->setText("Current users: " + QString::number(usersObject.keys().size()));

    QWidget *w = ui->scrollAreaWidgetContents;
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

    if (usersObject.keys().size() == 0) {
        QLabel *noUsers = new QLabel();
        QFont font;
        font.setBold(true);
        font.capitalization();
        font.setFamily("Microsoft JhengHei");
        font.setPointSize(36);
        font.setItalic(true);
        font.setLetterSpacing(QFont::AbsoluteSpacing, 5);
        noUsers->setFont(font);
        noUsers->setText("NO USERS");
        noUsers->setStyleSheet("QLabel {background: transparent;color: rgb(102, 102, 102);}");
        noUsers->setAlignment(Qt::AlignCenter);

        existingLayout->addWidget(noUsers);
        return;
    }

    for (const QString &userId : usersObject.keys()) {
        QJsonObject user = usersObject.value(userId).toObject();

        UserItem *item = new UserItem();
        item->setUserId(userId);
        item->setEmail(user.value("email").toString());
        item->setFullName(user.value("name").toString(), user.value("surname").toString());

        QString unreadPath = "Admin/Unread";
        queryParams = "orderBy=\"owner_id\"&equalTo=\"" + userId + "\"";
        QJsonObject unreadProtocols = dbHandler.performAuthenticatedGET(unreadPath,
                                                                        adminUser->getIdToken(),
                                                                        queryParams);

        if (unreadProtocols.keys().size() > 0) {
            item->setNewUpload(true);
        } else {
            item->setNewUpload(false);
        }

        QString headlinesPath = "Reports/Headlines";
        QJsonObject uploadedHeadlines = dbHandler.performAuthenticatedGET(headlinesPath,
                                                                          adminUser->getIdToken(),
                                                                          queryParams);

        item->setNumberOfUploads(uploadedHeadlines.keys().size());

        existingLayout->addWidget(item);
    }
    // Add a stretch at the end to push the widgets to the top
    existingLayout->addStretch();
}

//{
//    "EBd9QiQakhV6wgFmpc3yso1SOQn1" : {
//        "email": "olaf.dalach@gmail.com",
//        "finelog_id": "1234",
//        "isAdmin": true,
//        "name": "Iwo",
//        "phone_number": "7305213212",
//        "surname": "Dalach",
//        "user_id": "EBd9QiQakhV6wgFmpc3yso1SOQn1"
//    },
//    "cDFpIntWBGbvdcQ5ImNqUWJhPRy1":
//    {
//        "email" : "dalach.olaf@gmail.com",
//        "finelog_id" : "123",
//        "isAdmin" : false,
//        "name" : "Olaf",
//                                                      "phone_number" : "7304265390",
//                                                                       "surname" : "Dalach",
//                                                                                   "user_id"
//            : "cDFpIntWBGbvdcQ5ImNqUWJhPRy1"
//    }
//}

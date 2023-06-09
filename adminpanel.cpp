#include "adminpanel.h"
#include <QGraphicsDropShadowEffect>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPropertyAnimation>
#include <QScrollBar>
#include <QScroller>
#include <QScrollerProperties>
#include <QVariantMap>
#include "fineloguser.h"
#include "listitem.h"
#include "protocolform.h"
#include "ui_adminpanel.h"
#include "useritem.h"

AdminPanel::AdminPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminPanel)
{
    ui->setupUi(this);

    ui->pagination->setCurrentIndex(0);

    ui->scrollArea->setVerticalScrollBarPolicy(
        Qt::ScrollBarAsNeeded); // Show vertical scroll bar as needed
    ui->scrollArea->setHorizontalScrollBarPolicy(
        Qt::ScrollBarAlwaysOff); // Disable horizontal scroll bar

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

    ui->scrollArea_2->setWidgetResizable(true);
    ui->scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea_2->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QScroller::grabGesture(ui->scrollArea_2->viewport(),
                           QScroller::TouchGesture); // Enable touch scrolling

    // Configure the scrolling behavior
    scrollerProperties = QScroller::scroller(ui->scrollArea_2->viewport())->scrollerProperties();
    QScroller::scroller(ui->scrollArea_2->viewport())->setScrollerProperties(scrollerProperties);
    ui->scrollArea_2->setGraphicsEffect(shadowEffect);
}

AdminPanel::~AdminPanel()
{
    delete ui;
}

void AdminPanel::clickedOnUser(FinelogUser *user)
{
    previewUser = user;
    QVector<ReportHeadline> reports = user->getHeadlines();

    ui->fullNameLabel->setText(user->getName() + " " + user->getSurname());
    ui->phoneNumberLabel->setText("Phone number: " + user->getPhoneNumber());
    ui->emailLabel->setText("Email: " + user->getEmail());
    ui->idNumberLabel->setText("Finelog ID: " + user->getFinelogId());
    ui->reportsNumberLabel->setText("Reports uploaded: " + QString::number(reports.size()));
    ui->joinedOnLabel->setText("Joined on: " + user->getAccountCreatedAt().toString());

    QWidget *w = ui->scrollAreaWidgetContents_2;
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

    if (reports.size() == 0) {
        QLabel *reportsEmpty = new QLabel();
        QFont font;
        font.setBold(true);
        font.capitalization();
        font.setFamily("Microsoft JhengHei");
        font.setPointSize(36);
        font.setItalic(true);
        font.setLetterSpacing(QFont::AbsoluteSpacing, 5);
        reportsEmpty->setFont(font);
        reportsEmpty->setText("EMPTY");
        reportsEmpty->setStyleSheet("QLabel {background: transparent;color: rgb(102, 102, 102);}");
        reportsEmpty->setAlignment(Qt::AlignCenter);

        existingLayout->addWidget(reportsEmpty);
    } else {
        foreach (ReportHeadline headline, reports) {
            ListItem *newItem = new ListItem();
            newItem->setCarName(headline.carName);
            newItem->setProjectName(headline.projectName);
            newItem->setDate(headline.uploadDate);
            newItem->setTime(headline.uploadTime);
            newItem->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            newItem->setParent(ui->scrollAreaWidgetContents);
            newItem->setContentName(headline.contentName);

            connect(newItem, &ListItem::clicked, this, &AdminPanel::projectDetailsRequested);

            existingLayout->addWidget(newItem);
        }
        // Add a stretch at the end to push the widgets to the top
        existingLayout->addStretch();
    }

    ui->pagination->setCurrentIndex(1);
}

void AdminPanel::formReadyForDeletion() {}

void AdminPanel::projectDetailsRequested(const QString &contentName)
{
    form = new ProtocolForm();
    form->setCurrentUser(previewUser);
    form->initializeFormData(contentName, ui->protocolTitle);
    form->prepareFormToInspect();
    form->hideSendOptions();

    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->protocol_form->layout());
    if (layout) {
        layout->addWidget(form);
        ui->pagination->setCurrentIndex(1);
        qDebug() << "successfully added project details form";
    } else {
        qDebug() << "no layout";
    }
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
        QJsonObject userObject = usersObject.value(userId).toObject();

        UserItem *item = new UserItem();
        item->setEmail(userObject.value("email").toString());
        item->setUserId(userId);
        item->setFullName(userObject.value("name").toString(),
                          userObject.value("surname").toString());
        item->setPhoneNumber(userObject.value("phone_number").toString());
        item->setFinelogId(userObject.value("finelog_id").toString());
        item->setIdToken(adminUser->getIdToken());
        item->setAccountCreatedAt(
            QDate::fromString(userObject.value("accountCreatedAt").toString()));

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

        connect(item, &UserItem::clicked, this, &AdminPanel::clickedOnUser);

        existingLayout->addWidget(item);
    }
    // Add a stretch at the end to push the widgets to the top
    existingLayout->addStretch();
}

void AdminPanel::on_backToPanel_clicked()
{
    ui->pagination->setCurrentIndex(0);
}

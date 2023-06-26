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
#include <algorithm>
#include <queue>

AdminPanel::AdminPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AdminPanel)
{
    ui->setupUi(this);

    ui->pagination->setCurrentIndex(0);

    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ui->scrollArea_2->setWidgetResizable(true);
    ui->scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea_2->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QScroller::grabGesture(ui->scrollArea->viewport(),
                           QScroller::TouchGesture); // Enable touch scrolling

    QScroller::grabGesture(ui->scrollArea_2->viewport(),
                           QScroller::TouchGesture); // Enable touch scrolling

    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(0, 0, 0, 80));
    shadowEffect->setOffset(0, 0);
    ui->scrollArea->setGraphicsEffect(shadowEffect);

    QGraphicsDropShadowEffect *shadowEffect2 = new QGraphicsDropShadowEffect;
    shadowEffect2->setBlurRadius(20);
    shadowEffect2->setColor(QColor(0, 0, 0, 80));
    shadowEffect2->setOffset(0, 0);
    ui->scrollArea_2->setGraphicsEffect(shadowEffect2);

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

    // Configure the scrolling behavior
    scrollerProperties = QScroller::scroller(ui->scrollArea_2->viewport())->scrollerProperties();
    scrollerProperties.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.6);
    scrollerProperties.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.0);
    scrollerProperties.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.6);
    scrollerProperties.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.4);
    scrollerProperties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy,
                                       QScrollerProperties::OvershootAlwaysOff);
    QScroller::scroller(ui->scrollArea_2->viewport())->setScrollerProperties(scrollerProperties);

    ui->sortHeadlinesCombo->addItem("-- Sortuj --");
    ui->sortHeadlinesCombo->addItem("Nieprzeczytane");
    ui->sortHeadlinesCombo->addItem("Najnowsze");
    ui->sortHeadlinesCombo->addItem("Najstarsze");
    ui->sortHeadlinesCombo->addItem("Numer rejestracyjny");
}

AdminPanel::~AdminPanel()
{
    delete ui;
}

void AdminPanel::clearUser()
{
    adminUser = nullptr;
    delete adminUser;
}

void AdminPanel::initializeUserPreview(FinelogUser *user)
{
    previewUser = user;
    QVector<ReportHeadline> reports = user->getHeadlines();
    QVector<QString> unreadProtocols = user->getUnreadProtocols();

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
            newItem->setHasBeenRead(!unreadProtocols.contains(headline.contentName));
            newItem->setAdminIdToken(adminUser->getIdToken());

            if (unreadProtocols.contains(headline.contentName)) {
                newItem->setStyleSheet(
                    "QFrame#contentFrame { background-color: white; padding: 5px 5px; "
                    "border: 2px solid #68C668; }");
            }
            connect(newItem, &ListItem::clicked, this, &AdminPanel::projectDetailsRequested);

            existingLayout->addWidget(newItem);
        }
        // Add a stretch at the end to push the widgets to the top
        existingLayout->addStretch();
    }

    ui->pagination->setCurrentIndex(1);
}

void AdminPanel::formReadyForDeletion()
{
    ui->pagination->setCurrentIndex(0);

    ui->pagination->removeWidget(form);
    form->deleteLater();
    form = nullptr;
}

void AdminPanel::projectDetailsRequested(const QString &contentName)
{
    QString deletePath = "Admin/Unread/" + contentName;
    bool success = dbHandler.deleteDatabaseEntry(deletePath, adminUser->getIdToken());
    if (!success) {
        qDebug() << "could not delete from unread: " << contentName;
        return;
    }

    form = new ProtocolForm();
    form->setCurrentUser(previewUser);
    form->initializeFormData(contentName, ui->protocolTitle);
    form->prepareFormToInspect();
    form->hideSendOptions();

    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(ui->protocol_form->layout());
    if (layout) {
        layout->addWidget(form);
        ui->pagination->setCurrentIndex(2);
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

    // displaying a proper label if there are no users registered
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

    // initializing the user list
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

        qDebug() << unreadProtocols;

        item->unreadProtocolsForUser = unreadProtocols.keys();
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

        connect(item, &UserItem::clicked, this, &AdminPanel::initializeUserPreview);

        existingLayout->addWidget(item);
    }
    // Add a stretch at the end to push the widgets to the top
    existingLayout->addStretch();
}

void AdminPanel::on_backToPanel_clicked()
{
    previewUser = nullptr;
    delete previewUser;
    ui->pagination->setCurrentIndex(0);
    ui->sortHeadlinesCombo->setCurrentIndex(0);
    initializeDashboard();
}

void AdminPanel::on_backToPreview_clicked()
{
    // we want to initialize and update the dashboard once more as we might have made changes to the database
    // e.g. read a previously unread protocol
    //initializeUserPreview(previewUser);
    formReadyForDeletion();

    ui->pagination->setCurrentIndex(1);
}

void AdminPanel::on_logOutButton_clicked()
{
    emit logOutButtonClicked();
}

void AdminPanel::on_sortHeadlinesCombo_currentTextChanged(const QString &arg1)
{
    qDebug() << "sort text changed to: " << arg1;

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
    QVector<ListItem *> items;

    // clearing out any remaining widgets and adding them to the sort list
    while (QLayoutItem *item = existingLayout->takeAt(0)) {
        if (QWidget *widget = item->widget()) {
            if (ListItem *tempItem = qobject_cast<ListItem *>(widget)) {
                items.push_back(tempItem);
            }
            existingLayout->removeWidget(widget);
        }
    }

    if (arg1 == "Nieprzeczytane") {
        std::sort(items.begin(), items.end(), [](const ListItem *item1, const ListItem *item2) {
            return item1->getHasBeenRead() < item2->getHasBeenRead();
        });

    } else if (arg1 == "Najnowsze") {
        std::sort(items.begin(), items.end(), [](const ListItem *item1, const ListItem *item2) {
            return item1->getDateTime() > item2->getDateTime();
        });
    } else if (arg1 == "Najstarsze") {
        std::sort(items.begin(), items.end(), [](const ListItem *item1, const ListItem *item2) {
            return item1->getDateTime() < item2->getDateTime();
        });
    } else if (arg1 == "Numer rejestracyjny") {
        std::sort(items.begin(), items.end(), [](const ListItem *item1, const ListItem *item2) {
            return item1->getCarName() < item2->getCarName();
        });
    }
    // default - no sort filter, e.g. sort by oldest date
    else {
        std::sort(items.begin(), items.end(), [](const ListItem *item1, const ListItem *item2) {
            return item1->getDateTime() < item2->getDateTime();
        });
    }

    // add all the widgets back (but now they are in the desired sort order)
    foreach (ListItem *item, items) {
        existingLayout->addWidget(item);
    }
    // Add a stretch at the end to push the widgets to the top
    existingLayout->addStretch();
}

#include "userpanel.h"
#include "qgraphicseffect.h"
#include "settingspanel.h"
#include "stylesheetmanipulator.h"
#include "ui_userpanel.h"
#include "listitem.h"
#include <QDate>
#include <QTime>
#include <QJsonDocument>
#include <QScroller>
#include <QScrollerProperties>
#include <QJsonObject>
#include <QScrollBar>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include <QTimer>

UserPanel::UserPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserPanel)
{
    ui->setupUi(this);

    ui->pagination->setCurrentIndex(0);

    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); // Show vertical scroll bar as needed
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Disable horizontal scroll bar

    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::TouchGesture); // Enable touch scrolling

    // Configure the scrolling behavior
    QScrollerProperties scrollerProperties = QScroller::scroller(
                                                 ui->scrollArea->viewport())->scrollerProperties();
    scrollerProperties.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.6);
    scrollerProperties.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.0);
    scrollerProperties.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.6);
    scrollerProperties.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.4);
    scrollerProperties.setScrollMetric(QScrollerProperties::HorizontalOvershootPolicy, QScrollerProperties::OvershootAlwaysOff);
    QScroller::scroller(ui->scrollArea->viewport())->setScrollerProperties(scrollerProperties);

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setBlurRadius(20);
    shadowEffect->setColor(QColor(0, 0, 0, 80));
    shadowEffect->setOffset(0, 0);
    ui->scrollArea->setGraphicsEffect(shadowEffect);
}

UserPanel::~UserPanel()
{
    delete ui;
}

void UserPanel::clearUser()
{
    delete currentUser;
    currentUser = nullptr;
}

void UserPanel::setSettingsPanel(SettingsPanel *newPanel)
{
    settingsPanel = newPanel;
    connect(settingsPanel, &SettingsPanel::userDetailsChange, this, &UserPanel::userBasicDetailsChange);
}

void UserPanel::userBasicDetailsChange()
{
    ui->hiLabel->setText("Hi " + currentUser->getName() + "!");
    ui->phoneNumberLabel->setText("Phone number: " + currentUser->getPhoneNumber());
    ui->emailLabel->setText("Email: " + currentUser->getEmail());
    ui->idNumberLabel->setText("Finelog ID: " + currentUser->getFinelogId());
    ui->reportsNumberLabel->setText("Reports uploaded: " + QString::number(reports.size()));
    ui->joinedOnLabel->setText("Joined on: " + currentUser->getAccountCreatedAt().toString());

    QLabel* successBox = new QLabel();
    successBox->setFixedSize(140, 40);
    successBox->setText("Success!");
    successBox->setStyleSheet("QLabel { padding: 5px 10px; background-color: rgb(170, 255, 0); color: white; border-radius: 6px; font-size: 18px; font-weight: bold}");
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setBlurRadius(10);
    shadowEffect->setColor(Qt::black);
    shadowEffect->setOffset(0, 2);
    successBox->setGraphicsEffect(shadowEffect);
    successBox->show();
    successBox->raise();
// something is wrong with this, try fixing the fact that it appears on top :(

    QTimer::singleShot(0, this, [successBox]() {
        successBox->move(-140, 40);
    });
    // CODE FOR ANIMATING THE SLIDE OF THE SUCCESS BOX IN AND OUT
    //
    //
    QPropertyAnimation *animation = new QPropertyAnimation(successBox, "pos", this);
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->setEndValue(QPoint(0, 40));
    animation->start();

    connect(animation, &QPropertyAnimation::finished, this, [successBox, this]()->void {
        QTimer* timer = new QTimer(this);
        // after 5 seconds we would like to animate back
        connect(timer, &QTimer::timeout, successBox, [successBox, this]()->void {
            QPropertyAnimation *animationBack = new QPropertyAnimation(successBox, "pos", this);
            animationBack->setDuration(300);
            animationBack->setEasingCurve(QEasingCurve::InOutQuad);
            animationBack->setEndValue(QPoint(-140, 40));
            animationBack->start();

            connect(animationBack, &QPropertyAnimation::finished, successBox, [successBox]()->void {
                successBox->deleteLater();
            });
        });

        timer->start(5000);
    });
}

void UserPanel::setUserDisplayInfo()
{
    reports = currentUser->getHeadlines();

    ui->hiLabel->setText("Hi " + currentUser->getName() + "!");
    ui->phoneNumberLabel->setText("Phone number: " + currentUser->getPhoneNumber());
    ui->emailLabel->setText("Email: " + currentUser->getEmail());
    ui->idNumberLabel->setText("Finelog ID: " + currentUser->getFinelogId());
    ui->reportsNumberLabel->setText("Reports uploaded: " + QString::number(reports.size()));
    ui->joinedOnLabel->setText("Joined on: " + currentUser->getAccountCreatedAt().toString());

    QWidget* w = ui->scrollAreaWidgetContents;
    if(!w) {
        qCritical() << "SCROLL AREA widget DOES NOT EXIST";
        return;
    }
    QVBoxLayout* existingLayout = qobject_cast<QVBoxLayout*>(w->layout());
    if(!existingLayout){
        qCritical() << "SCROLL AREA LAYOUT DOES NOT EXIST";
        return;
    }

    // clearing out any remaining widgets
    while (QLayoutItem* item = existingLayout->takeAt(0))
    {
        if (QWidget* widget = item->widget())
            widget->deleteLater();

        delete item;
    }

    if(reports.size() == 0) {
        QLabel* reportsEmpty = new QLabel();
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
    }
    else {
        foreach(ReportHeadline headline, reports) {
            ListItem* newItem = new ListItem();
            newItem->setCarName(headline.carName);
            newItem->setProjectName(headline.projectName);
            newItem->setDate(headline.uploadDate);
            newItem->setTime(headline.uploadTime);
            newItem->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            newItem->setParent(ui->scrollAreaWidgetContents);
            newItem->setContentName(headline.contentName);

            existingLayout->addWidget(newItem);
        }
        // Add a stretch at the end to push the widgets to the top
        existingLayout->addStretch();
    }
}
void UserPanel::on_settingsButton_clicked()
{
    emit settingsButtonClicked();

    //ui->pagination->setCurrentIndex(1);
}


void UserPanel::on_newProtocolButton_clicked()
{
    // uploading report headline and report content data to db
    if(!currentUser->getEmailVerified()) {
        ui->newProtocolButton->setEnabled(false);
        ui->newProtocolButton->setText("Verify your email");
        ui->newProtocolButton->setStyleSheet(
            StylesheetManipulator::updateStylesheetProperty(
                ui->newProtocolButton->styleSheet(),
                "QPushButton", "color", "rgb(136, 136, 136);"));
        ui->newProtocolButton->setStyleSheet(
            StylesheetManipulator::updateStylesheetProperty(
                ui->newProtocolButton->styleSheet(),
                "QPushButton", "background-color", "rgb(102, 102, 102);"));


        // we want to emit this signal so that the user will be
        // guided how to verify the email, eg. the settings panel with
        // this option will appear
        emit settingsButtonClicked();

        return;
    }

    QVariantMap payload;
    payload["CarName"] = "Toyota";
    payload["ProjectName"] = "Paneco-Something";
    payload["Date"] = QDate::currentDate().toString();
    payload["Time"] = QTime::currentTime().toString();
    payload["WindowCondition"] = "Shattered";
    payload["owner_id"] = currentUser->getUserId();

    QString path = "Reports/Content";
    QJsonObject res = dbHandler.performAuthenticatedPOST(path,
                QJsonDocument::fromVariant(payload), currentUser->getIdToken());

    qDebug() << "added content: " << res;

    if(res.contains("error")) {
        // something went wrong, maybe a message box?
        return;
    }

    QString contentName = res.value("name").toString();

    QVariantMap headlinePayload;
    headlinePayload["CarName"] = "Toyota";
    headlinePayload["ProjectName"] = "Paneco-Something";
    headlinePayload["Date"] = QDate::currentDate().toString();
    headlinePayload["Time"] = QTime::currentTime().toString();
    headlinePayload["owner_id"] = currentUser->getUserId();
    headlinePayload["ContentName"] = contentName;

    path = "Reports/Headlines/" + contentName;
    res = dbHandler.performAuthenticatedPUT(path,
                 QJsonDocument::fromVariant(headlinePayload), currentUser->getIdToken());

    qDebug() << "headline put res: " << res;

    currentUser->fetchHeadlines();
    setUserDisplayInfo();
}

#include "userpanel.h"
#include "protocolform.h"
#include "qgraphicseffect.h"
#include "settingspanel.h"
#include "inputmanager.h"
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
#include <QGroupBox>
#define ipcs isPositiveChoiseSelected

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

    emit successBoxDisplayNeeded();
}

void UserPanel::formReadyForDeletion()
{
    setUserDisplayInfo();

    ui->pagination->setCurrentIndex(0);

    ui->pagination->removeWidget(form);
    form->deleteLater();
    form = nullptr;
}

void UserPanel::projectDetailsRequested(const QString& contentName)
{
    form = new ProtocolForm();
    form->setCurrentUser(currentUser);
    form->prepareForm();
    form->hideSendOptions();
    form->initializeFormData(contentName);

    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->protocol_form->layout());
    if(layout) {
        layout->addWidget(form);
        ui->pagination->setCurrentIndex(1);
        qDebug() << "successfully added project details form";
    }
    else {
        qDebug() << "no layout";
    }

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

            connect(newItem, &ListItem::clicked, this, &UserPanel::projectDetailsRequested);

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
    if(!currentUser->getEmailVerified()) {
        ui->newProtocolButton->setEnabled(false);
        ui->newProtocolButton->setText("Verify your email");
        InputManager::disableButton(ui->newProtocolButton);

        // we want to emit this signal so that the user will be
        // guided how to verify the email, eg. the settings panel with
        // this option will appear
        emit settingsButtonClicked();

        return;
    }
    QVBoxLayout* layout = qobject_cast<QVBoxLayout*>(ui->protocol_form->layout());
    if(layout) {
        bool formFound = false;
        for(QObject* w : ui->protocol_form->children()) {
            if(w->objectName() == "ProtocolForm") {
                qDebug() << "form already exists";
                formFound = true;
            }
        }
        if(!formFound) {
            form = new ProtocolForm();
            form->setCurrentUser(currentUser);
            form->prepareForm();

            connect(form, &ProtocolForm::formSubmitted, this, &UserPanel::formReadyForDeletion);

            layout->addWidget(form);
            qDebug() << "successfully added form";
        }

    }

    ui->pagination->setCurrentIndex(1);

}

void UserPanel::on_backToDashboard_clicked()
{
    formReadyForDeletion();
    ui->pagination->setCurrentIndex(0);
}


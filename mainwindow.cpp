#include "mainwindow.h"
#include "overlaywidget.h"
#include <QGraphicsEffect>
#include "settingspanel.h"
#include "ui_mainwindow.h"
#include "fineloguser.h"
#include <type_traits>
#include <QPropertyAnimation>
#include <QIntValidator>
#include <QMessageBox>
#include "stylesheetmanipulator.h"
#include <QTimer>

/*
 * https://cloud.google.com/identity-platform/docs/use-rest-api#section-confirm-email-verification
 * https://cloud.google.com/identity-platform/docs/reference/rest/v1/accounts/resetPassword
 *
 */

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->finelogLogo->setPixmap(QPixmap(":/images/content/finelog-logo.png"));

    ui->pagination->setCurrentIndex(0); // always start off with the login page

//    foreach(QWidget* widget, QApplication::allWidgets()) {
//        const int pSize = widget->font().pointSize();
//        widget->setFont(QFont("Bahnschrift", pSize));
//        widget->update();
//    }

    ui->loginErrorLabel->setText("");

    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
    shadowEffect->setBlurRadius(5);
    shadowEffect->setColor(QColor(0, 0, 0, 50));
    shadowEffect->setOffset(0, 1);
    ui->finelogLogo->setGraphicsEffect(shadowEffect);

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [this]()->void {
        QPropertyAnimation *animationBack = new QPropertyAnimation(successBox, "pos", this);
        animationBack->setDuration(300);
        animationBack->setEasingCurve(QEasingCurve::InOutQuad);
        animationBack->setEndValue(QPoint(-140, 40));
        animationBack->start();

        connect(animationBack, &QPropertyAnimation::finished, successBox, [this]()->void {
            successBox->hide();
        });
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loggedOutOfUserPanel()
{
    uPanel->clearUser();
    ui->pagination->removeWidget(uPanel);
    uPanel->deleteLater();
    uPanel = nullptr;

    ui->pagination->setCurrentIndex(0);
}

void MainWindow::showUserPanel()
{
    // show the settings panel widget here cause we need the whole window height
    // if we were to do this in the constructor it would not work
    // because at the constructor stage the mainwindow size is not yet defined
    settingsPanel = new SettingsPanel(this);
    overlay = new OverlayWidget(this);
    settingsPanel->setOverlayWidget(overlay);
    connect(settingsPanel, &SettingsPanel::logOutButtonClicked, this, &MainWindow::loggedOutOfUserPanel);

    if(!uPanel) return;

    uPanel->setSettingsPanel(settingsPanel);

    overlay->setStyleSheet("background-color: rgba(0, 0, 0, 0.5);");
    overlay->setGeometry(0, 0, this->width(), this->height());
    overlay->setPanel(settingsPanel);

    overlay->show();
    settingsPanel->raise();
    settingsPanel->setCurrentUser(uPanel->getCurrentUser());
    settingsPanel->performAnimation(200, QPoint(width() - settingsPanel->width(), 0), this);
}

void MainWindow::displaySuccessBox()
{
    QTimer::singleShot(1000, successBox, [this]() {
        successBox = new QLabel();
        successBox->setFixedSize(140, 40);
        successBox->setText("Success!");
        successBox->setStyleSheet("QLabel { padding: 5px 10px; background: rgb(80, 200, 120); color: white; border: none; border-radius: 6px; font-size: 18px; font-weight: bold}");
        successBox->raise();
        successBox->show();
        successBox->move(-140, 40);

        // something is wrong with this, try fixing the fact that it appears on top :(
        QPropertyAnimation *animation = new QPropertyAnimation(successBox, "pos", this);
        animation->setDuration(300);
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        animation->setEndValue(QPoint(0, 40));
        animation->start();
        // CODE FOR ANIMATING THE SLIDE OF THE SUCCESS BOX IN AND OUT
        //
        //

        connect(animation, &QPropertyAnimation::finished, this, [this]()->void {
            // after 5 seconds we would like to animate back
            timer->stop();
            timer->start(5000);
        });
    });
}

void MainWindow::on_loginButton_clicked()
{
    ui->loginButton->setDisabled(true);
    const QString email = ui->emailEdit->text().toLower();
    const QString password = ui->passwordEdit->text();

    bool result = InputManager::validateInputs(ui->emailEdit, ui->passwordEdit);
    if(!result){
        ui->loginButton->setDisabled(false);
        return;
    }

    FinelogUser* loggedInUser = dbHandler.logInWithEmailAndPassword(email, password, ui->loginErrorLabel);
    if(loggedInUser == nullptr) {
        InputManager::setErrorBorder(ui->emailEdit);
        InputManager::setErrorBorder(ui->passwordEdit);
        ui->loginButton->setDisabled(false);
        return;
    }

    bool success = dbHandler.emailChangedExternallyUpdate(loggedInUser);
    if(!success) {
        ui->loginErrorLabel->setText("An error occured. Try again");
        return;
    }

    uPanel = new UserPanel();
    ui->pagination->insertWidget(5, uPanel);
    uPanel->setCurrentUser(loggedInUser);
    InputManager::clearInputs(ui->emailEdit, ui->passwordEdit);

    connect(uPanel, &UserPanel::logOutButtonClicked, this, &MainWindow::loggedOutOfUserPanel);
    connect(uPanel, &UserPanel::settingsButtonClicked, this, &MainWindow::showUserPanel);
    connect(uPanel, &UserPanel::successBoxDisplayNeeded, this, &MainWindow::displaySuccessBox);

    // change to user panel
    ui->pagination->setCurrentIndex(5);
    ui->loginButton->setDisabled(false);
}

void MainWindow::on_registerButton_clicked()
{
    QString password1 = ui->passwordRegister->text();
    QString password2 = ui->confirmPassword->text();

    if(!InputManager::validatePassword(password1) || !InputManager::validatePassword(password2)
        || password1 != password2) return;

    registrationUser->setPassword(password1);

    bool success = dbHandler.registerNewUser(registrationUser);
    if(!success) return;

    InputManager::clearInputs(ui->nameRegister, ui->surnameRegister,
                              ui->emailRegister, ui->phoneNumberRegister,
                              ui->passwordRegister, ui->confirmPassword,
                              ui->idRegistration);

    // change to user panel
    uPanel = new UserPanel();
    ui->pagination->insertWidget(5, uPanel);
    uPanel->setCurrentUser(registrationUser);

    connect(uPanel, &UserPanel::logOutButtonClicked, this, &MainWindow::loggedOutOfUserPanel);
    connect(uPanel, &UserPanel::settingsButtonClicked, this, &MainWindow::showUserPanel);

    ui->pagination->setCurrentIndex(5);
}

void MainWindow::on_registerGoTo_clicked()
{
    registrationUser = new FinelogUser();
    if(!registrationUser) {
         qFatal("failed to instantiate registration user object");
         return;
    }
    InputManager::clearInputs(ui->emailEdit, ui->passwordEdit);
    ui->pagination->setCurrentIndex(1);
}


void MainWindow::on_idNext_clicked()
{
    QString id = ui->idRegistration->text();
    bool result = InputManager::validateInputs(ui->idRegistration);
    if(!result) return;

    // check somehow that the id exists

    registrationUser->setFinelogId(id);
    ui->pagination->setCurrentIndex(2);
}


void MainWindow::on_nextName_clicked()
{
    QString name = ui->nameRegister->text().toLower();
    QString surname = ui->surnameRegister->text().toLower();

    bool result = InputManager::validateInputs(ui->nameRegister, ui->surnameRegister);
    if(!result) return;

    name[0] = name[0].toUpper();
    surname[0] = surname[0].toUpper();

    registrationUser->setName(name);
    registrationUser->setSurname(surname);

    ui->pagination->setCurrentIndex(3);
}


void MainWindow::on_nextEmail_clicked()
{
    QString email = ui->emailRegister->text().toLower();
    QString phoneNumber = ui->phoneNumberRegister->text();

    bool result = InputManager::validateInputs(ui->emailRegister, ui->phoneNumberRegister);
    if(!result) return;

    bool emailOk = InputManager::validateEmail(email);
    if(!emailOk) {
         InputManager::setErrorBorder(ui->emailRegister);
         qDebug() << "email not ok";
         return;
    }
    if(phoneNumber.length() > 13) {
         InputManager::setErrorBorder(ui->phoneNumberRegister);
         qDebug() << "phone number not ok";
         return;
    }

    registrationUser->setEmail(email);
    registrationUser->setPhoneNumber(phoneNumber);

    ui->pagination->setCurrentIndex(4);
}

void MainWindow::on_loginGoTo_clicked()
{
    delete registrationUser;
    InputManager::clearInputs(ui->idRegistration, ui->emailRegister,
                              ui->nameRegister, ui->surnameRegister, ui->phoneNumberRegister,
                              ui->passwordRegister, ui->confirmPassword);
    ui->pagination->setCurrentIndex(0);
}


void MainWindow::on_loginGoTo_3_clicked()
{
    delete registrationUser;
    InputManager::clearInputs(ui->idRegistration, ui->emailRegister,
                              ui->nameRegister, ui->surnameRegister, ui->phoneNumberRegister,
                              ui->passwordRegister, ui->confirmPassword);
    ui->pagination->setCurrentIndex(0);
}


void MainWindow::on_loginGoTo_4_clicked()
{
    delete registrationUser;
    InputManager::clearInputs(ui->idRegistration, ui->emailRegister,
                              ui->nameRegister, ui->surnameRegister, ui->phoneNumberRegister,
                              ui->passwordRegister, ui->confirmPassword);
    ui->pagination->setCurrentIndex(0);
}


void MainWindow::on_loginGoTo_5_clicked()
{
    delete registrationUser;
    InputManager::clearInputs(ui->idRegistration, ui->emailRegister,
                              ui->nameRegister, ui->surnameRegister, ui->phoneNumberRegister,
                              ui->passwordRegister, ui->confirmPassword);
    ui->pagination->setCurrentIndex(0);
}

void MainWindow::on_confirmPassword_textChanged(const QString &arg1)
{
    if(arg1.length() < 6) {
         ui->registerErrorLabel->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->registerErrorLabel->styleSheet(),
                                                                                       "QLabel", "color", "red"));
         ui->registerErrorLabel->setText("Password must be at least 6 characters long");
    }
    else if(!InputManager::validatePassword(arg1)) {
         ui->registerErrorLabel->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->registerErrorLabel->styleSheet(),
                                                                                       "QLabel", "color", "red"));
         ui->registerErrorLabel->setText("Password must consist of a big and small letter and a number");
    }
    else if(arg1 == ui->passwordRegister->text()) {
         ui->registerErrorLabel->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->registerErrorLabel->styleSheet(),
                                                                                       "QLabel", "color", "green"));
         ui->registerErrorLabel->setText("Passwords match");
    }
    else {
         ui->registerErrorLabel->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->registerErrorLabel->styleSheet(),
                                                                                       "QLabel", "color", "red"));
         ui->registerErrorLabel->setText("Passwords do not match");
    }
}


void MainWindow::on_passwordRegister_textChanged(const QString &arg1)
{
    if(arg1.length() < 6) {
         ui->registerErrorLabel->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->registerErrorLabel->styleSheet(),
                                                                                       "QLabel", "color", "red"));
         ui->registerErrorLabel->setText("Password must be at least 6 characters long");
    }
    else if(!InputManager::validatePassword(arg1)) {
         ui->registerErrorLabel->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->registerErrorLabel->styleSheet(),
                                                                                       "QLabel", "color", "red"));
         ui->registerErrorLabel->setText("Password must consist of a big and small letter and a number");
    }
    else if(arg1 == ui->confirmPassword->text()) {
         ui->registerErrorLabel->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->registerErrorLabel->styleSheet(),
                                                                                       "QLabel", "color", "green"));
         ui->registerErrorLabel->setText("Passwords match");
    }
    else {
         ui->registerErrorLabel->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->registerErrorLabel->styleSheet(),
                                                                                       "QLabel", "color", "red"));
         ui->registerErrorLabel->setText("Passwords do not match");
    }
}


void MainWindow::on_backPassword_clicked()
{
    ui->pagination->setCurrentIndex(3);
}


void MainWindow::on_backEmail_clicked()
{
    ui->pagination->setCurrentIndex(2);
}


void MainWindow::on_backName_clicked()
{
    ui->pagination->setCurrentIndex(1);
}


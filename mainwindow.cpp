#include "mainwindow.h"
#include <QGraphicsEffect>
#include <QIntValidator>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QTimer>
#include "adminpanel.h"
#include "fineloguser.h"
#include "inputmanager.h"
#include "overlaywidget.h"
#include "settingspanel.h"
#include "stylesheetmanipulator.h"
#include "successbox.h"
#include "ui_mainwindow.h"
#include "userpanel.h"
#include <type_traits>
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
    QApplication::setWindowIcon(QIcon(":/images/content/app_icon.ico"));

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

void MainWindow::loggedOutOfAdminPanel()
{
    aPanel->clearUser();
    ui->pagination->removeWidget(aPanel);
    aPanel->deleteLater();
    aPanel = nullptr;

    ui->pagination->setCurrentIndex(0);
}

void MainWindow::showSettingsPanel()
{
    // show the settings panel widget here cause we need the whole window height
    // if we were to do this in the constructor it would not work
    // because at the constructor stage the mainwindow size is not yet defined
    settingsPanel = new SettingsPanel(this);
    overlay = new OverlayWidget(this);

    connect(settingsPanel,
            &SettingsPanel::logOutButtonClicked,
            this,
            &MainWindow::loggedOutOfUserPanel);

    connect(overlay, &OverlayWidget::clicked, settingsPanel, &SettingsPanel::exitFromView);
    connect(settingsPanel,
            &SettingsPanel::settingsPanelClosed,
            overlay,
            &OverlayWidget::exitFromView);

    if (!uPanel)
        return;

    uPanel->setSettingsPanel(settingsPanel);
    overlay->raise();
    overlay->show();

    settingsPanel->raise();
    settingsPanel->setCurrentUser(uPanel->getCurrentUser());
    settingsPanel->comeIntoView();
}

void MainWindow::displaySuccessBox()
{
    QTimer::singleShot(1000, successBox, [this]() {
        successBox = new SuccessBox(this);
        successBox->setMessage("Success");
        successBox->comeIntoView();
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

    if (loggedInUser->getIsAdmin() == false) { // change to user panel
        uPanel = new UserPanel(this);
        ui->pagination->insertWidget(5, uPanel);
        uPanel->setCurrentUser(loggedInUser);
        InputManager::clearInputs(ui->emailEdit, ui->passwordEdit);

        connect(uPanel, &UserPanel::logOutButtonClicked, this, &MainWindow::loggedOutOfUserPanel);
        connect(uPanel, &UserPanel::settingsButtonClicked, this, &MainWindow::showSettingsPanel);
        connect(uPanel, &UserPanel::successBoxDisplayNeeded, this, &MainWindow::displaySuccessBox);

    } else { // change to admin panel
        aPanel = new AdminPanel(this);
        aPanel->setUser(loggedInUser);
        aPanel->setMainWindow(this);
        ui->pagination->insertWidget(5, aPanel);
        InputManager::clearInputs(ui->emailEdit, ui->passwordEdit);

        connect(aPanel, &AdminPanel::logOutButtonClicked, this, &MainWindow::loggedOutOfAdminPanel);
    }

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

    bool success = dbHandler.registerNewUser(registrationUser, ui->registerErrorLabel);
    if(!success) return;

    InputManager::clearInputs(ui->nameRegister, ui->surnameRegister,
                              ui->emailRegister, ui->phoneNumberRegister,
                              ui->passwordRegister, ui->confirmPassword,
                              ui->idRegistration);

    // change to user panel
    uPanel = new UserPanel(this);
    ui->pagination->insertWidget(5, uPanel);
    uPanel->setCurrentUser(registrationUser);

    connect(uPanel, &UserPanel::logOutButtonClicked, this, &MainWindow::loggedOutOfUserPanel);
    connect(uPanel, &UserPanel::settingsButtonClicked, this, &MainWindow::showSettingsPanel);
    connect(uPanel, &UserPanel::successBoxDisplayNeeded, this, &MainWindow::displaySuccessBox);

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
    QString name = ui->nameRegister->text().toLower().trimmed();
    QString surname = ui->surnameRegister->text().toLower().trimmed();

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
    QString email = ui->emailRegister->text().toLower().trimmed();
    QString phoneNumber = ui->phoneNumberRegister->text().trimmed();

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
    goToLogin();
}


void MainWindow::on_loginGoTo_3_clicked()
{
    goToLogin();
}


void MainWindow::on_loginGoTo_4_clicked()
{
    goToLogin();
}


void MainWindow::on_loginGoTo_5_clicked()
{
    goToLogin();
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


void MainWindow::on_sendPasswordReset_clicked()
{
    QString email = ui->passwordForgotEdit->text().trimmed();

    if(!InputManager::validateInputs(ui->passwordForgotEdit)) {
         return;
    }
    else if(!InputManager::validateEmail(email)) {
         ui->passwordForgotError->setText("Enter a valid email");
         InputManager::setErrorBorder(ui->passwordForgotEdit);
         return;
    }

    QJsonObject res = dbHandler.sendPasswordResetLink(email);

    if(res.contains("error")) {
         QJsonObject errorObject = res.value("error").toObject();

         if(errorObject.value("message").toString() == "EMAIL_NOT_FOUND") {
             ui->passwordForgotError->setText("There is no account connected to this email");
         }
         else {
             ui->passwordForgotError->setText("An error occurred. Try again");
         }
         return;
    }

    ui->pagination->setCurrentIndex(6);
    ui->passwordForgotEdit->clear();
}


void MainWindow::on_forgotPassword_clicked()
{
    ui->passwordForgotError->setText("");
    ui->pagination->setCurrentIndex(5);
}


void MainWindow::on_loginGoTo_6_clicked()
{
    ui->pagination->setCurrentIndex(0);
}


void MainWindow::on_backToLogin_clicked()
{
    ui->passwordForgotEdit->clear();
    ui->pagination->setCurrentIndex(0);
}


void MainWindow::on_pagination_currentChanged(int arg1)
{
    ui->loginErrorLabel->setText("");
}

void MainWindow::goToLogin()
{
    delete registrationUser;
    registrationUser = nullptr;
    InputManager::clearInputs(ui->idRegistration, ui->emailRegister,
                              ui->nameRegister, ui->surnameRegister, ui->phoneNumberRegister,
                              ui->passwordRegister, ui->confirmPassword);
    ui->pagination->setCurrentIndex(0);
}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fineloguser.h"
#include <type_traits>
#include <QPropertyAnimation>
#include <QIntValidator>
#include <QMessageBox>
#include "stylesheetmanipulator.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->finelogLogo->setPixmap(QPixmap(":/logos/content/finelog-logo.png"));

    ui->pagination->setCurrentIndex(0); // always start off with the login page

//    foreach(QWidget* widget, QApplication::allWidgets()) {
//        const int pSize = widget->font().pointSize();
//        widget->setFont(QFont("Bahnschrift", pSize));
//        widget->update();
//    }

    uPanel = new UserPanel();
    qDebug() << (uPanel == nullptr);
    ui->pagination->insertWidget(5, uPanel);

    connect(uPanel, &UserPanel::logOutButtonClicked, this, &MainWindow::loggedOutOfUserPanel);

    ui->loginErrorLabel->setText("");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loggedOutOfUserPanel()
{
    ui->pagination->setCurrentIndex(0);
}

void MainWindow::on_loginButton_clicked()
{
    const QString email = ui->emailEdit->text();
    const QString password = ui->passwordEdit->text();

    bool result = InputManager::validateInputs(ui->emailEdit, ui->passwordEdit);
    if(!result) return;

    FinelogUser* loggedInUser = dbHandler.logInWithEmailAndPassword(email, password, ui->loginErrorLabel);
    if(loggedInUser == nullptr) return;

    uPanel->setCurrentUser(loggedInUser);
    InputManager::clearInputs(ui->emailEdit, ui->passwordEdit);

    // change to user panel
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

    ui->pagination->setCurrentIndex(2);
}


void MainWindow::on_nextName_clicked()
{
    QString name = ui->nameRegister->text();
    QString surname = ui->surnameRegister->text();

    bool result = InputManager::validateInputs(ui->nameRegister, ui->surnameRegister);
    if(!result) return;

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


void MainWindow::on_registerButton_clicked()
{
    QString password1 = ui->passwordRegister->text();
    QString password2 = ui->confirmPassword->text();

    if(!InputManager::validatePassword(password1) || !InputManager::validatePassword(password2)
        || password1 != password2) return;

    registrationUser->setPassword(password1);

    FinelogUser* createdUser = dbHandler.registerNewUser(registrationUser);
    if(createdUser == nullptr) return;

    uPanel->setCurrentUser(createdUser);
    InputManager::clearInputs(ui->nameRegister, ui->surnameRegister,
                              ui->emailRegister, ui->phoneNumberRegister,
                              ui->passwordRegister, ui->confirmPassword,
                              ui->idRegistration);

    // change to the UserPanel
    ui->pagination->setCurrentIndex(5);
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


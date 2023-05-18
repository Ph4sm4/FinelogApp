#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fineloguser.h"
#include <type_traits>

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
//!  label->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(label->styleSheet(), "QLabel", "color", "green"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    const QString id = ui->emailEdit->text();
    const QString pass = ui->passwordEdit->text();

    bool result = InputManager::validateInputs(ui->emailEdit, ui->passwordEdit);
    if(!result) return;

    qDebug() << dbHandler.getReponseJson("Pets").value("One");
    dbHandler.addNewUser(new FinelogUser("Olaf", "Dalach", "dalach.olaf@gmail.com"));
}

void MainWindow::on_registerGoTo_clicked()
{
    registrationUser = new FinelogUser();
    if(!registrationUser) {
         qFatal("failed to instantiate registration user object");
         return;
    }
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
    QString email = ui->emailRegister->text();
    QString phoneNumber = ui->phoneNumberRegister->text();

    bool result = InputManager::validateInputs(ui->emailRegister, ui->phoneNumberRegister);

    if(!result) return;

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

}

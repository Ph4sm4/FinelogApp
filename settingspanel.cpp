#include "settingspanel.h"
#include "ui_settingspanel.h"
#include <QPropertyAnimation>
#include "overlaywidget.h"
#include "fineloguser.h"
#include "inputmanager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>
#include "stylesheetmanipulator.h"

SettingsPanel::SettingsPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsPanel)
{
    ui->setupUi(this);
    if(!parent) {
        qWarning() << "NO PARENT FOR THE SETTINGS PANEL, COULD NOT INITIALIZE GEOMETRY";
        return;
    }

    this->setGeometry(0, 0, 2 * parent->width() / 3, parent->height());
    // Set the initial position of the panel outside the visible area
    this->move(parent->width(), 0);
    // Add the panel to the main window
    this->show();

    ui->sendErrorLabel->setText("");

    QPixmap pixmap(":/images/content/driver-min.jpeg");
    ui->ivecoImage->setScaledContents(true);
    ui->ivecoImage->setPixmap(pixmap.scaled(QSize(this->width() - 22, 300), Qt::KeepAspectRatio));

    ui->nameError->clear();
    ui->emailError->clear();
    ui->phoneError->clear();
    ui->passwordError->clear();
}

void SettingsPanel::performAnimation(const int& duration, const QPoint& endPoint, QObject* parent)
{
    // Create a property animation for the panel's position
    QPropertyAnimation *animation = new QPropertyAnimation(this, "pos", parent);
    animation->setDuration(duration);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    // Set the animation's target value (the final position of the panel)
    animation->setEndValue(endPoint);
    animation->start();
}

void SettingsPanel::setCurrentUser(FinelogUser *user)
{
    currentUser = user;
    if(!currentUser->getEmailVerified()) {
        ui->pagination->setCurrentIndex(0);
    }
    else {
        // straight to the settings page
        ui->pagination->setCurrentIndex(2);
    }
}

SettingsPanel::~SettingsPanel()
{
    delete ui;
}

void SettingsPanel::on_logoutButton_clicked()
{
    overlay->closeSettingsPanel(); // it will also hide the settings panel with the proper animation

    emit logOutButtonClicked();
}


void SettingsPanel::on_sendVerifyEmail_clicked()
{
    bool success = dbHandler.sendEmailVerification(currentUser->getIdToken());
    if(success) {
        ui->pagination->setCurrentIndex(1);
        return;
    }

    ui->sendErrorLabel->setText("There has been an error while sending the email");
}


void SettingsPanel::on_closePanel_clicked()
{
    overlay->closeSettingsPanel(); // it will also hide the settings panel with the proper animation
}


void SettingsPanel::on_nameSurnameButton_clicked()
{
    ui->nameEdit->setText(currentUser->getName());
    ui->surnameEdit->setText(currentUser->getSurname());
    InputManager::disableButton(ui->saveName);

    ui->pagination->setCurrentIndex(3);
}


void SettingsPanel::on_emailButton_clicked()
{
    ui->emailEdit->setText(currentUser->getEmail());
    InputManager::disableButton(ui->saveEmail);
    ui->pagination->setCurrentIndex(4);
}


void SettingsPanel::on_phoneButton_clicked()
{
    ui->phoneEdit->setText(currentUser->getPhoneNumber());
    ui->pagination->setCurrentIndex(5);
}


void SettingsPanel::on_passwordButton_clicked()
{
    ui->passwordEdit->setText(currentUser->getPassword());
    ui->confirmPassword->clear();
    ui->pagination->setCurrentIndex(6);
}


void SettingsPanel::on_backEmail_clicked()
{
    ui->pagination->setCurrentIndex(2);
}


void SettingsPanel::on_backName_clicked()
{
    ui->pagination->setCurrentIndex(2);
}


void SettingsPanel::on_backPassword_clicked()
{
    ui->pagination->setCurrentIndex(2);
}


void SettingsPanel::on_backPhone_clicked()
{
    ui->pagination->setCurrentIndex(2);
}


void SettingsPanel::on_nameSave_clicked()
{
    QString name = ui->nameEdit->text().toLower();
    QString surname = ui->surnameEdit->text().toLower();

    name[0] = name[0].toUpper();
    surname[0] = surname[0].toUpper();

    if(!InputManager::validateInputs(ui->nameEdit, ui->surnameEdit) ||
        (name == currentUser->getName() && surname == currentUser->getSurname())) return;

    bool success = dbHandler.changeAuthDisplayName(currentUser->getIdToken(), name + " " + surname);
    if(!success) {
        ui->nameError->setText("An error occurred. Try again");
        return;
    }

    QVariantMap payload;
    payload["Name"] = name;
    payload["Surname"] = surname;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    success = dbHandler.updateUserData(currentUser->getUserId(),
                                       doc, currentUser->getIdToken());

    if(!success) {
        ui->nameError->setText("An error occurred. Try again");
        return;
    }

    currentUser->setName(name);
    currentUser->setSurname(surname);

    InputManager::clearInputsBlack(ui->nameEdit, ui->surnameEdit);

    emit userDetailsChange();

    ui->pagination->setCurrentIndex(2);
}


void SettingsPanel::on_saveEmail_clicked()
{
    QString email = ui->emailEdit->text().toLower();

    if(!InputManager::validateInputs(ui->emailEdit) || !InputManager::validateEmail(email) || email == currentUser->getEmail() || !ui->confirmEmailCheckBox->isChecked()) return;

    QVariantMap payload;
    payload["Email"] = email;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    bool success = dbHandler.updateUserData(currentUser->getUserId(),
                                       doc, currentUser->getIdToken());

    if(!success) {
        ui->emailError->setText("An error occurred. Try again");
        return;
    }

    success = dbHandler.changeAuthUserEmail(currentUser->getIdToken(), email);
    if(!success) {
        ui->emailError->setText("An error occurred. Try again");
        return;
    }

    currentUser->setEmail(email);

    InputManager::clearInputsBlack(ui->emailEdit);

    emit userDetailsChange();

    ui->pagination->setCurrentIndex(2);
}


void SettingsPanel::on_savePhone_clicked()
{
    QString phoneNumber = ui->phoneEdit->text();

    if(!InputManager::validateInputs(ui->phoneEdit) || phoneNumber == currentUser->getPhoneNumber()) return;

    QVariantMap payload;
    payload["Phone_number"] = phoneNumber;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    bool success = dbHandler.updateUserData(currentUser->getUserId(),
                                            doc, currentUser->getIdToken());

    if(!success) {
        ui->phoneError->setText("An error occurred. Try again");
        return;
    }

    currentUser->setPhoneNumber(phoneNumber);

    InputManager::clearInputsBlack(ui->phoneEdit);

    emit userDetailsChange();

    ui->pagination->setCurrentIndex(2);
}


void SettingsPanel::on_savePassword_clicked()
{
    QString password = ui->passwordEdit->text();
    QString confirmPassword = ui->confirmPassword->text();

    if(!InputManager::validateInputs(ui->passwordEdit, ui->confirmPassword) || password == currentUser->getPassword() ||
        password != confirmPassword) return;

    bool success = dbHandler.changeAuthUserPassword(currentUser->getIdToken(), password);
    qDebug() << "success password: " << success;
    if(!success) {
        ui->passwordError->setText("An error occurred. Try again");
        return;
    }

    currentUser->setPassword(password);

    InputManager::clearInputsBlack(ui->passwordEdit);
    InputManager::clearInputsBlack(ui->confirmPassword);

    emit userDetailsChange();

    ui->pagination->setCurrentIndex(2);
}


void SettingsPanel::on_passwordEdit_textChanged(const QString &arg1)
{
    if(arg1.length() < 6) {
        ui->passwordError->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->passwordError->styleSheet(),
                                                                                              "QLabel", "color", "red"));
        ui->passwordError->setText("Password must be at least 6 characters long");
    }
    else if(!InputManager::validatePassword(arg1)) {
        ui->passwordError->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->passwordError->styleSheet(),
                                                                                              "QLabel", "color", "red"));
        ui->passwordError->setText("Password must consist of a big and small letter and a number");
    }
    else if(arg1 == ui->confirmPassword->text()) {
        ui->passwordError->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->passwordError->styleSheet(),
                                                                                              "QLabel", "color", "green"));
        ui->passwordError->setText("Passwords match");
    }
    else {
        ui->passwordError->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->passwordError->styleSheet(),
                                                                                              "QLabel", "color", "red"));
        ui->passwordError->setText("Passwords do not match");
    }

}



void SettingsPanel::on_confirmPassword_textChanged(const QString &arg1)
{
    if(arg1.length() < 6) {
        ui->passwordError->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->passwordError->styleSheet(),
                                                                                         "QLabel", "color", "red"));
        ui->passwordError->setText("Password must be at least 6 characters long");
    }
    else if(!InputManager::validatePassword(arg1)) {
        ui->passwordError->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->passwordError->styleSheet(),
                                                                                         "QLabel", "color", "red"));
        ui->passwordError->setText("Password must consist of a big and small letter and a number");
    }
    else if(arg1 == ui->passwordEdit->text()) {
        ui->passwordError->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->passwordError->styleSheet(),
                                                                                         "QLabel", "color", "green"));
        ui->passwordError->setText("Passwords match");
    }
    else {
        ui->passwordError->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(ui->passwordError->styleSheet(),
                                                                                         "QLabel", "color", "red"));
        ui->passwordError->setText("Passwords do not match");
    }
}


void SettingsPanel::on_emailEdit_textChanged(const QString &email)
{
    QString prevEmail = currentUser->getEmail();
    if(!InputManager::validateEmail(email)) {
        InputManager::setErrorBorder(ui->emailEdit);
        ui->emailError->setText("This is not a valid email");
        InputManager::disableButton(ui->saveEmail);
    }
    else if(email == prevEmail) {
        InputManager::deleteErrorBorderBlack(ui->emailEdit);
        ui->emailError->clear();
        InputManager::disableButton(ui->saveEmail);
    }
    else {
        InputManager::deleteErrorBorderBlack(ui->emailEdit);
        ui->emailError->clear();
        if(ui->confirmEmailCheckBox->isChecked() && email != prevEmail ){
         InputManager::enableButton(ui->saveEmail, "QPushButton {background-color:  rgb(249, 115, 22);height: 30px;border-radius: 10px;color: white;padding: 4px 0;}");
        }
    }
}


void SettingsPanel::on_confirmEmailCheckBox_stateChanged(int arg1)
{
    if(!arg1) {
        InputManager::disableButton(ui->saveEmail);
    }
    else {
        QString email = ui->emailEdit->text();
        QString prevEmail = currentUser->getEmail();
        ui->emailError->clear();
        if(InputManager::validateEmail(email) && email != prevEmail) {
         InputManager::enableButton(ui->saveEmail, "QPushButton {background-color:  rgb(249, 115, 22);height: 30px;border-radius: 10px;color: white;padding: 4px 0;}");
        }
    }
}


void SettingsPanel::on_nameEdit_textChanged(const QString &arg1)
{

}


void SettingsPanel::on_surnameEdit_textChanged(const QString &arg1)
{
    QString name = ui->nameEdit->text().toLower();
    QString surname = ui->surnameEdit->text().toLower();

    name[0] = name[0].toUpper();
    surname[0] = surname[0].toUpper();

    if(!InputManager::validateInputs(ui->nameEdit, ui->surnameEdit) ||
        (name == currentUser->getName() && surname == currentUser->getSurname())) {
        InputManager::disableButton(ui->saveName);
    }
}


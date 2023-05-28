#include "inputmanager.h"
#include "stylesheetmanipulator.h"
#include <QString>
#include <QPushButton>

InputManager::InputManager()
{

}

void InputManager::disableButton(QPushButton *button)
{
    button->setDisabled(true);
    button->setStyleSheet(
        StylesheetManipulator::updateStylesheetProperty(
            button->styleSheet(),
            "QPushButton", "color", "rgb(136, 136, 136);"));
    button->setStyleSheet(
        StylesheetManipulator::updateStylesheetProperty(
            button->styleSheet(),
            "QPushButton", "background-color", "rgb(102, 102, 102);"));
}

void InputManager::enableButton(QPushButton *button, const QString& stylesheet)
{
    button->setEnabled(true);
    button->setStyleSheet(stylesheet);
}

void InputManager::setErrorBorder(QLineEdit *e)
{
    e->setStyleSheet(
        StylesheetManipulator::updateStylesheetProperty(
            e->styleSheet(), "QLineEdit", "border", "1px solid red;"));
    e->setStyleSheet(
        StylesheetManipulator::updateStylesheetProperty(
            e->styleSheet(), "QLineEdit:focus", "border", "2px solid red;"));
}

void InputManager::deleteErrorBorder(QLineEdit *e)
{
    e->setStyleSheet(
        StylesheetManipulator::updateStylesheetProperty(
            e->styleSheet(), "QLineEdit", "border", "1px solid rgb(37, 39, 48);"));
    e->setStyleSheet(
        StylesheetManipulator::updateStylesheetProperty(
            e->styleSheet(), "QLineEdit:focus", "border", "2px solid rgb(37, 39, 48);"));
}

void InputManager::deleteErrorBorderBlack(QLineEdit *e)
{
    e->setStyleSheet(
        StylesheetManipulator::updateStylesheetProperty(
            e->styleSheet(), "QLineEdit", "border", "1px solid black;"));
    e->setStyleSheet(
        StylesheetManipulator::updateStylesheetProperty(
            e->styleSheet(), "QLineEdit:focus", "border", "2px solid black;"));
}

bool InputManager::validatePassword(QString password)
{
    bool containsNum = false;
    bool containsBigLetter = false;
    bool containsSmallLetter = false;
    for(int i = 0; i < password.length(); i++) {
        if(password[i].isUpper()) containsBigLetter = true;
        else if(password[i].isLower()) containsSmallLetter = true;
        else if(password[i].isNumber()) containsNum = true;
    }
    return containsNum && containsBigLetter && containsSmallLetter;
}

bool InputManager::validateEmail(QString email)
{
    QRegularExpression mailREX("^\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}\\b$");
    if(mailREX.match(email).hasMatch()) {
        return true;
    }
    return false;
}

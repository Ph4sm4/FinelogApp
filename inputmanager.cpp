#include "inputmanager.h"
#include "stylesheetmanipulator.h"
#include <QString>

InputManager::InputManager()
{

}

void InputManager::setErrorBorder(QLineEdit *e)
{
    e->setStyleSheet(
        StylesheetManipulator::updateStylesheetProperty(
            e->styleSheet(), "QLineEdit", "border", "2px solid red;"));
}

void InputManager::deleteErrorBorder(QLineEdit *e)
{
    e->setStyleSheet(
        StylesheetManipulator::updateStylesheetProperty(
            e->styleSheet(), "QLineEdit", "border", "2px solid rgb(37, 39, 48);"));
}

void InputManager::deleteErrorBorderBlack(QLineEdit *e)
{
    e->setStyleSheet(
        StylesheetManipulator::updateStylesheetProperty(
            e->styleSheet(), "QLineEdit", "border", "1px solid black;"));
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
    QRegularExpression mailREX("^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$");
    if(mailREX.match(email).hasMatch()) {
        return true;
    }
    return false;
}

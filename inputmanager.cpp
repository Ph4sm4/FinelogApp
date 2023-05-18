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

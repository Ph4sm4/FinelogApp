#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <type_traits>
#include <QLineEdit>

class InputManager
{
public:
    InputManager();

    template<typename T, typename... Args>
    static typename std::enable_if<std::is_same<T, QLineEdit*>::value, bool>::type
    validateInputs(T arg, Args... args) {
        QString content = arg->text();
        bool isValid = true;

        if(!content.length()) {
            setErrorBorder(arg);
            isValid = false;
        }
        else {
            deleteErrorBorder(arg);
        }

        return validateInputs(args...) && isValid;
    }

    template<typename T, typename... Args>
    static typename std::enable_if<std::is_same<T, QLineEdit*>::value>::type
    clearInputs(T arg, Args... args) {
        arg->clear();
        deleteErrorBorder(arg);
        clearInputs(args...);
    }

    template<typename T, typename... Args>
    static typename std::enable_if<std::is_same<T, QLineEdit*>::value>::type
    clearInputsBlack(T arg, Args... args) {
        arg->clear();
        deleteErrorBorderBlack(arg);
        clearInputsBlack(args...);
    }

    static void disableButton(class QPushButton* button);
    static void enableButton(class QPushButton* button, const QString& stylesheet);

    static void setErrorBorder(QLineEdit* e);
    static void deleteErrorBorder(QLineEdit* e);
    static void deleteErrorBorderBlack(QLineEdit* e);
    static bool validatePassword(QString password);
    static bool validateEmail(QString email);

private:
    // Base case for clearInputs: no arguments left
    static void clearInputs() {}

    static void clearInputsBlack() {}

    // Base case for validateInputs: no arguments left, return true
    static bool validateInputs() { return true; }

};

#endif // INPUTMANAGER_H

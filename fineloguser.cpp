#include "fineloguser.h"
#include <QRegularExpression>
FinelogUser::FinelogUser(QString name, QString surname, QString email)
{
    this->name = name;
    this->surname = surname;
    this->email = email;
}
//    QRegularExpression mailREX("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\bi");
//    if(mailREX.match(email).hasMatch()) {

//        return true;
//    }
//    return false;


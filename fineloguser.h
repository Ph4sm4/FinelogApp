#ifndef FINELOGUSER_H
#define FINELOGUSER_H
#include <QString>

class FinelogUser
{
public:
    explicit FinelogUser(QString name, QString surname, QString email, QString phoneNumber) :
        name(name), surname(surname), email(email), phoneNumber(phoneNumber) {}

    explicit FinelogUser(QString name, QString surname, QString email) :
        name(name), surname(surname), email(email) {}

    explicit FinelogUser(QString name, QString surname) :
        name(name), surname(surname) {}

    explicit FinelogUser(QString name) :
        name(name) {}

    explicit FinelogUser() {}


    QString getName() { return name; } const;
    QString getSurname() { return surname; } const;
    QString getEmail() { return email; } const;


    void setName(QString newName) { name = newName; }
    void setEmail(QString newEmail) { email = newEmail; }
    void setPhoneNumber(QString newPhoneNumber) { phoneNumber = newPhoneNumber; }
    void setSurname(QString newSurname) { surname = newSurname; }


private:
    QString name;
    QString surname;
    QString email;
    QString phoneNumber;
};

#endif // FINELOGUSER_H

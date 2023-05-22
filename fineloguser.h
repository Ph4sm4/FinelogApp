#ifndef FINELOGUSER_H
#define FINELOGUSER_H
#include <QString>

class FinelogUser
{
public:
    explicit FinelogUser(QString name, QString surname, QString email, QString phoneNumber, QString password) :
        name(name), surname(surname), email(email), phoneNumber(phoneNumber), password(password) {}

    explicit FinelogUser(QString name, QString surname, QString email, QString phoneNumber) :
        name(name), surname(surname), email(email), phoneNumber(phoneNumber) {}

    explicit FinelogUser(QString name, QString surname, QString email) :
        name(name), surname(surname), email(email) {}

    explicit FinelogUser(QString name, QString surname) :
        name(name), surname(surname) {}

    explicit FinelogUser(QString name) :
        name(name) {}

    explicit FinelogUser() {}


    QString getName() const { return name; }
    QString getSurname() const { return surname; }
    QString getEmail() const { return email; }
    QString getPhoneNumber() const { return phoneNumber; }
    QString getPassword() const { return password; }
    QString getUserId() const { return userId; }
    QString getIdToken() const { return idToken; }

    void setName(const QString& newName) { name = newName; }
    void setEmail(const QString& newEmail) { email = newEmail; }
    void setPhoneNumber(const QString& newPhoneNumber) { phoneNumber = newPhoneNumber; }
    void setSurname(const QString& newSurname) { surname = newSurname; }
    void setPassword(const QString& newPassword) { password = newPassword; }
    void setIdToken(const QString& newIdToken) { idToken = newIdToken;}
    void setUserID(const QString& newUserId) { userId = newUserId; }

private:
    QString name;
    QString surname;
    QString email;
    QString phoneNumber;
    QString password;

    //database related
    QString idToken;
    QString userId;
};

#endif // FINELOGUSER_H

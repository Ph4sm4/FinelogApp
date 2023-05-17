#ifndef FINELOGUSER_H
#define FINELOGUSER_H
#include <QString>

class FinelogUser
{
public:
    explicit FinelogUser(QString name, QString surname, QString email);
    QString getName() { return name; } const;
    QString getSurname() { return surname; } const;
    QString getEmail() { return email; } const;

private:
    QString name;
    QString surname;
    QString email;
};

#endif // FINELOGUSER_H

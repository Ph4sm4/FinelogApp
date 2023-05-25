#ifndef FINELOGUSER_H
#define FINELOGUSER_H
#include "databasehandler.h"
#include <QString>
#include <QVector>
#include "reportheadline.h"
#include "userreport.h"

class FinelogUser
{
public:
    explicit FinelogUser();


    QString getName() const { return name; }
    QString getSurname() const { return surname; }
    QString getEmail() const { return email; }
    QString getPhoneNumber() const { return phoneNumber; }
    QString getPassword() const { return password; }
    QString getUserId() const { return userId; }
    QString getIdToken() const { return idToken; }
    QString getFinelogId() const { return finelogId; }
    QVector<ReportHeadline> getHeadlines() const { return headlines; }
    UserReport getReportContent(const QString& name) const;

    void setName(const QString& newName) { name = newName; }
    void setEmail(const QString& newEmail) { email = newEmail; }
    void setPhoneNumber(const QString& newPhoneNumber) { phoneNumber = newPhoneNumber; }
    void setSurname(const QString& newSurname) { surname = newSurname; }
    void setPassword(const QString& newPassword) { password = newPassword; }
    void setIdToken(const QString& newIdToken) {
        idToken = newIdToken;
        fetchHeadlines();
    }
    void setFinelogId(const QString& newFinelogId) { finelogId = newFinelogId; }
    void fetchHeadlines();
    void setUserID(const QString& newUserId) { userId = newUserId; }

private:
    QString name;
    QString surname;
    QString email;
    QString phoneNumber;
    QString password;
    QString finelogId;

    //database related
    QString idToken;
    QString userId;
    QVector<ReportHeadline> headlines;
    DatabaseHandler dbHandler;
};

#endif // FINELOGUSER_H

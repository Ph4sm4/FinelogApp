#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class DatabaseHandler : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseHandler(QObject* parent = nullptr);
    ~DatabaseHandler();
    QJsonObject performAuthenticatedGET(const QString& databasePath, const QString& userIdToken, const QString& queryParams = "");
    bool registerNewUser(class FinelogUser* user, class QLabel* errorLabel = nullptr);
    class FinelogUser* logInWithEmailAndPassword(const QString& email, const QString& password, class QLabel* errorLabel = nullptr);
    QJsonObject performPOST(const QString& url, const QJsonDocument& payload);
    QJsonObject performAuthenticatedPUT(const QString &databasePath, const QJsonDocument &payload, const QString& userIdToken);
    QJsonObject performAuthenticatedPOST(const QString &databasePath, const QJsonDocument& payload, const QString &userIdToken);
    bool sendEmailVerification(const QString& idToken);
    bool changeAuthDisplayName(const QString& idToken, const QString &newName);
    bool updateUserData(const QString& userId, const QJsonDocument& fieldsToUpdate, const QString& idToken);
    QJsonObject performAuthenticatedPATCH(const QString& databasePath, const QJsonDocument& payload, const QString& userIdToken);
    bool changeAuthUserEmail(const QString& idToken, const QString& newEmail);
    bool changeAuthUserPassword(const QString& idToken, const QString& newPassword);
    bool emailChangedExternallyUpdate(FinelogUser* user);
    QJsonObject sendPasswordResetLink(const QString& email);
    bool uploadProtocol(const class FinelogUser* user, const class UserReport& report);
    bool deleteUserAccount(const QString& idToken);
    bool adminDeleteUserAccount(const QString &userId, const QString &adminIdToken);
    bool deleteDatabaseEntry(const QString &databasePath, const QString &idToken);
    QJsonObject getAuthAccountInfo(const QString &idToken);
    QJsonObject getFormInputData(const QString &idToken);
    bool uploadFormInputData(const QString &idToken, const QJsonObject &data);

public slots:
    //void networkReplyReadyRead();

signals:

private:
    QNetworkAccessManager* networkManager;
    QNetworkReply* networkReply;
    const QString api_key = "AIzaSyA--DNxqDYjviNPHl7qrZ0GiQM9A5c2_Bg";
    const QString dbBaseUrl = "https://finelogapp-default-rtdb.europe-west1.firebasedatabase.app/";
    QJsonObject signUpWithEmailAndPassword(const QString email, const QString password);
};

#endif // DATABASEHANDLER_H

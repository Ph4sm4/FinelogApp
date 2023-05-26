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
    class FinelogUser* registerNewUser(class FinelogUser* user, class QLabel* errorLabel = nullptr);
    class FinelogUser* logInWithEmailAndPassword(const QString& email, const QString& password, class QLabel* errorLabel = nullptr);
    QJsonObject performPOST(const QString& url, const QJsonDocument& payload);
    QJsonObject performAuthenticatedPUT(const QString &databasePath, const QJsonDocument &payload, const QString& userIdToken);
    QJsonObject performAuthenticatedPOST(const QString &databasePath, const QJsonDocument& payload, const QString &userIdToken);
    bool sendEmailVerification(const QString& idToken);

public slots:
    //void networkReplyReadyRead();

signals:

private:
    QNetworkAccessManager* networkManager;
    QNetworkReply* networkReply;
    const QString api_key = "AIzaSyA--DNxqDYjviNPHl7qrZ0GiQM9A5c2_Bg";
    QJsonObject signUpWithEmailAndPassword(const QString email, const QString password);
    QJsonObject getAccountInfo(const QString& idToken);
};

#endif // DATABASEHANDLER_H

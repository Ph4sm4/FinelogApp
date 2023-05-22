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
    class QJsonObject performAuthenticatedGET(const QString& databasePath, const QString& userIdToken);
    class FinelogUser* registerNewUser(class FinelogUser* user, class QLabel* errorLabel = nullptr);
    class FinelogUser* logInWithEmailAndPassword(const QString& email, const QString& password);
    //void changeUserCredentials

public slots:
    //void networkReplyReadyRead();

signals:

private:
    QNetworkAccessManager* networkManager;
    QNetworkReply* networkReply;
    QJsonObject performPOST(const QString& url, const QJsonDocument& payload);
    QString api_key = "AIzaSyA--DNxqDYjviNPHl7qrZ0GiQM9A5c2_Bg";
    QJsonObject signUpWithEmailAndPassword(const QString email, const QString password);
};

#endif // DATABASEHANDLER_H

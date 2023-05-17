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
    class QJsonObject getReponseJson(QString databasePath);
    void addNewUser(class FinelogUser* user);
    //void changeUserCredentials
    void signUpWithEmailAndPassword(QString email, QString password);

public slots:
    //void networkReplyReadyRead();

signals:

private:
    QNetworkAccessManager* m_networkManager;
    QNetworkReply* m_networkReply;

    QString m_networkResponseString;
};

#endif // DATABASEHANDLER_H

#include "databasehandler.h"
#include <QNetworkRequest>
#include <QDebug>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>
#include "fineloguser.h"

DatabaseHandler::DatabaseHandler(QObject* parent) : QObject(parent)
{
    m_networkManager = new QNetworkAccessManager(this);

//    m_networkReply = m_networkManager->get(
//        QNetworkRequest(QUrl("https://finelogapp-default-rtdb.europe-west1.firebasedatabase.app/Pets.json")));
//connect(m_networkReply, &QNetworkReply::readyRead, this, &DatabaseHandler::networkReplyReadyRead);
}

DatabaseHandler::~DatabaseHandler()
{
    m_networkManager->deleteLater();
}

QJsonObject DatabaseHandler::getReponseJson(QString databasePath)
{
    QEventLoop loop;
    m_networkReply = m_networkManager->get(
        QNetworkRequest(QUrl("https://finelogapp-default-rtdb.europe-west1.firebasedatabase.app/" + databasePath + ".json")));
    connect(m_networkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString(m_networkReply->readAll()).toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

void DatabaseHandler::addNewUser(FinelogUser* user)
{
    QVariantMap newUser;
    newUser["Name"] = user->getName();
    newUser["Surname"] = user->getSurname();
    newUser["Email"] = user->getEmail();

    QJsonDocument jsonDoc = QJsonDocument::fromVariant(newUser);
    QNetworkRequest newUserReq(QUrl("https://finelogapp-default-rtdb.europe-west1.firebasedatabase.app/Pets.json"));

    newUserReq.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    m_networkManager->post(newUserReq, jsonDoc.toJson());
}

void DatabaseHandler::signUpWithEmailAndPassword(QString email, QString password)
{
//AIzaSyA--DNxqDYjviNPHl7qrZ0GiQM9A5c2_Bg
}


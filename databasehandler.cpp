#include "databasehandler.h"
#include <QNetworkRequest>
#include <QDebug>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>
#include "fineloguser.h"
#include <QLabel>

DatabaseHandler::DatabaseHandler(QObject* parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);

//    m_networkReply = m_networkManager->get(
//        QNetworkRequest(QUrl("https://finelogapp-default-rtdb.europe-west1.firebasedatabase.app/Pets.json")));
//connect(m_networkReply, &QNetworkReply::readyRead, this, &DatabaseHandler::networkReplyReadyRead);
}

DatabaseHandler::~DatabaseHandler()
{
    networkManager->deleteLater();
}

QJsonObject DatabaseHandler::performAuthenticatedGET(const QString& databasePath, const QString& userIdToken)
{
    QString endPoint = "https://finelogapp-default-rtdb.europe-west1.firebasedatabase.app/"
                       + databasePath + ".json?auth=" + userIdToken;
    QEventLoop loop;
    networkReply = networkManager->get(
        QNetworkRequest(QUrl()));
    connect(networkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString(networkReply->readAll()).toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

FinelogUser* DatabaseHandler::registerNewUser(FinelogUser* user, QLabel* errorLabel)
{
    //first we need to create authentication for the user
    QJsonObject newUserData = signUpWithEmailAndPassword(user->getEmail(), user->getPassword());

    const QString UId = newUserData.value("localId").toString();
    const QString idToken = newUserData.value("idToken").toString();

    qDebug() << "obtained auth id token: " << idToken;
    qDebug() << "obtained user id: " << UId;
    user->setIdToken(idToken);
    user->setUserID(UId);
    // check for errors later !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!




    // second we need to add the user into the database
    QVariantMap newUser;

    newUser["email"] = "tutaj email";
    newUser["Email"] =  user->getEmail();
    newUser["Name"] = user->getName();
    newUser["Phone number"] =user->getPhoneNumber();
    newUser["Surname"] = user->getSurname();
    newUser["user_id"] = user->getUserId();

    //!OnBackInvokedCallback: Set 'android:enableOnBackInvokedCallback="true"' in the application manifest.

    // this endpoint is going to create a document labeled as UserId (UId)
    // idToken is required to perform an authorized database request
    QString endPoint = "https://finelogapp-default-rtdb.europe-west1.firebasedatabase.app/Users/" + UId + ".json?auth=" + idToken;

    QJsonDocument jsonDoc = QJsonDocument::fromVariant(newUser);
    QJsonObject response = performPOST(endPoint, jsonDoc);
    qDebug() << response;

    return user;
}
//curl -X POST -d '{"user_id" : "jack", "text" : "Ahoy!"}' \
//'https://[PROJECT_ID].firebaseio.com/message_list.json'

FinelogUser* DatabaseHandler::logInWithEmailAndPassword(const QString &email, const QString &password)
{
    QString endPoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + api_key;
    QVariantMap variantPayload;
    variantPayload["email"] = email;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant(variantPayload);
    QJsonObject reply = performPOST(endPoint, jsonPayload);

    return new FinelogUser();
}

QJsonObject DatabaseHandler::signUpWithEmailAndPassword(const QString email, const QString password)
{
    QString endPoint = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + api_key;

    QVariantMap variantPayload;
    variantPayload["email"] = email;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant(variantPayload);
    QJsonObject reply = performPOST(endPoint, jsonPayload);

    return reply;
}

QJsonObject DatabaseHandler::performPOST(const QString &url, const QJsonDocument &payload)
{
    QNetworkRequest newReq((QUrl(url)));
    newReq.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QEventLoop loop;
    networkReply = networkManager->post(newReq, payload.toJson());
    connect(networkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString(networkReply->readAll()).toUtf8());
    QJsonObject jsonObject = jsonDocument.object();

    return jsonObject;
}


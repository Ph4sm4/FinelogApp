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
}

DatabaseHandler::~DatabaseHandler()
{
    networkManager->deleteLater();
}

QJsonObject DatabaseHandler::performAuthenticatedGET(const QString& databasePath, const QString& userIdToken, const QString& queryParams)
{
    QString endPoint = "https://finelogapp-default-rtdb.europe-west1.firebasedatabase.app/" + databasePath + ".json" + (queryParams.length()? "?" + queryParams : "") +"?auth=" + userIdToken;
    QEventLoop loop;
    networkReply = networkManager->get(
        QNetworkRequest(QUrl(endPoint)));
    connect(networkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qDebug() << "error auth get: " << networkReply->errorString();

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
    QJsonObject response = performPUT(endPoint, jsonDoc);
    qDebug() << response;

    return user;
}

FinelogUser* DatabaseHandler::logInWithEmailAndPassword(const QString &email, const QString &password, class QLabel* errorLabel)
{
    QString endPoint = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + api_key;
    QVariantMap variantPayload;
    variantPayload["email"] = email;
    variantPayload["password"] = password;
    variantPayload["returnSecureToken"] = true;

    QJsonDocument jsonPayload = QJsonDocument::fromVariant(variantPayload);
    QJsonObject reply = performPOST(endPoint, jsonPayload);
    qDebug() << "login reply: " << reply;

    if(reply.contains("error") && errorLabel) {
        errorLabel->setText("Invalid email or password");
        return nullptr;
    }

    QString uId = reply.value("localId").toString();
    QString idToken = reply.value("idToken").toString();

    qDebug() << "uid and token: " << uId << " " << idToken ;

    QString userEndPoint = "Users/" + uId;
    QJsonObject userData = performAuthenticatedGET(userEndPoint, idToken);
    qDebug() << "logged in user data: " << userData;

    FinelogUser* loggedInUser = new FinelogUser();
    loggedInUser->setEmail(userData.value("Email").toString());
    loggedInUser->setUserID(uId);
    loggedInUser->setIdToken(idToken);
    loggedInUser->setName(userData.value("Name").toString());
    loggedInUser->setSurname(userData.value("Surname").toString());
    loggedInUser->setPhoneNumber(userData.value("Phone number").toString());
    loggedInUser->setPassword(password);

    if(errorLabel)
        errorLabel->setText("");

    return loggedInUser;
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

    qDebug() << "error post: " << networkReply->errorString();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString(networkReply->readAll()).toUtf8());
    QJsonObject jsonObject = jsonDocument.object();

    return jsonObject;
}

QJsonObject DatabaseHandler::performPUT(const QString &url, const QJsonDocument &payload)
{
    QNetworkRequest newReq((QUrl(url)));
    newReq.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QEventLoop loop;
    networkReply = networkManager->put(newReq, payload.toJson());
    connect(networkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qDebug() << "error put: " << networkReply->errorString();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString(networkReply->readAll()).toUtf8());
    QJsonObject jsonObject = jsonDocument.object();

    return jsonObject;
}

QJsonObject DatabaseHandler::performAuthenticatedPOST(const QString &databasePath, const QJsonDocument& payload, const QString &userIdToken)
{
    QString endPoint = "https://finelogapp-default-rtdb.europe-west1.firebasedatabase.app/" + databasePath + ".json?auth=" + userIdToken;
    QEventLoop loop;
    QNetworkRequest newReq((QUrl(endPoint)));
    newReq.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));

    networkReply = networkManager->post(newReq, payload.toJson());
    connect(networkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qDebug() << "error auth post: " << networkReply->errorString();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString(networkReply->readAll()).toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

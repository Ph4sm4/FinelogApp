#include "databasehandler.h"
#include <QNetworkRequest>
#include <QDebug>
#include <QEventLoop>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariantMap>
#include "fineloguser.h"
#include <QLabel>
#include <QJsonArray>

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
    QString endPoint = "https://finelogapp-default-rtdb.europe-west1.firebasedatabase.app/" + databasePath + ".json" + (queryParams.length()? "?" + queryParams : "") + (queryParams.length()? "&" : "?") + "auth=" + userIdToken;
    QEventLoop loop;
    networkReply = networkManager->get(
        QNetworkRequest(QUrl(endPoint)));
    connect(networkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qDebug() << "error auth get: " << networkReply->error();;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString(networkReply->readAll()).toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

bool DatabaseHandler::registerNewUser(FinelogUser* user, QLabel* errorLabel)
{
    //first we need to create authentication for the user
    QJsonObject newUserData = signUpWithEmailAndPassword(user->getEmail(), user->getPassword());

    // check for errors here



    // check for errors later !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    const QString UId = newUserData.value("localId").toString();
    const QString idToken = newUserData.value("idToken").toString();

    qDebug() << "obtained auth id token: " << idToken;
    qDebug() << "obtained user id: " << UId;
    user->setIdToken(idToken);
    user->setUserID(UId);

    bool success = changeAuthDisplayName(idToken, user->getName() + " " + user->getSurname());
    if(!success) {
        errorLabel->setText("Unable to upload authentication data. Please try again");
        return false;
    }

    QJsonObject accountInfo = getAccountInfo(idToken);
    if(accountInfo.contains("error") && errorLabel) {
        errorLabel->setText("Could not get account info. Please try again");
        return false;
    }

    user->setEmailVerified(accountInfo.value("emailVerified").toBool());

    // "created at" manipulations since the response is given as a string of milliseconds
    QString timestamp = accountInfo.value("createdAt").toString();
    long long milliseconds = timestamp.toLongLong();
    QDateTime time;
    time.setMSecsSinceEpoch(milliseconds);
    QDate date = time.date();
    qDebug() << "timestamp created at: " << date;
    user->setAccountCreatedAt(date);

    // add the user into the database
    QVariantMap newUser;

    newUser["Email"] =  user->getEmail();
    newUser["Name"] = user->getName();
    newUser["Phone number"] =user->getPhoneNumber();
    newUser["Surname"] = user->getSurname();
    newUser["user_id"] = user->getUserId();
    newUser["finelog_id"] = user->getFinelogId();

    // this endpoint is going to create a document labeled as UserId (UId)
    // idToken is required to perform an authorized database request
    QString path = "Users/" + UId;

    QJsonDocument jsonDoc = QJsonDocument::fromVariant(newUser);
    QJsonObject response = performAuthenticatedPUT(path, jsonDoc, idToken);
    qDebug() << response;

    if(errorLabel)
        errorLabel->setText("");

    return response.contains("error") == false;
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

    if(reply.contains("error") && errorLabel) {
        errorLabel->setText("Invalid email or password");
        return nullptr;
    }

    QString uId = reply.value("localId").toString();
    QString idToken = reply.value("idToken").toString();

    qDebug() << "uid and token: " << uId << " " << idToken ;

    QString userEndPoint = "Users/" + uId;
    QJsonObject userData = performAuthenticatedGET(userEndPoint, idToken);
    //qDebug() << "logged in user data: " << userData;

    QJsonObject accountInfo = getAccountInfo(idToken);
    if(accountInfo.contains("error") && errorLabel) {
        errorLabel->setText("Could not get account info. Please try again");
        return nullptr;
    }

    FinelogUser* loggedInUser = new FinelogUser();
    loggedInUser->setEmail(userData.value("Email").toString());
    loggedInUser->setUserID(uId);
    loggedInUser->setIdToken(idToken);
    loggedInUser->setName(userData.value("Name").toString());
    loggedInUser->setSurname(userData.value("Surname").toString());
    loggedInUser->setPhoneNumber(userData.value("Phone number").toString());
    loggedInUser->setPassword(password);
    loggedInUser->setFinelogId(userData.value("finelog_id").toString());
    loggedInUser->setEmailVerified(accountInfo.value("emailVerified").toBool());

    // "created at" manipulations since the response is given as a string of milliseconds
    QString timestamp = accountInfo.value("createdAt").toString();
    long long milliseconds = timestamp.toLongLong();
    QDateTime time;
    time.setMSecsSinceEpoch(milliseconds);
    QDate date = time.date();
    qDebug() << "timestamp created at: " << date;
    loggedInUser->setAccountCreatedAt(date);

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

QJsonObject DatabaseHandler::getAccountInfo(const QString &idToken)
{
    QString endPoint = "https://identitytoolkit.googleapis.com/v1/accounts:lookup?key=" + api_key;
    QVariantMap payload;
    payload["idToken"] = idToken;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    QJsonObject accountInfo = performPOST(endPoint, doc);
    qDebug() << accountInfo;
    QJsonArray users = accountInfo.value("users").toArray();

    QJsonObject ourUser = users.at(0).toObject();
    qDebug() << "our user: " << ourUser;

    return ourUser;
}

QJsonObject DatabaseHandler::performPOST(const QString &url, const QJsonDocument &payload)
{
    QNetworkRequest newReq((QUrl(url)));
    newReq.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QEventLoop loop;
    networkReply = networkManager->post(newReq, payload.toJson());
    connect(networkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qDebug() << "status post: " << networkReply->error();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString(networkReply->readAll()).toUtf8());
    QJsonObject jsonObject = jsonDocument.object();

    return jsonObject;
}

QJsonObject DatabaseHandler::performAuthenticatedPUT(const QString &databasePath, const QJsonDocument &payload, const QString& userIdToken)
{
    QString endPoint = "https://finelogapp-default-rtdb.europe-west1.firebasedatabase.app/" + databasePath + ".json?auth=" + userIdToken;

    QNetworkRequest newReq((QUrl(endPoint)));
    newReq.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));
    QEventLoop loop;
    networkReply = networkManager->put(newReq, payload.toJson());
    connect(networkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qDebug() << "error auth put: " << networkReply->error();

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

    qDebug() << "error auth post: " << networkReply->error();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString(networkReply->readAll()).toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

bool DatabaseHandler::sendEmailVerification(const QString &idToken)
{
    QString endPoint = "https://identitytoolkit.googleapis.com/v1/accounts:sendOobCode?key=" + api_key;

    QVariantMap payload;
    payload["idToken"] = idToken;
    payload["requestType"] = "VERIFY_EMAIL";
    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    QJsonObject res = performPOST(endPoint, doc);

    return res.contains("error") == false;
}

bool DatabaseHandler::changeAuthDisplayName(const QString& idToken, const QString &newName)
{
    QString endPoint = "https://identitytoolkit.googleapis.com/v1/accounts:update?key=" + api_key;

    QVariantMap payload;
    payload["idToken"] = idToken;
    payload["displayName"] = newName;

    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    QJsonObject res = performPOST(endPoint, doc);

    return res.contains("error") == false;
}

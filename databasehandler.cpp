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
#include "stylesheetmanipulator.h"
#include "userreport.h"

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
    QString endPoint = dbBaseUrl + databasePath + ".json"
                       + (queryParams.length() ? "?" + queryParams : "")
                       + (queryParams.length() ? "&" : "?") + "auth=" + userIdToken;

    //qDebug() << "auth get endpoint: " << endPoint;

    QEventLoop loop;
    networkReply = networkManager->get(
        QNetworkRequest(QUrl(endPoint)));
    connect(networkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qDebug() << "error auth get: " << networkReply->error();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString(networkReply->readAll()).toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

bool DatabaseHandler::registerNewUser(FinelogUser* user, QLabel* errorLabel)
{
    //first we need to create authentication for the user
    QJsonObject newUserData = signUpWithEmailAndPassword(user->getEmail(), user->getPassword());

    // check for errors here

    if(newUserData.contains("error")) {
        QJsonObject errObj = newUserData.value("error").toObject();
        if(errObj.value("message") == "EMAIL_EXISTS") {
            if(errorLabel) {
                errorLabel->setText("The account with specified email already exists");
                errorLabel->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(errorLabel->styleSheet(),
                                                                              "QLabel", "color", "red"));
            }
        }
        else {
            if(errorLabel) errorLabel->setText("An error occurred");
        }
        return false;
    }

    const QString UId = newUserData.value("localId").toString();
    const QString idToken = newUserData.value("idToken").toString();

    qDebug() << "obtained auth id token: " << idToken;
    qDebug() << "obtained user id: " << UId;
    user->setIdToken(idToken);
    user->setUserID(UId);

    bool success = changeAuthDisplayName(idToken, user->getName() + " " + user->getSurname());
    if(!success) {
        errorLabel->setText("Unable to upload authentication data. Please try again");
        deleteUserAccount(idToken);
        return false;
    }

    QJsonObject accountInfo = getAuthAccountInfo(idToken);
    if(accountInfo.contains("error") && errorLabel) {
        errorLabel->setText("Could not get account info. Please try again");
        deleteUserAccount(idToken);
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

    newUser["email"] =  user->getEmail();
    newUser["name"] = user->getName();
    newUser["phone_number"] =user->getPhoneNumber();
    newUser["surname"] = user->getSurname();
    newUser["user_id"] = user->getUserId();
    newUser["isAdmin"] = false;
    newUser["finelog_id"] = user->getFinelogId();
    newUser["accountCreatedAt"] = date.toString();

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

    QJsonObject accountInfo = getAuthAccountInfo(idToken);
    if(accountInfo.contains("error") && errorLabel) {
        errorLabel->setText("Could not get account info. Please try again");
        return nullptr;
    }

    FinelogUser* loggedInUser = new FinelogUser();
    loggedInUser->setEmail(userData.value("email").toString());
    loggedInUser->setUserID(uId);
    loggedInUser->setIdToken(idToken);
    loggedInUser->setName(userData.value("name").toString());
    loggedInUser->setSurname(userData.value("surname").toString());
    loggedInUser->setPhoneNumber(userData.value("phone_number").toString());
    loggedInUser->setPassword(password);
    loggedInUser->setFinelogId(userData.value("finelog_id").toString());
    loggedInUser->setEmailVerified(accountInfo.value("emailVerified").toBool());
    loggedInUser->setIsAdmin(userData.value("isAdmin").toBool());

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

QJsonObject DatabaseHandler::getAuthAccountInfo(const QString &idToken)
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

QJsonObject DatabaseHandler::getFormInputData(const QString &idToken)
{
    QString endPoint = "Reports/Inputs";
    QJsonObject inputsData = performAuthenticatedGET(endPoint, idToken);

    return inputsData;
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
    QString endPoint = dbBaseUrl + databasePath + ".json?auth=" + userIdToken;

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
    QString endPoint = dbBaseUrl + databasePath + ".json?auth=" + userIdToken;
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

bool DatabaseHandler::updateUserData(const QString &userId, const QJsonDocument& fieldsToUpdate, const QString& idToken)
{
    QString path = "Users/" + userId;
    QJsonObject reply = performAuthenticatedPATCH(path, fieldsToUpdate, idToken);

    return reply.contains("error") == false;
}

QJsonObject DatabaseHandler::performAuthenticatedPATCH(const QString &databasePath, const QJsonDocument &payload, const QString& userIdToken)
{
    QString endPoint = dbBaseUrl + databasePath + ".json?auth=" + userIdToken;
    QEventLoop loop;
    QNetworkRequest newReq((QUrl(endPoint)));
    newReq.setHeader(QNetworkRequest::ContentTypeHeader, QString("application/json"));

    networkReply = networkManager->sendCustomRequest(newReq, "PATCH", payload.toJson());
    connect(networkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    qDebug() << "error auth patch: " << networkReply->error();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString(networkReply->readAll()).toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject;
}

bool DatabaseHandler::changeAuthUserEmail(const QString &idToken, const QString &newEmail)
{
    QString endPoint = "https://identitytoolkit.googleapis.com/v1/accounts:update?key=" + api_key;

    QVariantMap payload;
    payload["idToken"] = idToken;
    payload["email"] = newEmail;

    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    QJsonObject res = performPOST(endPoint, doc);

    return res.contains("error") == false;
}

bool DatabaseHandler::changeAuthUserPassword(const QString &idToken, const QString &newPassword)
{
    QString endPoint = "https://identitytoolkit.googleapis.com/v1/accounts:update?key=" + api_key;

    QVariantMap payload;
    payload["idToken"] = idToken;
    payload["password"] = newPassword;

    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    QJsonObject res = performPOST(endPoint, doc);

    return res.contains("error") == false;
}

// this functions handles updates for the database if there was a situation
// in which user's email has been changed via external links (through mail service)
bool DatabaseHandler::emailChangedExternallyUpdate(FinelogUser *user)
{
    QJsonObject authData = getAuthAccountInfo(user->getIdToken());
    const QString currentEmail = user->getEmail();
    const QString properEmail = authData.value("email").toString();

    //qWarning() << "current email: " << currentEmail;
    //qWarning() << "proper email: " << properEmail;

    if(currentEmail != properEmail) {
        QString path = "Users/" + user->getUserId();
        QVariantMap payload;
        payload["email"] = properEmail;

        user->setEmail(properEmail);
        QJsonDocument doc = QJsonDocument::fromVariant(payload);
        QJsonObject res = performAuthenticatedPATCH(path, doc, user->getIdToken());

        return res.contains("error") == false;
    }


    return true;
}

QJsonObject DatabaseHandler::sendPasswordResetLink(const QString &email)
{
    QString endPoint = "https://identitytoolkit.googleapis.com/v1/accounts:sendOobCode?key=" + api_key;
    QVariantMap payload;
    payload["requestType"] = "PASSWORD_RESET";
    payload["email"] = email;

    QJsonDocument doc = QJsonDocument::fromVariant(payload);
    QJsonObject res = performPOST(endPoint, doc);

    return res;
}

bool DatabaseHandler::uploadProtocol(const FinelogUser *user, const UserReport &report)
{
    // uploading report headline and report content data to db

    QVariantMap payload;
    payload["owner_id"] = user->getUserId();
    payload["owner_email"] = user->getEmail();
    payload["owner_phone"] = user->getPhoneNumber();
    payload["date"] = QDate::currentDate().toString();
    payload["time"] = QTime::currentTime().toString();

    payload["carName"] = report.carName;
    payload["projectName"] = report.projectName;
    payload["driver"] = report.driverName;
    payload["semiTrailer"] = report.semiTrailer;
    payload["odometerReading"] = report.odometerReading;
    payload["fuelInTank"] = report.fuelInTank;
    payload["tollCollectProper"] = report.isTollCollectProper;
    payload["tollCollectProperComment"] = report.tollCollectProperComment;
    payload["lightsCondition"] = report.lightsCondition;
    payload["brief"] = report.briefcaseDocsPresent;
    payload["carRegCertPresent"] = report.carRegCertPresent;
    payload["semiTrailerRegCertPresent"] = report.semiTrailerRegCertPresent;
    payload["carInsurancePresent"] = report.carInsurancePresent;
    payload["licensePresent"] = report.licensePresent;
    payload["licenseNumber"] = report.licenseNumber;
    payload["dkvCardNumber"] = report.dkvCardNumber;
    payload["hoyerCardNumber"] = report.hoyerCardNumber;
    payload["overallCabinState"] = report.overallCabinState;
    payload["fridgeClean"] = report.fridgeClean;
    payload["cabinDamages"] = report.cabinDamages;
    payload["cabinDamagesComment"] = report.cabinDamagesComment;
    payload["tiresCondition"] = report.tiresCondition;
    payload["tiresConditionComment"] = report.tiresConditionComment;
    payload["adrPlateCondition"] = report.adrPlateCondition;
    payload["vehicleClean"] = report.vehicleClean;
    payload["visibleOuterDamages"] = report.visibleOuterDamages;
    payload["ebTriangles"] = report.ebTriangles;
    payload["ebGloves"] = report.ebGloves;
    payload["ebColorfulAdrInstruction"] = report.ebColorfulAdrInstruction;
    payload["ebReflectiveVest"] = report.ebReflectiveVest;
    payload["ebProtectiveGoggles"] = report.ebProtectiveGoggles;
    payload["ebFunctionalFlashlight"] = report.ebFunctionalFlashlight;
    payload["ebBrushAndScoop"] = report.ebBrushAndScoop;
    payload["ebWasteBox"] = report.ebWasteBox;
    payload["ebRubberShoes"] = report.ebRubberShoes;
    payload["ebWheelWedge"] = report.ebWheelWedge;
    payload["ebManholeMat"] = report.ebManholeMat;
    payload["fireExtinguisher6kg"] = report.fireExtinguisher6kg;
    payload["expirationDateExting6kg1"] = report.expirationDateExting6kg1;
    payload["expirationDateExting6kg2"] = report.expirationDateExting6kg2;
    payload["fireExtinguisher2kg"] = report.fireExtinguisher2kg;
    payload["expirationDateExting2kg"] = report.expirationDateExting2kg;
    payload["medkit"] = report.medkit;
    payload["expirationDateMedkit"] = report.expirationDateMedkit;
    payload["maskAndFilters"] = report.maskAndFilters;
    payload["expirationDateMaskAndFilters"] = report.expirationDateMaskAndFilters;
    payload["eyewash"] = report.eyewash;
    payload["expirationDateEyewash"] = report.expirationDateEyewash;
    payload["comments"] = report.comments;

    // semi trailer here

    payload["trailerAdrCondition"] = report.trailerAdrCondition;
    payload["trailerClean"] = report.trailerClean;
    payload["trailerWheelWedge"] = report.trailerWheelWedge;
    payload["trailerTireCondition"] = report.trailerTireCondition;
    payload["trailerLightsCondition"] = report.trailerLightsCondition;
    payload["trailerLightsDamageComment"] = report.trailerLightsDamageComment;
    payload["trailerVisibleDamages"] = report.trailerVisibleDamages;
    payload["trailerWireCondition"] = report.trailerWireCondition;
    payload["trailerWireComments"] = report.trailerWireComments;
    payload["trailerBarNumber"] = report.trailerBarNumber;
    payload["trailerBeltNumber"] = report.trailerBeltNumber;
    payload["trailerPalletTruck"] = report.trailerPalletTruck;
    payload["trailerExpansionPole"] = report.trailerExpansionPole;
    payload["trailerCornerNumber"] = report.trailerCornerNumber;
    payload["trailerAntislipMap"] = report.trailerAntislipMap;
    payload["trailerLoadedElevator"] = report.trailerLoadedElevator;
    payload["trailerUdtElevatorUpToDate"] = report.trailerUdtElevatorUpToDate;
    payload["trailerLngCertificate"] = report.trailerLngCertificate;
    payload["eni"] = report.eni;
    payload["eniNumber"] = report.eniNumber;
    payload["shell"] = report.shell;
    payload["shellNumber"] = report.shellNumber;
    payload["barmalgas"] = report.barmalgas;
    payload["barmalgasNumber"] = report.barmalgasNumber;
    payload["liqvis"] = report.liqvis;
    payload["liqvisNumber"] = report.liqvisNumber;
    payload["liquind"] = report.liquind;
    payload["liquindNumber"] = report.liquindNumber;
    payload["e100"] = report.e100;
    payload["e100Number"] = report.e100Number;
    payload["aral"] = report.aral;
    payload["aralNumber"] = report.aralNumber;

    // other

    payload["setConnectedToTrailer"] = report.setConnectedToTrailer;
    payload["missingBoxContent"] = report.missingBoxContent;
    payload["missinBoxContentComment"] = report.missinBoxContentComment;
    payload["missingDocsInCabin"] = report.missingDocsInCabin;
    payload["missingDocsInCabinComment"] = report.missingDocsInCabinComment;


    QString path = "Reports/Content";
    QJsonObject res = performAuthenticatedPOST(path,
                                               QJsonDocument::fromVariant(payload), user->getIdToken());

    qDebug() << "added content: " << res;

    if(res.contains("error")) {
        return false;
    }

    QString contentName = res.value("name").toString();

    QVariantMap headlinePayload;
    headlinePayload["carName"] = report.carName;
    headlinePayload["projectName"] = report.projectName;
    headlinePayload["date"] = QDate::currentDate().toString();
    headlinePayload["time"] = QTime::currentTime().toString();
    headlinePayload["owner_id"] = user->getUserId();
    headlinePayload["contentName"] = contentName;

    path = "Reports/Headlines/" + contentName;
    res = performAuthenticatedPUT(path,
                                  QJsonDocument::fromVariant(headlinePayload),
                                  user->getIdToken());

    // if there was an error during headline upload, then we must delete the content entry
    if (res.contains("error")) {
        QString contentPath = "Reports/Content/" + contentName;
        deleteDatabaseEntry(contentPath, user->getIdToken());
        return false;
    }
    qDebug() << "headline put res: " << res;

    // now we want to add this particular report (e.g. Content name) to the Unread collection in the database

    path = "Admin/Unread/" + contentName;
    QVariantMap unreadPayload;
    unreadPayload["owner_id"] = user->getUserId();

    res = performAuthenticatedPUT(path,
                                  QJsonDocument::fromVariant(unreadPayload),
                                  user->getIdToken());

    qDebug() << "unread put: " << res;

    // if during this step, an error occurred - we must delete already created entries (content + headline)
    if (res.contains("error")) {
        QString contentPath = "Reports/Content/" + contentName;
        QString headlinePath = "Reports/Headlines/" + contentName;

        deleteDatabaseEntry(contentPath, user->getIdToken());
        deleteDatabaseEntry(headlinePath, user->getIdToken());
        return false;
    }

    return res.contains("error") == false;
}

bool DatabaseHandler::deleteUserAccount(const QString &idToken)
{
    QString endPoint = "https://identitytoolkit.googleapis.com/v1/accounts:delete?key=" + api_key;

    QVariantMap payload;
    payload["idToken"] = idToken;
    QJsonDocument doc = QJsonDocument::fromVariant(payload);

    QJsonObject res = performPOST(endPoint, doc);

    return res.contains("error") == false;
}

bool DatabaseHandler::adminDeleteUserAccount(const QString &userId, const QString &adminIdToken)
{
    // we gotta use ngrok for it, right now only tho
    const QString deleteEndpoint = "https://db02-87-206-216-159.ngrok.io/adminDeleteUser";

    QVariantMap authPayload;
    authPayload["localId"] = userId;

    QJsonObject res = performPOST(deleteEndpoint, QJsonDocument::fromVariant(authPayload));

    const QString deletePath = "Users/" + userId;
    return deleteDatabaseEntry(deletePath, adminIdToken);
}

bool DatabaseHandler::deleteDatabaseEntry(const QString &databasePath, const QString &idToken)
{
    QString endPoint = dbBaseUrl + databasePath + ".json?auth=" + idToken;

    QEventLoop loop;
    QNetworkRequest req((QUrl(endPoint)));

    networkReply = networkManager->deleteResource(req);
    connect(networkReply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(QString(networkReply->readAll()).toUtf8());
    QJsonObject jsonObject = jsonDocument.object();
    return jsonObject.contains("error") == false;
}

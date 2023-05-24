#include "fineloguser.h"
#include <QRegularExpression>
#include "userreport.h"
#include "databasehandler.h"
#include <QJsonObject>
#include <QDebug>

FinelogUser::FinelogUser() {

}

void FinelogUser::fetchReports()
{
    const QString endPoint = "Reports";
    const QString queryParams = "orderBy=\"owner_id\"&equalTo=\"" + userId +"\"";
    QJsonObject reportsData = dbHandler.performAuthenticatedGET(
        endPoint, idToken, queryParams);

    qDebug() << "reportdata: " <<  reportsData;
    // we have the report data, now we got to fill the report vector with them
}

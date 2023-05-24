#include "fineloguser.h"
#include <QRegularExpression>
#include "userreport.h"
#include "databasehandler.h"
#include <QJsonObject>
#include <QDebug>

FinelogUser::FinelogUser() {

}

void FinelogUser::getDBReportsData()
{
    const QString endPoint = "Reports/report1";
    QJsonObject reportsData = dbHandler.performAuthenticatedGET(endPoint, idToken);

    qDebug() << "reportdata: " <<  reportsData;
}

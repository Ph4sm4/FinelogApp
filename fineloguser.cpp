#include "fineloguser.h"
#include <QRegularExpression>
#include "userreport.h"
#include "databasehandler.h"
#include <QJsonObject>
#include <QDebug>

FinelogUser::FinelogUser() {

}

void FinelogUser::fetchHeadlines()
{
    const QString endPoint = "Reports/Headlines";
    const QString queryParams = "orderBy=\"owner_id\"&equalTo=\"" + userId +"\"";
    QJsonObject headlinesData = dbHandler.performAuthenticatedGET(
        endPoint, idToken, queryParams);

    //qDebug() << "headline data: " <<  headlinesData;

    for (QJsonObject::iterator it = headlinesData.begin(); it != headlinesData.end(); ++it) {
        QJsonObject val = it.value().toObject();

        ReportHeadline headline;
        headline.projectName = val.value("projectName").toString();
        headline.carName = val.value("carName").toString();
        headline.uploadTime = QTime::fromString(val.value("time").toString());
        headline.uploadDate = QDate::fromString(val.value("date").toString());
        headline.contentName = val.value("contentName").toString();

        if(!headlines.contains(headline))
            headlines.append(headline);
    }
    qDebug() << "headlines size: " << headlines.size();
}

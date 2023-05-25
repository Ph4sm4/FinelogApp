#ifndef USERREPORT_H
#define USERREPORT_H

#include <QString>
#include <QDate>
#include <QTime>

class UserReport
{
public:
    UserReport();

    QString ownerId;
    QString carName;
    QDate uploadDate;
    QTime uploadTime;
    QString projectName;
    QString windowCondition;

};

#endif // USERREPORT_H

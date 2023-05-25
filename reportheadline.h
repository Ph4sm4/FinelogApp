#ifndef REPORTHEADLINE_H
#define REPORTHEADLINE_H

#include <QString>
#include <QDate>
#include <QTime>

class ReportHeadline
{
public:
    ReportHeadline();

    bool operator==(const ReportHeadline& other) const {
        return contentName == other.contentName;
    }

    QString carName;
    QString projectName;
    QString owner_id;
    QDate uploadDate;
    QTime uploadTime;
    QString contentName; // name of the file with the report's content
};

#endif // REPORTHEADLINE_H

#ifndef LISTITEM_H
#define LISTITEM_H

#include <QDate>
#include <QGesture>
#include <QGestureEvent>
#include <QTapGesture>
#include <QTime>
#include <QTouchEvent>
#include <QWidget>
#include "databasehandler.h"

namespace Ui {
class ListItem;
}

class ListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ListItem(QWidget *parent = nullptr);
    ~ListItem();

    void setDate(const QDate& dateOfUpload);
    void setTime(const QTime& timeOfUpload);
    void setProjectName(const QString& newName);
    void setCarName(const QString& newCarName);
    void setContentName(const QString& newContentName) { contentName = newContentName; }
    void setAdminIdToken(const QString &token) { adminIdToken = token; }

    bool operator==(const ListItem &other) { return contentName == other.contentName; }

    void setHasBeenRead(bool val) { hasBeenRead = val; }

    bool getHasBeenRead() const { return hasBeenRead; }
    QDateTime getDateTime() const { return QDateTime(protocolDate, protocolTime); }
    QString getCarName() const { return carName; }

    void setOwner(class FinelogUser *user) { owner = user; }

signals:
    void clicked(const QString& contentName);

protected:
    bool event(QEvent* event) override;
    bool gestureEvent(QGestureEvent* event);
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::ListItem *ui;

    QDate protocolDate;
    QTime protocolTime;
    QString projectName;
    QString carName;
    QString contentName; // required to properly display newly created items

    bool hasBeenRead = false;
    QString adminIdToken;

    DatabaseHandler dbHandler;

    class FinelogUser *owner
        = nullptr; // used in the admin panel, in order to delete contentName from unreadProtocols list for that user
};

#endif // LISTITEM_H

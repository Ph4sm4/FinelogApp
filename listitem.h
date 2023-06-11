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

    bool operator==(const ListItem &other) { return contentName == other.contentName; }

    void setUnreadProtocolsForUser(QVector<QString> *unread) { unreadProtocols = unread; }
    void setHasBeenRead(bool val) { hasBeenRead = val; }

signals:
    void clicked(const QString& contentName);

protected:
    bool event(QEvent* event) override;
    bool gestureEvent(QGestureEvent* event);
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            emit clicked(this->contentName);
        }
        QWidget::mousePressEvent(event); // Call the base class implementation
    }

private:
    Ui::ListItem *ui;

    QDate protocolDate;
    QTime protocolTime;
    QString projectName;
    QString carName;
    QString contentName; // required to properly display newly created items

    QVector<QString> *unreadProtocols;
    bool hasBeenRead = false;

    DatabaseHandler dbHandler;
};

#endif // LISTITEM_H

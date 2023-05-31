#ifndef LISTITEM_H
#define LISTITEM_H

#include <QWidget>
#include <QTime>
#include <QDate>
#include <QTouchEvent>
#include <QGestureEvent>
#include <QGesture>
#include <QTapGesture>

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

    bool operator==(const ListItem& other) {
        return contentName == other.contentName;
    }

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
};

#endif // LISTITEM_H

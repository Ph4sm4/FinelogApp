#ifndef USERITEM_H
#define USERITEM_H

#include <QGesture>
#include <QGestureEvent>
#include <QTapGesture>
#include <QTouchEvent>
#include <QWidget>

namespace Ui {
class UserItem;
}

class UserItem : public QWidget
{
    Q_OBJECT

public:
    explicit UserItem(QWidget *parent = nullptr);
    ~UserItem();

    //    bool operator==(const ListItem& other) {
    //        return contentName == other.contentName;
    //    }
    void setNewUpload(bool upload);
    void setFullName(const QString &name, const QString &surname);
    void setEmail(const QString &newEmail);
    void setNumberOfUploads(const int &uploadNumber);
    void setUserId(const QString &id) { userId = id; }

protected:
    bool event(QEvent *event) override;
    bool gestureEvent(QGestureEvent *event);
    void mousePressEvent(QMouseEvent *event) override
    {
        //        if (event->button() == Qt::LeftButton) {
        //            emit clicked(this->contentName);
        //        }
        QWidget::mousePressEvent(event); // Call the base class implementation
    }

private:
    Ui::UserItem *ui;

    bool newUpload;
    QString fullName;
    QString email;
    int numberOfUploads;

    QString userId;
};

#endif // USERITEM_H

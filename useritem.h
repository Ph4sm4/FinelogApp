#ifndef USERITEM_H
#define USERITEM_H

#include <QGesture>
#include <QGestureEvent>
#include <QTapGesture>
#include <QTouchEvent>
#include <QWidget>
#include "fineloguser.h"

namespace Ui {
class UserItem;
}

class UserItem : public QWidget
{
    Q_OBJECT

public:
    explicit UserItem(QWidget *parent = nullptr);
    ~UserItem();

    void setNewUpload(bool upload);
    void setFullName(const QString &name, const QString &surname);
    void setEmail(const QString &newEmail);
    void setNumberOfUploads(const int &uploadNumber);
    void setUserId(const QString &id) { user->setUserID(id); }
    void setAccountCreatedAt(const QDate &date) { user->setAccountCreatedAt(date); }
    void setPhoneNumber(const QString &number) { user->setPhoneNumber(number); }
    void setFinelogId(const QString &id) { user->setFinelogId(id); }
    void setEmailVerified(const bool verified) { user->setEmailVerified(verified); }
    void setIdToken(const QString &token) { user->setIdToken(token); }
    void setUnreadProtocolsForUser(const QVector<QString> &protocols)
    {
        user->setUnreadProtocols(protocols);
    }

protected:
    bool event(QEvent *event) override;
    bool gestureEvent(QGestureEvent *event);
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked(FinelogUser *clickedUser);

private:
    Ui::UserItem *ui;

    bool newUpload;
    int numberOfUploads;

    FinelogUser *user;
};

#endif // USERITEM_H

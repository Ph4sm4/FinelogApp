#ifndef USERDELETEITEM_H
#define USERDELETEITEM_H

#include <QWidget>

namespace Ui {
class UserDeleteItem;
}

class UserDeleteItem : public QWidget
{
    Q_OBJECT

public:
    explicit UserDeleteItem(QWidget *parent = nullptr);
    ~UserDeleteItem();

    void setFullName(const QString &name);
    void setEmail(const QString &e);

    QString getFullName() const { return fullName; }
    QString getEmail() const { return email; }

    QString userId;

private slots:
    void on_deleteButton_clicked();

signals:
    void deleteClicked(QString id);

private:
    Ui::UserDeleteItem *ui;

    QString fullName;
    QString email;
};

#endif // USERDELETEITEM_H

#ifndef LISTITEM_H
#define LISTITEM_H

#include <QWidget>
#include <QTime>
#include <QDate>

namespace Ui {
class ListItem;
}

class ListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ListItem(QWidget *parent = nullptr);
    ~ListItem();



private:
    Ui::ListItem *ui;

    QDate protocolDate;
    QTime protocolTime;
    QString projectName;
    QString car_name;
};

#endif // LISTITEM_H

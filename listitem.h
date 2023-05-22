#ifndef LISTITEM_H
#define LISTITEM_H

#include <QWidget>

namespace Ui {
class ListItem;
}

class ListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ListItem(QWidget *parent = nullptr);
    ~ListItem();

    explicit ListItem(QDate date, QTime time, QString project, QString car_name);

private:
    Ui::ListItem *ui;
};

#endif // LISTITEM_H

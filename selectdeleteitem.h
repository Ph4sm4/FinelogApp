#ifndef SELECTDELETEITEM_H
#define SELECTDELETEITEM_H

#include <QWidget>

namespace Ui {
class SelectDeleteItem;
}

class SelectDeleteItem : public QWidget
{
    Q_OBJECT

public:
    explicit SelectDeleteItem(QWidget *parent = nullptr);
    ~SelectDeleteItem();

private:
    Ui::SelectDeleteItem *ui;
};

#endif // SELECTDELETEITEM_H

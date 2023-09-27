#ifndef SELECTDELETEITEM_H
#define SELECTDELETEITEM_H

#include <QJsonValue>
#include <QWidget>

namespace Ui {
class SelectDeleteItem;
}

class SelectDeleteItem : public QWidget
{
    Q_OBJECT

public:
    explicit SelectDeleteItem(QWidget *parent = nullptr);
    explicit SelectDeleteItem(QJsonArray *val, int index);
    ~SelectDeleteItem();

private slots:
    void on_passwordEdit_textChanged(const QString &arg1);

private:
    Ui::SelectDeleteItem *ui;

    int itemIndex;
    QJsonArray *controlledValue = nullptr;
};

#endif // SELECTDELETEITEM_H

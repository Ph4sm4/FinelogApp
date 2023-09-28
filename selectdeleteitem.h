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
    explicit SelectDeleteItem(QJsonObject *val, const QString &key, int index);
    ~SelectDeleteItem();

private slots:
    void on_fieldEdit_textChanged(const QString &arg1);

    void on_deleteButton_clicked();

private:
    Ui::SelectDeleteItem *ui;

    int itemIndex;
    QJsonObject *controlledValue = nullptr;
    QString selectKey;
};

#endif // SELECTDELETEITEM_H

#ifndef SELECTCONTENTMANAGER_H
#define SELECTCONTENTMANAGER_H

#include <QJsonArray>
#include <QWidget>

namespace Ui {
class SelectContentManager;
}

class SelectContentManager : public QWidget
{
    Q_OBJECT

public:
    explicit SelectContentManager(QWidget *parent = nullptr);
    ~SelectContentManager();

    void setTitle(const QString &newTitle);
    void setItems(QJsonArray *newFields);

private:
    Ui::SelectContentManager *ui;

    QJsonArray *fieldsArray = nullptr;
};

#endif // SELECTCONTENTMANAGER_H

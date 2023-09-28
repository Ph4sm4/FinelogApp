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
    void setItems(QJsonObject *newFields);

public slots:

private slots:
    void on_addField_clicked();

private:
    Ui::SelectContentManager *ui;

    QJsonObject *dataObject = nullptr;
    QString key;
    class QVBoxLayout *existingLayout = nullptr;
};

#endif // SELECTCONTENTMANAGER_H

#ifndef SELECTCONTENTMANAGER_H
#define SELECTCONTENTMANAGER_H

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

private:
    Ui::SelectContentManager *ui;
};

#endif // SELECTCONTENTMANAGER_H

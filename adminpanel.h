#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QWidget>
#include "databasehandler.h"

namespace Ui {
class AdminPanel;
}

class AdminPanel : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPanel(QWidget *parent = nullptr);
    ~AdminPanel();

    void setUser(class FinelogUser *user)
    {
        adminUser = user;
        initializeDashboard();
    }

private:
    Ui::AdminPanel *ui;

    class FinelogUser *adminUser;
    DatabaseHandler dbHandler;
    void initializeDashboard();
};

#endif // ADMINPANEL_H

#ifndef USERPANEL_H
#define USERPANEL_H

#include "databasehandler.h"
#include <QWidget>
#include "fineloguser.h"

namespace Ui {
class UserPanel;
}

class UserPanel : public QWidget
{
    Q_OBJECT

public:
    explicit UserPanel(QWidget *parent = nullptr);
    ~UserPanel();

    void setCurrentUser(FinelogUser* newUser) { currentUser = newUser; }

private slots:

    void on_logoutButton_clicked();

signals:
    void logOutButtonClicked();

private:
    Ui::UserPanel *ui;

    DatabaseHandler dbHandler;
    FinelogUser* currentUser;
};

#endif // USERPANEL_H

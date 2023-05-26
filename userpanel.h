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

    void setCurrentUser(FinelogUser* newUser) {
        currentUser = newUser;
        setUserDisplayInfo();
    }

    void clearUser();
    FinelogUser* getCurrentUser() const { return currentUser; }

private slots:
    void on_settingsButton_clicked();

    void on_newProtocolButton_clicked();

signals:
    void settingsButtonClicked();

    void logOutButtonClicked();

private:
    Ui::UserPanel *ui;

    void setUserDisplayInfo();
    DatabaseHandler dbHandler;
    FinelogUser* currentUser;
};

#endif // USERPANEL_H

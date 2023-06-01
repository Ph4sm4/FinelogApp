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

    void setSettingsPanel(class SettingsPanel* newPanel);

public slots:
    void userBasicDetailsChange();

    void formReadyForDeletion();

    void projectDetailsRequested(const QString& contentName);

private slots:
    void on_settingsButton_clicked();

    void on_newProtocolButton_clicked();

    void on_backToDashboard_clicked();


signals:
    void settingsButtonClicked();

    void logOutButtonClicked();

    void successBoxDisplayNeeded();

private:
    Ui::UserPanel *ui;

    void setUserDisplayInfo();
    DatabaseHandler dbHandler;
    FinelogUser* currentUser;
    QVector<ReportHeadline> reports;
    class SettingsPanel* settingsPanel = nullptr;
    class ProtocolForm* form = nullptr;

    void formReadyForDeletionNoSuccessBox();

    void clearForm();
};

#endif // USERPANEL_H

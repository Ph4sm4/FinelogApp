#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QWidget>
#include "databasehandler.h"
#include "qjsonobject.h"
#include "qmainwindow.h"

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

    void setMainWindow(QMainWindow *w) { window = w; }
    QMainWindow *getMainWindow() const { return window; }

    void clearUser();

public slots:
    void initializeUserPreview(class FinelogUser *user);

    void formReadyForDeletion();

    void projectDetailsRequested(const QString &contentName);

    void userDeleteTriggered(QString userId);

    void deleteUserAccount(const QString &userId);

private slots:
    void on_backToPanel_clicked();

    void on_backToPreview_clicked();

    void on_logOutButton_clicked();

    void on_sortHeadlinesCombo_currentTextChanged(const QString &arg1);

    void on_backToPanel_2_clicked();

    void on_deleteAccountButton_clicked();

signals:
    void logOutButtonClicked();

private:
    Ui::AdminPanel *ui;

    class FinelogUser *adminUser = nullptr;
    class FinelogUser *previewUser = nullptr;
    class ProtocolForm *form = nullptr;
    QMainWindow *window = nullptr;

    DatabaseHandler dbHandler;
    void initializeDashboard();
    void initializeUserDeletion();

    QJsonObject usersObject;
};

#endif // ADMINPANEL_H

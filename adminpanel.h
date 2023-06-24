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

    void clearUser();

public slots:
    void initializeUserPreview(class FinelogUser *user);

    void formReadyForDeletion();

    void projectDetailsRequested(const QString &contentName);

private slots:
    void on_backToPanel_clicked();

    void on_backToPreview_clicked();

    void on_logOutButton_clicked();

    void on_sortHeadlinesCombo_currentTextChanged(const QString &arg1);

signals:
    void logOutButtonClicked();

private:
    Ui::AdminPanel *ui;

    class FinelogUser *adminUser = nullptr;
    class FinelogUser *previewUser = nullptr;
    class ProtocolForm *form = nullptr;

    DatabaseHandler dbHandler;
    void initializeDashboard();
};

#endif // ADMINPANEL_H

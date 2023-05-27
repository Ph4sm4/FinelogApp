#ifndef SETTINGSPANEL_H
#define SETTINGSPANEL_H

#include <QWidget>
#include "databasehandler.h"

namespace Ui {
class SettingsPanel;
}

class SettingsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPanel(QWidget *parent = nullptr);
    ~SettingsPanel();

    void performAnimation(const int& duration, const QPoint& endPoint, QObject* parent);
    void setOverlayWidget(class OverlayWidget* newOverlay) { overlay = newOverlay; }
    void setCurrentUser(class FinelogUser* user);

signals:
    void logOutButtonClicked();

private slots:
    void on_logoutButton_clicked();

    void on_sendVerifyEmail_clicked();

    void on_closePanel_clicked();

    void on_nameSurnameButton_clicked();

    void on_emailButton_clicked();

    void on_phoneButton_clicked();

    void on_passwordButton_clicked();

    void on_backEmail_clicked();

    void on_backName_clicked();

    void on_backPassword_clicked();

    void on_backPhone_clicked();

private:
    Ui::SettingsPanel *ui;
    class OverlayWidget* overlay = nullptr;
    class FinelogUser* currentUser = nullptr;
    DatabaseHandler dbHandler;
};

#endif // SETTINGSPANEL_H

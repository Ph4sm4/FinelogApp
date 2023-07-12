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
    explicit SettingsPanel(QWidget *parent);
    ~SettingsPanel();

    void performAnimation(const int& duration, const QPoint& endPoint, QObject* parent);
    void setCurrentUser(class FinelogUser *user);

    void comeIntoView();

signals:
    void logOutButtonClicked();

    void userDetailsChange();

    void settingsPanelClosed();

public slots:
    void exitFromView();

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

    void on_saveName_clicked();

    void on_saveEmail_clicked();

    void on_savePhone_clicked();

    void on_savePassword_clicked();

    void on_passwordEdit_textChanged(const QString &arg1);

    void on_confirmPassword_textChanged(const QString &arg1);

    void on_emailEdit_textChanged(const QString &arg1);

    void on_confirmEmailCheckBox_stateChanged(int arg1);

    void on_nameEdit_textChanged(const QString &arg1);

    void on_surnameEdit_textChanged(const QString &arg1);

    void on_phoneEdit_textChanged(const QString &arg1);

private:
    Ui::SettingsPanel *ui;
    class FinelogUser* currentUser = nullptr;
    DatabaseHandler dbHandler;
};

#endif // SETTINGSPANEL_H

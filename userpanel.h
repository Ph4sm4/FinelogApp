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

private slots:
    void on_settingsButton_clicked();

    void on_newProtocolButton_clicked();

    void on_backToDashboard_clicked();

    void on_sendForm_clicked();

    void on_formSendConfirmCheck_stateChanged(int arg1);

    void on_radioButton_26_toggled(bool checked);

    void on_radioButton_23_toggled(bool checked);

    void on_radioButton_29_toggled(bool checked);

    void on_radioButton_4_toggled(bool checked);

    void on_radioButton_63_toggled(bool checked);

    void on_radioButton_67_toggled(bool checked);

    void on_radioButton_69_toggled(bool checked);

    void on_radioButton_71_toggled(bool checked);

    void on_radioButton_73_toggled(bool checked);

    void on_radioButton_84_toggled(bool checked);

    void on_radioButton_88_toggled(bool checked);

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

    bool isPositiveChoiseSelected(class QGroupBox* box);
    void setFormEditState(bool checked, class QLineEdit* edit);
    void setFormDateEditState(bool checked, class QDateEdit* edit);
};

#endif // USERPANEL_H

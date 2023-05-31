#ifndef PROTOCOLFORM_H
#define PROTOCOLFORM_H

#include <QWidget>
#include "databasehandler.h"

namespace Ui {
class ProtocolForm;
}

class ProtocolForm : public QWidget
{
    Q_OBJECT

public:
    explicit ProtocolForm(QWidget *parent = nullptr);
    ~ProtocolForm();

    void prepareForm();
    void setCurrentUser(class FinelogUser* user) { currentUser = user; }
    void hideSendOptions();
    bool initializeFormData(const QString& contentName);

signals:
    void formSubmitted();

private slots:
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

    void on_radioButton_97_toggled(bool checked);

    void on_radioButton_99_toggled(bool checked);

    void on_radioButton_101_toggled(bool checked);

    void on_radioButton_103_toggled(bool checked);

    void on_radioButton_105_toggled(bool checked);

    void on_radioButton_107_toggled(bool checked);

    void on_radioButton_109_toggled(bool checked);

    void on_radioButton_113_toggled(bool checked);

    void on_radioButton_115_toggled(bool checked);

    void on_sendForm_clicked();

    void on_formSendConfirmCheck_stateChanged(int arg1);

private:
    Ui::ProtocolForm *ui;

    class FinelogUser* currentUser;

    bool isPositiveChoiseSelected(class QGroupBox* box);
    void setFormEditState(bool checked, class QLineEdit* edit);
    void setFormDateEditState(bool checked, class QDateEdit* edit);

    DatabaseHandler dbHandler;
};

#endif // PROTOCOLFORM_H

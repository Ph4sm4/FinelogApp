#ifndef CONFIRMATIONMODAL_H
#define CONFIRMATIONMODAL_H

#include <QWidget>

namespace Ui {
class ConfirmationModal;
}

class ConfirmationModal : public QWidget
{
    Q_OBJECT

public:
    explicit ConfirmationModal(QWidget *parent);
    ~ConfirmationModal();

signals:
    void cancelAction();

    void acceptAction();

private slots:
    void on_cancelButton_clicked();

    void on_confirmButton_clicked();

private:
    Ui::ConfirmationModal *ui;
};

#endif // CONFIRMATIONMODAL_H

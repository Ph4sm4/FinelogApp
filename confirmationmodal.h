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

    void setDeleteUserId(const QString &id) { userId = id; }

public slots:
    void exitFromView();

signals:
    void cancelAction();

    void acceptAction(const QString &id);

private slots:
    void on_cancelButton_clicked();

    void on_confirmButton_clicked();

private:
    Ui::ConfirmationModal *ui;

    QString userId;
};

#endif // CONFIRMATIONMODAL_H

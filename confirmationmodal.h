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
    explicit ConfirmationModal(QWidget *parent = nullptr);
    ~ConfirmationModal();

private:
    Ui::ConfirmationModal *ui;
};

#endif // CONFIRMATIONMODAL_H

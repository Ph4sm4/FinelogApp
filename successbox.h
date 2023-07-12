#ifndef SUCCESSBOX_H
#define SUCCESSBOX_H

#include <QWidget>

namespace Ui {
class SuccessBox;
}

class SuccessBox : public QWidget
{
    Q_OBJECT

public:
    explicit SuccessBox(QWidget *parent);
    ~SuccessBox();

    void comeIntoView();

    void setMessage(const QString &message);

private:
    Ui::SuccessBox *ui;

    class QTimer *timer;
};

#endif // SUCCESSBOX_H

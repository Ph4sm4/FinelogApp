#ifndef USERPANEL_H
#define USERPANEL_H

#include <QWidget>

namespace Ui {
class UserPanel;
}

class UserPanel : public QWidget
{
    Q_OBJECT

public:
    explicit UserPanel(QWidget *parent = nullptr);
    ~UserPanel();

private:
    Ui::UserPanel *ui;
};

#endif // USERPANEL_H

#ifndef SETTINGSPANEL_H
#define SETTINGSPANEL_H

#include <QWidget>

namespace Ui {
class SettingsPanel;
}

class SettingsPanel : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPanel(QWidget *parent = nullptr);
    ~SettingsPanel();

private:
    Ui::SettingsPanel *ui;
};

#endif // SETTINGSPANEL_H

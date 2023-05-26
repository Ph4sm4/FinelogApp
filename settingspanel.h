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

    void performAnimation(const int& duration, const QPoint& endPoint, QObject* parent);
    void setOverlayWidget(class OverlayWidget* newOverlay) { overlay = newOverlay; }

signals:
    void logOutButtonClicked();

private slots:
    void on_logoutButton_clicked();

private:
    Ui::SettingsPanel *ui;
    class OverlayWidget* overlay = nullptr;
};

#endif // SETTINGSPANEL_H

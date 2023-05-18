#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "databasehandler.h"
#include <QLineEdit>
#include <QJsonObject>
#include "inputmanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginButton_clicked();

    void on_loginGoTo_clicked();

    void on_registerGoTo_clicked();

    void on_idNext_clicked();

    void on_nextName_clicked();

    void on_nextEmail_clicked();

    void on_loginGoTo_3_clicked();

    void on_loginGoTo_4_clicked();

    void on_loginGoTo_5_clicked();

    void on_registerButton_clicked();

private:
    Ui::MainWindow *ui;

    DatabaseHandler dbHandler;

    class FinelogUser* registrationUser = nullptr;
};
#endif // MAINWINDOW_H

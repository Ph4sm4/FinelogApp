#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "stylesheetmanipulator.h"
#include "databasehandler.h"

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

private:
    Ui::MainWindow *ui;

    DatabaseHandler dbHandler;
    void setErrorBorder(class QLineEdit* e);
    void deleteErrorBorder(class QLineEdit* e);
};
#endif // MAINWINDOW_H

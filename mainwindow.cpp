#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fineloguser.h"
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->finelogLogo->setPixmap(QPixmap(":/logos/content/finelog-logo.png"));
    ui->finelogLogo2->setPixmap(QPixmap(":/logos/content/finelog-logo.png"));


//    foreach(QWidget* widget, QApplication::allWidgets()) {
//        const int pSize = widget->font().pointSize();
//        widget->setFont(QFont("Bahnschrift", pSize));
//        widget->update();
//    }
//!  label->setStyleSheet(StylesheetManipulator::updateStylesheetProperty(label->styleSheet(), "QLabel", "color", "green"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    const QString id = ui->idEdit->text();
    const QString pass = ui->passwordEdit->text();

    if(!id.length()) {
        setErrorBorder(ui->idEdit);
    }
    else {
        deleteErrorBorder(ui->idEdit);
    }
    if(!pass.length()){
        setErrorBorder( ui->passwordEdit);
    }
    else {
         deleteErrorBorder( ui->passwordEdit);
    }
    if(!id.length() || !pass.length()) return;

    qDebug() << dbHandler.getReponseJson("Pets").value("One");
    dbHandler.addNewUser(new FinelogUser("Olaf", "Dalach", "dalach.olaf@gmail.com"));
}

void MainWindow::setErrorBorder(QLineEdit *e)
{
    e->setStyleSheet(
        StylesheetManipulator::updateStylesheetProperty(
            e->styleSheet(), "#idEdit", "border", "2px solid red;"));
}

void MainWindow::deleteErrorBorder(QLineEdit *e)
{
    e->setStyleSheet(
        StylesheetManipulator::updateStylesheetProperty(
            e->styleSheet(), "#idEdit", "border", "2px solid rgb(37, 39, 48);"));
}


void MainWindow::on_loginGoTo_clicked()
{
    ui->pagination->setCurrentIndex(0);
}


void MainWindow::on_registerGoTo_clicked()
{
    ui->pagination->setCurrentIndex(1);
}


void MainWindow::on_idNext_clicked()
{
    ui->pagination->setCurrentIndex(2);
}


void MainWindow::on_nextName_clicked()
{
    ui->pagination->setCurrentIndex(3);

}


void MainWindow::on_nextEmail_clicked()
{
    ui->pagination->setCurrentIndex(4);

}


void MainWindow::on_loginGoTo_3_clicked()
{
    ui->pagination->setCurrentIndex(0);
}


void MainWindow::on_loginGoTo_4_clicked()
{
    ui->pagination->setCurrentIndex(0);
}


void MainWindow::on_loginGoTo_5_clicked()
{
    ui->pagination->setCurrentIndex(0);
}


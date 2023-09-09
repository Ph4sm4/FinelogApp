#include "selectcontentmanager.h"
#include "ui_selectcontentmanager.h"

SelectContentManager::SelectContentManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SelectContentManager)
{
    ui->setupUi(this);
}

SelectContentManager::~SelectContentManager()
{
    delete ui;
}

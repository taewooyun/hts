#include "depositdialog.h"
#include "ui_depositdialog.h"

DepositDialog::DepositDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DepositDialog)
{
    ui->setupUi(this);
}

DepositDialog::~DepositDialog()
{
    delete ui;
}

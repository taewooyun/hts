#include "senddialog.h"
#include "ui_senddialog.h"
#include <QMessageBox>

SendDialog::SendDialog(bankingdb *db, int userId, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::SendDialog),
    bankDB(db),
    m_userId(userId)
{
    ui->setupUi(this);
    loadAccounts();

    connect(ui->comboBox, &QComboBox::currentIndexChanged,
            this, &SendDialog::updateBalanceDisplay);

    connect(ui->pushButton, &QPushButton::clicked,
            this, &SendDialog::onSendClicked);
}

SendDialog::~SendDialog()
{
    delete ui;
}

void SendDialog::loadAccounts()
{
    ui->comboBox->clear();

    m_accList = bankDB->getUserAccounts(m_userId);

    for (auto &acc : m_accList)
        ui->comboBox->addItem(acc.accountNumber, acc.id);
}

void SendDialog::updateBalanceDisplay(int index)
{
    if (index < 0) return;

    int accountId = ui->comboBox->itemData(index).toInt();
    int balance = bankDB->getAccountBalance(accountId);

    ui->label_2->setText(QString("출금 가능 %1원").arg(balance));
}

void SendDialog::onSendClicked()
{
    int idx = ui->comboBox->currentIndex();
    if (idx < 0) {
        QMessageBox::warning(this, "오류", "내 계좌를 선택하세요.");
        return;
    }

    int fromId = ui->comboBox->itemData(idx).toInt();

    QString toAccount = ui->textEdit->toPlainText();
    int toId = bankDB->getAccountIdByNumber(toAccount);

    if (toId < 0) {
        QMessageBox::warning(this, "오류", "상대 계좌번호가 존재하지 않습니다.");
        return;
    }

    int amount = ui->textEdit_2->toPlainText().toInt();
    if (amount <= 0) {
        QMessageBox::warning(this, "오류", "송금 금액을 입력하세요.");
        return;
    }

    bool ok = bankDB->transfer(fromId, toId, amount, "");

    if (!ok) {
        QMessageBox::warning(this, "송금 실패", "잔액 부족 또는 처리 실패");
        return;
    }

    QMessageBox::information(this, "성공", "송금 완료!");
    accept();
}

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

    connect(ui->pushButton, &QPushButton::clicked,
            this, &SendDialog::onSendClicked);

    // 내 잔액 표시
    int balance = bankDB->getAccountBalance(m_userId);
    ui->label_2->setText(QString("출금 가능 %1원").arg(balance));
}

SendDialog::~SendDialog()
{
    delete ui;
}

void SendDialog::onSendClicked()
{
    // 출금 계좌 = 로그인한 사용자 계좌
    int fromId = m_userId;

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

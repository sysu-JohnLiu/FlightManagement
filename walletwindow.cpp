#include "walletwindow.h"
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QHBoxLayout>

WalletWindow::WalletWindow(QWidget *parent) : QWidget(parent), balance(0.0)
{
    // 设置窗口样式
    this->setStyleSheet(
        "QWidget { background-color: #f5f7fa; border-radius: 10px; border: 1px solid #e0e0e0; }");

    QVBoxLayout *mainLayout = new QVBoxLayout;

    // 钱包图标
    walletIconLabel = new QLabel(this);
    walletIconLabel->setFixedSize(120, 120);
    walletIconLabel->setStyleSheet(
        "border-radius: 60px; background-color: #e0e0e0; border: 3px solid #cccccc;");
    walletIconLabel->setAlignment(Qt::AlignCenter);
    walletIconLabel->setPixmap(QPixmap("C:/Users/23985/Desktop/wallet.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // 余额显示
    balanceLabel = new QLabel(QString("余额: %1 元").arg(balance, 0, 'f', 2), this);
    balanceLabel->setStyleSheet(
        "font-size: 24px; font-weight: bold; color: #333333;");
    balanceLabel->setAlignment(Qt::AlignCenter);

    // 充值和提现按钮
    rechargeButton = new QPushButton("充值", this);
    withdrawButton = new QPushButton("提现", this);

    // 按钮样式
    QString buttonStyle =
        "QPushButton {"
        "    background-color: #007BFF;"
        "    color: white;"
        "    border-radius: 8px;"
        "    padding: 8px 20px;"
        "    font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #0056b3;"
        "}";

    rechargeButton->setStyleSheet(buttonStyle);
    withdrawButton->setStyleSheet(buttonStyle);

    // 按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(rechargeButton);
    buttonLayout->addWidget(withdrawButton);

    // 组合布局
    mainLayout->addWidget(walletIconLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(balanceLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);

    // 信号槽连接
    connect(rechargeButton, &QPushButton::clicked, this, &WalletWindow::onRecharge);
    connect(withdrawButton, &QPushButton::clicked, this, &WalletWindow::onWithdraw);
}

WalletWindow::~WalletWindow() {}

void WalletWindow::onRecharge()
{
    bool ok;
    double amount = QInputDialog::getDouble(this, "充值", "请输入充值金额:", 0, 0, 10000, 2, &ok);
    if (ok && amount > 0) {
        balance += amount;
        balanceLabel->setText(QString("余额: %1 元").arg(balance, 0, 'f', 2));
    }
}

void WalletWindow::onWithdraw()
{
    bool ok;
    double amount = QInputDialog::getDouble(this, "提现", "请输入提现金额:", 0, 0, balance, 2, &ok);
    if (ok && amount > 0) {
        balance -= amount;
        balanceLabel->setText(QString("余额: %1 元").arg(balance, 0, 'f', 2));
    }
}

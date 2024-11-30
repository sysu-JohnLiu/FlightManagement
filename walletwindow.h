#ifndef WALLETWINDOW_H
#define WALLETWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class WalletWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WalletWindow(QWidget *parent = nullptr);
    ~WalletWindow();

private slots:
    void onRecharge(); // 充值
    void onWithdraw(); // 提现

private:
    QLabel *walletIconLabel; // 钱包图标
    QLabel *balanceLabel;    // 余额显示
    QPushButton *rechargeButton; // 充值按钮
    QPushButton *withdrawButton; // 提现按钮

    double balance; // 当前余额
};

#endif // WALLETWINDOW_H

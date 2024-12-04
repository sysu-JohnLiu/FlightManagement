#include "walletwindow.h"
#include <QVBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QLabel>
#include <QTextBrowser>
#include <QPushButton>

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
    walletIconLabel->setPixmap(QPixmap("D:/软件工程实训（初级）/航班票务管理系统/wallet.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));

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
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #7F7EFF, stop:1 #9E8CFE);"
        "    color: white;"
        "    border-radius: 20px;"
        "    font-size: 18px;"
        "    padding: 10px 20px;"
        "    min-width: 80px;"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #9E8CFE, stop:1 #BCA4FF);"
        "}"
        "QPushButton:pressed {"
        "    background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #6D6CFF, stop:1 #8E7AFE);"
        "}";

    rechargeButton->setStyleSheet(buttonStyle);
    withdrawButton->setStyleSheet(buttonStyle);

    // 按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(rechargeButton);
    buttonLayout->addWidget(withdrawButton);

    // 添加查看账单明细文字
    viewBillLabel = new QLabel("<a href='#'>点击查看账单明细 >></a>", this);
    viewBillLabel->setStyleSheet("font-size: 14px; color: #007BFF;");
    viewBillLabel->setAlignment(Qt::AlignCenter);
    viewBillLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);

    // 连接点击事件
    connect(viewBillLabel, &QLabel::linkActivated, this, &WalletWindow::onViewBillClicked);

    // 组合布局
    mainLayout->addWidget(walletIconLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(balanceLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(viewBillLabel, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    setLayout(mainLayout);

    // 初始化余额
    loadBalance();

    // 信号槽连接
    connect(rechargeButton, &QPushButton::clicked, this, &WalletWindow::onRecharge);
    connect(withdrawButton, &QPushButton::clicked, this, &WalletWindow::onWithdraw);
}

WalletWindow::~WalletWindow() {}

void WalletWindow::loadBalance()
{
    QSqlQuery query;
    query.prepare("SELECT balance FROM users WHERE username = :username");
    query.bindValue(":username", "user1");

    if (query.exec() && query.next()) {
        balance = query.value(0).toDouble();
        balanceLabel->setText(QString("余额: %1 元").arg(balance, 0, 'f', 2));
    } else {
        QMessageBox::warning(this, "加载失败", "无法加载用户余额：" + query.lastError().text());
    }
}

void WalletWindow::updateBalanceInDatabase()
{
    QSqlQuery query;
    query.prepare("UPDATE users SET balance = :balance WHERE username = :username");
    query.bindValue(":balance", balance);
    query.bindValue(":username", "user1");

    if (!query.exec()) {
        QMessageBox::warning(this, "更新失败", "无法更新余额：" + query.lastError().text());
    }
}

void WalletWindow::onRecharge()
{
    bool ok;
    double amount = QInputDialog::getDouble(this, "充值", "请输入充值金额:", 0, 0, 10000, 2, &ok);
    if (ok && amount > 0) {
        balance += amount;
        balanceLabel->setText(QString("余额: %1 元").arg(balance, 0, 'f', 2));
        updateBalanceInDatabase();
    }
}

void WalletWindow::onWithdraw()
{
    bool ok;
    double amount = QInputDialog::getDouble(this, "提现", "请输入提现金额:", 0, 0, balance, 2, &ok);
    if (ok && amount > 0) {
        balance -= amount;
        balanceLabel->setText(QString("余额: %1 元").arg(balance, 0, 'f', 2));
        updateBalanceInDatabase();
    }
}

void WalletWindow::onViewBillClicked()
{
    QSqlQuery query;
    query.prepare("SELECT transaction_type, amount, transaction_time, flight_number, departure_city, destination_city "
                  "FROM wallet_transactions WHERE username = :username ORDER BY transaction_time DESC");
    query.bindValue(":username", "user1");

    if (query.exec()) {
        QString billDetails;

        while (query.next()) {
            QString transactionType = query.value("transaction_type").toString();
            double amount = query.value("amount").toDouble();
            QString transactionTime = query.value("transaction_time").toString();
            QString flightNumber = query.value("flight_number").toString();
            QString departureCity = query.value("departure_city").toString();
            QString destinationCity = query.value("destination_city").toString();

            transactionTime.replace("T", " ");

            QString color;
            if (transactionType == "Recharge") {
                color = "green";
            } else if (transactionType == "Withdraw") {
                color = "blue";
            } else if (transactionType == "Buy") {
                color = "orange";
            } else if (transactionType == "Return") {
                color = "red";
            } else {
                color = "black";
            }

            billDetails.append(QString("<span style='color:%1;'>交易类型: %2</span><br>").arg(color, transactionType));
            billDetails.append(QString("金额: %1 元<br>").arg(amount, 0, 'f', 2));
            billDetails.append(QString("交易时间: %1<br>").arg(transactionTime));

            if (transactionType == "Buy" || transactionType == "Return") {
                billDetails.append(QString("航班号: %1<br>").arg(flightNumber));
                billDetails.append(QString("出发地: %1<br>").arg(departureCity));
                billDetails.append(QString("目的地: %1<br>").arg(destinationCity));
            }

            billDetails.append("<hr>");
        }

        if (billDetails.isEmpty()) {
            QMessageBox::information(this, "账单明细", "没有找到任何账单记录。");
        } else {
            QWidget *billWidget = new QWidget;
            billWidget->setWindowTitle("账单明细");
            billWidget->resize(600, 500);

            billWidget->setStyleSheet(
                "QWidget { background-color: #f8f9fa; border-radius: 10px; }"
                "QTextBrowser { border: none; font-family: 'Microsoft YaHei'; }");

            QVBoxLayout *layout = new QVBoxLayout(billWidget);
            layout->setContentsMargins(15, 15, 15, 15);

            QTextBrowser *billTextBrowser = new QTextBrowser(billWidget);
            billTextBrowser->setHtml(billDetails);
            billTextBrowser->setOpenLinks(false);

            QLabel *titleLabel = new QLabel("账单明细", billWidget);
            titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333333;");
            titleLabel->setAlignment(Qt::AlignCenter);

            QPushButton *closeButton = new QPushButton("关闭", billWidget);
            closeButton->setStyleSheet(buttonStyle);
            connect(closeButton, &QPushButton::clicked, billWidget, &QWidget::close);

            layout->addWidget(titleLabel, 0, Qt::AlignCenter);
            layout->addWidget(billTextBrowser);
            layout->addWidget(closeButton, 0, Qt::AlignCenter);

            billWidget->show();
        }
    } else {
        QMessageBox::warning(this, "加载失败", "无法加载账单明细：" + query.lastError().text());
    }
}

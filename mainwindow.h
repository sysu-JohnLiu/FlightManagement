#include "mainwindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 创建 stackedWidget，用于切换界面
    stackedWidget = new QStackedWidget(this);

    // 创建界面
    userInfoWidget = new UserInfo(this);     // 个人信息界面
    walletWidget = new WalletWindow(this);    // 钱包界面

    // 将界面添加到 stackedWidget
    stackedWidget->addWidget(userInfoWidget);
    stackedWidget->addWidget(walletWidget);

    // 创建按钮
    userInfoButton = new QPushButton("个人信息", this);
    walletButton = new QPushButton("钱包", this);

    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(userInfoButton);
    layout->addWidget(walletButton);

    // 右侧界面布局
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(stackedWidget);

    QWidget *rightWidget = new QWidget();
    rightWidget->setLayout(rightLayout);

    // 将左侧按钮和右侧界面放入主布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(layout, 1); // 左侧布局
    mainLayout->addWidget(rightWidget, 3); // 右侧布局

    centralWidget->setLayout(mainLayout);

    // 按钮点击事件绑定
    connect(userInfoButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(userInfoWidget); // 显示个人信息界面
    });

    connect(walletButton, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(walletWidget); // 显示钱包界面
    });

    // 默认显示个人信息界面
    stackedWidget->setCurrentWidget(userInfoWidget);
}

MainWindow::~MainWindow()
{
    delete stackedWidget;
    delete userInfoWidget;
    delete walletWidget;
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include "userinfo.h"     // 个人信息界面头文件
#include "walletwindow.h" // 钱包界面头文件

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QWidget *centralWidget;
    QStackedWidget *stackedWidget; // 用于切换界面的容器
    QPushButton *userInfoButton;   // 个人信息按钮
    QPushButton *walletButton;     // 钱包按钮
    UserInfo *userInfoWidget;      // 个人信息界面
    WalletWindow *walletWidget;    // 钱包界面
};

#endif // MAINWINDOW_H

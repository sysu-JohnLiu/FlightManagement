#ifndef USERINFO_H
#define USERINFO_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class UserInfo : public QWidget
{
    Q_OBJECT

public:
    explicit UserInfo(QWidget *parent = nullptr);
    ~UserInfo();

private slots:
    void onEditPhone();      // 修改手机号
    void onEditEmail();      // 修改邮箱
    void onEditNickname();   // 修改昵称
    void onEditName();       // 修改姓名
    void onEditGender();     // 修改性别
    void onEditBirthday();   // 修改生日
    void onEditAvatar();     // 修改头像
    void onEditPassword();   // 修改密码（新增）

private:
    QLabel *avatarLabel;       // 显示头像
    QLabel *phoneLabel;        // 显示手机号
    QLabel *emailLabel;        // 显示邮箱
    QLabel *nicknameLabel;     // 显示昵称
    QLabel *nameLabel;         // 显示姓名
    QLabel *genderLabel;       // 显示性别
    QLabel *birthdayLabel;     // 显示生日
    QLabel *passwordLabel;     // 显示密码（新增）

    QPushButton *avatarEditButton;   // 修改头像按钮
    QPushButton *phoneEditButton;    // 修改手机号按钮
    QPushButton *emailEditButton;    // 修改邮箱按钮
    QPushButton *nicknameEditButton; // 修改昵称按钮
    QPushButton *nameEditButton;     // 修改姓名按钮
    QPushButton *genderEditButton;   // 修改性别按钮
    QPushButton *birthdayEditButton; // 修改生日按钮
    QPushButton *passwordEditButton; // 修改密码按钮（新增）
};

#endif // USERINFO_H

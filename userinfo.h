#ifndef USERINFO_H
#define USERINFO_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class UserInfo : public QWidget {
    Q_OBJECT

public:
    explicit UserInfo(QWidget *parent = nullptr);
    ~UserInfo();

    void loadUserData(const QString &username); // 加载用户数据

private slots:
    void onEditAvatar();
    void onEditPhone();
    void onEditEmail();
    void onEditName();
    void onEditGender();
    void onEditBirthday();
    void onEditPassword();

private:
    void saveUserData(const QString &field, const QString &value); // 保存到数据库

    QLabel *avatarLabel;
    QLabel *phoneLabel;
    QLabel *emailLabel;
    QLabel *nicknameLabel;
    QLabel *nameLabel;
    QLabel *genderLabel;
    QLabel *birthdayLabel;
    QLabel *passwordLabel;
    QPushButton *avatarEditButton;
};

#endif // USERINFO_H

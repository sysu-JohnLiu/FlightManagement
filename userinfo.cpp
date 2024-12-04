#include "userinfo.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QInputDialog>
#include <QCalendarWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>

UserInfo::UserInfo(QWidget *parent) : QWidget(parent) {
    // 设置窗口样式：圆角背景
    this->setStyleSheet(
        "QWidget { background-color: #f5f7fa; border-radius: 10px; border: 1px solid #e0e0e0; }");

    QVBoxLayout *mainLayout = new QVBoxLayout;

    // 头像部分
    avatarLabel = new QLabel(this);
    avatarLabel->setFixedSize(120, 120);
    avatarLabel->setStyleSheet(
        "border-radius: 60px; border: 3px solid #cccccc; background-color: #e0e0e0;");
    avatarLabel->setAlignment(Qt::AlignCenter);
    avatarLabel->setPixmap(QPixmap(":/default-avatar.png").scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    avatarEditButton = new QPushButton("修改头像", this);
    avatarEditButton->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #7F7EFF, stop:1 #9E8CFE);"
        "color: white;"
        "border-radius: 20px;"
        "font-size: 18px;"
        "padding: 10px 20px;"
        "min-width: 80px;"
        "border: none;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #9E8CFE, stop:1 #BCA4FF);"
        "}"
        "QPushButton:pressed {"
        "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #6D6CFF, stop:1 #8E7AFE);"
        "}"
        );

    QVBoxLayout *avatarLayout = new QVBoxLayout;
    avatarLayout->addWidget(avatarLabel, 0, Qt::AlignCenter);
    avatarLayout->addWidget(avatarEditButton, 0, Qt::AlignCenter);

    // 信息部分
    QStringList infoTitles = {"手机", "邮箱", "姓名", "性别", "生日", "密码"};
    QList<QLabel *> infoLabels = {phoneLabel = new QLabel("未设置", this),
                                  emailLabel = new QLabel("未设置", this),
                                  nameLabel = new QLabel("未设置", this),
                                  genderLabel = new QLabel("未设置", this),
                                  birthdayLabel = new QLabel("未设置", this),
                                  passwordLabel = new QLabel("******", this)};

    QList<QPushButton *> editButtons;
    for (int i = 0; i < infoTitles.size(); ++i) {
        QPushButton *button = new QPushButton("✎", this);
        button->setFixedSize(25, 25);
        button->setStyleSheet(
            "QPushButton {"
            "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #7F7EFF, stop:1 #9E8CFE);"
            "color: white;"
            "border-radius: 20px;"
            "font-size: 18px;"
            "padding: 5px 10px;"
            "border: none;"
            "}"
            "QPushButton:hover {"
            "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #9E8CFE, stop:1 #BCA4FF);"
            "}"
            "QPushButton:pressed {"
            "background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 #6D6CFF, stop:1 #8E7AFE);"
            "}"
            );
        editButtons.append(button);
    }

    // 表单布局
    QVBoxLayout *formLayout = new QVBoxLayout;
    for (int i = 0; i < infoTitles.size(); ++i) {
        QHBoxLayout *rowLayout = new QHBoxLayout;
        QLabel *titleLabel = new QLabel(infoTitles[i] + ":", this);
        titleLabel->setStyleSheet("font-size: 16px; color: #333333; font-weight: 500;");
        infoLabels[i]->setStyleSheet("font-size: 16px; color: #555555;");
        rowLayout->addWidget(titleLabel);
        rowLayout->addWidget(infoLabels[i], 1);
        rowLayout->addWidget(editButtons[i], 0, Qt::AlignRight);
        formLayout->addLayout(rowLayout);
    }

    // 组合布局
    mainLayout->addLayout(avatarLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(formLayout);
    mainLayout->addStretch();
    setLayout(mainLayout);

    // 连接信号槽
    connect(avatarEditButton, &QPushButton::clicked, this, &UserInfo::onEditAvatar);
    connect(editButtons[0], &QPushButton::clicked, this, &UserInfo::onEditPhone);
    connect(editButtons[1], &QPushButton::clicked, this, &UserInfo::onEditEmail);
    connect(editButtons[2], &QPushButton::clicked, this, &UserInfo::onEditName);
    connect(editButtons[3], &QPushButton::clicked, this, &UserInfo::onEditGender);
    connect(editButtons[4], &QPushButton::clicked, this, &UserInfo::onEditBirthday);
    connect(editButtons[5], &QPushButton::clicked, this, &UserInfo::onEditPassword);

    // 加载用户数据（假设用户名为 'user1'）
    loadUserData("user1");
}

UserInfo::~UserInfo() {}

// 加载用户数据
void UserInfo::loadUserData(const QString &username) {
    QSqlQuery query;
    query.prepare("SELECT phone, email, username, gender, birthday, avatar, password FROM flightmanagementsystem.users WHERE username = :username");
    query.bindValue(":username", username);

    if (query.exec() && query.next()) {
        phoneLabel->setText(query.value("phone").toString());
        emailLabel->setText(query.value("email").toString());
        nameLabel->setText(query.value("username").toString());
        genderLabel->setText(query.value("gender").toString());
        birthdayLabel->setText(query.value("birthday").toDate().toString("yyyy-MM-dd"));
        QString avatarPath = query.value("avatar").toString();
        if (!avatarPath.isEmpty()) {
            avatarLabel->setPixmap(QPixmap(avatarPath).scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
        passwordLabel->setText("******");
    } else {
        QMessageBox::warning(this, "加载失败", "无法加载用户数据：" + query.lastError().text());
    }
}

// 保存用户数据
void UserInfo::saveUserData(const QString &field, const QString &value) {
    QSqlQuery query;
    query.prepare(QString("UPDATE flightmanagementsystem.users SET %1 = :value WHERE username = 'user1'").arg(field));
    query.bindValue(":value", value);

    if (!query.exec()) {
        QMessageBox::warning(this, "保存失败", "无法保存数据：" + query.lastError().text());
    }
}

// 修改头像
void UserInfo::onEditAvatar() {
    QString filePath = QFileDialog::getOpenFileName(this, "选择头像图片", "", "图片文件 (*.png *.jpg *.jpeg *.bmp);;所有文件 (*.*)");
    if (!filePath.isEmpty()) {
        QPixmap avatar(filePath);
        if (!avatar.isNull()) {
            avatarLabel->setPixmap(avatar.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            saveUserData("avatar", filePath);
        } else {
            QMessageBox::warning(this, "加载失败", "无法加载所选图片，请选择有效的图片文件。");
        }
    }
}

// 修改手机号
void UserInfo::onEditPhone() {
    QString phone = QInputDialog::getText(this, "修改手机", "请输入手机号码:");
    if (!phone.isEmpty()) {
        phoneLabel->setText(phone);
        saveUserData("phone", phone);
    }
}

// 修改邮箱
void UserInfo::onEditEmail() {
    QString email = QInputDialog::getText(this, "修改邮箱", "请输入邮箱地址:");
    if (!email.isEmpty()) {
        emailLabel->setText(email);
        saveUserData("email", email);
    }
}

// 修改姓名
void UserInfo::onEditName() {
    QString name = QInputDialog::getText(this, "修改姓名", "请输入姓名:");
    if (!name.isEmpty()) {
        nameLabel->setText(name);
        saveUserData("username", name);
    }
}

// 修改性别
void UserInfo::onEditGender() {
    QStringList genders = {"男", "女", "其他"};
    bool ok;
    QString gender = QInputDialog::getItem(this, "修改性别", "请选择性别:", genders, 0, false, &ok);
    if (ok) {
        genderLabel->setText(gender);
        saveUserData("gender", gender);
    }
}

// 修改生日
void UserInfo::onEditBirthday() {
    QCalendarWidget *calendar = new QCalendarWidget(this);
    calendar->setGridVisible(true);

    QDialog dialog(this);
    QVBoxLayout layout(&dialog);
    layout.addWidget(calendar);
    QPushButton *okButton = new QPushButton("确定", &dialog);
    layout.addWidget(okButton);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted) {
        QDate selectedDate = calendar->selectedDate();
        birthdayLabel->setText(selectedDate.toString("yyyy-MM-dd"));
        saveUserData("birthday", selectedDate.toString("yyyy-MM-dd"));
    }
}

// 修改密码
void UserInfo::onEditPassword() {
    QString password = QInputDialog::getText(this, "修改密码", "请输入新密码:", QLineEdit::Password);
    if (!password.isEmpty()) {
        passwordLabel->setText("******");
        saveUserData("password", password);
    }
}

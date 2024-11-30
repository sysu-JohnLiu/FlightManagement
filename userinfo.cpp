#include "userinfo.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QInputDialog>
#include <QCalendarWidget>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>

UserInfo::UserInfo(QWidget *parent) : QWidget(parent)
{
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
        "    background-color: #007BFF;"
        "    color: white;"
        "    border-radius: 8px;"
        "    padding: 5px 10px;"
        "    font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #0056b3;"
        "}");

    QVBoxLayout *avatarLayout = new QVBoxLayout;
    avatarLayout->addWidget(avatarLabel, 0, Qt::AlignCenter);
    avatarLayout->addWidget(avatarEditButton, 0, Qt::AlignCenter);

    // 信息部分
    QStringList infoTitles = {"手机", "邮箱", "昵称", "姓名", "性别", "生日", "密码"};
    QList<QLabel *> infoLabels = {phoneLabel = new QLabel("未设置", this),
                                  emailLabel = new QLabel("未设置", this),
                                  nicknameLabel = new QLabel("未设置", this),
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
            "    background-color: #f0f0f0;"
            "    border-radius: 12px;"
            "    color: #007BFF;"
            "    font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "    background-color: #e0e0e0;"
            "}");
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
        rowLayout->addWidget(infoLabels[i], 1); // 伸展填充剩余空间
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
    connect(editButtons[2], &QPushButton::clicked, this, &UserInfo::onEditNickname);
    connect(editButtons[3], &QPushButton::clicked, this, &UserInfo::onEditName);
    connect(editButtons[4], &QPushButton::clicked, this, &UserInfo::onEditGender);
    connect(editButtons[5], &QPushButton::clicked, this, &UserInfo::onEditBirthday);
    connect(editButtons[6], &QPushButton::clicked, this, &UserInfo::onEditPassword);
}

UserInfo::~UserInfo() {}

void UserInfo::onEditPhone() {
    QString phone = QInputDialog::getText(this, "修改手机", "请输入手机号码:");
    if (!phone.isEmpty()) {
        phoneLabel->setText(phone);
    }
}

void UserInfo::onEditEmail() {
    QString email = QInputDialog::getText(this, "修改邮箱", "请输入邮箱地址:");
    if (!email.isEmpty()) {
        emailLabel->setText(email);
    }
}

void UserInfo::onEditNickname() {
    QString nickname = QInputDialog::getText(this, "修改昵称", "请输入昵称:");
    if (!nickname.isEmpty()) {
        nicknameLabel->setText(nickname);
    }
}

void UserInfo::onEditName() {
    QString name = QInputDialog::getText(this, "修改姓名", "请输入姓名:");
    if (!name.isEmpty()) {
        nameLabel->setText(name);
    }
}

void UserInfo::onEditGender() {
    QString gender = QInputDialog::getItem(this, "修改性别", "选择性别:", {"男", "女"}, 0, false);
    if (!gender.isEmpty()) {
        genderLabel->setText(gender);
    }
}

void UserInfo::onEditBirthday() {
    QCalendarWidget *calendar = new QCalendarWidget(this);
    calendar->setSelectedDate(QDate::currentDate());
    QDialog dialog(this);
    QVBoxLayout *dialogLayout = new QVBoxLayout;
    dialogLayout->addWidget(calendar);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    dialogLayout->addWidget(buttonBox);
    dialog.setLayout(dialogLayout);

    connect(buttonBox, &QDialogButtonBox::accepted, [&]() {
        QDate selectedDate = calendar->selectedDate();
        birthdayLabel->setText(selectedDate.toString("yyyy-MM-dd"));
        dialog.accept();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    dialog.exec();
}

void UserInfo::onEditPassword() {
    bool ok1, ok2;
    QString password = QInputDialog::getText(this, "修改密码", "请输入新密码:", QLineEdit::Password, "", &ok1);
    if (!ok1 || password.isEmpty()) {
        return; // 用户取消或未输入密码
    }

    QString confirmPassword = QInputDialog::getText(this, "确认密码", "请再次输入新密码:", QLineEdit::Password, "", &ok2);
    if (!ok2 || confirmPassword.isEmpty()) {
        return; // 用户取消或未输入确认密码
    }

    if (password == confirmPassword) {
        passwordLabel->setText("******"); // 密码字段始终显示掩码
        QMessageBox::information(this, "成功", "密码已修改！");
    } else {
        QMessageBox::warning(this, "错误", "两次输入的密码不一致，请重新修改！");
    }
}


void UserInfo::onEditAvatar() {
    QString fileName = QFileDialog::getOpenFileName(this, "选择头像", "", "图片文件 (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty()) {
        avatarLabel->setPixmap(QPixmap(fileName).scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

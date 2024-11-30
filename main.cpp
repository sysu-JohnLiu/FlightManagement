#include "mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "FlightManagement_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    MainWindow w;
    w.setWindowTitle("航班管理系统");
    w.resize(800, 600);
    w.show();
    QSqlDatabase dbcon = QSqlDatabase::addDatabase("QODBC");
    dbcon.setHostName("127.0.0.1");
    dbcon.setPort(3306);
    dbcon.setDatabaseName("Local instance MySQL80");
    dbcon.setUserName("root");  // 添加用户名
    dbcon.setPassword("John1018");  // 添加密码

    bool ok = dbcon.open();
    if (!ok) {
        qDebug() << "Error, persondatabase 数据库文件打开失败！" << dbcon.lastError().text();
    } else {
        qDebug() << "Success, persondatabase 数据库文件打开成功！";
    }

    return a.exec();
}

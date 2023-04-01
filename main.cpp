#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QMessageBox>

QString ReadTXT(QString path){
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray array = file.readAll();
    file.close();
    return array;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString version = "1.7.0 install 版";
    if(QFile::exists("version.txt")){
        version = ReadTXT("version.txt");
    }
    else if (QFile::exists("/etc/version.txt")) {
        version = ReadTXT("/etc/version.txt");
    }
    else {
        QMessageBox::critical(NULL, "错误", "无法正确读取版本号");
        //return 1;
    }
    QCoreApplication::setApplicationName("dclc-information");
    QCoreApplication::setApplicationVersion(version);
    QCoreApplication::setOrganizationName("gfdgd xi");
    QCoreApplication::setOrganizationDomain("https://gitee.com/gfdgd-xi");
    MainWindow w;
    w.show();

    return a.exec();
}

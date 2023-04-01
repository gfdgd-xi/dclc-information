#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include <QDateTime>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkReply>
#include <QMessageBox>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString version = QCoreApplication::applicationVersion();
    QDateTime time = QDateTime::currentDateTime();
    ui->copyright->setText("<h3>©2022~" + time.toString("yyyy") + " gfdgd xi</h3>");
    ui->versionLabel->setText("<b>程序版本号：" + version + "</b>");
    this->setWindowTitle("关于 Deepin Community Live CD " + version);

}

QString MainWindow::GetHTTP(QString url){
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest req;
    req.setUrl(url);
    req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, 1);
    QNetworkReply *reply = manager->get(req);

    //req.setPriority();
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec(QEventLoop::ExcludeUserInputEvents);
    QVariant redirectAttr = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if(reply->error()
                     ||!redirectAttr.isNull()){
        qDebug() << "网络异常" << QString("发送get请求时出现错误：\n网址：%1\n错误信息：%2").arg(reply->request().url().toDisplayString(), reply->errorString());
    }
    QByteArray data = reply->readAll();
    reply->deleteLater();
    reply->close();
    delete reply;
    return data;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_2_clicked()
{
    QDesktopServices::openUrl(QUrl("https://github.com/gfdgd-xi/deepin-community-live-cd/"));
}

QString MainWindow::ReadTXT(QString path){
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray array = file.readAll();
    file.close();
    return array;
}

void MainWindow::on_isnewest_clicked()
{
    QString typeName = "install";
    if(QFile::exists("systemtype.txt")){
        typeName = ReadTXT("systemtype.txt").replace(" ", "").replace("\n", "");
    }
    else if (QFile::exists("/etc/systemtype.txt")) {
        typeName = ReadTXT("/etc/systemtype.txt").replace(" ", "").replace("\n", "");
    }
    QString version = QCoreApplication::applicationVersion();
    QString miniversion = version;
    if(version.contains(" ")){
        miniversion = "";
        for (int i = 0; i < version.indexOf(" "); i++) {
            miniversion += version[i];
        }

    }
    qDebug() << miniversion;
    QString result = GetHTTP("http://dclc.gfdgdxi.top/newversion-" + typeName + ".txt").replace("\n", "").replace(" ", "");
    if(result == ""){
        QMessageBox::critical(this, "错误", "无法连接服务器");
        return;
    }
    else if(result == miniversion){
        QMessageBox::information(this, "提示", "当前已是最新版本");
        return;
    }
    else {
        if(QMessageBox::question(this, "提示", "当前镜像不是最新版本，是否立即获取？\n检测到的最新版本：" + result + "\n当前版本：" + miniversion) == QMessageBox::Yes) {
            QDesktopServices::openUrl(QUrl("https://github.com/gfdgd-xi/deepin-community-live-cd/"));
        }
    }
}

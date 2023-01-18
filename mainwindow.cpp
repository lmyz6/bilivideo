#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
QString neturl;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Dialog *minwindow = new Dialog(this);//新建窗口对象

    ui->label->setStyleSheet("color:blue");


    connect(ui->pushButton,&QPushButton::clicked,this,[=]{
        netrun();

    });
    connect(ui->action1,&QAction::triggered,minwindow,[=]{
        minwindow->show();
    });
    connect(ui->lineEdit,&QLineEdit::textChanged,ui->pushButton_2,&QPushButton::hide);
    connect(ui->pushButton_2,&QPushButton::clicked,this,[=]{
         QNetworkAccessManager *dmanager = new QNetworkAccessManager(this);
         QNetworkReply *dreply = dmanager->get(QNetworkRequest(neturl));
         //下载视频
         QEventLoop loop;//循环请求数据
         connect(dreply,&QNetworkReply::finished,&loop,&QEventLoop::quit);

         connect(dreply,&QNetworkReply::readyRead,this,[=]{
             QString fileName = QFileDialog::getSaveFileName(nullptr, tr("保存文件"), "", tr("视频文件 (*.mp4)"));
             QFile file(fileName);
             if (file.open(QIODevice::WriteOnly)) {
                 file.write(dreply->readAll());
                 file.close();
              }
        });
    });
}
//函数编写
void MainWindow::netrun(){


    QNetworkAccessManager *manager = new QNetworkAccessManager(this);//创建网络访问成员
    QUrl url1;
    url1 = "https://xiaobapi.top/api/xb/api/tiktok_ks.php?url=";   //api链接

    QUrl url2(ui->lineEdit->text());//获取列表输入内容
    qDebug()<<url2;

    QNetworkReply *reply = manager->get(QNetworkRequest(url1.toString()+url2.toString()));//发送请求
    connect(reply,&QNetworkReply::finished,this,[=]{
        QByteArray data = reply->readAll();
        //处理数据
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);//转换成json
        qDebug()<<jsonDoc;

        QJsonObject jsonObject = jsonDoc.object();//转化成Object
        qDebug()<<jsonObject;

        QJsonValue message = jsonObject.value("message");//读值

        QJsonObject messageObject = message.toObject();//二次转化成Object

        QJsonValue netvideoname = messageObject.value("title");//进一步获取message中的值

        ui->videoname->setText(netvideoname.toString());

        QJsonValue netvideomaker = messageObject.value("user");

        ui->videomaker->setText(netvideomaker.toString());

        QJsonValue netvideomakerid = messageObject.value("userId");


        double temp = netvideomakerid.toDouble();//由于id是double类型，得进行处理
        ui->userid->setText(QString::number(temp,'f',0));//f是格式化浮点数，0表示输出小数点后0位，也就是不用输出小数点
        qDebug()<<temp;

         QJsonValue url3 = messageObject.value("url");
         neturl = url3.toString();
         qDebug()<<neturl;
        ui->pushButton_2->show();
    });

}


MainWindow::~MainWindow()
{
    delete ui;
}


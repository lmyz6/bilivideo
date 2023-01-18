#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setFixedSize(400,300);
    ui->label_5->setStyleSheet("color:red");
    connect(ui->pushButton,&QPushButton::clicked,this,[=]{
       close();
    });
}

Dialog::~Dialog()
{
    delete ui;
}

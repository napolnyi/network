#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket,&QTcpSocket::readyRead,this,&MainWindow::ReadyRead);
    connect(socket,&QTcpSocket::disconnected, socket,&QTcpSocket::deleteLater);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    socket->connectToHost("127.0.0.1",5005);

}

void MainWindow::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_14);
    out << str;
    socket->write(Data);
    ui->lineEdit->clear();
}

void MainWindow::ReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_14);
    if(in.status() == QDataStream::Ok)
    {
        QString str;
        in >> str;
        ui->textBrowser->append(str);
    }
    else
    {
        ui->textBrowser->append("read error..");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    SendToServer(ui->lineEdit->text());
}

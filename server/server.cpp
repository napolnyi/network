#include "server.h"
#include <iostream>

Server::Server()
{
    if(this->listen(QHostAddress::Any,5005))
    {
        qDebug() << "start";
    }
    else
    {
        qDebug() << "error";
    }

}

void Server::SendToClient(QString str)
{
    Data.clear();
    QDataStream out(&Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_14);
    out << str;

    for (int i = 0; i < Sockets.size(); i++)
    {
        Sockets[i]->write(Data);
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, &QTcpSocket::readyRead,this,&Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected,socket,&QTcpSocket::deleteLater);

    Sockets.push_back(socket);
    qDebug() << "client connected" << socketDescriptor;

}

void Server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion (QDataStream::Qt_5_14);

    if (in.status() == QDataStream::Ok)
    {
        qDebug() << "read....";
        QString str;
        in >> str;
        qDebug() << str;
        SendToClient(str);
    }
    else
    {
        qDebug() << "DataStream error";
    }

}

#include "localserver.h"
#include <QMessageBox>
#include <QLocalServer>
#include <QLocalSocket>
LocalServer::LocalServer(QObject *parent)
	: QObject(parent)
{



}

LocalServer::~LocalServer()
{}


void LocalServer::setServer()
{
    QLocalServer server;
    QObject::connect(&server, &QLocalServer::newConnection, [&]() {
        QLocalSocket* socket = server.nextPendingConnection();
        QObject::connect(socket, &QLocalSocket::readyRead, [&, socket]() {
            QByteArray data = socket->readAll();
            qDebug() << "Received:" << data;
            QMessageBox::information(nullptr, "SERVER OK", "HWND");
            emit getup();
            socket->close();
            socket->deleteLater();
            });
        });

    if (!server.listen("MyServer")) 
    {
        qDebug() << "Server could not start!";
        QMessageBox::information(nullptr, "not start", "HWND");
    }
    else {
        QMessageBox::information(nullptr, "started", "HWND");
        qDebug() << "Server started!";
    }
}
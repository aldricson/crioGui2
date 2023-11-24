#include "QtTcpClient.h"
#include <QDebug>

QtTcpClient::QtTcpClient(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)) {
    connect(socket, &QTcpSocket::connected, this, &QtTcpClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &QtTcpClient::onDataReceived);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void QtTcpClient::connectToServer(const QString &host, quint16 port) {
    socket->connectToHost(host, port);
}

void QtTcpClient::sendReadCurrentRequest(const QString &moduleAlias, unsigned int channelIndex) {
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        QString request = QString("readCurrent;%1;%2;").arg(moduleAlias).arg(channelIndex);
        m_lastRequest = "readCurrent";
        socket->write(request.toUtf8());
    }
}

void QtTcpClient::onConnected() {
    qDebug() << "Connected to server";
}

void QtTcpClient::onDataReceived() {
    QString response = QString::fromUtf8(socket->readAll());
    if (m_lastRequest == "readCurrent")
    {
        emit currentReadedSignal(response);
    }
    qDebug() << "Response from server:" << response;
}

void QtTcpClient::displayError(QAbstractSocket::SocketError socketError) {
    qDebug() << "Socket error:" << socketError;
}

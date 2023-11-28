#include "QtTcpClient.h"
#include <QDebug>

QtTcpClient::QtTcpClient(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)) {
    connect(socket, &QTcpSocket::connected, this, &QtTcpClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &QtTcpClient::onDataReceived);
    connect(socket, &QTcpSocket::errorOccurred, this, &QtTcpClient::displayError);

}

void QtTcpClient::connectToServer(const QString &host, quint16 port)
{
    socket->connectToHost(host, port);
}

void QtTcpClient::sendReadCurrentRequest(const QString &moduleAlias, unsigned int channelIndex)
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        QString request = QString("readCurrent;%1;%2;").arg(moduleAlias).arg(channelIndex);
        m_lastRequest = "readCurrent";
        socket->write(request.toUtf8());
    }
}

void QtTcpClient::sendReadVoltageRequest(const QString &moduleAlias, unsigned int channelIndex)
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        QString request = QString("readVoltage;%1;%2;").arg(moduleAlias).arg(channelIndex);
        m_lastRequest = "readVoltage";
        socket->write(request.toUtf8());
    }
}

void QtTcpClient::onConnected() {
    qDebug() << "Connected to server";
}

void QtTcpClient::onDataReceived()
{
    QString response = QString::fromUtf8(socket->readAll());
    if (response.contains("NACK"))
    {
        emit errorSignal(response);
        qDebug() << "Error from server:" << response;
        return;
    }
    else if (m_lastRequest == "readCurrent")
    {
        emit currentReadedSignal(response);
    }
    else if (m_lastRequest == "readVoltage")
    {
       emit voltageReadedSignal(response);
    }

    qDebug() << "Response from server:" << response;
}

void QtTcpClient::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Socket error:" << socketError;
}

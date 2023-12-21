#include "QTCPDebugClient.h"
#include <QDebug>

QTCPDebugClient::QTCPDebugClient(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)) {
    connect(socket, &QTcpSocket::connected, this, &QTCPDebugClient::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &QTCPDebugClient::onDataReceived);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &QTCPDebugClient::displayError);
}

void QTCPDebugClient::connectToDebugServer(const QString &host, quint16 port) {
    setHost(host);
    setPort(port);
    socket->connectToHost(m_host, m_port);
}

const QString &QTCPDebugClient::host() const {
    return m_host;
}

void QTCPDebugClient::setHost(const QString &newHost) {
    if (m_host == newHost)
        return;
    m_host = newHost;
    emit hostChanged();
}

quint16 QTCPDebugClient::port() const {
    return m_port;
}

void QTCPDebugClient::setPort(quint16 newPort) {
    if (m_port == newPort)
        return;
    m_port = newPort;
    emit portChanged();
}

void QTCPDebugClient::onConnected()
{
    qDebug() << "Connected to debug server";
}

void QTCPDebugClient::onDataReceived() {
    QString message = QString::fromUtf8(socket->readAll());
    emit debugMessageReceived(message);
}

void QTCPDebugClient::displayError(QAbstractSocket::SocketError socketError) {
    qDebug() << "Socket error:" << socketError;
}

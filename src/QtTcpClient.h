#ifndef QTTCPCLIENT_H
#define QTTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

class QtTcpClient : public QObject {
    Q_OBJECT
public:
    explicit QtTcpClient(QObject *parent = nullptr);
    void connectToServer(const QString &host, quint16 port);
    void sendReadCurrentRequest(const QString &moduleAlias, unsigned int channelIndex);

private:
    QTcpSocket *socket;

private slots:
    void onConnected();
    void onDataReceived();
    void displayError(QAbstractSocket::SocketError socketError);
};

#endif // QTTCPCLIENT_H

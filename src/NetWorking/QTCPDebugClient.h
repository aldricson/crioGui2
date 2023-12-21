#ifndef QTCPDEBUGCLIENT_H
#define QTCPDEBUGCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

class QTCPDebugClient : public QObject {
    Q_OBJECT
public:
    explicit QTCPDebugClient(QObject *parent = nullptr);
    void connectToDebugServer(const QString &host, quint16 port);

    const QString &host() const;
    void setHost(const QString &newHost);

    quint16 port() const;
    void setPort(quint16 newPort);

private:
    QTcpSocket *socket;
    QString m_host;
    quint16 m_port;

    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)

private slots:
    void onConnected();
    void onDataReceived();
    void displayError(QAbstractSocket::SocketError socketError);

signals:
    void debugMessageReceived(const QString &message);
    void hostChanged();
    void portChanged();
};

#endif // QTCPDEBUGCLIENT_H

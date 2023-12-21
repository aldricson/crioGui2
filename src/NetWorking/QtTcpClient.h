#ifndef QTTCPCLIENT_H
#define QTTCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

class QtTcpClient : public QObject {
    Q_OBJECT
public:
    explicit QtTcpClient           (QString clientName,QObject *parent = nullptr);
    void connectToServer           (const QString &host, quint16 port, const QString &message);
    void disconnectFromServer();
    bool checkConnect              (const QString &host, quint16 port);
    void sendReadCurrentRequest    (const QString &moduleAlias, unsigned int channelIndex);
    void sendReadVoltageRequest    (const QString &moduleAlias, unsigned int channelIndex);
    void sendStartModbusSimulation ();
    void sendStopSimulation        ();

    const QString &host() const;
    void setHost(const QString &newHost);

    quint16 port() const;
    void setPort(quint16 newPort);

private:
    QTcpSocket *socket;
    QString    m_lastRequest = "";
    QString    m_host        = "";
    quint16    m_port;

    Q_PROPERTY(QString host READ host WRITE setHost NOTIFY hostChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)

private slots:
    void onConnected();
    void onDataReceived();
    void displayError(QAbstractSocket::SocketError socketError);

signals:
    void currentReadedSignal     (const QString &response);
    void voltageReadedSignal     (const QString &response);
    void errorSignal             (const QString &response);
    void socketNotConnectedSignal(const QString &response);
    void simulationStartedSignal (const QString &response);
    void simulationStopedSignal  (const QString &response);

    void hostChanged();
    void portChanged();
};

#endif // QTTCPCLIENT_H
